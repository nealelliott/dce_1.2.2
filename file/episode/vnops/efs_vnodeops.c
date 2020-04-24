/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1990 Transarc Corporation
   All rights reserved.
 */

/*			    Episode File System
 *			Vnode operations and vector
 */
#include <dcedfs/osi.h>
#include <dcedfs/osi_kvnode.h>
#include <dcedfs/osi_cred.h>
#include <dcedfs/osi_user.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/debug.h>
#include <dcedfs/queue.h>		/* struct squeue */
#include <dcedfs/dacl.h>		/* dacl_perm_xxx */
#include <dcedfs/dir_buffer.h>
#include <dcedfs/dir.h>			/* dir functions && CONSTS */
#include <dcedfs/episode/critical.h>
#include <dcedfs/episode/logbuf.h>	/* logTranRec_t */
#include <dcedfs/episode/tlock.h>	/* struct tlock_data */
#include <dcedfs/episode/file.h>	/* struct epif_status */
#include <dcedfs/episode/dacl_anode.h>
#include <dcedfs/com_locks.h>		/* struct record_lock */
#include <dcedfs/xvfs_vnode.h>		/* struct xvfs_xops */
#include <dcedfs/aclint.h>		/* struct dfs_acl */
#include <dcedfs/volume.h>		/* VOL_RELE() */
#include <dcedfs/volreg.h>

#include <efs_dirhandle.h>		/* struct epi_Dir_Handle */
#include <efs_evnode.h>			/* struct evnode */
#include <efs_volvnops.h>
#include <efs_access.h>
#include <efs_vattr.h>
#include <efs_misc.h>
#include <efs_dir.h>
#include <efs_vnodeops.h>
#ifdef AFS_SUNOS5_ENV
#ifdef AFS_SUNOS54_ENV
#undef a_type				/* defined in aggr.h */
#endif /* AFS_SUNOS54_ENV */
#include <sys/fs_subr.h>
#ifndef _KERNEL
extern struct vnode *specvp(struct vnode *, dev_t, vtype_t, osi_cred_t *);
#endif /* !_KERNEL */
#endif /* AFS_SUNOS5_ENV */

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_vnodeops.c,v 10.30 1996/05/08 15:33:46 cfe Exp $")

/*
 * We originally wrote functions suitable for use in a vnodeops array under
 * an old version of VFS (e.g. for Suns running SunOS 3 or for RT's).  Now
 * we have wrapper functions for DEC GFS, IBM AIX, OSF/1, and more recent
 * versions of SunOS that make the xops functions suitable for use on those
 * systems; the wrapper functions are in the xvnode package.
 */

/*
 * Tlocks, transactions and equivalence classes.
 *
 * All operations in a vnode-op are typically grouped in a transaction.
 *
 * A thread may not start more than one transaction at a time.  All
 * operations have been ordered to ensure that this does not happen.
 * VN_RELE may start a transaction, so all calls to it are after
 * the main transaction of the operation.  Also, allocation of a new
 * vnode may start a transaction (because the file that the vnode used
 * to represent may have to be updated), so calls to vnm_Allocate
 * are before the main transaction.
 *
 * Every vnode has a ``tlock'' (transaction lock).  Whenever a vnode is locked
 * as part of a transaction, the transaction ID should be passed to the
 * locking primitives, either when the lock is obtained or when it is
 * released. This causes the equivalence classes to form correctly.
 *
 * Transactions, like locks, represent resources.  To avoid deadlocks in
 * resource allocation, we follow a discipline:  a vnode-op must obtain all
 * locks before starting any transactions.
 */

/*
 * Credential structure
 *
 * In the DFS environment, the credential structure is used in unconventional
 * ways.  The uid, gid, groups, etc. are largely ignored.  Instead, an index
 * to a table is stored (in scrambled format) in the first two groups of the
 * group array, and the ``real'' identity and group memberships of the process
 * or thread are looked up in that table.  For this reason, the code in this
 * module never accesses fields of the credential structure directly, but
 * calls functions to determine answers to access questions.
 */

/*
 * vd_len and VD_LENVALID
 *
 * In VM-integrated systems, output to a file may occur long after the
 * actual write system call returns.  The vnode ops layer therefore cannot
 * get the length of the file from the anode, but must keep track of it
 * itself.  It is kept in the vd_len member of the vnode.  The VD_LENVALID
 * bit tells if vd_len is up to date.  If it is not up to date, the correct
 * length of the file must be in the anode.
 */

/* EXPORT */
long afsdb_episode_vnops = 0;
long efs_allowSuserDirLinkOps = 0;	/* root can't link/unlink dirs */

#define DEBUG_THIS_FILE EFS_DEBUG_VNOPS

#ifndef	KERNEL
extern struct user u;
#endif	/* KERNEL */

/* Temporary until we figure out where to put these */

#define MaxJunctionSize 8192		/* # characters in junction, should be
					   some multiple of MAXPATHLENGTH ?*/

#ifndef MAXEND
#define MAXEND 0x7FFFFFFF		/* byte # in flock; taken from AIX
					   version of flock.h */
#endif

#ifdef	AFS_OSF_ENV
extern struct xvfs_vnodeops *xvfs_ops2, *xvfs_ops3;
#endif /* AFS_OSF_ENV */

/*
 * efs_null -- no-op vnode op
 */

/* EXPORT */
int efs_null()
{
    return (0);
}

/*
 * efs_invalid -- invalid vnode op
 */

/* EXPORT */
int efs_invalid()
{
    return EINVAL;
}

/*
 * efs_nosys -- function not implemented
 */
/* EXPORT */
int efs_nosys()
{
    return ENOSYS;
}

/*
 * efs_panic -- extra-bogus vnode op
 */

/* EXPORT */
int efs_panic()
{
    panic("efs_panic");
    /* NOTREACHED */
    return 0;
}

#if (!defined(KERNEL) || !(defined(AFS_AIX_ENV) || defined(AFS_SUNOS5_ENV)))

/* Non-VM integated version of rdwr */

int rdwr_novm(    
  struct vnode *vp,			/* vnode */
  struct uio *uiop,			/* uio structure */
  enum uio_rw rw,			/* tells whether reading or writing */
  int ioflag,				/* tells whether appending */
  osi_cred_t *cred)			/* credential structure */
{
    struct evnode *evp = VTOEV(vp);
    int code = 0;

    /* Check: no data to read/write */
    if (uiop->uio_resid == 0)
	return (0);

    /* Check: invalid offset or length */
    if (uiop->uio_offset < 0 || (uiop->uio_offset + uiop->uio_resid) < 0)
	return (EINVAL);

    /* De-phantomize */
    code = EV_CHECKSTALE(evp);
    if (code)
	return code;

    /* Not supposed to get here for special files */

    switch (rw) {
      case UIO_READ:

	BEGIN_TLOCK_READ (&evp->vd_tlock) {
	    BEGIN_LOCK_READ (&evp->vd_file.lock) {
		epi_anode_t ap = EVTOA(evp);
		u_long fileLen;
		u_long diff;
		u_long nbytes;

		/* Check: request starts beyond EOF */
		fileLen = AFS_hgetlo(epia_GetLength (ap));
		if (uiop->uio_offset >= fileLen)
		    EXIT_CRIT;		/* ignore this request */

		nbytes = uiop->uio_resid;
		diff = 0;

		/* Adjust request so it doesn't go beyond EOF */
		if (nbytes + uiop->uio_offset > fileLen) {
		    nbytes = fileLen - uiop->uio_offset;
		    diff = uiop->uio_resid - nbytes;
		    uiop->uio_resid = nbytes;
		}

		/* Read the data (calling anode layer) */
		code = epia_Read(ap, 0, uiop);

		/* Translate error codes */
		if (code) {
		    VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);
		} else {
		    uiop->uio_resid += diff;
		}
	    } END_LOCK;

	    /* Update cached atime */
	    vnm_SetAtime (evp);

	} END_LOCK;
	break;

      case UIO_WRITE:

	BEGIN_TLOCK_WRITE_NT (&evp->vd_tlock) {
	    BEGIN_LOCK_WRITE (&evp->vd_file.lock) {
		epi_anode_t ap = EVTOA(evp);
		afs_hyper_t hlen;		/* full length of evp */
		u_long fileLen;

		/* For APPEND mode, determine length of file */
		if (evp->vd_flags & VD_LENVALID) {
		    hlen = evp->vd_len;
		} else {
		    hlen = epia_GetLength (ap);
		}
		fileLen = AFS_hgetlo(hlen);

		/* Handle APPEND mode */
		if (ioflag & IO_APPEND)
		    uiop->uio_offset = fileLen;
#ifdef	AFS_SUNOS5_ENV
		if (uiop->uio_offset + uiop->uio_resid > uiop->uio_limit)
#else
		if (uiop->uio_offset + uiop->uio_resid > osi_getufilelimit())
#endif /* AFS_SUNOS5_ENV */
		    {
			ABORT_CRIT(EFBIG);
		    }
		/* Write the data (calling anode layer) */
		code = epia_Write(elbt_NullTranId, ap, 0, uiop);

		/* Translate error codes */
		if (code) {
		    VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);
		} else {			/* write succeeded */
		    /* Update mtime and ctime */
		    vnm_SetMCtime (evp, MARK_MC);

		    /* Clear SUID and SGID bits if necessary */
		    if (osi_GetUID (cred) != 0 &&
			(epif_GetMode (ap) & (VSUID | VSGID)))
			vnm_ClearSUID (evp);

		    /* May clear VD_LENVALID (don't know if this can happen) */
		    hlen = epia_GetLength (ap);
		    if (evp->vd_flags & VD_LENVALID)
			if (AFS_hcmp (evp->vd_len, hlen) < 0)
			    evp->vd_flags &= ~VD_LENVALID;
		}
	    } END_LOCK;
	} END_LOCK;
	if (code)
	    return code;

	/* Synchronous write if required */
#ifdef	AFS_HPUX_ENV
	if ((ioflag & IO_SYNC) || (uiop->uio_fpflags & FSYNCIO))
#else
	if (ioflag & IO_SYNC)
#endif
	    efs_fsync (vp, cred);
	break;

      default:
	panic("efs_rdwr");
    }

    return code;
}
#endif

/*
 * efs_rdwr -- read or write data from or to vnode, with locking
 *
 * LOCKS USED -- When reading, this function obtains the vnode's tlock for
 *		 reading, and later releases it, and then calls vnm_SetAtime,
 *		 which obtains the vnode's tlock for writing, and later
 *		 releases it.
 *
 *		 When writing, this function obtains the vnode's tlock for
 *		 writing, and later releases it.
 *
 * SPECIAL NOTE -- For platforms with VM, this function just calls the
 *		   appropriate VM I/O function.  On SunOS, we should only
 *		   get here from an internal caller (e.g., px), since there
 *		   is no native VOP_RDWR.
 */

/* EXPORT */
int efs_rdwr(vp, uiop, rw, ioflag, cred)
    struct vnode *vp;			/* vnode */
    struct uio *uiop;			/* uio structure */
    enum uio_rw rw;			/* tells whether reading or writing */
    int ioflag;				/* tells whether appending */
    osi_cred_t *cred;			/* credential structure */
{
    struct evnode *evp = VTOEV(vp);
    int code = 0;


    if (osi_vType (vp) == VDIR) {
	osi_MakePreemptionRight();
	if (rw != UIO_READ) {
	    code = EISDIR;
	} else {
	    code = EV_CHECKSTALE (evp);
	    if (!code) {
		code = vnd_Read(evp, uiop, cred, 2/*isPX*/, 0, 0);
	    }
	}
	osi_UnmakePreemptionRight();
    } else if (osi_vType (vp) != VREG) {
	code = 	EINVAL;
    } else {

	/* for files do the regular rdwr thing */

#if defined(AFS_AIX_ENV) && defined(KERNEL)
	code = efs_vmrdwr(vp, uiop, rw, ioflag, cred);
#elif defined(AFS_SUNOS5_ENV) && defined(_KERNEL)
	switch (rw) {
	  case UIO_WRITE:
	    code = efs_vmwrite (vp, uiop, ioflag, cred);
	    break;
	  case UIO_READ:
	    code = efs_vmread (vp, uiop, ioflag, cred);
	    break;
	  default:
	    panic("efs_rdwr");
	    /* NOTREACHED */
	}
#else
	osi_MakePreemptionRight();
	code = rdwr_novm(vp, uiop, rw, ioflag, cred);
	osi_UnmakePreemptionRight();
#endif
    }

    return code;
}

