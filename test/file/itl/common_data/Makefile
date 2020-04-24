#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.913.1  1996/10/17  18:25:17  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:52:44  damon]
#
# Revision 1.1.908.3  1994/02/07  16:28:16  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:29  mckeen]
# 
# Revision 1.1.908.2  1994/02/04  20:43:09  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:23:32  devsrc]
# 
# Revision 1.1.906.2  1994/01/20  18:43:55  annie
# 	added copyright header
# 	[1994/01/20  18:40:14  annie]
# 
# Revision 1.1.906.1  1993/12/07  17:43:16  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  19:07:44  jaffe]
# 
# $EndLog$
#
#  Makefile for DFS ITL common data types directory.
#

LIBRARIES = libitlcommon.a

EXPDIR = /usr/include/utils/itl/

CCTYPE = ansi

CFLAGS = ${USERCFLAGS} -U_BSD -DITL_FS_TEST

INCLUDES = itl_commonHyper.h itl_commonTimeval.h \
           itl_commonFid.h itl_commonSockaddr.h itl_commonSecurity.h

libitlcommon.a_OFILES = itl_commonHyper.o itl_commonTimeval.o \
                        itl_commonFid.o itl_commonSockaddr.o  \
			itl_commonSecurity.o

libitlcommon.a_EXPDIR = /usr/lib/

EXPLIB_TARGETS = ${LIBRARIES:S/^/export_/g}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
