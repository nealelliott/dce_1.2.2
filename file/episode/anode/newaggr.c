/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
*/
/*
 * HISTORY
 * $Log: newaggr.c,v $
 * Revision 1.1.22.1  1996/10/02  17:18:29  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:28:32  damon]
 *
 * Revision 1.1.16.3  1994/07/13  22:21:50  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  20:39:36  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:56:17  mbs]
 * 
 * Revision 1.1.16.2  1994/06/09  13:58:30  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:33:43  annie]
 * 
 * Revision 1.1.16.1  1994/02/04  20:11:45  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:09:26  devsrc]
 * 
 * Revision 1.1.14.2  1994/01/28  16:51:33  kinney
 * 	HPUX: in MakeAggregate(), try to detect a mounted disk -- and bail out
 * 	[1994/01/28  16:51:00  kinney]
 * 
 * Revision 1.1.14.1  1993/12/07  17:17:26  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:44:45  jaffe]
 * 
 * $EndLog$
 */
/* Copyright (C) 1995, 1990 Transarc Corporation - All rights reserved. */

/* newaggr.c -- Program to make a new Episode aggregate. */

#include <dcedfs/param.h>
#include <dcedfs/osi_device.h>
/* XXX FIX ME XXX */
#undef	PRIVATE
#undef	SHARED
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

#ifndef KERNEL
#include <pthread.h>
#endif /* KERNEL */

#include <dcedfs/ftutil.h>

#include <ctype.h>

#include <com_err.h>
#include <dcedfs/cmd.h>
#include <dcedfs/astab.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/logbuf.h>

/* user space I/O simulation */
#include <dcedfs/episode/us.h>

#include <anode.h>
#include <anode_block.h>
#include <volume.h>
#include <aggregate.h>
#include <fixed_anode.h>
#include <block_alloc.h>


#ifdef DIOCGDINFO
/* define DKTYPENAMES to get the identifiers dktypenames[], DKMAXTYPES,
 * fstypenames[], FSMAXTYPES */
/* #define DKTYPENAMES 1 */
#include <sys/disklabel.h>
#endif /* DIOCGDINFO */

#include <boot.h>
#include <badblock.h>

#include <errno.h>
#include <ustat.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/anode/newaggr.c,v 1.1.22.1 1996/10/02 17:18:29 damon Exp $")

static long arg_BlkSize;
static long arg_FragSize;
static daddr_t arg_FirstBlock;
static u_long arg_NBlocks;
static u_long arg_NLogBlocks;
static int arg_NoAction;
static int arg_OverwriteEpisode;
static int arg_OverwriteBSD;
static int arg_Verbose;
static char *arg_Device;

static char pnm[] = "newaggr";

/* Deal with some transitional 64bit stuff (db6444) */

#ifdef AFS_SUNOS5_ENV
extern offset_t llseek(int, offset_t, int);
#endif /* AFS_SUNOS5_ENV */

#if !defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX4_ENV)
#define llseek(fd,o,w) lseek(fd,(off_t)o,w)
#endif /* AFS_SUNOS5_ENV */

/* MakeEpisode -- Make an Episode file system. */

