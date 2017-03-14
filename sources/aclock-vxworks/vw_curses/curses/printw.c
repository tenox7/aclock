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
static char sccsid[] = "@(#)printw.c	5.4 (Berkeley) 6/30/88";
#endif /* not lint */

/*
 * printw and friends
 *
 */

# include	"curses.ext"

/*
 *	This routine implements a printf on the standard screen.
 *	Locking not needed at this level.
 */
printw(const char *fmt, ...)
{
	char	buf[512];
	va_list vaList;

	va_start(vaList, fmt);
	(void) vsprintf(buf, fmt, vaList);
	va_end(vaList);
	return waddstr(stdscr, buf);
}

/*
 *	This routine implements a printf on the given window.
 *	Locking not needed at this level.
 */
wprintw(WINDOW *win, const char *fmt, ...)
{
	char	buf[512];
	va_list vaList;

	va_start(vaList, fmt);
        if (win == NULL)
           return ERR;

	(void) vsprintf(buf, fmt, vaList);
	va_end(vaList);
	return waddstr(win, buf);
}
