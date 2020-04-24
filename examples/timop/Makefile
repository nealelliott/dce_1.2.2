# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:15:40  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:32:55  marty]
#
# Revision 1.1.4.1  1995/12/08  20:55:24  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  20:50:23  root]
# 
# Revision 1.1.2.1  1994/06/14  19:59:51  weir
# 	DCE 1.1 Beta check-in
# 	[1994/06/14  16:16:44  weir]
# 
# $EndLog$
# 
# 
#
# NOTE: This ODE Makefile only installs the timop source, it does not build
#  it; you must use the "Makefile_timop" Makefile to build the program.
#  By executing "build install_all" in this directory or in the
#
#    <dce-root-dir>/dce/src/examples
#
#  directory, you will cause the timop files to be installed into:
#
#    <dce-root-dir>/dce/install/<platform>/opt/dce1.1/usr/examples/sample
#
#  Note also that the
#
#    <dce-root-dir>/dce/src/examples
#
#  directory is NOT automatically visited during the DCE build or install
#  pass; to install any of the examples you must execute a "build install_all"
#  in the examples directory.
#
#
# See the OSF DCE Porting and Testing Guide, Chapter 12, for information
#  about ODE and ODE makefiles in general.
# 
#
IDIR		= /usr/examples/timop/

# The following specifies the permissions to set on the installed files.
IMODE		= 644

# The following specifies the list of files to install...
ILIST		= Makefile_timop README timop.idl timop.acf \
		  timop_aux.h timop_client.h timop_server.h \
		  timop_client.c timop_server.c timop_manager.c timop_refmon.c

# The following line includes the ODE common makefiles...
.include <${RULES_MK}>