static int MakeEpisode(
  int rfd,
  int wfd,
  char *RawDev,
  int isbsd,
  struct PartData *pdbsd,
  int isepi,
  struct PartData *pdepi,
  dev_t devNum,
  int ReLabel)
{
    int nSuperBlocks;
    long blkSize, fragSize;
    daddr_t firstBlock, superBlockNum;
    u_long nBlocks, nLogBlocks;
    u_long sBlocks[EDSK_MAXNSUPERBLOCKS];
    daddr_t EpiSBNums[EDSK_MAXNSUPERBLOCKS];
    u_long DiskBlocks, BigSects;
    struct async_device *aDev;
    long code;
    int ix;
    char *Data;
    int anyFail;
    dev_t FakeUserDev;
    char ErrMsg[500];
    int Problems;
    struct epig_params epigParams;

    Problems = 0;
#define IS_POWER_OF_TWO(x) (((x) & ((x)-1)) == 0)

    if (isbsd == edsk_FULL) {
	fprintf(stderr, "*** %s CONTAINS A BSD FILE SYSTEM WITH DATA\n",
		RawDev);
	if (!arg_NoAction) {
	    if (!arg_OverwriteBSD) ++Problems;
	    else fprintf(stderr, "*** CONTINUING\n");
	}
    } else if (isbsd == edsk_EMPTY) {
	fprintf(stderr, "%s contains an empty BSD file system\n", RawDev);
    }

    if (isepi == edsk_EMPTY || isepi == edsk_FULL) {
	fprintf(stderr, "*** %s ALREADY CONTAINS AN EPISODE FILE SYSTEM\n",
		RawDev);
	if (!arg_NoAction) {
	    if (!arg_OverwriteEpisode) ++Problems;
	    else fprintf(stderr, "*** CONTINUING\n");
	}
    }

    /* Screen the block and fragment sizes. */
    blkSize = arg_BlkSize;
    if (blkSize == 0) {
	fprintf(stderr, "Episode block size must be specified.  Aborting.\n");
	++Problems;
    }
    fragSize = arg_FragSize;
    if (fragSize == 0) {
	fprintf(stderr,
		"Episode fragment size must be specified.  Aborting.\n");
	++Problems;
    }
    if (blkSize != 0 && fragSize != 0) {
	if (!IS_POWER_OF_TWO(blkSize)) {
	    fprintf(stderr,
		    "Episode block size must be a power of two, not %ld.\n",
		    blkSize);
	    ++Problems;
	}
	if (!IS_POWER_OF_TWO(fragSize)) {
	    fprintf(stderr,
		    "Episode fragment size must be a power of two, not %ld.\n",
		    fragSize);
	    ++Problems;
	}
	if (blkSize < fragSize) {
	    fprintf(stderr, "Episode fragment size (%ld) must be no larger\
 than the block size (%ld).\n",
		    fragSize, blkSize);
	    ++Problems;
	}
	if ((blkSize % fragSize) != 0) {
	    fprintf(stderr, "Episode fragment size (%ld) must evenly divide\
 the block size (%ld).\n",
		    fragSize, blkSize);
	    ++Problems;
	}
	if (!IS_POWER_OF_TWO(blkSize/fragSize)) {
	    fprintf(stderr, "Episode blocks-per-fragment (%ld == %ld/%ld)\
 must be a power of two.\n",
		    blkSize/fragSize, blkSize, fragSize);
	    ++Problems;
	}
	if (blkSize > EPISODE_SUPERBLOCK_QUANTUM) {
	    fprintf(stderr, "Episode block size (%ld) must be no larger than\
 the compile-time quantum size (%ld).\n",
		    blkSize, EPISODE_SUPERBLOCK_QUANTUM);
	    ++Problems;
	}
	if (blkSize > ANODE_MAX_BLOCKSIZE) {
	    fprintf(stderr, "Episode block size (%ld) must be no larger than\
 the compile-time block size (%ld).\n",
		    blkSize, ANODE_MAX_BLOCKSIZE);
	    ++Problems;
	}
	if (blkSize < ANODE_MIN_KERNEL_BLOCKSIZE) {
	    fprintf(stderr,
		    "Episode block size (%ld) must be at least %dK.\n",
		    blkSize, ANODE_MIN_KERNEL_BLOCKSIZE >> 10);
	    ++Problems;
	}
	if (fragSize < 1024) {
	    fprintf(stderr,
		    "Episode fragment size (%ld) must be at least 1k (%ld).\n",
		    fragSize, 1024);
	    ++Problems;
	}
	if (blkSize != 8192 || fragSize != 1024) {
	    fprintf(stderr,
		    "Warning: The only supported block size/fragment size\
 combination is 8k/1k.\n");
	}
    }

    /* Check block range: start at firstBlock, continue for nBlocks */
    firstBlock = arg_FirstBlock;
    if (firstBlock == -1) {
	firstBlock = 1;
	fprintf(stderr,
		"*** Using default initialempty value of %d.\n", firstBlock);
    }
    if ((firstBlock * blkSize) > EPISODE_SUPERBLOCK_ADDR) {
	fprintf(stderr, "First block of %lu (block size %lu) starts the file \
system at byte %lu, which is after the canonical superblock address of %lu.\n",
		firstBlock, blkSize, firstBlock * blkSize,
		EPISODE_SUPERBLOCK_ADDR);
	++Problems;
    }

    /* Now, NumBigChunks is set to as many 64k-boundary chunks as there are
	that start with at least 1024 bytes of space.  That is,
	NumBigChunks*64k bytes may actually be larger than the size of the
	disk.  But at the same time, we require that there be a second chunk,
	of actual size at least 1024 bytes, in order to hold the Episode
	superblock, which is always stored at offset 64k bytes into the disk.
    */
    if (pdepi->NumBigChunks < 2) {
	BigSects = 0;
	nSuperBlocks = 0;
	fprintf(stderr, "%s: partition too small for Episode--holds fewer\
 than %ld bytes.\n",
		RawDev, EPISODE_SUPERBLOCK_QUANTUM+EPISODE_INITIAL_BLOCKSIZE);
	++Problems;
    } else {
	BigSects = pdepi->NumBigChunks;
	nSuperBlocks = edsk_NumSuperBlocks(BigSects);
	nSuperBlocks =
	    edsk_FillSuperBlockArray(BigSects, sBlocks, nSuperBlocks);
	if (arg_Verbose)
	    printf("%s: %lu %u-byte sectors, giving %d superblocks.\n",
		    RawDev, BigSects, EPISODE_SUPERBLOCK_QUANTUM,
		   nSuperBlocks);
    }

    /* Find the total size of the new aggregate. */
    DiskBlocks = 0;
    superBlockNum = 0;
    if (blkSize > 0 && pdepi->minBlkSize > 0
	&& (blkSize/pdepi->minBlkSize) > 0) {
	DiskBlocks = pdepi->minBlkCount / (blkSize / pdepi->minBlkSize);
	if (DiskBlocks <= 0) {
	    fprintf(stderr, "%s: partition too small for Episode--cannot hold\
 one block of %ld bytes.\n",
		    RawDev, blkSize);
	    ++Problems;
	}
    }
    nLogBlocks = 0;
    if (DiskBlocks > 0 && BigSects > 0 && blkSize > 0) {
	if (DiskBlocks <
	    ((BigSects-1) * (EPISODE_SUPERBLOCK_QUANTUM / blkSize))) {
	    fprintf(stderr, "%s: internal inconsistency: disk sizes are not\
 comparable; %ld %ld-byte disk blocks is smaller than %ld %ld-byte chunks.\n",
		    RawDev, DiskBlocks, blkSize, BigSects,
		    EPISODE_SUPERBLOCK_QUANTUM);
	    ++Problems;
	}
	if (firstBlock >= DiskBlocks) {
	    fprintf(stderr, "initialempty cannot be %lu; disk (in %lu-byte\
 blocks) is only %lu blocks big!\n",
		    firstBlock, blkSize, DiskBlocks);
	    ++Problems;
	    nBlocks = 0;
	} else {
	    if (arg_NBlocks != 0) {	/* explicit argument given--check it */
		if ((arg_NBlocks + firstBlock) > DiskBlocks) {
		    fprintf(stderr, "%lu is too many blocks (with \
initialempty %lu); (%lu + %lu) = %lu is larger than the number of physical \
disk blocks, %lu, of size %lu.\n",
			    arg_NBlocks, firstBlock, arg_NBlocks, firstBlock,
			    arg_NBlocks + firstBlock, DiskBlocks, blkSize);
		    ++Problems;
		    nBlocks = 0;
		} else {
		    nBlocks = arg_NBlocks;
		}
	    } else { 
		/* no explicit argument. Take the whole disk from 
		 * firstBlock onwards. */
		nBlocks = DiskBlocks - firstBlock;
		fprintf(stderr,
			"*** Using default number of (%lu-byte) blocks: %lu\n",
			blkSize, nBlocks);
	    }
	}
	if (nBlocks > 0) {

	    unsigned minlogsize;
	    unsigned maxload;
	    char defaultlogsize=0; /* flag to indicate default log size */
            /*
             * Limit the logsize to a value which allow many concurrent
             * transactions, but is small enough to avoid memory usage problems
             * in the salvager.  Otherwise the salvager may fail while running
             * recovery, since it may try to hold a significant portion of the
             * log in the memory.
             */
#define MAX_LOG_SIZE (16*1024*1024)
            unsigned maxlogsize = MAX_LOG_SIZE / blkSize;

	    /* This is a bit dangerous, but as a user space program it is
	     * probably okay. */
	    struct async_device *dev;
	    code = asevent_InitDevice(US_MAJOR_DEV, 1, blkSize, &dev);
	    if (code) {
		/* want to handle the case of a bad block-size, for instance,
		   without abort()ing. */
		com_err(pnm, code,
			"calling asevent_InitDevice: %s (%d,%d, %d) (%d,%d)",
			RawDev, US_MAJOR_DEV, 1, blkSize,
			osi_major(devNum), osi_minor(devNum));
		++Problems;
	    } else {

		minlogsize = elbb_ComputeMinLogSize (dev, 1);

		if (arg_NLogBlocks != 0) {	/* explicitly given */
		    if (arg_NLogBlocks >= nBlocks*7/8) {
			fprintf(stderr, "Cannot devote more than 7/8ths of\
 the aggregate's %lu %lu-byte blocks, to the log.\n",
			    blkSize, nBlocks);
			++Problems;
		    } else if ( arg_NLogBlocks < minlogsize ) {
			fprintf(stderr, "Aggregate log size must be at least\
 %u %lu-byte blocks.\n",
				minlogsize, blkSize);
			++Problems;
                    } else if (arg_NLogBlocks > maxlogsize) {
                        fprintf(stderr, "Aggregate log size cannot exceed\
 %uMbytes.\n",
                                MAX_LOG_SIZE>>20);
                        ++Problems;
		    } else {
			nLogBlocks = arg_NLogBlocks;
		    }
		} else {	/* Log size needs to be defaulted. */

		    defaultlogsize=1;

		    if (nBlocks <= 1) {
			fprintf(stderr, "Cannot devote any blocks, out of a\
 total of %lu blocks, to the log.\n",
				nBlocks);
			++Problems;
		    } else {
			/* Preliminary considerations suggest that a log needs
                         * at least 8K*2+fudge bytes (for an acl) or at least
                         * 3-5 blocks for operations that involve indirect
                         * blocks.  There are probably better limits to use.
                         * For many simulaneous users you probably want much
                         * more (say 1% of the aggregate). */

			/* The above is obsolete since the log module now
                         * exports its (definitive) estimate of the minsize. */

			/* Need bigger logs than 1% on small aggregates. */

			/* Start with 50 blocks, bound it by 1% and 10%
			 * of the aggregate size, then bound it by what the
			 * log module says. */

			nLogBlocks = 50;
			if (nLogBlocks < ((nBlocks +99)/ 100)) {
			    /* no less than 1% */
			    nLogBlocks = (nBlocks+99)/100;
			}
			if (nLogBlocks > (nBlocks/10)) {
			    /* no more than 10% */
			    nLogBlocks = nBlocks/10;
			}
			if (nLogBlocks < minlogsize) {
			    /* no less than what log module says it needs */
			    nLogBlocks = minlogsize;
			}
                        if (nLogBlocks > maxlogsize) {
                            /* bound to max log size */
                            nLogBlocks = maxlogsize;
                        }
		    }
		}

		/*
		 * If all ok, calculate max. transaction concurrency allowed.
		 */
		if ( nLogBlocks ) {
		    maxload = 1;
		    /* XXX A better search algorithm may be desired */
		    while (nLogBlocks >=
			   elbb_ComputeMinLogSize(dev, maxload+1)) {
			maxload++;
		    }
		    fprintf(stderr,
 "*** %s %lu log blocks (maximum of %d concurrent transactions).\n",
			    (defaultlogsize ? "Defaulting to" : "Reserving"),
			    nLogBlocks, maxload);
		}
		asevent_CloseDevice (dev);
	    }
	    if (nSuperBlocks > 0) {
		nSuperBlocks =
		    edsk_FillSuperBlockArray(BigSects, sBlocks, nSuperBlocks);
		superBlockNum = EPISODE_SUPERBLOCK_ADDR / blkSize;
	    }
	}
    }


    if (Problems != 0) {
	return 1;
    }

    for (ix = 0; ix < nSuperBlocks; ++ix) {
	EpiSBNums[ix] = sBlocks[ix] * (EPISODE_SUPERBLOCK_QUANTUM/blkSize);
    }

    if (arg_Verbose) {
	fprintf(stderr,
		"Device %s, major %d, minor %d; total %lu %lu-byte blocks.\n",
		RawDev, osi_major(devNum), osi_minor(devNum), DiskBlocks,
		blkSize);
	fprintf(stderr,
	    "Block size %lu, frag size %lu, firstBlock %lu, nBlocks %lu\n",
	     blkSize, fragSize, firstBlock, nBlocks);
	fprintf(stderr,
	    "Principal superblock at block %lu, nLogBlocks %lu.\n",
	     superBlockNum, nLogBlocks);
	if (nSuperBlocks > 1) {
	    fprintf(stderr,
		"%lu exceptions (additional superblock locations) at:\n",
		 nSuperBlocks-1);
	    for (ix = 1; ix < nSuperBlocks; ++ix) {
		fprintf(stderr,
		    "  except[%d]: bigblk %lu, block %lu\n",
		     ix, sBlocks[ix], EpiSBNums[ix]);
	    }
	}
    }

    if (arg_NoAction)
	return -1;

    /* Now we're ready to go ahead.  If we are relabeling the disk and marking
     * it as non-BSD, do it now so we don't overwrite BSD superblocks that are
     * really Episode structures.  We take it for granted that no Episode
     * structures look like BSD superblocks. */

    if (ReLabel) {
	code = 0;
	if (pdepi->Labelled != 0) {
	    code = edsk_RewriteDiskLabel(rfd, RawDev, pdepi);
	    if (code == 0) {
		printf("edsk_RewriteDiskLabel(%s): OK.", RawDev);
		printf ("Partition labeled as Episode.\n");
	    } else {
		printf("edsk_RewriteDiskLabel(%s): failure %d.\n",
			RawDev, code);
		return 1;
	    }
	}
	if (code == 0 && pdbsd->SectorSize != 0) {
	    code = edsk_MarkAsNotBSD(wfd, RawDev, pdbsd);
	    if (code == 0) {
		printf("%s: Marked as not a BSD file system any more.\n",
			RawDev);
	    } else if (code > 0) {
		printf("%s: Already marked as non-BSD.\n", RawDev);
	    } else {
		sprintf(ErrMsg,
			"%s: Attempt to mark as non-BSD system failed (%d)",
			RawDev, code);
		perror(ErrMsg);
		return 1;
	    }
	}
    }

    /* Initialize all the stuff Episode needs to run. */

    epigParams.elbbParams.bufferPool = (char *)osi_Alloc(1000000);
    epigParams.elbbParams.byteCount = 1000000;
    epigParams.maxHandles = 1000;
    if (code = epig_Init(&epigParams)) {
	com_err (pnm, code, "during module initialization");
	return 1;
    }

    /* Open device */
#ifdef KERNEL
    /* This is sort of irrelevant since newaggr doesn't run in the kernel. */
    code = asevent_InitDevice(osi_major(devNum), osi_minor(devNum),
		blkSize, &aDev);
    if (code != 0) {
	com_err(pnm, code, "opening device %s (%d,%d)",
		RawDev, osi_major(devNum), osi_minor(devNum));
	return 1;
    }
#else /* KERNEL */
    /* Have to do this in the user-space simulator. */
    code = us_InitDevice (RawDev, blkSize,
			 ((osi_off_t)(nBlocks + firstBlock)) *
			  ((osi_off_t)blkSize),
			 US_OFLAGS_SYNC, &FakeUserDev, &aDev);
    if (code != 0) {
	com_err (pnm, code, "Initializing device: %s (%d,%d) (%d,%d)",
		 RawDev, US_MAJOR_DEV, 1,
		 osi_major(devNum), osi_minor(devNum));
	return 1;
    }
#endif /* KERNEL */

    /* Do the epig_CreateAggregate call */
    code = epig_CreateAggregate(aDev, fragSize, firstBlock, superBlockNum,
				nBlocks, nLogBlocks,
				(nSuperBlocks > 1? (nSuperBlocks-1) : 0),
				&(EpiSBNums[1]));
    if (code != 0) {
	com_err(pnm, code, "creating an aggregate on device %s", RawDev);
	(void) asevent_CloseDevice(aDev);
	return 1;
    }
    if (arg_Verbose)
	fprintf(stderr, "%s: epig_CreateAggregate() returned OK.\n", RawDev);
    (void) asevent_CloseDevice(aDev);

    /* Overwrite the stuff in the Episode PartData block and replicate the
     * superblock to alternate locations. */

    pdepi->BlockSize = blkSize;
    pdepi->FragmentSize = fragSize;
    pdepi->TotalBlocks = nBlocks;
    pdepi->FirstBlock = firstBlock;
    pdepi->NLogBlocks = nLogBlocks;

    if (nSuperBlocks > 1) {
	/* Now make copies of real superblock in the alternate locations.  */
	Data = (char *) osi_Alloc(blkSize);
	if (Data == NULL) {
	    fprintf(stderr, "No space to use in copying superblock--no\
 backups written\n");
	    return 1;
	}
	code = llseek(rfd, (osi_off_t)EPISODE_SUPERBLOCK_ADDR, SEEK_SET);
	if (code == -1) {
	    sprintf(ErrMsg,
		    "%s: cannot seek to %lu to read new Episode superblock",
		    RawDev, EPISODE_SUPERBLOCK_ADDR);
	    perror(ErrMsg);
	    return 1;
	}
	code = read(rfd, Data, (long) blkSize);
	if (code != blkSize) {
	    sprintf(ErrMsg, "%s: cannot read new Episode superblock", RawDev);
	    perror(ErrMsg);
	    return 1;
	}
	anyFail = 0;
	for (ix = 1; ix < nSuperBlocks; ++ix) {
	    osi_off_t offset =
		(osi_off_t)sBlocks[ix] * EPISODE_SUPERBLOCK_QUANTUM;
	    code = llseek(wfd, offset, SEEK_SET);
	    if (code == -1) {
		sprintf(ErrMsg, "%s: cannot seek to %lu*%d to write backup\
 Episode superblock",
			RawDev, sBlocks[ix], EPISODE_SUPERBLOCK_QUANTUM);
		perror(ErrMsg);
		++anyFail;
		continue;
	    }
	    code = write(wfd, Data, (long) blkSize);
	    if (code != blkSize) {
		sprintf(ErrMsg, "%s: cannot write backup Episode superblock\
 at address %lu*%d",
			RawDev, sBlocks[ix], EPISODE_SUPERBLOCK_QUANTUM);
		perror(ErrMsg);
		++anyFail;
		continue;
	    }
	}
	osi_Free(Data, blkSize);
	if (anyFail) return 1;
    }
#ifndef	AFS_AIX31_ENV		/* XXXXXX FIX THIS XXXXXX */
    code = fsync(wfd);
    if (code != 0) {
	sprintf(ErrMsg, "%s: cannot fsync the device", RawDev);
	perror(ErrMsg);
	return 1;
    }
#endif
    return 0;
}

