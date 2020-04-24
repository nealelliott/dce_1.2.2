/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: anode_block.h,v $
 * Revision 1.1.83.1  1996/10/02  17:16:12  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:07:07  damon]
 *
 * Revision 1.1.77.2  1994/06/09  13:56:33  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:31:19  annie]
 * 
 * Revision 1.1.77.1  1994/02/04  20:10:37  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:08:45  devsrc]
 * 
 * Revision 1.1.75.1  1993/12/07  17:16:23  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:35:30  jaffe]
 * 
 * Revision 1.1.4.3  1993/01/18  20:55:05  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  17:42:01  cjd]
 * 
 * Revision 1.1.4.2  1992/11/18  18:22:39  jaffe
 * 	Transarc delta: ota-ot5910-anodes-per-block-typo 1.1
 * 	  Selected comments:
 * 	    Fix typo in epic_AnodesPerBlock.
 * 	    Change 3*sizeof(int32) to 4*sizeof(int32).
 * 	[1992/11/17  20:08:12  jaffe]
 * 
 * Revision 1.1.2.3  1992/05/13  21:54:00  jdp
 * 	Transarc delta: ota-16K-fid-index-map-2680 1.1
 * 	  Files modified:
 * 	    anode: anode_block.c, anode_block.h, fid.c, fixed_anode.c
 * 	    anode: many_files.test, volume_table.c
 * 	  Selected comments:
 * 	    In aggregates with 16K block size the calculation of how many anodes fit
 * 	    in a block was being done separately in fid.c and anode_block.h.
 * 	    Clearly this is a "bad thing".  Worse, however, was that the logic in
 * 	    fid.c was wrong!
 * 	    Change anode_block.c to export an array indexed by [log(block size)-10]
 * 	    of anodes per block.  Have fid.c use this.
 * 	    Declare epic_anodesPerBlock array.
 * 	    Correct epic_AnodesPerBlock macro to parenthesize its parameter.
 * 	    Fix logb_ prefixes from logbuf identifiers.
 * 	[1992/05/13  18:55:23  jdp]
 * 
 * Revision 1.1.2.2  1992/05/05  20:38:19  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  20:08:14  jdp]
 * 
 * Revision 1.1  1992/01/19  02:36:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/* anode_block.h -- This file contains structure declarations for blocks of
 *     anodes.  Its prefix is, arbitrarily, epic. */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/anode/anode_block.h,v 1.1.83.1 1996/10/02 17:16:12 damon Exp $" */

/* Copyright (C) 1993, 1990 Transarc Corporation - All rights reserved */

#ifndef TRANSARC_EPI_ANODE_BLOCK_H
#define TRANSARC_EPI_ANODE_BLOCK_H

#define ANODE_MAX_BLOCKSIZE BUF_MAXBUFSIZE

/* On VM systems don't allow the blocksize to be smaller than the pagesize in
 * the kernel, but allow test_anode to access the full range.  Othersize just
 * use the logbuf definition.  The KERNEL block size is always stricter. */

#if defined(AFS_SUNOS5_ENV) || defined(AFS_AIX_ENV)
#define ANODE_MIN_KERNEL_BLOCKSIZE PAGESIZE
#else
#define ANODE_MIN_KERNEL_BLOCKSIZE BUF_MINBLKSIZE
#endif

#ifdef KERNEL
#define ANODE_MIN_BLOCKSIZE ANODE_MIN_KERNEL_BLOCKSIZE
#else
#define ANODE_MIN_BLOCKSIZE BUF_MINBLKSIZE
#endif

#define ANODE_SIZE 252

/* These two values identify anode-containing blocks.  One for the AVL, and one
 * for all filesets. */
#define ANODE_BLOCK_AVL_MAGIC 0x3198A2E0
#define ANODE_BLOCK_VOLUME_MAGIC 0xb7afc1db

/* This value used in the first word of an anode to identify legal ones.  Only
 * the first byte is used by the volume_table module, but fixed_anode uses more
 * of the unused bits.  As an extra check, however, epit_Allocate set the flags
 * word to exactly ANODE_MAGIC, and epia_Delete also sets it to the same value.
 * This allows epit_Free to insist that it has this value. */

#define ANODE_MAGIC_MASK 0xff000000
#define ANODE_MAGIC_BASE 0xb4572c25
#define ANODE_MAGIC (ANODE_MAGIC_BASE & ANODE_MAGIC_MASK)

#define ANODE_BLOCK_HEADERSIZE (3*sizeof(int32))

#define epic_LegalAnode(a) \
    (((*(int32 *)(a)) & ANODE_MAGIC_MASK) == ANODE_MAGIC)
#define epic_IndexToOffset(i) (ANODE_BLOCK_HEADERSIZE + (i)*ANODE_SIZE)
#define epic_OffsetToIndex(o) ((o-ANODE_BLOCK_HEADERSIZE) / ANODE_SIZE)
#define epic_FirstOffset() ANODE_BLOCK_HEADERSIZE
#define epic_NextOffset(o) (o+ANODE_SIZE)
#define epic_LegalOffset(o,bsize) ((o+ANODE_SIZE) <= (bsize - sizeof(int32)))
#define epic_OffsetOK(o) (((o - ANODE_BLOCK_HEADERSIZE) % ANODE_SIZE) == 0)
#define epic_AnodesPerBlock(bsize) \
    (((bsize) - (ANODE_BLOCK_HEADERSIZE+sizeof(int32))) / ANODE_SIZE)

IMPORT u_int epic_anodesPerBlock[7];	/* index by log(bsize)-10 */

/* Functions shared with other episode modules, but not exported */

/* SHARED */
long epic_AnodeBlockType _TAKES((
  IN struct buffer *abb));

/* SHARED */
u_long epic_GetVolIndex _TAKES((
  IN struct buffer *abb));

/* SHARED */
int epic_AnodeBlockOkay _TAKES((
  IN struct buffer *abb,
  IN long magic,
  IN u_long volIndex));

/* SHARED */
long epic_FormatAnodeBlock _TAKES((
  IN buffer_tranRec_t trans,
  IN struct buffer **bP,
  IN u_long index,
  IN long magic));

/* Function exported to the mounter */

EXPORT long epic_HowMuchLikeAnodeBlock _TAKES((
  IN struct async_device *dev,
  IN opaque dataptr,
  OUT int *valP,
  OUT int32 *sigP,
  OUT opaque *firstP));

#endif
