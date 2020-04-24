# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.121.1  1996/10/02  21:13:07  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:50:58  damon]
#
# Revision 1.1.116.2  1994/06/09  14:24:40  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:36:47  annie]
# 
# Revision 1.1.116.1  1994/02/04  20:35:31  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:20:16  devsrc]
# 
# Revision 1.1.114.2  1994/01/20  18:43:41  annie
# 	added copyright header
# 	[1994/01/20  18:39:55  annie]
# 
# Revision 1.1.114.1  1993/12/07  17:37:43  jaffe
# 	New File from Transarc 1.0.3a
# 	[1993/12/02  21:44:33  jaffe]
# 
# $EndLog$
#

VPATH			= ../util

LIBRARIES		= libkafsutil.a

libkafsutil.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=

CFLAGS			= ${KERNELCFLAGS} -DDFS_BOMB

OFILES			= hash.o hyper.o

INCFLAGS		= -I../util

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

