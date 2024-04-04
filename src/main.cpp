#include <cstdio>
#include <cstdlib>
#include <unistd.h>
#include <libgen.h>

#include "BitmapReader/BitmapReader.hpp"
#include "Blenders/ArrayOptimizedBlender.hpp"
#include "Blenders/DefaultBlender.hpp"
#include "Blenders/SimdBlender.hpp"
#include "Config.hpp"
#include "ErrorCode.hpp"
#include "Timer.hpp"

static size_t    GetBlenderIndex         ();
static char     *GetApplicationDirectory ();
static ErrorCode LoadBitmaps             (BitmapImage *tableImage, BitmapImage *catImage);

int main () {   // TODO: there's lots of stuff is happening at main. Maybe rearrange it to other functions?
    BitmapImage tableImage = {};
    BitmapImage catImage   = {};

    if (LoadBitmaps (&tableImage, &catImage) != ErrorCode::NO_ERRORS) {
        fprintf (stderr, "Unable to load .bmp files\n");
        return 0;
    }

    size_t blenderIndex = GetBlenderIndex ();
    size_t batchSize    = 0; 

    #define BLENDER(FUNCTION, TITLE, INDEX, BATCH) if (blenderIndex == INDEX) {batchSize = BATCH;}

    #include "Blenders/Blenders.def"

    #undef BLENDER 

    InitTimers (TIMERS_COUNT);
    StartTimer (MAIN_TIMER);

    #define BLENDER(FUNCTION, TITLE, INDEX, ...)            \
        if (blenderIndex == INDEX) {                        \
            FUNCTION (&catImage, &tableImage, pixelIndex);  \
        }

    for (size_t test = 0; test < TESTS_COUNT; test++) {
        for (size_t pixelIndex = 0; pixelIndex < tableImage.infoHeader.imageSize; pixelIndex += BYTES_PER_PIXEL * batchSize) {
            #include "Blenders/Blenders.def"
        }
    }

    #undef BLENDER


    printf ("Test count: %lu\nTime: %lu\n", TESTS_COUNT, GetTimerValue (MAIN_TIMER));

    char *outputPath = (char *) calloc (FILENAME_MAX, sizeof (char));
    snprintf (outputPath, FILENAME_MAX, OUTPUT_IMAGE_PATH, getenv ("HOME"));

    SaveBitmap (&tableImage, outputPath);
    
    free (outputPath);
    DestroyBitmap (&tableImage);
    DestroyBitmap (&catImage);
    DestroyTimers ();


    return 0;
}

static size_t GetBlenderIndex () {
    #define BLENDER(FUNCTION, TITLE, INDEX, ...) printf ("[%d] - %s\n", INDEX, TITLE);

    printf ("Enter blender index:\n");

    #include "Blenders/Blenders.def"
    #undef BLENDER
    
    printf ("> ");

    size_t blenderIndex = 0;
    scanf ("%lu", &blenderIndex);

    while (blenderIndex >= BLENDERS_COUNT) {
        printf ("Incorrect index. Try again: ");
        scanf ("%lu", &blenderIndex);
    }

    return blenderIndex;
}

static char *GetApplicationDirectory () {
    char *applicationPath = (char *) calloc (FILENAME_MAX, sizeof (char));

    if (!applicationPath)
        return NULL;
    
    ssize_t count = readlink ("/proc/self/exe", applicationPath, FILENAME_MAX);
    
    return dirname (applicationPath);
}

static ErrorCode LoadBitmaps (BitmapImage *tableImage, BitmapImage *catImage) {
    #define LoadWithErrorCheck(IMAGE)                                                       \
        do {                                                                                \
            ErrorCode error_ = ErrorCode::NO_ERRORS;                                        \
            if ((error_ = LoadBitmapFromFile (IMAGE, imagePath)) != ErrorCode::NO_ERRORS) { \
                free (imagePath);                                                           \
                free (applicationDirectory);                                                \
                return error_;                                                              \
            }                                                                               \
        } while (0)

    char *applicationDirectory = GetApplicationDirectory ();
    
    if (!applicationDirectory)
        return ErrorCode::ALLOCATION_ERROR;

    char *imagePath = (char *) calloc (FILENAME_MAX, sizeof (char));

    if (!imagePath)
        return ErrorCode::ALLOCATION_ERROR;

    snprintf (imagePath, FILENAME_MAX, TABLE_IMAGE_PATH, applicationDirectory);
    LoadWithErrorCheck (tableImage);

    snprintf (imagePath, FILENAME_MAX, CAT_IMAGE_PATH,   applicationDirectory);
    LoadWithErrorCheck (catImage);

    free (imagePath);
    free (applicationDirectory);

    #undef LoadWithErrorCheck

    return ErrorCode::NO_ERRORS;
}
