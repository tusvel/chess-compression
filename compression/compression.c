#include <stdbool.h>
#include <stdlib.h>
#include "compression.h"
#include "../check_format/check_format.h"
#include "../open_files/open_files.h"

int height = 1000;
int width = 1000;

// Prototype
FILE* create_output();
void compress_image(int width, int height, RGBTRIPLE (*image)[width], FILE** files);

int compression(char** paths, int quantity)
{
    // Input files.
    FILE** files = open_files(paths, quantity, "r");
    if (files == NULL)
    {
        return 1;
    }

    // Check format input files.
    bool corrected = check_format(files, quantity);
    if (!corrected)
    {
        return 1;
    }

    // Create output file.
    FILE* output = create_output();

    // Skip header in input files.
    for (int i = 0; i < quantity; i++)
    {
        fseek(files[i], sizeof(BITMAPFILEHEADER), SEEK_SET);
        fseek(files[i], sizeof(BITMAPINFOHEADER), SEEK_CUR);
    }

    // Draft for output image 1000 by 1000px;
    RGBTRIPLE (*image)[width] = calloc(height, width * sizeof(RGBTRIPLE));
    compress_image(width, height, image, files);

    for (int i = 0; i < height; i++)
    {
        fwrite(image[i], sizeof(RGBTRIPLE), width, output);
    }

    // Close input files.
    for (int i = 0; i < 2; i++)
    {
        fclose(files[i]);
    }

    // Free memory for image
    free(image);
    free(files);
    
    // Close output file.
    fclose(output);

    return 0;
}


// Create output file.
FILE* create_output()
{
    // Open file with bmp header.
    FILE* header = fopen("header.txt", "r");

    // Create output file.
    FILE* output = fopen("res.bmp", "w");

    BITMAPFILEHEADER bf;
    fread(&bf, sizeof(BITMAPFILEHEADER), 1, header);

    BITMAPINFOHEADER bi;
    fread(&bi, sizeof(BITMAPINFOHEADER), 1, header);

    fwrite(&bf, sizeof(BITMAPFILEHEADER), 1, output);
    fwrite(&bi, sizeof(BITMAPINFOHEADER), 1, output);

    fclose(header);

    return output;
}

// Create compress image.
void compress_image(int width, int height, RGBTRIPLE (*image)[width], FILE** files)
{
    // Fill image with files[0] and files[1] by chess order.
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            if (i % 2 == 0)
            {
                if (j % 2 == 0)
                {
                    fread(&image[i][j], sizeof(RGBTRIPLE), 1, files[0]);
                    fseek(files[1], sizeof(RGBTRIPLE), SEEK_CUR);
                }
                else
                {
                    fread(&image[i][j], sizeof(RGBTRIPLE), 1, files[1]);
                    fseek(files[0], sizeof(RGBTRIPLE), SEEK_CUR);
                }
            }
            else
            {
                if (j % 2 == 0)
                {
                    fread(&image[i][j], sizeof(RGBTRIPLE), 1, files[1]);
                    fseek(files[0], sizeof(RGBTRIPLE), SEEK_CUR);
                }
                else
                {
                    fread(&image[i][j], sizeof(RGBTRIPLE), 1, files[0]);
                    fseek(files[1], sizeof(RGBTRIPLE), SEEK_CUR);
                }
            }
        }
    }
}
