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
	while (first->next)
		first = first->next;
	first->next = newChange;
}

float interpolateParameter(float p1, float p2, float fraction, interpolation_t* inter)
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

float change_value(change_t* first, U16 frame)
{
	change_t* previous = first;
	while (first && first->frame < frame)
	{
		previous = first;
		first = first->next;
	}
	if (!first)
		return previous->value;
	if (first->frame == frame)
	{
		float result;
		do 
		{
			result = first->value;
			first = first->next;
		}
		while (first && first->frame == frame);
		return result;
	}
	switch (first->function)
	{
		case CF_PUT:
			return first->value;
		case CF_CHANGE:
		{
			float fraction = (frame - previous->frame) / (float)(first->frame - previous->frame);
			return interpolateParameter(previous->value, first->value, fraction, first->interpolation);
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
    c.r = c1.r * (1-ratio) + c2.r * ratio;
    c.g = c1.g * (1-ratio) + c2.g * ratio;
    c.b = c1.b * (1-ratio) + c2.b * ratio;
    c.a = c1.a * (1-ratio) + c2.a * ratio;
    return c;
}

FILTER* interpolateFilter(FILTER* filter1,FILTER* filter2, float ratio, interpolation_t* inter)
{
	if(!filter1 && !filter2)
		return 0;
	if(!filter1)
		return interpolateFilter(filter2,filter1,1-ratio, inter);

	if(filter2 && filter2->type != filter1->type)
		syntaxerror("can't interpolate between %s and %s filters yet", filtername[filter1->type], filtername[filter2->type]);
   
	if(filter1->type == FILTERTYPE_BLUR)
	{
		FILTER_BLUR*f1 = (FILTER_BLUR*)filter1;
		FILTER_BLUR*f2 = (FILTER_BLUR*)filter2;
		if(f2 && f1->blurx == f2->blurx && f1->blury == f2->blury)
			return 0;
		FILTER_BLUR*f = (FILTER_BLUR*)swf_NewFilter(FILTERTYPE_BLUR);
		f->blurx= (f1->blurx)*(1-ratio) + (f2?f2->blurx:0)*ratio;
		f->blury= (f1->blury)*(1-ratio) + (f2?f2->blury:0)*ratio;
		f->passes= (f1->passes)*(1-ratio) + (f2?f2->passes:0)*ratio;
		return (FILTER*)f;
		}
	else
		if (filter1->type == FILTERTYPE_DROPSHADOW)
		{
			FILTER_DROPSHADOW*f1 = (FILTER_DROPSHADOW*)filter1;
			FILTER_DROPSHADOW*f2 = (FILTER_DROPSHADOW*)filter2;
			if(f2 && !memcmp(&f1->color,&f2->color,sizeof(RGBA)) && f1->strength == f2->strength && 
				f1->blurx == f2->blurx && f1->blury == f2->blury && 
				f1->angle == f2->angle && f1->distance == f2->distance)
				return 0;
			FILTER_DROPSHADOW*f = (FILTER_DROPSHADOW*)swf_NewFilter(FILTERTYPE_DROPSHADOW);
			memcpy(f, f1, sizeof(FILTER_DROPSHADOW));
			f->color = interpolateColor(f1->color, f2->color, ratio, inter);
			f->blurx= (f1->blurx)*(1-ratio) + (f2?f2->blurx:0)*ratio;
			f->blury= (f1->blury)*(1-ratio) + (f2?f2->blury:0)*ratio;
			f->passes= (f1->passes)*(1-ratio) + (f2?f2->passes:0)*ratio;
			f->angle= (f1->angle)*(1-ratio) + (f2?f2->angle:0)*ratio;
			f->distance= (f1->distance)*(1-ratio) + (f2?f2->distance:0)*ratio;
			f->strength= (f1->strength)*(1-ratio) + (f2?f2->strength:0)*ratio;
			return (FILTER*)f;
		}
		else
			if (filter1->type == FILTERTYPE_BEVEL)
			{
				FILTER_BEVEL*f1 = (FILTER_BEVEL*)filter1;
				FILTER_BEVEL*f2 = (FILTER_BEVEL*)filter2;
				if(f2 && !memcmp(&f1->shadow,&f2->shadow,sizeof(RGBA)) && 
					!memcmp(&f1->highlight,&f2->highlight,sizeof(RGBA)) && 
					f1->blurx == f2->blurx && f1->blury == f2->blury && f1->angle == f2->angle && f1->strength == f2->strength && f1->distance == f2->distance)
					return 0;
				FILTER_BEVEL*f = (FILTER_BEVEL*)swf_NewFilter(FILTERTYPE_BEVEL);
				memcpy(f, f1, sizeof(FILTER_BEVEL));
				f->shadow = interpolateColor(f1->shadow, f2->shadow, ratio, inter);
				f->highlight = interpolateColor(f1->highlight, f2->highlight, ratio, inter);
				f->blurx= (f1->blurx)*(1-ratio) + (f2?f2->blurx:0)*ratio;
				f->blury= (f1->blury)*(1-ratio) + (f2?f2->blury:0)*ratio;
				f->passes= (f1->passes)*(1-ratio) + (f2?f2->passes:0)*ratio;
				f->angle= (f1->angle)*(1-ratio) + (f2?f2->angle:0)*ratio;
				f->distance= (f1->distance)*(1-ratio) + (f2?f2->distance:0)*ratio;
				f->strength= (f1->strength)*(1-ratio) + (f2?f2->strength:0)*ratio;
				return (FILTER*)f;
			} /*else if (filter1->type == FILTERTYPE_GRADIENTGLOW) {
	FILTER_GRADIENTGLOW*f = (FILTER_GRADIENTGLOW*)swf_NewFilter(FILTERTYPE_GRADIENTGLOW);
	// can't interpolate gradients
	memcpy(f, filter1, sizeof(FILTER_GRADIENTGLOW));
	return (FILTER*)f;
    }*/ else
				syntaxerror("can't interpolate %s filters yet", filtername[filter1->type]);
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
			memcpy(copy, original, sizeof(FILTER_GRADIENTGLOW));
			break;
		case FILTERTYPE_DROPSHADOW:
			memcpy(copy, original, sizeof(FILTER_DROPSHADOW)); 
			break;
		case FILTERTYPE_BEVEL:
			memcpy(copy, original, sizeof(FILTER_BEVEL)); 
			break;
		default: printf("unsupported filterype");
	}
	return copy;
}

FILTER* changeFilter_value(changeFilter_t* first, U16 frame)
{
	changeFilter_t* previous = first;
	while (first && first->frame < frame)
	{
		previous = first;
		first = first->next;
	}
	if (!first)
		return copyFilter(previous->value);
	if (first->frame == frame)
	{
		FILTER* result;
		do 
		{
			result = first->value;
			first = first->next;
		}
		while (first && first->frame == frame);
		return copyFilter(result);
	}
	switch (first->function)
	{
		case CF_PUT:
			return copyFilter(first->value);
		case CF_CHANGE:
		{
			float fraction = (frame - previous->frame) / (float)(first->frame - previous->frame);
			return interpolateFilter(previous->value, first->value, fraction, first->interpolation);
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

float history_value(history_t* past, U16 frame, char* parameter)
{
	change_t* first = dictionary_lookup(past->changes, parameter);
	if (first)	//should always be true.
		return change_value(first, frame);
	printf("no history found for parameter %s\n", parameter);
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
