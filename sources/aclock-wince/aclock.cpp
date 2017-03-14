/*
 * aclock - ascii clock for Windows Mobile (WinCE) GUI
 *
 * Copyright (c) 1994-2012 by Antoni Sawicki <tenox@tenox.tc>
 * Copyright (c) 2012 by Michal Necasek <mnecasek@yahoo.com>
 * 
 */

#include "stdafx.h"
#include "aclock.h"
#include <commctrl.h>

#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846
#define MAX_LOADSTRING 100

#define ACLK_FONT       SYSTEM_FONT

/* */
#define TIMER_ID        1

//static char szClsName[] = "AClockClass";

typedef struct {
    LONG   pos_x;
    LONG   pos_y;
    LONG   chr_x;
    LONG   chr_y;
    LONG   max_x;
    LONG   max_y;
} CTX;


// Global Variables:
HINSTANCE			hInst;			// The current instance
HWND				hwndCB;			// The command bar handle

// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass	(HINSTANCE, LPTSTR);
BOOL				InitInstance	(HINSTANCE, int);
LRESULT CALLBACK	WndProc			(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK	About			(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(	HINSTANCE hInstance,
					HINSTANCE hPrevInstance,
					LPTSTR    lpCmdLine,
					int       nCmdShow)
{
	MSG msg;
	HACCEL hAccelTable;

	// Perform application initialization:
	if (!InitInstance (hInstance, nCmdShow)) 
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, (LPCTSTR)IDC_ACLOCK);

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0)) 
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg)) 
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	return msg.wParam;
}

void txt_init( CTX *ctx, LONG size_x, LONG size_y )
{
    ctx->chr_x = size_x;
    ctx->chr_y = size_y;
}

void txt_setrect( CTX *ctx, LONG size_x, LONG size_y )
{
    ctx->max_x = size_x / ctx->chr_x;
    ctx->max_y = size_y / ctx->chr_y;
}

void txt_setpos( CTX *ctx, LONG x, LONG y )
{
    ctx->pos_x = x;
    ctx->pos_y = y;
}

void txt_draw( CTX *ctx, HDC hdc, wchar_t *s )
{
    ExtTextOut( hdc, ctx->chr_x * ctx->pos_x, ctx->chr_y * ctx->pos_y, 	NULL, NULL, s, wcslen( s ), NULL );
}

void draw_text( CTX *ctx, HDC hdc, wchar_t *s, LONG x, LONG y )
{
    txt_setpos( ctx, x, y );
    txt_draw( ctx, hdc, s );
}

void draw_point( CTX *ctx, HDC hdc, LONG x, LONG y, wchar_t p )
{
    wchar_t s[4];

	swprintf(s, L"%c", p);

    txt_setpos( ctx, x, y );
    txt_draw( ctx, hdc, s );
}

void draw_circle( CTX *ctx, HDC hdc, LONG hand_max, LONG sXcen, LONG sYcen, float FontHW )
{
    LONG  x, y;
    int     r;
    wchar_t  c;

    for( r = 0; r < 60; ++r ) {
        x = cos( r * PI / 180 * 6 ) * hand_max * FontHW + sXcen;
        y = sin( r * PI / 180 * 6 ) * hand_max + sYcen;
        switch( r ) {
        case 0:
        case 5:
        case 10:
        case 15:
        case 20:
        case 25:
        case 30:
        case 35:
        case 40:
        case 45:
        case 50:
        case 55:
            c = L'o';
            break;
        default:
            c = L'.';
            break;
        }
        draw_point( ctx, hdc, x, y, c );
    }
}

void draw_hand( CTX *ctx, HDC hdc, int minute, LONG hlength, wchar_t c, LONG sXcen, LONG sYcen, float FontHW )
{
    LONG  x, y;
    int     n;
    float   r = (minute - 15) * (PI / 180) * 6;
    
    for( n = 1; n < hlength; ++n ) {
        x = cos( r ) * n * FontHW + sXcen;
        y = sin( r ) * n + sYcen;
        draw_point( ctx, hdc, x, y, c );
    }
}

void draw_clock( CTX *ctx, HDC hdc )
{
    wchar_t     digital_time[64];
    float       FontHW = 2;
    LONG       sXmax, sYmax, smax, hand_max, sXcen, sYcen;
    SYSTEMTIME st; 

	FontHW = (float)ctx->chr_y / ctx->chr_x;

    GetLocalTime(&st); 
    sXmax = ctx->max_x;
    sYmax = ctx->max_y -1 ;

    if( sXmax / 2 <= sYmax++ )
        smax = sXmax / 2;
    else
        smax = sYmax;

    hand_max = smax / 2 - 2;

    sXcen = sXmax / 2 ;
    sYcen = sYmax / 2 + 1;

    draw_circle( ctx, hdc, hand_max, sXcen, sYcen, FontHW );

    draw_hand( ctx, hdc, st.wHour * 5 + st.wMinute / 10, 2 * hand_max / 3, 'h', sXcen, sYcen, FontHW );
    draw_hand( ctx, hdc, st.wMinute, hand_max - 2, 'm', sXcen, sYcen, FontHW );
    draw_hand( ctx, hdc, st.wSecond, hand_max - 1, '.', sXcen, sYcen, FontHW );

    draw_text( ctx, hdc, L".:ACLOCK:.", sXcen - 5, sYcen / 3 );

    swprintf( digital_time, L"[%02d:%02d:%02d]", st.wHour, st.wMinute, st.wSecond);
    draw_text( ctx, hdc, digital_time, sXcen - 5, 4 * sYmax / 5);

}



