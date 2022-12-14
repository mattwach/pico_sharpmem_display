#ifndef LIB_SHARPDISP_CONSOLE
#define LIB_SHARPDISP_CONSOLE

// This library provides logic for supporting scrollable
// console displays with added SharpDisp integration
//
// This library is build for convienence in common cases.
// If this library is too rigid for your needs, you might
// look to bitmapconsole.h instead which works directly with the
// bitmap and has no references to the sharp display or text
// objects.  That will allow you do do such things as write
// to off-screen consoles.
//
// Example:
//
// #define WIDTH 400
// #define HEIGHT 240
// uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];
//
// int main() {
//   struct Console console;
//   sharpconsole_init(&console, disp_buffer, WIDTH, HEIGHT, terminal_font, 50);
//   while (1) {
//     sharpconsole_str(&console, "Hello World!\n");
//     sharpconsole_sleep_ms(1000);  // ensures that the console is updated
//   }
// }

#include "sharpdisp/sharpdisp.h"
#include "sharpdisp/bitmaptext.h"
#include "sharpdisp/bitmapconsole.h"

// Add a definition to resize this as-needed
#ifndef PRINTF_BUFFER_SIZE
#define PRINTF_BUFFER_SIZE 256
#endif

struct Console {
  struct SharpDisp* display;
  struct BitmapText text;
  struct BitmapConsole console;
  // How often to refresh the display.  Refreshing the display takes
  // time and will slow down console performance
  uint32_t refresh_period_ms;
  // The next timestamp to refresh
  uint32_t next_refresh_ms;
  char printf_buffer[PRINTF_BUFFER_SIZE];
};

void sharpconsole_init(
    struct Console* c,
    struct SharpDisp* display,
    const void* font,
    uint32_t refresh_period_ms);

void sharpconsole_init_default(
    struct Console* c,
    uint8_t* buff,
    uint16_t width,
    uint16_t height);

void sharpconsole_clear(struct Console* c);

// If your app has long pauses, you should call sharpconsole_flush()
// ahead of the pause unless you are fine with characters not being
// displayed until after the pause is finished and something else
// is printed.
void sharpconsole_flush(struct Console* c);

// convienence method that combines sleep_ms with sharpconsole_flush
// also accounts for the flush time in the sleeping
void sharpconsole_sleep_ms(struct Console* c, uint32_t ms);

// Outputs a single character to the console
void sharpconsole_char(struct Console* c, char ch);

static inline void sharpconsole_strlen(struct Console* c, const char* str, uint16_t length) {
  for (uint16_t i=0; i<length; ++i) {
    sharpconsole_char(c, str[i]);
  }
}

static inline void sharpconsole_str(struct Console* c, const char* str) {
  for (; *str; ++str) {
    sharpconsole_char(c, *str);
  }
}

// Used to support printf-style calling.  Max length is
// PRINTF_BUFFER_SIZE - 1 chars.  Longer string will be trimmed.
void sharpconsole_printf(struct Console* c, const char* fmt, ...);

#endif

