AC_DEFUN([RFX_CHECK_LOWERCASE_UPPERCASE],
[
AC_MSG_CHECKING([whether the filesystem normalizes file names])

echo test1 > __abcdefghijklmnopqrstuvwxyz.txt
echo test2 > __ABCDEFGHIJKLMNOPQRSTUVWXYZ.txt
if test `cat __abcdefghijklmnopqrstuvwxyz.txt` = "test2";then
    AC_MSG_RESULT(yes)
    AC_DEFINE([LOWERCASE_UPPERCASE], [1], [Define if the file system is case invariant])
else
    AC_MSG_RESULT(no)
fi

rm -f __abcdefghijklmnopqrstuvwxyz.txt
rm -f __ABCDEFGHIJKLMNOPQRSTUVWXYZ.txt
])

