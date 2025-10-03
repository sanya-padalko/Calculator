#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <strings.h>

#include "errors.h"
#include "stack.h"
#include "vars.h"
#include "calculator.h"
#include "assembler.h"
#include "executor.h"

int main() {
    const char* start = "../start.txt";
    const char* ex_file = "../ex_file.txt";
    
    assembler(start, ex_file);

    execution(ex_file);
}