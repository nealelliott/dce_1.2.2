/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1996, 1990 Transarc Corporation
 * All rights reserved.
 */

/*			   Episode File System
 *	    Support for operations on vnode attribute structures
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_kvnode.h>
#include <dcedfs/osi_buf.h>		/* btodb */

#include <dcedfs/lock.h>		/* struct lock_data */
#include <dcedfs/debug.h>
#include <dcedfs/dacl.h>		/* dacl_perm_xxx */
#include <dcedfs/xvfs_vnode.h>		/* struct xvfs_attr */
#include <dcedfs/aggr.h>
#include <dcedfs/dir_buffer.h>		/* struct dh_ops */
#include <dcedfs/aclint.h>		/* struct dfs_acl */

#include <dcedfs/episode/file.h>	/* struct epif_status */
#include <dcedfs/episode/volume.h>	/* struct epiv_status */
#include <dcedfs/episode/dacl_anode.h>

#include <efs_evnode.h>
#include <efs_volvnops.h>		/* VTOVH */
#include <efs_access.h>
#include <efs_misc.h>
#include <efs_dir.h>
#include <efs_vattr.h>
#include <efsmount.h>

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_vattr.c,v 10.24 1996/03/25 20:46:44 cfe Exp $")

#if !defined(KERNEL) && defined(AFS_AIX_ENV)
#include <dcedfs/osi_user.h>
extern struct user u;
#endif

#define DEBUG_THIS_FILE EFS_DEBUG_VATTR


/*
 * vnva_FileCreate -- create new file with attributes and defaults
 */

/* EXPORT */
int vnva_FileCreate(
    struct evnode *devp,		/* directory evnode */
    struct vattr *vap,			/* vnode attribute structure */
    epi_volFileId_t *fidP,		/* place to put ID of new file */
    epi_anode_t *fileP,			/* anode pointer for new file */
    int dxflags,			/* results of dirCheck */
    elbb_tranRec_t transId,		/* transaction ID */
    unsigned int umask)                 /* Umask value */
{
    epi_anode_t dap;			/* Parent dir anode */
    struct epif_status dirStat;		/* Parent dir status information */
    struct epif_status newStat;		/* file status information */
    int createFlags = 0;		/* flags for epif_Create */
    int code;				/* error return code */
    int inheritAcl = 0;			/* acl length */
    epi_anode_t aclSource;		/* file to get ACLs from */
    epi_anode_t plistSource;		/* file to get PLIST from */

    /* Set the fields in the status block */

    /*
     * If the appropriate initial acl on the parent dir exists, ignore
     * umask. For symlinks, the umask needs to be ignored anyway
     * Also set the flags for the epif_Create call.
     */
    if (vap->va_type == VLNK) {
	newStat.mode = vap->va_mode;
	/* Symlinks do not need ACLs or PLISTS */
	aclSource = plistSource = NULL;
    } else {
	createFlags = EPIF_CFLAGS_SHARE_ACL | EPIF_CFLAGS_SHARE_PLIST;
	if (vap->va_type == VREG) {
	    inheritAcl = (dxflags & DX_INHERITFILEACL);
	} else if (vap->va_type == VDIR) {
	    inheritAcl = (dxflags & DX_INHERITDIRACL);
	    createFlags |= EPIF_CFLAGS_DIRECTORY;
	}
	if (inheritAcl) {
	    newStat.mode = vap->va_mode;
	} else {
	    newStat.mode = vap->va_mode & ~umask;
	}
	aclSource = plistSource = EVTOA(devp);
    }
    newStat.mode |= EV_VTYPE_TO_MODE (vap->va_type);
    newStat.oid = (u_long) vap->va_uid;
    newStat.gid = (u_long) vap->va_gid;
    bzero ((caddr_t) &newStat.uuid, sizeof(newStat.uuid));

    /* Create the file */
    code = epif_Create (transId, VTOVH(devp->vd_volp),
			createFlags, 0, &newStat,
			aclSource, plistSource,
			fidP, fileP);
    if (code)
	VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);
    return (code);
}

/*
 * vnva_FileConstruct -- create a new file at a particular volume index
 *
 * Assumes caller (e.g. ftserver) will set ACL and Plist.
 */

