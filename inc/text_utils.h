#ifndef __TEXT_UTILS_H_
#define __TEXT_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>
#include "errors.h"

struct Text {
    const char *name = "";
    int file_size = 0;
    
    char* buf = 0;
};

#define make_text(file_name) TextCtor(file_name)

Text* TextCtor(const char* file_name);

int get_file_size(const char* file_name);

void input_data(Text* text);

#endif