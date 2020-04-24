#   
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#  
# All Rights Reserved
#    
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/10/03  20:18:11  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/03  19:16:37  arvind]
#
# Revision /main/DCE_1.2.2/1  1996/09/09  21:19 UTC  arvind
# 	PKSS drop from DEC (DCE1.2.2)
# 	[1996/08/30  15:35 UTC  arvind  /main/arvind_pkss/1]
# 
# 	GLORIA (R1.2.2) revision.
# 	[1996/06/24  20:50:33  zee]
# 
# $EndLog$
#
# @DEC_COPYRIGHT@
# Copyright (c) 1996
# Digital Equipment Corporation, Maynard, Massachusetts, USA
# All Rights Reserved

# Makefile for the GDB (G-L-O-R-I-A database),
# based upon UC Berkeley db44 V1.8.5.

NO_DNS_MALLOC=

libgdb_OFILES = \
  bt_close.o bt_conv.o bt_debug.o bt_delete.o bt_get.o bt_open.o \
  bt_overflow.o bt_page.o bt_put.o bt_search.o bt_seq.o mpool.o \
  bt_split.o bt_utils.o db.o bt_trans.o bt_thread.o gdb_db44.o

libgdb.a_OFILES	= ${libgdb_OFILES}

.if !defined(LIBDCE_LIBRARY_MAKEFILE)

EXPDIR			= /usr/include/
libgdb.a_EXPDIR	= /usr/lib/

#INCLUDES		= db.h cdefs.h gdb_db44.h compat.h

.if ${USE_SHARED_LIBRARIES} == 0
LIBRARIES		= libgdb.a
EXPLIB_TARGETS		= ${LIBRARIES:S/^/export_/g}
.endif

LIBRARIES		= libgdb.a
EXPSHLIB_TARGETS	= ${libgdb_OFILES} 

INCFLAGS	 = -I. -I./${TARGET_MACHINE} \
		   -I../../../../dce/utils/db44 -I.. -I../../includes

DEFS= -DO_EXLOCK=F_WRLCK -DO_SHLOCK=F_RDLCK -D__DBINTERFACE_PRIVATE -DTHREADS 

#
# CFLAGS should probably be changed not to include debugging and stats when
# in the product.
#
#CFLAGS	=  ${${TARGET_MACHINE}_CFLAGS} ${DEFS} 
CFLAGS	= -DSTATISTICS -DGDB_DEBUG -DDEBUG ${${TARGET_MACHINE}_CFLAGS} ${DEFS} 

.include <${RULES_MK}>

.endif
