# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.83.1  1996/10/02  17:57:16  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:43:22  damon]
#
# Revision 1.1.78.3  1994/07/13  22:05:28  devsrc
# 	merged with bl-10
# 	[1994/06/28  17:02:42  devsrc]
# 
# 	Added osi_caller.o into libosi.a.
# 	[1994/04/04  17:48:47  mbs]
# 
# Revision 1.1.78.2  1994/06/09  14:14:56  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:27:33  annie]
# 
# Revision 1.1.78.1  1994/02/04  20:23:48  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:15:21  devsrc]
# 
# Revision 1.1.76.2  1994/01/20  18:43:26  annie
# 	added copyright header
# 	[1994/01/20  18:39:42  annie]
# 
# Revision 1.1.76.1  1993/12/07  17:29:20  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:57:11  jaffe]
# 
# Revision 1.1.6.2  1993/07/19  19:32:32  zeliff
# 	HP port of DFS
# 	[1993/07/19  18:24:57  zeliff]
# 
# Revision 1.1.4.3  1993/07/16  20:07:52  kissel
# 	*** empty log message ***
# 	[1993/06/21  14:32:08  kissel]
# 
# 	Initial coding for HPUX.
# 	[1992/10/14  16:49:30  toml]
# 
# 	Initial GAMERA branch
# 	[1993/04/02  11:52:12  mgm]
# 
# Revision 1.1.2.2  1993/06/04  15:08:27  kissel
# 	Initial HPUX RP version.
# 	[1993/06/03  21:41:40  kissel]
# 
# $EndLog$

VPATH		= ${TARGET_OS}
INCLUDES	+= osi_port_mach.h sysincludes.h osi_wait.h \
		osi_intr_mach.h osi_kvnode_mach.h osi_uio_mach.h \
		osi_cred_mach.h osi_user_mach.h osi_net_mach.h \
		osi_lock_mach.h osi_buf_mach.h osi_ufs_mach.h \
		osi_vmm_mach.h osi_sysconfig_mach.h osi_device_mach.h

libosi.a_OFILES	+= osi_dfs2oserr.o osi_os2dfserr.o osi_port_os.o \
                osi_net_hpux.o osi_caller.o
