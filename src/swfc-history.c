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

enum
{
    T_BEFORE,
    T_AFTER,
    T_SYMMETRIC
};

state_t* state_new(U16 frame, int function, float value, interpolation_t* inter)
{
    state_t* newState = (state_t*)malloc(sizeof(state_t));
    state_init(newState);
    newState->frame = frame;
    newState->function = function;
    newState->value = value;
    newState->interpolation = inter;
    return newState;
}

void state_free(state_t* state)
{
    if (state->next)
	state_free(state->next);
    free(state);
}

void state_init(state_t* state)
{
    memset(state, 0, sizeof(state_t));
}

state_t* state_at(state_t* state, U16 frame)
{
    while (state->next && state->next->frame < frame)
    	state = state->next;
    return state;
}

void state_append(state_t* state, state_t* newState)
{
    state_t* previous = 0;
    state_t* start = state;
    float p0, p1, m0, m1;
    int previous_frames = 0, state_frames, new_frames;

    while (state->next)
    {
    	if (previous)
    	    previous_frames = state->frame - previous->frame;
    	previous = state;
	state = state->next;
    }
    state->next = newState;
    new_frames = newState->frame - state->frame;
    if (state->function == CF_SCHANGE)
    {
    	state_frames = state->frame - previous->frame;
    	p0 = previous->value;
    	p1 = state->value;
    	if (previous->function == CF_SCHANGE)
    	    m0 = (3 * previous->spline.a + 2 * previous->spline.b + previous->spline.c) * state_frames / previous_frames ;
    	else
    	    if (previous->function == CF_CHANGE || previous->function == CF_SWEEP)
    	    	m0 = state_tangent(start, previous->frame, T_BEFORE) * state_frames;
    	    else
    		m0 = (state->value - previous->value);
    	if (newState->function == CF_SCHANGE)
    	    m1 = /*0.5 * */(newState->value - previous->value) * state_frames / (new_frames + state_frames);
    	else
    	    if (newState->function == CF_CHANGE || newState->function == CF_SWEEP)
    		m1 = state_tangent(previous, state->frame, T_AFTER) * state_frames;
    	    else
  		m1 = (newState->value - state->value);
    	state->spline.a = 2 * p0 + m0 - 2 * p1 + m1;
    	state->spline.b = -3 * p0 - 2 * m0 + 3 * p1 - m1;
    	state->spline.c = m0;
    	state->spline.d = p0;
//    	printf("p0: %f, p1: %f, m0: %f, m1: %f.\n", p0, p1, m0, m1);
//    	printf("a: %f, b: %f, c: %f, d: %f.\n", state->spline.a, state->spline.b, state->spline.c, state->spline.d);
    }
    if (newState->function == CF_SCHANGE)
    {
    	p0 = state->value;
    	p1 = newState->value;
    	if (state->function == CF_SCHANGE)
    	    m0 = m1;
    	else
    	    if (state->function == CF_CHANGE || state->function == CF_SWEEP)
    	    	m0 = state_tangent(start, state->frame, T_BEFORE) * new_frames;
    	    else
	    	m0 = (newState->value - state->value);
    	m1 = (newState->value - state->value);
    	newState->spline.a = 2 * p0 + m0 - 2 * p1 + m1;
    	newState->spline.b = -3 * p0 - 2 * m0 + 3 * p1 - m1;
    	newState->spline.c = m0;
    	newState->spline.d = p0;
//    	printf("p0: %f, p1: %f, m0: %f, m1: %f.\n", p0, p1, m0, m1);
//    	printf("a: %f, b: %f, c: %f, d: %f.\n", newState->spline.a, newState->spline.b, newState->spline.c, newState->spline.d);
    }
    }

void state_insert(state_t* state, state_t* newState)
{
    while (state->next && state->next->frame < newState->frame)
    	state = state->next;
    newState->next = state->next;
    state->next = newState;
    // if this is going to be used to insert CF_SCHANGE states it will have to be extended
    // as in state_append above. I know this is not necessary right now, so I'll be lazy.
}

