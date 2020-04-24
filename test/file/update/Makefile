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
# Revision 1.1.412.1  1996/10/17  18:37:23  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:58:18  damon]
#
# Revision 1.1.407.1  1993/12/28  18:02:02  delgado
# 	use quotes for MAKETOP include instead of brackets
# 	[1993/12/28  17:58:43  delgado]
# 
# Revision 1.1.2.4  1993/03/09  21:24:03  jaffe
# 	Transarc delta: comer-ot7207-update-upserver-test 1.1
# 	  Selected comments:
# 	    Updated the upserver/upclient test.
# 	    Export new README file and sets proper install perms on files.
# 	[1993/03/09  20:59:44  jaffe]
# 
# Revision 1.1.2.3  1993/01/11  18:37:53  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:23:08  htf]
# 
# Revision 1.1.2.2  1992/11/02  15:36:18  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:17:09  root]
# 
# Revision 1.1  1992/01/19  03:18:58  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
#

DATAFILES =	uptest README

uptest_IMODE    = 	0755
README_IMODE	=	0644
 
ILIST	  = 	${DATAFILES}
 
.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.include <${RULES_MK}>
