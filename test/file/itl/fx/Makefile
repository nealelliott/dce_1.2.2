#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.912.1  1996/10/17  18:26:08  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:53:10  damon]
#
# Revision 1.1.907.3  1994/02/07  16:28:19  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:32  mckeen]
# 
# Revision 1.1.907.2  1994/02/04  20:44:06  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:23:56  devsrc]
# 
# Revision 1.1.905.2  1994/01/20  18:44:03  annie
# 	added copyright header
# 	[1994/01/20  18:40:22  annie]
# 
# Revision 1.1.905.1  1993/12/07  17:43:54  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  19:15:16  jaffe]
# 
# $EndLog$
#
#  Makefile for DFS ITL file exporter library.
#

LIBRARIES = libitlfx.a

EXPDIR = /usr/include/utils/itl/

CCTYPE = ansi

CFLAGS = ${USERCFLAGS} -U_BSD -DITL_FS_TEST

INCLUDES = itl_fxAPI.h itl_fxCommon.h itl_fxConnect.h itl_fxDCEError.h    \
	   itl_fxFileset.h itl_fxPrincipal.h itl_fxServer.h itl_fxToken.h \
           itl_fxStoreStatus.h itl_fxFetchStatus.h            \
	   itl_fxVolSync.h itl_fxFidName.h itl_fxPipe.h

libitlfx.a_OFILES = itl_fxAPI.o itl_fxConnect.o itl_fxFileset.o           \
		    itl_fxPrincipal.o itl_fxServer.o itl_fxToken.o        \
		    itl_fxStoreStatus.o itl_fxFetchStatus.o               \
		    itl_fxVolSync.o itl_fxFidName.o itl_fxPipe.o
libitlfx.a_EXPDIR = /usr/lib/

EXPLIB_TARGETS = ${LIBRARIES:S/^/export_/g}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
