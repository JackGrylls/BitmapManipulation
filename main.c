#include <stdio.h>
#include <stdlib.h>
#include "file_handling.c"
#include "pixel.c"

void gaussian_blur(image img)
{
    pixel * pix = getPix(img,33,81);

    for (int i = 0; i < img.width; i++)
    {
        for (int j = 0; j < img.height; j++)
        {   
            int ker[3][3] = {
                {1,2,1},
                {2,4,2},
                {1,2,1}
            };

            setPix(img,i,j,pix);
        }
    }

    setPix(img,33,81,colour(0xFF0000));
}

int main(int argc, char**argv)
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
