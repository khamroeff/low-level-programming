#include <filter_sepia.h>
#include <bmp.h>
#include <image.h>
#include <sys/time.h>
#include <sys/resource.h>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

void calc_average_time (struct image* img){

    printf("\n\nTest average time:\n");

    struct rusage r;
    struct timeval start;
    struct timeval end;
    getrusage(RUSAGE_SELF, &r);
    start = r.ru_utime;

    struct image image_copy;

    for (size_t i = 0; i < 30; i++) {
        image_copy = img_copy(img);
        sepia_asm_inplace(&image_copy);
    }
             
    getrusage(RUSAGE_SELF, &r );
    end = r.ru_utime;
    long res = ((end.tv_sec - start.tv_sec) * 1000000L) +
        end.tv_usec - start.tv_usec;
    printf( "%ld microseconds for asm programm\n", res );

    getrusage(RUSAGE_SELF, &r);
    start = r.ru_utime;

    for (size_t i = 0; i < 30; i++) {
        image_copy = img_copy(img);
        sepia_c_inplace(&image_copy);
    }
     
    getrusage(RUSAGE_SELF, &r);         
    end = r.ru_utime;
    res = ((end.tv_sec - start.tv_sec) * 1000000L) +
        end.tv_usec - start.tv_usec;
    printf( "%ld microseconds for c programm\n", res );
      
}

void sepia_test (struct image* img1, struct image* img2){
    struct image img_avg = img_copy(img1);

    printf("\n\n Test asm programm: ");

    struct rusage r;
    struct timeval start;
    struct timeval end;
    getrusage(RUSAGE_SELF, &r );
    start = r.ru_utime;

    sepia_asm_inplace(img1);
             
    getrusage(RUSAGE_SELF, &r );
    end = r.ru_utime;
    long res = ((end.tv_sec - start.tv_sec) * 1000000L) +
        end.tv_usec - start.tv_usec;
    printf( "%ld microseconds\n", res );
    
    printf("\n Test c programm: ");
    getrusage(RUSAGE_SELF, &r );
    start = r.ru_utime;

    sepia_c_inplace(img2);
             
    getrusage(RUSAGE_SELF, &r );
    end = r.ru_utime;
    res = ((end.tv_sec - start.tv_sec) * 1000000L) +
        end.tv_usec - start.tv_usec;
    printf( "%ld microseconds\n", res );

    calc_average_time(&img_avg);
}
