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
#include <math.h>
#include <memory.h>
#include "swfc-interpolation.h"

static inline float poly(float fraction, float start, float delta, float slope, int degree)
{
    return delta * ((1 - slope) * pow(fraction, degree) + slope * fraction) + start;
}

float linear(float fraction, float start, float delta)
{
    return poly(fraction, start, delta, 0, 1);
}

float quadIn(float fraction, float start, float delta, float slope)
{
    return poly(fraction, start, delta, slope, 2);
}

float quadOut(float fraction, float start, float delta, float slope)
{
    return quadIn(1 - fraction, start + delta, -delta, slope);
}

float quadInOut(float fraction, float start, float delta, float slope)
{
    if (fraction < 0.5)
        return quadIn(2 * fraction, start, delta / 2, slope);
    return quadOut(2 * fraction - 1, start + delta / 2, delta / 2, slope);
}

float cubicIn(float fraction, float start, float delta, float slope)
{
    return poly(fraction, start, delta, slope, 3);
}

float cubicOut(float fraction, float start, float delta, float slope)
{
    return cubicIn(1 - fraction, start + delta, -delta, slope);
}

float cubicInOut(float fraction, float start, float delta, float slope)
{
    if (fraction < 0.5)
        return cubicIn(2 * fraction, start, delta / 2, slope);
    return cubicOut(2 * fraction - 1, start + delta / 2, delta / 2, slope);
}

float quartIn(float fraction, float start, float delta, float slope)
{
    return poly(fraction, start, delta, slope, 4);
}

float quartOut(float fraction, float start, float delta, float slope)
{
    return quartIn(1 - fraction, start + delta, -delta, slope);
}

float quartInOut(float fraction, float start, float delta, float slope)
{
    if (fraction < 0.5)
        return quartIn(2 * fraction, start, delta / 2, slope);
    return quartOut(2 * fraction - 1, start + delta / 2, delta / 2, slope);
}

float quintIn(float fraction, float start, float delta, float slope)
{
    return poly(fraction, start, delta, slope, 5);
}

float quintOut(float fraction, float start, float delta, float slope)
{
    return quintIn(1 - fraction, start + delta, -delta, slope);
}

float quintInOut(float fraction, float start, float delta, float slope)
{
    if (fraction < 0.5)
        return quintIn(2 * fraction, start, delta / 2, slope);
    return quintOut(2 * fraction - 1, start + delta / 2, delta / 2, slope);
}

float circleIn(float fraction, float start, float delta, float slope)
{
    return delta * (1 - sqrt(1 - (1 - 2 * slope) * fraction * fraction - 2 * slope * fraction)) + start;
}

float circleOut(float fraction, float start, float delta, float slope)
{
    return circleIn(1 - fraction, start + delta, -delta, slope);
}

float circleInOut(float fraction, float start, float delta, float slope)
{
    if (fraction < 0.5)
        return circleIn(2 * fraction, start, delta / 2, slope);
    return circleOut(2 * fraction - 1, start + delta / 2, delta / 2, slope);
}

float exponentialIn(float fraction, float start, float delta)
{
    if (fraction == 0)
        return start;
    return delta * pow(2, 10 * (fraction - 1)) + start;
}

float exponentialOut(float fraction, float start, float delta)
{
    return exponentialIn(1 - fraction, start + delta, -delta);
}

float exponentialInOut(float fraction, float start, float delta)
{
    if (fraction < 0.5)
        return exponentialIn(2 * fraction, start, delta / 2);
    return exponentialOut(2 * fraction - 1, start + delta / 2, delta / 2);
}

float sineIn(float fraction, float start, float delta)
{
    return delta * (1 - cos(fraction * M_PI/2)) + start;
}

float sineOut(float fraction, float start, float delta)
{
    return sineIn(1 - fraction, start + delta, -delta);
}

float sineInOut(float fraction, float start, float delta)
{
    if (fraction < 0.5)
        return sineIn(2 * fraction, start, delta / 2);
    return sineOut(2 * fraction - 1, start + delta / 2, delta / 2);
}

float elasticIn(float fraction, float start, float delta, float amplitude, int bounces, float damping)
{
    if (fraction == 0 || delta == 0)
        return start;
    if (fraction == 1)
        return start + delta;
    if (amplitude < fabs(delta))
        amplitude = delta;
    float period = 1 / (bounces + 0.25);
    return amplitude * pow(2, damping * (fraction - 1)) * sin(fraction * (2 * M_PI) / period) + start;
}

float elasticOut(float fraction, float start, float delta, float amplitude, int bounces, float damping)
{
    return elasticIn(1 - fraction, start + delta, -delta, amplitude, bounces, damping);
}
    
