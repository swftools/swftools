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

#include "../config.h" //for swftools version

static int config_forAllUsers = 0;
static int config_createLinks = 0;
static int config_createStartmenu = 1;
static int config_createDesktop = 1;

static char path_startmenu[MAX_PATH] = "\0";
static char path_desktop[MAX_PATH] = "\0";
static char path_programfiles[MAX_PATH] = "\0";

extern char*crndata;
extern int crndata_len;
extern int crn_decompressed_size;

extern char*license_text;

static char*install_path = "c:\\swftools\\";
static char pathBuf[MAX_PATH];
static int do_abort = 0;

static char* pdf2swf_path;

static HBITMAP logo = 0;

static HINSTANCE me;

#define USER_SETMESSAGE 0x7f01

static void tball(HDC hdc, double px, double py, double sx, double sy)
{
    MoveToEx(hdc, 75.25*sx+px, -32.50*sy+py, 0);
    LineTo(hdc, 74.75*sx+px, -24.80*sy+py); LineTo(hdc, 74.95*sx+px, -3.45*sy+py); LineTo(hdc, 75.25*sx+px, -32.50*sy+py);
    LineTo(hdc, 44.85*sx+px, -104.30*sy+py); LineTo(hdc, 74.75*sx+px, -24.80*sy+py); LineTo(hdc, 17.30*sx+px, -91.80*sy+py);
    LineTo(hdc, 29.35*sx+px, -10.10*sy+py); LineTo(hdc, 74.75*sx+px, -24.80*sy+py); LineTo(hdc, 47.60*sx+px, 63.80*sy+py);
    LineTo(hdc, 74.75*sx+px, 51.10*sy+py); LineTo(hdc, 74.95*sx+px, -3.45*sy+py);
    MoveToEx(hdc, 74.75*sx+px, 51.10*sy+py, 0);
    LineTo(hdc, 29.35*sx+px, 118.75*sy+py); LineTo(hdc, -45.20*sx+px, 155.35*sy+py);
    LineTo(hdc, -127.95*sx+px, 145.10*sy+py); LineTo(hdc, -194.85*sx+px, 104.30*sy+py);
    LineTo(hdc, -225.25*sx+px, 32.50*sy+py); LineTo(hdc, -224.75*sx+px, -51.10*sy+py);
    LineTo(hdc, -179.35*sx+px, -118.75*sy+py); LineTo(hdc, -104.80*sx+px, -155.35*sy+py);
    LineTo(hdc, -22.05*sx+px, -145.10*sy+py); LineTo(hdc, 44.85*sx+px, -104.30*sy+py);
    LineTo(hdc, 17.30*sx+px, -91.80*sy+py); LineTo(hdc, -75.00*sx+px, -122.60*sy+py);
    LineTo(hdc, -45.20*sx+px, -43.70*sy+py); LineTo(hdc, 17.30*sx+px, -91.80*sy+py);
    LineTo(hdc, -22.05*sx+px, -145.10*sy+py); LineTo(hdc, -75.00*sx+px, -122.60*sy+py);
    LineTo(hdc, -104.80*sx+px, -155.35*sy+py); LineTo(hdc, -167.30*sx+px, -107.25*sy+py);
    LineTo(hdc, -75.00*sx+px, -122.60*sy+py); LineTo(hdc, -127.95*sx+px, -63.55*sy+py);
    LineTo(hdc, -45.20*sx+px, -43.70*sy+py); LineTo(hdc, 29.35*sx+px, -10.10*sy+py);
    LineTo(hdc, 47.60*sx+px, 63.80*sy+py); LineTo(hdc, -25.70*sx+px, 52.45*sy+py);
    LineTo(hdc, -25.70*sx+px, 128.45*sy+py); LineTo(hdc, 47.60*sx+px, 63.80*sy+py);
    LineTo(hdc, 29.35*sx+px, 118.75*sy+py); LineTo(hdc, -25.70*sx+px, 128.45*sy+py);
    LineTo(hdc, -45.20*sx+px, 155.35*sy+py); LineTo(hdc, -118.85*sx+px, 143.85*sy+py);
    LineTo(hdc, -127.95*sx+px, 145.10*sy+py);
    MoveToEx(hdc, -179.35*sx+px, -118.75*sy+py, 0);
    LineTo(hdc, -167.30*sx+px, -107.25*sy+py); LineTo(hdc, -127.95*sx+px, -63.55*sy+py);
    LineTo(hdc, -118.85*sx+px, 21.65*sy+py); LineTo(hdc, -45.20*sx+px, -43.70*sy+py);
    LineTo(hdc, -25.70*sx+px, 52.45*sy+py); LineTo(hdc, 29.35*sx+px, -10.10*sy+py);
    MoveToEx(hdc, -225.25*sx+px, 32.50*sy+py, 0);
    LineTo(hdc, -194.85*sx+px, -17.85*sy+py); LineTo(hdc, -224.75*sx+px, -51.10*sy+py);
    LineTo(hdc, -167.30*sx+px, -107.25*sy+py); LineTo(hdc, -194.85*sx+px, -17.85*sy+py);
    LineTo(hdc, -127.95*sx+px, -63.55*sy+py);
    MoveToEx(hdc, -175.85*sx+px, 77.60*sy+py, 0);
    LineTo(hdc, -96.35*sx+px, 99.55*sy+py); LineTo(hdc, -118.85*sx+px, 21.65*sy+py);
    LineTo(hdc, -175.85*sx+px, 77.60*sy+py); LineTo(hdc, -225.25*sx+px, 32.50*sy+py);
    MoveToEx(hdc, -118.85*sx+px, 143.85*sy+py, 0);
    LineTo(hdc, -175.85*sx+px, 77.60*sy+py); LineTo(hdc, -194.85*sx+px, 104.30*sy+py);
    LineTo(hdc, -118.85*sx+px, 143.85*sy+py); LineTo(hdc, -96.35*sx+px, 99.55*sy+py);
    LineTo(hdc, -25.70*sx+px, 52.45*sy+py); LineTo(hdc, -118.85*sx+px, 21.65*sy+py);
    LineTo(hdc, -194.85*sx+px, -17.85*sy+py); LineTo(hdc, -175.85*sx+px, 77.60*sy+py);
    MoveToEx(hdc, -96.35*sx+px, 99.55*sy+py, 0);
    LineTo(hdc, -25.70*sx+px, 128.45*sy+py); LineTo(hdc, -118.85*sx+px, 143.85*sy+py);
}

