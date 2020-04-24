/*
 * Copyright (C) 1994 Transarc Corporation
 * All rights reserved.
 */
/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_promote.h,v 1.6 1994/08/08 20:28:07 rajesh Exp $ */

#ifndef TRANSARC_EFS_PROMOTE_H
#define TRANSARC_EFS_PROMOTE_H

#include "efs_evnode.h"

extern int efs_MustBePromoted(struct evnode *evp, long newLen);
extern int efs_Promote(struct evnode *evp, long newLen, int useVM);

#endif /* TRANSARC_EFS_PROMOTE_H */
