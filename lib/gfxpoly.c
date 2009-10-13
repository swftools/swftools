/* gfxpoly.c 

   Various boolean polygon functions.

   Part of the swftools package.

   Copyright (c) 2005 Matthias Kramm <kramm@quiss.org> 

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

#include "../config.h"
#include "gfxdevice.h"
#include "gfxtools.h"
#include "gfxpoly.h"
#include "mem.h"
#include "art/libart.h"
#include "art/art_svp_intersect.h"
#include "art/art_svp_ops.h"
#include "log.h"
#include <assert.h>
#include <memory.h>
#include <math.h>

#define PERTURBATE
//#define SHEAR
//#define DEBUG

//----------------------------------------- svp renderer ----------------------------------------

typedef struct {
    int xmin;
    int ymin;
    int xmax;
    int ymax;
    int width;
    int height;
} intbbox_t;

typedef struct _renderpoint
{
    double x;
    signed char direction;
} renderpoint_t;

typedef struct _renderline
{
    renderpoint_t*points;
    int size;
    int num;
} renderline_t;

typedef struct _renderbuf
{
    intbbox_t bbox;
    int width;
    int height;
    double zoom;
    renderline_t*lines;
} renderbuf_t;

static inline void add_pixel(renderbuf_t*buf, double x, int y, signed char direction)
{
    renderpoint_t p;
    p.x = x;
    p.direction = direction;

    if(x >= buf->bbox.xmax || y >= buf->bbox.ymax || y < buf->bbox.ymin) 
        return;
    renderline_t*l = &buf->lines[y-buf->bbox.ymin];

    if(l->num == l->size) {
	l->size += 32;
	l->points = (renderpoint_t*)rfx_realloc(l->points, l->size * sizeof(renderpoint_t));
    }
    l->points[l->num] = p;
    l->num++;
}
#define CUT 0.5
#define INT(x) ((int)((x)+16)-16)
static void add_line(renderbuf_t*buf, double x1, double y1, double x2, double y2, signed char direction)
{
    x1 *= buf->zoom;
    y1 *= buf->zoom;
    x2 *= buf->zoom;
    y2 *= buf->zoom;
    double diffx, diffy;
    double ny1, ny2, stepx;
    if(y2 < y1) {
        double x,y;
	x = x1;x1 = x2;x2=x;
	y = y1;y1 = y2;y2=y;
    }
    diffx = x2 - x1;
    diffy = y2 - y1;
    ny1 = INT(y1)+CUT;
    ny2 = INT(y2)+CUT;

    if(ny1 < y1) {
        ny1 = INT(y1) + 1.0 + CUT;
    }
    if(ny2 >= y2) {
        ny2 = INT(y2) - 1.0 + CUT;
    }
    if(ny1 > ny2)
        return;

    stepx = diffx/diffy;
    x1 = x1 + (ny1-y1)*stepx;
    x2 = x2 + (ny2-y2)*stepx;

    int posy=INT(ny1);
    int endy=INT(ny2);
    double posx=0;
    double startx = x1;

    while(posy<=endy) {
        double xx = startx + posx;
        add_pixel(buf, xx, posy, direction);
        posx+=stepx;
        posy++;
    }
}

static int compare_renderpoints(const void * _a, const void * _b)
{
    renderpoint_t*a = (renderpoint_t*)_a;
    renderpoint_t*b = (renderpoint_t*)_b;
    if(a->x < b->x) return -1;
    if(a->x > b->x) return 1;
    return 0;
}

static void fill_bitwise(unsigned char*line, int x1, int x2)
{
    int p1 = x1>>3;
    int p2 = x2>>3;
    int b1 = 0xff >> (x1&7);
    int b2 = 0xff << (1+7-(x2&7));
    if(p1==p2) {
        line[p1] |= b1&b2;
    } else {
        line[p1] |= b1;
        memset(&line[p1+1], 255, p2-(p1+1));
        line[p2] = b2;
    }
}

unsigned char* render_svp(ArtSVP*svp, intbbox_t*bbox, double zoom, ArtWindRule rule)
{
    renderbuf_t _buf, *buf=&_buf;
    buf->width = (bbox->xmax - bbox->xmin);
    buf->height = (bbox->ymax - bbox->ymin);
    buf->bbox = *bbox;
    buf->zoom = zoom;
    int width8 = (buf->width+7) >> 3;
    unsigned char* image = (unsigned char*)malloc(width8*buf->height);
    memset(image, 0, width8*buf->height);
    
    buf->lines = (renderline_t*)rfx_alloc(buf->height*sizeof(renderline_t));
    int y;
    for(y=0;y<buf->height;y++) {
	memset(&buf->lines[y], 0, sizeof(renderline_t));
        buf->lines[y].points = 0;
        buf->lines[y].num = 0;
    }

    int t;
    for(t=0;t<svp->n_segs;t++) {
        ArtSVPSeg* seg = &svp->segs[t];
        int s;
        for(s=0;s<seg->n_points-1;s++) {
            int dir = seg->dir? 1 : -1;
            add_line(buf, seg->points[s].x, seg->points[s].y, seg->points[s+1].x, seg->points[s+1].y, dir);
        }
    }
    for(y=0;y<buf->height;y++) {
	renderpoint_t*points = buf->lines[y].points;
        unsigned char*line = &image[width8*y];
	int n;
	int num = buf->lines[y].num;
        int wind = 0;
        qsort(points, num, sizeof(renderpoint_t), compare_renderpoints);
        int lastx = 0;
        int fill = 0;

        for(n=0;n<num;n++) {
            renderpoint_t*p = &points[n];
            int x = (int)(p->x - bbox->xmin);
            
            if(x < lastx)
                x = lastx;
            if(x > buf->width) {
                break;
            }
            if(fill && x!=lastx) {
                fill_bitwise(line, lastx, x);
            }
            wind += p->direction;
            if(rule == ART_WIND_RULE_INTERSECT) {
                fill = wind>=2;
            } else if (rule == ART_WIND_RULE_NONZERO) {
                fill = wind!=0;
            } else if (rule == ART_WIND_RULE_ODDEVEN) {
                fill = wind&1;
            } else { // rule == ART_WIND_RULE_POSITIVE
                fill = wind>=1;
            }
            lastx = x;
        }
        if(fill && lastx!=buf->width)
            fill_bitwise(line, lastx, buf->width);
    }
    
    for(y=0;y<buf->height;y++) {
        if(buf->lines[y].points) {
            free(buf->lines[y].points);
        }
        memset(&buf->lines[y], 0, sizeof(renderline_t));
    }
    free(buf->lines);buf->lines=0;
    return image;
}

#define MAX_WIDTH 8192
#define MAX_HEIGHT 4096

intbbox_t get_svp_bbox(ArtSVP*svp, double zoom)
{
    int t;
    intbbox_t b = {0,0,0,0};
    if(svp->n_segs && svp->segs[0].n_points) {
        b.xmin = svp->segs[0].points[0].x;
        b.ymin = svp->segs[0].points[0].y;
        b.xmax = svp->segs[0].points[0].x;
        b.ymax = svp->segs[0].points[0].y;
    }
    for(t=0;t<svp->n_segs;t++) {
        ArtSVPSeg* seg = &svp->segs[t];
        int s;
        for(s=0;s<seg->n_points;s++) {
            double x = seg->points[s].x*zoom;
            double y = seg->points[s].y*zoom;
            int x1 = floor(x);
            int x2 = ceil(x);
            int y1 = floor(y);
            int y2 = ceil(y);
            if(x1 < b.xmin) b.xmin = x1;
            if(y1 < b.ymin) b.ymin = y1;
            if(x2 > b.xmax) b.xmax = x2;
            if(y2 > b.ymax) b.ymax = y2;
        }
    }
    if(b.xmax > (int)(MAX_WIDTH*zoom))
	b.xmax = (int)(MAX_WIDTH*zoom);
    if(b.ymax > (int)(MAX_HEIGHT*zoom))
	b.ymax = (int)(MAX_HEIGHT*zoom);
    if(b.xmin < -(int)(MAX_WIDTH*zoom))
	b.xmin = -(int)(MAX_WIDTH*zoom);
    if(b.ymin < -(int)(MAX_HEIGHT*zoom))
	b.ymin = -(int)(MAX_HEIGHT*zoom);
    
    if(b.xmin > b.xmax) 
	b.xmin = b.xmax;
    if(b.ymin > b.ymax) 
	b.ymin = b.ymax;

    b.width = b.xmax - b.xmin;
    b.height = b.ymax - b.ymin;
    return b;
}

#define B11100000 0xe0
#define B01110000 0x70
#define B00111000 0x38
#define B00011100 0x1c
#define B00001110 0x0e
#define B00000111 0x07
#define B10000000 0x80
#define B01000000 0x40
#define B00100000 0x20
#define B00010000 0x10
#define B00001000 0x08
#define B00000100 0x04
#define B00000010 0x02
#define B00000001 0x01

static int compare_bitmaps(intbbox_t*bbox, unsigned char*data1, unsigned char*data2)
{
    if(!data1 || !data2) 
        return 0;
    int x,y;
    int height = bbox->height;
    int width = bbox->width;
    int width8 = (width+7) >> 3;
    unsigned char*l1 = &data1[width8];
    unsigned char*l2 = &data2[width8];
    for(y=1;y<height-1;y++) {
        for(x=0;x<width8;x++) {
            unsigned a = l1[x-width8] & l1[x] & l1[x+width8];
            unsigned b = l2[x-width8] & l2[x] & l2[x+width8];
            
            if((a&B11100000) && !(l2[x]&B01000000)) return 0;
            if((a&B01110000) && !(l2[x]&B00100000)) return 0;
            if((a&B00111000) && !(l2[x]&B00010000)) return 0;
            if((a&B00011100) && !(l2[x]&B00001000)) return 0;
            if((a&B00001110) && !(l2[x]&B00000100)) return 0;
            if((a&B00000111) && !(l2[x]&B00000010)) return 0;

            if((b&B11100000) && !(l1[x]&B01000000)) return 0;
            if((b&B01110000) && !(l1[x]&B00100000)) return 0;
            if((b&B00111000) && !(l1[x]&B00010000)) return 0;
            if((b&B00011100) && !(l1[x]&B00001000)) return 0;
            if((b&B00001110) && !(l1[x]&B00000100)) return 0;
            if((b&B00000111) && !(l1[x]&B00000010)) return 0;
        }
        l1 += width8;
        l2 += width8;
    }
    return 1;
}


//-----------------------------------------------------------------------------------------------

static ArtVpath* gfxline_to_ArtVpath(gfxline_t*line, char fill)
{
    ArtVpath *vec = NULL;
    int pos=0,len=0;
    gfxline_t*l2;
    double x=0,y=0;

    /* factor which determines into how many line fragments a spline is converted */
    double subfraction = 2.4;//0.3

    l2 = line;
    while(l2) {
	if(l2->type == gfx_moveTo) {
	    pos ++;
	} else if(l2->type == gfx_lineTo) {
	    pos ++;
	} else if(l2->type == gfx_splineTo) {
            int parts = (int)(sqrt(fabs(l2->x-2*l2->sx+x) + fabs(l2->y-2*l2->sy+y))*subfraction);
            if(!parts) parts = 1;
            pos += parts + 1;
	}
	x = l2->x;
	y = l2->y;
	l2 = l2->next;
    }
    pos++;
    len = pos;

    vec = art_new (ArtVpath, len+1);

    pos = 0;
    l2 = line;
    int lastmove=-1;
    while(l2) {
	if(l2->type == gfx_moveTo) {
	    vec[pos].code = ART_MOVETO_OPEN;
	    vec[pos].x = l2->x;
	    vec[pos].y = l2->y;
            lastmove=pos;
	    pos++; 
	    assert(pos<=len);
	} else if(l2->type == gfx_lineTo) {
	    vec[pos].code = ART_LINETO;
	    vec[pos].x = l2->x;
	    vec[pos].y = l2->y;
	    pos++; 
	    assert(pos<=len);
	} else if(l2->type == gfx_splineTo) {
	    int i;
            int parts = (int)(sqrt(fabs(l2->x-2*l2->sx+x) + fabs(l2->y-2*l2->sy+y))*subfraction);
            if(!parts) parts = 1;
	    double stepsize = 1.0/parts;
	    for(i=0;i<=parts;i++) {
		double t = (double)i*stepsize;
		vec[pos].code = ART_LINETO;
		vec[pos].x = l2->x*t*t + 2*l2->sx*t*(1-t) + x*(1-t)*(1-t);
		vec[pos].y = l2->y*t*t + 2*l2->sy*t*(1-t) + y*(1-t)*(1-t);
		pos++;
		assert(pos<=len);
	    }
	}
	x = l2->x;
	y = l2->y;
       
        /* let closed line segments start w/ MOVETO instead of MOVETO_OPEN */
        if(lastmove>=0 && l2->type!=gfx_moveTo && (!l2->next || l2->next->type == gfx_moveTo)) {
            if(vec[lastmove].x == l2->x &&
               vec[lastmove].y == l2->y) {
                assert(vec[lastmove].code == ART_MOVETO_OPEN);
                vec[lastmove].code = ART_MOVETO;
            }
        }

	l2 = l2->next;
    }
    vec[pos++].code = ART_END;
    assert(pos == len);

    if(!fill) {
	/* Fix "dotted" lines. Those are lines where singular points are created
	   by a moveto x,y lineto x,y combination. We "fix" these by shifting the
	   point in the lineto a little bit to the right 
	   These should only occur in strokes, not in fills, so do this only
	   when we know we're not filling.
	 */
	int t;
	for(t=0;vec[t].code!=ART_END;t++) {
	    if(t>0 && (vec[t-1].code==ART_MOVETO_OPEN || vec[t-1].code==ART_MOVETO) 
                   && vec[t].code==ART_LINETO && vec[t+1].code!=ART_LINETO &&
		vec[t-1].x == vec[t].x && 
                vec[t-1].y == vec[t].y) {
		vec[t].x += 0.01;
	    }
	}
    }

    /* Find adjacent identical points. If an ajdacent pair of identical
       points is found, the second one is removed.
       So moveto x,y lineto x,y becomes moveto x,y
          lineto x,y lineto x,y becomes lineto x,y
          lineto x,y moveto x,y becomes lineto x,y
          moveto x,y moveto x,y becomes moveto x,y
          lineto x,y lineto x2,y2 becomes lineto x2,y2 (if dir(x,y) ~= dir(x2,y2))
     */
    pos = 0;
    int outpos = 0;
    while(1)
    {
        if(vec[pos].code == ART_END) {
            vec[outpos++] = vec[pos++];
            break;
        }

        char samedir = 0, samepoint = 0;
        if(outpos) {
            double dx = vec[pos].x-vec[pos-1].x;
            double dy = vec[pos].y-vec[pos-1].y;
            /*if(pos<len-1 && vec[pos].code == ART_LINETO && vec[pos+1].code == ART_LINETO) {
                double dx2 = vec[pos+1].x-vec[pos].x;
                double dy2 = vec[pos+1].y-vec[pos].y;
                if(fabs(dx*dy2 - dy*dx2) < 0.0001 && dx*dx2 + dy*dy2 >= 0) {
                    samedir=1;
                }
            }*/
            if(fabs(dx) + fabs(dy) < 0.0001) {
                samepoint=1;
            }
        }
        if(!samepoint && !samedir) {
            vec[outpos++] = vec[pos++];
        } else {
            pos++; // skip
        }
    }

    return vec;
}

