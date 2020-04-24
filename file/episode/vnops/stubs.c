/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: stubs.c,v $
 * Revision 1.1.20.1  1996/10/02  17:45:12  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:36:28  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1996, 1990 Transarc Corporation
 * All rights reserved.
 */
/*			    Episode File System
			Stubs for testing outside the kernel
 */
#include <stdio.h>
#include <dcedfs/osi_device.h>
/* XXX FIX ME XXX */
#undef	PRIVATE
#undef	SHARED
#include <dcedfs/osi.h>
#include <dcedfs/episode/us.h>		/* also gets afs/sysincludes */
#include <dcedfs/osi_kvnode.h>
#include <dcedfs/osi_user.h>
#include <dcedfs/osi_cred.h>
#ifdef AFS_SUNOS5_ENV
#include <vm/page.h>
#include <vm/pvn.h>
#endif /* AFS_SUNOS5_ENV */

#include <efs_evnode.h>			/* struct evnode */
#include <efs_misc.h>			/* vnm_GetFileH */
#include <efs_volvnops.h>		/* VOL_RELE */
#include <efs_vnodeops.h>		/* efs_inactive */
#include <efs_tmpcopy.h>
#include <stubs.h>

#ifdef KERNEL
#error This is a user-space only file.
#endif

#include <pthread.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/stubs.c,v 1.1.20.1 1996/10/02 17:45:12 damon Exp $")

struct user u;
#ifndef AFS_AIX31_ENV
struct proc dummy_up;
#endif

osi_cred_t Ucred;

struct vnodeops dummy_bdev_vop;

/* Probably it makes no difference but since us_open uses non-standard flag
 * values, do the mapping. */
#define FLAG2OFLAGS(f) (((f) & FWRITE) ? US_OFLAGS_RDWR : US_OFLAGS_RDONLY)

#ifdef	AFS_AIX31_ENV
/* EXPORT */
int dummyopen (vp, flag)
    struct vnode *vp;
    int flag;
{
    return us_open (vp->v_rdev, FLAG2OFLAGS(flag));
}
#elif	defined(AFS_OSF_ENV)
/* EXPORT */
int dummyopen (vpp, flag, cred)
    struct vnode **vpp;
    int flag;
    osi_cred_t *cred;
{
    return us_open ((dev_t) ((*vpp)->v_specinfo), FLAG2OFLAGS(flag));
}
#elif	defined(AFS_SUNOS5_ENV)
/* EXPORT */
int dummyopen (vpp, flag, cred)
    struct vnode **vpp;
    int flag;
    osi_cred_t *cred;
{
   return us_open ((*vpp)->v_rdev, FLAG2OFLAGS(flag));
}
#elif	defined(AFS_HPUX_ENV)
#if FULLDUX
/* EXPORT */
int opend (devP, dummy1, dummy2, flag, dummy3)
    dev_t *devP;
    int dummy1, dummy2, dummy3;
    int flag;
#else
/* EXPORT */
int opend (devP, dummy1, flag, dummy2)
    dev_t *devP;
    int dummy1, dummy2;
    int flag;
#endif	/* FULLDUX */
{
    return us_open (*devP, FLAG2OFLAGS(flag));
}
#endif

#ifdef	AFS_OSF_ENV
/* EXPORT */
int dummyclose (vp, flag, cred)
    struct vnode *vp;
    int flag;
    osi_cred_t *cred;
{
    return us_close ((dev_t) vp->v_specinfo, flag);
}
#elif	defined(AFS_AIX_ENV)
/* EXPORT */
int dummyclose (vp, flag)
    struct vnode *vp;
    int flag;
{
    return us_close (vp->v_rdev, flag);
}
#elif	defined(AFS_SUNOS5_ENV)
/* EXPORT */
int dummyclose (vp, flag, count, off, cred)
    struct vnode *vp;
    int flag;
    int count;
    offset_t off;
    osi_cred_t *cred;
{
    return us_close (vp->v_rdev, flag);
}
#elif AFS_HPUX_ENV
/* EXPORT */
int closed (dev, dummy1, flag)
    dev_t dev;
    int dummy1;
    int flag;
{
    return us_close (dev, flag);
}
#else
/* EXPORT */
int dummyclose (vp, flag, cred)
    struct vnode *vp;
    int flag;
    osi_cred_t *cred;
{
    return us_close (vp->v_rdev, flag);
}
#endif

