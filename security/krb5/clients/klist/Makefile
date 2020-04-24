# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.4.2  1996/03/09  23:29:31  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:44:52  marty]
#
# Revision 1.2.4.1  1995/12/08  17:39:11  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:56:27  root]
# 
# Revision 1.2.2.2  1992/12/29  13:55:18  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:50:12  zeliff]
# 
# Revision 1.2  1992/01/19  22:13:47  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
# 
#  OSF DCE Version 1.0 
#

#   Makefile.ODE V=3 10/16/91 //littl/prgy/krb5/clients/klist
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# krb5/clients/klist  Makefile
#

# Compiler flags for this directory
CFLAGS	 =  -DOSF_DCE ${${TARGET_MACHINE}_CFLAGS}
INCFLAGS =  -I${EXPORTBASE}/usr/include/krb5 -I../../../h

# Program to build
PROGRAMS =  klist
# Programs to install and where to install them
ILIST    = klist
IDIR     = /bin/

# Libraries to link the program with
LIBS	 =  -ldce ${${TARGET_MACHINE}_LIBS}

.include <${RULES_MK}>
