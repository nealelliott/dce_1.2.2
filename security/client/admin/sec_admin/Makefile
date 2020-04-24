# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.2  1996/03/11  13:27:29  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:15:20  marty]
#
# Revision 1.1.6.1  1995/12/08  18:02:34  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:30:51  root]
# 
# Revision 1.1.4.1  1994/03/25  17:34:00  sekhar
# 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
# 	[1994/03/25  17:32:59  sekhar]
# 
# Revision 1.1.2.4  1993/01/29  18:51:40  burati
# 	Add new modules
# 	[1993/01/14  22:59:33  burati]
# 
# Revision 1.1.2.3  1992/12/29  12:42:46  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:36:54  zeliff]
# 
# Revision 1.1.2.2  1992/11/04  22:25:23  burati
# 	Initial version
# 	[1992/11/04  21:00:30  burati]
# 
# $EndLog$

#
# Copyright (c) Hewlett-Packard Company 1992
# Unpublished work. All Rights Reserved.
#
# DCE Security Services
#

# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS}

INCFLAGS = -I../../../h

VPATH    = ../../../h

# Programs to build
PROGRAMS = sec_admin

# Files to install
ILIST = sec_admin

# Directory in which to install the files
IDIR = /bin/

sec_admin_OFILES = \
	sec_admin.o \
	sa_replist.o \
	sa_util.o    \
        dcesadmsg.o

# Libraries the programs need to link with
LDIRS = -L../../../utils
LIBS  =  -ldce

sec_admin_LIBS = ${LDIRS} -lsecutil ${LIBS} ${${TARGET_MACHINE}_LIBS}

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>