static void shear_svp(ArtSVP*svp, double v)
{
    /* do a "shearing" on the polygon. We do this to eliminate all
       horizontal lines (which libart can't handle properly, even though
       it tries). */

    int t;
    for(t=0;t<svp->n_segs;t++) {
        ArtSVPSeg* seg = &svp->segs[t];
        int s;
        for(s=0;s<seg->n_points;s++) {
            ArtPoint* point = &seg->points[s];
            point->y += point->x*v;
        }
    }
}

static double find_shear_value(ArtSVP*svp)
{
    /* We try random values until we find one
       where there's no slope below a given value, or if that fails,
       at least no slope of 0 */

    double v = 0;
    int tries = 0;
    while(1) {
        char fail = 0;
        int t;
        for(t=0;t<svp->n_segs;t++) {
            ArtSVPSeg* seg = &svp->segs[t];
            int s;
            for(s=0;s<seg->n_points-1;s++) {
                ArtPoint* p1 = &seg->points[s];
                ArtPoint* p2 = &seg->points[s+1];
                double dx = p2->x - p1->x;
                double dy = p2->y - p1->y;
                dy += dx*v;
                if(dy==0) {
                    fail = 1;
                    break;
                }
                if(tries<100 && dx && fabs(dy / dx) < 1e-5) {
                    fail = 1;
                    break;
                }
            }
            if(fail) 
                break;
        }
        if(!fail) 
            break;
#ifdef HAVE_LRAND48
	v = lrand48() / 2000000000.0;;
#elif HAVE_RAND
        v = rand() / 2000000000.0;
#else
#error "no lrand48()/rand() implementation found"
#endif
        tries++;
    }
    return v;
}

