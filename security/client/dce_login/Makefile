# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.7.2  1996/03/11  13:27:36  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:15:27  marty]
#
# Revision 1.2.7.1  1995/12/08  18:03:12  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:31:00  root]
# 
# Revision 1.2.5.1  1994/03/25  17:46:07  sekhar
# 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
# 	[1994/03/25  17:40:46  sekhar]
# 
# Revision 1.2.3.3  1992/12/29  12:42:51  zeliff
# 	Embedding copyright notice
# 	[1992/12/28  20:37:03  zeliff]
# 
# Revision 1.2.3.2  1992/10/27  20:15:32  sekhar
# 	[OT 5590]  Added SECURITY_CFLAGS (which sets the DES_NEUTER flag when
# 	           compiling an international version) to CFLAGS.
# 	[1992/10/27  15:25:38  sekhar]
# 
# Revision 1.2  1992/01/19  22:13:44  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#  OSF DCE Version 1.0 
#
#   Makefile.ODE V=5 12/09/91 //littl/prgy/src/client/dce_login
#
# Copyright (c) Hewlett-Packard Company 1991
# Unpublished work. All Rights Reserved.
#
#
# DCE Security Services 
# /client/dce_login
#

# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS} ${SECURITY_CFLAGS}

INCFLAGS = -I../../h

VPATH    = ../../h

# Programs to build
PROGRAMS = dce_login

# Files to install
ILIST = dce_login

# Directory in which to install the files
IDIR = /bin/

dce_login_OFILES = dce_login.o \
                   dcesadmsg.o

# Libraries the programs need to link with
LIBS = -ldce
#LIBS = -ldce -lsec -ldce_conf -lkrb5 -ldce

dce_login_LIBS = ${LIBS} ${${TARGET_MACHINE}_LIBS}

.include <${RULES_MK}>
