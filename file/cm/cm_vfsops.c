/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 *      Copyright (C) 1996, 1990 Transarc Corporation
 *      All rights reserved.
 */

#include <dcedfs/param.h>		/* Should be always first */
#include <dcedfs/osi_cred.h>
#include <dcedfs/osi_mount.h>
#include <sys/uio.h>
#include <cm.h>				/* Cm-based standard headers */
#ifdef AFS_OSF_ENV
#include <dcedfs/xvfs_vnode.h>
#endif /* AFS_OSF_ENV */
#include <cm_cell.h>
#include <cm_conn.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/cm/RCS/cm_vfsops.c,v 4.82 1996/03/20 21:11:38 andi Exp $")

#ifdef AFS_AIX31_ENV
#include <sys/statfs.h>
#endif

int cm_mount(), cm_unmount(), cm_root(), cm_statfs();
#ifdef AFS_SUNOS53_ENV
int cm_sync(struct osi_vfs *, short, osi_cred_t *);
#else /* AFS_SUNOS53_ENV */
int cm_sync();
#endif /* AFS_SUNOS53_ENV */
extern int cm_invalop();
int cm_vget();
#ifdef	AFS_HPUX_ENV
int cm_getmount();
#endif
#ifdef AFS_OSF_ENV
int cm_start(), cm_quotactl(), cm_fhtovp(), cm_vptofh(), cm_init();
#endif /* AFS_OSF_ENV */
#ifdef	AFS_HPUX_ENV
/*
 * AFS private data per mounted file system
 */
#ifndef	MAXMNTLEN
#define MAXMNTLEN 512	/* keep consistent with fs.h/cdfs.h/filsys.h/nfs_clnt.h ! */
#endif	/* MAXMNTLEN */
struct afsmntinfo {
#ifdef GETMOUNT
	char	         mi_fsmnt[MAXMNTLEN]; 	/* mounted on dir */
#endif
};
#endif	/* AFS_HPUX_ENV */

#ifdef AFS_OSF_ENV
#include <ufs/quota.h>
#include <ufs/ufsmount.h>
struct ufsmount afs_ufsmp;
#endif /* AFS_OSF_ENV */

struct osi_vfs *cm_globalVFS = 0;
struct cm_scache *cm_rootVnode = 0;
char *cm_rootName = 0;
int cm_rootNameLen = 0;

#ifdef AFS_OSF_ENV
struct vfsops afs_vfsops = {
    cm_mount,
    cm_start,
    cm_unmount,
    cm_root,
    cm_quotactl,
    cm_statfs,
    cm_sync,
    cm_fhtovp,
    cm_vptofh,
    cm_init
};
#else
struct vfsops afs_vfsops = {
    cm_mount,
    cm_unmount,
    cm_root,
    cm_statfs,
    cm_sync,
    cm_vget,
#ifdef	AFS_HPUX_ENV
    cm_getmount
#endif
#ifdef AFS_SUNOS5_ENV
    cm_invalop,		/* vfs_mountroot */
    cm_invalop		/* vfs_swapvp */
#endif
#ifdef AFS_AIX_ENV
     cm_invalop,	/* vfs_cntl */
     cm_invalop		/* vfs_quotactl */
#endif
#ifdef	AFS_DEFAULT_ENV
#error "Check your native vfsops to make sure they're here exactly right"
#endif	/* AFS_DEFAULT_ENV */
};
#endif /* AFS_OSF_ENV */

#ifdef AFS_OSF_ENV
#ifndef AFS_OSF11_ENV
static char afs_mntfromname[MNAMELEN] = "DCE File System";
#else
static char afs_mntfromname[] = "DCE File System";
#endif /* AFS_OSF11_ENV */
#endif /* AFS_OSF_ENV */

#ifdef	AFS_HPUX_ENV
static char afs_mntfromname[] = "DCE-DFS"; /* can't have spaces in the name */
#endif	/* AFS_HPUX_ENV */

/*
 * Mount the afs tree to "pathp"
 * (cfe) Under AIX31, the pathp and data args are missing.
 */

