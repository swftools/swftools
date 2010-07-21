#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

#include "../gfxdevice.h"
#include "../gfxtools.h"
#include "../MD5.h"
#include "../types.h"

#include <math.h>
#include <time.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include <stdarg.h>

//#define ZSTEP (1/65536.0)
#define ZSTEP (1/32.0)
//#define ZSTEP (1/4.0)

typedef struct _fontlist {
    gfxfont_t*font;
    struct _fontlist*next;
} fontlist_t;

typedef struct _internal {
    gfxfont_t*font;
    char*fontid;
    fontlist_t* fontlist;
    int width, height;
    int currentz;
   
    int config_polygonoutlines;
   
    GLUtesselator *tesselator;
    GLUtesselator *tesselator_line;
    GLUtesselator *tesselator_tex;
} internal_t;

static int verbose = 0;
static void dbg(char*format, ...)
{
    char buf[1024];
    int l;
    va_list arglist;
    if(!verbose)
	return;
    va_start(arglist, format);
    vsnprintf(buf, sizeof(buf)-1, format, arglist);
    va_end(arglist);
    l = strlen(buf);
    while(l && buf[l-1]=='\n') {
	buf[l-1] = 0;
	l--;
    }
    printf("(device-opengl) %s\n", buf);
    fflush(stdout);
}

#ifndef CALLBACK 
#define CALLBACK
#endif

typedef void(*callbackfunction_t)();

void CALLBACK errorCallback(GLenum errorCode)
{
   const GLubyte *estring;
   estring = gluErrorString(errorCode);
   printf("Tessellation Error: %s\n", estring);
   exit(0);
}
void CALLBACK beginCallback(GLenum which)
{
    glBegin(which);
}
void CALLBACK endCallback(void)
{
    glEnd();
}
void CALLBACK vertexCallback(GLvoid *vertex)
{
   double*xyz = (GLdouble*)vertex;
   glVertex3d(xyz[0],xyz[1],xyz[2]);
}
void CALLBACK combineCallback(GLdouble coords[3], GLdouble *data[4], GLfloat w[4], GLdouble **out)
{
   GLdouble *vertex;
   vertex = (GLdouble *) malloc(6 * sizeof(GLdouble));
   vertex[0] = coords[0];
   vertex[1] = coords[1];
   vertex[2] = coords[2];
   *out = vertex;
}
void CALLBACK vertexCallbackTex(GLvoid *vertex)
{
   double*v = (GLdouble*)vertex;
   glTexCoord2f(v[3],v[4]);
   glVertex3d(v[0],v[1],v[2]);
}
void CALLBACK combineCallbackTex(GLdouble coords[3], GLdouble *data[4], GLfloat w[4], GLdouble **out)
{
   GLdouble *vertex;
   vertex = (GLdouble *) malloc(5 * sizeof(GLdouble));
   vertex[0] = coords[0];
   vertex[1] = coords[1];
   vertex[2] = coords[2];
   if(data[2] && data[3]) {
       vertex[3] = w[0]*data[0][3] + w[1]*data[1][3] + w[2]*data[2][3] + w[3]*data[3][3];
       vertex[4] = w[0]*data[0][4] + w[1]*data[1][4] + w[2]*data[2][4] + w[3]*data[3][4];
   } else {
       vertex[3] = w[0]*data[0][3] + w[1]*data[1][3];
       vertex[4] = w[0]*data[0][4] + w[1]*data[1][4];
   }
   *out = vertex;
}

int opengl_setparameter(struct _gfxdevice*dev, const char*key, const char*value)
{
    internal_t*i = (internal_t*)dev->internal;
    dbg("setparameter %s=%s", key, value);
    if(!strcmp(key, "polygonoutlines")) {
        i->config_polygonoutlines = atoi(value);
    }
    return 0;
}

void opengl_startpage(struct _gfxdevice*dev, int width, int height)
{
    dbg("startpage %d %d", width, height);
    internal_t*i = (internal_t*)dev->internal;
    i->width = width;
    i->height = height;
    i->currentz = 0;
}

