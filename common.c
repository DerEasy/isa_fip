//
// Created by easy on 19.06.24.
//

#include "common.h"
#include <stdio.h>
#include <stdarg.h>

void *xmalloc(size_t size) {
    void *ptr = malloc(size += !size);
    if (ptr)
        return ptr;
    error_and_exit("malloc failed to allocate block of size %zu.\n", size);
    unreachable();
}

void *xcalloc(size_t amount, size_t width) {
    void *ptr = calloc(amount += !amount, width += !width);
    if (ptr)
        return ptr;
    error_and_exit("calloc failed to allocate block of size %zu.\n", amount * width);
    unreachable();
}

void *xrealloc(void *ptr, size_t size) {
    void *ptr2 = realloc(ptr, size += !size);
    if (ptr2)
        return ptr2;
    error_and_exit("realloc failed to allocate block of size %zu.\n", size);
    unreachable();
}

void error_and_exit(const char *format, ...) {
    va_list args;
    va_start(args);
    vfprintf(stderr, format, args);
    exit(EXIT_FAILURE);
}
