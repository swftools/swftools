/* swfbits.c

   Bitmap functions (needs libjpeg) 

   Extension module for the rfxswf library.
   Part of the swftools package.

   Copyright (c) 2000, 2001 Rainer Böhme <rfxswf@reflex-studio.de>
 
   This program is free software; you can redistribute it and/or modify
   it under the terms of the GNU General Public License as published by
   the Free Software Foundation; either version 2 of the License, or
   (at your option) any later version.

   This program is distributed in the hope that it will be useful,
   but WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
   GNU General Public License for more details.

   You should have received a copy of the GNU General Public License
   along with this program; if not, write to the Free Software
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA */

#define OUTBUFFER_SIZE 0x8000

#ifdef HAVE_JPEGLIB

typedef struct _JPEGDESTMGR
{ struct jpeg_destination_mgr mgr;
  TAG *  t;
  JOCTET * buffer;
  struct jpeg_compress_struct cinfo;
  struct jpeg_error_mgr jerr;
} JPEGDESTMGR, * LPJPEGDESTMGR;

// Destination manager callbacks

void RFXSWF_init_destination(j_compress_ptr cinfo) 
{ JPEGDESTMGR * dmgr = (JPEGDESTMGR *)cinfo->dest;
  dmgr->buffer = (JOCTET*)malloc(OUTBUFFER_SIZE);
  dmgr->mgr.next_output_byte = dmgr->buffer;
  dmgr->mgr.free_in_buffer = OUTBUFFER_SIZE;
}

boolean RFXSWF_empty_output_buffer(j_compress_ptr cinfo)
{ JPEGDESTMGR * dmgr = (JPEGDESTMGR *)cinfo->dest;
  swf_SetBlock(dmgr->t,(U8*)dmgr->buffer,OUTBUFFER_SIZE);
  dmgr->mgr.next_output_byte = dmgr->buffer;
  dmgr->mgr.free_in_buffer = OUTBUFFER_SIZE;
  return TRUE;
}

void RFXSWF_term_destination(j_compress_ptr cinfo) 
{ JPEGDESTMGR * dmgr = (JPEGDESTMGR *)cinfo->dest;
  swf_SetBlock(dmgr->t,(U8*)dmgr->buffer,OUTBUFFER_SIZE-dmgr->mgr.free_in_buffer);
  free(dmgr->buffer);
  dmgr->mgr.free_in_buffer = 0;
}

JPEGBITS * swf_SetJPEGBitsStart(TAG * t,int width,int height,int quality)
{
  JPEGDESTMGR * jpeg;
        
  // redirect compression lib output to local SWF Tag structure
  
  jpeg = (JPEGDESTMGR *)malloc(sizeof(JPEGDESTMGR));
  if (!jpeg) return NULL;
  
  memset(jpeg,0x00,sizeof(JPEGDESTMGR));
  jpeg->cinfo.err = jpeg_std_error(&jpeg->jerr);

  jpeg_create_compress(&jpeg->cinfo);

  jpeg->mgr.init_destination = RFXSWF_init_destination;
  jpeg->mgr.empty_output_buffer = RFXSWF_empty_output_buffer;
  jpeg->mgr.term_destination = RFXSWF_term_destination;
      
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

int swf_SetJPEGBitsLines(JPEGBITS * jpegbits,U8 ** data,int n)
{ JPEGDESTMGR * jpeg = (JPEGDESTMGR *)jpegbits;
  if (!jpeg) return -1;
  jpeg_write_scanlines(&jpeg->cinfo,data,n);
  return 0;
}

int swf_SetJPEGBitsLine(JPEGBITS * jpegbits,U8 * data)
{ return swf_SetJPEGBitsLines(jpegbits,&data,1);
}

int swf_SetJPEGBitsFinish(JPEGBITS * jpegbits)
{ JPEGDESTMGR * jpeg = (JPEGDESTMGR *)jpegbits;
  if (!jpeg) return -1;
  jpeg_finish_compress(&jpeg->cinfo);
  free(jpeg);
  return 0;
}

void swf_SetJPEGBits2(TAG * tag,U16 width,U16 height,RGBA* bitmap, int quality)
{
  JPEGBITS* jpeg;
  int y;
  jpeg = swf_SetJPEGBitsStart(tag,width,height,quality);
  for (y=0;y<height;y++)
  { U8 scanline[3*width];
    int x,p = 0;
    for (x=0;x<width;x++) 
    { scanline[p++] = bitmap[width*y+x].r;
      scanline[p++] = bitmap[width*y+x].g;
      scanline[p++] = bitmap[width*y+x].b;
    }
    swf_SetJPEGBitsLine(jpeg,scanline);
  }
  swf_SetJPEGBitsFinish(jpeg);
}

void swf_GetJPEGSize(char * fname, int*width, int*height)
{ struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  FILE * fi;
  *width = 0;
  *height = 0;
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo); 
  if ((fi=fopen(fname,"rb"))==NULL) {
      fprintf(stderr, "rfxswf: file open error\n");
      return;
  }
  jpeg_stdio_src(&cinfo, fi);
  jpeg_read_header(&cinfo, TRUE);
  *width = cinfo.image_width;
  *height = cinfo.image_height;
  jpeg_destroy_decompress(&cinfo);
  fclose(fi);
}

