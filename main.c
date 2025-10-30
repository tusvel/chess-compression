#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "./compression/compression.h"

typedef struct
{
    char* result_path;
}
decompression_t;

int main(int argc, char* argv[])
{
    // ./main -compress ./1.bmp ./2.bmp
    // ./main -decompress ./res.bmp

    if (argc == 4)
    {
        char** paths = (char**)malloc(2 * sizeof(char*));
        for (int i = 0; i < 2; i++)
        {
            paths[i] = (char*)malloc(strlen(argv[i + 2]) + 1); // +1 for \n
            strcpy(paths[i], argv[i + 2]);
            printf("path: %s\n", paths[i]);
        }
        compression(paths, 2);
    }
    // else if (argc == 3)
    // {
    //     // decompressoin
    //     printf("%s\n", argv[2]);
    //     FILE* src = fopen(argv[2], "r");
    //     FILE* out1 = fopen("./1.out.bmp", "w");
    //     FILE* out2 = fopen("./2.out.bmp", "w");

    //     BITMAPFILEHEADER bf;
    //     BITMAPINFOHEADER bi;

    //     fread(&bf, sizeof(BITMAPFILEHEADER), 1, src);
    //     fread(&bi, sizeof(BITMAPINFOHEADER), 1, src);

    //     fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, out1);
    //     fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, out2);
    //     fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, out1);
    //     fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, out2);

    //     RGBTRIPLE (*image1)[1000] = (RGBTRIPLE (*)[1000])calloc(1000, 1000 * sizeof(RGBTRIPLE));
    //     RGBTRIPLE (*image2)[1000] = (RGBTRIPLE (*)[1000])calloc(1000, 1000 * sizeof(RGBTRIPLE));

    //     for (int i = 0; i < 1000; i++)
    //     {
    //         for (int j = 0; j < 1000; j++)
    //         {
    //             if ((i % 2 == 0) && (j % 2 == 0))
    //             {
    //                 fread(&image1[i][j], sizeof(RGBTRIPLE), 1, src);
    //                 image2[i][j].rgbtBlue = 0x00;
    //                 image2[i][j].rgbtGreen = 0x00;
    //                 image2[i][j].rgbtRed = 0x00;
    //             }
    //             else if ((i % 2 != 0) && (j % 2 != 0))
    //             {
    //                 fread(&image1[i][j], sizeof(RGBTRIPLE), 1, src);
    //                 image2[i][j].rgbtBlue = 0x00;
    //                 image2[i][j].rgbtGreen = 0x00;
    //                 image2[i][j].rgbtRed = 0x00;
    //             }

    //             else if ((i % 2 == 0) && (j % 2 != 0))
    //             {
    //                 fread(&image2[i][j], sizeof(RGBTRIPLE), 1, src);
    //                 image1[i][j].rgbtBlue = 0x00;
    //                 image1[i][j].rgbtGreen = 0x00;
    //                 image1[i][j].rgbtRed = 0x00;
    //             }
    //             else if ((i % 2 != 0) && (j % 2 == 0))
    //             {
    //                 fread(&image2[i][j], sizeof(RGBTRIPLE), 1, src);
    //                 image1[i][j].rgbtBlue = 0x00;
    //                 image1[i][j].rgbtGreen = 0x00;
    //                 image1[i][j].rgbtRed = 0x00;
    //             }
    //         }
    //     }

    //     // fall black pixels
    //     for (int i = 0; i < 1000; i++)
    //     {
    //         for (int j = 0; j < 1000; j++)
    //         {
    //             // black pixels in image1 this
    //             if ((i % 2 == 0) && (j % 2 != 0))
    //             {
    //                 if (i > 0 && i < 999 && j > 0 && j < 999)
    //                 {
    //                     RGBTRIPLE average;
    //                     long blue = 0;
    //                     long green = 0;
    //                     long red = 0;

    //                     blue += image1[i-1][j].rgbtBlue;
    //                     green += image1[i-1][j].rgbtGreen;
    //                     red += image1[i-1][j].rgbtRed;
    //                     //
    //                     blue += image1[i][j+1].rgbtBlue;
    //                     green += image1[i][j+1].rgbtGreen;
    //                     red += image1[i][j+1].rgbtRed;
    //                     //
    //                     blue += image1[i+1][j].rgbtBlue;
    //                     green += image1[i+1][j].rgbtGreen;
    //                     red += image1[i+1][j].rgbtRed;
    //                     //
    //                     blue += image1[i][j-1].rgbtBlue;
    //                     green += image1[i][j-1].rgbtGreen;
    //                     red += image1[i][j-1].rgbtRed;

    //                     average.rgbtBlue = blue / 4;
    //                     average.rgbtGreen = green / 4;
    //                     average.rgbtRed = red / 4;

    //                     image1[i][j] = average;
    //                 }
    //             }
    //             else if ((i % 2 != 0) && (j % 2 == 0))
    //             {
    //                 if (i > 0 && i < 999 && j > 0 && j < 999)
    //                 {
    //                     RGBTRIPLE average;
    //                     long blue = 0;
    //                     long green = 0;
    //                     long red = 0;

    //                     blue += image1[i-1][j].rgbtBlue;
    //                     green += image1[i-1][j].rgbtGreen;
    //                     red += image1[i-1][j].rgbtRed;
    //                     //
    //                     blue += image1[i][j+1].rgbtBlue;
    //                     green += image1[i][j+1].rgbtGreen;
    //                     red += image1[i][j+1].rgbtRed;
    //                     //
    //                     blue += image1[i+1][j].rgbtBlue;
    //                     green += image1[i+1][j].rgbtGreen;
    //                     red += image1[i+1][j].rgbtRed;
    //                     //
    //                     blue += image1[i][j-1].rgbtBlue;
    //                     green += image1[i][j-1].rgbtGreen;
    //                     red += image1[i][j-1].rgbtRed;

    //                     average.rgbtBlue = blue / 4;
    //                     average.rgbtGreen = green / 4;
    //                     average.rgbtRed = red / 4;

    //                     image1[i][j] = average;
    //                 }
    //             }

    //             // black pixels in image2 this
    //             if ((i % 2 == 0) && (j % 2 == 0))
    //             {

    //             }
    //             else if ((i % 2 != 0) && (j % 2 != 0))
    //             {

    //             }
    //         }
    //     }

    //     for (int i = 0; i < 1000; i++)
    //     {
    //         fwrite(image1[i], sizeof(RGBTRIPLE), 1000, out1);
    //         fwrite(image2[i], sizeof(RGBTRIPLE), 1000, out2);
    //     }

    //     fclose(src);
    //     fclose(out1);
    //     fclose(out2);
    // }
    else {
        printf("Usage: ./main [-c|-d] [./first.bmp ./second.bmp|./result.bmp]\n");
        return 1;
    }

    return 0;
}

