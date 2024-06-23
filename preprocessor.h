//
// Created by easy on 19.06.24.
//

#ifndef ISA_FIP_PREPROCESSOR_H
#define ISA_FIP_PREPROCESSOR_H

#include <axvector.h>

struct SourceLine {
    char *line;
    size_t length;
    int64_t lineNumber;
};

bool is_string_ascii(const char *str);

axvector *str_splitlines(const char *str);

axvector *rstrip_with_comments(axvector *xs);

#endif //ISA_FIP_PREPROCESSOR_H
