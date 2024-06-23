//
// Created by easy on 18.06.24.
//

#include "common.h"

enum RegisterLabel : uint8_t {
    REGISTER_RFS, REGISTER_RIP, REGISTER_RSP, REGISTER_RBP,
    REGISTER_R0, REGISTER_R1, REGISTER_R2, REGISTER_R3,
    REGISTER_R4, REGISTER_R5, REGISTER_R6, REGISTER_R7,
    REGISTER_R8, REGISTER_R9, REGISTER_R10, REGISTER_R11,
    REGISTER_R12, REGISTER_R13, REGISTER_R14, REGISTER_R15,
    REGISTER_NULL
};

enum DataFlow : uint8_t {
    DATAFLOW_RR, DATAFLOW_RM, DATAFLOW_RI, DATAFLOW_MR,
    DATAFLOW_MI, DATAFLOW_R, DATAFLOW_M, DATAFLOW_I,
    DATAFLOW_NONE
};

enum SizePrefix : uint8_t {
    SIZE_R, SIZE_S, SIZE_Q, SIZE_B
};

struct alu {
    /* 4 special registers and 16 general purpose ones. */
    uint64_t registers[4 + 16];
    uint8_t *memory;
};

typedef uint8_t aligned_1_uint64[8];
struct IndirectAddress {
    enum SizePrefix size;
    enum RegisterLabel base;
    enum RegisterLabel index;
    uint8_t scale;
    aligned_1_uint64 offset;
};

struct InstructionRR {
    enum DataFlow nextDF;
    enum RegisterLabel reg0;
    enum RegisterLabel reg1;
    uint8_t reg0size;
    uint8_t reg1size;
};

struct InstructionRM {
    enum DataFlow nextDF;
    enum RegisterLabel reg0;
    uint8_t reg0size;
    struct IndirectAddress mem;
};

struct InstructionRI {
    enum DataFlow nextDF;
    enum RegisterLabel reg0;
    uint8_t reg0size;
    uint8_t immsize;
    uint64_t imm;
};

struct InstructionMR {
    enum DataFlow nextDF;
    enum RegisterLabel reg0;
    uint8_t reg0size;
    struct IndirectAddress mem;
};

struct InstructionMI {
    enum DataFlow nextDF;
    uint8_t immsize;
    struct IndirectAddress mem;
    uint64_t imm;
};

union Instruction {
    struct InstructionRR rr;
    struct InstructionRM rm;
    struct InstructionRI ri;
    struct InstructionMR mr;
    struct InstructionMI mi;
};


