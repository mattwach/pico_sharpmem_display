#include "common.h"
#include "sharpdisp/bitmap.h"
#include "sharpdisp/bitmaptext.h"
#include "fonts/liberation_sans_18.h"

// Tests the bitmap_point function (clear to 0xFF)
static struct TestData bitmap_char1_data = { "char1", 41,  0, {}};
struct TestData* test_text_char1(struct Bitmap* bitmap) {
  struct BitmapText text;
  text_init(&text, liberation_sans_18, bitmap);
  text_char(&text, 'A');
  assert_true(&bitmap_char1_data, text.error == 0, "text.error want=0, got=%d", text.error);
  assert_true(&bitmap_char1_data, text.y == 0, "text.y want=0, got=%d", text.y);
  assert_true(&bitmap_char1_data, text.x == 14, "text.x want=14, got=%d", text.x);
  return &bitmap_char1_data;
}