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
# Revision 1.1.741.1  1996/10/02  17:53:46  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:41:51  damon]
#
# $EndLog$
#

#src/file/libafs/Makefile

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

