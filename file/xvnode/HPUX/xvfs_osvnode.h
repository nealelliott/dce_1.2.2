/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xvfs_osvnode.h,v $
 * Revision 1.1.716.1  1996/10/02  19:03:32  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:57  damon]
 *
 * Revision 1.1.711.2  1994/06/09  14:25:58  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:37:54  annie]
 * 
 * Revision 1.1.711.1  1994/02/04  20:36:49  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:20:52  devsrc]
 * 
 * Revision 1.1.709.1  1993/12/07  17:38:43  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  18:18:45  jaffe]
 * 
 * Revision 1.1.6.2  1993/07/19  19:37:48  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:28:13  zeliff]
 * 
 * Revision 1.1.4.3  1993/07/16  21:00:57  kissel
 * 	*** empty log message ***
 * 	[1993/06/21  14:44:29  kissel]
 * 
 * 	afs -> dcedfs
 * 	[1993/01/14  19:43:20  toml]
 * 
 * 	Initial revision for HPUX.
 * 	[1992/10/14  17:11:03  toml]
 * 
 * 	Correct VOPN_PAGEIN, again.
 * 	[1993/05/06  22:11:37  sommerfeld]
 * 
 * 	Correct VOPN_PAGEIN for HP-UX, just in case we ever need it.
 * 	[1993/05/04  20:18:19  sommerfeld]
 * 
 * 	Initial GAMERA branch
 * 	[1993/04/02  12:00:07  mgm]
 * 
 * Revision 1.1.2.2  1993/06/04  15:47:04  kissel
 * 	Initial HPUX RP version.
 * 	[1993/06/03  21:56:51  kissel]
 * 
 * $EndLog$
 */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <sys/vnode.h>
#include <sys/vfs.h>

/*
 * Macros to the GLUED HP-UX vnodeops
 */
#define VOPO_OPEN(VPP,F,C) \
    (*((struct xvfs_vnodeops *)((*(VPP))->v_op))->oops.vn_open)(VPP, F, C)
#define VOPO_CLOSE(VP, F, C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_close)(VP, F, C)
#define VOPO_RDWR(VP,UIOP,RW,F,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_rdwr)(VP,UIOP,RW,F,C)
#define VOPO_IOCTL(VP,C,D,F,CR) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_ioctl)(VP,C,D,F,CR)
#define VOPO_SELECT(VP,W,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_select)(VP,W,C)
#define VOPO_GETATTR(VP,VA,C,S) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_getattr)(VP,VA,C,S)
#define VOPO_SETATTR(VP,VA,C,N) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_setattr)(VP,VA,C,N)
#define VOPO_ACCESS(VP,M,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_access)(VP,M,C)
#define VOPO_LOOKUP(VP,NM,VPP,C,MVP) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_lookup)(VP,NM,VPP,C,MVP)
#define VOPO_CREATE(VP,NM,VA,E,M,VPP,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_create)(VP,NM,VA,E,M,VPP,C)
#define VOPO_REMOVE(VP,NM,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_remove)(VP,NM,C)
#define VOPO_LINK(VP,TDVP,TNM,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_link)(VP,TDVP,TNM,C)
#define VOPO_RENAME(VP,NM,TDVP,TNM,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_rename)(VP,NM,TDVP,TNM,C)
#define VOPO_MKDIR(VP,NM,VA,VPP,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_mkdir)(VP,NM,VA,VPP,C)
#define VOPO_RMDIR(VP,NM,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_rmdir)(VP,NM,C)
#define VOPO_READDIR(VP,UIOP,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_readdir)(VP,UIOP,C)
#define VOPO_SYMLINK(VP,LNM,VA,TNM,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_symlink)(VP,LNM,VA,TNM,C)
#define VOPO_READLINK(VP,UIOP,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_readlink)(VP,UIOP,C)
#define VOPO_FSYNC(VP,C,S) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_fsync)(VP,C,S)
#define VOPO_INACTIVE(VP,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_inactive)(VP,C)
#define VOPO_BMAP(VP,BN,VPP,BNP) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_bmap)(VP,BN,VPP,BNP)
#define VOPO_STRATEGY(BP) \
    (*((struct xvfs_vnodeops *)((BP)->b_vp->v_op))->oops.vn_strategy)(BP)
