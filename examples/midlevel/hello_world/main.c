#include "pico/stdlib.h"
#include <fonts/liberation_sans_36.h>
#include <sharpdisp/sharpdisp.h>
#include <sharpdisp/bitmapshapes.h>
#include <sharpdisp/bitmaptext.h>

#define WIDTH 400
#define HEIGHT 240

uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];

int main() {
  sleep_ms(100);  // allow voltage to stabilize

  // Initailize
  struct SharpDisp sd;
  sharpdisp_init_default(&sd, disp_buffer, WIDTH, HEIGHT, 0x00);
  struct BitmapText text;
  text_init(&text, liberation_sans_36, &sd.bitmap);

  // Print Hello World!
  const char* hello = "Hello World!";
  text.x = (WIDTH - text_str_width(&text, hello)) / 2;  // center the string
  text.y = (HEIGHT - text_height(&text)) / 2;
  text_str(&text, hello);

  // Make a border
  const uint16_t border = 15;
  bitmap_rect(
      &sd.bitmap, border, border, WIDTH - border * 2, HEIGHT - border * 2);

  // Send to hardware
  sharpdisp_refresh(&sd);

  while (1) {
    sleep_ms(1000);
  }
}
