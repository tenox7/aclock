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
 */

#include <stdio.h>
#include <float.h>
#include <time.h>

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

bool dots(int r){
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
			return true;
		default:
			return false;
	}
}

void draw_hand(int len,char c,int x,int y,float ratio,float sin,float cos){
	int xh, yh, h;

	for(h=1; h<len; h++){
		xh = sin * h * ratio + x;
		yh = cos * h + y;
		draw_point(xh, yh, c);
	}
}

int main(void){
	char digital_time[15];

	float ratio;

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

	int max_x = LINLEN;		// to total of columns in text mode
	int max_y = CRTCNT;		// to total of rows in text mode

	int smax, hand, cen_x, cen_y;
	int r, x, y;

	time_t t=0;

	struct tm *ltime;

	if ( max_x > 64 )
		ratio = 2.0;
	else {
		if ( max_x > 48 )
			ratio = 1.5;
		else
			ratio = 1.0;
	}

	if ( max_x / 2 <= max_y )
		smax=max_x/2;
	else
		smax=max_y;

	hand = (smax/2)-1;

	cen_x = max_x/2;
	cen_y = max_y/2;

// while(1){
	cls();

	// time = get_time();
	
	time(&t);
	ltime=localtime(&t);

 	for(r=0; r<60; r++){
		x = sin_t[r] * hand * ratio + cen_x;
		y = cos_t[r] * hand + cen_y;
		if (dots) {
		    draw_point(x, y, 'o');
		}
	}

	draw_text(cen_x-5, max_y/4, ".:ACLOCK:.");

	// sprintf(digital_time, "[%02d:%02d:%02d]", time[0 time[1], time[2]);

	sprintf(digital_time, "[%02d:%02d:%02d]", ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
	draw_text(cen_x-5, 4*max_y/5, digital_time);

//	draw_hand((ltime->tm_hour*5)+(ltime->tm_min/10), 2*hand_max/3, 'h', sXcen, sYcen, FontHW, sin_t[], cos_t[]);
//
	draw_hand(hand-2, 'm', cen_x, cen_y, ratio, sin_t[ltime->tm_min], cos_t[ltime->tm_min]);
//	draw_hand(ltime->tm_sec, hand_max-1, '.', sXcen, sYcen, FontHW, sin_t[], cos_t[ltime->tm_sec]);

	return 0;
}

