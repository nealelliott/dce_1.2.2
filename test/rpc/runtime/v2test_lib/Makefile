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
# Revision 1.1.11.2  1996/03/11  02:29:29  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:54  marty]
#
# Revision 1.1.11.1  1995/12/11  20:15:25  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:48:21  root]
# 
# Revision 1.1.8.1  1993/11/02  22:02:07  tom
# 	Install scripts to run the tests and new README file.
# 	[1993/11/02  22:00:41  tom]
# 
# Revision 1.1.4.3  1993/01/12  00:03:13  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:26:08  bbelch]
# 
# Revision 1.1.4.2  1992/11/02  15:37:58  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:40:22  root]
# 
# Revision 1.1.2.2  1992/05/01  15:42:14  rsalz
# 	  20-mar-92 nm Cleanup and simplify as a result of .mk file improvements
# 	[1992/05/01  01:24:31  rsalz]
# 
# Revision 1.1  1992/01/19  03:42:02  devrcs
# 	Initial revision
# 
# $EndLog$
#
##
##  Copyright (c) 1991 by
##      Hewlett-Packard Company, Palo Alto, Ca. & 
##      Digital Equipment Corporation, Maynard, Mass.
##
##
##  NAME:
##
##      makefile.v2test_lib (src/test/rpc/runtime/v2test_lib/Makefile)
##
##  FACILITY:
##
##      Remote Procedure Call (RPC) 
##
##  ABSTRACT:
##
##  Makefile for v2test.
##
##

SCRIPTS		= v2test_tcp.sh v2test_udp.sh
PROGRAMS        = v2client v2server
DATAFILES	= README

ILIST		= ${PROGRAMS} ${SCRIPTS} ${DATAFILES}
README_IMODE	= 644

v2client_OFILES = v2client.o crc.o

v2server_OFILES = v2server.o

.include <${RULES_MK}>
