#include <cstdio>

#include "BitmapReader/BitmapReader.hpp"
#include "Blenders/DefaultBlender.hpp"

int main () {
    BitmapImage tableImage = {};
    BitmapImage catImage   = {};

    LoadBitmapFromFile (&tableImage, "/home/ip111/Downloads/table.bmp");
    LoadBitmapFromFile (&catImage,   "/home/ip111/Downloads/cat.bmp");

    for (size_t pixelIndex = 0; pixelIndex < tableImage.infoHeader.imageSize; pixelIndex += 4) {
        BlendBatch (&catImage, &tableImage, pixelIndex);
    }

    SaveBitmap (&tableImage, "/home/ip111/Downloads/aaa.bmp");
    SaveBitmap (&catImage,   "/home/ip111/Downloads/bbb.bmp");

    DestroyBitmap (&tableImage);
    DestroyBitmap (&catImage);

    return 0;
}
