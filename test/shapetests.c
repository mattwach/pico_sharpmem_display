
#include "common.h"
#include "sharpdisp/bitmapshapes.h"

static struct TestData image_hline_data = {
  "hline",
  19,
  9,
  {
    {0, 0, 1},    
    {1, 0, 1},    
    {2, 0, 1},    
    {3, 0, 1},    
    {4, 0, 1},    

    {0, 1, 1},    
    {1, 1, 1},    
    {2, 1, 1},    
    {3, 1, 1},    
  }
};
struct TestData* test_image_hline(struct Bitmap* bitmap) {
  bitmap_hline(bitmap, 0, 0, 5);  // 5
  bitmap_hline(bitmap, -1, 1, 5);  // 4
  bitmap_hline(bitmap, bitmap->width - 5, 2, 5);  // 4
  bitmap_hline(bitmap, bitmap->width - 4, 3, 5);  // 5
  bitmap_hline(bitmap, bitmap->width - 1, 4, 5);  // 1
  bitmap_hline(bitmap, bitmap->width, 5, 5);  // 0
  return &image_hline_data;
}