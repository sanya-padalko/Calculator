#include "processor.h"

CodeError_t ProcVerify(processor_t* proc);

processor_t* ProcCtor(const char* code_file ON_DEBUG(, VarInfo varinfo)) {
    ON_DEBUG(PrintVarInfo(varinfo));
    my_assert(code_file, NULLPTR, NULL);

    processor_t* proc = (processor_t*)calloc(1, sizeof(processor_t));
    my_assert(proc, NULLPTR, NULL);

    proc->stack = make_stack(BaseStackSize);
    my_assert(proc->stack, NULLPTR, NULL);

    proc->code = TextCtor(code_file);
    my_assert(proc->code, NULLPTR, NULL);

    proc->bytecode = (int*)calloc(proc->code->file_size, sizeof(int));
    my_assert(proc->bytecode, NULLPTR, NULL);

    proc->cmd_cnt = 0;
    proc->ic = 0;
}

CodeError_t ProcDtor(processor_t* proc) {
    CodeError_t code_error = ProcVerify(proc);
    my_assert(code_error == NOTHING, code_error, code_error);

    free(proc->stack);
    proc->stack = NULL;
    free(proc->code);
    proc->code = NULL;
    free(proc);
    proc = NULL;

    return NOTHING;
}

CodeError_t ProcVerify(processor_t* proc) {
    my_assert(proc, NULLPTR, NULLPTR);

    stackverify(proc->stack);
    my_assert(code_error == NOTHING, code_error, code_error);
    
    my_assert(proc->stack, STACK_ERR, STACK_ERR);
    
    my_assert(proc->code, NULLPTR, NULLPTR);

    my_assert(0 <= proc->ic && proc->ic <= proc->cmd_cnt, CMD_IND_ERR, CMD_IND_ERR);

    return NOTHING;
}

CodeError_t ProcPushReg(processor_t* proc) {
    CodeError_t code_error = ProcVerify(proc);
    my_assert(code_error == NOTHING, code_error, code_error);

    int regs_ind = proc->bytecode[proc->ic++];
    my_assert(0 <= regs_ind && regs_ind <= 3, REG_IND_ERR, REG_IND_ERR);

    CodeError_t error_code = StackPush(proc->stack, proc->regs[regs_ind]);

    return NOTHING;
}

CodeError_t ProcPopReg(processor_t* proc) {
    CodeError_t code_error = ProcVerify(proc);
    my_assert(code_error == NOTHING, code_error, code_error);

    int regs_ind = proc->bytecode[proc->ic++];
    my_assert(0 <= regs_ind && regs_ind <= 3, REG_IND_ERR, REG_IND_ERR);

    StackElem_t last = StackPop(proc->stack);

    if (last == POIZON_VALUE) {
        printerr(RED_COLOR "Last number from stack cannot be written in a register\n" RESET_COLOR);
        return STACK_ERR;
    }

    proc->regs[regs_ind] = last;

    return NOTHING;
}

CodeError_t ProcJmp(processor_t* proc) {
    CodeError_t code_error = ProcVerify(proc);
    my_assert(code_error == NOTHING, code_error, code_error);

    proc->ic = proc->bytecode[proc->ic];

    return NOTHING;
}

#define procj(expr)     a = StackPop(proc->stack); \
                        b = StackPop(proc->stack); \
                        if (b expr a) \
                            proc->ic = proc->bytecode[proc->ic]; \
                        else \
                            ++proc->ic; \

void ProcDump(processor_t* proc, VarInfo varinfo) {
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
            for (size_t ind = 0; ind < RegsCount; ++ind)
                printerr(GREEN_COLOR "%d " RESET_COLOR, proc->regs[ind]);

            printerr("};\n");
        }
    }

    printerr("\t}\n");
}

CodeError_t ParsingFile(processor_t *proc) {
    CodeError_t code_error = ProcVerify(proc);
    my_assert(code_error == NOTHING, code_error, code_error);

    int cmd_ind = 0;
    char* start = proc->code->buf;
    
    for (; start < proc->code->buf + proc->code->file_size; ) {
        int value = 0;
        int n = 0;
        sscanf(start, "%d %n", &value, &n);
        start += n;
        proc->bytecode[cmd_ind++] = value;
    }

    proc->cmd_cnt = cmd_ind;

    return NOTHING;
}

CodeError_t LoadFile(processor_t *proc) {
    my_assert(proc, NULLPTR, NULLPTR);

    CodeError_t error_code = ReadFile(proc->code);
    my_assert(error_code == NOTHING, error_code, error_code);

    error_code = ParsingFile(proc);
    my_assert(error_code == NOTHING, error_code, error_code);

    return NOTHING;
}

CodeError_t execution(processor_t *proc) {
    my_assert(proc, NULLPTR, NULLPTR);

    int operation = 0, a = 0, b = 0;

    for (proc->ic = 0; proc->ic < proc->cmd_cnt;) {
        operation = proc->bytecode[proc->ic++];
        StackElem_t value = 0;
        CodeError_t error_code = NOTHING;

        switch(operation) {
            case PUSH:
                value = proc->bytecode[proc->ic++];

                error_code = StackPush(proc->stack, value);
                break;
            case PUSHR:
                error_code = ProcPushReg(proc);
                break;
            case POPR:
                error_code = ProcPopReg(proc);
                break;
            case JMP:
                error_code = ProcJmp(proc);
                break;
            case JB:
                procj(<);
                break;
            case JBE:
                procj(<=);
                break;
            case JA:
                procj(>);
                break;
            case JAE:
                procj(>=);
                break;
            case JE:
                procj(==);
                break;
            case JNE:
                procj(!=);
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
                return NOTHING;
                break;
        }
        
        if (error_code != NOTHING) {
            procdump(proc);
            return error_code;
        }
    }

    return NOTHING;
}

#undef procj