void show_path(ArtSVP*path)
{
    int t;
    printf("Segments: %d\n", path->n_segs);
    for(t=0;t<path->n_segs;t++) {
	ArtSVPSeg* seg = &path->segs[t];
	printf("Segment %d: %d points, %s, BBox: (%f,%f,%f,%f)\n", 
		t, seg->n_points, seg->dir==0?"UP  ":"DOWN",
		seg->bbox.x0, seg->bbox.y0, seg->bbox.x1, seg->bbox.y1);
	int p;
	for(p=0;p<seg->n_points;p++) {
	    ArtPoint* point = &seg->points[p];
	    printf("        (%f,%f)\n", point->x, point->y);
	}
    }
    printf("\n");
}


typedef struct svp_segment_part
{
    double y1;
    double y2;
    char active;
} svp_segment_part_t;

int compare_double(const void*_y1, const void*_y2)
{
    const double*y1 = _y1;
    const double*y2 = _y2;
    if(*y1<*y2) return -1;
    if(*y1>*y2) return 1;
    else return 0;
}

int compare_seg_start(const void*_s1, const void*_s2)
{
    svp_segment_part_t*s1 = *(svp_segment_part_t**)_s1;
    svp_segment_part_t*s2 = *(svp_segment_part_t**)_s2;
    if(s1->y1 < s2->y1) return -1;
    if(s1->y1 > s2->y1) return 1;
    else return 0;
}

