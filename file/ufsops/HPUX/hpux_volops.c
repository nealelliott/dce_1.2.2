/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: hpux_volops.c,v $
 * Revision 1.1.737.1  1996/10/02  18:49:35  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:34  damon]
 *
 * $EndLog$
 */

/* Copyright (C) 1996 Transarc Corporation - All rights reserved. */

/*
 * Platform-specific fileset ops support for HPUX UFS
 */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/debug.h>
#include <dcedfs/common_data.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ufsops/HPUX/hpux_volops.c,v 1.1.737.1 1996/10/02 18:49:35 damon Exp $")

#if 0

/*
 * It appears this is no longer necessary due to changes in
 * ../ufs.h.
 */

/* MAJOR HACK ALERT---<dcedfs/volume.h> includes <sys/dir.h>.  However, we need to include
   <sys/dirent.h> to make the following code work, and on HPUX if you have included <sys/dir.h>
   you can't include <sys/dirent.h>.  So...
*/
#include <sys/dirent.h>
#undef _SYS_DIRENT_INCLUDED
#if __hpux1000p

#define _SYS_FSDIR_INCLUDED
#include <dcedfs/volume.h>
#undef  _SYS_FSDIR_INCLUDED

#else /* __hpux1000p */

#define _SYS_DIR_INCLUDED
#include <dcedfs/volume.h>
#undef  _SYS_DIR_INCLUDED

#endif /* __hpux1000p */
#define _SYS_DIRENT_INCLUDED
#else /* 0 */
#include <dcedfs/volume.h>
#endif /* 0 */

#include <dcedfs/xvfs_vnode.h>
#include <ufs.h>

#define	IN_LOCK(a)
#define	IN_UNLOCK(a)
#define	IN_WRITE_LOCK(a)
#define	IN_WRITE_UNLOCK(a)
#define	VN_LOCK(a)
#define	VN_UNLOCK(a)

/*
 * NOTES:
 *    Should we do something special about lost+found?  Such as not
 *    dump it (i.e., skip over it in scan) ... or not delete/re-create it?
 *    (inode #3)
 */

/*
 * Size of a dir entry
 */
#define LDIRSIZE(namlen) \
	((sizeof (struct dirent) - (_MAXNAMLEN + 1)) + (((namlen)+1 + 3) &~ 3))


int		got_vfss = 0;
struct vfs	*root_vfsp = NULL;
struct vfs	*dce_vfsp = NULL;

int vol_ufsOpen_mach(volp, type, errorType, handlerp)
	register struct volume *volp;
	int type;
	int errorType;
	struct vol_handle *handlerp;
{
    struct osi_vfs *vfsp = UFS_AGTOVFSP(volp->v_paggrp);
    struct vnode *vp;
    struct inode *ip;
    struct mount *mp;
    long rc;
    long i, max_inode;


    if (got_vfss == 0) {
	/*
	 * If we haven't already, determine the filesystems holding
	 * the root directory and the DCE binaries/data.  In the
	 * interest of avoiding deadlock (not proven, but seems possible),
	 * we want to dis-allow even read type backup volume operations
	 * on them.  It's not clear that even these are enough (e.g.:
	 * what about other file systems being used for swapping -- how
	 * do we recognize these??) ... but it's better than nothing.
	 * It'd be easier calling locallookupppn() which won't go remote,
	 * but osi_lookupname() is safer.
	 */
	got_vfss = 1;
	if (osi_lookupname("/", OSI_UIOSYS, FOLLOW_LINK,
			   (struct vnode *)0, &vp) == 0) {
	    root_vfsp = vp->v_vfsp;
	    VN_RELE(vp);	
	}
	if (osi_lookupname("/opt/dcelocal", OSI_UIOSYS, FOLLOW_LINK,
			   (struct vnode *)0, &vp) == 0) {
	    dce_vfsp = vp->v_vfsp;
	    VN_RELE(vp);
	}
    }

    rc = 0;
    osi_assert(vfsp);
    mp = (struct mount *)vfsp->vfs_data;

