#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include "stack.h"

CodeError_t StackAdd(stack_t *stack);

CodeError_t StackSub(stack_t *stack);

CodeError_t StackMul(stack_t *stack);

CodeError_t StackDiv(stack_t *stack);

CodeError_t StackSqrt(stack_t *stack);

CodeError_t StackPow(stack_t *stack);

CodeError_t StackOut(stack_t *stack);

CodeError_t StackTop(stack_t *stack);

CodeError_t StackIn(stack_t *stack);

#endif