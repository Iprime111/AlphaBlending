#ifndef DEFAULT_BLENDER_HPP_
#define DEFAULT_BLENDER_HPP_

#include <cstddef>

#include "BitmapReader/BitmapReader.hpp"
#include "ErrorCode.hpp"

ErrorCode BlendBatch (BitmapImage *source, BitmapImage *destination, size_t pixelIndex);

#endif
