#include "bmp.h"
#include "image.h"
#include <stdio.h>
#include <stdlib.h>

int main( int argc, char** argv ) {

    if (argc < 3 || argc > 3) {
        fprintf(stderr, "Wrong argument count\nUsage: image-transformer <source-image> <transformed-image>\n"); 
        exit(1);
    }
    
    struct image i1;
    if (!bmp_read(argv[1], &i1)) {
        fprintf(stderr, "Read error"); 
        exit(1);
    }

    struct image i2 = img_rot90(i1);
    if (!bmp_write(argv[2], &i2)) {
        fprintf(stderr, "Write error"); 
        exit(1);
    }

    img_free(i1);
    img_free(i2);

    return 0;
}