/*
 * efs_getattr and efs_getxattr -- Get file attributes
 *
 * efs_getattr conforms to the specs of the original vnode interface.
 * efs_getxattr conforms to the specs of the X-op interface.  The extra
 *   argument specifies whether the vnode attribute structure is ``extended.''
 */

/* EXPORT */
int efs_getattr(vp, vap, cred)
    struct vnode *vp;			/* vnode */
    struct vattr *vap;			/* vnode attributes */
    osi_cred_t *cred;			/* credential structure */
{
    struct evnode *evp = VTOEV(vp);
    int code;

    osi_MakePreemptionRight();

    evp = VTOEV(vp);
    code = EV_CHECKSTALE (evp);
    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    EFS_VMLEN_RLOCK(evp);
    tlock_ObtainRead(&evp->vd_tlock);

    /*
     * If the Mtime or Ctime are new, update the anode now.
     * This is supposed to keep clients from seeing these times go backward.
     * To put teeth in this precaution, shouldn't we commit the transaction?
     */

    if (evp->vd_cache.newMtime | evp->vd_cache.newCtime) {
	BEGIN_LOCK_WRITE(&evp->vd_file.lock) {
	    vnm_UpdateAnode (evp, elbb_zeroTrans);
	} END_LOCK;
    }

    code = vnva_GetAttr(evp, vap, cred, 0);
    if (code) {
	icl_Trace4(efs_iclSet, EFS_TRACE_GETATTR_ERROR,
		   ICL_TYPE_STRING, "efs_getattr",
		   ICL_TYPE_POINTER, evp,
		   ICL_TYPE_LONG, evp->vd_fid.index,
		   ICL_TYPE_LONG, code);
    }
    tlock_ReleaseRead(&evp->vd_tlock);
    EFS_VMLEN_RUNLOCK(evp);

    osi_UnmakePreemptionRight();
    return (code);
}

/* EXPORT */
int efs_getxattr (vp, vap, extended, cred)
    struct vnode *vp;			/* vnode */
    struct xvfs_attr *vap;			/* vnode attributes */
    int extended;			/* indicates extended attributes */
    osi_cred_t *cred;			/* credential structure */
{
    struct evnode *evp;
    int code;

    osi_MakePreemptionRight();

    evp = VTOEV(vp);
    code = EV_CHECKSTALE (evp);
    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    EFS_VMLEN_RLOCK(evp);
    tlock_ObtainRead(&evp->vd_tlock);

    /*
     * If the Mtime or Ctime are new, update the anode now.
     * This is supposed to keep clients from seeing these times go backward.
     * To put teeth in this precaution, shouldn't we commit the transaction?
     */

    if (evp->vd_cache.newMtime | evp->vd_cache.newCtime) {
	BEGIN_LOCK_WRITE(&evp->vd_file.lock) {
	    vnm_UpdateAnode (evp, elbb_zeroTrans);
	} END_LOCK;
    }

    code = vnva_GetAttr(evp, (struct vattr *)vap, cred, extended);
    if (code) {
	icl_Trace4(efs_iclSet, EFS_TRACE_GETATTR_ERROR,
		   ICL_TYPE_STRING, "efs_getxattr",
		   ICL_TYPE_POINTER, evp,
		   ICL_TYPE_LONG, evp->vd_fid.index,
		   ICL_TYPE_LONG, code);
    }
    tlock_ReleaseRead(&evp->vd_tlock);
    EFS_VMLEN_RUNLOCK(evp);

    osi_UnmakePreemptionRight();
    return (code);
}

/*
 * efs_setattr and efs_setxattr -- Set attributes of a vnode
 *
 * efs_setattr conforms to the specs of the original vnode interface.
 * efs_setxattr conforms to the specs of the X-op interface.  The extra
 *   argument specifies whether the vnode attribute structure is ``extended.''
 */

/* EXPORT */
int efs_setattr(vp, vap, cred)
    struct vnode *vp;			/* vnode */
    struct vattr *vap;			/* vnode attribute structure */
    osi_cred_t *cred;			/* credential structure */
{
    struct evnode *evp = VTOEV(vp);
    int code;				/* error return code */

    osi_MakePreemptionRight();
    code = EV_CHECKSTALE (evp);
    if (!code)
	code = vnva_SetAttr(evp, vap, cred, 0);
    osi_UnmakePreemptionRight();
    return (code);
}

/* EXPORT */
int efs_setxattr(vp, vap, flag, cred)
    struct vnode *vp;			/* vnode */
    struct xvfs_attr *vap;			/* vnode attribute structure */
    int flag;				/* indicates extended attributes */
    osi_cred_t *cred;			/* credential structure */
{
    struct evnode *evp = VTOEV(vp);
    int code;				/* error return code */

    osi_MakePreemptionRight();
    code = EV_CHECKSTALE (evp);
    if (!code)
	code = vnva_SetAttr (evp, (struct vattr *)vap, cred, flag);
    osi_UnmakePreemptionRight();
    return (code);
}

/*
 * efs_access -- Check access rights to a vnode
 */

/* EXPORT */
int efs_access(vp, mode, cred)
    struct vnode *vp;			/* vnode */
    int mode;				/* mode (requested access rights) */
    osi_cred_t *cred;			/* credential structure */
{
    int code;				/* error return code */
    struct evnode *evp = VTOEV(vp);

    osi_MakePreemptionRight();
    code = EV_CHECKSTALE (evp);
    if (!code) {
	BEGIN_TLOCK_READ (&evp->vd_tlock) {
	    dacl_permset_t permset = 0;	/* temp permset */
	    dacl_AddPermBitsToOnePermset_r (mode, &permset);
	    code = vnax_CanAccess (evp, NULL, permset, cred);
	} END_LOCK;
    }
    osi_UnmakePreemptionRight();
    return (code);
}

/*
 * efs_lookup -- Look up a name in a directory
 *
 * CAUTIONS -- As per POSIX, this does not set the parent directory's atime.
 */

/* EXPORT */
int efs_lookup(dvp, name, vpp, cred)
    struct vnode *dvp;			/* directory vnode */
    char *name;				/* name */
    struct vnode **vpp;			/* pointer to entry vnode */
    osi_cred_t *cred;			/* credential structure */
{
    int code;				/* error return code */
    epi_volFileId_t efid;		/* fid for entry */
    struct evnode *devp;		/* directory evnode */
    struct evnode *evp = NULL;		/* entry vnode */
    struct vnode *vp;

    osi_MakePreemptionRight();
    devp = VTOEV(dvp);
    code = EV_CHECKSTALE (devp);
    if (!code) {
	/* Sanity check -- is vnode a directory? */
	if (!EV_ISDIR (devp)) {
	    code = ENOTDIR;
	    icl_Trace4(efs_iclSet, EFS_TRACE_LOOKUP_NON_DIR,
		       ICL_TYPE_STRING, name,
		       ICL_TYPE_POINTER, devp,
		       ICL_TYPE_LONG, devp->vd_fid.index,
		       ICL_TYPE_LONG, code);

	}
	/* Check: directory has been unlinked */
	else if (devp->vd_flags & VD_DIRDELETED) {
	    code = ENOENT;
	    icl_Trace4(efs_iclSet, EFS_TRACE_LOOKUP_UNLINKED_DIR,
		       ICL_TYPE_STRING, name,
		       ICL_TYPE_POINTER, devp,
		       ICL_TYPE_LONG, devp->vd_fid.index,
		       ICL_TYPE_LONG, code);
	}
    }

    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    /* Obtain lock on parent directory */
    tlock_ObtainRead (&devp->vd_tlock);

    /* Check access rights */
    code = vnax_CanAccess (devp, NULL, dacl_perm_execute, cred);
    if (code) {
	icl_Trace4(efs_iclSet2, EFS_TRACE_LOOKUP_CANACCESS,
		   ICL_TYPE_STRING, name,
		   ICL_TYPE_POINTER, devp,
		   ICL_TYPE_LONG, devp->vd_fid.index,
		   ICL_TYPE_LONG, code);
	goto out;
    }

    /* See if it's in the dnlc */
    vp = osi_dnlc_lookup_r(dvp, name);
    if (vp != NULL) {
	evp = VTOEV(vp);
	goto out1;
    }

    /* 
     * Do the lookup, caching result if possible. 
     * Rely on tracing in vnd_dirLookup
     */
    code = vnd_dirLookup (devp, name, &efid);
    if (code) {
	goto out;
    }

    /* Shortcut lookup of self */
    if (epid_EqualIndexes (&efid, &devp->vd_fid)) {
	FAST_VN_HOLD (devp);
	evp = devp;
	goto out1;
    }

    /* 
     * Turn the fid into a vnode. 
     * Rely on tracing in vnm_FindVnode to trace evp
     */
    code = vnm_FindVnode (devp->vd_volp, &efid, &evp);
    if (code) {
	afsl_Assert (code != ESTALE);
	goto out;
    }

    osi_dnlc_enter_r(dvp, name, EVTOV(evp));

out1:
#if	!defined(AFS_AIX32_ENV) && !defined(AFS_HPUX_ENV)
    if (EV_ISVDEV (evp)) {
	struct vnode *newvp;
    /*
     * Handle device special files and FIFO's.  Note that AIX 3.2
     * does this above the vnode interface.
     */

	/* Trade in our vnode for a special vnode */
#ifdef AFS_OSF_ENV
	newvp = EVTOV(evp);
	if (EV_ISFIFO(newvp)) {
	   newvp->v_op = (struct osi_vnodeops *)xvfs_ops3;
	} else {
	   code = specalloc(newvp, evp->vd_rdev);
	   newvp->v_op = (struct osi_vnodeops *)xvfs_ops2;
	}
#else
	vp = EVTOV(evp);
	newvp = specvp(vp, evp->vd_rdev, vp->v_type, cred);
	VN_RELE(vp);
#endif /* AFS_OSF_ENV */
	evp = VTOEV(newvp);
    }
#endif	/* AFS_AIX32_ENV, AFS_HPUX_ENV */

    *vpp = EVTOV(evp);
    afsl_Assert (OSI_VP_TO_VFSP(*vpp) == OSI_VP_TO_VFSP(dvp));

out:
    /* Release lock on parent directory */
    tlock_ReleaseRead (&devp->vd_tlock);

    osi_UnmakePreemptionRight();
    return (code);
}

/*
 * efs_create -- create a new regular episode file
 *
 * PARAMETERS -- it is important to distinguish between two file access modes:
 *		 vap->va_mode -- this will be the created file's mode
 *		 mode -- this is derived from the O_XXXX arguments to the
 *			 open or creat function, and it represents the way
 *			 the creating process plans to access the file
 */