    /*
     * Never a problem with these
     */
    if (((type & VOL_ALLOK_OPS) == type) ||
	(type == VOL_OP_DETACH))
	return 0;

    /*
     * Don't allow either readonly or read-write ops to one of the
     * file systems we decided above might result in deadlock.
     */
    if ((vfsp == root_vfsp) || (vfsp == dce_vfsp))
	return EBUSY;

    /*
     * readonly ops are otherwise ok
     */
    if ((type & VOL_READONLY_OPS) == type)
	return 0;

    /*
     * Below here, we're talking about modifications to the volume
     *
     * Dealing with quotas is too difficult.  Rather than try to
     * just delete the quota file and clear the m_qinod field (undoubtedly
     * confusing higher layers), just give up.
     */
#ifdef QUOTA
    if (mp->m_qinod != (struct inode *)NULL)
	return EBUSY;
#endif

    /*
     * Purging the entire name cache seems like overkill -- there isn't
     * a device-specific purge (except for dnlc_purge_dev(dev) under an
     * #ifdef AUTOCHANGER).
     * Also call xumount() to remove unused sticky files from the text
     * cache.
     */
    if (type & VOL_DIRFLUSH_OPS) {
	xumount(vfsp);
	dnlc_purge();
    }

    /*
     * For modification ops, make certain that the file system
     * is idle.  Before checking inodes, try to flush the local glue's
     * token cache to release inode references.  Also perform a sync
     * to write superblock and modified inodes to disk.
     */
    /*
     * Flush the glue's token cache to release vnode/inode references.
     * Also, do a sync to write the superblock and modified inodes to
     * disk (does this do any good?)
     */
    (void) tkc_flushvfsp(vfsp);
    VFS_SYNC(vfsp);

    /*
     * iflush() will return -1 if there are any inuse inodes.
     * Passing it the root vnode tells it to ignore this: since
     * it's held at least by both the mount tbl and the
     * ag_ufsData entry.
     *
     * We can only do this under an #ifdef QUOTA ... since only then
     * does iflush() take the inode argument
     */
#ifdef QUOTA
    if (iflush(mp->m_dev, VTOI(AGRTOUFS(volp->v_paggrp)->rootvp)) < 0)
	return EBUSY;
#else
    /*
     * I wonder how long this takes for a LARGE volume?
     * Starting at VOL_UFSFIRSTINODE skips over the root
     * dir.
     */
    max_inode = UFSMAXINO(vfsp);
    for (i = VOL_UFSFIRSTINODE; !rc && i < max_inode; i++) {
	if (ip = iget(mp->m_dev, mp, i)) {
	    if (ip && ip->i_mode == 0)
		idrop(ip);
	    else {
		if (ITOV(ip)->v_count > 1)
		    rc = EBUSY;
		iput(ip);
	    }
	}
    }
#endif

    return (rc);
}

static unsigned long vol_vnodeunique(ip)
	struct inode *ip;
{
		return (ip->i_gen);
}

int vol_ufsScan_mach(vfsp, index, handlerp)
	struct osi_vfs *vfsp;
        long index;
	struct vol_handle *handlerp;
{
	struct inode *ip;
	struct mount *mp;

	mp = (struct mount *)vfsp->vfs_data;
	if ((ip = iget(mp->m_dev, mp, (ino_t)index)) == NULL) {
                return (ENOENT);
	}
	if (ip->i_mode == 0) {
	        idrop(ip);
		return (VOL_ERR_DELETED);
	}
	handlerp->fid.Vnode = (long)ip->i_number;
	handlerp->fid.Unique = (long)vol_vnodeunique(ip);
out:
	iput(ip);
	return (0);
}

/*
 * Sanitize a vattr struct for later processing by the HPUX
 * ufs setattr vnode operation.  We just disable things to get past the tests in
 * ufs_vnops.c|ufs_setattr.
 */
