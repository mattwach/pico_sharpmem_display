#include "ball.h"
#include "common.h"
#include <sharpdisp/bitmapshapes.h>
#include <sharpdisp/bitmaptext.h>
#include "hardware/structs/rosc.h"
#include <fonts/liberation_mono_14.h>
#include <float.h>
#include <math.h>

// Note: This file has nothing all to do with double buffering.  If we
// were not double buffering, the code in this file would be exactly
// the same.  If you are trying to understand the double buffering API,
// see main.c

#define BALL_COUNT 50

// If xv and yv were pixel integers, we would have some limitations on
// possible ball angles.  Thus we shift everything up by 4 (<< 4)
// to allow 4 bits of fractional value.  The final draw shifts those
// back out to convert to real pixels.
//
// As a reminder that these values are shifted, relevant variables have
// a "4" in them.  e.g the "x" coordinate is "x4" which suggests
// "pixel_x_coordinate << 4"

static const int16_t radius4 = RADIUS << 4;
static const int16_t diameter4 = (RADIUS * 2) << 4;
static const int32_t diameter4sq = diameter4 * diameter4;
static const int16_t max4x = (WIDTH - RADIUS) << 4;
static const int16_t max4y = (HEIGHT - RADIUS - 18) << 4;
static struct BitmapText text;
float energy_gain = 1.00f; // used to add energyor remove energy from the system

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
  int16_t x4;
  int16_t y4;
  int8_t xv4;
  int8_t yv4;
  char letter;
};

struct Ball ball[BALL_COUNT];

static bool balls_are_touching(struct Ball* b1, struct Ball* b2) {
  const int32_t dx4 = b1->x4 - b2->x4;
  if ((dx4 > diameter4) || (dx4 < -diameter4)) {
    // optimization early exit on far-away balls
    return false;
  }
  const int32_t dy4 = b1->y4 - b2->y4;
  if ((dy4 > diameter4) || (dy4 < -diameter4)) {
    // optimization early exit on far-away balls
    return false;
  }
  // pythagorean theorem
  return ((dx4 * dx4) + (dy4 * dy4)) <= diameter4sq;
}

static void init_a_ball(struct Ball* b) {
  b->x4 = rand16(radius4, max4x);
  b->y4 = rand16(radius4, max4y);
  b->xv4 = rand16(-32, 32);
  b->yv4 = rand16(-32, 32);
  b->letter = (char)(rand16('A', 'Z' + 1));
}

static bool ball_touching_anything(struct Ball* b, int num_added) {
  for (int i=0; i<num_added; ++i) {
    if (balls_are_touching(b, ball + i)) {
      return true;
    }
  }
  return false;
}

void init_balls(struct Bitmap* bitmap) {
  text_init(&text, liberation_mono_14, bitmap); 
  for (int i=0; i<BALL_COUNT; ++i) {
    struct Ball* b = ball + i;
    // try to place the ball in a new location
    for (int attempt=0; attempt < 10000; ++attempt) {
      init_a_ball(b);
      if (!ball_touching_anything(b, i)) {
        break;
      }
    }
  }
}

static void move_balls(void) {
  bool a_ball_is_stopped = false;
  bool a_ball_is_fast = false;
  for (int i=0; i < BALL_COUNT; ++i) {
    struct Ball* b = ball + i;
    b->x4 += b->xv4;
    b->y4 += b->yv4;
    // if things are stopping completly, then add some energy in
    if ((b->xv4 == 0) && (b->yv4 == 0)) {
      a_ball_is_stopped = true;
    }
    // make sure the velocity is not  getting out of control
    if (b->xv4 > 32) {
      b->xv4 = 32;
      a_ball_is_fast = true;
    } else if (b->xv4 < -32) {
      b->xv4 = -32;
      a_ball_is_fast = false;
    }
  }
  if (a_ball_is_stopped && (energy_gain < 1.05)) {
    energy_gain += 0.002;
  }
  if (a_ball_is_fast && (energy_gain > 0.95)) {
    energy_gain -= 0.002;
  }
}

