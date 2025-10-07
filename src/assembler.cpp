#include "assembler.h"

int CalcOperHash(char* operation) {
    int operation_code = 0;
    for (int c = 0; c < MaxOperationSize; ++c) {
        operation_code = ((operation_code * 13) + operation[c]) % 1000;
        operation[c] = 0;
    }

    return operation_code;
}

StackErr_t assembler(const char* text_file, const char* commands_file) {
    FILE* start = fopen(text_file, "r");
    if (!start) {
        printerr(RED_COLOR "Couldn't open the file %s" RESET_COLOR, commands_file);
        return FILE_ERR;
    }

    FILE* ex_file = fopen(commands_file, "w");
    if (!ex_file) {
        printerr(RED_COLOR "Couldn't open the file %s" RESET_COLOR, text_file);
        return FILE_ERR;
    }

    char operation[MaxOperationSize + 1] = {};
    StackErr_t error_code = NOTHING;
    bool is_end = false;

    while (fscanf(start, "%s", &operation) == 1) {
        int operation_code = CalcOperHash(operation);

        StackElem_t value = 0;
        int correct = 0;
        char reg_type[3] = {0};
        int new_ic = 0;
        
        switch (operation_code) {
            case PUSH_CODE:
                correct = fscanf(start, "%d", &value);
                check_scanf(correct, 1, "PUSH");

                fprintf(ex_file, "%d %d\n", PUSH, value);
                break;
            case PUSHR_CODE:
                correct = fscanf(start, " %s", &reg_type);
                check_scanf(correct, 1, "PUSHR");

                fprintf(ex_file, "%d %d\n", PUSHR, (reg_type[1] - 'A'));
                break;
            case POPR_CODE:
                correct = fscanf(start, " %s", &reg_type);
                check_scanf(correct, 1, "POPR");

                fprintf(ex_file, "%d %d\n", POPR, (reg_type[1] - 'A'));
                break;
            case JMP_CODE:
                correct = fscanf(start, "%d", &new_ic);
                check_scanf(correct, 1, "JMP");
                
                fprintf(ex_file, "%d %d\n", JMP, new_ic);
                break;
            case JB_CODE:
                correct = fscanf(start, "%d", &new_ic);
                check_scanf(correct, 1, "JB");
                
                fprintf(ex_file, "%d %d\n", JB, new_ic);
                break;
            case ADD_CODE:
                fprintf(ex_file, "%d\n", ADD);
                break;
            case SUB_CODE:
                fprintf(ex_file, "%d\n", SUB);
                break;
            case MUL_CODE:
                fprintf(ex_file, "%d\n", MUL);
                break;
            case DIV_CODE:
                fprintf(ex_file, "%d\n", DIV);
                break;
            case SQRT_CODE:
                fprintf(ex_file, "%d\n", SQRT);
                break;
            case POW_CODE:
                fprintf(ex_file, "%d\n", POW);
                break;
            case OUT_CODE:
                fprintf(ex_file, "%d\n", OUT);
                break;
            case IN_CODE:
                fprintf(ex_file, "%d\n", IN);
                break;
            case TOP_CODE:
                fprintf(ex_file, "%d\n", TOP);
                break;
            case HLT_CODE:
                fprintf(ex_file, "%d\n", HLT);
                is_end = true;
                break;
            default:
                printerr(RED_COLOR "Unknown operation\n" RESET_COLOR);
                fclose(start);
                fclose(ex_file);
                return OPERATION_ERR;
        }

        if (is_end)
            break;
    }

    if (!is_end) {
        printerr(RED_COLOR "The program termination operation is missing\n" RESET_COLOR);
        error_code = OPERATION_ERR;
    }

    fclose(start);
    fclose(ex_file);
    
    return error_code;
}