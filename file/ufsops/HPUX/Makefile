# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# @HP_COPYRIGHT@
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.15.1  1996/10/02  18:49:34  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:49:33  damon]
#
# Revision 1.1.10.2  1994/06/09  14:23:22  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:35:09  annie]
# 
# Revision 1.1.10.1  1994/02/04  20:33:42  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:19:28  devsrc]
# 
# Revision 1.1.8.1  1994/01/28  20:43:50  annie
# 	expand OSF copyright
# 	[1994/01/28  20:42:24  annie]
# 
# Revision 1.1.6.2  1993/07/19  19:36:17  zeliff
# 	HP port of DFS
# 	[1993/07/19  18:27:18  zeliff]
# 
# Revision 1.1.4.3  1993/07/16  20:47:14  kissel
# 	*** empty log message ***
# 	[1993/06/21  14:40:49  kissel]
# 
# 	Initial creation (modelled after the OSF1 version).
# 	[1993/04/12  18:22:15  toml]
# 
# Revision 1.1.2.2  1993/06/04  21:57:49  kissel
# 	Initial HPUX RP version.
# 	[1993/06/04  21:36:04  kissel]
# 
# $EndLog$
#
VPATH			= ${TARGET_OS}

OFILES			+= hpux_volops.o
CFLAGS			+= -DEXIST_MACH_VOLOPS +e
INCFLAGS		= -I${TARGET_OS}
