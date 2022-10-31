#include "pico/stdlib.h"
#include "hardware/structs/rosc.h"
#include <sharpdisp/sharpdisp.h>
#include <sharpdisp/doublebuffer.h>
#include <sharpdisp/bitmapshapes.h>
#include <sharpdisp/bitmaptext.h>
#include <fonts/liberation_mono_12.h>
#include <fonts/liberation_sans_24.h>

#define WIDTH 400
#define HEIGHT 240
#define SLEEP_MS 17

uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];
uint8_t disp_buffer2[BITMAP_SIZE(WIDTH, HEIGHT)];
struct SharpDisp display;
struct DoubleBuffer dbl_buff;
struct BitmapText text;

#define FILL_BUFF_SIZE 1024
uint32_t fill_buff[FILL_BUFF_SIZE];

#define FRAMES 400

// some good-enough random generation
static int16_t rand16(int16_t min, int16_t max) {
  uint16_t v = 0x0000;
  for (int i=0; i<16; ++i, v<<=1) {
    if (rosc_hw->randombit) {
      v |= 0x0001;
    }
  }
  return min + (v % (max - min));
}

struct Demo {
  uint8_t clear_byte;
  uint8_t mode;
  void (*fn)(void); 
};

static void title(const char* str) {
  const uint8_t shadow = 2;
  const uint8_t xpad = 40;
  const uint8_t ypad = 40;
  const uint16_t x = WIDTH - text_str_width(&text, str) - xpad;
  const uint16_t y = HEIGHT - text_height(&text) - ypad;
  const uint8_t old_mode = dbl_buff.bitmap.mode;

  text.x = x + shadow;
  text.y = y + shadow;
  dbl_buff.bitmap.mode = dbl_buff.bitmap.clear_byte ? BITMAP_WHITE : BITMAP_BLACK;
  text_str(&text, str);
  text.x = x;
  text.y = y;
  dbl_buff.bitmap.mode = dbl_buff.bitmap.clear_byte ? BITMAP_BLACK : BITMAP_WHITE;
  text_str(&text, str);
  dbl_buff.bitmap.mode = old_mode;
}

void points(void) {
  int8_t j_dir = 1;
  uint8_t j = 5;
  uint16_t i=0;
  for (; i <FRAMES; ++i) {
    bitmap_clear(&dbl_buff.bitmap);

    uint16_t y = 0;
    for (; y < HEIGHT; y += j) {
      uint16_t x = 0;
      for (; x < WIDTH; x += j) {
        bitmap_point(&dbl_buff.bitmap, x, y);
      }
    }

    j+=j_dir;
    if (j == 32 || j == 5) {
      j_dir = -j_dir;
    }

    title("Points");
    doublebuffer_swap(&dbl_buff);
  }
}

void lines(void) {
  int8_t j_dir = 1;
  uint8_t j = 10;
  uint16_t i=0;
  for (; i < FRAMES; ++i) {
    bitmap_clear(&dbl_buff.bitmap);

    // We purposefully go out of bounds to further exercise the
    // logic
    int16_t y = -20;
    for (; y < (HEIGHT + 20); y += j) {
      bitmap_hline(&dbl_buff.bitmap, -20, y, WIDTH + 20); 
    }

    int16_t x = -20;
    for (; x < (WIDTH + 20); x += j) {
      bitmap_vline(&dbl_buff.bitmap, x, -20, HEIGHT + 20); 
    }

    j+=j_dir;
    if (j == 64 || j == 10) {
      j_dir = -j_dir;
    }

    title("Lines");
    doublebuffer_swap(&dbl_buff);
  }
}