#ifdef AFS_SUNOS5_ENV
extern int dfs_fstype;		/* filled in when dfsconfig loads VFS type */
/* Solaris 2 mount */
cm_mount(vfsp, mOnVnodep, argsp, credp)
  struct vfs *vfsp;
  struct vnode *mOnVnodep;
  struct mounta *argsp;
  osi_cred_t *credp;
{
    char *pp;
    char *tbufp;
    u_int bufsize;
    int code = 0;

    if (!suser(credp))
	return EPERM;
    
    if (cm_globalVFS) {
	/* Don't allow remounts since some system don't handle it well. */
	return (EBUSY);
    }
    cm_globalVFS = vfsp;
    vfsp->vfs_fstype = dfs_fstype;
    vfsp->vfs_dev = makedevice(getudev(), 0);
    vfsp->vfs_fsid.val[0] = vfsp->vfs_dev;
    vfsp->vfs_fsid.val[1] = 0x051889;	/* Rebecca */
    vfsp->vfs_bcount = 0;
    vfsp->vfs_nsubmounts = 0;
    vfsp->vfs_flag |= VFS_NOTRUNC;
    vfsp->vfs_bsize = 8192;

    osi_PreemptionOff();
    if (cm_rootName == 0) {	/* save so we know what to put in our virt sym links */
	/* On AIX, the stub name comes in as one of many args in
	 * the ``struct vmount'' that the vfsp points to.
	 * Others pass the stub name in directly.
	 */
	pp = argsp->dir;
	tbufp = (char *)osi_AllocBufferSpace();
	code = osi_copyinstr(pp, (caddr_t)tbufp, osi_BUFFERSIZE - 1, &bufsize);
	if (code || (bufsize > MAXPATHLEN)) {
	    osi_FreeBufferSpace((struct osi_buffer *)tbufp);
	    if (bufsize > MAXPATHLEN)
		code = ENAMETOOLONG;
	    return code;
	}
	pp = tbufp;
	cm_rootNameLen = strlen(pp);
	cm_rootName = osi_Alloc(cm_rootNameLen + 2);
	strcpy(cm_rootName, pp);
	/* make sure it ends in a '/' */
	if (cm_rootName[cm_rootNameLen-1] != '/') {
	    strcpy(&cm_rootName[cm_rootNameLen], "/");
	    ++cm_rootNameLen;
	}
	osi_FreeBufferSpace((struct osi_buffer *)tbufp);
    }
    cm_UnShutdown();
    osi_RestorePreemption(0);
    return 0;
}

#else /* AFS_SUNOS5_ENV */

#ifdef AFS_OSF_ENV
cm_mount(vfsp, pathp, data, ndp)
    struct nameidata *ndp;
