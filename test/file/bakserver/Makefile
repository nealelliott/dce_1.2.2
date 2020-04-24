#
# @OSF_COPYRIGHT@
#  
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.5.1  1996/10/17  18:16:12  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  16:49:06  damon]
#
# $EndLog$
# 
# Copyright (C) 1994 Transarc Corporation.  All rights reserved.
#
#  Makefile --
#	For the test/file/bakserver directory.
#

SUBDIRS = scripts

DATA_SUBDIRS = scripts

DATAFILES = README runtests

PROGRAMS = bakserver_test 

ILIST = ${PROGRAMS} ${DATAFILES}

CCTYPE = ansi

CFLAGS = ${USERCFLAGS} -U_BSD -DITL_FS_TEST

bakserver_test_OFILES = bakserver_test.o

bakserver_test_LIBS = -litlbaksrv -litlcommon -litlUtils -litl   \
		      -lbudb -lnubik -licl \
		      -lncompat -ltpq \
		      -ldacl -ldauth -ldacllfs \
		      -ldacl -ldauth -ldacllfs \
		      -lafsutil -losi -lcom_err -lcommondata \
		      -lafssys -ldce ${OS_LIBS} 

.if ${TARGET_OS} == "SUNOS5"
bakserver_test_LIBS += -lsocket
.endif

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
