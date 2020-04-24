/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xvfs_aixvnode.h,v $
 * Revision 1.1.11.1  1996/10/02  19:04:04  damon
 * 	New DFS from Transarc
 * 	[1996/10/01  18:59:37  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1989, 1994 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>


/*
 * Macros to the GLUED AIX vnodeops
 */
#define	VOPO_ACCESS(VP, MODE, WHO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_access)(VP,MODE,WHO)
#define	VOPO_CLOSE(VP, FL, VINFO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_close)(VP,FL,VINFO)
#define	VOPO_CREATE(DP, VPP, FL, NM, MODE, VINFO) \
    (*((struct xvfs_vnodeops *)((DP)->v_gnode->gn_ops))->oops.vn_create)(DP,VPP,FL,NM,MODE,VINFO)
#define	VOPO_FCLEAR(VP, FL, OFF, LEN, VINFO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_fclear)(VP,FL,OFF,LEN,VINFO)
#define	VOPO_FID(VP, FID) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_fid)(VP,FID)
#define	VOPO_FSYNC(VP, FL, VINFO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_fsync)(VP,FL,VINFO)
#define	VOPO_FTRUNC(VP, FL, LEN, VINFO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_ftrunc)(VP,FL,LEN,VINFO)
#define	VOPO_GETATTR(VP, VA) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_getattr)(VP,VA)
#define	VOPO_REVOKE(VP, CMD, FL, VA) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_revoke)(VP,CMD,FL,VA)
#define	VOPO_HOLD(VP) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_hold)(VP)
#define	VOPO_IOCTL(VP, CMD, ARG, FL, EXT) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_ioctl)(VP,CMD,ARG,FL,EXT)
#define	VOPO_LOCKCTL(VP, OFF, LD, CMD, RF, RI) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_lockctl)(VP,OFF,LD,CMD,RF,RI)
#define	VOPO_MAP(VP, ADDR, LEN, OFF, FL) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_map)(VP,ADDR,LEN,OFF,FL)
#define	VOPO_OPEN(VP, FL, EXT, VINFO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_open)(VP,FL,EXT,VINFO)
#define	VOPO_SELECT(VP, CO, REQ, RTN, NOT, VINFO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_select)(VP,CO,REQ,RTN,NOT,VINFO)
#define	VOPO_RDWR(VP, OP, FL, UIO, EXT, VINFO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_rdwr)(VP,OP,FL,UIO,EXT,VINFO)
#define	VOPO_LINK(VP, DVP, TNM) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_link)(VP,DVP,TNM)
#define	VOPO_LOOKUP(DP, VPP, NAM, FL) \
    (*((struct xvfs_vnodeops *)((DP)->v_gnode->gn_ops))->oops.vn_lookup)(DP,VPP,NAM,FL)
#define	VOPO_MKDIR(VP, NAM, MODE) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_mkdir)(VP,NAM,MODE)
#define	VOPO_MKNOD(VP, NAM, MODE, DEV) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_mknod)(VP,NAM,MODE,DEV)
#define	VOPO_READDIR(VP, UIO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_readdir)(VP,UIO)
#define	VOPO_READLINK(VP, UIO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_readlink)(VP,UIO)
#define	VOPO_RELE(VP) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_rele)(VP)
#define	VOPO_REMOVE(DP, VP, NAM) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_remove)(DP,VP,NAM)
#define	VOPO_RENAME(VP, DP, NAM, TP, TDP, TNM) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_rename)(VP,DP,NAM,TP,TDP,TNM)
#define	VOPO_RMDIR(VP, DP, NAM) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_rmdir)(VP,DP,NAM)
#define	VOPO_SETATTR(VP, OP, AR1, AR2, AR3) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_setattr)(VP,OP,AR1,AR2,AR3)
#define	VOPO_STRATEGY(VP, BP) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_strategy)(VP,BP)
#define	VOPO_SYMLINK(VP, LNM, TNM) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_symlink)(VP,LNM,TNM)
#define	VOPO_UNMAP(VP, ADDR) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->oops.vn_unmap)(VP,ADDR)


/*
 * Macros to the original AIX vnodeops
 */
#define	VOPN_ACCESS(VP, MODE, WHO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_access)(VP,MODE,WHO)
#define	VOPN_CLOSE(VP, FL, VINFO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_close)(VP,FL,VINFO)
#define	VOPN_CREATE(DP, VPP, FL, NM, MODE, VINFO) \
    (*((struct xvfs_vnodeops *)((DP)->v_gnode->gn_ops))->nops.vn_create)(DP,VPP,FL,NM,MODE,VINFO)
#define	VOPN_FCLEAR(VP, FL, OFF, LEN, VINFO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_fclear)(VP,FL,OFF,LEN,VINFO)
#define	VOPN_FID(VP, FID) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_fid)(VP,FID)
#define	VOPN_FSYNC(VP, FL, VINFO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_fsync)(VP,FL,VINFO)
#define	VOPN_FTRUNC(VP, FL, LEN, VINFO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_ftrunc)(VP,FL,LEN,VINFO)
#define	VOPN_GETATTR(VP, VA) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_getattr)(VP,VA)
#define	VOPN_REVOKE(VP, CMD, FL, VA) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_revoke)(VP,CMD,FL,VA)
#define	VOPN_HOLD(VP) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_hold)(VP)
#define	VOPN_IOCTL(VP, CMD, ARG, FL, EXT) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_ioctl)(VP,CMD,ARG,FL,EXT)
#define	VOPN_LOCKCTL(VP, OFF, LD, CMD, RF, RI) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_lockctl)(VP,OFF,LD,CMD,RF,RI)
#define	VOPN_MAP(VP, ADDR, LEN, OFF, FL) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_map)(VP,ADDR,LEN,OFF,FL)
#define	VOPN_OPEN(VP, FL, EXT, VINFO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_open)(VP,FL,EXT,VINFO)
#define	VOPN_SELECT(VP, CO, REQ, RTN, NOT, VINFO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_select)(VP,CO,REQ,RTN,NOT,VINFO)
#define	VOPN_RDWR(VP, OP, FL, UIO, EXT, VINFO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_rdwr)(VP,OP,FL,UIO,EXT,VINFO)
#define	VOPN_LINK(VP, DVP, TNM) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_link)(VP,DVP,TNM)
#define	VOPN_LOOKUP(DP, VPP, NAM, FL) \
    (*((struct xvfs_vnodeops *)((DP)->v_gnode->gn_ops))->nops.vn_lookup)(DP,VPP,NAM,FL)
#define	VOPN_MKDIR(VP, NAM, MODE) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_mkdir)(VP,NAM,MODE)
#define	VOPN_MKNOD(VP, NAM, MODE, DEV) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_mknod)(VP,NAM,MODE,DEV)
#define	VOPN_READDIR(VP, UIO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_readdir)(VP,UIO)
#define	VOPN_READLINK(VP, UIO) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_readlink)(VP,UIO)
#define	VOPN_RELE(VP) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_rele)(VP)
#define	VOPN_REMOVE(DP, VP, NAM) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_remove)(DP,VP,NAM)
#define	VOPN_RENAME(VP, DP, NAM, TP, TDP, TNM) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_rename)(VP,DP,NAM,TP,TDP,TNM)
#define	VOPN_RMDIR(VP, DP, NAM) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_rmdir)(VP,DP,NAM)
#define	VOPN_SETATTR(VP, OP, AR1, AR2, AR3) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_setattr)(VP,OP,AR1,AR2,AR3)
#define	VOPN_STRATEGY(VP, BP) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_strategy)(VP,BP)
#define	VOPN_SYMLINK(VP, LNM, TNM) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_symlink)(VP,LNM,TNM)
#define	VOPN_UNMAP(VP, ADDR) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_unmap)(VP,ADDR)
#define VOPN_GETACL(VP,UIOP) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_getacl)(VP,UIOP)
#define VOPN_SETACL(VP,UIOP) \
    (*((struct xvfs_vnodeops *)((VP)->v_gnode->gn_ops))->nops.vn_setacl)(VP,UIOP)


/*
 * Converts from BSD to AIX mode bits
 */
#define SETUP_MODE(excl, flags) \
    (flags) = FCREAT; \
    if ((excl) == EXCL)  (flags) |= FEXCL;

#define	CONV_MODE(mode, excl, flags)	SETUP_MODE((excl), (flags))
