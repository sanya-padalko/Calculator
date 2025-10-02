#ifndef _STACK_H_
#define _STACK_H_

#include <stdlib.h>
#include "errors.h"
#include "verify.h"

#ifdef DEBUG
#define make_stack(name, capacity) \
        stack_t* name = StackCtor(capacity + 2, VarInfoCtor(#name, __FILE__, __FUNCTION__, __LINE__));
#define ON_DEBUG(...) __VA_ARGS__
#else
#define make_stack(name, capacity) \
        stack_t* name = StackCtor(capacity + 2);
#define ON_DEBUG(...)
#endif

stack_t* StackCtor(ssize_t capacity ON_DEBUG(, VarInfo varinfo));

StackErr_t StackDtor(stack_t *stack);

StackErr_t StackPush(stack_t *stack, StackElem_t new_value);

StackElem_t StackPop(stack_t *stack);

#endif