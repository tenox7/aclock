# Makefile - makefile for demo/vi/vw_curses/curses
#
# modification history
# --------------------
# 01e,16dec04,aeg  suppress compiler warnings since default warning level has
#		   increased.
# 01d,07jan04,jn   Support building for multiple architectures and toolchains
# 01c,07dec03,dat  replace ADDED CFLAGS with EXTRA_INCLUDE
# 01b,12nov03,job  Changed TGT_DIR to WIND_USR
# 01a,26sep03,job  written
#
# DESCRIPTION
# This file contains the makefile rules for building the curses library

LIB_NAME	= libcurses.a
LIB_DIR		= ../../lib/$(CPU)$(TOOL)

OBJS=	addbytes.o addch.o addstr.o box.o clear.o clrtobot.o clrtoeol.o \
	cr_put.o cr_tty.o curses.o delch.o deleteln.o delwin.o endwin.o \
	erase.o fullname.o getch.o getstr.o idlok.o id_subwins.o initscr.o \
	insch.o insertln.o longname.o move.o mvprintw.o mvscanw.o mvwin.o \
	newwin.o overlay.o overwrite.o printw.o putchar.o refresh.o scanw.o \
	scroll.o toucholap.o standout.o touchwin.o tstp.o unctrl.o semaphore.o

OBJ_DIR	= $(CPU)$(TOOL)
OBJS_IN_OBJ_DIR = $(foreach file, $(OBJS), $(OBJ_DIR)/$(file))

ADDED_CLEAN_LIBS += $(LIB_DIR)/$(LIB_NAME)

EXTRA_INCLUDE += -DVxWorks -I../h

$(LIB_DIR)/$(LIB_NAME):	$(OBJS_IN_OBJ_DIR)
	-$(MKDIR) $(LIB_DIR)
	@ $(AR) cruv $@ $(OBJS_IN_OBJ_DIR)

include $(WIND_USR)/make/rules.rtp

#
# Override the default warnings setting.  As the default warnings level is
# increased the sample source in this directory will generate many warnings.
#
CC_WARNINGS =
