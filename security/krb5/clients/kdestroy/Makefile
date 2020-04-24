# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.6.2  1996/03/09  23:29:29  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:44:50  marty]
#
# Revision 1.2.6.1  1995/12/08  17:38:52  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:56:22  root]
# 
# Revision 1.2.4.1  1994/06/02  21:28:49  mdf
# 	hp_sec_to_osf_3 drop, merge up with latest.
# 	[1994/05/24  15:16:35  mdf]
# 
# 	hp_sec_to_osf_3 drop
# 
# Revision 1.1.4.2  1993/12/06  23:32:19  rps
# 	[CHFts09498] Use DCEPATHS
# 	[1993/12/03  19:59:39  rps]
# 
# Revision 1.1.2.3  1993/09/15  15:42:09  root
# 	    Loading drop DCE1_0_3b03
# 
# Revision 1.2.2.2  1992/12/29  13:54:52  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:49:57  zeliff]
# 
# Revision 1.2  1992/01/19  22:13:46  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
# 
#  OSF DCE Version 1.0 
#
#

#   Makefile.ODE V=3 10/16/91 //littl/prgy/krb5/clients/kdestroy
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# krb5/clients/kdestroy Makefile
#

# Compiler flags for this directory
CFLAGS	 =  -DOSF_DCE ${DCEPATHS} ${${TARGET_MACHINE}_CFLAGS}
INCFLAGS =  -I${EXPORTBASE}/usr/include/krb5 -I../../../h

# Program to build
PROGRAMS =  kdestroy

# Programs to install and where to install them
ILIST    = kdestroy
IDIR     = /bin/


# Libraries to link the program with
LIBS	 =  -ldce ${${TARGET_MACHINE}_LIBS}

.include <${RULES_MK}>
