#include "bmp.h"

#include <stdlib.h>

typedef int LONG;
typedef unsigned char BYTE;
typedef unsigned int DWORD;
typedef unsigned short WORD;

typedef struct tagBITMAPFILEHEADER {
    WORD    bfType; // 2  /* Magic identifier */
    DWORD   bfSize; // 4  /* File size in bytes */
    WORD    bfReserved1; // 2
    WORD    bfReserved2; // 2
    DWORD   bfOffBits; // 4 /* Offset to image data, bytes */
} __attribute__((packed)) BITMAPFILEHEADER;

typedef struct tagBITMAPINFOHEADER {
    DWORD    biSize; // 4 /* Header size in bytes */
    LONG     biWidth; // 4 /* Width of image */
    LONG     biHeight; // 4 /* Height of image */
    WORD     biPlanes; // 2 /* Number of colour planes */
    WORD     biBitCount; // 2 /* Bits per pixel */
    DWORD    biCompress; // 4 /* Compression type */
    DWORD    biSizeImage; // 4 /* Image size in bytes */
    LONG     biXPelsPerMeter; // 4
    LONG     biYPelsPerMeter; // 4 /* Pixels per meter */
    DWORD    biClrUsed; // 4 /* Number of colours */
    DWORD    biClrImportant; // 4 /* Important colours */
} __attribute__((packed)) BITMAPINFOHEADER;

typedef struct {
    BYTE    b;
    BYTE    g;
    BYTE    r;
} RGB_data;

bool BuildBmpFileData(int width, int height, const uint8_t* source, char** outdata, size_t* outsize) {
    int numPixels = width * height;
    int fileSize =
        sizeof(BITMAPFILEHEADER) +
        sizeof(BITMAPINFOHEADER) +
        sizeof(RGB_data) * numPixels;

    char* const buf = malloc(fileSize);
    if (buf == NULL) {
        return false;
    }

    char* ptr = buf;
    BITMAPFILEHEADER* bmp_head = (BITMAPFILEHEADER*)ptr;
    ptr += sizeof(BITMAPFILEHEADER);
    BITMAPINFOHEADER* bmp_info = (BITMAPINFOHEADER*)ptr;
    ptr += sizeof(BITMAPINFOHEADER);
    RGB_data* data = (RGB_data*)ptr;

    bmp_head->bfType = 0x4D42; // 'BM'
    bmp_head->bfSize = fileSize;
    bmp_head->bfReserved1 = 0;
    bmp_head->bfReserved2 = 0;
    bmp_head->bfOffBits = ptr - buf;
    // finish the initial of head

    bmp_info->biSize = sizeof(BITMAPINFOHEADER);
    bmp_info->biWidth = width;
    bmp_info->biHeight = -height;
    bmp_info->biPlanes = 1;
    bmp_info->biBitCount = 24; // bit(s) per pixel, 24 is true color
    bmp_info->biCompress = 0;
    bmp_info->biSizeImage = numPixels;
    bmp_info->biXPelsPerMeter = 0;
    bmp_info->biYPelsPerMeter = 0;
    bmp_info->biClrUsed = 0 ;
    bmp_info->biClrImportant = 0;
    // finish the initial of infohead

    for (int i = 0; i < numPixels; ++i) {
        data[i].r = source[i];
        data[i].g = source[i + numPixels];
        data[i].b = source[i + numPixels + numPixels];
    }
    *outdata = buf;
    *outsize = fileSize;
    return true;
}
