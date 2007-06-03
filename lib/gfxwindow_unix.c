/* gfxwindow.h 

   Simple GUI abstraction- Unix implementation

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

#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>
#include <memory.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xresource.h>
#include <X11/xpm.h>
#include <X11/Xthreads.h>
#include <X11/Xatom.h>
#include <X11/extensions/XShm.h>
#include "gfxwindow.h"
#include "types.h"

#define USE_SHM 1
#define USE_PIXMAP 1 // use pixmap instead of ximage for off-screen rendering

#define initMutex(mutex) {pthread_mutex_init(mutex, 0);}
#define destroyMutex(mutex) {pthread_mutex_destroy((mutex));}
#define lockMutex(mutex) pthread_mutex_lock(mutex)
#define unlockMutex(mutex) pthread_mutex_unlock(mutex)

typedef struct _queue
{
    char* data;
    int readpos;
    int writepos;
    int number;
    int size;
    int nmemb;
    pthread_mutex_t mutex;
} queue_t;


queue_t* queue_init(int size, int nmemb)
{
    queue_t* queue = malloc(sizeof(queue_t));
    queue->data = (char*)malloc(size * nmemb);
    queue->size = size;
    queue->nmemb = nmemb;
    queue->readpos = 0;
    queue->writepos = 0;
    queue->number = 0;
    initMutex(&queue->mutex);
    return queue;
}

void queue_destroy(queue_t*queue)
{
    free(queue->data);queue->data = 0;
    destroyMutex(&queue->mutex);
    memset(queue, 0, sizeof(queue_t));
}

int queue_put(queue_t*queue, void*data)
{
    int tmp = queue->writepos+1;
    tmp%=queue->nmemb;

    lockMutex(&queue->mutex);
    if(tmp==queue->readpos) {
	unlockMutex(&queue->mutex);
	return 0;
    }
    memcpy(&queue->data[queue->writepos*queue->size], data, queue->size);
    queue->writepos = tmp;
    queue->number++;
    unlockMutex(&queue->mutex);
    return 1;
}

int queue_get(queue_t*queue, void*data)
{
    lockMutex(&queue->mutex);
    if(queue->writepos == queue->readpos) {
	unlockMutex(&queue->mutex);
	return 0;
    }
    memcpy(data, &queue->data[queue->readpos*queue->size], queue->size);
    queue->readpos++;
    queue->readpos %= queue->nmemb;
    queue->number--;
    unlockMutex(&queue->mutex);
    return 1;
}

void queue_flush(queue_t*queue)
{
    lockMutex(&queue->mutex);
    queue->number = 0;
    queue->readpos = 0;
    queue->writepos = 0;
    unlockMutex(&queue->mutex);
}

int queue_num(queue_t*queue)
{
    return queue->number;
}

static int initializexwindows();

struct _QXWindow;
struct X
{
    char isnative24;
    char isnative16;
    char isnative8;
    int bpp;
    int bypp;
    Atom atom_wm;
    Atom atom_wmdelete;

    int rootsizex;
    int rootsizey;

    Display *d;
    Window root;
    int s;
    Visual*v;

    char running;
    pthread_t eventhandler;
    pthread_mutex_t xmutex;

    struct _QXWindow*windowring;

    int stopall;
    Colormap cmap;
    U32 white;
    U32 black;
}; 
static struct X X;

static int initialized = 0;
   
void destroyX() {
    X.stopall=1;
    if(X.running)
      pthread_cancel(X.eventhandler);
    if(X.d)
      XCloseDisplay(X.d);
    pthread_mutex_destroy(&X.xmutex);
}

typedef struct _QXWindow {
    struct _QXWindow*next;
    struct _QXWindow*prev;

    XWMHints *xwmh;
    XClassHint *xch;

    GC gc;
    //XFontStruct *xfs;

    XSetWindowAttributes xswa;
    XWindowAttributes xwa;

#ifdef USE_PIXMAP
    Pixmap winbuf;
#else
    XImage* winbuf;
#endif

    U8*currentscr;

    int x,y;
    int lenx,leny;
    int bpp;
    int bypp;
    Window mywin;
    XGCValues xgcv;
    XSizeHints *xsh;
    
    pthread_mutex_t wmutex;

    queue_t* queue;
} QXWindow;

static int lastbit(U32 bits)
{
    int t;
    for(t=0;t<32;t++)
    {
	if(bits&1) break;
	bits>>=1;
    }
    return t;
}

static int firstbit(U32 bits)
{
    int t,s=-1;
    for(t=0;t<32;t++)
    {
	if(bits&1) s=t;
	bits>>=1;
    }
    return s;
}

static U32 shiftl(U32 bits,int shift)
{
    if(shift>0) return bits<<shift;
    else	return bits>>-shift;
}

static U32 getColorFromRGB(U8 r,U8 g,U8 b)
{
    U32 ret=0;
    ret|=shiftl(r,(firstbit(X.v->red_mask)-7))&X.v->red_mask;
    ret|=shiftl(g,(firstbit(X.v->green_mask)-7))&X.v->green_mask;
    ret|=shiftl(b,(firstbit(X.v->blue_mask)-7))&X.v->blue_mask;
    return ret;
}

static void whatever()
{
            XrmValue xrmvalue;
            XrmDatabase rdb;
            return;
}

static void*eventloop(void*);
static int debug;
static int initializexwindows()
{
    if(initialized)
	return 1;
    X.windowring=0;
    X.d=0;
    X.running=0;
    pthread_mutex_init(&X.xmutex, 0);

    //whatever();
    /* open display, and show some information */
    X.d=XOpenDisplay(0);//getenv("DISPLAY"));
    if(!X.d) {return 0;}
