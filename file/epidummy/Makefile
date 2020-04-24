# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.62.1  1996/10/02  17:15:24  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:06:50  damon]
#
# Revision 1.1.56.2  1994/06/09  13:55:51  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:30:24  annie]
# 
# Revision 1.1.56.1  1994/02/04  20:09:48  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:08:24  devsrc]
# 
# Revision 1.1.54.1  1993/12/07  17:15:45  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  13:32:27  jaffe]
# 
# Revision 1.1.4.2  1993/01/18  20:54:10  cjd
# 	embedded copyright notice
# 	[1993/01/18  17:40:52  cjd]
# 
# Revision 1.1.2.2  1992/01/24  01:51:06  devsrc
# 	Fixed logs
# 	[1992/01/23  22:10:25  devsrc]
# 
# $EndLog$
LIBRARIES		= libepidum.a

OFILES			= epidummy.o

EXPDIR			= /usr/lib/
KERNEL_CC		=
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}


CFLAGS			= ${KERNELCFLAGS}

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
