#include <vxWorks.h>
#include <ioLib.h>
#include <ctype.h>

/*
 * This is termlib for VxWorks, designed to run in VxWorks environments
 * with or without a file system. Internal behaviour has been changed
 * slightly to remove hideous bugs and inverse bugs present in the
 * original Berkeley termlib.
 *
 * After looking in the termcap file TERMCAP_FILE, termcaps are searched 
 * for in the list of selected termcaps hard-coded below. This allows for
 * systems that have no termcap file, or no file system at all. To add
 * hard-coded termcaps, you must concatenate each capability to form one
 * long line.
 *
 * We avoid using stdio for historical reasons.
 *
 * A slight change has been made to avoid display delays after clear-screen: 
 * cl has been changed to the vt100 equivalent in the 'sun' and 'sun-cmd' 
 * hard-coded termcaps below.
 *
 * Georg Feil   91/08/01
 */


/* name of termcap file, use "" if no termcap file or no file system */
#define TERMCAP_FILE    ""

#define BUFSIZE         1024
#define TCMAX           32      /* max number of tc= references */

/* forward declarations */
LOCAL int checktc();
LOCAL int match();
LOCAL int skip();
LOCAL char* decode();

/* variables local to this module */
LOCAL char* termcap_file = TERMCAP_FILE;
LOCAL char* tbuf = NULL;     /* points to "current" termcap after tgetent() */
LOCAL int tcnest = 0;        /* for infinite loop detection */