int compare_seg_end(const void*_s1, const void*_s2)
{
    svp_segment_part_t*s1 = *(svp_segment_part_t**)_s1;
    svp_segment_part_t*s2 = *(svp_segment_part_t**)_s2;
    if(s1->y2 < s2->y2) return -1;
    if(s1->y2 > s2->y2) return 1;
    else return 0;
}

void clean_svp(ArtSVP*svp)
{
    int t;
    int oldpoints = 0;
    int newpoints = 0;
    int oldsegs = 0;
    int newsegs = 0;
    for(t=0;t<svp->n_segs;t++) {
	ArtSVPSeg* seg = &svp->segs[t];
	int p;
	int pos=0;
	double lasty = 0;
	oldpoints += seg->n_points;
	for(p=0;p<seg->n_points;p++) {
	    ArtPoint* p1 = &seg->points[p];
	    if(!p || lasty!=p1->y) {
		seg->points[pos] = seg->points[p];
		pos++;
		lasty = p1->y;
            }
	}
	seg->n_points = pos;
	newpoints += seg->n_points;
    }
    int pos = 0;
    oldsegs = svp->n_segs;
    for(t=0;t<svp->n_segs;t++) {
	if(svp->segs[t].n_points > 1) {
	    svp->segs[pos++] = svp->segs[t];
	}
    }
    svp->n_segs = pos;
    newsegs = svp->n_segs;
    if(newsegs < oldsegs || newpoints < oldpoints) {
	msg("<verbose> Simplified polygon from %d points to %d points, %d to %d segs",
		oldpoints, newpoints, oldsegs, newsegs);
    }
}

