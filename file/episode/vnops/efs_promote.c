/*
 * Copyright (C) 1995, 1994 Transarc Corporation
 * All rights reserved.
 */
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_uio.h>
#include <dcedfs/osi_buf.h>
#include <dcedfs/osi_vmm.h>
#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/block_alloc.h>

#include "efs_evnode.h"
#include "efs_misc.h"
#include "efs_vnodeops.h"
#include "efs_tmpcopy.h"
#include "efs_newalloc.h"

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_promote.c,v 1.80 1995/03/10 15:38:35 ota Exp $")

#define DEBUG_THIS_FILE EFS_DEBUG_PROMOTE

/*
 * Convert storage class of vp if necessary so that it can grow to
 * desired_len.  In this function, we are only concerned with copying
 * whatever data the file already contains, e.g., from fragments to
 * blocks, and updating the anode allocation map.  We do not
 * invalidate any pages here or do any truncation. This function must
 * assumes that the new length has not been yet set if the file is growing
 *
 * LOCKS -- vp->fileLock should be write-locked on entry.
 */

/*
 * we'll use the epia_ws_method representation for the different storage
 * classes
 */

#define STORAGE_CLASS(ap, x) \
    ((x > 0) /* not empty */ + \
     (x > epix_GetMaxInlineData(ap)) /* not inline */ \
     + ((ap->device->blkSize == ap->device->fragSize) ? \
	(x > epix_GetMaxInlineData(ap)) : \
 	(x > (ap->device->blkSize - ap->device->fragSize)))) /* not fragged */

#define UPDATE_ANODE_LENGTH(trans, evp, ap)	\
MACRO_BEGIN \
    long macro__mbz; \
    macro__mbz = epix_SetLength(trans, ap, evp->vd_len, 0); \
    afsl_MBZ(macro__mbz); \
    BEGIN_LOCK_WRITE(&evp->vd_cache.lock) { \
	evp->vd_cache.newLen = 0; \
    } END_LOCK ; \
MACRO_END

/*
 * UsePartialBlocks tells us whether we should demote from blocked to
 * fragged in-place or by copying into an existing partially filled block.
 * The intent is that when an aggregate alread has many partially filled
 * blocks, we will copy (UsePartialBlocks returns 1), and otherwise we
 * will do it in place (UsePartialBlocks returns 0).
 */
#define UsePartialBlocks(device) 0 /* for now */

typedef int (*promo_func_t)(
  struct evnode *evp,
  int newLen,
  int useVM);

static int efs_NoPromotion(
  struct evnode *evp,
  int newLen,
  int useVM);
static int efs_EmptyToInline(
  struct evnode *evp,
  int newLen,
  int useVM);
static int efs_EmptyToFrag(
  struct evnode *evp,
  int newLen,
  int useVM);
static int efs_InlineToFrag(
  struct evnode *evp,
  int newLen,
  int useVM);
static int efs_InlineToBlock(
  struct evnode *evp,
  int newLen,
  int useVM);
static int efs_FragToEmpty(
  struct evnode *evp,
  int newLen,
  int useVM);
static int efs_FragToInline(
  struct evnode *evp,
  int newLen,
  int useVM);
static int efs_FragToFrag(
  struct evnode *evp,
  int newLen,
  int useVM);
static int efs_FragToBlock(
  struct evnode *evp,
  int newLen,
  int useVM);
static int efs_BlockToInline(
  struct evnode *evp,
  int newLen,
  int useVM);
static int efs_BlockToFrag(
  struct evnode *evp,
  int newLen,
  int useVM);

/*
 * No. of anode storage methods (empty, inline, frags, blocks)
 */
#define NWSMETHOD	4
static promo_func_t efs_promfunc[NWSMETHOD][NWSMETHOD] = {
  { efs_NoPromotion, efs_EmptyToInline, efs_EmptyToFrag, efs_NoPromotion },
  { efs_NoPromotion, efs_NoPromotion, efs_InlineToFrag, efs_InlineToBlock },
  { efs_FragToEmpty, efs_FragToInline, efs_FragToFrag, efs_FragToBlock },
  { efs_NoPromotion, efs_BlockToInline, efs_BlockToFrag, efs_NoPromotion }
};

