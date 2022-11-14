#include <stdio.h>
#include "pico/stdlib.h"
#include <sharpdisp/sharpdisp.h>
#include <sharpdisp/bitmapimage.h>
#include <sharpdisp/bitmapshapes.h>
#include <sharpdisp/bitmaptext.h>
#include <sharpdisp/bitmap.h>
#include <sharpdisp/doublebuffer.h>
#include <fonts/liberation_mono_10.h>
#include "common.h"

#define USB_WAIT_MS 2000
#define WAIT_MS 4000
#define REFRESH_MS 32
#define TEXT_HEIGHT 10
#define XPAD 4

// Declare all tests here
struct TestData* test_bitmap_pnt(struct Bitmap* bitmap);
struct TestData* test_bitmap_apst1(struct Bitmap* bitmap);
struct TestData* test_bitmap_apst2(struct Bitmap* bitmap);
struct TestData* test_bitmap_gtst1(struct Bitmap* bitmap);
struct TestData* test_bitmap_gtst2(struct Bitmap* bitmap);
struct TestData* test_bitmap_clr0(struct Bitmap* bitmap);
struct TestData* test_bitmap_clr1(struct Bitmap* bitmap);
struct TestData* test_bitmap_copy1(struct Bitmap* bitmap);
struct TestData* test_bitmap_copy2(struct Bitmap* bitmap);
struct TestData* test_bitmap_copy3(struct Bitmap* bitmap);
struct TestData* test_bitmap_cpyr1(struct Bitmap* bitmap);
struct TestData* test_bitmap_cpyr2(struct Bitmap* bitmap);
struct TestData* test_bitmap_blit0(struct Bitmap* bitmap);
struct TestData* test_bitmap_blit1(struct Bitmap* bitmap);
struct TestData* test_text_char1(struct Bitmap* bitmap);
struct TestData* test_text_char2(struct Bitmap* bitmap);
struct TestData* test_text_char3(struct Bitmap* bitmap);
struct TestData* test_text_char4(struct Bitmap* bitmap);
struct TestData* test_text_char5(struct Bitmap* bitmap);
struct TestData* test_text_char6(struct Bitmap* bitmap);
struct TestData* test_text_char7(struct Bitmap* bitmap);
struct TestData* test_text_char8(struct Bitmap* bitmap);
struct TestData* test_text_char9(struct Bitmap* bitmap);
struct TestData* test_text_chr10(struct Bitmap* bitmap);
struct TestData* test_text_chr11(struct Bitmap* bitmap);
struct TestData* test_text_slen1(struct Bitmap* bitmap);
struct TestData* test_text_str1(struct Bitmap* bitmap);

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
  test_bitmap_pnt,
  test_bitmap_apst1,
  test_bitmap_apst2,
  test_bitmap_gtst1,
  test_bitmap_gtst2,
  test_bitmap_clr0,
  test_bitmap_clr1,
  test_bitmap_copy1,
  test_bitmap_copy2,
  test_bitmap_copy3,
  test_bitmap_cpyr1,
  test_bitmap_cpyr2,
  test_bitmap_blit0,
  test_bitmap_blit1,
  test_text_char1,
  test_text_char2,
  test_text_char3,
  test_text_char4,
  test_text_char5,
  test_text_char6,
  test_text_char7,
  test_text_char8,
  test_text_char9,
  test_text_chr10,
  test_text_chr11,
  test_text_slen1,
  test_text_str1,
};

struct DrawState {
  uint16_t x;
  uint16_t y;
} ds;

static inline uint32_t uptime_ms() {
  return to_ms_since_boot(get_absolute_time());
}

static void usb_wait(void) {
  const uint32_t tstart = uptime_ms();
  uint32_t tend = tstart + USB_WAIT_MS;
  for (uint32_t t=tstart; t < tend; t=uptime_ms()) {
    bitmap_clear(&db.bitmap);
    text.x = 50;
    text.y = DISPLAY_HEIGHT / 2;
    text_printf(&text, "WAIT FOR USB: %d/%d ms", t-tstart, USB_WAIT_MS);
    doublebuffer_swap(&db);
  }
  bitmap_clear(&db.bitmap);
}

