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
# Revision 1.2.6.2  1996/03/11  02:28:31  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:19  marty]
#
# Revision 1.2.6.1  1995/12/11  20:14:27  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:47:15  root]
# 
# Revision 1.2.4.3  1993/01/13  19:54:20  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:22:00  bbelch]
# 
# Revision 1.2.4.2  1992/12/07  13:20:51  zeliff
# 	Removed duplicate OSF copyright markers
# 	[1992/12/06  00:44:49  zeliff]
# 
# Revision 1.2.2.2  1992/05/01  15:51:45  rsalz
# 	Edited as part of rpc6 drop.
# 	[1992/05/01  00:58:14  rsalz]
# 
# Revision 1.2  1992/01/19  22:14:19  devrcs
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
##      src/test/rpc/runtime/kv2test/Makefile
##
##  FACILITY:
##
##      Remote Procedure Call (RPC) 
##
##  ABSTRACT:
##
##  Makefile for kernel resident pieces of v2test.
##
##

VPATH		= ../v2test_lib

LIBRARIES	= libkv2test.a
EXPDIR		= /usr/lib/
INCFLAGS 	= -I. -I../v2test_lib
CFLAGS		= -DKERNEL -D_KERNEL -DINET ${${TARGET_MACHINE}_KERNEL_CFLAGS} 

libkv2test.a_OFILES	= v2server.o v2client.o crc.o

.if exists (${TARGET_MACHINE}/machdep.mk)
.include <${TARGET_MACHINE}/machdep.mk>
.endif
.include <${RULES_MK}>

