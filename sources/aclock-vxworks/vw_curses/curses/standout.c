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
static char sccsid[] = "@(#)standout.c	5.3 (Berkeley) 6/30/88";
#endif /* not lint */

/*
 * routines dealing with entering and exiting standout mode
 *
 */

# include	"curses.ext"

/*
 * enter standout mode
 */
int
wstandout(win)
reg WINDOW	*win;
{
	if (!SO && !UC)
		return FALSE;

	curses_P();

	if (win == NULL) {
	    curses_V();
	    return FALSE;
	}

	win->_flags |= _STANDOUT;

	curses_V();
	return 1;
}

/*
 * exit standout mode
 */
int
wstandend(win)
reg WINDOW	*win;
{
	if (!SO && !UC)
		return FALSE;

	curses_P();

	if (win == NULL) {
	    curses_V();
	    return FALSE;
	}

	win->_flags &= ~_STANDOUT;

	curses_V();
	return 1;
}
