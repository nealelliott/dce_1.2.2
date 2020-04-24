# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.1  1996/09/09  20:59:00  bfc
# 	finish TETizing and cleanup
# 	[1996/09/09  20:41:40  bfc]
#
# Revision 1.1.2.3  1996/03/09  20:48:48  marty
# 	Add OSF copyright
# 	[1996/03/09  20:27:21  marty]
# 
# Revision 1.1.2.2  1996/02/23  19:13:31  root
# 	OT 13362, fix for IBM C++ suffix
# 	[1996/02/23  19:11:51  root]
# 
# Revision 1.1.2.1  1995/12/11  19:34:18  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:18:02  root]
# 
# $EndLog$

# .SUFFIXES:	.o .c .cxx .h

TESTNAME	= local_object
IDLFILES	= savings.idl checking.idl account.idl
PROGRAMS        = client server
ILIST		= ${PROGRAMS}
SCRIPTS		= run
.if ${TARGET_MACHINE} == "RIOS"
SUFFIX=C
.elif ${TARGET_MACHINE} == "ALPHA"
SUFFIX=cxx
.endif

GEN_INC_FILES	= checking.h savings.h account.h
SERVER_STUB_FILES	= checking_sstub.o savings_sstub.o account_sstub.o
SERVER_CXX_FILES	= checking_sstub.${SUFFIX} savings_sstub.${SUFFIX} account_sstub.${SUFFIX}
CLIENT_STUB_FILES	= checking_cstub.o savings_cstub.o account_cstub.o
CLIENT_CXX_FILES	= checking_cstub.${SUFFIX} savings_cstub.${SUFFIX} account_cstub.${SUFFIX}

all:	${PROGRAMS}

client:	${GEN_INC_FILES} client.o
	${CPLUSPLUS_CC} -v ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o client.X \
	client.o \
	${_CPLUSPLUSFLAGS_} ${_cpluscplus_LIBDIRS} ${LIBS}
	mv client.X client

server:	client
	cp client server

.include "../idlcxx.mk"
.include <${RULES_MK}>

