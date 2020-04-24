# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/10/17  15:44:12  damon
# 	Submitting on behalf of Transarc
# 	[1996/10/17  15:40:49  damon]
#
# $EndLog$
#

VPATH			= ../../../file/bak

PROGRAMS        	= budb_test budb_dump

TECHNOLOGY		= dfs
SAMSFILES 		= bcm.sams
SAMS_TARGETS 		= dfsbcmmsg.h dfsbcmmsg.c dfsbcmmac.h \
			  dfsbcmsvc.c dfsbcmsvc.h dfsbcm.cat

IDIR			= /bin/
ILIST 			= ${PROGRAMS}
CFLAGS			= -D_VOPS -D_BSD_INCLUDES -Daiws -D__unix \
			  ${USERCFLAGS} -D_BSD -I../bakserver

budb_test_OFILES	= budb_test.o dfsbcmsvc.o dfsbcmmsg.o

budb_test_LIBS 		= -lpipe -lbudb -lbxdb -lbubasics -lbutm -lnubik \
			  -licl -ltpq \
			  -lvolc -lftutil -lafs4int -lftserver -lfldb \
                          -lncompat -lrep -lcommondata \
			  -ldacl -ldauth -ldacllfs -ldacl -ldauth -ldacllfs \
			  -ldfsncs -lbomb -losi -lcmd -lcom_err \
                  	  -lafsutil -lafssys -ldce ${OS_LIBS}

budb_dump_OFILES	= budb_dump.o

budb_dump_LIBS 		= -ldce ${OS_LIBS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
