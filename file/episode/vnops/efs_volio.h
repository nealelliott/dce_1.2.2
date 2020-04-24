/* Copyright (C) 1994 Transarc Corporation - All rights reserved. */
/*
 * $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_volio.h,v 1.2 1994/08/12 15:49:49 blake Exp $
 */

/*
 * Interfaces for volume op disk I/O.
 */
#ifndef TRANSARC_EFS_VOLIO_H
#define TRANSARC_EFS_VOLIO_H

#include <dcedfs/sysincludes.h>
#include "efs_evnode.h"

#define	VOL_EFS_BUFSIZE	8192
#define VOL_EFS_NBUFS	32

extern void vol_efsInitIO(u_int nbuf);

extern struct buf *vol_efsGetBuf(
  struct evnode *evp,
  int flags,
  size_t len,
  daddr_t dblk);

extern void vol_efsReleaseBuf(struct buf *bp);

extern int vol_efsStartIO(struct buf *bp);

extern int vol_efsBioWait(struct buf *bp);

extern void vol_efsWaitAll(void);

#endif /* !TRANSARC_EFS_VOLIO_H */
