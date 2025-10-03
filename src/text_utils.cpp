#include "text_utils.h"

int get_file_size(const char* file_name) {
    struct stat file_stat; 
    int stat_result = stat(file_name, &file_stat);

    return file_stat.st_size;
}

void count_strings(Text* text) {
    text->n_commands = 0;
    
    for (int i = 0; i < text->file_size; ++i) {
        if (text->buf[i] == '\n')
            ++text->n_commands;
    }

    return;
}

void separate_strings(Text* text) {
    text->mas_command = (char**)calloc(text->n_commands, sizeof(char*));

    char** start_point = (text->mas_command);

    char* ptr = text->buf;
    *(text->mas_command)++ = ptr;
    
    for (int i = 1; i < text->file_size; ++i) {
        if (*ptr++ == '\n') {
            *(text->mas_command)++ = ptr;
        }
    }

    text->mas_command = start_point;
}

void output_data(Text* text) {
    FILE* fp = fopen(text->name, "wb");

    int write_symbols = fwrite(text->buf, sizeof(char), text->file_size, fp);

    fclose(fp);
}

void input_data(Text* text) {
    FILE* fp = fopen(text->name, "rb");

    int read_symbols = fread(text->buf, sizeof(char), text->file_size, fp);

    fclose(fp);
}

void read_parse(Text* text) {
    text->file_size = get_file_size(text->name);

    text->buf = (char*)calloc(text->file_size, sizeof(char));

    input_data(text);
    
    count_strings(text);

    separate_strings(text);
}