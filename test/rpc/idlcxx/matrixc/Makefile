# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.1  1996/09/09  20:59:10  bfc
# 	finish TETizing and cleanup
# 	[1996/09/09  20:42:22  bfc]
#
# Revision 1.1.2.3  1996/03/09  20:49:12  marty
# 	Add OSF copyright
# 	[1996/03/09  20:27:39  marty]
# 
# Revision 1.1.2.2  1996/02/23  19:13:35  root
# 	OT 13362, fix for IBM C++ suffix
# 	[1996/02/23  19:11:57  root]
# 
# Revision 1.1.2.1  1995/12/11  19:34:44  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:18:18  root]
# 
# $EndLog$

PROGRAMS        = client server
ILIST		= ${PROGRAMS}
TESTNAME	= matrixc
SCRIPTS		= run
.if ${TARGET_MACHINE} == "RIOS"
SUFFIX=C
.elif ${TARGET_MACHINE} == "ALPHA"
SUFFIX=cxx
.endif

CFLAGS		+= 
IDLFLAGS	+= -lang cxx

IDLFILES		= matrix.idl
GEN_INC_FILE		= matrix.h
CLIENT_STUB_FILES	= matrix_cstub.o
SERVER_STUB_FILES	= matrix_sstub.o
CLIENT_CXX_FILES	= matrix_cstub.${SUFFIX}
SERVER_CXX_FILES	= matrix_sstub.${SUFFIX}

all:	${PROGRAMS}

server:	${GEN_INC_FILE} ${SERVER_CXX_FILES} server.o static.o svrlib.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o server.X \
	${SERVER_STUB_FILES} server.o static.o svrlib.o ${_CPLUSPLUSFLAGS_} \
	${_cpluscplus_LIBDIRS} \
	${LIBS}
	mv server.X server

client:	${GEN_INC_FILE} ${CLIENT_CXX_FILES} client.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o client.X \
	${CLIENT_STUB_FILES} ${SERVER_STUB_FILES} client.o ${_CPLUSPLUSFLAGS_} \
	${LIBS}
	mv client.X client

.include "../idlcxx.mk"
.include <${RULES_MK}>

