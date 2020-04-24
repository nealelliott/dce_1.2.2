/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: volume.h,v $
 * Revision 1.1.90.1  1996/10/02  17:19:53  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:29:17  damon]
 *
 * $EndLog$
*/

/* volume.h -- contains the external interface to the volume module. */

/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/anode/volume.h,v 1.1.90.1 1996/10/02 17:19:53 damon Exp $ */

#ifndef TRANSARC_EPI_VOLUME_H
#define TRANSARC_EPI_VOLUME_H

#include <dcedfs/episode/logbuf.h>
#include <dcedfs/episode/anode.h>

#define VOLUME_MAGIC		0xa197840a

/* This marks the volume handle for a bad volume.  Never used on disk. */
#define VOLUME_BAD_MAGIC	(~VOLUME_MAGIC)

/* The identity of a fileset stored (redundantly) in its anode status area, for
 * fast access. */
#define EPIV_IDENT_NAME_SIZE 112
struct epiv_ident {
    afs_hyper_t id;
    char name[EPIV_IDENT_NAME_SIZE];
};

#define EPIV_STATUS_MSG_SIZE 128
/* Must be less than 16 due to limitations of the epiv_SetStatus interface. */
#define EPIV_STATUS_FILESTATUS_WORDS 12
#define EPIV_STATUS_UPLEVELIDS_HYPERS 4

/* NOTE -- last 4 words of "sparel[9]" are now "visible". */

struct epiv_status {
    int32 magic;			/* VOLUME_MAGIC */
    struct epiv_ident ident;		/* fileset's id & name */
    int32 flags;			/* miscellaneous, persistent bits */
    u_int32 index;			/* of fileset table anode in avl */
    afs_hyper_t version;			/* replica fileset version number */
    u_int32 unique;			/* uniquifier for new anodes */
    u_int32 createTime;			/* time fileset created */
    /* These two are sometimes stored simultaneously */
    u_int32 modTime;			/*   last modification */
    u_int32 accessTime;			/*   last epiv Close */
    int32 backingVolIndex;		/* avl index of backing fileset */
    afs_hyper_t backingVolId;			/* id of backing fileset */
    afs_hyper_t upLevelIds[EPIV_STATUS_UPLEVELIDS_HYPERS]; /* for rep/ft server */
    struct epiv_quota {			/* quota data */
	u_int32 spare1;			/* in case we want hypers someday */
	u_int32 limit;			/* max fragments in fileset */
	u_int32 spare2;			/*  hypers  */
	u_int32 usage;			/* fragments used by fileset */
    } allocated;
    u_int32 upLevelTime1;		/* time values for */
    u_int32 upLevelTime2;		/*    rep/ft/backup */
    u_int32 upLevelTime3;		/*    servers       */
    /* 57 words to this point */
    int32 sparel[2];
    struct epiv_quota visible;		/* visible space "used". */
    /* data from second anode */
    char statusMsg[EPIV_STATUS_MSG_SIZE]; /* What's up Doc? */
    int32 fileStatus[EPIV_STATUS_FILESTATUS_WORDS];
    char sparec[76];
};

/* A fileset handle is just the anode handle of the fileset container. */
typedef epi_anode_t epi_volume_t;

/* Mask bits to allow selective setting of status fields.  Used by
 * epiv_SetStatus */
#define EPIV_STATUS_NAME		0x0001
#define EPIV_STATUS_ID			0x0002
#define EPIV_STATUS_FLAGS		0x0004
#define EPIV_STATUS_VERSION		0x0008
#define EPIV_STATUS_UNIQUE		0x0010
#define EPIV_STATUS_BACKING_INDEX	0x0020
#define EPIV_STATUS_BACKING_ID		0x0040
#define EPIV_STATUS_Q_ALLOCATED		0x0080
#define EPIV_STATUS_Q_VISIBLE		0x2000
#define EPIV_STATUS_MSG			0x0100

/* To set one word of fileStatus set the WORD_N bit and put the index to be set
 * in the high 4 bits of the mask.  Only that word from the fileStatus array
 * will be updated. */
