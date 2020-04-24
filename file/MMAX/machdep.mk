#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.69.1  1996/10/02  16:56:36  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:03:14  damon]
#
# Revision 1.1.65.2  1994/06/09  13:49:27  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:22:57  annie]
# 
# Revision 1.1.65.1  1994/02/04  20:03:45  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:05:30  devsrc]
# 
# Revision 1.1.63.1  1993/12/07  17:10:42  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/02  22:36:09  jaffe]
# 
# Revision 1.1.5.3  1993/01/18  19:48:05  cjd
# 	Embedded copyright notice
# 	[1993/01/18  19:21:13  cjd]
# 
# Revision 1.1.5.2  1992/11/24  15:13:12  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 	[1992/11/22  16:02:38  bolinger]
# 
# Revision 1.1.2.6  1992/05/20  18:30:39  garyf
# 	cleanup compilation options
# 	[1992/05/20  18:23:56  garyf]
# 
# Revision 1.1.2.5  1992/04/09  18:49:45  mckeen
# 	No longer set compiler values.  Only set OBJECT_FORMAT
# 	[1992/04/09  18:29:53  mckeen]
# 
# Revision 1.1.2.4  1992/04/07  19:07:37  mckeen
# 	Merged with mainline
# 	[1992/03/31  16:36:15  mckeen]
# 
# 	Removed definition of COMPILE_ET. Now in osf.dce.mk.
# 	[1992/03/13  15:39:54  mckeen]
# 
# Revision 1.1.2.3  1992/01/28  14:52:12  treff
# 	change to new ranlib
# 	[1992/01/28  14:51:13  treff]
# 
# 	Merging dfs5.0 to dfs5.5
# 
# Revision 1.1.2.2  1992/01/24  01:47:13  devsrc
# 	Bring in dfs6.3 sources
# 	[1992/01/22  19:28:26  devsrc]
# 
# Revision 1.1.5.3  92/01/09  13:04:05  garyf
# 	use compile_et from tools dir, not export tree
# 	[92/01/09  13:03:13  garyf]
# 
# Revision 1.1.5.2  91/12/19  12:50:44  devsrc
# 	Merging dfs5.11 to dfs6.3
# 
# Revision 1.1.4.4  91/11/16  18:57:16  garyf
# 	cleanup compilation options
# 	[91/11/16  18:56:41  garyf]
# 
# Revision 1.1.4.3  91/11/07  10:53:25  garyf
# 	add TARGET_OS and change incdirs
# 	[91/11/07  10:52:38  garyf]
# 
# Revision 1.1.4.2  91/10/31  15:55:46  devsrc
# 	Merging dfs5.5 to dfs5.11
# 
# Revision 1.1.2.3  91/07/03  09:25:47  garyf
# 	add ar and ranlib support
# 	[91/07/03  09:25:02  garyf]
# 
# Revision 1.1.2.2  91/06/03  11:24:28  garyf
# 	Multimax-specific compilation flags.  Derived from
# 	the PMAX version.
# 	[91/06/03  11:23:42  garyf]
# 
# $EndLog$

KERNEL_INCDIRS := ${KERNEL_CC:D-I${EXPORTBASE}/usr/include/kernel}

INCDIRS      := -I${EXPORTBASE}/usr/include/dcedfs ${KERNEL_INCDIRS} ${INCDIRS}

OBJECT_FORMAT=${KERNEL_CC:UMACHO:DCOFF}

#CC_OPT_LEVEL = -O 
#until top level makefiles fixed
OPT_LEVEL = -O

KERNELCFLAGS = -DAFS_DEBUG -DKERNEL -D_KERNEL \
               -traditional -D__OSF__ -DMACH \
	       -mregparm -Dvolatile=__volatile__ -Dsigned=__signed__

USERCFLAGS   = -DMACH

KERNEL_LDOPTS	= ${MAKETOP}/${OBJECTDIR}/kernel/${CONFIG}/memconf.mmax

OS_LIBS=

# module specific local modifications
osi_KERNELCFLAGS =