int
efs_MustBePromoted(struct evnode *evp, long newLen)
{
    epi_anode_t ap = EVTOA(evp);
    enum epia_ws_method src, dest;
    int oldlen = epia_GetLen32(ap);

    if (oldlen == 0) {
	afsl_Assert(EPIA_BLOCKED(ap));
	src = WS_EMPTY;
    } else if (EPIA_INLINE(ap))
	src = WS_INLINE;
    else if (EPIA_FRAGMENTED(ap)){
	src = WS_FRAGMENTED;
	if (NEXT_FRAG(oldlen,ap) < NEXT_FRAG(newLen,ap))
	    return(1);
    } else
	src = WS_BLOCKED;

    dest = STORAGE_CLASS(ap, newLen);

    return (src != dest && dest != WS_EMPTY);
}

/*
 * This is called from vnm_Truncate to set the length to newLen and
 * change the shape of the anode to match the new length. If the file
 * shrank the length has already been set and the quotas have been updated
 * so we just have to change the shape, but if the file is growing the
 * anode at this point still has the old length and we need to do the
 * length setting as well as the quota update
 *
 * The \arg{useVM} tells us whether we are allowed to create VM pages
 * or not during this promotion. Typically \arg{useVM} will be FALSE
 * when efs_Promote is called from volume ops.
 */
int
efs_Promote(struct evnode *evp, long newLen, int useVM)
{
    elbb_tranRec_t trans;
    enum epia_ws_method src, dest;
    int code = 0;
    epi_anode_t ap = EVTOA(evp);

    afsl_Assert(lock_Check(&evp->vd_vm.lock) != 0);
    /*
     * we already have the vp->fileLock so get the length straight
     * from the anode
     */
    if (epia_GetLen32(ap) == 0) {
	afsl_Assert(EPIA_BLOCKED(ap));
	src = WS_EMPTY;
    } else if (EPIA_INLINE(ap))
	src = WS_INLINE;
    else if (EPIA_FRAGMENTED(ap))
	src = WS_FRAGMENTED;
    else
	src = WS_BLOCKED;

    dest = STORAGE_CLASS(ap, newLen);
    code = (*efs_promfunc[src][dest])(evp, newLen, useVM);
    return (code);
}


/* first the easy cases */
static int
efs_NoPromotion(
  struct evnode *evp, 
  int newLen, 
  int useVM /* Not used */)
{
    epi_anode_t ap = EVTOA(evp);

    BEGIN_LOCK_WRITE(&evp->vd_cache.lock) {

	/* set it in the vnode, mark it dirty only if the anode disagrees */

	(void) efs_setlength(EVTOV(evp), newLen, NULL);
	evp->vd_cache.newLen = (newLen != epia_GetLen32(ap));
    } END_LOCK;
    return (0);
}

static int
efs_EmptyToInline(
  struct evnode *evp, 
  int newLen, 
  int useVM /* Not used */)
{
    elbb_tranRec_t trans;
    /*
     * if the inline size ever changes this won't work too well, but it
     * allows us to avoid a malloc and bzero.  Note that we depend on
     * buff being initialized with zeroes.
     */
    static char buff[EPIA_INLINE_MAX_LEN];
    epi_anode_t ap = EVTOA(evp);
    long code;

    afsl_Assert(epix_GetMaxInlineData(ap) == sizeof (buff));

    /*
     * zerofill the inline area. no need to use VM
     */
    code = epia_StartTran("efs_Promote", ap, &trans);
    afsl_MBZ(code);
    code = epix_InsertInline(trans, ap, 0, newLen, buff);
    afsl_MBZ(code);
    (void) efs_setlength(EVTOV(evp), newLen, NULL); /* set it in the vnode */
    UPDATE_ANODE_LENGTH(trans,evp, ap);
    (void) epia_EndTran(ap, trans);
    return (0);
}

