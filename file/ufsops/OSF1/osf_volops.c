/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osf_volops.c,v $
 * Revision 1.1.339.1  1996/10/02  18:49:59  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:42  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1993 Transarc Corporation - All rights reserved */

/*
 * Platform-specific fileset ops support for OSF/1 UFS
 */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/debug.h>
#include <dcedfs/common_data.h>
#include <dcedfs/volume.h>
#include <dcedfs/xvfs_vnode.h>
#include <ufs.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ufsops/OSF1/osf_volops.c,v 1.1.339.1 1996/10/02 18:49:59 damon Exp $")

int vol_ufsOpen_mach(volp, type, errorType, handlerp)
	register struct volume *volp;
	int type;
	int errorType;
	struct vol_handle *handlerp;
{
	struct osi_vfs *vfsp = UFS_AGTOVFSP(volp->v_paggrp);
	struct vnode *rvp;
	struct inode *ip, *rip;
	long rc;
	long i, max_inode;

	rc = 0;
	osi_assert(vfsp);
	max_inode = UFSMAXINO(vfsp);
	/* 
	 * Filesystem must be idle if there are operation types that might
	 * perform modifications.  Before checking inodes try to flush the
	 * local glue's token cache to release inode references.  Also
	 * perform a sync to write superblock and modified inodes to
	 * disk.
	 */
	if ((type & VOL_READONLY_OPS) != type) /* writable ops */ {
		int x_error;

		(void) tkc_flushvfsp(vfsp);
#if 0
		/*
		 * In the absence of volume serialization, we would need
		 * to synchronize with other users of this filesystem using
		 * the UNMOUNT_LOCK() mechanism.  However, we assume that
		 * all readers _and_ writers to this filesystem have been
		 * stopped by now.
		 */
		UNMOUNT_WRITE_LOCK(vfsp);
		(void) vfs_busy(vfsp, 1);
#endif
		VFS_SYNC(vfsp, MNT_WAIT, x_error);  /* ignore any error */
		mntflushbuf(vfsp, B_SYNC);
		if (mntinvalbuf(vfsp)) {
			rc = EBUSY;
			goto unlock;
		}
		/*
		 * Check root inode first.
		 */
		VFS_ROOT(vfsp, &rvp, rc);
		if (rc)
			goto unlock;
		rip = VTOI(rvp);
		if (rip->i_mode == 0) {
			rc = VOL_ERR_DELETED;
		}
		/*
		 * We expect 3 refs to the root of an idle filesystem --
		 * one from mount, one from dfsexport, and one from the
		 * VFS_ROOT call above.
		 */
		if (!rc && rip->i_mode &&
			(rvp->v_usecount > 3)) {
			rc = EBUSY;
		}
		/*
		 * Check out the rest of the inodes.
		 */
		for (i = VOL_UFSFIRSTINODE; !rc && (i < max_inode); i++) {
			if ((rc = iget(rip, i, &ip, 0)) == 0) {
				if (ip->i_mode && (ITOV(ip)->v_usecount > 1)) {
					rc = EBUSY;
				}
				iput(ip);
			}
		}
unlock:
#if 0
		/*
		 * OSF/1 synch code unneeded due to volume serialization
		 * (see above).
		 */
		vfs_unbusy(vfsp);
		UNMOUNT_WRITE_UNLOCK(vfsp);
#endif
		VN_RELE(rvp);
	}
	/*
	 * If opening volume for write then flush directory name cache
	 */
	if (!rc && type & VOL_DIRFLUSH_OPS) 
		cache_purgevfs(vfsp);
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
	int rc;
	struct inode *ip;
	struct vnode *rvp;

	rc = 0;
	VFS_ROOT(vfsp, &rvp, rc);
	if (rc)
		return (rc);
	if (rc = iget(VTOI(rvp), (ino_t)index, &ip, 0)) {
		VN_RELE(rvp);
		return (rc);
	}
	VN_RELE(rvp);
	if (ip->i_mode == 0) {
		rc = VOL_ERR_DELETED;
		goto out;
	}
	handlerp->fid.Vnode = (long)ip->i_number;
	handlerp->fid.Unique = (long)vol_vnodeunique(ip);
out:
	iput(ip);
	return (rc);
}

