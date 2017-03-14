/*
 * aclock - ascii clock for Windows Console
 *
 * Copyright (c) 1994-2010 by Antoni Sawicki <tenox@tenox.tc>
 * Version 2.2 (win32); Mountain View, November 2010
 *
 * Compile: cl aclock-win32.c
 *
 */

#include <windows.h>
#include <stdio.h>
#include <math.h>

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif


HANDLE hConsole;
CONSOLE_SCREEN_BUFFER_INFO console_info;

void cls(void){
    COORD home;
    DWORD size;
    int wchars;

    home.X = 0;
    home.Y = 0;
    size = console_info.dwSize.X  * console_info.dwSize.Y;
    FillConsoleOutputCharacter(hConsole, ' ', size, home, &wchars);
}

void draw_point(int x, int y, char p){
    COORD cursor_position;
    int wchars;
    cursor_position.X = x;
    cursor_position.Y = y;

    SetConsoleCursorPosition(hConsole, cursor_position);
    WriteConsole(hConsole, &p, 1, &wchars, NULL);
}

void draw_circle(int hand_max, int sYcen, int sXcen, int FontHW, int hour){
    int x,y,r;
    char c;

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
    float r=(minute-15)*(M_PI/180)*6;

    for(n=1; n<hlenght; n++){
        x=cos(r)*n*FontHW+sXcen;
        y=sin(r)*n+sYcen;
        draw_point(x,y,c);
    }
}


void draw_text(char p[], int x, int y){
    COORD cursor_position;
    int wchars;

    cursor_position.X = x;
    cursor_position.Y = y;

    SetConsoleCursorPosition(hConsole, cursor_position);
    WriteConsole(hConsole, p, strlen(p), &wchars, NULL);
}



int main(void){
    char INFO[]="Written by Antoni Sawicki <tenox@tenox.tc>"
                "Version 2.2; Mountain View, Nov 2010\n";
    char digital_time[15];
    int FontHW = 2;
    int sXmax, sYmax, smax, hand_max, sXcen, sYcen, wchars;
    CONSOLE_CURSOR_INFO cursor_info;
    SYSTEMTIME local_time;

    hConsole = GetStdHandle(STD_OUTPUT_HANDLE);
    SetConsoleTitle("aclock 2.2 (Win32)");
    GetConsoleScreenBufferInfo(hConsole, &console_info);
    
    cursor_info.bVisible = FALSE;
    cursor_info.dwSize = 1;
    SetConsoleCursorInfo(hConsole, &cursor_info);
    sXmax=sYmax=hand_max=sXcen=sYcen=0;

    while(1){

        GetConsoleScreenBufferInfo(hConsole, &console_info);

        sXmax = console_info.srWindow.Right;
        sYmax = console_info.srWindow.Bottom-console_info.srWindow.Top;

        if(sXmax/FontHW<=sYmax)
            smax=sXmax/FontHW;
        else
            smax=sYmax;

        hand_max = (smax/2)-1;

        sXcen = sXmax/2;
        sYcen = sYmax/2;

        GetLocalTime(&local_time);

        cls();

        draw_circle(hand_max, sYcen, sXcen, FontHW, local_time.wHour);

        draw_hand(local_time.wHour*5+local_time.wMinute/10, 2*hand_max/3, 'h', sXcen, sYcen, FontHW);
        draw_hand(local_time.wMinute, hand_max-2, 'm', sXcen, sYcen, FontHW);
        draw_hand(local_time.wSecond, hand_max-1, '.', sXcen, sYcen, FontHW);

        draw_text(".:ACLOCK:.", sXcen-5,sYcen-3*hand_max/5);
        sprintf(digital_time, "[%02d:%02d:%02d]", local_time.wHour, local_time.wMinute, local_time.wSecond);
        draw_text(digital_time, sXcen-5, sYcen+3*hand_max/5);

        Sleep(1000);

    }
    return 0;
}
