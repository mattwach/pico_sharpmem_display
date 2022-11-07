#include "pico/stdlib.h"
#include <sharpdisp/sharpdisp.h>
#include <sharpdisp/bitmapimage.h>
#include <sharpdisp/bitmapshapes.h>
#include "hardware/structs/rosc.h"

#include "rose.h"

#define WIDTH 400
#define HEIGHT 240
#define MIN_COPY_SIZE 64
#define MAX_COPY_SIZE 128
#define BORDER 16

uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];
uint8_t copy_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];
struct SharpDisp sd;
struct BitmapImages bi;

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

// need to keep refreshing the screen or it
// will sometimes blank out.
static void sleep_for(uint32_t ms) {
  const uint32_t steps = ms / 50; 
  for (uint32_t i=0; i<steps; ++i) {
    sharpdisp_refresh(&sd);
    sleep_ms(50);
  }
}

static void init_and_draw_image(void) {
  sharpdisp_init_default(&sd, disp_buffer, WIDTH, HEIGHT, 0xFF);
  sd.bitmap.mode = BITMAP_BLACK;
  image_init(&bi, rose, &sd.bitmap);
  image_draw(&bi, ROSE_IMG, 0, 0);
}

static void copy_region(void) {
  // Create a bitmap of a random size
  struct Bitmap bcopy;
  bitmap_init(
    &bcopy,
    copy_buffer,
    rand16(MIN_COPY_SIZE, MAX_COPY_SIZE),
    rand16(MIN_COPY_SIZE, MAX_COPY_SIZE),
    BITMAP_BLACK,
    0xFF
  );

  // Copy a random region of the screen to this bitmap
  bitmap_copy_rect(
    &bcopy,
    &sd.bitmap,
    rand16(-BORDER, WIDTH - bcopy.width + BORDER),
    rand16(-BORDER, HEIGHT - bcopy.height + BORDER)
  );

  // determine a random coordinate to paste the copy
  const int16_t destx = rand16(-BORDER, WIDTH - bcopy.width + BORDER);
  const int16_t desty = rand16(-BORDER, HEIGHT - bcopy.height + BORDER);

  // clear the area with a white background
  sd.bitmap.mode = BITMAP_WHITE;
  bitmap_filled_rect(
    &sd.bitmap,
    destx,
    desty,
    bcopy.width,
    bcopy.height
  );

  // paste in the bitmap
  sd.bitmap.mode = BITMAP_BLACK;
  bitmap_blit(
    &sd.bitmap,
    destx,
    desty,
    &bcopy
  );
}

int main() {
  sleep_ms(100);  // allow voltage to stabilize
  init_and_draw_image();

  while (1) {
    copy_region();
    sleep_for(2000);
  }
}