/*
 * Sanitize a vattr struct for later processing by the OSF/1
 * ufs setattr vnode operation.  We just disable things, except
 * for the link count, which unfortunately we have to pay
 * attention to.
 */
static void vol_auxsetattr(vp, xvattrp)
	struct vnode *vp;
	register struct xvfs_attr *xvattrp;
{
	struct inode *ip;
	struct vattr *vattrp = &xvattrp->vattr;

	vattrp->va_type = VNON;
	vattrp->va_blocksize = VNOVAL;
	vattrp->va_rdev = VNOVAL;
	vattrp->va_fileid = VNOVAL;
	vattrp->va_bytes = VNOVAL;
	vattrp->va_gen = VNOVAL;
	vattrp->va_fsid = VNOVAL;
	if (vp->v_type == VDIR)
		vattrp->va_size = VNOVAL;
	if (vattrp->va_nlink != VNOVAL) {
		ip = VTOI(vp);
		IN_LOCK(ip);
		ip->i_nlink = vattrp->va_nlink;
		ip->i_flag |= ICHG;
		IN_UNLOCK(ip);
		iupdat(ip, &time, &time, 1);
		vattrp->va_nlink = VNOVAL;
	}
	return;
}

/*
 * These don't seem to be declared in any kernel header file,
 * so drag them in ourselves.
 */
extern struct vnodeops spec_inodeops, fifo_inodeops;

