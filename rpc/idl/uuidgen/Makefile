# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.8.2  1996/03/09  23:25:58  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:38  marty]
#
# Revision 1.1.8.1  1995/12/07  22:38:26  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:18:56  root]
# 
# Revision 1.1.5.1  1994/04/15  14:39:45  rsalz
# 	Use SAMS and getopt; general cleanup (OT CR 10342).
# 	[1994/04/14  16:26:22  rsalz]
# 
# Revision 1.1.2.3  1993/01/03  22:34:18  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  19:49:09  bbelch]
# 
# Revision 1.1.2.2  1992/12/23  19:34:36  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  01:14:46  zeliff]
# 
# Revision 1.1  1992/01/19  03:05:07  devrcs
# 	Initial revision
# 
# $EndLog$
#

PROGRAMS	= uuidgen
OTHERS		= dceuid.cat
ILIST		= ${PROGRAMS} ${OTHERS}
IDIR		= /bin/
dceuid.cat_IDIR	= /nls/msg/en_US.ASCII/
dceuid.cat_IMODE= 444

SAMS_TARGETS	= dceuidmsg.h dceuidmsg.c dceuid.cat

uuidgen_OFILES	= uuidgen.o dceuidmsg.o
LIBS		= -ldce ${${TARGET_MACHINE}_LIBS}

.include <${RULES_MK}>

dceuid.cat:	uid.sams
	${SAMS} ${SAMSFLAGS} -omc ${.ALLSRC}
dceuidmsg.h:	uid.sams
	${SAMS} ${SAMSFLAGS} -oh ${.ALLSRC}
dceuidmsg.c:	uid.sams
	${SAMS} ${SAMSFLAGS} -ot ${.ALLSRC}

$(uuidgen_OFILES):	dceuidmsg.h