float elasticInOut(float fraction, float start, float delta, float amplitude, int bounces, float damping)
{
    if (fraction < 0.5)
        return elasticIn(2 * fraction, start, delta / 2, amplitude, bounces, damping);
    return elasticOut(2 * fraction - 1, start + delta / 2, delta / 2, amplitude, bounces, damping);
}

float backIn(float fraction, float start, float delta, float speed)
{
    return delta * fraction * fraction * ((speed + 1) * fraction - speed) + start;
}

float backOut(float fraction, float start, float delta, float speed)
{
    return backIn(1 - fraction, start + delta, -delta, speed);
}

float backInOut(float fraction, float start, float delta, float speed)
{
    if (fraction < 0.5)
        return backIn(2 * fraction, start, delta / 2, speed);
    return backOut(2 * fraction - 1, start + delta / 2, delta / 2, speed);
}

/* when applied to movement bounceIn the object 'hits the floor' bounces times 
 * (after leaving the floor first) before gently reaching the final position at the top of the final bounce
 * Each bounce takes growth times a long as the previous, except for the last one which lasts only half 
 * that time. The heights of the intermediate bounces are determined by the damping parameter.
 * Set damping to 0 for an undamped movement.*/

float bounceIn(float fraction, float start, float delta, int bounces, float growth, float damping)
{
    if (fraction == 0 || delta == 0)
        return start;
    if (fraction == 1)
        return start + delta;
    float w0;
    if (growth == 1.0)
        w0 = 1 / (bounces + 0.5);
    else
    {
        float gN = pow(growth, bounces);
        w0 = 1 / ((gN - 1) / (growth - 1) + gN / 2 );
    }
    float bounceStart = 0;
    int i;
    float w = w0;
    for (i = 0; i <= bounces; i++)
    {
        float bounceEnd = bounceStart + w;
        if (fraction >= bounceStart && fraction < bounceEnd)
        {
            float half = (bounceEnd + bounceStart) / 2;
            float top = delta / pow(2, damping * ((bounces - i)));
            fraction -= half;
            fraction /= (w / 2);
            return (1 - fraction * fraction) * top + start;
        }
        bounceStart = bounceEnd;
        w = w * growth;
    }
    return w;
}

/* bounceOut is a time-reversed bounceIn; therefore each bounce takes 1/growth times as long as
 * the previous, which I think fits the idea when applied to movement */

float bounceOut(float fraction, float start, float delta, int bounces, float growth, float damping)
{
    return bounceIn(1 - fraction, start + delta, -delta, bounces, growth, damping);
}

/* since bounceIn and bounceOut are combined, if growth > 1 then the bounce-times will increase in
 * the first half and decrease in the second half */

float bounceInOut(float fraction, float start, float delta, int bounces, float growth, float damping)
{
    if (fraction < 0.5)
        return bounceIn(2 * fraction, start, delta / 2, bounces, growth, damping);
    return bounceOut(2 * fraction - 1, start + delta / 2, delta / 2, bounces, growth, damping);
}
/* fastBounce(In/Out) doesn't end or start in a horizontal slope (= gentle end or start) as
 * bounce(In/Out) do which means fastBounceInOut doesn't have the 'delay' in the middle */
float fastBounceIn(float fraction, float start, float delta, int bounces, float growth, float damping)
{
    if (fraction == 0 || delta == 0)
        return start;
    if (fraction == 1)
        return start + delta;
    float w0;
    if (growth == 1.0)
        w0 = 1 / (bounces + 0.25); /* in general (bounces + 1 / (2 * f)) */
    else
    {
        float gN = pow(growth, bounces);
        w0 = 1 / ((gN - 1) / (growth - 1) + gN / 4 /* in general: gN / (2 * f) */ );
    }
    float bounceStart = 0;
    int i;
    float w = w0;
    for (i = 0; i <= bounces; i++)
    {
        float bounceEnd = bounceStart + w;
        if (fraction >= bounceStart && fraction < bounceEnd)
        {
            float half = (bounceEnd + bounceStart) / 2;
            float top = delta / 0.75/* in general: (1 - (1 / f) * (1 / f)) */ / pow(2, damping * ((bounces - i)));
            fraction -= half;
            fraction /= (w / 2);
            return (1 - fraction * fraction) * top + start;
        }
        bounceStart = bounceEnd;
        w = w * growth;
    }
    return 0;
}

float fastBounceOut(float fraction, float start, float delta, int bounces, float growth, float damping)
{
    return fastBounceIn(1 - fraction, start + delta, -delta, bounces, growth, damping);
}

float fastBounceInOut(float fraction, float start, float delta, int bounces, float growth, float damping)
{
    if (fraction < 0.5)
        return fastBounceIn(2 * fraction, start, delta / 2, bounces, growth, damping);
    return fastBounceOut(2 * fraction - 1, start + delta / 2, delta / 2, bounces, growth, damping);
}
