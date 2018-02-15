/*
 * aclock - ascii clock for CP/M-80 / ZX Spectrum +3
 *
 * Copyright (c) 2002 Antoni Sawicki <tenox@tenox.tc>
 * Version 1.8 (z80p3cpm); Dublin, June 2002
 *
 * Compilation: c aclock.c -lf (HI-TECH C)
 *
 */

#include <stdio.h>
#include <math.h>
#include <time.h>

#define M_PI 3.14

void cls(void) {
	printf("\33E");
}

void draw_point(int x, int y, char c) {
	printf("\033Y%c%c%c", y+32, x+32, c);
}

void draw_text(int x, int y, char *str) {
	printf("\33Y%c%c%s", y+32, x+32, str);
}

void draw_circle(int hand_max, int sYcen, int sXcen, float FontHW){
	int x,y,r;
	char c;

	for(r=0;r<60;r++){
		x=cosf(r*M_PI/180*6)*hand_max*FontHW+sXcen;
		y=sinf(r*M_PI/180*6)*hand_max+sYcen;
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

void draw_hand(int minute, int hlenght, char c, int sXcen, int sYcen, float FontHW){
	int x,y,n;
	float r=(minute-15)*(M_PI/180)*6;

	for(n=1; n<hlenght; n++){
		x=cosf(r)*n*FontHW+sXcen;
		y=sinf(r)*n+sYcen;
		draw_point(x,y,c);
	}
}


int main(void){
	char digital_time[15];
	float FontHW = 1.5;
	int sXmax, sYmax, smax, hand_max, sXcen, sYcen;
	time_t t=0;
	time_t ot=0;

	struct tm *ltime;
	sXmax = 50;
	sYmax = 23;

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

		draw_text(sXcen-5, sYmax/4, ".:ACLOCK:.");
		sprintf(digital_time, "[%02d:%02d:%02d]", ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
		draw_text(sXcen-5, 4*sYmax/5, digital_time);

		draw_hand((ltime->tm_hour*5)+(ltime->tm_min/10), 2*hand_max/3, 'h', sXcen, sYcen, FontHW);
		draw_hand(ltime->tm_min, hand_max-2, 'm', sXcen, sYcen, FontHW);
		draw_hand(ltime->tm_sec, hand_max-1, '.', sXcen, sYcen, FontHW);
	}
}
