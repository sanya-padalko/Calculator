#ifndef _VARS_H_
#define _VARS_H_

#include <stdio.h>

struct VarInfo {
    const char* object_name = "";

    const char* file_name  = "";
    
    const char* func_name  = "";

    int line_ind     =  0;
};

VarInfo VarInfoCtor(const char* object_name, const char* file_name, const char* func_name, const int line_ind);

void PrintVarInfo(const VarInfo varinfo);

#endif