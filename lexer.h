//
// Created by easy on 19.06.24.
//

#ifndef ISA_FIP_LEXER_H
#define ISA_FIP_LEXER_H

#include <axvector.h>

#define GENERATE_ENUM(ENUM) ENUM,
#define GENERATE_STRING(STRING) #STRING,
#define FOREACH_LEXER_TOKEN_TYPE(ACTION) \
ACTION(TOK_REGISTER)                     \
ACTION(TOK_INSTRUCTION)                  \
ACTION(TOK_DATA_DECLARATION)             \
ACTION(TOK_SIZE_CLASS)                   \
ACTION(TOK_LABEL)                        \
ACTION(TOK_LABEL_DECLARATION)            \
ACTION(TOK_STRING_LITERAL)               \
ACTION(TOK_DECIMAL_NUMBER)               \
ACTION(TOK_BINARY_NUMBER)                \
ACTION(TOK_HEX_NUMBER)                   \
ACTION(TOK_PLUS)                         \
ACTION(TOK_MINUS)                        \
ACTION(TOK_STAR)                         \
ACTION(TOK_PREDICATE)                    \
ACTION(TOK_COMMA)                        \
ACTION(TOK_BRACKET_OPEN)                 \
ACTION(TOK_BRACKET_CLOSE)                \
ACTION(TOK_BRACE_OPEN)                   \
ACTION(TOK_BRACE_CLOSE)                  \
ACTION(TOK_ILLEGAL)

#define ALL_LEXER_TOKEN_TYPES FOREACH_LEXER_TOKEN_TYPE(GENERATE_ENUM)
#define ALL_LEXER_TOKEN_TYPE_STRINGS FOREACH_LEXER_TOKEN_TYPE(GENERATE_STRING)

/* TOK_LABEL and TOK_*_NUMBER cannot be determined statically. */
enum LexerTokenType : uint8_t {
    ALL_LEXER_TOKEN_TYPES
};

struct LexerToken {
    char *name;
    int64_t lineNumber;
    int64_t columnNumber;
    enum LexerTokenType tokenType;
};

axvector *tokenise_sourcelines(axvector *xs);

#endif //ISA_FIP_LEXER_H
