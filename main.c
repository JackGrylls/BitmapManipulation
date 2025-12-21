#include <stdio.h>
#include <stdlib.h>

typedef struct metadata
{
    int width;
    int height;
    int bpp; // bits per pixel
} metadata;

// Assumes the file is at least 6 bytes long
// If the file is a .bmp, this function will retrieve its size by reading its file header. 
int getFilesize(FILE *fptr)
{
    rewind(fptr);

    // From https://en.wikipedia.org/wiki/BMP_file_format
    // Offset and length of "size" attribute
    int offset = 2;
    int size = 4;

    // We read just enough of the file to look at this specific part of the header
    __uint8_t contents[offset+size];
    fgets(contents,offset+size,fptr);
    
    // The first two bytes in a bmp file are a header field identifying the BMP
    // The next four represent the file size
    // These bitwise operations read those four bytes and convert them into an integer
    int test = 0;
    test = test^(contents[5] << 24);
    test = test^(contents[4] << 16);
    test = test^(contents[3] << 8);
    test = test^(contents[2]);

    rewind(fptr);
    return test;
}

// Reads the entire contents of a valid .bmp file into an array, returns array pointer
__uint8_t * readFile(FILE *fptr)
{   
    rewind(fptr);

    int size = getFilesize(fptr);
    __uint8_t * contents = malloc(sizeof(__uint8_t)*size);
    fgets(contents,size,fptr);
    
    
    
    return contents;
}

metadata getMetadata(__uint8_t * img)
{
    // From https://en.wikipedia.org/wiki/BMP_file_format
    // In DIB header, width is at offset 18 and is 2 bytes long
    __uint8_t left = img[18];
    __uint8_t right = img[19];
    __uint16_t width_16 = (0 ^ right << 8) ^ left;
    int width = 0 ^ width_16;

    // In DIB header, height is at offset 20 and is 2 bytes long
    // I noticed this doesn't actually seem to line up, and the actual offset is 22.
    left = img[22];
    right = img[23];
    __uint16_t height_16 = (0 ^ right << 8) ^ left;
    int height = 0 ^ height_16;

    // Bits per pixel is at offset 28
    left = img[28];
    right = img[29];
    __uint16_t bpp = (0 ^ right << 8) ^ left;

    metadata data;
    data.width = width;
    data.height = height;
    data.bpp = bpp;

    return data;
}

int getHeight(__uint8_t * img)
{
    // From https://en.wikipedia.org/wiki/BMP_file_format
    // In DIB header, height is at offset 20 and is 2 bytes long
    // I noticed this doesn't actually seem to line up, and the actual offset is 22.
    __uint8_t left = img[22];
    __uint8_t right = img[23];
    __uint16_t height_16 = (0 ^ right << 8) ^ left;
    int height = 0 ^ height_16;

    return height;
}

int main(int argc, char**argv)
{
    if (argc != 2)
    {
        printf("Usage: ./main.h <filename>.bmp\n");
        return 0;
    }
    
    FILE *fptr = fopen(argv[1],"r");

    __uint8_t * img = readFile(fptr);

    metadata data = getMetadata(img);
    printf("%dx%d, %d bit colour.\n",data.width,data.height,data.bpp);
}