#include "check_format.h"

bool check_format(FILE** files, int quantity)
{
    for (int i = 0; i < quantity; i++)
    {
        // Read header and check format with check_format.
        BITMAPFILEHEADER bf;
        fread(&bf, sizeof(BITMAPFILEHEADER), 1, files[i]);

        BITMAPINFOHEADER bi;
        fread(&bi, sizeof(BITMAPINFOHEADER), 1, files[i]);

        // Ensure infile is (likely) a 24-bit uncompressed BMP 4.0
        if (bf.bfType != 0x4d42 || bf.bfOffBits != 54 || bi.biSize != 40 ||
            bi.biBitCount != 24 || bi.biCompression != 0)
        {
            printf("Unsupported file format.\n");
            return false;
        }
    }

    return true;
}
