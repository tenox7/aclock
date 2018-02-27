/*
 * aclock - ascii clock for MSX, MSX2, MSX2+ & MSX turbo R machines
 *
 * Copyright (c) 2002-2018 by Antoni Sawicki <tenox@tenox.tc>
 * Copyright (c) 2018 by Giovanni Nunes <giovanni.nunes@gmail.com>
 *
 * Version 1.8 (sdcc-z80-msxdos); Campinas, February 2018
 *
 * To compile:
 * sdcc -I../msx -mz80 --no-std-crt0 --data-loc 0 --code-loc 0x107
 *      ../msx/crt0msx_msxdos.rel ../msx/putchar.rel ../msx/getchar.rel
 *      ../msx/dos.rel ../msx/conio.rel get_time.rel aclock.c
 *
 * Download the SDCC backend for MSXDOS in:
 * http://msx.atlantes.org/index_en.html#sdccmsxdos
 *
 */
#include <stdio.h>

#define LINLEN	(* (char *) 0xf3b0)
#define CRTCNT (* (char *) 0xf3b1)
#define CSRY 0xf3dc
#define CSRX 0xf3dd

static char * get_time(char *c);

static char* csry = (char *) CSRY;
static char* csrx = (char *) CSRX;

void cls(void){
	puts("\033E");
}

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

void draw_hand(int len,char c,int x,int y, int pos){
	int xh, yh, h;

	for(h=1; h<len; h++){
		xh = sin_t[pos] * h * ratio + x;
		yh = cos_t[pos] * h + y;
		draw_point(xh, yh, c);
	}
}

void main(void){

	char digital[15];

	static char dummy[]="HMSC";
	char *time;

	int cen_x, cen_y, hand, i, j, smax, x, y;

	int max_x = LINLEN;		// total of columns in text mode
	int max_y = CRTCNT;		// total of lines in text mode


	if ( max_x > 64 )		// MSX2/2+/turbo R supports 80 columns
		ratio = 2.0;		// text mode.
	else {
		if ( max_x > 40 )	// MSX2+/turbo R can operates in a 64
			ratio = 1.5;	// columns KANJI mode.
		else
			ratio = 1.0;	// for 40/32 columns text mode.
	}

	if ( max_x / 2 <= max_y )
		smax=max_x/2;
	else
		smax=max_y;

	hand = (smax/2)-1;
	cen_x = max_x/2;
	cen_y = max_y/2;

	j=-1;				// yo forces clock update

	puts("\033x5");			// disables cursor

	while(1){
		time = get_time(dummy);	// get current time using BDOS (CP/M)
		// routines, if there isn't a clock chip
		// the time will be allways 00:00:00

		if (j != time[2]){
			cls();

			draw_text(cen_x-5, max_y/4, ".:ACLOCK:.");

			sprintf(digital,"[%02d:%02d:%02d]",
					time[0],time[1],time[2]);

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

			draw_hand(2*hand/3, 'h', cen_x, cen_y, time[0]);
			draw_hand(hand-2, 'm', cen_x, cen_y, time[1]);
			draw_hand(hand-1, '.',cen_x, cen_y, time[2]);

		}
		j = time[2];		// holds the actual second value
	}
}

// MSX Rulez!
