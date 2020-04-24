#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# All Rights Reserved
#
# @DEC_COPYRIGHT@
# Copyright (c) 1996
# Digital Equipment Corporation, Maynard, Massachusetts, USA
# All Rights Reserved
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/11/13  20:42:38  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/13  19:59:36  arvind]
#
# Revision /main/DCE_1.2.2/1  1996/10/18  00:40 UTC  arvind
# 	CAPI drop from DEC
# 	[1996/10/16  20:31 UTC  arvind  /main/arvind_capi/1]
# 
# $EndLog$
#
#  



LIB_OBJS = xdsxx.o

IDIR = /usr/lib/

libpkxds.a_OFILES = ${LIB_OBJS}
libpkxds_OFILES = ${LIB_OBJS}

.if !defined(LIBDCE_LIBRARY_MAKEFILE)

CFLAGS          += ${${TARGET_MACHINE}_CFLAGS}

#CFLAGS += -DXDS_DEBUG

LIBRARIES = libpkxds.a

OBJECTS = ${LIB_OBJS}

.include <${RULES_MK}>

.endif
