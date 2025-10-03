#ifndef __TEXT_UTILS_H_
#define __TEXT_UTILS_H_

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <sys/stat.h>

struct Text {
    const char *name = "";
    int file_size = 0;
    
    char* buf = 0;
    
    int n_commands = 0;

    char** mas_command = 0;
};

int get_file_size(const char* file_name);

void count_strings(Text* text);

void separate_strings(Text* text);

void output_data(Text* text);

void input_data(Text* text);

void read_parse(Text* text);

#endif