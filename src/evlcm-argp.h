#ifndef _EV_LCM_ARGP_H_
#define _EV_LCM_ARGP_H_

#include "config.h"

#include <argp.h>
#include <inttypes.h>

extern char **environ;

const char *argp_program_version = PACKAGE_STRING;

const char *argp_program_bug_address = PACKAGE_BUGREPORT;

static char doc[] = "gfscan";

static char args_doc[] = "";

static struct argp_option options[] = {
    { "verbose", 'v', 0, 0, "say more" },
    { 0 }
};

struct arguments {
    int8_t verbosity;
};

static error_t parse_opt( int key, char *arg, struct argp_state *state ) {
    struct arguments *args = state->input;
    switch( key ){
        case 'v':
            args->verbosity += 1;
            break;
        case ARGP_KEY_ARG:
        case ARGP_KEY_END:
            if( state->arg_num != 0 ) argp_usage( state );
            break;
        default:
            return ARGP_ERR_UNKNOWN;
    }
    return 0;
}

static struct argp argp = { options, parse_opt, args_doc, doc };

struct arguments args; // just leave it as a global for now

#endif // _EV_LCM_ARGP_H_
