/*
 * aclock - ascii clock - Plan 9 Rio port (updated)
 *
 * Copyright (c) 2002-2005 Antoni Sawicki <tenox@tenox.tc>
 * Version 1.8 (plan9-rio); Dublin, March 2005
 *
 */


#include <u.h>
#include <stdio.h>
#include <libc.h>
#include <draw.h>
#include <cursor.h>

Image *white, *black, *blue;
Point size, pos;

void cls(void) {
	draw(screen, screen->r, white, nil, pos);
}

void draw_point(int x, int y, char c) {
	char str[2];
	str[0]=c;
	str[1]=0;
	string(screen, addpt(screen->r.min, Pt(x*size.x, y*size.y)), black, ZP, display->defaultfont, str);
}

void draw_text(int x, int y, char *str) {
	string(screen, addpt(screen->r.min, Pt(x*size.x, y*size.y)), blue, ZP, display->defaultfont, str);
}


void draw_circle(int hand_max, int sYcen, int sXcen, int FontHW){
	int x,y,r;
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
	int x,y,n;
	float r=(minute-15)*(PI/180)*6;

	for(n=1; n<hlenght; n++){
		x=cos(r)*n*FontHW+sXcen;
		y=sin(r)*n+sYcen;
		draw_point(x,y,c);
	}
}


int main(void){
	char INFO[]="Copyright (c) 2002 by Antek Sawicki <tenox@tenox.tc>\n"
		    "Version 1.8; Dublin, June 2002\n";
	char digital_time[15];
	int FontHW;
	int kbd, mse;
	int sXmax, sYmax, smax, hand_max, sXcen, sYcen;
	long t;
	struct Tm *ltime;

	initdraw(0,0,"aclock");
	white =  allocimage(display, Rect(0, 0, 1, 1), screen->chan, 1, DWhite);
	black = allocimage(display, Rect(0,0,1,1), screen->chan, 1, DBlack);
	blue = allocimage(display, Rect(0,0,1,1), screen->chan, 1, DBlue);
	kbd=open("/dev/consctl", OREAD);
	mse=open("/dev/mouse", OREAD);
	size=stringsize(display->defaultfont, " ");
	FontHW=size.y/size.x;

	while(1){
		time(&t);
		ltime=localtime(t);

		getwindow(display, Refnone);
		sXmax = (screen->r.max.x-screen->r.min.x)/size.x;
		sYmax = (screen->r.max.y-screen->r.min.y)/size.y;

		if((sXmax+2)/2<sYmax)
			smax=(sXmax/2)-2;
		else
			smax=sYmax;

		hand_max = (smax/2)-1;

		sXcen = sXmax/2;
		sYcen = sYmax/2;

		cls();
		draw_circle(hand_max, sYcen, sXcen, FontHW);

		draw_hand((ltime->hour*5)+(ltime->min/10), 2*hand_max/3, 'h', sXcen, sYcen, FontHW);
		draw_hand(ltime->min, hand_max-2, 'm', sXcen, sYcen, FontHW);
		draw_hand(ltime->sec, hand_max-1, '.', sXcen, sYcen, FontHW);

		draw_text(sXcen-5, sYmax/4, ".:ACLOCK:.");
		sprintf(digital_time, "[%02d:%02d:%02d]", ltime->hour, ltime->min, ltime->sec);
		draw_text(sXcen-5, 4*sYmax/5, digital_time); 

		flushimage(display, 1);
		sleep(1000);
	}
	return 0;
}
