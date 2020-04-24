#
# @(#)27	1.1  src/file/ufsops/RIOS/Makefile, dfs.src, dce12  1/7/93  19:12:48
#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.11.1  1996/10/02  18:50:02  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:49:44  damon]
#
# Revision 1.1.6.2  1994/06/09  14:23:32  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:35:20  annie]
# 
# Revision 1.1.6.1  1994/02/04  20:33:52  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:19:33  devsrc]
# 
# Revision 1.1.4.1  1994/01/28  20:43:52  annie
# 	expand OSF copyright
# 	[1994/01/28  20:42:25  annie]
# 
# Revision 1.1.2.2  1993/01/28  18:14:46  shl
# 	Transarc delta: cburnett-ot6781-aix-jfs-dump-restore-support 1.3
# 	  Selected comments:
# 	    Add AIX JFS support to ufsops for the new portable directory
# 	    dump/restore format.  Also fix some existing bugs in the
# 	    ufsops code.  In addition push AIX specific function into
# 	    a platform specific dir.
# 	    AIX specific Makefile.
# 	    The first revision of delta didn't compile under OSF/1.
# 	    We need one more round to get this to compile under OSF/1.
# 	[1993/01/27  21:05:41  shl]
# 
# Revision 1.1  1992/01/19  02:57:34  devrcs
# 	Initial revision
# 
# $EndLog$
#
VPATH			= ${TARGET_MACHINE}

OFILES			+= aix_volops.o
CFLAGS			+= -DEXIST_MACH_VOLOPS
INCFLAGS		= -I${TARGET_MACHINE}
