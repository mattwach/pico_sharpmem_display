#include "sharpdisp/console.h"
#include "fonts/liberation_mono_12.h"
#include "pico/stdlib.h"
#include <stdarg.h>
#include <stdio.h>

static char printf_buffer[PRINTF_BUFFER_SIZE];

static inline uint32_t uptime_ms() {
  return to_ms_since_boot(get_absolute_time());
}

void console_init(
    struct Console* c,
    uint8_t* buff,
    uint16_t width,
    uint16_t height,
    uint8_t clear_byte,
    const void* font,
    uint32_t refresh_period_ms,
    uint32_t spi_freq_hz) {
  sharpdisp_init_freq_hz(&(c->display), buff, width, height, clear_byte, spi_freq_hz);
  text_init(&(c->text), font, &(c->display.bitmap));
  bitmap_console_init(&(c->console), &(c->text));
  c->text.printf_buffer = printf_buffer;
  c->refresh_period_ms = refresh_period_ms;
  c->next_refresh_ms = 0;
  console_flush(c);
}

void console_init_default(
    struct Console* c,
    uint8_t* buff,
    uint16_t width,
    uint16_t height) {
  console_init(
      c,
      buff,
      width,
      height,
      0xFF,
      liberation_mono_12,
      32,
      8000000);
}

void console_clear(struct Console* c) {
  bitmap_console_clear(&(c->console));
  console_flush(c);
}

static void console_refresh(struct Console* c) {
  const uint32_t timestamp_ms = uptime_ms();
  if (timestamp_ms >= c->next_refresh_ms) {
    sharpdisp_refresh_vscroll(&(c->display), c->console.vscroll); 
    c->next_refresh_ms = timestamp_ms + c->refresh_period_ms;
  }
}

void console_flush(struct Console* c) {
  c->next_refresh_ms = 0;
  console_refresh(c);
}

void console_sleep_ms(struct Console* c, uint32_t ms) {
  const uint32_t t1 = uptime_ms();
  console_flush(c);
  const uint32_t delta = uptime_ms() - t1;
  if (delta < ms) {
    sleep_ms(ms - delta);
  }
}

void console_char(struct Console* c, char ch) {
  bitmap_console_char(&(c->console), ch); 
  console_refresh(c);
}

void console_printf(struct Console* c, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vsnprintf(printf_buffer, PRINTF_BUFFER_SIZE - 1, fmt, args);
  va_end(args);
  console_str(c, printf_buffer);
}
