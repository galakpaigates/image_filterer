#include <stdlib.h>
#include <stdio.h>
#include <math.h>

#include "helpers.h"


// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j;
    int sum = 0;

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            sum = image[i][j].rgbtBlue + image[i][j].rgbtGreen + image[i][j].rgbtRed;

            image[i][j].rgbtBlue = (int)round(sum / 3.0);
            image[i][j].rgbtGreen = (int)round(sum / 3.0);
            image[i][j].rgbtRed = (int)round(sum / 3.0);
        }
    }

    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE tmp;

    for (int i = 0; i < height; i++)
    {
        for (int j = (width - 1), idx = 0; j > (width/2)-1; j--, idx++)
        {
            // Store the Pixel from the right in the tmp var
            tmp = image[i][idx];

            image[i][idx] = image[i][j];

            image[i][j] = tmp;
        }
    }

    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    float sepiaRed = 0.0;
    float sepiaBlue = 0.0;
    float sepiaGreen   = 0.0;

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            sepiaBlue = .272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue;
            sepiaRed = .393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue;
            sepiaGreen = .349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue;

            // Handle Values more than 255
            cap_value(&sepiaRed);
            cap_value(&sepiaGreen);
            cap_value(&sepiaBlue);

            // Assign value to each channel
            image[i][j].rgbtRed = (int)round(sepiaRed);
            image[i][j].rgbtGreen = (int)round(sepiaGreen);
            image[i][j].rgbtBlue = (int)round(sepiaBlue);
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j;
    int red_sum = 0;
    int blue_sum = 0;
    int green_sum = 0;

    // Create Image to Save Image
    RGBTRIPLE(*copy)[width+2] = calloc((height+1), (width+1) * sizeof(RGBTRIPLE));

    // Copy original image into tmp
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            copy[i][j] = image[i][j];
        }
    }

    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            // Handle top row
            if (i < 1)
            {
                // Handle top left corner of the image
                if (j < 1)
                {
                    red_sum = copy[i][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i+1][j].rgbtRed + copy[i+1][j+1].rgbtRed;
                    green_sum = copy[i][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i+1][j+1].rgbtGreen;
                    blue_sum = copy[i][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i+1][j+1].rgbtBlue;

                    image[i][j].rgbtRed = (int)round(red_sum / 4.0);
                    image[i][j].rgbtGreen = (int)round(green_sum / 4.0);
                    image[i][j].rgbtBlue = (int)round(blue_sum / 4.0);

                    continue;
                }
                // Handle the top right corner of the image
                else if (j >= (width-1))
                {
                    red_sum = copy[i][j].rgbtRed + copy[i][j-1].rgbtRed + copy[i+1][j].rgbtRed + copy[i+1][j-1].rgbtRed;
                    green_sum = copy[i][j].rgbtGreen + copy[i][j-1].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i+1][j-1].rgbtGreen;
                    blue_sum = copy[i][j].rgbtBlue + copy[i][j-1].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i+1][j-1].rgbtBlue;

                    image[i][j].rgbtRed = (int)round(red_sum / 4.0);
                    image[i][j].rgbtGreen = (int)round(green_sum / 4.0);
                    image[i][j].rgbtBlue = (int)round(blue_sum / 4.0);

                    continue;
                }
                // Handle all other pixels on the bottom row of the image
                else
                {
                    red_sum = copy[i][j-1].rgbtRed + copy[i][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i+1][j-1].rgbtRed + copy[i+1][j].rgbtRed + copy[i+1][j+1].rgbtRed;
                    green_sum = copy[i][j-1].rgbtGreen + copy[i][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i+1][j-1].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i+1][j+1].rgbtGreen;
                    blue_sum = copy[i][j-1].rgbtBlue + copy[i][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i+1][j-1].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i+1][j+1].rgbtBlue;

                    image[i][j].rgbtRed = (int)round(red_sum / 6.0);
                    image[i][j].rgbtGreen = (int)round(green_sum / 6.0);
                    image[i][j].rgbtBlue = (int)round(blue_sum / 6.0);

                    continue;
                }
            }
            // Handle bottom row
            else if (i >= (height-1))
            {
                // Handle bottom left corner of the image
                if (j < 1)
                {
                    red_sum = copy[i][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i-1][j].rgbtRed + copy[i-1][j+1].rgbtRed;
                    green_sum = copy[i][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i-1][j+1].rgbtGreen;
                    blue_sum = copy[i][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i-1][j+1].rgbtBlue;

                    image[i][j].rgbtRed = (int)round(red_sum / 4.0);
                    image[i][j].rgbtGreen = (int)round(green_sum / 4.0);
                    image[i][j].rgbtBlue = (int)round(blue_sum / 4.0);

                    continue;
                }
                // Handle the bottom right corner of the image
                else if (j >= (width-1))
                {
                    red_sum = copy[i][j].rgbtRed + copy[i][j-1].rgbtRed + copy[i-1][j].rgbtRed + copy[i-1][j-1].rgbtRed;
                    green_sum = copy[i][j].rgbtGreen + copy[i][j-1].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i-1][j-1].rgbtGreen;
                    blue_sum = copy[i][j].rgbtBlue + copy[i][j-1].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i-1][j-1].rgbtBlue;

                    image[i][j].rgbtRed = (int)round(red_sum / 4.0);
                    image[i][j].rgbtGreen = (int)round(green_sum / 4.0);
                    image[i][j].rgbtBlue = (int)round(blue_sum / 4.0);

                    continue;
                }
                // Handle all other pixel at the bottom row of the image
                else
                {
                    red_sum = copy[i][j-1].rgbtRed + copy[i][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i-1][j-1].rgbtRed + copy[i-1][j].rgbtRed + copy[i-1][j+1].rgbtRed;
                    green_sum = copy[i][j-1].rgbtGreen + copy[i][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i-1][j-1].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i-1][j+1].rgbtGreen;
                    blue_sum = copy[i][j-1].rgbtBlue + copy[i][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i-1][j-1].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i-1][j+1].rgbtBlue;

                    image[i][j].rgbtRed = (int)round(red_sum / 6.0);
                    image[i][j].rgbtGreen = (int)round(green_sum / 6.0);
                    image[i][j].rgbtBlue = (int)round(blue_sum / 6.0);

                    continue;
                }
            }
            else if (j < 1)
            {
                // Handle left most column of the image
                red_sum = copy[i][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i-1][j].rgbtRed + copy[i-1][j+1].rgbtRed + copy[i+1][j].rgbtRed + copy[i+1][j+1].rgbtRed;
                green_sum = copy[i][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i-1][j+1].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i+1][j+1].rgbtGreen;
                blue_sum = copy[i][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i-1][j+1].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i+1][j+1].rgbtBlue;

                image[i][j].rgbtRed = (int)round(red_sum / 6.0);
                image[i][j].rgbtGreen = (int)round(green_sum / 6.0);
                image[i][j].rgbtBlue = (int)round(blue_sum / 6.0);

                continue;
            }
            else if (j >= (width-1))
            {
                // Handle right most column of the image
                red_sum = copy[i][j-1].rgbtRed + copy[i][j].rgbtRed + copy[i-1][j-1].rgbtRed + copy[i-1][j].rgbtRed + copy[i+1][j].rgbtRed + copy[i+1][j-1].rgbtRed;
                green_sum = copy[i][j-1].rgbtGreen + copy[i][j].rgbtGreen + copy[i-1][j-1].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i+1][j-1].rgbtGreen;
                blue_sum = copy[i][j-1].rgbtBlue + copy[i][j].rgbtBlue + copy[i-1][j-1].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i+1][j-1].rgbtBlue;

                image[i][j].rgbtRed = (int)round(red_sum / 6.0);
                image[i][j].rgbtGreen = (int)round(green_sum / 6.0);
                image[i][j].rgbtBlue = (int)round(blue_sum / 6.0);

                continue;
            }
            // Handle all other pixels in the middle
            else
            {
                red_sum = copy[i-1][j-1].rgbtRed + copy[i-1][j].rgbtRed + copy[i-1][j+1].rgbtRed + copy[i][j-1].rgbtRed + copy[i][j].rgbtRed + copy[i][j+1].rgbtRed + copy[i+1][j-1].rgbtRed + copy[i+1][j].rgbtRed + copy[i+1][j+1].rgbtRed;
                green_sum = copy[i-1][j-1].rgbtGreen + copy[i-1][j].rgbtGreen + copy[i-1][j+1].rgbtGreen + copy[i][j-1].rgbtGreen + copy[i][j].rgbtGreen + copy[i][j+1].rgbtGreen + copy[i+1][j-1].rgbtGreen + copy[i+1][j].rgbtGreen + copy[i+1][j+1].rgbtGreen;
                blue_sum = copy[i-1][j-1].rgbtBlue + copy[i-1][j].rgbtBlue + copy[i-1][j+1].rgbtBlue + copy[i][j-1].rgbtBlue + copy[i][j].rgbtBlue + copy[i][j+1].rgbtBlue + copy[i+1][j-1].rgbtBlue + copy[i+1][j].rgbtBlue + copy[i+1][j+1].rgbtBlue;

                image[i][j].rgbtRed = (int)round(red_sum / 9.0);
                image[i][j].rgbtGreen = (int)round(green_sum / 9.0);
                image[i][j].rgbtBlue = (int)round(blue_sum / 9.0);
            }
        }
    }

    free(copy);

    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    int i, j;

    int x_red = 0, x_green = 0, x_blue = 0;
    int y_red = 0, y_green = 0, y_blue = 0;

    int red_magnitude = 0;
    int green_magnitude = 0;
    int blue_magnitude = 0;

    // Create Image to Save Image with additional one pixel border
    RGBTRIPLE(*copy)[width+2] = calloc((height+2), (width+2) * sizeof(RGBTRIPLE));

    // Add one pixel black border around the image to avoid mem errors during runtime

        // Add one pixel border at the top row of the copy
        for (i = 0, j = 0; j < (width+2); j++)
        {
            copy[i][j].rgbtRed = 0x00;
            copy[i][j].rgbtGreen = 0x00;
            copy[i][j].rgbtBlue = 0x00;
        }

        // Add one pixel border at the bottom row of the image
        for (i = (height+1), j = 0; j < (width+2); j++)
        {
            copy[i][j].rgbtRed = 0x00;
            copy[i][j].rgbtGreen = 0x00;
            copy[i][j].rgbtBlue = 0x00;
        }

        // Add one pixel border at the left column of the copy
        for (i = 1, j = 0; i < (height+2); i++)
        {
            copy[i][j].rgbtRed = 0x00;
            copy[i][j].rgbtGreen = 0x00;
            copy[i][j].rgbtBlue = 0x00;
        }

        // Add one pixel border at the right column of the copy
        for (i = 1, j = width+1; i < (height+2); i++)
        {
            copy[i][j].rgbtRed = 0x00;
            copy[i][j].rgbtGreen = 0x00;
            copy[i][j].rgbtBlue = 0x00;
        }

    // Copy original image into tmp
    for (i = 0; i < height; i++)
    {
        for (j = 0; j < width; j++)
        {
            copy[i+1][j+1] = image[i][j];
        }
    }

    // Loop through image copy
    for (i = 1; i < height+1; i++)
    {
        for (j = 1; j < width+1; j++)
        {
            // Y Axis
            y_red = (copy[i-1][j-1].rgbtRed * -1) + (copy[i-1][j].rgbtRed * -2) + (copy[i-1][j+1].rgbtRed * -1) + (copy[i+1][j-1].rgbtRed) + (copy[i+1][j].rgbtRed * 2) + (copy[i+1][j+1].rgbtRed);
            y_green = (copy[i-1][j-1].rgbtGreen * -1) + (copy[i-1][j].rgbtGreen * -2) + (copy[i-1][j+1].rgbtGreen * -1) + (copy[i+1][j-1].rgbtGreen) + (copy[i+1][j].rgbtGreen * 2) + (copy[i+1][j+1].rgbtGreen);
            y_blue = (copy[i-1][j-1].rgbtBlue * -1) + (copy[i-1][j].rgbtBlue * -2) + (copy[i-1][j+1].rgbtBlue * -1) + (copy[i+1][j-1].rgbtBlue) + (copy[i+1][j].rgbtBlue * 2) + (copy[i+1][j+1].rgbtBlue);

            // X Axis
            x_red = (copy[i-1][j-1].rgbtRed * -1) + (copy[i][j-1].rgbtRed * -2) + (copy[i+1][j-1].rgbtRed * -1) + (copy[i-1][j+1].rgbtRed) + (copy[i][j+1].rgbtRed * 2) + (copy[i+1][j+1].rgbtRed);
            x_green = (copy[i-1][j-1].rgbtGreen * -1) + (copy[i][j-1].rgbtGreen * -2) + (copy[i+1][j-1].rgbtGreen * -1) + (copy[i-1][j+1].rgbtGreen) + (copy[i][j+1].rgbtGreen * 2) + (copy[i+1][j+1].rgbtGreen);
            x_blue = (copy[i-1][j-1].rgbtBlue * -1) + (copy[i][j-1].rgbtBlue * -2) + (copy[i+1][j-1].rgbtBlue * -1) + (copy[i-1][j+1].rgbtBlue) + (copy[i][j+1].rgbtBlue * 2) + (copy[i+1][j+1].rgbtBlue);

            // Sum Products from x and y axis
            red_magnitude = (int)round(sqrt(sqrt(x_red) + sqrt(y_red)));
            green_magnitude = (int)round(sqrt(sqrt(x_green) + sqrt(y_green)));
            blue_magnitude = (int)round(sqrt(sqrt(x_blue) + sqrt(y_blue)));

            // Cap Red Value
            if (red_magnitude > 255)
                red_magnitude = 255;

            // Cap Green Value
            if (green_magnitude > 255)
                green_magnitude = 255;

            // Cap Blue Value
            if (blue_magnitude > 255)
                blue_magnitude = 255;

            if (red_magnitude > 50 || green_magnitude > 50 || blue_magnitude > 50)
            {
                image[i-1][j-1].rgbtRed = red_magnitude;
                image[i-1][j-1].rgbtGreen = green_magnitude;
                image[i-1][j-1].rgbtBlue = blue_magnitude;
            }
        }
    }

    free(copy);

    return;
}

// ensure the channel value is atmost 255
void cap_value(float *value)
{
    if (*value > 255)
        *value -= *value - 255;

    return;
}
