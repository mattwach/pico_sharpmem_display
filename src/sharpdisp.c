#include "sharpdisp/sharpdisp.h"
#include "pico/stdlib.h"

#define SHARPMEM_BIT_WRITECMD 0x80
#define SHARPMEM_BIT_VCOM 0x40
#define SHARPMEM_BIT_CLEAR 0x20

static void sharpdisp_init_spi(
    spi_inst_t* spi,
    uint8_t cs_pin,
    uint8_t sck_pin,
    uint8_t mosi_pin,
    uint32_t freq_hz) {
  gpio_init(cs_pin);
  gpio_set_dir(cs_pin, GPIO_OUT);
  gpio_put(cs_pin, 0);  // this display is low on inactive

  spi_init(spi, freq_hz);
  // just a placeholder as the spi_init defaults already set everything
  // the same way.
  //spi_set_format(
  //    spi,
  //    8,  // bits per transfer
  //    0,  // polarity
  //    0,  // phase
  //    SPI_MSB_FIRST);
  gpio_set_function(mosi_pin, GPIO_FUNC_SPI);
  gpio_set_function(sck_pin, GPIO_FUNC_SPI);
}

void sharpdisp_init(
    struct SharpDisp* s,
    uint8_t* buff,
    uint16_t width,
    uint16_t height,
    uint8_t clear_byte,
    uint8_t cs_pin,
    uint8_t sck_pin,
    uint8_t mosi_pin,
    spi_inst_t* spi,
    uint32_t freq_hz) {
  bitmap_init(
    &(s->bitmap),
    buff,
    width,
    height,
    clear_byte ? BITMAP_BLACK : BITMAP_WHITE,
    clear_byte);
  bitmap_clear(&(s->bitmap));
  s->cs_pin = cs_pin;
  s->spi = spi;
  s->vcom = SHARPMEM_BIT_VCOM;
  sharpdisp_init_spi(spi, cs_pin, sck_pin, mosi_pin, freq_hz);
}

uint8_t reverse_bits(uint8_t b) {
  uint8_t c = 0;
  for (uint8_t x=0; x < 8; ++x) {
    c = (c << 1) | (b & 1);
    b >>= 1;
  }
  return c;
}

void sharpdisp_refresh_vscroll(struct SharpDisp* s, uint16_t vscroll) {
  gpio_put(s->cs_pin, 1);
  busy_wait_us_32(6);  // Datasheet tsSCS value

  uint8_t spi_byte = s->vcom | SHARPMEM_BIT_WRITECMD;
  spi_write_blocking(s->spi, &spi_byte, 1);
  // Not sure why this needs to be inverted each time but Adafruit_SharpMem.cpp
  // is doing it that way.  The specification is not making the reason clear
  // to me.
  s->vcom ^= SHARPMEM_BIT_VCOM;

  const uint16_t height = s->bitmap.height;
  uint16_t memy = vscroll;
  for (uint16_t y = 0; y < height; ++y, ++memy) {
    if (memy >= height) {
      memy = 0;  // wrap around
    }
    // send the line address, which is apparently 1-based
    spi_byte = reverse_bits(y + 1);
    spi_write_blocking(s->spi, &spi_byte, 1);
    // send the data
    spi_write_blocking(s->spi, s->bitmap.data + s->bitmap.width_bytes * memy, s->bitmap.width_bytes);
    // end with a zero for some reason
    spi_byte = 0;
    spi_write_blocking(s->spi, &spi_byte, 1);
  }

  // for some reason, end all data with another zero
  spi_byte = 0;
  spi_write_blocking(s->spi, &spi_byte, 1);

  gpio_put(s->cs_pin, 0);
  busy_wait_us_32(2);  // Datasheet thSCS
}