/*    printf("Xprotocol V%d.%d Vendor:%s Name:%s Defaults:%s",X.d->proto_major_version,
     			       X.d->proto_minor_version,
			       X.d->vendor,X.d->display_name,X.d->defaults);
    int t;
    printf("Formats:\n");
    for(t=0;t<X.d->nformats;t++) {
	ScreenFormat*sf=&X.d->pixmap_formats[t];
	printf("%d:depth:%d, bpp:%d, scanline:%d\n",sf->depth,sf->bits_per_pixel,sf->scanline_pad);
    }
    printf("Screens:\n");
    for(t=0;t<X.d->nscreens;t++) {
	Screen*s=&X.d->screens[t];
	printf("%d*%d, bpp:%d\n",s->width,s->height,s->root_depth);
    }*/

    /* open screen */
    X.s=DefaultScreen(X.d);
    if(debug) printf("Display:\n");
    int xbig=DisplayWidth(X.d, X.s);
    int ybig=DisplayHeight(X.d, X.s);
    X.rootsizex=xbig;
    X.rootsizey=ybig;
    int bppbig=X.bpp=DisplayPlanes(X.d, X.s);
    if(X.bpp==24) X.bypp=4; //Can this be 3, also?
    if(X.bpp==16) X.bypp=2;
    if(X.bpp==8) X.bypp=1;
    if(debug) printf("%d*%d:%d/%d/%d\n",xbig,ybig,bppbig, X.bpp, X.bypp);
    if(debug) printf("%d screens, vendor:%s, Ver:%d.%d(%d), at %s\n",
    ScreenCount(X.d),
    ServerVendor(X.d),
    ProtocolVersion(X.d),
    ProtocolRevision(X.d),
    VendorRelease(X.d),
    DisplayString(X.d));

    /* open visual */
    X.v=XDefaultVisualOfScreen(DefaultScreenOfDisplay(X.d));

    X.isnative24 = (bppbig==24 && X.v->red_mask==0xff0000 &&
	              X.v->green_mask==0x00ff00 &&
		      X.v->blue_mask==0x0000ff);
    X.isnative16 = (bppbig==16 && X.v->red_mask==0xf800 &&
	              X.v->green_mask==0x07e0 &&
		      X.v->blue_mask==0x1f);
    X.isnative8 = (bppbig==8);
    if(debug) printf("8bpp:%d 16bpp:%d 24(32)bpp:%d\n",X.isnative8,X.isnative16,X.isnative24);

    X.root=DefaultRootWindow(X.d);
    if(!X.root) {return 0;}

    X.atom_wm = XInternAtom( X.d, "WM_PROTOCOLS", False );
    X.atom_wmdelete = XInternAtom( X.d, "WM_DELETE_WINDOW", False );

    X.cmap=DefaultColormap(X.d,X.s);
    X.white=WhitePixel(X.d,X.s);
    X.black=BlackPixel(X.d,X.s);

    /* start event handler thread */
    X.running = 1;
    pthread_create(&X.eventhandler,0,eventloop,0);

    X.stopall=0;
    
    initialized=1;
    return 1;
}