static void vol_auxsetattr(vp, xvattrp)
	struct vnode *vp;
	register struct xvfs_attr *xvattrp;
{
	struct inode *ip;
	struct vattr *vattrp = &xvattrp->vattr;

	vattrp->va_nlink = -1;
	vattrp->va_blocksize = -1;
	vattrp->va_rdev = -1;
	vattrp->va_blocks = -1;
	vattrp->va_fsid = -1;
	vattrp->va_nodeid = -1;
#if ! __hpux1000p
	vattrp->va_rsite = -1;
	vattrp->va_fssite = -1;
#endif /* ! __hpux1000p */
	vattrp->va_realdev = -1;
	vattrp->va_type = -1;
	if (vp->v_type == VDIR)
		vattrp->va_size = -1;
	return;
}

int vol_ufsCreate_mach(vfsp, index, xvattrp, handlerp, credp)
	struct osi_vfs *vfsp;
        long index;
	register struct xvfs_attr *xvattrp;
	register struct vol_handle *handlerp;
	register struct ucred *credp;
{
	int imode, errorCode;
	struct inode *ip;
	struct vattr *vattrp = &xvattrp->vattr;
	struct vnode *rvp;
	struct mount *mp;

	errorCode = VFS_ROOT(vfsp, &rvp, NULL);
	if (errorCode)
		return (errorCode);
	if ((ino_t)index == UFSROOTINO) {
		/*
		 * We assume that the root inode is never deallocated (see
		 * delete code).
		 */
	        mp = (struct mount *)vfsp->vfs_data;
		if ((ip = iget(mp->m_dev, mp, (ino_t)index)) == NULL) {
			VN_RELE(rvp);
			return (ENOENT);
		}
		imode = ip->i_mode;
	}
	else {
		/*
		 * Try to allocate the requested inode -- and check below to
                 * see if we got the one we requested.  This inode should
		 * _always_ be unallocated, since a delete volop should have
		 * preceded this create if one was needed.
		 */
		imode = MAKEIMODE(vattrp->va_type, vattrp->va_mode);
		if ((ip = ialloc(VTOI(rvp), (ino_t)index, imode)) == NULL) {
			VN_RELE(rvp);
                        return (ENFILE);
		}
	}
	VN_RELE(rvp);
	/*
	 * If we don't get the desired inode number (i.e. position) we quit
	 * and return an error.
	 */
	if (ip->i_number != (ino_t)index) {
		ip->i_mode = 0;
		ip->i_flag |= ICHG;
		iput(ip);
		return (EBUSY);
	}

	ip->i_flag |= IACC|IUPD|ICHG;
	ip->i_mode = imode;
	if (vattrp->va_type == VDIR)
	    ip->i_nlink = 2;
	else
	    ip->i_nlink = 1;
	switch (imode & IFMT) {
	  case IFBLK:
	  case IFCHR:
#if ! __hpux1000p
	    ip->i_rsite = 0;
#endif
	    /* vattrp->va_rdev not being set to anything yet */
	    ip->i_vnode.v_rdev = ip->i_rdev = ip->i_device = 0;
	    break;
	  case IFSOCK:
	    errorCode = EOPNOTSUPP;
	    goto put;
	}
	ip->i_uid = vattrp->va_uid;
	ip->i_gid = vattrp->va_gid;
	ip->i_vnode.v_type = vattrp->va_type;

	/*
	 * ip->i_dquot		should already be NULL (see ufs_dir.c check)
	 * ip->i_size, i_blocks	should already be (unless we're the ROOT dir)
	 */


	/*
	 * Set up inode's Fid address in the vol handlerp and release inode.
	 */
	handlerp->fid.Vnode = (long)ip->i_number;
	handlerp->fid.Unique = vol_vnodeunique(ip);

put:
	iput(ip);
	return (errorCode);
}

int vol_ufsDelete_mach(vfsp, fidp, credp)
	struct osi_vfs *vfsp;
	register struct afsFid *fidp;
	struct ucred *credp;
{
	struct inode *ip;
	struct vattr va;
	struct mount *mp;
	long rc = 0;
	u_long offst;

	mp = (struct mount *)vfsp->vfs_data;
	if ((ip = iget(mp->m_dev, mp, (ino_t)fidp->Vnode)) == NULL) {
		return (0);	  /* object does not exist */
	}

