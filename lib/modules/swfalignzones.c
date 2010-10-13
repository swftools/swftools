#include "../rfxswf.h"
#include "../graphcut.c"
#include "../log.h"

static inline double sqr(double x) {return x*x;}

static void draw_line(float*row, float x1, float x2, float y1, float y2, int min, int max, double weight)
{
    if(x2<x1) {int x=x1;x1=x2;x2=x;}
    if(x1<min || x2>max) {
	fprintf(stderr, "error: glyph x stroke out of bounds\n");
	return;
    }
    x1 -= min;
    x2 -= min;

    double d = sqrt(sqr(y2-y1)+sqr(x2-x1));
    if(floor(x1)==floor(x2)) {
	row[(int)floor(x1)] += d*weight;
    } else {
	double i = d/(x2-x1);
	int x;
	int xx1 = ceil(x1);
	int xx2 = floor(x2);
	row[xx1] += i*(xx1-x1)*weight;
	row[xx2] += i*(x2-xx2)*weight;
	for(x=xx1;x<xx2;x++) {
	    row[x] += i*weight;
	}
    }
}

static void draw_line_xy(float*row,float*column, float x1, float y1, float x2, float y2,SRECT* area, double weight)
{
    draw_line(row, x1, x2, y1, y2, area->xmin, area->xmax, weight);
    draw_line(column, y1, y2, x1, x2, area->ymin, area->ymax, weight);
}

static void find_best(float*_row, int width, int*_x1, int*_x2, int min_size, int from, int to, int num, char debug)
{
    int x1=-1, x2=-1;
    float max1=-1e20,max2=-1e20;
    int t;

    if(from==to) {
	*_x1 = from;
	return;
    }

    float*row = malloc(sizeof(float)*(width+1));
    int filter_size = 25;
    float* filter = malloc(sizeof(float)*(filter_size*2+1));
    double var = filter_size/3;
    double sum = 0;
    for(t=-filter_size;t<=filter_size;t++) {
	double v = t/var;
        float r = v*v/2;
	filter[filter_size+t] = exp(-r);
	sum += filter[filter_size+t];
    }
    for(t=-filter_size;t<=filter_size;t++) {
	filter[filter_size+t]/=sum;
    }
    
    //filter[0]=1;filter_size=0;

    for(t=0;t<=width;t++) {
	int s;
	double sum = 0;
	for(s=-filter_size;s<=filter_size;s++) {
	    if(t+s<0) continue;
	    if(t+s>width) continue;
	    sum += _row[t+s]*filter[s+filter_size];
	}
	row[t] = sum;
    }
    free(filter);

    for(t=from;t<=to;t++) {
	if(row[t]>max1) {
	    max1 = row[t];
	    x1 = t;
	}
    }


    if(num<=1) {
	*_x1=x1;
    } else {
	/* this code is slightly wrong, in that it assumes that the glyph distortion problem 
	   gets worse when the font sizes get smaller. it doesn't. in fact, the smaller
	   the font size, the more of the scaling bugs disappear (http://www.quiss.org/files/scaletest.swf)
	   A better way would probably to use the font size you need for the two alignzones
	   to come to lie in different pixels, which what I think is what makes the problems
	   appear/disappear.
	*/

	double scale = min_size/1024.0;
	for(t=from;t<=to;t++) {
	    if(t==x1) {
		row[t]=-1e20;
		continue;
	    }
	    double r1 = (t<x1?t:x1)*scale;
	    double r2 = (t<x1?x1:t)*scale;
	    double d1 = r2-r1;
	    double d2 = d1+2;
	    double s = d2/d1;
	    double ext1 = r1-from*scale;
	    double ext2 = to*scale-r2;
	    double add1 = ext1*s - ext1;
	    double add2 = ext2*s - ext2;

	    /* don't allow the char to grow more than one pixel */
	    if(add1>=1 || add2>=1) {
		row[t]=-1e20;
	    }
	}

	for(t=from;t<=to;t++) {
	    if(row[t]>max2) {
		max2 = row[t];
		x2 = t;
	    }
	}

	if(x1>=0 && x2>=0 && x1>x2) {int x=x1;x1=x2;x2=x;}
    
	*_x1=x1;
	*_x2=x2;
    }
    
    free(row);
}

static void negate_y(SRECT* b)
{
    // negate y
    int by1=b->ymin,by2=b->ymax;
    b->ymin = -by2;
    b->ymax = -by1;
}

