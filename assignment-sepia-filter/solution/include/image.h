#ifndef _IMAGE_INTERNAL_H
#define _IMAGE_INTERNAL_H

#include <stdint.h>

struct image {
    uint64_t width, height;
    struct pixel* data;
};

struct pixel { 
    uint8_t b, g, r; 
};

struct image img_create(uint64_t width, uint64_t height);
void img_free(struct image img);
struct image img_copy(struct image* img);

#endif