#define VOPO_BREAD(VP,BN,BPP) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_bread)(VP,BN,BPP)
#define VOPO_BRELSE(VP,BP) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_brelse)(VP,BP)
/* check out vnode.h - it panics if not set... */
#define VOPO_PATHSEND(VPP,PNP,FOLLOW,NLINKP,DIRVPP,COMPVPP,OPCODE,DEPENDENT) \
    ((((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_pathsend) ?  \
     ((*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_pathsend)(VP,PNP,FOLLOW,NLINKP,DIRVPP,COMPVPP,OPCODE,DEPENDENT)) : (panic("VOPO_PATHSEND"),EINVAL))
#define VOPO_SETACL(VP,NT,BP) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_setacl)(VP,NT,BP)
#define VOPO_GETACL(VP,NT,BP) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_getacl)(VP,NT,BP)
#define VOPO_PATHCONF(VP,NT,BP,CR) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_pathconf)(VP,NT,BP,CR)
#define VOPO_FPATHCONF(VP,NT,BP,CR) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_fpathconf)(VP,NT,BP,CR)

/*
 * VNOPS for NFS 3.2.
 */
#define VOPO_LOCKCTL(VP,LD,CMD,C,FP,LB,UB) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_lockctl)(VP,LD,CMD,C,FP,LB,UB)
#define VOPO_LOCKF(VP,CMD,SIZE,C,FP,LB,UB) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_lockf)(VP,CMD,SIZE,C,FP,LB,UB)
#define VOPO_FID(VP,FIDPP) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_fid)(VP, FIDPP)
#define VOPO_FSCTL(VP,CMD,UIOP,CRED) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_fsctl)(VP,CMD,UIOP,CRED)
#ifdef	NEVER
#define VOPO_PREFILL(VP,PRP) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_prefill)(VP,PRP)
#define VOPO_DBDDUP(VP,DBD) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_dbddup)(VP,DBD)
#define VOPO_DBDDEALLOC(VP,DBD) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_dbddealloc)(VP,DBD)
#define VOPO_PAGEOUT(VP,PRP,START,END,FLAGS) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_pageout)(PRP,START,END,FLAGS)
#define VOPO_PAGEIN(VP,PRP,WRT,SPACE,VADDR) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->oops.vn_pagein)(PRP,WRT,SPACE,VADDR)
#endif	/* NEVER */

/*
 * Macros to the original HP-UX vnodeops
 */
#define VOPN_OPEN(VPP,F,C) \
    (*((struct xvfs_vnodeops *)((*(VPP))->v_op))->nops.vn_open)(VPP, F, C)
#define VOPN_CLOSE(VP, F, C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_close)(VP, F, C)
#define VOPN_RDWR(VP,UIOP,RW,F,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_rdwr)(VP,UIOP,RW,F,C)
#define VOPN_IOCTL(VP,C,D,F,CR) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_ioctl)(VP,C,D,F,CR)
#define VOPN_SELECT(VP,W,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_select)(VP,W,C)
#define VOPN_GETATTR(VP,VA,C,S) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_getattr)(VP,VA,C,S)
#define VOPN_SETATTR(VP,VA,C,N) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_setattr)(VP,VA,C,N)
#define VOPN_ACCESS(VP,M,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_access)(VP,M,C)
#define VOPN_LOOKUP(VP,NM,VPP,C,MVP) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_lookup)(VP,NM,VPP,C,MVP)
#define VOPN_CREATE(VP,NM,VA,E,M,VPP,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_create)(VP,NM,VA,E,M,VPP,C)
#define VOPN_REMOVE(VP,NM,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_remove)(VP,NM,C)
#define VOPN_LINK(VP,TDVP,TNM,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_link)(VP,TDVP,TNM,C)
#define VOPN_RENAME(VP,NM,TDVP,TNM,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_rename)(VP,NM,TDVP,TNM,C)
#define VOPN_MKDIR(VP,NM,VA,VPP,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_mkdir)(VP,NM,VA,VPP,C)
#define VOPN_RMDIR(VP,NM,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_rmdir)(VP,NM,C)
#define VOPN_READDIR(VP,UIOP,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_readdir)(VP,UIOP,C)
#define VOPN_SYMLINK(VP,LNM,VA,TNM,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_symlink)(VP,LNM,VA,TNM,C)
#define VOPN_READLINK(VP,UIOP,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_readlink)(VP,UIOP,C)
#define VOPN_FSYNC(VP,C,S) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_fsync)(VP,C,S)
#define VOPN_INACTIVE(VP,C) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_inactive)(VP,C)
#define VOPN_BMAP(VP,BN,VPP,BNP) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_bmap)(VP,BN,VPP,BNP)
#define VOPN_STRATEGY(BP) \
    (*((struct xvfs_vnodeops *)((BP)->b_vp->v_op))->nops.vn_strategy)(BP)
#define VOPN_BREAD(VP,BN,BPP) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_bread)(VP,BN,BPP)
#define VOPN_BRELSE(VP,BP) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_brelse)(VP,BP)
/* check out vnode.h - it panics if not set... */
#define VOPN_PATHSEND(VPP,PNP,FOLLOW,NLINKP,DIRVPP,COMPVPP,OPCODE,DEPENDENT) \
    ((((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_pathsend) ?  \
     ((*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_pathsend)(VP)) : \
     (panic("VOPN_PATHSEND"),EINVAL))
#define VOPN_SETACL(VP,NT,BP) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_setacl)(VP,NT,BP)
#define VOPN_GETACL(VP,NT,BP) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_getacl)(VP,NT,BP)
#define VOPN_PATHCONF(VP,NT,BP,CR) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_pathconf)(VP,NT,BP,CR)
#define VOPN_FPATHCONF(VP,NT,BP,CR) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_fpathconf)(VP,NT,BP,CR)

/*
 * VNOPS for NFS 3.2.
 */
#define VOPN_LOCKCTL(VP,LD,CMD,C,FP,LB,UB) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_lockctl)(VP,LD,CMD,C,FP,LB,UB)
#define VOPN_LOCKF(VP,CMD,SIZE,C,FP,LB,UB) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_lockf)(VP,CMD,SIZE,C,FP,LB,UB)
#define VOPN_FID(VP,FIDPP) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_fid)(VP, FIDPP)
#define VOPN_FSCTL(VP,CMD,UIOP,CRED) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_fsctl)(VP,CMD,UIOP,CRED)
#ifdef	NEVER
#define VOPN_PREFILL(VP,PRP) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_prefill)(VP,PRP)
#define VOPN_DBDDUP(VP,DBD) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_dbddup)(VP,DBD)
#define VOPN_DBDDEALLOC(VP,DBD) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_dbddealloc)(VP,DBD)
#endif	/* NEVER */
#define VOPN_PAGEOUT(VP,PRP,START,END,FLAGS) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_pageout)(PRP,START,END,FLAGS)
#define VOPN_PAGEIN(VP,PRP,WRT,SPACE,VADDR,START) \
    (*((struct xvfs_vnodeops *)((VP)->v_op))->nops.vn_pagein)(PRP,WRT,SPACE,VADDR,START)

