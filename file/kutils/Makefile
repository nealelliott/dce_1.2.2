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
# Revision 1.1.64.1  1996/10/02  17:52:43  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:41:16  damon]
#
# Revision 1.1.59.2  1994/06/09  14:11:55  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:25:29  annie]
# 
# Revision 1.1.59.1  1994/02/04  20:21:24  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:14:21  devsrc]
# 
# Revision 1.1.57.1  1993/12/07  17:27:08  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:40:18  jaffe]
# 
# Revision 1.1.4.5  1993/03/12  19:05:44  bolinger
# 	For OT 7346:  check for OS-dependent Makefile, and include if present.
# 	[1993/03/12  19:04:21  bolinger]
# 
# Revision 1.1.4.4  1993/01/19  16:05:44  cjd
# 	embedded copyright notice
# 	[1993/01/19  14:15:51  cjd]
# 
# Revision 1.1.4.3  1993/01/13  18:09:06  shl
# 	Transarc delta: kazar-ot6175-bound-cm-initiated-calls 1.4
# 	  Selected comments:
# 	    limit the # of concurrent calls because the RPC deadlocks when
# 	    too many concurrent calls are active.  Also, right now, the RPC
# 	    performs *terribly* when too many concurrent calls are active, due
# 	    to apparent bugs in packet rationing.
# 	    add krpc_pool.h installation
# 	    hung quickly, due to missing call release on exit path
# 	Transarc delta: kazar-ot6523-uuid-encodes-primary-secondary-pool-selection 1.3
# 	  Selected comments:
# 	    some changes to prevent deadlock between primary/secondary thread pools
# 	    Changes basic connection protocol.
# 	    add new file for uuid hacking
# 	    fix confusion about different types of sockaddr_ins
# 	[1993/01/12  21:21:51  shl]
# 
# Revision 1.1.4.2  1992/11/24  17:53:42  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  19:08:23  bolinger]
# 
# Revision 1.1  1992/01/19  02:55:24  devrcs
# 	Initial revision
# 
# $EndLog$
#

INCLUDES		= basicincludes.h com_locks.h direntry.h ioctl.h \
			  syscall.h volerrors.h krpc_pool.h vnl_trace.h

LIBRARIES		= libdfskutil.a libdfsncs.a

EXPDIR			= /usr/include/dcedfs/
libdfskutil.a_EXPDIR	= /usr/lib/
libdfsncs.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS			= dfszvl.cat

TECHNOLOGY=dfs

libdfsncs.a_OFILES	= uuidutils.o
libdfskutil.a_OFILES	= com_lockctl.o

ILIST			= ioctl.h syscall.h ${OTHERS}
IDIR			= /share/include/dcedfs/
dfszvl.cat_IDIR          = /nls/msg/en_US.ASCII/

CFLAGS			= ${USERCFLAGS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.if exists(${TARGET_MACHINE}/Makefile)
.include "${TARGET_MACHINE}/Makefile"
.endif

.if exists(${TARGET_OS}/Makefile)
.include "${TARGET_OS}/Makefile"
.endif

.include <${RULES_MK}>

vnl_trace.h vnl_trace.msf: vnl_trace.et
	${COMPILE_ET} ${vnl_trace.et:P} -h vnl_trace

dfszvl.cat: vnl_trace.msf
	 ${GENCAT} $@ $>