#else
cm_mount(vfsp, pathp, data)
#endif /* AFS_OSF_ENV */
    struct osi_vfs *vfsp;
    char *pathp;
    caddr_t data; 
{

    DEFINE_OSI_UERROR
    struct vnode *afsrootvp = NULL;
    int error;
    char *pp;
#ifdef	AFS_OSF_ENV
    struct ufs_args args;
    u_int size;
    struct vnode *cvp;
    char *tempstring;
#endif	/* AFS_OSF_ENV */


/*
 * OSF1.
 */
#ifdef AFS_OSF_ENV
#if SEC_BASE
	/*
	 * Must have the mount privilege.
	 */
	if (!privileged(SEC_MOUNT, EPERM))
		return (EPERM);
#else
	/*
	 * Must be super user
	 */
	if (error = suser(u.u_cred, &u.u_acflag))
		return (error);
#endif
	cvp = ndp->ni_vp;
	BM(MOUNT_LOCK(vfsp));
	if ((vfsp->m_flag & M_UPDATE) == 0) {
	  if (cvp->v_usecount != 1 )
		return(EBUSY);
	  if (cvp->v_type != VDIR)
		return(ENOTDIR);
	}
	BM(MOUNT_UNLOCK(vfsp));
#endif /* AFS_OSF_ENV */

/*
 * Everyone.
 */
    if (cm_globalVFS) {
	/* 
	 * Don't allow remounts since some system (like AIX) don't handle it well 
	 */
	osi_setuerror(EBUSY);
	return (EBUSY);
    }
    cm_globalVFS = vfsp;

/*
 * OSF1.
 */
#ifdef AFS_OSF_ENV
    vfsp->m_stat.f_bsize = 8192;
#else
    vfsp->vfs_bsize = 8192;
#endif /* AFS_OSF_ENV */

/*
 * AIX
 */
#if defined(AFS_AIX_ENV)
    vfsp->vfs_fsid.val[0] = AFS_VFSMAGIC;	/* magic */
    vfsp->vfs_fsid.val[1] = AFS_VFSFSID; 
#endif 

/*
 * HP/UX
 */
#ifdef	AFS_HPUX_ENV
    vfsp->vfs_fsid[0] = AFS_VFSMAGIC;		/* magic */
    vfsp->vfs_fsid[1] = vfsp->vfs_mtype;
#endif

/*
 * AIX.
 */
#ifdef	AFS_AIX_ENV
    /* 
     * For AFS, we don't allow file over file mounts! This condition should go
     * out of the ifdef and used by all OSs... 
     */
    if (vfsp->vfs_mntdover->v_type != VDIR)
	return(ENOTDIR);
    if (error = cm_root(vfsp, &afsrootvp))
	return(error);
    /* 
     * The setting of afs root is done in cm_root since that routine is called
     * often (via lookupname), and setting it there prevent us from temporarily
     * losing the root volume whenever it changes... 
     */
    /*vfsp->vfs_mntd = afsrootvp;*/
    vfsp->vfs_mntdover->v_mvfsp = vfsp;
    vfsp->vfs_flag |= VFS_REMOTE;
#endif

/*
 * OSF1.
 */
#ifdef AFS_OSF_ENV
    if (vfsp->m_vnodecovered->v_type != VDIR)
	    return(ENOTDIR);
    if (error = cm_root(vfsp, &afsrootvp))
	    return(error);
    /* 
     * The setting of afs root is done in cm_root since that routine is called
     * often (via lookupname), and setting it there prevent us from temporarily
     * losing the root volume whenever it changes... 
     */
    vfsp->m_flag &= ~M_LOCAL;

    tempstring = osi_Alloc(MAXPATHLEN);
    if (!tempstring)
	    return(ENOMEM);
    (void) osi_copyinstr(pathp, tempstring, MAXPATHLEN - 1, &size);
    vfsp->m_stat.f_mntonname = osi_Alloc(size);
    if (!vfsp->m_stat.f_mntonname) {
	    osi_Free(tempstring, MAXPATHLEN);
	    return(ENOMEM);
    }
    bcopy(tempstring, vfsp->m_stat.f_mntonname, size);
    vfsp->m_stat.f_mntfromname = osi_Alloc(sizeof(afs_mntfromname)); 
    if (!vfsp->m_stat.f_mntfromname) {
	    osi_Free(tempstring, MAXPATHLEN);
	    osi_Free(vfsp->m_stat.f_mntonname, sizeof(afs_mntfromname));
	    return(ENOMEM);
    }
    bcopy(afs_mntfromname, vfsp->m_stat.f_mntfromname, sizeof(afs_mntfromname));
    osi_Free(tempstring, MAXPATHLEN);
    bzero((char *)&afs_ufsmp, sizeof(afs_ufsmp));
    afs_ufsmp.um_dev = NODEV;		/* so we don't page onto AFS */
    vfsp->m_data = (qaddr_t)&afs_ufsmp;
    vfsp->m_flag |= M_SWAP_NEVER;	/* try to make sure we never swap */
    cm_statfs(vfsp);
#endif /* AFS_OSF_ENV */


/*
 * HPUX.
 */
#ifdef	AFS_HPUX_ENV

    if (error = cm_root(vfsp, &afsrootvp))
	return(error);

#ifdef	GETMOUNT
    {
    struct afsmntinfo *mi = NULL;	/* mount info, pointed at by vfs */

    /*
     * Copy the pathname of the 'mounted on' directory to the mount structure.
     */
    mi = (struct afsmntinfo *)kmem_alloc((u_int)sizeof(*mi));
    vfsp->vfs_data = (caddr_t)mi;
    (void) strncpy(mi->mi_fsmnt, pathp, sizeof(mi->mi_fsmnt));

    /*
     * Copy the standard string to the vfs_name.
     */
    strcpy(vfsp->vfs_name, afs_mntfromname);
    }
#endif	/* GETMOUNT */

#endif	/* AFS_HPUX_ENV */


/*
 * Template for people unlucky enough to
 * have been sent in here.
 */
#ifdef	AFS_DEFAULT_ENV
#error	"cm_mount: requires code"
#endif	/* AFS_DEFAULT_ENV */

/*
 * Common code.
 */
    if (cm_rootName == 0) {	/* save so we know what to put in our virt sym links */
	/* On AIX, the stub name comes in as one of many args in
	 * the ``struct vmount'' that the vfsp points to.
	 * Others pass the stub name in directly.
	 */
#ifdef AFS_AIX31_ENV
	pp = vmt2dataptr(vfsp->vfs_mdata, VMT_STUB);
#else /* AFS_AIX31_ENV */
	pp = pathp;
#endif /* AFS_AIX31_ENV */
	cm_rootNameLen = strlen(pp);
	cm_rootName = osi_Alloc(cm_rootNameLen + 2);
	strcpy(cm_rootName, pp);
	/* make sure it ends in a '/' */
	if (cm_rootName[cm_rootNameLen-1] != '/') {
	    strcpy(&cm_rootName[cm_rootNameLen], "/");
	    ++cm_rootNameLen;
	}
    }
    cm_UnShutdown();
    return 0;
} /* cm_mount() */
#endif /* not AFS_SUNOS5_ENV */