ATOM MyRegisterClass(HINSTANCE hInstance, LPTSTR szWindowClass)
{
	WNDCLASS	wc;

    wc.style			= CS_HREDRAW | CS_VREDRAW;
    wc.lpfnWndProc		= (WNDPROC) WndProc;
    wc.cbClsExtra		= 0;
    wc.cbWndExtra		= 0;
    wc.hInstance		= hInstance;
    wc.hIcon			= LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ACLOCK));
    wc.hCursor			= 0;
    wc.hbrBackground	= (HBRUSH) GetStockObject(WHITE_BRUSH);
    wc.lpszMenuName		= 0;
    wc.lpszClassName	= szWindowClass;

	return RegisterClass(&wc);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND	hWnd;
	TCHAR	szTitle[MAX_LOADSTRING];			// The title bar text
	TCHAR	szWindowClass[MAX_LOADSTRING];		// The window class name

	hInst = hInstance;		// Store instance handle in our global variable
	// Initialize global strings
	LoadString(hInstance, IDC_ACLOCK, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance, szWindowClass);

	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	hWnd = CreateWindow(szWindowClass, szTitle, WS_VISIBLE,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	if (!hWnd)
	{	
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);
	if (hwndCB)
		CommandBar_Show(hwndCB, TRUE);

    /* Set up a 1000ms timer for the window. */
    if( !SetTimer( hWnd, TIMER_ID, 1000, 0 ) ) {
        MessageBox( 0, L"Too many timers in use!", NULL, MB_ICONHAND + MB_OK + MB_APPLMODAL  );
        return( FALSE );
    }


	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hdc;
	int wmId, wmEvent;
	PAINTSTRUCT ps;
    TEXTMETRIC      tm;
    HFONT           hfnt;
    RECT            rt;
    static CTX      context;


	switch (message) 
	{
		case WM_COMMAND:
			wmId    = LOWORD(wParam); 
			wmEvent = HIWORD(wParam); 
			// Parse the menu selections:
			switch (wmId)
			{
				case IDM_HELP_ABOUT:
				   DialogBox(hInst, (LPCTSTR)IDD_ABOUTBOX, hWnd, (DLGPROC)About);
				   break;
				case IDM_FILE_EXIT:
				   DestroyWindow(hWnd);
				   break;
				default:
				   return DefWindowProc(hWnd, message, wParam, lParam);
			}
			break;
		case WM_CREATE:
			hwndCB = CommandBar_Create(hInst, hWnd, 1);			
			CommandBar_InsertMenubar(hwndCB, hInst, IDM_MENU, 0);
			CommandBar_AddAdornments(hwndCB, 0, 0);

            hdc = GetDC( hWnd );
            hfnt = (HFONT) GetStockObject( ACLK_FONT );
            SelectObject( hdc, hfnt );
            GetTextMetrics( hdc, &tm );
            ReleaseDC( hWnd, hdc );
            txt_init( &context, tm.tmAveCharWidth, tm.tmHeight + tm.tmExternalLeading );
			break;
		case WM_PAINT:
			hdc = BeginPaint(hWnd, &ps);
			GetClientRect(hWnd, &rt);

            hfnt = (HFONT) GetStockObject( ACLK_FONT );
            SelectObject( hdc, hfnt );
    
            draw_clock( &context, hdc );

			EndPaint(hWnd, &ps);
			break;

		case WM_SIZE:
			GetClientRect( hWnd, &rt );
			txt_setrect( &context, rt.right - rt.left, rt.bottom - rt.top );
			break;

        
		case WM_TIMER:
            InvalidateRect( hWnd, NULL, TRUE );
            break;
    
        case WM_CLOSE:
            DestroyWindow( hWnd );
            break;

		case WM_DESTROY:
			CommandBar_Destroy(hwndCB);
            KillTimer( hWnd, TIMER_ID );
			PostQuitMessage(0);
			break;
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
   }
   return 0;
}

LRESULT CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
	RECT rt, rt1;
	int DlgWidth, DlgHeight;	// dialog width and height in pixel units
	int NewPosX, NewPosY;

	switch (message)
	{
		case WM_INITDIALOG:
			// trying to center the About dialog
			if (GetWindowRect(hDlg, &rt1)) {
				GetClientRect(GetParent(hDlg), &rt);
				DlgWidth	= rt1.right - rt1.left;
				DlgHeight	= rt1.bottom - rt1.top ;
				NewPosX		= (rt.right - rt.left - DlgWidth)/2;
				NewPosY		= (rt.bottom - rt.top - DlgHeight)/2;
				
				// if the About box is larger than the physical screen 
				if (NewPosX < 0) NewPosX = 0;
				if (NewPosY < 0) NewPosY = 0;
				SetWindowPos(hDlg, 0, NewPosX, NewPosY,
					0, 0, SWP_NOZORDER | SWP_NOSIZE);
			}
			return TRUE;

		case WM_COMMAND:
			if ((LOWORD(wParam) == IDOK) || (LOWORD(wParam) == IDCANCEL))
			{
				EndDialog(hDlg, LOWORD(wParam));
				return TRUE;
			}
			break;
	}
    return FALSE;
}
