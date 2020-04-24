# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# (c) Copyright International Business Machines Corp. 1994, 1996
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/07/08  18:28:05  arvind
# 	Audit for 1.2.2
# 	[1996/06/21  20:25 UTC  arvind  /main/arvind_audit122/1]
#
# 	Audit for 1.2.2
# 	[1996/03/09  23:27:04  marty  1.1.4.2]
#
# Revision 1.1.4.2  1996/03/09  23:27:04  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:42:35  marty]
# 
# Revision 1.1.4.1  1995/12/08  17:40:15  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:17:54  root]
# 
# Revision 1.1.2.3  1994/04/12  13:31:13  rousseau
# 	Changed to export audit_control.acf
# 	[1994/04/12  13:30:48  rousseau]
# 
# Revision 1.1.2.2  1994/01/14  17:56:03  zeliff
# 	OT9744: change IDIR for header files
# 	[1994/01/14  17:50:26  zeliff]
# 
# Revision 1.1.2.1  1993/12/22  12:39:10  zeliff
# 	First version of code
# 	[1993/12/21  15:01:23  zeliff]
# 
# $EndLog$
#
#
# DCE Audit Facility
# /idl Makefile
#

# Flags for the IDL compiler
IDLFLAGS =  -keep none 

IDLFILES = audit_log.idl audit_control.idl audit_pickle.idl audit_store.idl \
	   audit_update.idl
IDLOBJ   = ${IDLFILES:.idl=.h}

# Files to export and their locations
INCLUDES = ${IDLFILES} ${IDLOBJ} audit_control.acf
EXPDIR	= /usr/include/dce/

# Files to intall and their locations
# ILIST   = ${IDLFILES} ${IDLOBJ}

ILIST   = audit_log.h audit_control.idl audit_control.h 
IDIR	= /share/include/dce/

.include <${RULES_MK}>
