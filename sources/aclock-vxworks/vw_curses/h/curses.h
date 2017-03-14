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
 *
 *	@(#)curses.h	5.4 (Berkeley) 6/30/88
 */

# ifndef WINDOW

# include	<stdio.h>
 
#ifndef VxWorks
# include	<sgtty.h>
#else
# include       <vxWorks.h>
# include       <ioLib.h>
#endif

# define	bool	char
# define	reg	register

#ifdef TRUE
#  undef TRUE
#  undef FALSE
#endif
# define	TRUE	(1)
# define	FALSE	(0)
# define	ERR	(-1)
# define	CURSES_ERR	ERR
# define	CURSES_OK	OK

# define	_ENDLINE	001
# define	_FULLWIN	002
# define	_SCROLLWIN	004
# define	_FLUSH		010
# define	_FULLLINE	020
# define	_IDLINE		040
# define	_STANDOUT	0200
# define	_NOCHANGE	-1

# define	_puts(s)	tputs(s, 0, _putchar)

#ifndef VxWorks
typedef	struct sgttyb	SGTTY;
#endif

/*
 * Capabilities from termcap
 */

extern bool     AM, BS, CA, DA, DB, EO, HC, HZ, IN, MI, MS, NC, NS, OS, UL,
		XB, XN, XT, XS, XX;
extern char	*AL, *BC, *BT, *CD, *CE, *CL, *CM, *CR, *CS, *DC, *DL,
		*DM, *DO, *ED, *EI, *K0, *K1, *K2, *K3, *K4, *K5, *K6,
		*K7, *K8, *K9, *HO, *IC, *IM, *IP, *KD, *KE, *KH, *KL,
		*KR, *KS, *KU, *LL, *MA, *ND, *NL, *RC, *SC, *SE, *SF,
		*SO, *SR, *TA, *TE, *TI, *UC, *UE, *UP, *US, *VB, *VS,
		*VE, *AL_PARM, *DL_PARM, *UP_PARM, *DOWN_PARM,
		*LEFT_PARM, *RIGHT_PARM;
extern char	PC;

/*
 * From the tty modes...
 */

extern bool	GT, NONL, UPPERCASE, normtty, _pfast;

struct _win_st {
	short		_cury, _curx;
	short		_maxy, _maxx;
	short		_begy, _begx;
	short		_flags;
	short		_ch_off;
	bool		_clear;
	bool		_leave;
	bool		_scroll;
	char		**_y;
	short		*_firstch;
	short		*_lastch;
	struct _win_st	*_nextp, *_orig;
};

# define	WINDOW	struct _win_st

extern bool	My_term, _echoit, _rawmode, _endwin;

extern char	*Def_term, ttytype[];

extern int	LINES, COLS, _tty_ch, _res_flg;

#ifndef VxWorks
extern SGTTY	_tty;
#endif

extern WINDOW	*stdscr, *curscr;

/*
 *	Define VOID to stop lint from generating "null effect"
 * comments.
 */
# ifdef lint
int	__void__;
# define	VOID(x)	(__void__ = (int) (x))
# else
# define	VOID(x)	(x)
# endif

/*
 * standard curses functions.
 *
 */

extern WINDOW * newwin(int lines, int cols, int sy, int sx);
extern WINDOW * renewwin(int lines, int cols, int sy, int sx, WINDOW* win);
extern WINDOW * subwin(WINDOW *w, int lines, int cols, int sy, int sx);
extern WINDOW * initscr();
extern int      box (WINDOW*, char, char);
extern int      delwin(WINDOW*);
extern int      redelwin(WINDOW*);
extern int      mvcur(int, int, int, int);
extern int      overlay(WINDOW*, WINDOW*);
extern int      overwrite(WINDOW*, WINDOW*);
extern int      scroll(WINDOW*);
extern int      touchline(WINDOW*, int, int, int);
extern int      hideline(WINDOW*, int, int, int);
extern int      touchoverlap(WINDOW*, WINDOW*);
extern int      hideoverlap(WINDOW*, WINDOW*);
extern int      touchwin(WINDOW*);
extern int      hidewin(WINDOW*);
extern int      waddbytes(WINDOW* win, const char* bytes, int count);
extern int      waddch(WINDOW*, char);
extern int      waddstr(WINDOW*, const char*);
extern int      wclear(WINDOW*);
extern int      wclrtobot(WINDOW*);
extern int      wclrtoeol(WINDOW*);
extern int      wdelch(WINDOW*);
extern int      wdeleteln(WINDOW*);
extern int      werase(WINDOW*);
extern int      wgetch(WINDOW*);
extern int      wgetstr(WINDOW*, char*);
extern int      winch(WINDOW*);
extern int      winsch(WINDOW*, char);
extern int      winsertln(WINDOW*);
extern int      wmove(WINDOW*, int, int);
extern int      wrefresh(WINDOW*);
extern int      wstandend(WINDOW*);
extern int      wstandout(WINDOW*);
extern int      printw(const char * fmt, ...);
extern int      wprintw(WINDOW*, const char * fmt, ...);
extern int      mvprintw(int y, int x, const char * fmt, ...);
extern int      mvwprintw(WINDOW*, int y, int x, const char * fmt, ...);
extern int      wscanw(WINDOW*, const char *, ...);
extern int      mvwscanw(WINDOW*, int, int, const char*, ...);
extern int      endwin();

extern char *longname(char *, char *);
extern char *getcap(char *);

/*
 * pseudo functions for standard screen
 */
# define	addch(ch)	VOID(waddch(stdscr, ch))
# define	getch()		VOID(wgetch(stdscr))
# define	addbytes(da,co)	VOID(waddbytes(stdscr, da,co))
# define	addstr(str)	VOID(waddbytes(stdscr, str, strlen(str)))
# define	getstr(str)	VOID(wgetstr(stdscr, str))
# define	move(y, x)	VOID(wmove(stdscr, y, x))
# define	clear()		VOID(wclear(stdscr))
# define	erase()		VOID(werase(stdscr))
# define	clrtobot()	VOID(wclrtobot(stdscr))
# define	clrtoeol()	VOID(wclrtoeol(stdscr))
# define	insertln()	VOID(winsertln(stdscr))
# define	deleteln()	VOID(wdeleteln(stdscr))
# define	refresh()	VOID(wrefresh(stdscr))
# define	inch()		VOID(winch(stdscr))
# define	insch(c)	VOID(winsch(stdscr,c))
# define	delch()		VOID(wdelch(stdscr))
# define	standout()	VOID(wstandout(stdscr))
# define	standend()	VOID(wstandend(stdscr))

/*
 * mv functions
 * We should really lock between the wmove and the action. (***)
 */
#define	mvwaddch(win,y,x,ch)	VOID(wmove(win,y,x)==ERR?ERR:waddch(win,ch))
#define	mvwgetch(win,y,x)	VOID(wmove(win,y,x)==ERR?ERR:wgetch(win))
#define	mvwaddbytes(win,y,x,da,co) \
		VOID(wmove(win,y,x)==ERR?ERR:waddbytes(win,da,co))
#define	mvwaddstr(win,y,x,str) \
		VOID(wmove(win,y,x)==ERR?ERR:waddbytes(win,str,strlen(str)))
#define mvwgetstr(win,y,x,str)  VOID(wmove(win,y,x)==ERR?ERR:wgetstr(win,str))
#define	mvwinch(win,y,x)	VOID(wmove(win,y,x) == ERR ? ERR : winch(win))
#define	mvwdelch(win,y,x)	VOID(wmove(win,y,x) == ERR ? ERR : wdelch(win))
#define	mvwinsch(win,y,x,c)	VOID(wmove(win,y,x) == ERR ? ERR:winsch(win,c))
#define	mvaddch(y,x,ch)		mvwaddch(stdscr,y,x,ch)
#define	mvgetch(y,x)		mvwgetch(stdscr,y,x)
#define	mvaddbytes(y,x,da,co)	mvwaddbytes(stdscr,y,x,da,co)
#define	mvaddstr(y,x,str)	mvwaddstr(stdscr,y,x,str)
#define mvgetstr(y,x,str)       mvwgetstr(stdscr,y,x,str)
#define	mvinch(y,x)		mvwinch(stdscr,y,x)
#define	mvdelch(y,x)		mvwdelch(stdscr,y,x)
#define	mvinsch(y,x,c)		mvwinsch(stdscr,y,x,c)

/*
 * psuedo functions
 */

#define	clearok(win,bf)	 (win==NULL ? ERR : (win->_clear = bf))
#define	leaveok(win,bf)	 (win==NULL ? ERR : (win->_leave = bf))
#define	scrollok(win,bf) (win==NULL ? ERR : (win->_scroll = bf))
#define flushok(win,bf)	 (win==NULL ? ERR : (bf ? (win->_flags |= _FLUSH):(win->_flags &= ~_FLUSH)))
#define	getyx(win,y,x)	 (win==NULL ? ERR : (y = win->_cury, x = win->_curx))
#define	winch(win)	 (win==NULL ? ERR : (win->_y[win->_cury][win->_curx] & 0177))

#ifndef VxWorks
#define raw()	 (_tty.sg_flags|=RAW, _pfast=_rawmode=TRUE, stty(_tty_ch,&_tty))
#define noraw()	 (_tty.sg_flags&=~RAW,_rawmode=FALSE,_pfast=!(_tty.sg_flags&CRMOD),stty(_tty_ch,&_tty))
#define cbreak() (_tty.sg_flags |= CBREAK, _rawmode = TRUE, stty(_tty_ch,&_tty))
#define nocbreak() (_tty.sg_flags &= ~CBREAK,_rawmode=FALSE,stty(_tty_ch,&_tty))
#define crmode() cbreak()	/* backwards compatability */
#define nocrmode() nocbreak()	/* backwards compatability */
#define echo()	 (_tty.sg_flags |= ECHO, _echoit = TRUE, stty(_tty_ch, &_tty))
#define noecho() (_tty.sg_flags &= ~ECHO, _echoit = FALSE, stty(_tty_ch, &_tty))
#define nl()	 (_tty.sg_flags |= CRMOD,_pfast = _rawmode,stty(_tty_ch, &_tty))
#define nonl()	 (_tty.sg_flags &= ~CRMOD, _pfast = TRUE, stty(_tty_ch, &_tty))
#define	savetty() ((void) gtty(_tty_ch, &_tty), _res_flg = _tty.sg_flags)
#define	resetty() (_tty.sg_flags = _res_flg, (void) stty(_tty_ch, &_tty))

#define	erasechar()	(_tty.sg_erase)
#define	killchar()	(_tty.sg_kill)
#define baudrate()	(_tty.sg_ospeed)

#else

#define raw()	 (cbreak())
#define noraw()	 (nocbreak())
#define cbreak() (_rawmode = TRUE,\
                  ioctl(0,FIOSETOPTIONS,ioctl(0,FIOGETOPTIONS,0) & ~OPT_LINE) )
