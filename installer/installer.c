/* installer.c

   Part of the rfx installer (Main program).
   
   Copyright (c) 2004-2008 Matthias Kramm <kramm@quiss.org> 
 
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
#ifndef DEINSTALL
#include "archive.h"
#endif
#include "utils.h"

static int config_forAllUsers = 0;
static int config_createLinks = 0;
static int config_createStartmenu = 1;
static int config_createDesktop = 1;
static int config_deleteextra = 1;

static char path_startmenu[MAX_PATH] = "\0";
static char path_desktop[MAX_PATH] = "\0";
static char path_programfiles[MAX_PATH] = "\0";

static char pathBuf[MAX_PATH];
static int do_abort = 0;

static char* pdf2swf_path;

static char registry_path[1024];

static char*install_path = "c:\\swftools\\";
#define SOFTWARE_DOMAIN "quiss.org"
#define SOFTWARE_NAME "SWFTools"
#define INSTALLER_NAME "SWFTools Installer"

static HBITMAP logo = 0;

static HINSTANCE me;

#define USER_SETMESSAGE 0x7f01

#ifndef DEINSTALL
extern char*crndata;
extern int crndata_len;
extern int crn_decompressed_size;
extern char*license_text;

#include "background.c"
#endif

typedef struct _filelist
{
    const char*filename;
    struct _filelist*next;
    char type;
} filelist_t;

static filelist_t* registerFile(filelist_t*next, const char*filename, char type)
{
    filelist_t*file = malloc(sizeof(filelist_t));
    file->filename = strdup(filename);
    file->type = type;
    file->next = next;
    return file;
}

static filelist_t* readFileList(char*filename)
{
    FILE*fi = fopen(filename, "rb");
    if(!fi) {
	return 0;
    }
    fseek(fi, 0, SEEK_END);
    int len = ftell(fi);
    fseek(fi, 0, SEEK_SET);
    char*data = malloc(len+1);
    fread(data, len, 1, fi);
    fclose(fi);
    int t=0;
    char*line = data;
    filelist_t*list = 0,*lpos=0;;
    while(t<len) {
	if(data[t]=='\r' || data[t]=='\n') {
	    data[t++] = 0;
	    if(strchr("DFI", line[0]) && line[1]==' ' && line[2]!=' ') {
		filelist_t*f = malloc(sizeof(filelist_t));
		f->type=line[0];
		f->filename=strdup(line+2);
		f->next = 0;
		if(!list) {
		    list = lpos = f;
		} else {
		    lpos->next = f;
		    lpos = f;
		}
	    } else {
		// skip line- this usually only happens if somebody tampered
	        // with the file
	    }
	    while(t<len && (data[t]=='\0' || data[t]=='\r' || data[t]=='\n'))
		t++;
	    line = &data[t];
	} else {
	    t++;
	}
    }
    return list;
}

static void writeFileList(filelist_t*file, const char*filename)
{
    FILE*fi = fopen(filename, "wb");
    fprintf(fi, "[%s installed files]\r\n", SOFTWARE_NAME);
    if(!fi) {
	char buf[1024];
	sprintf(buf, "Couldn't write file %s", filename);
	MessageBox(0, buf, INSTALLER_NAME, MB_OK|MB_ICONERROR);
	do_abort=1;
	exit(1);
    }
    while(file) {
	fprintf(fi, "%c %s\r\n", file->type, file->filename);
	file = file->next;
    }
    fclose(fi);
}

static filelist_t*installedFiles = 0;

static void addFile(const char*filename)
{
    installedFiles = registerFile(installedFiles, filename, 'F');
}
static void addDir(const char*filename)
{
    installedFiles = registerFile(installedFiles, filename, 'D');
}

static void handleTemplateFile(const char*filename)
{
    FILE*fi = fopen(filename, "rb");
    fseek(fi, 0, SEEK_END);
    int len = ftell(fi);
    fseek(fi, 0, SEEK_SET);
    char*file = malloc(len+1);
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
    if(ret1) {
	installedFiles = registerFile(installedFiles, key, 'k');
    }
    if(ret2) {
	installedFiles = registerFile(installedFiles, key, 'K');
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
    ret = setRegistryEntry(registry_path, install_dir);
    if(!ret) return 0;
    return 1;
}

int CreateShortcut(char*path, char*description, char*filename, char*arguments, int iconindex, char*iconpath, char*workdir)
{
    printf("Creating %s -> %s\n", filename, path);
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
    addFile(filename);
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
    addFile(path);
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

#ifndef DEINSTALL
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
	    browse.pidlRoot = SHBrowseForFolder(&browse);
	    if(browse.pszDisplayName) {
		if(SHGetPathFromIDList(browse.pidlRoot, browse.pszDisplayName)) {
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

void PropertyArchiveError(char*text)
{
    while(1) {
	int ret = MessageBox(0, text, "Error", MB_RETRYCANCEL|MB_ICONERROR);
	if(ret==IDRETRY) continue;
	else break;
    }
}

void PropertyArchive_NewFile(char*filename)
{
    addFile(filename);
    processMessages();
}
void PropertyArchive_NewDirectory(char*filename)
{
    addDir(filename);
    processMessages();
}

static int lastlen = 0;
void PropertyArchiveStatus(int pos, int len)
{
    if(len!=lastlen) {
	SendDlgItemMessage(statuswnd, IDC_PROGRESS, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0,len));
	lastlen = len;
    }
    SendDlgItemMessage(statuswnd, IDC_PROGRESS, PBM_SETPOS, pos, 0);
    processMessages();
    Sleep(30);
}
void PropertyArchiveMessage(char*text)
{
    if(text && text[0]=='[') {
	return;
    }
    SetDlgItemText(statuswnd, IDC_INFO, strdup(text));
    processMessages();
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
	if(GetDiskFreeSpaceEx(install_path, &available, &total, &totalfree)) {
	    print_space(buf, "Space available: ", available.QuadPart);
	} else {
	    sprintf(buf, "Space available: [Error %d]", GetLastError());
	    if(GetLastError() == ERROR_FILE_NOT_FOUND && install_path[0] && install_path[1]==':') {
		/* installation directory does not yet exist */
		char path[3]={'c',':',0};
		path[0] = install_path[0];
		if(GetDiskFreeSpaceEx(path, &available, &total, &totalfree)) {
		    print_space(buf, "Space available: ", available.QuadPart);
		}
	    } 
	}
	SetDlgItemText(hwnd, IDC_SPACE2, buf);
    }
    if(message == WM_NOTIFY && (((LPNMHDR)lParam)->code == PSN_WIZNEXT)) {
	SetDlgItemText(hwnd, IDC_SPACE1, "");
	SetDlgItemText(hwnd, IDC_SPACE2, "");
	PropSheet_SetWizButtons(dialog, 0);
	SendMessage(dialog, PSM_CANCELTOCLOSE, 0, 0); //makes wine display a warning
	SetDlgItemText(hwnd, IDC_TITLE, "Installing files...");
	statuswnd = hwnd;
	status_t status;
	status.status = PropertyArchiveStatus;
	status.message = PropertyArchiveMessage;
	status.error = PropertyArchiveError;
	status.new_file = PropertyArchive_NewFile;
	status.new_directory = PropertyArchive_NewDirectory;
	int success = unpack_archive(crndata, crndata_len, install_path, &status);
	memset(&status, 0, sizeof(status_t));
	if(!success) {
	    MessageBox(0, "Couldn't extract all installation files", INSTALLER_NAME, MB_OK|MB_ICONERROR);
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
	SendDlgItemMessage(hwnd, IDC_DESKTOP, BM_SETCHECK, config_createDesktop, 0);
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
	    MessageBox(0, "Couldn't create Registry Entries", INSTALLER_NAME, MB_OK|MB_ICONERROR);
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
	
	char*uninstall_path = concatPaths(install_path, "uninstall.exe");

	if(config_createLinks) {
	    if(config_createDesktop && path_desktop[0]) {
		char* linkName = concatPaths(path_desktop, "pdf2swf.lnk");
                printf("Creating desktop link %s -> %s\n", linkName, pdf2swf_path);
		if(!CreateShortcut(pdf2swf_path, "pdf2swf", linkName, 0, 0, 0, install_path)) {
		    MessageBox(0, "Couldn't create desktop shortcut", INSTALLER_NAME, MB_OK);
		    return 1;
		}
	    }
	    if(config_createStartmenu && path_startmenu[0]) {
		char* group = concatPaths(path_startmenu, "pdf2swf");
		CreateDirectory(group, 0);
		char* linkName = concatPaths(group, "pdf2swf.lnk");
		if(!CreateShortcut(pdf2swf_path, "pdf2swf", concatPaths(group, "pdf2swf.lnk"), 0, 0, 0, install_path) ||
		   !CreateShortcut(uninstall_path, "uninstall", concatPaths(group, "uninstall.lnk"), 0, 0, 0, install_path)) {
		    MessageBox(0, "Couldn't create start menu entry", INSTALLER_NAME, MB_OK);
		    return 1;
		}
	    }
	} else {
            printf("not creating desktop/startmenu links\n");
        }

	char*uninstall_ini = concatPaths(install_path, "uninstall.ini");
	addFile(uninstall_ini);
	writeFileList(installedFiles, uninstall_ini);
    }
    return PropertySheetFuncCommon(hwnd, message, wParam, lParam, PSWIZB_FINISH);
}
#endif

