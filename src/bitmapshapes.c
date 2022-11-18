#include "sharpdisp/bitmapshapes.h"

static inline void _bitmap_hline_single_byte(
    uint8_t* ptr,
    uint16_t x,
    uint16_t w,
    uint8_t mode) {
  // start with a full byte
  const uint8_t left_gap = x & 0x07;
  const uint8_t right_gap = (8 - ((x + w) & 0x07)) & 0x07;
  const uint8_t mask = (0xFF >> (left_gap + right_gap)) << right_gap;
  bitmap_apply(ptr, mode, mask);
}

static void _bitmap_hline_nocheck(
    struct Bitmap* bitmap,
    uint16_t x,
    uint16_t y,
    uint16_t w) {
  const uint16_t first_col = x >> 3;
  const uint16_t last_col = (x + w) >> 3;
  const uint8_t mode = bitmap->mode;
  uint8_t* ptr = bitmap->data + (y * bitmap->width_bytes) + (x >> 3);
  if (first_col == last_col) {
    // special case, the line starts and ends at the same bytes
    _bitmap_hline_single_byte(ptr, x, w, mode);
    return;
  }
  // do the first byte
  bitmap_apply(ptr++, mode, 0xFF >> (x & 0x07));
  // do the middle bytes
  for (uint16_t middle = first_col + 1; middle < last_col; ++middle, ++ptr) {
    bitmap_apply(ptr, mode, 0xFF);
  }
  // do the final byte
  const uint8_t right_gap = 8 - ((x + w) & 0x07);
  if (right_gap < 8) {
    bitmap_apply(ptr, mode, 0xFF << right_gap);
  }
}

void bitmap_hline(struct Bitmap* bitmap, int16_t x, int16_t y, uint16_t w) {
  if (w > bitmap->width) {
    w = bitmap->width;
  }
  if (x < 0) {
    if ((-x) >= w) {
      return;
    }
    w += x;
    x = 0;
  } else if (x >= bitmap->width) {
    return;
  }
  if (y < 0 || (y >= bitmap->height)) {
    return;
  }
  if ((x + w) > bitmap->width) {
    w = bitmap->width - x;
  }
  if (w == 0) {
    return;
  }
  _bitmap_hline_nocheck(bitmap, x, y, w);
}

void _bitmap_vline_nocheck(struct Bitmap* bitmap, uint16_t x, uint16_t y, uint16_t h) {
  const uint8_t mask = 0x80 >> (x & 0x07);
  uint8_t* ptr = bitmap->data + (y * bitmap->width_bytes) + (x >> 3);
  const uint8_t mode = bitmap->mode;
  for (uint16_t j=0; j<h; ++j, ptr += bitmap->width_bytes) {
    bitmap_apply(ptr, mode, mask);
  }
}

void bitmap_vline(
    struct Bitmap* bitmap,
    int16_t x,
    int16_t y,
    uint16_t h) {
  if (h > bitmap->height) {
    h = bitmap->height;
  }
  if ((x < 0) || (x >= bitmap->width)) {
    return;
  }
  if (y < 0) {
    if ((-y) >= h) {
      return;
    }
    h += y;
    y = 0;
  } else if (y >= bitmap->height) {
    return;
  }
  if ((y + h) >= bitmap->height) {
    h = bitmap->height - y;
  }
  if (h == 0) {
    return;
  }
  _bitmap_vline_nocheck(bitmap, x, y, h);
}

void bitmap_rect(
    struct Bitmap* bitmap,
    int16_t x,
    int16_t y,
    uint16_t w,
    uint16_t h) {
  if ((w == 0) || (h == 0)) {
    return;
  }
  bitmap_hline(bitmap, x, y, w);
  if (h > 2) {
    bitmap_vline(bitmap, x, y + 1, h - 2);
    if (w > 1) {
      bitmap_vline(bitmap, x + w - 1, y + 1, h - 2);
    }
  }
  if (h > 1) {
    bitmap_hline(bitmap, x, y + h - 1, w);
  }
}

void bitmap_filled_rect(
    struct Bitmap* bitmap,
    int16_t x,
    int16_t y,
    uint16_t w,
    uint16_t h) {
  // protect against overflow errors
  if (w > bitmap->width) {
    w = bitmap->width;
  }
  if (h > bitmap->height) {
    h = bitmap->height;
  }
  if (x < 0) {
    if ((-x) >= w) {
      return;
    }
    w += x;
    x = 0;
  } else if (x >= bitmap->width) {
    return;
  }
  if (y < 0) {
    if ((-y) >= h) {
      return;
    }
    h += y;
    y = 0;
  }
  if ((x+w) > bitmap->width) {
    w = bitmap->width - x;
  }
  if ((y+h) > bitmap->height) {
    h = bitmap->height - y;
  }
  if ((w == 0) || (h == 0)) {
    return;
  }
  const int16_t maxy = y+h;
  for (; y<maxy; ++y) {
    _bitmap_hline_nocheck(bitmap, x, y, w);
  }
}

