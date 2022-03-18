#include "image.h"
#include <inttypes.h>
#include <stdlib.h>
#include <string.h>

struct image img_create(uint64_t width, uint64_t height) {
    struct pixel* data = malloc(width * height * sizeof(struct pixel));
    return (struct image) { .width = width, .height = height, .data = data };
}

void img_free(struct image img) {
    free(img.data);
}

struct image img_copy(struct image* img) {
    struct image img_copy = img_create(img->width, img->height);
    memcpy(img_copy.data, img->data, sizeof(struct pixel) * img->width * img->height);
    return img_copy;
}
