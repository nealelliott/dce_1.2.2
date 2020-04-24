#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# Copyright (C) 1994 Transarc Corporation.  All rights reserved.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.5.1  1996/10/17  18:21:52  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:51:30  damon]
#
# $EndLog$
#
#
#

DATAFILES = mtest1 mtest2 mtest3 mtest4 mtest5 mtestsub1 mtestsub2 mtestsub4 \
	runtests statusCheck README

ILIST = ${DATAFILES}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
