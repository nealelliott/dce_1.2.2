# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.1  1996/09/09  20:58:43  bfc
# 	finish TETizing and cleanup
# 	[1996/09/09  19:48:59  bfc]
#
# Revision 1.1.2.3  1996/03/09  20:48:08  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:50  marty]
# 
# Revision 1.1.2.2  1996/02/23  19:13:27  root
# 	OT 13362, fix for IBM C++ suffix
# 	[1996/02/23  19:11:46  root]
# 
# Revision 1.1.2.1  1995/12/11  19:33:17  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:17:32  root]
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

client:	${GEN_INC_FILES} ${CLIENT_CXX_FILES} client.o
	${CPLUSPLUS_CC} -v ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o client.X \
	${CLIENT_STUB_FILES} client.o \
	${_CPLUSPLUSFLAGS_} ${_cpluscplus_LIBDIRS} ${LIBS}
	mv client.X client

server:	${GEN_INC_FILES} ${SERVER_CXX_FILES} server.o svrlib.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o server.X \
	${SERVER_STUB_FILES} server.o svrlib.o \
	${_CPLUSPLUSFLAGS_} ${_cpluscplus_LIBDIRS} ${LIBS}
	mv server.X server

.include "../idlcxx.mk"
.include <${RULES_MK}>

