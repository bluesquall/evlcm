#include "config.h"

#include "evlcm.h"
#include "evlcm-argp.h"

#include <ev.h>
#include <lcm/lcm.h>

#include <inttypes.h>
#include <stdlib.h>


static inline int64_t ev_utime (struct ev_loop * loop) {
    return (int64_t) (1e6 * ev_now (loop));
}


static void timeout_cb (EV_P_ ev_timer *w, int revents) {
    printf ("timeout @ t=%f s\t(%" PRId64 " us)\t",
            ev_now (EV_A), ev_utime (EV_A));
    // ^XXX^ not sure how I feel about libev using float representation of time

    // this causes the innermost ev_run to stop iterating
    ev_break (EV_A_ EVBREAK_ONE);
}


int main (int argc, char ** argv) {
    args.verbosity = 0;
    argp_parse (&argp, argc, argv, 0, 0, &args);

    if (args.verbosity > 0) {
        puts ("Aloha!");
    }

    struct ev_loop *loop = EV_DEFAULT;
    ev_timer_init (&timeout_watcher, timeout_cb, 5.5, 0.);
    ev_timer_start (loop, &timeout_watcher);

    ev_run (loop, 0);

    exit (EXIT_SUCCESS);
}
