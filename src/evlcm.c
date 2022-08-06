#include "config.h"

#include "evlcm.h"

#include <inttypes.h>
#include <math.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>

static inline int64_t
ev_utime(struct ev_loop* loop)
{
  return (int64_t)(1e6 * ev_now(loop));
}

static void
lcm_subscriber_cb(EV_P_ ev_io* w, int revents)
{
  if (args.verbosity > 1) {
    fprintf(stderr, "handling LCM input\n");
  }
  lcm_t* lcm = (lcm_t*)(w->data);
  lcm_handle(lcm);
  if (args.verbosity > 1) {
    fprintf(stderr, "\tLCM input handled\n");
  }
}

static void
stdin_cb(EV_P_ ev_io* w, int revents)
{
  if (args.verbosity > 2) {
    fprintf(stderr, "%s:%d\n", __FILE__, __LINE__);
  }
  char c = (char)getchar();
  if ('q' == c) {
    ev_io_stop(EV_A_ w);
    ev_break(EV_A_ EVBREAK_ALL);
  } else {
    fprintf(stderr, "unrecognized input: %c\n", c);
  }
}

static void
sine_publisher_cb(EV_P_ ev_timer* w, int revents)
{
  lcm_t* lcm = (lcm_t*)(w->data);
  evlcm_data_t tx = { 0 };
  tx.utime = ev_utime(EV_A);
  tx.data = sin(2 * M_PI * FREQUENCY * ev_now(EV_A));
  // N.B. ev_utime and ev_now refer to the same time
  //      (as long as called for the same event)
  evlcm_data_t_publish(lcm, Y_CHANNEL, &tx);
  if (args.verbosity > 0) {
    fprintf(stderr, "published y = %f on channel `%s`\n", tx.data, Y_CHANNEL);
  }
}

static void
y_handler(const lcm_recv_buf_t* rbuf,
          const char* channel,
          const evlcm_data_t* rx,
          void* user)
{
  ev_timer* w = (ev_timer*)(user);
  lcm_t* lcm = (lcm_t*)(w->data);

  evlcm_data_t tx = { 0 };
  tx.utime = tx.utime;
  // TODO: figure out how to access the `ev_loop` for `ev_utime(EV_A);`
  tx.data = cos(asin(rx->data));
  // ^ there's probably a more numerically stable way
  evlcm_data_t_publish(lcm, X_CHANNEL, &tx);
  if (args.verbosity > 0) {
    fprintf(stderr, "published x = %f on channel `%s`\n", tx.data, X_CHANNEL);
  }
}

static void*
sine_thread_loop(void* data)
{
  double publish_period = *(double*)data;

  lcm_t* lcm = lcm_create(NULL);
  if (!lcm) {
    exit(EXIT_FAILURE);
  }

  ev_timer timeout_watcher;
  timeout_watcher.data = (void*)lcm;

  struct ev_loop* loop = EV_DEFAULT;

  ev_timer_init(&timeout_watcher, sine_publisher_cb, 0., publish_period);
  ev_timer_again(loop, &timeout_watcher);

  ev_run(loop, 0);

  pthread_exit(NULL);
}

static void*
cosine_thread_loop(void* data)
{
  double publish_period = *(double*)data;

  lcm_t* lcm = lcm_create(NULL);
  if (!lcm) {
    exit(EXIT_FAILURE);
  }

  ev_io lcm_watcher;
  lcm_watcher.data = (void*)lcm;

  // ev_timer timeout_watcher;
  // timeout_watcher.data = (void*)lcm;

  struct ev_loop* loop = EV_DEFAULT;

  ev_io_init(&lcm_watcher, lcm_subscriber_cb, lcm_get_fileno(lcm), EV_READ);
  ev_io_start(loop, &lcm_watcher);

  // ev_timer_init(&timeout_watcher, cosine_publisher_cb, 0., publish_timeout);
  // ev_timer_again(loop, &timeout_watcher);

  evlcm_data_t_subscription_t* y_sub =
    evlcm_data_t_subscribe(lcm, Y_CHANNEL, &y_handler, (void*)(&lcm_watcher));
  evlcm_data_t_subscription_set_queue_capacity(y_sub, 3);

  ev_run(loop, 0);

  pthread_exit(NULL);
}

int
main(int argc, char** argv)
{
  args.verbosity = 0;
  argp_parse(&argp, argc, argv, 0, 0, &args);

  if (args.verbosity > 0) {
    puts("Aloha!");
  }

  double sine_publish_period = SINE_PUBLISH_PERIOD;
  double cosine_publish_period = COSINE_PUBLISH_PERIOD;

  pthread_t sine_thread, cosine_thread;
  int sine_thread_create_rc = pthread_create(
    &sine_thread, NULL, sine_thread_loop, (void*)(&sine_publish_period));
  if (sine_thread_create_rc != 0) {
    perror("pthread_create");
  }
  int cosine_thread_create_rc = pthread_create(
    &cosine_thread, NULL, cosine_thread_loop, (void*)(&cosine_publish_period));
  if (cosine_thread_create_rc != 0) {
    perror("pthread_create");
  }

  /*
  ev_io stdin_watcher;
  struct ev_loop* loop = EV_DEFAULT;
  ev_io_init(&stdin_watcher, stdin_cb, STDIN_FILENO, EV_READ);
  ev_io_start(loop, &stdin_watcher);

  ev_run(loop, 0);
  */

  pthread_exit(NULL);

  exit(EXIT_SUCCESS);
}
