//
// Created by easy on 19.06.24.
//

#include "lexer.h"
#include "common.h"
#include "preprocessor.h"
#include <axhashmap.h>
#include <axchunk.h>
#include <stdio.h>
#include <ctype.h>
#include <pcre.h>


/* The token map is supposed to be constant. Don't modify its contents. */
static axhashmap *tokenMap;
static const enum LexerTokenType tokTypesReference[] = {ALL_LEXER_TOKEN_TYPES};
static const char *tokTypesStrings[] = {ALL_LEXER_TOKEN_TYPE_STRINGS};    /* debugging info */

static void initialiseTokenMap(void) {
    struct StringToType {
        char *name;
        enum LexerTokenType tokenType;
    };
    static const struct StringToType tokStringAndType[] = {
            {"r0", TOK_REGISTER},
            {"r1", TOK_REGISTER},
            {"r2", TOK_REGISTER},
            {"r3", TOK_REGISTER},
            {"r4", TOK_REGISTER},
            {"r5", TOK_REGISTER},
            {"r6", TOK_REGISTER},
            {"r7", TOK_REGISTER},
            {"r8", TOK_REGISTER},
            {"r9", TOK_REGISTER},
            {"r10", TOK_REGISTER},
            {"r11", TOK_REGISTER},
            {"r12", TOK_REGISTER},
            {"r13", TOK_REGISTER},
            {"r14", TOK_REGISTER},
            {"r15", TOK_REGISTER},
            {"s0", TOK_REGISTER},
            {"s1", TOK_REGISTER},
            {"s2", TOK_REGISTER},
            {"s3", TOK_REGISTER},
            {"s4", TOK_REGISTER},
            {"s5", TOK_REGISTER},
            {"s6", TOK_REGISTER},
            {"s7", TOK_REGISTER},
            {"s8", TOK_REGISTER},
            {"s9", TOK_REGISTER},
            {"s10", TOK_REGISTER},
            {"s11", TOK_REGISTER},
            {"s12", TOK_REGISTER},
            {"s13", TOK_REGISTER},
            {"s14", TOK_REGISTER},
            {"s15", TOK_REGISTER},
            {"q0", TOK_REGISTER},
            {"q1", TOK_REGISTER},
            {"q2", TOK_REGISTER},
            {"q3", TOK_REGISTER},
            {"q4", TOK_REGISTER},
            {"q5", TOK_REGISTER},
            {"q6", TOK_REGISTER},
            {"q7", TOK_REGISTER},
            {"q8", TOK_REGISTER},
            {"q9", TOK_REGISTER},
            {"q10", TOK_REGISTER},
            {"q11", TOK_REGISTER},
            {"q12", TOK_REGISTER},
            {"q13", TOK_REGISTER},
            {"q14", TOK_REGISTER},
            {"q15", TOK_REGISTER},
            {"b0", TOK_REGISTER},
            {"b1", TOK_REGISTER},
            {"b2", TOK_REGISTER},
            {"b3", TOK_REGISTER},
            {"b4", TOK_REGISTER},
            {"b5", TOK_REGISTER},
            {"b6", TOK_REGISTER},
            {"b7", TOK_REGISTER},
            {"b8", TOK_REGISTER},
            {"b9", TOK_REGISTER},
            {"b10", TOK_REGISTER},
            {"b11", TOK_REGISTER},
            {"b12", TOK_REGISTER},
            {"b13", TOK_REGISTER},
            {"b14", TOK_REGISTER},
            {"b15", TOK_REGISTER},
            {"rfs", TOK_REGISTER},
            {"rip", TOK_REGISTER},
            {"rsp", TOK_REGISTER},
            {"rbp", TOK_REGISTER},
            {"sfs", TOK_REGISTER},
            {"sip", TOK_REGISTER},
            {"ssp", TOK_REGISTER},
            {"sbp", TOK_REGISTER},
            {"qfs", TOK_REGISTER},
            {"qip", TOK_REGISTER},
            {"qsp", TOK_REGISTER},
            {"qbp", TOK_REGISTER},
            {"bfs", TOK_REGISTER},
            {"bip", TOK_REGISTER},
            {"bsp", TOK_REGISTER},
            {"bbp", TOK_REGISTER},
            {"add", TOK_INSTRUCTION},
            {"adc", TOK_INSTRUCTION},
            {"sub", TOK_INSTRUCTION},
            {"sbb", TOK_INSTRUCTION},
            {"mul", TOK_INSTRUCTION},
            {"div", TOK_INSTRUCTION},
            {"imul", TOK_INSTRUCTION},
            {"idiv", TOK_INSTRUCTION},
            {"neg", TOK_INSTRUCTION},
            {"lea", TOK_INSTRUCTION},
            {"shl", TOK_INSTRUCTION},
            {"shr", TOK_INSTRUCTION},
            {"sar", TOK_INSTRUCTION},
            {"rol", TOK_INSTRUCTION},
            {"ror", TOK_INSTRUCTION},
            {"and", TOK_INSTRUCTION},
            {"or", TOK_INSTRUCTION},
            {"xor", TOK_INSTRUCTION},
            {"not", TOK_INSTRUCTION},
            {"set", TOK_INSTRUCTION},
            {"nop", TOK_INSTRUCTION},
            {"cmp", TOK_INSTRUCTION},
            {"jmp", TOK_INSTRUCTION},
            {"mov", TOK_INSTRUCTION},
            {"movsx", TOK_INSTRUCTION},
            {"movzx", TOK_INSTRUCTION},
            {"push", TOK_INSTRUCTION},
            {"pop", TOK_INSTRUCTION},
            {"call", TOK_INSTRUCTION},
            {"ret", TOK_INSTRUCTION},
            {"hlt", TOK_INSTRUCTION},
            {"in", TOK_INSTRUCTION},
            {"out", TOK_INSTRUCTION},
            {"+z", TOK_PREDICATE},
            {"+e", TOK_PREDICATE},
            {"+g", TOK_PREDICATE},
            {"+l", TOK_PREDICATE},
            {"+a", TOK_PREDICATE},
            {"+b", TOK_PREDICATE},
            {"+s", TOK_PREDICATE},
            {"+o", TOK_PREDICATE},
            {"+c", TOK_PREDICATE},
            {"+ge", TOK_PREDICATE},
            {"+le", TOK_PREDICATE},
            {"+ae", TOK_PREDICATE},
            {"+be", TOK_PREDICATE},
            {"-z", TOK_PREDICATE},
            {"-e", TOK_PREDICATE},
            {"-g", TOK_PREDICATE},
            {"-l", TOK_PREDICATE},
            {"-a", TOK_PREDICATE},
            {"-b", TOK_PREDICATE},
            {"-s", TOK_PREDICATE},
            {"-o", TOK_PREDICATE},
            {"-c", TOK_PREDICATE},
            {"-ge", TOK_PREDICATE},
            {"-le", TOK_PREDICATE},
            {"-ae", TOK_PREDICATE},
            {"-be", TOK_PREDICATE},
            {"data", TOK_DATA_DECLARATION},
            {"section", TOK_SIZE_CLASS},
            {"quarter", TOK_SIZE_CLASS},
            {"byte", TOK_SIZE_CLASS},
            {"+", TOK_PLUS},
            {"-", TOK_MINUS},
            {"*", TOK_STAR},
            {",", TOK_COMMA},
            {"[", TOK_BRACKET_OPEN},
            {"]", TOK_BRACKET_CLOSE},
            {"{", TOK_BRACE_CLOSE},
            {"}", TOK_BRACE_CLOSE},
    };

    /* choose a size that will fit all tokens without resizing the map */
    tokenMap = axh_newSized(0, 300, AXH_LOADFACTOR);
    for (uint64_t i = 0; i < sizeof tokStringAndType / sizeof *tokStringAndType; ++i) {
        /* cast away constness to make IDE and compiler shut up */
        void *key = tokStringAndType[i].name;
        void *value = (void *) &tokTypesReference[tokStringAndType[i].tokenType];
        axh_map(tokenMap, key, value);
    }
}

