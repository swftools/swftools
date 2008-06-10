# ----------- freetype.m4 ----------------
#
# Checks for:
#    * freetype library (-lfreetype)
#    * freetype headers (freetype2/freetype/freetype.h)
#    * whether a freetype test program can be compiled
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

AC_DEFUN([RFX_CHECK_FONTCONFIG],
[
OLDCPPFLAGS="${CPPFLAGS}"
OLDLIBS="${LIBS}"
	
if test -d /usr/include/fontconfig; then
    CPPFLAGS="$CPPFLAGS -I/usr/include/fontconfig"
fi

AC_CHECK_LIB(fontconfig, FcInit, [HAVE_LIB_FONTCONFIG=1],)
AC_CHECK_HEADERS(fontconfig.h,[HAVE_FONTCONFIG_H=1])

if test "x${HAVE_LIB_FONTCONFIG}" != "x";then
if test "x${HAVE_FONTCONFIG_H}" != "x";then
    HAVE_FONTCONFIG=1
fi
fi

if test "x${HAVE_FONTCONFIG}" = "x1"; then 
    LIBS="$LIBS -lfontconfig"
AC_MSG_CHECKING([whether we can compile the fontconfig test program])

if (echo $LIBS | grep lfreetype >/dev/null 2>&1); then
    #move freetype library to the end of libraries, otherwise fontconfig
    #won't compile.
    LIBS=`echo $LIBS | sed -e 's/-lfreetype//g' -e 's/$/ -lfreetype/'`
fi

cat > conftest.c << EOF
#include <string.h>
#include <fontconfig.h>

int main()
{
    FcPattern *pattern, *match;
    FcResult result;
    FcChar8 *v;
    char*s1="abc",*s2="ABC";
    strcasecmp(s1,s2);
    FcInit();
    pattern = FcPatternBuild(0, FC_FAMILY, FcTypeString, "", 0);
    FcPatternAddInteger(pattern, FC_SLANT, FC_SLANT_ITALIC);
    FcPatternAddInteger(pattern, FC_WEIGHT, FC_WEIGHT_BOLD);

    FcConfig*c = FcConfigCreate();
    FcConfigParseAndLoad(c, (FcChar8*)"", 1);
    FcConfigBuildFonts(c);
    FcConfigSetCurrent(c);
	
    FcFontSet * set =  FcConfigGetFonts(c, FcSetSystem);
    FcFontSetDestroy(set);

    FcConfigSubstitute(0, pattern, FcMatchPattern); 
    FcDefaultSubstitute(pattern);
    match = FcFontMatch(0, pattern, &result);
    FcPatternGetString(match, "family", 0, &v) == FcResultMatch;
    FcPatternGetBool(match, "family", 0, &v) == FcResultMatch;
    FcPatternPrint(pattern);
    FcPatternDestroy(pattern);
    FcPatternDestroy(match);
    FcFini();
    return 0;
}
EOF

    ac_link='$CC $CPPFLAGS $CFLAGS conftest.c $LDFLAGS $LIBS -o conftest${ac_exeext}'
    if { (eval echo freetype.m4:71: \"$ac_link\") 1>&5; (eval $ac_link) 2>&5; } && test -s conftest${ac_exeext}; then
	AC_MSG_RESULT(yes)
	AC_DEFINE([HAVE_FONTCONFIG], [1], [Define if fontconfig is available])
    else
	echo "configure: failed program was:" >&5
	cat conftest.c >&5
	HAVE_FONTCONFIG=0
	AC_MSG_RESULT(no)
    fi
    rm -f conftest*
fi

# if the above didn't work out, reset all changes to the compiler variables.
if test "x${HAVE_FONTCONFIG}" "!=" "x1"; then 
    CPPFLAGS=$OLDCPPFLAGS
    LIBS=$OLDLIBS
fi 
])