//needed to set the border attribute
#define MWM_HINTS_FUNCTIONS     (1L << 0)
#define MWM_HINTS_DECORATIONS   (1L << 1)
#define MWM_HINTS_INPUT_MODE    (1L << 2)
#define MWM_HINTS_STATUS        (1L << 3)

#define MWM_FUNC_ALL            (1L << 0)
#define MWM_FUNC_RESIZE         (1L << 1)
#define MWM_FUNC_MOVE           (1L << 2)
#define MWM_FUNC_MINIMIZE       (1L << 3)
#define MWM_FUNC_MAXIMIZE       (1L << 4)
#define MWM_FUNC_CLOSE          (1L << 5)

#define MWM_DECOR_ALL           (1L << 0)
#define MWM_DECOR_BORDER        (1L << 1)
#define MWM_DECOR_RESIZEH       (1L << 2)
#define MWM_DECOR_TITLE         (1L << 3)
#define MWM_DECOR_MENU          (1L << 4)
#define MWM_DECOR_MINIMIZE      (1L << 5)
#define MWM_DECOR_MAXIMIZE      (1L << 6)

#define MWM_INPUT_MODELESS 0
#define MWM_INPUT_PRIMARY_APPLICATION_MODAL 1
#define MWM_INPUT_SYSTEM_MODAL 2
#define MWM_INPUT_FULL_APPLICATION_MODAL 3
#define MWM_INPUT_APPLICATION_MODAL MWM_INPUT_PRIMARY_APPLICATION_MODAL

typedef struct _mwmhints {
  unsigned int flags;
  unsigned int functions;
  unsigned int decorations;
  int input_mode;
  unsigned int status;
} MWMHints;

