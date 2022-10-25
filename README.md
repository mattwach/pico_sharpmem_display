# pico_sharpmem_display
A Raspberry PI Pico library for driving a Sharp Memory Module, such as the
LS013B4DN04 or LS027B7DH01.  Includes low, mid and high level functions.

# Summary

This library is build on the pico-sdk SPI libraries.  It provides an API with
three conceptual layers:

   * *Low level*: At this level the display is represented as a `uint8_t[]` array. 
     You place bytes in the array and then call `sharpdisp_refresh()` to see
     results.  If you are trying to add sharp support to a different graphics
     library, this is probably the code you want to look at.
   * *Mid Level*: This adds functions for drawing text and simple shapes.
     Examples include `text_str()` from writing a string (with a choice of fonts)
     and `bitmap_circle()` for drawing a circle.
   * *High Level*: At this level, the sharp display is no longer directly
     visible but used lower down.  The one current exaple is the `console.h`
     library which lets you create and print text to a scrolling console with
     concering yourself with all of the setup otherwise needed to get it
     working.

# Getting Started - Hello World

## Hardware Connections

![hello world](images/hello_world.jpg)

## Building firmware

The mid level API is probably where you want to start.  From the parent
directory:

    ./bootstrap.sh
    cd build
    cd examples/midlevel/hello_world
    make -j

If all goes to plan, you will now have a `sharpdisp_hello_world.uf2` file
that you can load onto the pico.  You can use the USB drive method or
this command:

    picotool load sharpdisp_hello_world.uf2

of course this only works if the pico is ready to accept code (as
with loading any program to the pico) by holding the boot button
on reset or power on.

# In depth Hello World Analysis

Lets look at the code (located at `examples/midlevel/hello_world`):

```c
#include "pico/stdlib.h"
#include <fonts/liberation_sans_36.h>
#include <sharpdisp/sharpdisp.h>
#include <sharpdisp/bitmapshapes.h>
#include <sharpdisp/bitmaptext.h>

#define WIDTH 400
#define HEIGHT 240

uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];

int main() {
  sleep_ms(100);  // allow voltage to stabilize

  // Initailize
  struct SharpDisp sd;
  sharpdisp_init_default(&sd, disp_buffer, WIDTH, HEIGHT, 0x00);
  struct BitmapText text;
  text_init(&text, liberation_sans_36, &sd.bitmap);

  // Print Hello World!
  const char* hello = "Hello World!";
  text.x = (WIDTH - text_str_width(&text, hello)) / 2;  // center the string
  text.y = (HEIGHT - text_height(&text)) / 2;
  text_str(&text, hello);

  // Make a border
  const uint16_t border = 15;
  bitmap_rect(
      &sd.bitmap, border, border, WIDTH - border * 2, HEIGHT - border * 2);

  // Send to hardware
  sharpdisp_refresh(&sd);

  while (1) {
    sleep_ms(1000);
  }
}
```

## Initialization

Breaking it down.  We first need to initialize data structures:

```c
uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];

int main() {
  ...
  // Initailize
  struct SharpDisp sd;
  sharpdisp_init_default(&sd, disp_buffer, WIDTH, HEIGHT, 0x00);
  struct BitmapText text;
  text_init(&text, liberation_sans_36, &sd.bitmap);
  ...
}
```

## SharpDisp Initilization

Note that we need to provide the buffer memory.  Some libraries do this for you
with `malloc()` but dynamic memory allocation is not a great default on a
resource-limited microcontroller (you are free to create a `malloc()` helper
yourself if you want that).

`BITMAP_SIZE` is used to calculate the number of bytes needed.  The calculation
is generally `(WIDTH / 8) * HEIGHT` but there is a tricky corner case involving
widths that are not a multiple of 8.  All existing sharp displays do have a multiple
of 8 width but an offscreen buffer you make yourself might not - `BITMAP_SIZE`
gets the calculation correct in these cases too.

The last byte is the fill byte.  `0x00` will lead to a black background while
`0xFF` will lead to a white one.  You could use other values here to make a
striped background but, outside of experimentation. you probably will not want
to do this.

Let's look at the `SharpDisp` and `Bitmap` structures a bit:

```c
sharpdisp/sharpdisp.h

struct SharpDisp {
  spi_inst_t *spi;  // choice of SPI hardware
  struct Bitmap bitmap;
  uint8_t cs_pin;  // pin choice for CS
  uint8_t vcom;  // internal state
};
```

Not much in this structure of interest outside of `bitmap` which you will often
need to pass to drawing functions (as `bitmap_rect()` is doing above).

