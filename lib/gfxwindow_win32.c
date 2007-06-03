/* gfxwindow.h 

   Simple GUI abstraction- Windows implementation

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

#include <windows.h>
#include <commctrl.h>
#include <stdio.h>
#include <string.h>
#include "gfx.h"

static LRESULT CALLBACK WindowFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam);

static char szWinName[] = "MyWin";
static HDC hdc;
static HWND hwnd;
static char*text="";
static int textx;
static int texty;
static int a = 0;
static HDC dc, memDc;
static HBITMAP   piccy;

static int sizex=640;
static int sizey=388;
static char *screen;
static char *offscreen;
static int screensize;

static DWORD threadID;
static DWORD mainthreadID;
static HANDLE thread;
static HINSTANCE me,prev;
static int nWinMode;
static int posx,posy;
static RECT desktopSize;
static int initialized = 0;

static int cwidth=640,cheight=388;

#define DIB

static void openWindow(int _sizex, int _sizey)
{
    RECT r;int ok;

    sizex = _sizex;
    sizey = _sizey;
    
    ok = (int)GetClientRect(GetDesktopWindow(), &r);
    if(!ok) {
	r.left = r.top = 0;
	r.right = 1280;
	r.bottom = 1024;
    }
    desktopSize = r;

    hwnd=CreateWindow(szWinName,
		      0, //window title
		      WS_POPUP|WS_VISIBLE,
		      posx=100, //CW_USEDEFAULT,
		      posy=200, //CW_USEDEFAULT,
		      640,388,
		      HWND_DESKTOP,
		      NULL,me,NULL);
    SendMessage(hwnd, 1024+5 /*SB_SETBORDERS*/, 0, 0);
    
    ShowWindow(hwnd,nWinMode);
    UpdateWindow(hwnd);

    dc=GetDC(hwnd);
    memDc=CreateCompatibleDC(dc);
//    SetTimer(hwnd,1,1000,NULL);
//    SetTimer(hwnd,1,33,NULL);

#ifdef DIB
    void * ppvBits = 0;
    BITMAPINFO info;
    memset(&info, sizeof(info), 0);
    info.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
    info.bmiHeader.biWidth = sizex;
    info.bmiHeader.biHeight = sizey;
    info.bmiHeader.biPlanes= 1;
    info.bmiHeader.biBitCount = 24;
    info.bmiHeader.biCompression = BI_RGB;
    info.bmiHeader.biSizeImage = sizex*sizey*3;

    piccy = CreateDIBSection(0, &info, DIB_RGB_COLORS, &ppvBits, 0,0);
    screen = (char*)ppvBits;
    offscreen = (char*)malloc(sizex*sizey*4);
    screensize = sizex*sizey*3;
    GdiFlush();
#else
    screen = (char*)malloc(sizex*sizey*3);
    offscreen = (char*)malloc(sizex*sizey*4);
    screensize = sizex*sizey*3;
    piccy = CreateBitmap(sizex, sizey, 1, 24, screen);
#endif

    SelectObject(memDc,piccy);
    
}

static void closeWindow()
{
    DeleteDC(memDc);
    ReleaseDC(hwnd,dc);
    KillTimer(hwnd,1);
}

static int initwin32()
{
    MSG msg;
    int ret;
    WNDCLASS wcl;

    me =  GetModuleHandle(NULL);
    prev = NULL;
    nWinMode = SW_SHOW;

    if(GetClassInfo(0, szWinName, &wcl)) {
	/* already registered */
	exit(1);
    }
    wcl.hInstance    =me;
    wcl.lpszClassName=szWinName;
    wcl.lpfnWndProc  =WindowFunc;
    wcl.style        =2;
    wcl.hIcon        =LoadIcon  (NULL, IDI_HAND);
    wcl.hCursor      =LoadCursor(NULL, IDC_HAND);//IDC_CROSS);
    wcl.lpszMenuName =NULL;
    wcl.cbClsExtra   =0;
    wcl.cbWndExtra   =0;
    wcl.hbrBackground=(HBRUSH)GetStockObject(NULL_BRUSH);//WHITE_BRUSH);

    if(!RegisterClass(&wcl)) 
	exit(1);

    //mainthreadID = GetCurrentThreadId();

    //openWindow(640,388);

    //thread = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)(maincaller), (/*data*/0), 0, (LPDWORD)&(threadID));
  
    /*while(GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }*/

    //closeWindow();
    //ret = msg.wParam;

    initialized = 1;

    return ret;
}

