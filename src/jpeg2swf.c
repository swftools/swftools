/* jpeg2swf.c

   JPEG to SWF converter tool

   Part of the swftools package.

   Copyright (c) 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

#include <stdio.h>
#include <math.h>
#include <fcntl.h>
#include <jpeglib.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h" // not really a header ;-)

#define MAX_INPUT_FILES 1024
#define VERBOSE(x) (global.verbose>=x)

struct
{ int quality;
  int framerate;
  int max_image_width;
  int max_image_height;
  int force_width;
  int force_height;
  int nfiles;
  int verbose;
  char * files[MAX_INPUT_FILES];
  char * outfile;
} global;

TAG * MovieStart(SWF * swf,int framerate,int dx,int dy)
{ TAG * t;
  RGBA rgb;

  memset(swf,0x00,sizeof(SWF));

  swf->FileVersion       = 4;
  swf->FrameRate         = (25600/framerate);
  swf->MovieSize.xmax    = dx*20;
  swf->MovieSize.ymax    = dy*20;

  t = swf->FirstTag = InsertTag(NULL,ST_SETBACKGROUNDCOLOR);

  rgb.r = rgb.g = rgb.b = rgb.a  = 0x00;
  SetRGB(t,&rgb);

  return t;
}

int MovieFinish(SWF * swf,TAG * t,char * sname)
{  int handle, so = fileno(stdout);
   t = InsertTag(t,ST_END);

   if ((!isatty(so))&&(!sname)) handle = so;
   else
   { if (!sname) sname = "out.swf";
     handle = open(sname,O_RDWR|O_CREAT|O_TRUNC,0666);
   }
   if FAILED(WriteSWF(handle,swf)) if (VERBOSE(1)) fprintf(stderr,"Unable to write output file: %s\n",sname);
   if (handle!=so) close(handle);
   
   FreeTags(swf);
   return 0;
}

TAG * MovieAddFrame(SWF * swf,TAG * t,char * sname,int quality,int id)
{ SHAPE * s;
  SRECT r;
  MATRIX m;
  int fs;
  
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  LPJPEGBITS out;
  FILE * f;
  U8 * scanline;

  if ((f=fopen(sname,"rb"))==NULL)
  { if (VERBOSE(1)) fprintf(stderr,"Read access failed: %s\n",sname);
    return t;
  }
  
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo); 
  jpeg_stdio_src(&cinfo,f);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  t = InsertTag(t,ST_DEFINEBITSJPEG2);

        SetU16(t,id);  // id
  
        out = SetJPEGBitsStart(t,cinfo.output_width,cinfo.output_height,quality);
        scanline = (U8*)malloc(4*cinfo.output_width);
  
        if (scanline)
        { int y;
          U8 * js = scanline;
          for (y=0;y<cinfo.output_height;y++)
          { jpeg_read_scanlines(&cinfo,&js,1);
            SetJPEGBitsLines(out,(U8**)&js,1);
          }
          free(scanline);
        }
        
        SetJPEGBitsFinish(out);
    
  t = InsertTag(t,ST_DEFINESHAPE);

        NewShape(&s);
        GetMatrix(NULL,&m);
        m.sx = 20*0x10000;
        m.sy = 20*0x10000;
        fs = ShapeAddBitmapFillStyle(s,&m,id,0);
        
        SetU16(t,id+1); // id


        r.xmin = r.ymin = 0;
        r.xmax = cinfo.output_width*20;
        r.ymax = cinfo.output_height*20;
        SetRect(t,&r);
        
        SetShapeHeader(t,s);

        ShapeSetAll(t,s,0,0,0,fs,0);
        ShapeSetLine(t,s,r.xmax,0);
        ShapeSetLine(t,s,0,r.ymax);
        ShapeSetLine(t,s,-r.xmax,0);
        ShapeSetLine(t,s,0,-r.ymax);
        
        ShapeSetEnd(t);
        
  t = InsertTag(t,ST_REMOVEOBJECT2);
        SetU16(t,1); // depth

  t = InsertTag(t,ST_PLACEOBJECT2);

        GetMatrix(NULL,&m);
        m.tx = (swf->MovieSize.xmax-(int)cinfo.output_width*20)/2;
        m.ty = (swf->MovieSize.ymax-(int)cinfo.output_height*20)/2;
        ObjectPlace(t,id+1,1,&m,NULL,NULL);

  t = InsertTag(t,ST_SHOWFRAME);

  jpeg_finish_decompress(&cinfo);
  fclose(f);

  return t;
}

int CheckInputFile(char * fname,char ** realname)
{ struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * f;
  char * s = malloc(strlen(fname)+5);
  
  if (!s) exit(2);
  (*realname) = s;
  strcpy(s,fname);

  // Check whether file exists (with typical extensions)

  if ((f=fopen(s,"rb"))==NULL)
  { sprintf(s,"%s.jpg",fname);
    if ((f=fopen(s,"rb"))==NULL)
    { sprintf(s,"%s.jpeg",fname);
      if ((f=fopen(s,"rb"))==NULL)
      { sprintf(s,"%s.JPG",fname);
        if ((f=fopen(s,"rb"))==NULL)
        { sprintf(s,"%s.JPEG",fname);
          if ((f=fopen(s,"rb"))==NULL)
            return 0;
        }
      }
    }
  }
  
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo); 
  jpeg_stdio_src(&cinfo,f);
  jpeg_read_header(&cinfo, TRUE);

  // Get image dimensions

  if (global.max_image_width<cinfo.image_width) global.max_image_width = cinfo.image_width;
  if (global.max_image_height<cinfo.image_height) global.max_image_height = cinfo.image_height;
  
  jpeg_destroy_decompress(&cinfo);
  fclose(f);

  return 0;
}

int args_callback_option(char*arg,char*val)
{ int res = 0;
  if (arg[1]) res = -1;
  else switch (arg[0])
  { case 'q':
      if (val) global.quality = atoi(val);
      if ((global.quality<1)||(global.quality>100))
      { if (VERBOSE(1)) fprintf(stderr,"Error: You must specify a valid quality between 1 and 100.\n");
        exit(1);
      }
      res = 1;
      break;

    case 'r':
      if (val) global.framerate = atoi(val);
      if ((global.framerate<1)||(global.framerate>5000))
      { if (VERBOSE(1)) fprintf(stderr,"Error: You must specify a valid framerate between 1 and 10000.\n");
        exit(1);
      }
      res = 1;
      break;

    case 'o':
      if (val) global.outfile = val; res = 1; break;

    case 'v':
      if (val) global.verbose = atoi(val); res = 1; break;

    case 'X':
      if (val) global.force_width = atoi(val); res = 1; break;

    case 'Y':
      if (val) global.force_height = atoi(val); res = 1; break;

    case 'V':
      printf("jpeg2swf - part of %s %s\n", PACKAGE, VERSION);exit(0);
      
    default:
      res = -1;
      break;
  }
  
  if (res<0)
  { if (VERBOSE(1)) fprintf(stderr,"Unknown option: -v%s\n",arg);
    return 0;
  }
  return res;
}

struct options_t options[] =
{{"q","quality"},
 {"o","output"},
 {"r","rate"},
 {"v","verbose"},
 {"X","width"},
 {"Y","height"},
 {"V","version"}
};

int args_callback_longoption(char*name,char*val) {
    return args_long2shortoption(options, name, val);
}

int args_callback_command(char*arg,char*next)  // actually used as filename
{ char * s;
  if (CheckInputFile(arg,&s)<0)
  { if (VERBOSE(1)) fprintf(stderr, "Unable to open input file: %s\n",arg);
    free(s);
  }
  else
  { global.files[global.nfiles] = s;
    global.nfiles++;
    if (global.nfiles>=MAX_INPUT_FILES)
    { if (VERBOSE(1)) fprintf(stderr, "Error: Too many input files.\n");
      exit(1);
    }
  }
  return 0;
}

void args_callback_usage(char*name)
{ fprintf(stderr,"Usage: %s imagefiles[.jpg]|[.jpeg] [...] [-options [value]]\n",name);
  fprintf(stderr,"-q quality            (quality) Set JPEG compression quality (1-100)\n");
  fprintf(stderr,"-r framerate          (rate) Set movie framerate (100/sec)\n");
  fprintf(stderr,"-o outputfile         (output) Set name for SWF output file\n");
  fprintf(stderr,"-v level              (verbose) Set verbose level (0=quiet, 1=default, 2=debug)\n");
  fprintf(stderr,"-X pixel              (width) Force movie width to scale (default: autodetect)\n");
  fprintf(stderr,"-Y pixel              (height) Force movie height to scale (default: autodetect)\n");
  fprintf(stderr,"-V                    (version) Print version information and exit\n");
}


int main(int argc, char ** argv)
{ SWF swf;
  TAG * t;

  memset(&global,0x00,sizeof(global));
    
  global.quality                = 60;
  global.framerate              = 100;
  global.verbose                = 1;
  
  processargs(argc, argv);

  if (VERBOSE(2)) fprintf(stderr,"Processing %i file(s)...\n",global.nfiles);

  t = MovieStart(&swf,global.framerate,
                      global.force_width?global.force_width:global.max_image_width,
                      global.force_height?global.force_height:global.max_image_height);

  { int i;
    for (i=0;i<global.nfiles;i++)
    { if (VERBOSE(3)) fprintf(stderr,"[%03i] %s\n",i,global.files[i]);
      t = MovieAddFrame(&swf,t,global.files[i],global.quality,(i*2)+1);
      free(global.files[i]);
    }
  }

  MovieFinish(&swf,t,global.outfile);

  return 0;
}


// Old main routine

/*
int ConvertJPEG2SWF(char * sname,char * dname,int quality)
{ RGBA rgb;
  SWF swf;
  TAG * t;
  
  SHAPE * s;
  SRECT r;
  MATRIX m;
  int fs;
  
  struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  LPJPEGBITS out;
  FILE * f;
  U8 * scanline;

  int handle;

  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo); 

  if ((f=fopen(sname,"rb"))==NULL)
  { fprintf(stderr,"Read access failed: %s\n",sname);
    return -1;
  }
  
  jpeg_stdio_src(&cinfo,f);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);
  
  memset(&swf,0x00,sizeof(SWF));

  swf.FileVersion       = 4;
  swf.FrameRate         = 0x1000;
  swf.MovieSize.xmax    = cinfo.output_width*20;
  swf.MovieSize.ymax    = cinfo.output_height*20;

  printf("dx = %i, dy = %i\n",cinfo.output_width,cinfo.output_height);

  t = swf.FirstTag = InsertTag(NULL,ST_SETBACKGROUNDCOLOR);

        rgb.r = rgb.g = rgb.b = rgb.a  = 0x00;
        SetRGB(t,&rgb);

  t = InsertTag(t,ST_DEFINEBITSJPEG2);

        SetU16(t,1);  // id
  
        out = SetJPEGBitsStart(t,cinfo.output_width,cinfo.output_height,quality);
        scanline = (U8*)malloc(4*cinfo.output_width);
  
        if (scanline)
        { int y;
          U8 * js = scanline;
          for (y=0;y<cinfo.output_height;y++)
          { jpeg_read_scanlines(&cinfo,&js,1);
            SetJPEGBitsLines(out,(U8**)&js,1);
          }
          free(scanline);
        }
        
        SetJPEGBitsFinish(out);

        printf("JPEG Tag-Length: %06x\n",GetDataSize(t));

  t = InsertTag(t,ST_DEFINESHAPE);

        NewShape(&s);
        GetMatrix(NULL,&m);
        m.sx = 20*0x10000;
        m.sy = 20*0x10000;
        rgb.r = 0xff;
        fs = ShapeAddBitmapFillStyle(s,&m,1,0);
//        fs = ShapeAddSolidFillStyle(s,&rgb);
        
        SetU16(t,2); // id
        SetRect(t,&swf.MovieSize);
        SetShapeHeader(t,s);

        ShapeSetAll(t,s,0,0,0,fs,0);
        ShapeSetLine(t,s,swf.MovieSize.xmax,0);
        ShapeSetLine(t,s,0,swf.MovieSize.ymax);
        ShapeSetLine(t,s,-swf.MovieSize.xmax,0);
        ShapeSetLine(t,s,0,-swf.MovieSize.ymax);
        
        ShapeSetEnd(t);

  t = InsertTag(t,ST_PLACEOBJECT2);

        ObjectPlace(t,2,1,NULL,NULL,NULL);

  t = InsertTag(t,ST_SHOWFRAME);
  
  t = InsertTag(t,ST_END);

  jpeg_finish_decompress(&cinfo);
  fclose(f);
  
  handle = open(dname,O_RDWR|O_CREAT|O_TRUNC,0666);
  if FAILED(WriteSWF(handle,&swf)) fprintf(stderr,"WriteSWF() failed.\n");
  close(handle);

  return 0;
}
*/