	if (!ip->i_mode) {  /* already deleted */
	    idrop(ip);
	    return (0);
	}

	/*
	 * Truncate the file/directory to zero size.
	 * If this is the root dir: the system (and fsck) will be
	 * unhappy if we don't get around to sticking "." and ".."
	 * back in (via appenddir()) -- but leaving JUST those
	 * entries there is more trouble than it seems to be worth.
	 * Doing this via a VOPX_ operation seems to hang in in ufs
	 * code (since WE did an iget() above).
	 */
	if (rc = itrunc(ip, 0))
	    goto put;

	/*
	 * If this is the root directory, we can't actually
	 * delete it.
	 */
	if (fidp->Vnode == UFSROOTINO)
	    goto put;

	IN_LOCK(ip); 
	ip->i_nlink = 0;
	ip->i_flag |= ICHG;
	IN_UNLOCK(ip);
put:
	iput(ip);
	return rc;
}


int vol_rdwr_mach(vp, rwFlag, position, length, bufferp, credp, outLenp)
	register struct vnode *vp;  
	enum uio_rw rwFlag; 
	afs_hyper_t position;
	long length;
	char *bufferp;
	register struct ucred *credp;
	long *outLenp;
{
	struct vattr vattr;
	register long tlen, errorCode = 0;
	afs_hyper_t htlen, hdeslen;
	struct uio tuio;
	struct iovec iov;
	long inlen = length;

	if (vp->v_type != VREG && vp->v_type != VLNK && vp->v_type != VDIR)
		return EINVAL;
	
	if (rwFlag == UIO_READ) {
		if (errorCode = VOPX_GETATTR(vp, ((struct xvfs_attr *)&vattr), 0, credp))
			return errorCode;
		tlen = vattr.va_size;

		/* xxx if va_size is 64-bits, change the following line: xxx */
		AFS_hset64(htlen, 0, tlen);

		if (AFS_hcmp(position, htlen) > 0) { /* position > tlen? */
			inlen = 0;		/* don't read past end of file */
		} else {
			hdeslen = position;
			AFS_hadd32(hdeslen, inlen);

			if (AFS_hcmp(hdeslen, htlen) > 0) { /*  position + inlen > tlen ? */
				AFS_hsub(htlen, position); /* inlen = tlen - position  */
				/* NOTE: htlen must now fit 32bits, 'cuz inlen
                                 * was 32bits to start */
				inlen = AFS_hgetlo(htlen);
			}
		}
	} /* UIO_READ */

	/*
	 * Setup uio vector
	 */
	tuio.osi_uio_iov = &iov;
	tuio.osi_uio_iovcnt = 1;
	if (osi_uio_set_offset(tuio, position))
	  return EINVAL;

	tuio.osi_uio_resid = inlen;
	tuio.osi_uio_seg = OSI_UIOUSER;
	iov.iov_base = bufferp;
	iov.iov_len = inlen;

	if ((vp->v_type == VLNK) && (rwFlag == UIO_READ)) {
		errorCode = VOPX_READLINK(vp, &tuio, credp);	
		*outLenp = inlen - tuio.osi_uio_resid;
#ifdef CREATE_FASTLINKS
	/*
	 * As we're building currently, never true
	 */
	} else if ((vp->v_type == VLNK) && (rwFlag == UIO_WRITE) && 
		(inlen <= MAX_FASTLINK_SIZE)) {
		struct inode *ip = VTOI(vp);

		bcopy(bufferp, ip->i_symlink, inlen);
		IN_WRITE_LOCK(ip);
		IN_LOCK(ip); 
		ip->i_size = inlen;
		ip->i_symlink[inlen] = '\0';
		ip->i_flags |= IC_FASTLINK;
		ip->i_flag |= IACC|IUPD|ICHG;
		IN_UNLOCK(ip);
		IN_WRITE_UNLOCK(ip);
#if	__hpux1000p
		errorCode = iupdat(ip, BP_BWRITE);
#else	/* __hpux1000p (but since no FASTLINK on 800 pre10, following is only for 700)  */
#define	WAIT_FOR_IO 	1	/* need to wait for a sync'd write, just in case - for now */
#if	__hp9000s700
#define REORDER_IO_AT_WILL 0	/* since this change is *within* a single inode... */
		errorCode = iupdat(ip, WAIT_FOR_IO, REORDER_IO_AT_WILL);
#else	/* __hp9000s700 */
		errorCode = iupdat(ip, WAIT_FOR_IO);
#endif	/* __hp9000s700 */
#endif	/* __hpux1000p */
		*outLenp = inlen;
#endif /* MAX_FASTLINK_SIZE */
	} else {
		errorCode = VOPX_RDWR(vp, &tuio, rwFlag, IO_SYNC, credp);
		*outLenp = inlen - tuio.osi_uio_resid;
	}   
	return errorCode;
}

