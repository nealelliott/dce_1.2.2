/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: aix_volops.c,v $
 * Revision 1.1.640.1  1996/10/02  21:02:12  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:45  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1993 Transarc Corporation - All rights reserved */

/*
 * Platform specific filsetops support for the AIX FS
 */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/debug.h>
#include <dcedfs/common_data.h>
#include <dcedfs/volume.h>
#include <dcedfs/xvfs_vnode.h>
#include <ufs.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ufsops/RIOS/aix_volops.c,v 1.1.640.1 1996/10/02 21:02:12 damon Exp $")

int vol_ufsOpen_mach(volp, type, errorType, handlerp)
    struct volume *volp;
    int type;
    int errorType;
    struct vol_handle *handlerp;
{
    struct osi_vfs *vfsp = UFS_AGTOVFSP(volp->v_paggrp);
    struct inode *ip;
    long rc = 0;
    long i, max_inode;

    osi_assert(vfsp);
    max_inode = UFSMAXINO(vfsp);
    /* 
     * Filesystem must be idle if there are operation types that might
     * perform modifications. Before checking inodes try to flush the
     * local glue's token cache to release inode references.
     *
     * Detect ops that are not readonly, but don't do this on the behalf
     * of detach alone.
     */
    if ((type & VOL_READONLY_OPS) != type && (type & VOL_OP_DETACH) != type) {
	(void) tkc_flushvfsp(vfsp);
	/* Check root inode first */
	if ((rc = iget(vfsp->vfs_fsid.fsid_dev, UFSROOTINO, &ip)) == 0) {
	    if ((ip->i_nlink > 0) && (ip->i_count > 3)) {
		rc = EBUSY;
	    }
	    iput(ip);
	}

	/* Check out the rest of the inodes */
	for (i = VOL_UFSFIRSTINODE; !rc && (i < max_inode); i++) {
	    if ((rc = iget(vfsp->vfs_fsid.fsid_dev, i, &ip)) == 0) {
		if ((ip->i_nlink > 0) && (ip->i_count > 1)) {
		    rc = EBUSY;
		}
		iput(ip);
	    }
	}
    }
    /* If opening volume for write then flush directory name cache */
    if ((rc == 0) && (type & VOL_DIRFLUSH_OPS)) 
	dc_purge (VTOIP((vfsp)->vfs_mntd)->i_dev);

    return rc;
}


int vol_ufsScan_mach(vfsp, index, handlerp)
    struct osi_vfs *vfsp;
    int index;
    struct vol_handle *handlerp;
{
    struct inode *ip;
    long rc = 0;

    if (iget(vfsp->vfs_fsid.fsid_dev, (ino_t)index, &ip))
	return ENOENT;
    if (ip->i_nlink == 0 && ip->i_count == 1)
	rc = ENOENT;

    if (rc == 0) {
	handlerp->fid.Vnode = (long)ip->i_number;
	handlerp->fid.Unique = (long)ip->i_gen;
    }
out:
    iput(ip);
    return rc;
}


#define IPOOL_SIZE 8
int vol_ufsCreate_mach(vfsp, index, xvattrp, handlerp, credp)
    struct osi_vfs *vfsp;
    int index;
    struct xvfs_attr *xvattrp;
    struct vol_handle *handlerp;
    struct ucred *credp;
{
    int imode, errorCode = 0;
    struct vattr *vattrp = &xvattrp->vattr;
    struct inode *ip;
    struct inode *ipool[IPOOL_SIZE];
    int   pooled_inodes, i, found;
    mode_t mode = MAKEIMODE(vattrp->va_type, (vattrp->va_mode & 07777));

    if (errorCode = (int)iget(vfsp->vfs_fsid.fsid_dev, (ino_t)index, &ip))
	return errorCode;
    /* 
     * If is not allocated then try to allocate it 
     */
    if (ip->i_nlink == 0 && ip->i_count == 1) {
	iput(ip);
	found = pooled_inodes = 0;

    	while (!errorCode && !found) {
	        if (errorCode = dev_ialloc(vfsp->vfs_fsid.fsid_dev, 
					   index, mode, &ip))
			continue;
		/*
		 * May not get the desired inode. JFS rounds down
		 * the given hint to the lowest available inode in the
		 * byte in the bitmap. Place in a pool to be
	 	 * returned at the end of the create.
		 * If returned inode number is > than desired inode
		 * then the inode is really allocated.
		 */
		if (ip->i_number == (ino_t)index) {
		    found = 1;
		} else if (ip->i_number < (ino_t)index) {
		    osi_assert(pooled_inodes < IPOOL_SIZE);
		    ipool[pooled_inodes++] = ip;
    		} else if (ip->i_number > (ino_t)index) {
		    errorCode = EBUSY;
		    ip->i_nlink = 0;
		    iput(ip);
		    continue;
		}
	}

	/*
	 * Return pooled inodes back to filesyetem
	 */
	while(--pooled_inodes >= 0) {
	    ipool[pooled_inodes]->i_nlink = 0;
	    iput(ipool[pooled_inodes]);
	}
    }