#ifdef DEINSTALL

void findfiles(char*path, int*pos, char*data, int len, char del)
{
    WIN32_FIND_DATA findFileData;
    HANDLE hFind = FindFirstFile(concatPaths(path, "*"), &findFileData);
    if(hFind == INVALID_HANDLE_VALUE)
	return;
    do {
	if(findFileData.cFileName[0] == '.' &&
	   (findFileData.cFileName[0] == '.' || findFileData.cFileName == '\0'))
	    continue;
	char*f = concatPaths(path, findFileData.cFileName);
	if(findFileData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY) {
	    findfiles(f, pos, data, len, del);
	    if(del) {
		RemoveDirectory(f);
	    }
	} else {
	    int l = strlen(f);

	    /* don't list the uninstaller as file- it's going to be removed *after*
	       everything else is done */
	    char*uninstaller="uninstall.exe";
	    int ll = strlen(uninstaller);
	    if(l>=ll) {
		if(!strcasecmp(&f[l-ll],uninstaller)) {
		    continue;
		}
	    }

	    if(data) {
		if(*pos+l <= len) {
		    memcpy(&data[*pos], f, l);(*pos)+=l;
		    data[(*pos)++] = '\r';
		    data[(*pos)++] = '\n';
		    data[(*pos)] = 0;
		}
	    } else {
		(*pos) += l+2;
	    }
	    if(del) {
		DeleteFile(f);
	    }
	}
    } while(FindNextFile(hFind, &findFileData));
    FindClose(hFind);
}

