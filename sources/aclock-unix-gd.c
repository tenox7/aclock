/*
 * aclock - not so ascii clock for web
 *
 * Copyright (c) 2002 Antoni Sawicki <as@tenoware.com>
 * Version 1.8 (unix-gd); Dublin, June 2002
 *
 * Compilation: gcc aclock-gd.c -o aclock.cgi -I../gd-2.0.33 -I../cgic205 
 * -L../gd-2.0.33/.libs -lgd -L../cgic205 -lcgic -lz -lpng -lm -liconv
 *
 */

/* Example HTML code:
<HTML>
  <HEAD>
    <META HTTP-EQUIV="Refresh" CONTENT="60;">
  </HEAD>
  <BODY BGCOLOR="#FFFFFF">
    <TABLE BORDER=1 CELLSPACING=2 CELLPADDING=10>
      <TR>
        <TD><IMG SRC="/cgi-bin/aclock.cgi?Universal" ALT="Universal Coordinated Time"></TD>
        <TD><IMG SRC="/cgi-bin/aclock.cgi?Greenwitch" ALT="Greenwitch Mean Time"></TD>
        <TD><IMG SRC="/cgi-bin/aclock.cgi?GB-Eire" ALT="British Summert Time"></TD>
        <TD><IMG SRC="/cgi-bin/aclock.cgi?Poland" ALT="Warsaw Time"></TD>
      </TR>
      <TR>
        <TD><IMG SRC="/cgi-bin/aclock.cgi?US/Eastern" ALT="USA Eastern Time"></TD>
        <TD><IMG SRC="/cgi-bin/aclock.cgi?US/Central" ALT="USA Central Time"></TD>
        <TD><IMG SRC="/cgi-bin/aclock.cgi?US/Mountain" ALT="USA Mountain Time"></TD>
        <TD><IMG SRC="/cgi-bin/aclock.cgi?US/Pacific" ALT="USA Pacific Time"></TD>
      </TR>
      <TR>
        <TD><IMG SRC="/cgi-bin/aclock.cgi?AU/Sydney" ALT="Australia Sydney"></TD>
        <TD><IMG SRC="/cgi-bin/aclock.cgi?AU/Perth" ALT="Australia Perth"></TD>
        <TD><IMG SRC="/cgi-bin/aclock.cgi?Singapore" ALT="Singapore"></TD>
        <TD><IMG SRC="/cgi-bin/aclock.cgi?Japan" ALT="Japan"></TD>
      </TR>
    </TABLE>
  </BODY>
</HTML>
*/

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <time.h>
#include <gd.h>

#include <gdfontt.h>
#include <gdfonts.h>

#include "cgic.h"

#define SIZE_X 85
#define SIZE_Y 85

void draw_circle(int hand_max, int sYcen, int sXcen, gdImagePtr whereto, int color_normal, int color_bold);
void draw_hand(int minute, int hlenght, int sXcen, int sYcen, gdImagePtr whereto, int color);

int cgiMain(void){
	char digital_time[15];
	char TZ[64];
	int sXmax, sYmax, smax, hand_max, sXcen, sYcen;
	time_t t;
	struct tm *ltime;
	gdImagePtr aclock;
	int black, white, blue, red, grey, dotted[4];

	snprintf(TZ, sizeof(TZ), "TZ=%s", cgiQueryString);
	putenv(TZ);
	tzset();

	aclock = gdImageCreate(SIZE_X, SIZE_Y);
	white =  gdImageColorAllocate(aclock, 255, 255, 255);
	black =  gdImageColorAllocate(aclock,   0,   0,   0);
	blue  =  gdImageColorAllocate(aclock,   0,   0, 255);
	red   =  gdImageColorAllocate(aclock, 255,   0,   0);
	grey  =  gdImageColorAllocate(aclock, 128, 128, 128);

	dotted[0] = blue;
	dotted[1] = blue;
	dotted[2] = white;
	dotted[3] = white;

	time(&t);
	ltime=localtime(&t);

	sXmax = SIZE_X;
	sYmax = SIZE_Y-gdFontSmall->h-2;

	if(sXmax<=sYmax)
		smax=sXmax;
	else
		smax=sYmax;

	hand_max = (smax/2)-1;

	sXcen = sXmax/2;
	sYcen = sYmax/2;

	draw_circle(hand_max, sYcen, sXcen, aclock, blue, red);

	draw_hand((ltime->tm_hour*5)+(ltime->tm_min/10), 3*hand_max/5, sXcen, sYcen, aclock, black);

	draw_hand(ltime->tm_min, hand_max-5, sXcen, sYcen, aclock, black);
	
	//gdImageSetStyle(aclock, dotted, 4);
	//draw_hand(ltime->tm_sec, hand_max-5, sXcen, sYcen, aclock, gdStyled);

	gdImageString(aclock, gdFontSmall, sXmax/2 - (gdFontSmall->w * strlen(cgiQueryString))/2, \
			SIZE_Y - gdFontSmall->h, cgiQueryString, blue);

	snprintf(digital_time, sizeof(digital_time), "%02d:%02d", ltime->tm_hour, ltime->tm_min);
	gdImageString(aclock, gdFontTiny, sXmax/2 - (gdFontTiny->w * strlen(digital_time))/2, \
			SIZE_Y*5/9, digital_time, grey);

	cgiHeaderContentType("image/gif");
	gdImageGif(aclock, cgiOut);
	gdImageDestroy(aclock);

	return 0;
}

void draw_circle(int hand_max, int sYcen, int sXcen, gdImagePtr whereto, int color_normal, int color_bold){
	int x,y,r, color;
	for(r=0;r<60;r++){
		x=(cos(r*M_PI/180*6)*hand_max)+sXcen;
		y=(sin(r*M_PI/180*6)*hand_max)+sYcen;

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
				gdImageFilledRectangle(whereto, x-1, y-1, x, y, color_bold);
				break;
			default:
				gdImageSetPixel(whereto, x, y, color_normal);
				break;
		}
	}
}

void draw_hand(int minute, int hlenght, int sXcen, int sYcen, gdImagePtr whereto, int color){
	float r=(minute-15)*(M_PI/180)*6;
	gdImageLine(whereto, sXcen, sYcen, cos(r)*hlenght+sXcen, sin(r)*hlenght+sYcen, color);
}

