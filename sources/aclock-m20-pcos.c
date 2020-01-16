/*
 * aclock - ascii clock for M20 PCOS
 *
 * Copyright (c) 2002-2004 Antoni Sawicki <tenox@tenox.tc>
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/pcos.h>

int cosa[]={    0, 105, 208, 309, 407, 500, 588, 669, 743, 809, 866, 914, 951, 978, 995,1000, 995, 978, 951, 914, 866, 809, 743, 669, 588, 500, 407, 309, 208, 105,   0,-105,-208,-309,-407,-500,-588,-669,-743,-809,-866,-914,-951,-978,-995,-1000,-995,-978,-951,-914,-866,-809,-743,-669,-588,-500,-407,-309,-208,-105 };
int sina[]={ -1000,-995,-978,-951,-914,-866,-809,-743,-669,-588,-500,-407,-309,-208,-105,   0, 105, 208, 309, 407, 500, 588, 669, 743, 809, 866, 914, 951, 978, 995,1000, 995, 978, 951, 914, 866, 809, 743, 669, 588, 500, 407, 309, 208, 105,   0,-105,-208,-309,-407,-500,-588,-669,-743,-809,-866,-914,-951,-978,-995 };

void draw_point(int x, int y, char p){
        char string[2];
       
        string[0]=p;
        string[1]=0;
        _pcos_chgcur0(x+1, y+1);
        _pcos_dstring(string);
}

void draw_circle(int hand_max, int sXcen, int sYcen, int FontHW){
        int x,y;
        int r;
        char c;

 
        for(r=0;r<60;r++){
                x=cosa[r]*hand_max*FontHW/1000+sXcen;
                y=sina[r]*hand_max/1000+sYcen;
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
        int x,y;
        int n;

        for(n=1; n<hlenght; n++){
                x=(cosa[minute]*n*FontHW/1000)+sXcen;
                y=(sina[minute]*n/1000)+sYcen;
                draw_point(x,y,c);
        }
}


void draw_text(char *p, int x, int y){
        _pcos_chgcur0(x+1, y+1);
        _pcos_dstring(p);
}



int main(void) {
        char info[]="Written by Antoni Sawicki <tenox@tenox.tc>\n";
        char digital_time[15];
        short FontHW = 2;
        short sXmax, sYmax, smax, hand_max, sXcen, sYcen;
        time_t t;
        struct tm *ltime;
        int status, color;
        void *pointer;

        _pcos_grfinit(&color, &pointer);
	_pcos_resetbyte(17);
        sYmax = 16;  /* TODO: _pcos_readwindow */
        sXmax = 64;

        for(;;){
                time(&t);
                ltime=localtime(&t);

                if(sXmax/2<=sYmax)
                        smax=sXmax/2;
                else
                        smax=sYmax;

                hand_max = (smax/2)-1;

                sXcen = sXmax/2;
                sYcen = sYmax/2;

                _pcos_cls();

                draw_hand(ltime->tm_hour*5+ltime->tm_min/10, 2*hand_max/3, 'h', sXcen, sYcen, FontHW);
                draw_hand(ltime->tm_min, hand_max-2, 'm', sXcen, sYcen, FontHW);
                draw_hand(ltime->tm_sec, hand_max-1, '.', sXcen, sYcen, FontHW);

                draw_circle(hand_max, sXcen, sYcen, FontHW);
                draw_text(".:ACLOCK:.", sXcen-5, sYcen/2);

                sprintf(digital_time, "[%02d:%02d:%02d]", ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
                draw_text(digital_time, sXcen-5, 4*sYmax/5);

                sleep(1);

		_pcos_eof(17, &status);
		if(!status)
			return 0;
        }
        
}
