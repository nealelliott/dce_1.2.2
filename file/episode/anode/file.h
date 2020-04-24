/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: file.h,v $
 * Revision 1.1.90.1  1996/10/02  17:17:23  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:27:12  damon]
 *
 * $EndLog$
*/
/* file.h -- contains the external interface to the Episode file module. */

/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */
/* $Header: /u0/rcs_trees/dce/rcs/file/episode/anode/file.h,v 1.1.90.1 1996/10/02 17:17:23 damon Exp $ */

#ifndef TRANSARC_EPI_FILE_H
#define TRANSARC_EPI_FILE_H

#include <dcedfs/epi_id.h>
#include <dcedfs/hyper.h>		/* get dfsh_diskHyper_t */
#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/fid.h>
#include <dcedfs/episode/volume.h>

/* struct epif_status -- contains information stored for each file.  Fields
 *     marked with CIN are used during create, those marked by SIN are also
 *     used by SetStatus, other are read-only.  See also (struct diskStatus) in
 *     file.c for details. */

struct epif_status {
    /* SIN */ u_short flags;		/* misc file module bits */
	      short spares;		/* short spare */
    /* SIN */ u_short linkCount;	/* directory names pointing here */
    /* CIN */ u_short mode;		/* file type & access bits */
    /* SIN */ u_long unique;		/* anode index uniquifier */
    /* SIN */ afs_hyper_t volumeVersionNumber; /* VV of last update to this file */
    /* SIN */ u_long dataVersionNumber;	/* file contents uniquifier */
	      struct timeval /* ack, double words! */
    /* SIN */     mTime,		/*  time of last mod. of contents */
    /* SIN */	  cTime,		/*  ... modification of status */
    /* SIN */	  aTime;		/*  ... access to contents (approx) */
    /* CIN */ u_long oid;		/* owner user id */
    /* CIN */ u_long gid;		/* owning group id */
	      u_long acl;		/* length of acl */
	      u_long plist;		/* ... property list */
	      u_long reservedForUserPlist; /* ... user plist (not metadata) */
    /* CIN */ epi_uuid_t uuid;		/* file's object uuid */
	      long res_timeUncertainty; /* from DTS, encoded in 8 bits each */
	      long spare[2];
    /* SIN */ u_long clientBits;	/* client-only attributes */
	      /* valid only for directories. */
	      u_long initialDirAcl;	/* ... initial acl for new sub-dirs */
	      u_long initialFileAcl;	/* ... initial acl for new files */
};

/* mask bits to allow selective setting of status fields */

#define EPIF_SFLAGS_LINK     0x1	/* link count */
#define EPIF_SFLAGS_MODE     0x4	/* mode */
#define EPIF_SFLAGS_FLAGS    0x8	/* flags */
#define EPIF_SFLAGS_UNIQUE   0x10	/* uniquifier */
#define EPIF_SFLAGS_DVERSION 0x20	/* data version number */
#define EPIF_SFLAGS_OID      0x80	/* owner user id */
#define EPIF_SFLAGS_GID      0x100	/* owner group id */
#define EPIF_SFLAGS_ATIME    0x1000	/* accessTime */
#define EPIF_SFLAGS_MTIME    0x4000	/* modifyTime */
#define EPIF_SFLAGS_CTIME    0x2000	/* anodeTime */
#define EPIF_SFLAGS_UUID     0x8000	/* uuid */
#define EPIF_SFLAGS_VVERSION 0x10000	/* fileset version number */
#define EPIF_SFLAGS_CLIENTBITS 0x20000	/* client-only attrs */

#if 0
#define EPIF_SFLAGS_TYPE     0x2	/* type */
#define EPIF_SFLAGS_CID      0x40	/* creator user id */
#define EPIF_SFLAGS_ACL      0x200	/* acl */
#define EPIF_SFLAGS_DEFACL   0x400	/* default acl? */
#define EPIF_SFLAGS_PLIST    0x800	/* property list? */
#define EPIF_SFLAGS_PARENT   0x8000	/* parent dir? */
#endif

/* Flags for epif_Create */
    /* optional parameters: */
