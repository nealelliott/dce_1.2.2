/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1994, 1992 Transarc Corporation
 * All rights reserved.
 */
/*
 *    Misc. support for Episode VM integration
 */
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/osi_kvnode.h>
#include <dcedfs/osi_vmm.h>
#include <dcedfs/osi_buf.h>
#include <efs_evnode.h>
#include <efs_vnodeops.h>
#include <efs_tmpcopy.h>

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RIOS/RCS/efs_aixsubr.c,v 1.27 1995/04/20 14:03:09 ota Exp $")

#ifdef	KERNEL				/* entire file is in this ifdef */
/*
 * vnvm_fsync -- clean all VM pages associated with a vnode
 *
 * Called on behalf of fsync.
 *
 * LOCKS USED -- caller must NOT be holding the vnode's lock.
 */
int
vnvm_fsync(struct vnode *vp, osi_cred_t *credp)
{
    int code = 0;
    ulong seg = vp->v_gnode->gn_seg;
    if (seg) {
	code = vm_writep(seg, 0, MAXFSIZE/PAGESIZE - 1);
	afsl_MBZ(code);
	code = vms_iowait(seg);
    }
    return code;
}

/*
 * vnvm_schedule -- schedule all VM pages associated with a vnode
 *
 * PARAMETERS -- The flag tells whether we are called from sync (1) or from
 *     inactive (0).  Under AIX, scheduling is done only from sync so we ignore
 *     this flag.
 */
int vnvm_schedule(struct vnode *vp, osi_cred_t *credp, int syncflag)
{
    int code;
    ulong seg = vp->v_gnode->gn_seg;
    if (seg) {
	code = vm_writep(seg, 0, MAXFSIZE/PAGESIZE - 1);
	afsl_MBZ(code);
    }
    return 0;
}

/*
 * vnvm_delete -- invalidate/delete VM pages because a file is being deleted
 *
 * LOCKS USED -- called with only the idLock held.  This isn't the lock that
 *     protect the gn_seg but our caller guarantees that no other users can be
 *     using the segment or trying to reestablish it.
 */
int
vnvm_delete(struct vnode *vp, osi_cred_t *credp)
{
    int code;
    ulong seg = vp->v_gnode->gn_seg;
    if (seg) {
        code = vms_delete(seg);
	afsl_MBZ(code);
        vp->v_gnode->gn_seg = 0;
    }
    return 0;
}

/*
 * vnvm_recycle -- invalidate/delete VM pages because a vnode is being recycled
 *
 * LOCKS USED -- called with only the idLock held.  This isn't the lock that
 *     protect the gn_seg but our caller guarantees that no other users can be
 *     using the segment or trying to reestablish it.
 */
int
vnvm_recycle(struct vnode *vp, osi_cred_t *credp)
{
    int code;
    ulong seg = vp->v_gnode->gn_seg;
    if (seg) {
	code = vm_flushp(seg, 0, MAXFSIZE/PAGESIZE - 1);
	afsl_MBZ(code);
	code = vms_iowait(seg);
	afsl_MBZ(code);
	code = vms_delete(seg);
	afsl_MBZ(code);
	vp->v_gnode->gn_seg = 0;
    }
    return 0;
}

/*
 * vnvm_StopUse -- clean and invalidate VM pages because a vnode
 *		is being idled.
 *
 * Called on behalf of:  phantomize.
 */
int
vnvm_StopUse(struct vnode *vp, osi_cred_t *credp)
{
    int code = 0;
    ulong seg = vp->v_gnode->gn_seg;
    if (seg) {
	/* Start all pageouts going, but don't wait for them. */
	code = vm_flushp(seg, 0, MAXFSIZE/PAGESIZE - 1);
	afsl_MBZ(code);
	/* Wait for all pageouts, but don't wait for pageins. */
	code = vms_iowait(seg);
    }
    return code;
}

/*
 * vnvm_Clean -- clean and write protect VM pages because a vnode
 *     is being idled.
 *
 * Called on behalf of: phantomize.
 */
int
vnvm_Clean(struct vnode *vp, osi_cred_t *credp)
{
    /* 
     * Just use vnvm_StopUse, vm_protectp() over a whole segment
     * is very slow. It is better to just get rid of the pages.
     */
    return vnvm_StopUse(vp, credp);
}

/* Round up to nearest sector size */
#define BlockRoundup(b) roundup(b, UBSIZE)

/*
 * vnvm_PrepareUserBuffer -- prepare user buffer for I/O.
 *
 * RETURN VALUE --
 *	a buf whose address may be used to do the I/O; on many systems,
 *	this will be a kernel address that has been mapped to the user
 *	buffer.
 */
