/*
 * aclock - ascii clock for MSX-DOS running on MSX2, MSX2+ & MSX turbo R
 *
 * Copyright (c) 2002-2018 by Antoni Sawicki <tenox@tenox.tc>
 * Copyright (c) 2018 by Giovanni Nunes <giovanni.nunes@gmail.com>
 *
 * Version 1.8 (sdcc-z80-msxdos); Campinas, February 2018
 *
 * Download the SDCC backend for MSXDOS at:
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

void draw_point(int x,int y, char c){
	*csry = (char)y+1;
	*csrx = (char)x+1;
	printf("%c",c);
}

void draw_text(int x,int y,char *str){
	*csry = (char)y+1;
	*csrx = (char)x+1;
	printf("%s",str);
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
	char digital[15];

	static char dummy[]="HMSC";
	char *time;

	int cen_x, cen_y, hand, i, j, smax, x, y;

	int max_x = LINLEN;
	int max_y = CRTCNT;

	float sin_t[] = { -0.000, 0.105, 0.208, 0.309, 0.407, 0.500, 0.588,
		0.669, 0.743, 0.809, 0.866, 0.914, 0.951, 0.978, 0.995, 1.000,
		0.995, 0.978, 0.951, 0.914, 0.866, 0.809, 0.743, 0.669, 0.588,
		0.500, 0.407, 0.309, 0.208, 0.105, 0.000, -0.105, -0.208,
		-0.309, -0.407, -0.500, -0.588, -0.669, -0.743, -0.809, -0.866,
		-0.914, -0.951, -0.978, -0.995, -1.000, -0.995, -0.978,	-0.951,
		-0.914, -0.866, -0.809, -0.743, -0.669, -0.588, -0.500, -0.407,
		-0.309, -0.208, -0.105 };

	float cos_t[] = { -1.000, -0.995, -0.978, -0.951, -0.914, -0.866,
		-0.809, -0.743, -0.669, -0.588, -0.500, -0.407, -0.309, -0.208,
		-0.105, 0.000, 0.105, 0.208, 0.309, 0.407, 0.500, 0.588, 0.669,
		0.743, 0.809, 0.866, 0.914, 0.951, 0.978, 0.995, 1.000, 0.995,
		0.978, 0.951, 0.914, 0.866, 0.809, 0.743, 0.669, 0.588, 0.500,
		0.407, 0.309, 0.208, 0.105, 0.000, -0.105, -0.208, -0.309,
		-0.407, -0.500, -0.588, -0.669, -0.743, -0.809, -0.866, -0.914,
		-0.951, -0.978, -0.995 };

	csry = (char *) CSRY;
	csrx = (char *) CSRX;

	if ( max_x > 64 )
		// MSX2/2+/turbo R supports 80 columns text mode.
		ratio = 2.0;
	else {
		if ( max_x > 40 )
			// MSX2+/turbo R operates in 64 columns too.
			ratio = 1.5;
		else
			// for 40/32 columns text mode.
			ratio = 1.0;
	}

	if ( max_x / 2 <= max_y )
		smax=max_x/2;
	else
		smax=max_y;

	hand = (smax/2)-1;
	cen_x = max_x/2;
	cen_y = max_y/2;

	// to forces the first clock update
	j=-1;

	// don't prints cursor
	puts("\033x5");

	while(1){
		// get current time using BDOS (CP/M) routines, if there isn't
		// a clock it will allways returns 00:00:00
		time = get_time(dummy);

		if (j != time[2]){

			puts("\033E");	// clear screen

			draw_text(cen_x-5, max_y/4, ".:ACLOCK:.");

			sprintf(digital,"[%02d:%02d:%02d]",
				time[0], time[1], time[2]);

			draw_text(cen_x-5, 4*max_y/5, digital);

			for(i=0; i<60; i+=5) {
				x = sin_t[i] * hand * ratio + cen_x;
				y = cos_t[i] * hand + cen_y;
				draw_point(x, y, 'o');
			}

			if (time[0]>11)
				time[0]=5*(time[0]-12);
			else
				time[0]=5*time[0];

			draw_hand(2*hand/3, 'h', cen_x, cen_y,
				sin_t[time[0]], cos_t[time[0]]);

			draw_hand(hand-2, 'm', cen_x, cen_y,
				sin_t[time[1]], cos_t[time[1]]);

			draw_hand(hand-1, '.',cen_x, cen_y,
				sin_t[time[2]], cos_t[time[2]]);

		}
		// stores the seconds value
		j = time[2];
	}
}

// MSX Rulez!
