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
# Revision 1.1.433.1  1996/10/02  21:01:08  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:48:04  damon]
#
# Revision 1.1.428.2  1994/06/09  14:20:24  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:31:54  annie]
# 
# Revision 1.1.428.1  1994/02/04  20:30:10  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:18:05  devsrc]
# 
# Revision 1.1.426.1  1993/12/07  17:33:47  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  16:58:37  jaffe]
# 
# Revision 1.1.3.3  1993/01/21  15:20:52  cjd
# 	embedded copyright notice
# 	[1993/01/20  16:00:09  cjd]
# 
# Revision 1.1.3.2  1992/11/24  19:42:05  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  19:25:58  bolinger]
# 
# Revision 1.1  1992/01/19  02:59:00  devrcs
# 	Initial revision
# 
# $EndLog$
#
INCLUDES		= tkc.h tkc_trace.h
OTHERS			= dfszkc.cat

LIBRARIES		= libktkc.a

EXPDIR			= /usr/lib/

tkc.h_EXPDIR		= /usr/include/dcedfs/
tkc_trace.h_EXPDIR	= /usr/include/dcedfs/

EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=
CFLAGS			= ${KERNELCFLAGS} ${DFS_CORE}

libktkc.a_OFILES	= tkc.o tkc_alloc.o tkc_cache.o tkc_hostops.o \
			  tkc_locks.o tkc_revoke.o tkc_tokens.o tkc_linkage.o \
			  tkc_thread.o

ILIST			= ${OTHERS}
dfszkc.cat_IDIR		= /nls/msg/en_US.ASCII/

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

tkc_trace.h tkc_trace.msf: tkc_trace.et
	${COMPILE_ET} ${tkc_trace.et:P} -h tkc_trace

dfszkc.cat: tkc_trace.msf
	${GENCAT} $@ $>
