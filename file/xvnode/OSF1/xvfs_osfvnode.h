/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/* Copyright (C) 1989, 1990 Transarc Corporation - All rights reserved */

/*
 * HISTORY
 * $Log: xvfs_osfvnode.h,v $
 * Revision 1.1.12.1  1996/10/02  19:03:50  damon
 * 	New DFS from Transarc
 * 	[1996/10/01  18:59:32  damon]
 *
 * Revision 1.1.7.1  1994/06/09  14:26:10  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:38:05  annie]
 * 
 * Revision 1.1.5.3  1993/01/29  22:34:57  bolinger
 * 	Fix OT defect 6030:  add declarations for filelock glue layer
 * 	functions.
 * 	[1993/01/29  21:33:44  bolinger]
 * 
 * Revision 1.1.5.2  1993/01/21  16:34:58  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  18:28:41  cjd]
 * 
 * Revision 1.1.2.3  1992/05/22  20:51:52  garyf
 * 	remove OSF/1 conditional
 * 	[1992/05/22  03:16:23  garyf]
 * 
 * Revision 1.1.2.2  1992/01/24  04:14:42  devsrc
 * 	Fixed logs
 * 	[1992/01/23  22:25:20  devsrc]
 * 
 * $EndLog$
*/

#ifndef	_OSF_XVNODEH_
#define	_OSF_XVNODEH_

/*
   If we are running on a multiprocessor and have an unparallelized physical
   filesystem then the OSF/1 serial code compatibility option must be turned
   on AND all the VOP macros have to be rewritten to funnel the calls
*/

/*
 * Macros for glued vnodeops
 */
#define VOPO_LOOKUP(VP, NDP) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_lookup)(VP, NDP)
#define VOPO_CREATE(NDP, VAP) \
    (*((struct xvfs_vnodeops *)(NDP)->ni_dvp->v_op)->oops.vn_create)(NDP, VAP)
#define VOPO_MKNOD(NDP, VAP, CRED) \
    (*((struct xvfs_vnodeops *)(NDP)->ni_dvp->v_op)->oops.vn_mknod)(NDP, VAP, CRED)
#define VOPO_OPEN(VPP, MODE, CRED)	\
    (*((struct xvfs_vnodeops *)(*(VPP))->v_op)->oops.vn_open)(VPP, MODE, CRED)
#define VOPO_CLOSE(VP, FFLAG, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_close)(VP, FFLAG, CRED)
#define VOPO_ACCESS(VP, MODE, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_access)(VP, MODE, CRED)
#define VOPO_GETATTR(VP, VAP, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_getattr)(VP, VAP, CRED)
#define VOPO_SETATTR(VP, VAP, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_setattr)(VP, VAP, CRED)
#define VOPO_READ(VP, UIO, IOFLAG, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_read)(VP, UIO, IOFLAG, CRED)
#define VOPO_WRITE(VP, UIO, IOFLAG, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_write)(VP, UIO, IOFLAG, CRED)
#define VOPO_IOCTL(VP, COM, DATA, FFLAG, CRED)	\
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_ioctl)(VP, COM, DATA, FFLAG, CRED)
#define VOPO_SELECT(VP, EVENTS, REVENTS, SCAN, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_select)(VP, EVENTS, REVENTS, SCAN, CRED)
#define VOPO_MMAP(VP, FFLAGS, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_mmap)(VP, FFLAGS, CRED)
#define VOPO_FSYNC(VP, FFLAGS, CRED, WAITFOR) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_fsync)(VP, FFLAGS, CRED, WAITFOR)
#define VOPO_SEEK(VP, OLDOFF, NEWOFF, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_seek)(VP, OLDOFF, NEWOFF, CRED)
#define VOPO_REMOVE(NDP) \
    (*((struct xvfs_vnodeops *)(NDP)->ni_dvp->v_op)->oops.vn_remove)(NDP)
