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
# Revision 1.1.978.1  1996/10/02  17:56:40  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:43:06  damon]
#
# Revision 1.1.973.4  1994/07/13  22:25:02  devsrc
# 	merged with bl-10
# 	[1994/06/28  21:02:06  devsrc]
# 
# 	Moved osi_caller.o to HPUX machine dependent Makefile.
# 	[1994/04/07  18:23:27  mbs]
# 
# 	Added osi_caller.o to libkosi.a
# 	[1994/04/04  18:18:35  mbs]
# 
# Revision 1.1.973.3  1994/06/09  14:14:30  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:27:20  annie]
# 
# Revision 1.1.973.2  1994/02/04  20:23:44  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:15:21  devsrc]
# 
# Revision 1.1.97.1  1993/12/07  17:29:02  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:56:55  jaffe]
# 
# Revision 1.1.6.2  1993/05/12  14:45:54  delgado
# 	Don't compile osi_net.c for all platforms (e.g., not on
# 	OSF1); put it in the <os>/Makefile.
# 	[1993/05/12  14:34:43  delgado]
# 
# Revision 1.1.4.3  1993/01/21  14:49:47  cjd
# 	embedded copyright notice
# 	[1993/01/20  14:51:21  cjd]
# 
# Revision 1.1.4.2  1992/11/24  17:57:21  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  19:12:58  bolinger]
# 
# Revision 1.1  1992/01/19  02:54:47  devrcs
# 	Initial revision
# 
# $EndLog$
#

VPATH			= ../osi

LIBRARIES		= libkosi.a

EXPDIR			= /usr/include/dcedfs/
libkosi.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

CFLAGS			= ${KERNELCFLAGS} ${DFS_CORE} ${osi_KERNELCFLAGS}
OFILES			= osi_misc.o lock.o osi_lock_single.o \
			  uiomove.o osi_fio.o osi_printf.o osi_ufs.o \
			  osi_pag.o osi_linkage.o afsl_trace.o osi_trace_init.o
INCFLAGS		= -I../osi

KERNEL_CC		=

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