#define Q_REDRAW 0x7f01
#define Q_DUMMY  0x7f02

static LRESULT CALLBACK WindowFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
 switch(message)
 {
   case WM_DESTROY:
    PostQuitMessage(0);
    break;
   case WM_PAINT:
    BitBlt(dc,0,0,sizex,sizey,memDc,0,0,SRCCOPY);
   break;
   case Q_DUMMY:
   break;
   case Q_REDRAW: {
    int x,y;
    for(y=0;y<sizey;y++) {
	unsigned char*o = (unsigned char*)&offscreen[y*sizex*4];
	unsigned char*s = (unsigned char*)&screen[(sizey-y-1)*sizex*3];
	for(x=0;x<sizex;x++) {
	    s[0] = o[0];
	    s[1] = o[1];
	    s[2] = o[2];
	    s+=3;
	    o+=4;
	}
    }

#ifndef DIB
    SetBitmapBits(piccy, screensize, screen);
    InvalidateRect(hwnd,NULL,1);
#endif

#ifdef DIB
    //SetDIBitsToDevice
    BitBlt(dc,0,0,sizex,sizey,memDc,0,0,SRCCOPY);
#else
    BitBlt(dc,0,0,sizex,sizey,memDc,0,0,SRCCOPY);
#endif
   break;
   }
   case WM_TIMER:
    //InvalidateRect(hwnd,NULL,1);
   default:
    return DefWindowProc(hwnd, message, wParam, lParam);
 }
 return 0;
}