float calculateSpline(state_t* modification, float fraction)
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
	case IF_QUAD_IN: return quadIn(fraction, p1, p2 - p1, inter->slope);
	case IF_QUAD_OUT: return quadOut(fraction, p1, p2 - p1, inter->slope);
	case IF_QUAD_IN_OUT: return quadInOut(fraction, p1, p2 - p1, inter->slope);
	case IF_CUBIC_IN: return cubicIn(fraction, p1, p2 - p1, inter->slope);
	case IF_CUBIC_OUT: return cubicOut(fraction, p1, p2 - p1, inter->slope);
	case IF_CUBIC_IN_OUT: return cubicInOut(fraction, p1, p2 - p1, inter->slope);
	case IF_QUART_IN: return quartIn(fraction, p1, p2 - p1, inter->slope);
	case IF_QUART_OUT: return quartOut(fraction, p1, p2 - p1, inter->slope);
	case IF_QUART_IN_OUT: return quartInOut(fraction, p1, p2 - p1, inter->slope);
	case IF_QUINT_IN: return quintIn(fraction, p1, p2 - p1, inter->slope);
	case IF_QUINT_OUT: return quintOut(fraction, p1, p2 - p1, inter->slope);
	case IF_QUINT_IN_OUT: return quintInOut(fraction, p1, p2 - p1, inter->slope);
	case IF_CIRCLE_IN: return circleIn(fraction, p1, p2 - p1, inter->slope);
	case IF_CIRCLE_OUT: return circleOut(fraction, p1, p2 - p1, inter->slope);
	case IF_CIRCLE_IN_OUT: return circleInOut(fraction, p1, p2 - p1, inter->slope);
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

float calculateSweep(state_t* modification, float fraction)
{
    arc_t* a = &(modification->arc);
    float angle = a->angle + fraction * a->delta_angle;
    if (a->X)
    	return a->cX + a->r * cos(angle);
    else
    	return a->cY + a->r * sin(angle);

}

int state_differs(state_t* modification, U16 frame)
{
    state_t* previous = modification;
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

float state_tangent(state_t* modification, U16 frame, int tangent)
{
    float deltaFrame = 0.1;
    switch (tangent)
{
    	case T_BEFORE:
    	    return (state_value(modification, frame) - state_value(modification, frame - deltaFrame)) / deltaFrame;
    	case T_AFTER:
    	    return (state_value(modification, frame + deltaFrame) - state_value(modification, frame)) / deltaFrame;
    	default:
    	    return (state_value(modification, frame + deltaFrame) - state_value(modification, frame - deltaFrame)) / (2 * deltaFrame);
    }
}

float state_value(state_t* modification, float frame)
{
    state_t* previous = modification;
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
	case CF_SCHANGE:
	{
	    float fraction = (frame - previous->frame) / (float)(modification->frame - previous->frame);
	    fraction = interpolateScalar(0, 1, fraction, modification->interpolation);
	    return calculateSpline(modification, fraction);
		}
	case CF_SWEEP:
	{
	    float fraction = (frame - previous->frame) / (float)(modification->frame - previous->frame);
	    fraction = interpolateScalar(0, 1, fraction, modification->interpolation);
	    return calculateSweep(modification, fraction);
	}
		case CF_JUMP:
			return previous->value;
		default:
			return 0;
	}
}

filterState_t* filterState_new(U16 frame, int function, FILTERLIST* value, interpolation_t* inter)
{
    filterState_t* newChange = (filterState_t*)malloc(sizeof(filterState_t));
    filterState_init(newChange);
	newChange->frame = frame;
	newChange->function = function;
	newChange->value = value;
	newChange->interpolation = inter;
	return newChange;
}

void filterState_free(filterState_t *change)
{
	if (change->next)
	filterState_free(change->next);
    free(change->value);
	free(change);
}

void filterState_init(filterState_t* change)
{
    memset(change, 0, sizeof(filterState_t));
}