static int
efs_EmptyToFrag(
  struct evnode *evp, 
  int newLen,
  int useVM)
{
    long code = 0;
    epix_fragDescr_t frags;
    epia_quota_t delta;
    epi_anode_t ap = EVTOA(evp);
    async_device_t *aggr = ap->device;
    u_int nFrags = NEXT_FRAG(newLen, ap) >> aggr->logFragSize;
    elbb_tranRec_t trans;

    afsl_Assert(newLen <= aggr->blkSize - aggr->fragSize);
    code = epia_StartTran("efs_Promote", ap, &trans);
    afsl_MBZ(code);
    delta.a = delta.v = nFrags;
    if (code = epia_UseQuota(ap, trans, delta)) {
	(void) epia_EndTran(ap, trans);
	return (code);
    }
    if (code = epib_AllocateFrags(trans, ap, EPIB_MOVE_FRAGS, nFrags,  &frags))
	goto done;
    code = epix_InsertFragments(trans, ap, &frags);
    afsl_MBZ(code);

    afsl_Assert(frags.length == nFrags);
    (void) efs_setlength(EVTOV(evp), newLen, NULL); /* set it in the vnode */
    UPDATE_ANODE_LENGTH(trans,evp,ap);

    /*
     * we only need to zero data to the end of the fragment. 
     * If \arg{useVM} is true, we do so by creating dirty zerofilled pages.
     *
     * XXX If \arg{useVM} is false (implying volops during a restore), we do 
     * not zerofill the fragment, instead relying on the volops (actually, 
     * only vol_efsWrite) to any necessary zerofilling.
     */
    if (useVM) 
	efs_CreateDirtyZeroPages(evp, 0, NEXT_FRAG(newLen, ap)); 

  done:
    if (code) {
	int mbz;
	/* give the quota back */
	delta.a = -delta.a;
	delta.v = -delta.v;
	mbz = epia_UseQuota(ap, trans, delta);
	afsl_MBZ(mbz);
    }
    (void) epia_EndTran(ap, trans);
    return (code);
}

static int
efs_FragToEmpty(
  struct evnode *evp, 
  int newLen,
  int useVM)
{
    epix_fragDescr_t frags;
    epi_anode_t ap = EVTOA(evp);
    long code;
    epia_quota_t delta;
    elbb_tranRec_t trans;

    afsl_Assert(newLen == 0);

    code = epia_StartTran("efs_Promote", ap, &trans);
    afsl_MBZ(code);
    code = epix_GetFragInfo(ap, &frags);
    afsl_MBZ(code);

    /* update the quota */
    delta.v = -frags.length;
    delta.a = epix_IsBlockWritable(frags.block) ? delta.v : 0;
    code = epia_UseQuota(ap, trans, delta);
    afsl_MBZ(code);

    /* deallocate any allocated fragments */
    if (epix_IsBlockWritable(frags.block)) {
	code = epib_FreeFrags(trans, ap, 0, &frags);
	afsl_MBZ(code);
    }
    frags.length = 0;
    code = epix_InsertFragments(trans, ap, &frags);
    afsl_MBZ(code);

    (void) efs_setlength(EVTOV(evp), newLen, NULL); /* set it in the vnode */
    UPDATE_ANODE_LENGTH(trans,evp,ap);
    (void) epia_EndTran(ap, trans);
    return (0);
}

/*
 * Now the more complicated cases. The following routines need to
 * atomically change the allocation map and schedule the data that
 * was in the old location to be written to the new location. Also
 * when we are promoting a file up, the newly allocated block or
 * fragment needs to be zeroed from the old end of file on.
 *
 * They all take the following arguments :
 *	trans is the transaction that we have started in vnm_Truncate
 *		since to be doing promotion we must be changing the length
 *	evp is the vnode that we are promoting
 *	newLen is the new length
 */