/* hard-coded termcaps, searched after termcap file (caution -- long lines!) */
LOCAL char* termcaps[] = {

"d0|vt100|rosco|kermit|vt100-am|vt100am:do=^J:co#80:li#24:cl=50\\E[;H\\E[2J:sf=5\\ED:le=^H:bs:am:cm=5\\E[%i%d;%dH:nd=2\\E[C:up=2\\E[A:ce=3\\E[K:cd=50\\E[J:so=2\\E[7m:se=2\\E[m:us=2\\E[4m:ue=2\\E[m:md=2\\E[1m:mr=2\\E[7m:mb=2\\E[5m:me=2\\E[m:is=\\E[1;24r\\E[24;1H:rf=/usr/share/lib/tabset/vt100:rs=\\E>\\E[?3l\\E[?4l\\E[?5l\\E[?7h\\E[?8h:ks=\\E[?1h\\E=:ke=\\E[?1l\\E>:ku=\\E[A:kd=\\E[B:kr=\\E[C:kl=\\E[D:kb=^H:ho=\\E[H:pt:sr=5\\EM:vt#3:xn:sc=\\E7:rc=\\E8:cs=\\E[%i%d;%dr:k1=\\EOP:k2=\\EOQ:k3=\\EOR:k4=\\EOS:k5=\\EOT:k6=\\EOU:k7=\\EOV:k8=\\EOW:k9=\\EOX:k10=\\EOY:kD=\\EOZ",

"vs|xterm|vs100|xterm terminal emulator (X window system):cr=^M:do=^J:nl=^J:bl=^G:le=^H:ho=\\E[H:co#80:li#40:cl=\\E[H\\E[2J:bs:am:cm=\\E[%i%d;%dH:nd=\\E[C:up=\\E[A:ce=\\E[K:cd=\\E[J:so=\\E[7m:se=\\E[m:us=\\E[4m:ue=\\E[m:md=\\E[1m:mr=\\E[7m:me=\\E[m:ku=\\E[A:kd=\\E[B:kr=\\E[C:kl=\\E[D:kb=^H:k1=\\E[11~:k2=\\E[12~:k3=\\E[13~:k4=\\E[14~:k5=\\E[15~:k6=\\E[17~:k7=\\E[18~:k8=\\E[19~:ta=^I:pt:sf=\\n:sr=\\EM:al=\\E[L:dl=\\E[M:ic=\\E[@:dc=\\E[P:MT:ks=\\E[?1h\\E=:ke=\\E[?1l\\E>:is=\\E[r\\E[m\\E[2J\\E[H\\E[?7h\\E[?1;3;4;6l:rs=\\E[r\\E<\\E[m\\E[2J\\E[H\\E[?7h\\E[?1;3;4;6l:xn:AL=\\E[%dL:DL=\\E[%dM:IC=\\E[%d@:DC=\\E[%dP:ti=\\E7\\E[?47h:te=\\E[2J\\E[?47l\\E8:hs:ts=\\E[?E\\E[?%i%dT:fs=\\E[?F:es:ds=\\E[?E:",

"Mu|sun:am:bs:km:mi:ms:pt:li#34:co#80:cl=\\E[;H\\E[2J:cm=\\E[%i%d;%dH:ce=\\E[K:cd=\\E[J:so=\\E[7m:se=\\E[m:us=\\E[4m:ue=\\E[m:rs=\\E[s:md=\\E[1m:mr=\\E[7m:me=\\E[m:al=\\E[L:dl=\\E[M:im=:ei=:ic=\\E[@:dc=\\E[P:AL=\\E[%dL:DL=\\E[%dM:IC=\\E[%d@:DC=\\E[%dP:up=\\E[A:nd=\\E[C:ku=\\E[A:kd=\\E[B:kr=\\E[C:kl=\\E[D:k1=\\E[224z:k2=\\E[225z:k3=\\E[226z:k4=\\E[227z:k5=\\E[228z:k6=\\E[229z:k7=\\E[230z:k8=\\E[231z:k9=\\E[232z:",

"M+|sun-cmd:te=\\E[>4h:ti=\\E[>4l:am:bs:km:mi:ms:pt:li#34:co#80:cl=\\E[;H\\E[2J:cm=\\E[%i%d;%dH:ce=\\E[K:cd=\\E[J:so=\\E[7m:se=\\E[m:us=\\E[4m:ue=\\E[m:rs=\\E[s:md=\\E[1m:mr=\\E[7m:me=\\E[m:al=\\E[L:dl=\\E[M:im=:ei=:ic=\\E[@:dc=\\E[P:AL=\\E[%dL:DL=\\E[%dM:IC=\\E[%d@:DC=\\E[%dP:up=\\E[A:nd=\\E[C:ku=\\E[A:kd=\\E[B:kr=\\E[C:kl=\\E[D:k1=\\E[224z:k2=\\E[225z:k3=\\E[226z:k4=\\E[227z:k5=\\E[228z:k6=\\E[229z:k7=\\E[230z:k8=\\E[231z:k9=\\E[232z:",

"PC|nansi|ansi25|IBM PC/NANSI.SYS:al=\\E[L:am:bs:ce=\\E[K:cl=\\E[2J:cm=\\E[%i%d;%dH:co#80:DO=\\E[%dB:LE=\\E[%dD:RI=\\E[%dC:UP=\\E[%dA:dc=\\E[P:dl=\\E[M:do=\\E[B:ei=:eo:ho=\\E[1;0H:ic=\\E[@:im=:is=\\E[=2h\\E[=7h\\E[0;1;33m:kb=^H:le=\\E[D:li#25:mi:mb=\\E[5m:md=\\E[1;37m:me=\\E[0;1;33m:mr=\\E[0;30;46m:ms:nd=\\E[C:ku=\\EOM:kd=\\EOU:kr=\\EOR:kl=\\EOP:so=\\E[0;30;46m:se=\\E[0;1;33m:up=\\E[A:us=\\E[0;30;46m:ue=\\E[0;1;33m:pt:xt:k1=\\EO@:k2=\\EOA:k3=\\EOB:k4=\\EOC:k5=\\EOD:k6=\\EOE:k7=\\EOF:k8=\\EOG:",

"dw|vt52:do=^J:le=^H:bs:cd=\\EJ:ce=\\EK:cl=\\EH\\EJ:cm=\\EY%+\\040%+\\040:co#80:li#24:nd=\\EC:pt:sr=\\EI:up=\\EA:ku=\\EA:kd=\\EB:kr=\\EC:kl=\\ED:kb=^H:",

"" };        /* null-string marks the end */

void set_termcap_file(char *file)
{
    termcap_file = file;
}

int tgetent(bp, name)
   char* bp;
   char* name;
