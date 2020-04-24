# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.6.2  1996/03/09  23:26:00  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:40  marty]
#
# Revision 1.2.6.1  1995/12/08  00:13:24  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:55:18  root]
# 
# Revision 1.2.4.3  1993/01/03  22:34:26  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  19:49:24  bbelch]
# 
# Revision 1.2.4.2  1992/12/23  19:34:57  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  01:15:00  zeliff]
# 
# Revision 1.2.2.2  1992/05/01  17:35:54  rsalz
# 	  27-apr-92 nm    Reformat & remove KERNEL_CC
# 	[1992/05/01  17:33:38  rsalz]
# 
# Revision 1.2  1992/01/19  22:13:29  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#  OSF DCE Version 1.0 
#
#

##  Copyright (c) 1991 by
##      Hewlett-Packard Company, Palo Alto, Ca. & 
##      Digital Equipment Corporation, Maynard, Mass.
##
##
##  NAME:
##
##      src/test/rpc/kdes/Makefile
##
##  FACILITY:
##
##      Remote Procedure Call (RPC) 
##
##  ABSTRACT:
##
##  Makefile for Kernel RPC DES runtime library
##
##

VPATH		= ../sys_idl:${TARGET_MACHINE}:${MAKETOP}/security/krb5/lib/desneuter

INCFLAGS	= -I${TARGET_MACHINE}
LIBRARIES	= libkdes.a
EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
CFLAGS		= -DOSF_DCE ${${TARGET_MACHINE}_KERNEL_CFLAGS} -DNCK -DKERNEL -D_KERNEL

libkdes.a_EXPDIR= /usr/lib/
libkdes.a_OFILES= ${DES_OFILES}

.if exists (${TARGET_MACHINE}/machdep.mk)
.include <${TARGET_MACHINE}/machdep.mk>
.endif
.include <${RULES_MK}>

.if defined (USE_DES)
DES_OFILES	= cbc_encrypt.o ecb_encrypt.o cksum.o make_sched.o des_tables.o weak_key.o
.else
DES_OFILES	= des_neuter.o
.endif
