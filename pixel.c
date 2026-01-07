typedef struct pixel
{
    int R;
    int G;
    int B;
} pixel;

pixel * colour(int hex)
{
    pixel * pix = malloc(sizeof(pixel));
    
    pix->R = (0xFF0000 & hex) >> 16;
    pix->G = (0x00FF00 & hex) >> 8;
    pix->B = 0x0000FF & hex;

    return pix;
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