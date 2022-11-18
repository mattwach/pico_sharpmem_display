
#include "common.h"
#include "sharpdisp/bitmapshapes.h"

static struct TestData shapes_hline_data = {
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
struct TestData* test_shapes_hline(struct Bitmap* bitmap) {
  bitmap_hline(bitmap, 0, 0, 5);  // 5
  bitmap_hline(bitmap, -1, 1, 5);  // 4
  bitmap_hline(bitmap, bitmap->width - 5, 2, 5);  // 5
  bitmap_hline(bitmap, bitmap->width - 4, 3, 5);  // 4
  bitmap_hline(bitmap, bitmap->width - 1, 4, 5);  // 1
  bitmap_hline(bitmap, bitmap->width, 5, 5);  // 0
  return &shapes_hline_data;
}

static struct TestData shapes_vline_data = {
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
struct TestData* test_shapes_vline(struct Bitmap* bitmap) {
  bitmap_vline(bitmap, -1, 0, 5);  // 0
  bitmap_vline(bitmap, 0, 0, 5);  // 5
  bitmap_vline(bitmap, 1, -1, 5);  // 4
  bitmap_vline(bitmap, bitmap->width-1, 0, 5);  // 5
  bitmap_vline(bitmap, bitmap->width, 0, 5);  // 0
  bitmap_vline(bitmap, -1, bitmap->height-4, 5);  // 0
  bitmap_vline(bitmap, 0, bitmap->height-4, 5);  // 4
  bitmap_vline(bitmap, bitmap->width-1, bitmap->height-4, 5);  // 4
  bitmap_vline(bitmap, bitmap->width, bitmap->height-4, 5);  // 0
  return &shapes_vline_data;
}

static struct TestData shapes_line_data = {
  9,
  9,
  {
    {0, 0, 1},    
    {1, 0, 1},    
    {2, 1, 1},    
    {3, 1, 1},    
    {4, 2, 1},    
    {5, 2, 1},    
    {6, 3, 1},    
    {7, 3, 1},    
    {8, 4, 1},    
  }
};
struct TestData* test_shapes_line(struct Bitmap* bitmap) {
  bitmap_line(bitmap, 0, 0, 8, 4);
  return &shapes_line_data;
}

static struct TestData shapes_line2_data = {0, 0, {}};
struct TestData* test_shapes_line2(struct Bitmap* bitmap) {
  const uint16_t iterations = 500;
  bitmap->mode = BITMAP_INVERSE;
  for (uint16_t i=0; i<iterations; ++i) {
    int16_t x1 = rand16(-10, bitmap->width + 10);
    int16_t x2 = rand16(-10, bitmap->width + 10);
    int16_t y1 = rand16(-10, bitmap->height + 10);
    int16_t y2 = rand16(-10, bitmap->height + 10);
    bitmap_line(bitmap, x1, y1, x2, y2);
    if (rand16(0, 100) > 50) {
      int16_t tmp = x1;
      x1 = x2;
      x2 = tmp;
    }
    if (rand16(0, 100) > 50) {
      int16_t tmp = y1;
      y1 = y2;
      y2 = tmp;
    }
    bitmap_line(bitmap, x1, y1, x2, y2);
  }
  return &shapes_line2_data;
}

static struct TestData shapes_rect1_data = {
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
struct TestData* test_shapes_rect1(struct Bitmap* bitmap) {
  bitmap_rect(bitmap, 0, 0, 4, 3);
  return &shapes_rect1_data;
}

static struct TestData shapes_rect2_data = {
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
struct TestData* test_shapes_rect2(struct Bitmap* bitmap) {
  bitmap_rect(bitmap, -1, 0, 4, 3);
  return &shapes_rect2_data;
}

static struct TestData shapes_rect3_data = {
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
struct TestData* test_shapes_rect3(struct Bitmap* bitmap) {
  bitmap_rect(bitmap, 0, -1, 4, 3);
  return &shapes_rect3_data;
}

static struct TestData shapes_rect4_data = {
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
struct TestData* test_shapes_rect4(struct Bitmap* bitmap) {
  bitmap_rect(bitmap, WIDTH-4, HEIGHT-3, 4, 3);
  return &shapes_rect4_data;
}

static struct TestData shapes_rect5_data = {
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
struct TestData* test_shapes_rect5(struct Bitmap* bitmap) {
  bitmap_rect(bitmap, WIDTH-3, HEIGHT-3, 4, 3);
  return &shapes_rect5_data;
}

static struct TestData shapes_rect6_data = {
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
struct TestData* test_shapes_rect6(struct Bitmap* bitmap) {
  bitmap_rect(bitmap, WIDTH-4, HEIGHT-2, 4, 3);
  return &shapes_rect6_data;
}

static struct TestData shapes_rect7_data = {WIDTH * HEIGHT, 0, {}};
struct TestData* test_shapes_rect7(struct Bitmap* bitmap) {
  bitmap_filled_rect(bitmap, 0, 0, WIDTH, HEIGHT);
  uint16_t iterations = 100;
  for (uint16_t i=0; i<iterations; ++i) {
    const int16_t x = rand16(-10, WIDTH+10);
    const int16_t y = rand16(-10, HEIGHT+10);
    const uint16_t width = rand16(0, 100);
    const uint16_t height = rand16(0, 100);
    bitmap_rect(bitmap, x, y, width, height);
  }
  return &shapes_rect7_data;
}

static struct TestData shapes_frct1_data = {
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
struct TestData* test_shapes_frct1(struct Bitmap* bitmap) {
  bitmap_filled_rect(bitmap, 0, 0, 4, 3);
  return &shapes_frct1_data;
}

static struct TestData shapes_frct2_data = {
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
struct TestData* test_shapes_frct2(struct Bitmap* bitmap) {
  bitmap_filled_rect(bitmap, -1, 0, 4, 3);
  return &shapes_frct2_data;
}

static struct TestData shapes_frct3_data = {
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
struct TestData* test_shapes_frct3(struct Bitmap* bitmap) {
  bitmap_filled_rect(bitmap, 0, -1, 4, 3);
  return &shapes_frct3_data;
}

static struct TestData shapes_frct4_data = {
  12,
  12,
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
struct TestData* test_shapes_frct4(struct Bitmap* bitmap) {
  bitmap_filled_rect(bitmap, WIDTH-4, HEIGHT-3, 4, 3);
  return &shapes_frct4_data;
}

static struct TestData shapes_frct5_data = {
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
struct TestData* test_shapes_frct5(struct Bitmap* bitmap) {
  bitmap_filled_rect(bitmap, WIDTH-3, HEIGHT-3, 4, 3);
  return &shapes_frct5_data;
}

static struct TestData shapes_frct6_data = {
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
struct TestData* test_shapes_frct6(struct Bitmap* bitmap) {
  bitmap_filled_rect(bitmap, WIDTH-4, HEIGHT-2, 4, 3);
  return &shapes_frct6_data;
}

static struct TestData shapes_frct7_data = {WIDTH * HEIGHT, 0, {}};
struct TestData* test_shapes_frct7(struct Bitmap* bitmap) {
  bitmap_filled_rect(bitmap, 0, 0, WIDTH, HEIGHT);
  uint16_t iterations = 100;
  for (uint16_t i=0; i<iterations; ++i) {
    const int16_t x = rand16(-10, WIDTH+10);
    const int16_t y = rand16(-10, HEIGHT+10);
    const uint16_t width = rand16(0, 100);
    const uint16_t height = rand16(0, 100);
    bitmap_filled_rect(bitmap, x, y, width, height);
  }
  return &shapes_frct7_data;
}

static struct TestData shapes_oval1_data = {83, 0, {}};
struct TestData* test_shapes_oval1(struct Bitmap* bitmap) {
  const int16_t y = HEIGHT / 2;
  const int16_t ry = HEIGHT / 5;
  bitmap_oval(bitmap, 5, y, 0, ry);
  bitmap_oval(bitmap, 10, y, 1, ry);
  bitmap_oval(bitmap, 15, y, 2, ry);
  bitmap_oval(bitmap, 20, y, 3, ry);
  bitmap_oval(bitmap, 29, y, 5, ry);
  return &shapes_oval1_data;
}
