#include "assembler.h"

const int MaxOperationSize = 5;

int CalcOperHash(char* operation);
CodeError_t ReadCodeFile(assembler_t* assem);
CodeError_t ParseNumber(assembler_t* assem, const int* value);
CodeError_t ParseString(assembler_t* assem, const char* str, int pass_num);
CodeError_t PrintNumber(assembler_t* assem, const int value);
CodeError_t ParseLabel(assembler_t* assem, const int* value);
CodeError_t WriteToExFile(assembler_t* assem, const char* buf);

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

    assem->buf = assem->program->buf;

    CodeError_t error_code = ReadFile(assem->program);
    my_assert(error_code == NOTHING, error_code, NULLPTR);

    return NOTHING;
}

CodeError_t ParseNumber(assembler_t* assem, const int* value) {
    my_assert(value, NULLPTR, NULLPTR);
    my_assert(assem, NULLPTR, NULLPTR);

    int read_symbols = 0;
    int correct = sscanf(assem->buf, "%d%n", value, &read_symbols);
    my_assert(correct == 1, OPERATION_ERR, OPERATION_ERR);
    assem->buf += read_symbols;
    ++assem->ic;

    return NOTHING;
}

CodeError_t ParseLabel(assembler_t* assem, const int* value) {
    my_assert(value, NULLPTR, NULLPTR);
    my_assert(assem, NULLPTR, NULLPTR);

    int read_symbols;
    int correct = sscanf(assem->buf, " :%d%n", value, &read_symbols);
    my_assert(correct == 1, OPERATION_ERR, OPERATION_ERR);
    assem->buf += read_symbols;
    ++assem->ic;

    return NOTHING;
}

CodeError_t ParseString(assembler_t* assem, const char* str, int pass_num) {
    my_assert(str, NULLPTR, NULLPTR);
    my_assert(assem, NULLPTR, NULLPTR);

    int read_symbols = 0;
    int correct = sscanf(assem->buf, "%s%n", str, &read_symbols);
    my_assert(correct == 1, OPERATION_ERR, OPERATION_ERR);

    if (pass_num == 1 && str[0] == ':')
        assem->labels[str[1] - '0'] = assem->ic--;

    assem->buf += read_symbols;
    ++assem->ic;

    return NOTHING;
}

CodeError_t PrintNumber(assembler_t* assem, const int value) {
    my_assert(assem, NULLPTR, NULLPTR);

    sprintf(assem->ex_ptr, "%d ", value);
    while (*assem->ex_ptr != '\0')
        ++assem->ex_ptr;

    return NOTHING;
}

CodeError_t WriteToExFile(assembler_t* assem, const char* buf) {
    my_assert(assem, NULLPTR, NULLPTR);
    my_assert(buf, NULLPTR, NULLPTR);

    FILE* ex_file = fopen(assem->commands_file, "w");
    my_assert(ex_file, FILE_ERR, FILE_ERR);

    fwrite(buf, sizeof(char), assem->ex_ptr - buf, ex_file);
    fclose(ex_file);
}

