AC_DEFUN(RFX_CHECK_PYTHON,
[
AC_MSG_CHECKING([for Python.h and PIL])
# should we support python versions below 2.2?
if test -f "/usr/lib/python2.2/site-packages/PIL/_imaging.so" -a -f "/usr/include/python2.2/Python.h";then
    PY_VERSION=2.2
fi
if test -f "/usr/lib/python2.3/site-packages/PIL/_imaging.so" -a -f "/usr/include/python2.3/Python.h";then
    PY_VERSION=2.3
fi
if test -f "/usr/lib/python2.4/site-packages/PIL/_imaging.so" -a -f "/usr/include/python2.4/Python.h";then
    PY_VERSION=2.4
fi

if test "x$PY_VERSION" "!=" "x"; then
    AC_MSG_RESULT(python$PY_VERSION)
    if test "x$PYTHON_LIB" = "x";then
	PYTHON_LIB="-lpython$PY_VERSION /usr/lib/python$PY_VERSION/site-packages/PIL/_imaging.so"
    fi
    if test "x$PYTHON_INCLUDES" = "x";then
	PYTHON_INCLUDES="-I/usr/include/python$PY_VERSION"
    fi
    export PYTHON_INCLUDES PYTHON_LIB
    AC_SUBST(PYTHON_LIB)
    AC_SUBST(PYTHON_INCLUDES)
    AC_MSG_CHECKING([whether we can compile the python test program])
    
    cat > conftest.c << EOF
#include <Python.h>
#include <Imaging.h>

int main(int argn, char*argv[])
{
    return Py_Main(argn, argv);
}
bli bla blo
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
else
    AC_MSG_RESULT(nope)
fi
])
