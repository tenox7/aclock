/* Aclock for Windows 1.x
 *
 * Builds with Microsoft C 4.0 and Windows 1.03/1.04 SDK
 * Compile:  cl -c -Gsw -Zdp -FPa -Os aclock.c
 * Link:     link4 /align:16 /map aclock,,,slibw,aclock.def
 */

#include <windows.h>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <time.h>

#define PI 3.14159265358979323846

/* Stock fixed font used for display; should be OEM_FIXED_FONT or ANSI_FIXED_FONT. */
#define ACLK_FONT       OEM_FIXED_FONT

/* */
#define TIMER_ID        1

static char szClsName[] = "AClockClass";

typedef struct {
    short   pos_x;
    short   pos_y;
    short   chr_x;
    short   chr_y;
    short   max_x;
    short   max_y;
} CTX;


void txt_init( ctx, size_x, size_y )
CTX *ctx;
short size_x, size_y;
{
    ctx->chr_x = size_x;
    ctx->chr_y = size_y;
}

void txt_setrect( ctx, size_x, size_y )
CTX *ctx;
short size_x, size_y;
{
    ctx->max_x = size_x / ctx->chr_x;
    ctx->max_y = size_y / ctx->chr_y;
}

void txt_setpos( ctx, x, y )
CTX *ctx;
short x, y;
{
    ctx->pos_x = x;
    ctx->pos_y = y;
}

void txt_draw( ctx, hdc, s )
CTX *ctx;
HDC hdc;
char *s;
{
    TextOut( hdc, ctx->chr_x * ctx->pos_x,
             ctx->chr_y * ctx->pos_y, s, strlen( s ) );
}

void draw_text( ctx, hdc, s, x, y )
CTX *ctx;
HDC hdc;
char *s;
short x, y;
{
    txt_setpos( ctx, x, y );
    txt_draw( ctx, hdc, s );
}

void draw_point( ctx, hdc, x, y, p )
CTX *ctx;
HDC hdc;
short x, y;
char p;
{
    char    s[2];

    s[0] = p;
    s[1] = '\0';

    txt_setpos( ctx, x, y );
    txt_draw( ctx, hdc, s );
}

void draw_circle( ctx, hdc, hand_max, sXcen, sYcen, FontHW )
CTX *ctx;
HDC hdc;
int hand_max;
int sXcen, sYcen;
double FontHW;
{
    double  x, y;
    int     r;
    char    c;

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
            c = 'o';
            break;
        default:
            c = '.';
            break;
        }
        draw_point( ctx, hdc, (short)x, (short)y, c );
    }
}

void draw_hand( ctx, hdc, minute, hlength, c, sXcen, sYcen, FontHW )
CTX *ctx;
HDC hdc;
int minute, hlength;
char c;
int sXcen, sYcen;
double FontHW;
{
    double  x, y;
    int     n;
    double  r = (minute - 15) * (PI / 180) * 6;

    for( n = 1; n < hlength; ++n ) {
        x = cos( r ) * n * FontHW + sXcen;
        y = sin( r ) * n + sYcen;
        draw_point( ctx, hdc, (short)x, (short)y, c );
    }
}

void draw_clock( ctx, hdc )
CTX *ctx;
HDC hdc;
{
    char        digital_time[15];
    double      FontHW = 2;
    short       sXmax, sYmax, smax, hand_max, sXcen, sYcen;
    time_t      t;
    struct tm   *ltime;

    FontHW = (double)ctx->chr_y / ctx->chr_x;

    time( &t );
    ltime = localtime( &t );
    sXmax = ctx->max_x;
    sYmax = ctx->max_y;

    if( sXmax / 2 <= sYmax++ )
        smax = sXmax / 2;
    else
        smax = sYmax;

    hand_max = smax / 2 - 2;

    sXcen = sXmax / 2 - 1;
    sYcen = sYmax / 2 - 1;

    draw_circle( ctx, hdc, hand_max, sXcen, sYcen, FontHW );

    draw_hand( ctx, hdc, ltime->tm_hour * 5 + ltime->tm_min / 10, 2 * hand_max / 3, 'h', sXcen, sYcen, FontHW );
    draw_hand( ctx, hdc, ltime->tm_min, hand_max - 2, 'm', sXcen, sYcen, FontHW );
    draw_hand( ctx, hdc, ltime->tm_sec, hand_max - 1, '.', sXcen, sYcen, FontHW );

    draw_text( ctx, hdc, ".:ACLOCK:.", sXcen - 5, sYcen / 3 );

    sprintf( digital_time, "[%02d:%02d:%02d]", ltime->tm_hour, ltime->tm_min, ltime->tm_sec );
    draw_text( ctx, hdc, digital_time, sXcen - 5, 4 * sYmax / 5);
}