int swf_SetJPEGBits(TAG * t,char * fname,int quality)
{ struct jpeg_decompress_struct cinfo;
  struct jpeg_error_mgr jerr;
  JPEGBITS * out;
  FILE * f;
  U8 * scanline;
  
  cinfo.err = jpeg_std_error(&jerr);
  jpeg_create_decompress(&cinfo); 

  if ((f=fopen(fname,"rb"))==NULL) {
      fprintf(stderr, "rfxswf: file open error\n");
      return -1;
  }

  jpeg_stdio_src(&cinfo,f);
  jpeg_read_header(&cinfo, TRUE);
  jpeg_start_decompress(&cinfo);

  out = swf_SetJPEGBitsStart(t,cinfo.output_width,cinfo.output_height,quality);
  scanline = (U8*)malloc(4*cinfo.output_width);
  
  if (scanline) 
  { int y;
    U8 * js = scanline;
    if(cinfo.out_color_space == JCS_GRAYSCALE) {
	for (y=0;y<cinfo.output_height;y++)
	{ int x;
	  jpeg_read_scanlines(&cinfo,&js,1);
	  for(x=cinfo.output_width-1;x>=0;x--) {
	      js[x*3] = js[x*3+1] = js[x*3+2] = js[x];
	  }
	  swf_SetJPEGBitsLines(out,(U8**)&js,1);
	}
    }
    else if(cinfo.out_color_space == JCS_RGB) 
    {
	for (y=0;y<cinfo.output_height;y++)
	{ jpeg_read_scanlines(&cinfo,&js,1);
	  swf_SetJPEGBitsLines(out,(U8**)&js,1);
	}
    }
    else if(cinfo.out_color_space == JCS_YCCK) 
    {
	//FIXME
	fprintf(stderr, "Error: Can't convert YCCK to RGB.\n");
	return -1;
    }
    else if(cinfo.out_color_space == JCS_YCbCr) 
    {
	for (y=0;y<cinfo.output_height;y++) {
	  int x;
	  for(x=0;x<cinfo.output_width;x++) {
	      int y = js[x*3+0];
	      int u = js[x*3+1];
	      int v = js[x*3+1];
	      js[x*3+0] = y + ((360*(v-128))>>8);
	      js[x*3+1] = y - ((88*(u-128)+183*(v-128))>>8);
	      js[x*3+2] = y + ((455 * (u-128))>>8);
	  }
	}
    }
    else if(cinfo.out_color_space == JCS_CMYK) 
    { 
	for (y=0;y<cinfo.output_height;y++)
	{ int x;
	  jpeg_read_scanlines(&cinfo,&js,1);
	  /* This routine seems to work for now-
	     It's a mixture of 3 different
	     CMYK->RGB conversion routines I found in the
	     web. (which all produced garbage)
	     I'm happily accepting suggestions. (mk)*/
	  for(x=0;x<cinfo.output_width;x++) {
		int white = 255 - js[x*4+3];
		js[x*3+0] = white - ((js[x*4]*white)>>8);
		js[x*3+1] = white - ((js[x*4+1]*white)>>8);
		js[x*3+2] = white - ((js[x*4+2]*white)>>8);
	  }
	  swf_SetJPEGBitsLines(out,(U8**)&js,1);
	}
    }
  }

  swf_SetJPEGBitsFinish(out);
  jpeg_finish_decompress(&cinfo);
  fclose(f);
  
  return 0;
}

#endif // HAVE_JPEGLIB

// Lossless compression texture based on zlib

#ifdef HAVE_ZLIB

