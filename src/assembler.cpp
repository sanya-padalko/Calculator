#include "assembler.h"

StackErr_t assembler(const char* text_file, const char* commands_file) {
    FILE* start = fopen(text_file, "r");
    FILE* ex_file = fopen(commands_file, "w");

    if (!start) {
        printerr(RED_COLOR "Couldn't open the file %s" RESET_COLOR, commands_file);
        return FILE_ERR;
    }

    if (!ex_file) {
        printerr(RED_COLOR "Couldn't open the file %s" RESET_COLOR, text_file);
        return FILE_ERR;
    }

    char operation[5] = {};

    StackErr_t error_code = NOTHING;

    bool is_end = false;

    while (fscanf(start, "%s", &operation) == 1) {
        if (!strcmp(&operation[0], "PUSH")) {
            StackElem_t value = 0;

            int correct = fscanf(start, "%d", &value);
            if (correct != 1) {
                printerr(RED_COLOR "PUSH must have 1 argument\n" RESET_COLOR);
                error_code = VALUE_ERR;
                break;
            }
            fprintf(ex_file, "1 %d\n", value);  // magic number
        }
        else if (!strcmp(&operation[0], "ADD")) {
            fprintf(ex_file, "2\n");
        }
        else if (!strcmp(&operation[0], "SUB")) {
            fprintf(ex_file, "3\n");
        }
        else if (!strcmp(&operation[0], "MUL")) {
            fprintf(ex_file, "4\n");
        }
        else if (!strcmp(&operation[0], "DIV")) {
            fprintf(ex_file, "5\n");
        }
        else if (!strcmp(&operation[0], "SQRT")) {
            fprintf(ex_file, "6\n");
        }
        else if (!strcmp(&operation[0], "POW")) {
            fprintf(ex_file, "7\n");
        }
        else if (!strcmp(&operation[0], "OUT")) {
            fprintf(ex_file, "8\n");
        }
        else if (!strcmp(&operation[0], "HLT")) {
            fprintf(ex_file, "9\n");
            is_end = true;
            break;
        }
        else {
            printerr(RED_COLOR "Unknown operation\n" RESET_COLOR);
            error_code = OPERATION_ERR;
            break;
        }
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