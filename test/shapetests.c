
#include "common.h"
#include "sharpdisp/bitmapshapes.h"

static struct TestData image_hline_data = {
  19,
  19,
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

    {WIDTH-5, 2, 1},
    {WIDTH-4, 2, 1},
    {WIDTH-3, 2, 1},
    {WIDTH-2, 2, 1},
    {WIDTH-1, 2, 1},

    {WIDTH-4, 3, 1},
    {WIDTH-3, 3, 1},
    {WIDTH-2, 3, 1},
    {WIDTH-1, 3, 1},

    {WIDTH-1, 4, 1},
  }
};
struct TestData* test_image_hline(struct Bitmap* bitmap) {
  bitmap_hline(bitmap, 0, 0, 5);  // 5
  bitmap_hline(bitmap, -1, 1, 5);  // 4
  bitmap_hline(bitmap, bitmap->width - 5, 2, 5);  // 5
  bitmap_hline(bitmap, bitmap->width - 4, 3, 5);  // 4
  bitmap_hline(bitmap, bitmap->width - 1, 4, 5);  // 1
  bitmap_hline(bitmap, bitmap->width, 5, 5);  // 0
  return &image_hline_data;
}