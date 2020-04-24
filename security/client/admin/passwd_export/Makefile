# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.2  1996/03/11  13:26:15  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:14:17  marty]
#
# Revision 1.1.6.1  1995/12/08  17:56:07  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:28:46  root]
# 
# Revision 1.1.4.1  1994/03/25  17:21:54  sekhar
# 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
# 	[1994/03/25  17:16:54  sekhar]
# 
# Revision 1.1.2.2  1992/12/29  12:37:32  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:11:47  zeliff]
# 
# Revision 1.1  1992/01/19  14:46:44  devrcs
# 	Initial revision
# 
# $EndLog$
#

#   Makefile.ODE V=1 10/11/91 //littl/prgy/src/admin/passwd_export
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# /client/admin/rgy_edit Makefile
#

# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../../../h

VPATH    = ../../../h

# Programs to build
PROGRAMS = passwd_export 

# Files to install
ILIST = passwd_export 

# Directory in which to install the files
IDIR = /bin/

passwd_export_OFILES =	\
			passwd_export.o \
                        dcesadmsg.o

# Libraries the programs need to link with
LDIRS = 
LIBS =  -ldce
	
passwd_export_LIBS = ${LDIRS} ${LIBS} ${${TARGET_MACHINE}_LIBS} 

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
