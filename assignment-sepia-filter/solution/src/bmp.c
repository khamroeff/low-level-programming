#include "bmp.h"
#include "image.h"
#include <stdbool.h>
#include <stdio.h>

static const uint16_t BF_TYPE = 19778;
static const uint16_t BI_BIT_COUNT = 24;
static const uint32_t BF_HEADER_SIZE = 54;
static const uint16_t BI_SIZE = 40;
static const uint16_t BI_PLANES = 1;

static int8_t calc_padding(uint64_t width) {
    return (4 - width * 3) % 4;
}

//returns false if fail
bool bmp_read(const char* filename, struct image* image) {
    FILE* file = fopen(filename, "rb");
    if (file == NULL) 
        return false;

    struct bmp_header header;
    if (!fread( &header, sizeof( struct bmp_header ), 1, file )) 
        return false;

    *image = img_create(header.biWidth, header.biHeight);
    int8_t padding = calc_padding(image->width);
    for (size_t i = 0; i < image->height; i++) {
        if (fread(image->data + i*image->width, sizeof(struct pixel), image->width, file) != image->width)
            return false;
        if (fseek(file, padding, SEEK_CUR)) 
            return false;
    }

    fclose(file);
    return true;
}

//returns false if fail
bool bmp_write(const char* filename, struct image* image) {
    FILE* file = fopen(filename, "wb");
    if (file == NULL) 
        return false;

    int8_t padding = calc_padding(image->width);

    uint32_t file_size = image->height * (image->width * 3 + padding);
    struct bmp_header header = (struct bmp_header) {
        .bfType = BF_TYPE,
        .bfileSize = BF_HEADER_SIZE + file_size,
        .bfReserved = 0,
        .bOffBits = BF_HEADER_SIZE,
        .biSize = BI_SIZE,
        .biWidth = image->width,
        .biHeight = image->height,
        .biPlanes = BI_PLANES,
        .biBitCount = BI_BIT_COUNT,
        .biCompression = 0,
        .biSizeImage = file_size,
        .biXPelsPerMeter = 0,
        .biYPelsPerMeter = 0,
        .biClrUsed = 0,
        .biClrImportant = 0
    };
    if (!fwrite(&header, sizeof(struct bmp_header), 1, file)) 
        return false;

    const int32_t zero = 0;
    for (size_t i = 0; i < image->height; i++) {
        if (fwrite(image->data + i*image->width, sizeof(struct pixel), image->width, file) != image->width) 
            return false;
        if (fwrite(&zero, 1, padding, file) != padding) 
            return false;
    }

    fclose(file);
    return true;
}
