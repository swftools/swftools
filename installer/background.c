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


