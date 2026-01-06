#include <stdio.h>
#include <stdlib.h>
#include "file_handling.c"

typedef struct pixel
{
    int R;
    int G;
    int B;
} pixel;

void hexcode(int hex, pixel * pix)
{
    printf("Hex: %0x\n",hex);
    printf("Binary: %032b\n",hex);
    pix->R = (0xFF0000 & hex) >> 16;
    pix->G = (0x00FF00 & hex) >> 8;
    pix->B = 0x0000FF & hex;
    printf("%0x\n",pix->R);
    printf("%0x\n",pix->G);
    printf("%0x\n",pix->B);
}

int setPix(image img, int x, int y, pixel * p)
{
    pixel pix = *p;

    // Range check
    if (x >= img.width || y >= img.height || x < 0 || y < 0) return 1;

    // https://en.wikipedia.org/wiki/BMP_file_format
    // One row contains 3 bytes for each pixel, but each row must be a multiple of 4 so there is some padding.
    // Heading: "Pixel storage"
    int rowSize = (img.bpp * img.width + 31) / 32 * 4;

    // Converting 2D coords to flat array index
    int addr = img.offset + (y*rowSize) + x*3;

    // Set colours individually
    img.image[addr] = pix.B;
    img.image[addr+1] = pix.G;
    img.image[addr+2] = pix.R;

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
    pixel pix;
    hexcode(0x02f0ff,&pix);
    printf("%0x\n",pix.R);
    printf("%0x\n",pix.G);
    printf("%0x\n",pix.B);
    setPix(data,0,0,&pix);
    setPix(data,112,0,&pix);
    setPix(data,112,115,&pix);
    setPix(data,0,115,&pix);
    
    // Write to output file
    FILE *wptr = fopen(argv[2],"wb");
    fwrite(data.image,sizeof(__uint8_t),getFilesize(fptr),wptr);

    return 0;
}
