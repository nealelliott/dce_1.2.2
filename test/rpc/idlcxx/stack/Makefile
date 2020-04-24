# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.1  1996/09/09  20:59:23  bfc
# 	finish TETizing and cleanup
# 	[1996/09/09  20:43:19  bfc]
#
# Revision 1.1.2.3  1996/03/09  20:49:53  marty
# 	Add OSF copyright
# 	[1996/03/09  20:28:10  marty]
# 
# Revision 1.1.2.2  1996/02/23  19:13:41  root
# 	OT 13362, fix for IBM C++ suffix
# 	[1996/02/23  19:12:06  root]
# 
# Revision 1.1.2.1  1995/12/11  19:35:45  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:18:47  root]
# 
# $EndLog$

TESTNAME	= stack
IDLFILES	= icalculate.idl istack.idl
PROGRAMS        = client server
ILIST		= ${PROGRAMS}
SCRIPTS		= run
.if ${TARGET_MACHINE} == "RIOS"
SUFFIX=C
.elif ${TARGET_MACHINE} == "ALPHA"
SUFFIX=cxx
.endif

CFLAGS		+= 
IDLFLAGS	+= -lang cxx 

GEN_INC_FILES	= icalculate.h istack.h

SERVER_STUB_FILES	= icalculate_sstub.o istack_cstub.o
CLIENT_STUB_FILES	= icalculate_cstub.o istack_sstub.o
SERVER_CXX_FILES	= icalculate_sstub.${SUFFIX} istack_cstub.${SUFFIX}
CLIENT_CXX_FILES	= icalculate_cstub.${SUFFIX} istack_sstub.${SUFFIX}

all:	${PROGRAMS}

client:	${GEN_INC_FILES} ${CLIENT_CXX_FILES} client.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o client.X \
	${CLIENT_STUB_FILES} client.o ${_CPLUSPLUSFLAGS_} \
	${_cpluscplus_LIBDIRS} ${LIBS}
	mv client.X client

server:	${GEN_INC_FILES} ${SERVER_CXX_FILES} server.o svrlib.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o server.X \
	${SERVER_STUB_FILES} server.o svrlib.o ${_CPLUSPLUSFLAGS_} \
	${_cpluscplus_LIBDIRS} ${LIBS}
	mv server.X server

.include "../idlcxx.mk"
.include <${RULES_MK}>