void lines2(void) {
  uint16_t i=0;
  for (; i < FRAMES; ++i) {
    bitmap_clear(&dbl_buff.bitmap);

    const uint8_t offset = i & 0xF;
    uint16_t x = offset;
    for (; x < WIDTH; x += 16) {
      bitmap_line(
          &dbl_buff.bitmap,
          x,
          0,
          WIDTH - x - 1,
          HEIGHT - 1);
    }

    uint16_t y = x - WIDTH;
    for (; y < HEIGHT; y += 16) {
      bitmap_line(
          &dbl_buff.bitmap,
          WIDTH - 1,
          y,
          0,
          HEIGHT - y - 1);
    }

    title("More Lines");
    doublebuffer_swap(&dbl_buff);
  }
}

void lines3(void) {
  bitmap_clear(&dbl_buff.bitmap);
  for (uint16_t i=0; i<FRAMES; ++i) {
    memcpy(dbl_buff.bitmap.data, display.bitmap.data, sizeof(disp_buffer));
    bitmap_line(
        &dbl_buff.bitmap,
        rand16(-50, WIDTH + 50),
        rand16(-50, HEIGHT + 50),
        rand16(-50, WIDTH + 50),
        rand16(-50, HEIGHT+ 50));
    title("Random Lines");
    doublebuffer_swap(&dbl_buff);
  }
}

void text_array(void) {
  int8_t j_dir = 1;
  uint8_t j = 5;
  uint16_t i=0;
  struct BitmapText t;
  text_init(&t, liberation_mono_12, &dbl_buff.bitmap);
  for (; i < FRAMES; ++i) {
    bitmap_clear(&dbl_buff.bitmap);

    t.y = -10;
    uint8_t base = 'A';
    for (; t.y < (HEIGHT + 10); t.y += j, ++base) {
      t.x = -10;
      char c = base;
      for (; t.x < (WIDTH + 10); t.x += j) {
        text_char(&t, c);
        ++c;
      }
    }

    j+=j_dir;
    if (j == 32 || j == 5) {
      j_dir = -j_dir;
    }

    title("Text");
    doublebuffer_swap(&dbl_buff);
  }
}

static void rect_common(bool filled, const char* titlestr) {
  uint16_t i=0;
  int8_t j_dir = 1;
  int8_t j = -50;

  for (; i < FRAMES; ++i) {
    bitmap_clear(&dbl_buff.bitmap);

    int16_t offset = 0;
    for (; HEIGHT > (offset * 2); offset += 5) {
      if (filled) {
        bitmap_filled_rect(
            &dbl_buff.bitmap,
            j + offset,
            j + offset,
            WIDTH - offset * 2,
            HEIGHT - offset * 2);
      } else {
        bitmap_rect(
            &dbl_buff.bitmap,
            j + offset,
            j + offset,
            WIDTH - offset * 2,
            HEIGHT - offset * 2);
      }
    }

    j+=j_dir;
    if (j == HEIGHT || j == -50) {
      j_dir = -j_dir;
    }

    title(titlestr);
    doublebuffer_swap(&dbl_buff);
  }
}

void rect(void) {
  rect_common(false, "Rectangles");
}

void filled_rect(void) {
  rect_common(true, "Filled Rectangles");
}

void oval_common(bool filled, const char* titlestr) {
  uint16_t i=0;

  for (; i < FRAMES; ++i) {
    bitmap_clear(&dbl_buff.bitmap);

    const uint8_t f = i & 0x3f;
    for (int16_t y=-25; y < (HEIGHT + 100); y += 100) {
      for (int16_t x=-25; i < (WIDTH + 100); x += 100) {
        if (filled) {
          bitmap_oval(&dbl_buff.bitmap, x, y, f, 0x3f - f);
        } else {
          bitmap_filled_oval(&dbl_buff.bitmap, x, y, f, 0x3f - f);
        }
      }
    }

    title(titlestr);
    doublebuffer_swap(&dbl_buff);
  }
}

void oval(void) {
  oval_common(false, "Ovals");
}

void filled_oval(void) {
  oval_common(true, "Filled Ovals");
}

