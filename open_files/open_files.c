#include "open_files.h"
#include <stdio.h>
#include <stdlib.h>

FILE** open_files(char** paths, int qunatity, char* mode)
{
    FILE** files = (FILE**)malloc(qunatity * sizeof(FILE*));

    for (int i = 0; i < qunatity; i++)
    {
        // Open file.
        char* path = paths[i];
        FILE* open = fopen(path, mode);
        if (open == NULL)
        {
            printf("Cannot open file %s.\n", path);
            return NULL;
        }
        files[i] = open;
    }

    return files;
}