static void bounce_a_ball_off_walls(struct Ball* b) {
  if ((b->x4 < radius4) || (b->x4 > max4x)) {
    // off the edge of the screen in the x dimension
    // reverse the direction and back off the previous change
    b->xv4 = -b->xv4;
    b->x4 += b->xv4;
    // sometimes we are still off the screen.  This can happen
    // when a ball collides with another one and is off the
    // screen at the same time.  For these cases, we simply pull
    // things back onto the screen.
    if (b->x4 < radius4) {
      b->x4 = radius4;
      energy_gain -= 0.01;
    }
    if (b->x4 > max4x) {
      b->x4 = max4x;
      energy_gain -= 0.01;
    }
  }
  // now everything that was done for x should be done for y
  // maybe this could be factored out but the number of parms
  // to inject is high so maybe harder to read than it already is.
  if ((b->y4 < radius4) || (b->y4 > max4y)) {
    b->yv4 = -b->yv4;
    b->y4 += b->yv4;
    if (b->yv4 > 32) {
      b->yv4 = 32;
    } else if (b->yv4 < -32) {
      b->yv4 = -32;
    }
    if (b->y4 < radius4) {
      b->y4 = radius4;
    }
    if (b->y4 > max4y) {
      b->y4 = max4y;
    }
  }
}

static void bounce_balls_off_walls(void) {
  for (int i=0; i < BALL_COUNT; ++i) {
    bounce_a_ball_off_walls(ball + i);
  }
}

// This function assumes the the balls are definitely touching
// or overlapping and applies the dense/nasty vector math to
// execute the bounce.
static void bounce_a_ball_off_a_ball(struct Ball* b1, struct Ball* b2) {
  // Assume that move_balls() got us in this situation and undo the
  // operation
  b1->x4 -= b1->xv4;
  b1->y4 -= b1->yv4;
  b2->x4 -= b2->xv4;
  b2->y4 -= b2->yv4;
  // There is a slight chance that the balls are still overlapping.  This
  // can happen when multiple balls collide at the same time or if a wall
  // is involved.  This logic will allow the situation to eventually
  // resolve itself, although it may not look pretty.
  if (balls_are_touching(b1, b2)) {
    b2->x4 -= b2->xv4;
    b2->y4 -= b2->yv4;
  }
  // may as well use (slower) floats here since a bunch of pruning was already
  // done to get to this point
  const float b1xv = b1->xv4;
  const float b1yv = b1->yv4;
  const float b2xv = b2->xv4;
  const float b2yv = b2->yv4;

  const float dx = b2->x4 - b1->x4;
  const float dy = b2->y4 - b1->y4;
  const float length = sqrtf(dx*dx + dy*dy);
  // normalize the vectors
  const float nx = dx / length;
  const float ny = dy / length;
  // normalize the velocities
  const float v1mag = nx * b1xv + ny * b1yv;
  const float v1nx = nx * v1mag;
  const float v1ny = ny * v1mag;

  const float v2mag = nx * b2xv + ny * b2yv;
  const float v2nx = nx * v2mag;
  const float v2ny = ny * v2mag;
  // Adjust the velocities
  b1->xv4 = (int16_t)((b1xv + v2nx - v1nx) * energy_gain);
  b1->yv4 = (int16_t)((b1yv + v2ny - v1ny) * energy_gain);
  b2->xv4 = (int16_t)((b2xv + v1nx - v2nx) * energy_gain);
  b2->yv4 = (int16_t)((b2yv + v1ny - v2ny) * energy_gain);
}

static void maybe_bounce_a_ball_off_a_ball(struct Ball* b1, struct Ball* b2) {
  if (balls_are_touching(b1, b2)) {
    bounce_a_ball_off_a_ball(b1, b2);
  }
}

static void bounce_balls_off_balls(void) {
  // Yeah, this could be optimized but the complexity increase might not
  // be worth it for a "demo"
  for (int j=1; j < BALL_COUNT; ++j) {
    for (int i=0; i<j; ++i) {
      maybe_bounce_a_ball_off_a_ball(ball + i, ball + j);
    }
  }
}

void draw_balls(void) {
  for (int i=0; i < BALL_COUNT; ++i) {
    struct Ball* b = ball + i;
    const uint16_t x = b->x4 >> 4;
    const uint16_t y = b->y4 >> 4;
    bitmap_filled_circle(text.bitmap, x, y, RADIUS);
    text.bitmap->mode = BITMAP_WHITE;
    text.x = x - 5;
    text.y = y - 6;
    text_char(&text, b->letter);
    text.bitmap->mode = BITMAP_BLACK;
  }
  move_balls();
  bounce_balls_off_balls();
  bounce_balls_off_walls();
}
