#include "processor.h"

processor_t* ProcCtor(const char* code_file ON_DEBUG(, VarInfo varinfo)) {
    ON_DEBUG(PrintVarInfo(varinfo));

    processor_t* proc = (processor_t*)calloc(1, sizeof(processor_t));

    if (proc == NULL) {
        printerr(RED_COLOR "Calloc return NULL in ProcCtor\n" RESET_COLOR);
        return NULL;
    }

    proc->stack = make_stack(BaseStackSize);

    proc->code = make_text(code_file);

    proc->cmd_ind = 0;

    proc->regs[4] = NO_ERR;
    proc->regs[5] = NOTHING;
}

ProcessorErr_t ProcDtor(processor_t* proc) {
    free(proc->stack);
    proc->stack = NULL;
    free(proc->code);
    proc->code = NULL;
    free(proc);
    proc = NULL;

    return NO_ERR;
}

ProcessorErr_t ProcVerify(processor_t* proc) {
    if (proc == NULL)
        return NULL_ERR;
    
    if (proc->stack == NULL) {
        proc->regs[4] = (StackElem_t)STACK_ERR;
        return STACK_ERR;
    }

    if (proc->code == NULL) {
        proc->regs[4] = (StackElem_t)CODE_ERR;
        return CODE_ERR;
    }

    if (proc->cmd_ind < 0 || proc->cmd_ind > proc->code->n_commands) {
        proc->regs[4] = (StackElem_t)CMD_IND_ERR;
        return CMD_IND_ERR;
    }

    proc->regs[4] = (StackElem_t)NO_ERR;
    return NO_ERR;
}

ProcessorErr_t execution(const char* exec_file) {
    Text code = {};
    code.name = exec_file;

    read_parse(&code);

    processor_t *proc = make_processor(exec_file);
    if (proc == NULL)
        return NULL_ERR;

    if (proc->regs[4] != NO_ERR)
        return (ProcessorErr_t)proc->regs[4];

    proc->regs[5] = NOTHING;
    StackOper operation = VOID;

    for (size_t command_ind = 0; command_ind < code.n_commands; ++command_ind) {
        sscanf(code.mas_command[command_ind], "%d", &operation);
        StackElem_t value = 0;
        StackErr_t error_code = NOTHING;
        char* buf = code.mas_command[command_ind];
        
        switch(operation) {
            case PUSH:
                while (*buf++ != ' ');
                sscanf(buf, "%d", &value);

                error_code = StackPush(proc->stack, value);
                break;
            case ADD:
                error_code = StackAdd(proc->stack);
                break;
            case SUB:
                error_code = StackSub(proc->stack);
                break;
            case MUL:
                error_code = StackMul(proc->stack);
                break;
            case DIV:
                error_code = StackDiv(proc->stack);
                break;
            case SQRT:
                error_code = StackSqrt(proc->stack);
                break;
            case POW:
                error_code = StackPow(proc->stack);
                break;
            case OUT:
                error_code = StackOut(proc->stack);
                break;
            case HLT:
                return NO_ERR;
                break;
        }

        if (error_code != NOTHING) {
            proc->regs[4] = STACK_ERR;
            proc->regs[5] = error_code;
            return STACK_ERR;
        }

    }

    return NO_ERR;
}
