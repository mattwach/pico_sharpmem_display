#ifndef LIB_SHARPDISP_BITMAPIMAGE
#define LIB_SHARPDISP_BITMAPIMAGE

#include <inttypes.h>
#include "sharpdisp/bitmap.h"

// errors
#define IMAGE_BAD_ID_ERROR      0x01
#define IMAGE_BAD_RLE_DATA            0x02
#define IMAGE_INVALID_ID              0x03
#define IMAGE_UNEXPECTED_HEADER_SIZE  0x04

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
void image_init(struct BitmapImages* bi, const void* image_data, struct Bitmap* bitmap);

// Draw an image.   Zero-bits are transparent. One bits will use the Bitmap.mode mode to
// transfer.  Optionally use bitmapshapes.h bitmap_filled_rect() to clear out background
// pixels for simple cases.  If you really need pixel-perfect masking, then you will probably
// want to define a second image which will be the background mask.  For example a
// baseball would first fill in a white circle, then draw a black imamge on top of it
// for stitching/etc.
void image_draw(struct BitmapImages* bi, uint32_t id, int16_t x, int16_t y);

// image_draw_tiled helps make drawing very large images more efficient.
// The strategy is that you break an image into rectangular tiles of
// equal size (except for the far left and bottom) in the following
// pattern
//
// 01234
// 56789
//
// The example above would show the layout of a image broken into 5x2 tiles
//
// The way this is optimized is that the function will skip drawing tiles
// that are completly off the bitmap.  For images that are significantly
// larger than the bitmap, significant amounts of drawing will be skipped.
//
// A good general tile size is 64x64 pixels.  This gives 512 byte uncompressed
// tiles which gives the RLE engine something to compress.  To get actual ideal
// numbers, benchmark different sizes using sharpmetrics.h or similar.
//
// The make_images.py tool supports splitting a large image into tiles for you
// Provide it with tile_x and tile_y parameters and the image will be divided up.
void image_draw_tiled(
  struct BitmapImages* bi,
  uint32_t first_id,
  uint16_t columns,  // number of tiled columns
  uint16_t rows, // number of tiled rows
  int16_t x,
  int16_t y);

uint16_t image_width(struct BitmapImages* bi, uint32_t id);
uint16_t image_height(struct BitmapImages* bi, uint32_t id);
uint32_t image_count(struct BitmapImages* bi);

// Determines the overall width and height of a tiled image
uint16_t image_width_tiled(
  struct BitmapImages* bi, uint32_t first_id, uint16_t columns);
uint16_t image_height_tiled(
  struct BitmapImages* bi, uint32_t first_id, uint16_t columns, uint16_t rows);

#endif

