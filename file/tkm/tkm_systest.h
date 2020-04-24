/*
 * Copyright (c) 1996, 1994, Transarc Corp.
 */
/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/tkm/RCS/tkm_systest.h,v 1.3 1996/03/04 19:13:16 ota Exp $ */

#ifndef TKM_SYSTEST_H
#define TKM_SYSTEST_H
#include <dcedfs/tpq.h>
#include <dcedfs/osi.h>
#include "tkm_internal.h"
#include "tkm_ctable.h"
#include "tkm_range.h"
#include "tkm_conflicts.h"
#include "tkm_tokens.h"
#include "tkm_misc.h"

struct systest_token {
    struct systest_token *next;
    int hostId;
    afs_token_t *info;
    long rangedTypes;
    long noRangeTypes;
    tkm_byterange_t range; /* 
			    * the more convenient form of the range stored in
			    * the afs_token_t
			    */
    long dont_revoke; /* what revocations to refuse */
    long dont_accept; /* what slice & dice to refuse */
			    
};

struct systest_file {
    struct systest_file *next;
    afsFid *fid;
    struct systest_token *granted;
};

struct pardoArg {
    afsFid *fidP;
    int hostId;
    int reqNum; 
    long flags;
    long types;
    afs_hyper_t *beginRange;
    afs_hyper_t *endRange;
    afs_recordLock_t *lockDescriptor;
};

extern void * systest_threadPoolHandle;

extern long GrantToken(), RevokeToken(), 
    LockHost(), UnLockHost(), HoldHost(), ReleHost(), FailRevoke(), FailGrant();

extern struct hostops systest_hostops;

extern struct systest_file *GetRandomFile();
extern struct systest_file *FindFile();

#endif /* TKM_SYSTEST_H */