void opengl_startclip(struct _gfxdevice*dev, gfxline_t*line)
{
    dbg("startclip");
}

void opengl_endclip(struct _gfxdevice*dev)
{
    dbg("endclip");
}

void opengl_stroke(struct _gfxdevice*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
    dbg("stroke");
    internal_t*i = (internal_t*)dev->internal;
    i->currentz++;
    char running = 0;
    gfxline_t*l=0;

    glColor4f(color->r/255.0, color->g/255.0, color->b/255.0, color->a/255.0);
    
    //glLineWidth(width*64);
    if(width <= 0) {
        width = 1.0;
    }
    glLineWidth(width);
    double z = i->currentz*ZSTEP;

    glPolygonOffset(0.0, 500.0);

    l = line;
    while(l) {
	if(l->type == gfx_moveTo) {
	    if(running) {
		running = 0;
		glEnd();
	    }
	}
	if(!running) {
	    running = 1;
	    glBegin(GL_LINE_STRIP);
	}
	glVertex3d(l->x, l->y, z);
	l=l->next;
    }
    if(running) {
	running = 0;
	glEnd();
    }
    glLineWidth(1.0);
}

#define SPLINE_SUBDIVISION 2

void tesselatePolygon(GLUtesselator*tesselator, double z, gfxline_t*line)
{
    int len = 0;
    gfxline_t*l=0;
    double lastx=0,lasty=0;
    double*xyz=0;
    char running = 0;
    gluTessBeginPolygon(tesselator, NULL);
    l = line;
    len = 0;
    while(l) {
	if(l->type == gfx_splineTo) {
            double c = sqrt(abs(l->x-2*l->sx+lastx) + abs(l->y-2*l->sy+lasty))*SPLINE_SUBDIVISION;
	    int steps = (int)c;
	    if(steps<1) steps = 1;
	    len += steps;
	} else {
	    len++;
	}
	l = l->next;
    }
    //printf("full len:%d\n", len);
    xyz = malloc(sizeof(double)*3*len);
    l = line;
    len = 0;
    while(l) {
	if(l->type == gfx_moveTo) {
	    if(running) {
		running = 0;
		gluTessEndContour(tesselator);
	    }
	}
	if(!running) {
	    running = 1;
	    gluTessBeginContour(tesselator);
	}

	if(l->type == gfx_splineTo) {
	    int j;
            double c = sqrt(abs(l->x-2*l->sx+lastx) + abs(l->y-2*l->sy+lasty))*SPLINE_SUBDIVISION;
	    int steps = (int)c;
	    if(steps<1) steps = 1;
	    //printf("c=%f d1=%f (%f/%f) d2=%f (%f/%f)\n", c,d1,l->x-l->sx,l->y-l->sy,d2,lastx-l->sx,lasty-l->sy);
	    //printf("%f %f %f\n", lastx, l->sx, l->x);
	    //printf("%f %f %f\n", lasty, l->sy, l->y);
	    for(j=1;j<=steps;j++) {
		//printf("%d\n", j);
		double t = (double)j / (double)steps;
		xyz[len*3+0] = lastx*(1-t)*(1-t) + 2*l->sx*(1-t)*t + l->x*t*t;
		xyz[len*3+1] = lasty*(1-t)*(1-t) + 2*l->sy*(1-t)*t + l->y*t*t;
		xyz[len*3+2] = z;
		gluTessVertex(tesselator, &xyz[len*3], &xyz[len*3]);
		len++;
	    }
	    //printf("%d\n", len);
	} else {
	    xyz[len*3+0] = l->x;
	    xyz[len*3+1] = l->y;
	    xyz[len*3+2] = z;
	    gluTessVertex(tesselator, &xyz[len*3], &xyz[len*3]);
	    len++;
	}
	lastx = l->x;
	lasty = l->y;

	l=l->next;
    }
    if(running) {
	running = 0;
	gluTessEndContour(tesselator);
    }
    gluTessEndPolygon(tesselator);
    free(xyz);
}

