
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

static struct TestData image_vline_data = {
  22,
  22,
  {
    {0, 0, 1},    
    {0, 1, 1},    
    {0, 2, 1},    
    {0, 3, 1},    
    {0, 4, 1},    

    {1, 0, 1},    
    {1, 1, 1},    
    {1, 2, 1},    
    {1, 3, 1},    

    {WIDTH-1, 0, 1},    
    {WIDTH-1, 1, 1},    
    {WIDTH-1, 2, 1},    
    {WIDTH-1, 3, 1},    
    {WIDTH-1, 4, 1},    

    {0, HEIGHT-4, 1},    
    {0, HEIGHT-3, 1},    
    {0, HEIGHT-2, 1},    
    {0, HEIGHT-1, 1},    

    {WIDTH-1, HEIGHT-4, 1},    
    {WIDTH-1, HEIGHT-3, 1},    
    {WIDTH-1, HEIGHT-2, 1},    
    {WIDTH-1, HEIGHT-1, 1},    
  }
};
struct TestData* test_image_vline(struct Bitmap* bitmap) {
  bitmap_vline(bitmap, -1, 0, 5);  // 0
  bitmap_vline(bitmap, 0, 0, 5);  // 5
  bitmap_vline(bitmap, 1, -1, 5);  // 4
  bitmap_vline(bitmap, bitmap->width-1, 0, 5);  // 5
  bitmap_vline(bitmap, bitmap->width, 0, 5);  // 0
  bitmap_vline(bitmap, -1, bitmap->height-4, 5);  // 0
  bitmap_vline(bitmap, 0, bitmap->height-4, 5);  // 4
  bitmap_vline(bitmap, bitmap->width-1, bitmap->height-4, 5);  // 4
  bitmap_vline(bitmap, bitmap->width, bitmap->height-4, 5);  // 0
  return &image_vline_data;
}

static struct TestData image_line_data = {
  19,
  1,
  {
    {0, 0, 1},    
  }
};
struct TestData* test_image_line(struct Bitmap* bitmap) {
  bitmap_line(bitmap, 0, 0, 8, 4);
  return &image_line_data;
}

static struct TestData image_rect1_data = {
  10,
  10,
  {
    {0, 0, 1},    
    {1, 0, 1},    
    {2, 0, 1},    
    {3, 0, 1},    

    {0, 1, 1},    
    {3, 1, 1},    

    {0, 2, 1},    
    {1, 2, 1},    
    {2, 2, 1},    
    {3, 2, 1},    
  }
};
struct TestData* test_image_rect1(struct Bitmap* bitmap) {
  bitmap_rect(bitmap, 0, 0, 4, 3);
  return &image_rect1_data;
}

static struct TestData image_rect2_data = {
  7,
  7,
  {
    {0, 0, 1},    
    {1, 0, 1},    
    {2, 0, 1},    

    {2, 1, 1},    

    {0, 2, 1},    
    {1, 2, 1},    
    {2, 2, 1},    
  }
};
struct TestData* test_image_rect2(struct Bitmap* bitmap) {
  bitmap_rect(bitmap, -1, 0, 4, 3);
  return &image_rect2_data;
}

static struct TestData image_rect3_data = {
  6,
  6,
  {
    {0, 0, 1},    
    {3, 0, 1},    

    {0, 1, 1},    
    {1, 1, 1},    
    {2, 1, 1},    
    {3, 1, 1},    
  }
};
struct TestData* test_image_rect3(struct Bitmap* bitmap) {
  bitmap_rect(bitmap, 0, -1, 4, 3);
  return &image_rect3_data;
}

static struct TestData image_rect4_data = {
  10,
  10,
  {
    {WIDTH-4, HEIGHT-3, 1},    
    {WIDTH-3, HEIGHT-3, 1},    
    {WIDTH-2, HEIGHT-3, 1},    
    {WIDTH-1, HEIGHT-3, 1},    

    {WIDTH-4, HEIGHT-2, 1},    
    {WIDTH-1, HEIGHT-2, 1},    

    {WIDTH-4, HEIGHT-1, 1},    
    {WIDTH-3, HEIGHT-1, 1},    
    {WIDTH-2, HEIGHT-1, 1},    
    {WIDTH-1, HEIGHT-1, 1},    
  }
};
struct TestData* test_image_rect4(struct Bitmap* bitmap) {
  bitmap_rect(bitmap, WIDTH-4, HEIGHT-3, 4, 3);
  return &image_rect4_data;
}

static struct TestData image_rect5_data = {
  7,
  7,
  {
    {WIDTH-3, HEIGHT-3, 1},    
    {WIDTH-2, HEIGHT-3, 1},    
    {WIDTH-1, HEIGHT-3, 1},    

    {WIDTH-3, HEIGHT-2, 1},    

    {WIDTH-3, HEIGHT-1, 1},    
    {WIDTH-2, HEIGHT-1, 1},    
    {WIDTH-1, HEIGHT-1, 1},    
  }
};
struct TestData* test_image_rect5(struct Bitmap* bitmap) {
  bitmap_rect(bitmap, WIDTH-3, HEIGHT-3, 4, 3);
  return &image_rect5_data;
}

