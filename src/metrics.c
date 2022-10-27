#include "sharpdisp/metrics.h"
#include "pico/stdlib.h"
#include <string.h>

static inline uint32_t uptime_ms() {
  return to_ms_since_boot(get_absolute_time());
}

void metrics_init(struct SharpMetrics* m) {
  memset(m, 0, sizeof(struct SharpMetrics));
}

void metrics_start(struct SharpMetrics* m) {
  m->next_start_ms = uptime_ms();
}

void metrics_prerefresh(struct SharpMetrics* m) {
  m->start_ms = m->next_start_ms;
  m->start_refresh_ms = uptime_ms();
}

void metrics_postrefresh(struct SharpMetrics* m, uint32_t frame_ms) {
  ++m->frame_index;
  m->end_refresh_ms = uptime_ms();

  const uint32_t total_ms = m->end_refresh_ms - m->start_ms;
  if (total_ms < frame_ms) {
      sleep_ms(frame_ms - total_ms);
  }
  m->finish_ms = uptime_ms();
}

void metrics_refresh(
    struct SharpMetrics* m, struct SharpDisp* sd, uint32_t frame_ms) {
  metrics_prerefresh(m);
  sharpdisp_refresh(sd);
  metrics_postrefresh(m, frame_ms);
}
