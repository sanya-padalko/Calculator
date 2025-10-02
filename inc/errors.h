#ifndef _ERRORS_H_
#define _ERRORS_H_

#include <stdio.h>

#define printerr(...) fprintf(stderr, __VA_ARGS__)

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
};

void ParseErr(StackErr_t error_type);

#endif // _ERRORS_H