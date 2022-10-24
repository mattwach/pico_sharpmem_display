#include "sharpdisp/bitmapconsole.h"
#include <stdarg.h>
#include <stdio.h>

void bitmap_console_init(struct BitmapConsole* c, struct BitmapText* text) {
  c->text = text;
  c->vscroll = 0;
  c->is_scrolling = 0;
  c->printf_buffer = 0;
  bitmap_console_clear(c);
}

void bitmap_console_clear(struct BitmapConsole* c) {
  bitmap_clear(c->text->bitmap);
  c->is_scrolling = 0;
  c->text->x = 0;
  c->text->y = 0;
}

static void bitmap_console_scroll(struct BitmapConsole* c) {
  struct BitmapText* t = c->text;
  const uint8_t height = text_height(t);
  const uint16_t disp_height = t->bitmap->height - height + 1;

  // increment y and reset x
  t->x = 0;
  t->y += height;
  if (t->y >= disp_height) {
    c->is_scrolling = 1;
    t->y = 0;
  }

  if (c->is_scrolling) {
    c->vscroll += height;
    if (c->vscroll >= disp_height) {
      c->vscroll = 0;
    }
  }

  // clear the current line
  memset(
      t->bitmap->data + (t->y * t->bitmap->width_bytes),
      t->bitmap->clear_byte,
      height * t->bitmap->width_bytes);
}

void bitmap_console_char(struct BitmapConsole* c, char ch) {
  struct BitmapText* t = c->text;
  if (ch == '\n') {
    bitmap_console_scroll(c);
    return;
  }

  uint16_t width = text_char_width(t, ch);
  if ((t->x + width) > t->bitmap->width) {
    bitmap_console_scroll(c);
  }

  text_char(t, ch);
}

void bitmap_console_printf(struct BitmapConsole* c, const char* fmt, ...) {
  if (!c->printf_buffer) {
    // User never set this up
    return;
  }
  va_list args;
  va_start(args, fmt);
  vsprintf(c->printf_buffer, fmt, args);
  va_end(args);
  bitmap_console_str(c, c->printf_buffer);
}

