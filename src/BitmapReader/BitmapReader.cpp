#include <cassert>
#include <climits>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include "BitmapReader/BitmapReader.hpp"
#include "BitmapReader/BitmapDataTypes.hpp"
#include "ErrorCode.hpp"

//NOTE width and height can have negative values according to .bmp specifications
ErrorCode InitBitmap (BitmapImage *bitmap, int32_t width, int32_t height, uint16_t bitsPerPixel) {
    assert (bitmap);

    bitmap->header     = {};
    bitmap->infoHeader = {};

    bitmap->header.dataOffset = sizeof (BitmapHeader) + sizeof (BitmapInfoHeader) + sizeof (BitmapV4Additions);
    bitmap->infoHeader.bitsPerPixel = (uint16_t) bitsPerPixel;

    bitmap->infoHeader.pictureWidth = width;

    uint32_t pixelsArraySize = (uint32_t) width * (uint32_t) height * bitsPerPixel / CHAR_BIT;

    bitmap->header.fileSize = pixelsArraySize + bitmap->header.dataOffset;
    bitmap->infoHeader.imageSize = (uint32_t) width * (uint32_t) height * bitsPerPixel / CHAR_BIT;

    bitmap->pixels = (uint8_t *) calloc (pixelsArraySize, sizeof (uint8_t));

    if (!bitmap->pixels)
        return ErrorCode::ALLOCATION_ERROR;

    return ErrorCode::NO_ERRORS;
}

ErrorCode DestroyBitmap (BitmapImage *bitmap) {
    if (!bitmap)
        return ErrorCode::NO_OBJECT;

    bitmap->header.signature = 0x0000;
    bitmap->header.fileSize  = 0;

    bitmap->infoHeader.bitsPerPixel  = 0;
    bitmap->infoHeader.pictureWidth  = 0;
    bitmap->infoHeader.pictureHeight = 0;

    free (bitmap->pixels);
    bitmap->pixels = nullptr;

    return ErrorCode::NO_ERRORS;
}

ErrorCode LoadBitmapFromFile (BitmapImage *bitmap, const char *filename) {
    assert (bitmap);
    assert (filename);

    #define DestroyVariables() do {fclose (bitmapFile); DestroyBitmap (bitmap);} while (0)

    FILE *bitmapFile = fopen (filename, "rb");

    if (!bitmapFile)
        return ErrorCode::FILE_IO_ERROR;

    fread (&bitmap->header, sizeof (BitmapHeader), 1, bitmapFile);

    if (bitmap->header.signature != DEFAULT_SIGNATURE) {
        DestroyVariables ();
        
        return ErrorCode::FILE_TYPE_ERROR;
    }

    fread (&bitmap->infoHeader, sizeof (BitmapInfoHeader), 1, bitmapFile);

    if (bitmap->infoHeader.compression != BmpCompression::BI_BITFIELDS || bitmap->infoHeader.bitsPerPixel != 32) {
        DestroyVariables ();

        return ErrorCode::BMP_SETTINGS_ERROR;
    }
    
    uint32_t pictureSize = bitmap->infoHeader.imageSize; 

    bitmap->pixels = (uint8_t *) calloc (pictureSize, sizeof (uint8_t));

    if (!bitmap->pixels) {
        DestroyVariables ();

        return ErrorCode::ALLOCATION_ERROR;
    }

    fseek (bitmapFile, bitmap->header.dataOffset, SEEK_SET);
    fread (bitmap->pixels, sizeof (uint8_t), pictureSize, bitmapFile);

    bitmap->infoHeader.infoHeaderSize = sizeof (BitmapInfoHeader) + sizeof (BitmapV4Additions);
    bitmap->header.dataOffset         = sizeof (BitmapHeader)     + bitmap->infoHeader.infoHeaderSize;
    bitmap->header.fileSize           = bitmap->header.dataOffset + pictureSize;

    fclose (bitmapFile);
    return ErrorCode::NO_ERRORS;
}

ErrorCode SaveBitmap (BitmapImage *bitmap, const char *filename) {
    assert (bitmap);
    assert (filename);

    FILE *bitmapFile = fopen (filename, "wb");
    
    if (!bitmapFile)
        return ErrorCode::FILE_IO_ERROR;

    fwrite (bitmap, sizeof (BitmapHeader) + sizeof (BitmapInfoHeader) + sizeof (BitmapV4Additions), 1, bitmapFile);

    fwrite (bitmap->pixels, sizeof (uint8_t), bitmap->infoHeader.imageSize, bitmapFile);

    fclose (bitmapFile);
    return ErrorCode::NO_ERRORS;
}

