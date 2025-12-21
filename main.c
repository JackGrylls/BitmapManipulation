#include <stdio.h>

// Offset and size of the "size" header in the BMP file
#define SZ_OFFSET 2
#define SZ_SIZE 4

int getFilesize(FILE *fptr)
{
    // From https://en.wikipedia.org/wiki/BMP_file_format
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

    return test;
}


int main(int argc, char**argv)
{
    if (argc != 2)
    {
        printf("Usage: ./main.h <filename>.bmp\n");
        return 0;
    }

    char* filename = argv[1];
    FILE *fptr;
    fptr = fopen(filename,"r");
    // 39,494 bytes

    int filesize = getFilesize(fptr);
    printf("Size: %d\n",filesize);
}