#include "assembler.h"

const int MaxOperationSize = 5;

int CalcOperHash(char* operation);

int CalcOperHash(char* operation) {
    my_assert(operation, NULLPTR, -1);

    int operation_code = 0;
    for (int c = 0; c < MaxOperationSize; ++c) {
        operation_code = ((operation_code * 13) + operation[c]) % 1000;
        operation[c] = 0;
    }

    return operation_code;
}

Text* ReadCodeFile(const char* file_name) {
    Text* start = TextCtor(file_name);
    my_assert(start, FILE_ERR, NULL);

    CodeError_t error_code = ReadFile(start);
    my_assert(error_code == NOTHING, error_code, NULL);

    return start;
}

CodeError_t ParseNumber(const int* value, char** buf) {
    my_assert(value, NULLPTR, NULLPTR);
    my_assert(buf, NULLPTR, NULLPTR);
    my_assert(*buf, NULLPTR, NULLPTR);

    int read_symbols = 0;
    int correct = sscanf(*buf, "%d%n", value, &read_symbols);
    my_assert(correct == 1, OPERATION_ERR, OPERATION_ERR);
    *buf += read_symbols;

    return NOTHING;
}

CodeError_t ParseString(const char* str, char** buf) {
    my_assert(str, NULLPTR, NULLPTR);
    my_assert(buf, NULLPTR, NULLPTR);
    my_assert(*buf, NULLPTR, NULLPTR);

    int read_symbols = 0;
    int correct = sscanf(*buf, "%s%n", str, &read_symbols);
    my_assert(correct == 1, OPERATION_ERR, OPERATION_ERR);
    *buf += read_symbols;

    return NOTHING;
}

CodeError_t assembler(const char* text_file, const char* commands_file) {
    my_assert(text_file, NULLPTR, NULLPTR);
    my_assert(commands_file, NULLPTR, NULLPTR);

    Text* start = ReadCodeFile(text_file);
    my_assert(start, NULLPTR, NULLPTR);

    FILE* ex_file = fopen(commands_file, "w");
    my_assert(ex_file, FILE_ERR, FILE_ERR);

    char operation[MaxOperationSize + 1] = {};
    char* buf_ptr = start->buf;
    CodeError_t error_code = NOTHING;
    bool is_end = false;

    while (ParseString(operation, &buf_ptr) == NOTHING) {
        int operation_code = CalcOperHash(operation);
        my_assert(operation_code != -1, VALUE_ERR, VALUE_ERR);

        StackElem_t value = 0;
        int correct = 0;
        char reg_type[3] = {0};
        int new_ic = 0;
        
        switch (operation_code) {
            case PUSH_CODE:
                error_code = ParseNumber(&value, &buf_ptr);
                my_assert(error_code == NOTHING, error_code, error_code);

                fprintf(ex_file, "%d %d ", PUSH, value);
                break;
            case JMP_CODE:
                error_code = ParseNumber(&new_ic, &buf_ptr);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                fprintf(ex_file, "%d %d ", JMP, new_ic);
                break;
            case JB_CODE:
                error_code = ParseNumber(&new_ic, &buf_ptr);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                fprintf(ex_file, "%d %d ", JB, new_ic);
                break;
            case PUSHR_CODE:
                error_code = ParseString(reg_type, &buf_ptr);
                my_assert(error_code == NOTHING, error_code, error_code);
                my_assert(reg_type[0] == 'R' && reg_type[2] == 'X' && ('A' <= reg_type[1] && reg_type[1] <= 'D'), REG_IND_ERR, REG_IND_ERR);

                fprintf(ex_file, "%d %d ", PUSHR, (reg_type[1] - 'A'));
                break;
            case POPR_CODE:
                error_code = ParseString(reg_type, &buf_ptr);
                my_assert(error_code == NOTHING, error_code, error_code);
                my_assert(reg_type[0] == 'R' && reg_type[2] == 'X' && ('A' <= reg_type[1] && reg_type[1] <= 'D'), REG_IND_ERR, REG_IND_ERR);
                
                fprintf(ex_file, "%d %d ", POPR, (reg_type[1] - 'A'));
                break;
            case ADD_CODE:
                fprintf(ex_file, "%d ", ADD);
                break;
            case SUB_CODE:
                fprintf(ex_file, "%d ", SUB);
                break;
            case MUL_CODE:
                fprintf(ex_file, "%d ", MUL);
                break;
            case DIV_CODE:
                fprintf(ex_file, "%d ", DIV);
                break;
            case SQRT_CODE:
                fprintf(ex_file, "%d ", SQRT);
                break;
            case POW_CODE:
                fprintf(ex_file, "%d ", POW);
                break;
            case OUT_CODE:
                fprintf(ex_file, "%d ", OUT);
                break;
            case IN_CODE:
                fprintf(ex_file, "%d ", IN);
                break;
            case TOP_CODE:
                fprintf(ex_file, "%d ", TOP);
                break;
            case HLT_CODE:
                fprintf(ex_file, "%d ", HLT);
                is_end = true;
                break;
            default:
                printerr(RED_COLOR "Unknown operation\n" RESET_COLOR);
                fclose(ex_file);
                return OPERATION_ERR;
        }

        if (is_end)
            break;
    }

    fclose(ex_file);
    
    my_assert(is_end, TERM_ERR, TERM_ERR);

    return error_code;
}