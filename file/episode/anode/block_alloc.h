/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* This file contains the external interface to the block allocation module.
 * Primarily this consists of the allocation handle. */

/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/block_alloc.h,v 4.26 1994/11/02 19:15:40 cfe Exp $ */

/* Copyright (C) 1994, 1990 Transarc Corporation - All rights reserved */

#ifndef TRANSARC_EPI_BLOCK_ALLOC_H
#define TRANSARC_EPI_BLOCK_ALLOC_H

#include <dcedfs/episode/anode.h>

#define EPIB_MAX_FRAGMENTS_PER_BLOCK 64
#define EPIB_MAX_FREEDBLOCKS_PER_TRANS 10

typedef long epi_bitmap_t;

/* data returned by epib_GetInfo. */

struct epib_info {
    epi_anode_t bitmapC;		/* anode handle for bitmap container */
    daddr_t base;			/* address of first legal block */
    u_long length;			/* # blks in aggregate */
    u_long realFree;			/* # frags not allocated */
    u_long avail;			/* # frags free for routine use. 
					 * Includes freeFrags below */
    u_long freeFrags; 			/* # frags free for routing use in 
					 * partial blocks. Included in avail 
					 * above */
};

/* Flag bits passed epib_Alloc* */

#define EPIB_EXTEND_FRAGS	0x1	/* try to extent anode's frag group */
#define EPIB_MOVE_FRAGS		0x2	/* allocate new fragment group */
#define EPIB_CONTIG_ONLY	0x4	/* stop if no more contiguous blks */
#define EPIB_DONT_WITHHOLD	0x8	/* okay to consume withholding */

/* External macro definitions */

#define epib_GetAllocationHandle(h) ((epi_bitmap_t)epia_GetAllocationHandle(h))

/* External procedure definitions */

extern long epib_Init _TAKES(( void ));

extern long epib_CreateAllocationPool _TAKES((
  IN epi_anode_t bitmapC,
  IN daddr_t baseBlock,			/* first block addressed by bitmap */
  IN u_long nBlocks,			/* number of blocks in bitmap */
  IN u_int frags,			/* fragments per block */
  IN u_int nExceptions,			/* length of exception list */
  IN daddr_t exceptionBlocks[]));	/* blocks to exclude from alloc pool */

extern long epib_GrowAllocationPool(
  IN elbb_tranRec_t trans,		/* transaction identifier */
  IN epi_anode_t bitmapC,
  IN daddr_t baseBlock,			/* first block addressed by bitmap */
  IN u_long nBlocks,			/* number of blocks in bitmap */
  IN u_int nExceptions,			/* length of exception list */
  IN daddr_t exceptionBlocks[]		/* blocks to exclude from alloc pool */
);

extern long epib_InitAllocationPool _TAKES((
  IN epi_anode_t bitmapC,
  IN daddr_t firstBlock,		/* for consistency checking */
  IN u_long nBlocks,			/* ditto */
  OUT epi_bitmap_t *allocationHandleP));

extern long epib_CloseAllocationPool _TAKES((
  IN epi_bitmap_t allocationHandle));

extern long epib_GetInfo _TAKES((
  IN epi_bitmap_t allocationHandle,
  OUT struct epib_info *info));

/* SHARED */
extern long epib_StartTran _TAKES((
  IN char *serviceName,
  IN epi_anode_t anode,
  OUT buffer_tranRec_t *transP));

/* SHARED */
extern long epib_EndTran _TAKES((
  IN epi_anode_t anode,
  IN buffer_tranRec_t trans));

extern long epib_Allocate(
  buffer_tranRec_t trans,
  epi_anode_t anode,			/* to which blocks are to allocated */
  u_long *nBlocksP,			/* number to allocate */
  daddr_t bArray[],			/* for returned block addresses */
  int flags);				/* various flags */

extern long epib_Free(
  buffer_tranRec_t trans,
  epi_anode_t anode,			/* anode from which blocks come */
  u_long nBlocks,			/* number of blocks */
  daddr_t bArray[]);			/* block addresses */

extern long epib_AllocateFrags(
  buffer_tranRec_t trans,
  epi_anode_t anode,			/* to which blocks are to allocated */
  int flags,
  u_int nFrags,				/* number of frags needed */
  struct epix_fragDescription *frags);	/* returned frag group */

extern long epib_FreeFrags(
  buffer_tranRec_t trans,
  epi_anode_t anode,			/* anode from which frag group comes */
  u_int keep,				/* number to keep */
  struct epix_fragDescription *frags);	/* frag group */

#ifndef KERNEL
extern double epib_PercentFragmentCoverage(int map);
#endif /* not KERNEL */

#endif /* TRANSARC_EPI_BLOCK_ALLOC_H */
