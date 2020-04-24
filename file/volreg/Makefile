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
# Revision 1.1.428.1  1996/10/02  21:13:26  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:51:10  damon]
#
# Revision 1.1.423.2  1994/06/09  14:25:10  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:37:08  annie]
# 
# Revision 1.1.423.1  1994/02/04  20:35:41  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:20:23  devsrc]
# 
# Revision 1.1.421.1  1993/12/07  17:37:52  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  18:09:58  jaffe]
# 
# Revision 1.1.3.4  1993/01/21  16:32:09  cjd
# 	embedded copyright notice
# 	[1993/01/20  18:25:50  cjd]
# 
# Revision 1.1.3.3  1993/01/13  19:51:21  shl
# 	Transarc delta: jdp-ot6444-add-kernel-debugging-to-more-layers 1.1
# 	  Selected comments:
# 	    This delta allows for local kernel debugging of xaggr, xvolume, and volreg.
# 	    Add $volreg_KERNELCFLAGS to CFLAGS definition.
# 	[1993/01/12  22:32:53  shl]
# 
# Revision 1.1.3.2  1992/11/24  20:43:12  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  19:38:49  bolinger]
# 
# Revision 1.1  1992/01/19  02:57:40  devrcs
# 	Initial revision
# 
# $EndLog$
#

INCLUDES		= volreg.h

LIBRARIES		= libvolreg.a

EXPDIR			= /usr/include/dcedfs/
libvolreg.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=
CFLAGS			= ${KERNELCFLAGS} ${volreg_KERNELCFLAGS} ${DFS_CORE}
OFILES			= volreg.o volreg_linkage.o

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
