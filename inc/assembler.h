#ifndef __ASSEMBLER_H_
#define __ASSEMBLER_H_

#include <strings.h>

#include "stack.h"
#include "calculator.h"
#include "processor.h"

enum OperCode {
    PUSH_CODE = 1218,
    ADD_CODE  =  928,
    SUB_CODE  = 1136,
    MUL_CODE  = 1108,
    DIV_CODE  = 1008,
    SQRT_CODE = 1236,
    POW_CODE =  1130,
    OUT_CODE =  1140,
    HLT_CODE =  1048,
};

StackErr_t assembler(const char* text_file, const char* commands_file);

StackErr_t disassembler(const char* commands_file, const char* text_file);

#endif