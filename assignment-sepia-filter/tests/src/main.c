#include <bmp.h>
#include "../include/tests.h"
#include <string.h>

int main( int argc, char** argv ) {

    if (argc < 4 || argc > 4) {
        fprintf(stderr, "Wrong argument count\n"); 
        return 1;
    }
    
    struct image i1;
    if (!bmp_read(argv[1], &i1)) {
        fprintf(stderr, "Read error"); 
        return 1;
    }

    struct image i2 = img_copy(&i1);
    sepia_test(&i1, &i2);

    if (!bmp_write(argv[2], &i1)) {
        fprintf(stderr, "Write error"); 
        return 1;
    }

    if (!bmp_write(argv[3], &i2)) {
        fprintf(stderr, "Write error"); 
        return 1;
    }

    img_free(i1);
    img_free(i2);

    return 0;
}

