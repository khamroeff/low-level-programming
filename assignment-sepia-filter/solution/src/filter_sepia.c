#include <image.h>
#include <inttypes.h>
#include <stdio.h>
#include <string.h>
#include <malloc.h>

extern void sepia_asm(struct pixel* old_pixels, struct pixel* new_pixels, uint64_t size);

float int_to_float[256];

static unsigned char sat( uint64_t x) {
 if (x < 256) return x; return 255;
}

static struct pixel* pixel_of( struct image* img, uint64_t x, uint64_t y) {
 return &(img->data[y * img->width + x]);
}

static void sepia_one( struct pixel* const pixel ) {
  static const float c[3][3] = {
  { .393f, .769f, .189f },
  { .349f, .686f, .168f },
  { .272f, .543f, .131f } };

  struct pixel const old = *pixel;
  pixel->r = sat( (uint64_t)((int_to_float[old.r]) * c[0][0] + (int_to_float[old.g]) * c[0][1] 
     + (int_to_float[old.b]) * c[0][2]));
  pixel->g = sat( (uint64_t)((int_to_float[old.r]) * c[1][0] + (int_to_float[old.g]) * c[1][1] 
     + (int_to_float[old.b]) * c[1][2]));
  pixel->b = sat( (uint64_t)((int_to_float[old.r]) * c[2][0] + (int_to_float[old.g]) * c[2][1] 
     + (int_to_float[old.b]) * c[2][2]));
}

void sepia_asm_inplace( struct image* img ) {
  uint64_t img_size = img->width * img->height;

  struct image img_old = img_copy(img);
    
  uint64_t iteration_count = (img_size - img_size % 4) * 3;
  sepia_asm(img_old.data, img->data, iteration_count);

  for ( size_t i = 0; i < img_size % 4; i++ ) {
    sepia_one(&img->data[iteration_count / 3 + i]);
  }

}

void sepia_c_inplace( struct image* img ) {

  for (int i = 0; i < 256; i++) {
    int_to_float[i] = (float) i;
  }

  uint32_t x,y;

  for( y = 0; y < img->height; y++ )
    for( x = 0; x < img->width; x++ )
      sepia_one( pixel_of( img, x, y ) );
}
