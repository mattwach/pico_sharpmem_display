#ifndef CONSTANTS_H
#define CONSTATNS_H

#include <inttypes.h>
#include <string.h>

#define DISPLAY_WIDTH 400
#define DISPLAY_HEIGHT 240
#define WIDTH 35
#define HEIGHT 25

struct SamplePoint {
  uint8_t x;
  uint8_t y;
  uint8_t value;  // 0 or 1
};

struct TestData {
  uint16_t on_count;  // number of pixels that should be active
  uint16_t num_samples;
  const struct SamplePoint samples[];
};

// Utility functions
int16_t rand16(int16_t min, int16_t max);
// assert with a printf-syntax for failure
void assert_true(struct TestData* t, uint8_t condition, const char* fmt, ...);

#endif