void opengl_fill(struct _gfxdevice*dev, gfxline_t*line, gfxcolor_t*color)
{
    double z;
    dbg("fill %02x%02x%02x%02x", color->a, color->r, color->g, color->b);
    internal_t*i = (internal_t*)dev->internal;

    glDisable(GL_TEXTURE_2D);
    glColor4f(color->r/255.0, color->g/255.0, color->b/255.0, color->a/255.0);
    
    i->currentz ++;
    z = (i->currentz*ZSTEP);
    tesselatePolygon(i->tesselator, z, line);

    //tesselatePolygon(i->tesselator_line, z, line);
}

typedef struct _gfxhash
{
    unsigned char d[16];
} gfxhash_t;

char gfxhash_compare(gfxhash_t*h1, gfxhash_t*h2)
{
    return !memcmp(h1->d, h2->d, 16);
}

typedef struct _imgopengl
{
    gfxhash_t hash;
    GLuint texID;
    int width, height;
    unsigned char*data;
    struct _imgopengl*next;
} imgopengl_t;

static imgopengl_t*img2texid = 0;

static gfxhash_t gfximage_hash(gfximage_t*img)
{
    int t;
    int size = img->width*img->height*4;
    U8*data = (U8*)img->data;
    gfxhash_t hash;
    hash_md5(data, size, hash.d);
    return hash;
}

static void delTextures()
{
    imgopengl_t*i = img2texid;
    while(i) {
        imgopengl_t*next = i->next;
        if(i->data) {
            glDeleteTextures(1, &i->texID);
            free(i->data);
        }
        memset(i, 0, sizeof(imgopengl_t));
        free(i);
        i = next;
    }
}

static imgopengl_t*addTexture(gfximage_t*img)
{
    gfxhash_t hash = gfximage_hash(img);
    imgopengl_t*i = img2texid;
    
    int width_bits = 0;
    int height_bits = 0;
    while(1<<width_bits < img->width)
	width_bits++;
    while(1<<height_bits < img->height)
	height_bits++;
    int newwidth = 1<<width_bits;
    int newheight = 1<<height_bits;

    while(i) {
        if(gfxhash_compare(&hash, &i->hash) && newwidth==i->width && newheight==i->height) {
            return i;
        }
        i = i->next;
    }
    
    GLuint texIDs[1];
    glGenTextures(1, texIDs);

    i = malloc(sizeof(imgopengl_t));
    i->hash = hash;
    i->texID = texIDs[0];
    i->next = img2texid;
    img2texid = i;

    i->width = newwidth;
    i->height = newheight;

    unsigned char*data = malloc(newwidth*newheight*4);
    i->data = data;
    int x,y;
    for(y=0;y<img->height;y++) {
	for(x=0;x<img->width;x++) {
	    data[(y*newwidth+x)*4+0] = img->data[y*img->width+x].r;
	    data[(y*newwidth+x)*4+1] = img->data[y*img->width+x].g;
	    data[(y*newwidth+x)*4+2] = img->data[y*img->width+x].b;
	    data[(y*newwidth+x)*4+3] = img->data[y*img->width+x].a;
	}
        int lastx = img->width - 1;
        for(;x<newwidth;x++) {
	    data[(y*newwidth+x)*4+0] = img->data[y*img->width+lastx].r;
	    data[(y*newwidth+x)*4+1] = img->data[y*img->width+lastx].g;
	    data[(y*newwidth+x)*4+2] = img->data[y*img->width+lastx].b;
	    data[(y*newwidth+x)*4+3] = img->data[y*img->width+lastx].a;
        }
    }
    int lasty = img->height - 1;
    for(;y<newheight;y++) {
        for(x=0;x<newwidth;x++) {
	    data[(y*newwidth+x)*4+0] = img->data[lasty*img->width+x].r;
	    data[(y*newwidth+x)*4+1] = img->data[lasty*img->width+x].g;
	    data[(y*newwidth+x)*4+2] = img->data[lasty*img->width+x].b;
	    data[(y*newwidth+x)*4+3] = img->data[lasty*img->width+x].a;
        }
    }
    
    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, i->texID);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, i->width, i->height, 0, GL_RGBA, GL_UNSIGNED_BYTE, data);
    
    return i;
};

