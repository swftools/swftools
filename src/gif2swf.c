/* -*- mode: c; tab-width: 4; -*- ---------------------------[for (x)emacs]--

   $Id: gif2swf.c,v 1.7 2008/02/08 11:43:12 kramm Exp $
   GIF to SWF converter tool

   Part of the swftools package.

   Copyright (c) 2005 Daichi Shinozaki <dseg@shield.jp>

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
   Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307, USA.

   This file is derived from png2swf.c */

#include <stdio.h>
#include <fcntl.h>
#include <gif_lib.h>
#include "../lib/rfxswf.h"
#include "../lib/args.h"

#define MAX_INPUT_FILES 1024
#define VERBOSE(x) (global.verbose>=x)
#define AS_FIRSTFRAME "if(!n) n=0;"
#define AS_LASTFRAME "if(n<%d){n=n+1;gotoAndPlay(1);}else stop();"

static struct {
    float framerate;
    int max_image_width;
    int max_image_height;
    int force_width;
    int force_height;
    int nfiles;
    int verbose;
    int do_cgi;
    int version;
    char *outfile;
    int imagecount;
    int loopcount;
} global;

struct {
    char *filename;
} image[MAX_INPUT_FILES];

struct gif_header {
    int width;
    int height;
};

enum disposal_method {
    NONE,
    DO_NOT_DISPOSE,
    RESTORE_TO_BGCOLOR,
    RESTORE_TO_PREVIOUS
};


void SetFrameAction(TAG ** t, const char *src, int ver)
{
    ActionTAG *as;

    as = swf_ActionCompile(src, ver);
    if (!as)
        fprintf(stderr, "Couldn't compile ActionScript\n");
    else {
        *t = swf_InsertTag(*t, ST_DOACTION);
        swf_ActionSet(*t, as);
        swf_ActionFree(as);
    }
}

int getGifDisposalMethod(GifFileType * gft, int framenum)
{
    int i;
    ExtensionBlock *ext = gft->SavedImages[framenum].ExtensionBlocks;

    for (i = 0; i < gft->SavedImages[framenum].ExtensionBlockCount; i++, ext++)
        if (ext->Function == GRAPHICS_EXT_FUNC_CODE)
            return ((ext->Bytes[0] & 0x1C) >> 2);

    return -1;
}

int getGifLoopCount(GifFileType * gft)
{
    int i, loop = -1;
    ExtensionBlock *ext = gft->SavedImages[0].ExtensionBlocks;

    for (i = 0; i < gft->SavedImages[0].ExtensionBlockCount; i++, ext++)
        if (ext->Function == APPLICATION_EXT_FUNC_CODE) {
            // info: http://semmix.pl/color/exgraf/eeg24.htm
            if (ext->ByteCount == 11 &&
                (strncmp(&ext->Bytes[0], "NETSCAPE2.0", 11) == 0 ||
                 strncmp(&ext->Bytes[0], "ANIMEXTS1.0", 11) == 0)) {
                // check for the subblock
                ext++;
                if (ext->ByteCount != 3)
                    ext--;
                else {
                    loop = GET16(&ext->Bytes[1]);
                    break;
                }
            }
        }

    return loop;
}

U16 getGifDelayTime(GifFileType * gft, int framenum)
{
    int i;
    ExtensionBlock *ext = gft->SavedImages[framenum].ExtensionBlocks;

    for (i = 0; i < gft->SavedImages[framenum].ExtensionBlockCount; i++, ext++)
        if (ext->Function == GRAPHICS_EXT_FUNC_CODE)
            return GET16(&ext->Bytes[1]);

    return 0;
}

int getTransparentColor(GifFileType * gft, int framenum)
{
    int i;
    ExtensionBlock *ext = gft->SavedImages[framenum].ExtensionBlocks;

    // Get transparency color from graphic extension block
    for (i = 0; i < gft->SavedImages[framenum].ExtensionBlockCount; i++, ext++)
        if ((ext->Function == GRAPHICS_EXT_FUNC_CODE) && (ext->Bytes[0] & 1)) {
            // there is a transparent color
            return ext->Bytes[3] == 0 ? 0 :     // exception
                (U8) ext->Bytes[3];             // transparency color
        }

    return -1;
}

