/*
 * aclock - analog clock for Tektronix 4014/4010 connected to a Win32 host
 *
 * Copyright (c) 2002 Antoni Sawicki <tenox@tenox.tc>
 * Version 1.8 (unix-tek); Dublin, June 2002
 *
 */


#include <windows.h>
#include <stdio.h>
#include <math.h>

//#define COMM "COM1"
//#define MODE "9600,n,7,1,x"
#define M_PI 3.14159265358979323846

HANDLE comport;
int w;
int sent;

void cls(void) {
    WriteFile(comport, "\x1b\x0c", 2, &w, NULL); sent+=w;
}

void lineto(int x, int y) {
    char ch[4];

    y=780-y;
    ch[0]=(y / 32) + 32;
    ch[1]=(y % 32) + 96;
    ch[2]=(x / 32) + 32;
    ch[3]=(x % 32) + 64;
    WriteFile(comport, ch, 4, &w, NULL);
    sent+=w;
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
        WriteFile(comport, "\x1d", 1, &w, NULL); sent+=w;
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
    WriteFile(comport, "\x1d", 1, &w, NULL); sent+=w;
    lineto(sXcen,sYcen);
	lineto(x,y);
}

void usage(void) {

    printf("usage: aclock-wintek.exe <COMx> <DCBstr>\n\nwhere:\nCOMx - COM1, COM2, COM3, etc\n"
           "DCBstr - \"9600,n,7,1,x\" for xon/xoff or \"9600,n,8,1\" for rts/cts\n\n"
           "example: aclock-wintek.exe COM2 9600,n,7,1,x\n\n");

}

int main(int argc, char **argv) { 
    DCB dcb;
    char *COMM;
    char *MODE;
	char digital_time[15];
	int sXmax, sYmax, smax, hand_max, sXcen, sYcen;
    SYSTEMTIME local_time;

    
    if(argc!=3) {
        usage();
        return 0;
    }

    COMM=argv[1];
    MODE=argv[2];

    comport=CreateFile(COMM, GENERIC_READ | GENERIC_WRITE, 0, NULL, OPEN_EXISTING, 0, NULL);

	if(comport==INVALID_HANDLE_VALUE) {
		fprintf(stderr, "I/O Error: Unable to open %s:.\n", COMM);
		return 1;
	}

	PurgeComm(comport, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);

	if(!GetCommState(comport, &dcb)) {
		fprintf(stderr, "I/O Error: Unable to retreive %s: state.\n", COMM);
		return 1;
	}

    if(!BuildCommDCB(MODE, &dcb)) {
		fprintf(stderr, "Build DCB for %s failed.\n", COMM);
		return 1;
	}


    if(!SetCommState(comport, &dcb)) {
		fprintf(stderr, "I/O Error: Unable to set communication parameters for %s:.\n", COMM);
		return 1;
	}

	PurgeComm(comport, PURGE_TXABORT | PURGE_RXABORT | PURGE_TXCLEAR | PURGE_RXCLEAR);
	EscapeCommFunction(comport, CLRBREAK);

    printf("%s initialized to %s\n", COMM, MODE);

	sXmax = 780;
	sYmax = 780;

	hand_max = 330;

	sXcen = sXmax/2;
	sYcen = sYmax/2;

	while(1) {
		GetSystemTime(&local_time);

		cls();
        WriteFile(comport, "\x1b\x60", 2, &w, NULL); sent+=w;
        WriteFile(comport, "\x1d", 1, &w, NULL); sent+=w;
    
		//draw_circle(hand_max, sYcen, sXcen );
		draw_mins(hand_max, sYcen, sXcen );

		draw_hand((local_time.wHour*5)+(local_time.wMinute/10), 2*hand_max/3, 'h', sXcen, sYcen);
		draw_hand(local_time.wMinute, 6*hand_max/7, 'm', sXcen, sYcen);
        WriteFile(comport, "\x1b\x62", 2, &w, NULL); sent+=w;
        draw_hand(local_time.wSecond, hand_max-25, '.', sXcen, sYcen);

        WriteFile(comport, "\x1d", 1, &w, NULL); sent+=w;
		lineto(sXcen-70, sYmax/4);
        WriteFile(comport, "\x1f", 1, &w, NULL); sent+=w;
        WriteFile(comport, ".:ACLOCK:.", 10, &w, NULL); sent+=w;
        WriteFile(comport, "\x1d", 1, &w, NULL); sent+=w;
		lineto(sXcen-70, 4*sYmax/5);
        WriteFile(comport, "\x1f", 1, &w, NULL); sent+=w;
 		sprintf(digital_time, "[%02d:%02d:%02d]", local_time.wHour, local_time.wMinute, local_time.wSecond);
        WriteFile(comport, digital_time, 10, &w, NULL); sent+=w;

        WriteFile(comport, "\x1d", 1, &w, NULL); sent+=w;
		lineto(1,1);

        FlushFileBuffers(comport);
        printf("%s Sent=%d\n", digital_time, sent);
        sent=0;
        Sleep(1000);
    }

    return 0;
}
