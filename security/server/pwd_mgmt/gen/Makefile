# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:30:00  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:45:21  marty]
#
# Revision 1.1.4.1  1995/12/08  17:47:04  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:19:19  root]
# 
# Revision 1.1.2.2  1994/08/10  19:22:00  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:17  annie]
# 
# Revision 1.1.2.1  1994/07/15  15:00:46  mdf
# 	Hewlett Packard Security Code Drop
# 	[1994/07/14  17:18:19  mdf]
# 
# 	/main/ODESSA_2/1  1994/06/15  17:48 UTC  mullan_s
# 	Merge -from mullan_el_bl19 -to ODESSA_2
# 
# 	/main/mullan_el_bl19/1  1994/05/13  20:38 UTC  mullan_s
# 	Rules.
# 
# $EndLog$
# 
# Copyright (c) Hewlett-Packard Company 1994
# Unpublished work. All Rights Reserved.
#
#
# Compiler flags for this directory
CFLAGS  = -DOSF_DCE ${DCEPATHS} -D_BSD -DBSD ${${TARGET_MACHINE}_CFLAGS}

GEN_OBJS = gen.o
LIBRARIES = libgen.a
EXPLIB_TARGETS = libgen.a

libgen.a_OFILES = ${GEN_OBJS}

.include <${RULES_MK}>