static int
efs_FragToInline(
  struct evnode *evp, 
  int newLen,
  int useVM)
{
    epix_fragDescr_t frags;
    epi_anode_t ap = EVTOA(evp);
    long code;
    epia_quota_t delta;
    elbb_tranRec_t trans;
    TMPBUF_DECL

    /* XXX Ensure not called by volops */
    osi_assert(useVM == 1);

    code = epix_GetFragInfo(ap, &frags);
    afsl_MBZ(code);

    code = EFS_GETCONTENTS(evp, 0, newLen, EFS_FRAG_DADDR(ap->device, &frags));
    if (code != 0)
	return (code);

    code = epia_StartTran("efs_Promote", ap, &trans);
    afsl_MBZ(code);
    /* update the quota */
    delta.v = -frags.length;
    delta.a = epix_IsBlockWritable(frags.block) ? delta.v : 0;
    code = epia_UseQuota(ap, trans, delta);
    afsl_MBZ(code);

    /* deallocate any allocated fragments */
    if (epix_IsBlockWritable(frags.block)) {
	code = epib_FreeFrags(trans, ap, 0, &frags);
	afsl_MBZ(code);
    }
    frags.length = 0;
    code = epix_InsertFragments(trans, ap, &frags);
    afsl_MBZ(code);

    /* copy temporary buffer into inline space */
    EFS_PUTCONTENTS_TO_INLINE(trans, evp, newLen);

    /* set the vnode's length */
    (void) efs_setlength(EVTOV(evp), newLen, NULL); /* set it in the vnode */
    UPDATE_ANODE_LENGTH(trans,evp,ap);
    (void) epia_EndTran(ap, trans);
    return (0);
}

static int
efs_BlockToInline(
  struct evnode *evp, 
  int newLen,
  int useVM)
{
    long code;
    daddr_t dblk, fsb;
    epia_quota_t delta;
    epi_anode_t ap = EVTOA(evp);
    async_device_t *aggr = ap->device;
    u_int fragsPerBlock = aggr->blkSize >> aggr->logFragSize;
    epix_blockLoc_t loc;
    long alen;
    int isCow;
    elbb_tranRec_t trans;
    TMPBUF_DECL

    /* XXX Ensure not called by volops */
    osi_assert(useVM == 1);

    code = efs_CheckAllocation(evp, 0, &dblk, &alen, &isCow);
    afsl_MBZ(code);

    if (dblk == EFS_HOLE) 
	return efs_EmptyToInline(evp, newLen, useVM);


    code = epia_StartTran("efs_Promote", ap, &trans);
    afsl_MBZ(code);
    if (code = EFS_GETCONTENTS(evp, 0, newLen, dblk)) {
	(void) epia_EndTran(ap, trans);
	return (code);
    }

    afsl_Assert(isCow || epia_GetAllocated(ap) == fragsPerBlock);
    delta.v = -fragsPerBlock;
    delta.a = (isCow) ? 0 : delta.v;
    code = epia_UseQuota(ap, trans, delta);
    afsl_MBZ(code);

    fsb = dbtoab(aggr, dblk);
    EPIX_MakeDirectBlockLoc(&loc, 0);
    if (isCow) {
	fsb |= EPIX_ANODE_BLOCKBACKING;
    } else {
	code = epib_Free(trans, ap, 1, &fsb);
	afsl_MBZ(code);
    }
    code = epix_Insert(trans, ap, -1, &fsb, &loc); /* modifies fsb */
    afsl_MBZ(code);

    /* copy temporary buffer into inline space */
    EFS_PUTCONTENTS_TO_INLINE(trans, evp, newLen);

    (void) efs_setlength(EVTOV(evp), newLen, NULL); /* set it in the vnode */
    UPDATE_ANODE_LENGTH(trans,evp,ap);
    (void) epia_EndTran(ap, trans);
    return (0);
}

static int
efs_InlineToFrag(
  struct evnode *evp, 
  int newLen,
  int useVM)
{
    epix_fragDescr_t frags;
    long code;
    epia_quota_t delta;
    epi_anode_t ap = EVTOA(evp);
    async_device_t *adev = ap->device;
    u_int nFrags = NEXT_FRAG(newLen, ap) >> adev->logFragSize;
    long oldlen;
    elbb_tranRec_t trans;
    TMPBUF_DECL

    /* XXX Ensure not called by volops */
    osi_assert(useVM == 1);

    code = epia_StartTran("efs_Promote", ap, &trans);
    afsl_MBZ(code);
    delta.a = delta.v = nFrags;
    code = epia_UseQuota(ap, trans, delta);
    if (code != 0) {
	(void) epia_EndTran(ap, trans);
	return (code);
    }

    oldlen = epia_GetLen32(ap);
    code = EFS_GETCONTENTS(evp, 0, oldlen, EFS_HOLE);
    if (code != 0)
	goto done;

    code = epib_AllocateFrags(trans, ap, EPIB_MOVE_FRAGS, nFrags, &frags);
    if (code != 0) {
	EFS_PUTCONTENTS(evp, EFS_HOLE, 0, 1 /*justUnlock*/);
	goto done;
    }

    code = epix_InsertFragments(trans, ap, &frags);
    afsl_MBZ(code);

    EFS_PUTCONTENTS(evp, EFS_FRAG_DADDR(ap->device, &frags),
		    NEXT_FRAG(newLen, ap), 0);
    (void) efs_setlength(EVTOV(evp), newLen, NULL); /* set it in the vnode */
    UPDATE_ANODE_LENGTH(trans, evp,ap);
    /*
     * Zerofill; we assume that the in-line data fits in one page.
     */
    efs_CreateDirtyZeroPages(evp, PAGESIZE, NEXT_FRAG(newLen, ap));

  done:
    if (code != 0) {
	int mbz;
	/* give the quota back */
	delta.a = -delta.a;
	delta.v = -delta.v;
	mbz = epia_UseQuota(ap, trans, delta);
	afsl_MBZ(mbz);
    }
    (void) epia_EndTran(ap, trans);
    return (code);
}

