#include "pico/stdlib.h"
#include <sharpdisp/sharpdisp.h>
#include <sharpdisp/doublebuffer.h>
#include <sharpdisp/bitmap.h>
#include <sharpdisp/bitmapimage.h>
#include <sharpdisp/metrics.h>
#include <sharpdisp/bitmaptext.h>
#include <fonts/liberation_mono_12.h>

#include "map.h"

#define WIDTH 400
#define HEIGHT 240
#define SLEEP_MS 16

uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];
uint8_t disp_buffer2[BITMAP_SIZE(WIDTH, HEIGHT)];
char printf_buffer[256];
struct SharpDisp sd;
struct DoubleBuffer dub_buff;
struct SharpMetrics metrics;
struct BitmapText text;
struct BitmapImages bi;

static void init_metrics() {
  // Metric makes this example a bit more complex than needed, but
  // it is useful/interesting to show FPS and draw time as
  // a demonstration of the effecitveness of the method.
  metrics_init(&metrics);
  text_init(&text, liberation_mono_12, &dub_buff.bitmap);
  text.printf_buffer = printf_buffer;
}

static void draw_metrics(void) {
  text.x = 8;
  text.y = HEIGHT - 18;
  if (metrics.frame_index > 0) {
    const uint8_t old_mode = dub_buff.bitmap.mode;
    dub_buff.bitmap.mode = BITMAP_INVERSE;
    text_printf(
        &text,
        "Draw: %2u ms FPS: %u",
        metrics_draw_ms(&metrics),
        1000 / metrics_total_ms(&metrics));     
    dub_buff.bitmap.mode = old_mode;
  }
}

int main() {
  sleep_ms(100);  // allow voltage to stabilize

  // Initailize
  sharpdisp_init_default(&sd, disp_buffer, WIDTH, HEIGHT, 0xFF);
  doublebuffer_init(&dub_buff, &sd, disp_buffer2, SLEEP_MS);
  image_init(&bi, map, &dub_buff.bitmap);
  init_metrics();

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
    metrics_start(&metrics);
    bitmap_clear(&dub_buff.bitmap);
    image_draw_tiled(&bi, MAP_IMG_0_0, MAP_IMG_COLUMNS, MAP_IMG_ROWS, x, y);
    draw_metrics();
    metrics_prerefresh(&metrics);
    doublebuffer_swap(&dub_buff);
    metrics_postrefresh(&metrics, 0);

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
