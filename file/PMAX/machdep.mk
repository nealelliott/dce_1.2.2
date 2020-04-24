#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.19.1  1996/10/02  16:56:49  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:03:19  damon]
#
# Revision 1.1.13.2  1994/06/09  13:49:37  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:23:07  annie]
# 
# Revision 1.1.13.1  1994/02/04  20:03:50  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:05:32  devsrc]
# 
# Revision 1.1.11.1  1993/12/07  17:10:48  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/02  22:37:23  jaffe]
# 
# Revision 1.1.5.6  1993/01/18  19:48:45  cjd
# 	Embedded copyright notice
# 	[1993/01/18  19:21:56  cjd]
# 
# Revision 1.1.5.5  1993/01/13  15:22:25  shl
# 	Transarc delta: jaffe-cleanup-2.3-compiler-warnings 1.8
# 	  Selected comments:
# 	    Fix compiler warnings found when moving to OSF/1 1.1.1.
# 	    when computing DFS_INCDIRS, prepend only those directories which
# 	    actually changed.
# 	    Remove the function dacl_SetOwnerControl.  It is not used.
# 	[1993/01/12  16:30:11  shl]
# 
# Revision 1.1.5.4  1992/12/10  22:26:42  marty
# 	Upgrade DFS to OSF/1 1.1.1.  [OT defect 5236]
# 	[1992/12/10  21:55:04  marty]
# 
# 	cleanup compilation options
# 	[91/11/16  18:55:48  garyf]
# 
# 	Merging dfs5.5 to dfs5.11
# 
# Revision 1.1.5.3  1992/11/24  15:17:42  bolinger
# 	*** empty log message ***
# 	Revision 1.1.7.2  1992/11/30  22:53:36  marty
# 	OSF 1.1.1 port
# 
# Revision 1.1.8.2  1992/11/22  16:03:28  bolinger
# 	Change include file install directory from .../afs to .../dcedfs.
# 
# Revision 1.1.6.2  1992/09/30  16:33:26  garyf
# 	Merge up with DCE1_0_2 (post SVR4).
# 	[1992/09/30  16:29:23  garyf]
# 
# Revision 1.1.5.2  1992/08/31  17:59:51  jaffe
# 	Transarc delta: jess-ot3957-fix-PMAX_machdep.mk 1.1
# 	  Selected comments:
# 	    After the deletion of local definitions of DCELOCAL_PATH, the build
# 	    for pmax does not work. It needs ${DCEPATHS} defined in USERCFLAGS
# 	    in the file machdep.mk.
# 	    Add DCEPATHS flag in USERCFLAG in the file machdep.mk of PMAX.
# 	[1992/08/28  20:04:40  jaffe]
# 
# Revision 1.1.2.13  1992/06/30  21:42:40  mason
# 	add STUB_OPT_LEVEL variable to control optimization of stub files.
# 	[1992/06/30  21:27:08  mason]
# 
# Revision 1.1.2.12  1992/06/08  22:41:34  mason
# 	Transarc delta: mason-ot4102-fix-libbsd-in-Makefiles 1.1
# 	  Selected comments:
# 	    Most user space binaries included -lbsd.  This is an inherently
# 	    non-portable construct.  I've created a BSD_LIBS variable in the
# 	    file/ARCH/machkdep.mk directory to define the list of such libraries
# 	    (if any) for that platform.  BSD_LIBS is -lbsd on this platform.
# 	[1992/06/08  20:33:49  mason]
# 
# Revision 1.1.2.11  1992/06/02  19:18:39  garyf
# 	add -DCOMPAT_43
# 	[1992/06/02  19:09:09  garyf]
# 
# Revision 1.1.2.10  1992/05/22  20:06:53  garyf
# 	cleanup:
# 	1) cleanup compilation flags
# 	2) add DCEPATHS
# 	3) remove -I for usr/include/dcedfs
# 	[1992/05/22  03:02:42  garyf]
# 
# Revision 1.1.2.9  1992/05/20  18:30:42  garyf
# 	change OSF compilation option
# 	[1992/05/20  18:24:01  garyf]
# 
# Revision 1.1.2.8  1992/05/20  17:17:20  mhickey
# 	Set the INCDIRS line differently depending on whether we are building
# 	for KERNEL_CC or not.  Add parse_ext_args option to reset the INCDIRS
# 	line on the THIRD pass since we mant to pick up any include directories
# 	created curing the FIRST pass.
# 
# Revision 1.1.2.7  1992/05/18  21:10:35  mhickey
# 	Turned on caching for the call to parse_ext to save
# 	time.
# 
# 	Defect 3370.
# 	[1992/05/18  21:09:21  mhickey]
# 
# Revision 1.1.2.6  1992/05/11  20:11:12  mhickey
# 	Changed the addition of the explicit path to the sandbox export area
# 	for afs and kernel includes to an addition of afs and kernel includes
# 	from any existing directories in any backing trees as well as the
# 	export area of the sandbox.
# 	Defect 3370.
# 	[1992/05/11  20:10:40  mhickey]
# 
# Revision 1.1.2.5  1992/04/09  18:49:49  mckeen
# 	No longer set compiler values.  Only set OBJECT_FORMAT
# 	[1992/04/09  18:30:01  mckeen]
# 
# Revision 1.1.2.4  1992/04/07  19:07:40  mckeen
# 	Merged with mainline
# 	[1992/03/31  16:36:22  mckeen]
# 
# 	Removed definition of COMPILE_ET. Now in osf.dce.mk.
# 	[1992/03/13  15:40:02  mckeen]
# 
# Revision 1.1.2.3  1992/01/28  14:52:15  treff
# 	change to new ranlib
# 	[1992/01/28  14:51:18  treff]
# 
# 	add KERN_LDOPTS
# 	[91/05/08  16:20:44  garyf]
# 
# 	Merging AFS to dce.38
# 
# Revision 1.1.2.2  1992/01/24  01:47:42  devsrc
# 	Bring in dfs6.3 sources
# 	[1992/01/22  19:28:52  devsrc]
# 
# Revision 1.1.8.3  91/12/22  17:07:36  treff
# 	12/22/91 DCT @ OSF
# 	Make COMPILE_ET point to the one in tools,
# 	not the one in export . . .
# 	[91/12/22  17:06:56  treff]
# 
# Revision 1.1.8.2  91/12/19  12:51:24  devsrc
# 	Merging dfs5.11 to dfs6.3
# 
# Revision 1.1.6.3  91/10/26  16:33:36  garyf
# 	add -I for new kernel include dir
# 	add TARGET_OS so it doesn't have to be in rc_files
# 	[91/10/26  16:33:05  garyf]
# 
# Revision 1.1.6.2  91/08/20  14:03:22  devsrc
# 	Merging dfs5.0 to dfs5.5
# 
# Revision 1.1.2.8  91/04/19  09:47:34  garyf
# 	remove DCE_AFS in options
# 	[91/04/19  09:47:01  garyf]
# 
# Revision 1.1.2.7  91/04/04  17:11:45  garyf
# 	add optimization
# 	[91/04/04  17:11:19  garyf]
# 
# Revision 1.1.2.6  91/03/18  09:41:07  garyf
# 	turn debug off, add AFS_COMMON
# 	test KERNEL_CC
# 	[91/03/18  09:40:01  garyf]
# 
# Revision 1.1.2.5  91/03/11  10:11:23  garyf
# 	undo previous; move into separate file
# 	[91/03/11  10:10:23  garyf]
# 
# Revision 1.1.2.4  91/03/07  16:21:30  garyf
# 	add rules to use correct kernel compiler
# 	[91/03/07  16:20:27  garyf]
# 
# Revision 1.1.2.3  91/03/04  14:11:49  garyf
# 	define DCE_AFS to allow setting of ni_lookup
# 	[91/03/04  14:11:22  garyf]
# 
# Revision 1.1.2.2  90/12/10  14:26:06  garyf
# 	new for OSF
# 	[90/12/10  14:25:32  garyf]
# 
# $EndLog$

