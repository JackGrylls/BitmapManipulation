#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "file_handling.c"
#include "pixel.c"

#define pi 3.141592653589793

void gaussian_blur(image img,int ker_size, double sd)
{
    // Initialising the gaussian kernel
    float ker[ker_size][ker_size];
    float total = 0;
    
    // 2D gaussian equation applied on every item in matrix
    // G(x,y) = 1/(2*pi*sigma^2) * exp(-(x^2 + y^2)/2*sigma^2)
    // or:
    // G(x,y) = exp(-(x^2 + y^2)/2*sigma^2) / (2*pi*sigma^2)
    //          ^ numerator                     ^ denominator (unused)

    // Dividing by (2*pi*sigma^2) causes rounding errors and we divide the final
    // RGB values by the total of the whole matrix later anyway, so we skip this redundant step.
    for (int i = 0; i < ker_size; i++)
    {
        for (int j = 0; j < ker_size; j++)
        {
            // These coordinates represent the distance from the centre
            int di = i - ker_size/2;
            int dj = j - ker_size/2;

            float numerator = exp(-(di*di + dj*dj)/(2*sd*sd));
            ker[i][j] = numerator;
            total += numerator;
        }
    }


    // Now we use the "gaussian kernel" as an averaging filter on every pixel in the image
    for (int i = 0; i < img.width; i++)
    {
        for (int j = 0; j < img.height; j++)
        {   
            pixel colour;
            
            // each new RGB value is the sum of its neighbours multiplied by the corresponding kernel value
            // so we iteratively count the sum
            colour.R = 0;
            colour.G = 0;
            colour.B = 0;

            // Iterating over neighbouring pixels within the width of kernel
            for (int row = -ker_size / 2; row <= ker_size/2; row++)
            {
                for (int col = -ker_size / 2; col <= ker_size/2; col++)
                {
                    // Calculate neighbour coordinates
                    int xPos = i + row;
                    int yPos = j + col;

                    // Edge cases where's no neighbouring pixel (walls, corners)
                    if (xPos < 0) xPos = 0;
                    if (yPos < 0) yPos = 0;
                    if (xPos >= img.width) xPos = img.width - 1;
                    if (yPos >= img.height) yPos = img.height - 1;

                    // Multiply neighbour's RGB values by the corresponding kernel index
                    // Add result to running totals
                    colour.R += ker[row+ker_size/2][col+ker_size/2] * (getPix(img, xPos, yPos)->R);
                    colour.G += ker[row+ker_size/2][col+ker_size/2] * (getPix(img, xPos, yPos)->G);
                    colour.B += ker[row+ker_size/2][col+ker_size/2] * (getPix(img, xPos, yPos)->B);
                }
            }

            // This is an AVERAGING filter, so we must divide by the total
            // Doing it here rather than inside the loop prevents precision errors from division
            // and is also more efficient.
            colour.R = colour.R / total;
            colour.G = colour.G / total;
            colour.B = colour.B / total;

            // Finally, set the pixel colour.
            setPix(img,i,j,&colour);
        }
    }
}

int main(int argc, char** argv)
{
    if (argc <= 4)
    {
        printf("Usage: ./main.h <filename>.bmp <filename>.bmp <kernel size> <sigma>\n");
        printf("                ^input file    ^ output file\n");
        return 0;
    }
    
    // Get data from file
    FILE *fptr = fopen(argv[1],"r");
    image data = toImage(fptr);
    fclose(fptr);

    printf("%dx%d, %d bit colour\n",data.width,data.height,data.bpp);
    printf("File size: %d bytes\n",data.filesize);
    printf("Offset: %d bytes\n",data.offset);
    
    
    // Manipulate image based on args
    int kernel_size = atoi(argv[3]);
    int sigma = atoi(argv[4]);
    gaussian_blur(data,kernel_size,sigma);

    
    // Write to output file
    FILE *wptr = fopen(argv[2],"wb");
    fwrite(data.image,sizeof(__uint8_t),data.filesize,wptr);
    fclose(wptr);

    return 0;
}
