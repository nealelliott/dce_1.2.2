# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.1  1996/09/09  20:59:25  bfc
# 	finish TETizing and cleanup
# 	[1996/09/09  20:43:29  bfc]
#
# Revision 1.1.2.3  1996/03/09  20:50:02  marty
# 	Add OSF copyright
# 	[1996/03/09  20:28:16  marty]
# 
# Revision 1.1.2.2  1996/02/23  19:13:42  root
# 	OT 13362, fix for IBM C++ suffix
# 	[1996/02/23  19:12:07  root]
# 
# Revision 1.1.2.1  1995/12/11  19:36:00  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:18:52  root]
# 
# $EndLog$

# .SUFFIXES:	.o .c .cxx .h

TESTNAME	= static
IDLFILES	= static.idl
PROGRAMS        = client server
ILIST		= ${PROGRAMS}
SCRIPTS		= run
.if ${TARGET_MACHINE} == "RIOS"
SUFFIX=C
.elif ${TARGET_MACHINE} == "ALPHA"
SUFFIX=cxx
.endif

GEN_INC_FILES	= static.h

SERVER_STUB_FILES	= static_sstub.o
SERVER_CXX_FILES	= static_sstub.${SUFFIX}
CLIENT_STUB_FILES	= static_cstub.o
CLIENT_CXX_FILES	= static_cstub.${SUFFIX}

all:	${PROGRAMS}

client:	${GEN_INC_FILES} ${CLIENT_CXX_FILES} client.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o client.X \
	${CLIENT_STUB_FILES} client.o \
	${_CPLUSPLUSFLAGS_} ${_cpluscplus_LIBDIRS} ${LIBS}
	mv client.X client

server:	${GEN_INC_FILES} ${SERVER_CXX_FILES} server.o svrlib.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o server.X \
	${SERVER_STUB_FILES} server.o svrlib.o \
	${_CPLUSPLUSFLAGS_} ${_cpluscplus_LIBDIRS} ${LIBS}
	mv server.X server

${GEN_INC_FILE} ${SERVER_CXX_FILES} ${CLIENT_CXX_FILES}:	${IDLFILES}

.include "../idlcxx.mk"
.include <${RULES_MK}>

