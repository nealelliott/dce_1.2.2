# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
# All Rights Reserved
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.12.2  1996/03/09  23:06:08  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:24:58  marty]
#
# Revision 1.1.12.1  1995/12/08  00:09:44  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:53:00  root]
# 
# Revision 1.1.10.2  1994/01/14  19:25:55  ohara
# 	change the build line to use var defined in src/Makefile - more generic
# 	[1994/01/14  19:22:55  ohara]
# 
# Revision 1.1.10.1  1994/01/04  19:41:34  ohara
# 	fix the "SUBDIRS" to reflect 1.1 tree
# 	[1994/01/04  19:40:54  ohara]
# 
# Revision 1.1.8.1  1993/10/15  21:19:00  ohara
# 	remove catman from SUBDIRS (8596)
# 	[1993/10/15  16:20:30  ohara]
# 
# Revision 1.1.6.2  1993/06/24  19:44:42  hinman
# 	[hinman@sni] - Fix merge error
# 	[1993/06/17  17:56:21  hinman]
# 
# Revision 1.1.3.3  1992/12/06  03:02:36  zeliff
# 	Embedding Copyrights
# 	[1992/12/06  03:00:31  zeliff]
# 
# Revision 1.1.3.2  1992/09/29  18:42:36  devsrc
# 	New file for SVR4 OSF merge
# 	[1992/09/11  15:38:12  rec]
# 
# $EndLog$
# 

# 
# This file overrides the directory lists in src/Makefile for SVR4
#
SETUP_SUBDIRS	= tools dce custom time security rpc directory 
EXPINC_SUBDIRS	:= custom ${EXPINC_SUBDIRS}
EXPLIB_SUBDIRS	:= custom ${EXPLIB_SUBDIRS}
SUBDIRS		:= ${SUBDIRS} custom 
