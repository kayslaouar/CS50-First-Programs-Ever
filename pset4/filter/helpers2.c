#include "helpers.h"
# include <math.h>

/*
TLC = top left corner, TRC = top right corner, BLC = bottom left corner, BRC = bottom right corner,
TE = top edge, BE = bottom edge, LE = left edge, RE = right edge
all of these functions find the new averages given the pixel location on the image
*/

BYTE find_blue_avg(int i, int j, RGBTRIPLE image[height][width]);
BYTE find_green_avg(int i, int j, RGBTRIPLE image[height][width]);
BYTE find_red_avg(int i, int j, RGBTRIPLE image[height][width]);
RGBTRIPLE TLC(int i, int j, RGBTRIPLE image[height][width], RGBTRIPLE new_avgs);
RGBTRIPLE TRC(int i, int j, RGBTRIPLE image[height][width], RGBTRIPLE new_avgs);
RGBTRIPLE BLC(int i, int j, RGBTRIPLE image[height][width], RGBTRIPLE new_avgs);
RGBTRIPLE BRC(int i, int j, RGBTRIPLE image[height][width], RGBTRIPLE new_avgs);
RGBTRIPLE blur_edges(int i, int j, RGBTRIPLE image[height][width], RGBTRIPLE new_avgs);


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
    RGBTRIPLE new_avgs = {0, 0, 0};
    for (int i = 0; i < height; i ++)
    {
        for (int j = 0; j < width; j++)
        {
            if ((i != 0 || i != height - 1) && (j != 0 || j != width - 1))
            {
                image[i][j].rgbtBlue = find_blue_avg(i, j, image[i][j], *new_avgs);
                image[i][j].rgbtGreen = find_green_avg(i, j, image[i][j], *new_avgs);
                image[i][j].rgbtRed = find_red_avg(i, j, image[i][j], *new_avgs);
            }
            else if (i == 0 && j == 0)
            {
                TLC(i, j, image[i][j], *new_avgs)
            }
            else if (i == 0 && j == 0)
            {
                TRC(i, j, image[i][j], *new_avgs)
            }
            else if (i == 0 && j == 0)
            {
                BLC(i, j, image[i][j], *new_avgs)
            }
            else if (i == 0 && j == 0)
            {
                BRC(i, j, image[i][j], *new_avgs)
            }
            else
            {
                image[i][j] = blur_edges(i, j, image[i][j], *new_avgs);
            }
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}

BYTE find_blue_avg(int i, int j, RGBTRIPLE image[height][width])
{
    BYTE blue_avg = 0;
    double calc_blue = 0.0;
    calc_blue = (image[i][j].rgbtBlue + image[i][j - 1].rgbtBlue + image[i][j + 1].rgbtBlue + image[i + 1][j].rgbtBlue + image[i - 1][j].rgbtBlue + image[i + 1][j + 1].rgbtBlue + image[i - 1][j + 1].rgbtBlue + image[i - 1].rgbtBlue[j - 1].rgbtBlue + image[i + 1][j - 1].rgbtBlue;
    blue_avg = (BYTE) round(calc_blue);
    return blue_avg;
}

BYTE find_green_avg(int i, int j, RGBTRIPLE image[height][width])
{
    BYTE green_avg = 0;
    double calc_green = 0.0;
    calc_green = (image[i][j].rgbtGreen + image[i][j - 1].rgbtGreen + image[i][j + 1].rgbtGreen + image[i + 1][j].rgbtGreen + image[i - 1][j].rgbtGreen + image[i + 1][j + 1].rgbtGreen + image[i - 1][j + 1].rgbtGreen + image[i - 1].rgbtGreen[j - 1].rgbtGreen + image[i + 1][j - 1].rgbtGreen;
    blue_avg = (BYTE) round(calc_blue);
    return green_avg;
}

BYTE find_red_avg(int i, int j, RGBTRIPLE image[height][width])
{
    BYTE red_avg = 0;
    double calc_red = 0.0;
    calc_red = (image[i][j].rgbtRed + image[i][j - 1].rgbtRed + image[i][j + 1].rgbtRed + image[i + 1][j].rgbtRed + image[i - 1][j].rgbtRed + image[i + 1][j + 1].rgbtRed + image[i - 1][j + 1].rgbtRed + image[i - 1].rgbtRed[j - 1].rgbtRed + image[i + 1][j - 1].rgbtRed;
    blue_avg = (BYTE) round(calc_blue);
    return red_avg;
}

RGBTRIPLE blur_edges(int i, int j, RGBTRIPLE image[height][width], RGBTRIPLE new_avgs)
{
    if (i == 0)
    {
        image[i][j] = TE(i, j, image[i][j], *new_avgs);
    }
    else if (i == height - 1)
    {
        image[i][j] = BE(i, j, image[i][j], *new_avgs);
    }
    else if (j == 0)
    {
        image[i][j] = LE(i, j, image[i][j], *new_avgs);
    }
    else if (j == width - 1)
    {
        image[i][j] = RE(i, j, image[i][j], *new_avgs);
    }
}