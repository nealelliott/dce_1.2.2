# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# @DEC_COPYRIGHT@
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.2  1996/03/09  20:45:01  marty
# 	Add OSF copyright
# 	[1996/03/09  20:24:42  marty]
#
# Revision 1.1.2.1  1995/12/08  00:13:08  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/jrr_1.2_mothra/4  1995/11/28  22:39 UTC  psn
# 	Fix it to build on IBM.
# 
# 	HP revision /main/jrr_1.2_mothra/3  1995/11/22  22:43 UTC  psn
# 	Fix the install/export tree location for shared /static libraries.
# 
# 	HP revision /main/jrr_1.2_mothra/2  1995/11/21  23:11 UTC  psn
# 	Merge IBM porting changes.
# 
# 	HP revision /main/jrr_1.2_mothra/1  1995/11/21  22:47 UTC  psn
# 	New file for second XIDL drop for DCE 1.2.1
# 	[1995/11/17  17:09 UTC  dat  /main/dat_xidl2/1]
# 
# 	Fix install list
# 	[1994/12/12  15:05:08  acf]
# 	#
# 
# 	DCE for DEC OSF/1 AXP: Initial checkin
# 	[1994/12/09  19:38:22  acf]
# 
# $EndLog$
#

LIBRARIES 		= libidlcxx.a
EXPLIB_TARGETS 	= export_libidlcxx.a
ILIST			= libidlcxx.a
IDIR			= /usr/lib/

.if ${TARGET_MACHINE}!="RIOS"
EXPSHLIB_TARGETS 	= export_libidlcxx.so
libidlcxx.so_EXPDIR 	= /usr/shlib/
.endif

IDLFILES = refcnt.idl

OBJFILES	= refcnt_sstub.o refcnt_cstub.o \
		  hashtab.o ifacemgr.o ndrmi6.o ndrui6.o objref.o \
		  objtable.o proxymgr.o refcnt.o trace.o

libidlcxx.a_OFILES = ${OBJFILES} 

RIOS_CFLAGS		+= -DAIX32
CFLAGS			+= -DTRACE -DDEBUG ${${TARGET_MACHINE}_CFLAGS}

# -fvtable-thunks
ndrui6.o_CFLAGS	=  ${CFLAGS} -DMIA
# trace.o_CFLAGS	=  ${CFLAGS} -DTRACE	#include this line for tracing

INCFLAGS	= -I../lib

IDLFLAGS	= -keep c_source -cepv 

libidlcxx.so_LDFLAGS	= -shared -no_archive -L/usr/lib/cmplrs/cxx
libidlcxx.so_LIBS	=  -lcxx -ldce -lmach -lc_r -lc

libidlcxx.so : libidlcxx.a
	${_LD_} ${_SHLDFLAGS_} -o ${.TARGET} -all libidlcxx.a ${_LIBS_}

.include <${RULES_MK}>

ndrmi6.o objref.o : refcnt.h

