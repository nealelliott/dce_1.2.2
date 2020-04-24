# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.632.1  1996/10/02  21:11:06  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:51:18  damon]
#
# Revision 1.1.627.2  1994/06/09  14:25:15  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:37:12  annie]
# 
# Revision 1.1.627.1  1994/02/04  20:35:49  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:20:28  devsrc]
# 
# Revision 1.1.625.1  1993/12/07  17:37:57  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  18:10:51  jaffe]
# 
# Revision 1.1.5.3  1993/01/21  16:32:19  cjd
# 	embedded copyright notice
# 	[1993/01/20  18:26:07  cjd]
# 
# Revision 1.1.5.2  1993/01/13  19:51:43  shl
# 	Transarc delta: jdp-ot6444-add-kernel-debugging-to-more-layers 1.1
# 	  Selected comments:
# 	    This delta allows for local kernel debugging of xaggr, xvolume, and volreg.
# 	    Add $xaggr_KERNELCFLAGS to CFLAGS definition.
# 	[1993/01/12  22:34:14  shl]
# 
# Revision 1.1.3.4  1992/05/22  20:49:05  garyf
# 	fix RIOS build
# 	[1992/05/22  03:14:25  garyf]
# 
# Revision 1.1.3.3  1992/01/24  04:14:13  devsrc
# 	Merging dfs6.3
# 	[1992/01/24  00:22:16  devsrc]
# 
# Revision 1.1.3.2  1992/01/23  20:28:59  zeliff
# 	Moving files onto the branch for dfs6.3 merge
# 	[1992/01/23  18:46:46  zeliff]
# 	Revision 1.1.1.3  1992/01/23  22:24:50  devsrc
# 	Fixed logs
# 
# $EndLog$
#

VPATH			= ../xaggr

LIBRARIES		= libaggr.a

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=

CFLAGS			= ${KERNELCFLAGS} ${xaggr_KERNELCFLAGS} ${DFS_CORE}
INCFLAGS		= -I../xaggr

OFILES			= ag_init.o ag_registry.o xaggr_linkage.o

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
