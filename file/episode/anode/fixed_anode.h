/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

/*
 * This file contains structure declarations defining the structures used by
 * the fixed anode module (epix) which are shared within episode.  This
 * includes on-disk volume table format and details of an anode's block map.
 */
/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/fixed_anode.h,v 4.70 1996/03/04 19:00:39 ota Exp $ */

#ifndef TRANSARC_EPI_FIXED_ANODE_H
#define TRANSARC_EPI_FIXED_ANODE_H

#include <dcedfs/hyper.h>
#include <dcedfs/episode/critical.h>
#include <dcedfs/episode/async.h>
#include "anode.h"

/* This is no longer used. */
struct epix_diskVolData {
    int32 spare[2];			/* obs: fileset version number */
};

/* For a detailed description of this structure see the comment preceding
 * FindBlocks in fixed_anode.c */

/* tree == EPIX_DUMMY_BLOCK_LOCATION indicates a structure that was dummied
 * up and one that should not be passed to epix_Insert */
#define EPIX_DUMMY_BLOCK_LOCATION (-10)

#define DBprint(p) dmprintf (afsdb_episode_anode, DEBUG_THIS_FILE, p)
#define SleepPrint(p) \
    dprintf (afsdb_episode_anode & \
	     (((u_int)1 << (DEBUG_THIS_FILE -1 )) | \
		((u_int)1 << (EPIA_DEBUG_SLEEP - 1))), p)

/*
 * DFS_NOCODECOVERAGE compile time flag turns of the code coverage
 * counting mechanism in Episode. Refer icl/icl_macros.h.
 */

/* stuff for monitoring code coverage */
#if defined(DFS_NOCODECOVERAGE) || !defined(AFS_DEBUG)

#define epix_EnterCodeBlock(i)
#define epix_ConditionalCodeBlock(c, i)

#elif defined(AFS_DEBUG)

#define epix_codeBlock epia_codeBlock	/* temporary during conversion */
extern void epix_BumpCodeBlock (
  int fileNumber,
  int blockNumber);
#define epix_EnterCodeBlock(i) \
    ((afsdb_episode_anode & (1u << (EPIA_DEBUG_CODEBLOCKS - 1))) \
     ? (epix_BumpCodeBlock(DEBUG_THIS_FILE, i), 0) : 0)
#define epix_ConditionalCodeBlock(c, i) ((c) ? (epix_EnterCodeBlock(i), 0) : 0)

#endif

/* Get ICL tracing stuff. */

#include <dcedfs/icl.h>			/* icl_CreateLog */
#include "epi_trace.h"

extern struct icl_set *epi_iclSet;

enum epix_GCRockType {
    epix_gc_handle,			/* rock is anode handle */
    epix_gc_device,			/* matching specified device */
    epix_gc_volumeRock,			/* match rock against volumeRock */
    epix_gc_fileRock			/* match rock against fileRock */
};

/* Functions shared with other episode modules, but not exported */

extern long epix_Init(u_long maxHandles);

extern long SetGConClose(enum epix_GCRockType type, opaque rock);

extern void epix_FindAppropriateTree(
  baddr_t block,
  u_int ibsize,
  struct epix_blockLocation *loc);

extern long epix_GetType(epi_anode_t handle);

extern long epix_GetBuffer(
  epi_anode_t handle,
  baddr_t block,
  struct elbb_buffer **bP);

extern int epix_BlocksNeeded(epi_anode_t ap, int nblocks);

extern long epix_CreateAVL(
  async_device_t *device,
  daddr_t avlBlock,
  u_long avlIndex,
  epi_anode_t *avlP);

extern long epix_InitAggregate(
  async_device_t *device,
  long   flags,
  daddr_t avlBlock,
  u_long avlIndex,
  epi_anode_t *avlHandleP);

extern long epix_SetGConClose(enum epix_GCRockType type, opaque rock);

extern long epix_CountOpenAnodes(async_device_t *device, long *countP);

extern long epix_ZeroInline(
  elbb_tranRec_t trans,
  epi_anode_t h,
  u_long offset,
  u_long length);

extern long epix_MoveData(
  elbb_tranRec_t trans,
  epi_anode_t h,			/* copy on write container */
  epi_anode_t b,			/* backing container */
  long flags,
  u_int nBlocks,			/* size of output block array */
  daddr_t pBlock[],			/* physical block addresses */
  u_int *nFoundP,			/* number addresses returned */
  struct epix_fragDescription *frags, /* fragments to be freed */
  epia_quota_t *deltaCP,		/* needed quota adjustments */
  epia_quota_t *deltaBP);

extern long epix_GetBackingAnode(epi_anode_t h, epi_anode_t *backingP);

#ifndef KERNEL
#include "verify.h"
#include "salvage.h"

extern long epia_VerifyAnode(
  struct epiz_salvageState *ss,
  epi_anode_t VolAnode,
  long ix);

extern u_long epiz_MarkNoop(
  daddr_t blkno,
  baddr_t relblk,
  epi_anode_t file,
  struct epiz_salvageState *ss,
  long height);

extern u_long epiz_MarkUsed(
  daddr_t blkno,
  baddr_t relblk,
  epi_anode_t file,
  struct epiz_salvageState *ss,
  long height);

#endif /* not KERNEL */

#endif /* TRANSARC_EPI_FIXED_ANODE_H */