/* EXPORT */
void procinit()
{
#ifdef AFS_SUNOS5_ENV
    dummy_bdev_vop.vop_open = dummyopen;
    dummy_bdev_vop.vop_close = dummyclose;
#else
#ifndef AFS_HPUX_ENV
    dummy_bdev_vop.vn_open = dummyopen;
    dummy_bdev_vop.vn_close = dummyclose;
#endif
#endif
#if !defined(AFS_AIX31_ENV) && !defined(AFS_SUNOS5_ENV)
    u.u_procp = &dummy_up;
#endif
}

/* EXPORT */
void vn_rele (vp)
    struct vnode *vp;
{
    int last;
    int old;

    Assert (vp->v_count > 0);
    if (OSI_ISVDEV(vp)) {
	vp->v_count--;
	return;
    }

    efs_lockvp(VTOEV(vp));

    VN_DECR(vp);
    last = (vp->v_count == 0);
#if defined(AFS_SUNOS5_ENV) || defined(AFS_AIX_ENV)
    if (last) {
	osi_Reschedule();
	vp->v_count = 1;
    }
#endif

    efs_unlockvp(VTOEV(vp));
    if (last)
	efs_inactive(vp, &Ucred);
}

/* ARGSUSED */
int
vn_rdwr(rw, vp, base, len, offset, seg, ioflag, ulimit, cr, residp)
    enum uio_rw rw;
    struct vnode *vp;
    caddr_t base;
    int len;
    off_t offset;
    osi_uio_seg_t seg;
    int ioflag;
    long ulimit;
    osi_cred_t *cr;
    int *residp;
{
    return 0;
}

int suser()
{
    return (osi_GetUID(&Ucred) == 0);
}

void xrele(struct vnode *vp)
{;}

struct vnode *specvp(
    struct vnode *vp,
    dev_t dev,
    enum vtype type,
    osi_cred_t *cr)
{
    struct vnode *newvp = osi_Alloc(sizeof(struct vnode));
    bzero(newvp, sizeof(struct vnode));
    newvp->v_count = 1;
    newvp->v_type = vp->v_type;
    newvp->v_rdev = vp->v_rdev;
    return newvp;
}

/* EXPORT */
void vattr_null (vap)
	struct vattr *vap;
{
	int n;
	char *cp;

	n = sizeof (*vap);
	cp = (char *) vap;
	while (n--) *cp++ = (char) -1;

#ifdef ibm032
	vap->va_type = VINIT;
#endif
}

static struct vnode *bdevvp (
  dev_t dev)
{
    struct vnode *vp;

    vp = (struct vnode *) osi_Alloc (sizeof (struct vnode));
    vp->v_count = 1000;
#ifdef	AFS_AIX31_ENV
    vp->v_gnode = (struct gnode *) osi_Alloc (sizeof (struct gnode));
    vp->v_gnode->gn_ops = &dummy_bdev_vop;
    vp->v_rdev = dev;
#endif
#ifdef	AFS_OSF_ENV
    vp->v_specinfo = (struct specinfo *) dev;
    vp->v_op = &dummy_bdev_vop;
#endif
#if !defined(AFS_AIX_ENV) && !defined(AFS_OSF_ENV)
    vp->v_rdev = dev;
    vp->v_op = &dummy_bdev_vop;
#endif /* !AFS_AIX_ENV && !AFS_OSF_ENV */
    vp->v_type = VBLK;
    return (vp);
}

static int getvdev (fspec, devp, vpp)
    caddr_t fspec;
    dev_t *devp;
    struct vnode **vpp;
{
    int minordev;
    dev_t dev;

