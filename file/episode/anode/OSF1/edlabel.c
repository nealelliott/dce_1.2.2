/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: edlabel.c,v $
 * Revision 1.1.6.1  1996/10/02  17:15:45  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:07:01  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1994, 1990 Transarc Corporation - All rights reserved. */

/* Routines to do basic/bootstrap disk initialization for Episode. */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

#include <com_err.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/logbuf.h>

/* user space I/O simulation */
#include <dcedfs/episode/us.h>

#include "anode.h"
#include "anode_block.h"


#if !defined(KERNEL)
#include <pthread.h>
#endif /* !KERNEL */


#ifdef DIOCGDINFO
/* define DKTYPENAMES to get the identifiers dktypenames[], DKMAXTYPES, fstypenames[], FSMAXTYPES */
/* #define DKTYPENAMES 1 */
#include <sys/disklabel.h>
#endif /* DIOCGDINFO */

#include "boot.h"

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/anode/OSF1/edlabel.c,v 1.1.6.1 1996/10/02 17:15:45 damon Exp $")

#ifdef DIOCGDINFO
/* grab a type code */
#define	FS_TRANSARC_EPISODE	94
#endif /* DIOCGDINFO */

/* edsk_LabelTryReadDiskLabel -- Read the label on the disk partition open on file
 *     descriptor \arg{rfd} into \arg{pdp}.  The expected filesystem
 *     type is given by \arg{OKFileSys}.  The file name of the device,
 *     \arg{RawDev}, is used only for diagnostic purposes.  Returns (-1) on
 *     error and zero on success.
 */
EXPORT int edsk_LabelTryReadDiskLabel (rfd, RawDev, pdp, OKFileSys)
  IN int rfd;
  IN char *RawDev;
  OUT struct PartData *pdp;
  IN int OKFileSys;
{
#ifdef DIOCGDINFO
    struct disklabel lbl;
    struct partition *pptr;
    char c;
    int Prt;
    char ErrMsg[500];

    c = RawDev[strlen(RawDev)-1];
    if (isdigit(c) || (c >= 'a' && c <= 'h')) {
	if (ioctl(rfd, DIOCGDINFO, (char *)&lbl) < 0) {
	    sprintf(ErrMsg, "get-disk-label(%s)", RawDev);
	    perror(ErrMsg);
	    return -1;
	} else {
	    pdp->SectorSize = lbl.d_secsize;
	    pdp->SectorsPerTrack = lbl.d_nsectors;
	    pdp->TracksPerCylinder = lbl.d_ntracks;
	    pdp->SectorsPerCylinder = lbl.d_secpercyl;
	    pdp->TotalCylindersInDisk = lbl.d_ncylinders;
	    pdp->SpareSectorsPerTrack = lbl.d_sparespertrack;
	    pdp->SpareSectorsPerCylinder = lbl.d_sparespercyl;
	    pdp->Labelled = 1;
	    /* Now check the partition we've been handed. */
	    Prt = (isdigit(c) ? 0 : (c - 'a'));
	    pptr = &lab->d_partitions[Prt];
	    if (Prt < lab->d_npartitions) {
		if (pptr->p_fstype == FS_BSDFFS && pptr->p_fstype == OKFileSys) {
		    pdp->FragmentSize = pptr->p_fsize;
		    pdp->BlockSize = pptr->p_frag * pptr->p_fsize;
		    pdp->TotalSectors = pptr->p_size;
		    pdp->Labelled = 2;
		    if (lbl.d_secsize != 0 && (pptr->p_fsize / lbl.d_secsize) != 0) {
			pdp->TotalBlocks = pptr->p_size / (pptr->p_fsize / lbl.d_secsize);
			pdp->Labelled = 3;
		    }
		} else if (pptr->p_fstype == FS_TRANSARC_EPISODE && pptr->p_fstype == OKFileSys) {
		    pdp->FragmentSize = pptr->p_fsize;
		    pdp->BlockSize = pptr->p_frag * pptr->p_fsize;
		    pdp->TotalSectors = pptr->p_size;
		    pdp->Labelled = 2;
		    if (lbl.d_secsize != 0 && (pdp->BlockSize / lbl.d_secsize) != 0) {
			pdp->TotalBlocks = pptr->p_size / (pdp->BlockSize / lbl.d_secsize);
			pdp->Labelled = 3;
		    }
		}
	    }
	}
    }
    return 0;
#else /* DIOCGDINFO */
    return -1;
#endif /* DIOCGDINFO */
}

/* edsk_LabelRewriteDiskLabel -- Rewrites the label on the disk open on file
 *     descriptor \arg{rfd} to that specified in \arg{pdp} so that it
 *     looks like an Episode file system.  The file name of the device,
 *     \arg{RawDev}, is used only for diagnostic purposes.
 *
 *     Yes, this is an open-for-read file descriptor.  I don't know why this
 *     works.
 *
 *     Returns (-1) on error and zero on success.
 */
EXPORT int edsk_LabelRewriteDiskLabel (rfd, RawDev, pdp)
  IN int rfd;
  IN char *RawDev;
  IN struct PartData *pdp;
{
#ifdef DIOCGDINFO
    struct disklabel lbl;
    struct partition *pptr;
    char c;
    int Prt;
    char ErrMsg[500];

    c = RawDev[strlen(RawDev)-1];
    if (isdigit(c) || (c >= 'a' && c <= 'h')) {
	if (ioctl(rfd, DIOCGDINFO, (char *)&lbl) < 0) {
	    sprintf(ErrMsg, "get-disk-label(%s)", RawDev);
	    perror(ErrMsg);
	    return -1;
	} else {
	    /* Check the partition we've been handed. */
	    Prt = (isdigit(c) ? 0 : (c - 'a'));
	    pptr = &lab->d_partitions[Prt];
	    if (Prt < lab->d_npartitions) {
		pptr->p_fsize = pdp->FragmentSize;
		if (pdp->FragmentSize != 0) {
		    pptr->p_frag = pdp->BlockSize / pdp->FragmentSize;
		} else {
		    fprintf(stderr, "Bad FragmentSize of zero!\n");
		    return -1;
		}
		pptr->p_fstype = FS_TRANSARC_EPISODE;
		if (ioctl(rfd, DIOCWDINFO, (char *)&lbl) < 0) {
		    sprintf(ErrMsg, "write-disk-label(%s)", RawDev);
		    perror(ErrMsg);
		    return -1;
		}
	    }
	}
    }
    return 0;
#else /* DIOCGDINFO */
    return -1;
#endif /* DIOCGDINFO */
}
