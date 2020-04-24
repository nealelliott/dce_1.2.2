# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.14.1  1996/10/02  17:56:45  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:43:07  damon]
#
# Revision 1.1.9.1  1994/06/09  14:14:32  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:27:21  annie]
# 
# Revision 1.1.7.2  1993/05/12  14:09:50  delgado
# 	Don't use osi/osi_net.c for OSF1; use osi_net_osf.c
# 	[1993/05/12  13:58:39  delgado]
# 
# Revision 1.1.4.5  1993/01/21  14:49:50  cjd
# 	embedded copyright notice
# 	[1993/01/20  14:51:26  cjd]
# 
# Revision 1.1.4.4  1993/01/13  18:17:22  shl
# 	Transarc delta: kazar-ot6437-make-init-parent 1.1
# 	  Selected comments:
# 	    exiting threads weren't being wait3'd, and turned into zombies
# 	    add new file containing dummy for osi_MakeInitChild
# 	[1993/01/12  21:40:42  shl]
# 
# Revision 1.1.4.3  1992/10/02  21:21:37  toml
# 	Removed osi_user_newProc.o
# 	[1992/10/02  19:34:07  toml]
# 
# Revision 1.1.4.2  1992/08/31  20:16:12  jaffe
# 	Transarc delta: bab-ot2604-tkm-gc-impl 1.27
# 	  Selected comments:
# 	    Had to make another checkpoint so I could import another delta.
# 	    Checkpoint version that passes basic user-space (non-regression - no real
# 	    exercise of the gc, yet) tests.
# 	    Removed subset bug fix in order to put it into the re-org delta.
# 	    NEVER IMPORT THIS VERSION OF THIS DELTA!
# 	    Re-establish the token subset bug fix in this delta.
# 	    Need to import another delta.
# 	    Another check-point version, this one in preparation for the one that
# 	    removes tkm_Alloc.  This version passes basic user-space tests of garbage
# 	    collection and asynchronous revocation.
# 	    Another check-point.  This one removes tkm_Alloc (and, hence,
# 	    tkm_memory.[ch]) from the token manager, since it doesn't really gain us
# 	    that much, especially now that token cells are managed as an internal list.
# 	    Another buildable checkpoint that passes basic periodic gc tests.
# 	    Implemented a routine to be used as a "try waiting tokens" thread,
# 	    along the same lines as the garbage collection thread.
# 	    Add exception catching (if activated) to periodic try waiting routine,
# 	    and make tkm panic/exit if it can't get memory from osi_Alloc;
# 	    Fix stupid compilation bug left in in debugging a couple of versions ago.
# 	    Implementation of token manager control afs_syscall.
# 	    (Untested check-point...probably doesn't even compile)
# 	    Had to reopen to fix compilation errors that stopped the other testing.
# 	    Removed the token manager syscall routine from user-space compilation.
# 	    This version of this delta contains support for the token manager's
# 	    periodic activites thread (not yet activated) and for the afs_syscall
# 	    that will query and set various parameters that control the token manager.
# 	    Put ifdefs around prototype for routine that's only needed if testing without
# 	    other tsr deltas.
# 	    Check-point so that I can import another delta.
# 	    Support for token manager periodic activities thread, from within
# 	    the core kernel extension.
# 	    Build object file containing osi_NewProc() in kernel-space
# 	    and include it in the appropriate library.
# 	    Allow client routines to return (exit the thread) without
# 	    getting error messages.  Also changed the type of the thread base
# 	    routine to void.
# 	    Changed interface to osi_NewProc to support parameterization of
# 	    grabbing of kernel lock (on RIOS....blocking & restoration of kernel
# 	    preemption, in general).
# 	    Correct typo.
# 	    Set up for debugging.
# 	    Sometimes, the token manager helper thread would get an illegal trap
# 	    instruction on a rios machine (which Carl tells me is what assert
# 	    failures become when the code is shipped) on trying to pause (via
# 	    osi_Pause/osi_Wait). The problem is that the timer blocks in the kernel
# 	    had not been initialized by the time the token manager thread tried to run.
# 	    This version of this delta fixes the problem by making sure the timer
# 	    blocks are initialized in time.
# 	    Removed debugging control from testing of gc/async grant thread.
# 	    Debugged problems with tkm not identifying potential candidates if the
# 	    testing goes too fast (the reference times get too close together),
# 	    or tkm tries to gc a fid on which it already holds the lock.
# 	    Changed name of TakeBackTokens routine to reflect its new placement.
# 	[1992/08/30  03:08:10  jaffe]
# 
# Revision 1.1.2.4  1992/04/21  13:49:42  mason
# 	Transarc delta: tu-1143-error-mapping 1.8
# 	  Files modified:
# 	    cm: cm.h, cm_cell.c, cm_conn.c, cm_daemons.c, cm_dcache.c
# 	    cm: cm_dnamehash.c, cm_init.c, cm_pioctl.c, cm_rrequest.c
# 	    cm: cm_scache.c, cm_server.c, cm_subr.c, cm_tokens.c
# 	    cm: cm_vnodeops.c, cm_volume.c; fsprobe: fsprobe.c
# 	    libafs/RIOS: dfscore.exp; osi: Makefile, osi_dfserrors.h
# 	    osi.klib/OSF1: Makefile; osi.klib/RIOS: Makefile
# 	    osi/OSF1: Makefile, osi_dfs2osferr.c, osi_osf2dfserr.c
# 	    osi/RIOS: Makefile, osi_aix2dfserr.c, osi_dfs2aixerr.c
# 	    px: px_init.c, px_intops.c; rep: rep_main.c
# 	    userInt/fts: volc_tokens.c
# 	  Selected comments:
# 	    An enhancement to allow the DFS server to map returned kernel errors to the
# 	    canonical ones and to allow the CM to map the canonical ones to the
# 	    appropriate local kernel errors.
# 	    Please refer to the design memo, Error Conversion Protocol, for more detail.
# 	    Added osi_osf2dfserr.o and osi_dfs2osferr.o to the Makefile
# 	    The same as above.
# 	    Made osi_dfserrors.h and osi_dfs2aixerr.c osi_aix2dfserr.c osi_osf2dfserr.c and
# 	    osi_dfs2osferr.c avaliable in the user space. So, whoever references AFS_xxx
# 	    APIs should use the error conversion protocol.
# 	    fixed some typo errors and others
# 	    Changes did not compile on the PMAX.
# 	    DFS servers now maps errors to the canonical ones before returning to theirs
# 	    clients. Therefore, those clients who use AFS_xxx rpc calls must do the
# 	    error decoding work to get the appropriate error back.
# 	    CFE adds osi_initDecodeTable() and osi_errDecode() to repserver and fts.
# 	[1992/04/20  22:42:50  mason]
# 
# Revision 1.1.2.3  1992/04/14  04:09:26  mason
# 	Transarc delta: tu-1143-error-mapping 1.6
# 	  Files modified:
# 	    cm: cm.h, cm_cell.c, cm_conn.c, cm_daemons.c, cm_dcache.c
# 	    cm: cm_dnamehash.c, cm_init.c, cm_pioctl.c, cm_rrequest.c
# 	    cm: cm_scache.c, cm_server.c, cm_subr.c, cm_tokens.c
# 	    cm: cm_vnodeops.c, cm_volume.c; libafs/RIOS: dfscore.exp
# 	    osi: Makefile, osi_dfserrors.h; osi.klib/OSF1: Makefile
# 	    osi.klib/RIOS: Makefile
# 	    osi/OSF1: Makefile, osi_dfs2osferr.c, osi_osf2dfserr.c
# 	    osi/RIOS: Makefile, osi_aix2dfserr.c, osi_dfs2aixerr.c
# 	    px: px_init.c, px_intops.c
# 	  Selected comments:
# 	    An enhancement to allow the DFS server to map returned kernel errors to the
# 	    canonical ones and to allow the CM to map the canonical ones to the
# 	    appropriate local kernel errors.
# 	    Please refer to the design memo, Error Conversion Protocol, for more detail.
# 	    Added osi_osf2dfserr.o and osi_dfs2osferr.o to the Makefile
# 	    The same as above.
# 	    Made osi_dfserrors.h and osi_dfs2aixerr.c osi_aix2dfserr.c osi_osf2dfserr.c and
# 	    osi_dfs2osferr.c avaliable in the user space. So, whoever references AFS_xxx
# 	    APIs should use the error conversion protocol.
# 	    fixed some typo errors and others
# 	    Changes did not compile on the PMAX.
# 	[1992/04/13  17:00:31  mason]
# 
# Revision 1.1.2.2  1992/01/24  03:46:37  devsrc
# 	Fixed logs
# 	[1992/01/23  22:19:00  devsrc]
# 
# $EndLog$


VPATH			= ../osi/${TARGET_OS}:../osi
OFILES			+= osi_port_osf.o osi_osf2dfserr.o osi_dfs2osferr.o \
			   osi_ufs_mach.o osi_procutils.o osi_net_osf.o
INCFLAGS		+= -I../osi/${TARGET_OS}
