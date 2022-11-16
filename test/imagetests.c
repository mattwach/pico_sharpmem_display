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
    {1, 2, 1},
    {3, 2, 1},
    {5, 2, 1},
    {2, 3, 1},
    {4, 3, 1},
    {1, 4, 1},
    {3, 4, 1},
    {5, 4, 1},
    {0, 5, 1},
    {6, 5, 1},
  }
};
struct TestData* test_image_imgba(struct Bitmap* bitmap) {
  struct BitmapImages bi;
  image_init(&bi, images, bitmap);
  image_draw(&bi, SMALL_IMG, 0, 0);
  return &image_imgba_data;
}