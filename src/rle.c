#include "rle.h"

static void map_image_byte(
    struct Bitmap* b,
    int16_t x,
    int16_t y,
    uint8_t data) {
  if (!data) {
    return;
  }
  if ((y < 0) || (y >= b->height)) {
    // off the top or bottom
    return;
  }
  if (x < 0) {
    if (x <= -8) {
      // the entire byte is off-screen
      return;
    } else {
      // part of the byte is off-screen.  Thus we need to shift bits up.
      data <<= -x;
      x = 0;
    }
  }

  const uint8_t bit_offset = x & 0x07;
  if (bit_offset) {
    // not on an 8-bit boundary, therefore we need to execute this as two mappings
    map_image_byte(b, x & 0xFFF8, y, data >> bit_offset);
    map_image_byte(b, (x & 0xFFF8) + 8 , y, data << (8 - bit_offset));
    return;
  }

  // thanks to the logic above, we can now assume that x is on an 8-bit boundary
  uint16_t col = x >> 3;
  if (col >= b->width_bytes) {
    // off the edge
    return;
  }

  uint8_t* addr = b->data + (y * b->width_bytes) + col;
  bitmap_apply(addr, b->mode, data);
}

void map_rle_image(
    struct Bitmap* bitmap,
    const uint8_t* pgm_data,
    int16_t x,
    int16_t y,
    uint16_t width,
    uint16_t height,
    uint8_t* error
) {
  if (*error) {
    return;
  }

  if (!pgm_data) {
    return;
  }

  uint8_t bytes_remaining = 0;  // number of bytes remaining on the current run
  uint8_t repeat_mode = 0;
  const uint16_t num_cols = (width + 7) >> 3;
  uint8_t rle_byte = 0;

  // vertical stripes
  for (uint16_t col = 0; col < num_cols; ++col) {
    // each strip is height in length
    for (uint16_t row = 0; row < height; ++row) {
      if (bytes_remaining == 0) {
        // grab the next control byte
        bytes_remaining = *(pgm_data++);
        if (bytes_remaining & 0x80) {
          // a non-repeating sequence
          bytes_remaining &= 0x7F;
          repeat_mode = 0;
        } else {
          // a repeating sequence
          rle_byte = *pgm_data;
          repeat_mode = 1;
        }

        if (bytes_remaining == 0) {
          // data is invalid
          *error = RLE_BAD_DATA;
          return;
        }
      }

      --bytes_remaining;
      if (!repeat_mode || (bytes_remaining == 0)) {
        rle_byte = *(pgm_data++);
      }

      map_image_byte(
          bitmap,
          x + (col * 8),
          y + row,
          rle_byte);
    }
  }

}