static QXWindow* openwindow(int posx,int posy,int w,int h,char*winName)
{
    int noborder = 0;
    XTextProperty iname;
    XTextProperty wname;

    pthread_mutex_lock(&X.xmutex);

    if(!initialized) {
	fprintf(stderr, "Error: XWindows not yet initialized!\n");
	pthread_mutex_unlock(&X.xmutex);
	return 0;
    }

    QXWindow*qx= malloc(sizeof(QXWindow));
    if(!X.windowring) {
	qx->next=qx;
	qx->prev=qx;
	X.windowring=qx;
    } else {
	qx->next=X.windowring;
	qx->prev=X.windowring->prev;
	qx->prev->next=qx;
	X.windowring->prev=qx;
	X.windowring=qx;
    }

    char*iconName=winName;
    qx->x = posx;
    qx->y = posy;
    qx->lenx=w;
    qx->leny=h;

    qx->mywin=0;
    qx->mywin=XCreateSimpleWindow(X.d,X.root,posx,posy,qx->lenx,qx->leny,0,X.white,X.black);

    if(!qx->mywin) {
	printf("Error: Couldn't create SimpleWindow\n");
	pthread_mutex_unlock(&X.xmutex);
	return 0;
    }
    qx->xwmh=XAllocWMHints();
    qx->xsh=XAllocSizeHints();
    qx->xch=XAllocClassHint();

    qx->xsh->flags=(PPosition|PSize|PMinSize|PMaxSize);
    qx->xsh->height=qx->xsh->min_height=qx->xsh->max_height=h;
    qx->xsh->width =qx->xsh->min_width =qx->xsh->max_width =w;
    qx->xsh->x=0;
    qx->xsh->y=0;

    if(!XStringListToTextProperty(&winName, 1, &wname)) {
	printf("Error: XStringToTextProperty failed\n");
	pthread_mutex_unlock(&X.xmutex);
	return 0;
    }
    if(!XStringListToTextProperty(&iconName, 1, &iname)) {
	printf("Error: XStringToTextProperty failed\n");
	pthread_mutex_unlock(&X.xmutex);
	return 0;
    }

    XSetWMProperties(X.d,qx->mywin,&wname,&iname,0,0,qx->xsh,qx->xwmh,qx->xch);

    qx->xgcv.foreground=X.white;
    qx->xgcv.background=X.black;
    qx->gc=XCreateGC(X.d,qx->mywin,GCForeground|GCBackground,&qx->xgcv);

    XGetWindowAttributes(X.d,qx->mywin,&qx->xwa);

    // enable events for close window
    XChangeProperty(X.d, qx->mywin, X.atom_wm, XA_ATOM, 32, PropModeReplace, (unsigned char *)(&X.atom_wmdelete), 1 );

    // no border
    // (This code comes from the project xine (xine.sourceforge.net)
    //  I wonder if this works with all windowmanagers, though)
    if(noborder>0)
    {
	Atom atomprop = XInternAtom(X.d, "_MOTIF_WM_HINTS", True);
	if(atomprop)
	{
	    MWMHints mwmhints;
	    mwmhints.flags = MWM_HINTS_DECORATIONS;
	    if(noborder==1) mwmhints.decorations = 2;
	    if(noborder>1) mwmhints.decorations = 0;
	    XChangeProperty(X.d, qx->mywin, atomprop, atomprop, 32,
			    PropModeReplace, (unsigned char *) &mwmhints, 5);
	    XSetTransientForHint (X.d, qx->mywin, X.root);
	}
    }

    qx->bpp=qx->xwa.depth;
    if(qx->bpp==24) qx->bypp=4; //Can this be 3, also?
    if(qx->bpp==16) qx->bypp=2;
    if(qx->bpp==8) qx->bypp=1;

#ifdef USE_SHM
    XShmSegmentInfo segInfo;
    int bpl = qx->lenx*qx->bypp;
    segInfo.readOnly = 0;
    segInfo.shmid = shmget(IPC_PRIVATE,qx->leny*bpl,IPC_CREAT|0777);
    struct shmid_ds buf;
    if (segInfo.shmid <0) {
	perror("shmget");
	fprintf(stderr,"Size = %d x %d\n", qx->lenx, qx->leny);
    }
    segInfo.shmaddr = (char*)shmat (segInfo.shmid, 0, 0);
    if ((long)segInfo.shmaddr == -1) {
	perror("shmat");
    }
    XShmAttach(X.d, &segInfo);
    if (shmctl(segInfo.shmid, IPC_RMID, &buf) < 0) 
	perror("shmctl");

    qx->currentscr = (U8*)segInfo.shmaddr;

# ifdef USE_PIXMAP
    qx->winbuf = XShmCreatePixmap(X.d,qx->mywin,segInfo.shmaddr,&segInfo,qx->lenx,qx->leny,DefaultDepth(X.d, DefaultScreen(X.d)));
# else 
    qx->winbuf = XShmCreateImage(X.d,X.v,24,ZPixmap,(char*)segInfo.shmaddr, &segInfo, qx->lenx,qx->leny);
    XInitImage(qx->winbuf);
# endif

#else

# ifdef USE_PIXMAP
    qx->winbuf = XCreatePixmap(X.d,qx->mywin,qx->lenx,qx->leny, DefaultDepth(X.d,X.s));
# else
    qx->currentscr=malloc(qx->lenx*qx->leny*4);
    qx->winbuf = XCreateImage(X.d,X.v,24,ZPixmap,0,(char*)qx->currentscr,qx->lenx,qx->leny,8,qx->lenx*4);
    XInitImage(qx->winbuf);
# endif

#endif
    XSync(X.d, False);

    /* get the image data for the area this window will be/is placed on */
    /*{
	Window tmpwin;
	XImage*background;
	int x,y;
	XTranslateCoordinates(X.d,qx->mywin,X.root,0,0,&x,&y,&tmpwin);
	qx->background = XGetImage(X.d, DefaultRootWindow(X.d), x, y, qx->lenx, qx->leny, AllPlanes, ZPixmap);
	printf("%d %d\n",x,y);
    }*/
    
    XMapRaised(X.d,qx->mywin);
    XFlush(X.d);

    qx->xswa.event_mask=(qx->xwa.your_event_mask|=
	    ButtonPressMask|
	    ButtonReleaseMask|
	    //ExposureMask|
	    KeyPressMask|
    	    MotionNotify|
    	    EnterWindowMask|
    	    LeaveWindowMask|
    	    PointerMotionMask
	);
    XChangeWindowAttributes(X.d,qx->mywin,CWEventMask,&qx->xswa);

    qx->queue = queue_init(sizeof(gfxevent_t), 256);
    
    pthread_mutex_unlock(&X.xmutex);
	
    pthread_mutex_init(&qx->wmutex, 0);
    return qx;
}

