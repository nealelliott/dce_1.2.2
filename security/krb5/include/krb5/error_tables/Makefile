# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.2  1996/03/09  23:29:40  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:00  marty]
#
# Revision 1.1.6.1  1995/12/08  17:42:00  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:57:00  root]
# 
# Revision 1.1.4.2  1992/12/29  13:58:50  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:52:18  zeliff]
# 
# Revision 1.1.2.2  1992/04/07  19:18:39  mckeen
# 	Changed compile_et to ${COMPILE_ET}.  COMPILE_ET is now defined in osf.dce.mk
# 	[1992/03/13  15:46:24  mckeen]
# 
# Revision 1.1  1992/01/19  14:48:19  devrcs
# 	Initial revision
# 
# $EndLog$
#

#   Makefile.ODE V=1 07/03/91 //littl/prgy/krb5/include/krb5/error_tables
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# /krb5/include/krb5/error_tables Makefile
#

# Files to export for build
INCLUDES = 	isode_err.h \
		kdb5_err.h  \
		kdc5_err.h  \
		krb5_err.h 

# Directory in which to place exported files
EXPDIR = /usr/include/krb5/

.include <${RULES_MK}>

kdc5_err.h : kdc5_err.et
	${COMPILE_ET} $>