void filterState_append(filterState_t* first, filterState_t* newChange)
{
	while (first->next)
		first = first->next;
    if (!first->value || !newChange->value)
	first->next = newChange;
    else
    {
    	int i, mergedCount = 0;
    	int common = first->value->num < newChange->value->num ? first->value->num : newChange->value->num;
    	for (i = 0; i < common; i++)
    	{
    	    mergedCount++;
    	    if (newChange->value->filter[i]->type != first->value->filter[i]->type)
    	    	mergedCount++;
    	}
    	mergedCount = mergedCount + first->value->num - common + newChange->value->num - common;
    	if (mergedCount > 8)
    	{
    	    char* list1;
    	    char* list2;
    	    char* newList;
    	    list1 = (char*)malloc(1);
    	    *list1 = '\0';
    	    for (i = 0; i < first->value->num; i++)
    	    {
		char*filter = filtername[first->value->filter[i]->type];
    	    	newList = (char*)malloc(strlen(list1) + strlen(filter) + 2);
    	    	strcpy(newList, list1);
		strcat(newList, "+");
		strcat(newList, filtername[first->value->filter[i]->type]);
    	    	free(list1);
    	    	list1 = newList;
    	    }
    	    list2 = (char*)malloc(1);
    	    *list2 = '\0';
    	    for (i = 0; i < newChange->value->num; i++)
    	    {
		char*filter = filtername[newChange->value->filter[i]->type];
    	    	newList = (char*)malloc(strlen(list2) + strlen(filter) + 2);
		strcpy(newList, list2);
		strcat(newList, "+");
		strcat(newList, filter);
    	    	free(list2);
    	    	list2 = newList;
    	    }
    	    syntaxerror("filterlists %s and %s cannot be interpolated.", list1, list2);
    	}
    	first->next = newChange;
    }
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
	default: syntaxerror("Internal error: unsupported filterype, cannot copy");
    }
    return copy;
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
	return copyFilter(filter1);
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
		return copyFilter(filter1);
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
		return copyFilter(filter1);
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
		return copyFilter(filter1);
	FILTER_GRADIENTGLOW*f = (FILTER_GRADIENTGLOW*)swf_NewFilter(FILTERTYPE_GRADIENTGLOW);
    memcpy(f, f1, sizeof(FILTER_GRADIENTGLOW));
    f->blurx= interpolateScalar(f1->blurx, (f2->blurx), ratio, inter);
    f->blury= interpolateScalar(f1->blury, (f2->blury), ratio, inter);
    f->passes= interpolateScalar(f1->passes, (f2->passes), ratio, inter);
    f->angle= interpolateScalar(f1->angle, (f2->angle), ratio, inter);
    f->distance= interpolateScalar(f1->distance, (f2->distance), ratio, inter);
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

FILTERLIST* copyFilterList(FILTERLIST* original)
{
	if (!original)
		return original;
    int i;
    FILTERLIST* copy = (FILTERLIST*)malloc(sizeof(FILTERLIST));
    copy->num = original->num;
    for (i = 0; i < copy->num; i++)
    	copy->filter[i] = copyFilter(original->filter[i]);
    return copy;
}

FILTER* noFilter(int type)
{
    switch (type)
	{
		case FILTERTYPE_BLUR:
    	    return (FILTER*)noBlur;
			break;
    	case FILTERTYPE_BEVEL:
    	    return (FILTER*)noBevel;
			break;
		case FILTERTYPE_DROPSHADOW:
    	    return (FILTER*)noDropshadow;
			break;
    	case FILTERTYPE_GRADIENTGLOW:
    	    return (FILTER*)noGradientGlow;
			break;
    	default:
    	    syntaxerror("Internal error: unsupported filtertype, cannot match filterlists");
	}
    return 0;
}

