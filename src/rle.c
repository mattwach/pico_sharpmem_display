#include "rle.h"
#include "sharpdisp/bitmaptext.h"

uint8_t next_rle_byte(struct RLETracker* self, uint8_t* err) {
  if (*err) {
    return 0xFF;
  }
  if (self->bytes_remaining == 0) {
    // prep for the next sequence
    self->bytes_remaining = self->pgm_data[0];
    ++self->pgm_data;
    if (self->bytes_remaining & 0x80) {
      self->bytes_remaining &= 0x7F;
      self->repeat_mode = 0;
    } else {
      self->repeat_mode = 1;
    }

    if (self->bytes_remaining == 0) {
      // data is invalid
      *err = TEXT_INVALID_RLE_DATA;
      return 0xFF;
    }
  }

  const uint8_t b = self->pgm_data[0];
  --self->bytes_remaining;
  if (!self->repeat_mode || (self->bytes_remaining == 0)) {
    ++self->pgm_data;
  }
  return b;
}
