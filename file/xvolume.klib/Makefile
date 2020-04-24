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
# Revision 1.1.341.1  1996/10/02  19:04:21  damon
# 	New DFS from Transarc
# 	[1996/10/01  18:59:50  damon]
#
# Revision 1.1.336.2  1994/06/09  14:26:37  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:38:26  annie]
# 
# Revision 1.1.336.1  1994/02/04  20:37:30  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:21:14  devsrc]
# 
# Revision 1.1.334.1  1993/12/07  17:39:18  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  18:23:16  jaffe]
# 
# Revision 1.1.2.3  1993/01/21  16:36:06  cjd
# 	embedded copyright notice
# 	[1993/01/20  18:29:57  cjd]
# 
# Revision 1.1.2.2  1993/01/13  20:10:50  shl
# 	Transarc delta: jdp-ot6444-add-kernel-debugging-to-more-layers 1.1
# 	  Selected comments:
# 	    This delta allows for local kernel debugging of xaggr, xvolume, and volreg.
# 	    Add $xvolume_KERNELCFLAGS to CFLAGS definition.
# 	[1993/01/12  22:39:20  shl]
# 
# Revision 1.1  1992/01/19  02:58:58  devrcs
# 	Initial revision
# 
# $EndLog$
#
VPATH	= ../xvolume

LIBRARIES		= libvolume.a

EXPDIR			= /usr/lib/

EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=

CFLAGS			= ${KERNELCFLAGS} ${xvolume_KERNELCFLAGS} ${DFS_CORE}

OFILES			= vol_init.o vol_desc.o vol_misc.o vol_attvfs.o \
			  vol_linkage.o
INCFLAGS		= -I../xvolume

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
