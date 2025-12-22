#include <stdio.h>
#include <stdlib.h>

typedef struct image
{
    __uint8_t* image;
    int width;
    int height;
    int bpp; // bits per pixel
    int offset; // The offset at which the actual pixel data starts
} image;

int bytesToInt(__uint8_t* img, int offset, int size)
{
    int value = 0;

    for (int i = offset; i < offset+size; i++)
    {
        int shAmt = (i - offset) * 8;
        value = value ^ (img[i] << shAmt);
    }

    return value;
}

// Assumes the file is at least 6 bytes long
// If the file is a .bmp, this function will retrieve its size by reading its file header. 
int getFilesize(FILE* fptr)
{
    // From https://en.wikipedia.org/wiki/BMP_file_format
    // Offset and length of "size" attribute
    int offset = 2;
    int size = 4;

    // We read just enough of the file to look at this specific part of the header
    __uint8_t contents[offset+size];
    fread(contents,sizeof(__uint8_t),offset+size,fptr);
    
    // The first two bytes in a bmp file are a header field identifying the BMP
    // The next four represent the file size
    rewind(fptr);
    return bytesToInt(contents,2,4);
}

// Reads the entire contents of a valid .bmp file into an array, returns array pointer
__uint8_t * readFile(FILE *fptr)
{   
    int size = getFilesize(fptr);
    __uint8_t * contents = malloc(sizeof(__uint8_t)*size);
    fread(contents,sizeof(__uint8_t),size,fptr);
    
    rewind(fptr);
    return contents;
}

image toImage(FILE* fptr)
{
    __uint8_t* img = readFile(fptr);

    // From https://en.wikipedia.org/wiki/BMP_file_format
    // Specifically, this is read from the DIB data of the file, according to the Windows BITMAPINFOHEADER format.
    image data;
    data.image = img;
    data.width = bytesToInt(img,18,4);
    data.height = bytesToInt(img,22,4);
    data.bpp = bytesToInt(img,28,2);
    data.offset = bytesToInt(img,10,4);

    return data;
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

    FILE *wptr = fopen(argv[2],"wb");

    fwrite(data.image,sizeof(__uint8_t),getFilesize(fptr),wptr);

    return 0;
}
