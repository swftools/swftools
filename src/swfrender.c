#include "../config.h"
#include <stdio.h>
#include <stdarg.h>
#include <assert.h>
#include <unistd.h>
#include <fcntl.h>
#include "../lib/rfxswf.h"

// swfrender.c

typedef struct RENDERBUF 
{
    int width;
    int height;
    void*internal;
} RENDERBUF;

typedef struct heap_t
{
};

typedef struct _renderbuf_internal
{
    heap_t* lines;
} renderbuf_internal;


void swf_RenderShape(RENDERBUF*dest, SHAPE2*shape)
{
    renderbuf_internal*i = (renderbuf_internal)dest->internal;

    SHAPELINE*line = shape->lines;
    int x=0,y=0;

    while(line)
    {
        if(line->type == moveTo) {
            /* max 31 bits + sign */
            x = line->x;
            y = line->y;
        } else if(line->type == lineTo) {
            /* max 17 bits + sign */
            int diffx = line->x - x;
            int diffy = line->y - y;
            int stepx,posx=0,posy=0;
            if(diffy<0) {
                x = line->x;
                y = line->y;
                diffx = -diffx;
                diffy = -diffy;
            }
            stepx = (diffx*0x1000) / diffy;

            while(posy<diffy) {
                int xx = x + ((posx)/0x1000);
                int yy = posy;
                // TODO: add pixel to dest
                posx+=stepx;
                posy++;
            }

            x = line->x;
            y = line->y;
        } else if(line->type == splineTo) {
        }

        line = line->next;
    }
}

int main()
{
    char*filename = "output.swf";
    fi = open(filename, O_RDONLY|O_BINARY);
    if (fi<=0) { 
	fprintf(stderr,"Couldn't open %s\n", filename);
        perror(argv[1]);
        exit(1);
    }

    if(swf_ReadSWF(fi,&swf)<0) { 
	fprintf(stderr,"%s is not a valid SWF file or contains errors.\n",argv[1]);
	close(fi);
    }

    tag = swf.firstTag;
    while(tag) {
        if(tag.id == ST_DEFINESHAPE3) {
            SHAPE2 shape;
            swf_ParseDefineShape(tag, &shape);
            swf_RenderShape(tag, MATRIX m);
        }
    }
}
