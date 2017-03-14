/*
 * aclock - ascii clock for systems without floating point
*                               curses and ansi c compiler
 *
 * Copyright (c) 2002 Antoni Sawicki <as@tenoware.com>
 * Version 2.0 (nofloat-knr-termcap); Mountain View, June 2013
 *
 * Compilation: cc aclock-nofloat.c -o aclock
 *
 */


#include <stdio.h>
#include <time.h>
#include <termios.h>
#include <termcap.h>

#define FontWH_Ratio 2  

int cosa[]={    0, 105, 208, 309, 407, 500, 588, 669, 743, 809, 866, 914, 951, 978, 995,1000, 995, 978, 951, 914, 866, 809, 743, 669, 588, 500, 407, 309, 208, 105,   0,-105,-208,-309,-407,-500,-588,-669,-743,-809,-866,-914,-951,-978,-995,-1000,-995,-978,-951,-914,-866,-809,-743,-669,-588,-500,-407,-309,-208,-105 };
int sina[]={ -1000,-995,-978,-951,-914,-866,-809,-743,-669,-588,-500,-407,-309,-208,-105,   0, 105, 208, 309, 407, 500, 588, 669, 743, 809, 866, 914, 951, 978, 995,1000, 995, 978, 951, 914, 866, 809, 743, 669, 588, 500, 407, 309, 208, 105,   0,-105,-208,-309,-407,-500,-588,-669,-743,-809,-866,-914,-951,-978,-995 };

char *cm, *cl;

extern char *getenv();
extern char *tgetstr();
extern char *index();

putch(c) 
char c; {
    putchar(c);
}

cls() {
     tputs(tgoto(cl, 0, 0), 1, putch);
}

draw_point(x, y, c)
int x; int y; char c; {
    tputs(tgoto(cm, x, y), 1, putch);
    putchar(c);
}


draw_text(x, y, string) 
int x; int y; char *string; 
{
    tputs(tgoto(cm, x, y), 1, putch);
    printf("%s", string);
}

draw_hand(minute, hlenght, c, sXcen, sYcen)
int minute; int hlenght; char c; int sXcen; int sYcen; {
	int x,y,n;

    for(n=1; n<hlenght; n++)
    {
        x=(cosa[minute]*n*FontWH_Ratio/1000)+sXcen;
        y=(sina[minute]*n/1000)+sYcen;
        draw_point(x,y,c);
    }
}

draw_circle(hand_max, sYcen, sXcen)
int hand_max; int sYcen; int sXcen; {
    int x,y,r;
    char c;

    for(r=0;r<60;r++){
        x=(cosa[r]*hand_max*FontWH_Ratio/1000)+sXcen;
        y=(sina[r]*hand_max/1000)+sYcen;
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



main() {
    char INFO[]="Copyright (c) 1994-2013 Antoni Sawicki <as@tenoware.com>\n"
                "Version 2.3 (unix-nocfa); Mountain View, July 2013\n";
    char digital_time[15];
    time_t t;
    struct tm *ltime;
    int n;
    int sXmax, sYmax, smax, hand_max, sXcen, sYcen;
    char *term;
    char buff[1024];
    char termbuff[1024];
    struct winsize ws;
    struct termios tios;


    term=getenv("TERM");
    if(!term) {
        printf("TERM not set\n");
        return 1;
    }

    if(tgetent(buff, term)!=1) {
        printf("Unable to get termcap entry for %s\n", term);
        return 1;
    }

    cl=tgetstr("cl", NULL);
    cm=tgetstr("cm", NULL);
    

    while(1) {
        time(&t);
        ltime=localtime(&t);

        ioctl(1, TIOCGWINSZ, &ws);

        sXmax = ws.ws_col;
        sYmax = ws.ws_row;


        if(sXmax/FontWH_Ratio <= sYmax)
            smax=sXmax/FontWH_Ratio;
        else
            smax=sYmax;

        hand_max = (smax/2)-1;

        sXcen = sXmax/2;
        sYcen = sYmax/2;
        
        cls();

        draw_circle(hand_max, sYcen, sXcen);

        draw_hand((ltime->tm_hour*5)+(ltime->tm_min/10), 2*hand_max/3, 'h', sXcen, sYcen);
        draw_hand(ltime->tm_min, hand_max-2, 'm', sXcen, sYcen);
        draw_hand(ltime->tm_sec, hand_max-1, '.', sXcen, sYcen);
        
        draw_text(sXcen-5, sYcen-(3*hand_max/5), ".:ACLOCK:.");
        sprintf(digital_time, "[%02d:%02d:%02d]", ltime->tm_hour, ltime->tm_min, ltime->tm_sec);
        draw_text(sXcen-5, sYcen+(3*hand_max/5), digital_time);

        fflush(stdout);

        sleep(1);
    }

    return 0;
}
