#include "common.h"
#include "sharpdisp/bitmap.h"
#include "sharpdisp/bitmapimage.h"
#include "images.h"

static struct TestData image_imgba_data = {
  12,
  12,
  {
    {0, 0, 1},
    {6, 0, 1},
    {1, 1, 1},
    {3, 1, 1},
    {5, 1, 1},
    {2, 2, 1},
    {4, 2, 1},
    {1, 3, 1},
    {3, 3, 1},
    {5, 3, 1},
    {0, 4, 1},
    {6, 4, 1},
  }
};
struct TestData* test_image_imgba(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, 0, 0);
  return &image_imgba_data;
}

static struct TestData image_imgx1_data = {0, 0, {}};
struct TestData* test_image_imgx1(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, -7, 0);
  return &image_imgx1_data;
}

static struct TestData image_imgx2_data = {
  2,
  2,
  {
    {0, 0, 1},
    {0, 4, 1},
  }
};
struct TestData* test_image_imgx2(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, -6, 0);
  return &image_imgx2_data;
}

static struct TestData image_imgx3_data = {
  10,
  10,
  {
    {5, 0, 1},
    {0, 1, 1},
    {2, 1, 1},
    {4, 1, 1},
    {1, 2, 1},
    {3, 2, 1},
    {0, 3, 1},
    {2, 3, 1},
    {4, 3, 1},
    {5, 4, 1},
  }
};
struct TestData* test_image_imgx3(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, -1, 0);
  return &image_imgx3_data;
}

static struct TestData image_imgx4_data = {12, 0, {}};
struct TestData* test_image_imgx4(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, bitmap->width - 7, 0);
  return &image_imgx4_data;
}

static struct TestData image_imgx5_data = {10, 0, {}};
struct TestData* test_image_imgx5(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, bitmap->width - 6, 0);
  return &image_imgx5_data;
}

static struct TestData image_imgx6_data = {2, 0, {}};
struct TestData* test_image_imgx6(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, bitmap->width - 1, 0);
  return &image_imgx6_data;
}

static struct TestData image_imgx7_data = {0, 0, {}};
struct TestData* test_image_imgx7(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, bitmap->width, 0);
  return &image_imgx7_data;
}

static struct TestData image_imgy1_data = {0, 0, {}};
struct TestData* test_image_imgy1(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, 0, -5);
  return &image_imgy1_data;
}

static struct TestData image_imgy2_data = {
  2,
  2,
  {
    {0, 0, 1},
    {6, 0, 1},
  }
};
struct TestData* test_image_imgy2(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, 0, -4);
  return &image_imgy2_data;
}

static struct TestData image_imgy3_data = {10, 0, {}};
struct TestData* test_image_imgy3(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, 0, -1);
  return &image_imgy3_data;
}

static struct TestData image_imgy4_data = {12, 0, {}};
struct TestData* test_image_imgy4(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, 0, bitmap->height - 5);
  return &image_imgy4_data;
}

static struct TestData image_imgy5_data = {10, 0, {}};
struct TestData* test_image_imgy5(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, 0, bitmap->height - 4);
  return &image_imgy5_data;
}

static struct TestData image_imgy6_data = {2, 0, {}};
struct TestData* test_image_imgy6(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, 0, bitmap->height - 1);
  return &image_imgy6_data;
}

static struct TestData image_imgy7_data = {0, 0, {}};
struct TestData* test_image_imgy7(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, 0, bitmap->height);
  return &image_imgy7_data;
}

static struct TestData image_tile0_data = {30, 0, {}};
struct TestData* test_image_tile0(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw_tiled(&bi, MEDIUM_IMG_0_0, MEDIUM_IMG_COLUMNS, MEDIUM_IMG_ROWS, 0, 0);
  return &image_tile0_data;
}

static struct TestData image_tile1_data = {0, 0, {}};
struct TestData* test_image_tile1(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  bitmap->mode = BITMAP_INVERSE;
  const uint16_t rounds = 100;

  for (uint16_t i=0; i<rounds; ++i) {
    const uint16_t x = rand16(-10, bitmap->width + 1);
    const uint16_t y = rand16(-10, bitmap->height + 1);

    // draw it
    image_draw(&bi, MEDIUM_IMG, x, y);
    // erase it
    image_draw_tiled(&bi, MEDIUM_IMG_0_0, MEDIUM_IMG_COLUMNS, MEDIUM_IMG_ROWS, x, y);
  }

  return &image_tile1_data;
}