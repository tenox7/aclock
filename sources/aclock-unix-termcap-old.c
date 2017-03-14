/*
 * aclock - ascii clock for UNIX Console - termcap port
 *
 * Copyright (c) 1994-2010 Antoni Sawicki <tenox@tenox.tc>
 * Version 2.0 (unix-termcap); Dublin, April 2010
 *
 * Compilation: cc aclock-unix-termcap.c -o aclock -ltermcap -lm
 * Built on: BeOS, Zeta, SkyOS, Haiku, Amiga-GG, UnixWare
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

char *cm, *cl;

void cls(void) {
	tputs(tgoto(cl, 0, 0), 1, &putchar);
}

void draw_point(int x, int y, char c) {
	tputs(tgoto(cm, x, y), 1, &putchar);
	putchar(c);
}

void draw_text(int x, int y, char *string) {
	tputs(tgoto(cm, x, y), 1, &putchar);
	printf("%s", string);
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
		draw_point(x,y,c);
	}
}

void draw_hand(int minute, int hlenght, char c, int sXcen, int sYcen, int FontHW){
	int x,y,n;
	float r=(minute-15)*(M_PI/180)*6;

	for(n=1; n<hlenght; n++){
		x=cos(r)*n*FontHW+sXcen;
		y=sin(r)*n+sYcen;
		draw_point(x,y,c);
	}
}


int main(int argc, char **argv){
	char INFO[]="Copyright (c) 1994-2010 Antoni Sawicki <tenox@tenox.tc>\n"
		    "Version 2.0 (unix-termcap); Dublin, April 2010\n";
	char digital_time[15];
	int FontHW = 2;
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
			draw_circle(hand_max, sYcen, sXcen, FontHW);
		}

		time(&t);
		ltime=localtime(&t);

		draw_hand((ltime->tm_hour*5)+(ltime->tm_min/10), 2*hand_max/3, 'h', sXcen, sYcen, FontHW);
		draw_hand(ltime->tm_min, hand_max-2, 'm', sXcen, sYcen, FontHW);
		draw_hand(ltime->tm_sec, hand_max-1, '.', sXcen, sYcen, FontHW);

		draw_text(sXcen-5, sYmax/4, ".:ACLOCK:.");
		snprintf(digital_time, 15, "[%02d:%02d:%02d]", ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
		draw_text(sXcen-5, 4*sYmax/5, digital_time);
		
		fflush(stdout);
		sleep(1);

		draw_hand((ltime->tm_hour*5)+(ltime->tm_min/10), 2*hand_max/3, ' ', sXcen, sYcen, FontHW);
		draw_hand(ltime->tm_min, hand_max-2, ' ', sXcen, sYcen, FontHW);
		draw_hand(ltime->tm_sec, hand_max-1, ' ', sXcen, sYcen, FontHW);
	}
	return 0;
}
