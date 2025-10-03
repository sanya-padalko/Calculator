#include "executor.h"

StackErr_t execution(const char* exec_file) {
    Text code = {};
    code.name = exec_file;

    read_parse(&code);

    make_stack(stack, 1);

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

                error_code = StackPush(stack, value);
                break;
            case ADD:
                error_code = StackAdd(stack);
                break;
            case SUB:
                error_code = StackSub(stack);
                break;
            case MUL:
                error_code = StackMul(stack);
                break;
            case DIV:
                error_code = StackDiv(stack);
                break;
            case SQRT:
                error_code = StackSqrt(stack);
                break;
            case POW:
                error_code = StackPow(stack);
                break;
            case OUT:
                error_code = StackOut(stack);
                break;
            case HLT:
                return NOTHING;
                break;
        }

        if (error_code != NOTHING) {
            return error_code;
        }
    }

    return NOTHING;
}
