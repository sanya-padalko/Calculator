#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include "stack.h"

enum StackOper {
    VOID  =  0,
    PUSH  =  1,
    ADD   =  2,
    SUB   =  3,
    MUL   =  4,
    DIV   =  5,
    SQRT  =  6,
    POW   =  7,
    OUT   =  8,
    HLT   =  9,
    TOP   = 10,
    IN    = 11,
    PUSHR = 12, 
    POPR  = 13,
};

StackErr_t StackAdd(stack_t *stack);

StackErr_t StackSub(stack_t *stack);

StackErr_t StackMul(stack_t *stack);

StackErr_t StackDiv(stack_t *stack);

StackErr_t StackSqrt(stack_t *stack);

StackErr_t StackPow(stack_t *stack);

StackErr_t StackOut(stack_t *stack);

StackErr_t StackTop(stack_t *stack);

StackErr_t StackIn(stack_t *stack);

#endif