void bitmap_line(
    struct Bitmap* bitmap,
    int16_t x0,
    int16_t y0,
    int16_t x1,
    int16_t y1) {
  if (x0 == x1) {
    if (y1 > y0) {
      bitmap_vline(bitmap, x0, y0, y1 - y0);
    } else {
      bitmap_vline(bitmap, x0, y1, y0 - y1);
    }
    return;
  }
  if (y0 == y1) {
    if (x1 > x0) {
      bitmap_hline(bitmap, x0, y0, x1 - x0);
    } else {
      bitmap_hline(bitmap, x1, y0, x0 - x1);
    }
    return;
  }
  // Always draw from upper left to lower right for consistency
  int16_t tmp;
  if (x0 > x1) {
    tmp = x0;
    x0 = x1;
    x1 = tmp;
  }
  if (y0 > y1) {
    tmp = y0;
    y0 = y1;
    y1 = tmp;
  }
  const int16_t dx = x1 - x0;
  const int16_t dy = y1 - y0;
  const uint16_t width = bitmap->width;
  const uint16_t height = bitmap->height;

  int16_t D = 2 * dy - dx;
  int16_t x = x0;
  int16_t y = y0;

  if ((x >= 0) && (y >= 0) && (x < width) && (y < height)) {
    bitmap_point_nocheck(bitmap, x, y);
  }

  while (x != x1 || y != y1) {
    if (D > 0) {
      ++y;
      D -= 2 * dx;
    } 
    
    if (D <= 0) {
      ++x;
      D += 2 * dy;
    }

    if ((x >= 0) && (x < width) && (y >= 0) && (y < height)) {
      bitmap_point_nocheck(bitmap, x, y);
    }
  }
}

static void _symmetric_point(
    struct Bitmap* bitmap,
    int16_t cx,
    int16_t cy,
    uint16_t dx,
    uint16_t dy) {

  const int16_t left = cx - (int16_t)dx;
  const int16_t top = cy - (int16_t)dy;
  const int16_t right = cx + dx;
  const int16_t bottom = cy + dy;
  const uint16_t bm_width = bitmap->width;
  const uint16_t bm_height = bitmap->height;

  if ((top >= 0) && (top < bm_height)) {
    if ((left >= 0) && (left < bm_width)) {
      bitmap_point_nocheck(bitmap, left, top);
    }
    if ((left != right) && (right >= 0) && (right < bm_width)) {
      bitmap_point_nocheck(bitmap, right, top);
    }
  }

  if ((top != bottom) && (bottom >= 0) && (bottom < bm_height)) {
    if ((left >= 0) && (left < bm_width)) {
      bitmap_point_nocheck(bitmap, left, bottom);
    }
    if ((left != right) && (right >= 0) && (right < bm_width)) {
      bitmap_point_nocheck(bitmap, right, bottom);
    }
  }
}

static void _symmetric_hfill(
    struct Bitmap* bitmap,
    int16_t cx,
    int16_t cy,
    uint16_t dx,
    uint16_t dy) {
  const uint16_t bm_width = bitmap->width;
  const int16_t left = ((cx - dx) >= 0) ? (cx - dx) : 0;
  if (left >= bm_width) {
    return;
  }
  const int16_t top = cy - dy;
  const int16_t right = ((cx + dx) < bm_width) ? (cx + dx) : bm_width - 1;
  if (right <= left) {
    return;
  }
  const int16_t bottom = cy + dy;

  const uint16_t bm_height = bitmap->height;
  if ((top >= 0) && (top < bm_height)) {
    _bitmap_hline_nocheck(bitmap, left, top, right - left);
  }
  if ((bottom >= 0) && (bottom < bm_height) && bottom > top) {
    _bitmap_hline_nocheck(bitmap, left, bottom, right - left);
  }
}

