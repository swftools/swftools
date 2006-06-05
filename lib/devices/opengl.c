#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "../gfxdevice.h"
#include "../gfxtools.h"

#include <time.h>
#include <GL/gl.h>
#include <GL/glut.h>

#include <setjmp.h>

typedef struct _fontlist {
    gfxfont_t*font;
    char*id;
    struct _fontlist*next;
} fontlist_t;

typedef struct _internal {
    gfxfont_t*font;
    char*fontid;
    fontlist_t* fontlist;
    int width, height;
} internal_t;

int opengl_setparameter(struct _gfxdevice*dev, const char*key, const char*value)
{
    return 0;
}

void opengl_startpage(struct _gfxdevice*dev, int width, int height)
{
    internal_t*i = (internal_t*)dev->internal;
    i->width = width;
    i->height = height;
}

void opengl_startclip(struct _gfxdevice*dev, gfxline_t*line)
{
    int t;
    for(t=1;t<100;t++) {
	glColor3f(1.0,1.0,1.0);
	glBegin(GL_POLYGON);
	    glVertex3f(-t*10,-t*10,0);
	    glVertex3f(-t*10,t*10,0);
	    glVertex3f(t*10,t*10,0);
	    glVertex3f(t*10,-t*10,0);
	glEnd();
    }
    //glRectf(-50,-50,0.0,0.0);
}

void opengl_endclip(struct _gfxdevice*dev)
{
}

void opengl_stroke(struct _gfxdevice*dev, gfxline_t*line, gfxcoord_t width, gfxcolor_t*color, gfx_capType cap_style, gfx_joinType joint_style, gfxcoord_t miterLimit)
{
}

void opengl_fill(struct _gfxdevice*dev, gfxline_t*line, gfxcolor_t*color)
{
}

void opengl_fillbitmap(struct _gfxdevice*dev, gfxline_t*line, gfximage_t*img, gfxmatrix_t*matrix, gfxcxform_t*cxform)
{
}

void opengl_fillgradient(struct _gfxdevice*dev, gfxline_t*line, gfxgradient_t*gradient, gfxgradienttype_t type, gfxmatrix_t*matrix)
{
}

void opengl_addfont(struct _gfxdevice*dev, char*fontid, gfxfont_t*font)
{
}

void opengl_drawchar(struct _gfxdevice*dev, char*fontid, int glyph, gfxcolor_t*color, gfxmatrix_t*matrix)
{
}

void opengl_drawlink(struct _gfxdevice*dev, gfxline_t*line, char*action)
{
}

void opengl_endpage(struct _gfxdevice*dev)
{
}


int opengl_result_save(struct _gfxresult*gfx, char*filename)
{
    return 0;
}
void* opengl_result_get(struct _gfxresult*gfx, char*name)
{
    return 0;
}
void opengl_result_destroy(struct _gfxresult*gfx)
{
    free(gfx);
}

gfxresult_t*opengl_finish(struct _gfxdevice*dev)
{
    internal_t*i = (internal_t*)dev->internal;
    gfxresult_t*result = (gfxresult_t*)malloc(sizeof(gfxresult_t));
    memset(result, 0, sizeof(gfxresult_t));
    result->save = opengl_result_save;
    result->get = opengl_result_get;
    result->destroy = opengl_result_destroy;
    return result;
}

void gfxdevice_opengl_init(gfxdevice_t*dev)
{
    internal_t*i = (internal_t*)rfx_calloc(sizeof(internal_t));
    memset(dev, 0, sizeof(gfxdevice_t));

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
}
