/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: aggregate.h,v $
 * Revision 1.1.98.1  1996/10/02  17:15:59  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:07:04  damon]
 *
 * Revision 1.1.92.2  1994/06/09  13:56:22  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:31:02  annie]
 * 
 * Revision 1.1.92.1  1994/02/04  20:10:23  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:08:38  devsrc]
 * 
 * Revision 1.1.90.1  1993/12/07  17:16:10  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:33:57  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/18  20:54:36  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  17:41:29  cjd]
 * 
 * Revision 1.1.5.3  1992/11/24  16:18:24  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:16:38  bolinger]
 * 
 * Revision 1.1.5.2  1992/08/31  18:18:46  jaffe
 * 	Transarc delta: jaffe-fix-compilation-errors-dfs-102-1.46 1.1
 * 	  Selected comments:
 * 	    fix compilation warnings.
 * 	    include volume.h for the definition of epiv_status
 * 	Transarc delta: jdp-ot4179-lfs-encapsulate-initialization 1.3
 * 	  Selected comments:
 * 	    This delta simplifies Episode user-space initialization.  There are two parts
 * 	    to this change.  The first simplifies the process of calling the *_Init
 * 	    routines.  Previously, you typically had to call dirb_Init(), N anode init
 * 	    routines, and logbuf_Init().  With this change, you need only specify the
 * 	    initialization parameters to the necessary layers in one structure and call
 * 	    the top-most init routine.  Since each initialization routine is now
 * 	    responsible for calling the lower layers' initialization routines, only
 * 	    one call is necessary.  The second change made in this delta was the
 * 	    encapsulation of the user-space device initialization code in one routine,
 * 	    us_InitDevice().  This routine is declared in tools/us.h and defined in
 * 	    tools/us_io.c.
 * 	    Changed signature of epig_Init().
 * 	    Fix a couple of remaining compilation problems.
 * 	    Fix compilation problems on OSF/1.
 * 	[1992/08/30  01:44:25  jaffe]
 * 
 * Revision 1.1.3.4  1992/05/05  20:37:22  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  20:04:23  jdp]
 * 
 * Revision 1.1.3.3  1992/01/24  02:40:13  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:07:13  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:11:10  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:25:31  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:10:42  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
*/
/* This file contains the external interface to the aggregate module. */


/* Copyright (C) 1993, 1990 Transarc Corporation - All rights reserved */

#ifndef TRANSARC_EPI_AGGREGATE_H
#define TRANSARC_EPI_AGGREGATE_H

#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/volume.h>	/* for struct epiv_status */
#include <dcedfs/episode/volume_table.h>

#define EPIG_NOATTACH 0x01		/* don't attach newly created volume */

/* program creating */
#define EPIG_CREATOR_TEST_ANODE	    1
#define EPIG_CREATOR_TEST_VNODEOPS  2
#define EPIG_CREATOR_NEWAGGR	    3
#define epig_ProgramName(c) \
    ((c == EPIG_CREATOR_TEST_ANODE) ? "test_anode" : \
     ((c == EPIG_CREATOR_TEST_VNODEOPS) ? "test_vnodeops" : \
      ((c == EPIG_CREATOR_NEWAGGR) ? "newaggr" : \
       "UNKNOWN PROGRAM")))

/* disk format version number */
#define EPIG_DISK_MAJOR_VERSION     1	/* inc'd every incompatible change */
#define EPIG_DISK_MINOR_VERSION     4	/* inc'd on upward compatible change */
#define EPIG_OLD_MINOR_TIMESTAMPS   3	/* prior to AVL & logpage timestamps */
#define EPIG_OLD_MINOR_VISIBLE_Q    2	/* prior to visible quota chgs */
#define EPIG_OLD_MINOR_FIDINDEX	    1	/* prior to fid index mapping */

