#include "common.h"
#include "sharpdisp/bitmap.h"
#include "sharpdisp/bitmapshapes.h"

uint8_t buff[BITMAP_SIZE(WIDTH*2, HEIGHT*2)];

// Tests the bitmap_point function (clear to 0xFF)
static struct TestData bitmap_pnt_data = {
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
struct TestData* test_bitmap_pnt(struct Bitmap* bitmap) {
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
    {0, 9, 1},
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
struct TestData* test_bitmap_apst1(struct Bitmap* bitmap) {
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
    {0, 9, 1},
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
struct TestData* test_bitmap_apst2(struct Bitmap* bitmap) {
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

static struct TestData bitmap_gtst1_data = {
  27,  // 1 pixel count
  27,  // point test count
  // Point Tests
  {
    {0, 0, 1},
    {7, 0, 1},
    {8, 0, 1},
    {7, 2, 1},
    {6, 3, 1},
    {5, 4, 1},
    {4, 5, 1},
    {3, 6, 1},
    {2, 7, 1},
    {1, 8, 1},
    {0, 9, 1},
    {7, 9, 1},
    {7, 10, 1},
    {6, 10, 1},
    {6, 11, 1},
    {5, 11, 1},
    {5, 12, 1},
    {4, 12, 1},
    {4, 13, 1},
    {3, 13, 1},
    {3, 14, 1},
    {2, 14, 1},
    {2, 15, 1},
    {1, 15, 1},
    {1, 16, 1},
    {0, 16, 1},
    {0, 17, 1},
  }
};
struct TestData* test_bitmap_gtst1(struct Bitmap* bitmap) {
  // apply full stripes in a slant pattern
  //
  // Expecting this
  //
  //  0 *......**  // Seed line
  //  1 ........
  //  2 .......*
  //  3 ......*.
  //  4 .....*..
  //  5 ....*...
  //  6 ...*....
  //  7 ..*.....
  //  8 .*......
  //  9 *......*
  // 10 ......**
  // 11 .....**.
  // 12 ....**..
  // 13 ...**...
  // 14 ..**....
  // 15 .**.....
  // 16 **......
  // 17 *.......
  bitmap_point_nocheck(bitmap, 0, 0);
  bitmap_point_nocheck(bitmap, 7, 0);
  bitmap_point_nocheck(bitmap, 8, 0);

  for (int i=-8; i<18; ++i) {
    uint8_t data = bitmap_get_stripe(bitmap, i, 0);
    bitmap_apply_stripe(bitmap, 0, i+9, data);
  }
  return &bitmap_gtst1_data;
}

static struct TestData bitmap_gtst2_data = {0, 0, {}};
struct TestData* test_bitmap_gtst2(struct Bitmap* bitmap) {
  bitmap->mode = BITMAP_INVERSE;
  for (int i=0; i<100; ++i) {
    int16_t x = rand16(-10, bitmap->width+1);
    int16_t y = rand16(-10, bitmap->height+1);
    bitmap_apply_stripe(bitmap, x, y, rand16(0,256));
    uint8_t data = bitmap_get_stripe(bitmap, x, y);
    bitmap_apply_stripe(bitmap, x, y, data);
  }
  return &bitmap_gtst2_data;
}

// bitmap_blit test
static struct TestData bitmap_blit0_data = {
  8,
  8, 
  {
    {0, 0, 1},
    {WIDTH-1, 0, 1},
    {0, HEIGHT-1, 1},
    {WIDTH-1, HEIGHT-1, 1},
    {6, 5, 1},
    {5, 6, 1},
    {7, 6, 1},
    {6, 7, 1},
  }
};
struct TestData* test_bitmap_blit0(struct Bitmap* bitmap) {
  // Create a bitmapthat looks like this
  // .*.
  // *.*
  // .*.
  struct Bitmap b;
  bitmap_init(&b, buff, 3, 3, BITMAP_WHITE, 0x00);
  bitmap_clear(&b);
  bitmap_point(&b, 1, 0);
  bitmap_point(&b, 0, 1);
  bitmap_point(&b, 2, 1);
  bitmap_point(&b, 1, 2);
  // aiming for the 4 corners
  bitmap_blit(bitmap, -1, -2, &b);  // expose the bottom @ 0,0
  bitmap_blit(bitmap, bitmap->width-1, -1, &b);  // left at WIDTH-1, 0
  bitmap_blit(bitmap, -2, bitmap->height-2, &b);  // right @ 0,HEIGHT-1
  bitmap_blit(bitmap, bitmap->width-2, bitmap->height-1, &b);  // top at WIDTH-1, HEIGHT=1

