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
# Revision 1.1.2.1  1996/11/13  20:33:58  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/13  19:52:15  arvind]
#
# Revision /main/DCE_1.2.2/1  1996/10/18  00:39 UTC  arvind
# 	CAPI drop from DEC
# 	[1996/10/16  20:32 UTC  arvind  /main/arvind_capi/1]
# 
# $EndLog$
#
#  

LIB_OBJS = pkc_certs.o

libpktrust.a_OFILES = ${LIB_OBJS}
libpktrust_OFILES = ${LIB_OBJS}

${LIB_OBJS} : ../../h/pkc_certs.h \
	      ../../h/asn.h \
	      ../../h/pkc_common.h \
	      ../../h/x509.h \
	      ../../h/x500name.h \

.if !defined(LIBDCE_LIBRARY_MAKEFILE)

LIBRARIES = libpktrust.a
IDIR = /usr/lib/

.include <${RULES_MK}>

.endif
