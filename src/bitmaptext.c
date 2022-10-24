#include "sharpdisp/bitmaptext.h"

struct RLETracker {
  const uint8_t* pgm_data;  // address of next byte
  uint8_t bytes_remaining;  // number of bytes remaining on the current run
  uint8_t repeat_mode;  // if true, then we are in repeat mode
};

static uint8_t next_rle_byte(struct RLETracker* self, uint8_t* err) {
  if (*err) {
    return 0xFF;
  }
  if (self->bytes_remaining == 0) {
    // prep for the next sequence
    self->bytes_remaining = self->pgm_data[0];
    ++self->pgm_data;
    if (self->bytes_remaining & 0x80) {
      self->bytes_remaining &= 0x7F;
      self->repeat_mode = 0;
    } else {
      self->repeat_mode = 1;
    }

    if (self->bytes_remaining == 0) {
      // data is invalid
      *err = TEXT_INVALID_RLE_DATA;
      return 0xFF;
    }
  }

  const uint8_t b = self->pgm_data[0];
  --self->bytes_remaining;
  if (!self->repeat_mode || (self->bytes_remaining == 0)) {
    ++self->pgm_data;
  }
  return b;
}

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

static void map_char_data(
    struct Bitmap* b,
    uint16_t x,
    uint16_t y,
    uint8_t data) {
  if (!data) {
    return;
  }
  if (y >= b->height) {
    // off the bottom
    return;
  }

  const uint8_t bit_offset = x & 0x07;
  if (bit_offset) {
    // not on an 8-bit boundary, therefore we need to execute this as two mappings
    map_char_data(b, x & 0xFFF8, y, data >> bit_offset);
    map_char_data(b, (x & 0xFFF8) + 8 , y, data << (8 - bit_offset));
    return;
  }

  // thanks to the logic above, we can now assume that x is on an 8-bit boundary
  uint16_t col = x >> 3;
  if (col >= b->width_bytes) {
    // off the edge
    return;
  }

  uint8_t* addr = b->data + (y * b->width_bytes) + col;
  bitmap_apply(addr, b->mode, data);
}

void text_char(struct BitmapText* text, char c) {
  if (text->error) {
    return;
  }

  const struct SharpMemoryFont* font = (struct SharpMemoryFont*)text->font;
  const uint8_t height = font->height;

  uint8_t width = 0;
  struct RLETracker rle_tracker;
  rle_tracker.bytes_remaining = 0;
  rle_tracker.pgm_data = find_character_data(font, c, &width);

  if (!rle_tracker.pgm_data) {
    // Character not found
    return;
  }

  const uint8_t num_cols = (width + 7) >> 3;
  struct Bitmap* bitmap = text->bitmap;
  const uint16_t x = text->x;
  const uint16_t y = text->y;
  uint8_t* error = &(text->error);

  // vertical stripes
  for (uint8_t col = 0; col < num_cols; ++col) {
    // each strip is height in length
    for (uint8_t row = 0; row < height; ++row) {
      if (!text->error) {
        map_char_data(
            bitmap,
            x + (col * 8),
            y + row,
            next_rle_byte(&rle_tracker, error));
      }
    }
  }

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
  text_verify_font(text);
}