#define EPIV_STATUS_FILESTATUS	0x0200	/* set whole file status field */
#define EPIV_STATUS_FILESTATUS_WORD_N 0x0400
#define EPIV_STATUS_FILESTATUS_N_SHIFT 28

#define EPIV_STATUS_UPLEVELIDS	0x0800	/* set whole uplevelids array */
#define EPIV_STATUS_UPLEVELIDS_WORD_N 0x1000 /* as with filestatus */
#define EPIV_STATUS_UPLEVELIDS_N_SHIFT EPIV_STATUS_FILESTATUS_N_SHIFT

/* Up-level time fields. */
#define EPIV_STATUS_UPLEVELTIME_1	0x04000
#define EPIV_STATUS_UPLEVELTIME_2	0x08000
#define EPIV_STATUS_UPLEVELTIME_3	0x10000

#define EPIV_STATUS_LEGAL_BITS	0xf001ffff

/* flag bits for epiv_diskVolumeHeader.flags */
#define EPIV_STATUS_FLAGS_REPFIELD		0x000f
/* (values for EPIV_STATUS_FLAGS_REPFIELD:) */
#define EPIV_STATUS_FLAGS_REP_NONE	0x0000
#define EPIV_STATUS_FLAGS_REP_RELEASE	0x0001
#define EPIV_STATUS_FLAGS_REP_LAZY	0x0002

#define EPIV_STATUS_FLAGS_READONLY	0x0010
#define EPIV_STATUS_FLAGS_DELONSALVAGE	0x0020 /* obsolete name */
#define EPIV_STATUS_FLAGS_INCONSISTENT	0x0020
#define EPIV_STATUS_FLAGS_COMPLETE	0x0040

/* define a four-bit higher-level "type" field */
#define	EPIV_STATUS_FLAGS_TYPEFIELD	0x0780
#define	EPIV_STATUS_FLAGS_TYPE_RW	0x0080 /* ordinary r/w */
#define	EPIV_STATUS_FLAGS_TYPE_RO	0x0100 /* ".readonly" */
#define	EPIV_STATUS_FLAGS_TYPE_BK	0x0180 /* ".backup" */
#define	EPIV_STATUS_FLAGS_TYPE_TEMP	0x0200 /* transient volumes, for dumps
                                                * or moves, etc. */

#define	EPIV_STATUS_FLAGS_REPSERVER_MGD	0x0800 /* managed by repserver */

#define EPIV_STATUS_FLAGS_DELETENOW	0x1000 /* delete as soon as possible */
#define EPIV_STATUS_FLAGS_CLONEINPROG	0x2000 /* clone op. did not finish */
#define EPIV_STATUS_FLAGS_NOEXPORT	0x4000 /* not created for exporting */
#define EPIV_STATUS_FLAGS_IS_REPLICATED       0x8000 /* this R/W fileset is replicated */
#define EPIV_STATUS_FLAGS_MOVE_SOURCE 0x10000 /* source for a fileset move */
#define EPIV_STATUS_FLAGS_MOVE_TARGET 0x20000 /* target of a fileset move */

#define EPIV_STATUS_FLAGS_LEGALFLAGS  0x3ffff

/* Flags for Create Anode */

    /* optional parameters: */
#define EPIV_CFLAGS_USEFID      0x010000 /* must use specified index (seems
					  * like a bad name for this) */
#define EPIV_CFLAGS_USEINDEX EPIV_CFLAGS_USEFID	/* so here's an alias */
#define EPIV_CFLAGS_USEVOLDATA  0x020000
#define EPIV_CFLAGS_USENEARHINT 0x040000
        /* should use EPIA_CFLAGS_TYPE for this, but too big a change now. */
#define EPIV_CFLAGS_TYPE_FILE	0x080000 /* return index for a file */
#define EPIV_CFLAGS_TYPE_AUX	0x100000 /* return index for an aux cntnr */
#define EPIV_CFLAGS	        0xff0000
    /* Some EPIA_CFLAGS, in particular EPIA_CFLAGS_COPYONWRITE and
     * EPIA_CFLAGS_BACKING, are also appropriate */

/* Exported Macros */

#define epiv_GetVolume(anodeC) ((epi_volume_t)epia_GetVolumeRock (anodeC))
#define epiv_GetAnodeIndex(anodeC) (epia_GetIndex(anodeC) - 1)

