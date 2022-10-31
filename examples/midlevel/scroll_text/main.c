#include "pico/stdlib.h"
#include <fonts/liberation_sans_80.h>
#include <sharpdisp/sharpdisp.h>
#include <sharpdisp/bitmaptext.h>

#define WIDTH 400
#define HEIGHT 240

#define MINXY -80
#define MAXXY HEIGHT

#define FRAME_MS 16

uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];

static inline uint32_t uptime_ms() {
  return to_ms_since_boot(get_absolute_time());
}

int main() {
  sleep_ms(100);  // allow voltage to stabilize

  struct SharpDisp sd;
  sharpdisp_init_default(&sd, disp_buffer, WIDTH, HEIGHT, 0x00);
  struct BitmapText text;
  text_init(&text, liberation_sans_80, &sd.bitmap);

  int16_t xy = MINXY;
  int16_t delta = 1;

  while (1) {
    const uint32_t t1 = uptime_ms(); 
    bitmap_clear(&sd.bitmap);
    text.x = xy;
    text.y = xy;
    text_str(&text, "Hello World!");
    xy += delta;
    if ((xy < MINXY) || (xy >= MAXXY)) {
      delta = -delta;
      xy += delta;
    }
    sharpdisp_refresh(&sd);
    const uint32_t tdelta = uptime_ms() - t1;
    if (tdelta < FRAME_MS) {
      sleep_ms(FRAME_MS - tdelta);
    }
  }
  
}