int RFXSWF_deflate_wraper(TAG * t,z_stream * zs,boolean finish)
{ 
  U8*data=malloc(OUTBUFFER_SIZE);
  zs->next_out = data;
  zs->avail_out = OUTBUFFER_SIZE;
  while (1)
  { int status = deflate(zs,Z_NO_FLUSH);

    if (status!=Z_OK)
    {
#ifdef DEBUG_RFXSWF
      fprintf(stderr,"rfxswf: zlib compression error (%i)\n",status);
#endif
      free(data);
      return status;
    }

    if (zs->next_out!=data)
    { swf_SetBlock(t,data,zs->next_out - data);
      zs->next_out = data;
      zs->avail_out = OUTBUFFER_SIZE;
    }
    
    if (zs->avail_in==0)
      break;
  }

  if(!finish) {
      free(data);
      return 0;
  }

  while(1) {
    int status = deflate(zs,Z_FINISH);
    if (status!=Z_OK && status!=Z_STREAM_END)
    {
#ifdef DEBUG_RFXSWF
      fprintf(stderr,"rfxswf: zlib compression error (%i)\n",status);
#endif
      free(data);
      return status;
    }

    if (zs->next_out!=data)
    { 
      swf_SetBlock(t,data,zs->next_out - data);
      zs->next_out = data;
      zs->avail_out = OUTBUFFER_SIZE;
    }

    if(status == Z_STREAM_END)
	break;
  }
  free(data);
  return 0;
}


int swf_SetLosslessBits(TAG * t,U16 width,U16 height,void * bitmap,U8 bitmap_flags)
{ int res = 0;
  int bps;
  
  switch (bitmap_flags)
  { case BMF_8BIT:
      return swf_SetLosslessBitsIndexed(t,width,height,bitmap,NULL,256);
    case BMF_16BIT:
      bps = BYTES_PER_SCANLINE(sizeof(U16)*width);
      break;
    case BMF_32BIT:
      bps = width*4;
      break;
    default:
      fprintf(stderr, "rfxswf: unknown bitmap type %d\n", bitmap_flags);
      return -1;
  }

  swf_SetU8(t,bitmap_flags);
  swf_SetU16(t,width);
  swf_SetU16(t,height);

  { z_stream zs;
      
    memset(&zs,0x00,sizeof(z_stream));
    zs.zalloc = Z_NULL;
    zs.zfree  = Z_NULL;

    if (deflateInit(&zs,Z_DEFAULT_COMPRESSION)==Z_OK)
    { zs.avail_in         = bps*height;
      zs.next_in          = bitmap;

      if (RFXSWF_deflate_wraper(t,&zs,TRUE)<0) res = -3;
      deflateEnd(&zs);
      
    } else res = -3; // zlib error
  }
  return res;
}

int swf_SetLosslessBitsIndexed(TAG * t,U16 width,U16 height,U8 * bitmap,RGBA * palette,U16 ncolors)
{ RGBA * pal = palette;
  int bps = BYTES_PER_SCANLINE(width);
  int res = 0;
    
  if (!pal)     // create default palette for grayscale images
  { int i;
    pal = malloc(256*sizeof(RGBA));
    for (i=0;i<256;i++) { pal[i].r = pal[i].g = pal[i].b = i; pal[i].a = 0xff;}
    ncolors = 256;
  }
  
  if ((ncolors<2)||(ncolors>256)||(!t)) {
      fprintf(stderr, "rfxswf: unsupported number of colors: %d\n", ncolors);
      return -1; // parameter error
  }

  swf_SetU8(t,BMF_8BIT);
  swf_SetU16(t,width);
  swf_SetU16(t,height);
  swf_SetU8(t,ncolors-1); // number of pal entries

  { z_stream zs;

    memset(&zs,0x00,sizeof(z_stream));
    zs.zalloc = Z_NULL;
    zs.zfree  = Z_NULL;

    if (deflateInit(&zs,Z_DEFAULT_COMPRESSION)==Z_OK)
    { U8 * zpal;                    // compress palette
      if ((zpal = malloc(ncolors*4)))
      { U8 * pp = zpal;
        int i;

    /* be careful with ST_DEFINEBITSLOSSLESS2, because
       the Flash player produces great bugs if you use too many
       alpha colors in your palette. The only sensible result that
       can be archeived is setting one color to r=0,b=0,g=0,a=0 to
       make transparent parts in sprites. That's the cause why alpha
       handling is implemented in lossless routines of rfxswf.

       Indeed: I haven't understood yet how flash player handles
       alpha values different from 0 and 0xff in lossless bitmaps...
    */

        if (swf_GetTagID(t)==ST_DEFINEBITSLOSSLESS2)  // have alpha channel?
        { for (i=0;i<ncolors;i++)         
          { pp[0] = pal[i].r;
            pp[1] = pal[i].g;
            pp[2] = pal[i].b;
            pp[3] = pal[i].a;
            pp+=4; 
          }
          zs.avail_in = 4*ncolors;
        }
        else
        { for (i=0;i<ncolors;i++)         // pack RGBA structures to RGB 
          { pp[0] = pal[i].r;
            pp[1] = pal[i].g;
            pp[2] = pal[i].b;
            pp+=3;
          }
          zs.avail_in         = 3*ncolors;
        }

        zs.next_in          = zpal;

        if (RFXSWF_deflate_wraper(t,&zs,FALSE)<0) res = -3;

                                    // compress bitmap
        zs.next_in = bitmap;
        zs.avail_in = (bps*height*sizeof(U8));

        if (RFXSWF_deflate_wraper(t,&zs,TRUE)<0) res = -3;

        deflateEnd(&zs);

        free(zpal);
      } else res = -2; // memory error
    } else res = -3; // zlib error
  }
  
  if (!palette) free(pal);

  return res;
}

