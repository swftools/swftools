#include "../rfxswf.h"

static inline double sqr(double x) {return x*x;}

static void draw_line(float*row, float x1, float x2, float y1, float y2, int min, int max)
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
	row[(int)floor(x1)] += d;
    } else {
	double i = d/(x2-x1);
	int x;
	int xx1 = ceil(x1);
	int xx2 = floor(x2);
	row[xx1] += i*(xx1-x1);
	row[xx2] += i*(x2-xx2);
	for(x=xx1;x<xx2;x++) {
	    row[x]+=i;
	}
    }
}

static void draw_line_xy(float*row,float*column, float x1, float y1, float x2, float y2,SRECT* area)
{
    draw_line(row, x1, x2, y1, y2, area->xmin, area->xmax);
    draw_line(column, y1, y2, x1, x2, area->ymin, area->ymax);
}

static void find_best(float*_row, int width, int*_x1, int*_x2, int min_dist)
{
    int x1=-1, x2=-1;
    float max1=-1,max2=-1;
    int t;
    float*row = malloc(sizeof(float)*(width+1));
    int filter_size = 100;
    float* filter = malloc(sizeof(float)*(filter_size*2+1));
    double var = filter_size/3;
    for(t=-filter_size;t<=filter_size;t++) {
	double v = t/var;
        float r = v*v/2;
	filter[filter_size+t] = exp(-r);
    }
    filter[0]=1;filter_size=0;

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

    for(t=0;t<=width;t++) {
	if(row[t]>max1) {
	    max1 = row[t];
	    x1 = t;
	}
    }
    // invalidate everything around the maximum
    for(t=-min_dist+1;t<min_dist;t++) {
	if(t+x1<0 || t+x1>width) continue;
	row[t+x1]=-1;
    }
    for(t=0;t<=width;t++) {
	if(row[t]>max2) {
	    max2 = row[t];
	    x2 = t;
	}
    }

    if(x1>0 && x2>0 && x1>x2) {int x=x1;x1=x2;x2=x;}
    *_x1=x1;
    *_x2=x2;
    free(row);
}

static ALIGNZONE detect_for_char(SWFFONT * f, int nr)
{
    SWFGLYPH*g = &f->glyph[nr];
    SRECT b = f->layout->bounds[nr];
    ALIGNZONE a = {0xffff,0xffff,0xffff,0xffff};

    // negate y
    int by1=b.ymin,by2=b.ymax;
    b.ymin = -by2;
    b.ymax = -by1;

    int width = b.xmax - b.xmin + 1;
    int height = b.ymax - b.ymin + 1;
    if(!width || !height)
	return a;
	    
    float*row = rfx_calloc(sizeof(float)*width);
    float*column = rfx_calloc(sizeof(float)*height);

    SHAPE2*s = swf_ShapeToShape2(g->shape);
    SHAPELINE*l = s->lines;
    int x=0,y=0;
    while(l) {
	if(l->type == lineTo) {
	    draw_line_xy(row,column,x,-y,l->x,-l->y,&b);
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
                draw_line_xy(row,column,xx,-yy,nx,-ny,&b);
                xx = nx;
                yy = ny;
            }
	}
	x = l->x;
	y = l->y;
	l = l->next;
    }

    int t;

    /* find two best x values */
    int x1=-1,y1=-1,x2=-1,y2=-1;
    //int min_dist = 4000;
    int min_dist = 1;
    find_best(row,width,&x1,&x2,min_dist);
    find_best(column,height,&y1,&y2,min_dist);

    if(x1>=0) a.x  = floatToF16((x1+b.xmin) / 20480.0);
    if(x2>=0) a.dx = floatToF16((x2-x1) / 20480.0);
    if(y1>=0) a.y  = floatToF16((y1+b.ymin) / 20480.0);
    if(y2>=0) a.dy = floatToF16((y2-y1) / 20480.0);
    return a;
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
    f->alignzone_flags = FONTALIGN_THIN;

    if(!f->layout) {
	int t;
	for(t=0;t<f->numchars;t++) {
	    // just align the baseline
	    f->alignzones[t].x = 0xffff;
	    f->alignzones[t].y = 0;
	    f->alignzones[t].dx = 0xffff;
	    f->alignzones[t].dy = 0xffff;//floatToF16(460.80 / 1024.0);
	}
    } else {
	int t;
	for(t=0;t<f->numchars;t++) {
	    f->alignzones[t] = detect_for_char(f, t);
	}
    }

/*
    "-^_~\xad\xaf+`\xac\xb7\xf7" //chars for which to detect one y value
    "#=:;\xb1" //chars for which to detect two y values
    "\"\xa8" //chars for which to detect two x values
*/
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


