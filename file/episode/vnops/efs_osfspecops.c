/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_osfspecops.c,v $
 * Revision 1.1.5.1  1996/10/02  17:44:18  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:36:02  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1994, 1990 Transarc Corporation
   All rights reserved.
 */

/*
 * OSF/1-specific vnode ops for device special files and FIFO's.
 */
#include <dcedfs/osi.h>
#include <dcedfs/osi_kvnode.h>
#include <dcedfs/osi_cred.h>
#include <dcedfs/osi_user.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/debug.h>		/* MBZ */
#include <dcedfs/queue.h>		/* struct squeue */
#include <dcedfs/dacl.h>		/* dacl_perm_xxx */
#include <dcedfs/dir_buffer.h>
#include <dcedfs/dir.h>
#include <dcedfs/episode/logbuf.h>	/* logTranRec_t */
#include <dcedfs/episode/tlock.h>	/* struct tlock_data */
#include <dcedfs/episode/file.h>	/* struct epif_status */
#include <dcedfs/episode/dacl_anode.h>
#include <dcedfs/com_locks.h>		/* struct record_lock */
#include <dcedfs/xvfs_vnode.h>		/* struct xvfs_xops */
#include <dcedfs/aclint.h>		/* struct dfs_acl */
#include <dcedfs/volume.h>		/* VOL_RELE() */
#include <dcedfs/volreg.h>
#include "efs_dirhandle.h"		/* struct epi_Dir_Handle */
#include "efs_evnode.h"			/* struct evnode */
#include "efs_access.h"
#include "efs_vattr.h"
#include "efs_misc.h"
#include "efs_dir.h"
#include "efsmount.h"			/* struct efsMOUNT */
#include "efs_vnodeops.h"

#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */


RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/efs_osfspecops.c,v 1.1.5.1 1996/10/02 17:44:18 damon Exp $")

/*
 * Vnode ops vectors for device special files and for FIFO's in OSF/1
 */
int spec_lookup(), spec_badop(), spec_open(), efsspec_close();
int efsspec_read(), efsspec_write(), spec_ioctl(), spec_select();
int spec_badop(), spec_nullop(), spec_seek(), efsspec_reclaim();
int spec_bmap(), spec_strategy(), efs_print(), spec_page_read();
int spec_page_write(), nosf_access(), nosf_getattr(), nosf_setattr();
int nosf_inactive(), nosf_print();

/* this is an OSF/1 ops table */

struct vnodeops efs_specops = {
	spec_lookup,		/* lookup */
	spec_badop,		/* create */
	spec_badop,		/* mknod */
	spec_open,		/* open */
	efsspec_close,		/* close */
	nosf_access,		/* access */
	nosf_getattr,		/* getattr */
	nosf_setattr,		/* setattr */
	efsspec_read,		/* read */
	efsspec_write,		/* write */
	spec_ioctl,		/* ioctl */
	spec_select,		/* select */
	spec_badop,		/* mmap */
	spec_nullop,		/* fsync */
	spec_seek,		/* seek */
	spec_badop,		/* remove */
	spec_badop,		/* link */
	spec_badop,		/* rename */
	spec_badop,		/* mkdir */
	spec_badop,		/* rmdir */
	spec_badop,		/* symlink */
	spec_badop,		/* readdir */
	spec_badop,		/* readlink */
	spec_badop,		/* abortop */
	nosf_inactive,		/* inactive */
	efsspec_reclaim,	/* reclaim */
	spec_bmap,		/* bmap */
	spec_strategy,		/* strategy */
	nosf_print,		/* print */
	spec_page_read,		/* page_read */
	spec_page_write,	/* page_write */
};

int	fifo_open(), efsfifo_close(), efsfifo_read(), efsfifo_write();
int	fifo_ioctl(), efsfifo_getattr(), fifo_select();

/* this is an OSF/1 ops table */

struct vnodeops efs_fifoops = {
	spec_lookup,		/* lookup */
	spec_badop,		/* create */
	spec_badop,		/* mknod */
	fifo_open,		/* open */
	efsfifo_close,		/* close */
	nosf_access,		/* access */
	efsfifo_getattr,	/* getattr */
	nosf_setattr,		/* setattr */
	efsfifo_read,		/* read */
	efsfifo_write,		/* write */
	fifo_ioctl,		/* ioctl */
	fifo_select,		/* select */
	spec_badop,		/* mmap */
	spec_nullop,		/* fsync */
	spec_seek,		/* seek */
	spec_badop,		/* remove */
	spec_badop,		/* link */
	spec_badop,		/* rename */
	spec_badop,		/* mkdir */
	spec_badop,		/* rmdir */
	spec_badop,		/* symlink */
	spec_badop,		/* readdir */
	spec_badop,		/* readlink */
	spec_badop,		/* abortop */
	nosf_inactive,		/* inactive */
	efs_reclaim,		/* reclaim */
	spec_bmap,		/* bmap */
	spec_badop,		/* strategy */
	nosf_print,		/* print */
	spec_badop,		/* page_read */
	spec_badop,		/* page_write */
};

