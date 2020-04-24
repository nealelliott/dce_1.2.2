#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.60.1  1996/10/02  16:56:51  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:03:20  damon]
#
# Revision 1.1.54.2  1994/06/09  13:49:38  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:23:08  annie]
# 
# Revision 1.1.54.1  1994/02/04  20:03:52  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:05:33  devsrc]
# 
# Revision 1.1.52.1  1993/12/07  17:10:50  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  12:38:40  jaffe]
# 
# Revision 1.1.4.3  1993/01/18  19:48:54  cjd
# 	Embedded copyright notice
# 	[1993/01/18  19:22:04  cjd]
# 
# Revision 1.1.4.2  1992/08/31  17:59:57  jaffe
# 	Transarc delta: jaffe-ot4923-remove-krb5-from-scout 1.1
# 	  Selected comments:
# 	    remove libkrb5 from the RIOS platform.
# 	    We no longer need RIOS_LIBS for libkrb5.
# 	[1992/08/28  20:05:22  jaffe]
# 
# Revision 1.1.2.9  1992/07/01  20:07:17  rec
# 	Turned off CC_OPT_LEVEL to test build performance on RIOS AIX 3.2.
# 	[1992/07/01  20:06:51  rec]
# 
# Revision 1.1.2.8  1992/06/30  22:24:55  mason
# 	With the STUB_OPT_LEVEL stuff, we should be able to get away with having
# 	CC_OPT_LEVEL be set now.
# 	[1992/06/30  22:10:00  mason]
# 
# Revision 1.1.2.7  1992/06/30  21:42:47  mason
# 	add STUB_OPT_LEVEL definition to help out stub compiling times.
# 	[1992/06/30  21:27:29  mason]
# 
# Revision 1.1.2.6  1992/06/30  18:52:24  rec
# 	Commented out CC_OPT_LEVEL because of AIX 3.2 compiler
# 	problems optimizing very largt stub files.  This slows
# 	down the build too much.
# 	[1992/06/30  16:11:20  rec]
# 
# Revision 1.1.2.5  1992/06/26  05:49:49  jim
# 	Merge AIX 3.2 changes with the latest tree.
# 	[1992/06/24  15:09:41  jim]
# 
# Revision 1.1.2.4  1992/06/08  22:41:41  mason
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    Most user space binaries included -lbsd.  This is an inherently
# 	    non-portable construct.  I've created a BSD_LIBS variable in the
# 	    file/ARCH/machkdep.mk directory to define the list of such libraries
# 	    (if any) for that platform. BSD_LIBS is -lbsd on this platform.
# 	[1992/06/08  20:34:49  mason]
# 	Revision 1.1.3.2  1992/06/02  16:04:48  jim
# 	Changes for AIX 3.2
# 
# Revision 1.1.2.3  1992/04/09  19:46:56  rec
# 	Added conditional to set RIOS_LIBS to -lkrb5 to be included
# 	on link line where needed.  A more correct fix to export
# 	the required symbols will be done later.  This fixes bug 2648.
# 	[1992/04/09  19:00:20  rec]
# 
# Revision 1.1.2.2  1992/04/07  19:07:47  mckeen
# 	Removed definition of COMPILE_ET. Now in osf.dce.mk.
# 	[1992/03/13  15:40:11  mckeen]
# 
# Revision 1.1  1992/01/19  02:59:28  devrcs
# 	Initial revision
# 
# $EndLog$
#

# CC_OPT_LEVEL = -O

KERNELCFLAGS = -DAFS_DEBUG -DKERNEL -D_KERNEL -D_ALL_SOURCE -DAFS_AIX32_ENV
USERCFLAGS   = -D_BSD -DAFS_DEBUG ${DCEPATHS} -D_ALL_SOURCE -DAFS_AIX32_ENV

OS_LIBS = -lbsd
STUB_OPT_LEVEL = 

# module specific local modifications
osi_KERNELCFLAGS =
