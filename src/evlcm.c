#include "config.h"

#include "evlcm.h"
#include "evlcm-argp.h"

#include <ev.h>
#include <lcm/lcm.h>

#include <stdlib.h>


static void timeout_cb (EV_P_ ev_timer *w, int revents) {
    puts ("timeout");
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
