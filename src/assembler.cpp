#include "assembler.h"

int CalcOperHash(char* operation);
CodeError_t ReadCodeFile(assembler_t* assem);
CodeError_t ParseNumber(assembler_t* assem, const int* value);
CodeError_t ParseMem(assembler_t* assem, const char* str);
CodeError_t ParseString(assembler_t* assem, const char* str, int pass_num);
CodeError_t PrintNumber(assembler_t* assem, const int value);
CodeError_t ParseLabel(assembler_t* assem, const int* value);
CodeError_t WriteToExFile(assembler_t* assem, const char* buf);
CodeError_t ZeroOper(char* operation);

int CalcOperHash(char* operation) {
    my_assert(operation, NULLPTR, -1);

    int operation_code = 0;
    for (int c = 0; c < MaxOperationSize; c++) {
        operation_code = ((operation_code * 33) + operation[c]) % 1000;
    }

    return operation_code;
}

#define CheckReg(reg) my_assert(reg[0] == 'R' && reg[2] == 'X' && ('A' <= reg[1] && reg[1] <= 'D'), REG_IND_ERR, REG_IND_ERR)

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

    fprintf(assem->listing, "%-5d", *value);

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

    fprintf(assem->listing, ":%-4d", *value);
    
    return NOTHING;
}

CodeError_t ParseMem(assembler_t* assem, const char* str) {
    my_assert(str, NULLPTR, NULLPTR);
    my_assert(assem, NULLPTR, NULLPTR);

    int read_symbols = 0;
    int correct = sscanf(assem->buf, " [%s%n", str, &read_symbols);
    my_assert(correct == 1, OPERATION_ERR, OPERATION_ERR);
    assem->buf += read_symbols;
    ++assem->ic;

    fprintf(assem->listing, "[%3s]", str);

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

    if (pass_num == 2 && str[0] == ':')
        assem->ic--;

    assem->buf += read_symbols;
    ++assem->ic;

    return NOTHING;
}

CodeError_t PrintNumber(assembler_t* assem, const int value) {
    my_assert(assem, NULLPTR, NULLPTR);

    sprintf(assem->ex_ptr, "%d ", value);
    while (*assem->ex_ptr != '\0')
        ++assem->ex_ptr;

    fprintf(assem->listing, "%-4d", value);

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
    char operation[MaxOperationSize] = {0};

    CodeError_t error_code = NOTHING;
    assem->ic = 0;
    bool is_end = false;
    int last_ic = 0;

    assem->listing = fopen(assem->listing_file, "w");

    fprintf(assem->listing, "IC    |  Operation     |  Code\n");
    fprintf(assem->listing, "--------------------------------\n");

    while (ZeroOper(operation) == NOTHING && ParseString(assem, operation, pass_num) == NOTHING) {
        if (operation[0] == ':')
            continue;
        
        fprintf(assem->listing, "%04d  |  %-5s  ", last_ic, operation);
        
        int operation_code = CalcOperHash(operation);
        my_assert(operation_code != -1, VALUE_ERR, VALUE_ERR);
        
        size_t operation_counts = sizeof(operations) / sizeof(operation_t);
        
        StackElem_t value = 0;
        int new_ic = 0;
        char reg_type[3] = {0};

        bool known_oper = false;

        for (size_t i = 0; i < operation_counts; ++i) {
            if (operations[i].hash == operation_code) {

                known_oper = true;

                int oper_args = operations[i].args;

                if (oper_args & Number) {
                    error_code = ParseNumber(assem, &value);
                    my_assert(error_code == NOTHING, error_code, error_code);
                }

                if (oper_args & Label) {
                    error_code = ParseLabel(assem, &new_ic);
                    my_assert(error_code == NOTHING, error_code, error_code);
                }

                if (oper_args & Reg) {
                    error_code = ParseString(assem, reg_type, pass_num);
                    my_assert(error_code == NOTHING, error_code, error_code);
                    CheckReg(reg_type);

                    fprintf(assem->listing, "%-5s", reg_type);
                }

                if (oper_args & Mem) {
                    error_code = ParseMem(assem, reg_type);
                    my_assert(error_code == NOTHING, error_code, error_code);
                    CheckReg(reg_type);
                }

                if (oper_args == 0)
                    fprintf(assem->listing, "%*s", 5, "");

                fprintf(assem->listing, "  |  ");

                PrintNumber(assem, operations[i].code);

                if (operation_code == 0) {
                    is_end = true;
                    break;
                }

                if (oper_args & Number)
                    PrintNumber(assem, value);
                
                if (oper_args & Label)
                    PrintNumber(assem, assem->labels[new_ic]);
                
                if (oper_args & Reg)
                    PrintNumber(assem, reg_type[1] - 'A');
                
                if (oper_args & Mem)
                    PrintNumber(assem, reg_type[1] - 'A');

                break;
            }
        }

        fprintf(assem->listing, "\n");
        last_ic = assem->ic;

        if (!known_oper) {
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

CodeError_t ZeroOper(char* operation) {
    my_assert(operation, NULLPTR, NULLPTR);

    for (size_t i = 0 ; i < MaxOperationSize; ++i)
        operation[i] = 0;

    return NOTHING;
}

void CalcAllHashs() {
    size_t operation_counts = sizeof(operations) / sizeof(operations[0]);

    for (size_t i = 0; i < operation_counts; ++i) {
        char dopa_popa[MaxOperationSize] = {0};
        for (size_t j = 0; j < MaxOperationSize; ++j) {
            if (operations[i].name[j] == '\0')
                break;

            dopa_popa[j] = operations[i].name[j];
        }
        printf("%5s: %3d\n", operations[i].name, CalcOperHash(dopa_popa));
    }

    printerr("\n");
}

CodeError_t assembler(assembler_t* assem) {
    my_assert(assem, NULLPTR, NULLPTR);

    CodeError_t error_code = ReadCodeFile(assem);
    my_assert(error_code == NOTHING, error_code, error_code);

    assem->ex_ptr = (char*)calloc(assem->program->file_size, sizeof(char));
    my_assert(assem->ex_ptr, NULLPTR, NULLPTR);

    char operation[MaxOperationSize + 1] = {};

    char* exec_file = assem->ex_ptr;

    PassingCode(assem, 1);      // magic number

    fclose(assem->listing);
    assem->buf = assem->program->buf;
    assem->ex_ptr = exec_file;

    PassingCode(assem, 2);
    
    fclose(assem->listing);

    WriteToExFile(assem, exec_file);

    return error_code;
}