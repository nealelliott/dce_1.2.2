#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.25.1  1996/10/02  17:03:08  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:04:36  damon]
#
# Revision 1.1.19.2  1994/06/09  13:51:39  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:25:10  annie]
# 
# Revision 1.1.19.1  1994/02/04  20:05:50  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:37:36  devsrc]
# 
# Revision 1.1.17.1  1993/12/07  17:12:35  jaffe
# 	New File from Transarc 1.0.3a
# 	[1993/12/02  19:01:12  jaffe]
# 
# $EndLog$
#

VPATH			= ../bomb

LIBRARIES		= libkbomb.a

libkbomb.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=

CFLAGS			= ${KERNELCFLAGS} -DDFS_BOMB

OFILES			= bomb_core.o bomb_syscall.o

INCFLAGS		= -I../bomb

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

${OFILES}: bomb.h
