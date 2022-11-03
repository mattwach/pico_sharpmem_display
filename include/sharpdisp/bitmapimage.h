#ifndef LIB_SHARPDISP_BITMAPIMAGE
#define LIB_SHARPDISP_BITMAPIMAGE

#include <inttypes.h>
#include "sharpdisp/bitmap.h"

// errors
#define IMAGE_BAD_FONT_ID_ERROR 0x01
#define IMAGE_INVALID_RLE_DATA  0x02

struct BitmapImages {
    const uint8_t* images;    // Pointer to some font data
    struct Bitmap* bitmap;  // Pointer to the bitmap to update
    // Set to non-zero if any error occurs.  Most text functions
    // will do nothing if this is non-zero.
    uint8_t error;
};

struct SharpMemoryImage {
  uint8_t id[4];       // Should be set to 'SHI1'
  uint32_t num_images;   // number of images

  // Now for a lookup table.  Format is
  // uint16_t image_width
  // uint16_t image_height
  // uint32_t offset
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
  // The data itself is composed of vertical 8x1 strips.  This format
  // can make it efficient to transfer data to the bitmap while still
  // taking advantage of RLE patterns.
  uint8_t data[];     
};

// Use this to initialize a text object or change fonts in an existing one
void image_init(struct BitmapImages* images, const void* image_data, struct Bitmap* bitmap);

// Draw an image.   Zero-bits are transparent. One bits will use the Bitmap.mode mode to
// transfer.  Optionally use bitmapshapes.h bitmap_filled_rect() to clear out background
// pixels for simple cases.  If you really need pixel-perfect masking, then you will probably
// want to define a second image which will be the background mask.  For example a
// baseball would first fill in a white circle, then draw a black imamge on top of it
// for stitching/etc.
void image_draw(struct BitmapImages* images, uint32_t id, int16_t x, int16_t y);

#endif

