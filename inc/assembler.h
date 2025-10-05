#ifndef __ASSEMBLER_H_
#define __ASSEMBLER_H_

#include <strings.h>

#include "stack.h"
#include "calculator.h"
#include "processor.h"

enum OperCode { 
    PUSH_CODE  = 588,
    ADD_CODE   = 353,
    SUB_CODE   = 462,
    MUL_CODE   = 786,
    DIV_CODE   =  63,
    SQRT_CODE  = 470,
    POW_CODE   = 146,
    OUT_CODE   = 260,
    TOP_CODE   = 207,
    IN_CODE    = 319,
    PUSHR_CODE = 670,
    POPR_CODE  =  29,
    HLT_CODE   = 560,
};

StackErr_t assembler(const char* text_file, const char* commands_file);

StackErr_t disassembler(const char* commands_file, const char* text_file);

#endif