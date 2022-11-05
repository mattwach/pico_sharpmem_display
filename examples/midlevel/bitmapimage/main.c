#include "pico/stdlib.h"
#include <sharpdisp/sharpdisp.h>
#include <sharpdisp/bitmapimage.h>
#include "hardware/structs/rosc.h"

#include "images.h"

#define WIDTH 400
#define HEIGHT 240

uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];
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

static void show_an_image(uint32_t idx) {
  int16_t width = image_width(&bi, idx);
  int16_t height = image_height(&bi, idx);
  int16_t x = (WIDTH - width) / 2;
  int16_t y = (HEIGHT - height) / 2;
  if (rand16(0,100) > 50) {
    // randomize the location
    x = rand16(-width / 2, WIDTH - width / 2);
    y = rand16(-height / 2, HEIGHT - height / 2);
  }
  if (rand16(0,100) > 50) {
    sd.bitmap.clear_byte = 0xFF;
  } else {
    sd.bitmap.clear_byte = 0x00;
  }
  bitmap_clear(&sd.bitmap);
  image_draw(&bi, idx, x, y);
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

int main() {
  sleep_ms(100);  // allow voltage to stabilize

  // Initailize
  sharpdisp_init_default(&sd, disp_buffer, WIDTH, HEIGHT, 0xFF);
  sd.bitmap.mode = BITMAP_INVERSE;
  image_init(&bi, images, &sd.bitmap);

  const uint32_t num_images = image_count(&bi);
  while (!bi.error) {
    sleep_ms(1000);
    for (uint32_t idx = 0; idx < num_images; ++idx) {
      show_an_image(idx);
      sleep_for(2000);
    }
  }

  // an error occurred.  If you don't see images, add some debug messages here
  while (1) {
    sleep_ms(1000);
  }
}
