# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.7.2  1996/03/11  13:26:04  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:14:08  marty]
#
# Revision 1.2.7.1  1995/12/08  17:55:07  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:28:22  root]
# 
# Revision 1.2.5.2  1994/08/09  21:13:41  bhaim
# 	moved acl_server to test tree
# 	[1994/08/09  21:13:17  bhaim]
# 
# Revision 1.2.5.1  1994/03/25  14:33:43  sekhar
# 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
# 	[1994/03/25  14:33:11  sekhar]
# 
# Revision 1.2.2.2  1992/12/29  12:36:40  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:11:43  zeliff]
# 
# Revision 1.2  1992/01/19  22:13:41  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
#  OSF DCE Version 1.0 

#   Makefile.ODE V=4 09/19/91 //littl/prgy/src/admin/dacl_edit
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# /client/admin/acl_edit Makefile
#

# Subdirectories that build programs
# acl_server moved to src/test/security/svr_example
#SUBDIRS = svr_example

# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../../../h 

VPATH = ../../../h
# Programs to build
PROGRAMS = acl_edit

# Files to install and their locations
ILIST = acl_edit
IDIR = /bin/

acl_edit_OFILES =  	acl_edit.o \
			deif.o     \
			deif_err.o \
			tty.o      \
                        dcesadmsg.o

# Libraries the programs need to link with
LDIRS = -L../../../utils
LIBS = 	-ldce
acl_edit_LIBS = ${LDIRS} -lsecutil ${LIBS} ${${TARGET_MACHINE}_LIBS}

.include <${RULES_MK}>
