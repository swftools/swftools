#include <assert.h>
#include <math.h>

static ArtVpath* gfxline_to_ArtVpath(gfxline_t*line)
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
	} if(l2->type == gfx_lineTo) {
	    pos ++;
	} if(l2->type == gfx_splineTo) {
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
	    vec[pos].code = ART_MOVETO;
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
	    for(i=0;i<=parts;i++) {
		double t = (double)i/(double)parts;
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
    vec[pos].code = ART_END;

    return vec;
}

static void show_path(ArtSVP*path)
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

static ArtSVP* gfxfillToSVP(gfxline_t*line, int perturb)
{
    ArtVpath* vec = gfxline_to_ArtVpath(line);
    if(perturb) {
	ArtVpath* vec2 = art_vpath_perturb(vec);
	free(vec);
	vec = vec2;
    }
    ArtSVP *svp = art_svp_from_vpath(vec);
    free(vec);
    return svp;
}
static ArtSVP* boxToSVP(double x1, double y1,double x2, double y2)
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
    return svp;
}

static ArtSVP* gfxstrokeToSVP(gfxline_t*line, gfxcoord_t width, gfx_capType cap_style, gfx_joinType joint_style, double miterLimit)
{
    ArtVpath* vec = gfxline_to_ArtVpath(line);
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
    return svp;
}

static gfxline_t* SVPtogfxline(ArtSVP*svp)
{
    int size = 0;
    int t;
    int pos = 0;
    for(t=0;t<svp->n_segs;t++) {
	size += svp->segs[t].n_points + 1;
    }
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

