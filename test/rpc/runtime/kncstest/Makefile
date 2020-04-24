#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.61.2  1996/03/11  02:28:25  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:16  marty]
#
# Revision 1.2.61.1  1995/12/11  20:14:16  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:47:03  root]
# 
# Revision 1.2.59.2  1994/09/07  15:48:45  marty
# 	CR # 11497 - Remove '-v' option from IDLFLAGS defineittion.
# 	[1994/09/06  21:39:40  marty]
# 
# Revision 1.2.59.1  1993/12/28  22:11:39  pellis
# 	Updated for ODE 2.3 make
# 	[1993/12/28  21:55:04  pellis]
# 
# Revision 1.2.4.3  1993/01/13  19:53:33  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:21:09  bbelch]
# 
# Revision 1.2.4.2  1992/12/07  13:20:47  zeliff
# 	Removed duplicate OSF copyright markers
# 	[1992/12/06  00:44:46  zeliff]
# 
# Revision 1.2.2.4  1992/05/20  18:31:45  garyf
# 	fix incorrect dependencies
# 	fix incorrect include
# 	[1992/05/20  18:25:36  garyf]
# 
# Revision 1.2.2.3  1992/05/01  15:51:41  rsalz
# 	Edited as part of rpc6 drop.
# 	[1992/05/01  00:58:10  rsalz]
# 
# Revision 1.2.2.2  1992/04/07  19:20:29  mckeen
# 	Add dependency of OFILES on IDLFILES
# 	[1992/04/07  16:48:21  mckeen]
# 
# Revision 1.2  1992/01/19  22:14:08  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
##
##  Copyright (c) 1991 by
##      Hewlett-Packard Company, Palo Alto, Ca. & 
##      Digital Equipment Corporation, Maynard, Mass.
##
##  NAME:
##
##      src/test/rpc/runtime/kncstest/Makefile
##
##  FACILITY:
##
##      Remote Procedure Call (RPC) 
##
##  ABSTRACT:
##
##  Makefile for kernel resident pieces of ncstest
##
##
 
VPATH		= ../ncstest

LIBRARIES	= libkncstest.a
EXPDIR		= /usr/lib/
CFLAGS		= -DKERNEL -D_KERNEL ${${TARGET_MACHINE}_KERNEL_CFLAGS}
INCFLAGS	= -I. -I../ncstest
IDLINCFLAGS	= ${INCFLAGS}
IDLFLAGS 	= -no_cpp -keep c_source -no_mepv 

libkncstest.a_OFILES = unkif_cstub.o smpl_cstub.o afs_cstub.o \
		   smpl_sstub.o afs_sstub.o afs_saux.o \
		   smplt.o afst.o afsu.o \
		   smplm.o afsm.o \
		   client.o server.o

IDLFILES	= afs.idl smpl.idl unkif.idl

.if exists (${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif
.include <${RULES_MK}>

${libkncstest.a_OFILES} : ${IDLFILES:.idl=.h}

