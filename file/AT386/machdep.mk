#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.14.1  1996/10/02  16:54:58  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:03:12  damon]
#
# Revision 1.1.9.2  1994/06/09  13:49:25  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:22:53  annie]
# 
# Revision 1.1.9.1  1994/02/04  20:03:41  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:05:29  devsrc]
# 
# Revision 1.1.7.1  1994/01/28  20:43:27  annie
# 	expand OSF copyright
# 	[1994/01/28  20:42:06  annie]
# 
# 	expand OSF copyright
# 
# Revision 1.1.4.3  1993/09/22  20:53:25  tom
# 	APPEND the DFS_INCDIRS so we pick up the DCE pthread.h file.
# 	[1993/09/22  20:52:59  tom]
# 
# Revision 1.1.4.2  1993/09/22  20:27:44  tom
# 	Fix up DFS_INCDIRS to use OSF1_KERNEL_INCDIRS, set in Buildconf.
# 	[1993/09/22  20:26:28  tom]
# 
# Revision 1.1.4.1  1993/09/01  14:44:12  delgado
# 	DFS 486 port
# 	[1993/09/01  14:43:58  delgado]
# 
# $EndLog$

#
# List of directories to potentially add to the INCDIRS
#
DFS_INCDIRS := ${OSF1_KERNEL_INCDIRS}

#
# set "arglist" for parse_ext makfile
#
parse_ext_args = action_ADDEXISTS list_INCDIRS dirs_DFS_INCDIRS addhow_APPEND prefix_-I cachetag_fl_osf_1

#
# include osf.parse_ext.mk which will add any existing directories from the 
# DFS_INCDIRS list to INCDIRS
#
.include <osf.parse_ext.mk>

#CC_OPT_LEVEL	= -O 
#until top level makefiles fixed
OPT_LEVEL	= -O

KERNELCFLAGS = -DAFS_DEBUG -DCOMPAT_43 -DKERNEL -D_KERNEL \
               -D__OSF__ -DMACH -DAT386 \
	       -Wtrigraphs -Wcomment \
	       -pic-none -nostdinc -pedantic -Wpointer-arith

USERCFLAGS	= ${DCEPATHS} \
		-pedantic -Wpointer-arith -nostdinc -Wtrigraphs -Wcomment

KERNEL_LDOPTS	= -T c0100000

BSD_LIBS = -lbsd
STUB_OPT_LEVEL = -O
AT386_START	= _pstart
