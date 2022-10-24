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

// Clear the entire bitmap.  Change Bitmap.clear_byte to 0x00 for a black
// backgroud or 0xFF for a white one.  This information is stored in the
// structure so that functions that need-to-know it (like console scrolling)
// can have access.
static inline void bitmap_clear(struct Bitmap* b) {
  memset(b->data, b->clear_byte, b->width_bytes * b->height);
}

#endif