    if (!errorCode) {
    	ip->i_mode = mode;
	ip->i_nlink = (vattrp->va_type == VDIR)? 2: 1;
	ip->i_uid = vattrp->va_uid;
	ip->i_gid = vattrp->va_gid;

	switch (ip->i_mode & IFMT) {
	    case IFCHR:
	    case IFBLK:
                ip->i_rdev = vattrp->va_rdev;
                ITOGP(ip)->gn_rdev = vattrp->va_rdev;
                ITOGP(ip)->gn_chan= vattrp->va_chan;
		if (ip->i_mode & ISVTX)
			ITOGP(ip)->gn_type = VMPC;
	}

	ip->i_flag |= ICHG|IFSYNC;
	commit(1,ip);
	irele(ip);	/* dev_ialloc did an ilock */
    }

    /*
     * Setup this inode's Fid address in the vol handlerp and release inode.
     */
    if (!errorCode) {
	handlerp->fid.Vnode = (long) ip->i_number;
	handlerp->fid.Unique = (long) ip->i_gen;
	iput(ip);
    }
    return errorCode;
}


int vol_ufsDelete_mach(vfsp, fidp, credp)
    struct osi_vfs *vfsp;
    struct afsFid *fidp;
    struct ucred *credp;
{
    struct vnode *vp;
    struct inode *ip;
    long rc;

    if (rc = (int)iget(vfsp->vfs_fsid.fsid_dev, fidp->Vnode, &ip))
	return 0;	/* object does not exist */

    if (ip->i_nlink == 0) {	/* already deleted */
	iput(ip);
	goto out;
    }

    if (rc =  iptovp(vfsp, ip, &vp)) {
	iput(ip);
	goto out;
    }

    /* Truncate the object first to free disk and VMM resources */
    XVFS_CONVERT(vp);
    if (rc = VOPN_FTRUNC(vp, 0, 0, 0))
	goto vnrele;

    if (fidp->Vnode == UFSROOTINO) /* Don't really delete the root inode */
	goto vnrele;

    ilock(ip);
    ip->i_nlink = 0;
    ip->i_flag |= ICHG|IFSYNC;
    commit(1,ip);
    irele(ip);

vnrele:
    VN_RELE(vp);
out:
    return rc;
}


int vol_rdwr_mach(vp, rwFlag, position, length, bufferp, credp, outLenp)
    register struct vnode *vp;	
    enum uio_rw rwFlag;	
    afs_hyper_t position;
    int length;
    char *bufferp;
    register struct ucred *credp;
    int *outLenp;
{
    struct vattr vattr;
    register long tlen, errorCode = 0;
    afs_hyper_t htlen, hdeslen;
    struct uio tuio;
    struct iovec iov;
    long inlen = length;

    if (vp->v_type != VREG && vp->v_type != VLNK && vp->v_type != VDIR)
	return EINVAL;
    if (vp->v_type == VDIR && rwFlag == UIO_WRITE)
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
		/* NOTE: htlen must now fit 32bits, 'cuz inlen was 32bits to
                 * start */
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
	(inlen <= sizeof((VTOIP(vp))->i_symlink))) {
	struct inode *ip = VTOIP(vp);
	*outLenp = inlen;
	ilock(ip);
	bzero(ip->i_symlink, sizeof(ip->i_symlink));
	bcopy(bufferp, ip->i_symlink, inlen);
	ip->i_size = inlen;
	ip->i_flag |= ICHG|IFSYNC;
	commit(1,ip);
	irele(ip);
    } else {
	errorCode = VOPX_RDWR(vp, &tuio, rwFlag, IO_SYNC, credp);
	*outLenp = inlen - tuio.osi_uio_resid;
    }    
    return errorCode;
}


