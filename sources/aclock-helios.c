/*
 * aclock - ascii clock for HELIOS 1.3 (transputer)
 *
 * Copyright (c) 2002 Antoni Sawicki <as@tenoware.com>
 * Version 2.3 (unix-curses); Mountain View, July 2013
 *
 * Compilation: c -Fgs -O -D_BSD -lcurses -ltermcap -T4 -o aclockT4 aclock.c
 *              c -Fgs -O -D_BSD -lcurses -ltermcap -T8 -o aclockT8 aclock.c
 *
 */

#include <unistd.h>
#include <curses.h>
#include <math.h>
#include <time.h>
#include <string.h>
#include <signal.h>

#define M_6GRAD 0.10471975511965977461542144610932f
#define M_COS6  0.99452189536827333692269194498057f
#define M_SIN6  0.10452846326765347139983415480250f

#define FontWH_Ratio 2

char *INFO = "Copyright (c) 1994-2013 Antoni Sawicki <as@tenoware.com>\n"
             "Version 2.3 (unix-curses); Mountain View, July 2013\n";

extern void Delay( long int micros );

float sinf( float f ) {
    return (float)sin( (double)f );
}

float sqrtf( float f ) {
    return (float)sqrt( (double)f );
}

void draw_circle( int hand_max, int sXcen, int sYcen ){

    int   x, y, i = 1;
    char  c;
    float fx, fy, fx_old, hand_max_f = (float)hand_max;

    mvaddch( sYcen + hand_max, sXcen                          , 'o' ); /* 12 */
    mvaddch( sYcen           , sXcen + hand_max * FontWH_Ratio, 'o' ); /*  3 */
    mvaddch( sYcen - hand_max, sXcen                          , 'o' ); /*  6 */
    mvaddch( sYcen           , sXcen - hand_max * FontWH_Ratio, 'o' ); /*  9 */

    fx = M_COS6;
    fy = M_SIN6;
 
    for (;;) {

        c = ( i == 5 ) ? 'o' : '.';

        x = (int)( fx * ( hand_max_f * FontWH_Ratio ));
        y = (int)( fy * hand_max_f );

        mvaddch( sYcen + y, sXcen + x, c );
        mvaddch( sYcen + y, sXcen - x, c );
        mvaddch( sYcen - y, sXcen + x, c );
        mvaddch( sYcen - y, sXcen - x, c );

        x = (int)( fx * hand_max_f );
        y = (int)( fy * ( hand_max_f * FontWH_Ratio ));

        mvaddch( sYcen + x, sXcen + y, c );
        mvaddch( sYcen + x, sXcen - y, c );
        mvaddch( sYcen - x, sXcen + y, c );
        mvaddch( sYcen - x, sXcen - y, c );

        if ( i++ == 7 ) break;

        fx_old = fx;
        fx = fx_old * M_COS6 - fy     * M_SIN6;
        fy = fy     * M_COS6 + fx_old * M_SIN6;

    } /* endfor */
}

void draw_hand( int minute, int hlength, char c, int sXcen, int sYcen ) {

    float fx, fy, fn;

    fy = sinf( (float)( minute - 15 ) * M_6GRAD );
    fx = sqrtf( 1.0f - fy * fy ); if ( minute > 30 ) fx = -fx;
    fn = 1.0f;

    while ( hlength-- > 1 ) {
        mvaddch( sYcen + (int)( fy * fn )
               , sXcen + (int)( fx * ( fn * FontWH_Ratio ))
               , c );
        fn += 1.0f;
    } /* endwhile */
}

int main( void ) {

    int hand_max, sXcen, sYcen, tm_sec;
    time_t t;
    struct tm *ltime;

    initscr();

    time( &t );
    ltime = localtime( &t );

    for (;;) {

        tm_sec = ltime->tm_sec;

        sXcen = COLS  / 2;
        sYcen = LINES / 2;

        hand_max = sXcen / FontWH_Ratio - 1;

        if ( hand_max >= sYcen ) {
            hand_max = sYcen - 1;
        } /* endif */

        erase();

        draw_circle( hand_max, sXcen, sYcen );
        draw_hand( ltime->tm_sec, hand_max - 1, '.', sXcen, sYcen );
        draw_hand( ltime->tm_min, hand_max - 2, 'm', sXcen, sYcen );
        draw_hand(( ltime->tm_hour % 12 ) * 5 + ltime->tm_min / 10, 2 * hand_max / 3, 'h', sXcen, sYcen );

        mvaddstr( sYcen - 3 * hand_max / 5, sXcen - 5, ".:ACLOCK:." );
        mvprintw( sYcen + 3 * hand_max / 5, sXcen - 5, "[%02d:%02d:%02d]", ltime->tm_hour, ltime->tm_min, ltime->tm_sec );

        refresh();

        do { 
            Delay( 100 );
            time( &t );
            ltime = localtime( &t );
        } while ( ltime->tm_sec == tm_sec ); /* enddo */

    } /* endfor */

    endwin();

    return 0;
}