int check_svp(ArtSVP*svp)
{
    /* count number of y coordinates and segs */
    int t;
    int num_points = 0;
    int num_segs = 0;
    for(t=0;t<svp->n_segs;t++) {
	if(!svp->segs[t].n_points) {
            msg("<error> svp contains segment with zero points\n");
	    return 0;
	}
	num_points += svp->segs[t].n_points;
	num_segs += svp->segs[t].n_points - 1;
    }

    /* create segs and ys */
    double*y = malloc(sizeof(double)*num_points);
    svp_segment_part_t*segs = malloc(sizeof(svp_segment_part_t)*num_segs);
    svp_segment_part_t**seg_start = malloc(sizeof(svp_segment_part_t*)*num_segs);
    svp_segment_part_t**seg_end = malloc(sizeof(svp_segment_part_t*)*num_segs);
    
    int c1=0;
    num_segs = 0;
    for(t=0;t<svp->n_segs;t++) {
	ArtSVPSeg* seg = &svp->segs[t];
	int p;
	for(p=0;p<seg->n_points;p++) {
	    y[c1++] = seg->points[p].y;
	    assert(c1 <= num_points);
	}
	for(p=0;p<seg->n_points-1;p++) {
	    ArtPoint* p1 = &seg->points[p];
	    ArtPoint* p2 = &seg->points[p+1];
            
	    if(p1->y >= p2->y) {
                if(p1->y > p2->y) {
                    msg("<error> bad svp, y in seg is non-increasing %.16f -> %.16f\n", p1->y, p2->y);
                }
            } else {
		segs[num_segs].y1 = p1->y;
		segs[num_segs].y2 = p2->y;
		segs[num_segs].active = 0;
		seg_start[num_segs] = &segs[num_segs];
		seg_end[num_segs] = &segs[num_segs];
		num_segs++;
	    }
	}
    }

    qsort(y, num_points, sizeof(double), compare_double);
    qsort(seg_start, num_segs, sizeof(svp_segment_part_t*), compare_seg_start);
    qsort(seg_end, num_segs, sizeof(svp_segment_part_t*), compare_seg_end);

    double lasty = num_points?y[0]+1:0;
    int num_active = 0;
    int bleed = 0;
    double bleedy1=0,bleedy2 = 0;
    for(t=0;t<num_points;t++) {
	if(lasty == y[t])
	    continue;
	int s;
	for(s=0;s<num_segs;s++) {
	    if(segs[s].y1==y[t]) {
		/* segment is starting */
		segs[s].active = 1;
		num_active++;
	    } else if(segs[s].y2==y[t]) {
		segs[s].active = 0;
		num_active--;
	    }
	}
	if(num_active&1) {
	    bleed = num_active;
	    bleedy1 = y[t];
	} else {
	    if(bleed) {
		bleedy2 = y[t];
		break;
	    }
	}
	lasty = y[t];
    }
    if(bleed) {
	msg("<verbose> svp bleeds from y=%.16f to y=%.16f (%d/%d active segments)\n", 
		bleedy1, bleedy2,
		bleed, num_segs);
	free(y);free(segs);free(seg_start);free(seg_end);
	return 0;
    }

    free(y);
    free(segs);
    free(seg_start);
    free(seg_end);

    return 1;
}

