# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:15:32  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:32:47  marty]
#
# Revision 1.1.4.1  1995/12/08  20:55:01  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  20:50:07  root]
# 
# Revision 1.1.2.1  1994/06/14  19:57:25  weir
# 	DCE 1.1 Beta check-in
# 	[1994/06/14  16:15:09  weir]
# 
# $EndLog$
# 
#----------------------------------------------------------------------------------------
# [23.V.94]
# 
# ODE Makefile for timop_svc -- installs all files into:
# 
#     <dce-root-dir>/dce/install/<platform>/opt/dce1.1/usr/examples/svc/timop_svc
# 
# 
#----------------------------------------------------------------------------------------


ILIST	= README Makefile_timop_svc timop_svc.acf timop_svc.idl timop_svc_aux.h \
	timop_svc_client.c timop_svc_client.h timop_svc_manager.c timop_svc_refmon.c \
	timop_svc_server.c timop_svc_server.h timop_svc_svc.c tsv.sams \
	timop_svc_setup.dcecp timop_svc_unsetup.dcecp

IDIR	= /usr/examples/svc/timop_svc/

IMODE	= 644

# The following line explicitly includes the "common makefiles". For more
#  information, see Chapter 12 of the Porting Guide...
.include <${RULES_MK}>