static char* concatPaths(const char*base, const char*add)
{
    int l1 = strlen(base);
    int l2 = strlen(add);
    int pos = 0;
    char*n = 0;
    while(l1 && base[l1-1] == '\\')
	l1--;
    while(pos < l2 && add[pos] == '\\')
	pos++;

    n = (char*)malloc(l1 + (l2-pos) + 2);
    memcpy(n,base,l1);
    n[l1]='\\';
    strcpy(&n[l1+1],&add[pos]);
    return n;
}

static void handleTemplateFile(const char*filename)
{
    FILE*fi = fopen(filename, "rb");
    fseek(fi, SEEK_END, 0);
    int len = ftell(fi);
    fseek(fi, SEEK_SET, 0);
    char*file = malloc(len);
    fread(file, len, 1, fi);
    fclose(fi);
    int l = strlen(install_path);
    fi = fopen(filename, "wb");
    char*pos = file;
    char*lastpos = file;
    while(1) {
        pos = strstr(pos, "%%PATH%%");
        if(!pos) {
            pos = &file[len];
            break;
        }
        if(pos!=lastpos)
            fwrite(lastpos, pos-lastpos, 1, fi);
        fwrite(install_path, l, 1, fi);
        pos+=8; // length of "%%PATH%%"
        lastpos = pos;
    }
    fwrite(lastpos, pos-lastpos, 1, fi);
    fclose(fi);
    free(file);
}

static char* getRegistryEntry(char*path)
{
    int res = 0;
    HKEY key;
    long rc;
    long size = 0;
    DWORD type;
    char*buf;
    rc = RegOpenKeyEx(HKEY_LOCAL_MACHINE, path, 0, KEY_ALL_ACCESS/* KEY_READ*/, &key);
    if (rc != ERROR_SUCCESS) {
	fprintf(stderr, "RegOpenKeyEx failed\n");
	return 0;
    }
    rc = RegQueryValueEx(key, NULL, 0, 0, 0, (LPDWORD)&size) ;
    if(rc != ERROR_SUCCESS) {
	fprintf(stderr, "RegQueryValueEx(1) failed: %d\n", rc);
	return 0;
    }
    buf = (char*)malloc(size+1);
    rc = RegQueryValueEx(key, NULL, 0, &type, (BYTE*)buf, (LPDWORD)&size);
    if(rc != ERROR_SUCCESS) {
	fprintf(stderr, "RegQueryValueEx(2) failed: %d\n", rc);
	return 0;
    }
    if(type == REG_SZ || type == REG_EXPAND_SZ) {
	while(size && buf[size-1] == '\0')
          --size;
	buf[size] = 0;
	/* TODO: convert */
	return buf;
    } else if(type == REG_BINARY) {
	return buf;
    }
}