int vol_ufsSetattr_mach(vp, xvattrp, credp)
	struct vnode *vp;
	register struct xvfs_attr *xvattrp;
	register struct ucred *credp;
{
	struct inode *ip;

	ip = VTOI(vp);
	if (xvattrp->vattr.va_nlink != -1) {
	    /* VOP_SETATTR won't allow us to set .va_nlink ... so do it
	     * manually here. */
	    ip->i_nlink = xvattrp->vattr.va_nlink;
	    ip->i_flag |= ICHG;
	    xvattrp->vattr.va_nlink = -1;
        }

	vol_auxsetattr(vp, xvattrp);
	return (0);
}


/* 
 * Vget support that ignores the link count on the inode so that
 * deleted files can be accessed.  The DFS server needs this support.
 */
int vol_ufsVget_mach(vfsp, fidp, vpp)
    struct osi_vfs *vfsp;
    register struct afsFid *fidp;
    struct vnode **vpp;
{
    int code;
    struct ufs_afsFid ufsFid;

    *vpp = 0;
    code = 0;

    /* Need to make sure that this doesn't call any GLUE functions, or will
     * deadlock.
     */

    /*
     * XXX Special case volume's ROOT XXX
     */
    if (fidp->Vnode == UFSROOTINO) {
	code = VFSX_ROOT(vfsp, vpp, NULL);
    } else {
	/*
	 * Map to a ufs fid struct
	 */
	ufsFid.volume = 0;	/* ignored */
	ufsFid.inode = fidp->Vnode;
	ufsFid.igen = fidp->Unique;
	code = VFSX_VGET(vfsp, vpp, &ufsFid);
    }

    if (!code) {
	/*
	 * A zero return from VFS_VGET isn't a guarantee for a successful call!
	 */
	if (!*vpp) {
	    return ENOENT;
	}
	XVFS_CONVERT(*vpp);
    }
    return code;
}

int vol_ufsGetNextHoles_mach(vp, iterp, credp)
    struct vnode *vp;
    register struct vol_NextHole *iterp;
    struct ucred *credp;
{
    return ENOSYS;
}

#define VDIRSIZE(nlen) \
	((sizeof (struct vol_dirent) - (OSI_MAXNAMLEN+1)) + ((nlen+1 + 3) &~ 3))

