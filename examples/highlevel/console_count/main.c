#include "pico/stdlib.h"
#include <sharpdisp/console.h>

#define WIDTH 400
#define HEIGHT 240

uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];

int main() {
  sleep_ms(100);  // allow voltage to stabilize
  struct Console c;
  console_init_default(&c, disp_buffer, WIDTH, HEIGHT);
  uint32_t i=0;
  while (1) {
    for (uint8_t j=0; j<6; ++j,++i) {
      console_printf(&c, "%d  ", i);
    }
    console_char(&c, '\n');
  }
}