static int
efs_InlineToBlock(
  struct evnode *evp, 
  int newLen,
  int useVM)
{
    epi_anode_t ap = EVTOA(evp);
    async_device_t *aggr = ap->device;
    epix_blockLoc_t loc;
    daddr_t fsb;
    long code;
    u_int fragsPerBlock = aggr->blkSize >> aggr->logFragSize;
    epia_quota_t delta;
    long oldlen;
    u_long nalloc = 1;
    elbb_tranRec_t trans;
    TMPBUF_DECL

    /* XXX Ensure not called by volops */
    osi_assert(useVM == 1);

    code = epia_StartTran("efs_Promote", ap, &trans);
    afsl_MBZ(code);
    /* do the quota update */
    delta.a = delta.v = fragsPerBlock;
    if (code = epia_UseQuota(ap, trans, delta)) {
	(void) epia_EndTran(ap, trans);
	return (code);
    }

    oldlen = epia_GetLen32(ap);
    if (code = EFS_GETCONTENTS(evp, 0, oldlen, 0))
	goto done;

    if (code = epib_Allocate(trans, ap, &nalloc, &fsb, 0)) {
	EFS_PUTCONTENTS(evp, EFS_HOLE, 0, 1 /*justUnlock*/);
	goto done;
    }
    afsl_Assert(nalloc == 1);

    EPIX_MakeDirectBlockLoc(&loc, 0);
    code = epix_Insert(trans, ap, 1, &fsb, &loc);
    afsl_MBZ(code);

    EFS_PUTCONTENTS(evp, abtodb(aggr, fsb), aggr->blkSize, 0);

    (void) efs_setlength(EVTOV(evp), newLen, NULL); /* set it in the vnode */
    UPDATE_ANODE_LENGTH(trans, evp, ap);
    /*
     * Zerofill; we assume that the in-line data fits in one page.
     */
    efs_CreateDirtyZeroPages(evp, PAGESIZE, aggr->blkSize);
  done:
    if (code) {
	/* give the quota back */
	int mbz;
	delta.a = -delta.a;
	delta.v = -delta.v;
	mbz = epia_UseQuota(ap, trans, delta);
	afsl_MBZ(mbz);
    }
    (void) epia_EndTran(ap, trans);
    return (code);
}

/*
 * Fragment-to-fragment "promotion".  We allocate new fragments here if
 * we are extending the file.  Since fragments are often smaller than
 * PAGESIZE, we may not get a chance to do the allocation in VOP_GETPAGE.
 */
