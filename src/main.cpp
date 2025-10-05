#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <strings.h>

#include "errors.h"
#include "stack.h"
#include "vars.h"
#include "calculator.h"
#include "assembler.h"
#include "processor.h"

int main() {
    const char* start = "../start.txt";
    const char* ex_file = "../ex_file.txt";
    
    StackErr_t error_code = assembler(start, ex_file);

    if (error_code != NOTHING) {
        return 0;
    }

    execution(ex_file);
}