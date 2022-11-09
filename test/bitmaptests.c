#include "constants.h"
#include "sharpdisp/bitmap.h"
#include "sharpdisp/bitmapshapes.h"

uint8_t buff[BITMAP_SIZE(WIDTH*2, HEIGHT*2)];

// Tests the bitmap_point function (clear to 0xFF)
static struct TestData bitmap_pnt_data = {
  "point",
  10,  // 1 pixel count
  10,  // point test count
  // Point Tests
  {
    {0, 0, 1},
    {0, 3, 1},
    {3, 0, 1},
    {3, 3, 1},
    {1, 1, 1},
    {1, 11, 1},
    {11, 1, 1},
    {11, 11, 1},
    {21, 1, 1},
    {21, 21, 1},
  }
};
struct TestData* bitmap_pnt(struct Bitmap* bitmap) {
  bitmap_point(bitmap, 0, 0);
  bitmap_point(bitmap, 0, 3);
  bitmap_point(bitmap, 3, 0);
  bitmap_point(bitmap, 3, 3);
  bitmap_point(bitmap, 1, 1);
  bitmap_point(bitmap, 1, 11);
  bitmap_point(bitmap, 11, 1);
  bitmap_point(bitmap, 11, 11);
  bitmap_point(bitmap, WIDTH, 0);
  bitmap_point(bitmap, WIDTH, HEIGHT-1);
  bitmap_point(bitmap, 0, HEIGHT);
  bitmap_point(bitmap, WIDTH-1, HEIGHT);

  // some getpoint transfers
  if (bitmap_get_point(bitmap, 11, 1)) {
    bitmap_point(bitmap, 21, 1);
  }
  if (bitmap_get_point(bitmap, 11, 11)) {
    bitmap_point(bitmap, 21, 21);
  }

  // and some that should be zero
  if (bitmap_get_point(bitmap, 11, 2)) {
    bitmap_point(bitmap, 21, 2);
  }
  if (bitmap_get_point(bitmap, 11, 0)) {
    bitmap_point(bitmap, 21, 0);
  }
  if (bitmap_get_point(bitmap, 10, 1)) {
    bitmap_point(bitmap, 20, 1);
  }
  if (bitmap_get_point(bitmap, 12, 1)) {
    bitmap_point(bitmap, 22, 1);
  }
  return &bitmap_pnt_data;
}

// tests get and apply stripe
static struct TestData bitmap_apst1_data = {
  "apst1",
  92,  // 1 pixel count
  39,  // point test count
  // Point Tests
  {
    {0, 1, 0},
    {0, 2, 1},
    {0, 3, 1},
    {0, 4, 1},
    {0, 5, 1},
    {0, 6, 1},
    {0, 7, 1},
    {0, 8, 1},
    {0, 8, 1},
    {0, 10, 0},

    {1, 3, 1},
    {2, 4, 1},
    {3, 5, 1},
    {4, 6, 1},
    {5, 7, 1},
    {6, 8, 1},
    {7, 9, 1},
    {8, 10, 1},
    {9, 11, 1},
    {10, 12, 1},
    {11, 13, 1},
    {12, 14, 1},
    {13, 15, 1},
    {14, 16, 1},
    {15, 17, 0},

    {2, 3, 0},
    {3, 4, 0},
    {4, 5, 0},
    {5, 6, 0},
    {6, 7, 0},
    {7, 8, 0},
    {8, 9, 0},
    {9, 10, 0},
    {10, 11, 0},
    {11, 12, 0},
    {12, 13, 0},
    {13, 14, 0},
    {14, 15, 0},
    {15, 16, 0},
  }
};
struct TestData* bitmap_apst1(struct Bitmap* bitmap) {
  // apply full stripes in a slant pattern
  //
  // Expecting this
  //
  //  0 ...............
  //  1 ...............
  //  2 *..............
  //  3 **.............
  //  4 ***............
  //  5 ****...........
  //  6 *****..........
  //  7 ******.........
  //  8 *******........
  //  9 ********.......
  // 10 .********......
  // 11 ..********.....
  // 12 ...********....
  // 13 ....********...
  // 14 .....********..
  // 15 ......********.
  // 16 .......********
  for (int i=-9; i<8; ++i) {
    bitmap_apply_stripe(bitmap, i, i + 9, 0xFF);
  }
  return &bitmap_apst1_data;
}

