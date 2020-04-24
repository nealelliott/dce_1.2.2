/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* epia_volreg.c -- contains basically obsolete procedures for managing
 *     Episode's local volume registry. */

/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/osi.h>
#include <dcedfs/hash.h>
#include <dcedfs/episode/critical.h>

#include "anode.h"
#include "volreg.h"

RCSID ("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/anode/RCS/epia_volreg.c,v 4.26 1996/03/04 18:59:14 ota Exp $")

static osi_dlock_t vr_lock;
static struct dfsh_hashTable vr_id_ht;
static struct dfsh_hashTable vr_name_ht;

struct vr_vol {
    struct vr_vol *idNext;
    struct vr_vol *nameNext;
    epi_anode_t avl;			/* corresponding aggregate */
    afs_hyper_t id;				/* id of volume */
    char name[64];			/* name of volume */
    long index;				/* corresponding index, or zero */
    epi_anode_t vol;			/* handle if open */
};

/* These hash functions are derived from those in the backup database.
 *   -ota 900702 */

/* This is the result of the above search for good hash functions.  It seems
 * that the choice of multipliers is somewhat arbitrary but has several
 * constraints.  It shouldn't have too many or too few one bits and should be
 * odd.  It behaves better than the random hash function. */

static unsigned long VolNameHash(IN struct vr_vol *entry)
{
    unsigned char *str = (unsigned char *)entry->name;
    unsigned long hash = 1000003;	/* big prime to make "" hash nicely */
    /* The multiplicative constant should be odd and have a goodly number of
     * one bits. */
    while (*str) hash = (*str++) + hash * 0x10204081;
    return hash;
}

static unsigned long VolIdHash(IN struct vr_vol *entry)
{   unsigned long l,r;
    unsigned long lo,hi;

    hi = AFS_hgethi(entry->id);
    lo = AFS_hgetlo(entry->id);
    lo *= 81847;
    l = lo | 0xaaaaaaaa;
    r = lo | 0x55555555;
    lo = l*r;
    l = hi | 0xaaaaaaaa;
    r = hi | 0x55555555;
    return (l*r + lo);
}

/* EXPORT */
long vr_Init (void)
{
    static int inited = 0;
    long code;

    if (inited) return 0;

    lock_Init (&vr_lock);
    code = dfsh_HashInit (&vr_id_ht, VolIdHash,
			  offsetof(struct vr_vol, idNext));
    if (code) return code;
    code = dfsh_HashInitKeyFixed
	(&vr_id_ht, offsetof(struct vr_vol, id), sizeof(afs_hyper_t));
    if (code) return code;
    code = dfsh_HashInit (&vr_name_ht, VolNameHash,
			  offsetof(struct vr_vol, nameNext));
    if (code) return code;
    code = dfsh_HashInitKeyString (&vr_name_ht,
				   offsetof(struct vr_vol, name[0]));
    if (code) return code;
    inited++;
    return 0;
}

/* EXPORT */
long vr_LookupVolumeId(
  IN afs_hyper_t id,				/* volume ID to find */
  OUT epi_anode_t *avlP,		/* containing avl */
  OUT u_long *indexP)			/* index of volume */
{
    long code = 0;
    struct vr_vol key;
    struct vr_vol *vr;

    key.id = id;
    BEGIN_LOCK_READ (&vr_lock) {
	vr = dfsh_HashLookup (&vr_id_ht, &key);
	if (vr == 0) { ABORT_CRIT (EPI_E_NOENT); }
	if (avlP) *avlP = vr->avl;
	if (indexP) *indexP = vr->index;
    } END_LOCK;
    return code;
}

/* EXPORT */
long vr_LookupVolumeName(
  IN char *name,			/* volume name to find */
  OUT epi_anode_t *avlP,		/* containing avl */
  OUT u_long *indexP)			/* index of volume */
{
    long code = 0;
    struct vr_vol key;
    struct vr_vol *vr;

    if (strlen (name) >= sizeof(key.name)) return EPI_E_BADARGS;
    strcpy (key.name, name);
    BEGIN_LOCK_READ (&vr_lock) {
	vr = dfsh_HashLookup (&vr_name_ht, &key);
	if (vr == 0) { ABORT_CRIT (EPI_E_NOENT); }
	if (avlP) *avlP = vr->avl;
	if (indexP) *indexP = vr->index;
    } END_LOCK;
    return code;
}

/* EXPORT */
long vr_AddVolume(
  IN afs_hyper_t id,				/* volume ID */
  IN char *name,			/* volume name */
  IN epi_anode_t avl,			/* containing avl */
  IN u_long index)			/* index of volume */
{
    long code;
    struct vr_vol key;
    struct vr_vol *vr;

    if (strlen (name) >= sizeof(key.name)) return EPI_E_BADARGS;
    strcpy (key.name, name);
    key.id = id;
    BEGIN_LOCK_WRITE (&vr_lock) {
	vr = dfsh_HashLookup (&vr_id_ht, &key);
	if (vr) { ABORT_CRIT (EPI_E_VOLIDEXISTS); }
	vr = dfsh_HashLookup (&vr_name_ht, &key);
	if (vr) { ABORT_CRIT (EPI_E_VOLNAMEEXISTS); }
	vr = osi_Alloc_r(sizeof(*vr));
	bzero((caddr_t)vr, sizeof(*vr));
	vr->id = id;
	strcpy (vr->name, name);
	vr->avl = avl;
	vr->index = index;
	code = dfsh_HashIn_r (&vr_id_ht, vr);
	afsl_MBZ(code);
	code = dfsh_HashIn_r (&vr_name_ht, vr);
	afsl_MBZ(code);
    } END_LOCK;
    return code;
}

/* EXPORT */
long vr_RemoveVolume(
  IN afs_hyper_t id,
  IN epi_anode_t avl,			/* containing avl */
  IN u_long index)			/* index of volume */
{
    long code;
    struct vr_vol key;
    struct vr_vol *vr;

    key.id = id;
    BEGIN_LOCK_WRITE (&vr_lock) {
	vr = dfsh_HashLookup (&vr_id_ht, &key);
	if (vr == 0) { ABORT_CRIT (EPI_E_NOENT); }

	/* Check to make sure we've got the right one */
	if ((avl != vr->avl) || (index != vr->index))
	    { ABORT_CRIT (EPI_E_VOLIDEXISTS); }

	code = dfsh_HashOut_r (&vr_id_ht, vr);
	afsl_MBZ(code);
	code = dfsh_HashOut_r (&vr_name_ht, vr);
	afsl_MBZ(code);
	osi_Free_r((opaque)vr, sizeof (*vr));
    } END_LOCK;
    return code;
}
