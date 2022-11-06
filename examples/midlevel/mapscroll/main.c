#include "pico/stdlib.h"
#include <sharpdisp/sharpdisp.h>
#include <sharpdisp/doublebuffer.h>
#include <sharpdisp/bitmap.h>
#include <sharpdisp/bitmapimage.h>

#include "map.h"

#define WIDTH 400
#define HEIGHT 240
#define SLEEP_MS 25

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
  image_init(&bi, map, &dub_buff.bitmap);

  const int16_t xmin = WIDTH - image_width_tiled(
    &bi, MAP_IMG_0_0, MAP_IMG_COLUMNS);
  const int16_t xmax = 0;
  const int16_t ymin = HEIGHT - image_height_tiled(
    &bi, MAP_IMG_0_0, MAP_IMG_COLUMNS, MAP_IMG_ROWS);
  const int16_t ymax = 0;

  int16_t x = 0;
  int16_t y = 0;
  int16_t xv = -1;
  int16_t yv = -1;

  while (!bi.error) {
    bitmap_clear(&dub_buff.bitmap);
    image_draw_tiled(&bi, MAP_IMG_0_0, MAP_IMG_COLUMNS, MAP_IMG_ROWS, x, y);
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
