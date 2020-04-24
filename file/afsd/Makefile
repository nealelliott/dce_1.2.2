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
# Revision 1.1.61.1  1996/10/02  16:56:53  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:03:21  damon]
#
# $EndLog$
#
# $Header: /u0/rcs_trees/dce/rcs/file/afsd/Makefile,v 1.1.61.1 1996/10/02 16:56:53 damon Exp $
#
# Copyright (c) Transarc Corporation 1996.  All rights reserved.
#
# If the default values for the CM's initial/minimum protection levels have
#   been specified in the src/file/cm Makefile environment, those values and
#   definitions should be copied to this Makefile environment as well.
#

PROGRAMS 		= dfsd vsys

INCLUDES		= dfsdsdmsg.h dfsdsdsvc.h
EXPDIR			= /usr/include/dcedfs/

CFLAGS			= ${USERCFLAGS} -D_BSD
dfsd_OFILES		= afsd.o dfsdsdsvc.o dfsdsdmsg.o dfsd_svc.o
vsys_OFILES		= vsys.o dfsdsdsvc.o dfsdsdmsg.o dfsd_svc.o
LIBS	 		= -lncompat -lafssys -losi -lafsutil -lcmd -ldce ${OS_LIBS}

TECHNOLOGY=dfs

SAMSFILES = dsd.sams
SAMS_TARGETS = dfsdsdmsg.h dfsdsdmsg.c dfsdsdmac.h dfsdsdsvc.c dfsdsdsvc.h \
    dfsdsd.cat

ILIST			= dfsd  dfsdsd.cat
IDIR			= /bin/
dfsdsd.cat_IDIR          = /nls/msg/en_US.ASCII/

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