void write_svp_postscript(const char*filename, ArtSVP*svp)
{
    if(!svp)
	return;
    FILE*fi = fopen(filename, "wb");
    int i, j;
    double xmin=0,ymin=0,xmax=0,ymax=0;
    fprintf(fi, "%% begin\n");
    for (i = 0; i < svp->n_segs; i++) {
        for (j = 0; j < svp->segs[i].n_points; j++) {
            double x = svp->segs[i].points[j].x;
            double y = svp->segs[i].points[j].y;
            if(i==0 && j==0) {
                xmin = xmax = x;
                ymin = ymax = y;
            } else {
                if(x < xmin) xmin = x;
                if(x > xmax) xmax = x;
                if(y < ymin) ymin = y;
                if(y > ymax) ymax = y;
            }
        }
    }
    if(xmax == xmin) xmax=xmin+1;
    if(ymax == ymin) ymax=ymin+1;

    for (i = 0; i < svp->n_segs; i++)
      {
        fprintf(fi, "%g setgray\n", svp->segs[i].dir ? 0.7 : 0);
        for (j = 0; j < svp->segs[i].n_points; j++)
          {
            //fprintf(fi, "%g %g %s\n",
            //        20 + 550*(svp->segs[i].points[j].x-xmin)/(xmax-xmin),
            //        820 - 800*(svp->segs[i].points[j].y-ymin)/(ymax-ymin),
            //        j ? "lineto" : "moveto");
            fprintf(fi, "%.32f %.32f %s\n",
                    svp->segs[i].points[j].x,
                    svp->segs[i].points[j].y,
                    j ? "lineto" : "moveto");
          }
        fprintf(fi, "stroke\n");
      }

    fprintf(fi, "showpage\n");
    fclose(fi);
}

void write_vpath_postscript(const char*filename, ArtVpath*path)
{
    FILE*fi = fopen(filename, "wb");
    int i, j;
    double xmin=0,ymin=0,xmax=0,ymax=0;
    fprintf(fi, "%% begin\n");
    ArtVpath*p = path;
    char first = 1;
    while(p->code != ART_END) {
        if(p->code == ART_MOVETO || p->code == ART_MOVETO_OPEN) {
            if(!first)
                fprintf(fi, "stroke\n");
            first = 0;
            fprintf(fi, "0.0 setgray\n");
            fprintf(fi, "%.32f %.32f moveto\n", p->x, p->y);
        } else {
            fprintf(fi, "%.32f %.32f lineto\n", p->x, p->y);
        }
        p++;
    }
    if(!first)
        fprintf(fi, "stroke\n");
    fprintf(fi, "showpage\n");
    fclose(fi);
}

void write_gfxline_postscript(const char*filename, gfxline_t*line)
{
    FILE*fi = fopen(filename, "wb");
    int i, j;
    fprintf(fi, "%% begin\n");
    char first = 1;
    while(line) {
        if(line->type == gfx_moveTo) {
            if(!first)
                fprintf(fi, "stroke\n");
            first = 0;
            fprintf(fi, "0.0 setgray\n");
            fprintf(fi, "%.32f %.32f moveto\n", line->x, line->y);
        } else {
            fprintf(fi, "%.32f %.32f lineto\n", line->x, line->y);
        }
        line = line->next;
    }
    if(!first)
        fprintf(fi, "stroke\n");
    fprintf(fi, "showpage\n");
    fclose(fi);
}

static int vpath_len(ArtVpath*svp)
{
    int len = 0;
    while(svp->code != ART_END) {
        svp ++;
        len ++;
    }
    return len;
}

int gfxline_len(gfxline_t*line)
{
    gfxline_t*i = line;
    int len = 0;
    while(i) {
        len ++;
        i = i->next;
    }
    return len;
}

static ArtVpath*pvpath= 0;
static int cmppos(const void*_p1, const void*_p2)
{
    int*p1 = (int*)_p1;
    int*p2 = (int*)_p2;
    ArtVpath*v1 = &pvpath[*p1]; 
    ArtVpath*v2 = &pvpath[*p2]; 
    if(v1->y < v2->y)
        return -1;
    else if(v1->y > v2->y)
        return 1;
    else if(v1->x < v2->x)
        return -2;
    else if(v1->x > v2->x)
        return 2;
    else 
        return 0;
}

#define PERTURBATION 2e-3
static void my_perturb(ArtVpath*vpath)
{
    int t;
    int len = vpath_len(vpath);
    int*pos = (int*)malloc(len*sizeof(int));
    for(t=0;t<len;t++)
        pos[t] = t;
    pvpath = vpath;
    qsort(pos, len, sizeof(int), cmppos);
    t=0;
    while(t<len) {
        int t2=t+1;
        while(t2<len && !cmppos(&pos[t],&pos[t2])) {
            t2++;
        }

        double dx = (PERTURBATION * rand ()) / RAND_MAX - PERTURBATION * 0.5;
        double dy = (PERTURBATION * rand ()) / RAND_MAX - PERTURBATION * 0.5;
        int s;
        for(s=t;s<t2;s++) {
            vpath[pos[s]].x += dx;
            vpath[pos[s]].y += dy;
        }
        t = t2;
    }
    free(pos);
    pvpath = 0;
}

static ArtSVP* gfxfillToSVP(gfxline_t*line, int perturb)
{
    ArtVpath* vec = gfxline_to_ArtVpath(line, 1);
    msg("<verbose> Casting gfxline of %d segments (%d line segments) to a gfxpoly", gfxline_len(line), vpath_len(vec));

    if(perturb) {
	//ArtVpath* vec2 = art_vpath_perturb(vec);
	//free(vec);
	//vec = vec2;
        my_perturb(vec);
    }
    ArtSVP *svp = art_svp_from_vpath(vec);
    free(vec);

    return svp;
}

