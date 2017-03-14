/*
 * aclock - analog clock for Tektronix 4014/4010 or XTerm/Kermit Tek emulator
 *
 * Copyright (c) 2002 Antoni Sawicki <as@tenoware.com>
 * Version 1.8 (unix-tek); Dublin, June 2002
 *
 * Compilation: cc aclock-unix-tek.c -o aclock -lm
 *
 * Thanks to Bob Rosenbloom for extensive testing
 */

#include <stdio.h>
#include <unistd.h>
#include <math.h>
#include <time.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

void cls(void) {
	putchar(0x1b);
	putchar(0x0c);
}

void lineto(int x, int y) {
	y=780-y;
        putchar((y / 32) + 32);
        putchar((y % 32) + 96);
        putchar((x / 32) + 32);
        putchar((x % 32) + 64);
}


void draw_circle(int hand_max, int sYcen, int sXcen){
	int x,y,r;

	for(r=0;r<=60;r++){
		x=cos(r*M_PI/180*6)*hand_max+sXcen;
		y=sin(r*M_PI/180*6)*hand_max+sYcen;
		lineto(x,y);
	}
}

void draw_mins(int hand_max, int sXcen, int sYcen){
	int x,y,r;

	for(r=0;r<60;r++){
		putchar(0x1d);
		x=cos(r*M_PI/180*6)*hand_max+sXcen;
		y=sin(r*M_PI/180*6)*hand_max+sYcen;
		lineto(x,y);
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
				x=cos(r*M_PI/180*6)*(hand_max-14)+sXcen;
				y=sin(r*M_PI/180*6)*(hand_max-14)+sYcen;
				break;
			default:
				x=cos(r*M_PI/180*6)*(hand_max-5)+sXcen;
				y=sin(r*M_PI/180*6)*(hand_max-5)+sYcen;
				break;
		}
		lineto(x,y);
	}

}

void draw_hand(int minute, int hlenght, char c, int sXcen, int sYcen){
	int x,y,n;
	float r=(minute-15)*(M_PI/180)*6;

	x=cos(r)*hlenght+sXcen;
	y=sin(r)*hlenght+sYcen;
	putchar(0x1d);
	lineto(sXcen,sYcen);
	lineto(x,y);
}


int main(void){
	char INFO[]="Copyright (c) 2002 by Antek Sawicki <as@tenoware.com>\n"
		    "Version 1.8; Dublin, June 2002\n";
	char digital_time[15];
	int sXmax, sYmax, smax, hand_max, sXcen, sYcen;
	time_t t;
	struct tm *ltime;

	sXmax = 780;
	sYmax = 780;

	hand_max = 330;

	sXcen = sXmax/2;
	sYcen = sYmax/2;


	while(1) {
		time(&t);
		ltime=localtime(&t);
		cls();
		putchar(0x1b);
		putchar(0x60);
		putchar(0x1d);

		//draw_circle(hand_max, sYcen, sXcen );
		draw_mins(hand_max, sYcen, sXcen );


		draw_hand((ltime->tm_hour*5)+(ltime->tm_min/10), 2*hand_max/3, 'h', sXcen, sYcen);
		draw_hand(ltime->tm_min, 6*hand_max/7, 'm', sXcen, sYcen);
		putchar(0x1b);
		putchar(0x62);
		draw_hand(ltime->tm_sec, hand_max-25, '.', sXcen, sYcen);

		putchar(0x1d);
		lineto(sXcen-70, 230);
		putchar(0x1f);
		printf(".:ACLOCK:.");
		putchar(0x1d);
		lineto(sXcen-70, 560);
		putchar(0x1f);
 		printf("[%02d:%02d:%02d]", ltime->tm_hour, ltime->tm_min, ltime->tm_sec);

		putchar(0x1d);
		lineto(1,1);
		fflush(stdout);
		sleep(1);
	}
	return 0;
}
