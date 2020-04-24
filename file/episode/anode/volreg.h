/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: volreg.h,v $
 * Revision 1.1.13.1  1996/10/02  17:19:46  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:29:13  damon]
 *
 * $EndLog$
 */
/* This file contains procedures for managing the volume registry. */

/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */
/* $Header: /u0/rcs_trees/dce/rcs/file/episode/anode/volreg.h,v 1.1.13.1 1996/10/02 17:19:46 damon Exp $ */

#ifndef TRANSARC_VR_VOLREG_H
#define TRANSARC_VR_VOLREG_H

#include <dcedfs/episode/anode.h>

EXPORT long vr_Init _TAKES(( void ));

EXPORT long vr_LookupVolumeId _TAKES((
  IN afs_hyper_t id,				/* volume ID to find */
  OUT epi_anode_t *avlP,		/* containing avl */
  OUT u_long *indexP));			/* index of volume */

EXPORT long vr_LookupVolumeName _TAKES((
  IN char *name,			/* volume name to find */
  OUT epi_anode_t *avlP,		/* containing avl */
  OUT u_long *indexP));			/* index of volume */

EXPORT long vr_AddVolume _TAKES((
  IN afs_hyper_t id,				/* volume ID */
  IN char *name,			/* volume name */
  IN epi_anode_t avl,			/* containing avl */
  IN u_long index));			/* index of volume */

EXPORT long vr_RemoveVolume _TAKES((
  IN afs_hyper_t id,
  IN epi_anode_t avl,			/* containing avl */
  IN u_long index));			/* index of volume */

#endif
