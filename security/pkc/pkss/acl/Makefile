# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/10/03  20:29:24  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/03  19:24:22  arvind]
#
# 	OSF DCE 1.2.2 Drop 4
#
# Revision /main/DCE_1.2.2/1  1996/09/09  21:27 UTC  arvind
# 	#
# 	[1996/08/30  16:04 UTC  arvind  /main/arvind_pkss/1]
# 
# $EndLog$
#
#
# Copyright (c) Digital Equipment Corporation, 1996
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# pkc/h Makefile
#

BSAFE_INCDIR = /usr/local/rsadsi/include

# Files to export and their locations


EXPDIR = /usr/include/dce/

# Files to install and their locations

IDIR    = /share/include/dce/

#CFLAGS+= -E

INCFLAGS = -I. -I.. -I../server -I../../../../directory/cds/gloria/gdb -I../../../../directory/cds/includes -I../../../psm/h 

OBJECTS = pkss_acl_mgr.o 

pkss_acl_mgr.o : pkss_acl_mgr.cxx pkss_acl_mgr.h

VPATH=../../../directory/cds/library


.include <${RULES_MK}>

