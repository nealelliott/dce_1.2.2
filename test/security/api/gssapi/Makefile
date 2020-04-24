#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY 
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:34:06  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:03:11  marty]
#
# Revision 1.1.4.1  1995/12/11  20:19:39  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:51:33  root]
# 
# Revision 1.1.2.1  1994/06/28  20:01:07  wray
# 	GSSAPI BL3 (OT 10824)
# 	[1994/06/28  19:25:28  wray]
# 
# $EndLog$
#
# DCE Security Services, GSSAPI 
# test/security/api/gssapi Makefile
#

# Compiler flags for this directory
CFLAGS = ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../../../../security/h 

# Programs to build
PROGRAMS = 	\
		test-gssapi      \


test-gssapi_OFILES      = test-gssapi.o

# Libraries the programs need to link with
LIBS = -ldce ${${TARGET_MACHINE}_LIBS}

DATAFILES = \

ILIST		= ${PROGRAMS} ${DATAFILES}

.include <${RULES_MK}>

