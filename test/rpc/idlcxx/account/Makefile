# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.1  1996/09/09  20:58:32  bfc
# 	finish TETizing and cleanup
# 	[1996/09/09  19:48:26  bfc]
#
# Revision 1.1.2.3  1996/03/09  20:47:45  marty
# 	Add OSF copyright
# 	[1996/03/09  20:26:34  marty]
# 
# Revision 1.1.2.2  1996/02/23  19:13:25  root
# 	OT 13362, fix for IBM C++ suffix
# 	[1996/02/23  19:11:44  root]
# 
# Revision 1.1.2.1  1995/12/11  19:32:51  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:17:16  root]
# 
# $EndLog$

TESTNAME	= account
IDLFILES	= ${TESTNAME}.idl checking.idl savings.idl
PROGRAMS        = client server
ILIST		= ${PROGRAMS}
SCRIPTS		= run
.if ${TARGET_MACHINE} == "RIOS"
SUFFIX=C
.elif ${TARGET_MACHINE} == "ALPHA"
SUFFIX=cxx
.endif

GEN_INC_FILES		= checking.h savings.h ${TESTNAME}.h
SERVER_STUB_FILES	= checking_sstub.o savings_sstub.o ${TESTNAME}_sstub.o
CLIENT_STUB_FILES	= checking_cstub.o savings_cstub.o ${TESTNAME}_cstub.o
SERVER_CXX_FILES	= checking_sstub.${SUFFIX} savings_sstub.${SUFFIX} ${TESTNAME}_sstub.${SUFFIX}
CLIENT_CXX_FILES	=  checking_cstub.${SUFFIX} savings_cstub.${SUFFIX} ${TESTNAME}_cstub.${SUFFIX}

all:	${PROGRAMS}

client:	${GEN_INC_FILES} ${CLIENT_CXX_FILES} client.o
	echo ${SHLIBDIRS}
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o client.X \
	${CLIENT_STUB_FILES} client.o ${_CPLUSPLUSFLAGS_} \
	${_cpluscplus_LIBDIRS} ${LIBS}
	mv client.X client

server:	${GEN_INC_FILES} ${SERVER_CXX_FILES} server.o svrlib.o
	echo ${SHLIBDIRS}
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o server.X \
	${SERVER_STUB_FILES} server.o svrlib.o ${_CPLUSPLUSFLAGS_} \
	${_cpluscplus_LIBDIRS} ${LIBS}
	mv server.X server

.include "../idlcxx.mk"
.include <${RULES_MK}>

