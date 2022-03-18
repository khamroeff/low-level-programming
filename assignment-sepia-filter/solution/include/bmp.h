#ifndef _BMP_H_
#define _BMP_H_

#include "image.h"
#include <stdbool.h>

#define FOR_BMP_HEADER( FOR_FIELD ) \
        FOR_FIELD( uint16_t,bfType)\
        FOR_FIELD( uint32_t,bfileSize)\
        FOR_FIELD( uint32_t,bfReserved)\
        FOR_FIELD( uint32_t,bOffBits)\
        FOR_FIELD( uint32_t,biSize)\
        FOR_FIELD( uint32_t,biWidth)\
        FOR_FIELD( uint32_t,biHeight)\
        FOR_FIELD( uint16_t,biPlanes)\
        FOR_FIELD( uint16_t,biBitCount)\
        FOR_FIELD( uint32_t,biCompression)\
        FOR_FIELD( uint32_t,biSizeImage)\
        FOR_FIELD( uint32_t,biXPelsPerMeter)\
        FOR_FIELD( uint32_t,biYPelsPerMeter)\
        FOR_FIELD( uint32_t,biClrUsed)\
        FOR_FIELD( uint32_t,biClrImportant)

#define DECLARE_FIELD( t, n ) t n ;

struct __attribute__((packed)) bmp_header 
{
   FOR_BMP_HEADER( DECLARE_FIELD )
};

bool bmp_read(const char* filename, struct image* image);
bool bmp_write(const char* filename, struct image* image);

#endif
