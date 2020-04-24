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
# Revision 1.1.922.1  1996/10/02  21:13:09  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:50:59  damon]
#
# Revision 1.1.917.2  1994/06/09  14:24:41  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:36:48  annie]
# 
# Revision 1.1.917.1  1994/02/04  20:35:33  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:20:17  devsrc]
# 
# Revision 1.1.915.1  1993/12/07  17:37:44  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  18:08:17  jaffe]
# 
# Revision 1.1.6.3  1993/01/21  16:31:03  cjd
# 	embedded copyright notice
# 	[1993/01/20  18:24:38  cjd]
# 
# Revision 1.1.6.2  1992/11/24  20:42:34  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  19:38:17  bolinger]
# 
# Revision 1.1.2.2  1992/01/31  16:27:15  delgado
# 	Add modifications for building libdcedfs.a
# 	[1992/01/31  16:26:33  delgado]
# 
# Revision 1.1  1992/01/19  02:45:42  devrcs
# 	Initial revision
# 
# $EndLog$
#

libafsutil.a_OFILES	= casestrcpy.o ktime.o \
			  hostparse.o kreltime.o hash.o hyper.o


.if !defined(LIBDCEDFS_LIBRARY_MAKEFILE)
#
# Information for building components when we are not build libdcedfs.a
#
INCLUDES		= ktime.h hash.h hyper.h
LIBRARIES		= libafsutil.a

EXPDIR			= /usr/include/dcedfs/
libafsutil.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

ILIST			= ktime.h
IDIR			= /share/include/dcedfs/

CFLAGS			= ${USERCFLAGS}

PROGRAMS		+= test_hyper
test_hyper_LIBS		= libafsutil.a

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
.endif
