#ifndef LIB_SHARPDISP_METRICS
#define LIB_SHARPDISP_METRICS
// Simple performance stats wrapper.
// This wrapper makes it convienient to add simple performance measurements
//
// Example Usage:
//
// int main() {
//   struct SharpDisp sd;
//   sharpdisp_init_default(&sd, disp_buffer, WIDTH, HEIGHT, 0x00);
//   struct SharpMetrics m;
//   metrics_init(&m);
//   struct BitmapText text;
//   text_init(&text, liberation_sans_14, &sd.bitmap);
// 
//   while (1) {
//     metrics_start(&m);
//     bitmap_clear(&sd);
//     text.x = 8;
//     text.y = 50;
//     if (m.frame_index > 0) {
//       text_printf(
//           "Draw: %u ms Refresh: %u ms FPS: %u",
//           metrics_draw_ms(&m),
//           metrics_refresh_ms(&m),
//           1000 / metrics_total_ms(&m));     
//     }
//     metrics_refresh(&m, &sd, 17);
//   }
// }

#include <inttypes.h>
#include "sharpdisp/sharpdisp.h"

struct SharpMetrics {
    uint32_t frame_index;
    uint32_t start_ms;
    uint32_t start_refresh_ms;
    uint32_t end_refresh_ms;
    uint32_t finish_ms;
    uint32_t next_start_ms;
};

void metrics_init(struct SharpMetrics* m);
// call before drawing anything
void metrics_start(struct SharpMetrics* m);
// call this to render the frame
// If frame_ms is > 0, the code will sleep frame_ms - draw_ms - render_ms which
// should result in a consistent frame rate, asuming that frame_ms is not too
// agressive.
void metrics_refresh(
    struct SharpMetrics* m, struct SharpDisp* sd, uint32_t frame_ms);

// If you are using doublebuffer.h or some other special refresh approach,
// metrics_refresh() will be too high level to be usable.  In that case,
// you can use the two functions below in the following way:
//
// metrics_prerefresh(&m);
// doublebuffer_swap(&db);
// metrics_postrefresh(&m, 0);  // zero becuase doublebuffer_swap handles the delay.
void metrics_prerefresh(struct SharpMetrics* m);
void metrics_postrefresh(struct SharpMetrics* m, uint32_t frame_ms);

// All of these are for the previous frame
static inline uint32_t metrics_draw_ms(struct SharpMetrics* m) {
    return m->start_refresh_ms - m->start_ms;
}

static inline uint32_t metrics_refresh_ms(struct SharpMetrics* m) {
    return m->end_refresh_ms - m->start_refresh_ms;
}

static inline uint32_t metrics_total_ms(struct SharpMetrics* m) {
    return m->finish_ms - m->start_ms;
}

#endif