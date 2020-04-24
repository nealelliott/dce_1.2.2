# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.1  1996/09/09  20:58:37  bfc
# 	finish TETizing and cleanup
# 	[1996/09/09  19:48:42  bfc]
#
# Revision 1.1.2.3  1996/03/09  20:47:55  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:41  marty]
# 
# Revision 1.1.2.2  1996/02/23  19:13:26  root
# 	OT 13362, fix for IBM C++ suffix
# 	[1996/02/23  19:11:45  root]
# 
# Revision 1.1.2.1  1995/12/11  19:33:03  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:17:23  root]
# 
# $EndLog$

# .SUFFIXES:	.o .c .cxx .h
TESTNAME	= account
IDLFILES	= savings.idl checking.idl ${TESTNAME}.idl
PROGRAMS        = client server
ILIST		= ${PROGRAMS}
SCRIPTS		= run
.if ${TARGET_MACHINE} == "RIOS"
SUFFIX=C
.elif ${TARGET_MACHINE} == "ALPHA"
SUFFIX=cxx
.endif

GEN_INC_FILES	= checking.h savings.h ${TESTNAME}.h
SERVER_STUB_FILES	= checking_sstub.o savings_sstub.o ${TESTNAME}_sstub.o
SERVER_CXX_FILES	= checking_sstub.${SUFFIX} savings_sstub.${SUFFIX} account_sstub.${SUFFIX}

CLIENT_STUB_FILES	= checking_cstub.o savings_cstub.o ${TESTNAME}_cstub.o
CLIENT_CXX_FILES	= checking_cstub.${SUFFIX} savings_cstub.${SUFFIX} account_cstub.${SUFFIX}

all:	${PROGRAMS}

client:	${GEN_INC_FILES} statics.o ${CLIENT_CXX_FILES} client.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o client.X \
	client.o ${CLIENT_STUB_FILES} ${SERVER_STUB_FILES} statics.o \
	${_CPLUSPLUSFLAGS_} ${_cpluscplus_LIBDIRS} ${LIBS}
	mv client.X client

server:	${GEN_INC_FILES} statics.o ${SERVER_CXX_FILES} server.o svrlib.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o server.X \
	${SERVER_STUB_FILES} ${CLIENT_STUB_FILES} server.o statics.o svrlib.o \
	${_CPLUSPLUSFLAGS_} ${_cpluscplus_LIBDIRS} ${LIBS}
	mv server.X server

.include "../idlcxx.mk"
.include <${RULES_MK}>

