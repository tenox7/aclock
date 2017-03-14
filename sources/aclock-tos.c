/*
 * aclock - ascii clock for Atari TOS Text Mode
 *
 * Copyright (c) 2002-2004 Antoni Sawicki <tenox@tenox.tc>
 * Version 1.9a (atari-vdi); Dublin, July 2004
 *
 * Compilation: Lattice-C 5.60; Project file:
 --------------------------------------------
 ACLOCK.TOS
 .C	[-b4 -r6 -d0 -fl -m0 -rs -fm]
 .S	[-m0]
 .L	[NODEBUG BUFSIZE 1024]
 =
 C.O
 ACLOCK.C
 LCG.LIB
 LCM.LIB
 --------------------------------------------
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <aes.h>
#include <vdi.h>
#include <math.h>
#include <time.h>


short hConsole;

void cls(void){
	v_curhome(hConsole);
	v_eeos(hConsole);
}

void draw_point(int x, int y, char p){
	char string[2];
	
	string[0]=p;
	string[1]=0;

	vs_curaddress(hConsole, y, x);
	v_curtext(hConsole, string);
}

void draw_circle(int hand_max, int sXcen, int sYcen, int FontHW){
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
		x=(cos(r)*n*FontHW)+sXcen;
		y=(sin(r)*n)+sYcen;
		draw_point(x,y,c);
	}
}


void draw_text(char *p, int x, int y){
	vs_curaddress(hConsole, y, x);
	v_curtext(hConsole, p);
}


int main(int argc, char *argv[], char *envp[]) {
	char info[]="Written by Antoni Sawicki <tenox@tenox.tc>\nVersion 1.9a; Dublin, Mar 2004\n";
	char digital_time[15];
	short FontHW = 2;
	short sXmax, sYmax, smax, hand_max, sXcen, sYcen;
	time_t t;
	struct tm *ltime;
	short work_in[12] = {1,1,1,1,1,1,1,1,1,1,2,1};
	short work_out[57];
	short unused;
	short x,y,b;

	
	appl_init();
	hConsole=graf_handle(&unused, &unused, &unused, &unused);
	v_opnvwk(work_in, &hConsole, work_out);
	v_clrwk(hConsole);
	v_enter_cur(hConsole);

   	for(;;) {
		time(&t);
		ltime=localtime(&t);
		vq_chcells(hConsole, &sYmax, &sXmax);

		if(sXmax/2<=sYmax)
			smax=sXmax/2;
		else
			smax=sYmax;

		hand_max = (smax/2)-1;

		sXcen = sXmax/2;
		sYcen = sYmax/2;

		cls();

		draw_hand(ltime->tm_hour*5+ltime->tm_min/10, 2*hand_max/3, 'h', sXcen, sYcen, FontHW);
		draw_hand(ltime->tm_min, hand_max-2, 'm', sXcen, sYcen, FontHW);
		draw_hand(ltime->tm_sec, hand_max-1, '.', sXcen, sYcen, FontHW);

		draw_circle(hand_max, sXcen, sYcen, FontHW);
		draw_text(".:ACLOCK:.", sXcen-5, sYcen/3);

		sprintf(digital_time, "[%02d:%02d:%02d]", ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
		draw_text(digital_time, sXcen-5, 4*sYmax/5);

		vq_mouse(hConsole, &b, &x, &y);
		if(b)
			break;

		sleep(1);
	}

	v_exit_cur(hConsole);
	v_clsvwk(hConsole);
	appl_exit();
	
	return 0;

}