static struct TestData image_rect6_data = {
  6,
  6,
  {
    {WIDTH-4, HEIGHT-2, 1},    
    {WIDTH-3, HEIGHT-2, 1},    
    {WIDTH-2, HEIGHT-2, 1},    
    {WIDTH-1, HEIGHT-2, 1},    

    {WIDTH-4, HEIGHT-1, 1},    
    {WIDTH-1, HEIGHT-1, 1},    
  }
};
struct TestData* test_image_rect6(struct Bitmap* bitmap) {
  bitmap_rect(bitmap, WIDTH-4, HEIGHT-2, 4, 3);
  return &image_rect6_data;
}

static struct TestData image_frct1_data = {
  12,
  12,
  {
    {0, 0, 1},    
    {1, 0, 1},    
    {2, 0, 1},    
    {3, 0, 1},    

    {0, 1, 1},    
    {1, 1, 1},    
    {2, 1, 1},    
    {3, 1, 1},    

    {0, 2, 1},    
    {1, 2, 1},    
    {2, 2, 1},    
    {3, 2, 1},    
  }
};
struct TestData* test_image_frct1(struct Bitmap* bitmap) {
  bitmap_filled_rect(bitmap, 0, 0, 4, 3);
  return &image_frct1_data;
}

static struct TestData image_frct2_data = {
  9,
  9,
  {
    {0, 0, 1},    
    {1, 0, 1},    
    {2, 0, 1},    

    {0, 1, 1},    
    {1, 1, 1},    
    {2, 1, 1},    

    {0, 2, 1},    
    {1, 2, 1},    
    {2, 2, 1},    
  }
};
struct TestData* test_image_frct2(struct Bitmap* bitmap) {
  bitmap_filled_rect(bitmap, -1, 0, 4, 3);
  return &image_frct2_data;
}

static struct TestData image_frct3_data = {
  8,
  8,
  {
    {0, 0, 1},    
    {1, 0, 1},    
    {2, 0, 1},    
    {3, 0, 1},    

    {0, 1, 1},    
    {1, 1, 1},    
    {2, 1, 1},    
    {3, 1, 1},    
  }
};
struct TestData* test_image_frct3(struct Bitmap* bitmap) {
  bitmap_filled_rect(bitmap, 0, -1, 4, 3);
  return &image_frct3_data;
}

static struct TestData image_frct4_data = {
  10,
  10,
  {
    {WIDTH-4, HEIGHT-3, 1},    
    {WIDTH-3, HEIGHT-3, 1},    
    {WIDTH-2, HEIGHT-3, 1},    
    {WIDTH-1, HEIGHT-3, 1},    

    {WIDTH-4, HEIGHT-2, 1},    
    {WIDTH-3, HEIGHT-2, 1},    
    {WIDTH-2, HEIGHT-2, 1},    
    {WIDTH-1, HEIGHT-2, 1},    

    {WIDTH-4, HEIGHT-1, 1},    
    {WIDTH-3, HEIGHT-1, 1},    
    {WIDTH-2, HEIGHT-1, 1},    
    {WIDTH-1, HEIGHT-1, 1},    
  }
};
struct TestData* test_image_frct4(struct Bitmap* bitmap) {
  bitmap_filled_rect(bitmap, WIDTH-4, HEIGHT-3, 4, 3);
  return &image_frct4_data;
}

static struct TestData image_frct5_data = {
  9,
  9,
  {
    {WIDTH-3, HEIGHT-3, 1},    
    {WIDTH-2, HEIGHT-3, 1},    
    {WIDTH-1, HEIGHT-3, 1},    

    {WIDTH-3, HEIGHT-2, 1},    
    {WIDTH-2, HEIGHT-2, 1},    
    {WIDTH-1, HEIGHT-2, 1},    

    {WIDTH-3, HEIGHT-1, 1},    
    {WIDTH-2, HEIGHT-1, 1},    
    {WIDTH-1, HEIGHT-1, 1},    
  }
};
struct TestData* test_image_frct5(struct Bitmap* bitmap) {
  bitmap_filled_rect(bitmap, WIDTH-3, HEIGHT-3, 4, 3);
  return &image_frct5_data;
}

static struct TestData image_frct6_data = {
  8,
  8,
  {
    {WIDTH-4, HEIGHT-2, 1},    
    {WIDTH-3, HEIGHT-2, 1},    
    {WIDTH-2, HEIGHT-2, 1},    
    {WIDTH-1, HEIGHT-2, 1},    

    {WIDTH-4, HEIGHT-1, 1},    
    {WIDTH-3, HEIGHT-1, 1},    
    {WIDTH-2, HEIGHT-1, 1},    
    {WIDTH-1, HEIGHT-1, 1},    
  }
};
struct TestData* test_image_frct6(struct Bitmap* bitmap) {
  bitmap_filled_rect(bitmap, WIDTH-4, HEIGHT-2, 4, 3);
  return &image_frct6_data;
}