/* EXPORT */
int efs_create(dvp, fname, vap, exclusive, mode, vpp, cred)
    struct vnode *dvp;			/* directory vnode */
    char *fname;			/* new file name */
    struct vattr *vap;			/* file attributes */
    enum vcexcl exclusive;
    int mode;				/* file access mode */
    struct vnode **vpp;			/* pointer to new vnode */
    osi_cred_t  *cred;		/* who i am */
{
    struct evnode *evp;			/* pointer to new evnode */
    struct vnode *vp;			/* vnode equivalent to evp */
    epi_anode_t newFile;
    epi_volFileId_t newFid;
    epi_volFileId_t entFid;
    epi_uuid_t realm;
    int auth;
    logTranRec_t transId;		/* transaction ID */
    int32 offset;			/* piece # at which to insert */
    int dxflags, code;
    long ccode;
    struct evnode *devp = VTOEV(dvp);	/* directory evnode */
    u_int umask = osi_GetCMASK();
    int state = 0;			/* state var. for cleanup */

    osi_MakePreemptionRight();
    *vpp = 0;
    transId = elbt_NullTranId;

    /* Check new file name validity */
    code = vnd_CheckFilenameLength(fname);
    if (!code) {
	/* Check: creating a directory */
	if (vap->va_type == VDIR)
	    code = EISDIR;
	else {
	    /* De-phantomize parent directory */
	    code = EV_CHECKSTALE(devp);
	}
    }
    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    /* Obtain lock on parent directory */
    tlock_ObtainWriteNT(&devp->vd_tlock);

    /* Determine if file exists */
    code = vnd_dirCheck(devp, fname, cred, &entFid, DXC_CREATING,
			&offset, &dxflags);

    if (code == 0 && dxflags & DX_OBJEXISTS) { /* fname exists in parent dir */
	/* Check: exclusive create and entry exists */
	if (exclusive == EXCL)
	    code = EEXIST;
	else {
	    /* Get vnode for file */
	    code = vnm_FindVnode(devp->vd_volp, &entFid, &evp);
	    vp = EVTOV(evp);
	}

	if (code == 0)
	    state = 1;		/* vnode is held */

	if (code == 0) {
	    /* Obtain lock on file */
	    EFS_VMLEN_WLOCK(evp);
	    tlock_ObtainWriteNT(&evp->vd_tlock);
	    state = 2;		/* vnode is locked */
	}

	/* Check: trying to overwrite directory */
	if (code == 0 && EV_ISDIR(evp) && (mode & VWRITE))
	     code = EISDIR; /* can not overwrite the dir */

	/* Verify access right for file */
	if (code == 0 && mode) {
	    dacl_permset_t permset = 0;		/* temp perm set */
	    dacl_AddPermBitsToOnePermset_r(mode, &permset);
	    code = vnax_CanAccess(evp, NULL, permset, cred);
	}

	/* Truncate file if asked */
	if (code == 0 && EV_ISREG(evp) &&
	    osi_setting_size(vap) && vap->va_size == 0) {
	    code = vnm_Truncate(evp, (u_long)0, TRUNCATE_USEVM, cred);
	    if (!code)
		vnm_SetMCtime (evp, MARK_MC);
	}
	/* Release lock on file */
	if (code == 0) {
	    tlock_ReleaseWriteNT (&evp->vd_tlock);
	    EFS_VMLEN_WUNLOCK(evp);
	    state = 0;
	    goto done;
	}
    }  /* dxflags & DX_OBJEXISTS */

    if (code != 0)
	goto out;

    /* Get a vnode */
    evp = vnm_Allocate();
    if (!evp) {
	code = ENFILE;
	goto out;
    }
    vp = EVTOV(evp);

    /* Start a transaction */
    code = epia_StartTran("efs_create", EVTOVH(devp), &transId);
    afsl_MBZ(code);

    /* Set owner; non-trivial in DFS context */
    vnax_GetOwner(cred, devp, vap, &realm, &auth);

    /* Create file */
    code = vnva_FileCreate(devp, vap, &newFid, &newFile, dxflags,
			   transId, umask);
    if (code) {
	VN_RELE (vp);
	goto out;
    }

    /* Let vnode represent file. */
    code = vnm_SetIdentity (evp, devp->vd_volp, &newFid, newFile);
    afsl_MBZ(code);

    /* If inheriting ACL, intersect mode bits with corresponding ACL bits */
    if (dxflags & DX_INHERITFILEACL) {
	code = vnax_InheritAclMode(evp, transId);
	if (code)
	    goto badinsert;
    }

    /* For special files, record rdev */
    if (ISSPECTYPE(vap->va_type)) {
	struct uio uio;
	struct iovec iov;
	dev_t sfdev;
	evp->vd_rdev = sfdev = vap->va_rdev;
	epia_InitUio(&uio, &iov, 0, sizeof (sfdev), (char *)&sfdev);
	BEGIN_LOCK_WRITE (&evp->vd_file.lock) {
	    code = epia_Write(transId, EVTOA(evp), 0, &uio);
	} END_LOCK;
	MBZ (code);
    }

    /* Special ACL hack for foreign users */
    /*
     * vnax_CorrectRealm
     * compares creator's realm against acl's default realm and not storing
     * cell and change acl's realm if necessary.
     */
    code = vnax_CorrectAclRealm (evp, &realm, auth, transId);
    if (code)
	goto badinsert;

    /* Insert file in parent directory */
    code = vnd_Create(transId, devp, evp, fname, offset, 0);
    if (code)
	goto badinsert;
    
    /* Update the child's times, but leave the parent's times cached because
     * the times are the only change being made to the directory's anode
     * (compare mkdir). */

    vnm_UpdateAnode (evp, transId);

done:

    osi_dnlc_enter_r(EVTOV(devp), fname, vp);

    /*
     * Handle device special files and FIFO's.  Note that AIX 3.2 does this
     * above the vnode interface.
     */

#if	!defined(AFS_AIX32_ENV) && !defined(AFS_HPUX_ENV)
    if (EV_ISVDEV(evp)) {
	struct vnode *newvp;
#ifdef AFS_OSF_ENV
	newvp = vp;
	if (EV_ISFIFO(evp)) {
	   newvp->v_op = (struct osi_vnodeops *)xvfs_ops3;
	} else {
	   code = specalloc(newvp, evp->vd_rdev);
	   newvp->v_op = (struct osi_vnodeops *)xvfs_ops2;
	}
#else
	newvp = specvp(vp, evp->vd_rdev, vp->v_type, cred);
	VN_RELE(vp);
#endif /* AFS_OSF_ENV */
	evp = NULL;			/* No longer an Episode vnode */
	vp = newvp;			/* return spec vnode */
    }
#endif	/* AFS_AIX32_ENV, AFS_HPUX_ENV */

    *vpp = vp;

#ifdef AFS_OSF_ENV
    if (vp->v_type == VSOCK)
	vp->v_socket = (struct socket *)vap->va_socket;
#endif /* AFS_OSF_ENV */

    /*
     * Used to call efs_getattr (or VOP_GETATTR) here, but none of our
     * supported platforms require this any more.
     */
    goto out;

badinsert:
    ccode = epif_ChangeLink(transId, EVTOA (evp), -1, 0, (u_int *) 0);
    MBZ (ccode);
    state = 1;

out:
    /* Release lock on parent directory */
    tlock_ReleaseWrite(transId, &devp->vd_tlock);

    /* End transaction */
    if (transId != elbt_NullTranId) {

	/* Merge transaction */
	tlock_EquivAssign(transId, &devp->vd_tlock);
	ccode = epia_EndTran(EVTOVH(devp), transId);
	afsl_MBZ(ccode);
    }
    if (state >= 2)
	tlock_ReleaseWriteNT(&evp->vd_tlock);
    if (state >= 1)
	VN_RELE(vp);

    if (efs_SyncMetaData(devp)) {
	ccode = elbb_LogForce (transId, 1/*commit*/);
	MBZ(ccode);
    }

    if (code == 0) {
	icl_Trace4(efs_iclSet2, EFS_TRACE_CREATE_SUCCESS,
		ICL_TYPE_STRING, fname, ICL_TYPE_POINTER, devp,
		ICL_TYPE_LONG, devp->vd_fid.index, ICL_TYPE_POINTER, vp);
    } else {
	icl_Trace4(efs_iclSet2, EFS_TRACE_CREATE_ERROR,
	   ICL_TYPE_STRING, fname, ICL_TYPE_POINTER, devp,
	   ICL_TYPE_LONG, devp->vd_fid.index, ICL_TYPE_LONG, code);
    }
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * efs_remove -- delete a regular file
 */

/* EXPORT */
int efs_remove(dvp, fname, cred)
    struct vnode *dvp;			/* directory vnode */
    char *fname;			/* file entry to be removed */
    osi_cred_t *cred;			/* who I am */
{
    epi_volFileId_t entFid;		/* file ID of entry */
    logTranRec_t transId;		/* transaction ID */
    struct evnode *evp;
    int32 offset;
    int dxflags, code = 0;
    long ccode;
    struct evnode *devp;		/* directory evnode */
    int evpexist = 0;

    osi_MakePreemptionRight();
    devp = VTOEV(dvp);
    code = EV_CHECKSTALE (devp);
    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    transId = elbt_NullTranId;

    /* Obtain lock on parent directory */
    tlock_ObtainWriteNT (&devp->vd_tlock);

    /* Determine if entry exists */
    code = vnd_dirCheck(devp, fname, cred, &entFid, DXC_ISDELETE,
			&offset, &dxflags);
    if (code)
	goto out;
    if (!(dxflags & DX_OBJEXISTS)) {
	code = ENOENT;
	goto out;
    }

    /* Get vnode representing subfile */
    code = vnm_FindVnode(devp->vd_volp, &entFid, &evp);
    if (code)
	goto out;

    /* Only super-user can unlink directory */
    if (EV_ISDIR (evp) && !(efs_allowSuserDirLinkOps && osi_suser(cred))) {
	code = EPERM;
	VN_RELE (EVTOV (evp));
	goto out;
    }

    /* Obtain lock on subfile */
    tlock_ObtainWrite(transId, &evp->vd_tlock);
    evpexist = 1;

    /* Check for sticky bit */
    code = vnax_StickyCheck (devp, evp, cred);
    if (code)
	goto out;

    /* Start a transaction */
    code = epia_StartTran("efs_remove", EVTOA(devp), &transId);
    afsl_MBZ(code);

    /* Do the unlink */
    code = vnd_Delete(transId, devp, evp, offset, 0);
    tlock_EquivAssign(transId, &devp->vd_tlock);

out:
    /* End transaction */
    if (transId != elbt_NullTranId) {
	ccode = epia_EndTran (EVTOA(devp), transId);
	afsl_MBZ(ccode);
    }

    osi_dnlc_remove_r(dvp, fname);

    /* Release lock on parent directory */
    tlock_ReleaseWriteNT(&devp->vd_tlock);

    /* Release lock on subfile */
    if (evpexist) {
	tlock_ReleaseWriteNT (&evp->vd_tlock);
	VN_RELE(EVTOV(evp));
    }

    if (efs_SyncMetaData(devp)) {
	ccode = elbb_LogForce (transId, 1/*commit*/);
	MBZ(ccode);
    }

    osi_UnmakePreemptionRight();
    return(code);
}

/*
 * efs_mkdir -- create a new directory
 */

/* EXPORT */
int efs_mkdir(dvp, dirname, vap, vpp, cred)
    struct vnode *dvp;			/* directory vnode */
    char *dirname;			/* new directory name */
    struct vattr *vap;			/* file attributes */
    struct vnode **vpp;			/* pointer to new vnode */
    osi_cred_t *cred;			/* who I am */
{
    struct evnode *evp;			/* new directory evnode */
    struct epi_Dir_Handle newdirHandle; /* directory handle for evp */
    epi_anode_t newFile;
    epi_volFileId_t newFid;		/* file ID of new dir */
    epi_uuid_t realm;
    int auth;
    logTranRec_t transId;		/* transaction ID */
    int32 offset;			/* piece # at which to insert */
    int dxflags, code = 0;
    long ccode;
    struct evnode *devp;		/* evnode */
    unsigned int umask;

    osi_MakePreemptionRight();
    *vpp = 0;
    transId = elbt_NullTranId;

    /* Check new dir name validity */
    code = vnd_CheckFilenameLength(dirname);
    if (!code) {
	umask=osi_GetCMASK();
	devp = VTOEV(dvp);
	code = EV_CHECKSTALE (devp);
    }
    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    /* Obtain lock on parent directory */
    tlock_ObtainWriteNT (&devp->vd_tlock);

    /* Determine if entry exists */
    code = vnd_dirCheck(devp, dirname, cred, &newFid, DXC_CREATING,
			&offset, &dxflags);
    if (code)
	goto out;
    if (dxflags & DX_OBJEXISTS) {
	 /* the dirname exist in the parent dir */
	code = EEXIST;
	goto out;
    }

    /* Get a vnode */
    evp = vnm_Allocate();
    if (!evp) {
	code = ENFILE;
	goto out;
    }

    /* Start a transaction */
    code = epia_StartTran ("efs_mkdir", EVTOA (devp), &transId);
    afsl_MBZ(code);

    /* Set owner; non-trivial in DFS context */
    vnax_GetOwner (cred, devp, vap, &realm, &auth);

    /* Create subdirectory */
    code = vnva_FileCreate (devp, vap, &newFid, &newFile, dxflags,
			    transId, umask);
    if (code) {
	VN_RELE (EVTOV(evp));
	goto out;
    }

    /* Let vnode represent subdirectory */
    code = vnm_SetIdentity (evp, devp->vd_volp, &newFid, newFile);
    if (code) {
	VN_RELE (EVTOV(evp));
	goto out;
    }
    tlock_InitTrans (&evp->vd_tlock, transId);

    /* Initialize subdirectory */
    vndh_SetDirHandle(evp, &newdirHandle);
    code = dir_MakeDir((struct Dir_Handle *)&newdirHandle, &newFid,
			&devp->vd_fid, transId);
    if (code) goto badinsert;

    /* If inheriting ACL, intersect mode bits with corresponding ACL bits */
    if (dxflags & DX_INHERITDIRACL) {
	code = vnax_InheritAclMode (evp, transId);
	if (code) goto badinsert;
    }

    /* Special ACL hack for foreign users */
/*    dacl_GetLocalCellID((afsUUID*)&localCellID); */
/*    if (bcmp (&realm, &localCellID, sizeof (epi_uuid_t)) != 0) { */
	code = vnax_CorrectAclRealm (evp, &realm, auth, transId);
	if (code) goto badinsert;
/*    } */

    /* Insert subdirectory in parent directory */
    code = vnd_Create(transId, devp, evp, dirname, offset, DC_CHANGELINK);
    if (code) {
badinsert:
	ccode = epif_ChangeLink(transId, EVTOA (evp), -1, 0, (u_int *)0);
	afsl_MBZ(ccode);
	ccode = epia_EndTran(EVTOA(devp), transId);
	afsl_MBZ(ccode);
	transId = elbt_NullTranId;
	VN_RELE(EVTOV(evp));
	goto out;
    }
    *vpp = EVTOV(evp);

    /* Before we finish with this transaction update the time of both parent
     * and child, because the child is new and the parent's link count is
     * increased. */

    vnm_UpdateAnode (devp, transId);
    vnm_UpdateAnode (evp, transId);

out:
    /* Release lock on parent directory */
    tlock_ReleaseWrite (transId, &devp->vd_tlock);

    /* End transaction */
    if (transId != elbt_NullTranId) {
	ccode = epia_EndTran(EVTOA(devp), transId);
	afsl_MBZ(ccode);
	if (code == 0)
	    osi_dnlc_enter_r(dvp, dirname, EVTOV(evp));
    }

    if (efs_SyncMetaData(devp)) {
	ccode = elbb_LogForce (transId, 1/*commit*/);
	MBZ(ccode);
    }

    osi_UnmakePreemptionRight();
    return (code);
}


/*
 * efs_rmdir -- delete a directory
 */

/* EXPORT */
int efs_rmdir(dvp, dirname, cdir, cred)
    struct vnode *dvp;			/* directory vnode */
    char   *dirname;			/* dir entry to be removed */
    struct vnode *cdir;			/* current working dir or null */
    osi_cred_t  *cred;			/* who I am */
{
    epi_volFileId_t entFid;		/* file ID of entry */
    int32 offset;			/* piece no. of entry */
    logTranRec_t transId;		/* transaction ID */
    struct evnode *evp;			/* dir evnode to be removed */
    int dxflags, code = 0;
    int evpexist = 0;
    long ccode;
    struct evnode *devp;		/* directory evnode */

    osi_MakePreemptionRight();
    devp = VTOEV(dvp);
    code = EV_CHECKSTALE (devp);
    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    transId = elbt_NullTranId;

    /* Obtain lock on parent directory */
    tlock_ObtainWriteNT (&devp->vd_tlock);

    /* Determine if entry exists */
    code = vnd_dirCheck(devp, dirname, cred, &entFid, DXC_ISDELETE,
			&offset, &dxflags);
    if (code)
	goto out;
    if (!(dxflags & DX_OBJEXISTS)) {
	code = ENOENT;
	goto out;
    }

    /* Get vnode representing subdirectory */
    code = vnm_FindVnode(devp->vd_volp, &entFid, &evp);
    if (code)
	goto out;

    /* Check against cdir */
    if (cdir && cdir == EVTOV (evp)) {
	code = EINVAL;
	VN_RELE (EVTOV(evp));
	goto out;
    }

    /* Check: object must be directory */
    if (!EV_ISDIR (evp)) {
	code = ENOTDIR;
	VN_RELE (EVTOV (evp));
	goto out;
    }

    /* Obtain lock on subdirectory */
    tlock_ObtainWrite (transId, &evp->vd_tlock);
    evpexist = 1;

    /* Check for sticky bit */
    code = vnax_StickyCheck (devp, evp, cred);
    if (code)
	goto out;

    /* Start a transaction */
    code = epia_StartTran("efs_rmdir", EVTOA(devp), &transId);
    afsl_MBZ(code);

    /* Check: subdirectory is mounted on */
    if (OSI_IS_MOUNTED_ON (EVTOV (evp))) {
	code = EBUSY;
	goto out;
    }

    /* Do the unlink */
    code = vnd_Delete(transId, devp, evp, offset, 1);

    /* Since parent is being modified anyway, include [mc]time, however, child
     * is being deleted so don't bother. */
    vnm_UpdateAnode (devp, transId);

out:
    /* Release lock on parent directory */
    tlock_ReleaseWrite (transId, &devp->vd_tlock);

    /* End transaction */
    if (transId != elbt_NullTranId) {
	ccode = epia_EndTran(EVTOA(devp), transId);
	afsl_MBZ(ccode);
    }

    osi_dnlc_remove_r(dvp, dirname);

    /* Release lock on subdirectory */
    if (evpexist) {
	tlock_ReleaseWriteNT (&evp->vd_tlock);
	VN_RELE(EVTOV(evp));
    }

    if (efs_SyncMetaData(devp)) {
	ccode = elbb_LogForce (transId, 1/*commit*/);
	MBZ(ccode);
    }

    osi_UnmakePreemptionRight();
    return (code);
}


/*
 * efs_link -- link a regular episode file or a directory
 */

/* EXPORT */
int efs_link(svp, tdvp, fname, cred)
    struct vnode *svp;			/* source object vnode */
    struct vnode *tdvp;			/* target directory vnode */
    char *fname;			/* source object name to be linked */
    osi_cred_t *cred;			/* who I am */
{
    epi_anode_t sap;			/* anode handle of sevp */
    epi_volFileId_t entFid;		/* dummy argument to vnd_dirCheck */
    int32 offset;			/* piece # at which to insert */
    logTranRec_t transId;		/* transaction ID */
    int dxflags;
    int code = 0;
    long ccode;
    struct evnode *sevp;		/* source object evnode */
    struct evnode *tdevp;		/* target directory evnode */

    osi_MakePreemptionRight();

    /* Check new file/dir name validity */
    code = vnd_CheckFilenameLength(fname);
    if (!code) {
	sevp = VTOEV(svp);
	code = EV_CHECKSTALE (sevp);
    }
    if (!code) {
	tdevp = VTOEV(tdvp);
	code = EV_CHECKSTALE (tdevp);
    }
    if (!code) {
	/* Check: trying to link directory */
	if (EV_ISDIR (sevp) &&
	    !(efs_allowSuserDirLinkOps && osi_suser(cred)))
	    code = EPERM;
    }
    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    sap = EVTOA(sevp);

    /*
     * Lock both vnodes.
     *
     * This is unconventional.  We do not need to lock the vnodes
     * simultaneously to keep them consistent with each other.  It would
     * be sufficient to lock one, then unlock it, then lock the other, then
     * unlock the other.  But that would force us to violate our resource
     * hierarchy:  vnode locks must be obtained before transactions are
     * started.  So we obtain both locks first.
     */
    tlock_ObtainDoubleWriteNT (&sevp->vd_tlock, &tdevp->vd_tlock);

    /* Determine if entry exists */
    code = vnd_dirCheck (tdevp, fname, cred, &entFid, 0, &offset, &dxflags);
    if (code)
	goto out1;
    if (dxflags & DX_OBJEXISTS) {
	code = EEXIST;
	goto out1;
    }

    /* Check:  trying to link to a different volume */
    if (sevp->vd_volp != tdevp->vd_volp) 
	code = EXDEV;
    else {
	/* Start a transaction */
	code = epia_StartTran("efs_link", sap, &transId);
	afsl_MBZ(code);

	/* Insert source file into target directory */
	code = vnd_Create(transId, tdevp, sevp, fname, offset, DC_MARKCTIME);
	if (!code) {
	    /* Increment file's link count */
	    code = epif_ChangeLink(transId, sap, 1, 0, (u_int *) 0);
	    afsl_MBZ(code);
	    vnm_UpdateAnode (sevp, transId);
	}
    }

    /* Release lock on source file */
    tlock_ReleaseWrite (transId, &sevp->vd_tlock);

    /* Release lock on target directory */
    tlock_ReleaseWrite (transId, &tdevp->vd_tlock);

    /* End transaction */
    ccode = epia_EndTran(sap, transId);
    afsl_MBZ(ccode);

    if (code == 0)
	osi_dnlc_enter_r(tdvp, fname, svp);

    if (efs_SyncMetaData(sevp)) {
	ccode = elbb_LogForce (transId, 1/*commit*/);
	MBZ(ccode);
    }

    osi_UnmakePreemptionRight();
    return (code);

out1:
    tlock_ReleaseWriteNT (&sevp->vd_tlock);
    tlock_ReleaseWriteNT (&tdevp->vd_tlock);
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * efs_symlink -- create a symbolic link
 */

/* EXPORT */
int efs_symlink(tdvp, linkfname, vap, symstring, cred)
    struct vnode *tdvp;			/* directory vnode */
    char *linkfname;			/* name of symbolic link */
    struct vattr *vap;			/* file attributes */
    char *symstring;			/* contents of symbolic link */
    osi_cred_t *cred;			/* who I am */
{
    struct evnode *evp;			/* vnode for symbolic link */
    epi_anode_t newFile;
    epi_volFileId_t newFid;
    epi_volFileId_t entFid;
    logTranRec_t transId;		/* transaction ID */
    int dxflags, code = 0;
    long offset;
    long ccode;
    struct uio uio;
    struct iovec iov;
    struct evnode *tdevp;		/* directory evnode */

    osi_MakePreemptionRight();

    /* Check new symlink name validity */
    code = vnd_CheckFilenameLength(linkfname);
    if (!code) {
	transId = elbt_NullTranId;
	tdevp = VTOEV(tdvp);
	code = EV_CHECKSTALE (tdevp);
    }
    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    /* Obtain lock on parent directory */
    tlock_ObtainWriteNT (&tdevp->vd_tlock);

    /* Determine if entry exists */
    code = vnd_dirCheck (tdevp, linkfname, cred, &entFid, 0, &offset, &dxflags);
    if (code)
	goto out;
    if (dxflags & DX_OBJEXISTS) {
	code = EEXIST;
	goto out;
    }

    /* Get a vnode */
    evp = vnm_Allocate ();
    if (!evp) {
	code = ENFILE;
	goto out;
    }

     /* Start a transaction */
    code = epia_StartTran ("efs_symlink", EVTOA(tdevp), &transId);
    afsl_MBZ(code);

    /* Create symlink */
    vap->va_type = VLNK;
    /* Set owner; non-trivial in DFS context */
    vnax_GetOwner (cred, tdevp, vap, (epi_uuid_t *) 0, (int *) 0);
    code = vnva_FileCreate (tdevp, vap, &newFid, &newFile, 0,
			    transId, 0 /* umask of 0 */ );
    if (code) {
	VN_RELE (EVTOV (evp));
	goto out;
    }

    /* Let vnode represent symlink */
    code = vnm_SetIdentity (evp, tdevp->vd_volp, &newFid, newFile);
    MBZ (code);

    /* Insert symlink in parent directory */
    code = vnd_Create (transId, tdevp, evp, linkfname, offset, 0);
    if (code)
	goto out2;
    vnm_UpdateAnode (evp, transId);

    /* Merge transaction */
    tlock_EquivAssign (transId, &tdevp->vd_tlock);

    /* End transaction */
    ccode = epia_EndTran (EVTOA(tdevp), transId);
    afsl_MBZ(ccode);

    /* Write contents of symlink */
    epia_InitUio (&uio, &iov, 0, strlen (symstring), symstring);
    BEGIN_LOCK_WRITE (&evp->vd_file.lock) {
	code = epia_Write (elbt_NullTranId, EVTOA(evp), 0, &uio);
    } END_LOCK;

    /* Translate error codes */
    if (code) {
	VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);

	/* Could not write symlink; must delete it */
	ccode = epia_StartTran ("efs_symlink", EVTOA (tdevp), &transId);
	afsl_MBZ(ccode);
	(void) vnd_Delete (transId, tdevp, evp, offset, 0);
	tlock_EquivAssign (transId, &tdevp->vd_tlock);
	epia_EndTran (EVTOA (tdevp), transId);
    }

    if (code == 0)
	osi_dnlc_enter_r(tdvp, linkfname, EVTOV(evp));

    /* Release lock on parent directory */
    tlock_ReleaseWriteNT (&tdevp->vd_tlock);

    VN_RELE (EVTOV (evp));

    if (efs_SyncMetaData(tdevp)) {
	ccode = elbb_LogForce (transId, 1/*commit*/);
	MBZ(ccode);
    }

    osi_UnmakePreemptionRight();
    return (code);

out2:
    ccode = epif_ChangeLink(transId, EVTOA (evp), -1, 0, (u_int *) 0);
    afsl_MBZ(ccode);
    ccode = epia_EndTran (EVTOA (tdevp), transId);
    afsl_MBZ(ccode);
    transId = elbt_NullTranId;
    VN_RELE (EVTOV (evp));
out:
    tlock_ReleaseWrite (transId, &tdevp->vd_tlock);
    if (transId != elbt_NullTranId) {
	ccode = epia_EndTran (EVTOA (tdevp), transId);
	afsl_MBZ(ccode);
    }
    osi_UnmakePreemptionRight();
    return (code);
}

/*
 * efs_readdir -- read directory entries
 *
 * This procedure reads the episode directory entries and returns them
 * to the caller in the generic UNIX directory entry format.
 *
 * XXX There are really three flavors of "generic" directory entry:
 * the format used by the local OS, the DFS over-the-wire format, and
 * a third format used by volume operations.  The isPX parameter here
 * distinguishes between the first two cases; the volume ops call vnd_Read
 * directly with the fourth argument set to one.
 *
 * XXX It is unfortunate that we don't define any symbolic names
 * for the fourth argument to vnd_Read.
 *
 * The number of entries to be returned to the caller is limited by the
 * size of the buffer provided by the user.  Empty directory entries will
 * not be returned to the caller.  As a result, before returning, we
 * adjust uio_offset to an appropriate point.  We may pad the remaining
 * user buffer with zeros, if necessary.  All of this work takes place
 * in vnd_Read (which is where this comment really belongs).
 */

/* EXPORT */
int efs_readdir(dvp, uiop, cred, eofp, isPX)
    struct vnode *dvp;			/* target directory vnode */
    register struct uio *uiop;		/* pointer to uio data structure */
    osi_cred_t  *cred;			/* who I am  */
    int *eofp;				/* reached EOF? */
    int isPX;				/* called by PX */
{
    struct evnode *devp = VTOEV(dvp);
    int code;

    osi_MakePreemptionRight();
    /* De-phantomize */
    code = EV_CHECKSTALE(devp);
    if (!code)
	code = vnd_Read(devp, uiop, cred, isPX ? 2 : 0, 0, eofp);
    osi_UnmakePreemptionRight();
    return (code);
}

/*
 * efs_rename -- rename a vnode
 *
 * LOCKING:
 * efs_rename presents special locking problems.  The transaction
 * model requires that we lock both the source and target directories
 * before making any updates, and of course, we must do this without
 * deadlocking.  Moreover, when renaming a directory, we may have to
 * examine (and lock) a large number of directories to make sure that
 * the target is not a descendant of the source.  We handle this in
 * vnd_Rename, where we do the validity checks iteratively.
 * If we discover that a lock we need is held by someone else, we
 * drop the locks we already have, wait for the held lock, and then
 * start all over again.
 */
int
efs_rename(
    struct vnode *sdp,			/* source directory */
    char *sname,			/* name of source object */
    struct vnode *tdp,			/* target directory */
    char *tname,			/* name of target object */
    osi_cred_t *cred)			/* credential structure */
{
    struct evnode *sd = VTOEV(sdp);	/* source directory evnode */
    struct evnode *td = VTOEV(tdp);	/* target directory evnode */
    int code;				/* error return code */

    osi_MakePreemptionRight();
    code = EV_CHECKSTALE(sd);
    if (!code) {
	code = EV_CHECKSTALE(td);
	if (!code) {
	    code = vnd_Rename(sd, sname, td, tname, cred);
	}
    }
    if (code) {
	icl_Trace4(efs_iclSet2, EFS_TRACE_RENAME_ARGS,
		   ICL_TYPE_POINTER, sd,
		   ICL_TYPE_STRING, sname,
		   ICL_TYPE_POINTER, td,
		   ICL_TYPE_STRING, tname);
	icl_Trace1(efs_iclSet2, EFS_TRACE_RENAME_ERROR, ICL_TYPE_LONG, code);
    }
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * efs_readlink -- Read symbolic link
 */

/* EXPORT */
/* ARGSUSED */
int efs_readlink(vp, uiop, cred)
    struct vnode *vp;			/* link vnode */
    struct uio *uiop;			/* uio structure */
    osi_cred_t *cred;			/* credential structure */
{
    int code = 0;			/* error return code */
    struct evnode *evp;			/* link evnode */
    u_long diff;			/* amount requested but not read */

    osi_MakePreemptionRight();
    evp = VTOEV(vp);
    /* Sanity check:  is it a symbolic link? */
    if (!EV_ISLNK (evp)) {
	code = EINVAL;
    } else {
	code = EV_CHECKSTALE (evp);
    }
    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    /* Obtain lock on symlink */
    tlock_ObtainRead (&evp->vd_tlock);

    BEGIN_LOCK_READ (&evp->vd_file.lock) {
	epi_anode_t ap = EVTOA (evp);
	u_long fileLen;			/* short length */

	/* Adjust request so that it doesn't go past EOF */
	fileLen = AFS_hgetlo(epia_GetLength (ap));
	/* the following code assumes offset is zero */
	if (uiop->uio_resid > fileLen) {
	    diff = uiop->uio_resid - fileLen;
	    uiop->uio_resid = fileLen;
	} else
	    diff = 0;

	/* Read the data (calling anode layer) */
	code = epia_Read(ap, 0, uiop);
    } END_LOCK;

    /* Translate error codes */
    if (code) {
	VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);
    } else {
	uiop->uio_resid += diff;
    }

    /* Update cached atime */
    vnm_SetAtime (evp);

    /* Release lock on symlink */
    tlock_ReleaseRead (&evp->vd_tlock);

    osi_UnmakePreemptionRight();
    return (code);
}

/*
 * efs_fsync -- update disk version of a file
 *
 * LOCKS USED -- this function doesn't ever obtain the vnode's tlock.
 *		 Perhaps, however, it is necessary to lock the vnode between
 *		 flushing VM and unreserving storage.  I don't know.
 */

/* EXPORT */
int efs_fsync(vp, cred)
    struct vnode *vp;			/* vnode */
    osi_cred_t *cred;			/* credential structure */
{
    int code;
    struct evnode *evp;		 /* evnode */

    osi_MakePreemptionRight();
    evp = VTOEV(vp);
    code = EV_CHECKSTALE (evp);
    if (!code) {
	BEGIN_LOCK_WRITE(&evp->vd_file.lock) {
	    vnm_UpdateAnode (evp, elbb_zeroTrans);
	} END_LOCK;
	code = vnvm_fsync (vp, cred);
    }

    if (!code) {

	/* Flush all buffers associated with the vnode and make sure metadata
         * updates are persistent. */

	code = epia_Fsync (EVTOA (evp));
	afsl_MBZ(code);
	code = elbb_LogForce (tlock_GetTrans (&evp->vd_tlock), 1/*commit*/);
	afsl_MBZ(code);
    }
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * efs_inactive -- Make note of an inactive vnode
 *
 * This is called by any process that releases the last reference
 * to a vnode.  On multithreaded systems, the mechanism for releasing
 * a reference must be atomic, so that two callers do not enter this
 * function (with the same vnode) at once.
 *
 * On SunOS, efs_inactive has the responsibility of writing out the
 * vnode's dirty pages when it becomes inactive.  This can entail a
 * large amount of work, and moreover, it can come at inconvenient
 * times.  Very long code paths can give us kernel stack overflows
 * (and hence watchdog resets), and calling VOP_INACTIVE from within
 * the I/O path can lead to deadlock when we reenter the strategy
 * routines recursively.  For these reasons, we use a separate thread
 * to do the real work of making the vnode inactive; efs_inactive
 * puts the vnode on a queue.
 *
 * On AIX, in contrast, an inactive vnode may retain dirty pages.
 * Thus, we must be prepared to handle pageouts after the vnode is
 * ostensibly inactive; in fact, because the AIX VM system does
 * asynchronous fault-ahead, we may also have to handle pageins.
 * In both cases, the VM system _does not_ increment the vnode
 * ref. count when it does the I/O.  Only when the vnode's VM resources
 * are deleted (by calling vms_delete) can we be sure that there will
 * be no further activity.  For this reason, we leave the vnode's
 * anode handle open until we either delete the anode (here) or recycle
 * the vnode (in Recycle).
 *
 * The "real work" of efs_inactive take place in efs_inactive_work
 * in all cases.
 */

/*
 * On SunOS, vn_rele increases the vnode ref. count to one before calling
 * VOP_INACTIVE so that subsequent VN_HOLD/VN_RELE pairs on the vnode don't
 * reenter us before we are done; VOP_INACTIVE is responsible for lowering
 * the ref. count to zero before returning.  We follow the same convention
 * on AIX.
 *
 * On other platforms we just do a FAST_VN_HOLD before proceeding.
 */

/* LOCKS USED -- on SunOS5 we are not called with the preemption_lock held
 *     (becasue there, historically, was no glue for inactive). */

/* EXPORT */
int
efs_inactive(vp, cred)
    struct vnode *vp;
    osi_cred_t *cred;
{
    int code;

#if !defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX_ENV)
    /* XXX -- preemption? */
    FAST_VN_VOLD (vp);
#endif

#ifdef AFS_SUNOS5_ENV
    /* preemption should be emabled */
    code = vnm_Inactive(vp);

#else
    /* XXX -- preemption? */
    code = vnm_Inactive(vp);
#endif /* AFS_SUNOS5_ENV */

    return code;
}

/*
 * efs_bmap -- map logical block to physical block
 *
 * Implementation note:  epia_Map arguments are in units of Episode block
 * size.  efs_bmap input block number is in units of Episode block size,
 * but output block number is in units of the native block size.
 *
 * Implementation note:  we return the block-device vnode.  In the remote
 * filesystem (cache manager), a more general implementation method is used:
 * we return the input parameter vnode, and the remote filesystem has its own
 * strategy function, which calls that filesystem's rdwr function to read the
 * data.  That approach is necessary for the remote filesystem, but would
 * be inappropriate for a physical filesystem, since it would interpose an
 * unnecessary layer of buffering into text I/O.
 */

#if !defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX_ENV)

/* EXPORT */
int efs_bmap(vp, lbn, vpP, pbnP)
    struct vnode *vp;			/* vnode */
    daddr_t lbn;			/* logical block number */
    struct vnode **vpP;			/* place to put device vnode */
    daddr_t *pbnP;			/* place to put physical block number */
{
    struct async_device *dev;		/* Episode async device handle */
    daddr_t epbn;			/* Episode physical block number */
    int code;				/* error return code */
    struct evnode *evp;			/* evnode */

    osi_MakePreemptionRight();
    code = EV_CHECKSTALE (evp);
    if (!code && vpP) {
	struct osi_vfs *vfsp;		/* mounted virtual file system */
	struct efsMOUNT *mp;		/* private data for vfsp */

	/* Determine device vnode */

	vfsp = EVTOVFSP (evp);
	afsl_Assert (!EPI_ISNULLVFSP (vfsp));
	mp = VFSTOEFS (vfsp);
	*vpP = mp->m_devvp;
    }

    if (!code && pbnP) {
	/* Determine logical block number */
	BEGIN_LOCK_READ (&evp->vd_file.lock) {
	    code = epia_Map (EVTOA (evp), lbn, &epbn, &dev);
	} END_LOCK;
	if (code && code != EPI_E_NOBLOCK) panic ("efs_bmap");
	if (code)
	    *pbnP = -1;
	else
	    *pbnP = btodb (epbn << dev->logBlkSize);
    }

    osi_UnmakePreemptionRight();
    return code;
}

#endif /* AFS_SUNOS5_ENV, AFS_AIX_ENV */

/*
 * efs_bread -- get logical buffer for file
 *
 * This function is used on some systems to get data by the buffer-full
 * for NFS.  Not all the fields of the struct buf have to be valid.  In
 * particular, we only fill in the b_addr field.
 *
 * We obtain a struct buffer by calling epia_Map.  But NFS doesn't want
 * a struct buffer; it only understands struct bufs.  We could allocate
 * a struct buf here with its own data area, but that would require a bcopy
 * of the data from the struct buffer.  Instead, the struct buf uses the
 * struct buffer's data area.  This requires us to hold the struct buffer,
 * until we are ready to release the struct buf (i.e. until NFS calls
 * efs_brelse).  That is the reason for the shenanigans with a struct efs_buf.
 */

#if !defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX_ENV)

extern struct buf *geteblk(int size);

/* EXPORT */
int efs_bread(vp, lbn, bpP, sizeP)
    struct vnode *vp;			/* vnode */
    daddr_t lbn;			/* logical block # */
    struct buf **bpP;			/* place to put buffer ptr */
    long *sizeP;			/* place to put buffer size -- NOT USED */
{
    daddr_t pbn;			/* physical block # */
    struct async_device *dev;		/* device */
    struct efs_buf *ebp;		/* extended buf structure */
    struct buffer *ebuf;		/* Episode buffer */
    struct buf *bp;			/* Unix buffer */
    int code = 0;
    struct evnode *evp;			/* evnode */

    osi_MakePreemptionRight();
    evp = VTOEV(vp);
    code = EV_CHECKSTALE (evp);
    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    /* Obtain lock on file */
    tlock_ObtainRead (&evp->vd_tlock);

    /* Determine device and physical block number */
    BEGIN_LOCK_READ (&evp->vd_file.lock) {
	code = epia_Map (EVTOA (evp), lbn, &pbn, &dev);
	if (code == 0) {
	    /* Read the block (calling logbuf layer) */
	    code = buffer_Read (dev, pbn, elbb_bufData, &ebuf);
	}
    } END_LOCK;

    if (code == EPI_E_NOBLOCK) {

	/* If epia_Map says its a hole, return a block of zeros */

	bp = geteblk(dev->blkSize);
#ifndef AFS_OSF_ENV
	clrbuf(bp);
#endif /* AFS_OSF_ENV */
	goto out;
    } else if (code == BUF_BADBUF) {

	/* Serious logbuf failure translates to EIO. */

	EFS_TRACE_VNOPS(DEBUG_THIS_FILE, BUF_BADBUF, EIO);
	tlock_ReleaseRead (&evp->vd_tlock);
	osi_UnmakePreemptionRight();
	return (EIO);
    }
    /* no other errors are "reasonable". */
    MBZ (code);

    /* Make it look like a struct buf */
    ebp = vnm_AllocEBuffer ();
    ebp->epbuf = ebuf;
    bp = EBTOB (ebp);
    bp->b_un.b_addr = ebuf->data;
    bp->b_vp = (struct vnode *) bp;	/* mark as fake */

out:
    *bpP = bp;

    /* Update cached atime */
    vnm_SetAtime (evp);

    /* Release lock on file */
    tlock_ReleaseRead (&evp->vd_tlock);

    osi_UnmakePreemptionRight();
    return (0);
}

#endif /* AFS_SUNOS5_ENV, AFS_AIX_ENV */

/*
 * efs_brelse -- release logical buffer
 */

#if !defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX_ENV)

/* EXPORT */
int efs_brelse(vp, bp)
    struct vnode *vp;			/* vnode */
    struct buf *bp;			/* buffer */
{
    struct efs_buf *ebp;		/* extended buf structure */
    struct evnode *evp;			/* evnode */

    osi_MakePreemptionRight();
    evp = VTOEV(vp);
    code = EV_CHECKSTALE (evp);
    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    if (bp->b_vp == (struct vnode *) bp) { /* marked as fake */
	ebp = BTOEB (bp);
	elbb_Release(ebp->epbuf, elbb_lazy);
	vnm_FreeEBuffer (ebp);
    } else {				/* real Unix buffer */
#ifdef AFS_OSF_ENV
	brelse (bp);
#else
	ufs_brelse (bp);
#endif /* AFS_OSF_ENV */
    }
    osi_UnmakePreemptionRight();
    return 0;
}

#endif /* AFS_SUNOS5_ENV, AFS_AIX_ENV */

/*
 * efs_lockctl -- perform record locking functions
 * NOTE: the conditionals around the header have been moved to allow
 *       the tracing package to find the header correctly.
 *
 * This function relies on the common locking package (functions with
 * the vnl_ prefix), which is also used by the DFS Cache Manager.
 */

#ifndef AFS_SUNOS5_ENV

/* EXPORT */
int efs_lockctl(vp, flock, cmd, cred, id, offset)
    struct vnode *vp;			/* vnode */
    struct flock *flock;		/* file lock */
    int cmd;				/* op code */
    osi_cred_t *cred;			/* credential structure */
    int id;				/* locker? */
    long offset;			/* offset of FD for relative ops */
{
    int end;				/* index of last byte in record */
    int code;				/* error return code */
    struct record_lock *blocker;	/* lock blocking current request */
    struct squeue *marker;		/* placeholder on list of locks */
    struct record_lock *nl;		/* new record lock */
    struct evnode *evp;			/* evnode */

    /* Check: invalid length or starting byte */
    if (flock->l_len < 0 || flock->l_start < 0)
	return EINVAL;

    /* Check: overflow */
    if (flock->l_len == 0)
	end = MAXEND;
    else {
	end = flock->l_start + flock->l_len - 1;
	if (end < 0)
	    return EINVAL;
    }

    /* Check: invalid type */
    if (flock->l_type != F_UNLCK &&
	flock->l_type != F_WRLCK &&
	flock->l_type != F_RDLCK)
	return EINVAL;

    osi_MakePreemptionRight();
    evp = VTOEV(vp);
    vnl_idset (flock);

    if (cmd == F_GETLK) {		/* Find blocking lock */
	/* Obtain lock on file */
	tlock_ObtainRead (&evp->vd_tlock);

	/* Determine if request would cause blocking, and find blocker */
	if (vnl_blocked (flock, end, &evp->vd_flocks, &blocker, &marker)) {
	    *flock = blocker->data;
	    if (blocker->data_end == MAXEND)
		flock->l_len = 0;
	    else
		flock->l_len = blocker->data_end - blocker->data.l_start + 1;
	} else
	    flock->l_type = F_UNLCK;

	/* Release lock on file */
	tlock_ReleaseRead (&evp->vd_tlock);

	osi_UnmakePreemptionRight();
	return 0;
    }
    if (flock->l_type == F_UNLCK) {	/* Unlock */
	/* Obtain lock on file */
	tlock_ObtainWriteNT (&evp->vd_tlock);

	/* Release locks on byte range */
	code = vnl_adjust (flock, &end, &evp->vd_flocks,
			   &evp->vd_flocks, &marker);

	/* Clean up */
	goto out;
    }

    if (cmd == F_SETLK) {		/* Set lock without waiting */
	/* Obtain lock on file */
	tlock_ObtainWriteNT (&evp->vd_tlock);

	/* Check: request would block */
	if (vnl_blocked (flock, end, &evp->vd_flocks, &blocker, &marker)) {
	    code = EAGAIN;
	    goto out;
	}

	/* Satisfy request and clean up */
	goto addlock;
    }

    if (cmd == F_SETLKW) {		/* Set lock, wait if necessary */
	/* Obtain lock on file */
	tlock_ObtainWriteNT (&evp->vd_tlock);

	/* Loop while blocking */
	while (1) {
	    /* If possible, satisfy request and clean up */
	    if (!vnl_blocked (flock, end, &evp->vd_flocks, &blocker, &marker))
		goto addlock;

	    /* Wait for appropriate unlocks */
	    code = vnl_sleep (flock, end, blocker, &evp->vd_tlock.lock);
	    if (code) goto out;
	};
    }

    osi_UnmakePreemptionRight();
    return EINVAL;			/* if we get here,
					 * bad cmd or lock type */

addlock:
    /* Release any other locks on the byte range */
    code = vnl_adjust (flock, &end, marker, &evp->vd_flocks, &marker);
    if (code || !marker) goto out;

    /* Get a lock and initialize it */
    code = vnl_alloc (flock, end, &nl);
    if (code) goto out;
    nl->un.blocking = 0;

    /* Add the lock to the file's lock list */
    QAdd (marker, RLTOQ (nl));

out:
    /* Release lock on file */
    tlock_ReleaseWriteNT (&evp->vd_tlock);

    osi_UnmakePreemptionRight();
    return (code);
}

#endif	/* !AFS_SUNOS5_ENV */

/*
 * efs_fid -- file ID of vnode
 */

#if !defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX_ENV)

/* EXPORT */
int efs_fid(vp, fidpP)
    struct vnode *vp;			/* vnode */
    struct fid **fidpP;			/* place to put file ID ptr */
{
    struct fid *newfid;			/* file ID ptr */
    int fid_length;
    struct evnode *evp;			/* Episode vnode */
    int code;

    osi_MakePreemptionRight();
    evp = VTOEV(vp);
    code = EV_CHECKSTALE_NO_ANODE(evp);
    if (!code) {
	/*
	 * If there is structure padding, this may allocate a few more bytes
	 * than necessary, but this is simpler and more portable.
	 */
	fid_length = sizeof(struct fid);
#ifdef	KERNEL
	/* can't use osi_Alloc because it will be released by kmem_free */
	newfid = (struct fid *)kmem_alloc(fid_length);
#else
	newfid = osi_Alloc_r(fid_length);
#endif	/* KERNEL */
	newfid->fid_len = sizeof(epi_volFileId_t);
	bcopy((char *)&evp->vd_fid, newfid->fid_data, newfid->fid_len);
	*fidpP = newfid;
    }
    osi_UnmakePreemptionRight();
    return code;
}

#endif /* AFS_SUNOS5_ENV, AFS_AIX_ENV */

#ifdef AFS_AIX31_ENV
/* EXPORT */
int efs_fid(vp, fp)
    struct vnode *vp;
    struct fid *fp;
{
    int code;
    struct fileid *fidp = (struct fileid *)fp;
    efs_evnode_t *evp = VTOEV(vp);

    osi_MakePreemptionRight();
    code = EV_CHECKSTALE_NO_ANODE(evp);
    if (!code) {
	bzero (fidp, sizeof (*fidp));
	fidp->fid_len = MAXFIDSZ;
	fidp->fid_ino = epid_ToIndex (&evp->vd_fid);
	fidp->fid_gen = epid_ToUnique (&evp->vd_fid);
    }
    osi_UnmakePreemptionRight();
    return code;
}
#endif /* AFS_AIX31_ENV */


/*
 * efs_afsfid -- file ID of vnode
 */

/* EXPORT */
int efs_afsfid(vp, afsfidp, wantv)
    struct vnode *vp;			/* vnode */
    struct afsFid *afsfidp;		/* AFS standard fid */
    int wantv;				/* need cell/volume info? */
{
    efs_evnode_t *evp = VTOEV(vp);
    int code;

    osi_MakePreemptionRight();
    code = EV_CHECKSTALE_NO_ANODE(evp);
    if (!code) {
	if (wantv) {
	    VOLID_SET (afsfidp->Volume, evp->vd_volid);

	    /* Server's designation for local cell is (0,1) */

	    AFS_hset64(afsfidp->Cell, 0, 1);
	}
	afsfidp->Vnode = epid_ToIndex (&evp->vd_fid);
	afsfidp->Unique = epid_ToUnique (&evp->vd_fid);
    }
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * efs_GetVolume -- returns the (HELD) volume ptr for a vnode.
 *
 * LOCKS USED -- grabs the fileLock to prevent the fid from changing.  Actually
 *     any vnode lock below the idLock (which is used by recycle) or the
 *     vntableLock would work.  A problem is that (currently) VOL_HOLD grabs
 *     the volp->v_lock which is also used for other slow operations.  This
 *     might lead to performance problems if vntableLock was used in this
 *     situation.  See db6326 for more discussion of this issue. */

/* EXPORT */
int efs_GetVolume (
  struct vnode *vp,
  struct volume **volpP)		/* where to put volume ptr */
{
    int code;
    efs_evnode_t *evp = VTOEV(vp);

    code = 0;
    *volpP = NULL;
    BEGIN_LOCK_READ (&evp->vd_file.lock) {
	if (NoIdentity(evp)) {
	    icl_Trace1(efs_iclSet, EFS_TRACE_STALE, ICL_TYPE_POINTER, vp);
	    code = ESTALE;
	    EXIT_CRIT;
	}
	*volpP = evp->vd_volp;
	osi_UnmakePreemptionRight();
	VOL_HOLD(*volpP);
	osi_MakePreemptionRight();
    } END_LOCK;
    return code;
}

/* vnodeop version of above. */
int efs_getvolume (
  struct vnode *vp,
  struct volume **volpP)		/* where to put volume ptr */
{
    int code;
    osi_MakePreemptionRight();
    code = efs_GetVolume (vp, volpP);
    osi_UnmakePreemptionRight();
    return code;
}

#ifdef AFS_AIX_ENV
/*
 * efs_hold and efs_rele -- hold and release vnode
 */
/* EXPORT */
int efs_hold(vp)
    struct vnode *vp;
{
    /* XXX -- preemption? */
    FAST_VN_HOLD(VTOEV(vp));
    return 0;
}

/* EXPORT */
int efs_rele(vp)
  struct vnode *vp;
{
    int is_inactive;

    /* XXX -- preemption? */
    efs_lockvp(VTOEV(vp));
    afsl_Assert(vp->v_count != 0);
    vp->v_count -= 1;
    is_inactive = (vp->v_count == 0);	/* Dropping the last reference */
    if (is_inactive)
	vp->v_count = 1;
    efs_unlockvp(VTOEV(vp));

    if (is_inactive)
	return efs_inactive(vp, osi_getucred());
    else
	return 0;
}
#endif /* AFS_AIX_ENV */

u_long efs_GetLength(
  efs_vnode_t evp)
{
    u_long len;

    osi_assert((tlock_Check(&evp->vd_tlock) != 0) ||
	       (lock_Check(&evp->vd_file.lock) != 0) ||
	       (lock_Check(&evp->vd_vm.lock) != 0));
    afsl_Assert(!NoIdentity(evp) && evp->vd_file.anode);

    /* Determine file length */
    if (evp->vd_flags & VD_LENVALID)
	len = AFS_hgetlo(evp->vd_len);
    else
	len = epia_GetLen32(EVTOA(evp));

    return len;
}

/*
 * efs_getlength -- returns the 32-bit length of a vnode.
 *
 * XXX -- this should return the length as its value instead of
 *     a constant zero that has to be voided or MBZ'ed.
 *
 * LOCK USED -- the caller must hold at least one of the vnode, vm or file
 *     locks.
 */
/* ARGSUSED */
int efs_getlength(
    struct vnode *vp,
    long *lenp,
    osi_cred_t *cred)
{
    efs_evnode_t *evp = VTOEV(vp);

    osi_MakePreemptionRight();
    *lenp = efs_GetLength(evp);
    osi_UnmakePreemptionRight();
    return (0);
}

/*
 * efs_setlength -- sets the vnode's idea of the file's length.
 *
 * LOCK USED -- must be called with many locks held.
 */
/* ARGSUSED */
int efs_setlength(struct vnode *vp, long len, osi_cred_t *cred)
{
    struct evnode *evp = VTOEV(vp);

    afsl_Assert ((tlock_Check (&evp->vd_tlock) == -1) &&
		 (lock_Check (&evp->vd_vm.lock) == -1) &&
		 (lock_Check (&evp->vd_file.lock) == -1) &&
		 !NoIdentity(evp));
    AFS_hset64 (evp->vd_len, 0, len);
    evp->vd_flags |= VD_LENVALID;
    return 0;
}

/*
 * efs_GetBlockSize -- get block size of vnode's fileset
 *
 * LOCK USED -- the caller must hold at least one of the vnode, vm or file
 *     locks.
 *     XXX -- efs_putapage calls this without holding any locks.  However, we
 *         can prove that the vnode has an open anode at that point, and hence
 *         the call should be safe. */

long efs_GetBlockSize(struct vnode *vp)
{
    efs_evnode_t *evp = VTOEV(vp);

    afsl_Assert(evp->vd_file.anode);
    return epia_AnodeDevice(evp->vd_file.ap)->blkSize;
}

/*
 * efsx_getacl -- read ACL of file
 */

/* EXPORT */
/* ARGSUSED */
int efsx_getacl(vp, aclp, w, cred)
    struct vnode *vp;			/* vnode */
    struct dfs_acl *aclp;		/* where to put ACL */
    int w;				/* which ACL to get */
    osi_cred_t *cred;			/* credential structure */
{
    long code;
    struct epif_status fstat;		/* file status of file */
    epi_anode_t ap;			/* anode handle for evp */
    int flag;				/* which-acl flag for epif layer */
    u_long alength;			/* length of acl */
    int releaseit = 0;
    char *bbP;
    dacl_t *daclP;			/* parsed ACL */
    struct evnode *evp;			/* evnode */
    epi_uuid_t accessAclRealm;		/* default realm in regular acl */
    epi_uuid_t storingRealm;		/* realm of storing cell */
    dacl_t *accessAclP;			/* the regular acl */
    int foreignDefaultRealm = 0;	/* 1 = foreign default realm for acl */
    int aclType;
    int delegateAclOk = 0;

    osi_MakePreemptionRight();
    evp = VTOEV(vp);

    aclType = w & VNX_ACL_TYPE_MASK;
    delegateAclOk = w & VNX_ACL_DELEGATE_ENTRY_OK;

    code = EV_CHECKSTALE (evp);
    if (!code) {
	/* Only directories have default ACL's */
	if (aclType != VNX_ACL_REGULAR_ACL && !EV_ISDIR (evp))
	    code = EINVAL;
    }
    if (code) {
	osi_UnmakePreemptionRight();
	return code;
    }

    ap = EVTOA (evp);
    bbP = aclp->dfs_acl_val;

    /* Obtain lock on file */
    tlock_ObtainRead (&evp->vd_tlock);
    lock_ObtainWrite_r(&evp->vd_cache.lock);
    code = epif_GetStatus (ap, &fstat);
    MBZ (code);
    switch (aclType) {
	case VNX_ACL_REGULAR_ACL:
	    flag = EPIF_AUX_IS_ACL;
	    daclP = evp->vd_acl;
	    alength = fstat.acl;
	    break;
	case VNX_ACL_DEFAULT_ACL:
	    flag = EPIF_AUX_IS_INITIAL_DIR_ACL;
	    daclP = 0;
	    alength = fstat.initialDirAcl;
	    break;
	case VNX_ACL_INITIAL_ACL:
	    flag = EPIF_AUX_IS_INITIAL_FILE_ACL;
	    daclP = 0;
	    alength = fstat.initialFileAcl;
	    break;
	default:
	    code = EINVAL;
	    goto out;
    }

    /* If file has no ACL, create a dummy */
    if (alength == 0) {
	int mode;

	if (aclType == VNX_ACL_REGULAR_ACL) {
	    mode = fstat.mode;
	} else {
	    /* The FetchACL RPC does not carry the client process's
	     * umask to the server. This means that the umask that we could
	     * use here is that of the server thread that happens to be
	     * servicing the request. And this umask has no bearing on the
	     * issue at all. Hence we use a fixed dummy ACL.
	     */
	    mode = 0777;
	}

	eacl_AllocateAcl (&daclP);

	/*
	 * Directories owned by foreign users always have an explicit
	 * regular (access) acl with the default realm being the realm
	 * of the foreign owner. But they may not have initial acls.
	 * Hence, when conjuring up a dummy initial acl for such dirs,
	 * we have to ensure that the default realm of the initial acl is the
	 * realm of the foreign owner.
	 */
	if (aclType != VNX_ACL_REGULAR_ACL && fstat.acl != 0) {
	    /* read regular access acl if not cached */
	    if (!evp->vd_acl) {
		eacl_AllocateAcl (&accessAclP);
		code = eacl_ReadAclFromAnode(accessAclP, ap, fstat.acl,
					     EPIF_AUX_IS_ACL, 
					     &episodeAclMgmtUuid);
		if (!code) {
		    /* Cache read acl */
		    evp->vd_acl = accessAclP;
		} else {
		    EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, EIO);
		    code = EIO;
		    eacl_ReleaseAcl (accessAclP);
		    eacl_ReleaseAcl (daclP);
		    goto out;
		}
	    } else {
		accessAclP = evp->vd_acl;
	    }
	    dacl_GetLocalCellID_r((afsUUID *)&storingRealm);
	    dacl_GetDefaultRealm_r(accessAclP, &accessAclRealm);
	    if (bcmp((char *)&storingRealm, (char *)&accessAclRealm,
		sizeof(epi_uuid_t)) != 0 ) {
		foreignDefaultRealm = 1;
	    }
	}
        dacl_InitEpiAcl_r(daclP, (u_int32 *) &mode,
                          (EV_ISDIR (evp) &&
                           (aclType != VNX_ACL_INITIAL_ACL)),
                          (foreignDefaultRealm ? &accessAclRealm : NULL));

	aclp->dfs_acl_len = sizeof (aclp->dfs_acl_val);
	osi_UnmakePreemptionRight();
	code = dacl_epi_FlattenAcl (daclP, &bbP,
				    (unsigned int *) &aclp->dfs_acl_len,
				    mode & 0777);
	osi_MakePreemptionRight();
	afsl_MBZ(code);
	eacl_ReleaseAcl(daclP);
	goto out;
    }

    /* If ACL is not cached, read it */
    if (!daclP) {
	eacl_AllocateAcl (&daclP);
	code = eacl_ReadAclFromAnode(daclP, ap, alength, flag,
				     &episodeAclMgmtUuid);
	if (code) {
	    releaseit = 1;
	    EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, EIO);
	    code = EIO;
	} else {
	    if (aclType == VNX_ACL_REGULAR_ACL)
		evp->vd_acl = daclP;
	    else
		releaseit = 1;

	    /* 
	     * Handle clients that do not understand delegate ACL entries.
	     * This is not the most appropriate place to handle this. 
	     * Ideally the acl manipulation tool at the client end should
	     * handle this.
	     */
	    if (!delegateAclOk && DACL_HAS_DELEGATE_ENTRY(daclP)) {
		code = EIO;	    
		icl_Trace3(efs_iclSet, EFS_GETACL_DELEGATE_ENTRY_NOT_OK,
			   ICL_TYPE_POINTER, evp,
			   ICL_TYPE_LONG, aclType,
			   ICL_TYPE_LONG, code);
	    }
	}
    }

    /*
     * Flatten it
     *
     * Note that parsing followed by flattening seems wasteful, but it is
     * required, because UUID's are 32 bits before parsing and 128 bits after
     * flattening.
     */
    if (code == 0) {
	aclp->dfs_acl_len = sizeof (aclp->dfs_acl_val);
	osi_UnmakePreemptionRight();
	if (aclType == VNX_ACL_REGULAR_ACL) {
	  /* we only care about the mode for the regular acl */
	  code = dacl_epi_FlattenAcl(daclP, &bbP,
				     (u_int *) &aclp->dfs_acl_len,
				     fstat.mode & 0777);
	} else {
	  code = dacl_FlattenAclWithModeBits(daclP, &bbP,
					     (unsigned int *)
					     &aclp->dfs_acl_len,
					     0,
					     0 /* ignore passed mode bits */,
					     0 /* not for disk */);
	}
	osi_MakePreemptionRight();

	if (code) {
	    EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, ERANGE);
	    code = ERANGE;
	}
    }

    if (releaseit)
	eacl_ReleaseAcl (daclP);

out:
    lock_ReleaseWrite_r(&evp->vd_cache.lock);
    /* Release lock on file */
    tlock_ReleaseRead (&evp->vd_tlock);

    osi_UnmakePreemptionRight();
    return (code);
}

