/* Copyright (C) 1995, 1994 Transarc Corporation - All rights reserved. */
/*
 * For the code here to work correctly the following must be true:
 * 
 * Page-in must increment vp->pagefaults while holding vp->vmLock
 * Page-out cannot block and a vm_move that writes full pages does
 * not require a page-in
 */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/osi_buf.h>
#include <dcedfs/osi_vmm.h>
#include <dcedfs/debug.h>

#include "efs_evnode.h"
#include "efs_misc.h"
#include "efs_vnodeops.h"
#include "efs_tmpcopy.h"

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RIOS/RCS/efs_tmpcopy.c,v 1.37 1996/01/23 19:09:22 ota Exp $")

/*
 *	stores a file's user data into temporary storage. Since it pins
 *	memory to hold the data, it should be used with some caution:
 *	len shouldn't be much bigger than 1 filesystem block.  Also, we
 *	assume that the data is contiguous on disk.
 *
 *	input params :
 *		evp = a pointer to the file that we are reading from
 *		off = offset into the file (in bytes)
 *		len = how many bytes of contiguous data to read
 *		rblk = the disk block that marks the beginning of the
 *		       disk area containing  the data that we want to
 *		       read. We use this if the data is not already in
 *		       memory
 *	output params:
 *		uiopp = the struct uio that holds a pointer to the data
 *		memsize = how much we read
 *
 *	NOTE: THIS ROUTINE RETURNS HOLDING evp->vmLock !
 */
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
    osi_assert(*mem == NULL);
    tmp = osi_AllocPinned(NEXT_BLOCK(len, EVTOA(evp)));
    *mem = tmp;
    *memsize = NEXT_BLOCK(len, EVTOA(evp));
    /* 
     * at this point mem has the latest data and page-ins
     * are blocked
     */
    icl_Trace4(efs_iclSetalloc, EFS_TRACE_GETCONTENTS, ICL_TYPE_POINTER, (evp),
	       ICL_TYPE_LONG, (long) off,
	       ICL_TYPE_LONG, (long) len,
	       ICL_TYPE_LONG, (long) rblk);

    code = efs_GetContentsNoVM(evp, len, rblk, tmp);
    if (code) 
	return(code);
    /* simulate a pagezero() */
    if (*memsize > len)
	bzero(tmp+len, *memsize - len);
    return(0);
}

/*
 * We would like to do put the data from mem into VM pages and
 * mark them dirty, but since there is no safe way to do that
 * without dropping the vmLock we'll write synchronously to
 * disk instead.
 */
void
efs_PutContents(
  struct evnode *evp,
  daddr_t wblk,
  long len,
  int justUnlock,
  char *mem,
  int memsize )
{
    long code;

    icl_Trace0(efs_iclSetalloc, EFS_TRACE_PUTCONTENTS);    
    afsl_Assert(lock_Check(&evp->vd_vm.lock) != 0);
    osi_assert((mem != NULL) && (memsize >= len));
    if (!justUnlock) {
	code = efs_PutContentsNoVM(evp, len, wblk, mem);	
	afsl_MBZ (code);
    }
    osi_FreePinned(mem, memsize);
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
    osi_assert((mem != NULL) && (memlen >= length));
    icl_Trace0(efs_iclSetalloc, EFS_TRACE_PUTCONTENTS);    
    code = epix_InsertInline (trans, EVTOA(evp), 0, length, (char *) mem);
    afsl_MBZ (code);
    osi_FreePinned(mem, memlen);
}


/*
 * Create a zero filled dirty page.
 */
void
efs_CreateDirtyZeroPages(struct evnode *evp, long start, long finish)
{
    long alen = finish - start;
    vmid_t sid = (EVTOV(evp))->v_gnode->gn_seg;
    int code;

    osi_assert((start & (PAGESIZE - 1)) == 0);
    osi_assert(lock_Check(&evp->vd_vm.lock) != 0);
    osi_assert(sid != 0);

    if (alen <= 0)
	return;

    icl_Trace3(efs_iclSetalloc, EFS_TRACE_DIRTYZERO, ICL_TYPE_POINTER, (evp),
	       ICL_TYPE_LONG, (long) start,
	       ICL_TYPE_LONG, (long) finish); 

    evp->vd_vm.valid = 1;
    evp->vd_vm.dirty = 1;
    while (alen > 0) {
	code = vm_makep(sid, start >> PGSHIFT);
	afsl_MBZ(code);
	start += PAGESIZE;
	alen -= PAGESIZE;
    }
}

/* 
 * Zerofill past file EOF in the last allocated area on disk which could
 * be any of inline area or frag or block 
 */
void
efs_ZeroLastBlock(struct evnode *evp, long fileLen)
{
    epi_anode_t ap = EVTOA(evp);
    long fragSize, blkSize;
    long dblk, dcontig;
    long ioOff, ioLen, zeroOff, zeroLen;
    char *mem;
    int code;

    osi_assert(lock_WriteLocked(&evp->vd_vm.lock));
    osi_assert(lock_WriteLocked(&evp->vd_file.lock));
    osi_assert(fileLen > 0);
    
    if (EPIA_INLINE(ap)) {
	/* Inline files are handled by efs_promote() */
	return;
    } else if (EPIA_FRAGMENTED(ap)) {
	fragSize = ap->device->fragSize;
	if ((fileLen & (fragSize - 1)) == 0)
	    return;
	ioOff = fileLen & ~(fragSize - 1);
	ioLen = fragSize;
	zeroOff = fileLen & (fragSize - 1);
	zeroLen = fragSize - zeroOff;
    } else {
	blkSize = ap->device->blkSize;
	if ((fileLen & (blkSize - 1)) == 0) 
	    return;
	ioOff = fileLen & ~(blkSize - 1);
	ioLen = blkSize;
	zeroOff = fileLen & (blkSize - 1);
	zeroLen = blkSize - zeroOff;
    }

    code = efs_FindWritableBlocks(evp, ioOff, &dblk, &dcontig);
    /* 
     * XXX I would like to assert dcontig == ioLen but I cannot
     * because efs_FindWritableBlocks takes the file length into
     * account for inline files (though not for frag/blk files)
     */
    osi_assert(code == 0 && dcontig > 0);

    mem = osi_AllocPinned(ioLen);
    osi_assert(mem != NULL);

    /* Only read until EOF */
    code = efs_GetContentsNoVM(evp, ioLen, dblk, mem);
    MBZ(code);
    bzero(mem + zeroOff, zeroLen);
    code = efs_PutContentsNoVM(evp, ioLen, dblk, mem);
    MBZ(code);    
    
    osi_FreePinned(mem, ioLen);
}

/* 
 * Discard all pages past the new EOF. This routine is called
 * with the vnode's vd_lock, vmLock, and fileLock held and will 
 * need to drop and reobtain the fileLock. We can be sure
 * that all the pages are gone when we return since we don't drop
 * the vmLock
 */
efs_DiscardVM(struct evnode *evp, u_long oldLen, 
	      u_long newLen, osi_cred_t *credp)
{
    int firstpage;
    long code;
    vmid_t sid = EVTOV(evp)->v_gnode->gn_seg;

    if (!sid) return(0);
    afsl_AssertWriteLocked(&evp->vd_vm.lock);
    firstpage = (NEXT_PAGE(newLen)) >> PGSHIFT;
    lock_ReleaseWrite(&evp->vd_file.lock);
    code = vm_releasep(sid, firstpage, MAXFSIZE/PAGESIZE - (firstpage + 1));
    MBZ(code);
    code = vms_iowait(sid);
    lock_ObtainWrite(&evp->vd_file.lock);
    return(0);
}





