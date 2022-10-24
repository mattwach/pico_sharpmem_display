// Example that is intended to be short and to-the-point
#include "pico/stdlib.h"
#include <sharpdisp/sharpdisp.h>

#define WIDTH 400
#define HEIGHT 240

// We manage the buffer.  Changing the buffer directly
// ourselves (i.e. not with sharpdisp functions) is permitted.
// See bitmap.h for formulas that
// translate between x,y and byte array offset.
uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];

int main() {
  struct SharpDisp sd;
  // Default init picks "reasonable" values for pins, SPI rate etc.
  // see sharpdisp.h for more details and more initilization options.
  sharpdisp_init_default(&sd, disp_buffer, WIDTH, HEIGHT, 0xFF);

  // Draws a filled box using low-level direct access to the array.
  // This is mostly for educational purposes as you would normally
  // opt to use bitmapshapes.h or some other graphics library.
  for (uint16_t y=10; y < 110; ++y) {
    for (uint16_t x=1; x <= 10; ++x) {
      disp_buffer[y * (WIDTH / 8) + x] = 0xFF;  // Writes a 8x1 pixel slice
      // note, this would do the same thing
      // sd.bitmap.data[y * sd.bitmap.width_bytes + x] = 0xFF;
    }
  }
  // Send the buffer to the display hardware
  sharpdisp_refresh(&sd);
  while (1);  // hang
}
