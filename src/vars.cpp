#include "vars.h"

VarInfo VarInfoCtor(const char* stack_name, const char* file_name, const char* func_name, const int line_ind) {
    VarInfo varinfo = {};

    varinfo.stack_name = stack_name;
    varinfo.file_name  = file_name;
    varinfo.func_name  = func_name;
    varinfo.line_ind   = line_ind;

    return varinfo;
}

void PrintVarInfo(const VarInfo varinfo) {
    printf("\"%s\" %s, %s: %d\n", varinfo.stack_name, varinfo.file_name, varinfo.func_name, varinfo.line_ind);
}