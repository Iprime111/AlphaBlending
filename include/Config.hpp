#ifndef CONFIG_HPP_
#define CONFIG_HPP_

#include <cstddef>

const size_t TIMERS_COUNT    = 1;
const size_t MAIN_TIMER      = 0;
const size_t PIXEL_BATCH     = 8;
const size_t BYTES_PER_PIXEL = 4;
const size_t TESTS_COUNT     = 10000;
const size_t BLENDERS_COUNT  = 3;

const char CAT_IMAGE_PATH    [] = "%s/img/cat.bmp";
const char TABLE_IMAGE_PATH  [] = "%s/img/table.bmp";

const char OUTPUT_IMAGE_PATH [] = "%s/Downloads/out.bmp";
#endif