static char*extrafiles = 0;

BOOL CALLBACK PropertySheetFunc5(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    HWND dialog = GetParent(hwnd);
    if(message == WM_INITDIALOG) {
	SetDlgItemText(hwnd, IDC_INFO, "Ready to deinstall");
    }
    if(message == WM_NOTIFY && (((LPNMHDR)lParam)->code == PSN_WIZNEXT)) {

	filelist_t* list = readFileList("uninstall.ini");
	if(!list) {
	    list = readFileList(concatPaths(install_path, "uninstall.ini"));
	    if(!list) {
		//Don't abort. If there's still something there, it'll be catched by the "extra files"
		//functionality later
		//MessageBox(0, "Couldn't determine installed files list- did you run uninstall twice?", INSTALLER_NAME, MB_OK);
		//exit(-1);
	    }
	}
	filelist_t* l = list;
	int num = 0;
	while(l) {num++;l=l->next;}

	PropSheet_SetWizButtons(dialog, 0);
	SendMessage(dialog, PSM_CANCELTOCLOSE, 0, 0);
	SetDlgItemText(hwnd, IDC_TITLE, "Uninstalling files...");
	SendDlgItemMessage(hwnd, IDC_PROGRESS, PBM_SETRANGE, 0, (LPARAM)MAKELONG(0,num));
	num = 0;
	l = list;
	while(l) {
	    SendDlgItemMessage(hwnd, IDC_PROGRESS, PBM_SETPOS, num, 0);
	    if(l->type=='F')
		DeleteFile(l->filename);
	    else if(l->type=='D')
		RemoveDirectory(l->filename);
	    else if(l->type=='I') 
		/* skip- we will remove ourselves later */;
	    num++;l = l->next;
	}

	int len = 0;
	findfiles(install_path, &len, 0, 0, 0);
	if(len) {
	    extrafiles = malloc(len);
	    int pos = 0;
	    findfiles(install_path, &pos, extrafiles, len, 0);
	} else {
	    PropSheet_RemovePage(dialog, 1, 0);
	}
	return 0;
    }
    return PropertySheetFuncCommon(hwnd, message, wParam, lParam, PSWIZB_BACK|PSWIZB_NEXT);
}