/*
 * Unmount afs
 */
#ifdef AFS_SUNOS5_ENV
cm_unmount (vfsp, credp)
  struct osi_vfs *vfsp;
  osi_cred_t *credp;
{
    int rc;

    if (!suser(credp))
	return (EPERM);

    osi_PreemptionOff();
    rc = cm_shutdown();
    if (rc == 0) {
	cm_globalVFS = 0;
    }
    osi_RestorePreemption(0);
    return rc;
}
#else /* AFS_SUNOS5_ENV */

#if defined(AFS_AIX_ENV) || defined(AFS_OSF_ENV)
cm_unmount (vfsp, flag)
   int flag;
#else
cm_unmount (vfsp)
#endif
    struct osi_vfs *vfsp; 
{
    int rc;
#ifdef AFS_OSF_ENV
    int error;
#endif /* AFS_OSF_ENV */

#ifdef AFS_OSF_ENV
#if SEC_BASE
	/*
	 * Must have the mount privilege.
	 */
	if (!privileged(SEC_MOUNT, EPERM))
		return (EPERM);
#else
	if (error = suser(u.u_cred, &u.u_acflag))
		return (error);
#endif
#endif /* AFS_OSF_ENV */

    rc = cm_shutdown();
    if (rc == 0) {
	cm_globalVFS = 0;
    }
    return rc;
}
#endif /* not AFS_SUNOS5_ENV */

/*
 * Get root vnode of AFS
 */
cm_root (vfsp, vpp)
    struct osi_vfs *vfsp;
    struct vnode **vpp; 
{
    struct cm_rrequest rreq;
    register struct cm_scache *scp;
    struct cm_scache *tscp;
    register long code;

#ifdef AFS_SUNOS5_ENV
    osi_PreemptionOff();
#endif /* AFS_SUNOS5_ENV */
    cm_InitReq(&rreq, osi_credp);
    if (cm_initState <= CMOP_GO) {
	code = ENXIO;			/* never finished init phase */
	goto done;
    }
    if (code = cm_EnterVnode()) 
	goto done;
    if (cm_rootVnode) {
	scp = cm_rootVnode;
#ifdef CM_DISKLESS_SUPPORT
    } else if (cm_rootVolumeName[0] != 0) {
	/* someone has specified a real root volume to use */
        /* not true, but need to wait for helper startup */
	code = cm_ConfigDisklessRoot(cm_rootCellName, cm_rootVolumeName, 
				     &tscp, &rreq);
	if (code) {
	    cm_LeaveVnode();
	    goto done;
	}
	scp = tscp;
	cm_rootVnode = scp;
#endif /* CM_DISKLESS_SUPPORT */
    } else {
	/* configure global root file system */
	if (!(scp = cm_GetVDirSCache(VDIR))) {
	    cm_LeaveVnode();
	    code = ENOENT;
	    goto done;
	}
	/* 
	 * Next, complete initialization of scp as a global root vnode. 
	 */
	cm_ConfigRootVDir(scp);
	cm_rootVnode = scp;		/* remember for later */
    }
    if (CM_RC(scp) < 20000) 
	CM_RAISERC(scp);
#ifdef SCACHE_PUBLIC_POOL 
    *vpp=cm_get_vnode(scp);
    if (*vpp == NULLVP){
      cm_PutSCache(scp);
      cm_LeaveVnode();
      code = ENOMEM;
      goto done;
    }
#endif /* SCACHE_PUBLIC_POOL */
    /* SCTOV works because refcount on vnode still high */
    (SCTOV(scp))->v_flag |= VROOT;
    cm_globalVFS = vfsp;
    *vpp = SCTOV(scp);
#ifdef	AFS_AIX_ENV
    vfsp->vfs_mntd = *vpp;	  
#endif
    cm_LeaveVnode();
    code = 0;
  done:
#ifdef AFS_SUNOS5_ENV
    osi_RestorePreemption(0);
#endif /* AFS_SUNOS5_ENV */
    return code;
}


