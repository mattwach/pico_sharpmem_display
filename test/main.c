#include <stdio.h>
#include "pico/stdlib.h"
#include <sharpdisp/sharpdisp.h>
#include <sharpdisp/bitmapimage.h>
#include <sharpdisp/bitmapshapes.h>
#include <sharpdisp/bitmaptext.h>
#include <sharpdisp/bitmap.h>
#include <sharpdisp/doublebuffer.h>
#include <fonts/liberation_mono_10.h>
#include "constants.h"

#define WAIT_MS 2000
#define REFRESH_MS 32

// Declare all tests here
struct TestData* bitmap_clr0(struct Bitmap* bitmap);
struct TestData* bitmap_clr1(struct Bitmap* bitmap);

// Call all APIs that result in something being drawn to the display
// Then checks pixel counts and certain called-out pixel values

const uint8_t eyecatcher_bytes[] = { 0xAB, 0xCD, 0x12, 0x34 };
uint8_t disp_buffer[BITMAP_SIZE(DISPLAY_WIDTH, DISPLAY_HEIGHT)];
uint8_t disp_buffer2[BITMAP_SIZE(DISPLAY_WIDTH, DISPLAY_HEIGHT)];
uint8_t bitmap_buffer[BITMAP_SIZE(WIDTH, HEIGHT) + sizeof(eyecatcher_bytes) * 2];
char printf_buffer[128];
struct SharpDisp sd;
struct DoubleBuffer db;
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
  doublebuffer_init(&db, &sd, disp_buffer2, REFRESH_MS);
  text_init(&text, liberation_mono_10, &db.bitmap);
  text.printf_buffer = printf_buffer;
  printf("Test Framework Initialized\n");
}

static void set_eyecatcher(uint8_t addr[]) {
  for (int idx=0; idx < sizeof(eyecatcher_bytes); ++idx) {
    addr[idx] = eyecatcher_bytes[idx];
  }
}

static void prepare_bitmap(struct Bitmap* bitmap) {
  bitmap_init(
    bitmap,
    bitmap_buffer + sizeof(eyecatcher_bytes),
    WIDTH,
    HEIGHT,
    BITMAP_WHITE,
    0x00);
  memset(bitmap_buffer, 0, sizeof(bitmap_buffer));
  // Eye catchers are to help detect buffer overruns/underruns
  set_eyecatcher(bitmap_buffer);
  set_eyecatcher(bitmap_buffer + sizeof(bitmap_buffer) - sizeof(eyecatcher_bytes));
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
    doublebuffer_sleep_ms(&db, 0, WAIT_MS); 
    bitmap_clear(&db.bitmap);
  } else {
    sleep_ms(REFRESH_MS);
  }
}

static void draw_result(struct Bitmap* bitmap, const char* name) {
  db.bitmap.mode = bitmap->clear_byte ? BITMAP_WHITE : BITMAP_BLACK;
  bitmap_filled_rect(&db.bitmap, ds.x, ds.y, bitmap->width, bitmap->height);
  db.bitmap.mode = bitmap->clear_byte ? BITMAP_BLACK : BITMAP_WHITE;
  bitmap_blit(&db.bitmap, ds.x, ds.y, bitmap);
  db.bitmap.mode = BITMAP_WHITE;
  text.x = ds.x;
  text.y = ds.y + bitmap->height;
  text_str(&text, name);
  doublebuffer_swap(&db);
  increment_display_slot();
}

static uint8_t check_eyecatchers(const uint8_t* data, const char* context) {
  uint8_t errors = 0;
  for (int idx = 0; idx < sizeof(eyecatcher_bytes); ++idx) {
    if (data[idx] != eyecatcher_bytes[idx]) {
      printf("  Eyecatcher %s, byte %d FAIL (want=0x%02X, got=0x%02X)\n",
          context,
          idx,
          eyecatcher_bytes[idx],
          data[idx]);
      ++errors;
    }
  }
  if (errors == 0) {
    printf("  Eyecatcher %s: OK\n", context);
  }
  return (errors > 0) ? 1 : 0;
} 

static uint8_t check_test_data(struct Bitmap* bitmap, struct TestData* test_data) {
  uint8_t errors = 0;
  printf("%s:\b", test_data->name);
  errors += check_eyecatchers(bitmap->data, "start");
  errors += check_eyecatchers(
    bitmap->data + sizeof(bitmap_buffer) - sizeof(eyecatcher_bytes), "end");

  return (errors > 0) ? 1 : 0;
}

static uint8_t run_test(int index) {
  struct Bitmap bitmap;
  prepare_bitmap(&bitmap);
  struct TestData* test_data = tests[index](&bitmap);
  draw_result(&bitmap, test_data->name);
  return check_test_data(&bitmap, test_data);
}

static void final_status(uint32_t failures, uint32_t num_tests) {
  if (ds.x != 0 || ds.y != 0) {
    doublebuffer_sleep_ms(&db, 0, WAIT_MS);
  }
  printf("Tests Completed: %d failures.  %d/%d passed\n",
      failures,
      (num_tests - failures),
      num_tests);
  
  bitmap_clear(&db.bitmap);
  text.x = 10;
  text.y = DISPLAY_HEIGHT / 2;
  text_printf(&text, "Test Completed: %d failures.", failures);
  text.x = 10;
  text.y += 10;
  text_printf(&text, "Connect a USB console for more details.", failures);
  doublebuffer_swap(&db);
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
  final_status(failures, num_tests);
  while (1) {
    doublebuffer_refresh(&db);
  }
}
