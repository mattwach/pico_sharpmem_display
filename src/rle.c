#include "rle.h"

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
          if (rle_byte == 0x00) {
            // Optimization: When the byte is zero, the destination bitmap
            // is not changed at all.   Thus we can skip over the process of
            // even trying.
            row += bytes_remaining - 1;  // account for the increment inthe for loop above
            for (; row >= height; row -= height, ++col);
            // and skip to the next byte
            ++pgm_data;
            bytes_remaining = 0;
            continue;
          }
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

      bitmap_apply_stripe(
          bitmap,
          x + (col * 8),
          y + row,
          rle_byte);
    }
  }

}
