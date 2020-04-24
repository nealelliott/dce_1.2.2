#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/10/17  18:23:35  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:52:00  damon]
#
# Revision 1.1.2.2  1994/08/22  16:16:23  maunsell_c
# 	build new test3 and test4
# 	[1994/08/10  16:44:15  maunsell_c]
# 
# Revision 1.1.2.1  1994/07/28  17:38:04  mckeen
# 	Initial Revision
# 	[1994/07/28  14:10:24  mckeen]
# 
# $EndLog$

DATAFILES = README runtests test1 test2 test3 test4

ILIST = ${DATAFILES}

.if exists($MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>