#define VOPO_LINK(VP, NDP) \
    (*((struct xvfs_vnodeops *)(NDP)->ni_dvp->v_op)->oops.vn_link)(VP, NDP)
#define VOPO_RENAME(FNDP, TNDP) \
    (*((struct xvfs_vnodeops *)(FNDP)->ni_dvp->v_op)->oops.vn_rename)(FNDP, TNDP)
#define VOPO_MKDIR(NDP, VAP) \
    (*((struct xvfs_vnodeops *)(NDP)->ni_dvp->v_op)->oops.vn_mkdir)(NDP, VAP)
#define VOPO_RMDIR(NDP) \
    (*((struct xvfs_vnodeops *)(NDP)->ni_dvp->v_op)->oops.vn_rmdir)(NDP)
#define VOPO_SYMLINK(NDP, VAP, TARGET) \
    (*((struct xvfs_vnodeops *)(NDP)->ni_dvp->v_op)->oops.vn_symlink)(NDP, VAP, TARGET)
#define VOPO_READDIR(VP, UIO, CRED, EOFFLAGP) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_readdir)(VP, UIO, CRED, EOFFLAGP)
#define VOPO_READLINK(VP, UIOP, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_readlink)(VP, UIOP, CRED)
#define VOPO_ABORTOP(NDP) \
    (*((struct xvfs_vnodeops *)(NDP)->ni_dvp->v_op)->oops.vn_abortop)(NDP)
#define VOPO_INACTIVE(VP) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_inactive)(VP)
#define VOPO_RECLAIM(VP) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_reclaim)(VP)
#define VOPO_BMAP(VP, BN, VPP, BNP) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_bmap)(VP, BN, VPP, BNP)
#define VOPO_STRATEGY(BP) \
    (*((struct xvfs_vnodeops *)((BP)->b_vp)->v_op)->oops.vn_strategy)(BP)
#define VOPO_PRINT(VP) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_print)(VP)
#define VOPO_PGRD(VP, UIO, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_pgrd)(VP, UIO, CRED)
#define VOPO_PGWR(VP, UIO, CRED, PG, OFF) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->oops.vn_pgwr)(VP, UIO, CRED, PG, OFF)
#ifdef	AFS_OSF11_ENV
#define VOPO_FILELOCK(VP, FLOCK, FLAG, OFF) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_filelock)(VP, FLOCK, FLAG, OFF)
#endif	/* AFS_OSF11_ENV */

/*
 * Macros for standard (original) vnodeops
 */
#define VOPN_LOOKUP(VP, NDP) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_lookup)(VP, NDP)
#define VOPN_CREATE(NDP, VAP) \
    (*((struct xvfs_vnodeops *)(NDP)->ni_dvp->v_op)->nops.vn_create)(NDP, VAP)
#define VOPN_MKNOD(NDP, VAP, CRED) \
    (*((struct xvfs_vnodeops *)(NDP)->ni_dvp->v_op)->nops.vn_mknod)(NDP, VAP, CRED)
#define VOPN_OPEN(VPP, MODE, CRED)	\
    (*((struct xvfs_vnodeops *)(*(VPP))->v_op)->nops.vn_open)(VPP, MODE, CRED)
