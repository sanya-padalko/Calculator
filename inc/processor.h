#ifndef __EXECUTOR_H_
#define __EXECUTOR_H_

#include <sys/stat.h>

#include "stack.h"
#include "calculator.h"
#include "text_utils.h"
#include "vars.h"

const size_t RegsCount = 4;

struct processor_t {
    stack_t *stack = NULL;
    stack_t *stack_ret = NULL;

    Text *code = NULL;

    int *bytecode = {};

    int ic = 0;
    int cmd_cnt = 0;
    
    int ram[30000] = {};
    StackElem_t regs[RegsCount] = {0};
};

const int MaxOperationSize = 5;

struct operation_t {
    const char* name;

    int hash;

    int code;

    int args;
};

enum ProcOper {
    PUSH  =  1,
    POP   =  2,
    TOP   =  3,
    IN    =  4,
    OUT   =  5,

    ADD   = 10,
    SUB   = 11,
    MUL   = 12,
    DIV   = 13,
    SQRT  = 14,
    POW   = 15,

    JMP   = 20,
    JB    = 21,
    JBE   = 22,
    JA    = 23,
    JAE   = 24,
    JE    = 25,
    JNE   = 26,
    CALL  = 27,
    RET   = 28,
    
    PUSHM = 30,
    POPM  = 31,
    DRAW  = 32,

    PUSHR = 40, 
    POPR  = 41,
    
    HLT   = 50
};

#define make_processor(code_file) ProcCtor(code_file ON_DEBUG(, VarInfoCtor("processor", line_info)))

#define procdump(name) ProcDump(name, VarInfoCtor(#name, line_info))

const int err_regs_ind = 4;

processor_t* ProcCtor(const char* code_file ON_DEBUG(, VarInfo varinfo));
CodeError_t ProcDtor(processor_t* proc);
CodeError_t StackPushReg(processor_t* proc);
CodeError_t StackPopReg(processor_t* proc);
void ProcDump(processor_t* proc, VarInfo varinfo);

CodeError_t ProcStackPush(processor_t* proc);
CodeError_t ProcStackPop(processor_t* proc);
CodeError_t ProcCall(processor_t* proc);
CodeError_t ProcRet(processor_t* proc);
CodeError_t ProcPushReg(processor_t* proc);
CodeError_t ProcPopReg(processor_t* proc);
CodeError_t ProcPushRam(processor_t* proc);
CodeError_t ProcPopRam(processor_t* proc);
CodeError_t ProcJmp(processor_t* proc);
CodeError_t ProcDraw(processor_t* proc);

CodeError_t ParsingFile(processor_t *proc);
CodeError_t LoadFile(processor_t *proc);
CodeError_t execution(processor_t *proc);

#endif