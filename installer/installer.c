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
#include <stdio.h>
#include <stdlib.h>

#include "depack.h"

#include "../config.h" //for swftools version

extern char*crndata;

LRESULT CALLBACK WindowFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    printf("%08x, %d %08x %08x\n", hwnd, message, wParam, lParam);
    switch(message)
    {
	case WM_DESTROY:
	    PostQuitMessage(0);
	    break;
	default:
	    return DefWindowProc(hwnd, message, wParam, lParam);
    }
    return 0;
}

#if USE_CONSOLE
static HANDLE console;
#endif
static char*lastmessage = 0;
void myarchivestatus(int type, char*text)
{
    DWORD written = 0;
#if USE_CONSOLE
    if(console) {
	WriteConsole(console, text, strlen(text), &written, 0);
	WriteConsole(console, "\n", 1, &written, 0);
	fflush(stdout);
    } 
#endif
    if(!written) {
	printf("%s\n", text);
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

int WINAPI WinMain(HINSTANCE me,HINSTANCE hPrevInst,LPSTR lpszArgs, int nWinMode)
{
    WNDCLASS wcl;
    char*install_dir = "C:\\swftools\\";

    wcl.hInstance    = me;
    wcl.lpszClassName= "SWFTools-Installer";
    wcl.lpfnWndProc  = WindowFunc;
    wcl.style        = 0; //default style
    wcl.hIcon        = LoadIcon  (NULL, IDI_APPLICATION);
    wcl.hCursor      = LoadCursor(NULL, IDC_ARROW);
    wcl.lpszMenuName = NULL; //no menu
    wcl.cbClsExtra   = 0;
    wcl.cbWndExtra   = 0;
    wcl.hbrBackground=(HBRUSH)GetStockObject(DKGRAY_BRUSH);

    if(!RegisterClass(&wcl)) 
	return 0;
   
#if USE_CONSOLE
    FreeConsole();
#endif
   
    char buf[1024];
    sprintf(buf, "Do you want me to install SWFTools into the directory %s now?", install_dir);
    int ret = MessageBox(0, buf, "SWFTools Install", MB_YESNO|MB_ICONQUESTION);

    if(ret == IDNO)
	return 0;
    
#if USE_CONSOLE
    if(AllocConsole()) {
	DWORD written;
	console = GetStdHandle(STD_OUTPUT_HANDLE);
	WriteConsole(console, "\r\n", 1, &written, 0);
    }
#endif

    int success = unpack_archive(crndata, "C:\\swftools\\", myarchivestatus);

#if USE_CONSOLE
    FreeConsole();
#endif

    if(success) {
	sprintf(buf, "SWFTools Version %s has been installed into %s successfully", VERSION, install_dir);
	ret = MessageBox(0, buf, "SWFTools Install", MB_OK|MB_ICONINFORMATION);
    } else {
	/*sprintf(buf, "Installation failed\nLast message: %s", lastmessage);
	ret = MessageBox(0, buf, "SWFTools Install", MB_OK|MB_ICONERROR);*/
    }
    exit(0);
}



