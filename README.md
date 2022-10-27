# pico_sharpmem_display
The code here is a Raspberry PI Pico library for driving a Sharp Memory Module,
such as the LS013B4DN04 or LS027B7DH01.  Includes low, mid and high level
functions.

What is a Sharp Memory Display?

I consider it an evolution of LCD displays - higher contrast and 60FPS animations
with just a bit of ghosting.  Due to it's high contrast, it is often compared to
e-paper (or eink) displays.  Here is a quick comparison:

EPaper Advantages

  * Zero power needed to display a static image (around 80 uA for the Sharp)
  * Available in more sizes
  * Color versions are available
  * Potentially cheaper

Sharp LCD Advantages

  * Much less energy needed to *update* an image (around 2,000 times less energy)
  * Much faster updates makes animations practical

# Summary

This library is built on the pico-sdk SPI libraries.  It provides an API with
three conceptual layers:

   * *Low level*: At this level the display is represented as a `uint8_t[]` array. 
     You set bytes in the array (each byte represents an 8x1 pixel slice) and
     then call `sharpdisp_refresh()` to see results.  If you are trying to add
     sharp support to a different graphics library, this is probably the code
     you want to look at.
   * *Mid Level*: Functions for drawing text and simple shapes.
     Examples include `text_str()` from writing a string (with a choice of fonts)
     and `bitmap_circle()` for drawing a circle.
   * *High Level*: At this level, the sharp display is no longer directly
     visible but used lower down.  The one existing example is the `console.h`
     library which lets you create and print text to a scrolling console without
     concering yourself with all of the setup code otherwise needed to get it
     working.

# Getting Started - Hello World

## Hardware Connections

![hello world](images/hello_world.jpg)

