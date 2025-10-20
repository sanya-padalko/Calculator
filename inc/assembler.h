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

    char* buf = NULL;

    char* ex_ptr = NULL;

    int ic = 0;
    int labels[10] = {};
};

#define Number (1 << 0)
#define Label  (1 << 1)
#define Reg    (1 << 2)
#define Mem    (1 << 3)

const operation_t operations[] = {
    {  .name = "PUSH"  ,  .hash =  88  ,  .code =  PUSH  ,  .args = 0 + Number  },
    {  .name = "POP"   ,  .hash = 823  ,  .code =   POP  ,  .args = 0           },
    {  .name = "TOP"   ,  .hash = 507  ,  .code =   TOP  ,  .args = 0           },
    {  .name = "IN"    ,  .hash = 319  ,  .code =    IN  ,  .args = 0           },
    {  .name = "OUT"   ,  .hash = 880  ,  .code =   OUT  ,  .args = 0           },

    {  .name = "ADD"   ,  .hash = 633  ,  .code =   ADD  ,  .args = 0           },
    {  .name = "SUB"   ,  .hash = 962  ,  .code =   SUB  ,  .args = 0           },
    {  .name = "MUL"   ,  .hash = 326  ,  .code =   MUL  ,  .args = 0           },
    {  .name = "DIV"   ,  .hash = 683  ,  .code =   DIV  ,  .args = 0           },
    {  .name = "SQRT"  ,  .hash = 410  ,  .code =  SQRT  ,  .args = 0           },
    {  .name = "POW"   ,  .hash = 446  ,  .code =   POW  ,  .args = 0           },

    {  .name = "JMP"   ,  .hash = 423  ,  .code =   JMP  ,  .args = 0 + Label   },
    {  .name = "JB"    ,  .hash = 996  ,  .code =    JB  ,  .args = 0 + Label   },
    {  .name = "JBE"   ,  .hash = 137  ,  .code =   JBE  ,  .args = 0 + Label   },
    {  .name = "JA"    ,  .hash =  59  ,  .code =    JA  ,  .args = 0 + Label   },
    {  .name = "JAE"   ,  .hash = 200  ,  .code =   JAE  ,  .args = 0 + Label   },
    {  .name = "JE"    ,  .hash = 807  ,  .code =    JE  ,  .args = 0 + Label   },
    {  .name = "JNE"   ,  .hash = 381  ,  .code =   JNE  ,  .args = 0 + Label   },
    {  .name = "CALL"  ,  .hash = 884  ,  .code =  CALL  ,  .args = 0 + Label   },
    {  .name = "RET"   ,  .hash = 651  ,  .code =   RET  ,  .args = 0           },

    {  .name = "PUSHM" ,  .hash = 165  ,  .code = PUSHM  ,  .args = 0 + Mem     },
    {  .name = "POPM"  ,  .hash = 364  ,  .code =  POPM  ,  .args = 0 + Mem     },
    {  .name = "DRAW"  ,  .hash = 118  ,  .code =  DRAW  ,  .args = 0           },

    {  .name = "PUSHR" ,  .hash = 170  ,  .code = PUSHR  ,  .args = 0 + Reg     },
    {  .name = "POPR"  ,  .hash = 529  ,  .code =  POPR  ,  .args = 0 + Reg     },

    {  .name = "HLT"   ,  .hash =   0  ,  .code =   HLT  ,  .args = 0           }
};

CodeError_t assembler(assembler_t* assem);

#endif