/*
 * Horrendous hack for changing the link count. 
 * The setattr vnode op does not support changing it.
 * Also disallow changing size.
 */
int vol_ufsSetattr_mach(vp, xvattrp, credp)
    struct vnode *vp;
    struct xvfs_attr *xvattrp;
    struct ucred *credp;
{
    struct inode *ip;

    ip = VTOI(vp);
    if (xvattrp->vattr.va_nlink != -1) {
	ilock(ip);
	ip->i_nlink = xvattrp->vattr.va_nlink;
	ip->i_flag |= ICHG|IFSYNC;
	commit(1, ip);				/* log changes of inode */
	irele(ip);
	xvattrp->vattr.va_nlink = -1;
    }
    xvattrp->vattr.va_size = -1;   /* Do not allow size changes */
    return 0;
}


/* 
 * Vget support which ignores the link count on the inode so that
 * deleted files can be accessed.  The DFS server needs this support.
 */
int vol_ufsVget_mach(vfsp, fidp, vpp)
    struct osi_vfs *vfsp;
    struct afsFid *fidp;
    struct vnode **vpp;
{
    int code;
    struct inode *ip;
    *vpp = 0;
    if (!(code = (int)iget(vfsp->vfs_fsid.fsid_dev, fidp->Vnode, &ip))) {
	if (fidp->Unique == ip->i_gen) {
	    if (code = iptovp(vfsp, ip, vpp))
		iput(ip);
	} else {
	    /* Uniquifier mis-match; this is the wrong vnode. */
	    /* The one that was requested really doesn't exist. */
	    iput(ip);
	    code = ENOENT;
	}
    }

    if (!code) {
	if (!*vpp) {
	    return ENOENT;
	}
	XVFS_CONVERT(*vpp);
    }
    return code;
}


#define VDIRSIZE(nlen) \
    ((sizeof (struct vol_dirent) - (OSI_MAXNAMLEN+1)) + ((nlen+1 + 3) &~ 3))

int vol_ufsReaddir_mach(vp, length, bufferp, credp, positionp, numEntriesp)
    struct vnode *vp;
    char *bufferp;
    struct ucred *credp;
    afs_hyper_t *positionp;
    int *numEntriesp;
{
    long rc, pos;
    char *ufsdirs, *voldirs;
    struct uio uio;
    struct iovec iovec;
    struct vol_dirent *vdirentp;
    struct dirent *jfsdirentp;
    long bytes_read;
    long still_data;
    long voldirs_offset;
    long entries_read;

    *numEntriesp = entries_read = 0;
    if (!(AFS_hfitsinu32(*positionp)))
        return EINVAL;

    pos = AFS_hgetlo(*positionp);

    /* Get some temporary work space */
    ufsdirs = osi_Alloc(length);
    voldirs = osi_Alloc(length);

    /* Read directories from JFS */
    iovec.iov_base = ufsdirs;
    iovec.iov_len = length;
    uio.uio_iov = &iovec;
    uio.uio_resid = length;
    uio.uio_iovcnt = 1;
    uio.uio_offset = pos;
    uio.uio_segflg = UIO_SYSSPACE;

    if (rc = VOPX_READDIR(vp, &uio, credp, (int *)0, 0))
	goto out;

    if ((bytes_read = length - uio.uio_resid) == 0) {
	goto out; /* no data to process */
    }
    /* 
     * Parse through struct dirents converting them to a buffer
     * of struct vol_dirents.
     */
    vdirentp = (struct vol_dirent *)voldirs;
    jfsdirentp = (struct dirent *) ufsdirs;
    voldirs_offset = 0;
    while (bytes_read) {
	if ((VDIRSIZE(jfsdirentp->d_namlen) + voldirs_offset) > length) {
	    break; /* done, at end of buffer */
	}
	vdirentp->codesetTag = 0;
	vdirentp->vnodeNum = ((jfsdirentp->d_ino == UFSROOTINO)? VOL_ROOTINO:
	    (jfsdirentp->d_ino - VOL_UFSFIRSTINODE));
	vdirentp->namelen = jfsdirentp->d_namlen;
	bcopy(jfsdirentp->d_name, vdirentp->name, jfsdirentp->d_namlen + 1);
	vdirentp->reclen = VDIRSIZE(vdirentp->namelen);
	vdirentp->offset = pos; /* offset where JFS entry starts */
	voldirs_offset += vdirentp->reclen;

	bytes_read -= jfsdirentp->d_reclen;
	pos = jfsdirentp->d_offset;
	entries_read++;

	jfsdirentp = (struct dirent *) 
	    ((char *)jfsdirentp + jfsdirentp->d_reclen); /* next entry */
	vdirentp = (struct vol_dirent *) 
	    ((char *)vdirentp + vdirentp->reclen); /* next entry */
    }

    /* Move the output data to the caller's buffer and set return
     * parameters
     */
    if ((rc = copyout(voldirs, bufferp, voldirs_offset)) == 0) {
	*numEntriesp = entries_read;
	AFS_hset64(*positionp, 0, pos);
    }

out:
    osi_Free(ufsdirs, length);
    osi_Free(voldirs, length);

    return rc;
}


