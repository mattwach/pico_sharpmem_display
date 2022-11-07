#include <stdio.h>
#include "pico/stdlib.h"
#include <sharpdisp/sharpdisp.h>
#include <sharpdisp/bitmapimage.h>
#include <sharpdisp/bitmapshapes.h>
#include <sharpdisp/bitmaptext.h>
#include <sharpdisp/bitmap.h>
#include <fonts/liberation_mono_10.h>
#include "constants.h"

#define WAIT_MS 2000
#define REFRESH_MS 10

// Declare all tests here
struct TestData* bitmap_clr0(struct Bitmap* bitmap);
struct TestData* bitmap_clr1(struct Bitmap* bitmap);

// Call all APIs that result in something being drawn to the display
// Then checks pixel counts and certain called-out pixel values

#define EYECATCHER_BYTES 4

uint8_t disp_buffer[BITMAP_SIZE(DISPLAY_WIDTH, DISPLAY_HEIGHT)];
uint8_t bitmap_buffer[BITMAP_SIZE(WIDTH, HEIGHT) + EYECATCHER_BYTES * 2];
struct SharpDisp sd;
struct BitmapText text;

struct TestData* (*tests[])(struct Bitmap*) = {
  bitmap_clr0,
  bitmap_clr1,
};

struct DrawState {
  uint16_t x;
  uint16_t y;
} ds;

static void init(void) {
  stdio_init_all();
  sleep_ms(100);  // allow voltage to stabilize
  sharpdisp_init_default(&sd, disp_buffer, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0xFF);
  text_init(&text, liberation_mono_10, &sd.bitmap);
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

static inline uint32_t uptime_ms() {
  return to_ms_since_boot(get_absolute_time());
}

static void refresh_and_wait(void) {
  uint32_t done_time_ms = uptime_ms() + WAIT_MS;
  while (uptime_ms() < done_time_ms) {
    sharpdisp_refresh(&sd);
    sleep_ms(REFRESH_MS);
  }
  // clear for the next round
  bitmap_clear(&sd.bitmap);
}

static void increment_display_slot(void) {
  ds.x += WIDTH;
  if ((ds.x + WIDTH) > DISPLAY_WIDTH) {
    ds.x = 0;
    ds.y += HEIGHT;
  }
  // the 10 is for text height
  if ((ds.y + HEIGHT + 10) > DISPLAY_HEIGHT) {
    ds.y = 0;
    refresh_and_wait();
  } else {
    sharpdisp_refresh(&sd);
    sleep_ms(REFRESH_MS);
  }
}

static void draw_result(struct Bitmap* bitmap, const char* name) {
  sd.bitmap.mode = bitmap->clear_byte ? BITMAP_WHITE : BITMAP_BLACK;
  bitmap_filled_rect(&sd.bitmap, ds.x, ds.y, bitmap->width, bitmap->height);
  sd.bitmap.mode = bitmap->clear_byte ? BITMAP_BLACK : BITMAP_WHITE;
  bitmap_blit(&sd.bitmap, ds.x, ds.y, bitmap);
  sd.bitmap.mode = BITMAP_WHITE;
  text.x = ds.x;
  text.y = ds.y + bitmap->height;
  text_str(&text, name);
  increment_display_slot();
}

static uint8_t run_test(int index) {
  struct Bitmap bitmap;
  prepare_bitmap(&bitmap);
  struct TestData* test_data = tests[index](&bitmap);
  draw_result(&bitmap, test_data->name);
  return 0;
}

int main(void) {
  init();
  const int num_tests = sizeof(tests) / sizeof(tests[0]);
  uint32_t failures = 0;
  ds.x = 0;
  ds.y = 0;
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
