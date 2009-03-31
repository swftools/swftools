AC_DEFUN([RFX_CHECK_PYTHON],
[
AC_MSG_CHECKING([for Python.h])

if test "x$PYTHON_LIB" '!=' "x" -a "x$PYTHON_INCLUDES" '!=' "x";then
    PY_VERSION=unknown
else
    if test "x$PYTHON_LIB" '!=' "x" -o "x$PYTHON_INCLUDES" '!=' "x";then
        echo "Set both PYTHON_LIB and PYTHON_INCLUDES, or none at all"
    fi
    # iterate through version 2.2 to 2.6
    for v in 2 3 4 5 6; do
        # Linux
        if test -f "/usr/include/python2.$v/Python.h";then
            PY_VERSION=2.$v
	    PYTHON_LIB="-lpython$PY_VERSION"
            if test -f "/usr/lib/python2.$v/site-packages/PIL/_imaging.so";then 
                PYTHON_LIB2="$PYTHON_LIB /usr/lib/python2.$v/site-packages/PIL/_imaging.so"
                HAVE_PYTHON_IMAGING_LIB=1
            else
                PYTHON_LIB2="$PYTHON_LIB"
            fi
	    PYTHON_INCLUDES="-I/usr/include/python$PY_VERSION"
        # Mac OS X
        elif test -f "/Library/Frameworks/Python.framework/Versions/2.$v/include/python2.$v/Python.h";then
            PY_VERSION=2.$v
            PYTHON_LIB="-framework Python" 
            if test -f "/Library/Frameworks/Python.framework/Versions/2.$v/site-packages/PIL/_imaging.so";then
                PYTHON_LIB2="$PYTHON_LIB /Library/Python/2.$v/PIL/_imaging.so"
                HAVE_PYTHON_IMAGING_LIB=1
            else
                PYTHON_LIB2="$PYTHON_LIB"
            fi
            PYTHON_INCLUDES="-I/Library/Frameworks/Python.framework/Versions/2.$v/include/python2.$v/"
	# Mac OS X [Fink]:
        elif test "(" -f "/sw/lib/python2.$v/config/libpython2.$v.dylib" \
	           -o -f "/sw/lib/python2.$v/config/libpython2.$v.a" \
	           -o -f "/sw/lib/python2.$v/config/libpython2.$v.so" \
	          ")" \
	       -a -f "/sw/include/python2.$v/Python.h"; then
            PY_VERSION=2.$v
            PYTHON_LIB="-L /sw/lib/python2.$v/config/ -lpython$PY_VERSION /sw/lib/python2.$v/site-packages/PIL/_imaging.so"
            if test -f "/sw/lib/python2.$v/site-packages/PIL/_imaging.so";then
                PYTHON_LIB2="$PYTHON_LIB /sw/lib/python2.$v/site-packages/PIL/_imaging.so"
                HAVE_PYTHON_IMAGING_LIB=1
            else
                PYTHON_LIB2="$PYTHON_LIB"
            fi
            PYTHON_INCLUDES="-I /sw/include/python2.$v/"
        fi
    done
fi

if test "x$PY_VERSION" "!=" "x"; then
    AC_MSG_RESULT([$PY_VERSION])
    export PYTHON_INCLUDES PYTHON_LIB
    AC_SUBST(PYTHON_LIB)
    AC_SUBST(PYTHON_INCLUDES)
    AC_MSG_CHECKING([whether we can compile the python test program])
    
    cat > conftest.c << EOF
#include <Python.h>

int main()
{
    int ret;
    ret = Py_Main(0, 0);
    int x; // check also for gcc 2.95.x incompatibilities
    return ret;
}
EOF
    
    ac_link='$CC $CPPFLAGS $CFLAGS $PYTHON_INCLUDES conftest.c $LDFLAGS $PYTHON_LIB $LIBS -o conftest${ac_exeext}'
    if { (eval echo python.m4: \"$ac_link\") 1>&5; (eval $ac_link) 2>&5; } && test -s conftest${ac_exeext}; then
	AC_MSG_RESULT(yes)
	PYTHON_OK=yes
    else
        echo "configure: failed program was:" >&5
        cat conftest.c >&5
        AC_MSG_RESULT(no)
    fi
    rm -f conftest*
    if test "x$PYTHON_OK" = "xyes";then
        AC_MSG_CHECKING([for Python-Imaging])
    cat > conftest.c << EOF
#include <Python.h>
#include <Imaging.h>

int main()
{
    Py_Main(0, 0);
    return 0;
}
EOF
        if test "$HAVE_PYTHON_IMAGING_LIB"; then
            ac_link='$CC $CPPFLAGS $CFLAGS $PYTHON_INCLUDES conftest.c $LDFLAGS ${PYTHON_LIB2} $LIBS -o conftest${ac_exeext}'
            if { (eval echo python.m4: \"$ac_link\") 1>&5; (eval $ac_link) 2>&5; } && test -s conftest${ac_exeext}; then
                PYTHON_LIB="${PYTHON_LIB2}"
                AC_DEFINE([HAVE_PYTHON_IMAGING], [1], [whether python-imaging was found])
                HAVE_PYTHON_IMAGING=yes
                export HAVE_PYTHON_IMAGING
                AC_SUBST(HAVE_PYTHON_IMAGING)
                AC_MSG_RESULT(yes)
            else
                echo "configure: failed program was:" >&5
                cat conftest.c >&5
                AC_MSG_RESULT(no)
            fi
        else
            echo "(didn't find the Python-Imaging libraries)" >&5
            AC_MSG_RESULT(no)
        fi
    fi
    rm -f conftest*
else
    AC_MSG_RESULT([nope])
fi
])
