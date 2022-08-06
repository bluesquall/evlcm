bin_PROGRAMS = evlcm
dist_noinst_DATA = README.md src/evlcm_data_t.lcm
EXTRA_DIST = .build-aux/git-version-gen

evlcm_%_t.c evlcm_%_t.h: src/evlcm_%_t.lcm
	$(LCMGEN) --c --c-hpath @builddir@ --c-cpath @builddir@ $^

AM_CFLAGS = -std=gnu99 $(LCM_CFLAGS)

evlcm_SOURCES = config.h \
	src/evlcm.h \
	src/evlcm-argp.h \
	evlcm_data_t.h \
	evlcm_data_t.c \
	src/evlcm.c

MOSTLYCLEANFILES = *.gz *.bz2 *.xz