#
# List of directories to potentially add to the INCDIRS
#
DFS_INCDIRS := ${KERNEL_CC:D${INCDIRS:S;${target_machine}/usr/include;&/kernel;g:M*/usr/include/kernel:S/^-I//g}}

#
# set "arglist" for parse_ext makfile
#
.if defined(KERNEL_CC)
parse_ext_args = action_ADDEXISTS list_INCDIRS dirs_DFS_INCDIRS addhow_PREPEND prefix_-I cachetag_fl_pmx_mk1
.else
parse_ext_args = action_ADDEXISTS list_INCDIRS dirs_DFS_INCDIRS addhow_PREPEND prefix_-I cachetag_fl_pmx_mk2 cacheload_THIRD
.endif
#
# include osf.parse_ext.mk which will add any existing directories from the 
# DFS_INCDIRS list to INCDIRS
#
.include <osf.parse_ext.mk>

#CC_OPT_LEVEL	= -O 
#until top level makefiles fixed
OPT_LEVEL	= -O

KERNELCFLAGS = -DAFS_DEBUG -DCOMPAT_43 -DKERNEL -D_KERNEL \
               -D__OSF__ -DMACH -DPMAX \
	       -G 0 -fomit-frame-pointer -Wtrigraphs -Wcomment \
	       -pic-none -nostdinc -pedantic -Wpointer-arith

USERCFLAGS	= ${DCEPATHS} \
		-G 0 -pedantic -Wpointer-arith -nostdinc -Wtrigraphs -Wcomment

KERNEL_LDOPTS	= -T 80030000

# the following are a copy of the old options used for OSF/1 R1.0.4
#KERNELCFLAGS = -DCOMPAT_43 -DAFS_DEBUG -DKERNEL -D_KERNEL \
#              -traditional -D__OSF__ -DMACH \
#	       -Dvolatile=__volatile__ -G 0

#USERCFLAGS	= ${DCEPATHS} \
#		-G 0

#KERNEL_LDOPTS	= -G 18 -N -T 80030000

OS_LIBS = -lbsd
STUB_OPT_LEVEL = 

# module specific local modifications
osi_KERNELCFLAGS =
