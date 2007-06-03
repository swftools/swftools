/* gfxwindow.h 

   Simple GUI abstraction.

   Part of the swftools package.

   Copyright (c) 2005 Matthias Kramm <kramm@quiss.org> 

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

#ifndef __gfxwindow_h__
#define __gfxwindow_h__

#ifdef __cplusplus
extern "C" {
#endif

#define GFXEVENT_NOTHING 0

#define GFXEVENT_KEY 4
#define GFXEVENT_KEYPRESS 4
#define GFXEVENT_KEYRELEASE 5

#define GFXEVENT_MOUSEPRESS 8
#define GFXEVENT_MOUSERELEASE 9
#define GFXEVENT_MOUSEMOVE 10
#define GFXEVENT_MOUSEENTER 11
#define GFXEVENT_MOUSELEAVE 12

#define GFXEVENT_DESTROY 16

typedef struct _gfxevent
{
    void * internal;
    int type;
    int key; //if type == KEY
    int x,y; //if type == MOUSEMOVED
    int button; //if type = MOUSEPRESSED/RELEASED
} gfxevent_t;

typedef struct _gfxwindow
{
    void * internal;
    unsigned char* currentscr;
    unsigned char* lastscr;
    int width;
    int height;
    void (*flippage)(struct _gfxwindow*win);
    void (*move)(struct _gfxwindow*win,int x, int y);
    void (*resize)(struct _gfxwindow*win,int width, int height);
    gfxevent_t (*getEvent)(struct _gfxwindow*win);
    void (*destroy)(struct _gfxwindow*win);
} gfxwindow_t;

gfxwindow_t* gfxwindow_new(int width, int height);

#ifdef __cplusplus
}
#endif

#endif
