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
static char sccsid[] = "@(#)mvscanw.c	5.3 (Berkeley) 6/30/88";
#endif /* not lint */

# include	"curses.ext"

/*
 * implement the mvscanw commands.  Due to the variable number of
 * arguments, they cannot be macros.  Another sigh....
 *
 */

mvscanw(int y, int x, const char *fmt, ...)
{
	int retval;
	va_list vaList;

	va_start(vaList, fmt);
	curses_P();
	if (move(y, x) != OK) {
		curses_V();
		return ERR;
        }
	retval= _sscans(stdscr, fmt, vaList);
	curses_V();
	va_end(vaList);
	return retval;
}

mvwscanw(WINDOW *win, int y, int x, const char *fmt, ...)
{
	int retval;
	va_list vaList;

	va_start(vaList, fmt);
	curses_P();

	if (win == NULL) {
	    curses_V();
	    return ERR;
	}

	if (wmove(win, y, x) != OK) {
		curses_V();
		return ERR;
        }
	retval= _sscans(win, fmt, vaList);

	curses_V();
	va_end(vaList);
	return retval;
}