static void draw_char(SWFFONT * f, int nr, float*row, float*column, SRECT b, double weight)
{
    SWFGLYPH*g = &f->glyph[nr];

    SHAPE2*s = swf_ShapeToShape2(g->shape);
    SHAPELINE*l = s->lines;
    int x=0,y=0;
    while(l) {
	if(l->type == lineTo) {
	    draw_line_xy(row,column,x,-y,l->x,-l->y,&b,weight);
	} else if(l->type == splineTo) {
	    double x1=x,x2=l->sx,x3=l->x;
	    double y1=y,y2=l->sy,y3=l->y;
            double c = fabs(x3-2*x2+x1) + fabs(y3-2*y2+y1);
            int parts = ((int)(sqrt(c)/6))*2+1;
	    float xx=x1,yy=y1;
	    int t;
            for(t=1;t<=parts;t++) {
                float nx = ((t*t*x3 + 2*t*(parts-t)*x2 + (parts-t)*(parts-t)*x1)/(double)(parts*parts));
                float ny = ((t*t*y3 + 2*t*(parts-t)*y2 + (parts-t)*(parts-t)*y1)/(double)(parts*parts));
                draw_line_xy(row,column,xx,-yy,nx,-ny,&b,weight);
                xx = nx;
                yy = ny;
            }
	}
	x = l->x;
	y = l->y;
	l = l->next;
    }
    swf_Shape2Free(s);
    free(s);
}

static ALIGNZONE detect_for_char(SWFFONT * f, float*row, float*column, SRECT font_bbox, SRECT char_bbox)
{
    ALIGNZONE a = {0xffff,0xffff,0xffff,0xffff};
    int width = font_bbox.xmax - font_bbox.xmin;
    int height = font_bbox.ymax - font_bbox.ymin;
    if(!width || !height)
	return a;

    /* find two best x values */
    int x1=-1,y1=-1,x2=-1,y2=-1;

    int nr_x = 0;
    find_best(row, width, &x1, &x2, f->use->smallest_size, 
		char_bbox.xmin - font_bbox.xmin,
		char_bbox.xmax - font_bbox.xmin, nr_x,
		0);
    if(nr_x>0 && x1>=0) a.x  = floatToF16((x1+font_bbox.xmin) / 20480.0);
    if(nr_x>1 && x2>=0) a.dx = floatToF16((x2-x1) / 20480.0);

    find_best(column, height, &y1, &y2, f->use->smallest_size, 
		char_bbox.ymin - font_bbox.ymin,
		char_bbox.ymax - font_bbox.ymin, 2,
		0);
    if(y1>=0) a.y  = floatToF16((y1+font_bbox.ymin) / 20480.0);
    if(y2>=0) a.dy = floatToF16((y2-y1) / 20480.0);
    return a;
}

static graph_t*make_graph(SWFFONT*f)
{
    FONTUSAGE*use = f->use;
    graph_t*g = graph_new(f->numchars);
    int s,t;
    for(s=1;s<f->numchars;s++) {
	for(t=0;t<s;t++) {
	    if(f->glyph2ascii) {
		int c1 = f->glyph2ascii[s];
		int c2 = f->glyph2ascii[t];
		if((c1<'a' && c2>='a' && c2<='z') ||
		   (c2<'a' && c1>='a' && c1<='z')) {
		    /* never connect lowercase with any uppercase
		       or punctuation */
		    continue;
		}
	    }

	    int pos1 = swf_FontUseGetPair(f, s, t);
	    int pos2 = swf_FontUseGetPair(f, t, s);
	    if(pos1 || pos2) {
		int weight1 = pos1?use->neighbors[pos1-1].num:0;
		int weight2 = pos2?use->neighbors[pos2-1].num:0;
		int weight = weight1+weight2;
		
		/*printf("font %d: pair %c and %c\n",
			f->id, f->glyph2ascii[t], f->glyph2ascii[s]);*/
		graph_add_edge(&g->nodes[s], &g->nodes[t], weight, weight);
	    }
	}
    }
    return g;
}

