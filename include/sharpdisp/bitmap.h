#ifndef LIB_BITMAP
#define LIB_BITMAP

#include <inttypes.h>
#include <string.h>

// Transfer modes
#define BITMAP_WHITE   0x01  // a 1 in the data creates a white pixel
#define BITMAP_BLACK   0x02  // a 1 in the data creates a black pixel
#define BITMAP_INVERSE 0x03  // a 1 in the data inverts the current pixels

// A Bitmap is used to hold the image data that will be sent to the screen.
// It can also hold other image data that can be drawn to off-screen
//
// The format is 8 bits per pixel.  Each byte represents a 8x1 pixel slice.
// consecutive bytes orient from left to right with wrapping at the point
// where the width is exceeded.
//
// If the width is not a multiple of 8, then some buts at the end of the
// last byte in a row will go unused.
struct Bitmap {
  uint16_t width;
  // width_bytes is equal to (width + 7) / 8.  Normally it's not a great idea to
  // represent the same number with two variables in case someone changes just one.
  // But in this case, there is the benefit of making the pixel operations quite
  // a bit more efficient (as they don't have to calculate the byte width on every pixel
  // set/clear).
  uint16_t width_bytes;
  uint16_t height;
  // Draw function will use this transfer mode.   See above.
  uint8_t  mode;
  // 0x00 for a black background, 0xFF for a white background
  uint8_t  clear_byte;
  uint8_t* data;
};

#define BITMAP_SIZE(width, height) ((((width) + 7) >> 3) * (height))

void bitmap_init(
    struct Bitmap* b,
    uint8_t* buff,
    uint16_t width,
    uint16_t height,
    uint8_t mode,
    uint8_t clear_byte);

// Copies one bitmap to another one.  Rules:
//   - Bitmaps must point to different data
//   - Bitmaps must have the same widths
// Violating either of these rules will cause this function to do nothing.
// If this is too restrictive for your usecase, consider the slower
// bitmap_copy_rect() or bitmap_blit() functions.
void bitmap_copy(struct Bitmap* dest, const struct Bitmap* src);

// This function is intended to copy a rectangular area
// of a larger bitmap to a smaller one.  The width and height
// of dest represent the rect size.
void bitmap_copy_rect(
  struct Bitmap* dest,
  const struct Bitmap* src,
  int16_t src_x,
  int16_t src_y);

// Blits a bitmap onto another one.  Uses dest->mode
// in the blit operation thus you might need to preclear
// the area with bitmap_filled_rect() in some cases.
void bitmap_blit(
  struct Bitmap* dest,
  int16_t dest_x,
  int16_t dest_y,
  const struct Bitmap* src);

// A transfer mode helper for rendering functions
// data_byte is expected to already be pointing to the
// correct byte in bitmap->data, this is inconvienent for
// users but can save extra calculations within drawing functions.
// Users should look to higher-level functions such as bitmap_setpoint
// as well as the bitmapshapes library. 
static inline void bitmap_apply(
  uint8_t* data_byte,
  uint8_t mode,
  uint8_t data) {
  switch (mode) {
    case BITMAP_WHITE:
      *data_byte |= data;
      break;
    case BITMAP_BLACK:
      *data_byte &= ~data;
      break;
    case BITMAP_INVERSE:
      *data_byte ^= data;
      break;
    default:
      break;
  }
}

// bitmap_point can be an inefficient way to doing things in cases where
// bitmap_set_byte (or just a memset) can do the job.  Also look at
// bitmapshapes.h for shape utils that are usually more efficient than calling
// bitmap_point in a loop.
static inline void bitmap_point_nocheck(struct Bitmap* b, uint16_t x, uint16_t y) {
  bitmap_apply(
    b->data + (y * b->width_bytes) + (x >> 3),
    b->mode,
    0x80 >> (x & 0x07));
}
static inline void bitmap_point(struct Bitmap* b, uint16_t x, uint16_t y) {
  if ((x < b->width) && (y < b->height)) {
    bitmap_point_nocheck(b, x, y);
  }
}

// returns zero if clear, non zero (not necessarily 1) if set.
static inline uint8_t bitmap_get_point_no_check(
  const struct Bitmap* b,
  uint16_t x,
  uint16_t y) {
  return b->data[y * b->width_bytes + (x >> 3)] & (0x80 >> (x & 0x07));
}
static inline uint8_t bitmap_get_point(
  const struct Bitmap* b,
  uint16_t x,
  uint16_t y) {
  if ((x < b->width) && (y < b->height)) {
    return bitmap_get_point_no_check(b, x, y);
  }
  return b->clear_byte;
}

// set an 8x1 pixel stripe with provisions for out-of-bounds
static inline void bitmap_apply_stripe(
  struct Bitmap* b,
  int16_t x,
  int16_t y,
  uint8_t data) {
  if (!data) {
    return;
  }
  const uint16_t bwidth = b->width;
  const uint16_t bheight = b->height;
  const uint8_t mode = b->mode;
  // First, the common case of everything being in bounds
  if ((x >= 0) && (y >= 0) && (y < bheight) && (x < bwidth)) {
    uint8_t* base = b->data + (y * b->width_bytes) + (x >> 3);
    const uint8_t shift = x & 0x07;
    bitmap_apply(base, mode, data >> shift);
    if (shift && (x <= (bwidth - 8))) {
      bitmap_apply(base + 1, mode, data << (8 - shift)); 
    }
    return;
  }

  // the next case to check is things off the screen
  if ((x <= -8) || (y < 0) || (x >= bwidth) || (y >= bheight)) {
    return;
  }

  // at this point, x is either partially shifted off the left
  uint8_t* base = b->data + (y * b->width_bytes);
  bitmap_apply(base, mode, data << (-x));
}

// returns an 8x1 pixel stripe.  Out of bounds return zeros
static inline uint8_t bitmap_get_stripe(
  const struct Bitmap* b,
  int16_t x,
  int16_t y) {
  const uint16_t bwidth = b->width;
  const uint16_t bheight = b->height;
  // First, the common case of everything being in bounds
  if ((x >= 0) && (y >= 0) && (y < bheight) && (x <= (bwidth - 8))) {
    uint8_t* base = b->data + (y * b->width_bytes) + (x >> 3);
    const uint8_t shift = x & 0x07;
    if (shift) {
      return (base[0] << shift) | (base[1] >> (8 - shift));
    } else {
      return base[0];
    }
  }

  // the next case to check is things off the screen
  if ((x <= -8) || (y < 0) || (x >= bwidth) || (y >= bheight)) {
    return 0x00;
  }

  // at this point, x is either partially shifted off the left or the right
  uint8_t* base = b->data + (y * b->width_bytes);
  if (x < 0) {
    return base[0] >> (-x);
  }
  
  return base[x >> 3] << (x & 0x07);
}

// Clear the entire bitmap.  Change Bitmap.clear_byte to 0x00 for a black
// backgroud or 0xFF for a white one.  This information is stored in the
// structure so that functions that need-to-know it (like console scrolling)
// can have access.
static inline void bitmap_clear(struct Bitmap* b) {
  memset(b->data, b->clear_byte, b->width_bytes * b->height);
}

#endif
