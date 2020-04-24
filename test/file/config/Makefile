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
# Revision 1.1.418.1  1996/10/17  18:18:36  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:50:09  damon]
#
# Revision 1.1.413.1  1994/02/07  16:28:06  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:20  mckeen]
# 
# Revision 1.1.2.3  1993/01/11  18:26:07  htf
# 	Embedded copyright notice
# 	[1993/01/11  16:58:49  htf]
# 
# Revision 1.1.2.2  1992/11/02  15:35:58  root
# 	Created Makefile to move files during 'install' pass of the build.
# 	[1992/10/30  15:28:29  root]
# 
# $EndLog$
#
#
#

DATAFILES = chk_errnr echeck environment

ILIST	= ${DATAFILES}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
