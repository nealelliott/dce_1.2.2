/*
 *      Copyright (C) 1996, 1992 Transarc Corporation
 *      All rights reserved.
 */
/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/tkm/RCS/tkm_filetoken.h,v 1.3 1996/03/04 19:12:59 ota Exp $ */

#ifndef TRANSARC_TKM_FILETOKEN_H
#define TRANSARC_TKM_FILETOKEN_H
#include "tkm_internal.h"

extern int tkm_GetFileToken _TAKES((tkm_internalToken_t *newTokenP,
				    long flags,
				    afs_recordLock_t *lockDescriptorP));
extern int tkm_FixFileConflicts _TAKES((tkm_internalToken_t *tokenP,
					tkm_file_t *fileP,
					long flags,
					long *otherHostMaskP,
					afs_recordLock_t *lockDescriptorP));
extern void tkm_ReturnFileToken _TAKES((tkm_internalToken_t *tokenP));

#endif /*TRANSARC_TKM_FILETOKEN_H*/