static void _next_arc_point(uint32_t rsq, uint16_t* dx, uint16_t* dy) {
  // Three possibilities:
  //  left, (dx--)
  //  up, (dy--)
  //  left,up (dx--. dy--)
  //
  // Each will have an associated error vs rsq.  We pick the choice with the
  // lowest error.

  // calc dx*dx and (dx-1) * (dx-1)
  int16_t d = (int16_t)(*dx);
  const int32_t dxsq = d * d;
  --d;
  const int32_t dxmmsq = d * d;

  // calc dy*dy and (dy-1) * (dy-1)
  d = (int16_t)(*dy);
  const int32_t dysq = d * d;
  ++d;
  const int32_t dyppsq = d * d;

  // calc each error
  int32_t up_err = dxsq + dyppsq;
  int32_t left_err = dxmmsq + dysq;
  int32_t up_left_err = dxmmsq + dyppsq;

  up_err = up_err > rsq ? up_err - rsq : rsq - up_err;
  left_err = left_err > rsq ? left_err - rsq : rsq - left_err;
  up_left_err = up_left_err > rsq ? up_left_err - rsq : rsq - up_left_err;

  // find the minimum error
  if ((up_left_err <= up_err) && (up_left_err <= left_err)) {
    --(*dx);
    ++(*dy);
  } else if ((up_err <= left_err) && (up_err <= up_left_err)) {
    // up has the smallest error
    ++(*dy);
  } else {
    // left has the smallest error
    --(*dx);
  }
}

static void _draw_circle(
    struct Bitmap* bitmap,
    int16_t cx,
    int16_t cy,
    uint16_t r,
    uint8_t filled,
    void (*fill_op)(
        struct Bitmap* bitmap,
        int16_t cx,
        int16_t cy,
        uint16_t dx,
        uint16_t dy)) {
  if (r == 0) {
    return;
  }

  // find the center and radius squared
  uint32_t rsq = r * r;

  // circles have symmetry everywhere so we can just calculate
  // a small part and use simple reflection to find other parts
  // for this one, we'll calculate 0-90 degrees
  uint16_t dx = r;
  uint16_t dy = 0;
  uint16_t last_dy = dy + 1;

  while ((dy < r) || (dx > 0)) {
    if (!filled || (last_dy != dy)) {
      fill_op(bitmap, cx, cy, dx, dy);
    }
    last_dy = dy;
    _next_arc_point(rsq, &dx, &dy);
  }
  if (!filled || (last_dy != dy)) {
    fill_op(bitmap, cx, cy, dx, dy);
  }
}

void _draw_wide_oval(
    struct Bitmap* bitmap,
    int16_t cx,
    int16_t cy,
    uint16_t rx,
    uint16_t ry,
    uint8_t filled,
    void (*fill_op)(
        struct Bitmap* bitmap,
        int16_t cx,
        int16_t cy,
        uint16_t dx,
        uint16_t dy)) {
  if ((rx == 0) || (ry == 0)) {
    return;
  }

  // w is our guiding circle, then scale for h
  uint32_t rsq = rx * rx;

  uint16_t dx = rx;
  uint16_t dy = 0;
  uint16_t sdy = 0;
  uint16_t last_dx = dx + 1;
  uint16_t last_sdy = sdy + 1;

  while ((dy < rx) || (dx > 0)) {
    if ((!filled && (last_dx != dx)) || (last_sdy != sdy)) {
      fill_op(bitmap, cx, cy, dx, sdy);
      last_dx = dx;
      last_sdy = sdy;
    }
    _next_arc_point(rsq, &dx, &dy);
    sdy = ((uint16_t)dy * ry + 1) / rx;
  }
  if ((!filled && (last_dx != dx)) || (last_sdy != sdy)) {
    fill_op(bitmap, cx, cy, dx, sdy);
  }
}

void _draw_narrow_oval(
    struct Bitmap* bitmap,
    int16_t cx,
    int16_t cy,
    uint16_t rx,
    uint16_t ry,
    uint8_t filled,
    void (*fill_op)(
        struct Bitmap* bitmap,
        int16_t cx,
        int16_t cy,
        uint16_t dx,
        uint16_t dy)) {
  if ((rx == 0) || (ry == 0)) {
    return;
  }

  // h is our guiding circle, scale for w
  uint32_t rsq = ry * ry;

  uint16_t dx = rx;
  uint16_t dy = 0;
  uint16_t sdx = ((uint32_t)dx * rx + 1) / ry;
  uint16_t last_sdx = sdx + 1;
  uint16_t last_dy = dy + 1;

  while ((dy < rx) || (dx > 0)) {
    if ((!filled && (last_sdx != sdx)) || (last_dy != dy)) {
      fill_op(bitmap, cx, cy, sdx, dy);
      last_sdx = sdx;
      last_dy = dy;
    }
    _next_arc_point(rsq, &dx, &dy);
    sdx = ((uint32_t)dx * rx + 1) / ry;
  }
  if ((!filled && (last_sdx != sdx)) || (last_dy != dy)) {
    fill_op(bitmap, cx, cy, sdx, dy);
  }
}