void opengl_fillbitmap(struct _gfxdevice*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
    dbg("fillbitmap");
    internal_t*i = (internal_t*)dev->internal;
    char running = 0;
    int len = 0;
    double*xyz=0;
    gfxline_t*l=0;
    glColor4f(1.0,0,0.7,1.0);
    
    i->currentz ++;
  
    imgopengl_t* txt = addTexture(img);

    gfxmatrix_t m2;
    gfxmatrix_invert(matrix, &m2);
    m2.m00 /= txt->width;
    m2.m10 /= txt->width;
    m2.tx /= txt->width;
    m2.m01 /= txt->height;
    m2.m11 /= txt->height;
    m2.ty /= txt->height;

    glEnable(GL_TEXTURE_2D);
    glBindTexture(GL_TEXTURE_2D, txt->texID);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_REPLACE);
    
    gluTessBeginPolygon(i->tesselator_tex, NULL);
    l = line;
    len = 0;
    while(l) {
	len++;
	l = l->next;
    }
    xyz = malloc(sizeof(double)*5*len);
    l = line;
    len = 0;
    while(l) {
	if(l->type == gfx_moveTo) {
	    if(running) {
		running = 0;
		gluTessEndContour(i->tesselator_tex);
	    }
	}
	if(!running) {
	    running = 1;
	    gluTessBeginContour(i->tesselator_tex);
	}

	xyz[len*5+0] = l->x;
	xyz[len*5+1] = l->y;
	xyz[len*5+2] = (i->currentz*ZSTEP);
	xyz[len*5+3] = 0;
	xyz[len*5+4] = 0;
	gfxmatrix_transform(&m2, /*src*/&xyz[len*5+0], /*dest*/&xyz[len*5+3]);

	gluTessVertex(i->tesselator_tex, &xyz[len*5], &xyz[len*5]);
	len++;

	l=l->next;
    }

    if(running) {
	running = 0;
	gluTessEndContour(i->tesselator_tex);
    }
    gluTessEndPolygon(i->tesselator_tex);
    free(xyz);
    
    glDisable(GL_TEXTURE_2D);
}

void opengl_fillgradient(struct _gfxdevice*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
    dbg("fillgradient");
}

void opengl_addfont(gfxdevice_t*dev, gfxfont_t*font)
{
    internal_t*i = (internal_t*)dev->internal;
    
    fontlist_t*last=0,*l = i->fontlist;
    while(l) {
	last = l;
	if(!strcmp((char*)l->font->id, font->id)) {
	    return; // we already know this font
	}
	l = l->next;
    }
    l = (fontlist_t*)rfx_calloc(sizeof(fontlist_t));
    l->font = font;
    l->next = 0;
    if(last) {
	last->next = l;
    } else {
	i->fontlist = l;
    }
}

void opengl_drawchar(gfxdevice_t*dev, gfxfont_t*font, int glyphnr, gfxcolor_t*color, gfxmatrix_t*matrix)
{
    internal_t*i = (internal_t*)dev->internal;
    if(!font)
	return;

    if(i->font && i->font->id && !strcmp(font->id, i->font->id)) {
	// current font is correct
    } else {
	fontlist_t*l = i->fontlist;
	i->font = 0;
	while(l) {
	    if(!strcmp((char*)l->font->id, font->id)) {
		i->font = l->font;
		break;
	    }
	    l = l->next;
	}
	if(i->font == 0) {
            opengl_addfont(dev, font);
            i->font = font;
	    //fprintf(stderr, "Unknown font id: %s", font->id);
	    //return;
	}
    }

    gfxglyph_t*glyph = &i->font->glyphs[glyphnr];
    
    gfxline_t*line2 = gfxline_clone(glyph->line);
    gfxline_transform(line2, matrix);
    opengl_fill(dev, line2, color);
    gfxline_free(line2);
    
    i->currentz --;
    
    return;
}



