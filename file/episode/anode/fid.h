/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: fid.h,v $
 * Revision 1.1.98.1  1996/10/02  17:17:16  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:27:10  damon]
 *
 * $EndLog$
 */
/* This file contains the interface to file identifiers. */

/* Copyright (C) 1994, 1990 Transarc Corporation - All rights reserved */
/* $Header: /u0/rcs_trees/dce/rcs/file/episode/anode/fid.h,v 1.1.98.1 1996/10/02 17:17:16 damon Exp $ */

#ifndef TRANSARC_EPI_FID_H
#define TRANSARC_EPI_FID_H

#include <dcedfs/osi_net.h>
#include <dcedfs/episode/async.h>

typedef struct epid_volFileId {
    u_int32 index;
    u_int32 unique;
} epi_volFileId_t;

/* Special fid values */
#define EPID_DONTCARE_UNIQUE (-1)
#define EPID_DONTCARE_UNIQUE_NSBO (-1)	/* network standard byte order */

#define EPID_INVALID_INDEX  (0)
#define EPID_INVALID_INDEX_NSBO (0)     /* network standard byte order */
#define EPID_INVALID_UNIQUE (0)
#define EPID_INVALID_UNIQUE_NSBO (0)    /* network standard byte order */

#define epid_SetDCUnique(fid) (epid_UniquePart(fid) = EPID_DONTCARE_UNIQUE_NSBO)
#define epid_IsDCUnique(fid) (epid_UniquePart(fid) == EPID_DONTCARE_UNIQUE_NSBO)
#define epid_ClearUnique(fid) (epid_UniquePart(fid) = 0)
#define epid_MakeInvalid(fid) \
    (epid_IndexPart(fid)  = EPID_INVALID_INDEX_NSBO, \
     epid_UniquePart(fid) = EPID_INVALID_UNIQUE_NSBO)
#define epid_MakeVolumeRoot(fid) epid_Assemble(fid, 1, 1)
#define epid_MakeVolume(fid) epid_Assemble(fid, 0, 1)

/* Predicates on fids. */
#define epid_Equal(f1, f2) \
    ((epid_IndexPart(f1) == epid_IndexPart(f2)) && \
     (epid_UniquePart(f1) == epid_UniquePart(f2)))
#define epid_EqualIndexes(f1, f2) \
    (epid_IndexPart(f1) == epid_IndexPart(f2))
#define epid_IsLegal(fid) (epid_IndexPart(fid) != EPID_INVALID_INDEX_NSBO)
#define epid_IsInvalid(fid) \
    ((epid_IndexPart(fid)  == EPID_INVALID_INDEX_NSBO) && \
     (epid_UniquePart(fid) == EPID_INVALID_UNIQUE_NSBO))
#define epid_IsVolume(fid) \
    ((epid_IndexPart(fid) == 0) && (epid_ToUnique(fid) == 1))
#define epid_IsVolumeRoot(fid) \
    ((epid_ToIndex(fid) == 1) && (epid_ToUnique(fid) == 1))

/* epid_MakeIndexOnly -- builds a fid from a fid-style index. */

#define epid_MakeIndexOnly(fid, index) \
    epid_Assemble(fid, index, EPID_DONTCARE_UNIQUE)

extern u_long epid_LastIndex(
  IN u_long lastIndex,			/* epiv-style index */
  IN struct async_device *dev);

#define epid_ToUnique(fid) ntoh32(epid_UniquePart(fid))

/* epid_ToIndex -- Returns the file number from \arg{fid}. */

#define epid_ToIndex(fid) ntoh32(epid_IndexPart(fid))

/* epid_Assemble -- Builds \arg{fid} from the file number \arg{index} with a
 *     uniquifier of \arg{unique}. */

#define epid_Assemble(fid, index, unique) \
    (*(u_int32 *)(fid) = hton32(index), \
     *(u_int32 *)((char *)(fid) + sizeof(u_int32)) = hton32(unique))

/* epid_SetUnique -- set the uniquifier in a fid. */
#define epid_SetUnique(fid, unique) \
    (*(u_int32 *)((char *)(fid) + sizeof(u_int32)) = hton32(unique))

extern char *epid_PrintedFid(
  IN epi_volFileId_t *fid,
  IN int len,
  OUT char *buf);

/* PRIVATE operations on fids to be used only within episode/anode. */

/* SHARED */
extern u_long epid_IndexFromFid(
  IN epi_volFileId_t *fid,
  IN struct async_device *dev);
/* SHARED */
extern u_long epid_IndexForFid(
  IN u_long i,                          /* epiv-style index */
  IN struct async_device *dev);

/* epid_ToEPIVIndex -- returns the epiv-style index from a fid. */

#define epid_ToEPIVIndex(fid, dev) epid_IndexFromFid(fid, dev)

/* epid_Make -- builds a fid from an epiv-style index and a uniquifier. */

#define epid_Make(fid, dev, index, unique) \
      (epid_UniquePart(fid) = \
       ((epid_IndexPart(fid) = hton32(epid_IndexForFid(index, dev))) \
	? hton32(unique) : 0))

#define epid_IndexPart(fid) ((fid)->index)
#define epid_UniquePart(fid) ((fid)->unique)

#endif