TAG *MovieStart(SWF * swf, float framerate, int dx, int dy)
{
    TAG *t;
    RGBA rgb;

    memset(swf, 0x00, sizeof(SWF));

    swf->fileVersion = global.version;
    swf->frameRate = (int) (256.0 * framerate);
    swf->movieSize.xmax = dx * 20;
    swf->movieSize.ymax = dy * 20;

    t = swf->firstTag = swf_InsertTag(NULL, ST_SETBACKGROUNDCOLOR);

    rgb.r = rgb.g = rgb.b = rgb.a = 0x00;

    //rgb.g = 0xff; //<--- handy for testing alpha conversion
    swf_SetRGB(t, &rgb);

    return t;
}

int MovieFinish(SWF * swf, TAG * t, char *sname)
{
    int f, so = fileno(stdout);
    t = swf_InsertTag(t, ST_END);

    if ((!isatty(so)) && (!sname))
        f = so;
    else {
        if (!sname)
            sname = "output.swf";
        f = open(sname, O_WRONLY | O_CREAT | O_TRUNC | O_BINARY, 0644);
    }

    if (global.do_cgi) {
        if FAILED
            (swf_WriteCGI(swf)) fprintf(stderr, "WriteCGI() failed.\n");
    } else {
        if (swf_WriteSWF(f, swf) < 0)
            fprintf(stderr, "Unable to write output file: %s\n", sname);
        if (f != so)
            close(f);
    }

    swf_FreeTags(swf);
    return 0;
}

