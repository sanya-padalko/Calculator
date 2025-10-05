#include "assembler.h"

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

    char operation[5] = {};

    StackErr_t error_code = NOTHING;

    bool is_end = false;

    while (fscanf(start, "%s", &operation) == 1) {
        int operation_code = 0;
        for (int c = 0; c < 4; ++c)
            operation_code = (operation_code << 1) + operation[c];
        
        StackElem_t value = 0;
        int correct = 0;
        
        switch (operation_code) {
            case PUSH_CODE:
                correct = fscanf(start, "%d", &value);
                if (correct != 1) {
                    printerr(RED_COLOR "PUSH must have 1 argument\n" RESET_COLOR);
                    error_code = VALUE_ERR;
                    break;
                }
                fprintf(ex_file, "%d %d\n", PUSH, value);
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
            case HLT_CODE:
                fprintf(ex_file, "%d\n", HLT);
                is_end = true;
                break;
            default:
                printerr(RED_COLOR "Unknown operation\n" RESET_COLOR);
                error_code = OPERATION_ERR;
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

StackErr_t disassembler(const char* commands_file, const char* text_file) {
    FILE* start = fopen(text_file, "w");
    FILE* ex_file = fopen(commands_file, "r");

    if (!start) {
        printerr(RED_COLOR "Couldn't open the file %s" RESET_COLOR, commands_file);
        return FILE_ERR;
    }

    if (!ex_file) {
        printerr(RED_COLOR "Couldn't open the file %s" RESET_COLOR, text_file);
        return FILE_ERR;
    }

    int oper = 0;

    while (fscanf(ex_file, "%d", &oper) == 1) {
        int value = 0;

        switch(oper) {
            case 1:     // magic number
                fprintf(start, "PUSH ");
                if (fscanf(ex_file, "%d", &value) == 1) {
                    fprintf(start, "%d\n", value);
                }
                else {
                    printerr(RED_COLOR "First operation must have 1 argument\n" RESET_COLOR);
                    fclose(start);
                    fclose(ex_file);
                    return VALUE_ERR;
                }
                break;
            case 2:
                fprintf(start, "ADD\n");
                break;
            case 3:
                fprintf(start, "SUB\n");
                break;
            case 4:
                fprintf(start, "MUL\n");
                break;
            case 5:
                fprintf(start, "DIV\n");
                break;
            case 6:
                fprintf(start, "SQRT\n");
                break;
            case 7:
                fprintf(start, "POW\n");
                break;
            case 8:
                fprintf(start, "OUT\n");
                break;
            case 9:
                fprintf(start, "HLT\n");
                break;
            default:
                printerr(RED_COLOR "Unknown operation\n" RESET_COLOR);
                fclose(start);
                fclose(ex_file);
                return OPERATION_ERR;
        }
    }

    fclose(start);
    fclose(ex_file);

    return NOTHING;
}