FILTERLIST* interpolateFilterList(FILTERLIST* list1, FILTERLIST* list2, float ratio, interpolation_t* inter)
{
    if (!list1 && !list2)
	return list1;
    FILTERLIST start, target, dummy;
    dummy.num = 0;
    if (!list1)
    	list1 = &dummy;
    if (!list2)
    	list2 = &dummy;
    int i, j = 0;
    int common = list1->num < list2->num ? list1->num : list2->num;
        for (i = 0; i < common; i++)
    {
    	start.filter[j] = list1->filter[i];
    	if (list2->filter[i]->type == list1->filter[i]->type)
    	{
    	    target.filter[j] = list2->filter[i];
    	    j++;
    	}
    	else
    	{
    	    target.filter[j] = noFilter(list1->filter[i]->type);
    	    j++;
    	    start.filter[j] = noFilter(list2->filter[i]->type);
    	    target.filter[j] = list2->filter[i];
    	    j++;
    	}
    }
    if (list1->num > common)
    	for (i = common; i < list1->num; i++)
    	{
    	    start.filter[j] = list1->filter[i];
    	    target.filter[j] = noFilter(list1->filter[i]->type);
    	    j++;
    	}
    if (list2->num > common)
    	for (i = common; i < list2->num; i++)
    	{
    	    start.filter[j] = noFilter(list2->filter[i]->type);
    	    target.filter[j] = list2->filter[i];
    	    j++;
    	}
    start.num = j;
    target.num = j;
    FILTERLIST* mixedList = (FILTERLIST*)malloc(sizeof(FILTERLIST));
    mixedList->num = j;
    for (i = 0; i < j; i++)
    	mixedList->filter[i] = interpolateFilter(start.filter[i], target.filter[i], ratio, inter);
    return mixedList;
}