/* Use pcre library to do a very simple regex match. Only length of match is returned, not offset.
 * Patterns are cached in a static hashmap as to avoid repeated pattern compilation and memory allocation. */
static size_t simple_regex(char *pattern, char *text) {
    static axhashmap *regexCache;
    if (!regexCache)
        regexCache = axh_new(0);
    struct real_pcre *p = axh_get(regexCache, pattern);
    if (!p) {
        const char *error;
        int erroffset;
        p = pcre_compile(pattern, 0, &error, &erroffset, NULL);
        axh_map(regexCache, pattern, p);
        return simple_regex(pattern, text);
    }
    int matches[3] = {0};
    pcre_exec(p, NULL, text, (int) strlen(text), 0, 0, matches, 3);
    return matches[1] - matches[0];
}

static size_t try_lex_decimal_number(char *line) {
    return simple_regex("^\\d+[sqb]?", line);
}

static size_t try_lex_hex_number(char *line) {
    return simple_regex("^0x[0-9abcdefABCDEF]+[sqb]?", line);
}

static size_t try_lex_binary_number(char *line) {
    return simple_regex("^0b[01]+[sqb]?", line);
}

static size_t try_lex_label_declaration(char *line) {
    return simple_regex("^\\.?[_a-zA-Z]\\w*[ \t]*:", line);
}