    if (sscanf (fspec, "%d", &minordev) != 1)
	return (EINVAL);
    dev = osi_makedev (US_MAJOR_DEV, minordev);
    if (devp)
	*devp = dev;
    if (vpp)
	*vpp = bdevvp(dev);
    return (0);
}

#if  defined(AFS_AIX31_ENV) || defined (AFS_OSF_ENV) || defined (AFS_SUNOS5_ENV)
/* EXPORT */
int copyinstr (src, dest, maxlen, lenP)
    char *src;
    char *dest;
    u_int maxlen;
    u_int *lenP;
{
    *lenP = strlen (src);
    strcpy (dest, src);
    return 0;
}

#elif defined(AFS_HPUX_ENV)
/* EXPORT */
int copyoutstr (src, dest, maxlen, lenP)
    char *src;
    char *dest;
    unsigned int maxlen;
    unsigned int *lenP;
{
    *lenP = strlen (src);
    strcpy (dest, src);
    return 0;
}
#endif /* AFS_AIX31_ENV || AFS_OSF_ENV || AFS_SUNOS5_ENV */


#ifdef AFS_HPUX_ENV
/* EXPORT */
int getmdev (name, devp, sitep)
    char *name;
    dev_t *devp;
    site_t *sitep;
{
    int code = osi_getvdev (name, devp, 0);
    if (code) return code;
    *sitep = 0;
    return 0;
}
#endif /* AFS_HPUX_ENV */


void efs_CreateDirtyZeroPages(
  struct evnode *evp,
  long start,
  long finish)
{
}

#ifdef AFS_SUNOS5_ENV
int osi_lookupname (char *pathname, enum uio_seg seg, enum symfollow follow,
    vnode_t **dvpP, vnode_t **vpP)
{
    dev_t dev;

    getvdev(pathname, &dev, vpP);
    return 0;
}

struct vnode *makespecvp (dev_t dev, enum vtype type)
{
    struct vnode *vp;

    vp = bdevvp(dev);
    return vp;
}

/*
 * convoff - converts the given data (start, whence) to the
 * given whence.
 */
int
convoff(
  struct vnode *vp,
  struct flock *lckdat,
  int whence,
  off_t offset)
{
    int error;
    long size;

    if (error = efs_getlength(vp, &size, (osi_cred_t *)0))
	return (error);

    switch (lckdat->l_whence) {
      case 0:
	break;
      case 1:
	lckdat->l_start += offset;
	break;
      case 2:
	lckdat->l_start += size;
	break;
      default:
	return (EINVAL);
    }

    if (lckdat->l_start < 0)
	return (EINVAL);

    switch (whence) {
      case 0:
	break;
      case 1:
	lckdat->l_start -= offset;
	break;
      case 2:
	lckdat->l_start -= size;
	break;
      default:
	return (EINVAL);
    }

    lckdat->l_whence = (short)whence;
    return (0);
}
#endif

#ifdef AFS_SUNOS5_ENV
int
efs_putapage(
    struct vnode *vp,
    page_t *pp,
    u_int *offp,
    u_int *lenp,
    int flags,
    osi_cred_t *credp)
{
    return EIO;
}
#endif

int
efs_GetContents(
  struct evnode *evp,
  long off,
  long len,
  daddr_t rblk,
  char **mem,
  int *memsize)
{
    long code;
    char *tmp;

    afsl_Assert(lock_Check(&evp->vd_vm.lock) != 0);
    tmp = osi_Alloc(NEXT_PAGE(len));
    /*
     * at this point mem has the latest data and page-ins
     * are blocked
     */
    code = efs_GetContentsNoVM(evp, len, rblk, tmp);
    if (code)
	return(code);
    /* simulate a pagezero() */
    if (NEXT_PAGE(len) > len)
	bzero(tmp+len, NEXT_PAGE(len) - len);
    *mem = tmp;
    *memsize = NEXT_PAGE(len);
    return(0);
}

