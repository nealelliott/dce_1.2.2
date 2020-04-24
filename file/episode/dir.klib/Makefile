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
# Revision 1.1.796.1  1996/10/02  17:20:18  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:29:35  damon]
#
# Revision 1.1.790.3  1994/06/09  13:59:59  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:35:16  annie]
# 
# Revision 1.1.790.2  1994/02/04  20:12:44  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:09:59  devsrc]
# 
# Revision 1.1.78.1  1993/12/07  17:18:22  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  13:53:11  jaffe]
# 
# Revision 1.1.4.2  1993/01/18  22:10:16  cjd
# 	embedded copyright notice
# 	[1993/01/18  21:51:15  cjd]
# 
# Revision 1.1.2.2  1992/05/05  19:03:24  jdp
# 	delta jdp-epimerge-add-klib-dirs 1.1
# 	[1992/05/04  21:17:02  jdp]
# 
# Revision 1.1  1992/01/19  02:40:54  devrcs
# 	Initial revision
# 
# $EndLog$
#

VPATH			= ../dir

LIBRARIES		= libkdir.a

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=
CFLAGS			= ${KERNELCFLAGS}
libkdir.a_OFILES	= dir_buffer.o dir_main.o dir_salvage.o
INCFLAGS		= -I../dir

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

