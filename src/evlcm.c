#include "config.h"

#include "evlcm.h"
#include "evlcm-argp.h"
#include "evlcm_y_t.h"

#include <ev.h>
#include <lcm/lcm.h>

#include <inttypes.h>
#include <math.h>
#include <stdlib.h>
#include <unistd.h>


static inline int64_t ev_utime (struct ev_loop * loop) {
    return (int64_t) (1e6 * ev_now (loop));
}


static void timeout_cb (EV_P_ ev_timer *w, int revents) {
    lcm_t *lcm = (lcm_t*)(w->data);
    evlcm_y_t tx = {0};
    tx.utime = ev_utime (EV_A);
    tx.y = sin ( 2 * M_PI * FREQUENCY * ev_now (EV_A));
    // N.B. ev_utime and ev_now refer to the same time as long as called for the same event
    evlcm_y_t_publish(lcm, Y_CHANNEL, &tx);
    if (args.verbosity > 0) {
        fprintf(stderr, "published y = %f on channel `%s`\n", tx.y, Y_CHANNEL);
    }
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

    if (args.verbosity > 0) { puts ("Aloha!"); }

    lcm_t * lcm = lcm_create(NULL);
    if (!lcm) { exit (EXIT_FAILURE); }

    ev_timer timeout_watcher;
    ev_io stdin_watcher;

    timeout_watcher.data = (void *)lcm;

    struct ev_loop *loop = EV_DEFAULT;
    ev_timer_init (&timeout_watcher, timeout_cb, 0., 1e-1);
    ev_timer_again (loop, &timeout_watcher);

    ev_io_init (&stdin_watcher, stdin_cb, STDIN_FILENO, EV_READ);
    ev_io_start (loop, &stdin_watcher);

    ev_run (loop, 0);

    exit (EXIT_SUCCESS);
}
