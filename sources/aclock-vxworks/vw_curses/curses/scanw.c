/*
 * Copyright (c) 1981 Regents of the University of California.
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms are permitted
 * provided that the above copyright notice and this paragraph are
 * duplicated in all such forms and that any documentation,
 * advertising materials, and other materials related to such
 * distribution and use acknowledge that the software was developed
 * by the University of California, Berkeley.  The name of the
 * University may not be used to endorse or promote products derived
 * from this software without specific prior written permission.
 * THIS SOFTWARE IS PROVIDED ``AS IS'' AND WITHOUT ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, WITHOUT LIMITATION, THE IMPLIED
 * WARRANTIES OF MERCHANTIBILITY AND FITNESS FOR A PARTICULAR PURPOSE.
 */

#ifndef lint
static char sccsid[] = "@(#)scanw.c	5.3 (Berkeley) 6/30/88";
#endif /* not lint */

/*
 * scanw and friends
 *
 */
# include       <vxWorks.h>
# include       <stdio.h>
# include       <stdarg.h>
# include	"curses.ext"

/*
 *	This routine implements a scanf on the standard screen.
 */
scanw(const char *fmt, ...)
{

        void *args[10];
        void *tmp;
        int count = 0;
        va_list vaList;

        va_start(vaList, fmt);
        while ((tmp = va_arg(vaList, void *)) && (count < 10))
            {
                args[count] = tmp;
                count++;
            }
        va_end(vaList);
	return _sscans(stdscr, fmt, args);
}
/*
 *	This routine implements a scanf on the given window.
 */
wscanw(WINDOW *win, const char *fmt, ...)
{
        void *args[10];
        void *tmp;
        int count = 0;
        va_list vaList;

        va_start(vaList, fmt);
        while ((tmp = va_arg(vaList, void *)) && (count < 10))
            {
                args[count] = tmp;
                count++;
            }
        va_end(vaList);
	return _sscans(win, fmt, args);
}
/*
 *	This routine actually executes the scanf from the window.
 *
 *	This is really a modified version of "sscanf".  As such,
 * it assumes that sscanf interfaces with the other scanf functions
 * in a certain way.  If this is not how your system works, you
 * will have to modify this routine to use the interface that your
 * "sscanf" uses.
 *	Note: do not add semaphore locking here! wgetch() must be able
 *            to release semaphore for blocking read.
 */
_sscans(WINDOW *win, char *fmt, void **args)
{
	unsigned char	buf[100];

#ifdef VxWorks
	if (wgetstr(win, (char *) buf) == ERR)
		return ERR;

        sscanf((const char *)buf,fmt,args[0],args[1],args[2],args[3],
	       args[4],args[5],args[6],args[7],args[8],args[9]);
#else
        FILE	junk;
                
	junk._flag = _IOREAD|_IOSTRG;
	junk._base = junk._ptr = buf;
	if (wgetstr(win, buf) == ERR)
		return ERR;
	junk._cnt = strlen(buf);
	return _doscan(&junk, fmt, args);
#endif
}
