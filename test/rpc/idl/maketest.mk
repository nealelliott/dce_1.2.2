#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: maketest.mk,v $
# Revision 1.2.17.2  1996/03/11  02:14:46  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:53:39  marty]
#
# Revision 1.2.17.1  1995/12/11  20:08:42  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/07/29  19:15 UTC  tatsu_s
# 	Merged Mothra upto DCE 1.1 bata.
# 	[1995/12/11  19:32:41  root]
# 
# Revision 1.1.9.4  1993/10/26  13:58:58  tatsu_s
# 	Use the single-quote for -cc_opt, so that the included flags can
# 	have the double-quotes. Note: If the included flags have
# 	single-quotes, we are still in trouble.
# 	[1993/05/17  14:58:04  tatsu_s]
# 
# Revision 1.2.14.1  1994/05/24  17:57:57  tom
# 	Remove NEED_AUX_FILES since idl no longer gens them
# 	[1994/05/24  16:03:27  tom]
# 
# Revision 1.2.12.1  1993/10/15  21:01:31  ohara
# 	SVR4 porting changes - 8596
# 	[1993/10/15  16:34:56  ohara]
# 
# Revision 1.2.10.3  1993/08/20  21:59:18  tom
# 	Bug 7725 - Add DATAFILES to the install list so RUN_ARGS will
# 	get installed.
# 	[1993/08/20  21:58:59  tom]
# 
# Revision 1.2.10.2  1993/06/15  21:05:52  sommerfeld
# 	Fixed the order of libraries.
# 	Added ${TARGET_FLAGS} to IDL_CC_OPT for HP800
# 	[1993/06/14  21:19:22  sommerfeld]
# 
# Revision 1.2.4.3  1993/01/07  19:58:45  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  13:34:53  bbelch]
# 
# Revision 1.2.4.2  1992/11/02  15:36:28  root
# 	Added ILIST variable for install pass
# 	[1992/10/28  20:35:11  root]
# 
# Revision 1.2.2.2  1992/04/07  19:20:06  mckeen
# 	Added dependencies and changed ${IDLBASE}.h rule to not fire multiple times
# 	[1992/03/31  16:38:27  mckeen]
# 
# Revision 1.2  1992/01/19  22:14:00  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
#  OSF DCE Version 1.0 
# test/rpc/idl/maketest.mk:  a standard makefile for most idl testcases
#

VPATH		= ..
INCFLAGS	= -I..
IDLINCFLAGS	= -I. ${INCFLAGS}
LIBFLAGS	= -L..

# Programs to build
PROGRAMS	= client server

client_OFILES	= client.o ${CSTUB} ${CAUX} ${OTHER_CLIENT_FILES}
server_OFILES	= server.o ${SSTUB} ${SAUX} ${OTHER_SERVER_FILES} manager.o

CSTUB		= ${IDLBASE}_cstub.o
SSTUB		= ${IDLBASE}_sstub.o

MEPV		?= -no_mepv
IDLKEEP		?= both
IDLFLAGS	+= -no_cpp -keep ${IDLKEEP} ${MEPV} -v ${IDL_CC_OPT}
#IDL_CC_OPT	= -cc_opt "${CFLAGS} ${_CC_OL_}"
CFLAGS		+= ${${TARGET_MACHINE}_CFLAGS}

LIBS		= -lidltest -ldce ${${TARGET_MACHINE}_LIBS}

ILIST		= ${PROGRAMS} ${DATAFILES}

.include <${RULES_MK}>

# shared lib build against /usr/shlib/libdce.so --- hsiao
.if (${USE_SHARED_LIBRARIES} && ${TARGET_MACHINE} == "PMAX")
IDL_CC_OPT	= -cc_opt "${CFLAGS} ${_CC_OL_} -pic-lib -L/usr/shlib -ldce"
.elif (${TARGET_MACHINE} == "HP800" || ${TARGET_MACHINE} == "SVR4")
IDL_CC_OPT	= -cc_opt '${_CC_CFLAGS_} ${_CC_OL_} ${TARGET_FLAGS}'
.else
IDL_CC_OPT	= -cc_opt "${CFLAGS} ${_CC_OL_}"
.endif

.if ( ${_CC_} == "cc" )
IDL_CC	?=
.else
IDL_CC	?= -cc_cmd "${_CC_} -c"
.endif

# Other dependencies
.if ( ${IDLKEEP} == "both" || ${IDLKEEP} == "object" )
${CSTUB} ${SSTUB} ${CAUX} ${SAUX}: ${IDLBASE}.h

${IDLBASE}.h :	${IDLBASE}.idl
	${IDL} ${${IDLBASE:=.idl}:P} ${_IDLFLAGS_} ${IDL_CC}
.endif

server.o client.o manager.o client_aux.o ${OTHER_CLIENT_FILES}:	${IDLBASE}.h

# Note:  by defining $INHIBIT_S or $INHIBIT_C, we can prevent these rules
# from taking effect.  Thus, if a special server.c or client.c exists in
# a test directory, we will use it instead of generating it from a template.

server.c${INHIBIT_S}:	server.tmpl
	${SED} -e "s/TESTCASE/${IDLBASE}/" ${.ALLSRC} > ${.TARGET}

client.c${INHIBIT_C}:	client.tmpl
	${SED} -e "s/TESTCASE/${IDLBASE}/" ${.ALLSRC} > ${.TARGET}
