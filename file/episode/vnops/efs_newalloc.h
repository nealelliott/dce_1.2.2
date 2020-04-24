/*
 * Copyright (C) 1995, 1994 Transarc Corporation
 * All rights reserved.
 */
/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_newalloc.h,v 1.15 1995/02/15 20:33:02 ota Exp $ */

#ifndef TRANSARC_EFS_NEWALLOC_H
#define TRANSARC_EFS_NEWALLOC_H

#include <dcedfs/sysincludes.h>
#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/logbuf.h>
#include "efs_evnode.h"

extern long efs_CheckAllocation(
  efs_vnode_t evp,
  long off,
  daddr_t *dblkP,
  long *alenP,
  int *isCowP);

extern long efs_MakeWritableBlocks(
  struct evnode *evp,
  long off,
  long len,
  long flags);

/*
 * Flags for efs_MakeWritableBlocks
 */
#define MWB_USE_VM 	0x1	/* Use virtual memory */
#define	MWB_ZEROFILL	0x2	/* Arrange to zerofill any allocated blocks */
#define MWB_OVERWRITE	0x4	/* COW block to be overwritten; don't read */

extern long efs_FindBlocks(
  struct evnode *evp,
  long off,
  daddr_t *blockP,
  long *wrLenP,
  long *rdLenP);

extern long efs_FindWritableBlocks(
  struct evnode *evp,
  long off,
  daddr_t *blockP,
  long *lenP);

extern long efs_NextBlock(
  struct evnode *evp,
  long *offP,
  long len);

extern long efs_NextWritableBlock(
  struct evnode *evp,
  long *offP,
  long len);

extern long efs_ExtendFragAllocation(
  struct evnode *evp,
  elbb_tranRec_t trans,
  long len,
  epix_fragDescr_t *frags);

extern void efs_ZeroBytes(
  struct evnode *evp,
  daddr_t dblk,
  long start,
  long finish);

extern long efs_GetContentsNoVM(
  struct evnode *evp,
  long len,	
  daddr_t rblk, 
  char *tmp);

extern long efs_PutContentsNoVM(
  struct evnode *evp,
  long len,	
  daddr_t wblk, 
  char *tmp);

extern long efs_MakeABlock(
  struct evnode *evp,
  long off,
  daddr_t *dblk);

long efs_SplitCOWFrags(
  struct evnode *evp,
  daddr_t *dblkP);

#endif /* TRANSARC_EFS_NEWALLOC_H */
