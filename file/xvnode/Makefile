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
# Revision 1.1.717.1  1996/10/02  19:03:38  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:51:59  damon]
#
# $EndLog$
#
# Copyright (C) 1996 Transarc Corporation -- All Rights Reserved
#

INCLUDES		= xvfs_vnode.h xvfs_export.h xvfs_xattr.h
LIBRARIES		= libxvnode.a 
EXPDIR			= /usr/include/dcedfs/
libxvnode.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

ILIST			= xvfs_vnode.h xvfs_xattr.h
IDIR			= /share/include/dcedfs/

KERNEL_CC		=
CFLAGS			= ${KERNELCFLAGS} ${DFS_CORE}

libxvnode.a_OFILES      = xvfs_vnode.o xvfs_volume.o xvfs_linkage.o

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
