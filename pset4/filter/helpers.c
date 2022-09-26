#include "helpers.h"
#include <math.h>
#include <stdlib.h>
#include <stdio.h>

/*
TLC = top left corner, TRC = top right corner, BLC = bottom left corner, BRC = bottom right corner,
TE = top edge, BE = bottom edge, LE = left edge, RE = right edge
all of these functions find the new averages given the pixel location on the image
*/
// blur helper functions:
RGBTRIPLE colors_avgs(int i, int j, int height, int width, RGBTRIPLE image[height][width]);
void find_min_max(int *min_x, int *max_x, int *min_y, int *max_y, int i, int j, int height, int width);

// edges helper functions:
void calc_gx(RGBTRIPLE new_matrix[3][3], int gx[3][3], int *curr_blue, int *curr_green, int *curr_red);
void calc_gy(RGBTRIPLE new_matrix[3][3], int gy[3][3], int *curr_blue, int *curr_green, int *curr_red);
void calc_matrix(int height, int width, int i, int j, RGBTRIPLE image[height][width], RGBTRIPLE new_matrix[3][3]);
void initialize_gx_gy(int gx[3][3], int gy[3][3]);
void find_min_max2(int *min_x, int *max_x, int *min_y, int *max_y, int i, int j, int height, int width, int *start_x, int *start_y);
void check_if_above_255(float *b, float *g, float *r);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE average = 0;
    double calc = 0.0;
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j++)
        {
            calc = (image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.0;
            average = (BYTE) round(calc);
            image[i][j].rgbtBlue = average;
            image[i][j].rgbtGreen = average;
            image[i][j].rgbtRed = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE temp = {0, 0, 0};
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            // iterate up to width / 2 so we don't reswap what we have already swapped!
            temp = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp;
        }
    }
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // before, we were blurring each pixel and then using new blurred values to calculate the rgb values for every other pixel!
    // we need to make a new array of the image and manipulate it instead while leaving the og image completely untampered until we are completely done with our calculations
    // NOTE: we have been staring at this for 3 days with no progress. In two minutes of understanding the bug, we passed all tests.
    // The issue was not arithmetic or syntactical. It was a logical error. We were blurring the pixels of the orginal image as we went
    // through it. That is why we only passed the corner pixel test every single time because the top left corner is the first pixel we blur
    // and is therefore the only pixel we used original rgb values for. Once the top left corner was blurred, we then used those blurred average
    // numbers to calculate the rest of the pixels averages. We needed to use the og rgb values the entire time though.
    RGBTRIPLE new_image[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            new_image[i][j] = colors_avgs(i, j, height, width, image);
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = new_image[i][j];
        }
    }
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE new_image[height][width];
    int gx_matrix[3][3];
    int gy_matrix[3][3];
    RGBTRIPLE new_matrix[3][3];
    initialize_gx_gy(gx_matrix, gy_matrix);
    int curr_blue_x = 0, curr_green_x = 0, curr_red_x = 0, curr_blue_y = 0, curr_green_y = 0, curr_red_y = 0;
    float result_blue = 0, result_green = 0, result_red = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            calc_matrix(height, width, i, j, image, new_matrix);
            curr_blue_x = 0, curr_green_x = 0, curr_red_x = 0, curr_blue_y = 0, curr_green_y = 0, curr_red_y = 0;
            calc_gx(new_matrix, gx_matrix, &curr_blue_x, &curr_green_x, &curr_red_x);
            calc_gy(new_matrix, gy_matrix, &curr_blue_y, &curr_green_y, &curr_red_y);
            result_blue = round(sqrt((float)(pow(curr_blue_x, 2) + pow(curr_blue_y, 2))));
            result_green = round(sqrt((float)(pow(curr_green_x, 2) + pow(curr_green_y, 2))));
            result_red = round(sqrt((float)(pow(curr_red_x, 2) + pow(curr_red_y, 2))));
            check_if_above_255(&result_blue, &result_green, &result_red);
            new_image[i][j].rgbtBlue = (int) result_blue;
            new_image[i][j].rgbtGreen = (int) result_green;
            new_image[i][j].rgbtRed = (int) result_red;
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j] = new_image[i][j];
        }
    }
    return;
}

