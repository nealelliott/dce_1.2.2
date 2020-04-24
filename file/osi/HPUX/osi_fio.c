/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright 1992, 1994 Transarc Corporation - All rights reserved */

/*
 * HISTORY
 * $Log: osi_fio.c,v $
 * Revision 1.1.708.1  1996/10/02  17:57:33  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:26  damon]
 *
 * $EndLog$
 */
#include <dcedfs/param.h>	/* Should be always first */

#include <dcedfs/sysincludes.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/osi_cred.h>
#include <dcedfs/lock.h>

#include <sys/user.h>
#include <sys/buf.h>

osi_cred_t osi_cred;		/* Credential struct used when calling UFS layer */
struct vattr tvattr;		/* Holds file's attributes */
struct lock_data osi_xosi;	/* lock is for tvattr */
static long osi_fioinit = 0;


/* 
 * Generic local file system Stat interface 
 */
int
osi_Stat(afile, astat)
    register struct osi_file *afile;
    register struct osi_stat *astat; 
{
    register long code;

    lock_ObtainWrite(&osi_xosi);
    code = VOP_GETATTR(afile->vnode, &tvattr, &osi_cred, VASYNC);
    if (code == 0) {
	astat->size = tvattr.va_size;
	astat->blocksize = tvattr.va_blocksize;
	astat->mtime = tvattr.va_mtime.tv_sec;
	astat->atime = tvattr.va_atime.tv_sec;
    }
    lock_ReleaseWrite(&osi_xosi);
    return code;
}


/* 
 * Generic local file system Close interface 
 */
int
osi_Close(afile)
    register struct osi_file *afile; 
{
    VN_RELE(afile->vnode);
    osi_Free((char *)afile, sizeof(struct osi_file));
    return 0;
}


/* 
 * Generic local file system truncate interface 
 */
int
osi_Truncate(afile, asize)
    register struct osi_file *afile;
    long asize; 
{
    osi_cred_t *oldCred;
    register long code;
    int null_time = 0;

    lock_ObtainWrite(&osi_xosi);
    osi_vattr_null(&tvattr);
    tvattr.va_size = asize;
    oldCred = u.u_cred;			/* remember old credentials pointer */
    u.u_cred = &osi_cred;		/* temporarily use superuser creds. */
    code = VOP_SETATTR(afile->vnode, &tvattr, &osi_cred, null_time);
    u.u_cred = oldCred;			/* restore */
    lock_ReleaseWrite(&osi_xosi);
    return code;
}

/* 
 * Generic local file system read interface 
 */
int
osi_Read(afile, aptr, asize)
    register struct osi_file *afile;
    char *aptr;
    long asize; 
{
    unsigned int resid;
    register long code;

    code = osi_rdwr(UIO_READ, afile->vnode, (caddr_t)aptr, asize,
		    afile->offset, OSI_UIOSYS, IO_UNIT, &resid);
    if (code == 0) {
	code = asize - resid;
	afile->offset += code;
    } else {
	u.u_error = code;
	code = -1;
    }
    return code;
}


/* 
 * Generic local file system write interface 
 */
int
osi_Write(afile, aptr, asize)
    register struct osi_file *afile;
    char *aptr;
    long asize; 
{
    unsigned int resid;
    register long code;

    code = osi_rdwr(UIO_WRITE, afile->vnode, (caddr_t)aptr, asize,
		    afile->offset, OSI_UIOSYS, IO_UNIT, &resid);
    if (code == 0) {
	code = asize - resid;
	afile->offset += code;
    } else {
	u.u_error = code;
	code = -1;
    }
    if (afile->proc) {
	(*afile->proc)(afile, code);
    }
    return code;
}


/*
 * Generic local file system RDWR interface
 */
osi_RDWR(afile, arw, auio)
    struct osi_file *afile;
    enum uio_rw arw;
    struct uio *auio;
{
    return VOP_RDWR(afile->vnode, auio, arw, 0, osi_credp);
}


#ifdef REGION

/*
 * Buffer's data is in userland, or in some other
 *  currently inaccessable place.  We map in a page at a time
 *  and read it in.
 */
long
afs_strat_map(aproc, bp)
    int (*aproc)();
    register struct buf *bp;
{
    register daddr_t blkno = bp->b_blkno;
    register caddr_t base_vaddr;
    register long count;
    register int pfn, nbytes, npage = 0, off;
    caddr_t vaddr;
    caddr_t old_vaddr;
    space_t old_space;
    long old_count;
    extern caddr_t hdl_kmap();
    sv_sema_t afsSS;
    long code = 0;

    /* Record old addr from bp, set to KERNELSPACE */
    old_vaddr = bp->b_un.b_addr;
    old_space = bp->b_spaddr;
    old_count = bp->b_bcount;
    bp->b_spaddr = KERNELSPACE;
    base_vaddr = (caddr_t)((unsigned long)old_vaddr & ~(NBPG-1));

    /*
     * See how much to move, set count to one page max.  If we're
     *  reading partway into a page, go to the extra trouble of
     *  figuring out the correct initial count.
     */
    off = (int)old_vaddr & (NBPG-1);
    nbytes = bp->b_bcount;
    if (off) {
	count = NBPG-off;
	if (count > nbytes)
	    count = nbytes;
    } else {
	if (bp->b_bcount > NBPG)
	    count = NBPG;
	else
	    count = bp->b_bcount;
    }

    /* Move in data in page-size chunks */
    while (nbytes > 0) {

	/* Map the page into kernel memory, point bp to it */
	pfn = hdl_vpfn(old_space, base_vaddr+ptob(npage));
	vaddr = hdl_kmap(pfn);
	bp->b_un.b_addr = vaddr+off;

	/* Set up block field */
	bp->b_blkno = blkno;
	bp->b_bcount = count;
	bp->b_flags &= ~B_DONE;

	PXSEMA(&filesys_sema, &afsSS);
	/* Do the I/O */
	code = (*aproc)(bp);
	VXSEMA(&filesys_sema, &afsSS);

	/* Remap back to the user's space */
	hdl_remap(old_space, base_vaddr+ptob(npage), pfn);

	hdl_kunmap(vaddr);

	/* Call it quits on error */
	if (geterror(bp) || bp->b_resid) break;

	/* Update counts and offsets*/
	nbytes -= count;
	blkno += btodb(count);
	++npage;
	off = 0;
    }

    /* Restore bp fields and done */
    bp->b_un.b_addr = old_vaddr;
    bp->b_spaddr = old_space;
    bp->b_bcount = old_count;

    iodone(bp);

    return 0;
}
#endif /* REGION */

/* 
 * osi_MapStrategy -- call strategy function, but with buffer's data
 *                    temporarily made accessible.
 */
long osi_MapStrategy(aproc, bp)
	int (*aproc)();
	register struct buf *bp;
{
	long code;

#ifdef REGION
	/* Buffer isn't in kernel virtual address space, so handle specially */
	if (bvtospace(bp,bp->b_un.b_addr) != KERNELSPACE) {
		code = afs_strat_map(aproc, bp);
		return code;
	}
#endif   /* REGION */

	code = (*aproc) (bp);
	return code;
}

int
osi_vptofid(struct vnode *vp, osi_fid_t *fidp)
{
    osi_fid_t *tfidp;
    int code;

    code = VOP_FID(vp, &tfidp);
    if (code != 0)
	return code;

    *fidp = *tfidp;
    freefid(tfidp);

    return 0;
}
