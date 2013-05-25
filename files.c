#include <stdio.h> 
#include "files.h"

bool file_exist (const char *filename)
{
    FILE *tmp=fopen(filename, "r");
    if (tmp==NULL)
        return false;
    fclose (tmp);
    return true;
};
