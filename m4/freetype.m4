# ----------- freetype.m4 ----------------
#
# Checks for:
#    * freetype library (-lfreetype)
#    * freetype headers (freetype2/freetype/freetype.h)
# 
# Environment set:
#    * HAVE_FREETYPE
#
# Defines:
#    * HAVE_FREETYPE
#    * USE_FREETYPE
#
# Substitutes:
#

AC_DEFUN(RFX_CHECK_FREETYPE,
[
AC_CHECK_PROGS(FREETYPE_CONFIG, freetype-config,)
OLDCPPFLAGS="${CPPFLAGS}"
OLDLIBS="${LIBS}"
if test "x${FREETYPE_CONFIG}" != "x"; then
    CPPFLAGS="$CFLAGS "`freetype-config --cflags`
else if test -d /usr/include/freetype2; then
    CPPFLAGS="$CFLAGS -I/usr/include/freetype2"
else if test -d /usr/local/include/freetype2; then
    CPPFLAGS="$CFLAGS -I/usr/local/include/freetype2"
fi 
fi
fi
AC_CHECK_LIB(freetype, FT_Init_FreeType,HAVE_LIB_FREETYPE=1,)
AC_CHECK_HEADERS(freetype/freetype.h,HAVE_FREETYPE_FREETYPE_H=1)
if test "x${HAVE_LIB_FREETYPE}" != "x";then
if test "x${HAVE_FREETYPE_FREETYPE_H}" != "x";then
    HAVE_FREETYPE=1
fi
fi
if test "x${HAVE_FREETYPE}" != "x"; then 
    if test "x{$FREETYPE_CONFIG}" != "x"; then
	LIBS="$LIBS "`freetype-config --libs`
    else
	LIBS="$LIBS -lfreetype"
    fi
    AC_DEFINE_UNQUOTED(HAVE_FREETYPE, 1)
    AC_DEFINE_UNQUOTED(USE_FREETYPE, 1)  # for ttf2tp1
else
    CPPFLAGS=${OLDCPPFLAGS}
    LIBS=${OLDLIBS}
fi 
]
)

