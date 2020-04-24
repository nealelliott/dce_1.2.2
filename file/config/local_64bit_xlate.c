/* Copyright (C) 1996 Transarc Corporation - All rights reserved. */
/*
 * @DEC_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: local_64bit_xlate.c,v $
 * $EndLog$
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/common_data.h>
#include <dcedfs/osi.h>

#if defined(AFS_AIX31_ENV) && defined(KERNEL)
/* need to map malloc to xmalloc */
#include <sys/malloc.h>
#endif

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/config/RCS/local_64bit_xlate.c,v 1.3 1996/10/10 17:54:20 ota Exp $")

/***********************************************
 ** Translation routines for afsHyper / hyper **
 ***********************************************/

/* Allocate memory using the native memory manager since we are called from the
 * RPC system, which doesn't depend on DFS services.  In particular, on
 * AIX3.2.5 we are called with preemption enabled. */

void afsHyper_from_local(local_object, net_object)
  afs_hyper_t *local_object;
  afsHyper **net_object;
{
    *net_object = osi_kalloc_r(sizeof(afsHyper));

    (*net_object)->low = AFS_hgetlo(*local_object);
    (*net_object)->high = AFS_hgethi(*local_object);
}

void afsHyper_to_local(net_object, local_object)
  afsHyper *net_object;
  afs_hyper_t *local_object;
{
    AFS_hset64(*local_object, net_object->high, net_object->low);
}

void afsHyper_free_local(local_object)
  afs_hyper_t *local_object;
{
    /* Not Used? */
}

void afsHyper_free_inst(net_object)
  afsHyper *net_object;
{
    osi_kfree_r(net_object, sizeof(afsHyper));
}



/*************************************************
 ** Translation routines for afsToken / token_t **
 *************************************************/

void afsToken_from_local(local_object, net_object)
  afs_token_t *local_object;
  afsToken **net_object;
{
    *net_object = osi_kalloc_r(sizeof(afsToken));

    (*net_object)->tokenID = local_object->tokenID;
    (*net_object)->expirationTime = local_object->expirationTime;
    (*net_object)->type = local_object->type;
    (*net_object)->beginRange = AFS_hgetlo(local_object->beginRange);
    (*net_object)->beginRangeExt = AFS_hgethi(local_object->beginRange);
    (*net_object)->endRange = AFS_hgetlo(local_object->endRange);
    (*net_object)->endRangeExt = AFS_hgethi(local_object->endRange);
}

void afsToken_to_local(net_object, local_object)
  afsToken *net_object;
  afs_token_t *local_object;
{
    local_object->tokenID = net_object->tokenID;
    local_object->expirationTime = net_object->expirationTime;
    local_object->type = net_object->type;
    AFS_hset64(local_object->beginRange,
	       net_object->beginRangeExt, net_object->beginRange);
    AFS_hset64(local_object->endRange,
	       net_object->endRangeExt, net_object->endRange);
}

void afsToken_free_local(local_object)
  afs_token_t *local_object;
{
    /* Not Used? */
}

void afsToken_free_inst(net_object)
  afsToken *net_object;
{
    osi_kfree_r(net_object, sizeof(afsToken));
}


/*********************************************************** 
 ** Translation routines for afsRecordLock / recordLock_t **
 ***********************************************************/

void afsRecordLock_from_local(local_object, net_object)
  afs_recordLock_t *local_object;
  afsRecordLock **net_object;
{
    *net_object = osi_kalloc_r(sizeof(afsRecordLock));

    (*net_object)->l_type = local_object->l_type;
    (*net_object)->l_whence = local_object->l_whence;
    (*net_object)->l_start_pos = AFS_hgetlo(local_object->l_start_pos);
    (*net_object)->l_start_pos_ext = AFS_hgethi(local_object->l_start_pos);
    (*net_object)->l_end_pos = AFS_hgetlo(local_object->l_end_pos);
    (*net_object)->l_end_pos_ext = AFS_hgethi(local_object->l_end_pos);
    (*net_object)->l_pid = local_object->l_pid;
    (*net_object)->l_sysid = local_object->l_sysid;
    (*net_object)->l_fstype = local_object->l_fstype;
}

void afsRecordLock_to_local(net_object, local_object)
  afsRecordLock *net_object;
  afs_recordLock_t *local_object;
{
    local_object->l_type = net_object->l_type;
    local_object->l_whence = net_object->l_whence;
    AFS_hset64(local_object->l_start_pos,
	       net_object->l_start_pos_ext, net_object->l_start_pos);
    AFS_hset64(local_object->l_end_pos,
	       net_object->l_end_pos_ext, net_object->l_end_pos);
    local_object->l_pid = net_object->l_pid;
    local_object->l_sysid = net_object->l_sysid;
    local_object->l_fstype = net_object->l_fstype;
}

void afsRecordLock_free_local(local_object)
  afs_recordLock_t *local_object;
{
    /* Not Used? */
}

void afsRecordLock_free_inst(net_object)
  afsRecordLock *net_object;
{
    osi_kfree_r(net_object, sizeof(afsRecordLock));
}
