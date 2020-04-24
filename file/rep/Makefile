# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# $EndLog$


PROGRAMS        	= repclient repserver rep_queue_test
INCLUDES 	 	= rep_trace.h dfsrpsmsg.h dfsrpssvc.h

EXPDIR                  = /usr/include/dcedfs/

IDIR			= /bin/
repser.h_IDIR		= /share/include/dcedfs/
rep_trace.h_IDIR	= /share/include/dcedfs/

OTHERS			= dfszrp.cat
dfszrp.cat_IDIR		= /nls/msg/en_US.ASCII/

TECHNOLOGY		= dfs

SAMSFILES		= rps.sams
SAMS_TARGETS		= dfsrpsmsg.h dfsrpsmsg.c dfsrpsmac.h \
			  dfsrpssvc.c dfsrpssvc.h dfsrps.cat
dfsrps.cat_IDIR		= /nls/msg/en_US.ASCII

ILIST 			= ${PROGRAMS} ${INCLUDES} ${OTHERS} dfsrps.cat
CFLAGS                  = -Dunix ${USERCFLAGS} -D_BSD -DOSF_NO_SOCKET_DUMP

LIBS            	= -lrep \
			  -lnubik -lftserver -lfldb -lftutil -ltpq -lbomb \
			  -lasyncq -lncompat -ldacl -ldauth -ldacllfs -ldacl \
			  -lpipe -ldauth -ldacllfs -lcom_err -lcmd -lafsutil \
			  -lafs4int -licl -losi -lafssys -lcommondata \
			  -lasync -ldfsncs -ldce ${OS_LIBS} 

.if !defined(NO_LICENSE_MANAGEMENT)
rep_main.o_CFLAGS	= ${CFLAGS} -D_EPISODE_LIC_MGMT_
repserver_LIBS		= ${LIBS} -llic -lnetls -lncs15
.endif

repclient_OFILES 	= repclient.o
repserver_OFILES        = rep_main.o rep_peer.o rep_addr.o rep_boot.o \
                          rep_log.o rep_tkint.o rep_queue.o rep_host.o \
                          dfsrpssvc.o dfsrpsmsg.o
rep_queue_test_OFILES	= rep_queue_test.o rep_queue.o
rep_queue_test_LIBS	= -licl -losi -ldce ${OS_LIBS}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

dfszrp.cat: rep_trace.msf
	${GENCAT} $@ $>

rep_boot.o rep_log.o rep_main.o rep_tkint.o: dfsrpsmsg.h dfsrpssvc.h
rep_trace.h rep_trace.msf: rep_trace.et		
	${COMPILE_ET} ${rep_trace.et:P} -h rep_trace

