AC_DEFUN([RFX_CHECK_FFTW],
[
  OLDCPPFLAGS="${CPPFLAGS}"
  OLDLIBS="${LIBS}"
  	
  AC_CHECK_LIB(fftw3f, fftwf_plan_dft_r2c_2d, [HAVE_LIBFFTW3=1],)
  AC_CHECK_HEADERS(fftw3.h,[HAVE_FFTW3_H=1])
  
  if test "x${HAVE_LIBFFTW3}" != "x";then
    if test "x${HAVE_FFTW3_H}" != "x";then
      HAVE_FFTW3=1
    fi
  fi
  
  if test "x${HAVE_FFTW3}" = "x1"; then 
    LIBS="$LIBS -lfftw3f"
    AC_MSG_CHECKING([whether we can compile the fftw3 test program])
  
    cat > conftest.c << EOF
    #include <fftw3.h>
    
    int main()
    {
	char*data = fftw_malloc(sizeof(fftwf_complex)*600*800);
    	fftwf_plan plan = fftwf_plan_dft_2d(600, 800, (fftwf_complex*)data, (fftwf_complex*)data, FFTW_FORWARD, FFTW_ESTIMATE);
	plan = fftwf_plan_dft_r2c_2d(600, 800, (float*)data, (fftwf_complex*)data, FFTW_ESTIMATE);
	plan = fftwf_plan_dft_c2r_2d(600, 800, (fftwf_complex*)data, (float*)data, FFTW_ESTIMATE);
    	fftwf_execute(plan);
    	fftwf_destroy_plan(plan);
    }
EOF
    
    ac_link='$CC $CPPFLAGS $CFLAGS conftest.c $LDFLAGS $LIBS -o conftest${ac_exeext}'
    if { (eval echo fftw3.m4:71: \"$ac_link\") 1>&5; (eval $ac_link) 2>&5; } && test -s conftest${ac_exeext}; then
      AC_MSG_RESULT(yes)
      AC_DEFINE([HAVE_FFTW3], [1], [Define if fftw3 is available])
    else
      echo "configure: failed program was:" >&5
      cat conftest.c >&5
      HAVE_FFTW3=0
      AC_MSG_RESULT(no)
    fi
    rm -f conftest*
  fi
  
  # if the above didn't work out, reset all changes to the compiler variables.
  if test "x${HAVE_FFTW3}" "!=" "x1"; then 
      CPPFLAGS=$OLDCPPFLAGS
      LIBS=$OLDLIBS
  fi 
])

