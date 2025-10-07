#ifndef _ERRORS_H_
#define _ERRORS_H_

#include <stdio.h>

#define printerr(...) fprintf(stderr, __VA_ARGS__)

#define my_assert(comp, code_err, ret_value) \
    if (comp == 0) { \
        printerr("%d\n", comp); \
        printerr(RED_COLOR "error in %s: function -> %s, line %d: " RESET_COLOR, __FILE__, __FUNCTION__, __LINE__); \
        ParseErr(code_err); \
        return ret_value; \
    }

#define RESET_COLOR  "\033[0m"
#define RED_COLOR    "\033[31m"
#define GREEN_COLOR  "\033[32m"
#define YELLOW_COLOR "\033[33m"
#define BLUE_COLOR   "\033[36m" 

enum StackErr_t {
    NOTHING        =   0,
    NULLPTR        =   1,
    SIZE_ERR       =   2,
    EMPTY_STACK    =   3,
    CAPACITY_ERR   =   4,
    CANARY_ERR     =   5,
    REALLOC_ERR    =   6,
    HASH_ERR       =   7,
    CAP_SIZE_ERR   =   8,
    VALUE_ERR      =   9,
    OPERATION_ERR  =  10,  
    FILE_ERR       =  11,  
    STACK_ERR_CNT 
};

const char* const StackErrorMas[] = { "NOTHING", 
                                "NULLPTR", 
                                "SIZE_ERR",
                                "EMPTY_STACK",
                                "CAPACITY_ERR",
                                "CANARY_ERR",
                                "REALLOC_ERR",
                                "HASH_ERR",
                                "CAP_SIZE_ERR",
                                "VALUE_ERR",
                                "OPERATION_ERR",
                                "FILE_ERR"};

void ParseErr(StackErr_t error_type);

#endif // _ERRORS_H