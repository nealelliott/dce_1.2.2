# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.885.1  1996/10/02  17:15:31  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:06:54  damon]
#
# Revision 1.1.879.3  1994/06/09  13:56:01  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:30:34  annie]
# 
# Revision 1.1.879.2  1994/02/04  20:09:54  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:08:26  devsrc]
# 
# Revision 1.1.87.1  1993/12/07  17:15:48  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/06  13:34:50  jaffe]
# 
# $EndLog$
#

VPATH			= ../anode
LIBRARIES		= libkanode.a

EXPDIR			= /usr/include/dcedfs/episode/
libkanode.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=
.if ${TARGET_OS}=="AIX"
CFLAGS			= ${KERNELCFLAGS} ${anode_KERNELCFLAGS} -DAFS_AIX31_VM
.else
CFLAGS			= ${KERNELCFLAGS} ${anode_KERNELCFLAGS}
.endif

INCFLAGS		= -I../anode

OFILES			= anode_block.o fixed_anode.o \
			  block_alloc.o anode.o volume_table.o tlock.o \
			  volume.o file.o aggregate.o \
			  epia_volreg.o fid.o strategy.o

.if exists(${TARGET_MACHINE}/Makefile)
.include "${TARGET_MACHINE}/Makefile"
.endif
.if exists(${TARGET_OS}/Makefile)
.include "${TARGET_OS}/Makefile"
.endif

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
