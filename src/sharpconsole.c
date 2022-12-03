#include "sharpdisp/sharpconsole.h"
#include "fonts/liberation_mono_12.h"
#include "pico/stdlib.h"
#include <stdarg.h>
#include <stdio.h>

static inline uint32_t uptime_ms() {
  return to_ms_since_boot(get_absolute_time());
}

void sharpconsole_init(
    struct Console* c,
    struct SharpDisp* display,
    const void* font,
    uint32_t refresh_period_ms) {
  c->display = display;
  text_init(&(c->text), font, &(c->display->bitmap));
  bitmap_console_init(&(c->console), &(c->text));
  c->text.printf_buffer = c->printf_buffer;
  c->refresh_period_ms = refresh_period_ms;
  c->next_refresh_ms = 0;
  sharpconsole_flush(c);
}

void sharpconsole_init_default(
    struct Console* c,
    uint8_t* buff,
    uint16_t width,
    uint16_t height) {
  static struct SharpDisp display;
  sharpdisp_init_default(&display, buff, width, height, 0xFF);
  sharpconsole_init(
      c,
      &display,
      liberation_mono_12,
      32);
}

void sharpconsole_clear(struct Console* c) {
  bitmap_console_clear(&(c->console));
  sharpconsole_flush(c);
}

static void sharpconsole_refresh(struct Console* c) {
  const uint32_t timestamp_ms = uptime_ms();
  if (timestamp_ms >= c->next_refresh_ms) {
    sharpdisp_refresh_vscroll(c->display, c->console.vscroll); 
    c->next_refresh_ms = timestamp_ms + c->refresh_period_ms;
  }
}

void sharpconsole_flush(struct Console* c) {
  c->next_refresh_ms = 0;
  sharpconsole_refresh(c);
}

void sharpconsole_sleep_ms(struct Console* c, uint32_t ms) {
  const uint32_t t1 = uptime_ms();
  sharpconsole_flush(c);
  const uint32_t delta = uptime_ms() - t1;
  if (delta < ms) {
    sleep_ms(ms - delta);
  }
}

void sharpconsole_char(struct Console* c, char ch) {
  bitmap_console_char(&(c->console), ch); 
  sharpconsole_refresh(c);
}

void sharpconsole_printf(struct Console* c, const char* fmt, ...) {
  va_list args;
  va_start(args, fmt);
  vsnprintf(c->printf_buffer, PRINTF_BUFFER_SIZE - 1, fmt, args);
  va_end(args);
  sharpconsole_str(c, c->printf_buffer);
}
