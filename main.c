#include <stdio.h>
#include "common.h"
#include "preprocessor.h"
#include "lexer.h"
#include <axhashmap.h>

int main(void) {
    axv_memoryfn(xmalloc, xrealloc, free);
    axh_memoryfn(xmalloc, xcalloc, free);
    char *input = "sample_array:\n"
                  "        data section {123, -3, 0b1011q, -0x5f}\n"
                  "\n"
                  "map:\n"
                  "        ; r2 function, r3 array, r4 length (unsigned)\n"
                  "        cmp     r4, 0           ; length > 0\n"
                  "        ret     +z\n"
                  "        push    r10             ; save registers\n"
                  "        push    r11\n"
                  "        push    r12\n"
                  "        mov     r10, r2\n"
                  "        mov     r11, r3\n"
                  "        mov     r12, r4\n"
                  "        xor     r13, r13        ; i = 0\n"
                  ".L0:\n"
                  "        mov     r2, [r11+r13*8]         ; x = array[i]\n"
                  "        call    r10                     ; f(x)\n"
                  "        mov     [r11+r13*8], r0         ; array[i] = f(x)\n"
                  "        add     r13, 1                  ; ++i\n"
                  "        cmp     r13, r12                ; i < length\n"
                  "        jmp     .L0, +b\n"
                  "        pop     r12                     ; restore registers\n"
                  "        pop     r11\n"
                  "        pop     r10\n"
                  "        ret\n"
                  "\n"
                  "S0:\n"
                  "        data \"hello world!\"\n"
                  "\n"
                  "S0_:\n"
                  "        lea     r0, [S0_-S0]\n"
                  "        ret\n"
                  "\n"
                  "main:\n"
                  "        call    S0_\n"
                  "        xor     r1, r1\n"
                  ".L0:\n"
                  "        cmp     r1, r0\n"
                  "        jmp     .L1, +ge\n"
                  "        mov     b2, [S0+r1]\n"
                  "        out     b2, 0\n"
                  "        add     r1, 1\n"
                  "        jmp     .L0\n"
                  ".L1:\n"
                  "        xor     r0, r0\n"
                  "        ret";

    if (!is_string_ascii(input))
        error_and_exit("Input must consist of ASCII-only characters.");
    axvector *splitLines = str_splitlines(input);
    splitLines = rstrip_with_comments(splitLines);
    /* Work on this copy without the destructor. Then destroy both the vector and its context to release all memory.
     * The lines are stored in reverse order to easily pop them off the vector. */
    splitLines = axv_setContext(axv_copy(axv_reverse(splitLines)), splitLines);
    axvector *tokenisedLines = tokenise_sourcelines(splitLines);
    axv_destroy(axv_destroy(splitLines));
    axv_destroy(tokenisedLines);
}
