#include "pico/stdlib.h"
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

#define FILL_BUFF_SIZE 128
uint32_t fill_buff[FILL_BUFF_SIZE];

#define FRAMES 400

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
  const uint8_t old_mode = display.bitmap.mode;

  text.x = x + shadow;
  text.y = y + shadow;
  display.bitmap.mode = display.bitmap.clear_byte ? BITMAP_WHITE : BITMAP_BLACK;
  text_str(&text, str);
  text.x = x;
  text.y = y;
  display.bitmap.mode = display.bitmap.clear_byte ? BITMAP_BLACK : BITMAP_WHITE;
  text_str(&text, str);
  display.bitmap.mode = old_mode;
}

void points(void) {
  int8_t j_dir = 1;
  uint8_t j = 5;
  uint16_t i=0;
  for (; i <FRAMES; ++i) {
    bitmap_clear(&display.bitmap);

    uint16_t y = 0;
    for (; y < HEIGHT; y += j) {
      uint16_t x = 0;
      for (; x < WIDTH; x += j) {
        bitmap_point(&display.bitmap, x, y);
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
  uint8_t j = 5;
  uint16_t i=0;
  for (; i < FRAMES; ++i) {
    bitmap_clear(&display.bitmap);

    uint16_t y = 0;
    for (; y < HEIGHT; y += j) {
      bitmap_hline(&display.bitmap, 0, y, WIDTH); 
    }

    uint16_t x = 0;
    for (; x < WIDTH; x += j) {
      bitmap_vline(&display.bitmap, x, 0, HEIGHT); 
    }

    j+=j_dir;
    if (j == 32 || j == 5) {
      j_dir = -j_dir;
    }

    title("Lines");
    doublebuffer_swap(&dbl_buff);
  }
}

void lines2(void) {
  uint16_t i=0;
  for (; i < FRAMES; ++i) {
    bitmap_clear(&display.bitmap);

    const uint8_t offset = i & 0xF;
    uint16_t x = offset;
    for (; x < WIDTH; x += 16) {
      bitmap_line(
          &display.bitmap,
          x,
          0,
          WIDTH - x - 1,
          HEIGHT - 1);
    }

    uint16_t y = x - WIDTH;
    for (; y < HEIGHT; y += 16) {
      bitmap_line(
          &display.bitmap,
          WIDTH - 1,
          y,
          0,
          HEIGHT - y - 1);
    }

    title("More Lines");
    doublebuffer_swap(&dbl_buff);
  }
}

void text_array(void) {
  int8_t j_dir = 1;
  uint8_t j = 5;
  uint16_t i=0;
  struct BitmapText t;
  text_init(&t, liberation_mono_12, &display.bitmap);
  for (; i < FRAMES; ++i) {
    bitmap_clear(&display.bitmap);

    t.y = 0;
    uint8_t base = 'A';
    for (; t.y < HEIGHT; t.y += j, ++base) {
      t.x = 0;
      char c = base;
      for (; t.x < WIDTH; t.x += j) {
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

void rect(void) {
  uint16_t i=0;
  int8_t j_dir = 1;
  uint8_t j = 5;

  for (; i < FRAMES; ++i) {
    bitmap_clear(&display.bitmap);

    uint16_t offset = 0;
    for (; HEIGHT > (offset * 2); offset += 5) {
      bitmap_rect(
          &display.bitmap,
          j + offset,
          j + offset,
          WIDTH - offset * 2,
          HEIGHT - offset * 2);
    }

    j+=j_dir;
    if (j == HEIGHT || j == 0) {
      j_dir = -j_dir;
    }

    title("Rectangles");
    doublebuffer_swap(&dbl_buff);
  }
}

void filled_rect(void) {
  uint16_t i=0;
  int8_t j_dir = 1;
  uint8_t j = 5;

  for (; i < FRAMES; ++i) {
    bitmap_clear(&display.bitmap);

    uint16_t offset = 0;
    for (; HEIGHT > (offset * 2); offset += 5) {
      bitmap_filled_rect(
          &display.bitmap,
          j + offset,
          j + offset,
          WIDTH - offset * 2,
          HEIGHT - offset * 2);
    }

    j+=j_dir;
    if (j == HEIGHT || j == 0) {
      j_dir = -j_dir;
    }

    title("Filled Rectangles");
    doublebuffer_swap(&dbl_buff);
  }
}

void oval(void) {
  uint16_t i=0;

  for (; i < FRAMES; ++i) {
    bitmap_clear(&display.bitmap);

    const uint8_t f = i & 0x3f;
    const uint16_t cx = WIDTH >> 1;
    const uint16_t cy = HEIGHT >> 1;
    bitmap_oval(&display.bitmap, cx, cy, f, 0x3f - f);

    title("Ovals");
    doublebuffer_swap(&dbl_buff);
  }
}

void filled_oval(void) {
  uint16_t i=0;

  for (; i < FRAMES; ++i) {
    bitmap_clear(&display.bitmap);

    const uint8_t f = i & 0x3f;
    const uint16_t cx = WIDTH >> 1;
    const uint16_t cy = HEIGHT >> 1;
    bitmap_filled_oval(&display.bitmap, cx, cy, f, 0x3f - f);

    title("Filled Ovals");
    doublebuffer_swap(&dbl_buff);
  }
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
    bitmap_clear(&display.bitmap);

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
    bitmap_line(&display.bitmap, x0, y0, x1, y1);
    bitmap_line(&display.bitmap, x1, y1, x2, y2);
    bitmap_line(&display.bitmap, x2, y2, x0, y0);

    // find the center of the triangle
    const uint16_t x01 = (x0 + x1) >> 1;
    const uint16_t y01 = (y0 + y1) >> 1;
    const uint16_t x02 = (x0 + x2) >> 1;
    const uint16_t y02 = (y0 + y2) >> 1;
    const uint16_t cx = (x01 + x02) >> 1;
    const uint16_t cy = (y01 + y02) >> 1;

    bitmap_flood_fill(&display.bitmap, cx, cy, fill_buff, FILL_BUFF_SIZE);

    title("Flood Fill");
    doublebuffer_swap(&dbl_buff);
  }
}

struct Demo demos[] = {
  {0x00, BITMAP_WHITE, points},
  {0xFF, BITMAP_BLACK, points},
  {0x00, BITMAP_WHITE, lines},
  {0xFF, BITMAP_BLACK, lines},
  {0x00, BITMAP_WHITE, lines2},
  {0xFF, BITMAP_BLACK, lines2},
  {0x00, BITMAP_WHITE, text_array},
  {0xFF, BITMAP_BLACK, text_array},
  {0x00, BITMAP_WHITE, rect},
  {0xFF, BITMAP_BLACK, rect},
  {0x00, BITMAP_INVERSE, filled_rect},
  {0xFF, BITMAP_INVERSE, filled_rect},
  {0x00, BITMAP_WHITE, oval},
  {0xFF, BITMAP_BLACK, oval},
  {0x00, BITMAP_INVERSE, filled_oval},
  {0xFF, BITMAP_INVERSE, filled_oval},
  {0x00, BITMAP_WHITE, flood_fill},
};

int main(void) {
  sleep_ms(100);  // allow voltage to stabilize
  sharpdisp_init_default(&display, disp_buffer, WIDTH, HEIGHT, 0x00);
  doublebuffer_init(&dbl_buff, &display, disp_buffer2, 16);
  text_init(&text, liberation_sans_24, NULL);
  while (1) {
    int i=0;
    for (; i < (sizeof(demos) / sizeof(demos[0])); ++i) {
      struct Demo* demo = demos + i;
      display.bitmap.clear_byte = demo->clear_byte;
      display.bitmap.mode = demo->mode;
      demo->fn();
    }
  }
}
