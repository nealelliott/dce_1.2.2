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
# Revision 1.1.753.1  1996/10/02  17:20:02  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:29:21  damon]
#
# Revision 1.1.747.3  1994/06/09  13:59:42  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:34:58  annie]
# 
# Revision 1.1.747.2  1994/02/04  20:12:30  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:09:52  devsrc]
# 
# Revision 1.1.74.1  1993/12/07  17:18:05  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  13:51:03  jaffe]
# 
# Revision 1.1.4.2  1993/01/18  22:09:36  cjd
# 	embedded copyright notice
# 	[1993/01/18  21:50:23  cjd]
# 
# Revision 1.1.2.2  1992/05/05  20:04:44  jdp
# 	delta jdp-epimerge-add-klib-dirs 1.1
# 	[1992/05/04  20:59:50  jdp]
# 
# Revision 1.1  1992/01/19  02:40:43  devrcs
# 	Initial revision
# 
# $EndLog$
#

VPATH			= ../async

LIBRARIES		= libkasync.a

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=
CFLAGS			= ${KERNELCFLAGS}

OFILES			= asevent.o
INCFLAGS		= -I../async

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