static int setRegistryEntry(char*key,char*value)
{
    HKEY hkey1;
    HKEY hkey2;
    int ret1 = -1, ret2= -1;
    ret1 = RegCreateKey(HKEY_CURRENT_USER, key, &hkey1);
    if(config_forAllUsers) {
	ret2 = RegCreateKey(HKEY_LOCAL_MACHINE, key, &hkey2);
    }

    if(ret1 && ret2) {
	fprintf(stderr, "registry: CreateKey %s failed\n", key);
	return 0;
    }
    if(!ret1)
	ret1 = RegSetValue(hkey1, NULL, REG_SZ, value, strlen(value)+1);
    if(!ret2)
	ret2 = RegSetValue(hkey2, NULL, REG_SZ, value, strlen(value)+1);
    if(ret1 && ret2) {
	fprintf(stderr, "registry: SetValue %s failed\n", key);
	return 0;
    }
    return 1;
}

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

        double s = width / 1282.0;
	
        HPEN pen0 = CreatePen(PS_SOLID, 2, RGB(0, 32, 128));
	HPEN oldPen = (HPEN)SelectObject(hdc, pen0);
        tball(hdc, 0.7*width,0.2*height,s,s);

	HPEN pen1 = CreatePen(PS_SOLID, 2, RGB(0, 48, 128));
	(HPEN)SelectObject(hdc, pen1);
        tball(hdc, 0.233*width,0.3*height,s*2,s*2);
	
        HPEN pen2 = CreatePen(PS_SOLID, 2, RGB(0, 64, 128));
	(HPEN)SelectObject(hdc, pen2);
        tball(hdc, width, 1.2*height,s*3,-s*3);

	SelectObject(hdc, oldPen); 
	DeleteObject(pen1);
	DeleteObject(pen2);
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

static int CreateURL(const char*url, const char*path)
{
    FILE*fi = fopen(path, "wb");
    if(!fi)
        return 0;
    fprintf(fi, "[InternetShortcut]\r\n");
    fprintf(fi, "URL=http://localhost:8081/\r\n");
    fclose(fi);
    return 1;
}