BOOL CALLBACK PropertySheetFunc6(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if(message == WM_INITDIALOG) {
	SendDlgItemMessage(hwnd, IDC_DELETEEXTRA, BM_SETCHECK, config_deleteextra, 0);
	if(extrafiles) {
	    SetDlgItemText(hwnd, IDC_FILELIST, extrafiles);
	}
    }
    if(message == WM_COMMAND) {
	if((wParam&0xffff) == IDC_DELETEEXTRA) {
	    config_deleteextra = SendDlgItemMessage(hwnd, IDC_DELETEEXTRA, BM_GETCHECK, 0, 0);
	    config_deleteextra ^=1;
	    SendDlgItemMessage(hwnd, IDC_DELETEEXTRA, BM_SETCHECK, config_deleteextra, 0);
	    return 0;
	}
    }
    if(message == WM_NOTIFY && (((LPNMHDR)lParam)->code == PSN_WIZNEXT)) {
	if(config_deleteextra) {
	    int pos = 0;
	    findfiles(install_path, &pos, 0, 0, 1);
	}
    }
    return PropertySheetFuncCommon(hwnd, message, wParam, lParam, PSWIZB_NEXT);
}

BOOL CALLBACK PropertySheetFunc7(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam) {
    if(message == WM_INITDIALOG) {
	// ...
    }
    return PropertySheetFuncCommon(hwnd, message, wParam, lParam, PSWIZB_FINISH);
}
#endif

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
#ifndef DEINSTALL
	{PropertySheetFunc1, IDD_LICENSE},
	{PropertySheetFunc2, IDD_INSTALLDIR},
	{PropertySheetFunc3, IDD_PROGRESS},
	{PropertySheetFunc4, IDD_FINISH},
#else
	{PropertySheetFunc5, IDD_SURE},
	{PropertySheetFunc6, IDD_EXTRAFILES},
	{PropertySheetFunc7, IDD_DEINSTALLED},
#endif
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

#ifdef DEINSTALL

static void remove_self()
{
    char exename[MAX_PATH];
    char batname[MAX_PATH];
    FILE *fp;

    GetModuleFileName(NULL, exename, sizeof(exename));
    sprintf(batname, "%s.bat", exename);
    fp = fopen(batname, "w");
    fprintf(fp, ":Repeat\n");
    fprintf(fp, "del \"%s\"\n", exename);
    fprintf(fp, "if exist \"%s\" goto Repeat\n", exename);
    fprintf(fp, "del \"%s\"\n", batname);
    fclose(fp);

    STARTUPINFO si;
    PROCESS_INFORMATION pi;
    memset(&si, 0, sizeof(si));
    si.dwFlags = STARTF_USESHOWWINDOW;
    si.wShowWindow = SW_HIDE;
    si.cb = sizeof(si);
    if (CreateProcess(NULL, batname, NULL, NULL, FALSE,
		      CREATE_SUSPENDED | IDLE_PRIORITY_CLASS,
		      NULL, "\\", &si, &pi)) {
	    SetThreadPriority(pi.hThread, THREAD_PRIORITY_IDLE);
	    SetThreadPriority(GetCurrentThread(), THREAD_PRIORITY_TIME_CRITICAL);
	    SetPriorityClass(GetCurrentProcess(), HIGH_PRIORITY_CLASS);
	    CloseHandle(pi.hProcess);
	    ResumeThread(pi.hThread);
	    CloseHandle(pi.hThread);
    }

}