TAG *MovieAddFrame(SWF * swf, TAG * t, char *sname, int id, int imgidx)
{
    SHAPE *s;
    SRECT r;
    MATRIX m;
    int fs;

    U8 *imagedata, *from, *to;
    GifImageDesc *img;
    RGBA *pal;

    struct gif_header header;

    int i, j, numcolors, alphapalette;
    U8 bgcolor;
    int bpp;                    // byte per pixel
    int swf_width, padlen;

    ColorMapObject *colormap;
    GifColorType c;
    int interlacedOffset[] = { 0, 4, 2, 1 };    // The way Interlaced image should
    int interlacedJumps[] = { 8, 8, 4, 2 };     // be read - offsets and jumps...
    U16 delay, depth;
    int disposal;
    char *as_lastframe;

    GifFileType *gft;
    FILE *fi;

    if ((fi = fopen(sname, "rb")) == NULL) {
        if (VERBOSE(1))
            fprintf(stderr, "Read access failed: %s\n", sname);
        return t;
    }
    fclose(fi);

    if ((gft = DGifOpenFileName(sname)) == NULL) {
        fprintf(stderr, "%s is not a GIF file!\n", sname);
        return t;
    }

    if (DGifSlurp(gft) != GIF_OK) {
        PrintGifError();
        return t;
    }

    header.width = gft->SWidth;
    header.height = gft->SHeight;

    pal = (RGBA *) malloc(256 * sizeof(RGBA));
    memset(pal, 0, 256 * sizeof(RGBA));

    img = &gft->SavedImages[imgidx].ImageDesc;

    // Local colormap has precedence over Global colormap
    colormap = img->ColorMap ? img->ColorMap : gft->SColorMap;
    numcolors = colormap->ColorCount;
    alphapalette = getTransparentColor(gft, imgidx);
    if (VERBOSE(3))
        fprintf(stderr, "transparent palette index => %d\n", alphapalette);
    bpp = (alphapalette >= 0 ? 4 : 3);

    // bgcolor is the background color to fill the bitmap
    if (gft->SColorMap)         // There is a GlobalColorMap
        bgcolor = (U8) gft->SBackGroundColor;   // The SBGColor is meaningful
    else if (alphapalette >= 0) // There is a transparency color
        bgcolor = alphapalette; // set the bgcolor to tranparent
    else
        bgcolor = 0;
    // Don't know what to do here.
    // If this doesn't work, we could
    // create a new color and set the
    // alpha-channel to transparent
    // (unless we are using all the 256
    // colors, in which case either we
    // give up, or move to 16-bits palette
    if (VERBOSE(3))
        fprintf(stderr, "BG palette index => %u\n", bgcolor);

    for (i = 0; i < numcolors; i++) {
        c = colormap->Colors[i];
        if (i == bgcolor || i == alphapalette)
            pal[i].r = pal[i].g = pal[i].b = pal[i].a = 0;      // Fully transparent
        else {
            pal[i].r = c.Red;
            pal[i].g = c.Green;
            pal[i].b = c.Blue;
            pal[i].a = 255;     // Fully opaque
        }
    }

    t = swf_InsertTag(t, bpp == 4 ? ST_DEFINEBITSLOSSLESS2 : ST_DEFINEBITSLOSSLESS);
    swf_SetU16(t, id);          // id

    // Ah! The Flash specs says scanlines must be DWORD ALIGNED!
    // (but image width is the correct number of pixels)
    swf_width = BYTES_PER_SCANLINE(header.width);

    if ((imagedata = (U8 *) malloc(swf_width * header.height)) == NULL) {
        fprintf(stderr, "Failed to allocate memory required, aborted.");
        exit(2);
    }

    to = imagedata;
    from = (U8 *) gft->SavedImages[imgidx].RasterBits;

    if (swf_width == header.width) {
        // we are all nicely aligned and don't need to move the bitmap around.
        // Just copy the bits into the image buffer.
        if (!gft->Image.Interlace)
            if (header.width == img->Width && header.height == img->Height)
                memcpy(to, from, header.width * header.height);
            else {              //small screen
                for (i = 0; i < header.height; i++, to += header.width) {
                    memset(to, bgcolor, header.width);
                    if (i >= img->Top && i < img->Top + img->Height) {
                        memcpy(to + img->Left, from, img->Width);
                        from += img->Width;
                    }
                }
            }

        else                    // Need to perform 4 passes on the interlaced images
            for (i = 0; i < 4; i++)
                for (j = interlacedOffset[i]; j < header.height;
                     j += interlacedJumps[i], from += header.width)
                    memcpy(to + header.width * j, from, header.width);
    } else {
        padlen = swf_width - header.width;

        // here we need to pad the scanline
        if (!gft->Image.Interlace) {
            if (header.width == img->Width && header.height == img->Height) {
                for (i = 0; i < header.height; i++, from += header.width, to += swf_width) {
                    memcpy(to, from, header.width);
                    memset(to + header.width, bgcolor, padlen);
                }
            } else {            //small screen
                for (i = 0; i < header.height; i++, to += swf_width) {
                    memset(to, bgcolor, swf_width);
                    if (i >= img->Top && i < img->Top + img->Height) {
                        memcpy(to + img->Left, from, img->Width);
                        from += img->Width;
                    }
                }
            }
        } else {                // Need to perform 4 passes on the interlaced images
            for (i = 0; i < 4; i++)
                for (j = interlacedOffset[i]; j < header.height;
                     j += interlacedJumps[i], from += header.width) {
                    memcpy(to + swf_width * j, from, header.width);
                    memset(to + swf_width * j, bgcolor, padlen);
                }
        }
    }
    swf_SetLosslessBitsIndexed(t, header.width, header.height, imagedata, pal, 256);

    t = swf_InsertTag(t, ST_DEFINESHAPE);

    swf_ShapeNew(&s);
    swf_GetMatrix(NULL, &m);
    m.sx = 20 * 0x10000;
    m.sy = 20 * 0x10000;
    fs = swf_ShapeAddBitmapFillStyle(s, &m, id, 0);

    swf_SetU16(t, id + 1);      // id

    r.xmin = r.ymin = 0;
    r.xmax = header.width * 20;
    r.ymax = header.height * 20;
    swf_SetRect(t, &r);

    swf_SetShapeHeader(t, s);

    swf_ShapeSetAll(t, s, 0, 0, 0, fs, 0);
    swf_ShapeSetLine(t, s, r.xmax, 0);
    swf_ShapeSetLine(t, s, 0, r.ymax);
    swf_ShapeSetLine(t, s, -r.xmax, 0);
    swf_ShapeSetLine(t, s, 0, -r.ymax);

    swf_ShapeSetEnd(t);

    depth = imgidx + 1;
    if ((imgidx > 0) &&         // REMOVEOBJECT2 not needed at frame 1(imgidx==0)
        (global.imagecount > 1)) {
        // check last frame's disposal method
        if ((disposal = getGifDisposalMethod(gft, imgidx - 1)) >= 0) {
            switch (disposal) {
            case NONE:
                // [Replace one full-size, non-transparent frame with another]
                t = swf_InsertTag(t, ST_REMOVEOBJECT2);
                swf_SetU16(t, depth - 1);
                if (VERBOSE(3))
                    fprintf(stdout, "  [none]\n");
                break;
            case DO_NOT_DISPOSE:
                // [Any pixels not covered up by the next frame continue to display]
                if (VERBOSE(3))
                    fprintf(stdout, "  [don't dispose]\n");
                break;
            case RESTORE_TO_BGCOLOR:
                // [The background color or background tile -rather than a previous frame-
                //  shows through transparent pixels]
                t = swf_InsertTag(t, ST_REMOVEOBJECT2);
                swf_SetU16(t, depth - 2);
                if (VERBOSE(3))
                    fprintf(stdout, "  [restore to bg color]\n");
                break;
            case RESTORE_TO_PREVIOUS:
                // [Restores to the state of a previous, undisposed frame]
                // ** NOT IMPLEMENTED YET (same as "restore to bgcolor") **
                t = swf_InsertTag(t, ST_REMOVEOBJECT2);
                swf_SetU16(t, depth - 1);
                if (VERBOSE(3))
                    fprintf(stdout, "  [restore to previous]\n");
                break;
            default:
                break;
            }
        }
    }

    swf_SetU16(t, depth);
    t = swf_InsertTag(t, ST_PLACEOBJECT2);

    swf_GetMatrix(NULL, &m);
    m.tx = (swf->movieSize.xmax - (int) header.width * 20) / 2;
    m.ty = (swf->movieSize.ymax - (int) header.height * 20) / 2;
    swf_ObjectPlace(t, id + 1, depth, &m, NULL, NULL);

    if ((global.imagecount > 1) && (global.loopcount > 0)) { // 0 means infinite loop
        if (imgidx == 0)
            SetFrameAction(&t, AS_FIRSTFRAME, global.version);
    }

    t = swf_InsertTag(t, ST_SHOWFRAME);

    if (global.imagecount > 1) { // multi-frame GIF?
        int framecnt;
        delay = getGifDelayTime(gft, imgidx); // delay in 1/100 sec
        framecnt = (int) (global.framerate * (delay / 100.0));
        if (framecnt > 1) {
            if (VERBOSE(2))
                fprintf(stderr, "at frame %d: pad %d frames(%.3f sec)\n",
                        imgidx + 1, framecnt, delay / 100.0);

            framecnt -= 1; // already inserted a frame
            while (framecnt--)
                t = swf_InsertTag(t, ST_SHOWFRAME);
        }
        if ((imgidx == global.imagecount - 1) &&global.loopcount > 0) { // last frame
            as_lastframe = malloc(strlen(AS_LASTFRAME) + 5); // 0-99999
            sprintf(as_lastframe, AS_LASTFRAME, global.loopcount);
            SetFrameAction(&t, as_lastframe, global.version);
            if (as_lastframe)
                free(as_lastframe);
        }
    }

    free(pal);
    free(imagedata);
    DGifCloseFile(gft);

    return t;
}

