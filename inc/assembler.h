#ifndef __ASSEMBLER_H_
#define __ASSEMBLER_H_

#include <strings.h>

#include "stack.h"
#include "calculator.h"
#include "processor.h"

#define check_scanf(real_cnt, needed_cnt, func_name)    if (real_cnt != needed_cnt) { \
                                                            printerr(RED_COLOR "%s must have 1 argument\n" RESET_COLOR, func_name); \
                                                            return VALUE_ERR; \
                                                        }


enum OperCode { // в структуру
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
    JMP_CODE   = 203,
    JB_CODE    = 516,
};

CodeError_t assembler(const char* text_file, const char* commands_file);

#endif