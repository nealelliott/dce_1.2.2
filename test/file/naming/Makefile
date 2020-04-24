#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.679.1  1996/10/17  18:30:55  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:55:29  damon]
#
# Revision 1.1.674.1  1993/12/28  18:01:56  delgado
# 	use quotes for MAKETOP include instead of brackets
# 	[1993/12/28  17:58:35  delgado]
# 
# Revision 1.1.4.3  1993/01/11  18:36:21  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:19:51  htf]
# 
# Revision 1.1.4.2  1992/11/02  15:36:02  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:16:33  root]
# 
# Revision 1.1.2.2  1992/05/21  12:51:50  jaffe
# 	Transarc delta: fred-merge-tests 1.1
# 	  Selected comments:
# 	    Created this delta to merge Phil Hirsch's final DFS test code changes with
# 	    the test code that the OSF had.  Hopefully, this means we and the OSF
# 	    should be looking at the same test code.
# 	    Changes to facilitate merge of local DFS test code with the OSF's.
# 	[1992/05/20  21:06:52  jaffe]
# 
# Revision 1.1  1992/01/19  03:18:48  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#

DATAFILES =	fstest

ILIST	= ${DATAFILES}

 

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