void map_point(uint16_t p, uint16_t* x, uint16_t* y) {
  const uint16_t full = WIDTH * 2 + HEIGHT * 2;
  // normalize to the same screen
  while (p >= full) {
    p -= full;
  }

  if (p < WIDTH) {
    // top
    *y = 0;
    *x = p;
    return;
  }

  if (p < (WIDTH + HEIGHT)) {
    // right
    *x = WIDTH - 1;
    *y = p - WIDTH;
    return;
  }

  if (p < (WIDTH * 2 + HEIGHT)) {
    // bottom
    *x = WIDTH - (p - WIDTH - HEIGHT);
    *y = HEIGHT - 1;
    return;
  }

  // left
  *x = 0;
  *y = HEIGHT - (p - WIDTH * 2 - HEIGHT);
}

void flood_fill(void) {
  uint16_t i=0;

  for (; i < FRAMES; ++i) {
    bitmap_clear(&dbl_buff.bitmap);

    // Three triangular points that trace around the screen
    uint16_t p0 = i;
    uint16_t p1 = i * 3 + WIDTH + 3;
    uint16_t p2 = i * 4 + WIDTH + HEIGHT + WIDTH / 2 + 7;

    uint16_t x0;
    uint16_t y0;
    uint16_t x1;
    uint16_t y1;
    uint16_t x2;
    uint16_t y2;
    map_point(p0, &x0, &y0);
    map_point(p1, &x1, &y1);
    map_point(p2, &x2, &y2);
    bitmap_line(&dbl_buff.bitmap, x0, y0, x1, y1);
    bitmap_line(&dbl_buff.bitmap, x1, y1, x2, y2);
    bitmap_line(&dbl_buff.bitmap, x2, y2, x0, y0);

    // find the center of the triangle
    const uint16_t x01 = (x0 + x1) >> 1;
    const uint16_t y01 = (y0 + y1) >> 1;
    const uint16_t x02 = (x0 + x2) >> 1;
    const uint16_t y02 = (y0 + y2) >> 1;
    const uint16_t cx = (x01 + x02) >> 1;
    const uint16_t cy = (y01 + y02) >> 1;

    bitmap_flood_fill(&dbl_buff.bitmap, cx, cy, fill_buff, FILL_BUFF_SIZE);

    title("Flood Fill");
    doublebuffer_swap(&dbl_buff);
  }
}

struct Demo demos[] = {
//  {0x00, BITMAP_WHITE, points},
//  {0xFF, BITMAP_BLACK, points},
//  {0x00, BITMAP_WHITE, lines},
//  {0xFF, BITMAP_BLACK, lines},
//  {0x00, BITMAP_WHITE, lines2},
//  {0xFF, BITMAP_BLACK, lines2},
    {0x00, BITMAP_WHITE, lines3},
    {0xFF, BITMAP_BLACK, lines3},
//  {0x00, BITMAP_WHITE, text_array},
//  {0xFF, BITMAP_BLACK, text_array},
//  {0x00, BITMAP_WHITE, rect},
//  {0xFF, BITMAP_BLACK, rect},
//  {0x00, BITMAP_INVERSE, filled_rect},
//  {0xFF, BITMAP_INVERSE, filled_rect},
//  {0x00, BITMAP_WHITE, oval},
//  {0xFF, BITMAP_BLACK, oval},
//  {0x00, BITMAP_INVERSE, filled_oval},
//  {0xFF, BITMAP_INVERSE, filled_oval},
//  {0x00, BITMAP_WHITE, flood_fill},
};

int main(void) {
  sleep_ms(100);  // allow voltage to stabilize
  sharpdisp_init_default(&display, disp_buffer, WIDTH, HEIGHT, 0x00);
  doublebuffer_init(&dbl_buff, &display, disp_buffer2, 16);
  text_init(&text, liberation_sans_24, &dbl_buff.bitmap);
  while (1) {
    int i=0;
    for (; i < (sizeof(demos) / sizeof(demos[0])); ++i) {
      struct Demo* demo = demos + i;
      dbl_buff.bitmap.clear_byte = demo->clear_byte;
      dbl_buff.bitmap.mode = demo->mode;
      demo->fn();
    }
  }
}
