/* swfbits.c

   Bitmap functions (needs libjpeg) 

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2000, 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This file is distributed under the GPL, see file COPYING for details 

*/

#ifdef _JPEGLIB_INCLUDED_
#define OUTBUFFER_SIZE 0x8000

typedef struct _JPEGDESTMGR
{ struct jpeg_destination_mgr mgr;
  TAG *  t;
  JOCTET * buffer;
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
} JPEGDESTMGR, * LPJPEGDESTMGR;

// Destination manager callbacks

void swf_init_destination(j_compress_ptr cinfo) 
{ JPEGDESTMGR * dmgr = (JPEGDESTMGR *)cinfo->dest;
  dmgr->buffer = (JOCTET*)malloc(OUTBUFFER_SIZE);
  dmgr->mgr.next_output_byte = dmgr->buffer;
  dmgr->mgr.free_in_buffer = OUTBUFFER_SIZE;
}

boolean swf_empty_output_buffer(j_compress_ptr cinfo)
{ JPEGDESTMGR * dmgr = (JPEGDESTMGR *)cinfo->dest;
  SetBlock(dmgr->t,(U8*)dmgr->buffer,OUTBUFFER_SIZE);
  dmgr->mgr.next_output_byte = dmgr->buffer;
  dmgr->mgr.free_in_buffer = OUTBUFFER_SIZE;
  return TRUE;
}

void swf_term_destination(j_compress_ptr cinfo) 
{ JPEGDESTMGR * dmgr = (JPEGDESTMGR *)cinfo->dest;
  SetBlock(dmgr->t,(U8*)dmgr->buffer,OUTBUFFER_SIZE-dmgr->mgr.free_in_buffer);
  free(dmgr->buffer);
  dmgr->mgr.free_in_buffer = 0;
}

JPEGBITS * SetJPEGBitsStart(TAG * t,int width,int height,int quality)
{
  JPEGDESTMGR * jpeg;
        
  // redirect compression lib output to local SWF Tag structure
  
  jpeg = (JPEGDESTMGR *)malloc(sizeof(JPEGDESTMGR));
  if (!jpeg) return NULL;
  
  memset(jpeg,0x00,sizeof(JPEGDESTMGR));
  jpeg->cinfo.err = jpeg_std_error(&jpeg->jerr);

  jpeg_create_compress(&jpeg->cinfo);

  jpeg->mgr.init_destination =  swf_init_destination;
  jpeg->mgr.empty_output_buffer =       swf_empty_output_buffer;
  jpeg->mgr.term_destination =  swf_term_destination;
      
  jpeg->t = t;

  jpeg->cinfo.dest = (struct jpeg_destination_mgr *)jpeg;

  // init compression
  
  jpeg->cinfo.image_width  = width;
  jpeg->cinfo.image_height = height;
  jpeg->cinfo.input_components = 3;
  jpeg->cinfo.in_color_space = JCS_RGB;

  jpeg_set_defaults(&jpeg->cinfo);
  jpeg_set_quality(&jpeg->cinfo,quality,TRUE);

  // write tables to SWF
  
  jpeg_write_tables(&jpeg->cinfo);

  // compess image to SWF
   
  jpeg_suppress_tables(&jpeg->cinfo, TRUE);
  jpeg_start_compress(&jpeg->cinfo, FALSE);

  return (JPEGBITS *)jpeg;
}

int SetJPEGBitsLines(JPEGBITS * jpegbits,U8 ** data,int n)
{ JPEGDESTMGR * jpeg = (JPEGDESTMGR *)jpegbits;
  if (!jpeg) return -1;
  jpeg_write_scanlines(&jpeg->cinfo,data,n);
  return 0;
}

int SetJPEGBitsLine(JPEGBITS * jpegbits,U8 * data)
{ return SetJPEGBitsLines(jpegbits,&data,1);
}

int SetJPEGBitsFinish(JPEGBITS * jpegbits)
{ JPEGDESTMGR * jpeg = (JPEGDESTMGR *)jpegbits;
  if (!jpeg) return -1;
  jpeg_finish_compress(&jpeg->cinfo);
  free(jpeg);
  return 0;
}

int SetJPEGBits(TAG * t,char * fname,int quality)
{ struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JPEGBITS * out;
  FILE * f;
  U8 * scanline;
  
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo); 

  if ((f=fopen(fname,"rb"))==NULL) return -1;
  

  jpeg_stdio_src(&cinfo,f);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  out = SetJPEGBitsStart(t,cinfo.output_width,cinfo.output_height,quality);
  scanline = (U8*)malloc(4*cinfo.output_width);
  
  if (scanline)
  { int y;
    U8 * js = scanline;
    for (y=0;y<cinfo.output_height;y++)
    { jpeg_read_scanlines(&cinfo,&js,1);
      SetJPEGBitsLines(out,(U8**)&js,1);
    }
  }

  SetJPEGBitsFinish(out);
  jpeg_finish_decompress(&cinfo);
  fclose(f);
  
  return 0;
}

#undef OUTBUFFER_SIZE
#endif

// insert zlib/PNG functions here
