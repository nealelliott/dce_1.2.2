/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: edecheck.c,v $
 * Revision 1.1.21.1  1996/10/02  17:17:06  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:27:01  damon]
 *
 * $EndLog$
 */
/* edecheck.c -- Program to determine whether a disk partion looks like an
 *     Episode aggregate.. */

/* Copyright (C) 1995, 1990 Transarc Corporation - All rights reserved. */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

#include <com_err.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/logbuf.h>

/* user space I/O simulation */
#include <dcedfs/episode/us.h>
#include <anode.h>
#include <anode_block.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/anode/edecheck.c,v 1.1.21.1 1996/10/02 17:17:06 damon Exp $")

#ifdef DIOCGDINFO
/* define DKTYPENAMES to get the identifiers dktypenames[], DKMAXTYPES, fstypenames[], FSMAXTYPES */
/* #define DKTYPENAMES 1 */
#include <sys/disklabel.h>
#endif /* DIOCGDINFO */

#include "boot.h"

/* Deal with some transitional 64bit stuff (db6444) */

#ifdef AFS_SUNOS5_ENV
extern offset_t llseek(int, offset_t, int);
#endif /* AFS_SUNOS5_ENV */

#if !defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX4_ENV)
#define llseek(fd,o,w) lseek(fd,(off_t)o,w)
#endif /* AFS_SUNOS5_ENV */

/* edsk_CheckAsEpisode -- Checks whether the given device looks like a Episode
 *     file system, if so \arg{pdp} is filled in from the superblock.  If
 *     every thing went well, the superblock address is assigned into
 *     \arg{SBAddrP}.
 *
 *     It uses the same return codes as \fcn{edsk_CheckAsBSD}.
 *
 * PARAMETERS --
 *     rfd -- file descriptor of raw disk device, opened for read.
 *     RawDev -- file name of disk device.  Used for diagnostic purposes.
 *     pdp -- information about partition.
 *     SBAddrP -- pointer to superblock address (in quantums). */

