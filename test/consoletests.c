#include "common.h"
#include "sharpdisp/bitmapconsole.h"
#include "sharpdisp/bitmaptext.h"
#include "fonts/liberation_mono_10.h"

static struct TestData shapes_cons1_data = {36, 0, {}};
struct TestData* test_shapes_cons1(struct Bitmap* bitmap) {
  struct BitmapConsole bc;
  struct BitmapText text;
  text_init(&text, liberation_mono_10, bitmap);
  bitmap_console_init(&bc, &text);
  bitmap_console_char(&bc, 'A');
  bitmap_console_char(&bc, '\n');
  bitmap_console_char(&bc, 'B');
  return &shapes_cons1_data;
}

static struct TestData shapes_cons2_data = {94, 0, {}};
struct TestData* test_shapes_cons2(struct Bitmap* bitmap) {
  struct BitmapConsole bc;
  struct BitmapText text;
  text_init(&text, liberation_mono_10, bitmap);
  bitmap_console_init(&bc, &text);
  bitmap_console_strlen(&bc, "Hello\n", 6);
  bitmap_console_strlen(&bc, "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n", 6);
  return &shapes_cons2_data;
}

static struct TestData shapes_cons3_data = {15, 0, {}};
struct TestData* test_shapes_cons3(struct Bitmap* bitmap) {
  struct BitmapConsole bc;
  struct BitmapText text;
  text_init(&text, liberation_mono_10, bitmap);
  bitmap_console_init(&bc, &text);
  bitmap_console_str(&bc, "Hello\n");
  bitmap_console_str(&bc, "ABCDEFGHIJKLMNOPQRSTUVWXYZ\n");
  return &shapes_cons3_data;
}

static struct TestData shapes_cons4_data = {37, 0, {}};
struct TestData* test_shapes_cons4(struct Bitmap* bitmap) {
  char printf_buffer[80];
  struct BitmapConsole bc;
  struct BitmapText text;
  text_init(&text, liberation_mono_10, bitmap);
  text.printf_buffer = printf_buffer;
  bitmap_console_init(&bc, &text);
  for (uint8_t i=0; i<10; ++i) {
    bitmap_console_printf(&bc, "#%d\n", i);
  }
  return &shapes_cons4_data;
}
