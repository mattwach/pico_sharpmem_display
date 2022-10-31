#ifndef LIB_SHARPDISP_BITMAPSHAPES
#define LIB_SHARPDISP_BITMAPSHAPES

#include "sharpdisp/bitmap.h"

// Draws a horizontal line
//   bitmap: The bitmap to draw to
//   x: starting x
//   y: y coordicate
//   w: Width (line draws to the right)
//   bit_op: Bit operator.  e.g. bitmap_OR
void bitmap_hline(struct Bitmap* bitmap, int16_t x, int16_t y, uint16_t w);

// Draws a vertical line
//   bitmap: The bitmap to draw to
//   x: x coordinate
//   y: starting y coordinate
//   h: height
//   bit_op: Bit operator.  e.g. bitmap_OR
void bitmap_vline(struct Bitmap* bitmap, int16_t x, int16_t y, uint16_t h);

// Draws a line.  Prefer bitmap_hline and bitmap_vline
// for best performance.
//   bitmap: The bitmap to draw to
//   x0, y0: starting coordinate
//   x1, y1: ending coordinate
//   bit_op: Bit operator.  e.g. bitmap_OR
void bitmap_line(
    struct Bitmap* bitmap,
    int16_t x0,
    int16_t y0,
    int16_t x1,
    int16_t y1);

// Draws a rectangle
//   bitmap: The bitmap to draw to
//   x: top x coordinate
//   y: left y coordinate
//   w: width
//   h: height
//   bit_op: Bit operator.  e.g. bitmap_OR
void bitmap_rect(
    struct Bitmap* bitmap,
    uint16_t x,
    uint16_t y,
    uint16_t w,
    uint16_t h);
void bitmap_filled_rect(
    struct Bitmap* bitmap,
    uint16_t x,
    uint16_t y,
    uint16_t w,
    uint16_t h);

// Draws an oval.  Preper bitmap_circle
// when drawing circles for a small code footprint.
//   bitmap: The bitmap to draw to
//   cx: Center x
//   cy: Center y
//   r: radius (circle only)
//   rx: 0 degree radius
//   ry: 90 degtee radius
//   bit_op: Bit operator.  e.g. bitmap_OR
void bitmap_oval(
    struct Bitmap* bitmap,
    uint16_t cx,
    uint16_t cy,
    uint16_t rx,
    uint16_t ry);
void bitmap_filled_oval(
    struct Bitmap* bitmap,
    uint16_t cx,
    uint16_t cy,
    uint16_t rx,
    uint16_t ry);
void bitmap_circle(
    struct Bitmap* bitmap,
    uint16_t cx,
    uint16_t cy,
    uint16_t r);
void bitmap_filled_circle(
    struct Bitmap* bitmap,
    uint16_t cx,
    uint16_t cy,
    uint16_t r);

// Sets or clears a given region using a "flood fill" algorithm.
//
// Note: bitmap->mode must be BITMAP_BLACK or BITMAP_WHITE or this
// function will return without doing anything.  This is to avoid a
// potential infinite loop of setting/clearing pixels (as would happen
// with BITMAP_INVERSE).
//
//  bitmap: the bitmap
//  x: Starting x coordinate
//  y: Starting y coordinate
//  set: If != 0 then sert pixels, oterwise clear them
//  queue, queue_length: IMPORTANT: queue_length must be a
//    power of and at least 8 (e.g. 8, 16, 32, 64, 128, ...)
//    Flood fill needs some working memory to keep track
//    of regions-to-explore.  How much depends on the complexity of the
//    task.  Usually a queue_length of around 150 will work for most cases,
//    but much less for simple cases.  The amount of memory needed is
//    deterministic based on the bitmap contents and starting point.
//    Providing insufficient memory will result in the function returning
//    with some areas unfilled.
void bitmap_flood_fill(
    struct Bitmap* bitmap,
    uint16_t x,
    uint16_t y,
    uint32_t* queue,
    uint16_t queue_length);

#endif

