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
# Revision 1.1.471.1  1996/10/02  16:57:07  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:03:29  damon]
#
# Revision 1.1.465.2  1994/06/09  13:49:48  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:23:17  annie]
# 
# Revision 1.1.465.1  1993/12/07  17:54:55  mario
# 	Changed for ode2.3 - < and > becomes this except for RULES_MK
# 	[1993/12/07  17:25:37  mario]
# 
# Revision 1.1.2.4  1993/01/18  19:49:25  cjd
# 	Embedded copyright notice
# 	[1993/01/18  19:22:40  cjd]
# 
# Revision 1.1.2.3  1992/11/24  15:36:27  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  16:03:51  bolinger]
# 
# Revision 1.1.2.2  1992/11/18  17:36:42  jaffe
# 	Transarc delta: jdp-ot5415-mount-aggregate-on-attach 1.4
# 	  Selected comments:
# 	    This delta contains the bulk of the changes necessary to have aggregates
# 	    mounted at attach time.  The purpose of this feature is to provide a
# 	    means by which exported filesets (with no local mount points) will be
# 	    sync'd periodically.  To accomplish this, we have created a new VFS type
# 	    (agfs).  Whenever an aggregate is exported, a local mount of the aggregate
# 	    is also done; then, when the sync daemon does its thing, the aggregate's sync
# 	    vfsop will be called.  The agfs sync then calls the new sync aggrop, which
# 	    in the case of Episode, calls efs_sync().  The UFS sync aggrop doen't do
# 	    anything, since we know the UFS filesystem is mounted, and so, its sync vfsop
# 	    will get called anyway.
# 	    This delta must be bundled with the corresponding deltas in the Episode and
# 	    test configurations.
# 	    This is the makefile for the agfs directory.
# 	    Remove leftover junk from $OFILES.
# 	    Rearrange things slightly to make use of a vendor-supplied definition
# 	    of MOUNT_AGFS.
# 	    Fix bogus linker warnings about shared libraries when compiling .o files.
# 	[1992/11/17  19:27:48  jaffe]
# 
# $EndLog$
#
VPATH			= ${TARGET_MACHINE}:${TARGET_OS}

CFLAGS			= ${KERNELCFLAGS}
KERNEL_CC		=

INCLUDES		= agfs_mount.h
EXPDIR			= /usr/include/dcedfs/
LIBRARIES		= libagfs.a
libagfs.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

libagfs.a_OFILES	= agfs_vfsops.o agfs_vnops.o

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


