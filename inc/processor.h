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

    int *bytecode;

    int ic = 0;

    int cmd_cnt = 0;

    StackElem_t regs[RegsCount] = {0};
};

#ifdef DEBUG
#define make_processor(code_file) ProcCtor(code_file, VarInfoCtor("processor", __FILE__, __FUNCTION__, __LINE__))
#else
#define make_processor(code_file) ProcCtor(code_file)
#endif

#define procdump(name) ProcessorDump(name, VarInfoCtor(#name, __FILE__, __FUNCTION__, __LINE__))

const int err_regs_ind = 4;

enum ProcessorErr_t {
    NO_ERR      = 0,
    STACK_ERR   = 1,
    CODE_ERR    = 2,
    NULL_ERR    = 3,
    CMD_IND_ERR = 4,
    INPUT_ERR   = 5,
    REG_IND_ERR = 6,
    PROC_ERR_CNT
};

const char* const ProcErrorMas[] = {"NO_ERR", 
                            "STACK_ERR", 
                            "CODE_ERR", 
                            "NULL_ERR", 
                            "CMD_IND_ERR", 
                            "INPUT_ERR", 
                            "REG_IND_ERR"};

processor_t* ProcCtor(const char* code_file ON_DEBUG(, VarInfo varinfo));

ProcessorErr_t ProcDtor(processor_t* proc);

ProcessorErr_t ProcVerify(processor_t* proc);

ProcessorErr_t StackPushReg(processor_t* proc);

ProcessorErr_t StackPopReg(processor_t* proc);

void ProcessorDump(processor_t* proc, VarInfo varinfo);

ProcessorErr_t execution(const char* exec_file);

#endif