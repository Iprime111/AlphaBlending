#include <cstddef>
#include <emmintrin.h>
#include <immintrin.h>

#include "BitmapReader/BitmapReader.hpp"
#include "Blenders/SimdBlender.hpp"
#include "ErrorCode.hpp"

#define SrcByte(INDEX) ImageByte(source, INDEX)
#define DstByte(INDEX) ImageByte (destination, INDEX)
#define ImageByte(IMAGE, INDEX) (char) (IMAGE)->pixels [pixelIndex + INDEX]

// shuffle mask for copying alpha value from alpha channel to rgb channels (made for 128-bit long register)
#define AlphaCopyMask 1, 14, 1, 14, 1, 14, 1, 14, 1, 6, 1, 6, 1, 6, 1, 6 

static const __m512i AlphaSuffleMask = _mm512_set_epi8 (AlphaCopyMask, AlphaCopyMask, AlphaCopyMask, AlphaCopyMask);

ErrorCode BlendBatchSimd (BitmapImage *source, BitmapImage *destination, size_t pixelIndex) {

    // NOTE can not load pixels directly from memory 'cause BitmapImage alignment is off for .bmp format compatability

    __m256i srcPixels = _mm256_set_epi8 (SrcByte (31), SrcByte (30), SrcByte (29), SrcByte (28), 
                                         SrcByte (27), SrcByte (26), SrcByte (25), SrcByte (24), 
                                         SrcByte (23), SrcByte (22), SrcByte (21), SrcByte (20), 
                                         SrcByte (19), SrcByte (18), SrcByte (17), SrcByte (16), 
                                         SrcByte (15), SrcByte (14), SrcByte (13), SrcByte (12), 
                                         SrcByte (11), SrcByte (10), SrcByte (9),  SrcByte (8), 
                                         SrcByte (7),  SrcByte (6),  SrcByte (5),  SrcByte (4), 
                                         SrcByte (3),  SrcByte (2),  SrcByte (1),  SrcByte (0));

    __m256i dstPixels = _mm256_set_epi8 (DstByte (31), DstByte (30), DstByte (29), DstByte (28), 
                                         DstByte (27), DstByte (26), DstByte (25), DstByte (24),
                                         DstByte (23), DstByte (22), DstByte (21), DstByte (20),
                                         DstByte (19), DstByte (18), DstByte (17), DstByte (16), 
                                         DstByte (15), DstByte (14), DstByte (13), DstByte (12),
                                         DstByte (11), DstByte (10), DstByte (9),  DstByte (8), 
                                         DstByte (7),  DstByte (6),  DstByte (5),  DstByte (4), 
                                         DstByte (3),  DstByte (2),  DstByte (1),  DstByte (0));

    __m512i srcExt = _mm512_cvtepu8_epi16 (srcPixels);
    __m512i dstExt = _mm512_cvtepu8_epi16 (dstPixels);

    __mmask32 alphaMask   = _cvtu32_mask32 (0x88888888);
    __m512i alpha         =  _mm512_mask_blend_epi16 (alphaMask, _mm512_set1_epi16 (0), srcExt);

    __m512i alphaShuffled = _mm512_shuffle_epi8 (alpha, AlphaSuffleMask);

    __m512i resultSrc     = _mm512_mullo_epi16 (srcExt, alphaShuffled);
    __m512i resultDst     = _mm512_mullo_epi16 (dstExt, _mm512_sub_epi16(_mm512_set1_epi16 (0xff), alphaShuffled));

    __m512i imagesSum     = _mm512_add_epi16 (resultSrc, resultDst);

    //NOTE division by 255 algo is taken from https://stackoverflow.com/questions/35285324/how-to-divide-16-bit-integer-by-255-with-using-sse/
    __m512i result        = _mm512_srli_epi16 (_mm512_add_epi16 (_mm512_add_epi16 (imagesSum, _mm512_set1_epi16(1)), 
                                                             _mm512_srli_epi16 (imagesSum, 8)), 8);

    __m256i convertedResult = _mm512_cvtepi16_epi8 (result);
    
    _mm256_storeu_epi8 (&destination->pixels [pixelIndex], convertedResult);

    for (size_t alphaIndex = 0; alphaIndex < 8 * 4; alphaIndex += 4) {
        destination->pixels [pixelIndex + alphaIndex + 3] = 0xff;
    }

    return ErrorCode::NO_ERRORS;
}