static struct TestData bitmap_apst2_data = {
  "apst1",
  23,  // 1 pixel count
  39,  // point test count
  // Point Tests
  {
    {0, 1, 0},
    {0, 2, 1},
    {0, 3, 0},
    {0, 4, 0},
    {0, 5, 0},
    {0, 6, 0},
    {0, 7, 0},
    {0, 8, 0},
    {0, 8, 1},
    {0, 10, 0},

    {1, 3, 1},
    {2, 4, 1},
    {3, 5, 1},
    {4, 6, 1},
    {5, 7, 1},
    {6, 8, 1},
    {7, 9, 1},
    {8, 10, 1},
    {9, 11, 1},
    {10, 12, 1},
    {11, 13, 1},
    {12, 14, 1},
    {13, 15, 1},
    {14, 16, 1},
    {15, 17, 0},

    {2, 3, 0},
    {3, 4, 0},
    {4, 5, 0},
    {5, 6, 0},
    {6, 7, 0},
    {7, 8, 0},
    {8, 9, 0},
    {9, 10, 0},
    {10, 11, 0},
    {11, 12, 0},
    {12, 13, 0},
    {13, 14, 0},
    {14, 15, 0},
    {15, 16, 0},
  }
};
struct TestData* bitmap_apst2(struct Bitmap* bitmap) {
  // apply full stripes in a slant pattern
  //
  // Expecting this
  //
  //  0 ...............
  //  1 ...............
  //  2 *..............
  //  3 .*.............
  //  4 ..*............
  //  5 ...*...........
  //  6 ....*..........
  //  7 .....*.........
  //  8 ......*........
  //  9 *......*.......
  // 10 .*......*......
  // 11 ..*......*.....
  // 12 ...*......*....
  // 13 ....*......*...
  // 14 .....*......*..
  // 15 ......*......*.
  // 16 .......*......*
  for (int i=-9; i<8; ++i) {
    bitmap_apply_stripe(bitmap, i, i + 9, 0x81);
  }
  return &bitmap_apst2_data;
}


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
  bitmap_clear(&b);
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
  bitmap_clear(&b);
  bitmap_rect(&b, 1, 1, WIDTH-2, HEIGHT-7);
  bitmap_copy(bitmap, &b);
  return &bitmap_copy2_data;
}

// Tests the bitmap_copy function (dest is shorter)
static struct TestData bitmap_copy3_data = {
  "copy3",
  (WIDTH - 2) + (HEIGHT - 2) * 2,  // 1 pixel count
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
  bitmap_clear(&b);
  bitmap_rect(&b, 1, 1, WIDTH-2, HEIGHT+3);
  bitmap_copy(bitmap, &b);
  return &bitmap_copy3_data;
}

// copies a larger bitmap into a smaller one with an offset
static struct TestData bitmap_cpyr1_data = {
  "cpyr1",
  (WIDTH - 3) * 2 + (HEIGHT - 4) * 2,  // 1 pixel count
  6,  // point test count
  // Point Tests
  {
    {0, 0, 1},
    {1, 1, 0},
    {WIDTH - 3, 0, 0},
    {WIDTH - 4, 0, 1},
    {0, HEIGHT - 2, 0},
    {0, HEIGHT - 3, 1},
  }
};
struct TestData* bitmap_cpyr1(struct Bitmap* bitmap) {
  struct Bitmap b;
  bitmap_init(&b, buff, WIDTH * 2, HEIGHT * 2, BITMAP_WHITE, 0x00);
  bitmap_clear(&b);
  bitmap_rect(&b, 2, 1, WIDTH - 3, HEIGHT - 2);
  bitmap_copy_rect(bitmap, &b, 2, 1);
  return &bitmap_cpyr1_data;
}

static struct TestData bitmap_cpyr2_data = {
  "cpyr2",
  ((WIDTH - 6) * 2) + (HEIGHT - 12) * 2,
  6,  // point test count
  // Point Tests
  {
    {2, 4, 0},
    {3, 5, 1},
    {4, 6, 0},
    {WIDTH-6+3-2,HEIGHT-10+5-2,0},
    {WIDTH-6+3-1,HEIGHT-10+5-1,1},
    {WIDTH-6+3,HEIGHT-10+5,0},
  }
};
struct TestData* bitmap_cpyr2(struct Bitmap* bitmap) {
  struct Bitmap b;
  bitmap_init(&b, buff, WIDTH * 2, HEIGHT * 2, BITMAP_WHITE, 0x00);
  bitmap_clear(&b);
  bitmap_rect(&b, 0, 0, WIDTH-6, HEIGHT-10);
  bitmap_copy_rect(bitmap, &b, -3, -5);
  return &bitmap_cpyr2_data;
}