void
efs_PutContents(
  struct evnode *evp,
  daddr_t wblk,
  long len,
  int justUnlock,
  char *mem,
  int memsize)
{
    long code;

    afsl_Assert(lock_Check(&evp->vd_vm.lock) != 0);
    if (!justUnlock) {
	code = efs_PutContentsNoVM(evp, len, wblk, mem);
	afsl_MBZ (code);
    }
    osi_Free(mem, memsize);
}

void
efs_PutContentsToInline(
  elbb_tranRec_t trans,
  struct evnode *evp,
  long length,
  char *mem,
  int memlen)
{
    long code;
    afsl_Assert(lock_Check(&evp->vd_vm.lock) != 0);
    code = epix_InsertInline (trans, EVTOA(evp), 0, length, (char *) mem);
    afsl_MBZ (code);
    osi_Free(mem, memlen);
}

int
efs_DiscardVM(
  struct evnode *evp,
  u_long oldLen,
  u_long newLen,
  osi_cred_t *credp)
{
    return 0;
}

int
efs_AvoidVMRaces(
  struct evnode *evp,
  u_long len,
  u_long blocksize,
  int shrinking)
{
    return 0;
}

int efs_StabilizeVM(
    struct evnode *evp,
    off_t off,
    long len)
{
    return 0;
}
int efs_ZeroLastPage(struct evnode *evp, long len)
{
    return 0;
}

void
efs_ZeroLastBlock(struct evnode *evp, long fileLen)
{
    return;
}

#ifdef AFS_HPUX_ENV
struct vnode *devtovp (dev_t dev)
{
    return bdevvp(dev);
}
#endif /* AFS_HPUX_ENV */

#ifdef AFS_OSF_ENV
/* this is just a dummy for now */
int osi_osflookupname(pathname, segment, followlink, parentvp, compvp)
char * pathname;			/* name to look at */
int segment;				/* tell if path in user-land
					 * (OSI_UIOUSER) or kernel-land
					 * (OSI_UIOSYS) */
int followlink;				/* whether to follow sym-links */
struct vnode **parentvp;		/* if non-zero returns the parent vnode
					 * pointer for the last component */
struct vnode **compvp;			/* non-zero => the component vnode */
{
   dev_t dev;

    getvdev(pathname, &dev, compvp);
    return 0;
}

#endif /* AFS_OSF_ENV */

#ifdef AFS_AIX_ENV
/* EXPORT */
int lookupvp (name, flag, vpP)
    char *name;
    int flag;
    struct vnode **vpP;
{
    dev_t dev;

    getvdev(name, &dev, vpP);
    return 0;
}

#endif /* AFS_AIX_ENV */

#if !defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX_ENV)

/* EXPORT */
struct buf *geteblk(size)
    int size;
{
    struct buf *bp;

    bp = (struct buf *) osi_Alloc(sizeof(struct buf));
    bp->b_un.b_addr = osi_Alloc(size);
    bp->b_bcount = size;
    return (bp);
}

#ifndef AFS_OSF_ENV
/* EXPORT */
void blkclr (addr, len)
    caddr_t addr;
    int len;
{
    bzero (addr, len);
}
#endif /* AFS_OSF_ENV */

#ifdef AFS_OSF_ENV
/* EXPORT */
void brelse(bp)
#else
/* EXPORT */
void ufs_brelse(bp)
#endif /* AFS_OSF_ENV */
    struct buf *bp;
{
    osi_Free(bp->b_un.b_addr,bp->b_bcount);
    osi_Free(bp,sizeof(struct buf));
}

#endif /* AFS_SUNOS5_ENV, AFS_AIX_ENV */

static struct volume *myVolumeRegistry;
static epi_mutex_t volregLock;

long volreg_Lookup(
  struct afsFid *fidP,
  struct volume **volpP)
{
    struct volume *volp;
    afs_hyper_t volId = fidP->Volume;
    long code = ENODEV;

    BEGIN_MUTEX (&volregLock) {
	for (volp=myVolumeRegistry; volp; volp=volp->v_next) {
	    if (VOLID_EQ(volp->v_volId, volId)) {
		VOL_HOLD (volp);
		code = 0;
		break;
	    }
	}
    } END_LOCK;
    *volpP = volp;
    return code;
}

