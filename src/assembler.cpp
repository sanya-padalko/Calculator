#include "assembler.h"

const int MaxOperationSize = 5;

int CalcOperHash(char* operation);
CodeError_t ReadCodeFile(assembler_t* assem);
CodeError_t ParseNumber(const int* value, char** buf);
CodeError_t ParseString(const char* str, char** buf, int* labels, int* ic);
CodeError_t PrintNumber(const int value, char** buf);
CodeError_t WriteToExFile(const char* file_name, const char* buf, int s_count);

int CalcOperHash(char* operation) {
    my_assert(operation, NULLPTR, -1);

    int operation_code = 0;
    for (int c = 0; c < MaxOperationSize; c++) {
        operation_code = ((operation_code * 33) + operation[c]) % 1000;
        operation[c] = 0;
    }

    return operation_code;
}

CodeError_t ReadCodeFile(assembler_t* assem) {
    my_assert(assem, NULLPTR, NULLPTR);

    assem->program = TextCtor(assem->text_file);
    my_assert(assem->program, FILE_ERR, NULLPTR);

    CodeError_t error_code = ReadFile(assem->program);
    my_assert(error_code == NOTHING, error_code, NULLPTR);

    return NOTHING;
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

CodeError_t ParseString(const char* str, char** buf, int* labels, int* ic) {
    my_assert(str, NULLPTR, NULLPTR);
    my_assert(buf, NULLPTR, NULLPTR);
    my_assert(*buf, NULLPTR, NULLPTR);
    my_assert(labels, NULLPTR, NULLPTR);

    int read_symbols = 0;
    int correct = sscanf(*buf, "%s%n", str, &read_symbols);
    my_assert(correct == 1, OPERATION_ERR, OPERATION_ERR);

    if (str[0] == ':')
        labels[str[1] - '0'] = (*ic)--;

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

CodeError_t PassingCode(assembler_t* assem, int pass_num) {
    char operation[MaxOperationSize + 1] = {};

    CodeError_t error_code = NOTHING;
    assem->ic = 0;
    bool is_end = false;

    while (ParseString(operation, &assem->program->buf, assem->labels, &assem->ic) == NOTHING) {
        ++assem->ic;
        if (operation[0] == ':')
            continue;

        int operation_code = CalcOperHash(operation);
        my_assert(operation_code != -1, VALUE_ERR, VALUE_ERR);

        if (pass_num == 1) {
            if (operation_code == HLT_CODE) {
                break;
            }
            continue;
        }

        StackElem_t value = 0;
        char reg_type[3] = {0};
        int new_ic = 0;
        
        switch (operation_code) {
            case PUSH_CODE:
                error_code = ParseNumber(&value, &assem->program->buf);
                my_assert(error_code == NOTHING, error_code, error_code);

                PrintNumber(PUSH, &assem->ex_ptr);
                PrintNumber(value, &assem->ex_ptr);
                break;
            case JMP_CODE:
                error_code = ParseNumber(&new_ic, &assem->program->buf);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(JMP, &assem->ex_ptr);
                PrintNumber(assem->labels[new_ic], &assem->ex_ptr);
                break;
            case JB_CODE:
                error_code = ParseNumber(&new_ic, &assem->program->buf);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(JB, &assem->ex_ptr);
                PrintNumber(assem->labels[new_ic], &assem->ex_ptr);
                break;
            case JBE_CODE:
                error_code = ParseNumber(&new_ic, &assem->program->buf);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(JB, &assem->ex_ptr);
                PrintNumber(assem->labels[new_ic], &assem->ex_ptr);
                break;
            case JA_CODE:
                error_code = ParseNumber(&new_ic, &assem->program->buf);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(JA, &assem->ex_ptr);
                PrintNumber(assem->labels[new_ic], &assem->ex_ptr);
                break;
            case JAE_CODE:
                error_code = ParseNumber(&new_ic, &assem->program->buf);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(JAE, &assem->ex_ptr);
                PrintNumber(assem->labels[new_ic], &assem->ex_ptr);
                break;
            case JE_CODE:
                error_code = ParseNumber(&new_ic, &assem->program->buf);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(JE, &assem->ex_ptr);
                PrintNumber(assem->labels[new_ic], &assem->ex_ptr);
                break;
            case JNE_CODE:
                error_code = ParseNumber(&new_ic, &assem->program->buf);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(JNE, &assem->ex_ptr);
                PrintNumber(assem->labels[new_ic], &assem->ex_ptr);
                break;
            case PUSHR_CODE:
                error_code = ParseString(reg_type, &assem->program->buf, assem->labels, &assem->ic);
                my_assert(error_code == NOTHING, error_code, error_code);
                my_assert(reg_type[0] == 'R' && reg_type[2] == 'X' && ('A' <= reg_type[1] && reg_type[1] <= 'D'), REG_IND_ERR, REG_IND_ERR);

                PrintNumber(PUSHR, &assem->ex_ptr);
                PrintNumber(reg_type[1] - 'A', &assem->ex_ptr);
                break;
            case POPR_CODE:
                error_code = ParseString(reg_type, &assem->program->buf, assem->labels, &assem->ic);
                my_assert(error_code == NOTHING, error_code, error_code);
                my_assert(reg_type[0] == 'R' && reg_type[2] == 'X' && ('A' <= reg_type[1] && reg_type[1] <= 'D'), REG_IND_ERR, REG_IND_ERR);
                
                PrintNumber(POPR, &assem->ex_ptr);
                PrintNumber(reg_type[1] - 'A', &assem->ex_ptr);
                break;
            case ADD_CODE:
                PrintNumber(ADD, &assem->ex_ptr);
                break;
            case SUB_CODE:
                PrintNumber(SUB, &assem->ex_ptr);
                break;
            case MUL_CODE:
                PrintNumber(MUL, &assem->ex_ptr);
                break;
            case DIV_CODE:
                PrintNumber(DIV, &assem->ex_ptr);
                break;
            case SQRT_CODE:
                PrintNumber(SQRT, &assem->ex_ptr);
                break;
            case POW_CODE:
                PrintNumber(POW, &assem->ex_ptr);
                break;
            case OUT_CODE:
                PrintNumber(OUT, &assem->ex_ptr);
                break;
            case IN_CODE:
                PrintNumber(IN, &assem->ex_ptr);
                break;
            case TOP_CODE:
                PrintNumber(TOP, &assem->ex_ptr);
                break;
            case HLT_CODE:
                PrintNumber(HLT, &assem->ex_ptr);
                is_end = true;
                break;
            default:
                printerr("%d\n", operation_code);
                printerr(RED_COLOR "Unknown operation\n" RESET_COLOR);
                return OPERATION_ERR;
        }

        if (is_end)
            break;
    }

    if (pass_num == 2) my_assert(is_end, TERM_ERR, TERM_ERR);

    return NOTHING;
}

CodeError_t assembler(assembler_t* assem) {
    my_assert(assem, NULLPTR, NULLPTR);

    CodeError_t error_code = ReadCodeFile(assem);
    my_assert(error_code == NOTHING, error_code, error_code);

    assem->ex_ptr = (char*)calloc(assem->program->file_size, sizeof(char));
    my_assert(assem->ex_ptr, NULLPTR, NULLPTR);

    char operation[MaxOperationSize + 1] = {};

    char* exec_file = assem->ex_ptr;
    char* buf_ptr = assem->program->buf;

    PassingCode(assem, true);

    assem->program->buf = buf_ptr;

    PassingCode(assem, false);

    WriteToExFile(assem->commands_file, exec_file, assem->ex_ptr - exec_file);

    return error_code;
}