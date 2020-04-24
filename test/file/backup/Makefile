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
# Revision 1.1.10.1  1996/10/17  18:12:07  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:47:43  damon]
#
# Revision 1.1.2.3  1993/01/11  17:36:05  htf
# 	Embedded copyright notice
# 	[1993/01/11  16:52:46  htf]
# 
# Revision 1.1.2.2  1992/11/02  15:35:47  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:15:03  root]
# 
# Revision 1.1  1992/01/19  03:19:21  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#

DATAFILES =	makeconfig startbakserv check runtests test1 test2 test3 \
		test4 test5 test6 test7 test8

ILIST	  = ${DATAFILES}
 
.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.include <${RULES_MK}>
