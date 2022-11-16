#include "common.h"
#include "sharpdisp/bitmap.h"
#include "sharpdisp/bitmapimage.h"
#include "images.h"

static struct TestData image_imgba_data = {
  "imgba",
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

static struct TestData image_imgo1_data = { "imgo1", 12, 0, {} };
struct TestData* test_image_img01(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, -7, 0);
  return &image_imgo1_data;
}

static struct TestData image_imgo2_data = {
  "imgo2",
  2,
  2,
  {
    {0, 0, 1},
    {0, 4, 1},
  }
};
struct TestData* test_image_imgo2(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, -6, 0);
  return &image_imgo2_data;
}

static struct TestData image_imgo3_data = {
  "imgo3",
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
struct TestData* test_image_imgo3(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, -1, 0);
  return &image_imgo3_data;
}

static struct TestData image_imgo4_data = { "imgo4", 12, 0, {} };
struct TestData* test_image_imgo4(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, bitmap->width - 7, 0);
  return &image_imgo4_data;
}