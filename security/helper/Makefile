# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.2  1996/03/09  23:27:38  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:43:05  marty]
#
# Revision 1.1.6.1  1995/12/08  17:30:31  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:54:39  root]
# 
# Revision 1.1.4.2  1992/12/29  13:28:27  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:47:45  zeliff]
# 
# Revision 1.1.2.2  1992/06/03  19:01:54  ahop
# 	  Fix 2143: add DCEFLAGS to CFLAGS so that DCELOCAL gets defined
# 	[1992/05/22  19:42:20  ahop]
# 
# Revision 1.1  1992/01/19  14:55:47  devrcs
# 	Initial revision
# 
# $EndLog$
#

#   Makefile.ODE V=3 12/09/91 //littl/prgy/src/helper
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
##
##  Copyright (c) 1991 by
##      Hewlett-Packard Company, Palo Alto, Ca. 
##
##   Copyright (c) Hewlett-Packard Company 1991
##   Unpublished work. All Rights Reserved.
##  
##
##  NAME
##
##      src/security/helper/Makefile
##
##  FACILITY:
##
##      DCE Security - kernel helper process 
##
##  ABSTRACT:
## 	Makefile for helper process.
##
##    
##

# DCE security services
# helper  Makefile
#
CFLAGS	 =  -DOSF_DCE ${DCEPATHS} ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../h

# Files to export and their locations
INCLUDES  = auth_helper.h
auth_helper.h_EXPDIR    = /usr/include/dce/

# Libraries to build and their locations
LIBRARIES = libauthelper.a
EXPLIB_TARGETS = export_libauthelper.a
EXPDIR = /usr/lib/

libauthelper.a_OFILES = auth_helper.o

.include <${RULES_MK}>

