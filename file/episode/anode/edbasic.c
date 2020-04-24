/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: edbasic.c,v $
 * Revision 1.1.26.1  1996/10/02  17:17:03  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:26:59  damon]
 *
 * Revision 1.1.20.3  1994/07/13  22:21:32  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  20:31:43  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:55:56  mbs]
 * 
 * Revision 1.1.20.2  1994/06/09  13:57:21  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:32:23  annie]
 * 
 * Revision 1.1.20.1  1994/02/04  20:11:01  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:08:59  devsrc]
 * 
 * Revision 1.1.18.1  1993/12/07  17:16:48  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:38:53  jaffe]
 * 
 * Revision 1.1.6.6  1993/01/18  22:02:54  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  21:42:20  cjd]
 * 
 * Revision 1.1.6.5  1992/11/24  16:19:32  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:17:25  bolinger]
 * 
 * Revision 1.1.6.4  1992/10/13  18:10:52  marty
 * 	OSC1.1.1 upgrade.  Changes are osc1.0.4 compatible
 * 	[1992/10/13  17:09:58  marty]
 * 
 * Revision 1.1.7.2  1992/10/06  22:04:07  garyf
 * 	use POSIX for SEEK_SET
 * 	[1992/09/29  20:06:07  garyf]
 * 
 * Revision 1.1.6.3  1992/09/15  17:30:59  jaffe
 * 	Transarc delta: mason-add-hp800-osi-routines 1.15
 * 	  Selected comments:
 * 	    Need to checkpoint to fix a different bug.
 * 	    Checkpoint the first system where test_anode works.  Only one change is not
 * 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
 * 	    is -lBSD.  This should be fixed more generally.
 * 	    More HP-UX changes.
 * 	    More HP-UX related changes.
 * 	    Fix compile problem on the RIOS.
 * 	    Fix more compilation problems.
 * 	    Checkpoint.
 * 	    Checpoint.
 * 	    In this snap we turned all malloc() calls into osi_Alloc calls.
 * 	    Also turned free calls into osi_Free.
 * 	    s/malloc/osi_Alloc/
 * 	    HP-UX EOF detection fix.
 * 	    EOF detection on ENOSPC not ENXIO/EINVAL/EIO.
 * 	    Another checkpoint.
 * 	    This is a checkpoint of the first working HP-UX Episode system.
 * 	    Needed to add the vnops routines specific to HP-UX.
 * 	    Fix compilation problems.
 * 	Transarc delta: mason-ot2930-logbuf-parts-of-nbs 1.14
 * 	  Selected comments:
 * 	    This is the logbuf portion of new block security (making sure someone
 * 	    else's data does not appear in your file across a system crash.)  For
 * 	    details, see the OT report, the postings to transarc.afs.episode, and
 * 	    the white paper on this subject.  Not done yet.  SNAPSHOT ONLY.
 * 	    Just wanted to fix a small nit in test_res.
 * 	    More work on NBS.  This is only a snapshot.
 * 	    Another snapshot; this time we make the interfaces between anode & buffer
 * 	    uniform (each was using a different convention).  Clean up names at the same
 * 	    time.  SNAPSHOT ONLY.
 * 	    Name cleanup.
 * 	    Another snapshot.  NBS now works in the forward direction for basic.test!
 * 	    This part of this delta was removed and added to delta
 * 	    mason-ot5070-gc-tran-in-logfull-avoidance
 * 	    Snapshot of code working in the forward path in mp.test.
 * 	    Removed the contents of revision 1.6
 * 	    Checkpoint.  No work done here, just made sure that the other new
 * 	    delta works OK (mason-ot5070-gc-tran-in-logfull-avoidance).
 * 	    Make all_tests work again.
 * 	    Finish making all the tests run now.
 * 	    Cleanup compilation on OSF/1.
 * 	    Continue work to get new block security going.
 * 	    Add a script for the nbs tests.
 * 	[1992/09/14  19:23:32  jaffe]
 * 
 * Revision 1.1.6.2  1992/08/31  18:33:08  jaffe
 * 	Transarc delta: mason-add-hp800-osi-routines 1.15
 * 	  Selected comments:
 * 	    This delta is misnamed - these are really porting fixes.
 * 	    Need to checkpoint to fix a different bug.
 * 	    Checkpoint the first system where test_anode works.  Only one change is not
 * 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
 * 	    is -lBSD.  This should be fixed more generally.
 * 	    More HP-UX changes.
 * 	    More HP-UX related changes.
 * 	    Fix compile problem on the RIOS.
 * 	    Fix more compilation problems.
 * 	    Checkpoint.
 * 	    Checpoint.
 * 	    In this snap we turned all malloc() calls into osi_Alloc calls.
 * 	    Also turned free calls into osi_Free.
 * 	    s/malloc/osi_Alloc/
 * 	    HP-UX EOF detection fix.
 * 	    EOF detection on ENOSPC not ENXIO/EINVAL/EIO.
 * 	    Another checkpoint.
 * 	    This is a checkpoint of the first working HP-UX Episode system.
 * 	    Needed to add the vnops routines specific to HP-UX.
 * 	    Fix compilation problems.
 * 	[1992/08/30  01:46:53  jaffe]
 * 
 * Revision 1.1.3.5  1992/05/05  20:40:08  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  20:16:04  jdp]
 * 
 * Revision 1.1.3.4  1992/01/25  20:45:51  zeliff
 * 	dfs6.3 merge, part2
 * 	[1992/01/25  20:02:43  zeliff]
 * 
 * $EndLog$
 */
