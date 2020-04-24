# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.8.2  1996/03/11  13:26:18  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:14:20  marty]
#
# Revision 1.1.8.1  1995/12/08  17:56:27  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:28:54  root]
# 
# Revision 1.1.6.1  1994/03/25  17:29:28  sekhar
# 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
# 	[1994/03/25  17:27:48  sekhar]
# 
# Revision 1.1.4.3  1992/12/29  12:37:41  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:33:43  zeliff]
# 
# Revision 1.1.4.2  1992/12/07  18:04:41  burati
# 	CR5978 Add -D_PWD_INCLUDED to those files including pi_pwd.h on HP800
# 	[1992/12/07  16:28:52  burati]
# 
# Revision 1.1.2.2  1992/04/23  14:12:24  burati
# 	Initial version.
# 	[1992/04/23  01:10:25  burati]
# 
# $EndLog$
#

#   04/07/92 //littl/prgy2/src/security/admin/passwd_import/Makefile
#
# Copyright (c) Hewlett-Packard Company 1992
# Unpublished work. All Rights Reserved.
#
# DCE Security Services
#

# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS}

.if ${TARGET_MACHINE} == "HP800"
passwd_import.o_CFLAGS = ${CFLAGS} -D_PWD_INCLUDED
pi_getpwent.o_CFLAGS = ${CFLAGS} -D_PWD_INCLUDED
.endif

INCFLAGS = -I../../../h

VPATH    = ../../../h
# Programs to build
PROGRAMS = passwd_import

# Files to install
ILIST = passwd_import

# Directory in which to install the files
IDIR = /bin/

passwd_import_OFILES =	\
			passwd_import.o \
                        passwd_import_subs.o \
                        pi_getgrent.o \
                        pi_getpwent.o \
                        pi_strsep.o \
                        balanced_trees.o \
                        dcesadmsg.o

# Libraries the programs need to link with
LDIRS = -L../../../utils
LIBS  =  -ldce

passwd_import_LIBS = ${LDIRS} -lsecutil ${LIBS} ${${TARGET_MACHINE}_LIBS}

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
