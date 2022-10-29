#include "pico/stdlib.h"
#include <sharpdisp/bitmaptext.h>
#include <sharpdisp/doublebuffer.h>
#include <sharpdisp/metrics.h>
#include <sharpdisp/sharpdisp.h>
#include <fonts/liberation_sans_18.h>

// Bouncing the balls around looks cool but adds a lot of complicated
// logic.  Thus all of that code/complexity is moved to balls.c, so this
// file can focus on showcasing the double buffering logic.
#include "common.h"
#include "ball.h"

// you can experiment with changing the SPI speed to see how
// the double buffering responds.  At 10 Mhz, we are around 14ms
// per refresh so making it much slower will fairly quickly exceed the
// 16.7 ms needed for 60 FPS.
#define SPI_FREQ_HZ 10000000
#define SLEEP_MS 16

struct SharpDisp display;
struct DoubleBuffer dub_buff;
struct SharpMetrics metrics;
struct BitmapText text;

// need to provide buffers for the display (two because it's double buffering)
// and another so that text_printf will work.
uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];
uint8_t disp_buffer2[BITMAP_SIZE(WIDTH, HEIGHT)];
char printf_buffer[256];

static void init_metrics() {
  // Metric makes this example a bit more complex than needed, but
  // it is useful/interesting to show FPS and draw time as
  // a demonstration of the effecitveness of the method.
  metrics_init(&metrics);
  text_init(&text, liberation_sans_18, &display.bitmap);
  text.printf_buffer = printf_buffer;
}

static void draw_metrics(void) {
  text.x = 8;
  text.y = HEIGHT - 18;
  if (metrics.frame_index > 0) {
    text_printf(
        &text,
        "Draw: %u ms FPS: %u",
        metrics_draw_ms(&metrics),
        1000 / metrics_total_ms(&metrics));     
  }
}

int main() {
  sleep_ms(100);  // allow voltage to stabilize
  sharpdisp_init_freq_hz(
    &display,
    disp_buffer,
    WIDTH,
    HEIGHT,
    0xFF,
    SPI_FREQ_HZ);
  doublebuffer_init(&dub_buff, &display, disp_buffer2, SLEEP_MS);
  init_metrics();
  init_balls(&dub_buff.bitmap);

  while (1) {
    metrics_start(&metrics);
    bitmap_clear(&dub_buff.bitmap);
    draw_balls();
    draw_metrics();
    metrics_prerefresh(&metrics);
    doublebuffer_swap(&dub_buff);  // This instead of sharpdisp_refresh()
    metrics_postrefresh(&metrics, 0);
  }
}
