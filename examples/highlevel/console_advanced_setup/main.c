#include "pico/stdlib.h"
#include <sharpdisp/sharpdisp.h>
#include <fonts/liberation_sans_18.h>
#include <sharpdisp/sharpconsole.h>
#include "hardware/spi.h"

// This shows how to setup a consle using a more advanced setup
// You'll need to do this if you want specialized settings (for
// example choosing different output pins).
//
// See highlevel/console_count for a more straight forward
// example that uses default settings.

#define WIDTH 400
#define HEIGHT 240

uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];

int main() {
  sleep_ms(100);  // allow voltage to stabilize
  struct SharpDisp sd;
  sharpdisp_init(
      &sd,
      disp_buffer,
      WIDTH,
      HEIGHT,
      0x00,
      17,  // GP17 for CS
      18,  // GP18 for SCK
      19,  // GP19 for MOSI
      spi0,
      10000000
  );
  struct Console c;
  sharpconsole_init(&c, &sd, liberation_sans_18, 32);
  for (uint32_t i=0;; ++i) {
    sharpconsole_printf(&c, "%d\n", i);
  }
}