int vol_ufsReaddir_mach(vp, length, bufferp, credp, positionp, numEntriesp)
	struct vnode *vp;
	char *bufferp;
	struct ucred *credp;
	afs_hyper_t *positionp;
	u_long *numEntriesp;
{
	long rc, pos;
	char *ufsdirs, *voldirs;
	struct uio uio;
	struct iovec iovec;
	struct vol_dirent *vdirentp;
	struct dirent *ufsdirentp;
	long bytes_read;
	long still_data;
	long voldirs_offset;
	long entries_read;
	int eofflag;

	rc = 0;
	*numEntriesp = entries_read = 0;
	if (!(AFS_hfitsinu32(*positionp)))
	    return (EINVAL);
	pos = AFS_hgetlo(*positionp);

	/* Get some temporary work space */
	ufsdirs = osi_Alloc(length);
	voldirs = osi_Alloc(length);

	/*
	 * Read directories from ufs
	 */
	iovec.iov_base = ufsdirs;
	iovec.iov_len = length;
	uio.uio_iov = &iovec;
	uio.uio_resid = length;
	uio.uio_iovcnt = 1;
	uio.uio_offset = pos;
	uio.uio_seg = UIOSEG_KERNEL;

	/* The "0" tells VOPX_READDIR() to return
	 * native os format dir entries.  Although we
	 * don't get good offsets, passing in a "1" is
	 * little better since it gives us the offset
	 * of the NEXT entry ... and we want the offset
	 * of THIS entry.  To get good offsets would require
	 * pawing through the directory itself (as is done in
	 * xufs_readdir()) -- which just isn't worth the bother.
	 */
	if (rc = VOPX_READDIR(vp, &uio, credp, &eofflag, 0))
		goto out;
	if ((bytes_read = length - uio.uio_resid) == 0) {
		goto out; /* no data to process */
	}
	/* 
	 * Parse through struct dirents converting them to a buffer
	 * of struct vol_dirents.
	 */
	vdirentp = (struct vol_dirent *)voldirs;
	ufsdirentp = (struct dirent *) ufsdirs;
	voldirs_offset = 0;
	while (bytes_read > 0) {

	    /*
	     * Check for free (empty) entry and skip it (but preserve
	     * vdirentp->offset values of following entries).
	     *
	     * We're not getting good offsets (see above), but does that
	     * really matter for UFS filesets?
	     */
	    if (ufsdirentp->d_ino) {
		if ((VDIRSIZE(ufsdirentp->d_namlen) + voldirs_offset) > length) {
			break; /* done, at end of buffer */
		}
		vdirentp->codesetTag = 0;
		vdirentp->vnodeNum = ((ufsdirentp->d_ino == UFSROOTINO)? VOL_ROOTINO:
			(ufsdirentp->d_ino - VOL_UFSFIRSTINODE));
		vdirentp->namelen = ufsdirentp->d_namlen;
		bcopy(ufsdirentp->d_name, vdirentp->name, ufsdirentp->d_namlen + 1);
		vdirentp->reclen = VDIRSIZE(vdirentp->namelen);
		vdirentp->offset = pos; /* offset where ufs entry starts */
		voldirs_offset += vdirentp->reclen;
		entries_read++;
		vdirentp = (struct vol_dirent *) 
			((char *)vdirentp + vdirentp->reclen); /* next entry */
	    }
	    bytes_read -= ufsdirentp->d_reclen;
	    pos += ufsdirentp->d_reclen;
	    ufsdirentp = (struct dirent *) 
		((char *)ufsdirentp + ufsdirentp->d_reclen); /* next entry */
	}

	/*
	 * Move the output data to the caller's buffer and set return
	 * parameters
	 */
	if ((rc = copyout(voldirs, bufferp, voldirs_offset)) == 0) {
		*numEntriesp = entries_read;
		AFS_hset64(*positionp, 0, pos);
	}
out:
	osi_Free(ufsdirs, length);
	osi_Free(voldirs, length);
	return (rc);
}

static
int
read_last_dirblock(vp, block_num, bufferp, credp)
	struct vnode *vp;
	long *block_num;
	char *bufferp;
	struct ucred *credp;
{
	long errorCode = 0;
	struct uio tuio;
	struct iovec iov;
	long dsize = VTOI(vp)->i_size;

	if (vp->v_type != VDIR)
		return (EINVAL);
	if ((*block_num = ((dsize / DIRBLKSIZ) - 1)) < 0)
		return (0);
	/*
	 * Set up uio vector, and read block.
	 */
	tuio.uio_offset = (*block_num) * DIRBLKSIZ;
	tuio.osi_uio_resid = DIRBLKSIZ;
	tuio.osi_uio_seg = UIOSEG_KERNEL;
	iov.iov_len = DIRBLKSIZ;
	iov.iov_base = bufferp;
	tuio.uio_iov = &iov;
	tuio.uio_iovcnt = 1;

	errorCode = VOPX_RDWR(vp, &tuio, UIO_READ, IO_SYNC, credp);
	if (tuio.uio_resid)
		errorCode = EIO;

