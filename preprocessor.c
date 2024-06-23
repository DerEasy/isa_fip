//
// Created by easy on 19.06.24.
//

#include "preprocessor.h"
#include "common.h"
#include <ctype.h>

bool is_string_ascii(const char *str) {
    while (*str) {
        if (*str++ < 0)
            return false;
    }
    return true;
}

static void free_sourceline(void *ptr__) {
    struct SourceLine *ptr = ptr__;
    free(ptr->line);
    free(ptr);
}

axvector *str_splitlines(const char *str) {
    axvector *xs = axv_setDestructor(axv_new(), free_sourceline);
    int64_t lineNumber = 1;
    while (*str) {
        size_t lineLength = strcspn(str, "\n");
        char *line = xmalloc(lineLength + 1);
        memcpy(line, str, lineLength);
        line[lineLength] = '\0';
        struct SourceLine *sl = xmalloc(sizeof *sl);
        *sl = (struct SourceLine) {line, lineLength, lineNumber++};
        axv_push(xs, sl);
        str += lineLength;
        str += *str == '\n';
    }
    return xs;
}

static bool keep_nonblank_line(const void *ptr, void *_) {
    (void) _;
    const struct SourceLine *sl = ptr;
    size_t i;
    for (i = 0; i < sl->length; ++i) {
        if (!isspace(sl->line[i]))
            return sl->line[i] != ';';   /* make sure this isn't just a blank line with a comment */
    }
    return false;
}

/* Precondition: No source line is blank. */
static void *remove_comment_and_rstrip(void *ptr, void *_) {
    (void) _;
    struct SourceLine *sl = ptr;
    char *rightedge = strchr(sl->line, ';');    /* choose either the start of the comment... */
    if (!rightedge)
        rightedge = sl->line + sl->length;      /* ... or the end of the line as a starting point. */
    do
        --rightedge;
    while (isspace(*rightedge));
    size_t newLength = rightedge - sl->line + 1;
    newLength += !newLength;
    char *resizedLine = xrealloc(sl->line, newLength + 1);
    resizedLine[newLength] = '\0';
    sl->line = resizedLine;
    sl->length = newLength;
    return sl;
}

axvector *rstrip_with_comments(axvector *xs) {
    axv_filter(xs, keep_nonblank_line, NULL);
    axv_map(xs, remove_comment_and_rstrip, NULL);
    return xs;
}

