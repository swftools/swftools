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
#include <prsht.h>
#include <stdio.h>
#include <stdlib.h>
#include "installer.h"

#include "depack.h"

#include "../config.h" //for swftools version
#include "../lib/os.h" //for registry functions

static int config_forAllUsers = 0;
static int config_createLinks = 0;
static int config_createStartmenu = 1;
static int config_createDesktop = 1;

static char path_startmenu[MAX_PATH] = "\0";
static char path_desktop[MAX_PATH] = "\0";

extern char*crndata;
extern char*license_text;

static char*install_path = "c:\\swftools\\";
static char pathBuf[1024];
static int do_abort = 0;

static char* pdf2swf_path;

static HBITMAP logo;

static HINSTANCE me;

#define USER_SETMESSAGE 0x7f01

static HWND wnd_background = 0;
LRESULT CALLBACK WindowFunc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    if(message == WM_CREATE) {
	CREATESTRUCT*cs = ((LPCREATESTRUCT)lParam);
        if(cs->lpCreateParams && !strcmp((char*)cs->lpCreateParams, "background")) {
	    wnd_background = hwnd;
	}
    }
    if(hwnd == wnd_background && message == WM_PAINT) {
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rc;
        GetWindowRect(hwnd, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;

	hdc = BeginPaint(hwnd, &ps);
	SetBkMode(hdc, TRANSPARENT);

	HPEN pen = CreatePen(PS_SOLID, 2, RGB(32, 32, 128));
	HPEN oldPen = (HPEN)SelectObject(hdc, pen);

	int t;
	for(t=0;t<20;t++) {
	    MoveToEx(hdc, t*t, 0, 0);
	    LineTo(hdc, t*t, height);
	    MoveToEx(hdc, 0, t*t, 0);
	    LineTo(hdc, width, t*t);
	}

	SelectObject(hdc, oldPen); 
	DeleteObject(pen);
	EndPaint(hwnd, &ps);
	return 1;
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

int addRegistryEntries(char*install_dir)
{
    int ret;
    ret = setRegistryEntry("Software\\quiss.org\\swftools\\InstallPath", install_dir);
    if(!ret) return 0;
    return 1;
}

int CreateShortcut(char*path, char*description, char*filename, char*arguments, int iconindex, char*iconpath, char*workdir)
{
    WCHAR wszFilename[MAX_PATH];
    IShellLink *ps1 = NULL;
    IPersistFile *pPf = NULL;
    HRESULT hr;
    hr = CoCreateInstance(&CLSID_ShellLink, NULL, CLSCTX_INPROC_SERVER, &IID_IShellLink, (void*)&ps1);
    if(FAILED(hr)) return 0;
    hr = ps1->lpVtbl->QueryInterface(ps1, &IID_IPersistFile, (void **)&pPf);
    if(FAILED(hr)) return 0;
    hr = ps1->lpVtbl->SetPath(ps1, path);
    if(FAILED(hr)) return 0;
    hr = ps1->lpVtbl->SetDescription(ps1, description);
    
    if (arguments) {
	hr = ps1->lpVtbl->SetArguments(ps1, arguments);
	if(FAILED(hr)) return 0;
    }
    if (iconpath) {
	hr = ps1->lpVtbl->SetIconLocation(ps1, iconpath, iconindex);
	if (FAILED(hr)) return 0;
    }
    if (workdir) {
	hr = ps1->lpVtbl->SetWorkingDirectory(ps1, workdir);
	if (FAILED(hr)) return 0;
    }
    MultiByteToWideChar(CP_ACP, 0, filename, -1, wszFilename, MAX_PATH);
    hr = pPf->lpVtbl->Save(pPf, wszFilename, TRUE);
    if(FAILED(hr)) {
	return 0;
    }
    pPf->lpVtbl->Release(pPf);
    ps1->lpVtbl->Release(ps1);
    return 1;
}

BOOL CALLBACK PropertySheetFuncCommon(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, int buttons)
{
    LPNMHDR lpnm;
	
    HWND dialog = GetParent(hwnd);

    if(message == WM_INITDIALOG) {
	//    create_bitmap(hwnd);
	//    if(hBitmap)
	//	    SendDlgItemMessage(hwnd, IDC_BITMAP, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hBitmap);
        RECT rc;
        GetWindowRect(dialog, &rc);
	int width = rc.right - rc.left;
	int height = rc.bottom - rc.top;
        MoveWindow(dialog, (GetSystemMetrics(SM_CXSCREEN) - width)/2, (GetSystemMetrics(SM_CYSCREEN) - height)/2, width, height, FALSE);
	return FALSE;
     }

    if(message == WM_NOTIFY && (((LPNMHDR)lParam)->code == PSN_SETACTIVE)) {
	PropSheet_SetWizButtons(dialog, buttons);
    	return FALSE;
    }
    return FALSE;
}

BOOL CALLBACK PropertySheetFunc1(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if(message == WM_INITDIALOG) {
	SetDlgItemText(hwnd, IDC_LICENSE, license_text);
    }
    return PropertySheetFuncCommon(hwnd, message, wParam, lParam, PSWIZB_NEXT);
}
BOOL CALLBACK PropertySheetFunc2(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if(message == WM_INITDIALOG) {
	SetDlgItemText(hwnd, IDC_INSTALL_PATH, install_path);
    }
    if(message == WM_COMMAND) {
	if((wParam&0xffff) == IDC_BROWSE) {
	    BROWSEINFOA browse;
	    memset(&browse, 0, sizeof(browse));
	    browse.ulFlags = BIF_EDITBOX | BIF_NEWDIALOGSTYLE | BIF_USENEWUI;// | BIF_RETURNONLYFSDIRS; //BIF_VALIDATE
	    browse.pszDisplayName = (CHAR*)malloc(MAX_PATH);
	    memset(browse.pszDisplayName, 0, MAX_PATH);
	    browse.lpszTitle = "Select installation directory";
	    printf("Start browsing %s\n", browse.pszDisplayName);
	    browse.pidlRoot = SHBrowseForFolder(&browse);
	    printf("Browsing returns %s / %08x\n", browse.pszDisplayName, browse.pidlRoot);
	    if(browse.pszDisplayName) {
		if(SHGetPathFromIDList(browse.pidlRoot, browse.pszDisplayName)) {
		    printf("Path is %s\n", browse.pszDisplayName);
		    install_path = browse.pszDisplayName;
		}
	    }
	    SendDlgItemMessage(hwnd, IDC_INSTALL_PATH, WM_SETTEXT, 0, (LPARAM)install_path);
	    return 0;

	}
	else if((wParam&0xffff) == IDC_INSTALL_PATH) {
	    SendDlgItemMessage(hwnd, IDC_INSTALL_PATH, WM_GETTEXT, sizeof(pathBuf), (LPARAM)&(pathBuf[0]));
	    if(pathBuf[0]) {
		install_path = pathBuf;
	    }
	    return 0;
	}
    }
    return PropertySheetFuncCommon(hwnd, message, wParam, lParam, PSWIZB_BACK|PSWIZB_NEXT);
}
HWND statuswnd;
static int progress_pos = 0;
void PropertyArchiveStatus(int type, char*text)
{
    if(text && text[0]=='[') return;
    SetDlgItemText(statuswnd, IDC_INFO, strdup(text));
    int t;
    /* There are usually 6 messages, and a range of 54 to fill, so 
       step 9 times */
    for(t=0;t<9;t++) {
	SendDlgItemMessage(statuswnd, IDC_PROGRESS, PBM_SETPOS, ++progress_pos, 0);
	processMessages();
	Sleep(30);
    }
    if(type<0) {
	while(1) {
	    int ret = MessageBox(0, text, "Error", MB_RETRYCANCEL|MB_ICONERROR);
	    if(ret==IDRETRY) continue;
	    else break;
	}
    }
}
BOOL CALLBACK PropertySheetFunc3(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HWND dialog = GetParent(hwnd);
    if(message == WM_INITDIALOG) {
	SetDlgItemText(hwnd, IDC_INFO, "Ready to install");
    }
    if(message == WM_NOTIFY && (((LPNMHDR)lParam)->code == PSN_WIZNEXT)) {
	PropSheet_SetWizButtons(dialog, 0);
	SendMessage(dialog, PSM_CANCELTOCLOSE, 0, 0); //makes wine display a warning
	SetDlgItemText(hwnd, IDC_TITLE, "Installing files...");
	statuswnd = hwnd;
	SendDlgItemMessage(hwnd, IDC_PROGRESS, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0,54));
	progress_pos = 0;
	SendDlgItemMessage(hwnd, IDC_PROGRESS, PBM_SETPOS, progress_pos, 0);
	int success = unpack_archive(crndata, install_path, PropertyArchiveStatus);
	return 0;
    }
    return PropertySheetFuncCommon(hwnd, message, wParam, lParam, PSWIZB_BACK|PSWIZB_NEXT);
}
BOOL CALLBACK PropertySheetFunc4(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if(message == WM_INITDIALOG) {
	pdf2swf_path = concatPaths(install_path, "pdf2swf_gui.exe");
	FILE*fi = fopen(pdf2swf_path, "rb");
	if(fi) {
	    config_createLinks = 1;
	    fclose(fi);
	}
	if(!config_createLinks) {
	    SendDlgItemMessage(hwnd, IDC_STARTMENU, SW_HIDE, 0, 0);
	    SendDlgItemMessage(hwnd, IDC_DESKTOP, SW_HIDE, 0, 0);
	}

	SendDlgItemMessage(hwnd, IDC_STARTMENU, BM_SETCHECK, config_createStartmenu, 0);
	SendDlgItemMessage(hwnd, IDC_DESKTOP, BM_SETCHECK, config_createStartmenu, 0);
    }
    if(message == WM_COMMAND) {
	if((wParam&0xffff) == IDC_STARTMENU) {
	    config_createStartmenu = SendDlgItemMessage(hwnd, IDC_STARTMENU, BM_GETCHECK, 0, 0);
	    config_createStartmenu^=1;
	    SendDlgItemMessage(hwnd, IDC_STARTMENU, BM_SETCHECK, config_createStartmenu, 0);
	    return 0;
	}
	if((wParam&0xffff) == IDC_DESKTOP) {
	    config_createDesktop = SendDlgItemMessage(hwnd, IDC_DESKTOP, BM_GETCHECK, 0, 0);
	    config_createDesktop^=1;
	    SendDlgItemMessage(hwnd, IDC_DESKTOP, BM_SETCHECK, config_createDesktop, 0);
	    return 0;
	}
    }

    if(message == WM_NOTIFY && (((LPNMHDR)lParam)->code == PSN_WIZFINISH)) {
	if(!addRegistryEntries(install_path)) {
	    MessageBox(0, "Couldn't create Registry Entries", "SWFTools Install", MB_OK|MB_ICONERROR);
	    return 1;
	}
	if(config_createLinks) {
	    if(config_createDesktop && path_desktop[0]) {
		char* linkName = concatPaths(path_desktop, "pdf2swf.lnk");
		if(!CreateShortcut(pdf2swf_path, "pdf2swf", linkName, 0, 0, 0, install_path)) {
		    MessageBox(0, "Couldn't create desktop shortcut", "Install.exe", MB_OK);
		    return 1;
		}
	    }
	    if(config_createStartmenu && path_startmenu[0]) {
		char* group = concatPaths(path_startmenu, "pdf2swf");
		CreateDirectory(group, 0);
		char* linkName = concatPaths(group, "pdf2swf.lnk");
		if(!CreateShortcut(pdf2swf_path, "pdf2swf", linkName, 0, 0, 0, install_path)) {
		    MessageBox(0, "Couldn't create start menu entry", "Install.exe", MB_OK);
		    return 1;
		}
	    }
	}
    }
    return PropertySheetFuncCommon(hwnd, message, wParam, lParam, PSWIZB_FINISH);
}

#ifndef PSP_HIDEHEADER
#define PSP_HIDEHEADER	2048
#endif

typedef struct _wizardpage {
    DLGPROC function;
    int resource;
} wizardpage_t;

void runPropertySheet(HWND parent)
{
    PROPSHEETHEADER sheet;

    wizardpage_t wpage[5] = {
	{PropertySheetFunc1, IDD_LICENSE},
	{PropertySheetFunc2, IDD_INSTALLDIR},
	{PropertySheetFunc3, IDD_PROGRESS},
	{PropertySheetFunc4, IDD_FINISH},
    };
    int num = sizeof(wpage)/sizeof(wpage[0]);
    HPROPSHEETPAGE pages[num];
    int t;
    for(t=0;t<num;t++) {
	PROPSHEETPAGE page;
	memset(&page, 0, sizeof(PROPSHEETPAGE));
	page.dwSize = sizeof(PROPSHEETPAGE);
	page.dwFlags = PSP_DEFAULT|PSP_HIDEHEADER;
	page.hInstance = me;
	page.pfnDlgProc = wpage[t].function;
	page.pszTemplate = MAKEINTRESOURCE(wpage[t].resource);
	pages[t] = CreatePropertySheetPage(&page);
    }

    memset(&sheet, 0, sizeof(PROPSHEETHEADER));
    sheet.dwSize = sizeof(PROPSHEETHEADER);
    sheet.hInstance = me;
    sheet.hwndParent = parent;
    sheet.phpage = pages;
    sheet.dwFlags = PSH_WIZARD;
    sheet.nPages = num;
    PropertySheet(&sheet);
}

static HRESULT (WINAPI *f_SHGetSpecialFolderPath)(HWND hwnd, LPTSTR lpszPath, int nFolder, BOOL fCreate);

int WINAPI WinMain(HINSTANCE _me,HINSTANCE hPrevInst,LPSTR lpszArgs, int nWinMode)
{
    me = _me;
    WNDCLASSEX wcl_background;
    wcl_background.hInstance    = me;
    wcl_background.lpfnWndProc  = WindowFunc;
    wcl_background.lpszClassName= "SWFTools Install";
    wcl_background.style        = CS_HREDRAW | CS_VREDRAW;
    wcl_background.hIcon        = LoadIcon(NULL, IDI_APPLICATION);
    wcl_background.hIconSm      = LoadIcon(NULL, IDI_APPLICATION);
    wcl_background.hCursor      = LoadCursor(NULL, IDC_ARROW);
    wcl_background.lpszMenuName = NULL; //no menu
    wcl_background.cbClsExtra   = 0;
    wcl_background.cbWndExtra   = 0;
    wcl_background.hbrBackground= CreateSolidBrush(RGB(0, 0, 128));
    wcl_background.cbSize       = sizeof(WNDCLASSEX);
  
    HINSTANCE shell32 = LoadLibrary("shell32.dll");
    if(!shell32) {
	MessageBox(0, "Could not load shell32.dll", "Install.exe", MB_OK);
	return 1;
    }
    f_SHGetSpecialFolderPath = (HRESULT (WINAPI*)(HWND,LPTSTR,int,BOOL))GetProcAddress(shell32, "SHGetSpecialFolderPathA");
    if(!f_SHGetSpecialFolderPath) {
	MessageBox(0, "Could not load shell32.dll", "Install.exe", MB_OK);
	return 1;
    }
	
    HRESULT coret = CoInitialize(NULL);
    if(FAILED(coret)) {
	MessageBox(0, "Could not initialize COM interface", "Install.exe", MB_OK);
	return 1;
    }

    char mypath[MAX_PATH];
    path_startmenu[0] = 0;
    path_desktop[0] = 0;
    if(config_forAllUsers) {
	f_SHGetSpecialFolderPath(NULL, path_desktop, CSIDL_COMMON_DESKTOPDIRECTORY, 0);
	f_SHGetSpecialFolderPath(NULL, path_startmenu, CSIDL_COMMON_PROGRAMS, 0);
    } else {
	f_SHGetSpecialFolderPath(NULL, path_desktop, CSIDL_DESKTOPDIRECTORY, 0);
	f_SHGetSpecialFolderPath(NULL, path_startmenu, CSIDL_PROGRAMS, 0);
    }

    if(!RegisterClassEx(&wcl_background)) {
        MessageBox(0, "Could not register window background class", "Install.exe", MB_OK);
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
    SetForegroundWindow(background);
    UpdateWindow(background);

    logo = LoadBitmap(me, "SWFTOOLS");
    
    install_path = getRegistryEntry("Software\\quiss.org\\swftools\\InstallPath");
    if(!install_path || !install_path[0])
	install_path = "c:\\swftools\\";

    CoInitialize(0);
    InitCommonControls();

    RECT r = {0,0,0,0};
    GetWindowRect(background, &r);
    int xx = 320, yy = 200;
    if(r.right - r.left > 320)
	xx = r.right - r.left;
    if(r.right - r.left > 200)
	yy = r.bottom - r.top;
    
    int ret = 0;
    runPropertySheet(background);

    return ret;
}

