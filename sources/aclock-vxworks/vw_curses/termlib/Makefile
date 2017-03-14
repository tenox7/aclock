# Makefile - makefile for demo/vi/vw_curses/termlib
#
# modification history
# --------------------
# 01e,16dec04,aeg  suppress compiler warnings since default warning level has
#                  increased.
# 01d,07jan04,jn   Support building for multiple architectures and toolchains
# 01c,07dec03,dat  replace ADDED CFLAGS with EXTRA_INCLUDE
# 01b,12nov03,job  Changed TGT_DIR to WIND_USR
# 01a,26sep03,job  written
#
# DESCRIPTION
# This file contains the makefile rules for building the termlib library

LIB_NAME	= libtermlib.a
LIB_DIR		= ../../lib/$(CPU)$(TOOL)

OBJS=	termcap.o tgoto.o tputs.o

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