int CheckInputFile(char *fname, char **realname)
{
    FILE *fi;
    char *s = malloc(strlen(fname) + 5);
    GifFileType *gft;

    if (!s)
        exit(2);
    (*realname) = s;
    strcpy(s, fname);

    // Check whether file exists (with typical extensions)

    if ((fi = fopen(s, "rb")) == NULL) {
        sprintf(s, "%s.gif", fname);
        if ((fi = fopen(s, "rb")) == NULL) {
            sprintf(s, "%s.GIF", fname);
            if ((fi = fopen(s, "rb")) == NULL) {
                sprintf(s, "%s.Gif", fname);
                if ((fi = fopen(s, "rb")) == NULL) {
                    fprintf(stderr, "Couldn't open %s!\n", fname);
                    return -1;
                }
            }
        }
    }
    fclose(fi);

    if ((gft = DGifOpenFileName(s)) == NULL) {
        fprintf(stderr, "%s is not a GIF file!\n", fname);
        return -1;
    }

    if (global.max_image_width < gft->SWidth)
        global.max_image_width = gft->SWidth;
    if (global.max_image_height < gft->SHeight)
        global.max_image_height = gft->SHeight;

    if (DGifSlurp(gft) != GIF_OK) { 
        PrintGifError();
        return -1;
    }
    // After DGifSlurp() call, gft->ImageCount become available
    if ((global.imagecount = gft->ImageCount) >1) {
        if (global.loopcount < 0) {
            global.loopcount = getGifLoopCount(gft);
            if (VERBOSE(3))
                fprintf(stderr, "Loops => %d\n", global.loopcount);
        }
    }
    if (VERBOSE(2)) {
        U8 i;
        fprintf(stderr, "%d x %d, %d images total\n", gft->SWidth, gft->SHeight, gft->ImageCount);

        for (i = 0; i < gft->ImageCount; i++)
            fprintf(stderr, "frame: %u, delay: %.3f sec\n", i + 1, getGifDelayTime(gft, i) / 100.0);
    }

    DGifCloseFile(gft);

    return 0;
}

