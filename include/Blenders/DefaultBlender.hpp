#ifndef DEFAULT_BLENDER_HPP_
#define DEFAULT_BLENDER_HPP_

#include <cstddef>

#include "BitmapReader/BitmapReader.hpp"
#include "ErrorCode.hpp"

ErrorCode BlendBatchDefault (BitmapImage *source, BitmapImage *destination, size_t pixelIndex);

#endif
