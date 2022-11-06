#include "sharpdisp/bitmap.h"
#include "string.h"

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

void bitmap_copy(struct Bitmap* dest, const struct Bitmap* src) {
  if (dest->width_bytes != src->width_bytes) {
    return;
  }
  if (dest->data == src->data) {
    return;
  }
  const uint16_t height = dest->height <= src->height ? dest->height : src->height;
  memcpy(dest->data, src->data, height * dest->width_bytes);
}