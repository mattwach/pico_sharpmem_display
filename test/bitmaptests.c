#include "constants.h"
#include "sharpdisp/bitmap.h"

static struct TestData bitmap_clr0_data = { "clr0", 0, 0, NULL };
struct TestData* bitmap_clr0(struct Bitmap* bitmap) {
  bitmap->clear_byte = 0x00;
  bitmap_clear(bitmap);
  return &bitmap_clr0_data;
}

static struct TestData bitmap_clr1_data = { "clr1", WIDTH * HEIGHT, 0, NULL };
struct TestData* bitmap_clr1(struct Bitmap* bitmap) {
  bitmap->clear_byte = 0xFF;
  bitmap_clear(bitmap);
  return &bitmap_clr1_data;
}
