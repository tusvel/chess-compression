#include <stdbool.h>
#include <string.h>
#include <stdlib.h>
#include "decompression.h"
#include "../check_format/check_format.h"
#include "../open_files/open_files.h"
#include "../fill_header/fill_header.h"

#define WIDTH 1000
#define HEIGHT 1000

// Prototype
RGBTRIPLE interpolation(RGBTRIPLE (*image)[WIDTH], int y_pos, int x_pos);

int decompression(char* path)
{
    // Open compresed file.
    FILE* compressed = fopen(path, "r");

    // Check format.
    bool corrected = check_format(compressed);
    if (!corrected)
    {
        return 1;
    }

    // Path for output files.
    char** output_paths = (char**)malloc(2 * sizeof(char*));
    for (int i = 0; i < 2; i++)
    {
        output_paths[i] = (char*)malloc(10 * sizeof(char));
    }
    strcpy(output_paths[0], "1.out.bmp");
    strcpy(output_paths[1], "2.out.bmp");

    // Create output_files and fill header.
    FILE** output_files = open_files(output_paths, 2, "w");
    for (int i = 0; i < 2; i++)
    {
        fill_header(output_files[i]);
    }

    // Draft image for two output files.
    // RGBTRIPLE (*images)[2][WIDTH] = (RGBTRIPLE (*)[2][WIDTH])calloc(2, HEIGHT * WIDTH * sizeof(RGBTRIPLE));
    RGBTRIPLE (*image1)[WIDTH] = (RGBTRIPLE (*)[WIDTH])calloc(HEIGHT, WIDTH * sizeof(RGBTRIPLE));
    RGBTRIPLE (*image2)[WIDTH] = (RGBTRIPLE (*)[WIDTH])calloc(HEIGHT, WIDTH * sizeof(RGBTRIPLE));

    // Fill images by original pixels.
    for (int i = 0; i < HEIGHT; i++)
    {
        for (int j = 0; j < WIDTH; j++)
        {
            if ((i % 2 == 0) && (j % 2 == 0))
            {
                fread(&image1[i][j], sizeof(RGBTRIPLE), 1, compressed);
                image2[i][j].rgbtBlue = 0x00;
                image2[i][j].rgbtGreen = 0x00;
                image2[i][j].rgbtRed = 0x00;
            }
            else if ((i % 2 != 0) && (j % 2 != 0))
            {
                fread(&image1[i][j], sizeof(RGBTRIPLE), 1, compressed);
                image2[i][j].rgbtBlue = 0x00;
                image2[i][j].rgbtGreen = 0x00;
                image2[i][j].rgbtRed = 0x00;
            }

            else if ((i % 2 == 0) && (j % 2 != 0))
            {
                fread(&image2[i][j], sizeof(RGBTRIPLE), 1, compressed);
                image1[i][j].rgbtBlue = 0x00;
                image1[i][j].rgbtGreen = 0x00;
                image1[i][j].rgbtRed = 0x00;
            }
            else if ((i % 2 != 0) && (j % 2 == 0))
            {
                fread(&image2[i][j], sizeof(RGBTRIPLE), 1, compressed);
                image1[i][j].rgbtBlue = 0x00;
                image1[i][j].rgbtGreen = 0x00;
                image1[i][j].rgbtRed = 0x00;
            }
        }
    }

    // Fill black pixels with interpolation.
    for (int i = 0; i < 1000; i++)
    {
        for (int j = 0; j < 1000; j++)
        {
            // black pixels in images[0] this
            if ((i % 2 == 0) && (j % 2 != 0))
            {
                image1[i][j] = interpolation(image1, i, j);
            }
            else if ((i % 2 != 0) && (j % 2 == 0))
            {
                image1[i][j] = interpolation(image1, i, j);
            }

            // black pixels in image2 this
            if ((i % 2 == 0) && (j % 2 == 0))
            {
                image2[i][j] = interpolation(image2, i, j);
            }
            else if ((i % 2 != 0) && (j % 2 != 0))
            {
                image2[i][j] = interpolation(image2, i, j);
            }
        }
    }

    for (int i = 0; i < HEIGHT; i++)
    {
        fwrite(image1[i], sizeof(RGBTRIPLE), WIDTH, output_files[0]);
        fwrite(image2[i], sizeof(RGBTRIPLE), WIDTH, output_files[1]);
    }

    fclose(compressed);
    for (int i = 0; i < 2; i++)
    {
        fclose(output_files[i]);
    }
    free(output_paths);
    free(image1);
    free(image2);

    return 0;
}

