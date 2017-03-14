/*
 * aclock - ascii clock for OS/2 Text Mode
 *
 * Copyright (c) 2002-2006 Antoni Sawicki <tenox@tenox.tc>
 * Version 1.9 (os2-vio); Dublin, Dec 2006
 *
 * Compilation: Open Watcom 1.5
 *
 */

#define INCL_VIO

#include <os2.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define PI 3.14159265358979323846

void draw_point(int x, int y, char p){
        char string[2];
       
        string[0]=p;
        string[1]=0;

        VioWrtCharStr(string, 1, y, x, 0);
}

void draw_circle(int hand_max, int sXcen, int sYcen, int FontHW){
        double x,y;
        int r;
        char c;

 
        for(r=0;r<60;r++){
                x=cos(r*PI/180*6)*hand_max*FontHW+sXcen;
                y=sin(r*PI/180*6)*hand_max+sYcen;
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


void draw_hand(int minute, int hlenght, char c, int sXcen, int sYcen, int FontHW){
        double x,y;
        int n;
        float r=(minute-15)*(PI/180)*6;

        for(n=1; n<hlenght; n++){
                x=(cos(r)*n*FontHW)+sXcen;
                y=(sin(r)*n)+sYcen;
                draw_point(x,y,c);
        }
}


void draw_text(char *p, int x, int y){

        VioWrtCharStr(p, strlen(p), y, x, 0);

}



int main(void) {
        char info[]="Written by Antoni Sawicki <tenox@tenox.tc>\nVersion 1.9; Dublin, Dec 2006\n";
        char digital_time[15];
        short FontHW = 2;
        short sXmax, sYmax, smax, hand_max, sXcen, sYcen;
        time_t t;
        struct tm *ltime;
        VIOMODEINFO vmi;
        char s[3]={' ', 7, 0};

        vmi.cb=sizeof(VIOMODEINFO);

        for(;;){
                time(&t);
                ltime=localtime(&t);
                VioGetMode(&vmi, 0);
                sXmax=vmi.col;
                sYmax=vmi.row;

                if(sXmax/2<=sYmax++)
                        smax=sXmax/2;
                else
                        smax=sYmax;

                hand_max = (smax/2)-2;

                sXcen = (sXmax/2)-1;
                sYcen = (sYmax/2)-1;

                VioScrollUp (0, 0, 65535, 65535, 65535, s, 0);

                draw_circle(hand_max, sXcen, sYcen, FontHW);

                draw_hand(ltime->tm_hour*5+ltime->tm_min/10, 2*hand_max/3, 'h', sXcen, sYcen, FontHW);
                draw_hand(ltime->tm_min, hand_max-2, 'm', sXcen, sYcen, FontHW);
                draw_hand(ltime->tm_sec, hand_max-1, '.', sXcen, sYcen, FontHW);

                draw_text(".:ACLOCK:.", sXcen-5, sYcen/3);

                sprintf(digital_time, "[%02d:%02d:%02d]", ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
                draw_text(digital_time, sXcen-5, 4*sYmax/5);

                sleep(1);
        }
        
    return 0;
}

