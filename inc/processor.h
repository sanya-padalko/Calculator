#ifndef __EXECUTOR_H_
#define __EXECUTOR_H_

#include <sys/stat.h>

#include "stack.h"
#include "calculator.h"
#include "text_utils.h"
#include "vars.h"

#define Number (1 << 0)
#define Label  (1 << 1)
#define Reg    (1 << 2)
#define Mem    (1 << 3)

#define make_processor(code_file) ProcCtor(code_file ON_DEBUG(, VarInfoCtor("processor", line_info)))

#define procdump(name) ProcDump(name, VarInfoCtor(#name, line_info))

const size_t RegsCount = 8;
const size_t RamSize = 40000;
const int MaxOperationSize = 5;

struct processor_t {
    stack_t *stack = NULL;
    stack_t *stack_ret = NULL;

    Text *code = NULL;

    int *bytecode = {};

    int ic = 0;
    int cmd_cnt = 0;
    
    int ram[RamSize] = {};
    StackElem_t regs[RegsCount] = {0};
};

struct operation_t {
    const char* name;

    int hash;

    int code;

    CodeError_t (*func) (processor_t*);

    int args;
};

enum ProcOper {
    PUSH  ,
    POP   ,
    TOP   ,
    IN    ,
    OUT   ,

    ADD   ,
    SUB   ,
    MUL   ,
    DIV   ,
    SQRT  ,
    POW   ,

    JMP   ,
    JB    ,
    JBE   ,
    JA    ,
    JAE   ,
    JE    ,
    JNE   ,
    CALL  ,
    RET   ,
    
    PUSHM ,
    POPM  ,
    DRAW  ,

    PUSHR , 
    POPR  ,
    
    HLT
};

processor_t* ProcCtor(const char* code_file ON_DEBUG(, VarInfo varinfo));
CodeError_t ProcDtor(processor_t* proc);
CodeError_t ProcVerify(processor_t* proc);
void ProcDump(processor_t* proc, VarInfo varinfo);

CodeError_t ProcStackPush(processor_t* proc);
CodeError_t ProcStackPop(processor_t* proc);
CodeError_t ProcStackWork(processor_t* proc);
CodeError_t ProcCall(processor_t* proc);
CodeError_t ProcRet(processor_t* proc);
CodeError_t ProcPushReg(processor_t* proc);
CodeError_t ProcPopReg(processor_t* proc);
CodeError_t ProcPushRam(processor_t* proc);
CodeError_t ProcPopRam(processor_t* proc);
CodeError_t ProcJmp(processor_t* proc);
CodeError_t ProcCmpJump(processor_t* proc);
CodeError_t ProcDraw(processor_t* proc);

CodeError_t ParsingFile(processor_t *proc);
CodeError_t LoadFile(processor_t *proc);
CodeError_t execution(processor_t *proc);

const operation_t operations[] = {
    {  .name = "PUSH"  ,  .hash =  88  ,  .code =  PUSH  ,  .func = ProcStackPush,  .args = 0 + Number  },
    {  .name = "POP"   ,  .hash = 823  ,  .code =   POP  ,  .func =  ProcStackPop,  .args = 0           },
    {  .name = "TOP"   ,  .hash = 507  ,  .code =   TOP  ,  .func = ProcStackWork,  .args = 0           },
    {  .name = "IN"    ,  .hash = 319  ,  .code =    IN  ,  .func = ProcStackWork,  .args = 0           },
    {  .name = "OUT"   ,  .hash = 880  ,  .code =   OUT  ,  .func = ProcStackWork,  .args = 0           },

    {  .name = "ADD"   ,  .hash = 633  ,  .code =   ADD  ,  .func = ProcStackWork,  .args = 0           },
    {  .name = "SUB"   ,  .hash = 962  ,  .code =   SUB  ,  .func = ProcStackWork,  .args = 0           },
    {  .name = "MUL"   ,  .hash = 326  ,  .code =   MUL  ,  .func = ProcStackWork,  .args = 0           },
    {  .name = "DIV"   ,  .hash = 683  ,  .code =   DIV  ,  .func = ProcStackWork,  .args = 0           },
    {  .name = "SQRT"  ,  .hash = 410  ,  .code =  SQRT  ,  .func = ProcStackWork,  .args = 0           },
    {  .name = "POW"   ,  .hash = 446  ,  .code =   POW  ,  .func = ProcStackWork,  .args = 0           },

    {  .name = "JMP"   ,  .hash = 423  ,  .code =   JMP  ,  .func =       ProcJmp,  .args = 0 + Label   },
    {  .name = "JB"    ,  .hash = 996  ,  .code =    JB  ,  .func =   ProcCmpJump,  .args = 0 + Label   },
    {  .name = "JBE"   ,  .hash = 137  ,  .code =   JBE  ,  .func =   ProcCmpJump,  .args = 0 + Label   },
    {  .name = "JA"    ,  .hash =  59  ,  .code =    JA  ,  .func =   ProcCmpJump,  .args = 0 + Label   },
    {  .name = "JAE"   ,  .hash = 200  ,  .code =   JAE  ,  .func =   ProcCmpJump,  .args = 0 + Label   },
    {  .name = "JE"    ,  .hash = 807  ,  .code =    JE  ,  .func =   ProcCmpJump,  .args = 0 + Label   },
    {  .name = "JNE"   ,  .hash = 381  ,  .code =   JNE  ,  .func =   ProcCmpJump,  .args = 0 + Label   },
    {  .name = "CALL"  ,  .hash = 884  ,  .code =  CALL  ,  .func =      ProcCall,  .args = 0 + Label   },
    {  .name = "RET"   ,  .hash = 651  ,  .code =   RET  ,  .func =       ProcRet,  .args = 0           },

    {  .name = "PUSHM" ,  .hash = 165  ,  .code = PUSHM  ,  .func =   ProcPushRam,  .args = 0 + Mem     },
    {  .name = "POPM"  ,  .hash = 364  ,  .code =  POPM  ,  .func =    ProcPopRam,  .args = 0 + Mem     },
    {  .name = "DRAW"  ,  .hash = 118  ,  .code =  DRAW  ,  .func =      ProcDraw,  .args = 0           },

    {  .name = "PUSHR" ,  .hash = 170  ,  .code = PUSHR  ,  .func =   ProcPushReg,  .args = 0 + Reg     },
    {  .name = "POPR"  ,  .hash = 529  ,  .code =  POPR  ,  .func =    ProcPopReg,  .args = 0 + Reg     },

    {  .name = "HLT"   ,  .hash =   0  ,  .code =   HLT  ,  .func = ProcStackPush,  .args = 0           }
};

#endif