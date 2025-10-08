#ifndef __EXECUTOR_H_
#define __EXECUTOR_H_

#include <sys/stat.h>

#include "stack.h"
#include "calculator.h"
#include "text_utils.h"
#include "vars.h"

const size_t RegsCount = 6;
const size_t ErrorRegs = 2;

struct processor_t {
    stack_t *stack = NULL;

    Text *code = NULL;

    int *bytecode = {};

    int ic = 0;

    int cmd_cnt = 0;

    StackElem_t regs[RegsCount] = {0};
};

#define make_processor(code_file) ProcCtor(code_file ON_DEBUG(, VarInfoCtor("processor", __FILE__, __FUNCTION__, __LINE__)))

#define procdump(name) ProcessorDump(name, VarInfoCtor(#name, __FILE__, __FUNCTION__, __LINE__))

const int err_regs_ind = 4;

processor_t* ProcCtor(const char* code_file ON_DEBUG(, VarInfo varinfo));

CodeError_t ProcDtor(processor_t* proc);

CodeError_t ProcVerify(processor_t* proc);

CodeError_t StackPushReg(processor_t* proc);

CodeError_t StackPopReg(processor_t* proc);

void ProcessorDump(processor_t* proc, VarInfo varinfo);

CodeError_t execution(const char* exec_file);

#endif