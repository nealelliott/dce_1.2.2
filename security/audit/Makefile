# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# (c) Copyright International Business Machines Corp. 1994, 1996
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.1  1996/07/08  18:24:23  arvind
# 	Audit for 1.2.2
# 	[1996/06/21  20:23 UTC  arvind  /main/arvind_audit122/1]
#
# 	Audit for 1.2.2
# 	[1996/03/09  23:26:59  marty  1.1.4.2]
#
# Revision 1.1.4.2  1996/03/09  23:26:59  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:42:30  marty]
# 
# Revision 1.1.4.1  1995/12/08  17:37:49  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:17:27  root]
# 
# Revision 1.1.2.5  1994/07/06  20:22:54  zeliff
# 	ot11109: remove auditcp from SUBDIRS
# 	[1994/07/06  19:26:43  zeliff]
# 
# Revision 1.1.2.4  1994/03/15  22:43:02  zeliff
# 	OT10142: add DTS and AUD event class files
# 	[1994/03/15  22:42:42  zeliff]
# 
# Revision 1.1.2.3  1994/03/11  22:48:19  luan
# 	CR 9775 9776: Moved svc stuff from auditd to libaudit.
# 	[1994/03/11  22:46:30  luan]
# 
# Revision 1.1.2.2  1994/01/12  20:51:08  zeliff
# 	OT9726: turn Audit on in the build
# 	[1994/01/12  19:36:54  zeliff]
# 
# Revision 1.1.2.1  1993/12/22  12:38:24  zeliff
# 	First version of code
# 	[1993/12/21  15:00:49  zeliff]
# 
# $EndLog$
#
#
# DCE Audit Services 
# top level Makefile
#

# Subdirectories for the autogen pass
SAMIDL_SUBDIRS = libaudit

# Subdirectories that export files
EXPINC_SUBDIRS = h idl libaudit

# Sudirectories that export libraries
EXPLIB_SUBDIRS = libaudit

# Subdirectories that build installables
SUBDIRS = idl h libaudit auditd ec

.include <${RULES_MK}>
