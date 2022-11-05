#include "pico/stdlib.h"
#include <sharpdisp/sharpdisp.h>
#include <sharpdisp/doublebuffer.h>
#include <sharpdisp/bitmap.h>
#include <sharpdisp/bitmapimage.h>

#include "map.h"

#define WIDTH 400
#define HEIGHT 240
#define SLEEP_MS 16

uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];
uint8_t disp_buffer2[BITMAP_SIZE(WIDTH, HEIGHT)];
struct SharpDisp sd;
struct DoubleBuffer dub_buff;
struct BitmapImages bi;

int main() {
  sleep_ms(100);  // allow voltage to stabilize

  // Initailize
  sharpdisp_init_default(&sd, disp_buffer, WIDTH, HEIGHT, 0xFF);
  doublebuffer_init(&dub_buff, &sd, disp_buffer2, SLEEP_MS);
  image_init(&bi, map, &sd.bitmap);

  const int16_t xmin = WIDTH - image_width(&bi, MAP_IMG);
  const int16_t xmax = 0;
  const int16_t ymin = HEIGHT - image_height(&bi, MAP_IMG);
  const int16_t ymax = 0;

  int16_t x = 0;
  int16_t y = 0;
  int16_t xv = -1;
  int16_t yv = -1;

  while (!bi.error) {
    bitmap_clear(&dub_buff.bitmap);
    image_draw(&bi, MAP_IMG, x, y);
    doublebuffer_swap(&dub_buff);

    x += xv;
    if ((x < xmin) || (x > xmax)) {
      xv = -xv;
      x += xv;
    }
    y += yv;
    if ((y < ymin) || (y > ymax)) {
      yv = -yv;
      y += yv;
    }
  }

  // an error occurred.  If you don't see images, add some debug messages here
  while (1) {
    sleep_ms(1000);
  }
}