void bitmap_oval(
    struct Bitmap* bitmap,
    int16_t cx,
    int16_t cy,
    uint16_t rx,
    uint16_t ry) {
  if (rx == ry) {
    _draw_circle(bitmap, cx, cy, rx, 0, _symmetric_point);
  } else if (rx > ry) {
    _draw_wide_oval(bitmap, cx, cy, rx, ry, 0, _symmetric_point);
  } else {
    _draw_narrow_oval(bitmap, cx, cy, rx, ry, 0, _symmetric_point);
  }
}

void bitmap_filled_oval(
    struct Bitmap* bitmap,
    int16_t cx,
    int16_t cy,
    uint16_t rx,
    uint16_t ry) {
  if (rx == ry) {
    _draw_circle(bitmap, cx, cy, rx, 1, _symmetric_hfill);
  } else if (rx > ry) {
    _draw_wide_oval(bitmap, cx, cy, rx, ry, 1, _symmetric_hfill);
  } else {
    _draw_narrow_oval(bitmap, cx, cy, rx, ry, 1, _symmetric_hfill);
  }
}

void bitmap_circle(
    struct Bitmap* bitmap,
    int16_t cx,
    int16_t cy,
    uint16_t r) {
  _draw_circle(bitmap, cx, cy, r, 0, _symmetric_point);
}

void bitmap_filled_circle(
    struct Bitmap* bitmap,
    int16_t cx,
    int16_t cy,
    uint16_t r) {
  _draw_circle(bitmap, cx, cy, r, 1, _symmetric_hfill);
}

// Note: This struct must be 32-bits in length to align with the passed-in
// queue stucture.
struct BitmapQueue {
  uint16_t x;
  uint16_t y;
} __attribute__((packed));

static void _check_fill_point(
  const struct Bitmap* bitmap,
  struct BitmapQueue* queue,
  uint16_t queue_mask,
  uint16_t head,
  uint16_t* tail,
  uint16_t x,
  uint16_t y,
  uint8_t set) {
  const uint16_t end = *tail;
  if (((end + 1) & queue_mask) == head) {
    // queue is full
    return;
  }

  if ((bitmap_get_point_no_check(bitmap, x, y) != 0) == set) {
    // occupied
    return;
  }

  // Add it
  queue[end].x = x;
  queue[end].y = y;
  *tail = (end + 1) & queue_mask;
}

void bitmap_flood_fill(
    struct Bitmap* bitmap,
    uint16_t startx,
    uint16_t starty,
    uint32_t* queue32,
    uint16_t queue_length) {
  if (queue_length < 8 || queue_length > 1024) {
    return;
  }
  const uint16_t queue_mask = queue_length == 65536 ? 0xFFFF : queue_length - 1;
  if (queue_length & queue_mask) {
    // not a power of 2
    return;
  }

  struct BitmapQueue* queue = (struct BitmapQueue*)queue32;
  uint8_t set = 0;
  if (bitmap->mode == BITMAP_WHITE) {
    set = 1;
  } else if (bitmap->mode != BITMAP_BLACK) {
    // not supported
    return;
  }

  // normalize set

  // initialize with x,y
  uint16_t head = 0;
  uint16_t tail = 1;
  queue[0].x = startx;
  queue[0].y = starty;

  const uint16_t bm_width_max = bitmap->width - 1;
  const uint16_t bm_height_max = bitmap->height - 1;

  while (head != tail) {
    const uint16_t x = queue[head].x;
    const uint16_t y = queue[head].y;
    head = (head + 1) & queue_mask;

    if ((bitmap_get_point_no_check(bitmap, x, y) != 0) == set) {
      // this point is already in the desired state
      continue;
    }

    bitmap_point_nocheck(bitmap, x, y);

    // left
    if (x > 0) {
      _check_fill_point(bitmap, queue, queue_mask, head, &tail, x-1, y, set);
    }

    // right
    if (x < bm_width_max) {
      _check_fill_point(bitmap, queue, queue_mask, head, &tail, x+1, y, set);
    }

    // up
    if (y > 0) {
      _check_fill_point(bitmap, queue, queue_mask, head, &tail, x, y - 1, set);
    }

    // down
    if (y < bm_height_max) {
      _check_fill_point(bitmap, queue, queue_mask, head, &tail, x, y + 1, set);
    }
  }
 }