static int
efs_FragToFrag(
  struct evnode *evp, 
  int newLen, 
  int useVM)
{
    epi_anode_t ap = EVTOA(evp);
    async_device_t *aggr = ap->device;
    int lgfsize = aggr->logFragSize;
    epix_fragDescr_t frags;
    epia_quota_t delta;
    int oldLen, notCow;
    long code = 0, tcode;
    elbb_tranRec_t trans;
#ifdef AFS_AIX_ENV
    daddr_t dblk;
    long off, len;
#endif

    TMPBUF_DECL

    /* XXX Ensure not called by volops */
    osi_assert(useVM == 1);

    code = epia_StartTran("efs_Promote", ap, &trans);
    afsl_MBZ(code);

    code = epix_GetFragInfo(ap, &frags);
    afsl_MBZ(code);

    notCow = epix_IsBlockWritable(frags.block);
    oldLen = EFS_FRAG_LENGTH(aggr, &frags);
    delta.v = (NEXT_FRAG(newLen, ap) - NEXT_FRAG(oldLen, ap)) >> lgfsize;
    delta.a = notCow ? delta.v : NEXT_FRAG(newLen, ap) >> lgfsize;

    code = epia_UseQuota(ap, trans, delta);
    if (code != 0) {
	(void) epia_EndTran(ap, trans);
	return (code);
    }

    if (newLen <= oldLen && notCow) {	/* shrinking in place */
	u_int nfrags = NEXT_FRAG(newLen, ap) >> lgfsize;
	EFS_HOLD_CONTENTS(evp, 0, newLen);
	code = epib_FreeFrags(trans, ap, nfrags, &frags);
	afsl_MBZ(code);
	code = epix_InsertFragments(trans, ap, &frags);
	afsl_MBZ(code);
	EFS_RELEASE_CONTENTS(evp);
    } else {
	code = efs_ExtendFragAllocation(evp, trans, newLen, &frags);
	if (code != 0) {
	    delta.a = -delta.a;
	    delta.v = -delta.v;
	    tcode = epia_UseQuota(ap, trans, delta);
	    MBZ(tcode);
	    (void) epia_EndTran(ap, trans);
	    return code;
	}
	afsl_Assert(frags.length == NEXT_FRAG(newLen, ap) >> lgfsize);
#ifdef AFS_AIX_ENV
	/* 
	 * Zero new allocated frags in the same VM page as current EOF
	 * on disk. We do not need to do this on Solaris as there
	 * we have done the appropriate zerofilling in the EOF page.
	 */
	
	dblk = EFS_FRAG_DADDR(ap->device, &frags);
	off = NEXT_FRAG(oldLen, ap);
	len = MIN(NEXT_FRAG(newLen, ap) - NEXT_FRAG(oldLen, ap),
		  NEXT_PAGE(oldLen));
	efs_ZeroBytes(evp, dblk, off, len);
#endif	
	if (newLen > NEXT_PAGE(oldLen))
	    efs_CreateDirtyZeroPages(evp, NEXT_PAGE(oldLen), newLen);
    }
    (void) efs_setlength(EVTOV(evp), newLen, NULL); /* set it in the vnode */
    UPDATE_ANODE_LENGTH(trans, evp, ap);
    (void) epia_EndTran(ap, trans);
    return (0);
}