CodeError_t PassingCode(assembler_t* assem, int pass_num) {
    char operation[MaxOperationSize + 1] = {};

    CodeError_t error_code = NOTHING;
    assem->ic = 0;
    bool is_end = false;

    while (ParseString(assem, operation, pass_num) == NOTHING) {
        if (operation[0] == ':')
            continue;

        int operation_code = CalcOperHash(operation);
        my_assert(operation_code != -1, VALUE_ERR, VALUE_ERR);
        
        switch (operation_code) {
            case PUSH_CODE:
            {
                StackElem_t value = 0;
                error_code = ParseNumber(assem, &value);
                my_assert(error_code == NOTHING, error_code, error_code);

                PrintNumber(assem, PUSH);
                PrintNumber(assem, value);
                break;
            }
            case JMP_CODE:
            {
                int new_ic = 0;
                error_code = ParseLabel(assem, &new_ic);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(assem, JMP);
                PrintNumber(assem, assem->labels[new_ic]);
                break;
            }
            case JB_CODE:
            {
                int new_ic = 0;
                error_code = ParseLabel(assem, &new_ic);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(assem, JB);
                PrintNumber(assem, assem->labels[new_ic]);
                break;
            }
            case JBE_CODE:
            {
                int new_ic = 0;
                error_code = ParseLabel(assem, &new_ic);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(assem, JBE);
                PrintNumber(assem, assem->labels[new_ic]);
                break;
            }
            case JA_CODE:
            {
                int new_ic = 0;
                error_code = ParseLabel(assem, &new_ic);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(assem, JA);
                PrintNumber(assem, assem->labels[new_ic]);
                break;
            }
            case JAE_CODE:
            {
                int new_ic = 0;
                error_code = ParseLabel(assem, &new_ic);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(assem, JAE);
                PrintNumber(assem, assem->labels[new_ic]);
                break;
            }
            case JE_CODE:
            {
                int new_ic = 0;
                error_code = ParseLabel(assem, &new_ic);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(assem, JE);
                PrintNumber(assem, assem->labels[new_ic]);
                break;
            }
            case JNE_CODE:
            {
                int new_ic = 0;
                error_code = ParseLabel(assem, &new_ic);
                my_assert(error_code == NOTHING, error_code, error_code);
                
                PrintNumber(assem, JNE);
                PrintNumber(assem, assem->labels[new_ic]);
                break;
            }
            case CALL_CODE:
            {
                int new_ic = 0;
                error_code = ParseLabel(assem, &new_ic);
                my_assert(error_code == NOTHING, error_code, error_code);

                PrintNumber(assem, CALL);
                PrintNumber(assem, assem->labels[new_ic]);
                break;
            }
            case RET_CODE:
            {
                PrintNumber(assem, RET);
                break;
            }
            case PUSHR_CODE:
            {
                char reg_type[3] = {0};
                error_code = ParseString(assem, reg_type, pass_num);
                my_assert(error_code == NOTHING, error_code, error_code);
                my_assert(reg_type[0] == 'R' && reg_type[2] == 'X' && ('A' <= reg_type[1] && reg_type[1] <= 'D'), REG_IND_ERR, REG_IND_ERR);

                PrintNumber(assem, PUSHR);
                PrintNumber(assem, reg_type[1] - 'A');
                break;
            }
            case POPR_CODE:
            {
                char reg_type[3] = {0};
                error_code = ParseString(assem, reg_type, pass_num);
                my_assert(error_code == NOTHING, error_code, error_code);
                my_assert(reg_type[0] == 'R' && reg_type[2] == 'X' && ('A' <= reg_type[1] && reg_type[1] <= 'D'), REG_IND_ERR, REG_IND_ERR);
                
                PrintNumber(assem, POPR);
                PrintNumber(assem, reg_type[1] - 'A');
                break;
            }
            case ADD_CODE:
            {
                PrintNumber(assem, ADD);
                break;
            }
            case SUB_CODE:
            {
                PrintNumber(assem, SUB);
                break;
            }
            case MUL_CODE:
            {
                PrintNumber(assem, MUL);
                break;
            }
            case DIV_CODE:
            {
                PrintNumber(assem, DIV);
                break;
            }
            case SQRT_CODE:
            {
                PrintNumber(assem, SQRT);
                break;
            }
            case POW_CODE:
            {
                PrintNumber(assem, POW);
                break;
            }
            case OUT_CODE:
            {
                PrintNumber(assem, OUT);
                break;
            }
            case IN_CODE:
            {
                PrintNumber(assem, IN);
                break;
            }
            case TOP_CODE:
            {
                PrintNumber(assem, TOP);
                break;
            }
            case HLT_CODE:
            {
                PrintNumber(assem, HLT);
                is_end = true;
                break;
            }
            default:
            {
                printerr("%d\n", operation_code);
                printerr(RED_COLOR "Unknown operation\n" RESET_COLOR);
                return OPERATION_ERR;
            }
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

    PassingCode(assem, 1);

    assem->buf = assem->program->buf;
    assem->ex_ptr = exec_file;

    PassingCode(assem, 2);

    WriteToExFile(assem, exec_file);

    return error_code;
}