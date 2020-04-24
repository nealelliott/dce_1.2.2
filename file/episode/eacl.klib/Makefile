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
# Revision 1.1.777.1  1996/10/02  17:23:58  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:31:47  damon]
#
# Revision 1.1.772.3  1994/06/09  14:00:15  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:35:33  annie]
# 
# Revision 1.1.772.2  1994/02/04  20:13:06  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:10:13  devsrc]
# 
# Revision 1.1.77.1  1993/12/07  17:18:40  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  13:56:13  jaffe]
# 
# Revision 1.1.4.2  1993/01/19  15:26:18  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:46:19  cjd]
# 
# Revision 1.1.2.2  1992/05/05  19:05:40  jdp
# 	delta jdp-epimerge-add-klib-dirs 1.1
# 	[1992/05/04  21:26:04  jdp]
# 
# Revision 1.1  1992/01/19  02:40:55  devrcs
# 	Initial revision
# 
# $EndLog$
#

VPATH 			= ../eacl

LIBRARIES 		= libkeacl.a

EXPDIR 			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=
CFLAGS 			= ${KERNELCFLAGS}
OFILES 			= dacl_anode.o dacl_xcred.o
INCFLAGS		= -I../eacl

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
