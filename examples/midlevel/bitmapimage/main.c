#include "pico/stdlib.h"
#include <fonts/liberation_mono_14.h>
#include <sharpdisp/sharpdisp.h>
#include <sharpdisp/bitmapimage.h>
#include <sharpdisp/bitmaptext.h>
#include "hardware/structs/rosc.h"

#include "images.h"

#define WIDTH 400
#define HEIGHT 240

uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];
char printf_buffer[80];
struct SharpDisp sd;
struct BitmapImages bi;
struct BitmapText text;

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
  bitmap_clear(&sd.bitmap);
  image_draw(&bi, idx, x, y);
  text.x = 5;
  text.y = HEIGHT - 14;
  text_printf(
    &text,
    "i=%d x=%d y=%d w=%d h=%d",
    idx,
    x,
    y,
    width,
    height);
  sharpdisp_refresh(&sd);
}

int main() {
  sleep_ms(100);  // allow voltage to stabilize

  // Initailize
  sharpdisp_init_default(&sd, disp_buffer, WIDTH, HEIGHT, 0xFF);
  image_init(&bi, images, &sd.bitmap);
  text_init(&text, liberation_mono_14, &sd.bitmap);
  text.printf_buffer = printf_buffer;

  const uint32_t num_images = image_count(&bi);
  while (!bi.error) {
    sleep_ms(1000);
    for (uint32_t idx = 0; idx < num_images; ++idx) {
      show_an_image(idx);
      sleep_ms(2000);
    }
  }

  // an error occurred.  If you don't see images, add some debug messages here
  while (1) {
    sleep_ms(1000);
  }
}