```c
sharpdisp/bitmap.h

#define BITMAP_WHITE   0x01
#define BITMAP_BLACK   0x02
#define BITMAP_INVERSE 0x03

struct Bitmap {
  uint16_t width;
  uint16_t width_bytes;
  uint16_t height;
  uint8_t  mode;
  uint8_t  clear_byte;
  uint8_t* data;
};
```

The `width` and `height` fields are the dimensions of `data` in pixels.  The
`width_bytes` field is the width in bytes.  In most cases it will be `width/8`
as there are 8 pixels per byte.  But what if `width = 10`?  In that case
`width_bytes = 2` so, if you want to manipulate `data` directly, make sure you
use `width_bytes` instead of doing the calcualtion yourself.

The `mode` field is one of `BITMAP_WHITE`, 'BITMAP_BLACK` and `BITMAP_INVERSE`.
It is used by higher-level drawing functions such as `text_str()` and `bitmap_rect()`
to decide how to draw pixels.

The `clear_byte` field defines if the background color is black or white (or some
striped pattern).  It is used by libraries that want to scroll the bitmap,
such as `console.h`.

## Text Initilization

Here is the `BitmapText` structure:

```c
sharpdisp/bitmaptext.h


struct BitmapText {
    const uint8_t* font;    // Pointer to some font data
    struct Bitmap* bitmap;  // Pointer to the bitmap to update
    uint16_t x;
    uint16_t y;
    uint8_t error;
};
```

The `font` pointer points to a compiled object in the `fonts/` directory.  It
is purposefully left typeless so that different font formats can be supported.

The `bitmap` pointer is usually set to the same place as `SharpDisp.bitmap` but
you might choose a different bitmap if you want to do double buffering or
other special techniques.

The `x` and `y` fields point to the upper-right location of the next character.
After each character is output, `x` is automatically incremented by that
character's width.  `y` is not incremented, thus your mid-level code will
need to manage the `y` coordinate (if you use an API like `bitmapconsole.h`
or `console.h`, then `y` is updated for you in the manner that those
libraries deem appropriate.)

The `error` field should rarely be needed.  If you don't see any text, you can
check this field for non-zero values and compare those to the codes listed in
`bitmaptext.h`

## Print Hello World

```c
int main() {
  ...
  const char* hello = "Hello World!";
  text.x = (WIDTH - text_str_width(&text, hello)) / 2;  // center the string
  text.y = (HEIGHT - text_height(&text)) / 2;
  text_str(&text, hello);
  ...
}
```

Most of the complexity here comes from centering the text on the display, you could
go a simple route and just say:

      text_str(&text, "Hello World!");

but then the text would display in the upper-left corner (`text_init` sets `x` and `y`
to zero).

## Draw a rectangle

```c
int main() {
  ...
  const uint16_t border = 15;
  bitmap_rect(
      &sd.bitmap, border, border, WIDTH - border * 2, HEIGHT - border * 2);
  ...
}
```

A straight-forward task.  The format is:

```c
sharpdisp/bitmapshapes.h

void bitmap_rect(
    struct Bitmap* bitmap,
    uint16_t x,
    uint16_t y,
    uint16_t w,
    uint16_t h);
```

The same header file provides function for drawing lines, ovals, and flood fills.
The base `bitmap.h` file contains function for setting/getting individual pixels
(`bitmap_point()` and `bitmap_get_point()`).

## Update the hardware

```c
int main() {
  ...
  sharpdisp_refresh(&sd);
  ...
}
```

Everything before this point was just changing the `disp_buffer[]` array in memory.
The `sharpdisp_refresh()` function is what sends this buffer to the hardware.
At the default SPI speed of 10Mhz (which you can change), it will take about
2 ms to do the send and the code will be blocked during the operation.
You could streamline the system by taking advantage of the
Picos 2 processor cores, calling `sharpdisp_refresh()` on one CPU while the bitmap
for the next frame is prepared on the other (e.g. double-buffering).  Doing this
is straight-forward, assuming you are already experienced in working with
multithreaded systems (which is a topic that is way beyond the scope of this
document).  Later I might add a helper library for double buffering but
2 ms is not an actual problem for anything I'm currently using the library for.

# High Level API - The Console Interface

High level APIs here are ones that hide as much boilrplate as possible to allow
specific usecases to be streamlized.  The usecase in question is a scrolling
console.  You might want to add a scrolling console to some random project that
will not even use a sharp memory display in it's final form, just as a way
to view debug messages while it is running (a computer USB can also be used
for this purpose, assuming you have a dedicated computer for the task and
your project is not using USB for something else).  Here is some code:


