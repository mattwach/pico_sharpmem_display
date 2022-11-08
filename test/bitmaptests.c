#include "constants.h"
#include "sharpdisp/bitmap.h"
#include "sharpdisp/bitmapshapes.h"

uint8_t buff[BITMAP_SIZE(WIDTH*2, HEIGHT*2)];

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
  (WIDTH - 2) * 2 + (HEIGHT - 4) * 2,  // 1 pixel count
  8,  // point test count
  // Point Tests
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

// Tests the bitmap_copy function (dest is taller)
static struct TestData bitmap_copy2_data = {
  "copy2",
  (WIDTH - 2) * 2 + (HEIGHT - 9) * 2,  // 1 pixel count
  8,  // point test count
  // Point Tests
  {
    {0, 0, 0},
    {WIDTH-1, 0, 0},
    {0, HEIGHT-1, 0},
    {WIDTH-1, HEIGHT-1, 0},
    {1, 1, 1},
    {WIDTH-2, 1, 1},
    {1, HEIGHT-2, 0},  // these are zeros now
    {WIDTH-2, HEIGHT-2, 0},
  }
};
struct TestData* bitmap_copy2(struct Bitmap* bitmap) {
  struct Bitmap b;
  bitmap_init(&b, buff, WIDTH, HEIGHT - 5, BITMAP_WHITE, 0x00);
  bitmap_rect(&b, 1, 1, WIDTH-2, HEIGHT-6);
  bitmap_copy(bitmap, &b);
  return &bitmap_copy2_data;
}

// Tests the bitmap_copy function (dest is shorter)
static struct TestData bitmap_copy3_data = {
  "copy3",
  (WIDTH - 2) * 2 + (HEIGHT + 6) * 2,  // 1 pixel count
  8,  // point test count
  // Point Tests
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
struct TestData* bitmap_copy3(struct Bitmap* bitmap) {
  struct Bitmap b;
  bitmap_init(&b, buff, WIDTH, HEIGHT + 5, BITMAP_WHITE, 0x00);
  bitmap_rect(&b, 1, 1, WIDTH-2, HEIGHT+3);
  bitmap_copy(bitmap, &b);
  return &bitmap_copy2_data;
}

static struct TestData bitmap_cpyr1_data = {
  "cpyr1",
  (WIDTH - 2) * 2 + (HEIGHT - 4) * 2,  // 1 pixel count
  8,  // point test count
  // Point Tests
  {
    {0, 0, 1},
    {WIDTH-1, 0, 1},
    {0, HEIGHT-1, 1},
    {WIDTH-1, HEIGHT-1, 0},
    {1, 1, 0},
    {WIDTH-2, 1, 0},
    {1, HEIGHT-2, 0},
    {WIDTH-2, HEIGHT-2, 0},
  }
};
struct TestData* bitmap_cpr1(struct Bitmap* bitmap) {
  struct Bitmap b;
  bitmap_init(&b, buff, WIDTH * 2, HEIGHT * 2, BITMAP_WHITE, 0x00);
  bitmap_rect(&b, 2, 1, WIDTH - 3, HEIGHT - 2);
  bitmap_copy_rect(bitmap, -2, -1, &b);
  return &bitmap_cpr1_data;
}