#include "fill_header.h"
#include "../bmp.h"

void fill_header(FILE* file)
{
    // Open file with bmp header.
    FILE* header = fopen("header.txt", "r");

    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, header);

    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, header);

    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, file);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, file);

    fclose(header);
}