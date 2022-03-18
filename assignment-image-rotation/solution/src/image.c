#include "image.h"
#include <inttypes.h>
#include <stdlib.h>

const struct pixel null_pixel = (struct pixel) { .b = 0, .g = 0, .r = 0 };

struct image img_create(uint64_t width, uint64_t height) {
    struct pixel* data = malloc(width * height * sizeof(struct pixel));
    return (struct image) { .width = width, .height = height, .data = data };
}

void img_free(struct image img) {
    free(img.data);
}

struct pixel img_px_get(struct image img, uint64_t x, uint64_t y) {
    if (x >= img.width || y >= img.height) 
        return null_pixel;
    return img.data[y*img.width + x];
}

void img_px_set(struct image img, uint64_t x, uint64_t y, struct pixel pixel) {
    if (x >= img.width || y >= img.height) 
        return;
    img.data[y*img.width + x] = pixel;
}

struct image img_rot90( struct image img ) {
    struct image ret = img_create(img.height, img.width);
    
    for (size_t x = 0; x < img.width; x++) {
    for (size_t y = 0; y < img.height; y++) {
        img_px_set(ret, ret.width - y - 1, x, img_px_get(img, x, y));
    }}

    return ret;
}
