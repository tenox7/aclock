/*
 * aclock - ascii clock for MSX, MSX2, MSX2+ & MSX turbo R machines
 *
 * Copyright (c) 2002 Antoni Sawicki <tenox@tenox.tc>
 * Version 1.8 (z80p3cpm); Dublin, June 2002
 *
 * Compilation:
 * sdcc -I../msx -mz80 --no-std-crt0 --data-loc 0 --code-loc 0x107
 *      ../msx/crt0msx_msxdos.rel ../msx/putchar.rel ../msx/getchar.rel
 *      ../msx/dos.rel ../msx/conio.rel get_time.rel aclock.c
 *
 * To SDCC backend for MSXDOS go to:
 *    http://msx.atlantes.org/index_en.html#sdccmsxdos
 */

#include <stdio.h>
#include <float.h>

#define  LINLEN	       (* (char *) 0xf3b0)
#define  CRTCNT        (* (char *) 0xf3b1)

static char * get_time(char *c);

void cls(void){
	puts("\033E");
}

void draw_point(int x,int y,char c){
	printf("\033Y%c%c%c", y+32, x+32, c);
}

void draw_text(int x,int y,char *str){
	printf("\033Y%c%c%s", y+32, x+32, str);
}

void draw_hand(int len,char c,int x,int y,float ratio,float sin,float cos){
	int xh, yh, h;

	for(h=1; h<len; h++){
		xh = sin * h * ratio + x;
		yh = cos * h + y;
		draw_point(xh, yh, c);
	}
}

void main(void){

	char digital[15];

	static char dummy[]="HMSC";	// this is a very ugly dummy!
	char *time;

	float ratio;

	// Only need to know 60 values of sine...
	float sin_t[] = { -0.000004, 0.104524, 0.207908, 0.309013, 0.406733,
		0.499996, 0.587782, 0.669127, 0.743142, 0.809014, 0.866023,
		0.913543, 0.951055, 0.978147, 0.994521, 1.000000, 0.994522,
		0.978148, 0.951057, 0.913546, 0.866026, 0.809017, 0.743145,
		0.669131, 0.587786, 0.500001, 0.406738, 0.309018, 0.207913,
		0.104530, 0.000001, -0.104527, -0.207910, -0.309015, -0.406735,
		-0.499998, -0.587784, -0.669129, -0.743143, -0.809016,
		-0.866024, -0.913545, -0.951056, -0.978147, -0.994522,
		-1.000000, -0.994522, -0.978148, -0.951057, -0.913547,
		-0.866027, -0.809019, -0.743147, -0.669133, -0.587788,
		-0.500003, -0.406740, -0.309021, -0.207915, -0.104532 };

	// ...and cossine.
	float cos_t[] = { -1.000000, -0.994522, -0.978148, -0.951058,
		-0.913547, -0.866028, -0.809020, -0.743148, -0.669134,
		-0.587789, -0.500004, -0.406741, -0.309022, -0.207917,
		-0.104534, 0.000000, 0.104528, 0.207912, 0.309017, 0.406736,
		0.500000, 0.587785, 0.669130, 0.743144, 0.809017, 0.866025,
		0.913545, 0.951056, 0.978147, 0.994522, 1.000000, 0.994522,
		0.978148, 0.951057, 0.913546, 0.866026, 0.809018, 0.743146,
		0.669132, 0.587787, 0.500002, 0.406739, 0.309019, 0.207914,
		0.104531, 0.000003, -0.104526, -0.207909, -0.309014, -0.406734,
		-0.499997, -0.587783, -0.669128, -0.743143, -0.809015,
		-0.866024, -0.913544, -0.951055, -0.978147, -0.994521 };

	int max_x = LINLEN;		// total of columns in text mode
	int max_y = CRTCNT;		// total of lines in text mode

	int cen_x,cen_y,hand,i,j,smax,x,y;

	if ( max_x > 64 )		// MSX2/2+/turbo R supports 80 columns
		ratio = 2.0;		// text mode.
	else {
		if ( max_x > 48 )	// MSX2+ or turbo R machines can operate
			ratio = 1.5;	// in KANJI mode.
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

	j=-1;

	while(1){
		time = get_time(dummy);	// get current time using BDOS (CP/M)
					// routines, if there isn't a clock chip
					// the time will be allways 00:00:00

		if (j != time[2]){
			cls();

			draw_text(cen_x-5, max_y/4, ".:ACLOCK:.");

			sprintf(digital,"[%02d:%02d:%02d]",time[0],time[1],time[2]);

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

			draw_hand(2*hand/3,'h',cen_x,cen_y,ratio,sin_t[time[0]],cos_t[time[0]]);
			draw_hand(hand-2,'m',cen_x,cen_y,ratio,sin_t[time[1]],cos_t[time[1]]);
			draw_hand(hand-1,'.',cen_x,cen_y,ratio,sin_t[time[2]],cos_t[time[2]]);

		}
		j = time[2];
	}
}

// MSX Rulez!
