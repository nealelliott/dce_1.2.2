#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.850.1  1996/10/02  17:24:12  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:31:53  damon]
#
# Revision 1.1.845.3  1994/06/09  14:00:25  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:35:40  annie]
# 
# Revision 1.1.845.2  1994/02/04  20:13:12  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:10:18  devsrc]
# 
# Revision 1.1.84.2  1994/01/20  18:42:58  annie
# 	added copyright header
# 	[1994/01/20  18:39:14  annie]
# 
# Revision 1.1.84.1  1993/12/07  17:18:53  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  13:57:13  jaffe]
# 
# 	OT 8531 - HPUX does not support a standalone episode, so remove it from here.
# 	[1993/09/02  12:47:14  kissel]
# 
# Revision 1.1.3.2  1993/08/13  16:48:22  kinney
# 	<<<Empty log message>>>
# 	[1993/08/11  20:13:09  kinney]
# 
# 	Rearrange link of sefsvmunix
# 	[1993/05/04  19:23:46  sommerfeld]
# 
# 	Initial revision.
# 	[1993/04/12  18:32:29  toml]
# 
# $EndLog$

VPATH			= ${TARGET_OS}:${MAKETOP}/../export/${target_machine}/usr/lib

LIBRARIES		= libefs.a

KERNEL_CC		=

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}
CFLAGS			= ${KERNELCFLAGS}
OFILES			= epi_config.o efs_stubs.o

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists (${TARGET_MACHINE}/Makefile)
.include "${TARGET_MACHINE}/Makefile"
.endif

.include <${RULES_MK}>
