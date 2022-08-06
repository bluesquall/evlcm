#ifndef _EV_LCM_H_
#define _EV_LCM_H_

#define Y_CHANNEL "y"
#define FREQUENCY 1.2

#include <ev.h>

static void
timeout_cb(EV_P_ ev_timer* w, int revents);

#endif // _EV_LCM_H_
