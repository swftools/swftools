AC_DEFUN(RFX_CHECK_PYTHON,
[
AC_MSG_CHECKING([for Python.h and PIL])

if test "x$PYTHON_LIB" '!=' "x" -a "x$PYTHON_INCLUDES" '!=' "x";then
    PY_VERSION=unknown
else
    if test "x$PYTHON_LIB" '!=' "x" -o "x$PYTHON_INCLUDES" '!=' "x";then
        echo "Set both PYTHON_LIB and PYTHON_INCLUDES, or none at all"
    fi
    # iterate through version 2.2 to 2.5
    for v in 2 3 4 5; do
        # Linux
        if test -f "/usr/lib/python2.$v/site-packages/PIL/_imaging.so" -a -f "/usr/include/python2.$v/Python.h";then
            PY_VERSION=2.$v
	    PYTHON_LIB="-lpython$PY_VERSION /usr/lib/python$PY_VERSION/site-packages/PIL/_imaging.so"
	    PYTHON_INCLUDES="-I/usr/include/python$PY_VERSION"
        # Mac OS X
        elif test -f "/System/Library/Frameworks/Python.framework/Versions/2.$v/include/python2.$v/Python.h";then
            #TODO: test for /System/Library/Frameworks/Python.framework/Versions/2.3/Python ?
            PY_VERSION=2.$v
            PYTHON_LIB="-framework Python" 
            if test -f "/Library/Python/2.$v/PIL/_imaging.so";then
                PYTHON_LIB2="$PYTHON_LIB /Library/Python/2.$v/PIL/_imaging.so"
            else
                PYTHON_LIB2="$PYTHON_LIB"
            fi
            PYTHON_INCLUDES="-I/System/Library/Frameworks/Python.framework/Versions/2.$v/include/python2.$v/"
	# Mac OS X [Fink]:
        elif test "(" -f "/sw/lib/python2.$v/config/libpython2.$v.dylib" \
	           -o -f "/sw/lib/python2.$v/config/libpython2.$v.a" \
	           -o -f "/sw/lib/python2.$v/config/libpython2.$v.so" \
	          ")" \
	       -a -f "/sw/include/python2.$v/Python.h" \
               -a -f "/sw/lib/python2.$v/site-packages/PIL/_imaging.so";then
            PY_VERSION=2.$v
            PYTHON_LIB="-L /sw/lib/python2.$v/config/ -lpython$PY_VERSION /sw/lib/python2.$v/site-packages/PIL/_imaging.so"
            PYTHON_INCLUDES="-I /sw/include/python2.$v/"
        fi
    done
fi

if test "x$PY_VERSION" "!=" "x"; then
    AC_MSG_RESULT($PY_VERSION)
    export PYTHON_INCLUDES PYTHON_LIB
    AC_SUBST(PYTHON_LIB)
    AC_SUBST(PYTHON_INCLUDES)
    AC_MSG_CHECKING([whether we can compile the python test program])
    
    cat > conftest.c << EOF
#include <Python.h>
#include <Imaging.h>

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
        ac_link='$CC $CPPFLAGS $CFLAGS $PYTHON_INCLUDES conftest.c $LDFLAGS ${PYTHON_LIB2} $LIBS -o conftest${ac_exeext}'
        if { (eval echo python.m4: \"$ac_link\") 1>&5; (eval $ac_link) 2>&5; } && test -s conftest${ac_exeext}; then
            AC_MSG_RESULT(yes)
            PYTHON_LIB="${PYTHON_LIB2}"
            PYTHON_OK=yes
        else
            echo "configure: failed program was:" >&5
            cat conftest.c >&5
            AC_MSG_RESULT(no)
        fi
    fi
    rm -f conftest*
else
    AC_MSG_RESULT(nope)
fi
])
