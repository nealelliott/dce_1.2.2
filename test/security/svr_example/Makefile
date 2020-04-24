# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:36:27  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:04:27  marty]
#
# Revision 1.1.4.1  1995/12/11  21:52:28  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:54:09  root]
# 
# Revision 1.1.2.3  1994/08/10  19:17:23  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  19:17:07  annie]
# 
# Revision 1.1.2.2  1994/08/09  22:26:17  bhaim
# 	make sure acl_server is placed in install area
# 	[1994/08/09  22:25:57  bhaim]
# 
# Revision 1.1.2.1  1994/08/02  20:11:33  bhaim
# 	move acl_server to test area
# 	[1994/08/02  20:11:02  bhaim]
# 
# $EndLog$
#
#
# * The Example code is provided to you as "Pass-through Code".
# * Pass-through Code and its related Documentation are not warrented,
# * supported, or indemnified by OSF, and are provided "AS IS".
# * You can use, sublicense, or disribute the same at your own risk.
#
#  OSF DCE Version 1.0 
#
#   Makefile.ODE V=5 11/13/91 //littl/prgy/src/admin/dacl_edit/test
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# /client/admin/acl_edit/test Makefile
#

# Additional places to look for files
VPATH = ../../../security/idl:../../../security/client/acl

# Flags for the IDL compiler
IDLFLAGS = -keep c_source 
IDLINCFLAGS = -I../../../security/idl 
IDLFILES = 	\
		rdaclif.idl \
		rdaclifv0.idl



# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../../../security/h -I../../../security/server/rs -I../../../security/idl

# Programs to build
PROGRAMS = acl_server

ILIST           = ${PROGRAMS}

acl_server_OFILES = 	rdaclif_sstub.o    \
			rdaclifv0_sstub.o  \
			dacl_manager.o     \
			dacl_server.o

# Libraries the program needs to link with
LDIRS = -L../../../security/utils
EXE_LIBS =  -lsecutil -ldce
acl_server_LIBS = ${LDIRS} ${EXE_LIBS} ${${TARGET_MACHINE}_LIBS}

.include <${RULES_MK}>
