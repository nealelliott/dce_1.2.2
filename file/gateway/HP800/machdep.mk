# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.4.1  1996/10/02  17:49:18  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:38:54  damon]
#
# Revision 1.1.2.2  1994/10/06  20:26:33  agd
# 	expand copyright
# 	[1994/10/06  14:19:37  agd]
# 
# Revision 1.1.2.1  1994/08/08  18:54:26  mckeen
# 	Initial version
# 	[1994/08/08  17:46:28  mckeen]
# 
# $EndLog$

.ifdef DFS_KERNEL_ONLY
SUBDIRS		= libgwauth.klib
.else
SUBDIRS		= libgwauth libgwauth.klib dfsgw dfsgwd
.endif

EXPINC_SUBDIRS	= libgwauth

EXPLIB_SUBDIRS	= libgwauth.klib

SAMIDL_SUBDIRS  = dfsgwd dfsgw
