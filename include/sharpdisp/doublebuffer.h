#ifndef LIB_SHARPDISP_DOUBLEBUFFER
#define LIB_SHARPDISP_DOUBLEBUFFER

// Provides a mechanism for multi-core double-buffering.
//
// sharpdisp_refresh is blocking and can take a significant amount of time
// The measured refresh time at an SPI freqiency of 10Mhz is 14 ms.  If you want
// a 60 FPS refresh, you will need to trun frames every 16ms so not a lot of
// time for the drawing code to operate in this case.
//
// A solution is to use double buffering.
// The implementation uses both of the pi picos cores.  Core 0 is the general use
// core and core 1 will be used to send the SPI signals (Core 1 can also be used
// for other things if you need that).
//
// There are 2 buffers.  Core 0 writes to one while Core 1 sends output to SPI
// from the others.  The buffers swap on every frame rendered.
//
// It is possible for core 0 to complete much faster than core 1 or visa versa,
// therefore a locking system is needed to appropriately coordinate the usage
// of the two buffers.
//
// Example:
//
// #define WIDTH 400
// #define HEIGHT 240
//
// uint8_t disp_buffer1[BITMAP_SIZE(WIDTH, HEIGHT)]
// uint8_t disp_buffer2[BITMAP_SIZE(WIDTH, HEIGHT)]
//
// void main() {
//   struct SharpDisp sd;
//   sharpdisp_init_default(&sd, disp_buffer, WIDTH, HEIGHT, 0x00);
//   struct DoubleBuffer db;
//   doublebuffer_init(&db, &sd, disp_buffer2);
//   while (1) {
//     // db.bitmap will point to disp_buffer1 or disp_buffer2
//     draw_something(&db.bitmap);
//     // Waits for the refresh
//     doublebuffer_swap(&db);
//   }
// }

#include "sharpdisp/bitmap.h"
#include "sharpdisp/sharpdisp.h"
#include "pico/util/queue.h"

struct DoubleBuffer {
  // currently rendering
  struct SharpDisp* disp;
  // for writing
  struct Bitmap bitmap;
  // A single slot queue.  When it has an entry, the CPU1 is free to render disp
  queue_t frame_is_ready;
  // A single slot queue.  When it has an entry, CPU0 is free to swap the buffers
  queue_t frame_is_rendered;
};

// The _nostart version is used when you want to start up and control cpu1
// yourself so that you can use it for more things than just pushing frames.
// When using this method, your CPU1 thread will need to call
// doublebuffer_core1_render() oftten enough to achieve the desired FPS.
// Also note that doublebuffer_core1_render() can block up to frame_period_ms
// or even longer (if cpu0 is not clearing the freame for rendering).  Thus
// you will probably want to do all of your other CPU1 work first, then
// cann dobulebuffer_core1_render() at the end to wrap up the frame.
// Alternatively, set frame_period_ms to 0 and handle the FPS management your
// own way.
void doublebuffer_init_nostart(
    struct DoubleBuffer* db,
    struct SharpDisp* disp,
    uint8_t* buff2,
    uint32_t frame_period_ms);

void doublebuffer_init(
    struct DoubleBuffer* db,
    struct SharpDisp* disp,
    uint8_t* buff2,
    uint32_t frame_period_ms);

void doublebuffer_swap(struct DoubleBuffer* db);

// Only call this function if you used doublebuffer_init_nostart().  It is
// called in the case where you are setting up CPU1 yourself.  When calling
// doublebuffer_init(), all of the CPU1 setup is handled for you in trade for
// not being able to use CPU1 for anything outside of rendering frames.
void doublebuffer_core1_render(void);

#endif