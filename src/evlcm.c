#include "config.h"

#include "evlcm.h"
#include "evlcm-argp.h"

#include <ev.h>
#include <lcm/lcm.h>

#include <inttypes.h>
#include <stdlib.h>
#include <unistd.h>


static inline int64_t ev_utime (struct ev_loop * loop) {
    return (int64_t) (1e6 * ev_now (loop));
}


static void timeout_cb (EV_P_ ev_timer *w, int revents) {
    fprintf (stderr, "timeout @ t=%f s\t(%" PRId64 " us)\n",
            ev_now (EV_A), ev_utime (EV_A));
    // ^XXX^ not sure how I feel about libev using float representation of time
}


static void stdin_cb (EV_P_ ev_io *w, int revents) {
    if (args.verbosity > 2) {
        fprintf (stderr, "%s:%d\n", __FILE__, __LINE__);
    }
    char c = (char)getchar();
    if ('q' == c) {
        ev_io_stop (EV_A_ w);
        ev_break (EV_A_ EVBREAK_ALL);
    } else {
        fprintf (stderr, "unrecognized input: %c\n", c);
    }
}


int main (int argc, char ** argv) {
    args.verbosity = 0;
    argp_parse (&argp, argc, argv, 0, 0, &args);

    if (args.verbosity > 0) {
        puts ("Aloha!");
    }


    ev_timer timeout_watcher;
    ev_io stdin_watcher;

    struct ev_loop *loop = EV_DEFAULT;
    ev_timer_init (&timeout_watcher, timeout_cb, 0., 1.);
    ev_timer_again (loop, &timeout_watcher);

    ev_io_init (&stdin_watcher, stdin_cb, STDIN_FILENO, EV_READ);
    ev_io_start (loop, &stdin_watcher);

    ev_run (loop, 0);

    exit (EXIT_SUCCESS);
}