static void ModeRGB_on(window_t*win)
{
    MSG msg;
    //printf("on()\n");
    openWindow(sizex, sizey);
    win->currentscr = (unsigned char*)offscreen;

    while(GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
	if(msg.message == WM_PAINT)
	    break;
    }
}
static void ModeRGB_off(window_t*win)
{
    MSG msg;
    //printf("off()\n");
    //TODO: shouldn't this be DestroyWindow(hwnd)?
    PostMessage(hwnd, WM_DESTROY, 0, 0);
    while(GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
	if(msg.message == WM_DESTROY)
	    break;
    }
    closeWindow();
    win->currentscr = win->lastscr = 0;
}
static void ModeRGB_flippage(window_t*win)
{
    MSG msg;
    PostMessage(hwnd, Q_REDRAW, 0, 0);
    while(GetMessage(&msg,NULL,0,0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
	if(msg.message == Q_REDRAW)
	    break;
    }
    win->currentscr = (byte*)offscreen;
}
static gfxevent_t ModeRGB_getEvent(window_t*win)
{
    MSG msg;
    WPARAM wParam;
    LPARAM lParam;
    gfxevent_t event;

    //PostMessage(hwnd, Q_DUMMY, 0, 0);

    event.type = GFXEVENT_NOTHING;
    event.key = 0;
    event.x = 0;
    event.y = 0;
    event.type = 0;
    event.button = 0;

    if(!PeekMessage(&msg,NULL,0,0,0))
	return event;//nothing

    while(GetMessage(&msg,NULL,0,0))
    {
	lParam = msg.lParam;
	wParam = msg.wParam;
	if(msg.message == Q_DUMMY) {
	    event.type = GFXEVENT_NOTHING;
	    break;
	} else if(msg.message == WM_LBUTTONDOWN) {
	    event.type = GFXEVENT_MOUSEPRESS;
	    event.button = 1;
	    event.x = (signed short int)LOWORD(lParam);
	    event.y = (signed short int)HIWORD(lParam);
	    SetCapture(hwnd);
	    break;
	} else if(msg.message == WM_LBUTTONUP) {
	    event.type = GFXEVENT_MOUSERELEASE;
	    event.button = 1;
	    event.x = (signed short int)LOWORD(lParam);
	    event.y = (signed short int)HIWORD(lParam);
	    ReleaseCapture();
	    break;
	} else if(msg.message == WM_MBUTTONDOWN) {
	    event.type = GFXEVENT_MOUSEPRESS;
	    event.button = 2;
	    event.x = (signed short int)LOWORD(lParam);
	    event.y = (signed short int)HIWORD(lParam);
	    SetCapture(hwnd);
	    break;
	} else if(msg.message == WM_MBUTTONUP) {
	    event.type = GFXEVENT_MOUSERELEASE;
	    event.button = 1;
	    event.x = (signed short int)LOWORD(lParam);
	    event.y = (signed short int)HIWORD(lParam);
	    ReleaseCapture();
	    break;
	} else if(msg.message == WM_RBUTTONDOWN) {
	    event.type = GFXEVENT_MOUSEPRESS;
	    event.button = 3;
	    event.x = (signed short int)LOWORD(lParam);
	    event.y = (signed short int)HIWORD(lParam);
	    SetCapture(hwnd);
	    break;
	} else if(msg.message == WM_RBUTTONUP) {
	    event.type = GFXEVENT_MOUSERELEASE;
	    event.button = 3;
	    event.x = (signed short int)LOWORD(lParam);
	    event.y = (signed short int)HIWORD(lParam);
	    ReleaseCapture();
	    break;
	} else if(msg.message == WM_MOUSEMOVE) {
	    event.type = GFXEVENT_MOUSEMOVE;
	    event.x = (signed short int)LOWORD(lParam);
	    event.y = (signed short int)HIWORD(lParam);
	    break;
	} else if(msg.message == WM_CHAR) {
	    event.type = GFXEVENT_KEYPRESS;
	    event.key = (char)wParam;
	    break;
	} else {
	    TranslateMessage(&msg);
	    DispatchMessage(&msg);
	    event.type = GFXEVENT_NOTHING;
	    break;
	}
    }
    return event;
}
static void ModeRGB_move(window_t*win, int relx,int rely)
{
    MSG msg;
    PostMessage(hwnd, Q_DUMMY, 0, 0);
    while(GetMessage(&msg,NULL,0,0))
    {
	if(msg.message == WM_LBUTTONUP) {
	    SendMessage(hwnd, msg.message, msg.wParam, msg.lParam);
	    return;
	}
        TranslateMessage(&msg);
        DispatchMessage(&msg);
	if(msg.message == Q_DUMMY)
	    break;
    }
    posx+=relx;
    posy+=rely;
   
    if(posx > desktopSize.right-cwidth) posx = desktopSize.right-cwidth;
    if(posy > desktopSize.bottom-cheight) posy = desktopSize.bottom-cheight;
    if(posx < desktopSize.left) posx = desktopSize.left;
    if(posy < desktopSize.top) posy = desktopSize.top;

    SetWindowPos(hwnd, HWND_TOP, posx, posy, 0, 0, SWP_NOSIZE);
    PostMessage(hwnd, Q_DUMMY, 0, 0);
    while(GetMessage(&msg,NULL,0,0))
    {
	if(msg.message == WM_LBUTTONUP) {
	    SendMessage(hwnd, msg.message, msg.wParam, msg.lParam);
	    break;;
	}
        TranslateMessage(&msg);
        DispatchMessage(&msg);
	if(msg.message == Q_DUMMY)
	    break;
    }
}
static void ModeRGB_resize(window_t*win, int width,int height)
{
    if(width>sizex || height>sizey) {
	printf("mode24::resize: can only make windows smaller\n");
	return;
    }
    if(width<1) width=1;
    if(height<1) height=1;
    cwidth = width;
    cheight = height;
    SetWindowPos(hwnd, HWND_TOP, 0, 0, width, height, SWP_NOMOVE);
    win->move(win,0,0);
}


window_t* window_new(window_t*win, int width, int height)
{
    window_t*w = (window_t*)malloc(sizeof(window_t));
    memset(w, 0, sizeof(window_t));
    if(!initialized)
	initwin32();
    w->currentscr = w->lastscr = 0;
    sizex = width;
    sizey = height;
    cwidth = sizex;
    cheight = sizey;
    //printf("contruct(%d, %d)\n", x, y);
    w->on = ModeRGB_on;
    w->off = ModeRGB_off;
    w->move = ModeRGB_move;
    w->resize = ModeRGB_resize;
    w->flippage = ModeRGB_flippage;
    w->getEvent = ModeRGB_getEvent;
    w->width = width;
    w->height = height;
    return w;
}