int args_callback_option(char *arg, char *val)
{
    int res = 0;
    if (arg[1])
        res = -1;
    else
        switch (arg[0]) {
        case 'l':
            if (val)
                global.loopcount = atoi(val);
            res = 1;
            break;

        case 'r':
            if (val)
                global.framerate = atof(val);
            if ((global.framerate < 1.0 / 256) ||(global.framerate >= 256.0)) {
                if (VERBOSE(1))
                    fprintf(stderr,
                            "Error: You must specify a valid framerate between 1/256 and 255.\n");
                exit(1);
            }
            res = 1;
            break;

        case 'o':
            if (val)
                global.outfile = val;
            res = 1;
            break;

        case 'z':
            global.version = 6;
            res = 0;
            break;

        case 'C':
            global.do_cgi = 1;
            break;

        case 'v':
            if (val)
                global.verbose = atoi(val);
            res = 1;
            break;

        case 'X':
            if (val)
                global.force_width = atoi(val);
            res = 1;
            break;

        case 'Y':
            if (val)
                global.force_height = atoi(val);
            res = 1;
            break;

        case 'V':
            printf("gif2swf - part of %s %s\n", PACKAGE, VERSION);
            exit(0);

        default:
            res = -1;
            break;
        }

    if (res < 0) {
        if (VERBOSE(1))
            fprintf(stderr, "Unknown option: -%s\n", arg);
        exit(1);
        return 0;
    }
    return res;
}

static struct options_t options[] = {
{"r", "rate"},
{"o", "output"},
{"z", "zlib"},
{"l", "loop"},
{"X", "pixel"},
{"Y", "pixel"},
{"v", "verbose"},
{"C", "cgi"},
{"V", "version"},
{0,0}
};

int args_callback_longoption(char *name, char *val)
{
    return args_long2shortoption(options, name, val);
}

int args_callback_command(char *arg, char *next) // actually used as filename
{
    char *s;
    if (CheckInputFile(arg, &s) < 0) {
        if (VERBOSE(1))
            fprintf(stderr, "Error opening input file: %s\n", arg);
        free(s);

    } else {
        image[global.nfiles].filename = s;
        global.nfiles++;
        if (global.nfiles >= MAX_INPUT_FILES) {
            if (VERBOSE(1))
                fprintf(stderr, "Error: Too many input files.\n");
            exit(1);
        }
    }

    return 0;
}

void args_callback_usage(char *name)
{
    printf("\n");
    printf("Usage: %s [-X width] [-Y height] [-o file.swf] [-r rate] file1.gif [file2.gif ...]\n", name);
    printf("\n");
    printf("-r , --rate <framerate>        Set movie framerate (frames per second)\n");
    printf("-o , --output <filename>       Set name for SWF output file.\n");
    printf("-z , --zlib <zlib>             Enable Flash 6 (MX) Zlib Compression\n");
    printf("-l , --loop <loop count>           Set loop count. (default: 0 [=infinite loop])\n");
    printf("-X , --pixel <width>           Force movie width to <width> (default: autodetect)\n");
    printf("-Y , --pixel <height>          Force movie height to <height> (default: autodetect)\n");
    printf("-v , --verbose <level>         Set verbose level (0=quiet, 1=default, 2=debug)\n");
    printf("-C , --cgi                     For use as CGI- prepend http header, write to stdout\n");
    printf("-V , --version                 Print version information and exit\n");
    printf("\n");
}

int main(int argc, char **argv)
{
    SWF swf;
    TAG *t;

    memset(&global, 0x00, sizeof(global));

    global.framerate = 1.0;
    global.verbose = 1;
    global.version = 5;
    global.loopcount = -1;

    processargs(argc, argv);

    if (global.nfiles <= 0) {
        fprintf(stderr, "No gif files found in arguments\n");
        return 1;
    }

    if (VERBOSE(2))
        fprintf(stderr, "Processing %i file(s)...\n", global.nfiles);

    if (global.imagecount > 1)       // multi-frame GIF?
        if (global.framerate == 1.0) // user not specified '-r' option?
            global.framerate = 10.0;

    t = MovieStart(&swf, global.framerate,
                   global.force_width ? global.force_width : global.max_image_width,
                   global.force_height ? global.force_height : global.max_image_height);
    {
        int i, j;
        for (i = 0; i < global.nfiles; i++) {
            if (VERBOSE(3))
                fprintf(stderr, "[%03i] %s\n", i, image[i].filename);
            t = MovieAddFrame(&swf, t, image[i].filename, (i * 2) + 1, 0);
            for (j = 2; j <= global.imagecount; j++)
                t = MovieAddFrame(&swf, t, image[i].filename, (j * 2) - 1, j - 1);
            free(image[i].filename);
        }
    }

    MovieFinish(&swf, t, global.outfile);

    return 0;
}