static void MakeAggregate(void)
{
    int ac, rfd, wfd;
    char ErrMsg[MAXPATHLEN + 100];
    char RawDev[MAXPATHLEN + 1];
    struct PartData BSDData;
    struct PartData EpiData;
    int BSDRes, EpiRes;
    u_long SBAddr;
    dev_t DevNum;
    int exitcode = 0;
    long code;
    struct stat StatBuf;
    ftu_devLock_t DevLock;
    struct ustat ubuff;
#ifdef AFS_HPUX_ENV
    dev_t BlkDev, MajorNum;
#endif
    int DidLock = 0;


    rfd = edsk_GetRawDevice(arg_Device, RawDev, &DevNum);
    if (rfd < 0) {
	fprintf (stderr, "%s: cannot open raw device %s\n", pnm, arg_Device);
	exit (rfd);
    }

    if (stat(RawDev, &StatBuf) == -1) {
	com_err(pnm, errno, "stating raw device: %s", RawDev);
	exit(1);
    }

    /*
     * Check if device is in use by a mounted file system.
     *
     * On HP/UX, the device number of the char device is different from the
     * device number of the block device, so we have to convert.
     */
    if (S_ISCHR(StatBuf.st_mode)) {
#ifdef AFS_HPUX_ENV
	BlkDev = DevNum;
	MajorNum = (BlkDev >> 24) & 0xff;
	switch (MajorNum) {
	    case 47: MajorNum = 7; break;	/* s700 SCSI disk */
	    case 13: MajorNum = 7; break;	/* s800 SCSI disk */
	    case 12: MajorNum = 10; break;	/* s800 disc2 */
	    case 7: MajorNum = 8; break;	/* s800 disc1 */
	}
	BlkDev &= 0x00ffffff;
	BlkDev |= (MajorNum << 24);

	if (ustat (BlkDev, &ubuff) == 0)
#else
	if (ustat (DevNum, &ubuff) == 0)
#endif
	{
	    fprintf (stderr, "%s: %s appears to be a mounted disk\n",
		     pnm, arg_Device);
	    exit(1);
	}
    }

    if (S_ISCHR(StatBuf.st_mode) && !arg_NoAction) {
	code = ftu_LockAggrDevice(RawDev, &DevLock);
	if (code) {
	    com_err(pnm, code, "locking device");
	    exit(1);
	}
	DidLock = 1;
    }

    bzero((char *)&BSDData, sizeof(BSDData));
    BSDRes = edsk_CheckAsBSD(rfd, RawDev, &BSDData);
    if (arg_Verbose) {
	printf("edsk_CheckAsBSD(``%s'') returns %d.\n", RawDev, BSDRes);
	edsk_DumpPartData(&BSDData);
    }

    bzero((char *)&EpiData, sizeof(EpiData));
    EpiRes = edsk_CheckAsEpisode(rfd, RawDev, &EpiData, &SBAddr);
    if (arg_Verbose) {
	printf("edsk_CheckAsEpisode(``%s'') returns %d; SB@%lu*%d.\n",
		RawDev, EpiRes, SBAddr, EPISODE_SUPERBLOCK_QUANTUM);
	edsk_DumpPartData(&EpiData);
    }

    if (BSDData.SectorsPerTrack > 0 && BSDData.TracksPerCylinder > 0
	&& EpiData.NumBigChunks != 0) {
	int nsb, ix;
	u_long SBs[EDSK_MAXNSUPERBLOCKS];
	daddr_t sect;
	u_long cyl, trk;

	nsb = edsk_NumSuperBlocks(EpiData.NumBigChunks);
	printf("Checking scatter of %lu superblocks; ncyl %ld, ", nsb,
	       BSDData.TotalCylindersInPartition > 0
	       ? BSDData.TotalCylindersInPartition
	       : BSDData.TotalCylindersInDisk);
	if (BSDData.SectorsPerCylinder > 0)
	    printf("%ld sec/cyl, ", BSDData.SectorsPerCylinder);
	printf("%ld trk/cyl, %ld sec/trk\n",
		BSDData.TracksPerCylinder, BSDData.SectorsPerTrack);
	nsb = edsk_FillSuperBlockArray(EpiData.NumBigChunks, SBs, nsb);
	for (ix = 0; ix < nsb; ++ix) {
	    sect = SBs[ix] *
		    (EPISODE_SUPERBLOCK_QUANTUM /
		     (BSDData.SectorSize ? BSDData.SectorSize : 512));
	    printf("  sb[%d]: %lu, sect %lu: ", ix, SBs[ix], sect);
	    if (BSDData.SectorsPerCylinder > 0) {
		cyl = sect / BSDData.SectorsPerCylinder;
		sect -= cyl * BSDData.SectorsPerCylinder;
	    } else {
		cyl = sect /
		    (BSDData.SectorsPerTrack * BSDData.TracksPerCylinder);
		sect -= cyl *
		    (BSDData.SectorsPerTrack * BSDData.TracksPerCylinder);
	    }
	    trk = sect / BSDData.SectorsPerTrack;
	    sect -= trk * BSDData.SectorsPerTrack;
	    printf("cyl %lu, trk %lu, sect %lu\n", cyl, trk, sect);
	}
    }

    if (!arg_NoAction) {
	wfd = open(RawDev, O_RDWR, 0);
	if (wfd < 0) {
	    sprintf(ErrMsg, "open-for-write(%s)", RawDev);
	    perror(ErrMsg);
	    exitcode = 1;
	    goto rclose;
	}
    }
    ac = MakeEpisode(rfd, wfd,
		     RawDev, BSDRes, &BSDData, EpiRes, &EpiData, DevNum, 1);
    if (ac == 0) {
	fprintf(stderr, "Done.  %s is now an Episode aggregate.\n", RawDev);
	if (arg_Verbose)
	    edsk_DumpPartData(&EpiData);
    } else if (ac == -1) {		/* NoAction */
	goto rclose;
    } else {
	fprintf(stderr, "Problem with making %s an Episode aggregate.\n",
		RawDev);
	exitcode = 1;
    }
    close(wfd);
rclose:
    if (DidLock) {
	code = ftu_UnlockAggrDevice(DevLock);
	if (code) {
	    com_err(pnm, code, "unlocking device");
	    if (exitcode == 0) exitcode = 1;
	}
    }

    close(rfd);

    exit(exitcode);
}