struct xvfs_xops efs_deadxops = {
	efs_null,	/* open */
	efs_null,	/* close */
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_getxattr,		/* getattr */
	efs_setxattr,		/* setattr */
	efs_access,		/* access */
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_inactive,
	efs_invalid,	/* bmap */
	efs_invalid,	/* strategy */
	efs_invalid,	/* ustrategy */
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_null,	/* fid */
	efs_invalid,	/* hold */
	efs_invalid,	/* rele */
	efs_invalid,	/* setacl */
	efs_invalid,	/* getacl */
	efs_afsfid,
#ifdef KERNEL
	efs_getvolume,
#else
	efs_null,
#endif /* KERNEL */
	efs_invalid,	/* getlength */
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_null,	/* pvnlock */
	efs_null,	/* pvnunlock */
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_invalid,
	efs_null,	/* cached */
	efs_invalid,	/* quota */
	efs_invalid,	/* cntl */
	efs_invalid,	/* update */
	efs_null,	/* fid */
	efs_null,	/* read ahead */
	efs_null,	/* delay write */
	efs_invalid,	/* reclaim */
	efs_invalid,	/* read */
	efs_invalid,	/* write */
	efs_invalid,	/* realvp */
	efs_invalid,	/* rwlock */
	efs_invalid,	/* rwunlock */
	efs_invalid,	/* seek */
	efs_invalid,	/* space */
	efs_invalid,	/* getpage */
	efs_invalid,	/* putpage */
	efs_invalid,	/* addmap */
	efs_invalid,	/* delmap */
	efs_invalid,	/* pageio */
	efs_invalid,	/* pagein */
	efs_invalid,	/* pageout */
	efs_invalid,	/* setfl */
};

extern struct xvfs_vnodeops *xvfs_ops2, *xvfs_ops3;

/*
 * OSF-1 special device and fifo operations
 *
 * IMPORTANT: these operations are called with OSF/1
 * interfaces, not with extended interfaces
 */

efsspec_read(vp, uio, ioflag, cred)
    struct vnode *vp;
    struct uio *uio;
    int ioflag;
    osi_cred_t *cred;
{
    struct evnode *evp;			/* evnode */

    evp = VTOEV(vp);
    vnm_SetAtime (evp, EFS_RWFLAGS_LOCK);
    return (spec_read(vp, uio, ioflag, cred));
}

efsspec_write(vp, uio, ioflag, cred)
    struct vnode *vp;
    struct uio *uio;
    int ioflag;
    osi_cred_t *cred;
{
    epi_anode_t ap;			/* anode handle for evp */
    logTranRec_t transId;		/* transaction ID */
    struct evnode *evp;			/* evnode */

    evp = VTOEV(vp);
    ap = EVTOA (evp);
    elbb_StartTran("efs_rdwr", 0, epia_AnodeDevice (ap), &transId);
    epif_Mark (transId, ap, EPIF_SFLAGS_MTIME | EPIF_SFLAGS_CTIME);
    elbb_EndTran(transId);

    return (spec_write(vp, uio, ioflag, cred));
}

efsspec_close(vp, fflag, cred)
    struct vnode *vp;
    int fflag;
    osi_cred_t *cred;
{
    struct evnode *evp;			/* evnode */

    evp = VTOEV(vp);
    vnm_SetAtime (evp, EFS_RWFLAGS_LOCK);
    return (spec_close(vp, fflag, cred));
}

efsspec_reclaim(vp)
register struct vnode *vp;
{
    if (!spec_reclaim(vp))
	return (efs_reclaim(vp));
}

efsfifo_read(vp, uio, ioflag, cred)
    struct vnode *vp;
    struct uio *uio;
    int ioflag;
    osi_cred_t *cred;
{
    struct evnode *evp;			/* evnode */

    evp = VTOEV(vp);
    vnm_SetAtime (evp, EFS_RWFLAGS_LOCK);
    return (fifo_read(vp, uio, ioflag, cred));
}

efsfifo_write(vp, uio, ioflag, cred)
    struct vnode *vp;
    struct uio *uio;
    int ioflag;
    osi_cred_t *cred;
{
    epi_anode_t ap;			/* anode handle for evp */
    logTranRec_t transId;		/* transaction ID */
    struct evnode *evp;			/* evnode */

    evp = VTOEV(vp);
    ap = EVTOA (evp);
    elbb_StartTran("efs_rdwr", 0, epia_AnodeDevice (ap), &transId);
    epif_Mark (transId, ap, EPIF_SFLAGS_MTIME | EPIF_SFLAGS_CTIME);
    elbb_EndTran(transId);

    return (fifo_write(vp, uio, ioflag, cred));
}

efsfifo_close(vp, fflag, cred)
    struct vnode *vp;
    int fflag;
    osi_cred_t *cred;
{
    struct evnode *evp;			/* evnode */

    evp = VTOEV(vp);
    vnm_SetAtime (evp, EFS_RWFLAGS_LOCK);
    return (fifo_close(vp, fflag, cred));
}

efsfifo_getattr(vp, vap, cred)
    struct vnode *vp;
    register struct vattr *vap;
    osi_cred_t *cred;
{
    int error;
    struct evnode *evp;			/* evnode */

    evp = VTOEV(vp);

    /*
     * Get most attributes from the inode, rest
     * from the fifo.
     */
    if (error = efs_getattr(evp, vap, cred))
	return (error);
    return (fifo_getattr(vp, vap, cred));
}
