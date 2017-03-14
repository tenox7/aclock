/*
 * aclock - ascii clock for UNIX Console
 *
 * Copyright (c) 2002 by Antoni Sawicki <as@tenoware.com>
 * Version 1.8 (unix-aalib); Dublin, June 2002
 *
 * Compile: gcc -O2 aclock-unix-aalib.c -o aclock -laa -lm
 *
 */

#include <unistd.h>
#include <math.h>
#include <time.h>
#include <aalib.h>

aa_context *context;

void cls(int width, int height) {
	int x,y;
	for(x=0; x<=width; x++)
	for(y=0; y<=height; y++)
		aa_putpixel(context, x+1, y+1, 0);
}

void draw_circle(int hand_max, int sYcen, int sXcen, int FontHW){
	int x,y,r;
	int c;


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
				c=255;
				break;
			default:
				c=33;
				break;
		}
		aa_putpixel(context, x+1, y+1, c);
	}
}

void draw_hand(int minute, int hlenght, int c, int sXcen, int sYcen, int FontHW){
	int x,y,n;
	float r=(minute-15)*(M_PI/180)*6;

	for(n=1; n<hlenght; n++){
		x=cos(r)*n*FontHW+sXcen;
		y=sin(r)*n+sYcen;
		aa_putpixel(context,x,y,c);
	}
}


int main(void){
	char INFO[]="Copyright (c) 2002 by Antek Sawicki <as@tenoware.com>\n"
	            "Version 1.4; Dublin, Apr 2002\n";
	char digital_time[15];
	int FontHW = 2;
	int sXmax, sYmax, smax, hand_max, sXcen, sYcen;
	time_t t;
	struct tm *ltime;

	aa_recommendhidisplay("curses"); 
	aa_recommendlowdisplay("curses"); 

	context = aa_autoinit(&aa_defparams);

	if(context == NULL) {
		fprintf(stderr,"Cannot initialize AA-lib. Giving up.\n");
		exit(1);
	}

	for(;;){
		time(&t);
		ltime=localtime(&t);
		sXmax = aa_scrwidth(context) * 2 - 4;
		sYmax = aa_scrheight(context) * 2 - 4;

		if(sXmax/2<=sYmax)
			smax=sXmax/2;
		else
			smax=sYmax;

		hand_max = (smax/2)-1;

		sXcen = sXmax/2;
		sYcen = sYmax/2;

		cls(sXmax,sYmax);
		draw_circle(hand_max, sYcen, sXcen, FontHW);


		draw_hand((ltime->tm_hour*5)+(ltime->tm_min/10), 2*hand_max/3, 255, sXcen, sYcen, FontHW);
		draw_hand(ltime->tm_min, hand_max-2, 100, sXcen, sYcen, FontHW);
		draw_hand(ltime->tm_sec, hand_max-2, 33, sXcen, sYcen, FontHW);

		aa_fastrender(context, 0, 0, sXmax, sYmax);

		aa_printf(context, sXmax/4-4, sYmax/8, AA_BOLD, ".:ACLOCK:.");
		aa_printf(context, sXmax/4-4, sYmax/2.5, AA_NORMAL, "[%02d:%02d:%02d]",
					ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
		aa_flush(context);
		sleep(1);
	}
	return 0;
}
