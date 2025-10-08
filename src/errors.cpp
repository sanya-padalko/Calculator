#include "errors.h"

void PrintErr(StackErr_t error_type) {
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
        default:
            printerr(YELLOW_COLOR "Unknown error\n");
    }

    printerr(RESET_COLOR);
}
