#include <stdio.h>
#include <stdlib.h>
#include "file_handling.c"

int setPix(image img, int x, int y, int R, int G, int B)
{
    // Range check
    if (x >= img.width || y >= img.height || x < 0 || y < 0) return 1;

    // https://en.wikipedia.org/wiki/BMP_file_format
    // One row contains 3 bytes for each pixel, but each row must be a multiple of 4 so there is some padding.
    // Heading: "Pixel storage"
    int rowSize = (img.bpp * img.width + 31) / 32 * 4;

    // Converting 2D coords to flat array index
    int addr = img.offset + (y*rowSize) + x*3;

    // Set colours individually
    img.image[addr] = B;
    img.image[addr+1] = G;
    img.image[addr+2] = R;

    return 0;
}

int main(int argc, char**argv)
{
    if (argc <= 2)
    {
        printf("Usage: ./main.h <filename>.bmp\n");
        return 0;
    }
    
    FILE *fptr = fopen(argv[1],"r");

    image data = toImage(fptr);
    printf("%dx%d, %d bit colour.\n",data.width,data.height,data.bpp);
    printf("File size: %d\n",getFilesize(fptr));
    printf("Offset: %d\n",data.offset);
    
    // Example usage of setPix, sets all four corners to different colours.
    setPix(data,0,0,255,0,0);
    setPix(data,112,0,0,255,0);
    setPix(data,112,115,0,0,255);
    setPix(data,0,115,0,255,255);
    
    // Write to output file
    FILE *wptr = fopen(argv[2],"wb");
    fwrite(data.image,sizeof(__uint8_t),getFilesize(fptr),wptr);
    return 0;
}
