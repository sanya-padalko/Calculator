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
    
    int ram[100];
    StackElem_t regs[RegsCount] = {0};
};

enum ProcOper {
    VOID  =  0,
    PUSH  =  1,
    ADD   =  2,
    SUB   =  3,
    MUL   =  4,
    DIV   =  5,
    SQRT  =  6,
    POW   =  7,
    OUT   =  8,
    HLT   =  9,
    TOP   = 10,
    IN    = 11,
    PUSHR = 42, 
    POPR  = 43,
    JMP   = 14,
    JB    = 15,
    JBE   = 16,
    JA    = 17,
    JAE   = 18,
    JE    = 19,
    JNE   = 20,
    CALL  = 21,
    RET   = 22,
    PUSHM = 81,
    POPM  = 82,
};

#define make_processor(code_file) ProcCtor(code_file ON_DEBUG(, VarInfoCtor("processor", line_info)))

#define procdump(name) ProcDump(name, VarInfoCtor(#name, line_info))

const int err_regs_ind = 4;

processor_t* ProcCtor(const char* code_file ON_DEBUG(, VarInfo varinfo));

CodeError_t ProcDtor(processor_t* proc);

CodeError_t StackPushReg(processor_t* proc);

CodeError_t StackPopReg(processor_t* proc);

void ProcDump(processor_t* proc, VarInfo varinfo);

CodeError_t ParsingFile(processor_t *proc);

CodeError_t LoadFile(processor_t *proc);

CodeError_t execution(processor_t *proc);

#endif