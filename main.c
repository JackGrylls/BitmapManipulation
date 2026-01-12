#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include "file_handling.c"
#include "pixel.c"

#define PI 3.14159;

void gaussian_blur(image img,int ker_size, double sd)
{
    float ker[ker_size][ker_size];
    float total = 0;
    float divisor = sqrt(2 * 3.14159 * sd * sd);
    
    for (int i = 0; i < ker_size; i++)
    {
        for (int j = 0; j < ker_size; j++)
        {
            int di = i - (ker_size / 2);
            int dj = j - (ker_size / 2);

            float val = -((di*di + dj*dj)/(2*sd*sd));

            val = exp(val);
            ker[i][j] = val / divisor; 
        
            total += val;
        }
    }

    total = total / divisor;

    for (int i = 0; i < img.width; i++)
    {
        for (int j = 0; j < img.height; j++)
        {   
            pixel colour;

            colour.R = 0;
            colour.G = 0;
            colour.B = 0;

            for (int row = -ker_size / 2; row <= ker_size/2; row++)
            {
                for (int col = -ker_size/2; col <= ker_size/2; col++)
                {
                    int xPos = i + row;
                    int yPos = j + col;

                    if (xPos < 0) xPos = 0;
                    if (yPos < 0) yPos = 0;
                    if (xPos >= img.width) xPos = img.width - 1;
                    if (yPos >= img.height) yPos = img.height - 1;

                    colour.R += ker[row+ker_size/2][col+ker_size/2] * (getPix(img, xPos, yPos)->R);
                    colour.G += ker[row+ker_size/2][col+ker_size/2] * (getPix(img, xPos, yPos)->G);
                    colour.B += ker[row+ker_size/2][col+ker_size/2] * (getPix(img, xPos, yPos)->B);
                }
            }

            colour.R = colour.R / total;
            colour.G = colour.G / total;
            colour.B = colour.B / total;
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
    
    FILE *fptr = fopen(argv[1],"r");

    image data = toImage(fptr);
    printf("%dx%d, %d bit colour\n",data.width,data.height,data.bpp);
    printf("File size: %d bytes\n",getFilesize(fptr));
    printf("Offset: %d bytes\n",data.offset);
    
    
    // Manipulate image
    int kernel_size = atoi(argv[3]);
    int sigma = atoi(argv[4]);

    gaussian_blur(data,kernel_size,sigma);
    
    // Write to output file
    FILE *wptr = fopen(argv[2],"wb");
    fwrite(data.image,sizeof(__uint8_t),getFilesize(fptr),wptr);

    return 0;
}
