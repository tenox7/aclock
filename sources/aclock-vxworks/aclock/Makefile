# Makefile - makefile for vi RTP
#
# modification history
# --------------------
# 01k,16dec04,aeg  suppress compiler warnings since default warning level has
#                  increased.
# 01j,25oct04,pad  Renamed WIND_USR_LIBS into ADDED_LIB_DEPS.
# 01i,01oct04,job  Removed temp.o
# 01h,04jun04,job  WIND_USR_LIBS now points to locally built libraries.
# 01g,28apr04,job  Fixed location for .vxe
# 01f,07jan04,jn   Support building for multiple architectures and toolchains
# 01e,08dec03,jn   Change LIB_RTP to ADDED_LIBS
# 01d,07dec03,dat  replace ADDED CFLAGS with EXTRA_INCLUDE
# 01c,12nov03,job  Changed TGT_DIR to WIND_USR and extension to .vxe
# 01b,04nov03,job  temporarily added temp.o as a buildable object
# 01a,21sep03,job  written
#
# DESCRIPTION
# This file contains the makefile rules for building vi as an RTP

EXE = aclock-$(CPU)-vxworks6.vxe
EXE_DIR = ../$(CPU)$(TOOL)

OBJS=	aclock.o

LIB_DIR		= ../lib/$(CPU)$(TOOL)
LIBS		= -lcurses -ltermlib
ADDED_LIBS      += -L$(LIB_DIR) $(LIBS)

# Rebuild vi.exe if the libraries changed.

ADDED_LIB_DEPS  += $(LIB_DIR)/libcurses.a $(LIB_DIR)/libtermlib.a

EXTRA_INCLUDE += -DVxWorks -DINCvxWorksh -UBSD -I. -I../vw_curses/h

include $(WIND_USR)/make/rules.rtp

#
# Override the default warnings setting.  As the default warnings level is
# increased the sample source in this directory will generate many warnings.
#
CC_WARNINGS =
