# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.1  1996/09/09  20:58:53  bfc
# 	finish TETizing and cleanup
# 	[1996/09/09  19:49:31  bfc]
#
# Revision 1.1.2.3  1996/03/09  20:48:25  marty
# 	Add OSF copyright
# 	[1996/03/09  20:27:05  marty]
# 
# Revision 1.1.2.2  1996/02/23  19:13:29  root
# 	OT 13362, fix for IBM C++ suffix
# 	[1996/02/23  19:11:48  root]
# 
# Revision 1.1.2.1  1995/12/11  19:33:39  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:17:45  root]
# 
# $EndLog$

TESTNAME	= card
IDLFILES	= ${TESTNAME}.idl iplayer.idl
PROGRAMS        = client server
ILIST		= ${PROGRAMS}
SCRIPTS		= run
.if ${TARGET_MACHINE} == "RIOS"
SUFFIX=C
.elif ${TARGET_MACHINE} == "ALPHA"
SUFFIX=cxx
.endif

GEN_INC_FILES	= ${TESTNAME}.h iplayer.h

all:	${PROGRAMS}

client:	${GEN_INC_FILES} card_cstub.${SUFFIX} iplayer_sstub.${SUFFIX} client.o \
player.o util.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o client.X \
	card_cstub.o iplayer_sstub.o client.o player.o util.o \
	${_CPLUSPLUSFLAGS_} ${_cpluscplus_LIBDIRS} ${LIBS}
	mv client.X client

server:	${GEN_INC_FILES} card_sstub.${SUFFIX} iplayer_cstub.${SUFFIX} server.o \
svrlib.o player.o cardimpl.o util.o svrlib.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o server.X \
	card_sstub.o iplayer_cstub.o server.o svrlib.o player.o cardimpl.o \
	util.o \
	${_CPLUSPLUSFLAGS_} ${_cpluscplus_LIBDIRS} ${LIBS}
	mv server.X server

.include "../idlcxx.mk"
.include <${RULES_MK}>

