#ifndef SIMD_BLENDER_HPP_
#define SIMD_BLENDER_HPP_

#include <cstddef>

#include "BitmapReader/BitmapReader.hpp"
#include "ErrorCode.hpp"

ErrorCode BlendBatchSimd (BitmapImage *source, BitmapImage *destination, size_t pixelIndex);

#endif
