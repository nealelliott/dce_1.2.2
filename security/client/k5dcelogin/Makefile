#   
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#  
# All Rights Reserved
#    
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.1  1996/06/12  17:49:31  arvind
# 	Merge from DCE_1.2
# 	[1996/05/24  23:34 UTC  sommerfeld  /main/DCE_1.2.2/1]
#
# $EndLog$
#  
#
# Copyright (c) Hewlett-Packard Company 1996
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# /client/k5dcelogin
#

# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -DLOGIN_PROGRAM=\"/bin/login\" \
	  ${${TARGET_MACHINE}_CFLAGS} ${SECURITY_CFLAGS}

# Programs to build
PROGRAMS = k5dcelogin

# Files to install
ILIST = k5dcelogin

# Directory in which to install the files
IDIR = /bin/

k5dcelogin_OFILES = k5dcelogin.o

# Libraries the programs need to link with
LIBS = -ldce

k5dcelogin_LIBS = ${LIBS} ${${TARGET_MACHINE}_LIBS}

.include <${RULES_MK}>