/*
 * Get an entry from the termcap file. Reads through the list of termcap
 * entries in the file TERMCAP_FILE and in the array termcaps[]. It will 
 * participate in indirect recursion if the call to tnchktc() finds a 
 * tc= field.
 * 'bp' returns the termcap entry. There must be room for 1024 characters.
 * 'name' is the name if the terminal capability to look up.
 * Return value is 1 if entry found, 0 if entry not found, -1 if could not
 * open termcap file.
 */
{
   int j;
   int c;
   int err_result;        /* value to return on error */ 
   char buf[BUFSIZE];     /* buffer for reading termcap file */
   int nw;                /* number of bytes written into buffer 'buf' */
   int nr;                /* number of bytes read from buffer 'buf' */
   int hardline;          /* current line number in hard-coded termcaps */
   int fd;                /* file descriptor for termcap file */

   nr=nw=0;
   hardline=0;
   err_result=0;

   if (termcap_file[0]!=NULL)  {
      fd=open(termcap_file, O_RDONLY, 0444);    /* open for read */
      if (fd==-1)  {
         err_result=-1;
      }
      else  {
         while (fd!=-1)  {
            /* read termcap file in big chunks, extracting entire termcap 
                 entries */
            j=0;
            while (TRUE)  {
               if (nr>=nw)  {
                  nw=read(fd,buf,BUFSIZE);
                  if (nw <= 0) {
                     close(fd);
                     fd=-1;
                     break;            /* quit both while loops */
                  }
                  nr=0;
               }
               c=buf[nr++];
               if (c=='\n') {
                  if (j>0 && bp[j-1]=='\\')  {
                     j--;
                     continue;
                  }
                  break;       /* quit while(TRUE) loop */
               }

               if (j>=BUFSIZE) {
                  write(STD_ERR,"Termcap file entry too long\n",28);
                  break;
               }

               bp[j++] = c;
            }

            bp[j]=NULL;       /* null-terminate */

            if (match(bp,name))  {
               close(fd);
               return(checktc(bp));
            }
         }
      }
   }

   /* read hard-coded termcaps until we reach the end of the termcaps[] array
        (terminated by a null string) */
   while (termcaps[hardline][0]!=NULL)  {
      if (strlen(termcaps[hardline])>=BUFSIZE) {
         write(STD_ERR,"Hard-coded termcap entry too long\n",34);
         strncpy(bp,termcaps[hardline],BUFSIZE-1);
         bp[BUFSIZE-1]=0;    /* null-terminate */ 
      }
      else  {
         strcpy(bp,termcaps[hardline]);
      }

      if (match(bp,name))  {
         return(checktc(bp));
      }

      hardline++;
   }

   return (err_result);          /* termcap not found in hard-coded array */
}

int tgetnum(cap)
   char* cap;
/*
 * Search for numeric capabilities of the form li#80.
 * The capability name is separated from the numeric value by a '#'.
 * Numbers starting with a 0 are treated as octal.
 * 'cap' is the two-character name of the capability to search for.
 * Return value is non-negative numeric capability value, or -1 if not found.
 *
 * Caution: the termcap entry used is the one from the last call to tgetent().
 * The entry returned at that time *must* still be sitting in static memory
 * somewhere. It is pointed to by the static pointer 'tbuf'. 
 *
 * This fixes nasty bugs in original Berkeley termlib, e.g. it could not find
 * capabilities immediately following two consecutive colons '::'. Yes,
 * that's right folks, the tab you see at the start of every continuation
 * line in the termcap file is vital! Not in this version, thank you.
 */
{
   int result;
   int base;
   int i;

   if (tbuf==NULL)
      return(-1);

   i=0;
   while (TRUE) {
      i = skip(tbuf,i);
      if (tbuf[i]==NULL || tbuf[i+1]==NULL)
         return(-1);

      if (tbuf[i] != cap[0] || tbuf[i+1]!= cap[1])
         continue;

      if (tbuf[i+2]=='@')
         return(-1);

      if (tbuf[i+2]!='#')
         continue;

      i+=3;
      result=0;
      if (tbuf[i]=='0')
         base=8;
      else
         base=10;
      while (isdigit(tbuf[i]))  {
         result*=base;
         result+=tbuf[i++]-'0';
      }

      return (result);
   }
}

