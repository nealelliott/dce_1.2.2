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
# Revision 1.1.538.1  1996/10/02  18:49:40  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:49:41  damon]
#
# Revision /main/HPDCE02/brezak_transarc/1  1995/06/01  22:07 UTC  brezak
# 	create msgcat for ufs_trace
# 
# Revision /main/HPDCE02/1  1994/06/29  17:33 UTC  kinney
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	  Revision /main/HPDCE01/1  1994/05/19  17:09 UTC  rmm
# 	merge +e mod
# 
# 	  Revision /main/rmm_rodan_work/1  1994/05/19  17:07 UTC  rmm
# 	add +e option for osi_fio.c to work around dirent.h changes
# 
# Revision 1.1.532.3  1994/06/09  14:23:26  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:35:14  annie]
# 
# Revision 1.1.532.2  1994/02/04  20:33:48  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:19:31  devsrc]
# 
# Revision 1.1.531.1  1993/12/07  17:36:25  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  17:40:36  jaffe]
# 
# Revision 1.1.2.4  1993/02/24  21:56:54  bolinger
# 	OT defect 3542 (OSF/1 volops support):  Look in TARGET_OS
# 	directory for a Makefile.
# 	[1993/02/24  21:39:21  bolinger]
# 
# Revision 1.1.2.3  1993/01/28  18:14:27  shl
# 	Transarc delta: cburnett-ot6781-aix-jfs-dump-restore-support 1.3
# 	  Selected comments:
# 	    Add AIX JFS support to ufsops for the new portable directory
# 	    dump/restore format.  Also fix some existing bugs in the
# 	    ufsops code.  In addition push AIX specific function into
# 	    a platform specific dir.
# 	    Look for platform specific Makefile.
# 	    The first revision of delta didn't compile under OSF/1.
# 	    We need one more round to get this to compile under OSF/1.
# 	[1993/01/27  21:04:14  shl]
# 
# Revision 1.1.2.2  1993/01/21  15:56:53  cjd
# 	embedded copyright notice
# 	[1993/01/20  17:41:08  cjd]
# 
# Revision 1.1  1992/01/19  02:57:34  devrcs
# 	Initial revision
# 
# $EndLog$
#

LIBRARIES		= libufsops.a

EXPDIR			= /usr/include/dcedfs/
libufsops.a_EXPDIR		= /usr/lib/
EXPLIB_TARGETS		= ${LIBRARIES:S/^/export_/g}

INCLUDES		= ufs.h ufs_trace.h

KERNEL_CC		=
CFLAGS			= ${KERNELCFLAGS}
OFILES			= ufs_agops.o ufs_volops.o

OTHERS			= dfszuf.cat
dfszuf.cat_IDIR		= /nls/msg/en_US.ASCII/

ILIST			= ${OTHERS}
IDIR			= /nls/msg/en_US.ASCII/

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

ufs_trace.h ufs_trace.msf: ufs_trace.et
	${COMPILE_ET} ${ufs_trace.et:P} -h ufs_trace

dfszuf.cat: ufs_trace.msf
	${GENCAT} $@ $>