/*
 * Sync AFS (no-op)
 */
#ifdef AFS_OSF_ENV
/* ARGSUSED */
cm_sync(vfsp, waitfor)
    struct osi_vfs *vfsp; 
    int waitfor;
#else
#ifdef AFS_SUNOS53_ENV
/* ARGSUSED */
cm_sync(struct osi_vfs *vfsp, short waitfor, osi_cred_t *credp)
#else /* AFS_SUNOS53_ENV */
/* ARGSUSED */
cm_sync(vfsp)
    struct osi_vfs *vfsp; 
#endif /* AFS_SUNOS53_ENV */
#endif /* AFS_OSF_ENV */
{
    return 0;
}


/*
 * Get (any) reasonable stats about AFS; unfortunately most make no sense
 * to global file systems and therefore we return dummies to satisfy some
 * application programs.
 */
#ifdef AFS_OSF_ENV
cm_statfs(vfsp)
    register struct osi_vfs *vfsp;
#else
cm_statfs(vfsp, statfsp)
    register struct osi_vfs *vfsp;
    osi_statfs_t *statfsp; 
#endif /* AFS_OSF_ENV */
{
#ifdef AFS_OSF_ENV
    struct statfs *statfsp; 

    statfsp = &vfsp->m_stat;
#endif /* AFS_OSF_ENV */
#ifdef AFS_OSF_ENV
    statfsp->f_type = AFS_MOUNT_AFS;
#elif AFS_SUNOS5_ENV
    strcpy(statfsp->f_basetype, OSI_FSTYPE_DFS);
    statfsp->f_namemax = 255;
#else
    statfsp->f_type = 0;
#endif /* AFS_OSF_ENV */
#ifdef AFS_OSF_ENV
    statfsp->f_bsize = 8192;
    statfsp->f_fsize = 8192;
#else

#ifdef AFS_SUNOS5_ENV
    statfsp->f_frsize = vfsp->vfs_bsize;
#endif    

    statfsp->f_bsize = vfsp->vfs_bsize;
#endif /* AFS_OSF_ENV */
    /* 
     * Fake a high number below to satisfy programs that use the ustat (for 
     * AIX), or statfs (for the rest) call to make sure that there's enough 
     * space in the device partition before storing something there (like 
     * ed(1)). The curious number, when multiplied by f_bsize of 8k, gives 9 
     * million kbytes.
     */
    statfsp->f_blocks = statfsp->f_bfree = statfsp->f_bavail = 1125000;
    statfsp->f_files = statfsp->f_ffree  = 9000000;
#ifdef	AFS_AIX_ENV
    /*
     * AIX specific stafs fields 
     */
    statfsp->f_version = 0;
    statfsp->f_vfstype = AFS_VFSFSID;
    statfsp->f_vfsnumber = vfsp->vfs_number;
    statfsp->f_nlsdirtype = statfsp->f_vfsoff = statfsp->f_vfslen =  -1;
    statfsp->f_vfsvers = -1;
#endif
#if   defined(AFS_AIX_ENV)
    statfsp->f_fsid.val[0] = AFS_VFSMAGIC;	/* magic */
    statfsp->f_fsid.val[1] = AFS_VFSFSID;
#endif
#ifdef AFS_HPUX_ENV
    statfsp->f_fsid[0] = AFS_VFSMAGIC;		/* magic */
    statfsp->f_fsid[1] = vfsp->vfs_mtype;
#endif
#ifdef AFS_OSF_ENV
    statfsp->f_fsid.val[0] = AFS_VFSMAGIC;	/* magic */
    statfsp->f_fsid.val[1] = AFS_VFSFSID;
#ifdef AFS_OSF11_ENV
    statfsp->f_linkmax = LINK_MAX;
    statfsp->f_namemax = NAME_MAX;
    statfsp->f_pathmax = PATH_MAX;
    statfsp->f_fsflags = f_notrunc;
    strcpy(statfsp->f_vfstype, "dfs");
#endif /* AFS_OSF11_ENV */
#endif /* AFS_OSF_ENV */
#ifdef AFS_SUNOS5_ENV
    statfsp->f_fsid = vfsp->vfs_fsid.val[0];
#endif /* AFS_SUNOS5_ENV */
    return 0;
}


