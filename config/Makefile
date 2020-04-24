# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.19.2  1996/03/09  23:09:33  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:27:37  marty]
#
# Revision 1.2.19.1  1995/12/07  21:39:17  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  20:56:42  root]
# 
# Revision 1.2.17.4  1994/08/08  14:23:04  mdf
# 	made modification requested by OSF
# 	[1994/08/05  17:07:21  mdf]
# 
# 	Hewlett Packard Security Code Drop
# 	[1994/08/04  19:55:53  mdf]
# 
# Revision 1.2.17.3  1994/06/21  16:55:24  tom
# 	Change dce.clean to dce_shutdown.
# 	[1994/06/21  16:44:30  tom]
# 
# Revision 1.2.17.2  1994/05/31  14:52:29  rsalz
# 	Nothing but scripts here now (OT CR 9663).
# 	[1994/05/31  13:08:38  rsalz]
# 
# Revision 1.2.17.1  1993/12/29  19:21:53  rsalz
# 	Remove dce_cf_XXX routines (OT CR 9663).
# 	[1993/12/29  14:57:43  rsalz]
# 
# Revision 1.2.15.2  1993/07/19  19:25:18  zeliff
# 	HP port of DFS
# 	[1993/07/19  18:19:33  zeliff]
# 
# Revision 1.2.13.4  1993/07/16  22:36:57  kissel
# 	removed dce.login
# 	[1993/07/15  14:31:56  jake]
# 
# 	*** empty log message ***
# 	[1993/06/21  14:08:06  kissel]
# 
# Revision 1.2.11.2  1993/06/04  01:06:07  kissel
# 	Initial HPUX RP version.
# 	[1993/06/03  18:59:33  kissel]
# 
# Revision 1.1.7.4  1993/04/29  12:48:44  mgm
# 	Added dce.login so fpkg will not fail
# 	[1993/04/29  12:48:22  mgm]
# 
# Revision 1.1.7.3  1993/04/28  12:29:00  jake
# 	Added new scripts to Makefile
# 	[1993/04/28  12:14:28  jake]
# 
# Revision 1.1.7.2  1993/04/21  20:02:33  mgm
# 	Initial GAMERA branch
# 
# Revision 1.1.1.12  1993/04/21  14:15:24  root
# 	    Loading drop DCE1_0_2B23
# 
# Revision 1.2.6.3  1992/12/06  03:05:18  zeliff
# 	Embedding copyrights
# 	[1992/12/05  22:56:46  zeliff]
# 
# Revision 1.2.6.2  1992/10/01  19:50:08  rsarbo
# 	install rc.dfs
# 	[1992/10/01  19:46:21  rsarbo]
# 
# Revision 1.2.3.3  1992/06/02  20:06:28  melman
# 	Added dce.rm to SCRIPTS.  SCRIPTS generated rules for .sh and .csh
# 	files, but in this Makefile it is also used as part of ILIST.
# 	Also added dce.rm_IDIR = /etc/
# 	[1992/06/02  20:01:49  melman]
# 
# Revision 1.2.3.2  1992/05/11  17:49:55  mhickey
# 	Removed obsolete NEED_PROJECT_MAKE_DEFS lines -- defect 2031
# 	[1992/05/08  19:49:07  mhickey]
# 
# Revision 1.2.1.2  1992/04/22  15:21:05  mhickey
# 	Removed obsolete define of NEED_PROJECT_MAKE_DEFS
# 	and include of osf.dce.mk.  Fixes defect 2031.
# 
# Revision 1.2  1992/01/19  22:12:48  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#
#
#  OSF DCE Version 1.0 
# 
# Makefile for the configuration library
#
#
# 11-04-91; mhickey: Modified to build either shared objects for libdce.so or
#                    an archive library.
#
#
# 10-22-91; mhickey: Modified for the new build model for libdce.a.  This means
#                    that the OFILES list is seperated from the rest of the
#                    file by an 'if' statement, and any
#                    ${${TARGET_MACHINE}_OFILES} specifications are changed to
#                    ${${TARGET_MACHINE}_<libname>_OFILES}.
#

SUBDIRS		= util
OBJECTS		=
LIBS		=
SCRIPTS         = \
	rc.dce rc.dfs dce_shutdown dce_config dce.rm dce.unconfig dce_com_env \
	dce_com_utils dce_config_utils dce_config_env dfs.clean dfs.rm \
	dfs.unconfig dfs_config pwd_config
ILIST		= ${SCRIPTS}
IDIR		= /etc/

.include <${RULES_MK}>
