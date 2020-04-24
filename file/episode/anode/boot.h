/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: boot.h,v $
 * Revision 1.1.83.1  1996/10/02  17:16:44  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:18:09  damon]
 *
 * $EndLog$
 */
/* This file contains the interface to the generic boot-block descriptor. */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/anode/boot.h,v 1.1.83.1 1996/10/02 17:16:44 damon Exp $ */

/* Copyright (C) 1995, 1990 Transarc Corporation - All rights reserved */

#ifndef TRANSARC_EPI_BOOT_H
#define TRANSARC_EPI_BOOT_H

struct PartData {
    long    SectorSize;			/* In bytes.
					 * from bsdSB: fs_fsize>>fs_fsbtodb, or
					 * label: d_secsize, or default: 512 */

    /* Disk geometry */
    long    SectorsPerTrack;		/* from bsdSB: fs_nsect, or
					 * label: d_nsectors */
    long    TracksPerCylinder;		/* from bsdSB: fs_ntrak, or
					 * label: d_ntracks */
    long    TotalCylindersInDisk;	/* from label: d_ncylinders */
    long    TotalCylindersInPartition;	/* from bsdSB: fs_ncyl */

    /* Bad and alternate block room. */
    long    SectorsPerCylinder;		/* from bsdSB: fs_spc, or
					 * label: d_secpercyl */
    long    SpareSectorsPerTrack;	/* from bsdSB: fs_npsect-fs_nsect, or
					 * label: d_sparespertrack */
    long    SpareSectorsPerCylinder;	/* from bsdSB: fs_nsect*fs_ntrak -
					   fs_spc, or label: d_sparespercyl */

    /* Attributes imposed by file system */
    long    BlockSize;			/* from epiSB: agg->blockSize,
					 * or bsdSB: fs_bsize,
					 * or label-if-BSD: p_frag*p_fsize,
					 * or label-if-AFS: p_frag*p_fsize */
    long    FragmentSize;		/* from epiSB: agg->fragmentSize,
					 * or bsdSB: fs_fsize,
					 * or label-if-BSD: p_fsize,
					 * or label-if-AFS: p_fsize */
    u_long	TotalBlocks;		/* from epiSB: agg->nBlocks,
					 * or bsdSB: fs_size,
					 * or label-if-BSD:
					 * p_size/(p_fsize/d_secsize),
					 * or label-if-AFS:
					 * p_size/(BlockSize/d_secsize) */
    u_long	TotalSectors;		/* from label-if-BSD: p_size, or
					 * label-if-AFS: p_size */
    u_long	FileSysCreateTime;	/* from epiSB: agg->timeCreated, or
					 * bsdSB: fs_time */
    long        Labelled;		/* whether we saw a label */
    u_long	FirstBlock;		/* from epiSB: agg->firstBlock */
    u_long	NLogBlocks;		/* from epiSB: agg->nLogBlocks */
    u_long	lastVerifyTime;		/* from epiSB: agg->lastVerifyTime */
    u_long	lastVerifyCode;		/* from epiSB: agg->lastVerifyCode */
    u_long	NumBigChunks;		/* how many size-QUANTUM chunks could
                                         * be read, minus 1 */
    u_long	minBlkSize;		/* min block size from size-determining
                                         * experiment */
    u_long	minBlkCount;		/* block count from experiment */
    long	FileSysClean;		/* from bsdSB: fs_clean==FS_CLEAN */
    long	FileSysEmpty;		/* from bsdSB:
					 * few inodes&blocks in use */
    char        FileSysMountedAs[512];	/* from bsdSB: fs_fsmnt */
};

/* The super block quantum was chosen so that it will be at the beginning of
 * the largest possible block.  We need to identify this explicitly because
 * until we read the super block we don't actually know what the block size is.
 * Further, for redundancy we duplicate the super block in other locations on
 * the disk.  These locations are specified in multiples of this quantum (the
 * primary super block is at a quantum multiple of one).  The list of these
 * multiples is specified in the edsk_superBlockLockations array defined below.
 *
 * Since we don't know the block size when we need to read the super block we
 * temporarily use a blocksize of 1024 (EPISODE_INITIAL_BLOCKSIZE) when using
 * the buffer system to read the super block.  This is okay since all the
 * important information is contained in the first two anode slots (abourt 512
 * bytes) which contain the aggregate header and the avl anode. */

#define	EPISODE_SUPERBLOCK_QUANTUM	(1<<16)
#define	EPISODE_SUPERBLOCK_ADDR	(1*EPISODE_SUPERBLOCK_QUANTUM)
#define	EPISODE_INITIAL_BLOCKSIZE	(1024)
#define	EPISODE_DISKBUFFER_MINSIZE  (8192)

