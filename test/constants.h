#ifndef CONSTANTS_H
#define CONSTATNS_H

#include <inttypes.h>
#include <string.h>

#define DISPLAY_WIDTH 400
#define DISPLAY_HEIGHT 240
#define WIDTH 35
#define HEIGHT 11

struct SamplePoint {
  uint8_t x;
  uint8_t y;
  uint8_t value;  // 0 or 1
};

struct TestData {
  const char* name; // 5 characters
  uint16_t on_count;  // number of pixels that should be active
  uint8_t num_samples;
  const struct SamplePoint* samples;
};

#endif

