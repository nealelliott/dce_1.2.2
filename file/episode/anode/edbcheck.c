/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: edbcheck.c,v $
 * Revision 1.1.23.1  1996/10/02  17:17:05  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:27:01  damon]
 *
 * Revision 1.1.17.3  1994/07/13  22:21:33  devsrc
 * 	merged with bl-10
 * 	[1994/06/28  20:31:45  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:55:57  mbs]
 * 
 * Revision 1.1.17.2  1994/06/09  13:57:22  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:32:25  annie]
 * 
 * Revision 1.1.17.1  1994/02/04  20:11:03  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:09:00  devsrc]
 * 
 * Revision 1.1.15.2  1994/01/28  16:50:30  kinney
 * 	for HPUX: do better checking for BSD file system
 * 	[1994/01/28  16:49:53  kinney]
 * 
 * Revision 1.1.15.1  1993/12/07  17:16:51  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:39:10  jaffe]
 * 
 * Revision 1.1.5.5  1993/01/18  22:02:58  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  21:42:26  cjd]
 * 
 * Revision 1.1.5.4  1992/11/24  16:19:37  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:17:31  bolinger]
 * 
 * Revision 1.1.5.3  1992/10/13  18:10:58  marty
 * 	OSC1.1.1 upgrade.  Changes are osc1.0.4 compatible
 * 	[1992/10/13  17:10:09  marty]
 * 
 * Revision 1.1.6.2  1992/10/06  22:05:41  garyf
 * 	use POSIX for SEEK_SET
 * 	[1992/09/29  20:06:13  garyf]
 * 
 * Revision 1.1.5.2  1992/08/31  18:33:19  jaffe
 * 	fixed an RCSID line.
 * 	[1992/08/30  13:02:18  jaffe]
 * 
 * 	Transarc delta: mason-add-hp800-osi-routines 1.15
 * 	  Selected comments:
 * 	    This delta is misnamed - these are really porting fixes.
 * 	    Need to checkpoint to fix a different bug.
 * 	    Changes for HP-UX.
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
 * 	    HP-UX EOF detection fix.
 * 	    Another checkpoint.
 * 	    This is a checkpoint of the first working HP-UX Episode system.
 * 	    Needed to add the vnops routines specific to HP-UX.
 * 	    Fix compilation problems.
 * 	[1992/08/30  01:47:25  jaffe]
 * 
 * Revision 1.1.2.3  1992/05/13  21:54:50  jdp
 * 	Transarc delta: cburnett-ot2635-anode_dir_changes_for_aix32 1.1
 * 	  Files modified:
 * 	    anode: anode.c, critical.h, edbcheck.c, fixed_anode.c
 * 	  Selected comments:
 * 	    changes for ansi problems when building on AIX 3.2
 * 	    ansi changes
 * 	Transarc delta: cburnett-ot3138-newaggr_fails_on_aix 1.1
 * 	  Files modified:
 * 	    anode: edbcheck.c
 * 	  Selected comments:
 * 	    newaggr fails on AIX with a block size of 4k due to fsync
 * 	    ifdefed out fsync call for AIX.
 * 	[1992/05/13  19:40:44  jdp]
 * 
 * Revision 1.1.2.2  1992/05/05  20:40:19  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	delta junk-jfs-sb 1.1
 * 	[1992/05/04  15:26:59  jdp]
 * 
 * Revision 1.1  1992/01/19  02:37:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* Copyright (C) 1993, 1990 Transarc Corporation - All rights reserved. */

/* Program to scavenge a disk partition or maybe lots of them. */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#ifdef DIOCGDINFO
/* define DKTYPENAMES to get the identifiers dktypenames[], DKMAXTYPES, fstypenames[], FSMAXTYPES */
/* #define DKTYPENAMES 1 */
#include <sys/disklabel.h>
#endif /* DIOCGDINFO */
#include <boot.h>

#if defined(AFS_AIX31_ENV)
#include <sys/filsys.h>
#elif defined(AFS_SUNOS5_ENV)
#include <sys/fs/ufs_fs.h>
#endif

#ifdef AFS_HPUX_ENV
#include <sys/fs.h>
#endif

#ifdef	AFS_AIX31_ENV
typedef struct superblock sblock_t;
#define	fbtob(fs) ((fs) << (12))
#define SBLOCK_OFFSET	fbtob((daddr_t)SUPER_B)
#define FS_NAME		"AIX3"
#else /* AFS_AIX31_ENV */
typedef struct fs sblock_t;
#define SBLOCK_OFFSET	(SBLOCK * pdp->SectorSize)
#define FS_NAME		"BSD"
#endif /* AFS_AIX31_ENV */

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/anode/edbcheck.c,v 1.1.23.1 1996/10/02 17:17:05 damon Exp $")

