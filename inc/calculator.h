#ifndef _CALCULATOR_H_
#define _CALCULATOR_H_

#include "stack.h"

StackErr_t StackAdd(stack_t *stack);

StackErr_t StackSub(stack_t *stack);

StackErr_t StackMul(stack_t *stack);

StackErr_t StackDiv(stack_t *stack);

StackErr_t StackSqrt(stack_t *stack);

StackErr_t StackPow(stack_t *stack);

StackErr_t StackOut(stack_t *stack);

#endif