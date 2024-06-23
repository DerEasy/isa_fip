//
// Created by easy on 18.06.24.
//

#ifndef ISA_FIP_COMMON_H
#define ISA_FIP_COMMON_H

#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include <stdlib.h>
#include <fastmod.h>

#ifdef NDEBUG
#define DEBUG(x)
#else
#define DEBUG(x) x
#endif

typedef __uint128_t uint128_t;
typedef __int128_t int128_t;

/* First appears in preprocessor. */


/* System malloc with exit on fail. */
void *xmalloc(size_t size);
/* System calloc with exit on fail. */
void *xcalloc(size_t amount, size_t width);
/* System realloc with exit on fail. */
void *xrealloc(void *ptr, size_t size);
/* Write formatted string to stderr and properly exit with failure code. */
void error_and_exit(const char *format, ...);

#endif //ISA_FIP_COMMON_H
