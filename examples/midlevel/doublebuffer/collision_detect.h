#ifndef COLLISION_DETECT_H
#define COLLISION_DETECT_H

#include "common.h"
#include "ball.h"

#define GRID_SIZE (RADIUS*2)
#define CROWS (WIDTH/GRID_SIZE)
#define CCOLS (HEIGHT/GRID_SIZE)

static inline int16_t x_grid(int16_t x) {
  return (x / GRID_SIZE); 
}

static inline int16_t y_grid(int16_t y) {
  return (y / GRID_SIZE); 
}

static inline uint16_t grid_index(int16_t x, int16_t y) {
  const int16_t xg = x_grid(x);
  const int16_t yg = y_grid(y);
  return (yg * CCOLS) + xg;
}

struct CollisionDetect {
  // Used to first-pass-filter collisions.  Also serves double-duty
  // as a list of balls that might be colliding
  struct uint16_t grid[CROWS * CCOLS];  
  // holds potential collisions.  1:1 index match with ball_list array
  struct uint8_t potentials[BALL_COUNT];
  uint16_t potential_count;
  // for iterative indexing
  uint16_t i;
  uint16_t j;
};

void collision_prepare(struct Ball* ball_list);

int collision_next(struct Ball* ball_list, uint16_t* ball1, uint16_t ball2);

#endif

