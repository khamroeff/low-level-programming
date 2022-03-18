#ifndef FILTER_SEPIA_H
#define FILTER_SEPIA_H

#include <image.h>

void sepia_c_inplace( struct image* img );
void sepia_asm_inplace( struct image* img );

#endif
