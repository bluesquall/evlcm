#ifndef _EV_LCM_H_
#define _EV_LCM_H_

#define X_CHANNEL "x"
#define Y_CHANNEL "y"
#define FREQUENCY 1.2

#include "evlcm-argp.h"
#include "evlcm_data_t.h"

#include <ev.h>
#include <lcm/lcm.h>

static inline int64_t
ev_utime(struct ev_loop* loop);

static void
lcm_subscriber_cb(EV_P_ ev_io* w, int revents);

static void
stdin_cb(EV_P_ ev_io* w, int revents);

static void
timeout_cb(EV_P_ ev_timer* w, int revents);

static void
y_handler(const lcm_recv_buf_t* rbuf,
          const char* channel,
          const evlcm_data_t* rx,
          void* user);

static void
sine_thread_loop(const double publish_timeout);

static void
cosine_thread_loop(const double publish_timeout);

#endif // _EV_LCM_H_