static int
efs_FragToBlock(
  struct evnode *evp, 
  int newLen,
  int useVM)
{
    epi_anode_t ap = EVTOA(evp);
    async_device_t *aggr = ap->device;
    daddr_t fsb;
    long code = 0;
    u_long oldlen;
    epix_fragDescr_t frags;
    epix_blockLoc_t loc;
    u_int fragsPerBlock = aggr->blkSize >> aggr->logFragSize;
    epia_quota_t delta;
    elbb_tranRec_t trans;
    TMPBUF_DECL

    /* XXX Ensure not called by volops */
    osi_assert(useVM == 1);

    code = epia_StartTran("efs_Promote", ap, &trans);
    afsl_MBZ(code);
    oldlen = epia_GetLen32(ap);

    /* get the current frag allocation */
    code = epix_GetFragInfo(ap, &frags);
    afsl_MBZ(code);

    /* do the quota update */
    delta.v = fragsPerBlock - frags.length;
    delta.a = epix_IsBlockWritable(frags.block) ? delta.v : fragsPerBlock;
    if (code = epia_UseQuota(ap, trans, delta)) {
	(void) epia_EndTran(ap, trans);
	return (code);
    }

    /* can we just extend the current allocation ? */
    if (epix_IsBlockWritable(frags.block) && frags.first == 0)
	code = epib_AllocateFrags(trans, ap, EPIB_EXTEND_FRAGS, fragsPerBlock,
				  &frags);
    else
	code = EPI_E_CANNOTEXTEND;

    if (code == 0) {
	/*
	 * we managed to allocate the frags now all we need to
	 * do is change the allocation to be blocked
	 */
	fsb = frags.block;
	EPIX_MakeDirectBlockLoc(&loc, 0);
	EFS_HOLD_CONTENTS(evp, 0, oldlen);
	code = epix_Insert(trans, ap, 1, &frags.block, &loc);
	afsl_MBZ(code);
	EFS_RELEASE_CONTENTS(evp);
	/*
	 * now we are all done with the allocation but we still need
	 * to put some zeroes from the old end of the file to the end of
	 * the newly allocated block
	 */
	/*
	 * first set the new length so that we can create pages past the
	 * old EOF
	 */
	(void) efs_setlength(EVTOV(evp), newLen, NULL); /* set it in vnode */
	UPDATE_ANODE_LENGTH(trans, evp, ap);
	if (oldlen & (PAGESIZE - 1)) {
	    /*
	     * the old eof was not on a page boundary so we need to read
	     * the page that it was in and pad it out with 0's.
	     * efs_GetContents() will conveniently do this for us.
	     */
	    long pgoff = PAGE_START(oldlen);
	    daddr_t dblk = EFS_FRAG_DADDR(aggr, &frags) + btodb(pgoff);
	    code = EFS_GETCONTENTS(evp, pgoff, oldlen - pgoff, dblk);
	    if (code != 0)
		goto done;
	    EFS_PUTCONTENTS(evp, dblk, PAGESIZE, 0);
	}
    } else if (code != EPI_E_CANNOTEXTEND)
	goto done;
    else {
	/*
	 * if we got here we may be able to do the extra allocation but
	 * not by extending the frags so we have to copy the data in
	 * a temp buffer, free the frags, allocate the block, and copy
	 * the data back out
	 */
	u_long nalloc = 1;
	code = EFS_GETCONTENTS(evp, 0, oldlen, EFS_FRAG_DADDR(aggr, &frags));
	if (code != 0)
	    goto done;

	if (code = epib_Allocate(trans, ap, &nalloc, &fsb, 0)) {
	    EFS_PUTCONTENTS(evp, EFS_HOLE, 0, 1 /*justUnlock*/);
	    goto done;
	}
	afsl_Assert(nalloc == 1);

	/* deallocate any allocated fragments */
	if (epix_IsBlockWritable(frags.block)) {
	    code = epib_FreeFrags(trans, ap, 0, &frags);
	    afsl_MBZ(code);
	}

	/* insert the new block in the anode's block map */
	EPIX_MakeDirectBlockLoc(&loc, 0);
	code = epix_Insert(trans, ap, 1, &fsb, &loc);
	afsl_MBZ(code);

	EFS_PUTCONTENTS(evp, abtodb(aggr, fsb), aggr->blkSize, 0);

	/*
	 * first set the new length so that we can create pages past the
	 * old EOF
	 */
	(void) efs_setlength(EVTOV(evp), newLen, NULL); /* set it in vnode */
	UPDATE_ANODE_LENGTH(trans, evp, ap);
    }

    /*
     * if the old end of file was more than a page away from the end of the
     * block, we need to zerofill.
     */
    if (aggr->blkSize - oldlen  >= PAGESIZE)
	efs_CreateDirtyZeroPages(evp, NEXT_PAGE(oldlen), aggr->blkSize);

  done:
    if (code) {
	int mbz;
	/* give the quota back */
	delta.a = -delta.a;
	delta.v = -delta.v;
	mbz = epia_UseQuota(EVTOA(evp), trans, delta);
	afsl_MBZ(mbz);
    }
    (void) epia_EndTran(ap, trans);
    return (code);
}

