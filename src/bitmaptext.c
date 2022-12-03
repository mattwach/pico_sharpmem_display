#include "sharpdisp/bitmaptext.h"
#include <stdarg.h>
#include <stdio.h>
#include "rle.h"

void text_verify_font(struct BitmapText* text) {
  if (text->error) {
    return;
  }
  const struct SharpMemoryFont* font = (struct SharpMemoryFont*)text->font;
  const uint8_t* id = font->id;
  if ((id[0] != 'S') ||
      (id[1] != 'H') || 
      (id[2] != 'F') ||
      (id[3] != '1')) {
    text->error = TEXT_BAD_FONT_ID_ERROR;
  }
}

static const uint8_t* find_character_data(
    const struct SharpMemoryFont* font,
    const char c,
    uint8_t* width) {
  const uint8_t num_chars = font->num_chars;

  // binary search
  uint8_t mask = 0x80;
  while (mask > num_chars) {
    mask >>= 1;
  }

  uint8_t search_idx = 0;
  char slot_c = 0;
  for (; mask; mask >>= 1) {
    search_idx |= mask;
    if (search_idx >= num_chars) {
      // too high
      search_idx &= ~mask;
    } else {
      slot_c = font->data[search_idx << 2];
      if (slot_c > c) {
        // overshot the mark
        search_idx &= ~mask;
      }
    }
  }

  const uint8_t* slot_addr = font->data + (search_idx << 2);
  if (c == slot_addr[0]) {
    // found it
    *width = slot_addr[1];
    const uint16_t offset = (uint16_t)((slot_addr[2] << 8) | slot_addr[3]);
    return font->data + offset;
  }

  // did not find anything
  return 0;
}

void text_char(struct BitmapText* text, char c) {
  if (text->error) {
    return;
  }

  const struct SharpMemoryFont* font = (struct SharpMemoryFont*)text->font;
  const uint8_t height = font->height;
  uint8_t width = 0;
  const uint8_t* pgm_data = find_character_data(font, c, &width);
  uint8_t* error = &(text->error);

  map_rle_image(
    text->bitmap,
    pgm_data,
    text->x,
    text->y,
    width,
    height,
    error);

  if (text->x < text->bitmap->width) {
    text->x += width;
  }
}


uint8_t text_char_width(struct BitmapText* text, char c) {
  const struct SharpMemoryFont* font = (struct SharpMemoryFont*)text->font;
  uint8_t width = 0;
  find_character_data(font, c, &width);
  return width;
}


void text_init(struct BitmapText* text, const void* font, struct Bitmap* bitmap) {
  text->font = font;
  text->bitmap = bitmap;
  text->x = 0;
  text->y = 0;
  text->error = 0;
  text->printf_buffer = NULL;
  text_verify_font(text);
}

void text_printf(struct BitmapText* text, const char* fmt, ...) {
  if (!text->printf_buffer) {
    // User never set this up
    return;
  }
  va_list args;
  va_start(args, fmt);
  vsprintf(text->printf_buffer, fmt, args);
  va_end(args);
  text_str(text, text->printf_buffer);
}