long volreg_LookupByName(
  char *volname,
  struct volume **volpP)
{
    struct volume *volp;
    long code = ENODEV;

    BEGIN_MUTEX (&volregLock) {
	for (volp=myVolumeRegistry; volp; volp=volp->v_next) {
	    if (strcmp(volp->v_volName, volname) == 0) {
		VOL_HOLD (volp);
		code = 0;
		break;
	    }
	}
    } END_LOCK;
    *volpP = volp;
    return code;
}

long volreg_Enter(
  afs_hyper_t *volId,
  struct volume *avolp,
  char *volname)
{
    struct volume *volp;
    long code;

    BEGIN_MUTEX (&volregLock) {
	for (volp=myVolumeRegistry;
	     volp && !VOLID_EQ(volp->v_volId, *volId);
	     volp=volp->v_next)
	    /* null body */;
	if (volp) {
	    code = EEXIST;
	    EXIT_CRIT;
	}
	volp = avolp;
	afsl_Assert (VOLID_EQ(volp->v_volId, *volId) &&
		     (strcmp(volp->v_volName, volname) == 0) &&
		     (volp->v_count == 1));
	lock_Init (&volp->v_lock);
	volp->v_next = myVolumeRegistry;
	myVolumeRegistry = volp;
	code = 0;
    } END_LOCK;
    return code;
}

long volreg_Delete(
  afs_hyper_t *avolid,
  char *avolname)
{
    struct volume *volp;
    long code;

    BEGIN_MUTEX (&volregLock) {
	for (volp=myVolumeRegistry;
	     volp && !VOLID_EQ(volp->v_volId, *avolid);
	     volp=volp->v_next)
	    /* null body */;
	if (volp) {
	    /* never find this again! */
	    bzero (&volp->v_volId, sizeof(volp->v_volId));
	    code = 0;
	} else {
	    code = ENOENT;
	}
    } END_LOCK;
    return code;
}

/* EXPORT */
int volreg_Init ()
{
    epi_mutex_init (&volregLock, "volreg");
    myVolumeRegistry = NULL;
    return 0;
}

/* Volume synchronization glue for test_vnodeops */

void StartVnodeOpVol(struct volume *volp)
{
    afsl_Assert (volp->v_count > 0);
    while (1) {
	lock_ObtainWrite(&volp->v_lock);
	if (volp->v_states & VOL_BUSY) {
	    /* wait for volume to close */
	    volp->v_states |= VOL_LOOKUPWAITING;
	    osi_SleepW (&volp->v_states, &volp->v_lock);
	} else if (volp->v_states & VOL_GRABWAITING) {
	    /* give volops preference in starting */
	    volp->v_states |= VOL_LOOKUPWAITING;
	    osi_SleepW (&volp->v_states, &volp->v_lock);
	} else {
	    /* go ahead and start vnode op */
	    volp->v_activeVnops++;
	    lock_ReleaseWrite(&volp->v_lock);
	    break;
	}
    }
}

void EndVnodeOpVol(struct volume *volp)
{
    afsl_Assert (volp->v_count > 0);
    lock_ObtainWrite(&volp->v_lock);
    afsl_Assert (!(volp->v_states & VOL_BUSY) && (volp->v_activeVnops > 0));
    volp->v_activeVnops--;
    if ((volp->v_activeVnops == 0) && (volp->v_states & VOL_GRABWAITING)) {
	/* give volops preference in starting */
	osi_Wakeup(&volp->v_accStatus);
    }
    lock_ReleaseWrite(&volp->v_lock);
}