#define EPIF_CFLAGS_COPY_ACL	0x01000000 /* ignored */
#define EPIF_CFLAGS_COPY_PLIST	0x02000000 /* ignored */
#define EPIF_CFLAGS_SHARE_ACL	0x04000000 /* no-op */
#define EPIF_CFLAGS_SHARE_PLIST	0x08000000 /* no-op */
#define EPIF_CFLAGS_DIRECTORY	0x10000000
#define EPIF_CFLAGS_VOLUMEROOT	0x20000000
#define EPIF_CFLAGS_USEFID	0x40000000
#define EPIF_CFLAGS		0xff000000
/* some EPIA_CFLAGS and EPIV_CFLAGS may also appropriate */

/* Flags for ACL/PLIST operations */

/* These are the five defined values of the auxiliary container type.  They are
 * encoded in a 4 bit value used by epif_ReadAux, epif_WriteAux, and
 * epif_ReplaceAux. */

/* These values are also stored in the low 4 bits of the container's status'
 * flags word.  See the struct diskAux declaration in file.c. */

#define EPIF_AUX_IS_RESERVED1 0
#define EPIF_AUX_IS_ACL 1
#define EPIF_AUX_IS_INITIAL_DIR_ACL 2
#define EPIF_AUX_IS_INITIAL_FILE_ACL 3
#define EPIF_AUX_IS_RESERVED2 4
#define EPIF_AUX_IS_PLIST 5
#define EPIF_AUX_IS_USER_PLIST 6
#define EPIF_AUX_TYPE_PLIST 0x4		/* if 0x4 bit is on it is a plist */
#define EPIF_AUX_TYPE_DIRACL 0x2	/* bit 0x2 => approp. for dirs only */
#define EPIF_AUX_NTYPES 7

/* These bits are used for the epif_WriteAux operation, and are combined with
 * one of the above type values. */
#define EPIF_FLAGS_NEW_AUX      0x10
#define EPIF_FLAGS_TRUNCATE_AUX 0x20

/* These are commands that can be passed to epi_ioctl - must not 
   conflict with standard ioctl values */
/* Who uses this? -ota 930204 */

#define EPIF_IOCTL_DEBUG 0x0c000000

/* Convenient as this might be, it is for INTERNAL USE ONLY, in particular,
 * don't be tempted to try to access the acl field this way! */

#define EPIF_FILE_HANDLE_STATUS_OFFSET ((3+5)*4 + sizeof(struct tlock_data))
#define epif_GetStatusAddr(f) \
    ((char *)epia_GetFileRock(f) + EPIF_FILE_HANDLE_STATUS_OFFSET)

/* These are exported macros */

#define epif_FidToShortHandle(fid) epid_ToIndex(fid)

/* Here are some macros that ``cheat'' and access the file disk status
 * structures directly.  They are much faster than calling epif_GetStatus, but
 * do no locking.  Each of the following constants provides the offset of the
 * corresponding member of the diskStatus structure defined in file.c.  These
 * offsets are checked for correctness by asserts in file.c:epif_Init(). */

#define EPIF_DISKSTATUS_LINKCOUNT (2*sizeof(u_int16))
#define EPIF_DISKSTATUS_MODE (EPIF_DISKSTATUS_LINKCOUNT + sizeof(u_int16))
#define EPIF_DISKSTATUS_UNIQUE (EPIF_DISKSTATUS_MODE + sizeof(u_int16))
#define EPIF_DISKSTATUS_DV \
    (EPIF_DISKSTATUS_UNIQUE + sizeof(u_int32) + sizeof(dfsh_diskHyper_t))
#define EPIF_DISKSTATUS_MTIME (EPIF_DISKSTATUS_DV + sizeof(u_int32))
#define EPIF_DISKSTATUS_CTIME (EPIF_DISKSTATUS_MTIME + sizeof(struct timeval))
#define EPIF_DISKSTATUS_ATIME (EPIF_DISKSTATUS_CTIME + sizeof(struct timeval))
#define EPIF_DISKSTATUS_OID (EPIF_DISKSTATUS_ATIME + sizeof(struct timeval))
#define EPIF_DISKSTATUS_GID (EPIF_DISKSTATUS_OID + sizeof(u_int32))

#define epif_GetLinkCount(f) (*(u_int16 *)(epif_GetStatusAddr(f) + \
					   EPIF_DISKSTATUS_LINKCOUNT))
