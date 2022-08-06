AC_INIT([evlcm],
	m4_esyscmd([.build-aux/git-version-gen .tarball-version]),
	[https://github.com/bluesquall/evlcm/issues],
	[],
	[https://github.com/bluesquall/evlcm],
)
AC_CONFIG_AUX_DIR([.build-aux])
AC_CONFIG_MACRO_DIRS([.build-aux/m4])
AC_CONFIG_HEADERS([config.h])
AM_INIT_AUTOMAKE([subdir-objects no-dist-gzip dist-xz -Wall -Werror foreign])
AC_CONFIG_FILES([Makefile])

PKG_CHECK_MODULES([LCM], [lcm >= 1.3.0])

AC_CHECK_PROG([LCMGEN], [lcm-gen], [lcm-gen], [/bin/false])
AS_IF([test "${LCMGEN}" == "x/bin/false"],
      AC_MSG_ERROR([LCM generator `lcm-gen` not found])])

AC_SEARCH_LIBS([ev_run],[ev])
AC_SEARCH_LIBS([lcm_create],[lcm])
AC_SEARCH_LIBS([sin],[m])

AC_PROG_CC
AC_OUTPUT
