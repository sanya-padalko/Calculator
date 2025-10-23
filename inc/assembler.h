#ifndef __ASSEMBLER_H_
#define __ASSEMBLER_H_

#include <strings.h>

#include "stack.h"
#include "calculator.h"
#include "processor.h"

const int NumberCnt = 10;
const int AlphaCnt = 26;
const int FirstPass = 1;
const int SecondPass = 2;

struct label_t {
    int ic = -1;

    label_t* nxt[AlphaCnt + NumberCnt] = {NULL};
};

struct assembler_t {
    const char* text_file = NULL;
    const char* commands_file = NULL;

    const char* listing_file = NULL;
    FILE* listing = NULL;

    Text* program = NULL;
    char* buf = NULL;
    char* ex_ptr = NULL;

    label_t* label = NULL;

    int ic = 0;
};

CodeError_t assembler(assembler_t* assem);

#endif