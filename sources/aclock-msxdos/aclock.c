/*
 * aclock - ASCII clock for MSX-DOS to run on MSX2, MSX2+ & MSX turbo R
 *
 * Copyright (c) 2002-2018 by Antoni Sawicki <tenox@tenox.tc>
 * Copyright (c) 2018-2020 by Giovanni Nunes <giovanni.nunes@gmail.com>
 *
 * Version 1.9 (sdcc-z80-msxdos); Campinas, January 2020
 *
 * Download SDCC backend for MSXDOS at:
 * http://msx.atlantes.org/index_en.html#sdccmsxdos
 *
 * Use 'make' to compile.
 *
 */
#include <stdio.h>

#define LINLEN	(* (char *) 0xf3b0)
#define CRTCNT (* (char *) 0xf3b1)

#define CSRY 0xf3dc
#define CSRX 0xf3dd

static char * get_time(char *c);

static char* csry;
static char* csrx;

static float ratio;

typedef struct {
	char y;
	char x;
	char c;	
} clock_display;

void draw_point(int x,int y, char c){
	*csry = (char)y+1;
	*csrx = (char)x+1;
	putchar(c);
}

void draw_text(int x,int y,char *str){
	*csry = (char)y+1;
	*csrx = (char)x+1;
	puts(str);
}

void draw_hand(int len,char c,int x,int y, float sin, float cos){
	int xh, yh, h;
	for(h=1; h<len; h++){
		xh = sin * h * ratio + x;
		yh = cos * h + y;
		draw_point(xh, yh, c);
	}
}

void main(void){

	static char dummy[]="HMSC";

	clock_display analog[60];

	char digital[15];
	char *time;

	int cen_x, cen_y, hand, i, j, smax;
	int max_x = LINLEN;
	int max_y = CRTCNT;

	float sin_t[] = {
		-0.000,  0.105,  0.208,  0.309,  0.407,  0.500,
		 0.588,  0.669,  0.743,  0.809,  0.866,  0.914,
		 0.951,  0.978,  0.995,  1.000,  0.995,  0.978,
		 0.951,  0.914,  0.866,  0.809,  0.743,  0.669,
		 0.588,  0.500,  0.407,  0.309,  0.208,  0.105,
		 0.000, -0.105, -0.208, -0.309, -0.407, -0.500,
		-0.588, -0.669, -0.743,	-0.809, -0.866, -0.914,
		-0.951, -0.978, -0.995,	-1.000, -0.995, -0.978,
		-0.951, -0.914, -0.866, -0.809, -0.743, -0.669,
		-0.588, -0.500, -0.407,	-0.309, -0.208, -0.105 };

	float cos_t[] = {
		-1.000, -0.995, -0.978, -0.951, -0.914, -0.866,
		-0.809, -0.743, -0.669, -0.588, -0.500, -0.407,
		-0.309, -0.208, -0.105,  0.000,  0.105,  0.208,
		 0.309,  0.407,  0.500,  0.588,  0.669,  0.743,
		 0.809,  0.866,  0.914,  0.951,  0.978,  0.995,
		 1.000,  0.995,  0.978,  0.951,  0.914,  0.866,
		 0.809,  0.743,  0.669,  0.588,  0.500,  0.407,
		 0.309,  0.208,  0.105,  0.000, -0.105, -0.208,
		-0.309,- 0.407, -0.500, -0.588, -0.669, -0.743,
		-0.809, -0.866, -0.914, -0.951, -0.978, -0.995 };

	csry = (char *) CSRY;
	csrx = (char *) CSRX;

	if ( max_x > 64 )
		// MSX2, MSX 2+ and MSX turbo R using 80 columns mode (ASCII or Kanji-ROM)
		ratio = 2.0;
	else {
		if ( max_x > 40 )
			// MSX2, MSX 2+, MSX turbo R using 64 columns mode (maybe Kanji-ROM)
			ratio = 1.5;
		else
			// Any MSX model at 40/32 columns mode (ASCII and Kanji-ROM).
			ratio = 1.0;
	}

	if ( max_x / 2 <= max_y )
		smax = max_x / 2;
	else
		smax = max_y;

	hand = (smax / 2) - 1;
	cen_x = max_x / 2;
	cen_y = max_y / 2;

	// that's not necessary calculate this each second, so 
	for(i = 0; i < 60; i++) {
		analog[i].x = sin_t[i] * hand * ratio + cen_x;
		analog[i].y = cos_t[i] * hand + cen_y;
		switch (i) {
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
				analog[i].c = 'o';
				break;
			default:
				analog[i].c = '.';
			break;
		}
	}

	// to force the first clock update
	j=-1;

	// disable cursor on screen
	puts("\033x5");

	while(1){
		/*
			get current time using BDOS (CP/M BIOS) routines but if there isn't
			a real time clock circuit installed always "00:00:00" will be returned
		*/
		time = get_time(dummy);

		if (j != time[2]){

			puts("\033E");	// clear screen

			draw_text(cen_x - 5, max_y / 4, ".:ACLOCK:.");

			sprintf(digital,"[%02d:%02d:%02d]",
				time[0], time[1], time[2]);

			draw_text(cen_x - 5, 4*max_y / 5, digital);

			for(i = 0; i < 60; i++) {
				draw_point(analog[i].x, analog[i].y, analog[i].c);
			}

			// this is a 12 hour clock
			if (time[0] > 11)
				time[0] = 5 * (time[0] - 12);
			else
				time[0] = 5 * time[0];

			draw_hand(2 * hand / 3, 'h', cen_x, cen_y,
				sin_t[time[0]], cos_t[time[0]]);

			draw_hand(hand - 2, 'm', cen_x, cen_y,
				sin_t[time[1]], cos_t[time[1]]);

			draw_hand(hand - 1, '.', cen_x, cen_y,
				sin_t[time[2]], cos_t[time[2]]);

		}
		// save the seconds value
		j = time[2];
	}
}

// MSX Rulez!