#define epif_GetMode(f)	     (*(u_int16 *)(epif_GetStatusAddr(f) + \
					   EPIF_DISKSTATUS_MODE))
#define epif_GetUnique(f)    (*(u_int32 *)(epif_GetStatusAddr(f) + \
					   EPIF_DISKSTATUS_UNIQUE))
#define epif_GetDV(f)	     (*(u_int32 *)(epif_GetStatusAddr(f) + \
					   EPIF_DISKSTATUS_DV))
#define epif_GetMTime(f)     (*(struct timeval *)(epif_GetStatusAddr(f) + \
						  EPIF_DISKSTATUS_MTIME))
#define epif_GetCTime(f)     (*(struct timeval *)(epif_GetStatusAddr(f) + \
						  EPIF_DISKSTATUS_CTIME))
#define epif_GetATime(f)     (*(struct timeval *)(epif_GetStatusAddr(f) + \
						  EPIF_DISKSTATUS_ATIME))
#define epif_GetOid(f)	     (*(u_int32 *)(epif_GetStatusAddr(f) + \
					   EPIF_DISKSTATUS_OID))
#define epif_GetGid(f)	     (*(u_int32 *)(epif_GetStatusAddr(f) + \
					   EPIF_DISKSTATUS_GID))

/* These are exported functions */

extern long epif_Init _TAKES(( void ));

extern long epif_NextDeletedFile(
  IN epi_volume_t vol,
  IN u_long iter,
  OUT u_long *nextIterP,
  OUT epi_volFileId_t *fid
);

extern long epif_Create _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_volume_t vol,
  IN long flags,
  IN unsigned long estLength,
  IN struct epif_status *initialStatus,
  IN epi_anode_t aclSource,		/* file to get acl from */
  IN epi_anode_t plistSource,		/* file to get plist from */
  OUT epi_volFileId_t *fid,
  OUT epi_anode_t *fileP));

extern long epif_CreateE _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_volume_t vol,
  IN long flags,
  IN unsigned long estLength,
  IN struct epif_status *initialStatus,
  IN epi_anode_t aclSource,		/* file to get acl from */
  IN epi_anode_t plistSource,		/* file to get plist from */
  IN epi_anode_t backingSource,		/* file to be copy on write from */
  OUT epi_volFileId_t *fid,
  OUT epi_anode_t *fileP));

extern void epif_AllowOpens (
  epi_anode_t fileC);

extern long epif_Open _TAKES((
  IN epi_volume_t vol,			/* volume handle */
  IN epi_volFileId_t *fid,		/* File to open */
  OUT epi_anode_t *fileP));		/* file handle */

extern long epif_Close _TAKES((
  IN epi_anode_t fileC));

extern long epif_SetStatusAndMark _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_anode_t f,
  IN long markFlags,
  IN long setStatusFlags,
  IN struct epif_status *status));

#define epif_Mark(trans,f,flags) epif_SetStatusAndMark(trans,f,flags,0,0)
#define epif_SetStatus(trans,f,flags,status) \
				 epif_SetStatusAndMark(trans,f,0,flags,status)

extern long epif_ChangeLink _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_anode_t f,
  IN int delta,				/* amount to change link count by */
  IN int delete,			/* boolean: delete if count is zero */
  OUT u_int *newLinkP));		/* resultiing link count */

#define EPIF_DELETE_IGNORE_LINKCNT 0x1
extern long epif_Delete _TAKES((
  IN epi_anode_t f,
  IN int flags,
  OUT u_int* newLinkP));

extern long epif_GetStatus _TAKES((
  IN epi_anode_t f,
  OUT struct epif_status *status));

extern long epif_ReadAux _TAKES((
  IN epi_anode_t file,
  IN int whichAux,
  IN u_long offset,
  IN u_long length,
  OUT char *buffer));

extern long epif_WriteAux _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_anode_t file,
  IN long flags,
  IN u_long offset,
  IN u_long length,
  IN char *buffer));

extern long epif_ReplaceAux _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_anode_t file,
  IN long flags,
  IN epi_anode_t auxFile));

extern long epif_Clone _TAKES((
  IN buffer_tranRec_t trans,
  IN epi_anode_t f,
  IN long flags));

extern void epif_UniqueTime(OUT struct timeval *t);

#endif
