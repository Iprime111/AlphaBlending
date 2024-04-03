#include <cstddef>
#include <cstdint>

#include "Blenders/ArrayOptimizedBlender.hpp"
#include "BitmapReader/BitmapReader.hpp"
#include "Config.hpp"
#include "ErrorCode.hpp"

const static size_t BATCH_RGB_SIZE  = BYTES_PER_PIXEL - 1;

#define Foreach(EXPRESSION) do {for (size_t colorChannel = 0; colorChannel < BATCH_RGB_SIZE; colorChannel++) {EXPRESSION;}} while (0)

ErrorCode BlendBatchArrayOptimized (BitmapImage *source, BitmapImage *destination, size_t pixelIndex) {
    
    uint8_t alpha         = source->pixels [pixelIndex + 3];
    uint8_t reversedAlpha = 255 - alpha;
    
    #define channelIndex pixelIndex + colorChannel

    uint16_t dstPixel [BATCH_RGB_SIZE] = {}; Foreach (dstPixel [colorChannel] = destination->pixels [channelIndex]);
    uint16_t srcPixel [BATCH_RGB_SIZE] = {}; Foreach (srcPixel [colorChannel] = source->pixels      [channelIndex]);
    uint16_t dstMult  [BATCH_RGB_SIZE] = {}; Foreach (dstMult  [colorChannel] = dstPixel [colorChannel] * reversedAlpha);
    uint16_t srcMult  [BATCH_RGB_SIZE] = {}; Foreach (srcMult  [colorChannel] = srcPixel [colorChannel] * alpha);
    uint16_t sumPixel [BATCH_RGB_SIZE] = {}; Foreach (sumPixel [colorChannel] = dstPixel [colorChannel] + srcPixel [colorChannel]);
    uint16_t resPixel [BATCH_RGB_SIZE] = {}; Foreach (resPixel [colorChannel] = sumPixel [colorChannel] / 255);

    Foreach (destination->pixels [channelIndex] = resPixel [colorChannel]);

    destination->pixels [pixelIndex + 3] = 255;

    #undef channelIndex

    return ErrorCode::NO_ERRORS;
}
