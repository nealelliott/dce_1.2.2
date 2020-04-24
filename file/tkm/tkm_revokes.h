/* Copyright (C) 1996, 1994 Transarc Corporation - All rights reserved. */
/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/tkm/RCS/tkm_revokes.h,v 1.4 1996/04/26 16:16:43 cfe Exp $ */

#ifndef TRANSARC_TKM_REVOKES
#define TRANSARC_TKM_REVOKES
#include "tkm_conflicts.h"

extern long tkm_ParallelRevoke _TAKES((struct tkm_tokenConflictQ *conflicts,
				       int flags,
				       afs_recordLock_t *lockDescriptorP));
#define TKM_PARA_REVOKE_FLAG_DOINGGC	0x1
#define TKM_PARA_REVOKE_FLAG_FORVOLTOKEN	0x2

#endif /* TRANSARC_TKM_REVOKES */
