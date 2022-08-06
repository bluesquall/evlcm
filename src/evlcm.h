#ifndef _EV_LCM_H_
#define _EV_LCM_H_

#include <ev.h>

ev_timer timeout_watcher;

static void timeout_cb (EV_P_ ev_timer *w, int revents);

#endif // _EV_LCM_H_
