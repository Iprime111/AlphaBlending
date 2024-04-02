#include <cstdio>

#include "BitmapReader/BitmapReader.hpp"
#include "Blenders/ArrayOptimizedBlender.hpp"
#include "Blenders/DefaultBlender.hpp"
#include "Blenders/SimdBlender.hpp"
#include "Config.hpp"
#include "Timer.hpp"

int main () {
    BitmapImage tableImage = {};
    BitmapImage catImage   = {};

    LoadBitmapFromFile (&tableImage, "/home/ip111/Downloads/table.bmp");
    LoadBitmapFromFile (&catImage,   "/home/ip111/Downloads/cat.bmp");

    InitTimers (TIMERS_COUNT);

    StartTimer (0);
    for (size_t test = 0; test < 1000; test++) {
        for (size_t pixelIndex = 0; pixelIndex < tableImage.infoHeader.imageSize; pixelIndex += 4 ) {
            BlendBatchDefault (&catImage, &tableImage, pixelIndex);
        }
    }
    printf ("%lu\n", GetTimerValue (0));

    StartTimer (0);
    for (size_t test = 0; test < 1000; test++) {
        for (size_t pixelIndex = 0; pixelIndex < tableImage.infoHeader.imageSize; pixelIndex += 4 ) {
            BlendBatchArrayOptimized (&catImage, &tableImage, pixelIndex);
        }
    }
    printf ("%lu\n", GetTimerValue (0));


    StartTimer (MAIN_TIMER);

    for (size_t test = 0; test < 1000; test++) {
        for (size_t pixelIndex = 0; pixelIndex < tableImage.infoHeader.imageSize; pixelIndex += 4 * PIXEL_BATCH) {
            BlendBatchSimd (&catImage, &tableImage, pixelIndex);
        }
    }
    printf ("%lu\n", GetTimerValue (MAIN_TIMER));

    SaveBitmap (&tableImage, "/home/ip111/Downloads/aaa.bmp");

    DestroyBitmap (&tableImage);
    DestroyBitmap (&catImage);
    DestroyTimers ();

    return 0;
}