int filterState_differs(filterState_t* modification, U16 frame)
{
    filterState_t* previous = modification;
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

FILTERLIST* filterState_value(filterState_t* modification, U16 frame)
{
    filterState_t* previous = modification;
    while (modification && modification->frame < frame)
    {
	previous = modification;
	modification = modification->next;
    }
    if (!modification)
	return copyFilterList(previous->value);
    if (modification->frame == frame)
	{
		do 
		{
	    previous = modification;
	    modification = modification->next;
		}
	while (modification && modification->frame == frame);
	return copyFilterList(previous->value);
	}
    switch (modification->function)
	{
		case CF_PUT:
	    return copyFilterList(modification->value);
		case CF_CHANGE:
		{
	    float fraction = (frame - previous->frame) / (float)(modification->frame - previous->frame);
	    return interpolateFilterList(previous->value, modification->value, fraction, modification->interpolation);
		}
		case CF_JUMP:
	    return copyFilterList(previous->value);
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
    state_free(dict_lookup(past->states, "x"));
    state_free(dict_lookup(past->states, "y"));
    state_free(dict_lookup(past->states, "scalex"));
    state_free(dict_lookup(past->states, "scaley"));
    state_free(dict_lookup(past->states, "cxform.r0"));
    state_free(dict_lookup(past->states, "cxform.g0"));
    state_free(dict_lookup(past->states, "cxform.b0"));
    state_free(dict_lookup(past->states, "cxform.a0"));
    state_free(dict_lookup(past->states, "cxform.r1"));
    state_free(dict_lookup(past->states, "cxform.g1"));
    state_free(dict_lookup(past->states, "cxform.b1"));
    state_free(dict_lookup(past->states, "cxform.a1"));
    state_free(dict_lookup(past->states, "rotate"));
    state_free(dict_lookup(past->states, "shear"));
    state_free(dict_lookup(past->states, "pivot.x"));
    state_free(dict_lookup(past->states, "pivot.y"));
    state_free(dict_lookup(past->states, "pin.x"));
    state_free(dict_lookup(past->states, "pin.y"));
    state_free(dict_lookup(past->states, "blendmode"));
    state_free(dict_lookup(past->states, "flags"));
    filterState_free(dict_lookup(past->states, "filter"));
    dict_destroy(past->states);
    free(past);
}

void history_init(history_t* past)
{
    past->states = (dict_t*)malloc(sizeof(dict_t));
    dict_init(past->states, 16);
}

void history_begin(history_t* past, char* parameter, U16 frame, TAG* tag, float value)
{
    state_t* first = state_new(frame, CF_PUT, value, 0);
	past->firstTag = tag;
	past->firstFrame = frame;
    dict_put2(past->states, parameter, first);
}

void history_beginFilter(history_t* past, U16 frame, TAG* tag, FILTERLIST* value)
{
    filterState_t* first = filterState_new(frame, CF_PUT, value, 0);
	past->firstTag = tag;
	past->firstFrame = frame;
    dict_put2(past->states, "filter", first);
}

void history_remember(history_t* past, char* parameter, U16 frame, int function, float value, interpolation_t* inter)
{
    past->lastFrame = frame;
    state_t* state = dict_lookup(past->states, parameter);
    if (state) //should always be true
    {
	state_t* next = state_new(frame, function, value, inter);
	state_append(state, next);
    }
    else
    	syntaxerror("Internal error: changing parameter %s, which is unknown for the instance.", parameter);
}

static float getAngle(float dX, float dY)
{
    float radius = sqrt(dX * dX + dY * dY);
    if (radius == 0)
    	return 0.0;
    if (dX >= 0)
    	if (dY > 0)
    	    return acos(dX / radius);
    	else
    	    return 2 * M_PI - acos(dX / radius);
    else
    	if (dY > 0)
    	    return M_PI - acos(-dX / radius);
    	else
    	    return M_PI + acos(-dX / radius);
}

static float getDeltaAngle(float angle1, float angle2, int clockwise)
	{
    	    if (!clockwise)
    	    {
    	    	if (angle1 > angle2)
    	    	    return angle2 - angle1;
    	    	else
    	    	    return angle2 - angle1 - 2 * M_PI;
    	    }
    	    else
    	    {
    	    	if (angle1 > angle2)
    	    	    return 2 * M_PI - angle1 + angle2;
    	    	else
    	    	    return angle2 - angle1;
	}
}

void history_rememberSweep(history_t* past, U16 frame, float x, float y, float r, int clockwise, int short_arc, interpolation_t* inter)
{
    float lastX, lastY, dX, dY;
    U16 lastFrame;

    past->lastFrame = frame;
    state_t* change = dict_lookup(past->states, "x");
    if (change) //should always be true
    {
    	while (change->next)
    	    change = change->next;
    	lastFrame = change->frame;
    	lastX = change->value;
    	change = dict_lookup(past->states, "y");
    	if (change) //should always be true
    	{
    	    while (change->next)
    	    	change = change->next;
    	    lastY = change->value;
    	    dX = x - lastX;
    	    dY = y - lastY;
    	    if (dX == 0 && dY == 0)
	    	syntaxerror("sweep not possible: startpoint and endpoint must not be equal");
   	    if ((dX) * (dX) + (dY) * (dY) > 4 * r * r)
	    	syntaxerror("sweep not possible: radius is to small");
    	    if (change->frame > lastFrame)
    	    {
    	    	lastFrame = change->frame;
    	    	history_remember(past, "x", lastFrame, CF_JUMP, lastX, 0);
    	    }
    	    else
    	    	if (change->frame < lastFrame)
    	    	    history_remember(past, "y", lastFrame, CF_JUMP, lastY, 0);
    	    float c1X, c1Y, c2X, c2Y;
    	    if (dX == 0) //vertical
    	    {
    	    	c1Y = c2Y = (lastY + y) / 2;
    	    	c1X = x + sqrt(r * r - (c1Y - y) * (c1Y - y));
    	    	c2X = 2 * x -c1X;
    	    }
    	    else
	    	if (dY == 0) //horizontal
    	    	{
    	    	    c1X = c2X = (lastX + x) / 2;
    	    	    c1Y = y +sqrt(r * r - (c1X - x) * (c1X - x));
    	    	    c2Y = 2 * y -c1Y;
    	    	}
    	    	else
    	    	{
    	    	    c1X = sqrt((r * r - (dX * dX + dY * dY) / 4) / (1 + dX * dX / dY / dY));
    	    	    c2X = -c1X;
    	    	    c1Y = -dX / dY * c1X;
    	    	    c2Y = -c1Y;
    	    	    c1X += (x + lastX) / 2;
    	    	    c2X += (x + lastX) / 2;
    	    	    c1Y += (y + lastY) / 2;
    	    	    c2Y += (y + lastY) / 2;
    	    	}
    	    float angle1, angle2, delta_angle, centerX, centerY;
    	    angle1 = getAngle(lastX - c1X, lastY - c1Y);
    	    angle2 = getAngle(x - c1X, y - c1Y);
    	    delta_angle = getDeltaAngle(angle1, angle2, clockwise);
    	    if ((short_arc && fabs(delta_angle) <= M_PI) || (! short_arc && fabs(delta_angle) >= M_PI))
    	    {
    	        centerX = c1X;
    	        centerY = c1Y;
    	    }
    	    else
    	    {
	        angle1 = getAngle(lastX - c2X, lastY - c2Y);
    		angle2 = getAngle(x - c2X, y - c2Y);
    	    	delta_angle = getDeltaAngle(angle1, angle2, clockwise);
    	    	centerX = c2X;
    	    	centerY = c2Y;
    	    }
    	    change = dict_lookup(past->states, "x");
	    state_t* nextX = state_new(frame, CF_SWEEP, x, inter);
	    nextX->arc.r = r;
	    nextX->arc.angle = angle1;
	    nextX->arc.delta_angle = delta_angle;
	    nextX->arc.cX = centerX;
	    nextX->arc.cY = centerY;
	    nextX->arc.X = 1;
	    state_append(change, nextX);
    	    change = dict_lookup(past->states, "y");
	    state_t* nextY = state_new(frame, CF_SWEEP, y, inter);
	    nextY->arc.r = r;
	    nextY->arc.angle = angle1;
	    nextY->arc.delta_angle = delta_angle;
	    nextY->arc.cX = centerX;
	    nextY->arc.cY = centerY;
	    nextY->arc.X = 0;
	    state_append(change, nextY);
    	}
    	else
    	    syntaxerror("Internal error: changing parameter y in sweep, which is unknown for the instance.");
    }
    else
    	syntaxerror("Internal error: changing parameter x in sweep, which is unknown for the instance.");
}

void history_rememberFilter(history_t* past, U16 frame, int function, FILTERLIST* value, interpolation_t* inter)
{
    past->lastFrame = frame;
    filterState_t* first = dict_lookup(past->states, "filter");
	if (first) //should always be true
	{
	filterState_t* next = filterState_new(frame, function, value, inter);
	filterState_append(first, next);
    }
    else
    	syntaxerror("Internal error: changing a filter not set for the instance.");
}

void history_processFlags(history_t* past)
// to be called after completely recording this history, before calculating any values.
{
    state_t* flagState = dict_lookup(past->states, "flags");
    state_t* nextState;
    U16 nextFlags, toggledFlags, currentFlags = (U16)flagState->value;
    while (flagState->next)
    {
    	nextState = flagState->next;
    	nextFlags = (U16)nextState->value;
    	toggledFlags = currentFlags ^ nextFlags;
    	if (toggledFlags & IF_FIXED_ALIGNMENT)
    	{ // the IF_FIXED_ALIGNMENT bit will change in the next state
    	    if (nextFlags & IF_FIXED_ALIGNMENT)
    	    { // the IF_FIXED_ALIGNMENT bit will be set
    	    	int onFrame = nextState->frame;
	    	state_t* rotations = dict_lookup(past->states, "rotate");
	    	nextState->params.instanceAngle = state_value(rotations, onFrame);
	    	state_t* resetRotate = state_new(onFrame, CF_JUMP, 0, 0);
	    	state_insert(rotations, resetRotate);
	    	if (onFrame == past->firstFrame)
	    	    onFrame++;
	    	state_t *x, *y;
	    	float dx, dy;
	    	do
	    	{
    	    	    x = dict_lookup(past->states, "x");
    	    	    dx = state_tangent(x, onFrame, T_SYMMETRIC);
    	    	    y = dict_lookup(past->states, "y");
    	    	    dy = state_tangent(y, onFrame, T_SYMMETRIC);
    	    	    onFrame++;
	    	}
	    	while (dx == 0 && dy == 0 && onFrame < past->lastFrame);
	    	if (onFrame == past->lastFrame)
    	    	    nextState->params.pathAngle = 0;
	    	else
    	    	    nextState->params.pathAngle = getAngle(dx, dy) / M_PI * 180;
    	    }
    	    else // the IF_FIXED_ALIGNMENT bit will be reset
    	    {
    	    	int offFrame = nextState->frame;
	    	state_t* rotations = dict_lookup(past->states, "rotate");
	    	state_t* setRotate = state_new(offFrame, CF_JUMP, flagState->params.instanceAngle + state_value(rotations, offFrame), 0);
	    	state_insert(rotations, setRotate);
    	    }
    	}
    	else // the IF_FIXED_ALIGNMENT bit will not change but some processing may be
    	     // required just the same
    	{
    	    if (nextFlags & IF_FIXED_ALIGNMENT)
    	    {
    	    	nextState->params.instanceAngle = flagState->params.instanceAngle;
    	    	nextState->params.pathAngle = flagState->params.pathAngle;
    	    }
    	}
// and so on for all the other bits.
    	flagState = nextState;
    	currentFlags = nextFlags;
	}
}

int history_change(history_t* past, U16 frame, char* parameter)
{
    state_t* first = dict_lookup(past->states, parameter);
    if (first)	//should always be true.
	return state_differs(first, frame);
    syntaxerror("no history found to predict changes for parameter %s.\n", parameter);
    return 0;
}

float history_value(history_t* past, U16 frame, char* parameter)
{
    state_t* state = dict_lookup(past->states, parameter);
    if (state)	//should always be true.
	return state_value(state, frame);
    syntaxerror("no history found to get a value for parameter %s.\n", parameter);
    return 0;
}

float history_rotateValue(history_t* past, U16 frame)
{
    state_t* rotations = dict_lookup(past->states, "rotate");
    if (rotations)	//should always be true.
    {
    	float angle = state_value(rotations, frame);
    	state_t* flags = dict_lookup(past->states, "flags");
    	U16 currentflags = state_value(flags, frame);
    	if (currentflags & IF_FIXED_ALIGNMENT)
    	{
	    flags = state_at(flags, frame);
	    if (frame == past->firstFrame)
	    	frame++;
	    state_t *x, *y;
	    float dx, dy, pathAngle;
	    do
	    {
    	    	x = dict_lookup(past->states, "x");
    	    	dx = state_value(x, frame) - state_value(x, frame - 1);
    	    	y = dict_lookup(past->states, "y");
    	    	dy = state_value(y, frame) - state_value(y, frame - 1);
    	    	frame--;
	    }
	    while (dx == 0 && dy == 0 && frame > past->firstFrame);
	    if (frame == past->firstFrame)
    	    	pathAngle = 0;
	    else
    	    	pathAngle = getAngle(dx, dy) / M_PI * 180;
    	    return angle + flags->params.instanceAngle + pathAngle - flags->params.pathAngle;
    	}
	else
	    return angle;
    }
    syntaxerror("no history found to get a value for parameter rotate.\n");
	return 0;
}

int history_changeFilter(history_t* past, U16 frame)
{
    filterState_t* first = dict_lookup(past->states, "filter");
    if (first)	//should always be true.
	return filterState_differs(first, frame);
    syntaxerror("no history found to predict changes for parameter filter.\n");
    return 0;
}

FILTERLIST* history_filterValue(history_t* past, U16 frame)
{
    filterState_t* first = dict_lookup(past->states, "filter");
	if (first)	//should always be true.
	return filterState_value(first, frame);
    syntaxerror("no history found to get a value for parameter filter.\n");
	return 0;
}