void StartVolumeOp (struct volume *volp)
{
    afsl_Assert (volp->v_count > 0);
    while (1) {
	lock_ObtainWrite(&volp->v_lock);
	if ((volp->v_states & VOL_BUSY) || (volp->v_activeVnops > 0)) {
	    /* somebody has the volume open or some vnode ops are running. */
	    volp->v_states |= VOL_GRABWAITING;
	    osi_SleepW (&volp->v_accStatus, &volp->v_lock);
	} else {
	    /* go ahead and start vnode op */
	    volp->v_states |= VOL_BUSY;
	    volp->v_states &= ~VOL_GRABWAITING;
	    lock_ReleaseWrite(&volp->v_lock);
	    break;
	}
    }
}

void EndVolumeOp (struct volume *volp)
{
    afsl_Assert (volp->v_count > 0);
    lock_ObtainWrite(&volp->v_lock);
    afsl_Assert ((volp->v_states & VOL_BUSY) && (volp->v_activeVnops == 0));
    volp->v_states &= ~VOL_BUSY;
    if (volp->v_states & VOL_GRABWAITING) {
	/* give volops preference in starting */
	osi_Wakeup(&volp->v_accStatus);
    } else if (volp->v_states & VOL_LOOKUPWAITING) {
	volp->v_states &= ~VOL_LOOKUPWAITING;
	osi_Wakeup(&volp->v_states);
    }
    lock_ReleaseWrite(&volp->v_lock);

}

int StartVnodeOp (struct evnode *evp)
{
    struct volume *volp;
    long code;

    Assert (EVTOV(evp)->v_count > 0);
    code = efs_getvolume (evp, &volp);
    if (code == ESTALE)
	return code;
    afsl_MBZ (code);
    StartVnodeOpVol(volp);
    {   struct volume *ovolp;
	code = efs_getvolume (evp, &ovolp);
	if (!code)
	    VOL_RELE (ovolp);
	if (code == ESTALE) {
	    /* We lost the race and our vnode was staled out before we got the
	     * vnode operation started.  Our caller will have no way to end the
	     * vnode operation because his vnode will no longer be attached to
	     * any volume. */
	    EndVnodeOpVol(volp);
	} else {
	    afsl_MBZ (code);
	}
    }
    VOL_RELE (volp);
    return code;
}

void EndVnodeOp (struct evnode *evp)
{
    struct volume *volp;
    long code;

    Assert (EVTOV(evp)->v_count > 0);
    code = efs_getvolume (evp, &volp);
    afsl_MBZ (code);
    EndVnodeOpVol(volp);
    VOL_RELE (volp);
}

void StartVFSOp (struct osi_vfs *vfsp)
{
    struct volume *volp;
    long code;

    code = efs_vfsgetvolume (vfsp, &volp);
    afsl_MBZ (code);
    StartVnodeOpVol(volp);
    VOL_RELE (volp);
}

void EndVFSOp (struct osi_vfs *vfsp)
{
    struct volume *volp;
    long code;

    code = efs_vfsgetvolume (vfsp, &volp);
    afsl_MBZ (code);
    EndVnodeOpVol(volp);
    VOL_RELE (volp);
}

/*
 * Volume's reference count is being decremented from 1.
 * Called with volp->v_lock write-locked.
 */
int vol_VolInactive(volp)
  struct volume *volp;
{
    Assert (volp->v_count > 0);
    volp->v_count--;
    lock_ReleaseWrite(&volp->v_lock);
    return 0;
}

/* Simulated AGOPS */
/* See also AG_EFSSETHANDLE in stubs.h */
/*
 * ag_efsGetHandle -- get Episode aggregate handle
 */
epi_anode_t ag_efsGetHandle(struct aggr *aggrp)
{
    return ((epi_anode_t)(aggrp->a_fsDatap));
}

/* EXPORT */
tkc_Init ()
{;}

/* EXPORT */
xvfs_InitFromXOps (axfuns, afuns)
int axfuns;
struct osi_vnodeops *afuns;
{;}

xvfs_InitFromVFSOps(
    struct osi_vfsops *aofuns,
    struct xvfs_vfsops *afuns,
    int (*getvolfn)())
{;}

#ifdef AFS_AIX_ENV			/* Temporary hack */
struct osi_vnodeops naix_ops;
#endif

