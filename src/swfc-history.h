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

#ifndef __HISTORY_H
#define __HISTORY_Y

#include "../lib/types.h"
#include "../lib/rfxswf.h"
#include "../lib/q.h"
#include "swfc-feedback.h"
#include "swfc-interpolation.h"

enum
{
	CF_PUT = 1,
	CF_CHANGE = 2,
    CF_SCHANGE = 3,
    CF_SWEEP = 4,
	CF_JUMP = 5	
};

#define SF_X 0x0001
#define SF_Y 0x0002
#define SF_SCALEX 0x0004
#define SF_SCALEY 0x0008
#define SF_CX_R 0x0010
#define SF_CX_G 0x0020
#define SF_CX_B 0x0040
#define SF_CX_A 0x0080
#define SF_ROTATE 0x0100
#define SF_SHEAR 0x0200
#define SF_PIVOT 0x0400
#define SF_PIN 0x0800
#define SF_BLEND 0x1000
#define SF_FILTER 0x2000
#define SF_ALL 0x3fff

#define IF_FIXED_ALIGNMENT 0x0001

FILTER* noFilters;
FILTER_BLUR* noBlur;
FILTER_BEVEL* noBevel;
FILTER_DROPSHADOW* noDropshadow;
FILTER_GRADIENTGLOW* noGradientGlow;

typedef struct _spline
{
    float a, b, c, d;
} spline_t;

typedef struct _arc
{
    float r, angle, delta_angle, cX, cY;
    int X; // boolean: 1 if this is for x; 0 if this is for y;
} arc_t;

typedef struct _flagparms
{
    float pathAngle, instanceAngle;
} flagparams_t;

typedef struct _state
{
	U16 frame;
	float value;
	int function;
	interpolation_t* interpolation;
    spline_t spline;
    arc_t arc;
    flagparams_t params;
    struct _state* next;
} state_t;

state_t* state_new(U16 frame, int function, float value, interpolation_t* inter);
void state_free(state_t* state);
void state_init(state_t* state);
state_t* state_at(state_t* state, U16 frame);
void state_append(state_t* state, state_t* newState);
void state_insert(state_t* state, state_t* newState);
float state_value(state_t* first, float frame);
float state_tangent(state_t* modification, U16 frame, int tangent);

typedef struct _filterState
{
	U16 frame;
    FILTERLIST* value;
	int function;
	interpolation_t* interpolation;
    struct _filterState* next;
} filterState_t;

filterState_t* filterState_new(U16 frame, int function, FILTERLIST* value, interpolation_t* inter);
void filterState_free(filterState_t* change);
void filterState_init(filterState_t* change);
void filterState_append(filterState_t* first, filterState_t* newChange);
FILTERLIST* filterState_value(filterState_t* first, U16 frame);

typedef struct _history
{
    U16 firstFrame, lastFrame;
	TAG* firstTag;
    dict_t* states;
} history_t;

history_t* history_new();
void history_free(history_t* past);
void history_init(history_t* past);
void history_begin(history_t* past, char* parameter, U16 frame, TAG* tag, float value);
void history_beginFilter(history_t* past, U16 frame, TAG* tag, FILTERLIST* value);
void history_remember(history_t* past, char* parameter, U16 frame, int function, float value, interpolation_t* inter);
void history_rememberSweep(history_t* past, U16 frame, float x, float y, float r, int clockwise, int short_arc, interpolation_t* inter);
void history_rememberFilter(history_t* past, U16 frame, int function, FILTERLIST* value, interpolation_t* inter);
void history_processFlags(history_t* past);
int history_change(history_t* past, U16 frame, char* parameter);
float history_value(history_t* past, U16 frame, char* parameter);
float history_rotateValue(history_t* past, U16 frame);
int history_changeFilter(history_t* past, U16 frame);
FILTERLIST* history_filterValue(history_t* past, U16 frame);

#endif
