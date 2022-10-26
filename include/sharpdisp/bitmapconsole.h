#ifndef LIB_SHARPDISP_BITMAPCONSOLE
#define LIB_SHARPDISP_BITMAPCONSOLE

#include "sharpdisp/bitmaptext.h"

// This library provides logic for supporting scrollable console displays
// Also see console.h which is requires less code to use in trade for
// being more limited (focusing on the "common" usecase of a full-screen
// console).
//
// Example:
//
// #define WIDTH 400
// #define HEIGHT 240
// uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];
// 
// int main() {
//   struct SharpDisp sd;
//   sharpdisp_init_default(&sd, disp_buffer, WIDTH, HEIGHT);
//   struct BitmapText text;
//   text_init(&text, terminal_font, &sd.bitmap);
//   struct BitmapConsole console;
//   bitmap_console_init(&console, &text, 0xFF);
//   while (1) {
//     bitmap_console_str(&console, "Hello World!\n");
//     sharpdisp_refresh_vscroll(&sd, console.vscroll);
//     sleep_ms(1000);
//   }
// }
//

struct BitmapConsole {
  struct BitmapText* text;
  // for the sake of efficiency, the bitmap is wrapped and the vscroll
  // parameter is used with the sharpdisp_refresh_vscroll() function for
  // better scrolling efficiency (less memory movement needed).
  uint16_t vscroll;
  // a flag used by the logic to tell if the console is in a scrolling state
  uint8_t is_scrolling;
};

void bitmap_console_init(
    struct BitmapConsole* c,
    struct BitmapText* text);

// clear console and reset text position
void bitmap_console_clear(struct BitmapConsole* c);

// output a single character to the console, wrapping and scrolling
// as-needed.
void bitmap_console_char(struct BitmapConsole* c, char ch);

static inline void bitmap_console_strlen(
    struct BitmapConsole* c,
    const char* str,
    uint16_t length) {
  for (uint16_t i=0; i<length; ++i) {
    bitmap_console_char(c, str[i]);
  }
}

static inline void bitmap_console_str(struct BitmapConsole* c, const char* str) {
  for (; *str; ++str) {
    bitmap_console_char(c, *str);
  }
}

// Used to support printf-style calling.  IMPORTANT: you MUST set BitmapText.printf_buffer
// before calling this.  If it is null, then calling this function is a NOOP
void bitmap_console_printf(struct BitmapConsole* c, const char* fmt, ...);

#endif
