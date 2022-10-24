#include "sharpdisp/bitmap.h"

void bitmap_init(
    struct Bitmap* b,
    uint8_t* buff,
    uint16_t width,
    uint16_t height,
    uint8_t mode,
    uint8_t clear_byte) {
  b->data = buff;
  b->width = width;
  b->height = height;
  b->mode = mode;
  b->clear_byte = clear_byte;
  b->width_bytes = (width + 7) >> 3;
}
