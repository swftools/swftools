/* swfc- Compiles swf code (.sc) files into .swf files.

   Part of the swftools package.

   Copyright (c) 2007 Huub Schaeks <huub@h-schaeks.speedlinq.nl>
   Copyright (c) 2007 Matthias Kramm <kramm@quiss.org>
 
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

#include <stdlib.h>
#include <memory.h>
#include "swfc-history.h"

change_t* change_new(U16 frame, int function, float value, interpolation_t* inter)
{
	change_t* newChange = (change_t*)malloc(sizeof(change_t));
	change_init(newChange);
	newChange->frame = frame;
	newChange->function = function;
	newChange->value = value;
	newChange->interpolation = inter;
	return newChange;
}

void change_free(change_t *change)
{
	if (change->next)
		change_free(change->next);
	free(change);
}

void change_init(change_t* change)
{
	memset(change, 0, sizeof(change_t));
}

void change_append(change_t* first, change_t* newChange)
{
    change_t* previous = 0;
    change_t* start = first;
    float p0, p1, m0, m1;

	while (first->next)
    {
    	previous = first;
		first = first->next;
    }
	first->next = newChange;
    if (first->function == CF_QCHANGE)
    {
    	p0 = previous->value;
    	p1 = first->value;
    	if (previous->function == CF_QCHANGE)
    	    m0 = (3 * previous->spline.a + 2 * previous->spline.b + previous->spline.c);
    	else
    	    if (previous->function == CF_CHANGE)
    	    	m0 = (change_value(start, previous->frame) - change_value(start, previous->frame - 1)) * (first->frame - previous->frame);
    	    else
    		m0 = (first->value - previous->value);
    	if (newChange->function == CF_QCHANGE)
    	    m1 = 0.5 * (newChange->value - previous->value);
    	else
    	    if (newChange->function == CF_CHANGE)
    		m1 = (change_value(previous, first->frame + 1) - change_value(previous, first->frame)) * (first->frame - previous->frame);
    	    else
  		m1 = (first->value - previous->value);
    	first->spline.a = 2 * p0 + m0 - 2 * p1 + m1;
    	first->spline.b = -3 * p0 - 2 * m0 + 3 * p1 - m1;
    	first->spline.c = m0;
    	first->spline.d = p0;
    }
    if (newChange->function == CF_QCHANGE)
    {
    	p0 = first->value;
    	p1 = newChange->value;
    	if (first->function == CF_QCHANGE)
    	    m0 = m1;
    	else
    	    if (first->function == CF_CHANGE)
    	    	m0 = (change_value(start, first->frame) - change_value(start, first->frame - 1)) * (first->frame - previous->frame);
    	    else
	    	m0 = (newChange->value - first->value);
    	m1 = (newChange->value - first->value);
    	newChange->spline.a = 2 * p0 + m0 - 2 * p1 + m1;
    	newChange->spline.b = -3 * p0 - 2 * m0 + 3 * p1 - m1;
    	newChange->spline.c = m0;
    	newChange->spline.d = p0;
    }
}

float calculateSpline(change_t* modification, float fraction)
{
    spline_t s = modification->spline;
    return (((s.a * fraction) + s.b) * fraction + s.c) * fraction + s.d;
}

float interpolateScalar(float p1, float p2, float fraction, interpolation_t* inter)
{
	if (!inter)
		return linear(fraction, p1, p2 - p1);
	switch (inter->function)
	{
		case IF_LINEAR: return linear(fraction, p1, p2 - p1);
		case IF_QUAD_IN: return quadIn(fraction, p1, p2 - p1);
		case IF_QUAD_OUT: return quadOut(fraction, p1, p2 - p1);
		case IF_QUAD_IN_OUT: return quadInOut(fraction, p1, p2 - p1);
		case IF_CUBIC_IN: return cubicIn(fraction, p1, p2 - p1);
		case IF_CUBIC_OUT: return cubicOut(fraction, p1, p2 - p1);
		case IF_CUBIC_IN_OUT: return cubicInOut(fraction, p1, p2 - p1);
		case IF_QUART_IN: return quartIn(fraction, p1, p2 - p1);
		case IF_QUART_OUT: return quartOut(fraction, p1, p2 - p1);
		case IF_QUART_IN_OUT: return quartInOut(fraction, p1, p2 - p1);
		case IF_QUINT_IN: return quintIn(fraction, p1, p2 - p1);
		case IF_QUINT_OUT: return quintOut(fraction, p1, p2 - p1);
		case IF_QUINT_IN_OUT: return quintInOut(fraction, p1, p2 - p1);
		case IF_CIRCLE_IN: return circleIn(fraction, p1, p2 - p1);
		case IF_CIRCLE_OUT: return circleOut(fraction, p1, p2 - p1);
		case IF_CIRCLE_IN_OUT: return circleInOut(fraction, p1, p2 - p1);
		case IF_EXPONENTIAL_IN: return exponentialIn(fraction, p1, p2 - p1);
		case IF_EXPONENTIAL_OUT: return exponentialOut(fraction, p1, p2 - p1);
		case IF_EXPONENTIAL_IN_OUT: return exponentialInOut(fraction, p1, p2 - p1);
		case IF_SINE_IN: return sineIn(fraction, p1, p2 - p1);
		case IF_SINE_OUT: return sineOut(fraction, p1, p2 - p1);
		case IF_SINE_IN_OUT: return sineInOut(fraction, p1, p2 - p1);
		case IF_ELASTIC_IN: return elasticIn(fraction, p1, p2 - p1, inter->amplitude, inter->bounces, inter->damping);
		case IF_ELASTIC_OUT: return elasticOut(fraction, p1, p2 - p1, inter->amplitude, inter->bounces, inter->damping);
		case IF_ELASTIC_IN_OUT: return elasticInOut(fraction, p1, p2 - p1, inter->amplitude, inter->bounces, inter->damping);
		case IF_BACK_IN: return backIn(fraction, p1, p2 - p1, inter->speed);
		case IF_BACK_OUT: return backOut(fraction, p1, p2 - p1, inter->speed);
		case IF_BACK_IN_OUT: return backInOut(fraction, p1, p2 - p1, inter->speed);
		case IF_BOUNCE_IN: return bounceIn(fraction, p1, p2 - p1, inter->bounces, inter->growth, inter->damping);
		case IF_BOUNCE_OUT: return bounceOut(fraction, p1, p2 - p1, inter->bounces, inter->growth, inter->damping);
		case IF_BOUNCE_IN_OUT: return bounceInOut(fraction, p1, p2 - p1, inter->bounces, inter->growth, inter->damping);
		case IF_FAST_BOUNCE_IN: return fastBounceIn(fraction, p1, p2 - p1, inter->bounces, inter->growth, inter->damping);
		case IF_FAST_BOUNCE_OUT: return fastBounceOut(fraction, p1, p2 - p1, inter->bounces, inter->growth, inter->damping);
		case IF_FAST_BOUNCE_IN_OUT: return fastBounceInOut(fraction, p1, p2 - p1, inter->bounces, inter->growth, inter->damping);
		default: return linear(fraction, p1, p2 - p1);
	}
}

int change_differs(change_t* modification, U16 frame)
{
    change_t* previous = modification;
    while (modification && modification->frame < frame)
	{
	previous = modification;
	modification = modification->next;
    }
    if (!modification)
	return 0;
    if (modification->frame == frame)
    	return 1;
    return (modification->function != CF_JUMP);
	}

float change_value(change_t* modification, U16 frame)
{
    change_t* previous = modification;
    while (modification && modification->frame < frame)
    {
	previous = modification;
	modification = modification->next;
    }
    if (!modification)
		return previous->value;
    if (modification->frame == frame)
	{
		do 
		{
	    previous = modification;
	    modification = modification->next;
		}
	while (modification && modification->frame == frame);
	return previous->value;
	}
    switch (modification->function)
	{
		case CF_PUT:
	    return modification->value;
		case CF_CHANGE:
		{
	    float fraction = (frame - previous->frame) / (float)(modification->frame - previous->frame);
	    return interpolateScalar(previous->value, modification->value, fraction, modification->interpolation);
	}
	case CF_QCHANGE:
	{
	    float fraction = (frame - previous->frame) / (float)(modification->frame - previous->frame);
	    return calculateSpline(modification, fraction);
		}
		case CF_JUMP:
			return previous->value;
		default:
			return 0;
	}
}

changeFilter_t* changeFilter_new(U16 frame, int function, FILTER* value, interpolation_t* inter)
{
	changeFilter_t* newChange = (changeFilter_t*)malloc(sizeof(changeFilter_t));
	changeFilter_init(newChange);
	newChange->frame = frame;
	newChange->function = function;
	newChange->value = value;
	newChange->interpolation = inter;
	return newChange;
}

void changeFilter_free(changeFilter_t *change)
{
	if (change->next)
		changeFilter_free(change->next);
	free(change);
}

void changeFilter_init(changeFilter_t* change)
{
	memset(change, 0, sizeof(changeFilter_t));
}

void changeFilter_append(changeFilter_t* first, changeFilter_t* newChange)
{
	while (first->next)
		first = first->next;
	first->next = newChange;
}

RGBA interpolateColor(RGBA c1, RGBA c2, float ratio, interpolation_t* inter)
{
    RGBA c;
    c.r = interpolateScalar(c1.r, c2.r, ratio, inter);
    c.g = interpolateScalar(c1.g, c2.g, ratio, inter);
    c.b = interpolateScalar(c1.b, c2.b, ratio, inter);
    c.a = interpolateScalar(c1.a, c2.a, ratio, inter);
    return c;
}

GRADIENT* interpolateNodes(GRADIENT* g1, GRADIENT* g2, float fraction, interpolation_t* inter)
{
    if (g1->num != g2->num)
    	syntaxerror("Internal error: gradients are not equal in size");

    int i;
    GRADIENT* g = (GRADIENT*) malloc(sizeof(GRADIENT));
    g->ratios = rfx_calloc(16*sizeof(U8));
    g->rgba = rfx_calloc(16*sizeof(RGBA));
    g->num = g1->num;
    for (i = 0; i < g->num; i++)
    {
    	g->ratios[i] = interpolateScalar(g1->ratios[i], g2->ratios[i], fraction, inter);
    	g->rgba[i] = interpolateColor(g1->rgba[i], g2->rgba[i], fraction, inter);
    }
    return g;
}

void copyGradient(GRADIENT* dest, GRADIENT* source)
{
    dest->num = source->num;
    memcpy(dest->ratios, source->ratios, source->num * sizeof(U8));
    memcpy(dest->rgba, source->rgba, source->num * sizeof(RGBA));
}

void insertNode(GRADIENT* g, int pos)
{
    memmove(&g->ratios[pos + 1], &g->ratios[pos], (g->num - pos) * sizeof(U8));
    memmove(&g->rgba[pos + 1], &g->rgba[pos], (g->num - pos) * sizeof(RGBA));
    if (pos == 0)
    {
    	g->ratios[0] = g->ratios[1] / 2;
    	g->rgba[0] = g->rgba[1];
    }
    else
    	if (pos == g->num)
    	{
    	    g->ratios[pos] = (255 + g->ratios[g->num - 1]) / 2;
    	    g->rgba[pos] = g->rgba[pos - 1];
    	}
    	else
    	{
	    g->ratios[pos] = (g->ratios[pos - 1] + g->ratios[pos + 1]) / 2;
	    g->rgba[pos] = interpolateColor(g->rgba[pos - 1], g->rgba[pos + 1], 0.5, 0);
    	}
    g->num++;
}

void insertOptimalNode(GRADIENT* g)
{
    int i, next_gap;
    int pos = 0;
    int gap = g->ratios[0];
    for (i = 0; i < g->num - 1; i++)
    {
    	next_gap = g->ratios[i + 1] - g->ratios[i];
    	if (next_gap > gap)
    	{
    	    gap = next_gap;
    	    pos = i + 1;
    	}
    }
    next_gap = 255 - g->ratios[g->num -1];
    if (next_gap > gap)
    	pos = g->num;
    insertNode(g, pos);
}
   
void growGradient(GRADIENT* start, int size)
{
    while (start->num < size)
    	insertOptimalNode(start);
}

GRADIENT* interpolateGradient(GRADIENT* g1, GRADIENT* g2, float fraction, interpolation_t* inter)
{
    int i;
    GRADIENT g;
    g.ratios = rfx_calloc(16*sizeof(U8));
    g.rgba = rfx_calloc(16*sizeof(RGBA));

    if (g1->num > g2->num)
    {
    	copyGradient(&g, g2);
    	growGradient(&g, g1->num);
    	GRADIENT* result = interpolateNodes(g1, &g, fraction, inter);
    	rfx_free(g.rgba);
    	rfx_free(g.ratios);
    	return result;
    }
    else
    	if (g1->num < g2->num)
    	{
    	    copyGradient(&g, g1);
    	    growGradient(&g, g2->num);
    	    GRADIENT* result = interpolateNodes(&g, g2, fraction, inter);
    	    rfx_free(g.rgba);
    	    rfx_free(g.ratios);
    	    return result;
    	}
    	else
    	    return interpolateNodes(g1, g2, fraction, inter);
}

FILTER* interpolateBlur(FILTER* filter1, FILTER* filter2, float ratio, interpolation_t* inter)
	{
		FILTER_BLUR*f1 = (FILTER_BLUR*)filter1;
		FILTER_BLUR*f2 = (FILTER_BLUR*)filter2;
    if (!f1)
    	f1 = noBlur;
    if (!f2)
    	f2 = noBlur;
    if(f1->blurx == f2->blurx && f1->blury == f2->blury)
			return 0;
		FILTER_BLUR*f = (FILTER_BLUR*)swf_NewFilter(FILTERTYPE_BLUR);
    f->blurx= interpolateScalar(f1->blurx, (f2->blurx), ratio, inter);
    f->blury= interpolateScalar(f1->blury, (f2->blury), ratio, inter);
    f->passes= interpolateScalar(f1->passes, (f2->passes), ratio, inter);
		return (FILTER*)f;
		}

void matchDropshadowFlags(FILTER_DROPSHADOW* unset, FILTER_DROPSHADOW* target)
{
    unset->innershadow = target->innershadow;
    unset->knockout = target->knockout;
    unset->composite = target->composite;
}

FILTER* interpolateDropshadow(FILTER* filter1,FILTER* filter2, float ratio, interpolation_t* inter)
		{
			FILTER_DROPSHADOW*f1 = (FILTER_DROPSHADOW*)filter1;
			FILTER_DROPSHADOW*f2 = (FILTER_DROPSHADOW*)filter2;
    if (!f1)
    	f1 = noDropshadow;
    if (!f2)
    	f2 = noDropshadow;
    if(!memcmp(&f1->color,&f2->color,sizeof(RGBA)) && f1->strength == f2->strength &&
				f1->blurx == f2->blurx && f1->blury == f2->blury && 
				f1->angle == f2->angle && f1->distance == f2->distance)
				return 0;
			FILTER_DROPSHADOW*f = (FILTER_DROPSHADOW*)swf_NewFilter(FILTERTYPE_DROPSHADOW);
			memcpy(f, f1, sizeof(FILTER_DROPSHADOW));
			f->color = interpolateColor(f1->color, f2->color, ratio, inter);
    f->blurx= interpolateScalar(f1->blurx, (f2->blurx), ratio, inter);
    f->blury= interpolateScalar(f1->blury, (f2->blury), ratio, inter);
    f->passes= interpolateScalar(f1->passes, (f2->passes), ratio, inter);
    f->angle= interpolateScalar(f1->angle, (f2->angle), ratio, inter);
    f->distance= interpolateScalar(f1->distance, (f2->distance), ratio, inter);
    f->strength= interpolateScalar(f1->strength, (f2->strength), ratio, inter);
    if (f1 == noDropshadow)
    {
    	if (f2 != noDropshadow)
    	    matchDropshadowFlags(f, f2);
		}
		else
    	if (f2 == noDropshadow)
	    matchDropshadowFlags(f, f1);
	else
	    if (ratio > 0.5)
    		matchDropshadowFlags(f, f2);
	    else
    		matchDropshadowFlags(f, f1);
    return (FILTER*)f;
}

void matchBevelFlags(FILTER_BEVEL* unset, FILTER_BEVEL* target)
{
    unset->innershadow = target->innershadow;
    unset->knockout = target->knockout;
    unset->composite = target->composite;
    unset->ontop = target->ontop;
}

FILTER* interpolateBevel(FILTER* filter1,FILTER* filter2, float ratio, interpolation_t* inter)
			{
				FILTER_BEVEL*f1 = (FILTER_BEVEL*)filter1;
				FILTER_BEVEL*f2 = (FILTER_BEVEL*)filter2;
    if (!f1)
    	f1 = noBevel;
    if (!f2)
    	f2 = noBevel;
    if(!memcmp(&f1->shadow,&f2->shadow,sizeof(RGBA)) &&
					!memcmp(&f1->highlight,&f2->highlight,sizeof(RGBA)) && 
					f1->blurx == f2->blurx && f1->blury == f2->blury && f1->angle == f2->angle && f1->strength == f2->strength && f1->distance == f2->distance)
					return 0;
				FILTER_BEVEL*f = (FILTER_BEVEL*)swf_NewFilter(FILTERTYPE_BEVEL);
				memcpy(f, f1, sizeof(FILTER_BEVEL));
				f->shadow = interpolateColor(f1->shadow, f2->shadow, ratio, inter);
				f->highlight = interpolateColor(f1->highlight, f2->highlight, ratio, inter);
    f->blurx= interpolateScalar(f1->blurx, (f2->blurx), ratio, inter);
    f->blury= interpolateScalar(f1->blury, (f2->blury), ratio, inter);
    f->passes= interpolateScalar(f1->passes, (f2->passes), ratio, inter);
    f->angle= interpolateScalar(f1->angle, (f2->angle), ratio, inter);
    f->distance= interpolateScalar(f1->distance, (f2->distance), ratio, inter);
    f->strength= interpolateScalar(f1->strength, (f2->strength), ratio, inter);
    if (f1 == noBevel)
    {
    	if (f2 != noBevel)
    	    matchBevelFlags(f, f2);
    }
    else
    	if (f2 == noBevel)
	    matchBevelFlags(f, f1);
	else
	    if (ratio > 0.5)
    		matchBevelFlags(f, f2);
	    else
    		matchBevelFlags(f, f1);
				return (FILTER*)f;
}

void matchGradientGlowFlags(FILTER_GRADIENTGLOW* unset, FILTER_GRADIENTGLOW* target)
{
    unset->innershadow = target->innershadow;
    unset->knockout = target->knockout;
    unset->composite = target->composite;
    unset->ontop = target->ontop;
}

FILTER* interpolateGradientGlow(FILTER* filter1,FILTER* filter2, float ratio, interpolation_t* inter)
{
    FILTER_GRADIENTGLOW*f1 = (FILTER_GRADIENTGLOW*)filter1;
    FILTER_GRADIENTGLOW*f2 = (FILTER_GRADIENTGLOW*)filter2;
    if (!f1)
    	f1 = noGradientGlow;
    if (!f2)
    	f2 = noGradientGlow;
    if(f1->gradient->num == f2->gradient->num &&
    		!memcmp(&f1->gradient->ratios,&f2->gradient->ratios,f1->gradient->num * sizeof(U8)) &&
    		!memcmp(&f1->gradient->rgba,&f2->gradient->rgba,f1->gradient->num * sizeof(RGBA)) &&
		f1->blurx == f2->blurx && f1->blury == f2->blury && f1->angle == f2->angle && f1->strength == f2->strength && f1->distance == f2->distance)
		return 0;
	FILTER_GRADIENTGLOW*f = (FILTER_GRADIENTGLOW*)swf_NewFilter(FILTERTYPE_GRADIENTGLOW);
    memcpy(f, f1, sizeof(FILTER_GRADIENTGLOW));
    f->blurx= interpolateScalar(f1->blurx, (f2->blurx), ratio, inter);
    f->blury= interpolateScalar(f1->blury, (f2->blury), ratio, inter);
    f->passes= interpolateScalar(f1->passes, (f2->passes), ratio, inter);
    f->angle= interpolateScalar(f1->angle, (f2->angle), ratio, inter);
    f->distance= interpolateScalar(f1->distance, (f2->distance), ratio, inter);
    double d = f->distance;
    double fr;
    if (d < 0)
    	fr = ((int)d - d)*65536;
    else
    	fr = (d - (int)d)*65536;
    	printf("%.3f <> %.3f : %.3f = %.3f [%.3f . %.3f] - %.5u.%.5u\n", f1->distance, f2->distance, ratio, f->distance, d, fr/65536, (U16)d, (U16)fr);
    f->strength= interpolateScalar(f1->strength, (f2->strength), ratio, inter);
    f->gradient= interpolateGradient(f1->gradient, f2->gradient, ratio, inter);
    if (f1 == noGradientGlow)
    {
    	if (f2 != noGradientGlow)
    	    matchGradientGlowFlags(f, f2);
    }
    else
    	if (f2 == noGradientGlow)
	    matchGradientGlowFlags(f, f1);
	else
	    if (ratio > 0.5)
    		matchGradientGlowFlags(f, f2);
	    else
    		matchGradientGlowFlags(f, f1);
	return (FILTER*)f;
}

FILTER* interpolateFilter(FILTER* filter1,FILTER* filter2, float ratio, interpolation_t* inter)
{
    if(!filter1 && !filter2)
	return 0;


    int filter_type;
    if (!filter1)
    	filter_type = filter2->type;
    else
    	if (!filter2)
    	    filter_type = filter1->type;
    	else
	    if(filter2->type != filter1->type)
		syntaxerror("can't interpolate between %s and %s filters yet", filtername[filter1->type], filtername[filter2->type]);
	    else
	    	filter_type = filter1->type;


    switch (filter_type)
    {
        case FILTERTYPE_BLUR:
            return interpolateBlur(filter1, filter2, ratio, inter);
        case FILTERTYPE_BEVEL:
            return interpolateBevel(filter1, filter2, ratio, inter);
        case FILTERTYPE_DROPSHADOW:
            return interpolateDropshadow(filter1, filter2, ratio, inter);
        case FILTERTYPE_GRADIENTGLOW:
            return interpolateGradientGlow(filter1, filter2, ratio, inter);
        default:
            syntaxerror("Filtertype %s not supported yet.\n", filtername[filter1->type]);
    }
	return 0;
}

FILTER* copyFilter(FILTER* original)
{
	if (!original)
		return original;
	FILTER* copy = swf_NewFilter(original->type);
	switch (original->type)
	{
		case FILTERTYPE_BLUR:
			memcpy(copy, original, sizeof(FILTER_BLUR));
			break;
		case FILTERTYPE_GRADIENTGLOW:
	{
			memcpy(copy, original, sizeof(FILTER_GRADIENTGLOW));
	    FILTER_GRADIENTGLOW* ggcopy = (FILTER_GRADIENTGLOW*)copy;
	    ggcopy->gradient = (GRADIENT*)malloc(sizeof(GRADIENT));
	    ggcopy->gradient->ratios = (U8*)malloc(16 * sizeof(U8));
	    ggcopy->gradient->rgba = (RGBA*)malloc(16 * sizeof(RGBA));
	    copyGradient(ggcopy->gradient, ((FILTER_GRADIENTGLOW*)original)->gradient);
	}
			break;
		case FILTERTYPE_DROPSHADOW:
			memcpy(copy, original, sizeof(FILTER_DROPSHADOW)); 
			break;
		case FILTERTYPE_BEVEL:
			memcpy(copy, original, sizeof(FILTER_BEVEL)); 
			break;
	default: syntaxerror("Internal error: unsupported filterype");
	}
	return copy;
}

int changeFilter_differs(changeFilter_t* modification, U16 frame)
{
    changeFilter_t* previous = modification;
    while (modification && modification->frame < frame)
	{
	previous = modification;
	modification = modification->next;
	}
    if (!modification)
	return 0;
    if (modification->frame == frame)
    	return 1;
    return (modification->function != CF_JUMP);
}

FILTER* changeFilter_value(changeFilter_t* modification, U16 frame)
{
    changeFilter_t* previous = modification;
    while (modification && modification->frame < frame)
    {
	previous = modification;
	modification = modification->next;
    }
    if (!modification)
		return copyFilter(previous->value);
    if (modification->frame == frame)
	{
		do 
		{
	    previous = modification;
	    modification = modification->next;
		}
	while (modification && modification->frame == frame);
	return copyFilter(previous->value);
	}
    switch (modification->function)
	{
		case CF_PUT:
	    return copyFilter(modification->value);
		case CF_CHANGE:
		{
	    float fraction = (frame - previous->frame) / (float)(modification->frame - previous->frame);
	    return interpolateFilter(previous->value, modification->value, fraction, modification->interpolation);
		}
		case CF_JUMP:
			return copyFilter(previous->value);
		default:
			return 0;
	}
}

history_t* history_new()
{
	history_t* newHistory = (history_t*)malloc(sizeof(history_t));
	history_init(newHistory);
	return newHistory;
}

void history_free(history_t* past)
{
	change_free(dictionary_lookup(past->changes, "x"));
	change_free(dictionary_lookup(past->changes, "y"));
	change_free(dictionary_lookup(past->changes, "scalex"));
	change_free(dictionary_lookup(past->changes, "scaley"));
	change_free(dictionary_lookup(past->changes, "cxform.r0"));
	change_free(dictionary_lookup(past->changes, "cxform.g0"));
	change_free(dictionary_lookup(past->changes, "cxform.b0"));
	change_free(dictionary_lookup(past->changes, "cxform.a0"));
	change_free(dictionary_lookup(past->changes, "cxform.r1"));
	change_free(dictionary_lookup(past->changes, "cxform.g1"));
	change_free(dictionary_lookup(past->changes, "cxform.b1"));
	change_free(dictionary_lookup(past->changes, "cxform.a1"));
	change_free(dictionary_lookup(past->changes, "rotate"));
	change_free(dictionary_lookup(past->changes, "shear"));
	change_free(dictionary_lookup(past->changes, "pivot.x"));
	change_free(dictionary_lookup(past->changes, "pivot.y"));
	change_free(dictionary_lookup(past->changes, "pin.x"));
	change_free(dictionary_lookup(past->changes, "pin.y"));
	change_free(dictionary_lookup(past->changes, "blendmode"));
	changeFilter_free(dictionary_lookup(past->changes, "filter"));
	dictionary_destroy(past->changes);
	free(past);
}

void history_init(history_t* past)
{
	past->changes = (dictionary_t*)malloc(sizeof(dictionary_t));
	dictionary_init(past->changes);
}

void history_begin(history_t* past, char* parameter, U16 frame, TAG* tag, float value)
{
	change_t* first = change_new(frame, CF_PUT, value, 0);
	past->firstTag = tag;
	past->firstFrame = frame;
	dictionary_put2(past->changes, parameter, first);
}

void history_beginFilter(history_t* past, U16 frame, TAG* tag, FILTER* value)
{
	changeFilter_t* first = changeFilter_new(frame, CF_PUT, value, 0);
	past->firstTag = tag;
	past->firstFrame = frame;
	dictionary_put2(past->changes, "filter", first);
}

void history_remember(history_t* past, char* parameter, U16 frame, int function, float value, interpolation_t* inter)
{
	change_t* first = dictionary_lookup(past->changes, parameter);
	if (first) //should always be true
	{
		change_t* next = change_new(frame, function, value, inter);
		change_append(first, next);
	}
}

void history_rememberFilter(history_t* past, U16 frame, int function, FILTER* value, interpolation_t* inter)
{
	changeFilter_t* first = dictionary_lookup(past->changes, "filter");
	if (first) //should always be true
	{
		changeFilter_t* next = changeFilter_new(frame, function, value, inter);
		changeFilter_append(first, next);
	}
}

int history_change(history_t* past, U16 frame, char* parameter)
{
    change_t* first = dictionary_lookup(past->changes, parameter);
    if (first)	//should always be true.
	return change_differs(first, frame);
    printf("no history found for parameter %s\n", parameter);
    return 0;
}

float history_value(history_t* past, U16 frame, char* parameter)
{
	change_t* first = dictionary_lookup(past->changes, parameter);
	if (first)	//should always be true.
		return change_value(first, frame);
	printf("no history found for parameter %s\n", parameter);
	return 0;
}

int history_changeFilter(history_t* past, U16 frame)
{
    changeFilter_t* first = dictionary_lookup(past->changes, "filter");
    if (first)	//should always be true.
	return changeFilter_differs(first, frame);
    printf("no history found for parameter filter\n");
    return 0;
}

FILTER* history_valueFilter(history_t* past, U16 frame)
{
	changeFilter_t* first = dictionary_lookup(past->changes, "filter");
	if (first)	//should always be true.
		return changeFilter_value(first, frame);
	printf("no history found for parameter filter\n");
	return 0;
}