static int
efs_BlockToFrag(
  struct evnode *evp, 
  int newLen, 
  int useVM)
{
    epix_fragDescr_t frags;
    epi_anode_t ap = EVTOA(evp);
    async_device_t *aggr = ap->device;
    int nFrags = NEXT_FRAG(newLen, ap) >> aggr->logFragSize;
    int fragsPerBlock = aggr->blkSize >> aggr->logFragSize;
    daddr_t fsb, dblk;
    long code;
    int oldlen;
    long alen;
    u_long endOfLastFrag;
    epia_quota_t delta;
    epix_blockLoc_t loc;
    int isCow;
    elbb_tranRec_t trans;
    TMPBUF_DECL

    /* XXX Ensure not called by volops */
    osi_assert(useVM == 1);

    code = efs_CheckAllocation(evp, 0, &dblk, &alen, &isCow);
    afsl_MBZ(code);

    if (dblk == EFS_HOLE)
	return efs_NoPromotion(evp, newLen, useVM);

    code = epia_StartTran("efs_Promote", ap, &trans);
    afsl_MBZ(code);

    afsl_Assert(isCow || epia_GetAllocated(ap) == fragsPerBlock);
    delta.v = nFrags - fragsPerBlock;
    delta.a = (isCow) ? nFrags : delta.v;
    afsl_Assert(delta.v < 0);

    /*
     * We can choose here to simply demote the already allocated block
     * or to allocate a new set of fragments and free the block.
     */
    fsb = dbtoab(aggr, dblk);
    if (isCow)
	fsb |= EPIX_ANODE_BLOCKBACKING;
    EPIX_MakeDirectBlockLoc(&loc, 0);
    oldlen = epia_GetLen32(ap);
    if (isCow || UsePartialBlocks(aggr)) {
	if (code = EFS_GETCONTENTS(evp, 0, MIN(oldlen, newLen), dblk)) {
	    (void) epia_EndTran(ap, trans);
	    return (code);
	}
	/*
	 * allocate as many frags as we need
	 */
	code = epib_AllocateFrags(trans, ap, EPIB_EXTEND_FRAGS, nFrags,
				  &frags);
	if (code) {
	    EFS_PUTCONTENTS(evp, EFS_HOLE, 0, 1 /*justUnlock*/); 
	    (void) epia_EndTran(ap, trans);
	    return (code);
	}

	if (!isCow) {
	    code = epib_Free(trans, ap, 1, &fsb);
	    afsl_MBZ(code);
	}
	code = epix_Insert(trans, ap, -1, &fsb, &loc); /* modifies fsb */
	afsl_MBZ(code);

	code = epix_InsertFragments(trans, ap, &frags);
	afsl_MBZ(code);
	EFS_PUTCONTENTS(evp, EFS_FRAG_DADDR(ap->device, &frags),
			NEXT_FRAG(newLen, ap), 0);
    } else {
	EPIX_MakeDirectBlockLoc(&loc, 0);
	EFS_HOLD_CONTENTS(evp, 0, oldlen);
	/* cons up frags description */
	frags.block = fsb;
	frags.first = 0;
	frags.length = fragsPerBlock;
	code = epib_FreeFrags(trans, ap, nFrags /*keep*/, &frags);
	afsl_MBZ(code);
	code = epix_Insert(trans, ap, -1, &fsb, &loc); /* modifies fsb */
	afsl_MBZ(code);
	code = epix_InsertFragments(trans, ap, &frags);
	afsl_MBZ(code);
	EFS_RELEASE_CONTENTS(evp);
    }

    afsl_Assert(frags.length == nFrags);
    (void) efs_setlength(EVTOV(evp), newLen, NULL); /* set it in the vnode */
    UPDATE_ANODE_LENGTH(trans, evp, ap);
    /*
     * now we might have to zero-fill the last of the file to the
     * end of the last frag
     */
    /*
     * first case to consider is if the newLen is more than a page away
     * from the last allocated frag.
     */
    endOfLastFrag = frags.length * aggr->fragSize;
    if (endOfLastFrag - newLen >= PAGESIZE)
	efs_CreateDirtyZeroPages(evp, newLen, endOfLastFrag);

    if (oldlen < newLen) {
	off_t oldLenPageEnd, newLenPageEnd;

	/*
	 * a second case where we need to zerofill is if oldlen is smaller
	 * than and in a different page from newLen.
	 */
	oldLenPageEnd = NEXT_PAGE(oldlen);
	newLenPageEnd = NEXT_PAGE(newLen);
	if (oldLenPageEnd != newLenPageEnd)
	    efs_CreateDirtyZeroPages(evp, oldLenPageEnd, newLenPageEnd);

    } /* else the page that newLen is in has been handled earlier */
   code = epia_UseQuota(EVTOA(evp), trans, delta);
   afsl_MBZ(code);
    (void) epia_EndTran(ap, trans);
   return (0);
}
