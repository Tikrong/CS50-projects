#include "helpers.h"
#include <math.h>

typedef struct
{
    long rgbtBlue;
    long rgbtGreen;
    long rgbtRed;
}
RGBTRIPLE2;

RGBTRIPLE2 calc_gx(int y, int x, int height, int width, RGBTRIPLE image[y][x]);
RGBTRIPLE2 calc_gy(int y, int x, int height, int width, RGBTRIPLE image[y][x]);
RGBTRIPLE calc_ch(RGBTRIPLE2 gx, RGBTRIPLE2 gy);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE tmp = 0;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            tmp = round((image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed) / 3.000);
            image[i][j].rgbtBlue = tmp;
            image[i][j].rgbtGreen = tmp;
            image[i][j].rgbtRed = tmp;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    BYTE tmp;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0, n = width / 2; j < n; j++)
        {
            //swapping pixels
            tmp = image[i][j].rgbtRed;
            image[i][j].rgbtRed = image[i][width - j - 1].rgbtRed;
            image[i][width - j - 1].rgbtRed = tmp;

            tmp = image[i][j].rgbtGreen;
            image[i][j].rgbtGreen = image[i][width - j - 1].rgbtGreen;
            image[i][width - j - 1].rgbtGreen = tmp;

            tmp = image[i][j].rgbtBlue;
            image[i][j].rgbtBlue = image[i][width - j - 1].rgbtBlue;
            image[i][width - j - 1].rgbtBlue = tmp;

        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
        int tmp_r = 0;
    int tmp_gr = 0;
    int tmp_bl = 0;
    int counter = 0;

    RGBTRIPLE image2[height][width]; //creating temp array

for (int y = 0; y < height; y++)
{
    for (int x = 0; x < width; x++)
    {
        for (int i = -1; i < 2; i++)
        {
            if ((y + i >= 0) && (y + i <= height - 1))
            {
                for (int j = -1; j < 2; j++)
                {
                    if ((x + j >= 0) && (x + j <= width - 1))
                    {
                        tmp_r += image[y + i][x + j].rgbtRed;
                        tmp_gr += image[y + i][x + j].rgbtGreen;
                        tmp_bl += image[y + i][x + j].rgbtBlue;
                        counter += 1;
                    }
                }
            }

        }
            image2[y][x].rgbtRed = round(tmp_r / (float)counter); // getting new data for each pixel
            image2[y][x].rgbtGreen = round(tmp_gr / (float)counter);
            image2[y][x].rgbtBlue = round(tmp_bl / (float)counter);
            counter = 0;
            tmp_r = 0;
            tmp_bl = 0;
            tmp_gr = 0;
    }
}

    for (int t = 0; t < height; t++) // copying our temp array to original one
    {
        for (int s = 0; s < width; s++)
        {
            image[t][s].rgbtRed = image2[t][s].rgbtRed;
            image[t][s].rgbtGreen = image2[t][s].rgbtGreen;
            image[t][s].rgbtBlue = image2[t][s].rgbtBlue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE image2[height][width]; //declaring temp array

    for (int j = 0; j < height; j++) //scrolling through all the lines
    {
        for (int i = 0; i < width; i++) // scrolling through all the columns
        {
            RGBTRIPLE2 gx = calc_gx(j, i, height, width, image); // function for calculaating Gx
            RGBTRIPLE2 gy = calc_gy(j, i, height, width, image); // function for calculation Gy
            image2[j][i] = calc_ch(gx, gy); // function for combining the result

        }
    }

    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            image[j][i] = image2[j][i];
        }
    }

    return;
}

RGBTRIPLE2 calc_gx(int y, int x, int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE2 tmp; // temp for weighted sum
    tmp.rgbtRed = 0;
    tmp.rgbtGreen = 0;
    tmp.rgbtBlue = 0;

    int sign = 1; // for changing argument for weightet sum (-1, -2, -1 then 1, 2, 1)
    for (int n = -1; n < 2; n += 2) //scrolling through left and right columns
    {
        for (int f = -1, k = n; f < 2; f++) //scrolling through each lines. k is used to modify our multiplication
        {
            if (y + f >= 0 && y + f <= height - 1 && x + n >= 0 && x + n <= width - 1) //checking that we are within our image
            {
                tmp.rgbtRed += image[y + f][x + n].rgbtRed * k;
                tmp.rgbtGreen += image[y + f][x + n].rgbtGreen * k;
                tmp.rgbtBlue += image[y + f][x + n].rgbtBlue * k;
            }
            k = k + n * sign; //for the first cicle k is -1
            sign = sign * -1;
        }
        sign = 1;
    }
    return tmp; //returning new RGB chanels for pixel
}

RGBTRIPLE2 calc_gy(int y, int x, int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE2 tmp; // temp for weighted sum
    tmp.rgbtRed = 0;
    tmp.rgbtGreen = 0;
    tmp.rgbtBlue = 0;

    int sign = 1; // for changing argument for weightet sum (-1, -2, -1 then 1, 2, 1)
    for (int n = -1; n < 2; n += 2) //scrolling through upper and lower lines
    {
        for (int f = -1, k = n; f < 2; f++) //scrolling through each column. k is used to modify our multiplication
        {
            if (y + n >= 0 && y + n <= height - 1 && x + f >= 0 && x + f <= width - 1) //checking that we are within our image
            {
                tmp.rgbtRed += image[y + n][x + f].rgbtRed * k;
                tmp.rgbtGreen += image[y + n][x + f].rgbtGreen * k;
                tmp.rgbtBlue += image[y + n][x + f].rgbtBlue * k;
            }
            k = k + n * sign; //for the first cicle k is -1
            sign = sign * -1;
        }
        sign = 1;
    }
    return tmp; //returning new RGB chanels for pixel
}

RGBTRIPLE calc_ch(RGBTRIPLE2 gx, RGBTRIPLE2 gy)
{
    RGBTRIPLE tmp;
    int r, g, b;
    //getting mixed channel
    r = round(sqrt(gx.rgbtRed * gx.rgbtRed + gy.rgbtRed * gy.rgbtRed));
    b = round(sqrt(gx.rgbtBlue * gx.rgbtBlue + gy.rgbtBlue * gy.rgbtBlue));
    g = round(sqrt(gx.rgbtGreen * gx.rgbtGreen + gy.rgbtGreen * gy.rgbtGreen));

    //checking that color value isn't bigger than 255
    tmp.rgbtRed = r > 255 ? 255 : r;
    tmp.rgbtBlue = b > 255 ? 255 : b;
    tmp.rgbtGreen = g > 255 ? 255 : g;

    return tmp;
}