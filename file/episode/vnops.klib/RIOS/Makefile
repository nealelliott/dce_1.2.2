# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.63.1  1996/10/02  17:42:50  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:35:16  damon]
#
# $EndLog$
#

CFLAGS                  += -DMOUNT_EFS=10
libkefsops.a_OFILES	+= efs_aixvmm.o efs_mount.o efs_aixmap.o efs_aixsubr.o efs_pageio.o efs_tmpcopy.o
INCFLAGS		= -I${TARGET_MACHINE} -I../vnops
CFLAGS			+= -DAFS_AIX31_VM