int tgetflag(cap)
   char* cap;
/*
 * Search for flag capabilitites. These consist of a two-letter name
 * only, i.e. they are always followed by a ':' or the end of the buffer.
 * 'cap' is the two-character name of the capability to search for.
 * Return value is 1 if the flag is found, otherwise 0.
 *
 * Caution: the termcap entry used is the one from the last call to tgetent().
 * The entry returned at that time *must* still be sitting in static memory
 * somewhere. It is pointed to by the static pointer 'tbuf'. 
 */
{
   int i;

   if (tbuf==NULL)
      return(0);

   i=0;
   while (TRUE) {
      i = skip(tbuf,i);
      if (tbuf[i]==NULL || tbuf[i+1]==NULL)
         return(0);

      if (tbuf[i]==cap[0] && tbuf[i+1]==cap[1])  {
         if (tbuf[i+2]=='@')
            return(0);

         if (tbuf[i+2]==NULL || tbuf[i+2]==':')
            return(1);
      }
   }
}

char* tgetstr(cap, bufpp)
   char* cap;
   char** bufpp;
/*
 * Search for and decode string capabilities of the form cl=^Z.
 * The capability name is separated from the string value by a '='. 
 * 'cap' is the two-character name of the capability to search for.
 * 'bufpp' returns the decoded capabiltity string (double dereferenced
 *         pointer). The pointer itself is left pointing one past the
 *         end of the string.
 * Return value is the original value of the 'bufpp' pointer, i.e. the
 * start of the capability string, or NULL if capability not found.
 *
 * Caution: the termcap entry used is the one from the last call to tgetent().
 * The entry returned at that time *must* still be sitting in static memory
 * somewhere. It is pointed to by the static pointer 'tbuf'. 
 */
{
   int i;

   if (tbuf==NULL)
      return(NULL);

   i = 0;
   while (TRUE) {
      i = skip(tbuf,i);
      if (tbuf[i] == NULL || tbuf[i+1] == NULL)
         return(NULL);

      if (tbuf[i] != cap[0] || tbuf[i+1] != cap[1])
         continue;

      if (tbuf[i+2] == '@')
         return(NULL);

      if (tbuf[i+2] != '=')
         continue;

      i += 3;
      return(decode(tbuf+i,bufpp));
   }
}


/* Misc local utility routines */

LOCAL int checktc(bp)
   char* bp;
/*
 * Check the last capability in the termcap 'bp'; if it is tc=xxx,
 * recursively find the entry for xxx and append that entry (minus the
 * terminal names) in place of the tc=xxx capability. This allows termcap
 * entries to say "like an HP2621 but doesn't turn on the labels".
 * Note that this relies on the fact that tgetstr() et al match the earliest
 * occurrence of a capability if it appears more than once.
 * Return value is 1 if entry found, 0 if entry not found, -1 if could not
 * open termcap file.
 * **Note: this routine sets the static variable 'tbuf' appropriately.
 */
{
   int err;
   char tcname[80];        /* name of similar terminal */
   char tcbuf[BUFSIZE];
   int len;                /* misc length holder */
   int tc;                 /* index into tc string of tc capability */
   int strt;               /* start of capabilities in tcbuf string */

   tc=strlen(bp)-1;                /* last char, usually a colon */
   while (tc>0 && bp[tc-1]!=':')   /* search backwards for prev colon */
      tc--;

   if (tc<=0) {
      write(STD_ERR,"Bad termcap entry\n",18);
      tbuf=NULL;           /* no valid 'tbuf' */
      return (0);
   }

   if (strncmp(bp+tc,"tc=",3) != 0)  {
      tbuf=bp;             /* remember location of buffer for tgetstr() et al */
      return(1);           /* last capability not tc=, so we're done */
   }

   if (++tcnest > TCMAX) {
      write(STD_ERR,"Infinite tc= loop\n",18);
      tbuf=NULL;           /* no valid 'tbuf' */
      return(0);
   }

   /* get name portion of tc entry */
   strcpy(tcname,bp+tc+3);
   len=strlen(tcname)-1;
   /* remove trailing ':' if any */
   if (tcname[len]==':')
      tcname[len]=NULL;

   err=tgetent(tcbuf,tcname);
   if (err!=1) {
      tcnest=0;        /* no more nesting possible, prepare for next time  */
      tbuf=NULL;       /* no valid 'tbuf' */
      return(err);
   }

   /* find first capability, mark with 'strt' */
   strt=1;
   len=strlen(tcbuf);
   while (tcbuf[strt-1]!=':' && strt<len)
      strt++;

   if (tc+len-strt >= BUFSIZE) {
      write(STD_ERR,"Termcap entry too long\n",23);
      tcbuf[strt+BUFSIZE-tc-1] = NULL;    /* truncate */
   }

   strcpy(bp+tc, tcbuf+strt);      /* concatenate tc'd entries */

   tcnest=0;
   tbuf=bp;             /* remember location of buffer for tgetstr() et al */

   return(1);           /* done */
}

