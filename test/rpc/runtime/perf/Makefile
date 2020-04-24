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
# Revision 1.1.11.2  1996/03/11  02:28:54  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:29  marty]
#
# Revision 1.1.11.1  1995/12/11  20:14:52  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/06  15:47 UTC  tatsu_s
# 	Submitted the pipe test.
# 
# 	HP revision /main/tatsu_s_func_test_b1/1  1994/10/10  13:49 UTC  tatsu_s
# 	Added the pipe test.
# 	[1995/12/11  19:47:42  root]
# 
# Revision 1.1.8.1  1993/11/02  22:02:06  tom
# 	Install scripts to run the tests and new README file.
# 	[1993/11/02  22:00:38  tom]
# 
# Revision 1.1.4.3  1993/01/13  19:56:04  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:23:48  bbelch]
# 
# Revision 1.1.4.2  1992/11/02  15:37:44  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:39:52  root]
# 
# Revision 1.1.2.3  1992/05/01  15:51:48  rsalz
# 	  20-mar-92 nm Cleanup and simplify as a result of .mk file improvements
# 	[1992/05/01  00:58:19  rsalz]
# 
# Revision 1.1.2.2  1992/04/07  19:20:35  mckeen
# 	Added  ${server_OFILES} and ${client_OFILES} dependencies
# 	[1992/03/13  15:48:03  mckeen]
# 
# Revision 1.1  1992/01/19  03:41:33  devrcs
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
##      makefile.perf (src/test/rpc/runtime/perf/Makefile)
##
##  FACILITY:
##
##      Remote Procedure Call (RPC) 
##
##  ABSTRACT:
##
##  Makefile for perf test.
##

PROGRAMS        = client server
SCRIPTS		= perf_tcp.sh perf_udp.sh run_client.sh
DATAFILES	= README

ILIST		= ${PROGRAMS} ${SCRIPTS} ${DATAFILES}
README_IMODE	= 644

server_OFILES   = perf_sstub.o    perfb_sstub.o \
                  perfc_cstub.o   perfg_sstub.o  \
		  perfp_sstub.o   perfp.o \
                  perfb.o         perfg.o           perf.o \
                  util.o          server.o

client_OFILES   = perf_cstub.o   perfb_cstub.o \
                  perfc_sstub.o  perfg_cstub.o \
		  perfp_cstub.o \
                  util.o         client.o

IDLFLAGS        = -no_mepv -keep c_source
IDLFILES        = perf.idl perfb.idl perfc.idl perfg.idl perfp.idl


.include <${RULES_MK}>

${server_OFILES} ${client_OFILES} : ${IDLFILES:.idl=.h}