static size_t try_lex_label(char *line) {
    return simple_regex("^\\.?[_a-zA-Z]\\w*", line);
}

static size_t try_lex_string(char *line) {
    return simple_regex("^\"[^\"\\\\]*(\\\\.[^\"\\\\]*)*\"", line);
}

static enum LexerTokenType lex_dynamic_token(char *line, size_t *span) {
    if ((*span = try_lex_hex_number(line)))
        return TOK_HEX_NUMBER;
    if ((*span = try_lex_binary_number(line)))
        return TOK_BINARY_NUMBER;
    if ((*span = try_lex_decimal_number(line)))
        return TOK_DECIMAL_NUMBER;
    if ((*span = try_lex_string(line)))
        return TOK_STRING_LITERAL;
    if ((*span = try_lex_label_declaration(line)))
        return TOK_LABEL_DECLARATION;
    if ((*span = try_lex_label(line)))
        return TOK_LABEL;
    return TOK_ILLEGAL;
}

static void handle_illegal_token(char *line, size_t i, size_t *errorStart, size_t *errorEnd) {

}

static void free_lexertoken(void *ptr) {
    struct LexerToken *lt = ptr;
    free(lt->name);
    free(lt);
}

axvector *tokenise_line(char *line, size_t length) {
    axvector *lineTokens = axv_setDestructor(axv_new(), free_lexertoken);
    axchunk *chunk = axc_newSized(1, length + 1);
    size_t i = strspn(line, " \t");
    size_t errorStart = 0;
    size_t errorEnd = 0;

    while (i < length) {
        if (isspace(line[i])) {
            ++i;
            continue;
        }

        axc_clear(chunk);
        axc_write(chunk, 0, &line[i], length - i);
        axc_push(chunk, "");
        enum LexerTokenType *tokenType = NULL;
        struct LexerToken *lt = xmalloc(sizeof *lt);
        lt->columnNumber = (int64_t) i + 1;
        size_t span;

        while (axc_len(chunk) > 1 && !axh_tryGet(tokenMap, axc_data(chunk), &tokenType)) {
            axc_discard(chunk, 2);
            axc_push(chunk, "");
        }
        if (axc_len(chunk) > 1) {
            lt->tokenType = *tokenType;
            lt->name = axc_internalCopy(chunk);
            axv_push(lineTokens, lt);
            i += axc_len(chunk) - 1;
        } else if ((lt->tokenType = lex_dynamic_token(&line[i], &span)) != TOK_ILLEGAL){
            axc_write(chunk, 0, &line[i], span);
            axc_push(chunk, "");
            lt->name = axc_internalCopy(chunk);
            axv_push(lineTokens, lt);
            i += span;
        } else {
            free(lt);
            if (i - errorEnd > 1) {

            }
        }
    }
    axc_destroy(chunk);
    return lineTokens;
}

/* for debugging */
static bool print_lexertoken(void *ptr, void *arg) {
    bool *first = arg;
    if (!*first)
        putchar('\t');
    *first = false;
    struct LexerToken *lt = ptr;
    printf("['%s' %s | %li:%li]\n", lt->name, tokTypesStrings[lt->tokenType], lt->lineNumber, lt->columnNumber);
    return true;
}

static void *add_line_number(void *ptr, void *lineNumber) {
    struct LexerToken *lt = ptr;
    lt->lineNumber = *(int64_t *) lineNumber;
    return lt;
}

static void free_axvector(void *xs) {
    axv_destroy(xs);
}

/* The argument vector is consumed. The return value is a vector of tokenised lines. Each tokenised line is itself a
 * vector of tokens. Tokens are of type struct LexerToken. */
axvector *tokenise_sourcelines(axvector *xs) {
    if (!tokenMap)
        initialiseTokenMap();
    axvector *tokenisedLines = axv_setDestructor(axv_newSized(axv_len(xs)), free_axvector);
    while (axv_len(xs)) {
        struct SourceLine *sl = axv_pop(xs);
        axvector *lineTokens = tokenise_line(sl->line, sl->length);
        axv_map(lineTokens, add_line_number, &sl->lineNumber);
        DEBUG(axv_foreach(lineTokens, print_lexertoken, &(bool){true}));
        axv_push(tokenisedLines, lineTokens);
    }
    return tokenisedLines;
}

