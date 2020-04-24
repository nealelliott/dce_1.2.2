# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.127.1  1996/10/02  17:20:04  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:29:23  damon]
#
# Revision 1.1.121.3  1994/06/09  13:59:45  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:35:00  annie]
# 
# Revision 1.1.121.2  1994/02/04  20:12:31  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:09:53  devsrc]
# 
# Revision 1.1.12.1  1993/12/07  17:18:07  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  13:51:20  jaffe]
# 
# $EndLog$

INCLUDES		= async.h us.h asevent_debug.h

LIBRARIES		= libasync.a

EXPDIR			= /usr/include/dcedfs/episode/
libasync.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS			= dfsasy.cat
ILIST			= ${OTHERS}
dfsasy.cat_IDIR		= /nls/msg/en_US.ASCII/

CFLAGS                  = ${USERCFLAGS} -DAFS_DEBUG -DAFSDEBUG

OFILES			= asevent.o us_io.o

GARBAGE			= async.h

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.if exists (${TARGET_MACHINE}/Makefile)
.include "${TARGET_MACHINE}/Makefile"
.endif
.if exists (${TARGET_OS}/Makefile)
.include "${TARGET_OS}/Makefile"
.endif

.include <${RULES_MK}>

async.h asev_errs.msf: asev_errs.et async.p.h
	-${RM} -f ./async.p.h	
	-${CP} ${async.p.h:P} .
	${COMPILE_ET} ${asev_errs.et:P} -h async
	-${RM} -f ./async.p.h	

dfsasy.cat: asev_errs.msf
	 ${GENCAT} $@ $>

