# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.7.2  1996/03/11  13:25:54  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:14:00  marty]
#
# Revision 1.1.7.1  1995/12/08  17:53:58  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:27:53  root]
# 
# Revision 1.1.5.2  1992/12/29  12:36:15  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:32:09  zeliff]
# 
# Revision 1.1.3.3  1992/05/26  21:03:41  burati
# 	Remove references to rdacliftmp
# 	[1992/05/26  20:20:41  burati]
# 
# Revision 1.1.3.2  1992/04/07  19:18:13  mckeen
# 	Removed DATAFILES definition
# 	[1992/03/13  15:41:41  mckeen]
# 
# Revision 1.1  1992/01/19  14:47:53  devrcs
# 	Initial revision
# 
# $EndLog$
#

#   Makefile.ODE V=2 12/09/91 //littl/prgy/src/client/acl
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# /client/acl Makefile
#

# Additional places to search for files
VPATH = ../../idl

# Flags for the IDL compiler
IDLFLAGS = -keep c_source -cepv
IDLINCFLAGS = -I. -I../../idl
IDLFILES = 	\
		rdaclif.idl
rdaclif_ACF =

# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../../h -I../../idl

IDLDATAFILES =  rdaclif.h

.include <${RULES_MK}>

dacl_ifr.o : rdaclif.h
