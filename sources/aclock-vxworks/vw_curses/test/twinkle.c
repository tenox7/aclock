#include <stdio.h>
#include <stdlib.h>
#include <curses.h>

/*
 * Curses demo program 'twinkle', from Sun Programmer's Guide vol. 2.
 * Before compiling, set TERMINAL definition below as appropriate.
 *
 * Notes:
 * This version does not use signals.
 * VxWorks version of curses always uses Def_term (there's no getenv()).
 * initscr() seems to require about 41000 bytes from malloc(), so
 *   at least 45000 should be made available.
 * Remember: The OK macro is set by curses.h, so it can't be used for
 *   VxWorks calls anymore! Use VX_OK instead (defined in ../h/stdio.h).
 */

#define TERMINAL "vt100"

#define	NCOLS 80
#define	NLINES 24
#define MAXPATTERNS 4

struct locs  {
   char y,x;
};

typedef struct locs LOCS;

LOCS Layout [NCOLS * NLINES];   /* current board layout */

int Pattern,                    /* current pattern number */
    Numstars;                   /* number of stars in pattern */

void makeboard();
bool ison(reg int y, reg int x);
void puton(reg char ch);

int main
    (
    int argc,
    char *argv[]
    )
    {
   int i;

   srandom(1);

   /* Initialize curses: set default terminal type (always used on
    * VxWorks) and call initscr(), checking to be sure there's enough
    * malloc() free store. Then set various parameters.
    */
   strcpy(Def_term,TERMINAL);
   if ((int)initscr() == ERR)  {
      fprintf(stderr,"twinkle: Error from initscr (not enough free store)\n");
      exit(1);
   }
   noecho();
   leaveok(stdscr, TRUE);
   scrollok(stdscr, FALSE);

   /* main loop */
   for (i=1; i<=10; i++)  {
      makeboard();              /* make the board setup */
      puton('*');               /* put on '*'s */
      puton(' ');               /* cover up with ' 's */
   }

   /* Shut down curses: move the cursor to the lower-left corner
    * by direct addressing, since current location is not guaranteed.
    * We lie and say we used to be at upper right corner to ensure
    * absolute addressing.
    */
   mvcur(0,COLS-1, LINES-1,0);
   fflush(stdout);     /* always flush after mvcur() */
   endwin();
}


/*
 * Make the current board setup. It picks a random pattern and calls
 * ison() to determine if the character is on that pattern or not.
 */
void makeboard()
{
   reg int y,x;
   reg LOCS *lp;

   Pattern = random() % MAXPATTERNS;
   lp=Layout;
   for (y=0; y<NLINES; y++)
      for (x=0; x<NCOLS; x++)
         if (ison(y,x))  {
            lp->y=y;
            lp++->x=x;
         }
   Numstars=lp-Layout;
}

/*
 * Return TRUE if (y,x) is on the current pattern.
 */
bool ison(reg int y, reg int x)
{
   switch (Pattern)  {
      case 0:     /* alternating lines */
         return !(y & 01);
      case 1:     /* box */
         if (x>=LINES && y>=NCOLS)
            return FALSE;
         if (y<3 || y>=NLINES-3)
            return TRUE;
         return(x<3 || x>=NCOLS-3);
      case 2:     /* holy pattern! */
         return((x+y) & 01);
      case 3:     /* bar across center */
         return(y>=9 && y<=15);
      default:
         exit(2);
   }
   /* NOTREACHED */
}

void puton(reg char ch)
{
   reg LOCS *lp;
   reg int r;
   reg LOCS *end;
   LOCS temp;

   end=&Layout[Numstars];
   for (lp=Layout; lp<end; lp++)  {
      r=random() % Numstars;
      temp=*lp;
      *lp=Layout[r];
      Layout[r]=temp;
   }

   for (lp=Layout; lp<end; lp++)  {
      mvaddch(lp->y, lp->x, ch);
      refresh();
   }
}
