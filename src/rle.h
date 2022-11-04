#ifndef SHARPDISP_RLE_H
#define SHARPDISP_RLE_H

#include <inttypes.h>

struct RLETracker {
  const uint8_t* pgm_data;  // address of next byte
  uint8_t bytes_remaining;  // number of bytes remaining on the current run
  uint8_t repeat_mode;  // if true, then we are in repeat mode
};

uint8_t next_rle_byte(struct RLETracker* self, uint8_t* err);

#endif