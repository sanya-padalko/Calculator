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

    proc->bytecode = (int*)calloc(proc->code->file_size, sizeof(int));

    proc->cmd_cnt = 0;

    proc->ic = 0;

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

    if (proc->ic < 0 || proc->ic > proc->cmd_cnt) {
        proc->regs[4] = (StackElem_t)CMD_IND_ERR;
        return CMD_IND_ERR;
    }

    proc->regs[4] = (StackElem_t)NO_ERR;
    return NO_ERR;
}

ProcessorErr_t ProcPushReg(processor_t* proc) {
    ProcVerify(proc);

    int regs_ind = proc->bytecode[proc->ic++];

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

ProcessorErr_t ProcPopReg(processor_t* proc) {
    ProcVerify(proc);

    int regs_ind = proc->bytecode[proc->ic++];

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

ProcessorErr_t ProcJmp(processor_t* proc) {
    ProcVerify(proc);

    proc->ic = proc->bytecode[proc->ic];

    return NO_ERR;
}

ProcessorErr_t ProcJb(processor_t *proc) {
    ProcVerify(proc);

    if (proc->stack->size < 2) {
        proc->regs[RegsCount - ErrorRegs + 1] = SIZE_ERR;
        return STACK_ERR;
    }

    StackElem_t a = StackPop(proc->stack);
    StackElem_t b = StackPop(proc->stack);

    if (b < a)
        proc->ic = proc->bytecode[proc->ic];

    return NO_ERR;
}

void ProcessorDump(processor_t* proc, VarInfo varinfo) {
    printerr("\nProcDump called from %s: in function %s, line %d\n", varinfo.file_name, varinfo.func_name, varinfo.line_ind);
    printerr("%s ", varinfo.object_name);

    printerr("[");
    if (proc == NULL) {
        printerr(RED_COLOR "NULL" RESET_COLOR);
        printerr("] ");
        printerr(RED_COLOR "(!!!! BAD !!!!)\n" RESET_COLOR);
    }
    else {
        printerr(GREEN_COLOR "%x" RESET_COLOR, proc);
        printerr("]\n");
    }

    printerr("\t{\n");

    if (proc) {
        printerr("\tstack[");
        if (proc->stack == NULL) {
            printerr(RED_COLOR "NULL" RESET_COLOR);
            printerr("] ");
            printerr(RED_COLOR "(!!!! BAD !!!!)\n" RESET_COLOR);
        }
        else {
            printerr(GREEN_COLOR "%x" RESET_COLOR, proc->stack);
            printerr("] = { ");
            stackverify(proc->stack);
            if (code_error == NOTHING) {
                for (size_t ind = 0; ind < proc->stack->size; ++ind)
                    printerr(GREEN_COLOR "%d " RESET_COLOR, proc->stack->data[ind]);
            }
            else {
                printerr(RED_COLOR " Some problem with stack of processor " RESET_COLOR);
            }
            printerr("};\n");
        }

        printerr("\n\tcode[");
        if (proc->code == NULL) {
            printerr(RED_COLOR "NULL" RESET_COLOR);
            printerr("]; ");
            printerr(RED_COLOR "(!!!! BAD !!!!)\n" RESET_COLOR);
        }
        else {
            printerr(GREEN_COLOR "%x" RESET_COLOR, proc->stack);
            printerr("];\n");
        }

        printerr("\n\tCount of commands = %d;\n", proc->cmd_cnt);

        printerr("\n\tIndex of command = %d;\n", proc->ic);

        printerr("\n\tregister[");
        if (proc->regs == NULL) {
            printerr(RED_COLOR "NULL" RESET_COLOR);
            printerr("] ");
            printerr(RED_COLOR "(!!!! BAD !!!!)\n" RESET_COLOR);
        }
        else {
            printerr(GREEN_COLOR "%x" RESET_COLOR, proc->stack);
            printerr("] = { ");
            for (size_t ind = 0; ind < RegsCount - ErrorRegs; ++ind)
                printerr(GREEN_COLOR "%d " RESET_COLOR, proc->regs[ind]);

            int error_code = proc->regs[RegsCount - ErrorRegs];
            if (error_code < 0 || error_code >= PROC_ERR_CNT) {
                printerr(RED_COLOR "UNKNOWN_ERROR " RESET_COLOR);
            }
            else {
                if (error_code)
                    printerr(RED_COLOR " %s " RESET_COLOR, ProcErrorMas[error_code]);
                else   
                    printerr(GREEN_COLOR " %s " RESET_COLOR, ProcErrorMas[error_code]);
            }

            error_code = proc->regs[RegsCount - ErrorRegs + 1];
            if (error_code < 0 || error_code >= STACK_ERR_CNT) {
                printerr(RED_COLOR "UNKNOWN_ERROR " RESET_COLOR);
            }
            else {
                if (error_code)
                    printerr(RED_COLOR " %s " RESET_COLOR, StackErrorMas[error_code]);
                else   
                    printerr(GREEN_COLOR " %s " RESET_COLOR, StackErrorMas[error_code]);
            }

            printerr("};\n");
        }
    }

    printerr("\t}\n");
}

ProcessorErr_t separate_commands(processor_t *proc) {
    int cmd_ind = 0;

    
    
    for (size_t ind = 0; ind < proc->code->file_size; ) {
        char c = proc->code->buf[ind];
        if (c < '0' || c > '9') {
            ++ind;
            continue;
        }
        int x = 0;
        while ('0' <= c && c <= '9') {
            x = x * 10 + (int)((char)c - '0');
            ++ind;
            c = proc->code->buf[ind];
        }

        proc->bytecode[cmd_ind++] = x;
    }

    proc->cmd_cnt = cmd_ind;
}

ProcessorErr_t execution(const char* exec_file) {
    processor_t *proc = make_processor(exec_file);
    if (proc == NULL)
        return NULL_ERR;

    input_data(proc->code);
    separate_commands(proc);

    if (proc->regs[err_regs_ind] != NO_ERR)
        return (ProcessorErr_t)proc->regs[err_regs_ind];

    proc->regs[err_regs_ind + 1] = NOTHING;
    int operation = 0;

    for (proc->ic = 0; proc->ic < proc->cmd_cnt;) {
        operation = proc->bytecode[proc->ic++];
        StackElem_t value = 0;
        StackErr_t error_code = NOTHING;
        ProcessorErr_t proc_error = NO_ERR;

        switch(operation) {
            case PUSH:
                value = proc->bytecode[proc->ic++];

                error_code = StackPush(proc->stack, value);
                break;
            case PUSHR:
                proc_error = ProcPushReg(proc);
                break;
            case POPR:
                proc_error = ProcPopReg(proc);
                break;
            case JMP:
                proc_error = ProcJmp(proc);
                break;
            case JB:
                proc_error = ProcJb(proc);
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
                procdump(proc);
                return NO_ERR;
                break;
        }

        if (error_code != NOTHING) {
            proc->regs[err_regs_ind] = STACK_ERR;
            proc->regs[err_regs_ind + 1] = error_code;
            procdump(proc);
            return STACK_ERR;
        }
        
        if (proc_error != NO_ERR) {
            proc->regs[err_regs_ind] = proc_error;
            procdump(proc);
            return proc_error;
        }
    }

    return NO_ERR;
}
