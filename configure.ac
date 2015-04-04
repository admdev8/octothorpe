AC_INIT([octothorpe], [0.1], [dennis@yurichev.com], [octothorpe], [http://github.com/dennis714/octothorpe])
AM_INIT_AUTOMAKE([foreign -Wall -Werror])

CFLAGS="-Wall -std=c11"

AC_PROG_CC
AC_PROG_RANLIB
AM_PROG_AR

AC_CHECK_FUNC(stricmp, stricmp=yes, stricmp=no)
if test $stricmp = no; then
	AC_CHECK_FUNC(strcasecmp, 
		AC_DEFINE(stricmp, strcasecmp, [Define stricmp as strcasecmp if you have one but not the other]),
		AC_MSG_ERROR([Neither stricmp nor strcasecmp found])
	)
fi

AC_CANONICAL_HOST
case "${host}" in
  i[[3456]]86-*-*)
	AC_SUBST([BITS],[32])
        AC_DEFINE([O_BITS32], [], [ ])
	;;
  x86_64-*-*)
	AC_SUBST([BITS],[64])
        AC_DEFINE([O_BITS64], [], [ ])
	;;
  *)
	AC_MSG_ERROR([Unsupported (so far) host: ${host}])
	;;
esac

dnl Do not compile with debugging by default.
AC_ARG_ENABLE([debug],
  [AS_HELP_STRING([--enable-debug], [Build debugging code])],
[if test "x$enable_debug" = "xyes" ; then
  enable_debug="1"
else
  enable_debug="0"
fi
],
[enable_debug="0"]
)
AC_SUBST([enable_debug])

if test "x$enable_debug" = "x1" ; then
  AC_DEFINE([OCTOTHORPE_DEBUG], [], [ ])
  AC_DEFINE([OASSERT_DEBUG], [], [ ])
  AC_DEFINE([RE_USE_DMALLOC], [], [ ])
  AC_SUBST([OCTOTHORPE_DEBUG],[1])
  CFLAGS="$CFLAGS -g -O0"
else
  CFLAGS="$CFLAGS -O3"
fi

AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile lib/Makefile include/Makefile tests/Makefile])
AC_OUTPUT
