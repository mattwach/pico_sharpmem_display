#include "pico/stdlib.h"
#include <sharpdisp/bitmaptext.h>
#include <sharpdisp/doublebuffer.h>
#include <sharpdisp/metrics.h>
#include <sharpdisp/sharpdisp.h>
#include <fonts/liberation_sans_18.h>

#include "common.h"
#include "ball.h"

#define SPI_FREQ_HZ 10000000
#define SLEEP_MS 16

uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];
uint8_t disp_buffer2[BITMAP_SIZE(WIDTH, HEIGHT)];
char printf_buffer[256];

int main() {
  sleep_ms(100);  // allow voltage to stabilize
  struct SharpDisp sd;
  sharpdisp_init_freq_hz(
    &sd,
    disp_buffer,
    WIDTH,
    HEIGHT,
    0xFF,
    SPI_FREQ_HZ);
  sd.bitmap.mode = BITMAP_INVERSE;
  struct DoubleBuffer db;
  doublebuffer_init(&db, &sd, disp_buffer2, SLEEP_MS);
  struct SharpMetrics m;
  metrics_init(&m);
  struct BitmapText text;
  text_init(&text, liberation_sans_18, &sd.bitmap);
  text.printf_buffer = printf_buffer;
  init_balls();

  while (1) {
    metrics_start(&m);

    bitmap_clear(&db.bitmap);
    draw_balls(&db.bitmap);

    text.x = 8;
    text.y = HEIGHT - 18;
    if (m.frame_index > 0) {
      text_printf(
          &text,
          "Draw: %u ms FPS: %u",
          metrics_draw_ms(&m),
          1000 / metrics_total_ms(&m));     
    }

    metrics_prerefresh(&m);
    doublebuffer_swap(&db);
    metrics_postrefresh(&m, 0);
  }
}
