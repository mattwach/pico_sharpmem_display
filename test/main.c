#include <stdio.h>
#include "pico/stdlib.h"
#include <sharpdisp/sharpdisp.h>
#include <sharpdisp/bitmapimage.h>
#include <sharpdisp/bitmap.h>
#include "constants.h"

// Declare all tests here
struct TestData* bitmap_clr0(struct TestData* test_data, struct Bitmap* bitmap);
struct TestData* bitmap_clr1(struct TestData* test_data, struct Bitmap* bitmap);

// Call all APIs that result in something being drawn to the display
// Then checks pixel counts and certain called-out pixel values

#define EYECATCHER_BYTES 4

uint8_t disp_buffer[BITMAP_SIZE(DISPLAY_WIDTH, DISPLAY_HEIGHT)];
uint8_t bitmap_buffer[BITMAP_SIZE(WIDTH, HEIGHT) + EYECATCHER_BYTES * 2];
struct SharpDisp sd;

struct TestData* (*tests[])(struct TestData* test_data, struct Bitmap*) = {
  bitmap_clr0,
  bitmap_clr1,
};

static void init(void) {
  stdio_init_all();
  sleep_ms(100);  // allow voltage to stabilize
  sharpdisp_init_default(&sd, disp_buffer, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0xFF);
  printf("Test Framework Initialized\n");
}

static void set_eyecatcher(uint8_t addr[]) {
  addr[0] = 0xAB;
  addr[1] = 0xCD;
  addr[2] = 0x12;
  addr[3] = 0x34;
}

static void prepare_bitmap(struct Bitmap* bitmap) {
  bitmap_init(
    bitmap,
    bitmap_buffer + EYECATCHER_BYTES,
    WIDTH,
    HEIGHT,
    BITMAP_WHITE,
    0x00);
  memset(bitmap_buffer, 0, sizeof(bitmap_buffer));
  // Eye catchers are to help detect buffer overruns/underruns
  set_eyecatcher(bitmap_buffer);
  set_eyecatcher(bitmap_buffer + sizeof(bitmap_buffer) - EYECATCHER_BYTES);
}

static uint8_t run_test(int index) {
  struct Bitmap bitmap;
  prepare_bitmap(&bitmap);
}

int main(void) {
  init();
  const int num_tests = sizeof(tests) / sizeof(tests[0]);
  uint32_t failures = 0;
  for (int i=0; i < num_tests; ++i) {
    failures += run_test(i);
  }
  printf("Tests Completed: %d failures.  %d/%d passed\n",
      failures,
      (num_tests - failures),
      num_tests);
  while (1) {
    sleep_ms(1000);
  }
}
