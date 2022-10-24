#ifndef LIB_SHARPDISP
#define LIB_SHARPDISP

#include "hardware/spi.h"
#include "sharpdisp/bitmap.h"

// Sharp memory display controller API
//
// Here is an example that draws a 40x50 filled rectangle starting at 10,10
//
// #define WIDTH 400
// #define HEIGHT 240
//
// uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)]
//
// void main() {
//   struct SharpDisp sd;
//   sharpdisp_init_default(&sd, disp_buffer, WIDTH, HEIGHT, 0xFF);
//   for (uint16_t x=0; x < 40; ++x) {
//     for (uint16_t y=0; y < 50; ++y) {
//       // bitmap_set_byte would be quicker but more complicated
//       bitmap_point(&sd.bitmap, 10 + x, 10 + y);
//     }
//   }
//
//   sharpdisp_refresh(&sd);
//   while (1);
// }

struct SharpDisp {
  spi_inst_t *spi;  // choice of SPI hardware
  struct Bitmap bitmap;
  uint8_t cs_pin;  // pin choice for CS
  uint8_t vcom;  // internal state
};

void sharpdisp_init(
    struct SharpDisp* s, // object to initialize
    uint8_t* buff, // client provides the memory buffer
    uint16_t width,  // display width, in pixels
    uint16_t height,  // display height, in pixels
    uint8_t clear_byte, // 0x00 for black, 0xFF for white
    uint8_t cs_pin,  // choice for CS pin,
    uint8_t sck_pin,  // choice for SCK pin, constrained by spi choice - see datasheet
    uint8_t mosi_pin, // choice for mosi pin, constrained by spi choice - see datasheet
    spi_inst_t *spi,  // choice of SPI hardware
    uint32_t freq_hz  // spi clock freequency
);

// sharpdisp_init_default provides some reasonable default for cases
// where you are not particular
static inline void sharpdisp_init_freq_hz(
    struct SharpDisp* s,
    uint8_t* buff,
    uint16_t width,
    uint16_t height,
    uint8_t clear_byte,
    uint32_t baud) {
  sharpdisp_init(
      s,
      buff,
      width,
      height,
      clear_byte,
      17,  // cs_pin will be GP17 (pin 22)
      18,  // sck_pin will be GP18 (pin 24)
      19,  // mosi_pin will be GP19 (pin 25)
      spi0,
      baud);
}

static inline void sharpdisp_init_default(
    struct SharpDisp* s,
    uint8_t* buff,
    uint16_t width,
    uint16_t height,
    uint8_t clear_byte) {
  sharpdisp_init_freq_hz(s, buff, width, height, clear_byte, 10000000);
}

// vscroll determines the vertical location of y=0 on the display.
// for eaxample, if the value is 10 then y=0 would start on physical
// line 10, lines off the bottom wrap back to the top.  The advantage
// of using this variable is that you can implement console scrolling
// without needing to use memmove() like operations to scroll the
// display up.
void sharpdisp_refresh_vscroll(struct SharpDisp* s, uint16_t vscroll);

static inline void sharpdisp_refresh(struct SharpDisp* s) {
  sharpdisp_refresh_vscroll(s, 0);
}

#endif

