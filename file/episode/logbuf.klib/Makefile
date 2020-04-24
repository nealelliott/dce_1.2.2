# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.903.1  1996/10/02  17:24:35  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:32:07  damon]
#
# Revision 1.1.898.3  1994/06/09  14:00:38  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:35:55  annie]
# 
# Revision 1.1.898.2  1994/02/04  20:13:27  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:10:27  devsrc]
# 
# Revision 1.1.89.1  1993/12/07  17:19:08  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  13:59:22  jaffe]
# 
# Revision 1.1.5.2  1993/01/19  15:27:12  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:47:42  cjd]
# 
# Revision 1.1.3.4  1992/05/05  19:08:17  jdp
# 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
# 	delta jdp-epimerge-add-klib-dirs 1.1
# 	[1992/05/04  22:09:56  jdp]
# 
# Revision 1.1.3.3  1992/01/24  03:00:20  devsrc
# 	Merging dfs6.3
# 	[1992/01/24  00:09:36  devsrc]
# 
# Revision 1.1.3.2  1992/01/23  19:42:48  zeliff
# 	Moving files onto the branch for dfs6.3 merge
# 	[1992/01/23  18:28:55  zeliff]
# 	Revision 1.1.1.3  1992/01/23  22:13:07  devsrc
# 	Fixed logs
# 
# $EndLog$
#

VPATH			= ../logbuf

LIBRARIES		= libklogbuf.a

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=
CFLAGS			= ${KERNELCFLAGS}
OFILES			= buffer.o log.o recover.o tran.o crc.o
INCFLAGS		= -I../logbuf

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