	return (errorCode);
}


static
int
write_dirblock(vp, block_num, bufferp, credp)
	struct vnode *vp;   
	long block_num;
	char *bufferp;
	struct ucred *credp;
{
	long errorCode = 0;
	struct uio tuio;
	struct iovec iov;
	struct inode *ip = VTOI(vp);

	if (vp->v_type != VDIR)
		return (EINVAL);
	/*
	 * Setup uio vector, and read block
	 */
	tuio.uio_offset = block_num * DIRBLKSIZ;
	tuio.osi_uio_resid = DIRBLKSIZ;
	tuio.osi_uio_seg = UIOSEG_KERNEL;
	iov.iov_len = DIRBLKSIZ;
	iov.iov_base = bufferp;
	tuio.uio_iov = &iov;
	tuio.uio_iovcnt = 1;

	errorCode = VOPX_RDWR(vp, &tuio, UIO_WRITE, IO_SYNC, credp);
	if (tuio.uio_resid)
		errorCode = EIO;

	return (errorCode);
}


int vol_ufsAppenddir_mach(vp, numEntries, length, bufferp, 
	preserveOffsets, credp)

	struct vnode *vp;
	u_long numEntries;
	u_long length;
	char *bufferp;
	int preserveOffsets;
	struct ucred *credp;
{
	long rc, i;
	char *indirsp;
	char outblk[DIRBLKSIZ];
	struct vol_dirent *vdirp;
	struct dirent *ufsdirp, *prev_ufsdirp;
	long current_block, current_block_offset;
	long current_dir_offset;
	long min_dir_offset;
	int ignore_vdir_offsets;

	/*
	 * If we've been asked to preserve directory offsets, we'll
	 * try, but we won't otherwise.  Populating an HPUX UFS
	 * directory using LFS offsets causes problems for fsck
	 * and (more interestingly) the kernel.
	 *
	 * Here we assume that preserveOffsets is set only for
	 * restores of dumps made on HPUX UFS.
	 *
	 * For now, don't bother trying to preserve offsets.  Although
	 * it can be done (see vol_ufsReaddir_mach), it's not clear what
	 * benefit there is.  Code was:
	 * 	ignore_vdir_offsets = !preserveOffsets;
	 */
	ignore_vdir_offsets = 1;

	indirsp = (char *)osi_Alloc(length);
	vdirp = (struct vol_dirent *)indirsp;

	if (rc = copyin(bufferp, indirsp, length))
		goto out;

	if (rc = read_last_dirblock(vp, &current_block, outblk, credp))
	    goto out;

