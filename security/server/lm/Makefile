
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:29:58  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:19  marty]
#
# Revision 1.1.4.1  1995/12/08  17:46:44  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:19:15  root]
# 
# Revision 1.1.2.1  1994/07/15  15:00:39  mdf
# 	Hewlett Packard Security Code Drop
# 	[1994/07/14  17:18:12  mdf]
# 
# 	HP revision /main/ODESSA_2/1  1994/06/29  21:25 UTC  hondo
# 	invalid login tracking
# 
# 	HP revision /main/hondo_il3/1  1994/06/29  16:19 UTC  hondo
# 	login activity tracking.
# 
# $EndLog$

# Copyright (c) Hewlett-Packard Company 1994
# Unpublished work. All Rights Reserved.
#
# DCE Security Services
# /server/lm Makefile
#
# Additional places to search for files
VPATH = ../../idl

# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD -DRS_REP_NAUTH ${${TARGET_MACHINE}_CFLAGS} -DDCE_AUD_POINTS

INCFLAGS = -I../../h -I../../idl

LM_OBJS = lm_jrnl.o          \
        lm_login.o

# Libraries to build
LIBRARIES = liblm.a
EXPLIB_TARGETS = liblm.a

liblm.a_OFILES = ${LM_OBJS}

.include <${RULES_MK}>

