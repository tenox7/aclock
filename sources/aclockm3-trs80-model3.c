/*
 * aclock - ascii clock for TRS-80 Model III
 *
 * Based on Version 1.9 (msdos-watcom) 
 *  and 2.0 (nofloat-knr-termcap) by Antoni Sawicki 
 * Ported by John Dahlman
 *
 * Compiled with Mike Stump's zC 2.0
 * (which only works on the Model 3)
 *
 * zc aclockm3/c aclockm3/as
 * zas aclockm3/as aclockm3/lnk
 * zlink aclockm3/lnk aclockm3/cmd  
 *   Answer zlink's questions with:
 * In what file is $GINT? stdio/lnk                                
 * In what file is gotoxy? goto/lnk                                
 * In what file is printf? printf/lnk                              
 * In what file is getkey? stdio1/lnk                              
 * In what file is $main? cmdline1/lnk                             
 * In what file is utoi? utoi/lnk                                  
 * Optional: use SUPERZAP or PATCH to change BFBF to 8267    
 *
 * Only runs on Model 3. See aclockm1.c for Model 1
 * Tested on TRSDOS 1.3, NEWDOS/80 2.0, LDOS 5.3.1
 *
 */

#include<stdio.h>

#define FontWH_Ratio 3

#ifdef MODEL1

asm
ltime: EQU 16449 ;clock addr for Model 1
endasm

#else

asm
ltime: EQU 16919 ;clock addr for Model 3
endasm

#endif

extern struct {
  unsigned char tm_sec;
  unsigned char tm_min;
  unsigned char tm_hour;
} ltime;

int cosa[]={    0, 105, 208, 309, 407, 500, 588, 669, 743, 809,
              866, 914, 951, 978, 995,1000, 995, 978, 951, 914, 
              866, 809, 743, 669, 588, 500, 407, 309, 208, 105,
                0,-105,-208,-309,-407,-500,-588,-669,-743,-809,
             -866,-914,-951,-978,-995,-1000,-995,-978,-951,-914,
             -866,-809,-743,-669,-588,-500,-407,-309,-208,-105 };
int sina[]={ -1000,-995,-978,-951,-914,-866,-809,-743,-669,-588,
              -500,-407,-309,-208,-105,   0, 105, 208, 309, 407,
               500, 588, 669, 743, 809, 866, 914, 951, 978, 995,
              1000, 995, 978, 951, 914, 866, 809, 743, 669, 588,
               500, 407, 309, 208, 105,   0,-105,-208,-309,-407,
              -500,-588,-669,-743,-809,-866,-914,-951,-978,-995 };

draw_point(x, y, c)
int x; int y; char c; {
    gotoxy(x, y);
    putchar(c);
}

draw_hand(minute, hlenght, c, sXcen, sYcen)
int minute; int hlenght; char c; int sXcen; int sYcen; {
  int x,y,n;

  for(n=1; n<hlenght; n++) {
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

void sleep1sec() {
  unsigned char sec;
  sec=ltime.tm_sec;
  while(sec==ltime.tm_sec) ;
}


int main(void) {
  /* char INFO[]="Original Written by Antoni Sawicki\n
    TRS-80 Version by John Dahlman\n"; */ /* no room! */
  int sXmax, sYmax, smax, hand_max, sXcen, sYcen;
  
  sXmax = 64;
  sYmax = 16;

  if(sXmax/FontWH_Ratio <= sYmax)
    smax=sXmax/FontWH_Ratio;
  else
    smax=sYmax;

  hand_max = (smax/2)-1;

  sXcen = sXmax/2;
  sYcen = sYmax/2;

  for(;;){
    cls();

    draw_circle(hand_max, sYcen, sXcen);

    draw_hand(((ltime.tm_hour%12)*5)+(ltime.tm_min/10), 
      2*hand_max/3, 'h', sXcen, sYcen);
    draw_hand(ltime.tm_min, hand_max-2, 'm', sXcen, sYcen);
    draw_hand(ltime.tm_sec, hand_max-1, '.', sXcen, sYcen);

    gotoxy(sXcen-5, sYcen-(3*hand_max/5)); 
    printf(".:ACLOCK:.");
    gotoxy(sXcen-5, sYcen+(3*hand_max/5)); 
    printf("<%02u:%02u:%02u>", ltime.tm_hour, ltime.tm_min, 
      ltime.tm_sec);
    cursor_off();
    
    sleep1sec();

    if(getkey()) 
      return 0;
  }
  return 0;
}