/* Copyright (C) 1994, 1990 Transarc Corporation - All rights reserved. */

/* Routines to do basic/bootstrap disk initialization for Episode. */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>
#include <dcedfs/astab.h>

#include <com_err.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/logbuf.h>

/* user space I/O simulation */
#include <dcedfs/episode/us.h>

#include <anode.h>
#include <anode_block.h>


#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */


#ifdef DIOCGDINFO
/* define DKTYPENAMES to get the identifiers dktypenames[], DKMAXTYPES, fstypenames[], FSMAXTYPES */
/* #define DKTYPENAMES 1 */
#include <sys/disklabel.h>
#endif /* DIOCGDINFO */

#include <boot.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/anode/edbasic.c,v 1.1.26.1 1996/10/02 17:17:03 damon Exp $")

/* edsk_GetRawDevice -- Opens the raw device corresponding to the pathname
 *     \arg{dev}, returning its file descriptor, open for read, or (-1)
 *     indicating an error.  The full pathname of the raw device is returned in
 *     \arg{Path}.  The device number is returned in \arg{DevnumP}.
 */

EXPORT int edsk_GetRawDevice (dev, Path, DevnumP)
  IN char *dev;
  OUT char *Path;
  OUT dev_t *DevnumP;
{
    return edsk_GetDevice(dev, Path, DevnumP, NULL);

}

/* edsk_GetDevice -- Helper for edsk_GetRawDevice that may also be called
 *	directly by clients that want to know the name of the corresponding
 *	block device (vnops/efs_growaggr.c is such a client).  The full path
 *	to the block device is returned in \arg{BlockDevice} if it is non-null.
 */