/*
 * Get a vnode associated with a Fid handle;
 * this is called by the NFS/DFS translator.
 *
 * If we have just rebooted and NFS is giving us fids from before we crashed,
 * they may be confusing.  The cell ID's may be unrecognizable if we haven't
 * already contacted the given cells.  Worse, the cell indexes (which we use
 * in the small fid case) are not kept in stable storage and don't survive a
 * crash.  XXX.  At least local cell accesses will probably work.
 */
cm_vget(vfsp, scpp, fidp)
     struct vfs *vfsp;
     struct cm_scache **scpp;
     struct fid *fidp;
{
     long code = 0;
     struct afsFid afid;
     struct cm_rrequest rreq;
#if	CM_MAXFIDSZ<24
     struct SmallFid Sfid;
#endif	/* CM_MAXFIDSZ<24 */

#if	CM_MAXFIDSZ<20
     struct cm_cell *cellp;
     long cellindex;
#endif
	
#ifdef AFS_SUNOS5_ENV
    (void) osi_PreemptionOff();
#endif

#if	CM_MAXFIDSZ>=24

    bcopy(fidp->fid_data, (char *)&afid, sizeof (afid));

#else	/* CM_MAXFIDSZ<24 */

    bcopy(fidp->fid_data, (char *)&Sfid, sizeof(Sfid));

#if	CM_MAXFIDSZ>=20

    afid.Cell = Sfid.Cell;
    AFS_hset64(afid.Volume, 0, Sfid.Volume);
    afid.Vnode = Sfid.Vnode;
    afid.Unique = Sfid.Unique;

#else	/* CM_MAXFIDSZ<20 */

    cellindex = (Sfid.CellVolAndVnodeHigh >> 24) & 0xff;
    if (!(cellp = cm_GetCellByIndex(cellindex))) {
	if (cellindex == 0xff) {
	    /* the cell index doesn't matter! */
	    AFS_hset64(afid.Cell, 0xffffffff, 0xffffffff);
	    AFS_hset64(afid.Volume, 0xffffffff, 0xffffffff);
	} else {
#ifdef AFS_SUNOS5_ENV
	    osi_RestorePreemption(0);
#endif
	    return ENOENT;	/* XXX Better error code XXX */
	}
    } else {
	if ((cellp->lclStates & CLL_IDSET) == 0) {
	    if (code = cm_FindCellID(cellp)) {
#ifdef AFS_SUNOS5_ENV
		osi_RestorePreemption(0);
#endif
		return ESTALE;
	    }
	}
	afid.Cell = cellp->cellId;
	AFS_hset64(afid.Volume, 0,
		   (Sfid.CellVolAndVnodeHigh >> 4) & 0x000fffff);
    }
    afid.Unique = Sfid.Unique;
    afid.Vnode = (Sfid.CellVolAndVnodeHigh & 0x0000000f) << 16;
    afid.Vnode |= (u_long)(Sfid.Vnodelow & 0x0000ffff);

    if (afid.Vnode == (AFS_ROOTVNODE & 0x000fffff))
	afid.Vnode = AFS_ROOTVNODE;

#endif	/* CM_MAXFIDSZ>=20 */
#endif	/* CM_MAXFIDSZ>=24 */

