/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xvfs_xattr.h,v $
BINRevision 1.1.2.1  1996/10/02  19:04:19  damon
BIN	New DFS from Transarc
BIN
 * $EndLog$
 */
/* Copyright (C) 1996 Transarc Corporation - All rights reserved */

/* $Header: /u0/rcs_trees/dce/rcs/file/xvnode/xvfs_xattr.h,v 1.1.2.1 1996/10/02 19:04:19 damon Exp $ */

#ifndef	TRANSARC_XATTR_H
#define	TRANSARC_XATTR_H
#include <dcedfs/common_data.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_cred.h>

/* Exported structures and functions are preceded by module name xvfs_ */

/*
 * Additional attribute fields that have special meaning in AFS; used by both 
 * Episode and UFS.
 */
struct Txvattr {
    afs_hyper_t dataVersion;
    afs_hyper_t fileID;
    afs_hyper_t volVersion;
    u_long author;
    u_long callerAccess;
    u_long anonAccess;
    u_long parentVnode;
    u_long parentUnique;
    afsTimeval serverModTime;
    u_long fstype;	/* type of the containing aggregate, from aggr.h */
    /* Values that are principally derived from what Episode stores for DFS */
    afsUUID    objid;
    u_long  timeUncertainty;
    /* for Episode, the CFLAGS from epia_GetInfo (anode.h),
	  including EPIA_CFLAGS_COPYONWRITE */
    u_long  representationFlags;
    /* Magic cookies usable to identify common ancestry. */
    u_long  backingIndex;
    u_long  backingVolIndex;
    /*
     * These Ix values are cookies that are interpretable only by the
     * underlying representation.
     */
    u_long  aclIx;
    u_long  initDirAclIx;
    u_long  initFileAclIx;
    u_long  plistIx;
    u_long  uPlistIx;

    u_long	clientOnlyAttrs;	/* reserved for machines that will never run FXs */
    u_long	spare1;
    u_long	spare2;
    u_long	spare3;
    u_long	spare4;
    u_long	spare5;
    u_long	spare6;
};


/*
 * The "Enhanced" attribute structure: The standard "vattr" structure is
 * simply an overlay (always first) in this structure.
 */
struct xvfs_attr {
    struct vattr vattr;
    struct Txvattr xvattr;
};

#include <dcedfs/volume.h>

#endif	/* TRANSARC_XATTR_H */