static void closeWindow(QXWindow*qx)
{
    if(qx->mywin) {
	pthread_mutex_lock(&X.xmutex);
	XDestroyWindow(X.d,qx->mywin);
	pthread_mutex_unlock(&X.xmutex);
	qx->mywin = 0;
    }
}

static void processEvent(gfxevent_t*event)
{
    fd_set fdset;
    struct timeval de;
    int status;

    XEvent xe;

    FD_ZERO(&fdset);
    FD_SET(ConnectionNumber(X.d),&fdset);
    de.tv_sec = 0;
    de.tv_usec = 5000; // 1/200 s

    pthread_mutex_lock(&X.xmutex);
    FD_ZERO(&fdset);
    FD_SET(ConnectionNumber(X.d),&fdset);
    de.tv_sec = de.tv_usec = 0;
    if(!select(ConnectionNumber(X.d)+1, &fdset, 0, 0, &de)) {
	pthread_mutex_unlock(&X.xmutex);
	usleep(1000);
	return;
    }
    XNextEvent(X.d,&xe);
    pthread_mutex_unlock(&X.xmutex);
    
    /* 1: find out which windows the message is for */
    Window w = 0;
    switch(xe.type)
    {
	case ClientMessage: w = xe.xclient.window;break;
	case Expose:	    w = xe.xexpose.window;break;
	case NoExpose:	    w = xe.xexpose.window;break;
	case ButtonPress:   w = xe.xbutton.window;break;
	case ButtonRelease: w = xe.xbutton.window;break;
	case KeyPress:	    w = xe.xkey.window;   break;
	case KeyRelease:    w = xe.xkey.window;   break;
	case MotionNotify:  w = xe.xmotion.window;break;
	case EnterNotify:   w = xe.xmotion.window;break;
	case LeaveNotify:   w = xe.xmotion.window;break;
	default:
	/* this usually happens for unknown events which
	   we don't care about */
	    return;
    }

    QXWindow*qx=X.windowring;
    QXWindow*win=0;
    event->internal = 0;
    do {
	if(w == qx->mywin) {
	    win = qx;
	}
    }
    while(qx!=X.windowring);
    
    if(!win) {
	/* Not one of our windows. This may be a bug */
	return;
    }
    event->internal = win;

    /* 2: Go for the event construction */

    switch(xe.type)
    {
	case ButtonPress:
	    event->type = GFXEVENT_MOUSEPRESS;
	    event->button = xe.xbutton.button;
	break;
	case ButtonRelease:
	    event->type = GFXEVENT_MOUSERELEASE;
	    event->button = xe.xbutton.button;
	break;
	case KeyPress:
	    event->type = GFXEVENT_KEYPRESS;
	    event->key = xe.xkey.keycode;
	break;
	case KeyRelease:
	    event->type = GFXEVENT_KEYRELEASE;
	    event->key = xe.xkey.keycode;
	break;
	case MotionNotify:
	    event->type = GFXEVENT_MOUSEMOVE;
	    pthread_mutex_lock(&qx->wmutex);
	    event->x = xe.xmotion.x_root - win->x;
	    event->y = xe.xmotion.y_root - win->y;
	    if(!queue_put(win->queue, event)) {
		printf("Queue overflow for window %08x!\n", win);
	    }
	    pthread_mutex_unlock(&qx->wmutex);
	break;
	case EnterNotify:
	    event->type = GFXEVENT_MOUSEENTER;
	    pthread_mutex_lock(&qx->wmutex);
	    event->x = xe.xmotion.x_root - win->x;
	    event->y = xe.xmotion.y_root - win->y;
	    pthread_mutex_unlock(&qx->wmutex);
	break;
	case LeaveNotify:
	    event->type = GFXEVENT_MOUSELEAVE;
	    pthread_mutex_lock(&qx->wmutex);
	    event->x = xe.xmotion.x - win->x;
	    event->y = xe.xmotion.y - win->y;
	    pthread_mutex_unlock(&qx->wmutex);
	break;
	case ClientMessage:
	    if ((xe.xclient.message_type == X.atom_wm ) &&
		((unsigned)xe.xclient.data.l[0] == X.atom_wmdelete) ) {
	        pthread_mutex_lock(&qx->wmutex);
		closeWindow(win);
		event->type = GFXEVENT_DESTROY;
	        pthread_mutex_unlock(&qx->wmutex);
	    }
	break;
    }
}

