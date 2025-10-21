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
    const char* start = "../factorial.txt";
    const char* ex_file = "../ex_file.txt";
    const char* list_file = "../listing.txt";
    
    assembler_t assem = {};
    assem.text_file = start;
    assem.commands_file = ex_file;
    assem.listing_file = list_file;

    CodeError_t error_code = assembler(&assem);

    if (error_code != NOTHING)
        printerr("(((\n");

    processor_t *proc = make_processor(ex_file);
    my_assert(proc, NULLPTR, 0);

    LoadFile(proc);

    execution(proc);
}