# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:49:58  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:10:35  marty]
#
# Revision 1.1.4.1  1995/12/11  22:53:26  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:33:47  root]
# 
# Revision 1.1.2.2  1993/07/20  20:41:10  cmckeen
# 	Initial DCE version
# 	[1993/07/15  16:20:46  cmckeen]
# 
# $EndLog$

LIBRARIES	= libdtettcm.a libdtettcmrem.a libdtettcmchild.a
#LIBRARIES	= libmtcm.a libstcm.a libtcmrem.a libdtcmchild.a
EXPDIR          = /usr/lib/
INCFLAGS        = -I../inc

# generic dtet tcm .o files - this list is also in dtet.mk
OFILES_GN = tcmfuncs.o fake.o

# INET-specific dtet tcm .o files - this list is also in dtet.mk
OFILES_IN = tcm_in.o tcm_bs.o

OOFILES = $(OFILES_GN) $(OFILES_IN)

libdtettcm.a_OFILES 	= mtcmfuncs.o dtcm.o mtcmdist.o ictp.o $(OOFILES)
#libmtcm.a_OFILES 	= mtcmfuncs.o dtcm.o mtcmdist.o ictp.o $(OOFILES)
#libstcm.a_OFILES 	= stcmfuncs.o dtcm.o stcmdist.o ictp.o $(OOFILES)
libdtettcmchild.a_OFILES= child.o ctcmfuncs.o $(OOFILES)
libdtettcmrem.a_OFILES 	= child.o rtcmfuncs.o $(OOFILES)

TET_CFLAGS		= -D_POSIX_SOURCE -DNSIG=32
dtcm.o_CFLAGS		= ${TET_CFLAGS}
mtcmdist.o_CFLAGS	= ${TET_CFLAGS}
stcmdist.o_CFLAGS	= ${TET_CFLAGS}
ictp.o_CFLAGS		= ${TET_CFLAGS}

.include <${RULES_MK}>

RM = echo rm 
