# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:50:32  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:11:04  marty]
#
# Revision 1.1.4.1  1995/12/11  23:06:20  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  22:37:22  root]
# 
# Revision 1.1.2.2  1994/06/10  20:51:27  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:20:13  devsrc]
# 
# Revision 1.1.2.1  1994/04/29  21:03:02  pwang
# 	Initial Implementation
# 	[1994/04/29  21:01:01  pwang]
# 
# $EndLog$

#
# Tool programs
#
PROGRAMS 		= tet_setup
tet_setup_OFILES	= tet_setup.o

#
# Install variables
#
ILIST	 		= ${PROGRAMS}
tet_setup_IDIR		= /test/tet/tools/
tet_setup_IMODE		= 4755
tet_setup_IOWNER	= root

#
# Compiler flags
#
#CFLAGS = ${${TARGET_OS}_CFLAGS} -pedantic -Wall
CFLAGS = ${${TARGET_OS}_CFLAGS}
LIBS = -ldce


.include <${RULES_MK}>

.if exists(${TARGET_OS}/osdep.mk)
.include "${TARGET_OS}/osdep.mk"
.endif


