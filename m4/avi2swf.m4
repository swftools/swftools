AC_DEFUN(RFX_CHECK_AVI2SWF,
[

AC_CHECK_PROGS(AVIFILE_CONFIG, avifile-config)

if test "x$AVIFILE_CONFIG" '!=' "x";then
    OLDCPPFLAGS="$CPPFLAGS"
    #OLDLDFLAGS="$LDFLAGS"
    CPPFLAGS="$CPPFLAGS $CXXFLAGS "`$AVIFILE_CONFIG --cflags`
    #LDFLAGS="$LDFLAGS `$AVIFILE_CONFIG --libs`"
    AC_CHECK_HEADERS(avifile/version.h version.h)
    #LDFLAGS="$OLDLDFLAGS"
    CPPFLAGS="$OLDCPPFLAGS"
fi

AC_MSG_CHECKING([whether we can compile the avifile test program])

if test "x${ac_cv_header_version_h}" '=' "xyes";then
    HAVE_VERSION_H_DEFINE='#define HAVE_VERSION_H'
fi
if test "x${ac_cv_header_avifile_version_h}" '=' "xyes";then
    HAVE_AVIFILE_VERSION_H_DEFINE='#define HAVE_AVIFILE_VERSION_H'
fi

cat > conftest.cpp << EOF

// hack- we can't yet access the variables in config.h (because it hasn't been generated yet),
// so we have to introduce them this way
$HAVE_VERSION_H_DEFINE
$HAVE_AVIFILE_VERSION_H_DEFINE

#ifdef HAVE_VERSION_H
  #include <version.h>
#endif
#ifdef HAVE_AVIFILE_VERSION_H
  #include <avifile/version.h>
#endif

#if (AVIFILE_MAJOR_VERSION == 0) && (AVIFILE_MINOR_VERSION>=6) 
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

void test()
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
int main (int argc, char*argv[])
{
    return 0;
}
EOF

if test "x$AVIFILE_CONFIG" '!=' "x";then
    ac_link='$CXX $CPPFLAGS $CXXFLAGS `$AVIFILE_CONFIG --cflags` conftest.cpp `$AVIFILE_CONFIG --libs` -o conftest${ac_exeext}'
    if { (eval echo avi2swf.m4:71: \"$ac_link\") 1>&5; (eval $ac_link) 2>&5; } && test -s conftest${ac_exeext} && ./conftest${ac_exeext}; then
      AC_MSG_RESULT(yes)
      AVIFILE=true
      export AVIFILE
      AC_DEFINE_UNQUOTED(AVIFILE, true)
    else
      echo "configure: failed program was:" >&5
      cat conftest.cpp >&5
      AC_MSG_RESULT(no)
    fi
else
    AC_MSG_RESULT(no)
fi
rm -f conftest*
])


