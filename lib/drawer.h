/*  drawer.h 
    part of swftools

    A generic structure for providing vector drawing.

    Copyright (C) 2003 Matthias Kramm <kramm@quiss.org>

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

#ifndef __drawer_h__
#define __drawer_h__

typedef struct _FPOINT
{
    float x,y;
} FPOINT;

typedef struct _drawer_t
{
    void*internal;
    
    FPOINT pos; //last "to"

    void (*setLineStyle)(struct _drawer_t*draw, void*linestyle);
    void (*setFillStyle)(struct _drawer_t*draw, void*fillstyle);

    void (*moveTo)(struct _drawer_t*draw, FPOINT * to);
    void (*lineTo)(struct _drawer_t*draw, FPOINT * to);
    void (*splineTo)(struct _drawer_t*draw, FPOINT*c, FPOINT * to);
    void (*finish)(struct _drawer_t*draw);

    void (*dealloc)(struct _drawer_t*draw);

} drawer_t;

void draw_cubicTo(drawer_t*drawer, FPOINT*  control1, FPOINT* control2, FPOINT*  to);
void draw_conicTo(drawer_t*drawer, FPOINT*  control, FPOINT*  to);
void draw_string(drawer_t*drawer, const char*code);

#endif
