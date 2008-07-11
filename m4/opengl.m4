AC_DEFUN([RFX_CHECK_OPENGL],
[
OLDCPPFLAGS="${CPPFLAGS}"
OLDLIBS="${LIBS}"
	
AC_CHECK_LIB(GL, glBegin)
AC_CHECK_LIB(GLU, gluBeginSurface)
AC_CHECK_LIB(glut, glutInit)
AC_CHECK_HEADERS(GL/gl.h GL/glut.h)

cat > conftest.c << EOF
#include <GL/gl.h>
#include <GL/glu.h>
#include <GL/glut.h>

int main(int argc, char*argv[])
{
    glutInit(&argc, argv);
    glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    glutInitWindowSize(320,200);
    glutInitWindowPosition(0,0);
    glutCreateWindow("main");
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
    glShadeModel(GL_SMOOTH);
    glEnable (GL_LINE_SMOOTH);
    glEnable (GL_POLYGON_SMOOTH);
    glEnable (GL_BLEND);
    glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
    glHint (GL_LINE_SMOOTH_HINT, GL_DONT_CARE);
    glHint (GL_POLYGON_SMOOTH_HINT, GL_DONT_CARE);
}
EOF

AC_MSG_CHECKING([whether we can compile the opengl test program])

ac_link='$CC $CPPFLAGS $CFLAGS conftest.c $LDFLAGS $LIBS -o conftest${ac_exeext}'
if { (eval echo opengl.m4:71: \"$ac_link\") 1>&5; (eval $ac_link) 2>&5; } && test -s conftest${ac_exeext}; then
    AC_MSG_RESULT(yes)
    HAVE_OPENGL=1
    AC_DEFINE([HAVE_OPENGL], [1], [Defined if opengl is available])
else
    echo "configure: failed program was:" >&5
    cat conftest.c >&5
    HAVE_OPENGL=0
    AC_MSG_RESULT(no)
fi
rm -f conftest*

# if the above didn't work out, reset all changes to the compiler variables.
if test "x${HAVE_OPENGL}" "!=" "x1"; then 
    CPPFLAGS=$OLDCPPFLAGS
    LIBS=$OLDLIBS
fi 
])