Hardware connections are inline with a standard PI Pico SPI device.  The pin
assignments are not fixed and can be changed within the limitations of
the hardware (see [this pinout](https://datasheets.raspberrypi.com/pico/Pico-R3-A4-Pinout.pdf)
for an overview and [the design documentation](https://www.raspberrypi.com/documentation/microcontrollers/raspberry-pi-pico.html) for details)

Here they are:

| PI Pin Name | PI Pin # | LCD Pin Name | LCD Pin # | Notes                        |
|-------------|----------|--------------|-----------|------------------------------|
| VSYS        | 39       | VIN          | 1         | VBUS (40) could also be used |
| GND         | 38       | GND          | 3         | Any PI GND can be used       |
| SPI0 SCK    | 24       | CLK          | 4         |                              |
| SPI0 TX     | 25       | DIN          | 5         | Also called MOSI             |
| SPI0 CSn    | 22       | CS           | 6         |                              |

The button array in the image above is providing a route from the RUN pin to ground.
It can be useful for any project to make loading new firmware more convenient and
is not specific to the Sharp LCD.

## Building Firmware

From the parent directory:

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

If the code loads but the image doesn't show, one thing to consider
(along with the usual checking connections) is to lower the SPI
frequency.  Refer to `include/sharpdisp/sharpdisp.h` for guidance on
how to do this (i.e. use `sharpdisp_init_freq_hz()`).

# In-Depth Hello World Analysis

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

## SharpDisp Initialization

```c
uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];

int main() {
  ...
  struct SharpDisp sd;
  sharpdisp_init_default(&sd, disp_buffer, WIDTH, HEIGHT, 0x00);
  ...
}
```

Note that we need to provide the buffer memory.  Some libraries do this for you
with `malloc()` but use of dynamic memory allocation is debatable on a
resource-limited microcontroller.  Feel free to add a `malloc()` init wrapper
if that is your preference.

`BITMAP_SIZE` is used to calculate the number of bytes needed.  The calculation
is generally `(WIDTH / 8) * HEIGHT` but there is a corner case involving
widths that are not a multiple of 8.  All existing sharp displays *do* have a multiple
of 8 width but an offscreen buffer you make yourself might not - `BITMAP_SIZE`
gets the calculation correct in these cases too.

The last parameter represents the background fill byte.  `0x00` will lead to a
black background while `0xFF` will lead to a white one.  You could use other
values here to make a striped background but, outside of experimentation, you
likely will not want to do this.

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
need to pass to drawing functions (as `bitmap_rect()` is doing above).  On to
`Bitmap`, which is a field of `SharpDisp` but can also be used separately:

```c
sharpdisp/bitmap.h

#define BITMAP_WHITE   0x01
#define BITMAP_BLACK   0x02
#define BITMAP_INVERSE 0x03

struct Bitmap {
  uint16_t width;        // width in pixels
  uint16_t width_bytes;  // width in bytes, for buffer calculations
  uint16_t height;       // height in pixels
  uint8_t  mode;         // BITMAP_WHITE, BITMAP_BLACK or BITMAP_INVERSE
  uint8_t  clear_byte;   // 0x00 for black, 0xFF for white
  uint8_t* data;         // contains pixel data
};
```

The `mode` field is used by higher-level drawing functions such as `text_str()`
and `bitmap_rect()` to decide how to draw pixels.

The `clear_byte` field defines if the background "color".  It is used by
libraries that need to "scroll" the bitmap, such as `console.h`.

## Text Initialization

```c
int main() {
  ...
  struct BitmapText text;
  text_init(&text, liberation_sans_36, &sd.bitmap);
  ...
}
```

Here is the `BitmapText` structure:

```c
sharpdisp/bitmaptext.h


struct BitmapText {
    const uint8_t* font;    // Pointer to some font data
    struct Bitmap* bitmap;  // Pointer to the bitmap to update
    uint16_t x;             // Left edge of next drawn character
    uint16_t y;             // Top edge of next drawn character
    uint8_t error;          // Non-zero if any errors happen
};
```

The `font` pointer points to a compiled object in the `fonts/` directory.  It
is purposefully left typeless so that different font formats can be supported.

The `bitmap` pointer is usually set to the same place as `SharpDisp.bitmap` but
you might choose a different bitmap if you want to do double buffering or
other special techniques.

After a character is drawn, `x` is automatically incremented by that
character's width.  `y` is not incremented, thus your mid-level code will
need to manage the `y` coordinate (if you use an API like `bitmapconsole.h`
or `console.h`, then `y` is updated for you in the manner that those
libraries deem appropriate.)

The `error` field can be ignored unless text is not drawing and you need to
investigate. Non-zero values represent an error an can be referenced in
`bitmaptext.h`.

## Drawing "Hello World!"

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

but then the text would display in the upper-left corner (`text_init()` sets `x` and `y`
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
The base `bitmap.h` file contains functions for setting/getting individual pixels
(`bitmap_point()` and `bitmap_get_point()`).

## Refresh the Sharp LCD

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
Pico's dual processor cores, calling `sharpdisp_refresh()` on one CPU while the bitmap
for the next frame is prepared on the other (e.g. double-buffering).  Doing this
is straight-forward if you are familiar with multithreaded programming but
explaining details go beyond the scope of this guide.
Later I might add a helper library for double buffering, If I happen to need the
performance boost.

# High Level API - The Console Interface

High level APIs here are ones that hide as much boilerplate as possible to allow
specific usecases to be streamlined.  The usecase in question is a scrolling
console.  You might want to add a scrolling console to a project that
will not even use a Graphic LCD in it's final form, as a way
to view debug messages while it is running.  The USB port or SWI interfaces can
also be used here, of course, if you have a spare computer and are not using the
Pico USB hardware for something else.

Building:

    ./bootstrap.sh
    cd build
    cd examples/highlevel/console_count
    make -j


Code:

```c
#include "pico/stdlib.h"
#include <sharpdisp/console.h>

#define WIDTH 400
#define HEIGHT 240

uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];

int main() {
  sleep_ms(100);  // allow voltage to stabilize
  struct Console c;
  console_init_default(&c, disp_buffer, WIDTH, HEIGHT);
  uint32_t i=0;
  while (1) {
    for (uint8_t j=0; j<6; ++j,++i) {
      console_printf(&c, "%d  ", i);
    }
    console_char(&c, '\n');
  }
}
```

The `console_init_defalt()` function makes many decisions for you.  There is
also a `console_init()` alternative that takes many additional parameters,
allowing you to choose a different font, background color, and refresh
rate.  The default refresh rate is 30 Hz.  The LCD does not update with
every write to the console but instead updates at the refresh rate.  At 30
Hz and the default font, this gives the console the ability to
print/scroll around 7,000 lines per second.  You can scroll even faster by
*lowering* the refresh rate as a refresh is the real bottleneck for
performance.

Another trick the console uses for speed is that is calls `sharpdisp_refresh_vscroll()`
instead of `sharpdisp_refresh()`.  The extended form allows you to change where the
`y=0` point in the bitmap is.  Having this ability means that the console needs to
update/move much less data for a scroll event which boosts scrolling performance.
I suppose these are details you don't *need* to know - just know that the console
code is using tricks to be faster than a naive implementation would likely be able
to achieve.

# Low Level API - Direct Array Manipulation

Building:

    ./bootstrap.sh
    cd build
    cd examples/lowlevel/buffer_direct
    make -j

Code:

```c
#include "pico/stdlib.h"
#include <sharpdisp/sharpdisp.h>

#define WIDTH 400
#define HEIGHT 240

uint8_t disp_buffer[BITMAP_SIZE(WIDTH, HEIGHT)];

int main() {
  struct SharpDisp sd;
  sharpdisp_init_default(&sd, disp_buffer, WIDTH, HEIGHT, 0xFF);

  for (uint16_t y=10; y < 110; ++y) {
    for (uint16_t x=1; x <= 10; ++x) {
      disp_buffer[y * (WIDTH / 8) + x] = 0xFF;  // Writes a 8x1 pixel slice
      // note, this would do the same thing
      // sd.bitmap.data[y * sd.bitmap.width_bytes + x] = 0x00;
    }
  }

  sharpdisp_refresh(&sd);
  while (1);  // hang
}
```

For lowlevel, the only files you need to look at are `sharpdisp.c` and `bitmap.c`.
Very little code overall.

The main usecase here you be if you already have a graphics library you are
using and want some example code you can use or port to make the
Sharp LCD a target.

