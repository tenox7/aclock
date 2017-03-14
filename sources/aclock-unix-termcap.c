/*
 * aclock - ascii clock for UNIX Console - termcap port
 *
 * Copyright (c) 1994-2013 Antoni Sawicki <as@tenoware.com>
 * Version 2.3 (unix-termcap); Mountain View, July 2013
 *
 * Compilation: cc aclock-unix-termcap.c -o aclock -ltermcap -lm
 * Built on: BeOS, Zeta, SkyOS, Haiku, Amiga-GG, UnixWare, Risc/OS
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <termcap.h>
#include <math.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

#define FontWH_Ratio 2  

char *cm, *cl;

void my_putchar(int c) {
    putchar(c);
}

void cls(void) {
    tputs(tgoto(cl, 0, 0), 1, (void*)my_putchar);
}

void draw_point(int x, int y, char c) {
    tputs(tgoto(cm, x, y), 1, (void*)my_putchar);
    putchar(c);
}

void draw_text(int x, int y, char *string) {
    tputs(tgoto(cm, x, y), 1, (void*)my_putchar);
    puts(string);
}


void draw_circle(int hand_max, int sYcen, int sXcen){
    int x,y,r;
    char c;

    for(r=0;r<60;r++){
        x=cos(r*M_PI/180*6)*hand_max*FontWH_Ratio+sXcen;
        y=sin(r*M_PI/180*6)*hand_max+sYcen;
        switch (r) {
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
                c='o';
                break;
            default:
                c='.';
                break;
        }
        draw_point(x,y,c);
    }
}

void draw_hand(int minute, int hlenght, char c, int sXcen, int sYcen){
    int x,y,n;
    float r=(minute-15)*(M_PI/180)*6;

    for(n=1; n<hlenght; n++){
        x=cos(r)*n*FontWH_Ratio+sXcen;
        y=sin(r)*n+sYcen;
        draw_point(x,y,c);
    }
}


int main(int argc, char **argv){
    char INFO[]="Copyright (c) 1994-2013 Antoni Sawicki <as@tenoware.com>\n"
                "Version 2.3 (unix-termcap); Mountain View, July 2013\n";
    char digital_time[32];
    int sXmax, sYmax, sXmaxo, sYmaxo, smax, hand_max, sXcen, sYcen;
    time_t t;
    struct tm *ltime;
    char *term_env;
    char term_buffer[2048];
    struct winsize ws;
    struct termios tios;



    term_env=getenv("TERM");

    if(!term_env) {
        fprintf(stderr, "no terminal type defined\n");
        exit(1);
    }

    if(!tgetent(NULL, term_env)) {
        fprintf(stderr, "unknown terminal type\n");
        exit(1);
    }

    cm=(char*)tgetstr("cm", NULL);
    cl=(char*)tgetstr("cl", NULL);

    sXmaxo=sYmaxo=sXmax=sYmax=0;

    while(1){
        sXmaxo=sXmax;
        sYmaxo=sYmax;

        ioctl(1, TIOCGWINSZ, &ws);

        sXmax = ws.ws_col;
        sYmax = ws.ws_row;

        if((sXmax!=sXmaxo) || (sYmax!=sYmaxo)) {
            if(sXmax/2<=sYmax)
                    smax=sXmax/2;
            else
                smax=sYmax;

            hand_max = (smax/2)-1;

            sXcen = sXmax/2;
            sYcen = sYmax/2;

            cls();
            draw_circle(hand_max, sYcen, sXcen);
        }

        time(&t);
        ltime=localtime(&t);

        draw_hand((ltime->tm_hour*5)+(ltime->tm_min/10), 2*hand_max/3, 'h', sXcen, sYcen);
        draw_hand(ltime->tm_min, hand_max-2, 'm', sXcen, sYcen);
        draw_hand(ltime->tm_sec, hand_max-1, '.', sXcen, sYcen);

        draw_text(sXcen-5, sYcen-(3*hand_max/5), ".:ACLOCK:.");
        sprintf(digital_time, "[%02d:%02d:%02d]", ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
        draw_text(sXcen-5, sYcen+(3*hand_max/5), digital_time);
        
        fflush(stdout);
        sleep(1);

        draw_hand((ltime->tm_hour*5)+(ltime->tm_min/10), 2*hand_max/3, ' ', sXcen, sYcen);
        draw_hand(ltime->tm_min, hand_max-2, ' ', sXcen, sYcen);
        draw_hand(ltime->tm_sec, hand_max-1, ' ', sXcen, sYcen);
    }
    return 0;
}