/* EXPORT */
int vnva_FileConstruct (
  epi_volume_t volh,			/* volume */
  struct vattr *vap,			/* vnode attribute structure */
  epi_volFileId_t *fidP,		/* ID of new file */
  epi_anode_t *fileP,			/* anode pointer for new file */
  elbb_tranRec_t transId)		/* transaction ID */
{
    struct epif_status newStat;		/* file status information */
    int flags;				/* flags for epif_Create */
    int code;				/* error return code */

    /* Set the fields in the status block */

    newStat.mode = vap->va_mode;
    newStat.mode |= EV_VTYPE_TO_MODE (vap->va_type);
    newStat.oid = (u_long) vap->va_uid;
    newStat.gid = (u_long) vap->va_gid;
    bzero ((caddr_t) &newStat.uuid, sizeof(newStat.uuid));

    /* Set the flags */

    if (epid_ToIndex (fidP) == 1)
	flags = EPIF_CFLAGS_VOLUMEROOT;
    else
	flags = EPIF_CFLAGS_USEFID;

    if (vap->va_type == VDIR)
	flags |= EPIF_CFLAGS_DIRECTORY;

    /* Create the file */

    code = epif_Create (transId, volh, flags, 0, &newStat, 0, 0, fidP, fileP);

    if (code)
	VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);
    return (code);
}

/*
 * vnva_GetAttr -- get attributes of a file
 *
 * This performs the work of VOP_GETATTR, but leaves locking/unlocking of
 * the vnode up to the caller.
 */

/* EXPORT */
int vnva_GetAttr(
    struct evnode *evp,			/* vnode */
    struct vattr *vap,			/* vnode attributes */
    osi_cred_t *cred,			/* credential structure */
    int extended) 			/* extended attribute structure? */    
{
    epi_anode_t ap;			/* anode handle for evp */
    afs_hyper_t hlen;				/* full length of evp */
    struct epif_status fstat;		/* status information about evp */
    struct epia_info ainfo;		/* status about its anode */
    struct async_device *dev;		/* device of file */
    struct xvfs_attr *xvap = (struct xvfs_attr *) vap;
    int vid;				/* low word of volume id */
    int lfpb;				/* log of fragments per block */
    int code;

    ap = EVTOA (evp);
    dev = epia_AnodeDevice (ap);
    lfpb = dev->logBlkSize - dev->logFragSize;
    if (evp->vd_flags & VD_LENVALID) {
	hlen = evp->vd_len;
    } else {
	hlen = epia_GetLength (ap);
    }
    if ((code = epif_GetStatus (ap, &fstat)) != 0) {
	icl_Trace4(efs_iclSet, EFS_TRACE_VNVA_GETATTR_FILE_STATUS,
		   ICL_TYPE_POINTER, evp,
		   ICL_TYPE_POINTER, ap,
		   ICL_TYPE_LONG, evp->vd_fid.index,
		   ICL_TYPE_LONG, code);
	EFS_TRACE_VNOPS(DEBUG_THIS_FILE,code, EIO);
	return EIO;
    }
    vid = AFS_hgetlo(evp->vd_volid);

/*  vap->va_type = (enum vtype) fstat.type; */
    vap->va_type = osi_vType (EVTOV (evp));
    vap->va_mode = fstat.mode;
    vap->va_uid = fstat.oid;
    vap->va_gid = fstat.gid;

    /*
     * FS ID's:
     *	We cannot just use the device (as other physical file systems do),
     *  because the FS ID has to be unique, and Episode allows multiple FS'es
     *  per device.  We cram the device minor, device major, and low 16 bits
     *  of the volume ID into 32 bits, and cross our fingers.
     */

    /* In AIX 3 and SunOS 5, device majors and minors are 16 bits. */
#if	defined(AFS_AIX31_ENV) || defined(AFS_SUNOS5_ENV)
    vap->va_fsid = (vid << 16)
		    + ((dev->majdev & 0xff) << 8)
		    + (dev->mindev & 0xff);
#else
    vap->va_fsid = (vid << 16) + (dev->majdev << 8) + (dev->mindev);
#endif
    vap->va_nodeid = epif_FidToShortHandle (&evp->vd_fid);

    vap->va_nlink = fstat.linkCount;
    vap->va_rdev = evp->vd_rdev;
#ifdef AFS_OSF_ENV
    vap->va_size_rsv = AFS_hgethi(hlen);
    vap->va_size = AFS_hgetlo(hlen);
    vap->va_bytes = epia_GetVisible (ap) << dev->logFragSize;
#else
    vap->va_size = AFS_hgetlo(hlen);
    vap->va_blocks = btodb(epia_GetVisible(ap) << dev->logFragSize);
#endif /* AFS_OSF_ENV */

    /* Note that the definitive version of all the times and the data version
     * number are in the vnode. */

    BEGIN_LOCK_WRITE (&evp->vd_cache.lock) {

	osi_SubFromTval (vap->va_atime, evp->vd_cache.lastAtime);
	osi_SubFromTval (vap->va_mtime, evp->vd_cache.lastMtime);
	osi_SubFromTval (vap->va_ctime, evp->vd_cache.lastCtime);

	if (extended) {
	    xvap->xvattr.serverModTime.sec = evp->vd_cache.lastCtime.tv_sec;
	    xvap->xvattr.serverModTime.usec = evp->vd_cache.lastCtime.tv_usec;
	    AFS_hset64(xvap->xvattr.dataVersion, 0, evp->vd_cache.dataVersion);
	}

    } END_LOCK;

    vap->va_blocksize = dev->blkSize;

    if (extended) {
	int code;
	u_long perm;

	code = epia_GetInfo (ap, &ainfo);
	afsl_MBZ (code);
	AFS_hset64(xvap->xvattr.fileID,
		   epid_ToIndex (&evp->vd_fid), epid_ToUnique (&evp->vd_fid));
	xvap->xvattr.volVersion = fstat.volumeVersionNumber;
	/* lots of other fields, have no idea how to set them. */
	xvap->xvattr.author = -1;
	code = vnax_GetAccess (evp, &fstat, &perm, cred);
	if (code) return (code);
	xvap->xvattr.callerAccess = perm;
	code = vnax_GetAnonAccess (evp, &fstat, &perm);
	if (code) return (code);
	xvap->xvattr.anonAccess = perm;
	xvap->xvattr.parentVnode = -1;
	xvap->xvattr.parentUnique = -1;
	xvap->xvattr.fstype = AG_TYPE_EPI;
	xvap->xvattr.objid = *((afsUUID *) &fstat.uuid);
	xvap->xvattr.timeUncertainty = fstat.res_timeUncertainty;
	xvap->xvattr.representationFlags = ainfo.flags;
	xvap->xvattr.backingIndex = ainfo.backingIndex;
	xvap->xvattr.backingVolIndex = ainfo.backingVolIndex;
	xvap->xvattr.clientOnlyAttrs = fstat.clientBits;
	/*
	 * The following settings are bogus.  The xvattr fields are supposed
	 * to be indexes, but they are being initialized from fstat fields that
	 * are lengths.  Fortunately nobody is using these fields yet anyway.
	 */
	xvap->xvattr.aclIx = fstat.acl;
	xvap->xvattr.initDirAclIx = fstat.initialDirAcl;
	xvap->xvattr.initFileAclIx = fstat.initialFileAcl;
	xvap->xvattr.plistIx = fstat.plist;
	xvap->xvattr.uPlistIx = fstat.reservedForUserPlist;
    }

    return (0);
}

