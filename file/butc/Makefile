# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# $EndLog$

VPATH			= ../bak

PROGRAMS        	= butc 

TECHNOLOGY		= dfs
SAMSFILES 		= bcm.sams
SAMS_TARGETS 		= dfsbcmmsg.h dfsbcmmsg.c dfsbcmmac.h \
			  dfsbcmsvc.c dfsbcmsvc.h dfsbcm.cat

IDIR			= /bin/
ILIST 			= ${PROGRAMS}
CFLAGS			= -D_VOPS -D_BSD_INCLUDES -Daiws -D__unix \
			  ${USERCFLAGS} -D_BSD -DOSF_NO_SOCKET_DUMP

butc_OFILES		= tcprocs.o lwps.o tcmain.o list.o recoverDb.o \
			  tcudbprocs.o dump.o tcstatus.o  \
			  dfsbcmsvc.o dfsbcmmsg.o

LIBS            	= -lpipe -lbudb -lbxdb -lbubasics -lbutm -lnubik \
			  -licl -ltpq -lasyncq \
			  -lvolc -lftutil -lafs4int -lftserver -lfldb \
                          -lncompat -lrep -lcommondata -lpipe -lcom_err \
			  -ldacl -ldauth -ldacllfs -ldacl -ldauth -ldacllfs \
			  -ldfsncs -lbomb -losi -lcmd \
                  	  -lafsutil -lafssys -ldce ${OS_LIBS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>