#ifdef AFS_SUNOS5_ENV			/* Temporary hack */
struct osi_vnodeops nsun5_ops;

int efs_fstype = 37;

u_long vf_to_stf (vf)
   u_long vf;
{
   return 0;
}

struct vfssw vfssw[100];
#endif

#ifdef AFS_HPUX_ENV
int osi_MapStrategy (aproc, bp)
    int (*aproc)();
    osi_buf_t *bp;
{
    (*aproc)(bp);
    return 0;
}

int efs_vfs_mtype;
#endif

#ifdef AFS_OSF_ENV
VREF(vp)
struct vnode *vp;
{
vp->v_usecount++;
}

VUNREF(vp)
struct vnode *vp;
{
 vn_rele(vp);
}

/* dummies for device support */
int spec_lookup() {}
int spec_badop() {}
int spec_open() {}
int nosf_access() {}
int nosf_getattr() {}
int nosf_setattr() {}
int spec_ioctl() {}
int spec_select() {}
int spec_nullop() {}
int spec_seek() {}
int nosf_inactive() {}
int spec_bmap() {}
int spec_strategy() {}
int nosf_print() {}
int spec_page_read() {}
int spec_page_write() {}
int fifo_open() {}
int fifo_ioctl() {}
int fifo_select() {}
int nosf_reclaim() {}
int specalloc() {}
int spec_read() {}
int spec_write() {}
int spec_close() {}
int spec_reclaim() {}
int fifo_read() {}
int fifo_write() {}
int fifo_close() {}
int fifo_getattr() {}

#endif /* AFS_OSF_ENV */

#ifdef AFS_SUNOS5_ENV
int fs_frlock() {}
struct buf * pageio_setup(struct page *a, u_int b, struct vnode *c, int d) {}
void pageio_done(struct buf *a) {}
void page_sub(page_t **ppp, page_t *pp) {}
void page_add(page_t **ppp, page_t *pp) {}
void page_list_concat(page_t **ppp, page_t **nppp) {}
void page_list_break(page_t **oppp, page_t **nppp, u_int npages) {}
void pagezero(page_t *pp, u_int off, u_int len) {}
void pvn_read_done(struct page *pp, int flags) {}
void pvn_write_done(struct page *pp, int flags) {}
void pvn_vpzero(struct vnode *vp, u_int vplen, u_int zbytes) {}

int pvn_vplist_dirty(
  struct vnode *vp,
  u_int off,
  int (*putapage)(
	struct vnode *, page_t *, u_int *, u_int *, int, osi_cred_t *),
 int flags,
 osi_cred_t *cred)
{
    return (0);
}
#endif /* AFS_SUNOS5_ENV */

/* Temporary to enable us to link new dacl stuff */

sec_rgy_pgo_name_to_id () {;}
sec_rgy_pgo_id_to_name () {;}
RgySec_ErrorString () {;}
SysErrString () {;}

/* Dummy dacl_GetLocalCellID() */

void dacl_GetLocalCellID(afsUUID *id)
{
    static epi_uuid_t dummyID;
    *id = *((afsUUID *) &dummyID);
}

void dacl_SetLocalCellID(afsUUID *id)
{ ; }

/* Dummy dacl_GetSysAdminGroupID() */

void dacl_GetSysAdminGroupID(afsUUID *id)
{
    static epi_uuid_t dummyID;
    *id = *((afsUUID *) &dummyID);
}

void dacl_SetSysAdminGroupID(afsUUID *id)
{ ; }

void 
dacl_GetNetworkRootID(afsUUID *id)
{
    static epi_uuid_t dummyID;

    *id = *((afsUUID *)&dummyID);
}

/* Local Cell ID */

epi_uuid_t localCellID;

int osi_crequal(osi_cred_t *cred1p, osi_cred_t *cred2p) 
{
    return 0;
}

osi_cred_t *crdup(osi_cred_t *credp)
{
    return (osi_cred_t *)NULL;
}

void crfree(osi_cred_t *credp) 
{
    return;
}