/*
 * vnva_SetAttr -- set attributes of a file
 *
 * This performs the work of efs_setattr, efs_setxattr, and vol_efsSetAttr.
 *
 * LOCKS USED -- unlike other functions in this file, vnva_SetAttr obtains
 *		 and releases the vnode's lock.
 */

/* EXPORT */
int vnva_SetAttr(
    struct evnode *evp,			/* vnode */
    struct vattr *vap,			/* vnode attribute structure */
    osi_cred_t *cred,			/* credential structure */
    int flags)				/* various flags */
{
    int extended = (flags & XVN_EXTENDED);
    int volop = (flags & XVN_VOL_SETATTR);	/* caller is vol_efsSetAttr */
    epi_anode_t ap;			/* anode handle for evp */
    elbb_tranRec_t transId;
    struct epif_status fstat;		/* file status info for evp */
    long mask = 0;			/* list of status fields to set */
    int markmask = 0;			/* flags for epif_Mark */
    int code = 0;			/* error return code */
    int issuper;			/* is process super-user? */
    int ingroups;			/* can process access file's group? */

    osi_assert(!volop || extended);
    if (osi_setting_size(vap)) 
	EFS_VMLEN_WLOCK(evp);
	
    BEGIN_TLOCK_WRITE_NT (&evp->vd_tlock) {
	ap = EVTOA (evp);
	if (code = epif_GetStatus (ap, &fstat)) {
	    tlock_ReleaseWriteNT (&evp->vd_tlock);
	    EFS_TRACE_VNOPS(DEBUG_THIS_FILE,code, EIO);
	    if (osi_setting_size(vap)) 
		EFS_VMLEN_WUNLOCK(evp);
	    return (EIO);
	}

	/* change ctime -- only allowed for volume operations */
	if (volop && osi_setting_ctime (vap)) {
	    osi_TvalFromSub (fstat.cTime, vap->va_ctime);
	    mask |= EPIF_SFLAGS_CTIME;
	}

	/* change link count -- only allowed for volops */
	if (volop && osi_setting_link (vap)) {
	    fstat.linkCount = vap->va_nlink;
	    mask |= EPIF_SFLAGS_LINK;
	}

	/* change volume version number -- only allowed for volops */
	if (volop &&
	    AFS_hgetlo(((struct xvfs_attr *) vap)->xvattr.volVersion) != -1) {
	    fstat.volumeVersionNumber =
		((struct xvfs_attr *) vap)->xvattr.volVersion;
	    mask |= EPIF_SFLAGS_VVERSION;
	}

	/* change data version number -- only allowed for volops */
	if (volop &&
	    AFS_hgetlo(((struct xvfs_attr *)vap)->xvattr.dataVersion) != -1) {
	    fstat.dataVersionNumber =
		AFS_hgetlo(((struct xvfs_attr *) vap)->xvattr.dataVersion);
	    mask |= EPIF_SFLAGS_DVERSION;
	}

	/* change uniquifier -- only allowed for volops */
	if (volop &&
	    AFS_hgetlo(((struct xvfs_attr *) vap)->xvattr.fileID) != -1) {
	    fstat.unique =
		AFS_hgetlo(((struct xvfs_attr *) vap)->xvattr.fileID);
	    mask |= EPIF_SFLAGS_UNIQUE;
	}

	/* change clientBits -- only allowed in extended mode */
	if (extended &&
	    ((struct xvfs_attr *)vap)->xvattr.clientOnlyAttrs != -1) {
	    fstat.clientBits =
		((struct xvfs_attr *)vap)->xvattr.clientOnlyAttrs;
	    mask |= EPIF_SFLAGS_CLIENTBITS;
	}

	/* chmod */
	if (osi_setting_mode (vap)) {
	    if (!volop) {
		if (code = vnax_CanAdminister (evp, &fstat, &fstat.gid, cred,
					       &issuper, &ingroups))
		    EXIT_CRIT;
	    }

	    mask |= EPIF_SFLAGS_MODE;
	    fstat.mode &= S_IFMT;
	    fstat.mode |= (vap->va_mode & 07777);
	    if (!volop && !issuper) {
		fstat.mode &= ~VSVTX;
		if (!ingroups)
		    fstat.mode &= ~VSGID;
	    }

#if	!defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX31_ENV)
	    /* if changing text to non-sticky, remove from text table */
	    if (((EVTOV(evp))->v_flag & VTEXT) && ((fstat.mode & VSVTX) == 0))
		xrele (evp);
#endif					/* AFS_SUNOS5_ENV, AFS_AIX31_ENV */

	}

	/* chown */
	if (osi_setting_uid(vap)) {
	    if (volop ? evp->vd_file.anodeRO : EV_ISREADONLY(evp)) {
		code = EROFS;
		EXIT_CRIT;
	    }
	    code = vnax_DoChown (evp, &mask, &fstat, cred, 
				 vap->va_uid, volop);
	    if (code) 
		EXIT_CRIT;
	    if (!volop)
		markmask |= EPIF_SFLAGS_CTIME;
	}

	/* chgrp */
	if (osi_setting_gid(vap)) {
	    if (volop ? evp->vd_file.anodeRO : EV_ISREADONLY(evp)) {
		code = EROFS;
		EXIT_CRIT;
	    }
	    code = vnax_DoChgrp (evp, &mask, &fstat, cred,
				 vap->va_gid, volop);
	    if (code) 
		EXIT_CRIT;
	    if (!volop)
		markmask |= EPIF_SFLAGS_CTIME;
	}

	/* truncate */
	/*
	 * This can also be used to grow a file.  A layer above us (the xvnode
	 * portability code) decides whether it's OK to call VOP_SETATTR when
	 * it would cause the file to grow.
	 *
	 * Also note that there is no access check here any more.  Most kernels
	 * seem to be doing this above the VFS (or equivalent) interface.
         * Also, the PX definitely doesn't want us to do it.
         */
	if (osi_setting_size (vap)) {
	    if (EV_ISDIR (evp)) {
		code = EISDIR;
		EXIT_CRIT;
	    }
	    if (code = vnm_Truncate (evp, vap->va_size, 
				     volop ? 0 : TRUNCATE_USEVM, cred))
		EXIT_CRIT;
	    markmask |= (EPIF_SFLAGS_MTIME | EPIF_SFLAGS_CTIME);
	}

	/* utimes */
	if (flags & XVN_SET_TIMES_NOW) { /* System-V style utime call */
	    struct timeval t;

	    if (code = vnax_CanAdminister (evp, &fstat, 0, cred,
					   &issuper, &ingroups))
		/* Not owner:  it is still OK if we have write access */
		if (code = vnax_CanAccess (evp, &fstat, dacl_perm_write, cred))
		    EXIT_CRIT;
	    mask |= (EPIF_SFLAGS_MTIME|EPIF_SFLAGS_ATIME|EPIF_SFLAGS_CTIME);
	    epif_UniqueTime (&t);
	    fstat.mTime = fstat.aTime = fstat.cTime = t;
	    markmask |= EPIF_SFLAGS_VVERSION;
	} else {			/* Berkeley style utimes call */
	    if (osi_setting_atime (vap)) {
		if (!volop &&
		    (code = vnax_CanAdminister (evp, &fstat, 0, cred,
						&issuper, &ingroups)))
		    EXIT_CRIT;
		osi_TvalFromSub (fstat.aTime, vap->va_atime);
		mask |= EPIF_SFLAGS_ATIME;
	    }
	    if (osi_setting_mtime (vap)) {
		if (!volop &&
		    (code = vnax_CanAdminister (evp, &fstat, 0, cred,
						&issuper, &ingroups)))
		    EXIT_CRIT;
		osi_TvalFromSub (fstat.mTime, vap->va_mtime);
		mask |= EPIF_SFLAGS_MTIME;
	    }
	}

	if (!mask && !markmask && !EV_ISDIRTYTIMES(evp))
	    EXIT_CRIT;			/* nothing to do */

	BEGIN_CRIT (code = epia_StartTran ("vnva_SetAttr", ap, &transId),
		    code = epia_EndTran (ap, transId)) {

	    if (mask & EPIF_SFLAGS_LINK) {
		int lc = epif_GetLinkCount (ap);
		code = epif_ChangeLink (transId, ap, vap->va_nlink - lc,
					0, (u_int *) 0);
		if (code) {
		    EFS_TRACE_VNOPS(DEBUG_THIS_FILE,code, EIO);
		    code = EIO;
		    EXIT_CRIT;
		}
		mask &= ~EPIF_SFLAGS_LINK;
	    }
	
	    /* 
	     * See if there are any pending Atime, Ctime or Mtime changes to
             * propagate.  However, these should not overwrite any newer times
             * being explicitly provided.
             */

	    BEGIN_LOCK_WRITE (&evp->vd_cache.lock) {
		if (evp->vd_cache.newAtime && !(mask & EPIF_SFLAGS_ATIME)) {
		    fstat.aTime = evp->vd_cache.lastAtime;
		    mask |= EPIF_SFLAGS_ATIME;
		}
	    
		if (evp->vd_cache.newMtime) {
		    if (!(mask & EPIF_SFLAGS_MTIME)) {
			fstat.mTime = evp->vd_cache.lastMtime;
			mask |= EPIF_SFLAGS_MTIME;
		    }
		    if (!(mask & EPIF_SFLAGS_DVERSION)) {
			fstat.dataVersionNumber = evp->vd_cache.dataVersion;
			mask |= EPIF_SFLAGS_DVERSION;
		    }
		}
	    
		if (evp->vd_cache.newCtime) {
		    if (!(mask & EPIF_SFLAGS_CTIME)) {
			fstat.cTime = evp->vd_cache.lastCtime;
			mask |= (EPIF_SFLAGS_CTIME);
		    }
		    markmask |= EPIF_SFLAGS_VVERSION;
		}

		/* A few special checks, just to be safe: be sure we are
                 * setting ctime, and use only latest times for M/Ctime. */

		if (!volop && (mask & ~EPIF_SFLAGS_ATIME))
		    markmask |= EPIF_SFLAGS_CTIME;
		markmask &= ~(mask & (EPIF_SFLAGS_MTIME | EPIF_SFLAGS_DVERSION
				      | EPIF_SFLAGS_CTIME));

		code = epif_SetStatusAndMark (transId, ap, markmask, mask,
					      &fstat);
		if (code) {
		    EFS_TRACE_VNOPS(DEBUG_THIS_FILE,code, EIO);
		    code = EIO;
		    EXIT_CRIT;
		}

		evp->vd_cache.newAtime = evp->vd_cache.newMtime =
		    evp->vd_cache.newCtime = 0;
		evp->vd_cache.lastAtime = epif_GetATime (ap);
		evp->vd_cache.lastMtime = epif_GetMTime (ap);
		evp->vd_cache.lastCtime = epif_GetCTime (ap);
		evp->vd_cache.dataVersion = epif_GetDV (ap);

		/* 
		 * Invalidate any cached access for change in file mode, 
		 * owner or primary group
		 */
		if (evp->vd_cred &&
		    (mask &
		     (EPIF_SFLAGS_MODE | EPIF_SFLAGS_OID | EPIF_SFLAGS_GID))) {

		    icl_Trace1(efs_iclSet2, 
			       EFS_TRACE_ACCESS_CACHE_SETATTR_INVALIDATE,
			       ICL_TYPE_POINTER, evp);
		    crfree(evp->vd_cred);
		    evp->vd_cred = NULL;
		    evp->vd_access = 0;

		}
	    } END_LOCK; /* cacheLock */
	} END_CRIT; /* start/end tran */
    } END_LOCK; /* tlock */
    if (osi_setting_size(vap)) 
	EFS_VMLEN_WUNLOCK(evp);
    if (code)
	return code;

    if (efs_SyncMetaData(evp)) {
	long ccode = elbb_LogForce (transId, 1 /*commit*/);
	afsl_MBZ(ccode);
    }
    return 0;
}