BOOL CALLBACK PropertySheetFuncCommon(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam, int buttons)
{
    LPNMHDR lpnm;
	
    HWND dialog = GetParent(hwnd);

    if(message == WM_INITDIALOG) {
	if(logo)
	    SendDlgItemMessage(hwnd, IDC_BITMAP, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)logo);

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

        config_forAllUsers = 0;
        SendDlgItemMessage(hwnd, IDC_ALLUSERS, BM_SETCHECK, config_forAllUsers, 0);
        SendDlgItemMessage(hwnd, IDC_CURRENTUSER, BM_SETCHECK, config_forAllUsers^1, 0);
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
                    int l = strlen(install_path);
                    while(l && install_path[l-1]=='\\') {
                        install_path[--l]=0;
                    }   
		}
	    }
	    SendDlgItemMessage(hwnd, IDC_INSTALL_PATH, WM_SETTEXT, 0, (LPARAM)install_path);
	    return 0;

	}
	else if((wParam&0xffff) == IDC_ALLUSERS) {
            config_forAllUsers = 1;
            SendDlgItemMessage(hwnd, IDC_ALLUSERS, BM_SETCHECK, config_forAllUsers, 0);
            SendDlgItemMessage(hwnd, IDC_CURRENTUSER, BM_SETCHECK, config_forAllUsers^1, 0);
        }
	else if((wParam&0xffff) == IDC_CURRENTUSER) {
            config_forAllUsers = 0;
            SendDlgItemMessage(hwnd, IDC_ALLUSERS, BM_SETCHECK, config_forAllUsers, 0);
            SendDlgItemMessage(hwnd, IDC_CURRENTUSER, BM_SETCHECK, config_forAllUsers^1, 0);
        }
	else if((wParam&0xffff) == IDC_INSTALL_PATH) {
	    SendDlgItemMessage(hwnd, IDC_INSTALL_PATH, WM_GETTEXT, sizeof(pathBuf), (LPARAM)&(pathBuf[0]));
	    if(pathBuf[0]) {
		install_path = pathBuf;
                int l = strlen(install_path);
                while(l && install_path[l-1]=='\\') {
                    install_path[--l]=0;
                }   
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
    /* There are usually 4 messages, and a range of 40 to fill, so 
       step 9 times */
    for(t=0;t<10;t++) {
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

void print_space(char*dest, char*msg, ULONGLONG size)
{
    if(size < 1024)
	sprintf(dest, "%s%d Bytes", msg, size);
    else if(size < 1048576l)
	sprintf(dest, "%s%.2f Kb", msg, size/1024.0);
    else if(size < 1073741824l)
	sprintf(dest, "%s%.2f Mb", msg, size/1048576.0);
    else if(size < 1099511627776ll)
	sprintf(dest, "%s%.2f Gb", msg, size/1073741824.0);
    else
	sprintf(dest, "%s%.2f Tb", msg, size/1125899906842624.0);
}

BOOL CALLBACK PropertySheetFunc3(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HWND dialog = GetParent(hwnd);
    if(message == WM_INITDIALOG) {
	SetDlgItemText(hwnd, IDC_INFO, "Ready to install");

	char buf[256];
	print_space(buf, "Space required: ", crn_decompressed_size);
	SetDlgItemText(hwnd, IDC_SPACE1, buf);
	ULARGE_INTEGER available,total,totalfree;
	available.QuadPart=0;
	total.QuadPart=0;
	totalfree.QuadPart=0;
	GetDiskFreeSpaceEx(install_path, &available, &total, &totalfree);
	print_space(buf, "Space available: ", available.QuadPart);
	SetDlgItemText(hwnd, IDC_SPACE2, buf);
    }
    if(message == WM_NOTIFY && (((LPNMHDR)lParam)->code == PSN_WIZNEXT)) {
	SetDlgItemText(hwnd, IDC_SPACE1, "");
	SetDlgItemText(hwnd, IDC_SPACE2, "");
	PropSheet_SetWizButtons(dialog, 0);
	SendMessage(dialog, PSM_CANCELTOCLOSE, 0, 0); //makes wine display a warning
	SetDlgItemText(hwnd, IDC_TITLE, "Installing files...");
	statuswnd = hwnd;
	SendDlgItemMessage(hwnd, IDC_PROGRESS, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0,40));
	progress_pos = 0;
	SendDlgItemMessage(hwnd, IDC_PROGRESS, PBM_SETPOS, progress_pos, 0);
	int success = unpack_archive(crndata, crndata_len, install_path, PropertyArchiveStatus);
	if(!success) {
	    MessageBox(0, "Couldn't extract all installation files", "SWFTools Install", MB_OK|MB_ICONERROR);
	    do_abort=1;
	    exit(1);
	}
	return 0;
    }
    return PropertySheetFuncCommon(hwnd, message, wParam, lParam, PSWIZB_BACK|PSWIZB_NEXT);
}

static HRESULT (WINAPI *f_SHGetSpecialFolderPath)(HWND hwnd, LPTSTR lpszPath, int nFolder, BOOL fCreate);

BOOL CALLBACK PropertySheetFunc4(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if(message == WM_INITDIALOG) {
	pdf2swf_path = concatPaths(install_path, "pdf2swf_gui.exe");
	FILE*fi = fopen(pdf2swf_path, "rb");
	if(fi) {
	    printf("a GUI program exists, creating desktop/startmenu links\n");
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

        char mypath[MAX_PATH];
        path_startmenu[0] = 0;
        path_desktop[0] = 0;
        if(config_forAllUsers) {
            f_SHGetSpecialFolderPath(NULL, path_desktop, CSIDL_COMMON_DESKTOPDIRECTORY, 0);
            f_SHGetSpecialFolderPath(NULL, path_startmenu, CSIDL_COMMON_PROGRAMS, 0);
        }
        /* get local program/desktop directory- this is both for forAllUsers=0 as well
           as a fallback if the above didn't return any paths */
        if(!path_startmenu[0]) {
            f_SHGetSpecialFolderPath(NULL, path_startmenu, CSIDL_PROGRAMS, 0);
        }
        if(!path_desktop[0]) {
            f_SHGetSpecialFolderPath(NULL, path_desktop, CSIDL_DESKTOPDIRECTORY, 0);
        }

	if(config_createLinks) {
	    if(config_createDesktop && path_desktop[0]) {
		char* linkName = concatPaths(path_desktop, "pdf2swf.lnk");
                printf("Creating desktop link %s -> %s\n", linkName, pdf2swf_path);
		if(!CreateShortcut(pdf2swf_path, "pdf2swf", linkName, 0, 0, 0, install_path)) {
		    MessageBox(0, "Couldn't create desktop shortcut", "Install.exe", MB_OK);
		    return 1;
		}
	    }
	    if(config_createStartmenu && path_startmenu[0]) {
		char* group = concatPaths(path_startmenu, "pdf2swf");
		CreateDirectory(group, 0);
		char* linkName = concatPaths(group, "pdf2swf.lnk");
                printf("Creating %s -> %s\n", linkName, pdf2swf_path);
		if(!CreateShortcut(pdf2swf_path, "pdf2swf", linkName, 0, 0, 0, install_path)) {
		    MessageBox(0, "Couldn't create start menu entry", "Install.exe", MB_OK);
		    return 1;
		}
	    }
	} else {
            printf("not creating desktop/startmenu links\n");
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

    path_programfiles[0] = 0;
    f_SHGetSpecialFolderPath(NULL, path_programfiles, CSIDL_PROGRAM_FILES, 0);

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
    if(!install_path || !install_path[0]) {
        if(path_programfiles[0]) {
            install_path = concatPaths(path_programfiles, "SWFTools");
        } else {
	    install_path = "c:\\swftools\\";
        }
    }

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