struct epi_aggregate_status {
    daddr_t whichSuperBlockUsed;
    u_long blockSize;
    u_long fragmentSize;
    u_long firstBlock;			/* first device block usable */
    u_long nBlocks;			/* nBlocks in allocation pool */
    u_long nLogBlocks;			/* size of log */
    u_long reservedIndexes;		/* for now this is AVL_INDEX */
    u_long bitmapIndex, logIndex;	/* indexes for these containers */
    u_long badblocksIndex;
    u_long timeCreated;
    u_long timeInited;
    u_long deviceMajor, deviceMinor;
    u_long diskFlags;
    u_long coreFlags;
    u_long creatingProgram;		/* program that created the aggr */
    u_long diskFormatMajorVersion;	/* disk format major version */
    u_long diskFormatMinorVersion;	/*  minor  */
    u_long fragsPerBlock;		/* number of fragments in a block */
    u_long sparel1;			/* high word of ff */
    u_long fragmentsFree;		/* # free fragments */
    u_long fragmentsFreeReally;		/* # frags free for admin ops */
    u_long fragmentsFreePartial;	/* # frags free in partial blocks */
					/* fragmentation = partial/free */
    u_long lastVerifyTime;		/* time of last verify/salvage */
    u_int lastVerifyCode;		/* last verify/salvage exit status */
    long spare[8];			/* copied directly from the disk hdr */
};
#define epig_status epi_aggregate_status

struct epig_params {
    u_long maxHandles;			/* # anode handles to preallocate */
    long spare[7];
    struct elbb_params elbbParams;
};

extern long epig_Init _TAKES((
  IN struct epig_params *initParams));

extern long epig_CreateAggregate _TAKES((
  IN struct async_device *device,
  IN u_long fragmentSize,		/* fragment size for this aggregate */
  IN daddr_t firstBlock,		/* first usable block in device */
  IN daddr_t superBlock,		/* location of primary superblock */
  IN u_long nBlocks,			/* number of blocks to use */
  IN u_long nLogBlocks,			/* size of log */
  IN u_int nExceptions,
  IN daddr_t *exceptions));		/* blocks to exclude from allocation */

extern long epig_GrowAggregate(
  IN epi_anode_t avl,
  IN u_long blocks,                     /* desired new aggregate size */
  IN u_int nExceptions,
  IN daddr_t exceptions[]		/* blocks to exclude from allocation */
);

extern long epig_InitAggregate _TAKES((
  IN int maj,				/* device numbers */
  IN int min,
  IN int recvrLog,
  OUT struct async_device **deviceP,	/* device pointer */
  OUT epi_anode_t *handleP));		/* avl */

extern long epig_GetAggregateStatus _TAKES((
  IN epi_anode_t avl,
  INOUT struct epig_status *StatusP));

extern long epig_GetFragmentSize _TAKES((
  IN epi_anode_t avl,
  OUT u_long *fragmentSizeP));

extern int epig_SayWhyNotLikeSuperBlock _TAKES((
  IN struct async_device *dev,		/* device ptr or zero */
  IN opaque dataptr,			/* ptr to superblock data */
  IN int len,				/* length of buffer */
  OUT char *buf));			/* printable reason */

extern long epig_CloseAggregate _TAKES((
  IN epi_anode_t avl));

extern long epig_CreateVolume _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_anode_t avl,
  IN long flags,
  IN struct epiv_status *status,
  OUT u_long *indexP));

extern long epig_OpenVolume _TAKES((
  IN epi_anode_t avl,
  IN u_long index,
  OUT epi_anode_t *volP));

extern long epig_CloseVolume _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_anode_t volC));

extern long epig_DeleteVolume(
  IN elbb_tranRec_t trans,
  IN epi_volume_t volC
);

#define epig_LastIndex(volC, indexP) epit_LastIndex(volC, indexP)

/* testing only */

extern long epig_AttachAggregate _TAKES((
  IN epi_anode_t avl));
extern long epig_DetachAggregate _TAKES((
  IN epi_anode_t avl));

#endif
