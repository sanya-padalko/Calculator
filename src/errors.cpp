#include "errors.h"

void PrintErr(CodeError_t error_type, const char* file_name, const char* func_name, const int line_ind) {
    printerr(RED_COLOR "error in %s: function -> %s, line %d: " RESET_COLOR, file_name, func_name, line_ind);
    switch (error_type) {
        case NOTHING:
            printerr(GREEN_COLOR "Everything is OK\n");
            break;
        case NULLPTR:
            printerr(RED_COLOR "Pointer went equal NULL\n");
            break;
        case SIZE_ERR:
            printerr(RED_COLOR "Stack has unavailable size\n");
            break;
        case CAPACITY_ERR:
            printerr(RED_COLOR "Stack has unavailable capactiy\n");
            break;
        case EMPTY_STACK:
            printerr(RED_COLOR "Trying to work with empty stack\n");
            break;
        case CANARY_ERR:
            printerr(RED_COLOR "The canaries have been changed\n");
            break;
        case REALLOC_ERR:
            printerr(RED_COLOR "Realloc went wrong\n");
            break;
        case HASH_ERR:
            printerr(RED_COLOR "Hash of stack went wrong. The inside of the stack has been changed\n");
            break;
        case CAP_SIZE_ERR:
            printerr(RED_COLOR "Size become greater than capacity in stack\n");
            break;
        case VALUE_ERR:
            printerr(RED_COLOR "Trying to work with POIZON value\n");
            break;
        case OPERATION_ERR:
            printerr(RED_COLOR "Unknown operation to stack\n");
            break;
        case FILE_ERR:
            printerr(RED_COLOR "Working with file went wrong\n");
            break;
        case STACK_ERR:
            printerr(RED_COLOR "Something went wrong with stack\n");
            break;
        case CODE_ERR:
            printerr(RED_COLOR "Something went wrong witch code\n");
            break;
        case CMD_IND_ERR:
            printerr(RED_COLOR "Unavailbable index of command\n");
            break;
        case INPUT_ERR:
            printerr(RED_COLOR "Wrong input\n");
            break;
        case REG_IND_ERR:
            printerr(RED_COLOR "Unavailable index of register\n");
            break;
        default:
            printerr(YELLOW_COLOR "Unknown error\n");
    }

    printerr(RESET_COLOR);
}
