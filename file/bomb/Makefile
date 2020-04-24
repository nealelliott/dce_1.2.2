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
# Revision 1.1.46.1  1996/10/02  17:03:09  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:04:37  damon]
#
# Revision 1.1.40.2  1994/06/09  13:51:40  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:25:12  annie]
# 
# Revision 1.1.40.1  1994/02/04  20:05:52  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:06:27  devsrc]
# 
# Revision 1.1.38.1  1993/12/07  17:12:37  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  12:53:15  jaffe]
# 
# Revision 1.1.2.3  1993/06/09  21:22:29  zeliff
# 	OT 8132: provide IDIR for libbomb, bomb.h
# 	[1993/06/09  20:06:20  zeliff]
# 
# Revision 1.1.2.2  1993/05/11  12:43:52  jaffe
# 	Transarc delta: jdp-db3358-add-bomb-point-utilities 1.5
# 	  Selected comments:
# 	    This delta adds a bomb point (error injection) package to DFS.  The
# 	    README file in src/file/bomb describes the package in detail.
# 	    Builds the DFS bomb package.
# 	    Made a variety of improvements like including the proper copyright/history
# 	    header at the top of each file, fixing initialization problems,
# 	    changing the delay and wait bomb points to affect the thread instead
# 	    of the process, and making bomb points cheaper when none are set to go off.
# 	    Add header.
# 	    Fix an oversight that caused compilation errors.
# 	    Integrate the package into the big build.
# 	    Install bomb_set in /opt/dcelocal/bin/.
# 	[1993/05/10  16:31:43  jaffe]
# 
# $EndLog$

# Copyright (C) 1993 Transarc Corporation - All rights reserved
#
# $Header: /u0/rcs_trees/dce/rcs/file/bomb/Makefile,v 1.1.46.1 1996/10/02 17:03:09 damon Exp $

PROGRAMS = bomb bomb_test
bomb_LIBS = -lbomb -losi -lafsutil -lafssys -lcmd -lcmdAux -lncompat -lcom_err\
	    -ldce ${OS_LIBS}
bomb_test_LIBS = -lbomb -losi -lafsutil -lafssys -lncompat -lcom_err -ldce \
	    ${OS_LIBS}

INCLUDES = bomb.h

LIBRARIES = libbomb.a
libbomb.a_OFILES = bomb_core.o bomb_syscall.o
libbomb.a_EXPDIR = /usr/lib/

SCRIPTS = bomb_cat

OTHERS = dfsbmb.cat

CFLAGS = ${USERCFLAGS} -DDFS_BOMB
EXPDIR = /usr/include/dcedfs/
EXPLIB_TARGETS = ${LIBRARIES:S/^/export_/g}
ILIST = ${INCLUDES} ${OTHERS} ${SCRIPTS} ${PROGRAMS}
IDIR = /bin/
bomb.h_IDIR = /share/include/dcedfs/
dfsbmb.cat_IDIR = /nls/msg/en_US.ASCII/

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

bomb.h: bomb.p.h bomb_errors.et
	-${CP} ${bomb.p.h:P} .
	${COMPILE_ET} ${bomb_errors.et:P} -h bomb
	${RM} -f bomb.p.h

dfsbmb.cat: bomb_errors.msf
	${GENCAT} $@ $>

${libbomb.a_OFILES} bomb.o bomb_test.o: bomb.h

${PROGRAMS}: libbomb.a
