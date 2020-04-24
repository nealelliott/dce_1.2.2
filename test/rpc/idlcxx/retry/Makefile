# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.1  1996/09/09  20:59:20  bfc
# 	finish TETizing and cleanup
# 	[1996/09/09  20:43:08  bfc]
#
# Revision 1.1.2.3  1996/03/09  20:49:45  marty
# 	Add OSF copyright
# 	[1996/03/09  20:28:04  marty]
# 
# Revision 1.1.2.2  1996/02/23  19:13:40  root
# 	OT 13362, fix for IBM C++ suffix
# 	[1996/02/23  19:12:04  root]
# 
# Revision 1.1.2.1  1995/12/11  19:35:36  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:18:42  root]
# 
# $EndLog$

# .SUFFIXES:	.o .c .cxx .h

TESTNAME	= retry
IDLFILES	= number.idl
PROGRAMS        = client server serverx
ILIST		= ${PROGRAMS}
SCRIPTS		= run
.if ${TARGET_MACHINE} == "RIOS"
SUFFIX=C
.elif ${TARGET_MACHINE} == "ALPHA"
SUFFIX=cxx
.endif

GEN_INC_FILES		= number.h
SERVER_STUB_FILES	= number_sstub.o
CLIENT_STUB_FILES	= number_cstub.o
SERVER_CXX_FILES	= number_sstub.${SUFFIX}
CLIENT_CXX_FILES	= number_cstub.${SUFFIX}

all:	${PROGRAMS}

client:	${GEN_INC_FILES} ${CLIENT_CXX_FILES} client.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o client.X \
	${CLIENT_STUB_FILES} client.o ${_CPLUSPLUSFLAGS_} \
	${_cpluscplus_LIBDIRS} ${LIBS}
	mv client.X client

serverx:	${GEN_INC_FILES} ${SERVER_CXX_FILES} server.o svrlib.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o serverx.X \
	${SERVER_STUB_FILES} server.o svrlib.o ${_CPLUSPLUSFLAGS_} \
	${_cpluscplus_LIBDIRS} ${LIBS}
	mv serverx.X serverx

server:
	echo "serverx ncacn_ip_tcp &" > server
	echo "serverx ncadg_ip_udp &" >> server
	chmod +x server

.include "../idlcxx.mk"
.include <${RULES_MK}>