static void*eventloop(void*r)
{
    //while(!XEventsQueued(X.d,QueuedAfterReading))
    while(X.running)
    {
	gfxevent_t event;
	event.type = GFXEVENT_NOTHING;
	event.key = event.x = event.y = event.button = -1;
	processEvent(&event);
	if(event.type != GFXEVENT_NOTHING && event.type != GFXEVENT_MOUSEMOVE) {
	    QXWindow*win = (QXWindow*)event.internal;
	    pthread_mutex_lock(&win->wmutex);
	    if(!queue_put(win->queue,&event)) {
		fprintf(stderr, "Queue overflow for window %08x!\n", win);
	    }
	    pthread_mutex_unlock(&win->wmutex);
	}
    }
    return 0;
}

typedef struct _internal {
    GC gc;
    char*name;
    QXWindow*qx;
    int lenx, leny;
    int tmplenx, tmpleny;
    void*currentscr2;
} internal_t;

static void gfxwindow_on(gfxwindow_t*win)
{
    internal_t*i = (internal_t*)win->internal;
	 
    i->qx=openwindow(64, 64, win->width, win->height, i->name);
    if(!i->qx)
	return;
    i->currentscr2=0;
    if(i->qx->bypp != 4) {
	fprintf(stderr, "Warning: Not a native 32 bit screen, using second buffer\n");
	i->currentscr2 = malloc(win->width*win->height*4);
	win->currentscr = (unsigned char*)i->currentscr2;
    } else {
	win->currentscr = i->qx->currentscr;
    }
    win->lastscr = 0;
}

static void gfxwindow_off(gfxwindow_t*win)
{
    internal_t*i = (internal_t*)win->internal;
    if(i->currentscr2)
	free(i->currentscr2);
    closeWindow(i->qx);
}

static void gfxwindow_flippage(gfxwindow_t*win)
{
    internal_t*i = (internal_t*)win->internal;
    pthread_mutex_lock(&X.xmutex);

    if(i->currentscr2) {
	if(i->qx->bypp==2 && X.isnative16) {
	    int t;
	    int l = win->width*win->height;
	    unsigned char*dest = i->qx->currentscr;
	    unsigned char*src = (unsigned char*)i->currentscr2;
	    do {
		dest[1] = (src[2]&0xf8)|(src[1]>>5);
		dest[0] = ((src[1]<<5)&0xe0)|(src[0]>>3);
		dest+=2;
		src+=4;
	    } while(--l);
	} else {
	    memcpy(i->qx->currentscr, i->currentscr2, i->lenx*i->leny*i->qx->bypp);
	}
    }

#ifdef USE_PIXMAP
    XSetFunction(X.d,i->qx->gc,GXcopy);
    XCopyArea(X.d,i->qx->winbuf,i->qx->mywin,i->qx->gc, 0, 0, i->qx->lenx,i->qx->leny, 0, 0);
    XFlush(X.d);
#else
    XPutImage(X.d,qx->mywin,qx->gc,qx->winbuf,0,0,0,0,lenx,leny);
#endif
    pthread_mutex_unlock(&X.xmutex);
}