RGBTRIPLE interpolation(RGBTRIPLE (*image)[WIDTH], int y_pos, int x_pos)
{
    RGBTRIPLE average;
    long blue = 0;
    long green = 0;
    long red = 0;

    if (y_pos == 0 && x_pos == 0)
    {
        // take 2 pixels
        blue += image[y_pos][x_pos + 1].rgbtBlue;
        green += image[y_pos][x_pos + 1].rgbtGreen;
        red += image[y_pos][x_pos + 1].rgbtRed;

        blue += image[y_pos + 1][x_pos].rgbtBlue;
        green += image[y_pos + 1][x_pos].rgbtGreen;
        red += image[y_pos + 1][x_pos].rgbtRed;

        average.rgbtBlue = blue / 2;
        average.rgbtGreen = green / 2;
        average.rgbtRed = red / 2;
    }
    else if (y_pos == 0 && x_pos == WIDTH - 1)
    {
        // take 2 pixels
        blue += image[y_pos][x_pos - 1].rgbtBlue;
        green += image[y_pos][x_pos - 1].rgbtGreen;
        red += image[y_pos][x_pos - 1].rgbtRed;

        blue += image[y_pos + 1][x_pos].rgbtBlue;
        green += image[y_pos + 1][x_pos].rgbtGreen;
        red += image[y_pos + 1][x_pos].rgbtRed;

        average.rgbtBlue = blue / 2;
        average.rgbtGreen = green / 2;
        average.rgbtRed = red / 2;
    }
    else if (y_pos == HEIGHT - 1 && x_pos == 0)
    {
        // take 2 pixels
        blue += image[y_pos - 1][x_pos].rgbtBlue;
        green += image[y_pos - 1][x_pos].rgbtGreen;
        red += image[y_pos - 1][x_pos].rgbtRed;

        blue += image[y_pos][x_pos + 1].rgbtBlue;
        green += image[y_pos][x_pos + 1].rgbtGreen;
        red += image[y_pos][x_pos + 1].rgbtRed;

        average.rgbtBlue = blue / 2;
        average.rgbtGreen = green / 2;
        average.rgbtRed = red / 2;
    }
    else if (y_pos == HEIGHT - 1 && x_pos == WIDTH - 1)
    {
        // take 2 pixels
        blue += image[y_pos - 1][x_pos].rgbtBlue;
        green += image[y_pos - 1][x_pos].rgbtGreen;
        red += image[y_pos - 1][x_pos].rgbtRed;

        blue += image[y_pos][x_pos - 1].rgbtBlue;
        green += image[y_pos][x_pos - 1].rgbtGreen;
        red += image[y_pos][x_pos - 1].rgbtRed;

        average.rgbtBlue = blue / 2;
        average.rgbtGreen = green / 2;
        average.rgbtRed = red / 2;
    }
    else if (y_pos == 0)
    {
        // take 3 pixels
        blue += image[y_pos][x_pos - 1].rgbtBlue;
        green += image[y_pos][x_pos - 1].rgbtGreen;
        red += image[y_pos][x_pos - 1].rgbtRed;

        blue += image[y_pos][x_pos + 1].rgbtBlue;
        green += image[y_pos][x_pos + 1].rgbtGreen;
        red += image[y_pos][x_pos + 1].rgbtRed;

        blue += image[y_pos + 1][x_pos].rgbtBlue;
        green += image[y_pos + 1][x_pos].rgbtGreen;
        red += image[y_pos + 1][x_pos].rgbtRed;

        average.rgbtBlue = blue / 3;
        average.rgbtGreen = green / 3;
        average.rgbtRed = red / 3;
    }
    else if (y_pos == HEIGHT - 1)
    {
        // take 3 pixels
        blue += image[y_pos][x_pos - 1].rgbtBlue;
        green += image[y_pos][x_pos - 1].rgbtGreen;
        red += image[y_pos][x_pos - 1].rgbtRed;

        blue += image[y_pos][x_pos + 1].rgbtBlue;
        green += image[y_pos][x_pos + 1].rgbtGreen;
        red += image[y_pos][x_pos + 1].rgbtRed;

        blue += image[y_pos - 1][x_pos].rgbtBlue;
        green += image[y_pos - 1][x_pos].rgbtGreen;
        red += image[y_pos - 1][x_pos].rgbtRed;

        average.rgbtBlue = blue / 3;
        average.rgbtGreen = green / 3;
        average.rgbtRed = red / 3;
    }
    else if (x_pos == 0)
    {
        // take 3 pixels
        blue += image[y_pos - 1][x_pos].rgbtBlue;
        green += image[y_pos - 1][x_pos].rgbtGreen;
        red += image[y_pos - 1][x_pos].rgbtRed;

        blue += image[y_pos + 1][x_pos].rgbtBlue;
        green += image[y_pos + 1][x_pos].rgbtGreen;
        red += image[y_pos + 1][x_pos].rgbtRed;

        blue += image[y_pos][x_pos + 1].rgbtBlue;
        green += image[y_pos][x_pos + 1].rgbtGreen;
        red += image[y_pos][x_pos + 1].rgbtRed;

        average.rgbtBlue = blue / 3;
        average.rgbtGreen = green / 3;
        average.rgbtRed = red / 3;
    }
    else if (x_pos == WIDTH - 1)
    {
        // take 3 pixels
        blue += image[y_pos - 1][x_pos].rgbtBlue;
        green += image[y_pos - 1][x_pos].rgbtGreen;
        red += image[y_pos - 1][x_pos].rgbtRed;

        blue += image[y_pos + 1][x_pos].rgbtBlue;
        green += image[y_pos + 1][x_pos].rgbtGreen;
        red += image[y_pos + 1][x_pos].rgbtRed;

        blue += image[y_pos][x_pos - 1].rgbtBlue;
        green += image[y_pos][x_pos - 1].rgbtGreen;
        red += image[y_pos][x_pos - 1].rgbtRed;

        average.rgbtBlue = blue / 3;
        average.rgbtGreen = green / 3;
        average.rgbtRed = red / 3;
    }
    // If its not bordered value
    else
    {
        // take 4 pixels
        blue += image[y_pos - 1][x_pos].rgbtBlue;
        green += image[y_pos - 1][x_pos].rgbtGreen;
        red += image[y_pos - 1][x_pos].rgbtRed;
        //
        blue += image[y_pos][x_pos + 1].rgbtBlue;
        green += image[y_pos][x_pos + 1].rgbtGreen;
        red += image[y_pos][x_pos + 1].rgbtRed;
        //
        blue += image[y_pos + 1][x_pos].rgbtBlue;
        green += image[y_pos + 1][x_pos].rgbtGreen;
        red += image[y_pos + 1][x_pos].rgbtRed;
        //
        blue += image[y_pos][x_pos - 1].rgbtBlue;
        green += image[y_pos][x_pos - 1].rgbtGreen;
        red += image[y_pos][x_pos - 1].rgbtRed;

        average.rgbtBlue = blue / 4;
        average.rgbtGreen = green / 4;
        average.rgbtRed = red / 4;
    }

    return average;
}