void opengl_drawlink(struct _gfxdevice*dev, gfxline_t*line, const char*action, const char*text)
{
    dbg("link");
}

void opengl_endpage(struct _gfxdevice*dev)
{
    dbg("endpage");
}

int opengl_result_save(struct _gfxresult*gfx, const char*filename)
{
    dbg("result:save");
    return 0;
}
void* opengl_result_get(struct _gfxresult*gfx, const char*name)
{
    dbg("result:get");
    return 0;
}
void opengl_result_destroy(struct _gfxresult*gfx)
{
    dbg("result:destroy");
    free(gfx);
    delTextures();
}

gfxresult_t*opengl_finish(struct _gfxdevice*dev)
{
    dbg("finish");
    internal_t*i = (internal_t*)dev->internal;
    gluDeleteTess(i->tesselator);i->tesselator=0;
    gluDeleteTess(i->tesselator_tex);i->tesselator_tex=0;
    gfxresult_t*result = (gfxresult_t*)malloc(sizeof(gfxresult_t));
    memset(result, 0, sizeof(gfxresult_t));
    result->save = opengl_result_save;
    result->get = opengl_result_get;
    result->destroy = opengl_result_destroy;
    return result;
}

void gfxdevice_opengl_init(gfxdevice_t*dev)
{
    dbg("init");
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    memset(dev, 0, sizeof(gfxdevice_t));
    
    dev->name = "opengl";

    dev->internal = i;
    
    dev->setparameter = opengl_setparameter;
    dev->startpage = opengl_startpage;
    dev->startclip = opengl_startclip;
    dev->endclip = opengl_endclip;
    dev->stroke = opengl_stroke;
    dev->fill = opengl_fill;
    dev->fillbitmap = opengl_fillbitmap;
    dev->fillgradient = opengl_fillgradient;
    dev->addfont = opengl_addfont;
    dev->drawchar = opengl_drawchar;
    dev->drawlink = opengl_drawlink;
    dev->endpage = opengl_endpage;
    dev->finish = opengl_finish;

    i->tesselator = gluNewTess();
    gluTessCallback(i->tesselator, GLU_TESS_ERROR, (callbackfunction_t)errorCallback);
    gluTessCallback(i->tesselator, GLU_TESS_VERTEX, (callbackfunction_t)vertexCallback);
    gluTessCallback(i->tesselator, GLU_TESS_BEGIN, (callbackfunction_t)beginCallback);
    gluTessCallback(i->tesselator, GLU_TESS_END, (callbackfunction_t)endCallback);
    gluTessCallback(i->tesselator, GLU_TESS_COMBINE, (callbackfunction_t)combineCallback);
    
    i->tesselator_line = gluNewTess();
    gluTessCallback(i->tesselator_line, GLU_TESS_ERROR, (callbackfunction_t)errorCallback);
    gluTessCallback(i->tesselator_line, GLU_TESS_VERTEX, (callbackfunction_t)vertexCallback);
    gluTessCallback(i->tesselator_line, GLU_TESS_BEGIN, (callbackfunction_t)beginCallback);
    gluTessCallback(i->tesselator_line, GLU_TESS_END, (callbackfunction_t)endCallback);
    gluTessProperty(i->tesselator_line, GLU_TESS_BOUNDARY_ONLY, 1.0);
    
    i->tesselator_tex = gluNewTess();
    gluTessCallback(i->tesselator_tex, GLU_TESS_ERROR, (callbackfunction_t)errorCallback);
    gluTessCallback(i->tesselator_tex, GLU_TESS_VERTEX, (callbackfunction_t)vertexCallbackTex);
    gluTessCallback(i->tesselator_tex, GLU_TESS_BEGIN, (callbackfunction_t)beginCallback);
    gluTessCallback(i->tesselator_tex, GLU_TESS_END, (callbackfunction_t)endCallback);
    gluTessCallback(i->tesselator_tex, GLU_TESS_COMBINE, (callbackfunction_t)combineCallbackTex);
    
    glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
}
