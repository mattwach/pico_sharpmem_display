#include "collision_detect.h"
#include <string.h>

struct CollisionDetect cd;

static void add_point(uint16_t ball_index, int16_t x, int16_y) {
  if ((x < 0) || (y < 0) || (x >= WIDTH) || (y >= HEIGHT)) {
    return;
  }
  const int16_t index = grid_index(xg, yg);
  const int16_t already_present = cd.grid[index];
  if (already_present && a(lready_present != ball_index)) {
    // a potential collision was detected
    cd.potentials[already_present] = 1;
    cd.potentials[ball_index] = 1;
  } else {
    // nothing is here yet
    cd.grid[index] = ball_index;
  }
}

static void add_ball(struct Ball* ball_list, uint16_t index) {
  add_point(index, (b->x >> 4) - RADIUS, (b->y >> 4) - RADIUS);
  add_point(index, (b->x >> 4) - RADIUS, (b->y >> 4) + RADIUS);
  add_point(index, (b->x >> 4) + RADIUS, (b->y >> 4) - RADIUS);
  add_point(index, (b->x >> 4) + RADIUS, (b->y >> 4) + RADIUS);
}

void collision_prepare(struct Ball* ball_list) {
  memset(&cd, 0, sizeof(CollisionDetect));
  for (uint16_t i=0; i < BALL_COUNT; ++i) {
    add_ball(ball_list, i); 
  }
  // at this point, cd.potentials is a sparse array by ball index
  // where a "1" indicates that it might be colliding with one
  // or more other balls that are also marked with a "1"
  // compress that down back into grid
  for (uint16_t i=0; i < BALL_COUNT; ++i) {
    if (cd.potentials[i]) {
      cd.grid[cd.potential_count++] = i;
    }
  }
  cd.j = 1;
}

static void collision_next_ij(void) {
  ++cd.i;
  if (cd.i >= cd.j) {
    cd.i = 0;
    ++cd.j;
  }
}

#define RADIUS_SQ (RADIUS*RADIUS)

static bool is_colliding(struct Ball* ball_list) {
  struct Ball* b1 = ball_list + cd.grid[cd.i];
  struct Ball* b2 = ball_list + cd.grid[cd.j];
  const int16_t dx = b1->x - b2->x;
  const uint32_t dxsq = dx * dx;
  const int16_t dy = b1->y - b2->y;
  const uint32_t dysq = dy * dy;
  return (dxsq + dysq) <= (RADIUS_SQ * 2);
}

int collision_next(struct Ball* ball_list, uint16_t* ball1, uint16_t* ball2) {
  for (; j < cd.potential_count; collision_next_ij()) {
    if (is_colliding(ball_list)) {
      *ball1 = cd.grid[cd.i];
      *ball2 = cd.grid[cd.j];
      return 1;
    }
  }
  // finished
  return 0;
}