EXPORT int edsk_CheckAsEpisode (rfd, RawDev, pdp, SBAddrP)
  IN int rfd;
  IN char *RawDev;
  OUT struct PartData *pdp;
  OUT u_long *SBAddrP;
{
    char Data[EPISODE_INITIAL_BLOCKSIZE];
    char ErrMsg[500];                   /* best error message found */
    long bestVal = 0;			/* best value for bad superblock */
    int Val, minSize, whichAddr;
    int32 sig;
    opaque firstAnode;
    long code;
    u_long Addrs[EDSK_MAXNSUPERBLOCKS];
    int nSuperB;
    u_long bSize;			/* minimum block size of device */
    u_long Size;
    u_long qBSize;			/* bSize blks per QUANTUM */
    u_long initBSize;			/* how many bSize blks needed for a superblock */

#ifdef DIOCGDINFO
    code = edsk_TryReadDiskLabel(rfd, RawDev, pdp, FS_TRANSARC_EPISODE);
#else /* DIOCGDINFO */
    code = edsk_TryReadDiskLabel(rfd, RawDev, pdp, 0);
#endif /* DIOCGDINFO */
    *SBAddrP = 0;

    pdp->NumBigChunks = 0;
    pdp->minBlkSize = pdp->minBlkCount = 0;
    for (bSize = 128; bSize <= EPISODE_SUPERBLOCK_QUANTUM; bSize <<= 1) {
	Size = edsk_GuessDeviceCapacity(rfd, RawDev, bSize);
	if (Size != 0 && Size != -1) break;
    }
    if (bSize > EPISODE_SUPERBLOCK_QUANTUM ||
	Size == 0 || Size == -1 || bSize == 0) {
	printf("%s: empty partition\n", RawDev);
	pdp->NumBigChunks = 0;
	return edsk_NOWAY;
    }

    /* It seems that Size is really the offset of the highest sucessfully read
     * block, so bump by one to get count of valid blocks.  Except that a value
     * of zero is invalid. */
    ++Size;
    pdp->minBlkSize = bSize;
    pdp->minBlkCount = Size;

    /* A superblock really requires only EPISODE_INITIAL_BLOCKSIZE bytes of
     * storage, aligned at an address that's a multiple of
     * EPISODE_SUPERBLOCK_QUANTUM bytes.  We want NumBigChunks not really to
     * be the number of SUPERBLOCK chunk sizes available on the aggregate,
     * but rather to be the number of such spots available for holding
     * superblocks.  Thus, we compute this value by counting the number of
     * SUPERBLOCK chunk sizes available in the Size value, and then counting
     * the remainder as one if it's large enough to hold an
     * EPISODE_INITIAL_BLOCKSIZE.
     */

    /* First, get the SUPERBLOCK quantum size in bSize units */
    qBSize = (EPISODE_SUPERBLOCK_QUANTUM / bSize);
    /* Now compute how much space must be in the remainder chunk in order to
     * be able to store the superblock data, again in bSize units.
     */
    initBSize = (EPISODE_INITIAL_BLOCKSIZE + bSize-1) / bSize;
    /* Start with the truncated count.. */
    pdp->NumBigChunks = Size / qBSize;
    /* ..and bump by one if the remainder could serve as a SB. */
    if ((Size % qBSize) >= initBSize)
        pdp->NumBigChunks++;

    /* Need at least two such slots, because the first SB must be at byte
     * address 64k.
     */
    if (pdp->NumBigChunks < 2) {
	printf("%s: partition too small (under %lu bytes)\n",
	       RawDev,
	       (EPISODE_SUPERBLOCK_QUANTUM+EPISODE_INITIAL_BLOCKSIZE));
	return edsk_NOWAY;
    }

    nSuperB = edsk_NumSuperBlocks(pdp->NumBigChunks);
    nSuperB = edsk_FillSuperBlockArray(pdp->NumBigChunks, Addrs, nSuperB);
    for (whichAddr = 0; whichAddr < nSuperB; ++whichAddr) {
	osi_off_t offset =
	    (osi_off_t)Addrs[whichAddr] * EPISODE_SUPERBLOCK_QUANTUM;
	code = llseek(rfd, offset, SEEK_SET);
	if (code == -1) {
	    sprintf(ErrMsg,
		    "%s: cannot seek to %lu*%d to read Episode superblock",
		    RawDev, Addrs[whichAddr], EPISODE_SUPERBLOCK_QUANTUM);
	    perror(ErrMsg);
	    bestVal = 0;
	    continue;
	}
	minSize = sizeof(Data);
	if (pdp->BlockSize != 0 && pdp->BlockSize < minSize)
		minSize = pdp->BlockSize;
	code = read(rfd, Data, minSize);
	if (code != minSize) {
	    sprintf(ErrMsg, "%s: cannot read Episode superblock at %ld*%d",
		    RawDev, Addrs[whichAddr], EPISODE_SUPERBLOCK_QUANTUM);
	    if (code == -1) perror(ErrMsg);
	    else /* read succeeded but didn't get a whole block */
		fprintf (stderr, ErrMsg);
	    bestVal = 0;
	    continue;
	}

	code = epic_HowMuchLikeAnodeBlock(NULL, Data, &Val, &sig, &firstAnode);
	if (code != 0) {
	    printf("epic_HowMuchLikeAnodeBlock() (address %ld*%d) returns %ld.\n",
		   Addrs[whichAddr], EPISODE_SUPERBLOCK_QUANTUM, code);
	    continue;
	}

	{   char msg[500];
	    long val;
	    val = epig_SayWhyNotLikeSuperBlock (0, Data, sizeof(msg), msg);
	    if (val > bestVal) {
		strcpy (ErrMsg, msg);
		bestVal = val;
	    }
	}

	if (sig != ANODE_BLOCK_AVL_MAGIC || Val < 5000) continue;

	code = epig_FillInPartInfo(firstAnode, pdp);
	if (code != 0) continue;

	/* The call to epig_FillInPartInfo sets the BlockSize from the 
	 * aggregate header on disk */
	if ((pdp->BlockSize / pdp->minBlkSize) <= 0) {
	    printf("%s: useless sizes (blk %lu, minblk %lu)\n",
		   RawDev, pdp->BlockSize, pdp->minBlkSize);
	    return edsk_NOWAY;
	}
	Size = pdp->minBlkCount / (pdp->BlockSize / pdp->minBlkSize);
	if (Size <= 0) {
	    printf("%s: useless sizes (blk %lu, minblk %lu, blk count %lu)\n",
		   RawDev, pdp->BlockSize, pdp->minBlkSize, Size);
	    return edsk_NOWAY;
	}
/*	printf("%s: %lu %u-byte sectors (%lu %u-byte blocks).\n",
	       RawDev, Size * (pdp->BlockSize/pdp->SectorSize),
	       pdp->SectorSize, Size, pdp->BlockSize); */
	*SBAddrP = Addrs[whichAddr];
	return edsk_FULL;
    }
    if (bestVal > 0) {
	printf ("Aggregate was no good: %s\n", ErrMsg);
    }
    return edsk_NOWAY;
}
