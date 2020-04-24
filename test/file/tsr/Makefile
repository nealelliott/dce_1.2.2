# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.112.1  1996/10/17  18:36:44  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:58:03  damon]
#
# Revision 1.1.107.3  1994/02/07  16:28:23  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:36  mckeen]
# 
# Revision 1.1.107.2  1994/02/04  20:47:27  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:25:33  devsrc]
# 
# Revision 1.1.105.1  1993/12/07  17:46:31  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  19:41:21  jaffe]
# 
# Revision 1.1.2.4  1993/04/09  17:15:42  gmd
# 	Added TSR-README to ILIST via DATAFILES.
# 	[1993/04/09  00:08:40  gmd]
# 
# Revision 1.1.2.3  1993/01/11  18:37:20  htf
# 	Embedded copyright notice
# 	[1993/01/11  17:22:03  htf]
# 
# Revision 1.1.2.2  1992/11/19  13:36:59  jaffe
# 	Transarc delta: tu-add-tsr-test 1.1
# 	  Selected comments:
# 	    Add a bran new test program, tsrtest and its Makefile
# 	    Build the tsr test program
# 	[1992/11/18  17:01:14  jaffe]
# 
# $EndLog$
#
#  Makefile --
#	For the test/file/tsr directory.
#

IMODE = 		0755
trylock.c_IMODE	=	0644
README_IMODE	=	0644
TSR-README_IMODE =	0644

PROGRAMS = tsrtest trylock

CCTYPE = ansi

CFLAGS = ${USERCFLAGS}

tsrtest_OFILES = tsrtest.o

DATAFILES =	README TSR-README trylock.c

SCRIPTS = 	movetsrtest utils remotelock

ILIST = ${SCRIPTS} ${DATAFILES} ${PROGRAMS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
