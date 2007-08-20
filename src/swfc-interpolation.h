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

#ifndef __EASE_H
#define __EASE_H

typedef struct _interpolation {
	int function;
	float slope, speed, amplitude, growth, damping;
	int bounces;
} interpolation_t;

enum {
	IF_LINEAR = 1,
	IF_QUAD_IN = 2,
	IF_QUAD_OUT = 3,
	IF_QUAD_IN_OUT = 4,
	IF_CUBIC_IN = 5,
	IF_CUBIC_OUT = 6,
	IF_CUBIC_IN_OUT = 7,
	IF_QUART_IN = 8,
	IF_QUART_OUT = 9,
	IF_QUART_IN_OUT = 10,
	IF_QUINT_IN = 11,
	IF_QUINT_OUT = 12,
	IF_QUINT_IN_OUT = 13,
	IF_CIRCLE_IN = 14,
	IF_CIRCLE_OUT = 15,
	IF_CIRCLE_IN_OUT = 16,
	IF_EXPONENTIAL_IN = 17,
	IF_EXPONENTIAL_OUT = 18,
	IF_EXPONENTIAL_IN_OUT = 19,
	IF_SINE_IN = 20,
	IF_SINE_OUT = 21,
	IF_SINE_IN_OUT = 22,
	IF_ELASTIC_IN = 23,
	IF_ELASTIC_OUT = 24,
	IF_ELASTIC_IN_OUT = 25,
	IF_BACK_IN = 26,
	IF_BACK_OUT = 27,
	IF_BACK_IN_OUT = 28,
	IF_BOUNCE_IN = 29,
	IF_BOUNCE_OUT = 30,
	IF_BOUNCE_IN_OUT = 31,
	IF_FAST_BOUNCE_IN = 32,
	IF_FAST_BOUNCE_OUT = 33,
	IF_FAST_BOUNCE_IN_OUT = 34
};

float linear(float fraction, float start, float delta);

float quadIn(float fraction, float start, float delta, float slope);
float quadOut(float fraction, float start, float delta, float slope);
float quadInOut(float fraction, float start, float delta, float slope);

float cubicIn(float fraction, float start, float delta, float slope);
float cubicOut(float fraction, float start, float delta, float slope);
float cubicInOut(float fraction, float start, float delta, float slope);

float quartIn(float fraction, float start, float delta, float slope);
float quartOut(float fraction, float start, float delta, float slope);
float quartInOut(float fraction, float start, float delta, float slope);

float quintIn(float fraction, float start, float delta, float slope);
float quintOut(float fraction, float start, float delta, float slope);
float quintInOut(float fraction, float start, float delta, float slope);

float circleIn(float fraction, float start, float delta, float slope);
float circleOut(float fraction, float start, float delta, float slope);
float circleInOut(float fraction, float start, float delta, float slope);

float exponentialIn(float fraction, float start, float delta);
float exponentialOut(float fraction, float start, float delta);
float exponentialInOut(float fraction, float start, float delta);

float sineIn(float fraction, float start, float delta);
float sineOut(float fraction, float start, float delta);
float sineInOut(float fraction, float start, float delta);

float elasticIn(float fraction, float start, float delta, float amplitude, int bounces, float damping);
float elasticOut(float fraction, float start, float delta, float amplitude, int bounces, float damping);
float elasticInOut(float fraction, float start, float delta, float amplitude, int bounces, float damping);

float backIn(float fraction, float start, float delta, float speed);
float backOut(float fraction, float start, float delta, float speed);
float backInOut(float fraction, float start, float delta, float speed);

float bounceIn(float fraction, float start, float delta, int bounces, float growth, float damping);
float bounceOut(float fraction, float start, float delta, int bounces, float growth, float damping);
float bounceInOut(float fraction, float start, float delta, int bounces, float growth, float damping);

float fastBounceIn(float fraction, float start, float delta, int bounces, float growth, float damping);
float fastBounceOut(float fraction, float start, float delta, int bounces, float growth, float damping);
float fastBounceInOut(float fraction, float start, float delta, int bounces, float growth, float damping);

#endif