//#ifdef SHEAR
//    double shear = find_shear_value(svp);
//    gfxline_t*line =  gfxline_from_gfxpoly((gfxpoly_t*)svp);
//    gfxline_t*l = line;
//    while(l) {
//        l->y += l->x*shear;
//        l->sy += l->sx*shear;
//        l= l->next;
//    }
//    svp = (ArtSVP*)gfxpoly_fillToPoly(line);
//    printf("shearing svp by %.16f\n", shear);
//#endif
// ....
//#ifdef SHEAR
//    art_svp_free(svp);
//    shear_svp(result, -shear);
//#endif


ArtSVP* run_intersector(ArtSVP*svp, ArtWindRule rule)
{
    ArtSvpWriter * swr = art_svp_writer_rewind_new(rule);

    double zoom = 1.0;
    intbbox_t bbox = get_svp_bbox(svp, zoom);

    art_svp_intersector(svp, swr);
    ArtSVP*result = art_svp_writer_rewind_reap(swr);
    clean_svp(result);
    if(!check_svp(result)) {
	current_svp = result;
	art_report_error(); // might set art_error_in_intersector
    } else {
        msg("<verbose> Comparing polygon renderings of size %dx%d and %dx%d", bbox.width, bbox.height, bbox.width, bbox.height);
        unsigned char*data1 = render_svp(svp, &bbox, zoom, rule);
        unsigned char*data2 = render_svp(result, &bbox, zoom, ART_WIND_RULE_ODDEVEN);
        if(!compare_bitmaps(&bbox, data1, data2)) {
            msg("<verbose> Bad SVP rewinding result- polygons don't match");
            current_svp = result;
            art_report_error(); // might set art_error_in_intersector
        }
        free(data1);
        free(data2);
    }

    if(art_error_in_intersector) {
	msg("<verbose> Error in polygon processing");
	art_svp_free(result);
	art_error_in_intersector=0;
	return 0;
    }
    return result;
}

gfxline_t* gfxline_from_gfxpoly(gfxpoly_t*poly)
{
    ArtSVP*svp = (ArtSVP*)poly;
    int size = 0;
    int t;
    int pos = 0;

    msg("<verbose> Casting polygon of %d segments back to gfxline", svp->n_segs);
    
    for(t=0;t<svp->n_segs;t++) {
	size += svp->segs[t].n_points;
    }
    size = size + 1;
    gfxline_t* lines = (gfxline_t*)rfx_alloc(sizeof(gfxline_t)*size);

    for(t=0;t<svp->n_segs;t++) {
	ArtSVPSeg* seg = &svp->segs[t];
	int p;
	for(p=0;p<seg->n_points;p++) {
	    lines[pos].type = p==0?gfx_moveTo:gfx_lineTo;
	    ArtPoint* point = &seg->points[p];
	    lines[pos].x = point->x;
	    lines[pos].y = point->y;
	    lines[pos].next = &lines[pos+1];
	    pos++;
	}
    }
    if(pos) {
	lines[pos-1].next = 0;
	return lines;
    } else {
	return 0;
    }
}

gfxpoly_t* gfxpoly_from_fill(gfxline_t*line, double gridsize)
{
    /* I'm not sure whether doing perturbation here is actually
       a good idea- if that line has been run through the machinery
       several times already, it might be safer to leave it alone,
       since it should already be in a format libart can handle */
#ifdef PERTURBATE
    ArtSVP* svp = gfxfillToSVP(line, 1);
#else
    ArtSVP* svp = gfxfillToSVP(line, 0);
#endif

#ifdef DEBUG
    char filename[80];
    static int counter = 0;
    sprintf(filename, "svp%d.ps", counter);
    write_svp_postscript(filename, svp);
    sprintf(filename, "gfxline%d.ps", counter);
    write_gfxline_postscript(filename, line);
#endif

    /* we do xor-filling by default, so dir is always 1 
       (actually for oddeven rewinding it makes no difference, but
        it's "cleaner")
     */
    int t;
    for(t=0; t<svp->n_segs; t++) {
        svp->segs[t].dir = 1;
    }
            
    /* for some reason, we need to rewind / self-intersect the polygons that gfxfillToSVP
       returns- art probably uses a different fill mode (circular?) for vpaths */
    ArtSVP*svp_uncrossed=0;
   
#ifdef DEBUG
    sprintf(filename, "svp%d_in.ps", counter);
    write_svp_postscript(filename, svp);
    counter++;
#endif

    svp_uncrossed = run_intersector(svp, ART_WIND_RULE_ODDEVEN);

    art_svp_free(svp);
    svp=svp_uncrossed;

    return (gfxpoly_t*)svp;
}

