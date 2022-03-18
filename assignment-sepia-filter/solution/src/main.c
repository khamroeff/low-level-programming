#include "bmp.h"
#include "image.h"
#include "filter_sepia.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main( int argc, char** argv ) {

    if (argc < 4 || argc > 4) {
        fprintf(stderr, "Wrong argument count\nUsage: image-transformer <source-image> <transformed-image> <language (c / asm)>\n"); 
        exit(1);
    }
    
    struct image i1;
    if (!bmp_read(argv[1], &i1)) {
        fprintf(stderr, "Read error"); 
        exit(1);
    }

    if (strcmp(argv[3], "c") == 0) {
        sepia_asm_inplace(&i1);
    } else if (strcmp(argv[3], "asm") == 0) {
        sepia_asm_inplace(&i1);
    } else {
        fprintf(stderr, "Wrong 'language' argument\n"); 
        exit(1);
    }

    if (!bmp_write(argv[2], &i1)) {
        fprintf(stderr, "Write error"); 
        exit(1);
    }

    img_free(i1);

    return 0;
}