int WINAPI WinMain(HINSTANCE _me,HINSTANCE hPrevInst,LPSTR lpszArgs, int nWinMode)
{
    me = _me;
    sprintf(registry_path, "Software\\%s\\%s\\InstallPath", SOFTWARE_DOMAIN, SOFTWARE_NAME);

    HWND background = 0;
    logo = LoadBitmap(me, "LOGO");

    install_path = getRegistryEntry(registry_path);
    if(!install_path || !install_path[0]) {
	MessageBox(0, "Couldn't find software installation directory- did you run the deinstallation twice?", INSTALLER_NAME, MB_OK);
	return 1;
    }

    CoInitialize(0);
    InitCommonControls();

    runPropertySheet(background);

    remove_self();
    return 0;
}
#else
int WINAPI WinMain(HINSTANCE _me,HINSTANCE hPrevInst,LPSTR lpszArgs, int nWinMode)
{
    me = _me;
    WNDCLASSEX wcl_background;
    wcl_background.hInstance    = me;
    wcl_background.lpfnWndProc  = WindowFunc;
    wcl_background.lpszClassName= INSTALLER_NAME;
    wcl_background.style        = CS_HREDRAW | CS_VREDRAW;
    wcl_background.hIcon        = LoadIcon(NULL, IDI_APPLICATION);
    wcl_background.hIconSm      = LoadIcon(NULL, IDI_APPLICATION);
    wcl_background.hCursor      = LoadCursor(NULL, IDC_ARROW);
    wcl_background.lpszMenuName = NULL; //no menu
    wcl_background.cbClsExtra   = 0;
    wcl_background.cbWndExtra   = 0;
    wcl_background.hbrBackground= CreateSolidBrush(RGB(0, 0, 128));
    wcl_background.cbSize       = sizeof(WNDCLASSEX);

    sprintf(registry_path, "Software\\%s\\%s\\InstallPath", SOFTWARE_DOMAIN, SOFTWARE_NAME);

    HINSTANCE shell32 = LoadLibrary("shell32.dll");
    if(!shell32) {
	MessageBox(0, "Could not load shell32.dll", INSTALLER_NAME, MB_OK);
	return 1;
    }
    f_SHGetSpecialFolderPath = (HRESULT (WINAPI*)(HWND,LPTSTR,int,BOOL))GetProcAddress(shell32, "SHGetSpecialFolderPathA");
    if(!f_SHGetSpecialFolderPath) {
	MessageBox(0, "Could not load shell32.dll", INSTALLER_NAME, MB_OK);
	return 1;
    }
	
    HRESULT coret = CoInitialize(NULL);
    if(FAILED(coret)) {
	MessageBox(0, "Could not initialize COM interface", INSTALLER_NAME, MB_OK);
	return 1;
    }

    path_programfiles[0] = 0;
    f_SHGetSpecialFolderPath(NULL, path_programfiles, CSIDL_PROGRAM_FILES, 0);

    if(!RegisterClassEx(&wcl_background)) {
        MessageBox(0, "Could not register window background class", INSTALLER_NAME, MB_OK);
        return 1;
    }

    HWND background = CreateWindow(wcl_background.lpszClassName, INSTALLER_NAME,
			 0, 0, 0, 
			 GetSystemMetrics(SM_CXFULLSCREEN),
			 GetSystemMetrics(SM_CYFULLSCREEN),
			 NULL, NULL, me, 
	                 (void*)"background");
    
    if(!background) {
	MessageBox(0, "Could not create installation background window", INSTALLER_NAME, MB_OK);
	return 1;
    }

    ShowWindow(background, SW_SHOWMAXIMIZED);
    SetForegroundWindow(background);
    UpdateWindow(background);

    logo = LoadBitmap(me, "LOGO");

    install_path = getRegistryEntry(registry_path);
    if(!install_path || !install_path[0]) {
        if(path_programfiles[0]) {
            install_path = concatPaths(path_programfiles, SOFTWARE_NAME);
        } else {
	    install_path = concatPaths("c:\\", SOFTWARE_NAME);
        }
    }

    InitCommonControls();

    runPropertySheet(background);
    return 0;
}
#endif
