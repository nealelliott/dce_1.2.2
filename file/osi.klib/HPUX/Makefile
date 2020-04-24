#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.93.1  1996/10/02  17:56:38  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:43:05  damon]
#
# Revision 1.1.88.3  1994/07/13  22:25:03  devsrc
# 	merged with bl-10
# 	[1994/06/28  21:02:06  devsrc]
# 
# 	Added osi_caller.o to libkosi.a
# 	[1994/04/07  18:25:03  mbs]
# 
# Revision 1.1.88.2  1994/06/09  14:14:29  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:27:19  annie]
# 
# Revision 1.1.88.1  1994/02/04  20:23:42  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:15:20  devsrc]
# 
# Revision 1.1.86.2  1994/01/20  18:43:25  annie
# 	added copyright header
# 	[1994/01/20  18:39:41  annie]
# 
# Revision 1.1.86.1  1993/12/07  17:29:00  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:56:38  jaffe]
# 
# Revision 1.1.7.2  1993/07/19  19:32:23  zeliff
# 	HP port of DFS
# 	[1993/07/19  18:24:46  zeliff]
# 
# Revision 1.1.5.3  1993/07/16  20:07:30  kissel
# 	Merged with dce1.0.2a.  osi_net.c no longer builds for all platforms, but it
# 	still does build on HPUX, so add it here.
# 	[1993/06/21  14:31:51  kissel]
# 
# 	Initial GAMERA branch
# 	[1993/04/02  11:51:30  mgm]
# 
# Revision 1.1.2.2  1993/06/04  15:15:01  kissel
# 	Initial HPUX RP version.
# 	[1993/06/03  21:41:20  kissel]
# 
# Revision 1.1.3.2  1992/10/15  21:05:35  toml
# 	Initial revision for HPUX.
# 	[1992/10/14  16:48:15  toml]
# 
# $EndLog$
#

VPATH			= ../osi/${TARGET_OS}:../osi
OFILES			+= osi_port_os.o osi_os2dfserr.o osi_dfs2oserr.o \
			   osi_net.o osi_net_hpux.o osi_ufs_mach.o osi_caller.o
INCFLAGS		+= -I../osi/${TARGET_OS}
