AC_DEFUN(SWF_CHECK_BYTEORDER,
[
AC_MSG_CHECKING([for byte order])
AC_TRY_RUN([
int main (int argc, char *argv[])
{
    int i = 1;
    return *(char*)&i;
}
], BIGENDIAN=1, LITTLEENDIAN=1, CROSSCOMPILE=1)
export CROSSCOMPILE 

if test "x${CROSSCOMPILE}" = "x1"; then
 AC_MSG_RESULT(we are cross compiling- trying to guess from system type)
 AC_CANONICAL_SYSTEM
 AC_MSG_CHECKING([for byte order, try 2])
 case "${target}" in
  *86* | *-pc-* )
     LITTLEENDIAN=1
     ;;
  *sparc* | *68* | *88k* | *mac* | *Mac* | *sun* | *Sun* | *Amiga* | *amiga* )
     BIGENDIAN=1
     ;;
 esac
fi

export LITTLEENDIAN
if test "x${LITTLEENDIAN}" = "x1"; then
AC_MSG_RESULT(little endian)
BYTEORDERCHECKOK=1
fi
export BIGENDIAN
if test "x${BIGENDIAN}" = "x1"; then
AC_MSG_RESULT(big endian)
BYTEORDERCHECKOK=1
WORDS_BIGENDIAN=1
export WORDS_BIGENDIAN
AC_DEFINE_UNQUOTED(WORDS_BIGENDIAN, 1)
fi
if test "x${BYTEORDERCHECKOK}" != "x1"; then
AC_MSG_RESULT(unknown)
echo Byte order could not determined. Try to insert your system type into aclocal.m4 about line 21
exit 1
fi
])

AC_DEFUN(SWF_CHECK_SYSTEM_BACKTICKS,
[
AC_MSG_CHECKING([whether system() can handle command substitution])
AC_TRY_RUN([
#include "stdlib.h"
int main (int argc, char*argv[])
{
    return system("test `touch config.tmp2`");
}
], OK=OK,,CROSSCOMPILE=1)

if test "x${OK}" = "xOK";then
    test -f config.tmp2 || OK=
fi

if test "x${CROSSCOMPILE}" = "x1";then
    OK=OK;
fi
if test "x${OK}" = "xOK";then
rm -f config.tmp2
AC_MSG_RESULT(yes)
SYSTEM_BACKTICKS=1
export SYSTEM_BACKTICKS
AC_DEFINE_UNQUOTED(SYSTEM_BACKTICKS, 1)
else
AC_MSG_RESULT(no)
fi
])

AC_DEFUN(SWF_CHECK_AVI2SWF,
[
AC_MSG_CHECKING([whether we can compile the avifile test program])

cat > conftest.$ac_ext << EOF
#include <avifile/version.h>
#if (AVIFILE_MAJOR_VERSION == 0) && (AVIFILE_MINOR_VERSION==6) 
   #include <avifile.h>
   #include <aviplay.h>
   #include <fourcc.h>
   #include <creators.h>
   #include <StreamInfo.h>
   #define VERSION6
#else
   #include <avifile.h>
   #include <aviplay.h>
   #include <aviutil.h>
   #define Width width
   #define Height height
   #define Data data
   #define Bpp bpp
#endif

int main (int argc, char*argv[])
{
  IAviReadFile* player;
  IAviReadStream* astream;
  IAviReadStream* vstream;
  player = CreateIAviReadFile(0);
  astream = player->GetStream(0, AviStream::Audio);
  vstream = player->GetStream(0, AviStream::Video);
  vstream -> StartStreaming();
  astream -> StartStreaming();
#ifndef VERSION6
  MainAVIHeader head;
  player->GetFileHeader(&head);
  head.dwMicroSecPerFrame;
  head.dwTotalFrames;
  head.dwStreams;
  head.dwWidth;
  head.dwHeight;
  head.dwMicroSecPerFrame;
  astream->GetEndPos(),
  astream->GetEndTime());
  player->StreamCount();
#else
  StreamInfo*audioinfo;
  StreamInfo*videoinfo;
  audioinfo = astream->GetStreamInfo();
  videoinfo = vstream->GetStreamInfo();
  videoinfo->GetVideoWidth();
  videoinfo->GetVideoHeight();
  audioinfo->GetAudioSamplesPerSec();
  videoinfo->GetFps();
#endif
  CImage*img = vstream->GetFrame();
  img->ToRGB();
  img->Data();
  img->Bpp();
  img->Width();
  img->Height();
  WAVEFORMATEX wave;
  astream->GetAudioFormatInfo(&wave,0);
  uint_t a,b;
  astream->ReadFrames(0,0,0,a,b);
}
EOF

ac_link='g++ `avifile-config --cflags` `avifile-config --libs` conftest.$ac_ext -o conftest${ac_exeext}'
if {(eval $ac_link) 2>&5; } && test -s conftest${ac_exeext}; then
  AC_MSG_RESULT(yes)
  AVIFILE=true
  export AVIFILE
  AC_DEFINE_UNQUOTED(AVIFILE, true)
else
  echo "configure: failed program was:" >&5
  cat conftest.$ac_ext >&5
  AC_MSG_RESULT(no)
fi
rm -f conftest*
])