#define nocbreak() (_rawmode = FALSE,\
                    ioctl(0,FIOSETOPTIONS,ioctl(0,FIOGETOPTIONS,0) | OPT_LINE) )
#define crmode() cbreak()	/* backwards compatability */
#define nocrmode() nocbreak()	/* backwards compatability */
#define echo() (_echoit = TRUE,\
                ioctl(0,FIOSETOPTIONS,ioctl(0,FIOGETOPTIONS,0) | OPT_ECHO) )
#define noecho() (_echoit = FALSE,\
                  ioctl(0,FIOSETOPTIONS,ioctl(0,FIOGETOPTIONS,0) & ~OPT_ECHO) )
#define nl()	 (OK)
#define nonl()	  ( \
                     fprintf(stderr,"curses.h: nonl() not supported\n"); \
                     exit(1); \
                  )
#define	savetty() (_res_flg = ioctl(0,FIOGETOPTIONS,0))
#define	resetty() (ioctl(0,FIOSETOPTIONS,_res_flg))

#define	erasechar()  ('')
#define killchar()   ( \
                      fprintf(stderr,"curses.h: killchar() not supported\n"); \
                      exit(1); \
                     )
#define baudrate()   (13)      /* this is the termio/gtty code for 9600 baud,
                                    used by tputs() in the termlib library */
#endif

/*
 * Used to be in unctrl.h.
 */
#define	unctrl(c)	_unctrl[(c) & 0177]
extern char *_unctrl[];
# endif
