#include "common.h"
#include "sharpdisp/bitmap.h"
#include "sharpdisp/bitmaptext.h"
#include "fonts/liberation_sans_18.h"

static struct TestData bitmap_char1_data = {41,  0, {}};
struct TestData* test_text_char1(struct Bitmap* bitmap) {
  struct BitmapText text;
  text_init(&text, liberation_sans_18, bitmap);
  text_char(&text, 'A');
  assert_true(&bitmap_char1_data, text.error == 0, "text.error want=0, got=%d", text.error);
  assert_true(&bitmap_char1_data, text.y == 0, "text.y want=0, got=%d", text.y);
  assert_true(&bitmap_char1_data, text.x == 14, "text.x want=14, got=%d", text.x);
  return &bitmap_char1_data;
}

static struct TestData bitmap_char2_data = {0,  0, {}};
struct TestData* test_text_char2(struct Bitmap* bitmap) {
  struct BitmapText text;
  text_init(&text, liberation_sans_18, bitmap);
  text.x = -14;
  text_char(&text, 'A');
  assert_true(&bitmap_char2_data, text.error == 0, "text.error want=0, got=%d", text.error);
  assert_true(&bitmap_char2_data, text.x == 0, "text.x want=0, got=%d", text.x);
  return &bitmap_char2_data;
}

static struct TestData bitmap_char3_data = {20,  0, {}};
struct TestData* test_text_char3(struct Bitmap* bitmap) {
  struct BitmapText text;
  text_init(&text, liberation_sans_18, bitmap);
  text.x = -7;
  text_char(&text, 'A');
  assert_true(&bitmap_char3_data, text.error == 0, "text.error want=0, got=%d", text.error);
  assert_true(&bitmap_char3_data, text.x == 7, "text.x want=7, got=%d", text.x);
  return &bitmap_char3_data;
}

static struct TestData bitmap_char4_data = {41,  0, {}};
struct TestData* test_text_char4(struct Bitmap* bitmap) {
  struct BitmapText text;
  text_init(&text, liberation_sans_18, bitmap);
  text.x = bitmap->width - 14;
  text_char(&text, 'A');
  assert_true(&bitmap_char4_data, text.error == 0, "text.error want=0, got=%d", text.error);
  assert_true(&bitmap_char4_data, text.x == bitmap->width, "text.x want=%d, got=%d", bitmap->width, text.x);
  return &bitmap_char4_data;
}

static struct TestData bitmap_char5_data = {21,  0, {}};
struct TestData* test_text_char5(struct Bitmap* bitmap) {
  struct BitmapText text;
  text_init(&text, liberation_sans_18, bitmap);
  text.x = bitmap->width - 7;
  text_char(&text, 'A');
  assert_true(&bitmap_char5_data, text.error == 0, "text.error want=0, got=%d", text.error);
  assert_true(&bitmap_char5_data, text.x == bitmap->width + 7, "text.x want=%d, got=%d", bitmap->width + 7, text.x);
  return &bitmap_char5_data;
}

static struct TestData bitmap_char6_data = {0,  0, {}};
struct TestData* test_text_char6(struct Bitmap* bitmap) {
  struct BitmapText text;
  text_init(&text, liberation_sans_18, bitmap);
  text.x = bitmap->width - 1;
  text_char(&text, 'A');
  assert_true(&bitmap_char6_data, text.error == 0, "text.error want=0, got=%d", text.error);
  assert_true(&bitmap_char6_data, text.x == bitmap->width + 13, "text.x want=%d, got=%d", bitmap->width + 13, text.x);
  return &bitmap_char6_data;
}

static struct TestData bitmap_char7_data = {0,  0, {}};
struct TestData* test_text_char7(struct Bitmap* bitmap) {
  struct BitmapText text;
  text_init(&text, liberation_sans_18, bitmap);
  text.x = bitmap->width;
  text_char(&text, 'A');
  assert_true(&bitmap_char7_data, text.error == 0, "text.error want=0, got=%d", text.error);
  assert_true(&bitmap_char7_data, text.x == bitmap->width, "text.x want=%d, got=%d", bitmap->width + 14, text.x);
  return &bitmap_char7_data;
}

static struct TestData bitmap_char8_data = {28,  0, {}};
struct TestData* test_text_char8(struct Bitmap* bitmap) {
  struct BitmapText text;
  text_init(&text, liberation_sans_18, bitmap);
  text.y = -7;
  text_char(&text, 'A');
  assert_true(&bitmap_char8_data, text.error == 0, "text.error want=0, got=%d", text.error);
  assert_true(&bitmap_char8_data, text.y == -7, "text.y want=7, got=%d", text.y);
  return &bitmap_char8_data;
}