/* edsk_CheckAsBSD -- Checks whether the given device looks like a BSD file
 *     system, if so \arg{pdp} is filled in from the superblock.
 *
 *     The return code indicates how much the partition looked like the
 *     expected file system.  It will return one of:
 *         edsk_NOWAY -- (0) meaning file system was not as expected.
 *	   edsk_UNKNOWN -- (1) something strange went wrong.
 *	   edsk_EMPTY -- (2) BSD file system is empty.
 *	   edsk_FULL -- (3) meaning every thing looked fine, but the file
 *	       system has not free space.
 *
 *     This function attempts to understand the operating system's native file
 *     systems.  For AIX this means JFS and for other systems this will usually
 *     refer to the local BSD FFS variant.  Because of this the implementation
 *     is highly non-portable.
 *
 * PARAMETERS --
 *     rfd -- file descriptor of raw disk device, opened for read.
 *     RawDev -- file name of disk device.  Used for diagnostic purposes.
 *     pdp -- information about partition. */

EXPORT int edsk_CheckAsBSD (rfd, RawDev, pdp)
  IN int rfd;
  IN char *RawDev;
  OUT struct PartData *pdp;
{
    union {
	sblock_t FS;	/* attempt to read it here */
	char dum[SBSIZE];
    } Super;
    char ErrMsg[500];
    long Res;

#ifdef DIOCGDINFO
    Res = edsk_TryReadDiskLabel(rfd, RawDev, pdp, FS_BSDFFS);
#else /* DIOCGDINFO */
    Res = edsk_TryReadDiskLabel(rfd, RawDev, pdp, 0);
#endif /* DIOCGDINFO */

    Res = lseek(rfd, (long) SBLOCK_OFFSET, SEEK_SET);
    if (Res == -1) {
	sprintf(ErrMsg, "%s: cannot seek to address %d to read %s superblock",
		FS_NAME, RawDev, SBLOCK_OFFSET);
	perror(ErrMsg);
	return edsk_UNKNOWN;
    }
    Res = read(rfd, Super.dum, (long) SBSIZE);
    if (Res != SBSIZE) {
	sprintf(ErrMsg, "%s: cannot read local file system's superblock", RawDev);
	perror(ErrMsg);
	return edsk_UNKNOWN;
    }
#ifdef	AFS_AIX31_ENV
    /*
     * XXX Not too much checking on the JFS for now XXX
     */
    if (strncmp(Super.FS.s_magic, fsv3magic, strlen(fsv3magic)) != 0)
	return edsk_NOWAY;
    if (Super.FS.s_fsize > FS_MAXSIZE) return edsk_NOWAY;
    if (Super.FS.s_agsize == 0) return edsk_NOWAY;
    if (Super.FS.s_fsize == 0) return edsk_NOWAY;
    if (Super.FS.s_bsize == 0) return edsk_NOWAY;
    if (Super.FS.s_fmod > 8) return edsk_NOWAY;
    /*
     * XXX Don't feel any of the AIX's JFS fields yet since most are different XX
     */
    pdp->FileSysCreateTime = Super.FS.s_time;
    pdp->FileSysClean = ((Super.FS.s_fmod == FM_CLEAN)
			  || (Super.FS.s_fmod == FM_MOUNT));
    strncpy(pdp->FileSysMountedAs, Super.FS.s_fname, sizeof(Super.FS.s_fname));
    pdp->BlockSize = Super.FS.s_bsize;
    pdp->FragmentSize = 512;
    pdp->TotalBlocks = Super.FS.s_fsize;
#else /* AFS_AIX31_ENV */

#ifdef AFS_HPUX_ENV
    if ((Super.FS.fs_magic != FS_MAGIC) &&
	(Super.FS.fs_magic != FS_MAGIC_LFN) &&
	(Super.FS.fs_magic != FD_FSMAGIC))
	return edsk_NOWAY;
#else
    if (Super.FS.fs_magic != FS_MAGIC) return edsk_NOWAY;
#endif
    if (Super.FS.fs_ncg < 1 || Super.FS.fs_cpg < 1) return edsk_NOWAY;
    if (Super.FS.fs_ncg * Super.FS.fs_cpg < Super.FS.fs_ncyl) return edsk_NOWAY;
    if ((Super.FS.fs_ncg - 1) * Super.FS.fs_cpg >= Super.FS.fs_ncyl) return edsk_NOWAY;
    if (Super.FS.fs_sbsize > SBSIZE) return edsk_NOWAY;
    if (Super.FS.fs_optim != FS_OPTTIME && Super.FS.fs_optim != FS_OPTSPACE)
	return edsk_NOWAY;
    if (Super.FS.fs_minfree < 0 || Super.FS.fs_minfree > 99) return edsk_NOWAY;
#ifdef DIOCGDINFO
    if (Super.FS.fs_interleave < 1 || Super.FS.fs_npsect < Super.FS.fs_nsect)
	return edsk_NOWAY;
#endif /* DIOCGDINFO */
    if (Super.FS.fs_bsize <= 0 || Super.FS.fs_fsize <= 0) return edsk_NOWAY;
    if ((Super.FS.fs_bsize < Super.FS.fs_fsize) || ((Super.FS.fs_frag * Super.FS.fs_fsize) != Super.FS.fs_bsize))
	return edsk_NOWAY;
    if (Super.FS.fs_fsbtodb > 20 || Super.FS.fs_fsbtodb < -10) return edsk_NOWAY;

    pdp->FileSysCreateTime = Super.FS.fs_time;
#ifdef FS_CLEAN
    pdp->FileSysClean = (Super.FS.fs_clean == FS_CLEAN);
#else /* FS_CLEAN */
    pdp->FileSysClean = (Super.FS.fs_clean != 0);
#endif /* FS_CLEAN */
    strncpy(pdp->FileSysMountedAs, Super.FS.fs_fsmnt, sizeof(pdp->FileSysMountedAs));
    pdp->BlockSize = Super.FS.fs_bsize;
    pdp->FragmentSize = Super.FS.fs_fsize;
    pdp->SectorsPerTrack = Super.FS.fs_nsect;
    pdp->TracksPerCylinder = Super.FS.fs_ntrak;
    pdp->TotalCylindersInPartition = Super.FS.fs_ncyl;
    pdp->SectorsPerCylinder = Super.FS.fs_spc;
#ifdef DIOCGDINFO
    pdp->SpareSectorsPerTrack = Super.FS.fs_npsect - Super.FS.fs_nsect;
#endif /* DIOCGDINFO */
    pdp->SpareSectorsPerCylinder = (Super.FS.fs_nsect * Super.FS.fs_ntrak) - Super.FS.fs_spc;
    pdp->SectorSize = Super.FS.fs_fsize >> Super.FS.fs_fsbtodb;
    pdp->TotalBlocks = Super.FS.fs_size;
/*    printf("%s: tot inodes %lu, %lu in use\n",
	      RawDev, Super.FS.fs_ncg * Super.FS.fs_ipg,
	      (Super.FS.fs_ncg * Super.FS.fs_ipg) - (Super.FS.fs_cstotal.cs_nifree)); */
/*    printf("%s: tot blks %lu, %lu in use\n",
	      RawDev, Super.FS.fs_dsize,
	      Super.FS.fs_dsize - ((Super.FS.fs_cstotal.cs_nbfree * Super.FS.fs_frag) + Super.FS.fs_cstotal.cs_nffree)); */
/* Make a guess as to whether the file system has any data on it */
/* Assume that an empty file system uses fewer than 5 inodes and fewer than 4 blocks */
    pdp->FileSysEmpty = (((Super.FS.fs_ncg * Super.FS.fs_ipg) - (Super.FS.fs_cstotal.cs_nifree) < 5)
			  &&
			  ((Super.FS.fs_dsize - ((Super.FS.fs_cstotal.cs_nbfree * Super.FS.fs_frag) + Super.FS.fs_cstotal.cs_nffree))
			      < 4*Super.FS.fs_frag));
#endif	/* AFS_AIX31_ENV */

#if 0
    Res = edsk_GuessDeviceCapacity(rfd, RawDev, pdp->FragmentSize);
    if (Res == 0 || Res == -1) {
	printf("%s: empty partition\n", RawDev);
    } else {
	++Res;
	Res *= pdp->FragmentSize;
	printf("%s: %lu (%#lx) %u-byte sectors.\n", RawDev, Res/pdp->SectorSize,
	       Res/pdp->SectorSize, pdp->SectorSize);
    }
#endif /* 0 */

    if (pdp->FileSysEmpty) return edsk_EMPTY;
    else return edsk_FULL;
}