#define VOPN_CLOSE(VP, FFLAG, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_close)(VP, FFLAG, CRED)
#define VOPN_ACCESS(VP, MODE, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_access)(VP, MODE, CRED)
#define VOPN_GETATTR(VP, VAP, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_getattr)(VP, VAP, CRED)
#define VOPN_SETATTR(VP, VAP, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_setattr)(VP, VAP, CRED)
#define VOPN_READ(VP, UIO, IOFLAG, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_read)(VP, UIO, IOFLAG, CRED)
#define VOPN_WRITE(VP, UIO, IOFLAG, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_write)(VP, UIO, IOFLAG, CRED)
#ifndef AFS_OSF11_ENV
#define VOPN_IOCTL(VP, COM, DATA, FFLAG, CRED)	\
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_ioctl)(VP, COM, DATA, FFLAG, CRED)
#else
#define VOPN_IOCTL(VP, COM, DATA, FFLAG, CRED, RETVAL)	\
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_ioctl)(VP, COM, DATA, FFLAG, CRED, RETVAL)
#endif /* AFS_OSF11_ENV */
#define VOPN_SELECT(VP, EVENTS, REVENTS, SCAN, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_select)(VP, EVENTS, REVENTS, SCAN, CRED)
#define VOPN_MMAP(VP, FFLAGS, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_mmap)(VP, FFLAGS, CRED)
#define VOPN_FSYNC(VP, FFLAGS, CRED, WAITFOR) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_fsync)(VP, FFLAGS, CRED, WAITFOR)
#define VOPN_SEEK(VP, OLDOFF, NEWOFF, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_seek)(VP, OLDOFF, NEWOFF, CRED)
#define VOPN_REMOVE(NDP) \
    (*((struct xvfs_vnodeops *)(NDP)->ni_dvp->v_op)->nops.vn_remove)(NDP)
#define VOPN_LINK(VP, NDP) \
    (*((struct xvfs_vnodeops *)(NDP)->ni_dvp->v_op)->nops.vn_link)(VP, NDP)
#define VOPN_RENAME(FNDP, TNDP) \
    (*((struct xvfs_vnodeops *)(FNDP)->ni_dvp->v_op)->nops.vn_rename)(FNDP, TNDP)
#define VOPN_MKDIR(NDP, VAP) \
    (*((struct xvfs_vnodeops *)(NDP)->ni_dvp->v_op)->nops.vn_mkdir)(NDP, VAP)
#define VOPN_RMDIR(NDP) \
    (*((struct xvfs_vnodeops *)(NDP)->ni_dvp->v_op)->nops.vn_rmdir)(NDP)
#define VOPN_SYMLINK(NDP, VAP, TARGET) \
    (*((struct xvfs_vnodeops *)(NDP)->ni_dvp->v_op)->nops.vn_symlink)(NDP, VAP, TARGET)
#define VOPN_READDIR(VP, UIO, CRED, EOFFLAGP) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_readdir)(VP, UIO, CRED, EOFFLAGP)
#define VOPN_READLINK(VP, UIOP, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_readlink)(VP, UIOP, CRED)
#define VOPN_ABORTOP(NDP) \
    (*((struct xvfs_vnodeops *)(NDP)->ni_dvp->v_op)->nops.vn_abortop)(NDP)
#define VOPN_INACTIVE(VP) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_inactive)(VP)
#define VOPN_RECLAIM(VP) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_reclaim)(VP)
#define VOPN_BMAP(VP, BN, VPP, BNP) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_bmap)(VP, BN, VPP, BNP)
#define VOPN_STRATEGY(BP) \
    (*((struct xvfs_vnodeops *)((BP)->b_vp)->v_op)->nops.vn_strategy)(BP)
#define VOPN_PRINT(VP) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_print)(VP)
#define VOPN_PGRD(VP, UIO, CRED) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_pgrd)(VP, UIO, CRED)
#define VOPN_PGWR(VP, UIO, CRED, PG, OFF) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_pgwr)(VP, UIO, CRED, PG, OFF)
#ifdef	AFS_OSF11_ENV
#define VOPN_FILELOCK(VP, FLOCK, FLAG, OFF) \
    (*((struct xvfs_vnodeops *)(VP)->v_op)->nops.vn_filelock)(VP, FLOCK, FLAG, OFF)
#endif	/* AFS_OSF11_ENV */


/*
**      OSF1- specific glue
*/
int xglue_mmap(), xglue_abortop(), xglue_pgrd(), xglue_pgwr(), xglue_print(),
    xglue_read();
int xglue_reclaim(), xglue_seek(), xglue_write();
#ifdef	AFS_OSF11_ENV
int xglue_filelock();
#endif	/* AFS_OSF11_ENV */

#endif /* _OSF_XVNODEH_ */
