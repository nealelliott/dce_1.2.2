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
# Revision 1.1.2.1  1996/11/13  20:31:11  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/13  19:50:04  arvind]
#
# Revision /main/DCE_1.2.2/1  1996/10/18  00:38 UTC  arvind
# 	CAPI drop from DEC
# 	[1996/10/16  20:32 UTC  arvind  /main/arvind_capi/1]
# 
# $EndLog$
#
#  


LIB_OBJS = pkc_crypto_alg_rsamd2.o \
	   pkc_crypto_alg_rsamd5.o \
	   pkc_crypto_preload.o \
	   pkc_crypto_reg.o \

$(LIB_OBJS) : ../../h/pkc_crypto_reg.h

pkc_crypto_alg_rsamd2.o : ../../h/pkc_crypto_alg_rsamd2.h

pkc_crypto_alg_rsamd5.o : ../../h/pkc_crypto_alg_rsamd5.h

EXPDIR = /usr/lib/

libpkcrypto.a_OFILES = ${LIB_OBJS}
libpkcrypto_OFILES = ${LIB_OBJS}

.if !defined(LIBDCE_LIBRARY_MAKEFILE)

OBJECTS=${LIB_OBJS}

LIBRARIES = libpkcrypto.a

INCFLAGS = -I../../h -I../../../h

.include <${RULES_MK}>

.endif
