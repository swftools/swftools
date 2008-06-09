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

AC_DEFUN([RFX_CHECK_FREETYPE],
[

AC_PATH_PROG([FREETYPE_CONFIG], [freetype-config],, ["$PATH:/usr/local/bin:/sw/bin:/opt/local/bin"])

OLDCPPFLAGS="${CPPFLAGS}"
OLDLIBS="${LIBS}"
if test "x${FREETYPE_CONFIG}" '!=' "x"; then
    CPPFLAGS="$CPPFLAGS "`$FREETYPE_CONFIG --cflags`
else if test -d /usr/include/freetype2; then
    CPPFLAGS="$CPPFLAGS -I/usr/include/freetype2"
else if test -d /usr/local/include/freetype2; then
    CPPFLAGS="$CPPFLAGS -I/usr/local/include/freetype2"
fi 
fi
fi

if test "x${FREETYPE_CONFIG}" '=' "x";then
    # if we didn't find the freetype-config program, we won't
    # know where the libs are expected to be. So just blindly
    # try to link against them.
    AC_CHECK_LIB([freetype], [FT_Init_FreeType],[HAVE_LIB_FREETYPE=1],)
fi

AC_CHECK_HEADERS([ft2build.h],[HAVE_FT2BUILD_H=1])
if test "x${HAVE_FT2BUILD_H}" '=' "x";then
    AC_CHECK_HEADERS([freetype/freetype.h],[HAVE_FREETYPE_FREETYPE_H=1])
fi

if test "x${HAVE_LIB_FREETYPE}" '!=' "x" -o \
        "x${FREETYPE_CONFIG}" '!=' "x";then
    if test "x${HAVE_FREETYPE_FREETYPE_H}" '!=' "x";then
	HAVE_FREETYPE=1
    fi
    if test "x${HAVE_FT2BUILD_H}" '!=' "x";then
	HAVE_FREETYPE=1
    fi
fi

if test "x${HAVE_FREETYPE}" = "x1"; then 
    if test "x${FREETYPE_CONFIG}" '!=' "x"; then
	LIBS="$LIBS "`$FREETYPE_CONFIG --libs`
    else
	LIBS="$LIBS -lfreetype"
    fi

    if test "x${HAVE_FT2BUILD_H}" = "x1"; then
	HAVE_FT2BUILD_H_DEFINE='#define HAVE_FT2BUILD_H'
    fi

    AC_MSG_CHECKING([whether we can compile the freetype test program])

    cat > conftest.c << EOF
$HAVE_FT2BUILD_H_DEFINE

#ifdef HAVE_FT2BUILD_H
#include <ft2build.h>
#include FT_FREETYPE_H
#include FT_GLYPH_H
#include FT_SIZES_H
#include FT_SFNT_NAMES_H
#include FT_TRUETYPE_IDS_H
#include FT_OUTLINE_H
#else
#include <freetype/freetype.h>
#include <freetype/ftglyph.h>
#include <freetype/ftsizes.h>
#include <freetype/ftsnames.h>
#include <freetype/ttnameid.h>
#include <freetype/ftoutln.h>
#endif

int main()
{
    FT_Library ftlibrary;
    FT_Face face;
    FT_Error error;
    FT_ULong charcode;
    FT_UInt gindex;
    FT_Glyph glyph;
    FT_BBox bbox;
    FT_Matrix matrix;
    FT_UInt i;
    char* name = 0;
    char italic, bold;
   
    FT_Init_FreeType(&ftlibrary);
    FT_New_Face(ftlibrary, "filename", 0, &face);
    i = face->num_glyphs - 1;
    italic = face->style_flags&FT_STYLE_FLAG_ITALIC;
    bold = face->style_flags&FT_STYLE_FLAG_BOLD;
    FT_Get_Postscript_Name(face);
    FT_Get_Char_Index(face, 33);
    FT_Get_First_Char(face, &i);
    FT_Get_Next_Char(face, 33, &i);
    if(FT_HAS_GLYPH_NAMES(face)) {
	FT_Get_Glyph_Name(face, 33, name, 127);
    }
    FT_Load_Glyph(face, 33, FT_LOAD_NO_BITMAP|FT_LOAD_NO_SCALE);
    FT_Get_Glyph(face->glyph, &glyph);
    FT_Glyph_Get_CBox(glyph, ft_glyph_bbox_unscaled, &bbox);
    FT_Done_Glyph(glyph);
    FT_Done_Face(face);
    FT_Done_FreeType(ftlibrary);
    return 0;
}
EOF

    ac_link='$CC $CPPFLAGS $CFLAGS conftest.c $LDFLAGS $LIBS -o conftest${ac_exeext}'
    if { (eval echo freetype.m4:71: \"$ac_link\") 1>&5; (eval $ac_link) 2>&5; } && test -s conftest${ac_exeext}; then
	AC_MSG_RESULT([yes])
	AC_DEFINE([HAVE_FREETYPE], [1], [have/use freetype library])
	AC_DEFINE([HAVE_FREETYPE_FREETYPE_H], [1], [Define if freetype headers are available])
	AC_DEFINE([USE_FREETYPE], [1], [Define if freetype is available])  # for ttf2tp1
    else
	echo "configure: failed program was:" >&5
	cat conftest.c >&5
	HAVE_FREETYPE=0
	AC_MSG_RESULT([no])
    fi
    rm -f conftest*
fi

# if the above didn't work out, reset all changes to the compiler variables.
if test "x${HAVE_FREETYPE}" '!=' "x1"; then 
    CPPFLAGS=$OLDCPPFLAGS
    LIBS=$OLDLIBS
fi 
])

