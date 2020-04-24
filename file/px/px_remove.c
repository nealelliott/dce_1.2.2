/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 *      Copyright (C) 1995, 1990 Transarc Corporation
 *      All rights reserved.
 */
#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/fshs.h>
#include <dcedfs/aggr.h>
#include <dcedfs/volume.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/tkset.h>
#include <dcedfs/flserver.h>
#include <dcedfs/rep_data.h>
#include <dcedfs/zlc.h>
#include <px.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/px/RCS/px_remove.c,v 4.34 1995/10/24 14:31:30 moyer Exp $")

/*
 * All file removal handling has been moved to a separate ZLC (zero linkcount)
 * module which holds the vnode until conditions are such that an OFD token
 * can be requested and subsequently granted, at which time the ZLC module
 * performs the necessary token cache flushes and releases the vnode.
 */

int px_TryRemove(avp, afidp, volp)
    register struct vnode *avp;
    afsFid *afidp;
    struct volume *volp;  /* currently unused */
{
    zlc_TryRemove(avp, afidp);
    return 0;
}


#ifdef notdef
/* no longer consult the KNOWDALLY flag, look at the IS_REPLICATED flag, instead */
PRIVATE void px_CheckForDally(volp)
struct volume *volp;
{
    struct vldbentry *vlentp;
    long code;

    /* Issues an rpc call to get the desired volume entry */
    vlentp = (struct vldbentry *) osi_AllocBufferSpace();
    code = px_FLGetEntryByID(&volp->v_volId, VOLTIX_TO_VOLTYPE(RWVOL), vlentp);
    if (code == 0) {
	lock_ObtainWrite(&volp->v_lock);
	if ((vlentp->flags & (VLF_LAZYREP|VLF_LAZYRELE)) != 0) {
	    volp->v_reclaimDally = vlentp->reclaimDally;
	} else {
	    volp->v_reclaimDally = 0;
	}
	volp->v_states |= VOL_KNOWDALLY;
	lock_ReleaseWrite(&volp->v_lock);
    }
    osi_FreeBufferSpace((struct osi_buffer *)vlentp);
}
#endif /* notdef */
