# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.26.1  1996/10/02  16:57:43  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:03:40  damon]
#
# Revision 1.1.20.3  1994/08/25  17:34:23  mckeen
# 	Added s12y calls
# 	[1994/08/25  17:22:48  mckeen]
# 
# Revision 1.1.20.2  1994/06/09  13:50:00  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:23:28  annie]
# 
# Revision 1.1.20.1  1994/02/04  20:04:18  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:05:44  devsrc]
# 
# Revision 1.1.18.1  1993/12/07  17:11:20  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  12:42:02  jaffe]
# 
# Revision 1.1.12.4  1993/01/18  19:50:07  cjd
# 	Embedded copyright notice
# 	[1993/01/18  19:23:47  cjd]
# 
# $EndLog$
#

INCLUDES		= bc.h  dfsbcmmsg.h dfsbcmsvc.h

LIBRARIES		= libbxdb.a
PROGRAMS        	= bak

EXPDIR			= /usr/include/dcedfs/
dfsbcmmsg.h_EXPDIR	= /usr/include/dcedfs/
dfsbcmsvc.h_EXPDIR	= /usr/include/dcedfs/
libbxdb.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS			= dfsbak.cat

TECHNOLOGY=dfs

SAMSFILES = bcm.sams
SAMS_TARGETS = dfsbcmmsg.h dfsbcmmsg.c dfsbcmmac.h dfsbcmsvc.c dfsbcmsvc.h \
    dfsbcm.cat

ILIST 			= ${PROGRAMS} ${OTHERS} bc.h  dfsbcm.cat
IDIR			= /bin/
dfsbak.cat_IDIR         = /nls/msg/en_US.ASCII/
dfsbcm.cat_IDIR		= /nls/msg/en_US.ASCII/
bc.h_IDIR		= /share/include/dcedfs/

CFLAGS			= -DUBIK_DB -D_VOPS -D_BSD_INCLUDES -Daiws \
			  -D__unix ${USERCFLAGS} -D_BSD

LIBS            	= -lbxdb -lbudb -lbubasics -lbutm -lnubik -licl -lvolc\
			  -lpipe -lftutil -lftserver -lfldb -lncompat -ltpq\
			  -ldacl -ldauth -ldacllfs -ldacl -ldauth -ldacllfs \
			  -lrep -ldfsncs -lcmd -lcmdAux -lcommondata \
			  -lcom_err -lafs4int -lbomb -lafsutil \
			  -lafssys -losi -ldce ${OS_LIBS}

libbxdb.a_OFILES	= ubik_db_if.o status.o dlq.o tape_hosts.o volstub.o
bak_OFILES		= main.o regex.o dsvs.o dump.o restore.o dump_sched.o \
			  vol_sets.o bc_status.o commands.o  dfsbcmsvc.o \
			  dfsbcmmsg.o

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

bc.h bucoord_errs.msf: bucoord_errs.et bc.p.h
	-${CP} ${bc.p.h:P} .
	${COMPILE_ET} ${bucoord_errs.et:P} -h bc
	${RM} -f bc.p.h

dfsbak.cat: bucoord_errs.msf
	 ${GENCAT} $@ $>
