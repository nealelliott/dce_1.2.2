# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.7.2  1996/03/09  23:26:18  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:56  marty]
#
# Revision 1.1.7.1  1995/12/08  00:15:42  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:56:38  root]
# 
# Revision 1.1.5.3  1993/01/03  22:37:09  bbelch
# 	Embedding copyright notice
# 	[1993/01/03  19:53:58  bbelch]
# 
# Revision 1.1.5.2  1992/12/23  19:40:51  zeliff
# 	Embedding copyright notice
# 	[1992/12/23  14:54:46  zeliff]
# 
# Revision 1.1.2.2  1992/05/01  18:36:10  rsalz
# 	"Changed as part of rpc6 drop."
# 	[1992/05/01  18:24:30  rsalz]
# 
# 	  20-mar-92 nm    Cleanup and simplify as a result of .mk file improvements
# 	[1992/05/01  18:04:44  rsalz]
# 
# Revision 1.1  1992/01/19  03:14:17  devrcs
# 	Initial revision
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
##      makefile.rpccp (src/rpc/rpccp/Makefile)
##
##  FACILITY:
##
##      Remote Procedure Call (RPC) 
##
##  ABSTRACT:
##
##  Makefile for rpccp.
##

INCFLAGS        = -I../runtime -I../runtime/${TARGET_MACHINE}

PROGRAMS	= rpccp

IDIR		= /bin/
EXPDIR		= /usr/bin/

ILIST		= ${PROGRAMS}

OFILES		= rpccp.o cp_help.o

.include <${RULES_MK}>