long FAR PASCAL WndProc( hwnd, msg, wParam, lParam )
HWND hwnd;
unsigned msg;
WORD wParam;
LONG lParam;
{
    TEXTMETRIC      tm;
    PAINTSTRUCT     ps;
    HFONT           hfnt;
    HDC             hdc;
    RECT            r;
    static CTX      context;

    switch( msg ) {
    case WM_CREATE:
        hdc = GetDC( hwnd );
        hfnt = GetStockObject( ACLK_FONT );
        SelectObject( hdc, hfnt );
        GetTextMetrics( hdc, &tm );
        ReleaseDC( hwnd, hdc );
        txt_init( &context, tm.tmAveCharWidth, tm.tmHeight + tm.tmExternalLeading );
        break;

    case WM_SIZE:
        GetClientRect( hwnd, &r );
        txt_setrect( &context, r.right - r.left, r.bottom - r.top );
        break;

    case WM_PAINT:
        hdc = BeginPaint( hwnd, &ps );

        hfnt = GetStockObject( ACLK_FONT );
        SelectObject( hdc, hfnt );

        draw_clock( &context, hdc );

        EndPaint( hwnd, &ps );
        break;

    case WM_TIMER:
        InvalidateRect( hwnd, NULL, TRUE );
        break;

    case WM_CLOSE:
        DestroyWindow( hwnd );
        break;

    case WM_DESTROY:
        KillTimer( hwnd, TIMER_ID );
        PostQuitMessage( 0 );
        break;

    default:
        return( DefWindowProc( hwnd, msg, wParam, lParam ) );
    }
    return( FALSE );
}

/* Main program routine. */
int PASCAL WinMain( hInstance, hPrevInstance,lpszCmdLine, nCmdShow )
HANDLE hInstance, hPrevInstance;
LPSTR lpszCmdLine;
int nCmdShow;
{
    WNDCLASS    wc;
    HWND        hwnd;
    MSG         msg;

    /* Register window class if this is the first instance. */
    if( !hPrevInstance ) {
        wc.style         = CS_HREDRAW | CS_VREDRAW;
        wc.lpfnWndProc   = WndProc;
        wc.cbClsExtra    = 0;
        wc.cbWndExtra    = 0;
        wc.hInstance     = hInstance;
        wc.hIcon         = LoadIcon( NULL, IDI_APPLICATION );
        wc.hCursor       = LoadCursor( NULL, IDC_ARROW );
        wc.hbrBackground = GetStockObject( WHITE_BRUSH );
        wc.lpszMenuName  = NULL;
        wc.lpszClassName = szClsName;

        if( !RegisterClass( &wc ) ) {
            return( FALSE );
        }
    }

    /* Create and show the application window. */
    hwnd = CreateWindow( szClsName, "aclock",
                         WS_TILEDWINDOW,
                         0, 0,
                         0, 0,
                         NULL, NULL, hInstance, NULL) ;
    ShowWindow( hwnd, nCmdShow );
    UpdateWindow( hwnd );

    /* Set up a 1000ms timer for the window. */
    if( !SetTimer( hwnd, TIMER_ID, 1000, 0 ) ) {
        MessageBox( 0, "Too many timers in use!", NULL,
                    MB_ICONHAND + MB_OK + MB_SYSTEMMODAL );
        return( FALSE );
    }

    /* The classic message dispatch loop. */
    while( GetMessage( &msg, NULL, 0, 0 ) ) {
        TranslateMessage( &msg );
        DispatchMessage( &msg );
    }

    return( msg.wParam );
}