#if 0
/* Quota field referencing macros. */
#define epiv_q_allocated allocated.usage
#define epiv_q_alimit	 allocated.limit
#define epiv_q_visible	 visible.usage
#define epiv_q_vlimit	 visible.limit
#endif

/* Shared Macros */
	/* Based on the definition of struct volume in volume.c */
#define EPIV_AUXTLOCK_OFF (2 * sizeof(struct tmutex_data))
#define epiv_GetAuxTLock(volC) \
    ((struct tlock_data *)((char *)epia_GetFileRock(volC) + EPIV_AUXTLOCK_OFF))
#define EPIV_ZEROLOCK_OFF (EPIV_AUXTLOCK_OFF + sizeof(struct tlock_data))
#define epiv_GetZeroLock(volC) \
    ((epi_mutex_t *)((char *)epia_GetFileRock(volC) + EPIV_ZEROLOCK_OFF))

/* Exported Functions */

extern long epiv_Init _TAKES(( void ));

extern long epiv_Create _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_anode_t avl,
  IN epi_anode_t volC,			/* for volume table */
  IN struct epiv_status *status));

extern long epiv_Open _TAKES((
  IN epi_anode_t avl,			/* corresponding aggregate */
  IN u_long index,			/* for volume table */
  OUT epi_volume_t *volP));		/* volume handle */

extern long epiv_Close _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_volume_t vol));			/* volume handle */

extern long epiv_Delete _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_volume_t vol));			/* volume handle */

extern long epiv_GetAggregate _TAKES((
  IN epi_volume_t h,
  OUT epi_anode_t *avlP));

extern long epiv_GetStatus _TAKES((
  IN epi_volume_t h,
  OUT struct epiv_status *status));

extern long epiv_GetZLCAddr _TAKES((
  IN epi_volume_t h,
  OUT int32 **zlcListP));

extern long epiv_GetFlags _TAKES((
  IN epi_volume_t h,
  OUT int32 *flagsP));

extern long epiv_GetIdent _TAKES((
  IN epi_volume_t h,
  OUT struct epiv_ident *ident));

extern void epiv_GetVV _TAKES((
  IN epi_volume_t h,
  OUT afs_hyper_t *versionP));

extern long epiv_SetStatus _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_anode_t h,
  IN long mask,
  IN struct epiv_status *status));

extern long epiv_QuiesceHeader(
  IN epi_volume_t h,
  IN unsigned consistency
);

extern long epiv_TwiddleInconBit _TAKES((
  IN epi_volume_t h,
  IN int turnon));

extern long epiv_SuppressQuotaCheck _TAKES((
  IN epi_volume_t volC,
  IN unsigned on));

extern long epiv_NewUnique _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_volume_t volC,
  IN int gen,				/* generate new unique */
  IN u_long index,
  INOUT u_long *uniqueP));

extern long epiv_NewVolumeVersion _TAKES((
  IN elbb_tranRec_t trans,
  IN epi_volume_t volC,
  OUT afs_hyper_t *newVersionP));

extern long epiv_CreateAnode _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_volume_t volC,
  IN long flags,
  IN unsigned int statusSize,
  IN unsigned long estLength,
  IN epi_anode_t backingAnodeC,
  IN u_long backingIndex,
  IN u_long nearIndex,
  INOUT u_long *indexP,
  OUT epi_anode_t *anodeP));

extern long epiv_DeleteAnode _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_anode_t anodeC));

extern long epiv_OpenAnode _TAKES((
  IN epi_volume_t volC,
  IN u_long index,
  OUT epi_anode_t *hP));

extern long epiv_CloseAnode _TAKES((
  IN epi_volume_t anodeC));

extern long epiv_OpenBackingAnode _TAKES((
  IN epi_anode_t anodeC,
  OUT epi_volume_t *vP,			/* desired backing volume */
  OUT epi_anode_t *hP));			/* desired backing anode */

extern long epiv_LastIndex _TAKES((
  IN epi_volume_t volC,
  OUT u_long *indexP));

extern long epiv_Deflate _TAKES((
  IN epi_volume_t volC));

#endif /* TRANSARC_EPI_VOLUME_H */
