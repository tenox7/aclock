#include <vxWorks.h>
#include <ctype.h>

/* The following must be defined by the linking application. If curses is used,
     they are defined by curses */
extern short ospeed;
extern char PC;

/* Times to transmit one serial character in tenths of milliseconds.
     Indexing is according to the speed codes returned by gtty(),
     e.g. code 7 == 300 baud == 33.3 milliseconds/char. */
static short chartime[] = { 0, 2000, 1333, 909, 743, 666, 500, 333, 166, 
                            83, 55, 41, 20, 10, 5 };


void tputs(cp, affcnt, outc)
   char *cp;
   int affcnt;
   int (*outc)();
/*
 * Output the character string 'cp' with padding as described by an initial
 * delay constant, in milliseconds, embedded in the string, e.g. "2\E[A". 
 * This routine should be used when outputting terminal capability strings 
 * from tgetstr(). Cursor motion strings should first be expanded using tgoto().
 * 'affcnt' is the number of lines affected ('*' code only).
 * 'outc' is the routine to be used to output each character.
 */
{
   int j;          /* index into 'cp' */
   int delay;      /* delay to be implemented, in 10ths of milliseconds */
   int delayc;     /* number of padding characters to output */

   if (cp==NULL)
      return;

   j=0;

   /* parse the delay code */
   delay=0;
   while (isdigit(cp[j]))  {
      delay*=10;
      delay+=cp[j++]-'0';
   }

   delay*=10;           /* change milliseconds to 10ths of ms */

   /* handle fractions */
   if (cp[j]=='.') {
      j++;
      if (isdigit(cp[j]))
         delay+=cp[j++]-'0';

      /* we only use the first fractional digit */
      while (isdigit(cp[j]))
         j++;
   }

   /* check for '*' and multiply by the affected lines count */
   if (cp[j]=='*')  {
      j++;
      delay*=affcnt;
   }

   /* output the main body of the string */
   while (cp[j]!=NULL)
      (*outc)(cp[j++]);

   /* don't delay if none specified or 'ospeed' is out of range */
   if (delay==0 || ospeed<= 0 || ospeed>=((sizeof chartime)/sizeof(short)))
      return;

   /* calculate padding needed to the nearest number of characters */
   delayc = (delay+chartime[ospeed]/2)/chartime[ospeed];

   /* transmit padding chars */
   while (delayc>0)  {
      (*outc)(PC);
      delayc--;
   }
}