static void init(void) {
  stdio_init_all();
  sleep_ms(100);  // wait for voltages to stabilize
  sharpdisp_init_default(&sd, disp_buffer, DISPLAY_WIDTH, DISPLAY_HEIGHT, 0x00);
  doublebuffer_init(&db, &sd, disp_buffer2, REFRESH_MS);
  text_init(&text, liberation_mono_10, &db.bitmap);
  text.printf_buffer = printf_buffer;
  usb_wait();
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
  ds.x += WIDTH + XPAD;
  if ((ds.x + WIDTH) > DISPLAY_WIDTH) {
    ds.x = 0;
    ds.y += HEIGHT + TEXT_HEIGHT;
  }
  // the 10 is for text height
  if ((ds.y + HEIGHT + TEXT_HEIGHT) > DISPLAY_HEIGHT) {
    ds.y = 0;
    doublebuffer_sleep_ms(&db, 0, WAIT_MS); 
    bitmap_clear(&db.bitmap);
  } else {
    sleep_ms(REFRESH_MS);
  }
}

static void draw_result(struct Bitmap* bitmap, const char* name, uint8_t errors) {
  db.bitmap.mode = BITMAP_WHITE;
  bitmap_blit(&db.bitmap, ds.x, ds.y, bitmap);
  if (errors) {
    bitmap_filled_rect(
      &db.bitmap, ds.x, ds.y + bitmap->height, bitmap->width, TEXT_HEIGHT);
  }
  db.bitmap.mode = BITMAP_INVERSE;
  text.x = ds.x;
  text.y = ds.y + bitmap->height;
  text_str(&text, name);
  doublebuffer_swap(&db);
  bitmap_copy(&db.bitmap, &sd.bitmap);
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
  return (errors > 0) ? 1 : 0;
} 

static uint8_t check_pixel_count(struct Bitmap* bitmap, uint16_t want) {
  uint16_t got = 0;
  const uint16_t width = bitmap->width;
  const uint16_t height = bitmap->height;
  // This code could be faster if it considered entire bytes.
  // But the gial here is not speed as much as simplicity as
  // it's a testing framework.
  for (uint16_t y=0; y<height; ++y) {
    for (uint16_t x=0; x<width; ++x) {
      if (bitmap_get_point_no_check(bitmap, x, y)) {
        ++got;
      }
    }
  }

  if (want != got) {
    printf("  Pixel Count: FAIL want=%d, got=%d\n", want, got);
  }

  return want != got;
}

static uint8_t test_a_sample(struct Bitmap* bitmap, const struct SamplePoint* p) {
  uint8_t want = p->value != 0;
  uint8_t got = bitmap_get_point(bitmap, p->x, p->y) != 0;
  if (want != got) {
    printf("  Sample: [%d,%d] = want=%d, got=%d FAIL\n", p->x, p->y, want, got);
  }
  return want != got; 
}

static uint8_t check_test_data(struct Bitmap* bitmap, struct TestData* test_data) {
  uint8_t errors = 0;
  printf("%s:\n", test_data->name);
  errors += check_eyecatchers(bitmap_buffer, "start");
  errors += check_eyecatchers(
    bitmap_buffer + sizeof(bitmap_buffer) - sizeof(eyecatcher_bytes), "end");
  errors += check_pixel_count(bitmap, test_data->on_count);
  for (uint16_t sample = 0; sample < test_data->num_samples; ++sample) {
    errors += test_a_sample(bitmap, test_data->samples + sample);
  }
  return (errors > 0) ? 1 : 0;
}

static uint8_t run_test(int index) {
  struct Bitmap bitmap;
  prepare_bitmap(&bitmap);
  struct TestData* test_data = tests[index](&bitmap);
  uint8_t errors = check_test_data(&bitmap, test_data);
  draw_result(&bitmap, test_data->name, errors);
  return errors;
}

static void final_status(uint32_t failures, uint32_t num_tests) {
  printf("Tests Completed: %d failures.  %d/%d passed\n",
      failures,
      (num_tests - failures),
      num_tests);
  
  text.x = 10;
  text.y = DISPLAY_HEIGHT - TEXT_HEIGHT * 2;
  bitmap_copy(&db.bitmap, &sd.bitmap);
  if (failures > 0) {
    text_printf(&text, "Test Completed: %d/%d FAILED.", failures, num_tests);
  } else {
    text_printf(&text, "ALL %d TESTS PASSED.", num_tests);
  }
  text.x = 10;
  text.y += TEXT_HEIGHT;
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