static int newaggr(struct cmd_syndesc *as, char *arock)
{
    struct astab astab;
    struct ufs_astab ufstab;
    FILE *filep;
    extern char *AstabFileName;
    char buffer[256];

    arg_Device = as->parms[0].items->data;
    arg_BlkSize = atol(as->parms[1].items->data);
    arg_FragSize = atol(as->parms[2].items->data);
    if (as->parms[3].items)
	arg_FirstBlock = atol(as->parms[3].items->data);
    else arg_FirstBlock = -1;
    if (as->parms[4].items)
	arg_NBlocks = atol(as->parms[4].items->data);
    else arg_NBlocks = 0;
    if (as->parms[5].items)
	arg_NLogBlocks = atol(as->parms[5].items->data);
    else arg_NLogBlocks = 0;
    arg_OverwriteBSD = arg_OverwriteEpisode = (as->parms[6].items != 0);
    arg_Verbose = (as->parms[7].items != 0);
    arg_NoAction = (as->parms[8].items != 0);

    MakeAggregate ();
    return 0;
}

main(int argc, char **argv)
{
    struct cmd_syndesc *ts;

    osi_setlocale(LC_ALL, "");
    ts = cmd_CreateSyntax((char *)0, (int (*)())newaggr, (char *)0,
			  "Create an aggregate\n" );
    cmd_AddParm (ts, "-aggregate", CMD_SINGLE, 0, "name");
    cmd_AddParm (ts, "-blocksize", CMD_SINGLE, 0, "bytes");
    cmd_AddParm (ts, "-fragsize", CMD_SINGLE, 0, "bytes");
    cmd_AddParm (ts, "-initialempty", CMD_SINGLE, CMD_OPTIONAL, "blocks");
    cmd_AddParm (ts, "-aggrsize", CMD_SINGLE, CMD_OPTIONAL, "blocks");
    cmd_AddParm (ts, "-logsize", CMD_SINGLE, CMD_OPTIONAL, "blocks");
    cmd_AddParm(ts, "-overwrite", CMD_FLAG, CMD_OPTIONAL,
		"replace any existing file system");
    cmd_AddParm(ts, "-verbose", CMD_FLAG, CMD_OPTIONAL,
		"report more verbose information during salvage");
    cmd_AddParm(ts, "-noaction", CMD_FLAG, CMD_OPTIONAL,
		"don't actually modify device");

    return cmd_Dispatch(argc, argv);
}
