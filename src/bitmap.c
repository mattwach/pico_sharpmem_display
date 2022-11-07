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

void bitmap_copy_rect(
  struct Bitmap* dest,
  const struct Bitmap* src,
  int16_t src_x,
  int16_t src_y) {
  const uint16_t num_columns = dest->width_bytes;
  const uint16_t num_rows = dest->height;
  uint8_t* dest_data = dest->data;
  for (uint16_t row = 0; row < num_rows; ++row) {
    for (uint16_t column = 0; column < num_columns; ++column, ++dest_data) {
      *dest_data = bitmap_get_stripe(
        src,
        src_x + (column << 3),
        src_y + row
      );
    }
  }
}

void bitmap_blit(
  struct Bitmap* dest,
  int16_t dest_x,
  int16_t dest_y,
  const struct Bitmap* src) {
  const uint16_t num_columns = src->width_bytes;
  const uint16_t num_rows = src->height;
  uint8_t* src_data = src->data;
  for (uint16_t row = 0; row < num_rows; ++row) {
    for (uint16_t column = 0; column < num_columns; ++column, ++src_data) {
      bitmap_apply_stripe(
        dest,
        dest_x,
        dest_y,
        *src_data);
    }
  }
}