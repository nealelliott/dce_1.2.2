# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
# the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/10/17  18:23:44  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:52:04  damon]
#
# Revision 1.1.2.1  1994/07/28  17:38:16  mckeen
# 	Initial Revision
# 	[1994/07/28  14:10:28  mckeen]
# 
# $EndLog$
#
DATAFILES = README runtests test1 test2 test3 test4 test5

ILIST = ${DATAFILES}

.if exists($MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>

