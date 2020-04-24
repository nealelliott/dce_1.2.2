#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.20.1  1996/10/02  17:06:26  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:05:33  damon]
#
# $EndLog$
#

PROGRAMS        	= 
LIBRARIES		= libbutm.a

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

CFLAGS			= -D_VOPS -D_BSD_INCLUDES -Daiws -D__unix ${USERCFLAGS}
libbutm.a_OFILES	= file_tm.o

LIBS            	= 

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
