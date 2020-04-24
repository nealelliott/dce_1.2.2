# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.969.1  1996/10/02  17:52:37  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:41:13  damon]
#
# Revision 1.1.964.3  1994/06/09  14:11:53  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:25:26  annie]
# 
# Revision 1.1.964.2  1994/02/04  20:21:23  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:14:21  devsrc]
# 
# Revision 1.1.96.1  1993/12/07  17:27:06  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:40:01  jaffe]
# 
# Revision 1.1.6.5  1993/03/11  16:25:48  delgado
# 	Build krpc_dbg.o for OSF1; this lets krpc debugging
# 	use the dfs icl package.
# 	[1993/03/10  22:46:36  delgado]
# 
# Revision 1.1.6.4  1993/03/09  19:15:31  jaffe
# 	Transarc delta: kazar-ot7223-use-new-rpc-features 1.1
# 	  Selected comments:
# 	    use new funs for controlling qlength for thread pools, and max # of calls
# 	    move krpc_pool to dfs core, so can be called by tkc
# 	[1993/03/09  17:28:33  jaffe]
# 
# Revision 1.1.6.3  1993/01/19  16:05:33  cjd
# 	embedded copyright notice
# 	[1993/01/19  14:15:42  cjd]
# 
# Revision 1.1.6.2  1993/01/13  18:09:03  shl
# 	Transarc delta: kazar-ot6175-bound-cm-initiated-calls 1.4
# 	  Selected comments:
# 	    limit the # of concurrent calls because the RPC deadlocks when
# 	    too many concurrent calls are active.  Also, right now, the RPC
# 	    performs *terribly* when too many concurrent calls are active, due
# 	    to apparent bugs in packet rationing.
# 	    add krpc_pool.c compilation
# 	    hung quickly, due to missing call release on exit path
# 	Transarc delta: kazar-ot6523-uuid-encodes-primary-secondary-pool-selection 1.3
# 	  Selected comments:
# 	    some changes to prevent deadlock between primary/secondary thread pools
# 	    Changes basic connection protocol.
# 	    add new file for uuid hacking
# 	    fix confusion about different types of sockaddr_ins
# 	[1993/01/12  21:21:19  shl]
# 
# Revision 1.1.3.6  1992/06/08  22:45:42  mason
# 	Transarc delta: cburnett-ot3873-setup-syscalls-in-dfscorecfg 1.1
# 	  Selected comments:
# 	    The dfs_core_config function which performs initialization for
# 	    the dfscore kernel extension should setup the the syscalls supported
# 	    by the extension instead of counting on them being statically setup
# 	    in syscall.c
# 	    The -DCOMMON is no longer needed
# 	[1992/06/08  21:39:11  mason]
# 
# Revision 1.1.3.5  1992/02/03  19:40:07  toml
# 	Add .include for $TARGET_OS and $TARGET_MACHINE Makefile's.
# 	[1992/02/03  19:39:33  toml]
# 
# Revision 1.1.3.4  1992/01/30  13:46:25  carl
# 	Fixed defines for common Makefile.
# 	[1992/01/30  13:45:58  carl]
# 
# Revision 1.1.3.3  1992/01/24  03:45:48  devsrc
# 	Merging dfs6.3
# 	[1992/01/24  00:14:00  devsrc]
# 
# Revision 1.1.3.2  1992/01/23  19:50:27  zeliff
# 	Moving files onto the branch for dfs6.3 merge
# 	[1992/01/23  18:34:51  zeliff]
# 	Revision 1.1.1.3  1992/01/23  22:17:41  devsrc
# 	Fixed logs
# 
# $EndLog$

VPATH			= ../kutils

LIBRARIES		= libkdfskutil.a libkdfsncs.a

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

KERNEL_CC		=

CFLAGS			= ${KERNELCFLAGS} ${DFS_CORE}
krpc_misc.o_CFLAGS	= ${KERNELCFLAGS}
uuidutils.o_CFLAGS	= ${KERNELCFLAGS}

libkdfskutil.a_OFILES	= syscall.o com_lockctl.o krpc_pool.o \
			  kutil_linkage.o ${${TARGET_OS}_OBJS}
libkdfsncs.a_OFILES	= krpc_misc.o uuidutils.o
INCFLAGS		= -I../kutils

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.if exists (${TARGET_MACHINE}/Makefile)
.include "${TARGET_MACHINE}/Makefile"
.endif

.if exists (${TARGET_OS}/Makefile)
.include "${TARGET_OS}/Makefile"
.endif

.include <${RULES_MK}>
