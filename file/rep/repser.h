/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: repser.h,v $
 * $EndLog$
 */

#ifndef	_REPLICA_H_
#define	_REPLICA_H_

/*
 * (C) Copyright 1996, 1990 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 *
 * $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/rep/RCS/repser.h,v 4.43 1996/10/12 18:11:39 madhuc Exp $
 *
 * repser.h
 *	Definitions for the replication server's internal data structures.
 */

#include <dcedfs/stds.h>	/* Transarc coding standards */
#include <dcedfs/afs4int.h>	/* Basic data types */
#include <dcedfs/lock.h>	/* Locking package */
#include <dcedfs/volume.h>	/* Volume definitions */
#include <dcedfs/aggr.h>	/* aggregates */
#include <dcedfs/tkm_tokens.h>	/* TKM token fields */

#ifndef IPPROTO_TCP
#ifndef SOCK_STREAM
#include <sys/socket.h>
#endif /* SOCK_STREAM */
#include <netinet/in.h>
#endif /* IPPROTO_TCP */
#include <dcedfs/fldb_data.h>	/* MAXKPRINCIPALLEN */

#include <dcedfs/rep_errs.h>	/* Generated errors */
#include "rep_host.h"
#include "rep_queue.h"

/*
 *----------------------- Exported definitions  -----------------------
 *
 * These are the definitions that our clients must see in order to use
 * this module.
 */

/*
  * Define a handy name for the successful return code, which is NOT
  * included in the compiled error return codes.
  */
#define REP_ERR_SUCCESS  0

/* Flags for the exported functions. */
#define	REP_FLAG_ISREPLICA	(0x0001)
#define	REP_FLAG_ISLAZY  (0x0002)
#define	REP_FLAG_IMMEDIATE (0x0004)
#define	REP_FLAG_EXECUTING  (0x0008)
#define	REP_FLAG_QUICKDESTROY    (0x0010)
#define	REP_FLAG_AWAIT   (0x0020)


/* Compare two network addresses. */
#define	SameAddress(ad1, ad2) (bcmp(((char*)(ad1)), ((char*)(ad2)), sizeof(struct afsNetAddr)) == 0)
#define	SameAddresses(ad1, ad2, num) (bcmp(((char*)(ad1)), ((char*)(ad2)), (sizeof(struct afsNetAddr)*(num))) == 0)

/* Shared definitions */
/* The ``replica'' structure describes a unit of abstract work for this module--a volume that is to be replicated on this site.
    Up to two ``struct volume'' structures may point to it, as they would if they were being managed by this abstract work item.
	That is, in general there might be a volume being published as a replica, as well as a volume in the process of being built by the replication server on this site.
    The replicaP->lockObject controls the ``struct replica'' fields and all its sub-fields, even though they
	are named as ``struct vldbRepInfo'' and ``struct RepSite''.
*/

struct FileKA {
    u_int32 VnodeIx;
    u_int32 Uniq;
    u_int32 SentWhen;
    u_int32 RecvWhen;
    char Flags;
#define	KA_EXECUTING	(0x01)
#define	KA_SOON	(0x02)
};

struct localvol {
    struct localvol	*Next;	/* singly-linked */
    afs_hyper_t	volId;
    afs_hyper_t	volVersion;
    afs_hyper_t	llBackId;
    afs_hyper_t	llFwdId;
    afsTimeval	diskCurr;
    afsTimeval	diskPingCurr;
    afsTimeval	diskTknExp;
    u_int32		aggr;
    u_int32		lastCallTime;	/* updated to this time */
    int32		volDesc;	/* used if we have it open (via syscall interface) */
    u_int32		itsStates;	/* copy of vol_st.states */
    u_int32		flags;	/* any additional flags we need locally */
#define	lvf_GotStatus	0x001
#define	lvf_Deleted	0x002
#define	lvf_FreeID	0x004
};

#define	ADDRSPERSITE	4
struct RepSite {
    struct afsNetAddr AddrInfo[ADDRSPERSITE];	/* some address info */
    u_int32 NumAddrs;
    u_int32 ReplicaLatency;	/* replicaLatency value for this one */
    u_int32 ReplicaAggregate;	/* aggregate-on-site identification */
};

