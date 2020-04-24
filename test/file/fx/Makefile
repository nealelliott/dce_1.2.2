#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#  
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.943.1  1996/10/17  18:23:24  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:51:53  damon]
#
# Revision 1.1.938.3  1994/02/07  16:28:11  mckeen
# 	Fixed to work with ode 2.3 make
# 	[1994/02/07  16:27:25  mckeen]
# 
# Revision 1.1.938.2  1994/02/04  20:42:38  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:23:20  devsrc]
# 
# Revision 1.1.936.1  1993/12/07  17:43:00  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  19:05:30  jaffe]
# 
# $EndLog$
# 
#
#  Makefile --
#	For the test/file/fx directory.
#

SUBDIRS = scripts

DATA_SUBDIRS = scripts

DATAFILES = README

PROGRAMS = fx_test

ILIST = ${DATAFILES} ${PROGRAMS}

CCTYPE = ansi

CFLAGS = ${USERCFLAGS} -U_BSD -DITL_FS_TEST

fx_test_OFILES = fx_test.o
fx_test_LIBS   = -litlfx -litlcommon -litlUtils -litl \
		 -lafsutil -ldfsncs -lafs4int -lfldb -lnubik -ltpq \
		 -lncompat -ldacl -ldauth -ldacllfs -ldacl \
		 -ldauth -ldacllfs -licl -lafsutil -losi -lcom_err \
		 -lcommondata -lafssys -ldce ${OS_LIBS}

.if ${TARGET_OS} == "SUNOS5"
fx_test_LIBS += -lsocket
.endif

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>



