# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.685.1  1996/10/02  21:10:11  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:51:38  damon]
#
# Revision 1.1.680.2  1994/06/09  14:25:37  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:37:35  annie]
# 
# Revision 1.1.680.1  1994/02/04  20:36:17  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:20:41  devsrc]
# 
# Revision 1.1.678.1  1993/12/07  17:38:23  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  18:15:25  jaffe]
# 
# Revision 1.1.5.2  1993/01/21  16:33:32  cjd
# 	embedded copyright notice
# 	[1993/01/20  18:27:26  cjd]
# 
# Revision 1.1.3.3  1992/01/24  04:14:21  devsrc
# 	Merging dfs6.3
# 	[1992/01/24  00:22:29  devsrc]
# 
# Revision 1.1.3.2  1992/01/23  20:29:26  zeliff
# 	Moving files onto the branch for dfs6.3 merge
# 	[1992/01/23  18:47:05  zeliff]
# 	Revision 1.1.1.3  1992/01/23  22:25:02  devsrc
# 	Fixed logs
# 
# $EndLog$
#

VPATH			= ../xcred

LIBRARIES		= libkxcred.a

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=
CFLAGS			= ${KERNELCFLAGS} ${DFS_CORE}
OFILES			= xcred.o xcred_linkage.o
INCFLAGS		= -I../xcred

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
