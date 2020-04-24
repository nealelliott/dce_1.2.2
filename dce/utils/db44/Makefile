#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.2  1996/03/09  23:10:51  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:28:31  marty]
#
# Revision 1.1.6.1  1995/12/08  17:36:11  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:17:09  root]
# 
# Revision 1.1.4.1  1994/01/26  16:26:46  bowe
# 	Set up for enclusion in libdce.{a,so}
# 	[1994/01/26  16:26:29  bowe]
# 
# Revision 1.1.2.1  1993/12/01  22:29:52  bowe
# 	Changed DEFS variable to use symbols common to OSF1 and HPUX.
# 	[1993/12/01  22:24:54  bowe]
# 
# 	Initial checkin.
# 	[1993/12/01  15:14:14  bowe]
# 
# $EndLog$
#

libdb44_OFILES = \
  bt_close.o bt_conv.o bt_debug.o bt_delete.o bt_get.o bt_open.o \
  bt_overflow.o bt_page.o bt_put.o bt_search.o bt_seq.o bt_split.o \
  bt_stack.o bt_utils.o db.o hash.o hash_bigkey.o hash_buf.o hash_func.o \
  hash_log2.o hash_page.o hsearch.o mpool.o rec_close.o rec_delete.o \
  rec_get.o rec_open.o rec_put.o rec_search.o rec_seq.o rec_utils.o snprintf.o

libdb44.a_OFILES	= ${libdb44_OFILES}

.if !defined(LIBDCE_LIBRARY_MAKEFILE)

EXPDIR			= /usr/include/
libdb44.a_EXPDIR	= /usr/lib/
INCLUDES		= db.h cdefs.h

.if ${USE_SHARED_LIBRARIES} == 0
LIBRARIES		= libdb44.a
EXPLIB_TARGETS		= ${LIBRARIES:S/^/export_/g}
.endif
LIBRARIES		= libdb44.a
EXPSHLIB_TARGETS	= ${libdb44_OFILES}

INCFLAGS	 = -I.
DEFS		 = -DO_EXLOCK=F_WRLCK -DO_SHLOCK=F_RDLCK
CFLAGS		 = ${${TARGET_MACHINE}_CFLAGS} ${DEFS}

.include <${RULES_MK}>

.endif
