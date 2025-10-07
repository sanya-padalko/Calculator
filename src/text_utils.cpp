#include "text_utils.h"

Text* TextCtor(const char* file_name) {
    Text* text = (Text*)calloc(1, sizeof(Text));

    if (text == NULL) {
        printerr(RED_COLOR "Calloc return NULL in TextCtor\n" RESET_COLOR);
        return NULL;
    }

    text->name = file_name;
    
    input_data(text);

    return text;
}

int get_file_size(const char* file_name) {
    struct stat file_stat; 
    int stat_result = stat(file_name, &file_stat);

    return file_stat.st_size;
}

void input_data(Text* text) {
    text->file_size = get_file_size(text->name);

    text->buf = (char*)calloc(text->file_size, sizeof(char));

    FILE* fp = fopen(text->name, "rb");

    int read_symbols = fread(text->buf, sizeof(char), text->file_size, fp);

    fclose(fp);
}