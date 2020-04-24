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
# Revision 1.1.6.2  1996/03/11  02:29:26  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:53  marty]
#
# Revision 1.1.6.1  1995/12/11  20:15:24  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:48:20  root]
# 
# Revision 1.1.4.3  1993/01/12  00:03:11  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:26:03  bbelch]
# 
# Revision 1.1.4.2  1992/11/02  15:37:54  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:40:10  root]
# 
# Revision 1.1.2.3  1992/05/20  18:32:12  garyf
# 	fix incorrect include
# 	[1992/05/20  18:26:23  garyf]
# 
# Revision 1.1.2.2  1992/05/01  15:42:09  rsalz
# 	Part of rpc6 code drop.
# 	[1992/05/01  01:24:26  rsalz]
# 
# $EndLog$
#
#  OSF DCE Version 1.0 
##
##  Copyright (c) 1991 by
##      Hewlett-Packard Company, Palo Alto, Ca. & 
##      Digital Equipment Corporation, Maynard, Mass.
##
##  %a%copyright(,##  )
##
##  NAME:
##
##      src/test/rpc/runtime/uv2test/Makefile
##
##  FACILITY:
##
##      Remote Procedure Call (RPC) 
##
##  ABSTRACT:
##
##  Makefile for user space component of kernel v2test.
##

VPATH		= ../v2test_lib:${TARGET_MACHINE}

LIBS_DCE	= 
PROGRAMS	= kernel_v2client kernel_v2server
INCFLAGS	= -I../v2test_lib
CFLAGS		= -DDEBUG -DKERNEL_TEST_DRIVER ${${TARGET_MACHINE}_CFLAGS}

ILIST		= ${PROGRAMS}

kernel_v2server_OFILES	= v2server.o ${${TARGET_MACHINE}_S_OFILES}
kernel_v2client_OFILES	= v2client.o ${${TARGET_MACHINE}_C_OFILES}

.if exists (${TARGET_MACHINE}/machdep.mk)
.include <${TARGET_MACHINE}/machdep.mk>
.endif
.include <${RULES_MK}>
