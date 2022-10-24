#ifndef LIB_SHARPDISP_BITMAPTEXT
#define LIB_SHARPDISP_BITMAPTEXT

#include <inttypes.h>
#include "sharpdisp/bitmap.h"

// errors
#define TEXT_BAD_FONT_ID_ERROR 0x01
#define TEXT_INVALID_RLE_DATA  0x02

struct BitmapText {
    const uint8_t* font;    // Pointer to some font data
    struct Bitmap* bitmap;  // Pointer to the bitmap to update
    // Coordinates of next character to print (upper left corner).  X will automatically
    // advance after printing, unless if is already past the end of the buffer
    uint16_t x;
    uint16_t y;
    // Set to non-zero if any error occurs.  Most text functions
    // will do nothing if this is non-zero.
    uint8_t error;
};

struct SharpMemoryFont {
  uint8_t id[4];       // Should be set to 'SHF1'
  uint8_t num_chars;   // number of characters
  uint8_t height;      // the height of each character in pixels

  // Now for a lookup table.  Format is
  // uint8_t char_idx
  // uint8_t char_width
  // uint16_t offset
  // ...
  //
  // Where offset is the number of bytes from the start of data[]
  // char_idx must be in order to support binary search

  // then comes all of the data bytes.  These are stored in a simple
  // but effective RLE format of the following pattern:
  // l1, b  Length of sequence, sequence byte (l1 must be <128)
  // l2 | 0x80, b1, b2, b3 ...  Length of non repeat followed by bytes
  //
  // example:
  // 0x05 0xFF 0x83 0x01 0x02 0x03
  //
  // Would result in
  // 0xFF 0xFF 0xFF 0xFF 0xFF 0x01 0x02 0x03
  //
  // The pattern is effective due to all of the whitespace and repeating
  // patterns in a typical font.  Think about 'H', '-', '.' etc...
  //
  // The data itself is composed of vertical 8x1 strips.  This format
  // can make it efficient to transfer data to the bitmap while still
  // taking advantage of RLE patterns.
  uint8_t data[];     
};

// Use this to initialize a text object or change fonts in an existing one
void text_init(struct BitmapText* text, const void* font, struct Bitmap* bitmap);

// Output A Single character
void text_char(struct BitmapText* text, char c);

// output a sequence of characters
static inline void text_strlen(struct BitmapText* text, const char* str, uint16_t length) {
  for (uint16_t i=0; i<length; ++i) {
    text_char(text, str[i]);
  }
}

// output a null-terminated string
static inline void text_str(struct BitmapText* text, const char* str) {
  for (; *str; ++str) {
    text_char(text, *str);
  }
}

// returns the pixel height of the current font
static inline uint8_t text_height(struct BitmapText* text) {
  const struct SharpMemoryFont* font = (struct SharpMemoryFont*)(text->font);
  return font->height;
}

// returns the pixel width of a character in the current font
uint8_t text_char_width(struct BitmapText* text, char c);

// returns the pixel width of a length-provided string
static inline uint16_t text_strlen_width(struct BitmapText* text, const char* str, uint16_t length) {
  uint16_t len = 0;
  for (uint16_t i=0; i<length; ++i) {
    len += text_char_width(text, str[i]);
  }
  return len;
}

// returns the pixel width of a null-terminated string
static inline uint16_t text_str_width(struct BitmapText* text, const char* str) {
  uint16_t len = 0;
  for (; *str; ++str) {
    len += text_char_width(text, *str);
  }
  return len;
}

#endif