struct buf *
vnvm_PrepareUserBuffer(caddr_t uaddr, size_t length, int is_write)
{
    struct buf *bp = osi_GetBuf();
    long roundLength = BlockRoundup(length);
    int code;

    bp->b_flags |= (is_write) ? B_WRITE : B_READ;
    bp->b_un.b_addr = uaddr;
    bp->b_bcount = length;

    code = pinu(uaddr, roundLength, OSI_UIOUSER);
    afsl_MBZ (code);
    bp->b_xmemd.aspace_id = XMEM_INVAL;
    code = xmattach(uaddr, roundLength, &bp->b_xmemd, OSI_UIOUSER);
    afsl_MBZ (code);

    return (bp);
}

/*
 * vnvm_ReleaseUserBuffer -- release resources after I/O to user buffer.
 */
void
vnvm_ReleaseUserBuffer(struct buf *bp, caddr_t uaddr, size_t length)
{
    long roundLength = BlockRoundup(length);
    int code;

    code = unpinu(uaddr, roundLength, OSI_UIOUSER);
    afsl_MBZ(code);
    code = xmdetach(&bp->b_xmemd);
    afsl_MBZ(code);
    bp->b_flags |= B_DONE;
    osi_ReleaseBuf(bp);
}

/*
 * Because the AIX VM system provides no locking or synchronization
 * primitives, we must flush all of a file's active pages to disk before
 * doing file promotion or COW splitting.  This is very expensive, so
 * we only do it if we need to.
 */
int
efs_AvoidVMRaces(
  struct evnode *evp, 
  u_long len, 
  u_long blocksize, 
  int shrinking)
{
    off_t syncOff;
    long syncLen = 0;
    int mustSync = 0;
    daddr_t d;
    long w;

    if (shrinking) {
	/* 
	 * XXX If also promotion do not need to execute the following
	 * body of code since the code under the promotion test 
	 * will effectively always set syncOff to 0 and syncLen to
	 * len ? -- ra.
	 */
	long code = efs_FindWritableBlocks(evp, len, &d, &w);
	if (code)
	    return code;
	if (w == 0 && d != EFS_HOLE) {
	    /*
	     * we are are truncating into a COW block
	     */
	    mustSync = 1;
	    /* we need to sync the entire filesystem block */
	    syncOff = len & ~(blocksize - 1);
	    /* XXX Do not need MIN here ! */
	    syncLen = MIN(len - syncOff, blocksize);
	} else if (len & (PAGESIZE - 1)) {
	    /*
	     * The truncate is in the middle of a page. Flush
	     * this page making sure to store any modifications back.
	     */
	    mustSync = 1;
	    syncOff = len;   /* XXX len & ~PAGEMASK ? */
	    syncLen = PAGESIZE;
	}
    }
    if (efs_MustBePromoted(evp, len)) {
	mustSync = 1;
	syncOff = 0;
	syncLen = MAX(len - syncOff, syncLen); /* XXX Bogus len - syncOff */
	syncLen = MIN(syncLen, blocksize);
    }
    if (mustSync) {
	lock_ReleaseWrite_r(&evp->vd_file.lock);
	efs_StabilizeVM(evp, syncOff, syncLen);
	lock_ObtainWrite_r(&evp->vd_file.lock);
    }
    return 0;
}

/*
 * write protects and  flushes to disk all pages in the range defined by 
 * off and len. Since we can't hold the vmLock over flushp because it could 
 * cause a deadlock we have to use the pagefaults count to be able to 
 * tell when there really is no more dirty data in VM
 */
int efs_StabilizeVM(struct evnode *evp, off_t off, long len)
{
    int first = off >> PGSHIFT;
    int npages = NEXT_PAGE(len) >> PGSHIFT;
    long sid = EVTOV(evp)->v_gnode->gn_seg;
    int old, new;

    if (sid == 0) return 0;
    afsl_Assert(lock_WriteLocked(&(evp->vd_vm.lock)));
    do {
	old = evp->pagefaults;
	/* 
	 * make sure that any write to the area that we are about to
	 * copy will cause a pagefault
	 */
	vm_protectp(sid, first, npages, RDONLY);
	vm_flushp(sid, first, npages);
	lock_ReleaseWrite_r(&(evp->vd_vm.lock));
	(void)vms_iowait(sid);
	lock_ObtainWrite_r(&(evp->vd_vm.lock));
	new = evp->pagefaults;
    } while (new != old);
    return 0;
}
#endif /* KERNEL */