gfxpoly_t* gfxpoly_intersect(gfxpoly_t*poly1, gfxpoly_t*poly2)
{
    ArtSvpWriter *swr;

    static int counter = 0;
    
    ArtSVP* svp1 = (ArtSVP*)poly1;
    ArtSVP* svp2 = (ArtSVP*)poly2;
    msg("<verbose> Intersecting two polygons of %d and %d segments", svp1->n_segs, svp2->n_segs);
    
#ifdef DEBUG
    char filename[80];
    sprintf(filename, "isvp%d_src1.ps", counter);
    write_svp_postscript(filename, svp1);
    sprintf(filename, "isvp%d_src2.ps", counter);
    write_svp_postscript(filename, svp2);
#endif
    
    ArtSVP* svp3 = art_svp_merge (svp1, svp2);

#ifdef DEBUG
    sprintf(filename, "isvp%d_src.ps", counter);
    write_svp_postscript(filename, svp3);
#endif

    //write_svp_postscript("svp.ps", svp3);
    ArtSVP*svp_new = run_intersector(svp3, ART_WIND_RULE_INTERSECT);

    art_free (svp3); /* shallow free because svp3 contains shared segments */

#ifdef DEBUG
    sprintf(filename, "isvp%d.ps", counter);
    write_svp_postscript(filename, svp_new);
#endif

    counter++;
    
    //write_svp_postscript("svp_new.ps", svp_new);
	
    return (gfxpoly_t*)svp_new;
}

gfxpoly_t* gfxpoly_union(gfxpoly_t*poly1, gfxpoly_t*poly2)
{
    ArtSVP* svp1 = (ArtSVP*)poly1;
    ArtSVP* svp2 = (ArtSVP*)poly2;
    msg("<verbose> Unifying two polygons of %d and %d segments", svp1->n_segs, svp2->n_segs);
	
    ArtSVP* svp = art_svp_union(svp1, svp2);
    return (gfxpoly_t*)svp;
}

gfxpoly_t* gfxpoly_from_stroke(gfxline_t*line, gfxcoord_t width, gfx_capType cap_style, gfx_joinType joint_style, double miterLimit, double gridsize)
{
    ArtVpath* vec = gfxline_to_ArtVpath(line, 0);
    msg("<verbose> Casting gfxline of %d segments to a stroke-polygon", gfxline_len(line));

    ArtVpath* vec2 = art_vpath_perturb(vec);
    free(vec);
    vec = vec2;

    ArtSVP *svp = art_svp_vpath_stroke (vec,
			(joint_style==gfx_joinMiter)?ART_PATH_STROKE_JOIN_MITER:
			((joint_style==gfx_joinRound)?ART_PATH_STROKE_JOIN_ROUND:
			 ((joint_style==gfx_joinBevel)?ART_PATH_STROKE_JOIN_BEVEL:ART_PATH_STROKE_JOIN_BEVEL)),
			(cap_style==gfx_capButt)?ART_PATH_STROKE_CAP_BUTT:
			((cap_style==gfx_capRound)?ART_PATH_STROKE_CAP_ROUND:
			 ((cap_style==gfx_capSquare)?ART_PATH_STROKE_CAP_SQUARE:ART_PATH_STROKE_CAP_SQUARE)),
			width, //line_width
			miterLimit, //miter_limit
			0.05 //flatness
			);
    free(vec);
    return (gfxpoly_t*)svp;
}

gfxline_t* gfxpoly_circular_to_evenodd(gfxline_t*line, double gridsize)
{
    msg("<verbose> Converting circular-filled gfxline of %d segments to even-odd filled gfxline", gfxline_len(line));
    ArtSVP* svp = gfxfillToSVP(line, 1);

    ArtSVP* svp_rewinded;
    
    svp_rewinded = run_intersector(svp, ART_WIND_RULE_NONZERO);
    if(!svp_rewinded) {
	art_svp_free(svp);
	return 0;
    }

    gfxline_t* result = gfxline_from_gfxpoly((gfxpoly_t*)svp_rewinded);
    art_svp_free(svp);
    art_svp_free(svp_rewinded);
    return result;
}

gfxpoly_t* gfxpoly_createbox(double x1, double y1,double x2, double y2, double gridsize)
{
    ArtVpath *vec = art_new (ArtVpath, 5+1);
    vec[0].code = ART_MOVETO;
    vec[0].x = x1;
    vec[0].y = y1;
    vec[1].code = ART_LINETO;
    vec[1].x = x1;
    vec[1].y = y2;
    vec[2].code = ART_LINETO;
    vec[2].x = x2;
    vec[2].y = y2;
    vec[3].code = ART_LINETO;
    vec[3].x = x2;
    vec[3].y = y1;
    vec[4].code = ART_LINETO;
    vec[4].x = x1;
    vec[4].y = y1;
    vec[5].code = ART_END;
    vec[5].x = 0;
    vec[5].y = 0;
    ArtSVP *svp = art_svp_from_vpath(vec);
    free(vec);
    return (gfxpoly_t*)svp;
}

void gfxpoly_destroy(gfxpoly_t*poly)
{
    ArtSVP*svp = (ArtSVP*)poly;
    art_svp_free(svp);
}