LOCAL int match(bp,name)
   char* bp;
   char* name;
/*
 * Search for 'name' in the name portion of the termcap 'bp' (the first field 
 * of the termcap entry is a sequence of names separated by |'s).
 * The normal ':' terminator after the last name (before the first field) 
 * stops us. Return value is 1 on success, 0 on failure.
 */
{
   int i;
   int ln;         /* length of 'name' */
   int lb;         /* length of name portion of 'bp' */
   char c;

   if (bp[0]== '#')     /* no match if comment */
      return(0);

   /* find first colon, mark with 'lb', the effective length of 'bp' */
   lb=0;
   while (bp[lb]!=NULL && bp[lb]!=':')
      lb++;

   ln=strlen(name);

   i=0;
   while (i+ln <= lb) {
      /* check if aligned with valid terminator at end of match */
      c=bp[i+ln];
      if (c==NULL || c=='|' || c==':')  {
         /* check if name matches */
         if (strncmp(name,bp+i,ln)==0)
            return (1);              /* match found */
      }

      i++;
   }

   /* name ran past usable length of 'bp', so failed */
   return(0);
}

LOCAL int skip(bp,i)
   char* bp;
   int i;
/*
 * Skip to the next field.  Note that this does not handle \: escapes 
 * properly, so these have to be entered in the termcap file in octal.
 * 'bp' is termcap entry.
 * 'i' is index to start skipping from.
 * Return value is index of start of new capability.
 */
{
   int firsti=i;

   while (bp[i]!=NULL && (i==firsti || bp[i-1]!=':'))
      i++;

   return (i);
}

LOCAL char* decode(str, bufpp)
   char* str;
   char** bufpp;
/*
 * Decode string capability escape sequences from 'str', copying them
 * to 'bufpp' (double dereferenced).
 * 'bufpp' returns the decoded capabiltity string (double dereferenced
 *         pointer). The pointer itself is left pointing one past the
 *         end of the string.
 * Return value is the original value of the 'bufpp' pointer, i.e. the
 * start of the capability string.
 */
{
   int j;
   int c;
   int is;
   int id;
   char* dest;
   char esc_map[] = "E\033^^\\\\::n\nr\rt\tb\bf\f";  /* length must be even! */

   dest=*bufpp;        /* where the decoded capability goes */
   id=0;
   is=0;
   while ((c=str[is++])!=NULL && c != ':') {
      switch (c) {

      case '^':
         c=str[is++] & 0x1f;
         break;

      case '\\':
         c=str[is++];

         if (isdigit(c)) {
            c=c-'0';
            if (isdigit(str[is]))  {
               c=(c<<3) | (str[is++]-'0');
               if (isdigit(str[is]))
                  c=(c<<3) | (str[is++]-'0');
            }
         }
         else  {
            for (j=0; esc_map[j]!=NULL; j+=2)  {
               if (esc_map[j]==c) {
                  c=esc_map[j+1];
                  break;
               }
            }
         }
         break;

      default:
        ;  /* do nothing */
      }

      dest[id++]=c;
   }

   dest[id++]=0;              /* null-terminate */

   *bufpp=dest+id;
   return(dest);
}
