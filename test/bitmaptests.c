#include "constants.h"
#include "sharpdisp/bitmap.h"
#include "sharpdisp/bitmapshapes.h"

uint8_t buff[BITMAP_SIZE(WIDTH, HEIGHT)];

// Tests the bitmap_clear function (clear to 0x00)
static struct TestData bitmap_clr0_data = { "clr0", 0, 0, {} };
struct TestData* bitmap_clr0(struct Bitmap* bitmap) {
  bitmap->clear_byte = 0x00;
  bitmap_clear(bitmap);
  return &bitmap_clr0_data;
}

// Tests the bitmap_clear function (clear to 0xFF)
static struct TestData bitmap_clr1_data = { "clr1", WIDTH * HEIGHT, 0, {} };
struct TestData* bitmap_clr1(struct Bitmap* bitmap) {
  bitmap->clear_byte = 0xFF;
  bitmap_clear(bitmap);
  return &bitmap_clr1_data;
}

// Tests the bitmap_copy function (same height)
static struct TestData bitmap_copy1_data = {
  "copy1",
  (WIDTH - 2) * 2 + (HEIGHT - 4) * 2,
  8,
  {
    {0, 0, 0},
    {WIDTH-1, 0, 0},
    {0, HEIGHT-1, 0},
    {WIDTH-1, HEIGHT-1, 0},
    {1, 1, 1},
    {WIDTH-2, 1, 1},
    {1, HEIGHT-2, 1},
    {WIDTH-2, HEIGHT-2, 1},
  }
};
struct TestData* bitmap_copy1(struct Bitmap* bitmap) {
  struct Bitmap b;
  bitmap_init(&b, buff, WIDTH, HEIGHT, BITMAP_WHITE, 0x00);
  bitmap_rect(&b, 1, 1, WIDTH-2, HEIGHT-2);
  bitmap_copy(bitmap, &b);
  return &bitmap_copy1_data;
}