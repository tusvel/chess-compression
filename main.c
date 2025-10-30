#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./compression/compression.h"
#include "./decompression/decompression.h"

int main(int argc, char* argv[])
{
    if (argc == 4)
    {
        char** paths = (char**)malloc(2 * sizeof(char*));
        for (int i = 0; i < 2; i++)
        {
            paths[i] = (char*)malloc(strlen(argv[i + 2]) + 1); // +1 for \n
            strcpy(paths[i], argv[i + 2]);
        }
        return compression(paths, 2);
    }
    else if (argc == 3)
    {
        return decompression(argv[2]);
    }
    else {
        printf("Usage: ./main -c 1.bmp 2.bmp\n");
        printf("Usage: ./main -d compressed.bmp\n");
        return 1;
    }

    return 0;
}
