#ifndef ARRAY_OPTIMIZED_BLENDER_HPP_
#define ARRAY_OPTIMIZED_BLENDER_HPP_

#include <cstddef>

#include "BitmapReader/BitmapReader.hpp"
#include "ErrorCode.hpp"

ErrorCode BlendBatchArrayOptimized (BitmapImage *source, BitmapImage *destination, size_t pixelIndex);

#endif
