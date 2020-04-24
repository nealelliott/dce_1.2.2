#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.9.1  1996/10/02  18:49:42  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:49:41  damon]
#
# Revision 1.1.4.1  1994/06/09  14:23:27  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:35:16  annie]
# 
# Revision 1.1.2.2  1993/02/24  21:57:00  bolinger
# 	OT defect 3542 (OSF/1 volops support): make OSF/1-specific
# 	files, define symbol indicating these files are present.
# 	[1993/02/24  21:40:58  bolinger]
# 
# $EndLog$
#
VPATH			= ${TARGET_OS}

OFILES			+= osf_volops.o
CFLAGS			+= -DEXIST_MACH_VOLOPS
INCFLAGS		= -I${TARGET_OS}
