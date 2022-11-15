#include "common.h"
#include <stdarg.h>
#include <stdio.h>
#include "hardware/structs/rosc.h"

// some good-enough random generation
int16_t rand16(int16_t min, int16_t max) {
  min *= 2;
  max *= 2;
  uint16_t v = 0x0000;
  for (int i=0; i<16; ++i, v<<=1) {
    if (rosc_hw->randombit) {
      v |= 0x0001;
    }
  }
  return (min + (v % (max - min))) / 2;
}

void assert_true(struct TestData* t, uint8_t condition, const char* fmt, ...) {
  if (!condition) {
    t->on_count = 10000;  // the real bitmap can not get this high
    printf("%s assert failure: ", t->name);
    va_list args;
    va_start(args, fmt);
    vprintf(fmt, args);
    va_end(args);
    printf("\n");
  }
}