int vol_ufsAppenddir_mach(vp, numEntries, length, bufferp, 
    preserveOffsets, credp)
    struct vnode *vp;
    u_int numEntries;
    u_int length;
    char *bufferp;
    int preserveOffsets;
    struct ucred *credp;
{
    long rc, i;
    char *indirsp;
    char outblk[DIRBLKSIZ];
    struct vol_dirent *vdirp;
    struct direct *jfsdirp, *prev_jfsdirp;
    long current_block, current_block_offset;
    long current_dir_offset;
    long min_offset;
    int ignore_vdir_offsets = 0;

    indirsp = (char *)osi_Alloc(length);
    vdirp = (struct vol_dirent *)indirsp;

    if (rc = copyin(bufferp, indirsp, length))
	goto out;

    read_last_dirblock(vp, &current_block, outblk, credp);

    prev_jfsdirp = 0;
    jfsdirp = (struct direct *)outblk;
    if (current_block < 0) {	/* empty dir */
	current_block = current_dir_offset = current_block_offset = 0;
    } else {
	/* Check for space at the end of this block for more entries */
	current_block_offset = 0;
	while (1) {
	    if ((current_block_offset + jfsdirp->d_reclen) >= DIRBLKSIZ) {
		break;
	    }
	    current_block_offset += jfsdirp->d_reclen;
	    jfsdirp = (struct direct *)((char *)jfsdirp + jfsdirp->d_reclen);
	}

	if (jfsdirp->d_reclen > LDIRSIZE(jfsdirp->d_namlen)) {
	    current_dir_offset = 
		current_block_offset + (current_block * DIRBLKSIZ);
	    prev_jfsdirp = jfsdirp;
	} else {
	    current_block++;
	    current_dir_offset = current_block * DIRBLKSIZ;
	    current_block_offset = 0;
	    jfsdirp = (struct direct *)outblk;
	}
    }

    /* Go through entries converting into JFS struct directs */
    for (i = 0; i < numEntries; i++) {
	if ((char *)vdirp >= (indirsp + length)) {
	    rc = E2BIG;
	    goto out;
	}
	if (vdirp->namelen > MAXNAMLEN) {
	    rc = EIO;
	    goto out;
	}

	/* If next entry will not fit in current block, write it out */
	if (prev_jfsdirp && (((vdirp->offset / DIRBLKSIZ) > current_block) ||
	    ((LDIRSIZE(prev_jfsdirp->d_namlen) + LDIRSIZE(vdirp->namelen) + 
	    current_block_offset) > DIRBLKSIZ))) {
	    /* Adjust last entry's reclen to go to end of block */
	    prev_jfsdirp->d_reclen = (DIRBLKSIZ - current_block_offset);

	    /* Write out current block */
	    write_dirblock(vp, current_block, outblk, credp);

	    /* reset book marks */
	    jfsdirp = (struct direct *)outblk;
	    prev_jfsdirp = 0;
	    current_block++;
	    current_block_offset = 0;
	    current_dir_offset = current_block * DIRBLKSIZ;
	}

	/* See if vdirp's offset can be used and update book marks */
	if (prev_jfsdirp) {
	    min_offset = 
		current_dir_offset + LDIRSIZE(prev_jfsdirp->d_namlen);
	    if (!ignore_vdir_offsets && 
		((vdirp->offset / DIRBLKSIZ) == current_block) &&
		(vdirp->offset >= min_offset)) {
		prev_jfsdirp->d_reclen = vdirp->offset - current_dir_offset;
	    } else {
		/* NOTE: preserve offsets flag is ignored */
		ignore_vdir_offsets = 1;
		prev_jfsdirp->d_reclen = min_offset - current_dir_offset;
	    }
	    current_block_offset += prev_jfsdirp->d_reclen;
	    current_dir_offset += prev_jfsdirp->d_reclen;
	    jfsdirp = (struct direct *)
		((char *)prev_jfsdirp + prev_jfsdirp->d_reclen);
	    prev_jfsdirp = jfsdirp;
	} else {
	    prev_jfsdirp = jfsdirp;
	}

	/* Sanity check that jfsdirp has not gone out of bounds */
	osi_assert(((long)jfsdirp > 0));
	osi_assert((((long)jfsdirp + LDIRSIZE(vdirp->namelen) - (long)outblk) 
	    <= DIRBLKSIZ));

	/* Make jfsdirp entry */
	jfsdirp->d_ino = ((vdirp->vnodeNum == VOL_ROOTINO)? UFSROOTINO:
	    (vdirp->vnodeNum + VOL_UFSFIRSTINODE));
	jfsdirp->d_namlen = vdirp->namelen;
	bcopy (vdirp->name, jfsdirp->d_name, jfsdirp->d_namlen + 1);

	/* Goto next entries */
	vdirp = (struct vol_dirent *)((char *)vdirp + vdirp->reclen);
    } /* End for */