void calc_matrix(int height, int width, int i, int j, RGBTRIPLE image[height][width], RGBTRIPLE new_matrix[3][3])
{
    int start_x = 0, start_y = 0;
    for (int a = 0; a < 3; a++)
    {
        for (int b = 0; b < 3; b++)
        {
            new_matrix[a][b].rgbtBlue = 0, new_matrix[a][b].rgbtGreen = 0, new_matrix[a][b].rgbtRed = 0;
        }
    }
    int min_x = j - 1, max_x = j + 1, min_y = i - 1, max_y = i + 1;
    find_min_max2(&min_x, &max_x, &min_y, &max_y, i, j, height, width, &start_x, &start_y);
    for (int m = start_y, k = min_y; k <= max_y; m++, k++)
    {
        for (int n = start_x, l = min_x; l <= max_x; n++, l++)
        {
            new_matrix[m][n] = image[k][l];
        }
    }
}

void calc_gx(RGBTRIPLE new_matrix[3][3], int gx[3][3], int *curr_blue_x, int *curr_green_x, int *curr_red_x)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            *curr_blue_x += (((int) new_matrix[i][j].rgbtBlue) * gx[i][j]);
            *curr_green_x += (((int) new_matrix[i][j].rgbtGreen) * gx[i][j]);
            *curr_red_x += (((int) new_matrix[i][j].rgbtRed) * gx[i][j]);
        }
    }
}

void calc_gy(RGBTRIPLE new_matrix[3][3], int gy[3][3], int *curr_blue_y, int *curr_green_y, int *curr_red_y)
{
    for (int i = 0; i < 3; i++)
    {
        for (int j = 0; j < 3; j++)
        {
            *curr_blue_y += (((int) new_matrix[i][j].rgbtBlue) * gy[i][j]);
            *curr_green_y += (((int) new_matrix[i][j].rgbtGreen) * gy[i][j]);
            *curr_red_y += (((int) new_matrix[i][j].rgbtRed) * gy[i][j]);
        }
    }
}

void initialize_gx_gy(int gx[3][3], int gy[3][3])
{
    gx[0][0] = -1, gx[0][1] = 0, gx[0][2] = 1;
    gx[1][0] = -2, gx[1][1] = 0, gx[1][2] = 2;
    gx[2][0] = -1, gx[2][1] = 0, gx[2][2] = 1;
    gy[0][0] = -1, gy[0][1] = -2, gy[0][2] = -1;
    gy[1][0] = 0, gy[1][1] = 0, gy[1][2] = 0;
    gy[2][0] = 1, gy[2][1] = 2, gy[2][2] = 1;
}

void check_if_above_255(float *b, float *g, float *r)
{
    if (*b > 255)
    {
        *b = 255;
    }
    if (*g > 255)
    {
        *g = 255;
    }
    if (*r > 255)
    {
        *r = 255;
    }
}

RGBTRIPLE colors_avgs(int i, int j, int height, int width, RGBTRIPLE image[height][width])
{
    int min_x = j - 1, max_x = j + 1, min_y = i - 1, max_y = i + 1;
    find_min_max(&min_x, &max_x, &min_y, &max_y, i, j, height, width);
    RGBTRIPLE curr_pixel;
    int sum_blue = 0, sum_green = 0, sum_red = 0;
    float divisor = 0.0;
    for (int k = min_y; k <= max_y; k++)
    {
        for (int l = min_x; l <= max_x; l++)
        {
            curr_pixel = image[k][l];
            sum_blue += (int) curr_pixel.rgbtBlue;
            sum_green += (int) curr_pixel.rgbtGreen;
            sum_red += (int) curr_pixel.rgbtRed;
            divisor++;
        }
    }
    sum_blue = round((float) sum_blue / divisor);
    sum_green = round((float) sum_green / divisor);
    sum_red = round((float) sum_red / divisor);
    curr_pixel.rgbtBlue = sum_blue, curr_pixel.rgbtGreen = sum_green, curr_pixel.rgbtRed = sum_red;
    return curr_pixel;
}

// we want to start at the higher boundary and end at the lower boundary when at the edges of the image
void find_min_max(int *min_x, int *max_x, int *min_y, int *max_y, int i, int j, int height, int width)
{
    if (i == 0)
        // iterator of rows
    {
        *min_y = *min_y + 1;
    }
    else if (i == height - 1)
    {
        *max_y = height - 1;
    }
    if (j == 0)
        // iterator of pixels within rows
    {
        *min_x = *min_x + 1;
    }
    else if (j == width - 1)
    {
        *max_x = width - 1;
    }
}

void find_min_max2(int *min_x, int *max_x, int *min_y, int *max_y, int i, int j, int height, int width, int *start_x, int *start_y)
{
    if (i == 0)
    {
        *min_y = *min_y + 1;
        *start_y = 1;
    }
    else if (i == (height - 1))
    {
        *max_y = height - 1;
    }
    if (j == 0)
    {
        *min_x = *min_x + 1;
        *start_x = 1;
    }
    else if (j == (width - 1))
    {
        *max_x = width - 1;
    }
}