struct vldbRepInfo {
    afs_hyper_t VolIDOfPrimary;
    afs_hyper_t VolIDOfSecondary;
    afs_hyper_t VolIDOfBackup;
    u_int32 maxTotalLatency, hardMaxTotalLatency, minimumPounceDally;
    u_int32 reclaimDally;
    u_int32 vlflags;
    struct afsNetAddr   PrimaryAddrs[ADDRSPERSITE];	/* Site for primary replica */
    u_int32 NumPrimaryAddrs;
    u_int32 PrimaryAggregate;	/* aggregate-on-site info for that */
    u_int32 StageAggregate;	/* aggregate-on-site info for staging replica, if any */
    struct RepSite *Reps;		/* array of RepSite structures */
    u_int32 NumReps;		/* count of that array */
    u_int32 MaxReps;		/* space for that array */
    char mainVolName[VOLNAMESIZE];
    char hName[MAXKPRINCIPALLEN];	/* for authentication */
};

struct freeVolID {
    struct freeVolID *Next;
    afs_hyper_t ID;
    char wasReused;
};

/* Structure for GetVolChangedFiles to use */
struct volChanged {
    struct volChanged *Another;
    u_int32 whenCreated;
    u_int32 whenReferenced;
    afs_hyper_t fromVV;
    afs_hyper_t toVV;
    u_int32 numFids, maxFids;
    struct fidInVol *fids;
};

struct replica {
    struct replica *Next;    /* hash chains */
    osi_dlock_t refLock;     /* locks ref count (lower than lockObject) */
    int32 RefCount;
    osi_dlock_t lockObject;    /* locks this structure, v, and v->Reps */
    struct vldbRepInfo v;
    afs_hyper_t CellId;
    struct localvol *AllLocals;	/* this is where the list is rooted */
    struct localvol *PublishedP;	/* may point within list */
    struct localvol *BuildingP;	/* also may point within the list */
    struct localvol *JunkP;	/* also may point within the list */
    u_int32 MymaxReplicaLatency;
    u_int32 MyReplicaAggregate;
    int32 mySiteIx;
    struct freeVolID *fvptr;
    rep_host_t *hostP;
    afs_hyper_t WVT_ReadTokenID;
    afs_hyper_t WVT_TypesLeft;
    afs_hyper_t WVT_TypesRevoked;
    struct timeval TimeOfWVTExpiration;
    struct timeval TimeOfReadTokenLoss;
    struct timeval TimeOfLatestKnownCurrent;
    struct timeval TimeOfLastPing;
    struct timeval TimeOfLastRetrievalTry;
    struct timeval LastPublishTime;
    struct timeval nextLookTime;
    afs_hyper_t VVOnSource;
    struct timeval LastKASweep;
    u_int32 remoteBusyWait;		/* small timeout value for these parameters */
    u_int32 kaLeadTime;
    u_int32 tokenLeadTime;
    u_int32 memoryWait;
    u_int32 localDiskWait;
    u_int32 hostLifeGuarantee;
    struct FileKA *KA;	/* keep-alive information for this volume */
    u_int32 NumKAs;
    u_int32 MaxKAs;
    u_int32 Flags;
/* values for struct-replica.Flags */
/* #define	repflag_HaveWVTRead	(0x00001) */
#define	repflag_KillMe		(0x00002)
#define	repflag_GrabbingWVTRead	(0x00004)
#define	repflag_UserWantsUpdate	(0x00008)
#define	repflag_KANow		(0x00010)
#define	repflag_KAThisPass	(0x00020)
#define	repflag_KnowVLDB	(0x00040)
#define	repflag_ReleaseOnly	(0x00080)
#if !defined(OSF_NO_SOCKET_DUMP)
#define repflag_UseSocket	(0x00100)
#endif /* OSF_NO_SOCKET_DUMP */
    struct volChanged	*Changes;
    char RepMsg[REPMSG_SIZE];
    rep_queue_cookie_t qCookie;
};
/* Table lock (RepTableLock) locks all the Next pointers and the GFlags. */

extern int Debugging;

extern osi_dlock_t RepTableLock;
extern struct replica *RepRoot;
extern int numReplicas;

extern char ErrBuff[];
const extern char WhoIAm[];
extern struct ubik_client *VLDBcstruct;
extern afs_hyper_t localCellID;
extern char localCellName[];
extern struct icl_set *rep_iclSetp;
extern struct replica *NewReplicaStruct();
extern long CheckVLDBRelationship();
extern pthread_cond_t	actNowCond;

/* from rep_peer.c */
extern u_long DoWillCalls();
extern void initPeers();

/* from rep_addr.c */
extern struct afsNetAddr myNetAddrs[ADDRSPERSITE];
extern int numMyNetAddrs;
extern int goodMyNetAddrs;
extern int getMyNetAddrs();
extern int OnThisMachine();

/* from rep_boot.c */
extern long EnumerateVLDB();
extern long EnumerateLocalVolumes();

/* from rep_tkint.c */
extern struct afsNetData revocationNetData;
extern unsigned long revocationEpoch;
extern void rep_initTokens();

#endif	/* _REPLICA_H_ */