int vol_ufsCreate_mach(vfsp, index, xvattrp, handlerp, credp)
	struct osi_vfs *vfsp;
        long index;
	register struct xvfs_attr *xvattrp;
	register struct vol_handle *handlerp;
	register struct ucred *credp;
{
	int imode, errorCode;
	struct inode *ip, *rip;
	struct vattr *vattrp = &xvattrp->vattr;
	struct vnode *vp, *rvp;
	struct xvfs_attr lxvattr = *xvattrp;

	errorCode = 0;
	VFS_ROOT(vfsp, &rvp, errorCode);
	if (errorCode)
		return (errorCode);
	if ((ino_t)index == UFSROOTINO) {
		/*
		 * We assume that the root inode is never deallocated (see
		 * delete code).
		 */
		if (errorCode = iget(VTOI(rvp), (ino_t)index, &ip, 0)) {
			VN_RELE(rvp);
			return (errorCode);
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
		if (errorCode = ialloc(VTOI(rvp), (ino_t)index, imode, &ip)) {
			VN_RELE(rvp);
			return (errorCode);
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
	IN_LOCK(ip);
	vp = ITOV(ip);
	ip->i_flag |= IACC|IUPD|ICHG;
	ip->i_mode = imode;
	if (vattrp->va_type == VDIR)
		ip->i_nlink = 2;			/* XXX */
	else
		ip->i_nlink = 1;
	ip->i_uid = u.u_uid;
	IN_UNLOCK(ip);
	VN_LOCK(vp);
	vp->v_type = IFTOVT(imode);
	switch (ip->i_mode & IFMT) {
	case IFCHR:
	case IFBLK:
		VN_UNLOCK(vp);
		IN_LOCK(ip);
		ip->i_rdev = vattrp->va_rdev;
		IN_UNLOCK(ip);
		if (errorCode = specalloc(vp, ip->i_rdev)) {
			/*
			 * Get rid of this bogus inode.  Anyone else
			 * finding this inode in the cache will be
			 * awaken, see that there was an error, and
			 * return EIO themselves.
			 */
			VN_LOCK(vp);
			vp->v_type = VNON;
			VN_UNLOCK(vp);
			idrop(ip);
			return (errorCode);
		}
		VN_LOCK(vp);
		vp->v_op = &spec_inodeops;
		VN_UNLOCK(vp);
		break;
	case IFIFO:
		vp->v_op = &fifo_inodeops;
		VN_UNLOCK(vp);
		break;
	case IFSOCK:
		vp->v_socket = (struct socket *)vattrp->va_socket;
		VN_UNLOCK(vp);
		break;
	default:
		VN_UNLOCK(vp);
		break;
	}
	/*
	 * Set up inode's Fid address in the vol handlerp and release inode.
	 */
	handlerp->fid.Vnode = (long)ip->i_number;
	handlerp->fid.Unique = vol_vnodeunique(ip);
	vol_auxsetattr(vp, &lxvattr);
	/*
	 * Don't set a size here -- no data have been written
	 * yet and ip->i_size is zero.  Setting a non-zero
	 * size just wastes I/O bandwidth (because itrunc() is
	 * called to lengthen the inode).  For most symlinks,
	 * it's incorrect as well, because short symlinks are
	 * stored in the inode, not on disk.
	 */
	lxvattr.vattr.va_size = VNOVAL;
	XVFS_CONVERT(vp);
	errorCode = VOPX_SETATTR(vp, &lxvattr, 0, credp);
	if (errorCode) {
		ip->i_mode = 0;
		ip->i_flag |= ICHG;
	}
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
	struct vnode *vp;
	struct vnode *rvp;
	long rc;

	rc = 0;
	VFS_ROOT(vfsp, &rvp, rc);
	if (rc)
		return (rc);
	if (rc = iget(VTOI(rvp), (ino_t)fidp->Vnode, &ip, 0)) {
		VN_RELE(rvp);
		return (0);	  /* object does not exist */
	}
	VN_RELE(rvp);
	if (!ip->i_mode) {  /* already deleted */
		goto put;
	}
	/*
	 * Use setattr vnode op to truncate file length to zero.
	 * Probably, this call is unneeded, since calling iput()
	 * (and hence vrele() and VOP_INACTIVE()) with i_nlink
	 * equal to zero will truncate the inode as well.  But
	 * it's safer and certainly clearer to truncate the file
	 * here.
	 *
	 * Can't change size of a directory, though, so in that
	 * case we have to depend on the vrele() path to do the
	 * job.
	 */
	if ((ip->i_mode & IFMT) != IFDIR) {
		vp = ITOV(ip);
		XVFS_CONVERT(vp);
		vattr_null(&va);
		va.va_size = 0;
		rc = VOPX_SETATTR(vp, ((struct xvfs_attr *)&va), 0, credp);
		if (rc)
			goto put;
	}
	/*
	 * Can't use implicit truncate path on root inode (it's
	 * referenced by UFS and DFS mount code) -- no choice but
	 * to truncate (blech) ourselves.
	 */
	if (fidp->Vnode == UFSROOTINO) { 
		IN_WRITE_LOCK(ip);
		itrunc(ip, (u_long)0, IO_SYNC);
		IN_WRITE_UNLOCK(ip);
		goto put;
	}
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
		errorCode = iupdat(ip, &time, &time, 1);
		*outLenp = inlen;
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
	struct inode *ip;
	struct ufs_afsFid ufsFid;

	*vpp = 0;
	code = 0;
	/*
	 * Need to make sure that this doesn't call any GLUE functions, 
	 * or will deadlock.
	 */
	/*
	 * XXX Special case volume's ROOT XXX
	 */
	if (fidp->Vnode == UFSROOTINO)
		VFS_ROOT(vfsp, vpp, code);
	else {
		ufsFid.ufid_len = sizeof (ufsFid);
		ufsFid.ufid_pad = 0;
		ufsFid.inode = fidp->Vnode;
		ufsFid.igen = fidp->Unique;
		VFS_FHTOVP(vfsp, &ufsFid, vpp, code);
	}
	if (!code) {
		if (!*vpp) {
			return (ENOENT);
		}
		XVFS_CONVERT(*vpp);
	}
	return (code);
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
	long *endp;
	char *ufsdirs, *voldirs;
	struct uio uio;
	struct iovec iovec;
	struct vol_dirent *vdirentp;
	struct dirent *ufsdirentp;
	long bytes_read;
	long still_data;
	long voldirs_offset;
	long entries_read;

	rc = 0;
	*numEntriesp = entries_read = 0;
	if (!(AFS_hfitsinu32(*positionp)))
	    return (EINVAL);
	pos = AFS_hgetlo(*positionp);

	/* Get some temporary work space */
	ufsdirs = osi_Alloc(length);
	voldirs = osi_Alloc(length);
	/* 
	 * Parse through struct dirents converting them to a buffer
	 * of struct vol_dirents.
	 */
	vdirentp = (struct vol_dirent *)voldirs;
	ufsdirentp = (struct dirent *) ufsdirs;
	voldirs_offset = 0;
	bytes_read = 0;
	while (1) {
		osi_assert(bytes_read >= 0);
		/*
		 * Read directories from ufs
		 */
		iovec.iov_base = ufsdirs;
		iovec.iov_len = length;
		uio.uio_iov = &iovec;
		uio.uio_resid = length;
		uio.uio_iovcnt = 1;
		uio.uio_offset = pos;
		uio.uio_segflg = UIO_SYSSPACE;
		if (bytes_read == 0) {
			if (rc = VOPX_READDIR(vp, &uio, credp, endp))
				break;
			if ((bytes_read = length - uio.uio_resid) == 0) {
				break; /* no data to process */
			}
		}
		/*
		 * Check for free (empty) entry and skip it (but preserve
		 * vdirentp->offset values of following entries).
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
	tuio.osi_uio_seg = UIO_SYSSPACE;
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
	tuio.osi_uio_seg = UIO_SYSSPACE;
	iov.iov_len = DIRBLKSIZ;
	iov.iov_base = bufferp;
	tuio.uio_iov = &iov;
	tuio.uio_iovcnt = 1;

	errorCode = VOPX_RDWR(vp, &tuio, UIO_WRITE, IO_SYNC, credp);
	if (tuio.uio_resid)
		errorCode = EIO;

	return (errorCode);
}

#define LDIRSIZE(namlen) \
	((sizeof (struct dirent) - 256) + (((namlen)+1 + 3) &~ 3))

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
	 * try, but we won't otherwise.  Populating an OSF/1 UFS
	 * directory using LFS offsets causes problems for fsck
	 * and (more interestingly) the kernel.
	 *
	 * Here we assume that preserveOffsets is set only for
	 * restores of dumps made on OSF/1 UFS.
	 */
	ignore_vdir_offsets = !preserveOffsets;

	indirsp = (char *)osi_Alloc(length);
	vdirp = (struct vol_dirent *)indirsp;

	if (rc = copyin(bufferp, indirsp, length))
		goto out;

	read_last_dirblock(vp, &current_block, outblk, credp);

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
		if (vdirp->namelen > NAME_MAX) {
			rc = EIO;
			goto out;
		}
		/*
		 * If next entry will not fit in current block, write it out.
		 */
		if (prev_ufsdirp && (((vdirp->offset / DIRBLKSIZ) > current_block) ||
			((LDIRSIZE(prev_ufsdirp->d_namlen) + LDIRSIZE(vdirp->namelen) + 
			current_block_offset) > DIRBLKSIZ))) {
			/* Adjust last entry's reclen to go to end of block */
			prev_ufsdirp->d_reclen = (DIRBLKSIZ - current_block_offset);
			/*
			 * Write out current block.
			 */
			write_dirblock(vp, current_block, outblk, credp);
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
		vdirp = (struct vol_dirent *)((char *)vdirp + vdirp->reclen);
	} /* End for */
	/*
	 * Write last dir block
	 */
	osi_assert(prev_ufsdirp);
	prev_ufsdirp->d_reclen = DIRBLKSIZ - current_block_offset;
	write_dirblock(vp, current_block, outblk, credp);
out:
	osi_Free(indirsp, length);
	return (rc);
}

/* 
 * Pick up the file system's creation time -- actually, in our case,
 * the modification time of its root inode.
 */
int
osf_fscrtime(vfsp, vsp)
	struct osi_vfs *vfsp;
	struct ufs_volData *vsp;
{
	int rc;
	struct ufsmount *ump;
	struct vnode *rvp;

	if (vfsp) {
		VFS_ROOT(vfsp, &rvp, rc);
		if (rc)
			return (rc);
		if (rvp) {
			vsp->ufsData.creationDate.sec = VTOI(rvp)->i_ctime;
			vsp->ufsData.creationDate.usec = VTOI(rvp)->i_ctime_usec;
		}
		VN_RELE(rvp);
	}
	return (0);
}
