# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.1  1996/09/09  20:59:06  bfc
# 	finish TETizing and cleanup
# 	[1996/09/09  20:42:06  bfc]
#
# Revision 1.1.2.3  1996/03/09  20:49:04  marty
# 	Add OSF copyright
# 	[1996/03/09  20:27:33  marty]
# 
# Revision 1.1.2.2  1996/02/23  19:13:33  root
# 	OT 13362, fix for IBM C++ suffix
# 	[1996/02/23  19:11:55  root]
# 
# Revision 1.1.2.1  1995/12/11  19:34:35  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:18:12  root]
# 
# $EndLog$

# .SUFFIXES:  .o .c .cxx .h

TESTNAME	= matrix
IDLFILES	= matrix.idl
PROGRAMS        = client server
ILIST		= ${PROGRAMS}
SCRIPTS		= run
.if ${TARGET_MACHINE} == "RIOS"
SUFFIX=C
.elif ${TARGET_MACHINE} == "ALPHA"
SUFFIX=cxx
.endif

GEN_INC_FILES	= ${TESTNAME}.h
stub_CXXFILES	= ${TESTNAME}_sstub.${SUFFIX} ${TESTNAME}_cstub.${SUFFIX}
stub_OFILES	= ${TESTNAME}_sstub.o ${TESTNAME}_cstub.o

all:	${PROGRAMS}

server:	${GEN_INC_FILES} ${stub_CXXFILES} server.o svrlib.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o server.X \
	${stub_OFILES} server.o svrlib.o ${_CPLUSPLUSFLAGS_} \
	${_cpluscplus_LIBDIRS} ${LIBS}
	mv server.X server

client:	${GEN_INC_FILES} ${stub_CXXFILES} client.o
	${CPLUSPLUS_CC} ${CPLUSPLUS_LDFLAGS} ${LIBDIRS} -o client.X \
	${stub_OFILES} client.o ${_CPLUSPLUSFLAGS_} \
	${LIBS}
	mv client.X client

.include "../idlcxx.mk"
.include <${RULES_MK}>