int swf_SetLosslessBitsGrayscale(TAG * t,U16 width,U16 height,U8 * bitmap)
{ return swf_SetLosslessBitsIndexed(t,width,height,bitmap,NULL,256);
}


#endif // HAVE_ZLIB

#if defined(HAVE_ZLIB) && defined(HAVE_JPEGLIB)
int swf_SetJPEGBits3(TAG * tag,U16 width,U16 height,RGBA* bitmap, int quality)
{
  JPEGBITS* jpeg;
  int y;
  int pos;
  int res = 0;
  U8 * data;
  z_stream zs;
  
  pos = tag->len;
  swf_SetU32(tag, 0); //placeholder
  jpeg = swf_SetJPEGBitsStart(tag,width,height,quality);
  for (y=0;y<height;y++)
  { U8 scanline[3*width];
    int x,p = 0;
    for (x=0;x<width;x++) 
    { scanline[p++] = bitmap[width*y+x].r;
      scanline[p++] = bitmap[width*y+x].g;
      scanline[p++] = bitmap[width*y+x].b;
    }
    swf_SetJPEGBitsLine(jpeg,scanline);
  }
  swf_SetJPEGBitsFinish(jpeg);
  PUT32(&tag->data[pos], tag->len - pos - 4);

  data=malloc(OUTBUFFER_SIZE);
  memset(&zs,0x00,sizeof(z_stream));

  if (deflateInit(&zs,Z_DEFAULT_COMPRESSION)!=Z_OK) {
      fprintf(stderr, "rfxswf: zlib compression failed");
      return -3;
  }
    
  zs.next_out         = data;
  zs.avail_out        = OUTBUFFER_SIZE;

  for (y=0;y<height;y++)
  { U8 scanline[width];
    int x,p = 0;
    for (x=0;x<width;x++) {
      scanline[p++] = bitmap[width*y+x].a;
    }
    zs.avail_in         = width;
    zs.next_in          = scanline;

    while(1) {
	if(deflate(&zs, Z_NO_FLUSH) != Z_OK) {
	    fprintf(stderr, "rfxswf: zlib compression failed");
	    return -4;
	}
	if(zs.next_out != data) {
	    swf_SetBlock(tag, data, zs.next_out - data);
	    zs.next_out = data;
	    zs.avail_out = OUTBUFFER_SIZE;
	}
	if(!zs.avail_in) {
	    break;
	}
    }
  }

  while(1) {
      int ret = deflate(&zs, Z_FINISH);
      if (ret != Z_OK &&
	  ret != Z_STREAM_END)  {
	  fprintf(stderr, "rfxswf: zlib compression failed");
	  return -5;
      }
      if(zs.next_out != data) {
	  swf_SetBlock(tag, data, zs.next_out - data);
	  zs.next_out = data;
	  zs.avail_out = OUTBUFFER_SIZE;
      }
      if (ret == Z_STREAM_END) {
	  break;
      }
  }

  deflateEnd(&zs);
  free(data);
  return 0;
}
#endif

#undef OUTBUFFER_SIZE

