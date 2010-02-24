AC_DEFUN([RFX_CHECK_RUBY],
[
AC_PATH_PROG([RUBY], [ruby],,)

RUBY_CPPFLAGS=""
RUBY_LDFLAGS=""
RUBY_LIBS=""

rubylib=ruby18

AC_MSG_CHECKING([for ruby's include directory])
if test "x$RUBY" '!=' "x";then
    rubyinc=`$RUBY -e 'require "rbconfig";puts Config::expand(Config::CONFIG.send("\x5b\x5d", "archdir"))' 2>/dev/null`
    if test -d $rubyinc;then
	AC_MSG_RESULT($rubyinc)

	# If we got a sensible archdir, we also ask ruby to supply us with CPPFLAGS
	# and LDFLAGS.
	AC_MSG_CHECKING([for ruby's cpp flags])
	rubycpp=`$RUBY -e 'require "rbconfig";puts Config::expand(Config::CONFIG.send("\x5b\x5d", "CPPFLAGS"))' 2>/dev/null`
	AC_MSG_RESULT($rubycpp)
	
	AC_MSG_CHECKING([for ruby's ld flags])
	rubyld=`$RUBY -e 'require "rbconfig";puts Config::expand(Config::CONFIG.send("\x5b\x5d", "LIBRUBYARG_SHARED"))' 2>/dev/null`
	rubyld2=`$RUBY -e 'require "rbconfig";v=Config::expand(Config::CONFIG.send("\x5b\x5d", "LIBRUBY_LDSHARED"));puts v.gsub(/^g?cc/,"")' 2>/dev/null`
	AC_MSG_RESULT("$rubyld $rubyld2")
	
	AC_MSG_CHECKING([for ruby library name])
	rubylib=`$RUBY -e 'require "rbconfig";puts Config::expand(Config::CONFIG.send("\x5b\x5d", "RUBY_SO_NAME"))' 2>/dev/null`
	AC_MSG_RESULT($rubylib)
	
	RUBY_CPPFLAGS="$rubycpp -I$rubyinc $RUBY_CPPFLAGS"
	RUBY_LDFLAGS="$rubyld $rubyld2 $RUBY_LDFLAGS"
	RUBY_INSTALLDIR="$rubyinc"
    else
	AC_MSG_RESULT('unknown')
    fi
else
    AC_MSG_RESULT('unknown')
fi

#OLDLDFLAGS="$LDFLAGS"
#LDFLAGS="$LDFLAGS $RUBY_LDFLAGS"
#AC_CHECK_LIB($rubylib, rb_define_method, HAVE_RUBY_LIB=1,)
#LDFLAGS="$OLDLDFLAGS"

OLDCPPFLAGS="$CPPFLAGS"
CPPFLAGS="$CPPFLAGS $RUBY_CPPFLAGS"
AC_CHECK_HEADERS([ruby.h],[HAVE_RUBY_H=1])
CPPFLAGS="$OLDCPPFLAGS"

AC_MSG_CHECKING([whether we should compile the ruby module])
if test "x${HAVE_RUBY_H}" '!=' "x";then
    #RUBY_LIBS="$RUBY_LIBS -l$rubylib"

    cat > conftest.c << EOF
#include <ruby.h>
static VALUE foobar;
VALUE foobar_set_foo(VALUE module, VALUE _key, VALUE _value)
{
    return Qnil;
}
int Init_foobar()
{
    foobar = rb_define_module("foobar");
    rb_define_module_function(foobar, "set_foo", foobar_set_foo, 2);
}
int main() {return 0;}
EOF
    ac_link='$CC $SHARED $CPPFLAGS $RUBY_CPPFLAGS $CFLAGS conftest.c $LDFLAGS $RUBY_LDFLAGS $LIBS $RUBY_LIBS -o conftest${SLEXT}'
    if { (eval echo ruby.m4: \"$ac_link\") 1>&5; (eval $ac_link) 2>&5; } && test -s conftest${SLEXT}; then
	RUBY_OK=yes
    else
        echo "configure: failed program was:" >&5
        cat conftest.c >&5
    fi
    rm -f conftest*
fi

if test "x${RUBY_OK}" '=' "xyes"; then 
    AC_MSG_RESULT([yes])
    AC_SUBST(RUBY_LIBS)
    AC_SUBST(RUBY_CPPFLAGS)
    AC_SUBST(RUBY_LDFLAGS)
    AC_SUBST(RUBY_INSTALLDIR)
else
    AC_MSG_RESULT([no])
    RUBY_CPPFLAGS=""
    RUBY_LDFLAGS=""
    RUBY_LIBS=""
    RUBY_INSTALLDIR=""
fi
])
