#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.9.1  1996/10/17  18:25:04  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:52:39  damon]
#
# Revision 1.1.4.2  1994/02/07  16:28:15  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:28  mckeen]
# 
# Revision 1.1.4.1  1994/02/04  20:43:03  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:23:30  devsrc]
# 
# Revision 1.1.2.2  1994/01/20  18:43:53  annie
# 	added copyright header
# 	[1994/01/20  18:40:11  annie]
# 
# Revision 1.1.2.1  1993/12/09  21:20:44  jaffe
# 	Include itl_bomb library for fset tests
# 	[1993/12/09  21:16:35  jaffe]
# 
# $EndLog$

LIBRARIES		= libitlBomb.a
INCLUDES		= itl_bomb.h

EXPDIR			= /usr/include/utils/itl/
EXPLIB_TARGETS		= ${LIBRARIES:S/^/export_/g}

libitlBomb.a_OFILES	= itl_bomb.o
libitlBomb.a_EXPDIR	= /usr/lib/

CFLAGS			= ${USERCFLAGS} -DITL_FS_TEST

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>

