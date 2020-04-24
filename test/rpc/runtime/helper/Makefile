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
# Revision 1.1.59.2  1996/03/11  02:28:16  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:09  marty]
#
# Revision 1.1.59.1  1995/12/11  20:13:41  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:46:40  root]
# 
# Revision 1.1.57.1  1993/12/28  22:11:36  pellis
# 	Updated for ODE 2.3 make
# 	[1993/12/28  21:55:01  pellis]
# 
# Revision 1.1.4.2  1993/01/13  19:51:53  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:19:23  bbelch]
# 
# Revision 1.1.2.3  1992/05/20  18:31:34  garyf
# 	fix incorrect include
# 	[1992/05/20  18:25:04  garyf]
# 
# Revision 1.1.2.2  1992/05/01  13:48:06  rsalz
# 	Added as part of rpc6 drop.
# 	[1992/04/30  23:22:06  rsalz]
# 
# $EndLog$
#
#
##
##  Copyright (c) 1991 by
##      Hewlett-Packard Company, Palo Alto, Ca. & 
##      Digital Equipment Corporation, Maynard, Mass.
##
##  NAME:
##
##      src/test/rpc/runtime/helper/Makefile
##
##  FACILITY:
##
##      Remote Procedure Call (RPC) 
##
##  ABSTRACT:
##
##  Makefile for user resident pieces of kernel helper code for 
##  device-driver-based helper.
##

VPATH		= ${TARGET_MACHINE}

PROGRAMS	= ${${TARGET_MACHINE}_PROGRAMS}

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif
.include <${RULES_MK}>
