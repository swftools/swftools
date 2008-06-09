AC_DEFUN([RFX_CHECK_MING],
[
AC_CANONICAL_HOST
AC_EXEEXT

AC_MSG_CHECKING([for target system])
CYGWIN=
MINGW=
case $host_os in
    *cygwin* ) CYGWIN=yes;;
    *mingw* ) MINGW=yes;;
esac

AR=ar
if test "x$MINGW" = "xyes"; then
    #fix for the debian distribution of mingw
    if test -x "/usr/i586-mingw32msvc/bin/ar";then
	AR="/usr/i586-mingw32msvc/bin/ar"
    fi
    if test -x "/usr/i586-mingw32msvc/bin/ranlib";then
	RANLIB="/usr/i586-mingw32msvc/bin/ranlib"
    fi
    #fix for the gentoo distribution of mingw
    if test -x "/opt/xmingw/bin/i386-mingw32msvc-ar";then
	AR="/opt/xmingw/bin/i386-mingw32msvc-ar"
    fi
    if test -x "/opt/xmingw/bin/i386-mingw32msvc-ranlib";then
	RANLIB="/opt/xmingw/bin/i386-mingw32msvc-ranlib"
    fi
    if test -x "/opt/xmingw/bin/i386-mingw32msvc-strip";then
	STRIP="/opt/xmingw/bin/i386-mingw32msvc-strip"
    fi
fi
AC_SUBST(AR)

dnl Checks for system services

if test "x${CYGWIN}" = "xyes"; then
    AC_DEFINE([CYGWIN], [1], [Define on cygwin])
    AC_MSG_RESULT(cygwin)
else
    if test "x${MINGW}" = "xyes"; then
	AC_DEFINE([MINGW], [1], [Define on Mingw])
	WIN32=1
	AC_DEFINE([WIN32], [1], [Define on windows])
	LIBS="$LIBS -lws2_32 -lgdi32"
	AC_MSG_RESULT([mingw])
    else
	LINUX=1
	export LINUX
	AC_DEFINE([LINUX], [1], [Define if not on cygwin or mingw])
	AC_MSG_RESULT()
    fi
fi
])
