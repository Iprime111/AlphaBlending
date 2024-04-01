#ifndef BITMAP_READER_HPP_
#define BITMAP_READER_HPP_

#include <cstdint>

#include "ErrorCode.hpp"
#include "BitmapDataTypes.hpp"

//WARNING works only with argb32 with rgba color masks : 0x00FF0000, 0x0000FF00, 0x000000FF, 0xFF000000
//WARNING imageSize parameter should be set

struct BitmapImage {
    BitmapHeader      header     = {};
    BitmapInfoHeader  infoHeader = {};
    BitmapV4Additions additions  = {};
    
    uint8_t *pixels = nullptr;
} __attribute__ ((packed));

ErrorCode InitBitmap         (BitmapImage *bitmap, int32_t width, int32_t height, uint16_t bitsPerPixel);
ErrorCode DestroyBitmap      (BitmapImage *bitmap);
ErrorCode LoadBitmapFromFile (BitmapImage *bitmap, const char *filename);
ErrorCode SaveBitmap         (BitmapImage *bitmap, const char *filename);


#endif
