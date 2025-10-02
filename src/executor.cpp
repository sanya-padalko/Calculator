#include "executor.h"

StackErr_t execution(const char* exec_file) {
    FILE* ex_file = fopen(exec_file, "r");

    if (!ex_file) {
        printerr(RED_COLOR "Couldn't open the file %s" RESET_COLOR, exec_file);
        return FILE_ERR;
    }

    make_stack(stack, 0);

    int operation = 0;

    while (fscanf(ex_file, "%d", &operation) == 1) {
        int value = 0;
        StackErr_t error_code = NOTHING;

        switch(operation) {
            case 1:
                fscanf(ex_file, "%d", &value);
                error_code = StackPush(stack, value);
                break;
            case 2:
                error_code = StackAdd(stack);
                break;
            case 3:
                error_code = StackSub(stack);
                break;
            case 4:
                error_code = StackMul(stack);
                break;
            case 5:
                error_code = StackDiv(stack);
                break;
            case 6:
                error_code = StackSqrt(stack);
                break;
            case 7:
                error_code = StackPow(stack);
                break;
            case 8:
                error_code = StackOut(stack);
                break;
            case 9:
                return NOTHING;
                break;
        }

        if (error_code != NOTHING) {
            ParseErr(error_code);
            return error_code;
        }
    }

    fclose(ex_file);
}