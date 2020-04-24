/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
** (c) Copyright 1992,1991,1990 OPEN SOFTWARE FOUNDATION, INC.
** ALL RIGHTS RESERVED
*/

/* Copyright (C) 1995, 1990 Transarc Corporation - All rights reserved. */

#ifdef AFS_AIX31_VM

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

#include "efs_evnode.h"

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RIOS/RCS/efs_aixmap.c,v 1.13 1995/09/26 15:33:09 blake Exp $")

int efs_map(vp, addr, length, offset, flags)
    struct vnode *vp;
    caddr_t addr;
    u_int   length, offset, flags;
{
    int code;
    struct evnode *evp = VTOEV(vp);

    /* Check for stale file */
    code = EV_CHECKSTALE(evp);
    if (code != 0)
	return code;

    tlock_ObtainWriteNT(&evp->vd_tlock);
    code = efs_getvmh(vp);
    if (code == 0) {
	if (flags & SHM_RDONLY) {
	    vp->v_gnode->gn_mrdcnt++;
	} else {
	    vp->v_gnode->gn_mwrcnt++;
	}
    }
    tlock_ReleaseWriteNT(&evp->vd_tlock);

    if (code == 0)
	FAST_VN_HOLD(VTOEV(vp));
    return code;
}

int efs_unmap(vp, flag)
    struct vnode *vp;
    int flag;
{
    struct evnode *evp = VTOEV(vp);

    tlock_ObtainWriteNT(&evp->vd_tlock);
    if (flag & SHM_RDONLY) {
	vp->v_gnode->gn_mrdcnt--;
	if (vp->v_gnode->gn_mrdcnt <=0) vp->v_gnode->gn_mrdcnt = 0;
    } else {
	vp->v_gnode->gn_mwrcnt--;
	if (vp->v_gnode->gn_mwrcnt <=0) vp->v_gnode->gn_mwrcnt = 0;
    }
    tlock_ReleaseWriteNT(&evp->vd_tlock);

    VN_RELE(vp);
    return(0);
}

#endif /* AFS_AIX31_VM */