/* edsk_MarkAsNotBSD -- Changes the label on the partition to be not BSD by
 *     overwriting key attributes of the local operating system's native file
 *     system.  The raw device holding the partition is opened for write on
 *     file descriptor \argname{wfd}.  The file name of the device,
 *     \argname{RawDev}, is used only for diagnostic purposes.  The previously
 *     obtained partition data contained in \argname{pdp} is used to locate the
 *     superblock to bash. Its block size is cleared to indicate that the
 *     corresponding partition is now invalid.
 *
 *     The return value is zero if everything went okay, greater than zero if
 *     the partition had already been marked, and negative on any error.
 *
 *     This is another highly non-portable routine that knows about the
 *     structure of the local file system. */

EXPORT int edsk_MarkAsNotBSD (wfd, RawDev, pdp)
  IN int wfd;
  IN char *RawDev;
  INOUT struct PartData *pdp;
{
    union {
	sblock_t FS;	/* attempt to read it here */
	char dum[SBSIZE];
    } Super;
    char ErrMsg[500];
    long Res;

    Res = lseek(wfd, (long) SBLOCK_OFFSET, SEEK_SET);
    if (Res == -1) {
	sprintf(ErrMsg, "%s: cannot seek to address %d to read %s superblock",
		FS_NAME, RawDev, SBLOCK_OFFSET);
	perror(ErrMsg);
	return -1;
    }
    Res = read(wfd, Super.dum, (long) SBSIZE);
    if (Res != SBSIZE) {
	sprintf(ErrMsg, "%s: cannot read local file system's superblock", RawDev);
	perror(ErrMsg);
	return -1;
    }
#ifdef	AFS_AIX31_ENV
    if (strncmp(Super.FS.s_magic, fsv3magic, strlen(fsv3magic)) != 0 &&
	 Super.FS.s_type == 0xf1 &&
	 Super.FS.s_fmod == 99 &&
	 Super.FS.s_agsize == 0 &&
	 Super.FS.s_fsize == 0 &&
	 Super.FS.s_bsize == 0 &&
	 Super.FS.s_fname[0] == 'E')
	return 1;	/* looks like it was already trashed */
/* OK, now, trash some bits. */
    bcopy("EpFS", Super.FS.s_magic, strlen(fsv3magic));
    Super.FS.s_type = 0xf1;
    Super.FS.s_fmod = 99;
    Super.FS.s_agsize = 0;
    Super.FS.s_fsize = 0;
    Super.FS.s_bsize = 0;
    Super.FS.s_logdev = 0;
    bcopy("Episod", Super.FS.s_fname, sizeof(Super.FS.s_fname));
    bcopy("Episod", Super.FS.s_fpack, sizeof(Super.FS.s_fpack));
#else /* AFS_AIX31_ENV */
    if (Super.FS.fs_magic != FS_MAGIC &&
	 Super.FS.fs_fsmnt[0] == '-' &&
	 Super.FS.fs_ncg == 0 &&
	 Super.FS.fs_cpg == 0 &&
	 Super.FS.fs_bsize == 0 &&
	 Super.FS.fs_frag == 0)
	return 1;	/* Looks like it was already done */
/* OK, now, trash some bits. */
    Super.FS.fs_magic = ((~(FS_MAGIC)) & ~(0xfff)) | 0xcfe;
    Super.FS.fs_ncg = 0;
    Super.FS.fs_cpg = 0;
    Super.FS.fs_bsize = 0;
    Super.FS.fs_frag = 0;
    Super.FS.fs_time = time(0);
    strncpy(Super.FS.fs_fsmnt, "---now Episode---", sizeof(Super.FS.fs_fsmnt));
    pdp->BlockSize = Super.FS.fs_bsize;
#endif /* AFS_AIX31_ENV */
    Res = lseek(wfd, (long) SBLOCK_OFFSET, SEEK_SET);
    if (Res == -1) {
	sprintf(ErrMsg, "%s: cannot seek to address %d to write %s superblock",
		FS_NAME, RawDev, SBLOCK_OFFSET);
	perror(ErrMsg);
	return -1;
    }
    Res = write(wfd, Super.dum, (long) SBSIZE);
    if (Res != SBSIZE) {
	sprintf(ErrMsg, "%s: cannot overwrite local filesystem's superblock", RawDev);
	perror(ErrMsg);
	return -1;
    }
#ifndef	AFS_AIX31_ENV		/* XXXXXX FIX THIS XXXXXX */
    Res = fsync(wfd);
    if (Res != 0) {
	sprintf(ErrMsg, "%s: cannot fsync after overwriting lfs superblock", RawDev);
	perror(ErrMsg);
	return -1;
    }
#endif /* AFS_AIX31_ENV */
    return 0;
}
