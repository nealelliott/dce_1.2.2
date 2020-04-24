#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#  HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/11/13  20:32:20  arvind
# 	OSF DCE 1.2.2 Drop 5
# 	[1996/11/13  19:51:04  arvind]
#
# Revision /main/DCE_1.2.2/1  1996/10/18  00:38 UTC  arvind
# 	# 	#
# 	# 	[1996/10/16  20:32 UTC  arvind  /main/arvind_capi/1]
# 
# $EndLog$
#
#  @DEC_COPYRIGHT@
#  Copyright (c) 1996
#  Digital Equipment Corporation, Maynard, Massachusetts, USA
#  All Rights Reserved
#  









LIB_OBJS = pkc_plcy_api.o \
           pkc_plcy_hierarchy.o \
           pkc_plcy_preload.o \
           pkc_plcy_reg.o \
           pkc_plcy_registry.o \
           pkc_cross_cert.o \

INCFLAGS = -I../../h
CFLAGS = -DDEBUG -DNEW_FNCAP

VPATH = .:../../h

$(LIB_OBJS) : asn.h \
	      x509.h \
	      x500name.h \
	      xdsxx.h \
	      pkc_api.h \
	      pkc_certs.h \
	      pkc_common.h \
	      pkc_cross_cert.h \
	      pkc_crypto_reg.h \
	      pkc_plcy_hierarchy.h \
	      pkc_plcy_registry.h \
	      pkc_plcy_reg.h \
	      x500_att.h \
	      x501.h


libpkplcy.a_OFILES = ${LIB_OBJS}
libpkplcy_OFILES = ${LIB_OBJS}

.if !defined(LIBDCE_LIBRARY_MAKEFILE) 


LIBRARIES = libpkplcy.a
IDIR = /usr/lib/

.include<${RULES_MK}>

.endif
