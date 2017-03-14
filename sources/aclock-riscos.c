/*
 * aclock - ascii clock for Acorn RISC OS
 *
 * Copyright (c) 2002 Antoni Sawicki <tenox@tenox.tc>
 * Version 1.8 (riscos-vdu); Dublin, June 2002
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>
#include <swis.h>

#define M_PI 3.14159265

void sleep(int delay) {
    int start=0;

    start=_swi(OS_ReadMonotonicTime, 0);
    while(_swi(OS_ReadMonotonicTime, 0) <= start + delay) {  }
}

void cls(void) {
     (void) _swix(OS_WriteI + 12, 0);
}

void draw_point(int y, int x, char c) {
     (void) _swix(OS_WriteI + 31, 0);
     (void) _swix(OS_WriteI + x, 0);
     (void) _swix(OS_WriteI + y, 0);
     (void) _swix(OS_WriteI + c, 0);
}

void draw_text(int y, int x, char *str) {
    (void) _swix(OS_WriteI + 31, 0);
    (void) _swix(OS_WriteI + x, 0);
    (void) _swix(OS_WriteI + y, 0);
    (void) _swix(OS_WriteN, _INR(0,1), str, strlen(str));
}

void draw_circle(int hand_max, int sYcen, int sXcen, int FontHW){
    int x,y,r;
    char c;

    for(r=0;r<60;r++){
        x=cos(r*M_PI/180*6)*hand_max*FontHW+sXcen;
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
        draw_point(y, x, c);
    }
}



void draw_hand(int minute, int hlenght, char c, int sXcen, int sYcen, int FontHW){
    int x,y,n;
    float r=(minute-15)*(M_PI/180)*6;

    for(n=1; n<hlenght; n++){
        x=cos(r)*n*FontHW+sXcen;
        y=sin(r)*n+sYcen;
        draw_point(y,x,c);
    }
}


int main(void){
    char INFO[]="Copyright (c) 2002 by Antoni Sawicki <tenox@tenox.tc>\n"
                "Version 1.8; Dublin, June 2002\n";
    char digital_time[15];
    int FontHW = 2;
    int sXmax, sYmax, smax, hand_max, sXcen, sYcen;
    time_t t;
    struct tm *ltime;
    int vdu_vars[] = {256, 257, 167, 168, -1};


    (void) _swix(OS_ReadVduVariables, _INR(0,1), &vdu_vars, &vdu_vars);

    sXmax = vdu_vars[0];
    sYmax = vdu_vars[1];

    FontHW=vdu_vars[3]/vdu_vars[2];

    if(sXmax/2<=sYmax)
        smax=sXmax/2;
    else
        smax=sYmax;

    hand_max = (smax/2)-1;

    sXcen = sXmax/2;
    sYcen = sYmax/2;


    while(1){
        time(&t);
        ltime=localtime(&t);

        cls();

        draw_circle(hand_max, sYcen, sXcen, FontHW);

        draw_hand((ltime->tm_hour*5)+(ltime->tm_min/10), 2*hand_max/3, 'h', sXcen, sYcen, FontHW);
        draw_hand(ltime->tm_min, hand_max-2, 'm', sXcen, sYcen, FontHW);
        draw_hand(ltime->tm_sec, hand_max-1, '.', sXcen, sYcen, FontHW);

        draw_text(sYmax/4, sXcen-5, ".:ACLOCK:.");
        sprintf(digital_time, "[%02d:%02d:%02d]", ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
        draw_text(4*sYmax/5, sXcen-5, digital_time);

        sleep(100);
    }

    return 0;
}
