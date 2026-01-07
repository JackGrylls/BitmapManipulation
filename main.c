#include <stdio.h>
#include <stdlib.h>
#include "file_handling.c"
#include "pixel.c"

void gaussian_blur(image img)
{
    
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
    
    
    // Example usage of setPix, sets all four corners to different colours.
    pixel pix;
    setPix(data,0,0,colour(0xff0000));
    setPix(data,112,0,colour(0x02f0ff));
    setPix(data,112,115,colour(0x02f0ff));
    setPix(data,0,115,colour(0x02f0ff));
    
    // Write to output file
    FILE *wptr = fopen(argv[2],"wb");
    fwrite(data.image,sizeof(__uint8_t),getFilesize(fptr),wptr);

    return 0;
}
