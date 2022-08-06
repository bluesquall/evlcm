#include "config.h"

#include "evlcm.h"
#include "evlcm-argp.h"

#include <ev.h>
#include <lcm/lcm.h>

#include <stdlib.h>

int main( int argc, char ** argv ) {
    args.verbosity = 0;
    argp_parse( &argp, argc, argv, 0, 0, &args );

    if(args.verbosity > 0) {
        puts("Aloha!");
    }

    exit( EXIT_SUCCESS );
}
