#include "ball.h"
#include "common.h"
#include <sharpdisp/bitmapshapes.h>
#include "hardware/structs/rosc.h"

const int16_t min_x = RADIUS << 4;
const int16_t min_y = min_x;
const int16_t max_x = (WIDTH - RADIUS) << 4;
const int16_t max_y = (HEIGHT - RADIUS - 18) << 4;

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

struct Ball {
  // all values are pixel x 16 for some subpixel allowances 
  int16_t x;
  int16_t y;
  int8_t xv;
  int8_t yv;
};

struct Ball ball[BALL_COUNT];

static void init_a_ball(struct Ball* b) {
  b->x = rand16(min_x, max_x);
  b->y = rand16(min_y, max_y);
  b->xv = rand16(-32, 32);
  b->yv = rand16(-32, 32);
}

void init_balls(void) {
  for (int i=0; i<BALL_COUNT; ++i) {
    init_a_ball(ball + i);
  }
}

static void draw_a_ball(struct Bitmap* bitmap, struct Ball* b) {
  bitmap_filled_circle(bitmap, b->x >> 4, b->y >> 4, RADIUS);
  b->x += b->xv;
  if ((b->x < min_x) || (b->x > max_x)) {
    b->xv = -b->xv;
    if (b->xv > 32) {
      b->xv = 32;
    } else if (b->xv < -32) {
      b->xv = -32;
    }
    if (b->x < min_x) {
      b->x = min_x;
    }
    if (b->x > max_x) {
      b->x = max_x;
    }
  }
  b->y += b->yv;
  if ((b->y < min_y) || (b->y > max_y)) {
    b->yv = -b->yv;
    b->y += b->yv;
    if (b->yv > 32) {
      b->yv = 32;
    } else if (b->yv < -32) {
      b->yv = -32;
    }
    if (b->y < min_y) {
      b->y = min_y;
    }
    if (b->y > max_y) {
      b->y = max_y;
    }
  }
}

void draw_balls(struct Bitmap* bitmap) {
  for (int i=0; i < BALL_COUNT; ++i) {
    draw_a_ball(bitmap, ball + i);
  }
}