EXPORT int edsk_GetDevice (dev, RawDevice, DevnumP, BlockDevice)
  IN char *dev;
  OUT char *RawDevice;
  OUT dev_t *DevnumP;
  OUT char *BlockDevice;
{
    char FullName[MAXPATHLEN + 1];
    char ErrMsg[MAXPATHLEN + sizeof "close()"];
    char *cp;
    int rfd;
    struct stat Stat;
    FILE *dfstab;

    FullName[0] = '\0';

    as_init();

    if ((dfstab = fopen(AstabFileName, "r")) == NULL && errno != ENOENT) {
	sprintf(ErrMsg, "open(%s)", AstabFileName);
	perror(ErrMsg);
	return (-1);
    }

    if (dfstab) {
	struct astab astab;
	struct ufs_astab ufstab;
	char buf[256];
	struct flock rlock;

	rlock.l_type = F_RDLCK;
	rlock.l_whence = rlock.l_start = rlock.l_len = 0;

	if (fcntl(fileno(dfstab), F_SETLK, &rlock) == -1) {
	    sprintf(ErrMsg, "lock(%s)", AstabFileName);
	    perror(ErrMsg);
	    return (-1);
	}	    

	while (as_getent(dfstab, buf, &astab, &ufstab)) {
	    if (strcmp(astab.as_aggrName, dev) == 0 ||
		strcmp(astab.as_spec, dev) == 0) {
		if (strcmp(ASTABTYPE_EPI, astab.as_type) != 0) {
		    fprintf(stderr,
			    "%s: wrong aggregate type--expecting %s, not %s",
			    astab.as_spec, ASTABTYPE_EPI, astab.as_type);
		    (void)fclose(dfstab);
		    return (-1);
		}
		(void)strcpy(FullName, astab.as_spec);
		break;
	    }
	}

	if (fclose(dfstab) == -1) {
	    sprintf(ErrMsg, "close(%s)", AstabFileName);
	    perror(ErrMsg);
	    return (-1);
	}
    }

    if (FullName[0] != '\0') {
	if (stat(FullName, &Stat) == -1) {
	   sprintf(ErrMsg, "stat(%s)", FullName);
	   perror(ErrMsg);
	   return -1;
       }
    } else if (ftu_GetFile(dev, FullName, &Stat) != 0) {
	return (-1);
    }

    if (S_ISBLK(Stat.st_mode)) {
	if (ftu_GetRawDeviceName(FullName, RawDevice) != 0)
	    return (-1);
	*DevnumP = Stat.st_rdev;
	if (BlockDevice)
	    (void) strcpy(BlockDevice, FullName);
    } else if (!S_ISCHR(Stat.st_mode) && !S_ISREG(Stat.st_mode)) {
	return (-1);
    } else {
	(void) strcpy(RawDevice, FullName);
	*DevnumP = Stat.st_dev;
    }

    rfd = open(RawDevice, O_RDONLY, 0);
    if (rfd < 0) {
	sprintf(ErrMsg, "open(%s)", RawDevice);
	perror(ErrMsg);
	return -1;
    }
    return (rfd);
}

static int doLongSeek(fd, fact1, fact2)
int fd;
unsigned long fact1, fact2;
{
    osi_off_t oBigAddr;
    osi_off_t result;
#ifdef AFS_SUNOS5_ENV
    extern offset_t llseek(int, offset_t, int);
#endif /* AFS_SUNOS5_ENV */

    oBigAddr = ((osi_off_t) fact1) * ((osi_off_t) fact2);
#ifdef AFS_SUNOS5_ENV
    result = llseek(fd, oBigAddr, SEEK_SET);
#else /* AFS_SUNOS5_ENV */
    result = lseek(fd, oBigAddr, SEEK_SET);
#endif /* AFS_SUNOS5_ENV */
    return ((result == -1) ? -1 : 0);
}

/* edsk_GuessDeviceCapacity -- Tries to determine the capacity of a partition
 *     opened for read on file descriptor \arg{rfd}.  The file name of the
 *     device, \arg{RawDev}, is used for diagnostic purposes.  The size is
 *     returned in units of \arg{blksize} which is also used as the
 *     granularity of the search.  A (-1) is returned if an error is
 *     encountered. */

