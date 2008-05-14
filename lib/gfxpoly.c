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
    while(l2) {
	if(l2->type == gfx_moveTo) {
	    vec[pos].code = ART_MOVETO_OPEN;
	    vec[pos].x = l2->x;
	    vec[pos].y = l2->y;
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
       points is found, the second is removed.
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
    /* do a "shearing on the polygon. We do this to eliminate all
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
        v = lrand48() / 2000000000.0;
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

void check_svp(ArtSVP*svp)
{
    int t;
    for(t=0;t<svp->n_segs;t++) {
	ArtSVPSeg* seg = &svp->segs[t];
	int p;
	for(p=0;p<seg->n_points-1;p++) {
	    ArtPoint* p1 = &seg->points[p];
	    ArtPoint* p2 = &seg->points[p+1];
            if(p1->y > p2->y) {
                fprintf(stderr, "bad svp, y in seg %08x is non-increasing\n");
                exit(0);
            }
	}
    }
}

void write_svp_postscript(const char*filename, ArtSVP*svp)
{
    printf("writing %s\n", filename);
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

ArtSVP* run_intersector(ArtSVP*svp, ArtWindRule rule)
{
    ArtSvpWriter * swr = art_svp_writer_rewind_new(rule);

#ifdef SHEAR
    double shear = find_shear_value(svp);
    gfxline_t*line =  gfxpoly_to_gfxline((gfxpoly_t*)svp);
    gfxline_t*l = line;
    while(l) {
        l->y += l->x*shear;
        l->sy += l->sx*shear;
        l= l->next;
    }
    svp = (ArtSVP*)gfxpoly_fillToPoly(line);
    printf("shearing svp by %.16f\n", shear);
#endif

    art_svp_intersector(svp, swr);
    ArtSVP*result = art_svp_writer_rewind_reap(swr);
    check_svp(result);

#ifdef SHEAR
    art_svp_free(svp);
    shear_svp(result, -shear);
#endif

    return result;
}


gfxline_t* gfxpoly_to_gfxline(gfxpoly_t*poly)
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

gfxpoly_t* gfxpoly_fillToPoly(gfxline_t*line)
{
    /* I'm not sure whether doing perturbation here is actually
       a good idea- if that line has been run through the machine
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

gfxpoly_t* gfxpoly_strokeToPoly(gfxline_t*line, gfxcoord_t width, gfx_capType cap_style, gfx_joinType joint_style, double miterLimit)
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

gfxline_t* gfxline_circularToEvenOdd(gfxline_t*line)
{
    msg("<verbose> Converting circular-filled gfxline of %d segments to even-odd filled gfxline", gfxline_len(line));
    ArtSVP* svp = gfxfillToSVP(line, 1);

    /* TODO: ART_WIND_RULE_POSITIVE means that a shape is visible if
             positive and negative line segments add up to something positive.
             I *think* that clockwise fill in PDF is defined in a way, however,
             that the *last* shape's direction will determine whether something
             is filled */
    ArtSVP* svp_rewinded;
    
    svp_rewinded = run_intersector(svp, ART_WIND_RULE_POSITIVE);

    gfxline_t* result = gfxpoly_to_gfxline((gfxpoly_t*)svp_rewinded);
    art_svp_free(svp);
    art_svp_free(svp_rewinded);
    return result;
}

gfxpoly_t* gfxpoly_createbox(double x1, double y1,double x2, double y2)
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

void gfxpoly_free(gfxpoly_t*poly)
{
    ArtSVP*svp = (ArtSVP*)poly;
    art_svp_free(svp);
}