void swf_FontCreateAlignZones(SWFFONT * f)
{
    if(f->alignzones)
	return;

    if(!f->layout) {
	fprintf(stderr, "Error: font needs a layout for alignzones to be detected.");
	return;
    }

    f->alignzones = (ALIGNZONE*)rfx_calloc(sizeof(ALIGNZONE)*f->numchars);
    f->alignzone_flags = FONTALIGN_MEDIUM;

    if(!f->layout || !f->use) {
	int t;
	for(t=0;t<f->numchars;t++) {
	    // just align the baseline
	    f->alignzones[t].x = 0xffff;
	    f->alignzones[t].y = 0;
	    f->alignzones[t].dx = 0xffff;
	    f->alignzones[t].dy = 0xffff;//floatToF16(460.80 / 1024.0);
	}
    } else {
	graph_t*g = make_graph(f);

	int num_components = graph_find_components(g);
	msg("<notice> Building font alignzone information for font %d (%d characters, %d components, %d pairs)\n",
		f->id, f->numchars, num_components, f->use->num_neighbors);

	SRECT bounds = {0,0,0,0};
	int t;
	for(t=0;t<f->numchars;t++) {
	    SRECT b = f->layout->bounds[t];
	    negate_y(&b);
	    swf_ExpandRect2(&bounds, &b);
	}

	int width = bounds.xmax - bounds.xmin;
	int height = bounds.ymax - bounds.ymin;
	float*row = rfx_calloc(sizeof(float)*(width+1));
	float*global_column = rfx_calloc(sizeof(float)*(height+1));
	float*column = rfx_calloc(sizeof(float)*(height+1));

	const double SELF_WEIGHT = 0.00; // ignore own char

	int c;
	for(c=0;c<num_components;c++) {
	    int drawn = 0;
	    memset(global_column, 0, sizeof(float)*(height+1));
	    SRECT local_bounds = {0,0,0,0};
	    for(t=0;t<f->numchars;t++) {
		if(g->nodes[t].tmp == c) {
		    draw_char(f, t, row, global_column, bounds, 1.0-SELF_WEIGHT);
		    SRECT b = f->layout->bounds[t];
		    negate_y(&b);
		    swf_ExpandRect2(&local_bounds, &b);
		    drawn++;
		}
	    }

	    for(t=0;t<=height;t++) {
		global_column[t] /= drawn;
	    }

	    memcpy(column, global_column, sizeof(float)*(height+1));
	    memset(row, 0, sizeof(float)*(width+1));
	    ALIGNZONE a = detect_for_char(f, row, column, bounds, local_bounds);

	    for(t=0;t<f->numchars;t++) {
		if(g->nodes[t].tmp == c) {
		    f->alignzones[t] = a;
		}
	    }
	}
	free(row);
	free(column);
	free(global_column);

	graph_delete(g);
    }
}

void swf_FontPostprocess(SWF*swf)
{
    TAG*tag = swf->firstTag;
    while(tag) {
	TAG*next = tag->next;
	if(tag->id == ST_DEFINEFONT3) {
	    U16 id = swf_GetDefineID(tag);
	    SWFFONT*font = 0;
	    swf_FontExtract(swf, id, &font);
	    if(!font->alignzones) {
		swf_FontCreateAlignZones(font);
		tag = swf_InsertTag(tag, ST_DEFINEFONTALIGNZONES);
		swf_FontSetAlignZones(tag, font);
	    }
	    swf_FontFree(font);
	}
	tag = next;
    }
}

void swf_FontSetAlignZones(TAG*t, SWFFONT *f)
{
    swf_SetU16(t, f->id);
    swf_SetU8(t, f->alignzone_flags);
    int i;
    for(i=0;i<f->numchars;i++) {
	ALIGNZONE*a = &f->alignzones[i];
	U8 flags = 0;
	if((a->x & a->dx)!=0xffff)
	    flags |= 1;
	if((a->y & a->dy)!=0xffff)
	    flags |= 2;
	int num = 1;
	if(a->dx != 0xffff || a->dy != 0xffff)
	    num++;
	swf_SetU8(t, num);
	if(flags&1) swf_SetU16(t, a->x); else swf_SetU16(t, 0);
	if(flags&2) swf_SetU16(t, a->y); else swf_SetU16(t, 0);
	if(num==2) {
	    if((flags&1) && a->dx!=0xffff) swf_SetU16(t, a->dx); else swf_SetU16(t, 0);
	    if((flags&2) && a->dy!=0xffff) swf_SetU16(t, a->dy); else swf_SetU16(t, 0);
	}
	swf_SetU8(t, flags);
    }
}

