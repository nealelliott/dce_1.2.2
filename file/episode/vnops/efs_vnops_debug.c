/*
 * Copyright (C) 1995, 1994 Transarc Corporation.
 * All rights reserved 
 */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include "efs_vnops_debug.h"
#include "efs_evnode.h"

#ifdef AFS_SUNOS5_ENV
#include <sys/file.h>
#endif

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_vnops_debug.c,v 1.8 1996/05/02 21:22:01 rajesh Exp $")

extern void vnm_SyncVnodes();

/*
 * efs_vnops_debug
 *	Debugging/testing hooks into the Episode vnode package.
 */
/* EXPORT */
long efs_vnops_debug(long op, long parm1, long parm2)
{
    long code = 0;

    switch (op) {
      case EFS_DEBUG_VNOPS_SYNC_VNODES:
	/* parm1 is the metaDataOnly flag */
	/* parm2 is not used */
	vnm_SyncVnodes((parm1 & EFS_DEBUG_VNOPS_SYNC_VNODES_METAONLY) ? 1 : 0);
	code = 0;
	break;
      case EFS_DEBUG_VNOPS_GET_ALLOCINFO:
	code = efs_DebugAllocInfo((int)parm1, (struct efs_allocInfo *)parm2);
	break;
      default: 
	code = EINVAL;
    }
    return code;
}


int efs_DebugAllocInfo(int fdes, struct efs_allocInfo *uAllocInfoP)
{
#ifndef AFS_SUNOS5_ENV
    return ENOSYS;
#else 
    return efs_sun5_DebugAllocInfo(fdes, uAllocInfoP);
#endif
}

