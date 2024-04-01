#include <cstddef>
#include <cstdint>

#include "Blenders/DefaultBlender.hpp"
#include "BitmapReader/BitmapReader.hpp"
#include "ErrorCode.hpp"

const size_t BYTES_PER_PIXEL = 4;

ErrorCode BlendBatch (BitmapImage *source, BitmapImage *destination, size_t pixelIndex) {
    
    uint8_t alpha = source->pixels [pixelIndex + 3];

    #define channelIndex pixelIndex + colorChannel

    destination->pixels [pixelIndex + 3] = 255;

    for (size_t colorChannel = 0; colorChannel < BYTES_PER_PIXEL - 1; colorChannel++) {
        destination->pixels [channelIndex] = (uint8_t) (destination->pixels [channelIndex] * (255 - alpha) / 255 + source->pixels [channelIndex] * alpha / 255); 
    }

    #undef channelIndex

    return ErrorCode::NO_ERRORS;
}