EXPORT long edsk_GuessDeviceCapacity (rfd, RawDev, blksiz)
  IN int rfd;
  IN char *RawDev;
  IN long blksiz;
{
    char ErrMsg[500];
    unsigned long L, H, M;
    int High, Res, Size;
    char *Data;

    Size = blksiz;
    if (Size < EPISODE_DISKBUFFER_MINSIZE) Size = EPISODE_DISKBUFFER_MINSIZE;
    Data = (char *) osi_Alloc(Size);
    if (Data == NULL) return -1;
    L = 1;	/* needs to be > 0 so that the loop can terminate if H hits 0 */
#ifdef AFS_SUNOS5_ENV
    H = 0xfffffffe;
#else /* AFS_SUNOS5_ENV */
    H = (0xffffffff) / ((unsigned long) blksiz) - 1;
#endif /* AFS_SUNOS5_ENV */
    while (L <= H) {
	M = (L + H) / 2;
	High = 0;
	errno = 0;
	if (doLongSeek(rfd, M, blksiz) == -1) {
	    if (errno == EINVAL) High = 1;
	    else {
		sprintf(ErrMsg, "lseek(%lu) of %s", (M*blksiz), RawDev);
		perror(ErrMsg);
		osi_Free(Data, Size);
		return -1;
	    }
	} else {
	    errno = 0;
	    Res = read(rfd, Data, blksiz);
	    if (Res == blksiz) {
		High = -1;
	    } else if (Res >= 0 && Res < blksiz) {
		High = 1;
	    } else if ((Res > blksiz) || Res < 0) {
		/*
		 * Platform dependent test:
		 * - On HP/UX, reading a raw device with a buffer size that
		 *   is not a multiple of the block size (512) returns ENXIO.
		 * - On HP/UX, reading a raw device past EOF returns ENOSPC.
		 * - On other platforms, I do not know what errors are
		 *   possible; the present code, which checks for ENXIO,
		 *   EINVAL, and EIO, seems to work on the existing platforms.
		 * BWL 10/6/93
		 */
#ifdef AFS_HPUX_ENV
		if (errno == ENOSPC || errno == ENXIO)
#else /* AFS_HPUX_ENV */
		if (errno == ENXIO || errno == EINVAL || errno == EIO)
#endif /* AFS_HPUX_ENV */
		{
		    High = 1;
		} else {
		    sprintf(ErrMsg, "read() of %s, %lu for %d", RawDev, (M*blksiz), blksiz);
		    perror(ErrMsg);
		    osi_Free(Data, Size);
		    return -1;
		}

	    }
	}
	if (High > 0) {
	    H = M - 1;
	} else {
	    L = M + 1;
	}
    }
    osi_Free(Data, Size);
    return H;
}

#ifdef DIOCGDINFO
/* grab a type code */
#define	FS_TRANSARC_EPISODE	94
#endif /* DIOCGDINFO */

/* edsk_TryReadDiskLabel -- Read the label on the disk partition open on file
 *     descriptor \arg{rfd} into \arg{pdp}.  The expected filesystem
 *     type is given by \arg{OKFileSys}.  The file name of the device,
 *     \arg{RawDev}, is used only for diagnostic purposes.  Returns (-1) on
 *     error and zero on success. */

EXPORT int edsk_TryReadDiskLabel (rfd, RawDev, pdp, OKFileSys)
  IN int rfd;
  IN char *RawDev;
  OUT struct PartData *pdp;
  IN int OKFileSys;
{
    pdp->SectorSize = DEV_BSIZE;
#ifdef DIOCGDINFO
    return edsk_LabelTryReadDiskLabel(rfd, RawDev, pdp, OKFileSys);
#else /* DIOCGDINFO */
    return -1;
#endif /* DIOCGDINFO */
}

/* edsk_RewriteDiskLabel -- Rewrites the label on the disk open on file
 *     descriptor \arg{rfd} to that specified in \arg{pdp} so that it
 *     looks like an Episode file system.  The file name of the device,
 *     \arg{RawDev}, is used only for diagnostic purposes.
 *
 *     Yes, this is an open-for-read file descriptor.  I don't know why this
 *     works.
 *
 *     Returns (-1) on error and zero on success. */

EXPORT int edsk_RewriteDiskLabel (rfd, RawDev, pdp)
  IN int rfd;
  IN char *RawDev;
  IN struct PartData *pdp;
{
#ifdef DIOCGDINFO
    return edsk_LabelRewriteDiskLabel (rfd, RawDev, pdp);
#else /* DIOCGDINFO */
    return -1;
#endif /* DIOCGDINFO */
}

