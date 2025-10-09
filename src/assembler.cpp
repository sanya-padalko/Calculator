#include "assembler.h"

const int MaxOperationSize = 5;

int CalcOperHash(char* operation);

int CalcOperHash(char* operation) {
    my_assert(operation, NULLPTR, -1);

    int operation_code = 0;
    for (int c = 0; c < MaxOperationSize; ++c) {
        operation_code = ((operation_code * 33) + operation[c]) % 1000;
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

CodeError_t PrintNumber(const int value, char** buf) {
    my_assert(buf, NULLPTR, NULLPTR);
    my_assert(*buf, NULLPTR, NULLPTR);

    sprintf(*buf, "%d ", value);
    while (**buf != '\0')
        ++*buf;

    return NOTHING;
}

CodeError_t WriteToExFile(const char* file_name, const char* buf, int s_count) {
    my_assert(file_name, NULLPTR, NULLPTR);
    my_assert(buf, NULLPTR, NULLPTR);

    FILE* ex_file = fopen(file_name, "w");
    my_assert(ex_file, FILE_ERR, FILE_ERR);

    fwrite(buf, sizeof(char), s_count, ex_file);
    fclose(ex_file);
}

CodeError_t assembler(const char* text_file, const char* commands_file) {
    my_assert(text_file, NULLPTR, NULLPTR);
    my_assert(commands_file, NULLPTR, NULLPTR);

    Text* start = ReadCodeFile(text_file);
    my_assert(start, NULLPTR, NULLPTR);

    char* exec_file = (char*)calloc(start->file_size, sizeof(char));
    my_assert(exec_file, NULLPTR, NULLPTR);

    char operation[MaxOperationSize + 1] = {};

    char* buf_ptr = start->buf;
    char* ex_ptr  = exec_file;

    CodeError_t error_code = NOTHING;
    bool is_end = false;

    while (ParseString(operation, &buf_ptr) == NOTHING) {
        int operation_code = CalcOperHash(operation);
        my_assert(operation_code != -1, VALUE_ERR, VALUE_ERR);

        StackElem_t value = 0;
        char reg_type[3] = {0};
        int new_ic = 0;
        
        switch (operation_code) {
            case PUSH_CODE:
                error_code = ParseNumber(&value, &buf_ptr);
                my_assert(error_code == NOTHING, error_code, error_code);

                PrintNumber(PUSH, &ex_ptr);
                PrintNumber(value, &ex_ptr);
                break;
            case JMP_CODE:
                error_code = ParseNumber(&new_ic, &buf_ptr);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(JMP, &ex_ptr);
                PrintNumber(new_ic, &ex_ptr);
                break;
            case JB_CODE:
                error_code = ParseNumber(&new_ic, &buf_ptr);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(JB, &ex_ptr);
                PrintNumber(new_ic, &ex_ptr);
                break;
            case JBE_CODE:
                error_code = ParseNumber(&new_ic, &buf_ptr);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(JB, &ex_ptr);
                PrintNumber(new_ic, &ex_ptr);
                break;
            case JA_CODE:
                error_code = ParseNumber(&new_ic, &buf_ptr);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(JA, &ex_ptr);
                PrintNumber(new_ic, &ex_ptr);
                break;
            case JAE_CODE:
                error_code = ParseNumber(&new_ic, &buf_ptr);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(JAE, &ex_ptr);
                PrintNumber(new_ic, &ex_ptr);
                break;
            case JE_CODE:
                error_code = ParseNumber(&new_ic, &buf_ptr);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(JE, &ex_ptr);
                PrintNumber(new_ic, &ex_ptr);
                break;
            case JNE_CODE:
                error_code = ParseNumber(&new_ic, &buf_ptr);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(JNE, &ex_ptr);
                PrintNumber(new_ic, &ex_ptr);
                break;
            case PUSHR_CODE:
                error_code = ParseString(reg_type, &buf_ptr);
                my_assert(error_code == NOTHING, error_code, error_code);
                my_assert(reg_type[0] == 'R' && reg_type[2] == 'X' && ('A' <= reg_type[1] && reg_type[1] <= 'D'), REG_IND_ERR, REG_IND_ERR);

                PrintNumber(PUSHR, &ex_ptr);
                PrintNumber(reg_type[1] - 'A', &ex_ptr);
                break;
            case POPR_CODE:
                error_code = ParseString(reg_type, &buf_ptr);
                my_assert(error_code == NOTHING, error_code, error_code);
                my_assert(reg_type[0] == 'R' && reg_type[2] == 'X' && ('A' <= reg_type[1] && reg_type[1] <= 'D'), REG_IND_ERR, REG_IND_ERR);
                
                PrintNumber(POPR, &ex_ptr);
                PrintNumber(reg_type[1] - 'A', &ex_ptr);
                break;
            case ADD_CODE:
                PrintNumber(ADD, &ex_ptr);
                break;
            case SUB_CODE:
                PrintNumber(SUB, &ex_ptr);
                break;
            case MUL_CODE:
                PrintNumber(MUL, &ex_ptr);
                break;
            case DIV_CODE:
                PrintNumber(DIV, &ex_ptr);
                break;
            case SQRT_CODE:
                PrintNumber(SQRT, &ex_ptr);
                break;
            case POW_CODE:
                PrintNumber(POW, &ex_ptr);
                break;
            case OUT_CODE:
                PrintNumber(OUT, &ex_ptr);
                break;
            case IN_CODE:
                PrintNumber(IN, &ex_ptr);
                break;
            case TOP_CODE:
                PrintNumber(TOP, &ex_ptr);
                break;
            case HLT_CODE:
                PrintNumber(HLT, &ex_ptr);
                is_end = true;
                break;
            default:
                printerr(RED_COLOR "Unknown operation\n" RESET_COLOR);
                return OPERATION_ERR;
        }

        if (is_end)
            break;
    }
    
    my_assert(is_end, TERM_ERR, TERM_ERR);

    WriteToExFile(commands_file, exec_file, ex_ptr - exec_file);

    return error_code;
}