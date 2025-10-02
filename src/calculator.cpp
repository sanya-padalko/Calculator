#include "calculator.h"

StackErr_t StackAdd(stack_t *stack) {
    stackverify(stack);
    if (code_error != NOTHING)
        return code_error;

    if (stack->size < 4) {
        printerr(RED_COLOR "Not enough numbers for addition\n" RESET_COLOR);
        return SIZE_ERR;
    }

    StackElem_t a = StackPop(stack);
    StackElem_t b = StackPop(stack);

    StackErr_t error_code = StackPush(stack, b + a);

    if (error_code != NOTHING)
        printerr(RED_COLOR "Addition was not completed\n" RESET_COLOR);

    return error_code;
}

StackErr_t StackSub(stack_t *stack) {
    stackverify(stack);
    if (code_error != NOTHING)
        return code_error;

    if (stack->size < 4) {
        printerr(RED_COLOR "Not enough numbers for subtraction\n" RESET_COLOR);
        return SIZE_ERR;
    }

    StackElem_t a = StackPop(stack);
    StackElem_t b = StackPop(stack);

    StackErr_t error_code = StackPush(stack, b - a);

    if (error_code != NOTHING)
        printerr(RED_COLOR "Subtraction was not completed\n" RESET_COLOR);

    return error_code;
}

StackErr_t StackMul(stack_t *stack) {
    stackverify(stack);
    if (code_error != NOTHING)
        return code_error;

    if (stack->size < 4) {
        printerr(RED_COLOR "Not enough numbers for multiplication\n" RESET_COLOR);
        return SIZE_ERR;
    }

    StackElem_t a = StackPop(stack);
    StackElem_t b = StackPop(stack);

    StackErr_t error_code = StackPush(stack, b * a);

    if (error_code != NOTHING)
        printerr(RED_COLOR "Multiplication was not completed\n" RESET_COLOR);

    return error_code;
}

StackErr_t StackDiv(stack_t *stack) {
    stackverify(stack);
    if (code_error != NOTHING)
        return code_error;

    if (stack->size < 4) {
        printerr(RED_COLOR "Not enough numbers for division\n" RESET_COLOR);
        return SIZE_ERR;
    }

    StackElem_t a = StackPop(stack);
    StackElem_t b = StackPop(stack);

    if (a == 0) {
        printerr(RED_COLOR "You cannot divise by zero\n" RESET_COLOR);
        return VALUE_ERR;
    }

    StackErr_t error_code = StackPush(stack, b / a);

    if (error_code != NOTHING)
        printerr(RED_COLOR "Division was not completed\n" RESET_COLOR);

    return error_code;
}

static StackElem_t calc_sqrt(StackElem_t value) {
    if (value < 0) {
        printerr(RED_COLOR "Trying to take the root of a negative number\n" RESET_COLOR);
        return -1;
    }

    StackElem_t l = 0, r = value;

    while (r - l > 1) {
        StackElem_t m = (l + r) / 2;

        if (m * m > value)
            r = m;
        else
            l = m;
    }

    return l;
}

StackErr_t StackSqrt(stack_t *stack) {
    stackverify(stack);
    if (code_error != NOTHING)
        return code_error;

    if (stack->size == 2) {
        ParseErr(EMPTY_STACK);
        return EMPTY_STACK;
    }

    StackElem_t a = StackPop(stack);

    StackElem_t sqrt_a = calc_sqrt(a);

    if (sqrt_a < 0) {
        printerr(RED_COLOR "Squarting was not completed" RESET_COLOR);
        return VALUE_ERR;
    }

    StackErr_t error_code = StackPush(stack, sqrt_a);

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

StackErr_t StackPow(stack_t *stack) {
    stackverify(stack);
    if (code_error != NOTHING)
        return code_error;

    if (stack->size < 4) {
        printerr(RED_COLOR "Not enough numbers for exponentiation\n" RESET_COLOR);
        return SIZE_ERR;
    }

    StackElem_t a = StackPop(stack);
    StackElem_t b = StackPop(stack);

    if (b < 0) {
        printerr(RED_COLOR "You can't raising a negative number to a power" RESET_COLOR);
        return VALUE_ERR;
    }

    StackErr_t error_code = StackPush(stack, pow(b, a));

    if (error_code != NOTHING)
        printerr(RED_COLOR "Exponentation was not completed\n" RESET_COLOR);

    return error_code;
}

StackErr_t StackOut(stack_t *stack) {
    stackverify(stack);
    if (code_error)
        return code_error;

    if (stack->size < 3) {
        ParseErr(EMPTY_STACK);
        return EMPTY_STACK;
    }

    int a = StackPop(stack);

    if (a == POIZON_VALUE) {
        printerr(RED_COLOR "Something in Outputing went wrong\n" RESET_COLOR);
        return VALUE_ERR;
    }

    printf("Result: %d\n", a);

    return NOTHING;
}