/*
 * efsx_setacl -- write ACL of file
 *
 * CAUTIONS -- This does not set the mode bits to match the new acl if we're
 *     using the replace aux branch.  This is OK for volume utilities like
 *     restore or move, but a system call interface may be incorrect with this
 *     behavior.  This should probably be fixed and/or put under control of a
 *     flag bit.
 */

/* EXPORT */
int efsx_setacl(vp, aclp, sp, dw, sw, cred)
    struct vnode *vp;			/* vnode */
    struct dfs_acl *aclp;		/* new data for acl */
    struct vnode *sp;			/* file from which to copy acl */
    int dw;				/* which ACL to set */
    int sw;				/* which ACL to copy */
    osi_cred_t *cred;			/* credential structure */
{
    struct evnode *evp, *svp;
    int code;

    osi_MakePreemptionRight();
    evp = VTOEV(vp);
    code = EV_CHECKSTALE (evp);
    if (!code && sp) {
	svp = VTOEV(sp);
	code = EV_CHECKSTALE (svp);
    } else
	svp = NULL;

    if (!code)
	code = vnva_SetAcl(evp, aclp, svp, dw, sw, cred, 0/*isVolop:=FALSE*/);
    osi_UnmakePreemptionRight();
    return code;
}

/*
 * efs_getsecattr -- Get SunOS security attributes (acls)
 *
 * This function only calls the fs_fab_acl() after releasing the
 * preemption lock; as should any code that redirects outside of DFS.
 * fs_fab_acl() gives back a SunOS compatible acl based on mode bits alone.
 * Use the native acl fabrication code to falsify SunOS acls for
 * the getsecattr vnode op.  This is an interim solution to Sun's
 * SCOPUS 204951 which we filed claiming that the file system should
 * not be forced to provide getsecattr and setsecattr vnops.
 */
