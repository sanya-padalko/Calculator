#ifndef __ASSEMBLER_H_
#define __ASSEMBLER_H_

#include <strings.h>

#include "stack.h"

StackErr_t assembler(const char* text_file, const char* commands_file);

StackErr_t disassembler(const char* commands_file, const char* text_file);

#endif