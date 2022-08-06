bin_PROGRAMS = evlcm
dist_noinst_DATA = README.md
EXTRA_DIST = .build-aux/git-version-gen

AM_CFLAGS = -std=gnu99 $(LCM_CFLAGS)

evlcm_SOURCES = config.h \
	src/evlcm.h \
	src/evlcm-argp.h \
	src/evlcm.c

MOSTLYCLEANFILES = *.gz *.bz2 *.xz