    /*
     * First call FindSCache.  Don't call GetSCache yet because it chokes on
     * vdirs (fids with no volume).
     *
     * In the small-fid case (NFS fid can't hold entire AFS fid), we call a
     * special NFS-only version of FindSCache, which does a partial match,
     * using only the bits in the fid that we know.
     */
    lock_ObtainWrite(&cm_scachelock);
#if CM_MAXFIDSZ>=24
    *scpp = cm_FindSCache(&afid);
#else
    *scpp = cm_NFSFindSCache(&afid);
#endif
    lock_ReleaseWrite(&cm_scachelock);
    if (*scpp == NULL) {
	cm_InitReq(&rreq, osi_getucred());
	if (cm_GetSCache(&afid, scpp, &rreq)) {
	    code = ENOENT;
	}
	code = cm_CheckError(code, &rreq);
    }
#ifdef AFS_SUNOS5_ENV
    osi_RestorePreemption(0);
#endif
    return(code);
}

#if	defined(AFS_OSF_ENV) || defined(AFS_HPUX_ENV)

/*
 * Make a filesystem operational.
 * Nothing to do at the moment.
 */
cm_start(mp, flags)
	struct osi_vfs *mp;
	int flags;
{
	return (0);
}
/*
 * Do operations associated with quotas
 */
cm_quotactl(mp, cmds, uid, arg)
	struct osi_vfs *mp;
	int cmds;
	uid_t uid;
	caddr_t arg;
{
	return (EOPNOTSUPP);
}

struct afsFidh {
	u_short		fid_len;		/* length of data in bytes */
	u_short		fid_reserved;		/* force longword alignment */
	u_long          fid_Cell;               /* the cell id (only 32 bits) */
	afs_hyper_t fid_Volume;
	u_long          fid_Vnode;
	u_long          fid_Unique;
};

#ifdef	AFS_OSF_ENV
cm_fhtovp(mp, fhp, vpp)
	register struct osi_vfs *mp;
	struct fid *fhp;
	struct vnode **vpp;
{
    register long code;
    struct cm_rrequest rreq;
    register struct cm_scache *scp;
    struct afsFidh *fidp;
    struct afsFid myfid;

    icl_Trace0(cm_iclSetp, CM_TRACE_FHTOVP);
    cm_InitReq(&rreq, osi_credp);
    fidp = (struct afsFidh *)fhp;
    AFS_hset64(myfid.Cell, 0, fidp->fid_Cell);
    myfid.Volume = fidp->fid_Volume;
    myfid.Vnode = fidp->fid_Vnode;
    myfid.Unique = fidp->fid_Unique;
    if (cm_GetSCache(&myfid, &scp, &rreq))
	return ENOENT;
#ifdef SCACHE_PUBLIC_POOL
    *vpp = cm_get_vnode(scp);
    if (*vpp == NULLVP){
      cm_PutSCache(scp);
      return(ENOMEM);
    }
#else
    *vpp = SCTOV(scp);
#endif /* SCACHE_PUBLIC_POOL */
    return 0;
}
#endif	/* AFS_OSF_ENV */

#endif /* defined(AFS_OSF_ENV) || defined(AFS_HPUX_ENV) */

#ifdef	AFS_OSF_ENV
cm_vptofh(vp, fhp)
	struct vnode *vp;
	struct fid *fhp;
{
    struct afsFid myfid;
    struct afsFidh *fidp;
    int code;

    code = VOPX_AFSFID(vp, &myfid, /* want volume */ 1);
    if (code) return code;

    fidp = (struct afsFidh *)fhp;
    fidp->fid_len = sizeof(struct afsFidh);
    fidp->fid_Cell = AFS_hgetlo(myfid.Cell);
    fidp->fid_Volume = myfid.Volume;
    fidp->fid_Vnode = myfid.Vnode;
    fidp->fid_Unique = myfid.Unique;
    return(0);    
}

cm_init(mp)
	register struct osi_vfs *mp;
{
  return(0);
}
#endif	/* AFS_OSF_ENV */

#ifdef	AFS_HPUX_ENV

#ifdef GETMOUNT

/*
 * get mount table information
 */

int
cm_getmount(vfsp, fsmntdir, mdp)
struct vfs *vfsp;
caddr_t fsmntdir;
struct mount_data *mdp;
{
	struct afsmntinfo *mi = (struct afsmntinfo *)vfsp->vfs_data;
	int l;
	
	/*
	 * For now, our only function is to return the pathname
	 * of the 'mounted on' directory, typically "/...".
	 */
	return(copyoutstr(mi->mi_fsmnt, fsmntdir, sizeof(mi->mi_fsmnt), &l));

}
#endif	/* GETMOUNT */


#endif	/* AFS_HPUX_ENV*/
