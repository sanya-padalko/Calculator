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

    proc->cmd_cnt = proc->code->n_commands;

    proc->cmd_ind = 0;

    proc->regs[4] = NO_ERR;
    proc->regs[5] = NOTHING;
}

ProcessorErr_t ProcDtor(processor_t* proc) {
    if (ProcVerify(proc) != NO_ERR)
        return ProcVerify(proc);

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
    
    stackverify(proc->stack);

    if (proc->stack == NULL || code_error != NOTHING) {
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

ProcessorErr_t StackPushReg(processor_t* proc) {
    ProcVerify(proc);

    int regs_ind = 0;
    int scanf_check = sscanf(proc->code->mas_command[proc->cmd_ind] + 3, "%d", &regs_ind);

    if (scanf_check != 1) {
        printerr(RED_COLOR "Incorrect index of register input" RESET_COLOR);
        return INPUT_ERR;
    }

    if (regs_ind < 0 || regs_ind > 3) {
        printerr(RED_COLOR "Unavailable index of register\n" RESET_COLOR);
        return REG_IND_ERR;
    }

    StackErr_t error_code = StackPush(proc->stack, proc->regs[regs_ind]);

    if (error_code != NOTHING) {
        printerr(RED_COLOR "Pushing from regitster to stack went wrong\n", RESET_COLOR);
        return STACK_ERR;
    }

    return NO_ERR;
}

ProcessorErr_t StackPopReg(processor_t* proc) {
    ProcVerify(proc);

    int regs_ind = 0;
    int scanf_check = sscanf(proc->code->mas_command[proc->cmd_ind] + 3, "%d", &regs_ind);

    if (scanf_check != 1) {
        printerr(RED_COLOR "Incorrect index of register input" RESET_COLOR);
        return INPUT_ERR;
    }

    if (regs_ind < 0 || regs_ind > 3) {
        printerr(RED_COLOR "Unavailable index of register\n" RESET_COLOR);
        return REG_IND_ERR;
    }

    StackElem_t last = StackPop(proc->stack);

    if (last == POIZON_VALUE) {
        printerr(RED_COLOR "Last number from stack cannot be written in a register\n" RESET_COLOR);
        return STACK_ERR;
    }

    proc->regs[regs_ind] = last;

    return NO_ERR;
}

ProcessorErr_t execution(const char* exec_file) {
    processor_t *proc = make_processor(exec_file);

    read_parse(proc->code);

    if (proc == NULL)
        return NULL_ERR;

    if (proc->regs[err_regs_ind] != NO_ERR)
        return (ProcessorErr_t)proc->regs[err_regs_ind];

    proc->regs[err_regs_ind + 1] = NOTHING;
    StackOper operation = VOID;

    for (proc->cmd_ind = 0; proc->cmd_ind < proc->cmd_cnt; ++proc->cmd_ind) {
        sscanf(proc->code->mas_command[proc->cmd_ind], "%d", &operation);
        StackElem_t value = 0;
        StackErr_t error_code = NOTHING;
        ProcessorErr_t proc_error = NO_ERR;

        switch(operation) {
            case PUSH:
                sscanf(proc->code->mas_command[proc->cmd_ind] + 2, "%d", &value);

                error_code = StackPush(proc->stack, value);
                break;
            case PUSHR:
                proc_error = StackPushReg(proc);
                if (proc_error != NO_ERR)
                    return proc_error;

                break;
            case POPR:
                proc_error = StackPopReg(proc);
                if (proc_error != NO_ERR)
                    return proc_error;

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
            case TOP:
                error_code = StackTop(proc->stack);
                break;
            case IN:
                error_code = StackIn(proc->stack);
                break;
            case HLT:
                return NO_ERR;
                break;
        }

        if (error_code != NOTHING) {
            proc->regs[err_regs_ind] = STACK_ERR;
            proc->regs[err_regs_ind + 1] = error_code;
            return STACK_ERR;
        }

    }

    return NO_ERR;
}