void gfxwindow_setcolor(gfxwindow_t*win, U32 c)
{
    internal_t*i = (internal_t*)win->internal;
    pthread_mutex_lock(&X.xmutex);
    i->qx->xgcv.foreground=getColorFromRGB((U8)(c>>16),(U8)(c>>8),(U8)c);
    i->gc=XCreateGC(X.d,i->qx->mywin,GCForeground|GCBackground,&i->qx->xgcv);
    pthread_mutex_unlock(&X.xmutex);
}
void gfxwindow_putpixel(gfxwindow_t*win, int x, int y, unsigned char c)
{
    internal_t*i = (internal_t*)win->internal;
    if(((U32)x)<(U32)win->width && ((U32)y)<(U32)win->height)
	*(U32*)&win->currentscr[y*win->width*i->qx->bypp+x*i->qx->bypp]=c;
}
static gfxevent_t gfxwindow_getEvent(gfxwindow_t*win)
{
    internal_t*i = (internal_t*)win->internal;
    gfxevent_t event;
    pthread_mutex_lock(&i->qx->wmutex);
    if(!i->qx->queue || !queue_get(i->qx->queue,&event)) {
	event.type = GFXEVENT_NOTHING;
    } else if(event.type == GFXEVENT_DESTROY) {
	queue_destroy(i->qx->queue);i->qx->queue=0;
    }
    pthread_mutex_unlock(&i->qx->wmutex);
    return event;
}
   
static void gfxwindow_move(gfxwindow_t*win, int x,int y)
{
    internal_t*i = (internal_t*)win->internal;
    pthread_mutex_lock(&i->qx->wmutex);
    pthread_mutex_lock(&X.xmutex);
    XWindowAttributes a;
    i->qx->x += x;
    i->qx->y += y;
    XMoveResizeWindow(X.d, i->qx->mywin, i->qx->x, i->qx->y, i->tmplenx, i->tmpleny);

    queue_t* queue2 = queue_init(sizeof(gfxevent_t), 256);
    gfxevent_t event;
    /* HACK: now that we have moved the window, all mouse move events
       are outdated- remove them*/
    while(queue_get(i->qx->queue, &event)) {
	if(event.type!=GFXEVENT_MOUSEMOVE)
	    queue_put(queue2, &event);
    }
    queue_destroy(i->qx->queue);
    i->qx->queue = queue2;

    pthread_mutex_unlock(&X.xmutex);
    pthread_mutex_unlock(&i->qx->wmutex);
}

static void gfxwindow_resize(gfxwindow_t*win, int x,int y)
{
    internal_t*i = (internal_t*)win->internal;
    if(x>win->width || y>win->height) {
	fprintf(stderr, "resize: can only make windows smaller\n");
	return;
    }
    if(x<1) x=1;
    if(y<1) y=1;
    pthread_mutex_lock(&i->qx->wmutex);
    i->tmplenx=x;
    i->tmpleny=y;
    pthread_mutex_lock(&X.xmutex);
    XMoveResizeWindow(X.d, i->qx->mywin, i->qx->x, i->qx->y, i->tmplenx, i->tmpleny);
    pthread_mutex_unlock(&X.xmutex);
    pthread_mutex_unlock(&i->qx->wmutex);
}
static void gfxwindow_destroy(gfxwindow_t*win)
{
    internal_t*i = (internal_t*)win->internal;
    gfxwindow_off(win);
    pthread_mutex_destroy(&i->qx->wmutex);
}

gfxwindow_t* gfxwindow_new(int width, int height)
{
    gfxwindow_t*w = (gfxwindow_t*)malloc(sizeof(gfxwindow_t));
    if(!initializexwindows()) {
	fprintf(stderr, "Warning: Couldn't initialize X-Windows\n");
    }
    internal_t*i = malloc(sizeof(internal_t));
    w->internal = i;
    w->move = gfxwindow_move;
    w->resize = gfxwindow_resize;
    w->flippage = gfxwindow_flippage;
    w->getEvent = gfxwindow_getEvent;
    w->width = width;
    w->height = height;
    w->destroy = gfxwindow_destroy;

    i->lenx = i->tmplenx = width;
    i->leny = i->tmpleny = height;
    i->name = "";

    gfxwindow_on(w);
    return w;
};

