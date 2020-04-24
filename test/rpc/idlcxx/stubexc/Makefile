# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.1  1996/09/09  20:59:27  bfc
# 	finish TETizing and cleanup
# 	[1996/09/09  20:43:40  bfc]
#
# Revision 1.1.2.3  1996/03/09  20:50:11  marty
# 	Add OSF copyright
# 	[1996/03/09  20:28:21  marty]
# 
# Revision 1.1.2.2  1996/02/23  19:13:43  root
# 	OT 13362, fix for IBM C++ suffix
# 	[1996/02/23  19:12:10  root]
# 
# Revision 1.1.2.1  1995/12/11  19:36:07  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:18:57  root]
# 
# $EndLog$

# .SUFFIXES:	.o .c .cxx .h

TESTNAME	= stubexc
IDLFILES	= savings.idl checking.idl account.idl
PROGRAMS        = client server
ILIST		= ${PROGRAMS}
SCRIPTS		= run
.if ${TARGET_MACHINE} == "RIOS"
SUFFIX=C
.elif ${TARGET_MACHINE} == "ALPHA"
SUFFIX=cxx
.endif

GEN_INC_FILES		= checking.h savings.h account.h
SERVER_STUB_FILES	= checking_sstub.o savings_sstub.o account_sstub.o
SERVER_CXX_FILES	= checking_sstub.${SUFFIX} savings_sstub.${SUFFIX} account_sstub.${SUFFIX}
CLIENT_STUB_FILES	= checking_cstub.o savings_cstub.o account_cstub.o
CLIENT_CXX_FILES	= checking_cstub.${SUFFIX} savings_cstub.${SUFFIX} account_cstub.${SUFFIX}

all:	${PROGRAMS}

client:	${GEN_INC_FILES} ${CLIENT_CXX_FILES} client.o statics.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o client.X \
	${CLIENT_STUB_FILES} account_sstub.o savings_sstub.o statics.o \
	client.o ${_CPLUSPLUSFLAGS_} ${_cpluscplus_LIBDIRS} ${LIBS}
	mv client.X client

server:	${GEN_INC_FILES} ${SERVER_CXX_FILES} server.o svrlib.o statics.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o server.X \
	${SERVER_STUB_FILES} savings_cstub.o statics.o \
	server.o svrlib.o ${_CPLUSPLUSFLAGS_} ${_cpluscplus_LIBDIRS} ${LIBS}
	mv server.X server

.include "../idlcxx.mk"
.include <${RULES_MK}>