/* These are the random locations for superblocks:  The values are calculated
 * in bc using "kas getrandomkey" to produce a 32bit random mantissa.  Then
 * these numbers are distributed logrithmically over the range from 1 to 2^32.
 * The first superBlock is at 1 (QUANTUM) the first is discarded and the rest
 * are used a multipliers of the QUANTUM with the fractional part dropped. */

/* ibase=16
   scale=10
   x=2.0 ^ 20
   y=l(x)
   e(y*0.0A4F91C3900000000) = 2.4433359477594905
   e(y*0.17074E76F00000000) = 7.3541960057393043
   e(y*0.2C6B5C82800000000) = 46.9294791054720092
   e(y*0.3D698817000000000) = 204.5811060704495292
   e(y*0.4DABF630D00000000) = 836.9314579419636064
   e(y*0.5F98B154300000000) = 3955.2604374434844586
   e(y*0.6D2F10B1E00000000) = 12836.6494019113693955
   e(y*0.78F447E5E00000000) = 35592.3053302408569321
   e(y*0.82E29EC1E00000000) = 84148.7586049009764318
   e(y*0.99FE403D800000000) = 623118.0427470130395597
   e(y*0.AE7F69E2600000000) = 3682396.8349259909234893
   e(y*0.B42A8A62700000000) = 6017662.4798564387536370
   e(y*0.C9EC8295100000000) = 39640819.1936690193334341
   e(y*0.D113D7E9E00000000) = 73675885.8574193476054380
   e(y*0.E88D4061800000000) = 563160323.0932189186146662
   e(y*0.F1EEE808600000000) = 1269535423.6079161463149392
 */

/* Maximum number of Episode superblocks, including the basic one */
#define EDSK_MAXNSUPERBLOCKS 16
/* SHARED */
u_long edsk_superBlockLocations[EDSK_MAXNSUPERBLOCKS]
#ifdef EDSK_INITIALIZE_SUPERBLOCKLOCATIONS
 = {1, 7, 46, 204, 836, 3955, 12836, 35592, 84148, 623118, 3682396,
    6017662, 39640819, 73675885, 563160323, 1269535423}
#endif
;

/* Return values from edsk_CheckAsBSD and edsk_CheckAsEpisode. */
#define	edsk_NOWAY		0
#define	edsk_UNKNOWN	1
#define	edsk_EMPTY		2
#define	edsk_FULL		3

/* from edbasics.c */
/* Open the device in raw mode, returning its file descriptor or an error. */
/* Return the full path name via Path. */
IMPORT int edsk_GetRawDevice _TAKES((
				      char *dev,
				      char *Path,
				      dev_t *DevnumP));

/* Open the device in raw mode, returning its file descriptor or an error. */
/* Return the full path to raw device and block device via RawDevice and */
/* BlockDevice, respectively. */
IMPORT int edsk_GetDevice _TAKES((
				   char *dev,
				   char *RawDevice,
				   dev_t *DevnumP,
				   char *BlockDevice));

/* Guess the capacity of the given device, in blksiz units, or return -1 on
 * error. */
IMPORT long edsk_GuessDeviceCapacity _TAKES((
					      int rfd,
					      char *RawDev,
					      long blksiz));

/* Try to fill in *pdp by reading the disk label. */
IMPORT int edsk_TryReadDiskLabel _TAKES((
					  int rfd,
					  char *RawDev,
					  struct PartData *pdp,
					  int OKFileSys));

/* Write a new disk label corresponding to the formatting as an Episode file
 * system. */
IMPORT int edsk_RewriteDiskLabel _TAKES((
					  int rfd,
					  char *RawDev,
					  struct PartData *pdp));

/* Dump out a PartData structure on stdout. */
IMPORT void edsk_DumpPartData _TAKES((struct PartData *pdp));

/* from ederoot.c */
/* Return the number of superblocks that will be written on a device if there
 * are Chunks chunks on that device. */
IMPORT int edsk_NumSuperBlocks _TAKES((u_long Chunks));

/* Fill the given array with no more than maxCount superblock numbers. */
/* Return the number actually given. */
IMPORT int edsk_FillSuperBlockArray _TAKES((
					     u_long numChunks,
					     u_long Arr[],
					     int maxCount));

/* from edbcheck.c */
/* Check whether the given device looks at all like a BSD file system. */
IMPORT int edsk_CheckAsBSD _TAKES((
				    int rfd,
				    char *RawDev,
				    struct PartData *pdp));

/* Mark the given device as not a BSD system.  Overwrite key attributes. */
/* Return 0 if done OK, >0 if was already done, <0 on any error. */
IMPORT int edsk_MarkAsNotBSD _TAKES((
				      int wfd,
				      char *RawDev,
				      struct PartData *pdp));

/* from edecheck.c */
/* Check whether the given device looks at all like an Episode file system. */
IMPORT int edsk_CheckAsEpisode _TAKES((
					int rfd,
					char *RawDev,
					struct PartData *pdp,
					u_long *SBAddrP));

#endif /* TRANSARC_EPI_BOOT_H */