/* edsk_DumpPartData -- Prints the values of the fields in \arg{pdp} to stdout.
 */

EXPORT void edsk_DumpPartData (pdp)
  IN struct PartData *pdp;
{
    int num;

    num = 0;
    if (pdp->SectorSize != 0) {++num; printf(" SectorSize %ld;", pdp->SectorSize);}
    if (pdp->SectorsPerTrack != 0) {++num; printf(" SectorsPerTrack %ld;", pdp->SectorsPerTrack);}
    if (pdp->TracksPerCylinder != 0) {++num; printf(" TracksPerCylinder %ld;", pdp->TracksPerCylinder);}
    if (num > 2) {printf("\n"); num = 0;}
    if (pdp->TotalCylindersInDisk != 0) {++num; printf(" TotalCylindersInDisk %ld;", pdp->TotalCylindersInDisk);}
    if (num > 2) {printf("\n"); num = 0;}
    if (pdp->TotalCylindersInPartition != 0) {++num; printf(" TotalCylindersInPartition %ld;", pdp->TotalCylindersInPartition);}
    if (num > 2) {printf("\n"); num = 0;}
    if (pdp->SectorsPerCylinder != 0) {++num; printf(" SectorsPerCylinder %ld;", pdp->SectorsPerCylinder);}
    if (num > 2) {printf("\n"); num = 0;}
    if (pdp->Labelled != 0) {++num; printf(" Labelled %ld;", pdp->Labelled);}
    if (num > 2) {printf("\n"); num = 0;}
    if (pdp->TotalSectors != 0) {++num; printf(" TotalSectors %ld;", pdp->TotalSectors);}
    if (num > 2) {printf("\n"); num = 0;}
    if (pdp->TotalBlocks != 0) {++num; printf(" TotalBlocks %ld;", pdp->TotalBlocks);}
    if (num > 2) {printf("\n"); num = 0;}
    if (pdp->SpareSectorsPerTrack != 0) {++num; printf(" SpareSectorsPerTrack %ld;", pdp->SpareSectorsPerTrack);}
    if (num > 2) {printf("\n"); num = 0;}
    if (pdp->SpareSectorsPerCylinder != 0) {++num; printf(" SpareSectorsPerCylinder %ld;", pdp->SpareSectorsPerCylinder);}
    if (num > 2) {printf("\n"); num = 0;}
    if (pdp->BlockSize != 0) {++num; printf(" BlockSize %ld;", pdp->BlockSize);}
    if (num > 2) {printf("\n"); num = 0;}
    if (pdp->FragmentSize != 0) {++num; printf(" FragmentSize %ld;", pdp->FragmentSize);}
    if (num > 2) {printf("\n"); num = 0;}
    if (pdp->FirstBlock != 0) {++num; printf(" FirstBlock %ld;", pdp->FirstBlock);}
    if (num > 2) {printf("\n"); num = 0;}
    if (pdp->NLogBlocks != 0) {++num; printf(" NLogBlocks %ld;", pdp->NLogBlocks);}
    if (num > 2) {printf("\n"); num = 0;}
    if (pdp->NumBigChunks != 0) {++num; printf(" NumBigChunks %ld;", pdp->NumBigChunks);}
    if (num > 2) {printf("\n"); num = 0;}
    if (pdp->minBlkSize != 0) {++num; printf(" minBlkSize %ld;", pdp->minBlkSize);}
    if (num > 2) {printf("\n"); num = 0;}
    if (pdp->minBlkCount != 0) {++num; printf(" minBlkCount %ld;", pdp->minBlkCount);}
    if (num > 2) {printf("\n"); num = 0;}
    if (pdp->FileSysCreateTime != 0) {++num; printf(" FileSysCreateTime %lu;", pdp->FileSysCreateTime);}
    if (num > 0) {printf("\n"); num = 0;}
    printf(" FileSysClean %ld; FileSysEmpty %ld; FileSysMountedAs ``%s''\n",
	    pdp->FileSysClean, pdp->FileSysEmpty, pdp->FileSysMountedAs);
}
