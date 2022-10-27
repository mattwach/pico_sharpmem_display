#include "pico/stdlib.h"
#include <sharpdisp/bitmapshapes.h>
#include <sharpdisp/bitmaptext.h>
#include <sharpdisp/metrics.h>
#include <sharpdisp/sharpdisp.h>
#include <fonts/liberation_sans_18.h>

#define WIDTH 400  // 400 pixels
#define HEIGHT 240
#define RADIUS 10
#define SPI_FREQ_HZ 10000000
#define SLEEP_MS 16

uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];
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
  struct SharpMetrics m;
  metrics_init(&m);
  struct BitmapText text;
  text_init(&text, liberation_sans_18, &sd.bitmap);
  text.printf_buffer = printf_buffer;

  int16_t x = 50;
  int16_t y = 50;
  int16_t x_vel = 2;
  int16_t y_vel = 1;
  const int16_t max_x = WIDTH - RADIUS;
  const int16_t max_y = HEIGHT - RADIUS;

  while (1) {
    metrics_start(&m);

    bitmap_clear(&sd.bitmap);
    bitmap_filled_circle(&sd.bitmap, x, y, RADIUS);
    text.x = 8;
    text.y = HEIGHT - 32;
    if (m.frame_index > 0) {
      text_printf(
          &text,
          "Draw: %u ms Refresh: %u ms FPS: %u",
          metrics_draw_ms(&m),
          metrics_refresh_ms(&m),
          1000 / metrics_total_ms(&m));     
    }

    x += x_vel;
    if ((x < RADIUS) || (x > max_x)) {
      x_vel = -x_vel;
      x += x_vel;
    }
    y += y_vel;
    if ((y < RADIUS) || (y > max_y)) {
      y_vel = -y_vel;
      y += y_vel;
    }

    metrics_refresh(&m, &sd, SLEEP_MS);
  }
}