	prev_ufsdirp = 0;
	ufsdirp = (struct dirent *)outblk;
	if (current_block < 0) {	/* empty dir */
		current_block = current_dir_offset = current_block_offset = 0;
	} else {
		/* Check for space at the end of this block for more entries */
		current_block_offset = 0;
		while (1) {
			if ((current_block_offset + ufsdirp->d_reclen) >= DIRBLKSIZ) {
				break;
			}
			current_block_offset += ufsdirp->d_reclen;
			ufsdirp = (struct dirent *)((char *)ufsdirp + ufsdirp->d_reclen);
		}
		if (ufsdirp->d_reclen > LDIRSIZE(ufsdirp->d_namlen)) {
			current_dir_offset = 
				current_block_offset + (current_block * DIRBLKSIZ);
			prev_ufsdirp = ufsdirp;
		}
		else {
			current_block++;
			current_dir_offset = current_block * DIRBLKSIZ;
			current_block_offset = 0;
			ufsdirp = (struct dirent *)outblk;
		}
	}
	/*
	 * Go through entries converting into UFS struct dirents.
	 */
	for (i = 0; i < numEntries; i++) {
		if ((char *)vdirp >= (indirsp + length)) {
			rc = E2BIG;
			goto out;
		}
		if (vdirp->namelen > _MAXNAMLEN) {
			rc = EIO;
			goto out;
		}
		/*
		 * If next entry will not fit in current block, write it out.
		 *
		 * For now, don't bother with maintaining offsets.  Code used to be
		 * the following:
		 * if (prev_ufsdirp && (((vdirp->offset / DIRBLKSIZ) > current_block) ||
		 *	((LDIRSIZE(prev_ufsdirp->d_namlen) + LDIRSIZE(vdirp->namelen) + 
		 *	current_block_offset) > DIRBLKSIZ))) {
		 */
		if (prev_ufsdirp &&
			(LDIRSIZE(prev_ufsdirp->d_namlen) + LDIRSIZE(vdirp->namelen) + 
			current_block_offset > DIRBLKSIZ)) {
			/* Adjust last entry's reclen to go to end of block */
			prev_ufsdirp->d_reclen = (DIRBLKSIZ - current_block_offset);
			/*
			 * Write out current block.
			 */
			if (rc = write_dirblock(vp, current_block, outblk, credp))
			    goto out;
			/*
			 * Reset bookkeeping data.
			 */
			ufsdirp = (struct dirent *)outblk;
			prev_ufsdirp = 0;
			current_block++;
			current_block_offset = 0;
			current_dir_offset = current_block * DIRBLKSIZ;
		}
		/*
		 * See if vdirp's offset can be used and update bookkeeping data.
		 */
		if (prev_ufsdirp) {
			min_dir_offset = 
				current_dir_offset + LDIRSIZE(prev_ufsdirp->d_namlen);
			if (!ignore_vdir_offsets && 
				((vdirp->offset / DIRBLKSIZ) == current_block) &&
				(vdirp->offset >= min_dir_offset)) {
				prev_ufsdirp->d_reclen = vdirp->offset - current_dir_offset;
			} else {
				ignore_vdir_offsets = 1;
				prev_ufsdirp->d_reclen = min_dir_offset - current_dir_offset;
			}
			current_block_offset += prev_ufsdirp->d_reclen;
			current_dir_offset += prev_ufsdirp->d_reclen;
			ufsdirp = (struct dirent *)
				((char *)prev_ufsdirp + prev_ufsdirp->d_reclen);
			prev_ufsdirp = ufsdirp;
		} else {
			prev_ufsdirp = ufsdirp;
		}
		/*
		 * Sanity check that ufsdirp has not gone out of bounds.
		 */
		osi_assert(((long)ufsdirp >= (long)outblk));
		osi_assert((((long)ufsdirp + LDIRSIZE(vdirp->namelen) - (long)outblk) 
			<= DIRBLKSIZ));
		/*
		 * Make ufsdirp entry
		 */
		ufsdirp->d_ino = (vdirp->vnodeNum == VOL_ROOTINO) ? UFSROOTINO :
			(vdirp->vnodeNum + VOL_UFSFIRSTINODE);
		ufsdirp->d_namlen = vdirp->namelen;
		bcopy (vdirp->name, ufsdirp->d_name, ufsdirp->d_namlen + 1);
		/*
		 * Goto next entries
		 */
next_ent:
		vdirp = (struct vol_dirent *)((char *)vdirp + vdirp->reclen);
	} /* End for */
	/*
	 * Write last dir block
	 */
	osi_assert(prev_ufsdirp);
	prev_ufsdirp->d_reclen = DIRBLKSIZ - current_block_offset;
	rc = write_dirblock(vp, current_block, outblk, credp);
out:
	osi_Free(indirsp, length);
	return (rc);
}

/* 
 * Pick up the file system's creation time -- actually, in our case,
 * the modification time of its root inode.
 */
int
hpux_fscrtime(vfsp, vsp)
	struct osi_vfs *vfsp;
	struct ufs_volData *vsp;
{
	int rc;
	struct ufsmount *ump;
	struct vnode *rvp;

	if (vfsp) {
		rc = VFS_ROOT(vfsp, &rvp, NULL);
		if (rc)
			return (rc);
		if (rvp) {
			vsp->ufsData.creationDate.sec = VTOI(rvp)->i_ctime.tv_sec;
			vsp->ufsData.creationDate.usec = VTOI(rvp)->i_ctime.tv_usec;
		}
		VN_RELE(rvp);
	}
	return (0);
}
