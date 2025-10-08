#include "calculator.h"

static StackElem_t calc_sqrt(StackElem_t value);
static StackElem_t pow(StackElem_t a, StackElem_t b);

CodeError_t StackAdd(stack_t *stack) {
    stackverify(stack);
    my_assert(code_error == NOTHING, code_error, code_error);

    my_assert(stack->size >= 2, SIZE_ERR, SIZE_ERR);

    StackElem_t a = StackPop(stack);
    StackElem_t b = StackPop(stack);

    CodeError_t error_code = StackPush(stack, b + a);

    if (error_code != NOTHING)
        printerr(RED_COLOR "Addition was not completed\n" RESET_COLOR);

    return error_code;
}

CodeError_t StackSub(stack_t *stack) {
    stackverify(stack);
    my_assert(code_error == NOTHING, code_error, code_error);

    my_assert(stack->size >= 2, SIZE_ERR, SIZE_ERR);

    StackElem_t a = StackPop(stack);
    StackElem_t b = StackPop(stack);

    CodeError_t error_code = StackPush(stack, b - a);

    if (error_code != NOTHING)
        printerr(RED_COLOR "Subtraction was not completed\n" RESET_COLOR);

    return error_code;
}

CodeError_t StackMul(stack_t *stack) {
    stackverify(stack);
    my_assert(code_error == NOTHING, code_error, code_error);

    my_assert(stack->size >= 2, SIZE_ERR, SIZE_ERR);

    StackElem_t a = StackPop(stack);
    StackElem_t b = StackPop(stack);

    CodeError_t error_code = StackPush(stack, b * a);

    if (error_code != NOTHING)
        printerr(RED_COLOR "Multiplication was not completed\n" RESET_COLOR);

    return error_code;
}

CodeError_t StackDiv(stack_t *stack) {
    stackverify(stack);
    my_assert(code_error == NOTHING, code_error, code_error);

    my_assert(stack->size >= 2, SIZE_ERR, SIZE_ERR);

    StackElem_t a = StackPop(stack);
    StackElem_t b = StackPop(stack);

    if (a == 0) {
        printerr(RED_COLOR "You cannot divise by zero\n" RESET_COLOR);
        return VALUE_ERR;
    }

    CodeError_t error_code = StackPush(stack, b / a);

    if (error_code != NOTHING)
        printerr(RED_COLOR "Division was not completed\n" RESET_COLOR);

    return error_code;
}

static StackElem_t calc_sqrt(StackElem_t value) {
    my_assert(value >= 0, VALUE_ERR, VALUE_ERR);

    StackElem_t l = 0, r = value + 1;

    while (r - l > 1) {
        StackElem_t m = (l + r) / 2;

        if (m * m > value)
            r = m;
        else
            l = m;
    }

    return l;
}

CodeError_t StackSqrt(stack_t *stack) {
    stackverify(stack);
    my_assert(code_error == NOTHING, code_error, code_error);

    my_assert(stack->size, EMPTY_STACK, EMPTY_STACK);

    StackElem_t a = StackPop(stack);

    StackElem_t sqrt_a = calc_sqrt(a);

    if (sqrt_a < 0) {
        printerr(RED_COLOR "Squarting was not completed" RESET_COLOR);
        return VALUE_ERR;
    }

    CodeError_t error_code = StackPush(stack, sqrt_a);

    if (error_code != NOTHING)
        printerr(RED_COLOR "Squarting was not completed\n" RESET_COLOR);

    return error_code;
}

static StackElem_t pow(StackElem_t a, StackElem_t b) {
    StackElem_t res = 1;

    for (size_t i = 0; i < b; ++i)
        res *= a;
    
    return res;
}

CodeError_t StackPow(stack_t *stack) {
    stackverify(stack);
    if (code_error != NOTHING)
        return code_error;

    if (stack->size < 2) {
        printerr(RED_COLOR "Not enough numbers for exponentiation\n" RESET_COLOR);
        return SIZE_ERR;
    }

    StackElem_t a = StackPop(stack);
    StackElem_t b = StackPop(stack);

    my_assert(a >= 0, VALUE_ERR, VALUE_ERR);

    CodeError_t error_code = StackPush(stack, pow(b, a));

    if (error_code != NOTHING)
        printerr(RED_COLOR "Exponentation was not completed\n" RESET_COLOR);

    return error_code;
}

CodeError_t StackOut(stack_t *stack) {
    stackverify(stack);
    my_assert(code_error == NOTHING, code_error, code_error);

    my_assert(stack->size > 0, EMPTY_STACK, EMPTY_STACK);

    StackElem_t a = StackPop(stack);

    if (a == POIZON_VALUE) {
        printerr(RED_COLOR "Something in Outputing went wrong\n" RESET_COLOR);
        return VALUE_ERR;
    }

    printf("Result: %d\n", a);

    return NOTHING;
}

CodeError_t StackTop(stack_t *stack) {
    stackverify(stack);
    my_assert(code_error == NOTHING, code_error, code_error);

    my_assert(stack->size > 0, EMPTY_STACK, EMPTY_STACK);

    StackElem_t a = stack->data[stack->size - 1];

    return NOTHING;
}

CodeError_t StackIn(stack_t *stack) {
    stackverify(stack);
    my_assert(code_error == NOTHING, code_error, code_error);

    StackElem_t value = 0;
    int scanf_check = scanf("%d", &value);

    my_assert(scanf_check == 1, INPUT_ERR, INPUT_ERR);

    CodeError_t error_code = StackPush(stack, value);

    if (error_code != NOTHING)
        printerr(RED_COLOR "Input was not completed\n" RESET_COLOR);

    return error_code;
}