    /* Write last dir block */
    osi_assert(prev_jfsdirp);
    prev_jfsdirp->d_reclen = DIRBLKSIZ - current_block_offset;
    write_dirblock(vp, current_block, outblk, credp);

out:
    osi_Free(indirsp, length);
    return rc;
}


static read_last_dirblock(vp, block_num, bufferp, credp)
    struct vnode *vp;
    long *block_num;
    char *bufferp;
    struct ucred *credp;
{
    long errorCode = 0;
    struct uio tuio;
    struct iovec iov;
    long dsize = VTOIP(vp)->i_size;

    if (vp->v_type != VDIR) return EINVAL;
    if ((*block_num = ((dsize / DIRBLKSIZ) - 1)) < 0)
	return;
    /*
     * Setup uio vector, and read block
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

    return errorCode;
}


static write_dirblock(vp, block_num, bufferp, credp)
    struct vnode *vp;	
    long block_num;
    char *bufferp;
    struct ucred *credp;
{
    long errorCode = 0;
    struct uio tuio;
    struct iovec iov;
    struct inode *ip = VTOIP(vp);

    if (vp->v_type != VDIR) return EINVAL;
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

    /*
     * The JFS does not allow writing to directories through the rdwr 
     * vnode op. So temporarily make the directory look like a regular
     * file so that it can be written.
     */
    ilock(ip);
    ip->i_mode &= ~(IFDIR|IFJOURNAL);
    ip->i_mode |= IFREG;
    irele(ip);

    errorCode = VOPX_RDWR(vp, &tuio, UIO_WRITE, IO_SYNC, credp);
    if (tuio.uio_resid)
	errorCode = EIO;

    /*
     * Convert inode back into a dir.
     * The ip->i_seg must also be cleaned since it is created differently
     * for a regular file vs. a directory inode.  This will cause the seg
     * to be properly created when the JFS accesses the directory through
     * the normal means.  iuncache() will do this cleanup.
     */
    ilock(ip);
    ip->i_flag |= ICHG;
    irele(ip);
    iuncache (ip);
    ilock(ip);
    ip->i_mode &= ~IFREG;
    ip->i_mode |=  (IFDIR|IFJOURNAL);
    ip->i_flag |= ICHG|IFSYNC;
    commit (1,ip);
    irele(ip);

    return errorCode;
}


int vol_ufsGetNextHoles_mach(vp, iterp, credp)
    struct vnode *vp;
    struct vol_NextHole *iterp;
    struct ucred *credp;
{
    return ENOSYS;
}


long aixmaxino(vfsp)
    register struct vfs *vfsp;
{
    struct statfs sb;

    vfstovfsop(vfsp, vfs_statfs)(vfsp, &sb);
    return sb.f_files;
}


/* 
 * Pick up the file system's creation time.
 */
int aix_fscrtime(vfsp, vsp)
    struct osi_vfs *vfsp;
    struct ufs_volData *vsp;
{
    struct vmount *vmtp;

    if (vfsp) {
	vmtp = vfsp->vfs_mdata;
	vsp->ufsData.creationDate.sec = vmtp->vmt_time;
	/* vsp->ufsData.creationDate.usec = vmtp->vmt_timepad; */ /* later */
    }
}
