#include "sharpdisp/doublebuffer.h"
#include <stdio.h>
#include "pico/stdlib.h"
#include "pico/util/queue.h"
#include "pico/multicore.h"

// Global data used by core 1
static struct CoreData {
    struct DoubleBuffer* db;
    uint32_t frame_period_ms;
} core1;

static inline uint32_t uptime_ms() {
  return to_ms_since_boot(get_absolute_time());
}

void doublebuffer_init(
  struct DoubleBuffer* db, struct SharpDisp* disp, uint8_t* buff2) {
  db->disp = disp;
  bitmap_init(
      &(db->bitmap),
      buff2,
      disp->bitmap.width,
      disp->bitmap.height,
      disp->bitmap.mode,
      disp->bitmap.clear_byte);
  uint32_t unused_entry = 0;
  queue_init(&(db->frame_is_rendered), sizeof(unused_entry), 1);
  queue_init(&(db->frame_is_ready), sizeof(unused_entry), 1);
  queue_add_blocking(&(db->frame_is_rendered), &unused_entry);
}

void doublebuffer_start_core1(void) {
    uint32_t unused_entry = 0;
    uint32_t last_render_time = 0;
    while (1) {
        queue_remove_blocking(&(core1.db->frame_is_ready), &unused_entry);
        const uint32_t current_time = uptime_ms();
        if ((current_time - last_render_time) < core1.frame_period_ms) {
            sleep_ms(core1.frame_period_ms - (current_time - last_render_time));
        }
        last_render_time = uptime_ms();
        sharpdisp_refresh(core1.db->disp);
        queue_add_blocking(&(core1.db->frame_is_rendered), &unused_entry);
    }
}

void doublebuffer_start(struct DoubleBuffer* db, uint32_t frame_period_ms) {
    core1.db = db;
    core1.frame_period_ms = frame_period_ms;
    multicore_launch_core1(doublebuffer_start_core1);    
}

void doublebuffer_swap(struct DoubleBuffer* db) {
    uint32_t unused_entry = 0;
    // wait for any rendering to finish
    queue_remove_blocking(&(db->frame_is_rendered), &unused_entry);
    // swap the frames
    uint8_t* tmp = db->disp->bitmap.data;
    db->disp->bitmap.data = db->bitmap.data;
    db->bitmap.data = tmp;
    // allow core1 to start rendering this frame
    queue_add_blocking(&(db->frame_is_ready), &unused_entry);
}