static struct TestData bitmap_char9_data = {13,  0, {}};
struct TestData* test_text_char9(struct Bitmap* bitmap) {
  struct BitmapText text;
  text_init(&text, liberation_sans_18, bitmap);
  text.y = bitmap->height - 7;
  text_char(&text, 'A');
  assert_true(&bitmap_char9_data, text.error == 0, "text.error want=0, got=%d", text.error);
  assert_true(&bitmap_char9_data, text.y == bitmap->height - 7, "text.y want=%d, got=%d", bitmap->height + 7, text.y);
  return &bitmap_char9_data;
}

// Unknown character
static struct TestData bitmap_chr10_data = {0,  0, {}};
struct TestData* test_text_chr10(struct Bitmap* bitmap) {
  struct BitmapText text;
  text_init(&text, liberation_sans_18, bitmap);
  text_char(&text, 200);
  assert_true(&bitmap_chr10_data, text.error == 0, "text.error want=0, got=%d", text.error);
  assert_true(&bitmap_chr10_data, text.y == 0, "text.y want=0, got=%d", text.y);
  assert_true(&bitmap_chr10_data, text.x == 0, "text.x want=0, got=%d", text.x);
  return &bitmap_chr10_data;
}

// Bad font data
static struct TestData bitmap_chr11_data = {0,  0, {}};
struct TestData* test_text_chr11(struct Bitmap* bitmap) {
  struct BitmapText text;
  text_init(&text, liberation_sans_18 + 1, bitmap);
  text_char(&text, 'A');
  assert_true(&bitmap_chr11_data, text.error != 0, "text.error want!=0, got=0");
  assert_true(&bitmap_chr11_data, text.y == 0, "text.y want=0, got=%d", text.y);
  assert_true(&bitmap_chr11_data, text.x == 0, "text.x want=0, got=%d", text.x);
  return &bitmap_chr11_data;
}

static struct TestData bitmap_slen1_data = {79,  0, {}};
struct TestData* test_text_slen1(struct Bitmap* bitmap) {
  struct BitmapText text;
  text_init(&text, liberation_sans_18, bitmap);
  text_strlen(&text, "Hi", 2);
  assert_true(&bitmap_slen1_data, text.error == 0, "text.error want=0, got=%d", text.error);
  assert_true(&bitmap_slen1_data, text.x == 21, "text.x want=21, got=%d", text.x);
  return &bitmap_slen1_data;
}

static struct TestData bitmap_str1_data = {79,  0, {}};
struct TestData* test_text_str1(struct Bitmap* bitmap) {
  struct BitmapText text;
  text_init(&text, liberation_sans_18, bitmap);
  text_str(&text, "Hi");
  assert_true(&bitmap_str1_data, text.error == 0, "text.error want=0, got=%d", text.error);
  assert_true(&bitmap_str1_data, text.x == 21, "text.x want=21, got=%d", text.x);
  return &bitmap_str1_data;
}

static struct TestData bitmap_prtf1_data = {61,  0, {}};
struct TestData* test_text_prtf1(struct Bitmap* bitmap) {
  struct BitmapText text;
  char str[16];
  text_init(&text, liberation_sans_18, bitmap);
  text.printf_buffer = str;
  text_printf(&text, "%d", 10);
  assert_true(&bitmap_prtf1_data, text.error == 0, "text.error want=0, got=%d", text.error);
  assert_true(&bitmap_prtf1_data, text.x == 24, "text.x want=24, got=%d", text.x);
  return &bitmap_prtf1_data;
}

static struct TestData bitmap_mtric_data = {0,  0, {}};
struct TestData* test_text_mtric(struct Bitmap* bitmap) {
  struct BitmapText text;
  text_init(&text, liberation_sans_18, bitmap);
  uint8_t w = text_char_width(&text, '0');
  assert_true(&bitmap_mtric_data, w == 12, "char_width want=12, got=%d", w);
  w = text_char_width(&text, 200);
  assert_true(&bitmap_mtric_data, w == 0, "char_width want=0, got=%d", w);
  w = text_strlen_width(&text, "Hi", 2);
  assert_true(&bitmap_mtric_data, w == 21, "char_width want=21, got=%d", w);
  w = text_str_width(&text, "OK");
  assert_true(&bitmap_mtric_data, w == 30, "char_width want=30, got=%d", w);
  uint8_t h = text_height(&text);
  assert_true(&bitmap_mtric_data, h == 18, "char_height want=18, got=%d", h);
  return &bitmap_mtric_data;
}