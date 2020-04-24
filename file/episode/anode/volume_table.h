/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: volume_table.h,v $
 * Revision 1.1.13.1  1996/10/02  17:19:58  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:29:19  damon]
 *
 * Revision 1.1.7.1  1994/06/09  13:59:35  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:34:54  annie]
 * 
 * Revision 1.1.5.4  1993/01/28  16:42:35  shl
 * 	Transarc delta: ota-ot3802-deflate-needs-tlock 1.2
 * 	  Selected comments:
 * 	    The basic problem here is that epit_Deflate does not grab the volume's
 * 	    tlock (since there is not volume_table tlock) and so it doesn't merge
 * 	    with transactions that deleted files from the pages being deflated.
 * 	    The conditions actually required to reproduce this problem in practice
 * 	    are pretty damn obtuse.  That these changes actually accomplish this,
 * 	    however, proves that they are not impossibly obtuse.  This first
 * 	    revisions just allow that reproduction.  The actual bug fix will be in a
 * 	    subsequent revision.
 * 	    Basically moved Deflate function from volume_table.c where there is no
 * 	    appropriate tlock to volume.c where it can grab the same tlock that
 * 	    Delete uses.
 * 	    Changed export decl for epit_Deflate.
 * 	Transarc delta: ota-ot5926-AFS-quota 1.7
 * 	  Selected comments:
 * 	    Motivation:
 * 	    This delta augments Episode to maintain two types of size
 * 	    information about filesets and their constituent anodes.  Further,
 * 	    independent limits are omposed for each size measures.  The two
 * 	    measures are called allocated and visible.  The first matches the
 * 	    old semantics and counts fragments actually allocated to the anode
 * 	    or fileset.  The second (new) measure counts fragments visible in an
 * 	    anode or fileset.  The principle difference appears when COW is
 * 	    being used.  A freshly cloned anode will have zero allocated size
 * 	    but visible size which is the same as that for its backing anode.
 * 	    Similarly deleting a large file which was just clones will not
 * 	    reduce the allocated usage of the containing fileset but will return
 * 	    the expected amount of space to the visible usage, even if not
 * 	    blocks were actually freed in the underlying aggregate.
 * 	    Details of the implementation strategy are in:
 * 	    ~ota/doc/episode/FilesetQuota.text.
 * 	    Status:
 * 	    This delta is restricted to Episode only changes.  The corresponding
 * 	    changes in the fileset layers (including ftutil, ftserver, and the
 * 	    fts command, not to mention the doc changes) are in other OTs and
 * 	    deltas.
 * 	    Declare epit_Delete.
 * 	    Introduced a bug in recloning an already recloned fragmented fileset.
 * 	    This was exposed by the fts tests.
 * 	    Fix several problems that turned up when running the fts tests.
 * 	    All_tests was failing on multi-block-frag.int.test.  The cause was a
 * 	    confusion of the calling conventions of MoveUnblockedAnodes; it can be
 * 	    called with the backing container BLOCKED, with the visible field not
 * 	    yet updated.
 * 	    A typo augmented by liberal cut-and-paste caused several volume quota
 * 	    updates to be done with a transaction id of zero (and therefore not
 * 	    logged).  This lead to errors after recovery.
 * 	    Found several salvage problems while tracking down another problem.  But
 * 	    since the afflicted code had been modified in this delta, I thought it
 * 	    would be reduce the risk of merge conflicts to make the change as part
 * 	    of this delta.
 * 	    Changing the IsAnodeAllEmpty predicate to use the visible field is a
 * 	    great efficiency win but the salvager gets really unhappy when it tries
 * 	    to read in directories with a zero visible field (i.e. running on
 * 	    -verify on an old aggregate).
 * 	    For now, for upward compatibility, go back to the old scheme.
 * 	[1993/01/27  17:13:29  shl]
 * 
 * Revision 1.1.5.3  1993/01/18  22:09:16  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  21:50:08  cjd]
 * 
 * Revision 1.1.5.2  1992/11/24  16:23:21  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:21:05  bolinger]
 * 
 * Revision 1.1.3.4  1992/05/05  20:04:34  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  20:58:23  jdp]
 * 
 * Revision 1.1.3.3  1992/01/24  02:59:57  devsrc
 * 	Merging dfs6.3
 * 	[1992/01/24  00:08:56  devsrc]
 * 
 * Revision 1.1.3.2  1992/01/23  19:41:42  zeliff
 * 	Moving files onto the branch for dfs6.3 merge
 * 	[1992/01/23  18:27:55  zeliff]
 * 	Revision 1.1.1.3  1992/01/23  22:12:27  devsrc
 * 	Fixed logs
 * 
 * $EndLog$
 */

/* volume_table.h -- contains the external interface to the volume table
 *     module. */

/* Copyright (C) 1991, 1990 Transarc Corporation - All rights reserved */

#ifndef TRANSARC_EPI_VOLUME_TABLE_H
#define TRANSARC_EPI_VOLUME_TABLE_H

#include <dcedfs/episode/anode.h>

/* For specifying type of index to allocate. */

/* SHARED */
int epit_useOldFidIndex;		/* support old fid indexing */

#define EPIT_TYPE_DONTCARE	0
#define EPIT_TYPE_FILE		1
#define EPIT_TYPE_AUX		2
#define EPIT_DONT_WITHHOLD	4

/* External procedure definitions */

EXPORT long epit_Init _TAKES(( void ));

EXPORT long epit_InitVolumeTable _TAKES((
  IN epi_anode_t volC,
  IN u_int reservedIndexes));

EXPORT long epit_Allocate _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_anode_t volC,
  IN int flags,				/* type of index; use withholding */
  IN u_long nearest,			/* nearby index, if possible */
  OUT u_long *indexP));

EXPORT long epit_Free _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_anode_t volC,
  IN u_long index));

EXPORT long epit_EnumerateVolumeTable _TAKES((
  IN epi_anode_t volC,
  IN long (*proc)(),			/* call for each entry */
  IN char *rock));			/* pass through parameter */

EXPORT long epit_Create _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_anode_t volC,
  IN u_long reservedIndexes));

EXPORT long epit_Delete _TAKES((
  IN elbb_tranRec_t trans,
  IN epi_anode_t volC,
  IN u_int reservedIndexes));

EXPORT long epit_CreateAVL _TAKES((
  IN struct async_device *device,
  IN daddr_t avlAnodeBlock,
  IN u_long reservedIndexes,
  OUT epi_anode_t *avlP));

EXPORT long epit_LastIndex _TAKES((
  IN epi_anode_t volC,
  OUT u_long *indexP));

EXPORT long epit_Deflate _TAKES((
  IN elbb_tranRec_t trans,
  IN epi_anode_t volC));

#endif
