# Makefile - makefile fr demo/vi/vw_curses/test
#
# modification history
# --------------------
# 01i,16dec04,aeg  suppress compiler warnings since default warning level has
#                  increased.
# 01h,19sep04,mcm  No need to link in librtp.a - it comes from libc_internal.a
# 01g,07jan04,jn   Support building for multiple architectures and toolchains
# 01f,08dec03,jn   Change LIB_RTP to ADDED_LIBS
# 01e,07dec03,dat  replace ADDED CFLAGS with EXTRA_INCLUDE
# 01d,03dec03,mcm  Added -lrtp
# 01c,12nov03,job  Changed TGT_DIR to WIND_USR and extension to .vxe
# 01b,26sep03,job  Updated to build a test RTP
# 01a,22aug03,md   created
#
# DESCRIPTION
# This file contains the makefile rules for building the curses test programs
#
#

EXE             = twinkle.vxe

LIB_DIR		= ../../lib/$(CPU)$(TOOL)
LIBS		= -lcurses -ltermlib
ADDED_LIBS      = -L$(LIB_DIR) $(LIBS)

EXTRA_INCLUDE   += -DVxWorks -I../h

OBJS		= twinkle.o random.o

include $(WIND_USR)/make/rules.rtp

#
# Override the default warnings setting.  As the default warnings level is
# increased the sample source in this directory will generate many warnings.
#
CC_WARNINGS =