  // now put one at 5,5
  bitmap_blit(bitmap, 5, 5, &b);  // expose the bottom @ 0,0
  return &bitmap_blit0_data;
}

static struct TestData bitmap_blit1_data = {0, 0, {}};
struct TestData* test_bitmap_blit1(struct Bitmap* bitmap) {
  // Create boxes of random points, get them, put them back in XOR mode
  // when all done there should be no lit pixels
  const uint16_t num_rounds = 100;
  const uint16_t num_points = 100;
  const uint16_t min_size = 1;
  const uint16_t max_size = 24;

  for (uint16_t i=0; i<num_rounds; ++i) {
    const int16_t x = rand16(-max_size, bitmap->width + 1);
    const int16_t y = rand16(-max_size, bitmap->height + 1);
    const uint16_t w = rand16(min_size, max_size);
    const uint16_t h = rand16(min_size, max_size);

    struct Bitmap b;
    bitmap_init(&b, buff, w, h, BITMAP_WHITE, 0x00);
    bitmap_clear(&b);
    bitmap->mode = BITMAP_WHITE;
    for (uint16_t j=0; j < num_points; ++j) {
      const int16_t px = rand16(0, w);
      const int16_t py = rand16(0, h);
      bitmap_point(bitmap, x + px, y + py);
      bitmap_point(&b, px, py);
    }
    bitmap->mode = BITMAP_INVERSE;
    bitmap_blit(bitmap, x, y, &b);  // this should clear everythign back
  }
  return &bitmap_blit1_data;
}


// Tests the bitmap_clear function (clear to 0x00)
static struct TestData bitmap_clr0_data = {0, 0, {}};
struct TestData* test_bitmap_clr0(struct Bitmap* bitmap) {
  bitmap->clear_byte = 0x00;
  bitmap_clear(bitmap);
  return &bitmap_clr0_data;
}

// Tests the bitmap_clear function (clear to 0xFF)
static struct TestData bitmap_clr1_data = {WIDTH * HEIGHT, 0, {}};
struct TestData* test_bitmap_clr1(struct Bitmap* bitmap) {
  bitmap->clear_byte = 0xFF;
  bitmap_clear(bitmap);
  return &bitmap_clr1_data;
}

// Tests the bitmap_copy function (same height)
static struct TestData bitmap_copy1_data = {
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
struct TestData* test_bitmap_copy1(struct Bitmap* bitmap) {
  struct Bitmap b;
  bitmap_init(&b, buff, WIDTH, HEIGHT, BITMAP_WHITE, 0x00);
  bitmap_clear(&b);
  bitmap_rect(&b, 1, 1, WIDTH-2, HEIGHT-2);
  bitmap_copy(bitmap, &b);
  return &bitmap_copy1_data;
}

// Tests the bitmap_copy function (dest is taller)
static struct TestData bitmap_copy2_data = {
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
struct TestData* test_bitmap_copy2(struct Bitmap* bitmap) {
  struct Bitmap b;
  bitmap_init(&b, buff, WIDTH, HEIGHT - 5, BITMAP_WHITE, 0x00);
  bitmap_clear(&b);
  bitmap_rect(&b, 1, 1, WIDTH-2, HEIGHT-7);
  bitmap_copy(bitmap, &b);
  return &bitmap_copy2_data;
}

// Tests the bitmap_copy function (dest is shorter)
static struct TestData bitmap_copy3_data = {
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
struct TestData* test_bitmap_copy3(struct Bitmap* bitmap) {
  struct Bitmap b;
  bitmap_init(&b, buff, WIDTH, HEIGHT + 5, BITMAP_WHITE, 0x00);
  bitmap_clear(&b);
  bitmap_rect(&b, 1, 1, WIDTH-2, HEIGHT+3);
  bitmap_copy(bitmap, &b);
  return &bitmap_copy3_data;
}

// copies a larger bitmap into a smaller one with an offset
static struct TestData bitmap_cpyr1_data = {
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
struct TestData* test_bitmap_cpyr1(struct Bitmap* bitmap) {
  struct Bitmap b;
  bitmap_init(&b, buff, WIDTH * 2, HEIGHT * 2, BITMAP_WHITE, 0x00);
  bitmap_clear(&b);
  bitmap_rect(&b, 2, 1, WIDTH - 3, HEIGHT - 2);
  bitmap_copy_rect(bitmap, &b, 2, 1);
  return &bitmap_cpyr1_data;
}

static struct TestData bitmap_cpyr2_data = {
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
struct TestData* test_bitmap_cpyr2(struct Bitmap* bitmap) {
  struct Bitmap b;
  bitmap_init(&b, buff, WIDTH * 2, HEIGHT * 2, BITMAP_WHITE, 0x00);
  bitmap_clear(&b);
  bitmap_rect(&b, 0, 0, WIDTH-6, HEIGHT-10);
  bitmap_copy_rect(bitmap, &b, -3, -5);
  return &bitmap_cpyr2_data;
}