int vnva_SetAcl(evp, aclp, svp, dw, sw, cred, isVolop)
    struct evnode *evp;			/* vnode */
    struct dfs_acl *aclp;		/* new data for acl */
    struct evnode *svp;			/* file from which to copy acl */
    int dw;				/* which ACL to set */
    int sw;				/* which ACL to copy */
    osi_cred_t *cred;			/* credential structure */
    int isVolop;			/* true if called from volop */
{
    epi_anode_t ap;			/* anode handle for evp */
    struct epif_status fstat;		/* file status info for evp */
    int issuper, ingroups;		/* dummy parms for CanAdminister */
    int flag;				/* which-acl flag for epif layer */
    int code;				/* error return code */
    long ccode;
    u_int32 failCode = 0;		/* code determining cached acl fate */
    char *bbP = 0;			/* buffer for flattened ACL */
    u_int len = 0;			/* length of flattened ACL */
    dacl_t *daclP = NULL;		/* validated new ACL */
    dacl_t *currentDaclP;		/* Current regular ACL */
    u_int32 permBits;			/* permissios from validated ACL */
    epi_uuid_t newAclRealm;		/* default realm of acl to be set */
    epi_uuid_t currentAclRealm;		/* default realm of regular acl */

    /* Sanity check: either write or replace but not both */
    if (aclp && svp) return EINVAL;
    if (!aclp && !svp) return EINVAL;

    /* Only directories have default ACL's */
    if (dw != VNX_ACL_REGULAR_ACL && !EV_ISDIR (evp))
	return (EINVAL);
    if (svp)
	if (sw != VNX_ACL_REGULAR_ACL && !EV_ISDIR (svp))
	    return (EINVAL);

    /*
     * Validate and extract permission bits. Also, extract realm of the
     * passed in ACL. NYI in the case of copying an ACL.
     */
    if (aclp && aclp->dfs_acl_len > 0) {
	/* Allocate storage for a parsed ACL */
	eacl_AllocateAcl (&daclP);

	/* Parse and validate */
	osi_UnmakePreemptionRight();
	code = dacl_epi_ValidateBuffer (aclp->dfs_acl_val,
					(unsigned int *)&aclp->dfs_acl_len,
					0, &daclP);
	osi_MakePreemptionRight();
	if (code) {		/* == DACL_ERROR_VALIDATE_BUFFER */
	    EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, EINVAL);
	    eacl_ReleaseAcl(daclP);
	    return EINVAL;
	}

	if (dw == VNX_ACL_REGULAR_ACL) {
	    /* Extract permission bits */
	    code = dacl_ExtractPermBits_r (daclP, &permBits);
	    if (code) {
		EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, ESPIPE);
		eacl_ReleaseAcl(daclP);
		return ESPIPE;
	    }
	}

	/* Extract acl realm */
	dacl_GetDefaultRealm_r(daclP, &newAclRealm);

	/*
	 * Flatten it
	 *
	 * Parsing followed by flattening may seem wasteful, but it is
	 * required, because UUID's are 128 bits before parsing, and 32 bits
	 * after flattening.
	 */
	osi_UnmakePreemptionRight();
	code = dacl_FlattenAcl (daclP, &bbP, &len);
	osi_MakePreemptionRight();
	if (code) {
	    EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, ENOMEM);
	    eacl_ReleaseAcl(daclP);
	    return ENOMEM;
	}

	/* Do not release the parsed ACL. We may want to cache it later */
    }

    switch (dw) {
	case VNX_ACL_REGULAR_ACL:
	    flag = EPIF_AUX_IS_ACL; break;
	case VNX_ACL_DEFAULT_ACL:
	    flag = EPIF_AUX_IS_INITIAL_DIR_ACL; break;
	case VNX_ACL_INITIAL_ACL:
	    flag = EPIF_AUX_IS_INITIAL_FILE_ACL; break;
	default:
	    if (daclP != (dacl_t *)NULL)
		eacl_ReleaseAcl(daclP);
	    return EINVAL;
    }
    ap = EVTOA (evp);

    /* Obtain lock on file */
    tlock_ObtainWriteNT (&evp->vd_tlock);
    code = epif_GetStatus (ap, &fstat);
    MBZ (code);

    /*
     * Check permission to change ACL. The check consists of two checks.
     *
     * 1.
     * Verify that the user has administer rights ( c right ) on file/dir.
     *
     * 2.
     * If the regular acl realm is being changed, verify that the user is
     * a privileged user who has rights to administer the filesystem.
     * ( Motivation: Change in file's ownership is restricted by Episode
     *   to members of filesystem admin group. Since the realm of a file/dir's
     *   regular acl is used to determine the cell to which the file/dir's
     *   owner belongs, changing the regular acl's realm amounts to changing
     *   the owner. )
     * To make this check, compare realm in acl being passed in
     * with { the realm of existing acl | the storing cell }. */

    /* Check 1 */
    if (!isVolop &&
	(code =
	 vnax_CanAdminister(evp, &fstat, 0, cred, &issuper, &ingroups))) {
	tlock_ReleaseWriteNT (&evp->vd_tlock);

	if (daclP != (dacl_t *)NULL) 
	    eacl_ReleaseAcl(daclP);
	return (code);
    }

    /* Check 2
     * Only perform this check if we have passed in an explicit acl to set.
     * If we have been called to share acls, do not make this check
     * that interface is to be only used by fileset ops to allow acl
     * sharing */

    lock_ObtainWrite_r(&evp->vd_cache.lock);
    if (aclp && aclp->dfs_acl_len > 0) {
	/* Determine current realm of regular acl */
	if (fstat.acl) {
	    if (!evp->vd_acl) {
		/* Read acl from disk */
		eacl_AllocateAcl (&currentDaclP);
		code = eacl_ReadAclFromAnode(currentDaclP, ap, fstat.acl,
					     EPIF_AUX_IS_ACL,
					     &episodeAclMgmtUuid);
		if (code) {
		    EFS_TRACE_VNOPS(DEBUG_THIS_FILE, code, EIO);
		    code = EIO;
		    eacl_ReleaseAcl(daclP);
		    eacl_ReleaseAcl(currentDaclP);
		    lock_ReleaseWrite_r(&evp->vd_cache.lock);
		    tlock_ReleaseWriteNT (&evp->vd_tlock);
		    return code;
		} else {
		    /* Cache acl in case we do not have permissions
		     * to store new acl */
		    evp->vd_acl = currentDaclP;
		}
	    }
	    /* Determine acl's realm */
	    dacl_GetDefaultRealm_r(evp->vd_acl, &currentAclRealm);
	} else {
	    /* Use storing cell's realm */
	    dacl_GetLocalCellID_r((afsUUID *)&currentAclRealm);
	}
	if (dw == VNX_ACL_REGULAR_ACL) {
	    if (bcmp((char *)&newAclRealm, (char *)&currentAclRealm,
		     sizeof(epi_uuid_t)) &&
		!issuper) {
		/* return permission denied error */
		eacl_ReleaseAcl (daclP);
		lock_ReleaseWrite_r(&evp->vd_cache.lock);
		tlock_ReleaseWriteNT (&evp->vd_tlock);
		return EACCES;
	    }
	}
    }
    lock_ReleaseWrite_r(&evp->vd_cache.lock);

    BEGIN_LCRIT (elbb_tranRec_t transId,
		 code = epia_StartTran ("vnva_SetAttr", ap, &transId),
		 code = epia_EndTran (ap, transId)) {

	if (aclp) {
	    /* Write the ACL (calling anode layer) */
	    code =
		epif_WriteAux
		    (transId, ap,
		     flag | EPIF_FLAGS_NEW_AUX | EPIF_FLAGS_TRUNCATE_AUX,
		     0, len, bbP);
	    if (code)
		failCode = 1;
	    /* Free the buffer created during flattening */
	    if (bbP) osi_Free_r (bbP, (long) len);

	    /* Use the extracted permission bits */
	    if (!code && bbP && (dw == VNX_ACL_REGULAR_ACL)) {
		fstat.mode &= ~0777;	/* zero out permission bits */
		fstat.mode |= permBits;
		code = epif_SetStatus (transId, ap, EPIF_SFLAGS_MODE, &fstat);
		if (code)
		    failCode = 2;
	    }
	} else {
	    /* Get source which in second 4 bits of flag */
	    switch (sw) {
	      case VNX_ACL_REGULAR_ACL:
		flag |= EPIF_AUX_IS_ACL<<4; break;
	      case VNX_ACL_DEFAULT_ACL:
		flag |= EPIF_AUX_IS_INITIAL_DIR_ACL<<4; break;
	      case VNX_ACL_INITIAL_ACL:
		flag |= EPIF_AUX_IS_INITIAL_FILE_ACL<<4; break;
	      default:
		code = EINVAL;
	    }

	    if (!code) code = epif_ReplaceAux (transId, ap, flag, EVTOA (svp));
	    if (code)
		failCode = 3;
	}

	/* Update ctime */
	vnm_SetMCtime (evp, MARK_C);
	vnm_UpdateAnode (evp, transId);

    } END_CRIT;

    /* For a file/dir's regular ACL,
     *     invalidate cached ACL if necessary and cache new acl if we get
     *     it for free.
     */
    lock_ObtainWrite_r(&evp->vd_cache.lock);
    if (dw == VNX_ACL_REGULAR_ACL) {
	if (!failCode || failCode == 2) {
	    if (evp->vd_acl) {
		/* 
		 * No failure OR
		 * a condition whereby the acl for the file has been set
		 * but the mode bits could be updated.  We may want to mark
		 * this file/dir a "inconsistent" in some fashion in the 
		 * future.
		 */
		eacl_ReleaseAcl (evp->vd_acl);
		if (aclp) {
		    evp->vd_acl = daclP;
		} else {
		    evp->vd_acl = 0;
		}
	    }
	    /* Invalidate any cached access */
	    if (evp->vd_cred) {
		icl_Trace1(efs_iclSet2, 
			   EFS_TRACE_ACCESS_CACHE_SETACL_INVALIDATE,
			   ICL_TYPE_POINTER, evp);
		crfree(evp->vd_cred);
		evp->vd_cred = NULL;
		evp->vd_access = 0;
	    }
	 } else if (failCode == 1) {
	     /* epif_WriteAux failed - Do not invalidate cached acl */
	     eacl_ReleaseAcl(daclP);
	 } else if (failCode == 3) {
	     /* epif_ReplaceAux failed - Do not invalidate cached acl */
	     /* NULL */
	 }
    }

    /* For a dir's initial object/container ACL, free the parsed acl  */
    if (aclp && (aclp->dfs_acl_len > 0) && (dw != VNX_ACL_REGULAR_ACL)) {
	eacl_ReleaseAcl(daclP);
    }

    lock_ReleaseWrite_r(&evp->vd_cache.lock);
    /* Release lock on file */
    tlock_ReleaseWriteNT (&evp->vd_tlock);

    /* Translate error codes */
    if (code)
	VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);

    return (code);
}
