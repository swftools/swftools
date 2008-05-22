/* installer.c

   Part of the swftools installer (Main program).
   
   Copyright (c) 2004 Matthias Kramm <kramm@quiss.org> 
 
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
#include <commdlg.h>
#include <shlobj.h>
#include <stdio.h>
#include <stdlib.h>

#include "depack.h"

#include "../config.h" //for swftools version
#include "../lib/os.h" //for registry functions

extern char*crndata;

static char*install_path = "c:\\swftools\\";
static char pathBuf[1024];
static int do_abort = 0;

static HWND wnd_params = 0;
static HWND wnd_progress = 0;
static HWND wnd_finish = 0;
static HWND wnd_background = 0;

static HBITMAP logo;

#define USER_SETMESSAGE 0x7f01

struct progress_data {
    int width,height;
    int bar_width;
    int bar_height;
    int bar_posx;
    int bar_posy;
    int pos,step,range;
    char*text1;
    char*text2;
    char*text3;
    HWND hwndButton;
    HWND wnd_text3;
};
struct params_data {
    int width,height;
    int ok;
    HWND installButton;
    HWND edit;
    HWND explore;
};
struct finish_data {
    int width,height;
    int ok;
    char*text;
    HWND installButton;
    HWND check1;
    HWND check2;
};

LRESULT CALLBACK WindowFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    //printf("%08x, %d %08x %08x\n", hwnd, message, wParam, lParam);
   
    /* in order for the delegation below to also work for
       WM_CREATE, we need to assign our window pointers *before* the
       CreateWindow returns, because that's when the WM_CREATE event 
       is sent  */

    if(message == WM_CREATE) {
	CREATESTRUCT*cs = ((LPCREATESTRUCT)lParam);
	if(cs->lpCreateParams && !strcmp((char*)cs->lpCreateParams, "params")) {
	    wnd_params = hwnd;
	}
        else if(cs->lpCreateParams && !strcmp((char*)cs->lpCreateParams, "progress")) {
	    wnd_progress = hwnd;
	}
        else if(cs->lpCreateParams && !strcmp((char*)cs->lpCreateParams, "finish")) {
	    wnd_finish = hwnd;
	}
        else if(cs->lpCreateParams && !strcmp((char*)cs->lpCreateParams, "background")) {
	    wnd_background = hwnd;
	}
    }

    if(hwnd == 0) {
	return DefWindowProc(hwnd, message, wParam, lParam);
    } else if(hwnd == wnd_progress) {
	static struct progress_data data;

	switch(message)
	{
	    case USER_SETMESSAGE:
		data.text3 = (char*)wParam;
		SendMessage(data.wnd_text3, WM_SETTEXT, 0, (LPARAM)data.text3);
		return 0;
	    case WM_CREATE: {
		memset(&data, 0, sizeof(data));
		data.text1 = "Installing SWFTools";
		data.text2 = (char*)malloc(strlen(install_path)+250);
                data.text3 = "";
		sprintf(data.text2, "to %s", install_path);
		data.pos = 0;
		data.step = 1;

		CREATESTRUCT*cs = ((LPCREATESTRUCT)lParam);
		RECT rc;
		GetClientRect (hwnd, &rc);
		data.width = rc.right - rc.left;
		data.height = rc.bottom - rc.top;
		data.bar_width = cs->cx - 17;
		data.bar_height= 16;
		data.bar_posx = (data.width -data.bar_width)/2;
		data.bar_posy = 56;
		data.range = 50;

		data.hwndButton = CreateWindow (
			PROGRESS_CLASS,
			"Progress",
			WS_CHILD | WS_VISIBLE,
			data.bar_posx,
			data.bar_posy,
			data.bar_width, 
			data.bar_height,
			hwnd,  /* Parent */
			0,
			cs->hInstance,
			NULL
			);

		SendMessage(data.hwndButton, PBM_SETRANGE, 0, (LPARAM) MAKELONG(0,data.range));
		SendMessage(data.hwndButton, PBM_SETSTEP, (WPARAM) data.step, 0);
		return 0;
	    }   
	    case PBM_STEPIT: {
		if(data.pos+data.step < data.range) {
		    data.pos += data.step;
		    SendMessage(data.hwndButton, PBM_STEPIT, wParam, lParam);
		}
	    }
	    case WM_PAINT: {
		TEXTMETRIC    tm;
		HDC           hdc;             /* A device context used for drawing */
		PAINTSTRUCT   ps;              /* Also used during window drawing */
		RECT          rc;              /* A rectangle used during drawing */
                
                hdc = GetDC(hwnd);
                SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
                GetTextMetrics(hdc, &tm);
                ReleaseDC(hwnd, hdc);

		hdc = BeginPaint (hwnd, &ps);

		/*
		// draw logo 
		HDC memDc=CreateCompatibleDC(hdc);
		SelectObject(memDc,logo);
		BitBlt(hdc,0,0,406,93,memDc,0,0,SRCCOPY);
		DeleteDC(memDc);
		// /
		*/

		SetBkMode(hdc, TRANSPARENT);
		
		rc.top = 8; rc.left= 0; rc.right = data.width; rc.bottom = 24;
		DrawText(hdc, data.text1, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		char buf[256];
		char*text = data.text2;
		if(tm.tmAveCharWidth * strlen(text) > data.width) {
		    int chars = (data.width / tm.tmAveCharWidth)-8;
		    if(chars>240) chars=240;
		    strncpy(buf, text, chars);
		    strcpy(&buf[chars],"...");
		    text = buf;
		}

		rc.top = 32; rc.left= 0; rc.right = data.width; rc.bottom = 48;
		DrawText(hdc, text, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		
                //rc.top = data.height-32; rc.left= 0; rc.right = data.width; rc.bottom = data.height;
		//DrawText(hdc, data.text3, -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);

		EndPaint (hwnd, &ps);
		return 0;
	    }
	    case WM_DESTROY:
		wnd_progress = 0;
		return DefWindowProc(hwnd, message, wParam, lParam);
	    default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
    } else if(hwnd == wnd_params) {
	static struct params_data data;
	switch(message)
	{
	    case WM_CREATE: {
		memset(&data, 0, sizeof(data));
		CREATESTRUCT*cs = ((LPCREATESTRUCT)lParam);
		RECT rc;
		GetClientRect (hwnd, &rc);
		data.width = rc.right - rc.left;
		data.height = rc.bottom - rc.top;

		//EDITTEXT IDD_EDIT,68,8,72,12, ES_LEFT | ES_AUTOHSCROLL | WS_CHILD | WS_VISIBLE | WS_BORDER  | WS_TABSTOP
		data.edit = CreateWindow (
			WC_EDIT,
			"EditPath",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | WS_BORDER | ES_AUTOHSCROLL,
			32, 
			48,
			(data.width-64)-32*2, 
			20,
			hwnd,  /* Parent */
			(HMENU)0x1234,
			cs->hInstance,
			NULL
			);
		
		data.explore = CreateWindow (
			WC_BUTTON,
			"Browse",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			data.width-32-64,
			48,
			64, 
			20,
			hwnd,  /* Parent */
			(HMENU)0x9999,
			cs->hInstance,
			NULL
			);
		
		data.installButton = CreateWindow (
			WC_BUTTON,
			"Install",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			(data.width - 80)/2,
			data.height - 32*2,
			80, 
			32,
			hwnd,  /* Parent */
			(HMENU)0xabcd,
			cs->hInstance,
			NULL
			);
		
		SendMessage(data.edit, WM_SETTEXT, 0, (LPARAM)install_path);
		return 0;
	    }   
	    case USER_SETMESSAGE: {
		//install_path = (char*)lParam;
		SendMessage(data.edit, WM_SETTEXT, 0, (LPARAM)install_path);
		printf("Setting path to %s\n", install_path);
		return 0;
	    }
	    case WM_PAINT: {
		TEXTMETRIC tm;
		HDC hdc;
		PAINTSTRUCT ps;
		RECT rc;
                hdc = GetDC(hwnd);
                SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
                GetTextMetrics(hdc, &tm);
                ReleaseDC(hwnd, hdc);
		hdc = BeginPaint (hwnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
		rc.top = 32; rc.left= 16; rc.right = data.width-32*2; rc.bottom = 20;
		DrawText(hdc, "Select Installation directory", -1, &rc, DT_SINGLELINE | DT_CENTER | DT_VCENTER);
		EndPaint (hwnd, &ps);
		return 0;
	    }
	    case WM_COMMAND: {
		if((wParam&0xffff) == 0x9999) {
		    BROWSEINFOA browse;
		    memset(&browse, 0, sizeof(browse));
		    browse.ulFlags = BIF_EDITBOX | BIF_NEWDIALOGSTYLE | BIF_USENEWUI;// | BIF_RETURNONLYFSDIRS; //BIF_VALIDATE
		    browse.pszDisplayName = (CHAR*)malloc(MAX_PATH);
		    memset(browse.pszDisplayName, 0, MAX_PATH);
		    browse.lpszTitle = "Select installation directory";
		    /*browse.pidlRoot = (ITEMIDLIST*)malloc(sizeof(ITEMIDLIST)*200);
		    memset((void*)browse.pidlRoot, 0, sizeof(ITEMIDLIST)*200);*/
		    printf("Start browsing %s\n", browse.pszDisplayName);
		    //SHGetDesktopFolder
		    //ParseDisplayName(install_path,0,&browse.pidlRoot,0,0);
		    //SHParseDisplayName(install_path,0,&browse.pidlRoot,0,0);
		    //SHBrowseForFolderA(&browse);
		    browse.pidlRoot = SHBrowseForFolder(&browse);
		    printf("Browsing returns %s / %08x\n", browse.pszDisplayName, browse.pidlRoot);
		    if(browse.pszDisplayName) {
			if(SHGetPathFromIDList(browse.pidlRoot, browse.pszDisplayName)) {
			    printf("Path is %s\n", browse.pszDisplayName);
			    install_path = browse.pszDisplayName;
			}
		    }
		    SendMessage(data.edit, WM_SETTEXT, 0, (LPARAM)install_path);
		    return 0;
		} else if((wParam&0xffff) == 0xabcd) {
		    data.ok = 1;
		    DestroyWindow(hwnd);
		    return 0;
		} else if((wParam&0xffff) == 0x1234) {
		    SendMessage(data.edit, WM_GETTEXT, sizeof(pathBuf), (LPARAM)&(pathBuf[0]));
		    if(pathBuf[0]) {
			install_path = pathBuf;
		    }
		    return 0;
		}
		return DefWindowProc(hwnd, message, wParam, lParam);
	    }
	    case WM_KEYDOWN: {
		if(wParam == 0x49) {
		    DestroyWindow(hwnd);
		}
		return 0;
	    }
	    case WM_DESTROY:
		if(!data.ok) {
		    do_abort = 1;
                    PostQuitMessage (0);
		}
		wnd_params = 0;
		return DefWindowProc(hwnd, message, wParam, lParam);
	    default:
		return DefWindowProc(hwnd, message, wParam, lParam);
	}
    } else if(hwnd == wnd_finish) {
	static struct finish_data data;
	switch(message)
	{
	    case WM_CREATE: {
		/* TODO:

		   "swftools has been installed into directory %s
		   successfully"

		   [x] Create Desktop Shortcut
		   [x] Create Start Menu Entry

		*/

		RECT rc;
		CREATESTRUCT*cs = ((LPCREATESTRUCT)lParam);
		GetClientRect (hwnd, &rc);
		data.width = rc.right - rc.left;
		data.height = rc.bottom - rc.top;

                data.text = malloc(strlen(install_path)+256);
                sprintf(data.text, "SWFTools has been installed into directory\r\n%s\r\nsucessfully.", install_path);
		
		data.installButton = CreateWindow (
			WC_BUTTON,
			"Finish",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP,
			(data.width - 80)/2,
			data.height - 32,
			80, 
			32,
			hwnd, 
			(HMENU)0xabce,
			cs->hInstance,
			NULL
			);
		
                data.check1 = CreateWindow (
			WC_BUTTON,
			"Create Desktop Shortcut",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_CHECKBOX,
			32, data.height - 96, 
                        data.width-32*2, 32, 
                        hwnd, (HMENU)0xabcf, cs->hInstance, NULL);
                
                data.check2 = CreateWindow (
			WC_BUTTON,
			"Create Start Menu Entry",
			WS_CHILD | WS_VISIBLE | WS_TABSTOP | BS_CHECKBOX,
			32, data.height - 64, 
                        data.width-32*2, 32, 
                        hwnd, (HMENU)0xabd0, cs->hInstance, NULL);
		
	    }
	    case WM_PAINT: {
		TEXTMETRIC tm;
		HDC hdc;
		PAINTSTRUCT ps;
		RECT rc;
                hdc = GetDC(hwnd);
                SelectObject(hdc, GetStockObject(DEFAULT_GUI_FONT));
                GetTextMetrics(hdc, &tm);
                ReleaseDC(hwnd, hdc);
		hdc = BeginPaint (hwnd, &ps);
		SetBkMode(hdc, TRANSPARENT);
                rc.left = 0; 
		rc.top = 10;
                rc.right = data.width; 
                rc.bottom = data.height-40-32;
		DrawText(hdc, data.text, -1, &rc, DT_CENTER | DT_VCENTER);
		EndPaint (hwnd, &ps);
		return 0;

		return DefWindowProc(hwnd, message, wParam, lParam);
	    }
	    case WM_COMMAND: {
		if((wParam&0xffff) == 0xabce) {
		    data.ok = 1;
		    DestroyWindow(hwnd);
		    return 0;
                }
		if((wParam&0xffff) == 0xabcf) {
		    return 1;
                }
		if((wParam&0xffff) == 0xabd0) {
		    return 0;
                }
            }
	    case WM_DESTROY: {
                free(data.text);data.text = 0;
		if(!data.ok) {
		    do_abort = 1;
                    PostQuitMessage(0);
		}
		wnd_finish = 0;
		return DefWindowProc(hwnd, message, wParam, lParam);
            }
	}
    } else if(hwnd == wnd_background) {
	switch(message)
	{
	    case WM_PAINT: {
		HDC hdc;
		PAINTSTRUCT ps;
		RECT rc;
		hdc = BeginPaint(hwnd, &ps);
		SetBkMode(hdc, TRANSPARENT);

                HPEN pen = CreatePen(PS_SOLID, 2, RGB(255, 255, 0));    
                HPEN oldPen = (HPEN)SelectObject(hdc, pen);

                MoveToEx(hdc, 10, 10, 0);
                LineTo(hdc, 100, 100);

                SelectObject(hdc, oldPen); 
                DeleteObject(pen);

		EndPaint(hwnd, &ps);
		return 1;
            }
        }
    }
    return DefWindowProc(hwnd, message, wParam, lParam);
}

void processMessages()
{
    MSG msg;
    while(PeekMessage(&msg,NULL,0,0,0))
    {
	GetMessage(&msg, NULL, 0, 0);
	TranslateMessage(&msg);
	DispatchMessage(&msg);
    }
}

static char*lastmessage = 0;
void myarchivestatus(int type, char*text)
{
    if(text && text[0]=='[')
	return;
    //printf("%s\n", text);
			
    SendMessage(wnd_progress, USER_SETMESSAGE, (WPARAM)strdup(text), 0);
    SendMessage(wnd_progress, WM_PAINT, 0, 0);
    int t;
    for(t=0;t<9;t++) {
	SendMessage(wnd_progress, PBM_STEPIT, 0, 0);
	/* while we're here, we might also make ourselves useful */
	processMessages();
	/* we want the user to see what we're writing, right? */
	Sleep(30);
    }

    if(type<0) {
	while(1) {
	    int ret = MessageBox(0, text, "Error", MB_RETRYCANCEL|MB_ICONERROR);
	    
	    /* there is no MB_CANCEL, so, *sigh*, we have to display
	       the "retry" button. So pretend it's doing anything... */
	    if(ret==IDRETRY)
		continue;
	    else
		break;
	}
    }
}

int addRegistryEntries(char*install_dir)
{
    int ret;
    ret = setRegistryEntry("Software\\quiss.org\\swftools\\InstallPath", install_dir);
    if(!ret) return 0;
    return 1;
}

int WINAPI WinMain(HINSTANCE me,HINSTANCE hPrevInst,LPSTR lpszArgs, int nWinMode)
{
    WNDCLASSEX wcl;
    wcl.hInstance    = me;
    wcl.lpfnWndProc  = WindowFunc;
    wcl.lpszClassName= "SWFTools-Install";
    wcl.style        = CS_HREDRAW | CS_VREDRAW;
    wcl.hIcon        = LoadIcon(NULL, IDI_APPLICATION);
    wcl.hIconSm      = LoadIcon(NULL, IDI_APPLICATION);
    wcl.hCursor      = LoadCursor(NULL, IDC_ARROW);
    wcl.lpszMenuName = NULL; //no menu
    wcl.cbClsExtra   = 0;
    wcl.cbWndExtra   = 0;
    wcl.hbrBackground= (HBRUSH)GetStockObject(LTGRAY_BRUSH);
    wcl.cbSize       = sizeof(WNDCLASSEX);
    
    WNDCLASSEX wcl_text;
    memcpy(&wcl_text, &wcl, sizeof(WNDCLASSEX));
    wcl_text.lpszClassName= "TextClass";
    wcl_text.hbrBackground = GetStockObject(HOLLOW_BRUSH);

    WNDCLASSEX wcl_background;
    memcpy(&wcl_background, &wcl, sizeof(WNDCLASSEX));
    wcl_background.lpszClassName= "SWFTools Installer";
    wcl_background.hbrBackground= CreateSolidBrush(RGB(0, 0, 128));
    

    if(!RegisterClassEx(&wcl)) {
	MessageBox(0, "Could not register window class", "Install.exe", MB_OK);
	return 1;
    }
    if(!RegisterClassEx(&wcl_background)) {
        MessageBox(0, "Could not register window class 2", "Install.exe", MB_OK);
        return 1;
    }

    HWND background = CreateWindow(wcl_background.lpszClassName, "Setup SWFTools",
			 0, 0, 0, 
			 GetSystemMetrics(SM_CXFULLSCREEN),
			 GetSystemMetrics(SM_CYFULLSCREEN),
			 NULL, NULL, me, 
	                 (void*)"background");
    
    if(!background) {
	MessageBox(0, "Could not create installation background window", "Install.exe", MB_OK);
	return 1;
    }

    ShowWindow(background, SW_SHOWMAXIMIZED);
    UpdateWindow(background);

    RECT r = {0,0,0,0};
    GetWindowRect(background, &r);
    int xx = 320, yy = 200;
    if(r.right - r.left > 320)
	xx = r.right - r.left;
    if(r.right - r.left > 200)
	yy = r.bottom - r.top;
    
    logo = LoadBitmap(me, "SWFTOOLS");
    
    install_path = getRegistryEntry("Software\\quiss.org\\swftools\\InstallPath");
    if(!install_path || !install_path[0])
	install_path = "c:\\swftools\\";

    CoInitialize(0);
    InitCommonControls();
    
    HWND installpath_window = CreateWindow(
	    wcl.lpszClassName,          /* Class name */
	    "SWFTools Installer",       /* Caption */
	    WS_CHILD | WS_CAPTION,
	    (xx-320)/2,                 /* Initial x  */
	    (yy-200)/2,                 /* Initial y  */
	    320,                        /* Initial x size */
	    200,                        /* Initial y size */
	    background,                       /* No parent window */
	    NULL,                       /* No menu */
	    me,                         /* This program instance */
	    (void*)"params"		/* Creation parameters */
	    );

    if(!installpath_window) {
	MessageBox(background, "Could not create installation window", "Install.exe", MB_OK);
	return 1;
    }

    ShowWindow (wnd_params, nWinMode);
    UpdateWindow (wnd_params);
   
    MSG msg;
    while(wnd_params)
    {
	GetMessage(&msg,NULL,0,0);
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    if(do_abort) {
	MessageBox(background, "Aborting Installation", "Error", MB_OK|MB_ICONERROR);
	return 1;
    }
   
    /*char buf[1024];
    sprintf(buf, "Do you want me to install SWFTools into the directory %s now?", install_path);
    int ret = MessageBox(0, buf, "SWFTools Install", MB_YESNO|MB_ICONQUESTION);
    if(ret == IDNO)
	return 0;*/
    
    CreateWindow (
	    wcl.lpszClassName,          /* Class name */
	    "Installing...",            /* Caption */
	    WS_CHILD | WS_CAPTION,
	    //WS_OVERLAPPEDWINDOW&(~WS_SIZEBOX),        /* Style */
	    (xx-260)/2, (yy-128)/2,
	    260,                        /* Initial x size */
	    128,                        /* Initial y size */
	    background,                 /* No parent window */
	    NULL,                       /* No menu */
	    me,                         /* This program instance */
	    (void*)"progress"		/* Creation parameters */
	    );
    ShowWindow (wnd_progress, nWinMode);
    UpdateWindow (wnd_progress);
    
    int success = unpack_archive(crndata, install_path, myarchivestatus);
   
    DestroyWindow(wnd_progress);

    while(wnd_progress)
	processMessages();
    if(do_abort) {
	MessageBox(background, "Aborting Installation", "Error", MB_OK|MB_ICONERROR);
	return 1;
    }

    CreateWindow (
	    wcl.lpszClassName,          /* Class name */
	    "Finished",                 /* Caption */
	    WS_CHILD | WS_CAPTION,
	    //WS_OVERLAPPEDWINDOW&(~WS_SIZEBOX),        /* Style */
	    (xx-320)/2, (yy-160)/2,
	    320,                        /* Initial x size */
	    200,                        /* Initial y size */
	    background,                 /* No parent window */
	    NULL,                       /* No menu */
	    me,                         /* This program instance */
	    (void*)"finish"		/* Creation parameters */
	    );
    ShowWindow(wnd_finish, nWinMode);
    UpdateWindow(wnd_finish);

    while(wnd_finish)
	processMessages();
    if(do_abort) {
	MessageBox(0, "Aborting Installation", "Error", MB_OK|MB_ICONERROR);
	return 1;
    }

    if(!addRegistryEntries(install_path)) {
	success = 0;
	MessageBox(0, "Couldn't create Registry Entries", "SWFTools Install", MB_OK|MB_ICONERROR);
    }

    if(success) {
	//char buf[1024];
	//sprintf(buf, "SWFTools Version %s has been installed into %s successfully", VERSION, install_path);
	//MessageBox(0, buf, "SWFTools Install", MB_OK|MB_ICONINFORMATION);
    } else {
	/* error will already have been notified by either myarchivestatus or some other
	   routine */
	/*sprintf(buf, "Installation failed\nLast message: %s", lastmessage);
	ret = MessageBox(0, buf, "SWFTools Install", MB_OK|MB_ICONERROR);*/
    }
    exit(0);
}