#if	defined(AFS_SUNOS5_ENV) && defined(_KERNEL)
/* EXPORT */
int efs_getsecattr(vp, vsecattr, flag, cred)
    struct vnode	*vp;
    vsecattr_t		*vsecattr;
    int			flag;
    osi_cred_t		*cred;
{
    int code;

    osi_MakePreemptionRight();
    code = fs_fab_acl(vp, vsecattr, flag, cred);
    osi_UnmakePreemptionRight();
    return code;
}
#endif /* AFS_SUNOS5_ENV && _KERNEL */

struct xvfs_xops efs_xops = {
	(int (*)(struct vnode **, long, osi_cred_t *))efs_null,	/* open */
	(int (*)(struct vnode *, long, osi_cred_t *))efs_null,	/* close */
	efs_rdwr,
	(int (*)(struct vnode *, int, int))efs_invalid,	/* ioctl */
#ifdef AFS_OSF_ENV
	(int (*)(struct vnode *, int, osi_cred_t *))efs_null,	/* select */
#else
	(int (*)(struct vnode *, int, osi_cred_t *))efs_invalid,	/* select */
#endif
	efs_getxattr,
	efs_setxattr,
	efs_access,
	efs_lookup,
	efs_create,
	efs_remove,
	efs_link,
	efs_rename,
	efs_mkdir,
	efs_rmdir,
	efs_readdir,
	efs_symlink,
	efs_readlink,
	efs_fsync,
	efs_inactive,
#ifdef	AFS_HPUX_ENV
	efs_bmap,
	efs_hpstrategy,
	efs_hpustrategy,
#else
	(int (*)(struct vnode *, long, struct vnode **, long *))efs_panic,	/* bmap */
#if	defined(AFS_AIX31_VM) && defined(KERNEL)
	efs_vmstrategy,
#else
#ifdef AFS_SUNOS5_ENV
	(int (*)(struct buf *, osi_cred_t *))efs_panic,	/* strategy */
#else /* AFS_SUNOS5_ENV */
	(int (*)(struct buf *))efs_panic,	/* strategy */
#endif /* AFS_SUNOS5_ENV */
#endif	/* AFS_AIX31_VM */
#ifdef AFS_SUNOS5_ENV
	(int (*)(struct buf *, osi_cred_t *))efs_panic,	/* ustrategy */
#else /* AFS_SUNOS5_ENV */
	(int (*)(struct buf *))efs_panic,	/* ustrategy */
#endif /* AFS_SUNOS5_ENV */
#endif	/* AFS_HPUX_ENV */
#ifdef AFS_OSF_ENV
	efs_bread,
	efs_brelse,
#else
	(int (*)(struct vnode *, daddr_t, struct buf **))efs_panic,	/* bread */
	(int (*)(struct vnode *, struct buf *))efs_panic,	/* brelse */
#endif /* AFS_OSF_ENV */
#ifdef	AFS_SUNOS5_ENV
	efs_frlock,
#else
	efs_lockctl,
#endif	/* AFS_SUNOS5_ENV */
#if defined(AFS_AIX31_ENV) || defined(AFS_SUNOS5_ENV) || defined(AFS_HPUX_ENV)
	efs_fid,
#else /* AFS_AIX31_ENV or AFS_SUNOS5_ENV or AFS_HPUX_ENV */
	(int (*)(struct vnode *, struct fid *))efs_null,	/* fid */
#endif
#ifdef AFS_AIX_ENV
	efs_hold,
	efs_rele,
#else
	(int (*)(struct vnode *))efs_nosys,	/* hold */
	(int (*)(struct vnode *))efs_nosys,	/* rele */
#endif /* AFS_AIX_ENV */
	efsx_setacl,
	efsx_getacl,
	efs_afsfid,
	efs_getvolume,
	efs_getlength,
#if	defined(AFS_AIX31_VM) && defined(KERNEL)
	efs_map,
	efs_unmap,
#elif	defined(AFS_SUNOS5_ENV) && defined(_KERNEL)
	efs_map,
	(int (*)(struct vnode *, int))efs_nosys,
#else
#ifdef AFS_SUNOS5_ENV
	(int (*)(struct vnode *, offset_t, struct as *, caddr_t *, u_int,
		  u_char, u_char, u_int, osi_cred_t *))efs_nosys,
#else /* AFS_SUNOS5_ENV */
	(int (*)(struct vnode *, caddr_t, u_int, u_int, u_int))efs_nosys,
#endif /* AFS_SUNOS5_ENV */
	(int (*)(struct vnode *, int))efs_nosys,
#endif

#ifdef AFS_OSF_ENV
	efs_reclaim,
#else
	(int (*)(struct vnode *))efs_panic,	/* reclaim */
#endif /* AFS_OSF_ENV */

#if	defined(AFS_SUNOS5_ENV) && defined(_KERNEL)
	efs_vmread,
	efs_vmwrite,
	(int (*)(struct vnode *, struct vnode **))efs_nosys,	/* realvp */
	efs_rwlock,
	efs_rwunlock,
	efs_seek,
	efs_space,
	efs_getpage,
	efs_putpage,
	efs_addmap,
	efs_delmap,
	efs_pageio,
#else
	(int (*)(struct vnode *, struct uio *, int, osi_cred_t *))efs_nosys,	/* read */
	(int (*)(struct vnode *, struct uio *, int, osi_cred_t *))efs_nosys,	/* write */
	(int (*)(struct vnode *, struct vnode **))efs_nosys,	/* realvp */
	(void (*)(struct vnode *, int)) efs_nosys,	/* rwlock */
	(void (*)(struct vnode *, int))efs_nosys,	/* rwunlock */
	efs_nosys,	/* seek */
	efs_nosys,	/* space */
	efs_nosys,	/* getpage */
	efs_nosys,	/* putpage */
	efs_nosys,	/* addmap */
	efs_nosys,	/* delmap */
	efs_nosys,	/* pageio */
#endif	/* AFS_SUNOS5_ENV && _KERNEL */
#if	defined(AFS_HPUX_ENV) && defined(_KERNEL)
	efs_pagein,
	efs_pageout,
#else
	efs_nosys,	/* pagein */
	efs_nosys,	/* pageout */
#endif	/* AFS_HPUX_ENV && _KERNEL */
#if	defined(AFS_SUNOS5_ENV) && defined(_KERNEL)
	fs_setfl,	/* setfl */
#else
	(int (*)(struct vnode *, int, int, osi_cred_t *))efs_nosys,	/* setfl */
#endif	/* defined(AFS_SUNOS5_ENV) && defined(_KERNEL) */
#if	defined(AFS_SUNOS54_ENV) && defined(_KERNEL)
	fs_dispose,	/* dispose */
	fs_nosys,	/* setsecattr */
	efs_getsecattr,	/* getsecattr */
#else
	(void (*)()) efs_nosys,	/* dispose */
	(int (*)())efs_nosys,	/* setsecattr */
	(int (*)())efs_nosys	/* getsecattr */
#endif	/* defined(AFS_SUNOS54_ENV) && defined(_KERNEL) */
};
