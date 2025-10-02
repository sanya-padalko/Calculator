#ifndef _VERIFY_H_
#define _VERIFY_H_

#include <stdio.h>
#include <ctype.h>
#include "vars.h"
#include "errors.h"

typedef int StackElem_t;

struct stack_t {
    StackElem_t* data = NULL;

    unsigned long hash = 0;

    ssize_t size      = 0;
    ssize_t capacity  = 0;
};

const int POIZON_VALUE = 146988;
const int CANARY_LEFT  = 52954; // CEDA
const int CANARY_RIGHT = 65242; // FEDA

const long mod = 998244353;

#define stackdump(name) StackDump(name, VarInfoCtor(#name, __FILE__, __FUNCTION__, __LINE__));

#define stackverify(stack)  StackErr_t code_error = StackVerify(stack); \
                            if (code_error != NOTHING) { \
                                ON_DEBUG(stackdump(stack)); \
                                ParseErr(code_error); \
                            }

const int MaxPrintedCount = 15;

unsigned long calc_hash(stack_t *stack);

StackErr_t StackVerify(stack_t* stack);

void StackDump(stack_t *stack, VarInfo varinfo);

#endif