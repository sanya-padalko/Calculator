#ifndef __ASSEMBLER_H_
#define __ASSEMBLER_H_

#include <strings.h>

#include "stack.h"
#include "calculator.h"
#include "processor.h"

struct assembler_t {
    const char* text_file = NULL;
    const char* commands_file = NULL;

    Text* program = NULL;

    char* ex_ptr = NULL;

    int ic = 0;
    int labels[10] = {};
};

enum OperCode {
    PUSH_CODE  =  88,
    POP_CODE   = 823,
    ADD_CODE   = 633,
    SUB_CODE   = 962,
    MUL_CODE   = 326,
    DIV_CODE   = 683,
    SQRT_CODE  = 410,
    POW_CODE   = 446,
    OUT_CODE   = 880,
    TOP_CODE   = 507,
    IN_CODE    = 319,
    PUSHR_CODE = 170,
    POPR_CODE  = 529,
    JMP_CODE   = 423,
    JB_CODE    = 996,
    JBE_CODE   = 137,
    JA_CODE    =  59,
    JAE_CODE   = 200,
    JE_CODE    = 807,
    JNE_CODE   = 381,
    HLT_CODE   =   0
};

CodeError_t assembler(assembler_t* assem);

#endif