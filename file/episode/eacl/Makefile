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
# Revision 1.1.786.1  1996/10/02  17:23:59  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:31:48  damon]
#
# Revision 1.1.781.3  1994/06/09  14:00:16  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:35:34  annie]
# 
# Revision 1.1.781.2  1994/02/04  20:13:07  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:10:14  devsrc]
# 
# Revision 1.1.78.1  1993/12/07  17:18:41  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  13:56:28  jaffe]
# 
# Revision 1.1.4.2  1993/01/19  15:26:24  cjd
# 	embedded copyright notice
# 	[1993/01/19  13:46:23  cjd]
# 
# Revision 1.1.2.2  1992/05/05  19:06:18  jdp
# 	delta jdp-epimerge-use-ode-makefiles 1.5
# 	[1992/05/04  21:27:27  jdp]
# 
# Revision 1.1  1992/01/19  02:39:34  devrcs
# 	Initial revision
# 
# $EndLog$
#

INCLUDES		= dacl_anode.h
LIBRARIES		= libeacl.a
EXPDIR			= /usr/include/dcedfs/episode/
libeacl.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

CFLAGS			= ${USERCFLAGS}
OFILES			= dacl_anode.o dacl_xcred.o

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
