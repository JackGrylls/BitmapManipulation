#include <stdio.h>
#include <stdlib.h>

// Offset and size of the "size" header in the BMP file
#define SZ_OFFSET 2
#define SZ_SIZE 4

int getFilesize(FILE *fptr)
{
    rewind(fptr);
    // From https://en.wikipedia.org/wiki/BMP_file_format
    int offset = 2;
    int size = 4;

    // We read just enough of the file to look at this specific part of the header
    __uint8_t contents[offset+size];
    fgets(contents,offset+size,fptr);
    
    // The first two bytes in a bmp file are a header field identifying the BMP
    // The next four represent the file size
    // These bitwise operations read those four bytes and convert them into an integer
    
    // for (int i = 0; i < 6; i++)
    // {
    //     printf("%08b\n",contents[i]);
    // }

    int test = 0;
    test = test^(contents[5] << 24);
    test = test^(contents[4] << 16);
    test = test^(contents[3] << 8);
    test = test^(contents[2]);

    rewind(fptr);
    return test;
}

__uint8_t * readFile(FILE *fptr)
{   
    rewind(fptr);

    int size = getFilesize(fptr);
    __uint8_t * contents = malloc(sizeof(__uint8_t)*size);
    fgets(contents,size,fptr);
    
    
    
    return contents;
}

int main(int argc, char**argv)
{
    if (argc != 2)
    {
        printf("Usage: ./main.h <filename>.bmp\n");
        return 0;
    }
    
    FILE *fptr = fopen(argv[1],"r");

    __uint8_t * file = readFile(fptr);

    for(int i = 0; i < 128; i++)
    {
        printf("%d: %0x\n",i,file[i]);
    }
}