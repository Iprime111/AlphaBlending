#ifndef BITMAP_DATA_TYPES_HPP_
#define BITMAP_DATA_TYPES_HPP_

#include <cstdint>

const uint16_t DEFAULT_SIGNATURE = 0x4d42;

enum class BmpCompression : uint32_t {
    BI_RGB            = 0,
    BI_RLE8           = 1,
    BI_RLE4           = 2,
    BI_BITFIELDS      = 3,
    BI_JPEG           = 4,
    BI_PNG            = 5,
    BI_ALPHABITFIELDS = 6,
    BI_CMYK           = 11, // TODO: since when 6 + 1 = 11? https://imgur.com/a/BhW4gAa
    BI_CMYKRLE8       = 12,
    BI_CMYKRLE4       = 13,
};

struct BitmapHeader {
    uint16_t signature  = DEFAULT_SIGNATURE;
    uint32_t fileSize   = 0;
    uint16_t reserved1  = 0;
    uint16_t reserved2  = 0;
    uint32_t dataOffset = 0;
} __attribute__((packed));

struct BitmapInfoHeader {
    uint32_t       infoHeaderSize  = sizeof (BitmapInfoHeader);
    int32_t       pictureWidth     = 0; // TODO not properly aligned
    int32_t        pictureHeight   = 0;
    uint16_t       planesNumber    = 1;
    uint16_t       bitsPerPixel    = 32;
    BmpCompression compression     = BmpCompression::BI_BITFIELDS;
    uint32_t       imageSize       = 0;
    int32_t        horizontalRes   = 0;
    int32_t        verticalRes     = 0;
    uint32_t       paletteSize     = 0;
    uint32_t       importantColors = 0;
} __attribute__((packed));

struct BitmapV4Additions {
    uint32_t redMask    = 0x00ff0000;
    uint32_t greenMask  = 0x0000ff00;
    uint32_t blueMask   = 0x000000ff;
    uint32_t alphaMask  = 0xff000000;
    uint32_t colorSpace = 0x57696e20; // Windows color space
    
    uint8_t  colorSpaceEndpoints [0x24] = {};

    uint32_t redGamma   = 0;
    uint32_t greenGamma = 0;
    uint32_t blueGamma  = 0;
    
} __attribute__ ((packed));

#endif
