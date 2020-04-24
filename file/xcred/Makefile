# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.686.1  1996/10/02  21:10:14  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:51:39  damon]
#
# Revision 1.1.681.3  1994/06/09  14:25:38  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:37:36  annie]
# 
# Revision 1.1.681.2  1994/05/20  22:03:27  sommerfeld
# 	Avoid continuous rebuild of xcred.h
# 	[1994/05/17  20:28:08  sommerfeld]
# 
# Revision 1.1.681.1  1994/02/04  20:36:18  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:20:41  devsrc]
# 
# Revision 1.1.679.1  1993/12/07  17:38:25  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  18:15:59  jaffe]
# 
# Revision 1.1.4.3  1993/09/28  19:36:32  sommerfeld
# 	Clean up bogus history comments.
# 	[1993/09/28  19:36:12  sommerfeld]
# 
# 	embedded copyright notice
# 	[1993/01/20  18:27:30  cjd]
# 
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  19:40:13  bolinger]
# 
# 	minor changes
# 	[1992/08/30  13:56:28  jaffe]
# 
# Revision 1.1.4.2  1993/09/28  19:31:58  sommerfeld
# 	avoid continuous rebuild of xcred.h.
# 	[1993/09/17  17:13:57  sommerfe]
# 
# $EndLog$

INCLUDES		= xcred.h xcred_errs.h xcred_kint.h

LIBRARIES		= libxcred.a
PROGRAMS		= xcred_test xcred_ktest

xcred.h_EXPDIR		= /usr/include/dcedfs/
xcred_errs.h_EXPDIR	= /usr/include/dcedfs/
xcred_kint.h_EXPDIR	= /usr/include/dcedfs/
libxcred.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS			= dfsxcr.cat
ILIST			= ${OTHERS}
dfsxcr.cat_IDIR		= /nls/msg/en_US.ASCII/

CFLAGS			= ${USERCFLAGS}
libxcred.a_OFILES	= xcred.o
xcred_test_OFILES	= xcred_test.o
xcred_ktest_OFILES	= xcred_ktest.o

xcred_test_LIBS		= libxcred.a -ldce ${OS_LIBS}
xcred_ktest_LIBS	= libxcred.a -lafssys -ldce ${OS_LIBS}

GARBAGE			= xcred_errs.h xcred.h

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

xcred_errs.h xcred_errs.msf: xcred_errs.et
	${COMPILE_ET} ${xcred_errs.et:P} -h xcred_errs

xcred.h: xcred_errs.et xcred.p.h
	-${CP} ${xcred.p.h:P} .
	-${CP} ${xcred_errs.et:P} xcred.et
	${COMPILE_ET} xcred.et -h xcred
	${RM} xcred.p.h
	${RM} xcred.et

dfsxcr.cat: xcred_errs.msf
	 ${GENCAT} $@ $>
