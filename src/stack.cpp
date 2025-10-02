#include "stack.h"

stack_t* StackCtor(ssize_t capacity ON_DEBUG(, VarInfo varinfo)) {
    ON_DEBUG(PrintVarInfo(varinfo));

    stack_t *stack = (stack_t*)calloc(1, sizeof(stack_t));

    if (stack == NULL) {
        printerr(RED_COLOR "Calloc return NULL (during creating struct) in Stack's Constuctor\n" RESET_COLOR);
        return NULL;
    }

    if (capacity > 10000000 || capacity < 2) {      // magic number
        printerr(RED_COLOR "Unavailable size of stack in Stack's Constructor\n" RESET_COLOR);
        return NULL;
    }

    stack->size = 2;        // 0
    stack->capacity = capacity;

    StackElem_t* stack->data = (StackElem_t*)calloc(stack->capacity, sizeof(StackElem_t));
    if (stack->data == NULL) {
        printerr(RED_COLOR "Calloc return NULL in Stack Constructor\n" RESET_COLOR);
        return NULL;
    }

    stack->data[0] = CANARY_LEFT;
    stack->data[stack->capacity - 1] = CANARY_RIGHT;

    for (size_t i = 1; i < stack->capacity - 1; ++i)
        stack->data[i] = POIZON_VALUE;

    stack->hash = calc_hash(stack);

    return stack;
}

StackErr_t StackDtor(stack_t *stack) {
    stackverify(stack);
    if (code_error != NOTHING)
        return code_error;

    free(stack->data);
    stack->data = NULL;
    free(stack);
    stack = NULL;

    return NOTHING;
}

static int StackRealloc(stack_t *stack, ssize_t new_size) {
    stackverify(stack);
    if (code_error != NOTHING)
        return -1;  // magic number

    if (new_size < stack->size) {
        printerr(RED_COLOR "Unavailable realloc-size of stack\n" RESET_COLOR);
        return -1;
    }

    void* realloc_ptr = realloc(stack->data, new_size * sizeof(StackElem_t));
    if (realloc_ptr == NULL) {
        ON_DEBUG(stackdump(stack));
        ParseErr(REALLOC_ERR);
        return -1;
    }

    stack->data = (StackElem_t*)realloc_ptr;

    stack->data[stack->capacity - 1] = POIZON_VALUE;

    while (stack->capacity < new_size) {
        ++stack->capacity;
        stack->data[stack->capacity - 1] = POIZON_VALUE;
    }

    while (stack->capacity > new_size)
        --stack->capacity;

    stack->data[stack->capacity - 1] = CANARY_RIGHT;

    stack->hash = calc_hash(stack);

    return stack->capacity;
}

StackErr_t StackPush(stack_t *stack, StackElem_t new_value) {
    stackverify(stack);
    if (code_error != NOTHING)
        return code_error;

    if (stack->size == stack->capacity) {
        int new_capacity = StackRealloc(stack, stack->size * 2);            // magic number

        if (new_capacity == -1) {
            printerr(RED_COLOR "Push was not completed\n" RESET_COLOR);
            return REALLOC_ERR;
        }
    }

    *(stack->data + stack->size - 1) = new_value;
    
    stack->hash = calc_hash(stack);

    ++stack->size;

    return NOTHING;
}

StackElem_t StackPop(stack_t *stack) {
    stackverify(stack);
    if (code_error != NOTHING)
        return POIZON_VALUE;

    if (stack->size == 2) {
        ON_DEBUG(stackdump(stack));
        ParseErr(EMPTY_STACK);
        printerr(RED_COLOR "You cannot pop from empty stack\n\n" RESET_COLOR);
        return POIZON_VALUE;
    }

    --stack->size;
    StackElem_t last = *(stack->data + stack->size - 1);
    *(stack->data + stack->size - 1) = POIZON_VALUE;

    stack->hash = calc_hash(stack);

    int new_capacity = stack->capacity;
    if (stack->size * 4 == stack->capacity)
        new_capacity = StackRealloc(stack, stack->size);

    if (new_capacity == -1)
        return POIZON_VALUE;

    return last;
}