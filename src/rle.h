#ifndef SHARPDISP_RLE_H
#define SHARPDISP_RLE_H

#include <inttypes.h>
#include "sharpdisp/bitmap.h"

#define RLE_BAD_DATA            0x02

void map_rle_image(
    struct Bitmap* bitmap,
    const uint8_t* pgm_data,
    int16_t x,
    int16_t y,
    uint16_t width,
    uint16_t height,
    uint8_t* error
);

#endif