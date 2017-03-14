#include <vxWorks.h>

#define MAXRESULTLEN 64

/* the following must be defined by the linking application. If curses is used,
     they are defined by curses */
extern char *UP;
extern char *BC;


char* tgoto(cmstr, destcol, destline)
   char* cmstr;
   int destcol;
   int destline;
/*
 * Interpret and expand cursor motion capability strings.
 * 'cmstr' is the capability string to use for cursor addressing, and
 *         describes how to output 'destcol' and 'destline' using %-escapes.
 *         'destline' is normally output first.
 * 'destcol' is the destination column number (horizontal).
 * 'destline' is the destination row number (vertical).
 *
 * The % escapes have the following meanings:
 *        %%     produce the character %
 *        %d     output value as in printf %d
 *        %2     output value as in printf %2d
 *        %3     output value as in printf %3d
 *        %.     output value as in printf %c
 *        %+x    add x to value, then do `%.'
 *        %>xy   if value > x then add y, no output
 *        %r     reverse order of two parameters, no output
 *        %i     increment by one, no output
 *        %n     exclusive-or all parameters with 0140 (Datamedia 2500)
 *        %B     BCD (16*(value/10)) + (value%10), no output
 *        %D     Reverse coding (value - 2*(value%16)), no output (Delta Data)
 *
 * Return value is a pointer to the expanded cursor motion string to be output.
 * The string "OOPS" is returned if there is an error.
 * ***Caution for VxWorks: This uses a static array to return the result.
 */
{
   static char result[MAXRESULTLEN];
   int is;                    /* current index in cmstr */
   int ir;                    /* current index in result */
   int coord;                 /* current coordinate, either col or line */
   int curcol;                /* TRUE if 'coord' is col, FALSE if line */
   int extracols = 0;         /* motion corrections from %. */
   int extralines = 0;
   int c;

#define COORD_SET  coord = (curcol ? destcol : destline)
#define COORD_SWAP curcol=!curcol; COORD_SET

   if (cmstr==NULL) {
      /* output "OOPS" on errors */
      return ("OOPS");
   }

   curcol = FALSE;
   COORD_SET;

   is=0;
   ir=0;

   while ((c=cmstr[is++])!=NULL)  {
      if (c != '%') {
         result[ir++]=c;
         continue;
      }

      /* get % escape code */
      c=cmstr[is++];

      switch (c) {

      case '2':
      case '3':
      case 'd':
         if (c=='3' || c=='d' && coord >= 100)  {
            result[ir++] = coord/100 + '0';
            coord %= 100;
         }
         if (c=='2' || c=='d' && coord >= 10)  {
            result[ir++] = coord/10 + '0';
         }
         result[ir++] = (coord % 10) + '0';
         COORD_SWAP;
         break;

      case '+':
      case '.':
         if (c=='+')
            coord += cmstr[is++];

         /* Output current coordinate in binary, but avoid outputting
              nulls, newlines, and cntl-D's because they may be interpreted
              as something else. We go "one ahead" instead, and back up using
              UP or BC */
         if (coord==0 || coord=='\n' || coord=='\004')  {
            if (curcol)  {
               extracols++;
               coord++;
            }
            else if (UP!=NULL)  {
               extralines++;
               coord++;
            }
         }
         result[ir++] = coord;
         COORD_SWAP;
         break;

      case '>':
         if (coord > cmstr[is++])
            coord += cmstr[is++];
         else
            is++;
         break;

      case 'r':
         curcol=TRUE;
         COORD_SET;
         break;

      case 'i':
         destcol++;
         destline++;
         coord++;
         break;

      case 'n':
         destcol ^= 0140;
         destline ^= 0140;
         COORD_SET;
         break;

      case 'B':
         coord = (coord/10 * 16) + (coord % 10);
         break;

      case 'D':
         coord = coord - 2*(coord % 16);
         break;

      case '%':
         result[ir++] = '%';
         break;

      default:
         return ("OOPS");
      }
   }

   result[ir]=NULL;            /* null-terminate result */

   while (extracols>0)  {
      strcat(result,((BC!=NULL) ? BC : "\b"));
      extracols--;
   }

   while (extralines>0)  {
      strcat(result,UP);
      extralines--;
   }

   return (result);
}
