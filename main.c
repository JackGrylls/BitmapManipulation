#include <stdio.h>
#include <stdlib.h>
#include "file_handling.c"
#include "pixel.c"

void gaussian_blur(image img)
{
    for (int i = 0; i < img.width; i++)
    {
        for (int j = 0; j < img.height; j++)
        {   
            pixel colour;

            int ker[3][3] = {
                {1,2,1},
                {2,4,2},
                {1,2,1}
            };

            colour.R = 0;
            colour.G = 0;
            colour.B = 0;

            for (int row = -1; row <= 1; row++)
            {
                for (int col = -1; col <= 1; col++)
                {
                    int xPos = i + row;
                    int yPos = j + col;

                    if (xPos < 0) xPos = 0;
                    if (yPos < 0) yPos = 0;
                    if (xPos >= img.width) xPos = img.width - 1;
                    if (yPos >= img.height) yPos = img.height - 1;

                    colour.R += ker[row+1][col+1] * (getPix(img, xPos, yPos)->R);
                    colour.G += ker[row+1][col+1] * (getPix(img, xPos, yPos)->G);
                    colour.B += ker[row+1][col+1] * (getPix(img, xPos, yPos)->B);
                }
            }

            colour.R = colour.R / 16;
            colour.G = colour.G / 16;
            colour.B = colour.B / 16;

            setPix(img,i,j,&colour);
        }
    }
}

int main(int argc, char** argv)
{
    if (argc <= 2)
    {
        printf("Usage: ./main.h <filename>.bmp <filename>.bmp\n");
        printf("                ^input file    ^ output file\n");
        return 0;
    }
    
    FILE *fptr = fopen(argv[1],"r");

    image data = toImage(fptr);
    printf("%dx%d, %d bit colour\n",data.width,data.height,data.bpp);
    printf("File size: %d bytes\n",getFilesize(fptr));
    printf("Offset: %d bytes\n",data.offset);
    
    
    // Manipulate image
    gaussian_blur(data);
    
    // Write to output file
    FILE *wptr = fopen(argv[2],"wb");
    fwrite(data.image,sizeof(__uint8_t),getFilesize(fptr),wptr);

    return 0;
}
