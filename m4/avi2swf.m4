AC_DEFUN(RFX_CHECK_AVI2SWF,
[
AC_MSG_CHECKING([whether we can compile the avifile test program])

cat > conftest.$ac_ext << EOF
#include <avifile/version.h>
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

ac_link='$CXX $CPPFLAGS $CXXFLAGS `avifile-config --cflags` `avifile-config --libs` conftest.$ac_ext -o conftest${ac_exeext}'
##if {(eval $ac_link) 2>&5; } && test -s conftest${ac_exeext}; then
if { (eval echo avi2swf.m4:71: \"$ac_link\") 1>&5; (eval $ac_link) 2>&5; } && test -s conftest${ac_exeext}; then
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


