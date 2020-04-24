/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: flutils.c,v $
 * Revision 1.1.74.1  1996/10/02  17:46:21  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:37:09  damon]
 *
 * $EndLog$
*/

/* Copyright (C) 1991, 1996 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <stdio.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/lock.h>
#include <dcedfs/compat.h>
#ifdef AFS_OSF_ENV
#include <sys/types.h>
#endif /* AFS_OSF_ENV */
#include <dcedfs/nubik.h>
#include <dce/binding.h>
#include <dce/pgo.h>
#include <flserver.h>
#include "flinternal.h"

#include <dcedfs/icl.h>
#include <fl_trace.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfslsvmac.h>
#include <dfslsvsvc.h>
#include <dfslsvmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/flserver/flutils.c,v 1.1.74.1 1996/10/02 17:46:21 damon Exp $")

extern struct icl_set *fl_iclSetp;
extern int flserver_verbose;

extern struct vlheader    cheader;
extern struct ubik_dbase *VL_dbase;
extern osi_dlock_t vl_fsbLock;
extern struct siteBlock FirstSiteBlock;
extern struct vlconf_cell LocalCellInfo;
extern afs_hyper_t trialCellID;

#define	DOFFSET(abase,astr,aitem) ((abase)+(((char *)(aitem)) - ((char *)(astr))))

static int index_OK _TAKES((struct ubik_trans *trans,
			    long blockindex));

/* Hashing algorithm based on the volume id; HASHSIZE must be prime */
long IDHash(volumeid)
afs_hyper_t	*volumeid;
{
    long Res;

    if (AFS_hgethi(*volumeid) != 0) {
	if (AFS_hgetlo(*volumeid) != 0) {
	    Res = ((AFS_hgethi(*volumeid) % HASHSIZE) *
		   (AFS_hgetlo(*volumeid) % HASHSIZE)) % HASHSIZE;
	} else {
	    Res = AFS_hgethi(*volumeid) % HASHSIZE;
	}
    } else {
	Res = AFS_hgetlo(*volumeid) % HASHSIZE;
    }
    return (Res);
}


/*
 * Hashing algorithm based on the volume name; name's size is implicit
 * (64 chars) and if changed it should be reflected here.
 */
long NameHash (volumename)
register char	*volumename;
{
    register unsigned int   hash;
    register int	    i;

    hash = 0;
    for (i=strlen(volumename), volumename += i-1; i--; volumename--)
	hash = (hash*63) + (*((unsigned char *)volumename) - 63);
    return (hash % HASHSIZE);
}


/* package up seek and write into one procedure for ease of use */
long vlwrite (trans, offset, buffer, length)
struct ubik_trans   *trans;
long		    offset;
char		    *buffer;
long		    length;
{
    long	errorcode;

    icl_Trace4(fl_iclSetp, FL_TRACE_VLWRITE_ENTRY,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, offset,
	       ICL_TYPE_POINTER, buffer,
	       ICL_TYPE_LONG, length);
    if ((errorcode = ubik_Seek(trans, 0, offset)) == 0)
	errorcode = ubik_Write(trans, buffer, length);

    icl_Trace1(fl_iclSetp, FL_TRACE_VLWRITE_EXIT, ICL_TYPE_LONG, errorcode);
    return errorcode;
}


/* Package up seek and read into one procedure for ease of use */
long vlread (trans, offset, buffer, length)
struct ubik_trans   *trans;
long		    offset;
char		    *buffer;
long		    length;
{
    long	errorcode;

    icl_Trace4(fl_iclSetp, FL_TRACE_VLREAD_ENTRY,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, offset,
	       ICL_TYPE_POINTER, buffer,
	       ICL_TYPE_LONG, length);
    if ((errorcode = ubik_Seek(trans, 0, offset)) == 0)
	errorcode = ubik_Read(trans, buffer, length);

    icl_Trace1(fl_iclSetp, FL_TRACE_VLREAD_EXIT, ICL_TYPE_LONG, errorcode);
    return errorcode;
}

/* Convert *uuidp, in place, from network to host order. */
void ntohuuid(uuidp)
uuid_t *uuidp;
{
    uuidp->time_low = ntohl(uuidp->time_low);
    uuidp->time_mid = ntohs(uuidp->time_mid);
    uuidp->time_hi_and_version = ntohs(uuidp->time_hi_and_version);
}

/* Convert *uuidp, in place, from host to network order. */
void htonuuid(uuidp)
uuid_t *uuidp;
{
    uuidp->time_low = htonl(uuidp->time_low);
    uuidp->time_mid = htons(uuidp->time_mid);
    uuidp->time_hi_and_version = htons(uuidp->time_hi_and_version);
}

/* take entry and convert to network order and write to disk.
 *
 * NOTE -- volids are left in disk representation so don't need conversion. */

long vlentrywrite(trans, offset, buffer, length)
struct ubik_trans   *trans;
long		    offset;
char		    *buffer;
long		    length;
{
    struct vlentry tentry;
    register long i;
    long errorcode;

    icl_Trace4(fl_iclSetp, FL_TRACE_VLENTWRITE_ENTRY,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, offset,
	       ICL_TYPE_POINTER, buffer,
	       ICL_TYPE_LONG, length);
    if (length != sizeof(tentry)) {
	icl_Trace1(fl_iclSetp, FL_TRACE_VLENTWRITE_EXIT,
		   ICL_TYPE_LONG, VL_INCONSISTENT);
	return VL_INCONSISTENT;
    }
    bcopy(buffer, (char *)&tentry, sizeof(struct vlentry));
    tentry.reclaimDally = htonl(tentry.reclaimDally);
    tentry.flags = htonl(tentry.flags);
    tentry.LockTimestamp = htonl(tentry.LockTimestamp);
    for (i=0; i<MAXTYPES; ++i) {
	tentry.nextIdHash[i] = htonl(tentry.nextIdHash[i]);
    }
    tentry.nextNameHash = htonl(tentry.nextNameHash);
    tentry.maxTotalLatency = htonl(tentry.maxTotalLatency);
    tentry.hardMaxTotalLatency = htonl(tentry.hardMaxTotalLatency);
    tentry.minimumPounceDally = htonl(tentry.minimumPounceDally);
    tentry.defaultMaxReplicaLatency = htonl(tentry.defaultMaxReplicaLatency);
    tentry.nextNameHash = htonl(tentry.nextNameHash);
    for (i = 0; i < MAXNSERVERS; ++i) {
	tentry.serverSite[i] = htonl(tentry.serverSite[i]);
	tentry.serverPartition[i] = htonl(tentry.serverPartition[i]);
	tentry.sitemaxReplicaLatency[i] = htonl(tentry.sitemaxReplicaLatency[i]);
    }

    errorcode = vlwrite(trans, offset, (char *) &tentry, length);
    icl_Trace1(fl_iclSetp, FL_TRACE_VLENTWRITE_EXIT, ICL_TYPE_LONG, errorcode);

    return errorcode;
}

/* read entry and convert to host order.
 *
 * NOTE -- volids are left in disk representation. */

long vlentryread(trans, offset, buffer, length)
struct ubik_trans   *trans;
long		    offset;
char		    *buffer;
long		    length;
{
    struct vlentry tentry;
    register long i;

    icl_Trace4(fl_iclSetp, FL_TRACE_VLENTREAD_ENTRY,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, offset,
	       ICL_TYPE_POINTER, buffer,
	       ICL_TYPE_LONG, length);
    if (length != sizeof(tentry)) {
	icl_Trace1(fl_iclSetp, FL_TRACE_VLENTREAD_EXIT,
		   ICL_TYPE_LONG, VL_INCONSISTENT);
	return VL_INCONSISTENT;
    }
    i = vlread(trans, offset, (char *) &tentry, length);
    if (i) {
	icl_Trace1(fl_iclSetp, FL_TRACE_VLENTREAD_EXIT,
		   ICL_TYPE_LONG, i);
	return i;
    }
    tentry.reclaimDally = ntohl(tentry.reclaimDally);
    tentry.flags = ntohl(tentry.flags);
    tentry.LockTimestamp = ntohl(tentry.LockTimestamp);
    for (i = 0; i < MAXTYPES; ++i) {
	tentry.nextIdHash[i] = ntohl(tentry.nextIdHash[i]);
    }
    tentry.nextNameHash = ntohl(tentry.nextNameHash);
    tentry.maxTotalLatency = ntohl(tentry.maxTotalLatency);
    tentry.hardMaxTotalLatency = ntohl(tentry.hardMaxTotalLatency);
    tentry.minimumPounceDally = ntohl(tentry.minimumPounceDally);
    tentry.defaultMaxReplicaLatency = ntohl(tentry.defaultMaxReplicaLatency);
    tentry.nextNameHash = ntohl(tentry.nextNameHash);
    for (i = 0; i < MAXNSERVERS; ++i) {
	tentry.serverSite[i] = ntohl(tentry.serverSite[i]);
	tentry.serverPartition[i] = ntohl(tentry.serverPartition[i]);
	tentry.sitemaxReplicaLatency[i] = ntohl(tentry.sitemaxReplicaLatency[i]);
    }
    bcopy((char *)&tentry, buffer, sizeof(struct vlentry));
    icl_Trace1(fl_iclSetp, FL_TRACE_VLENTREAD_EXIT,
	       ICL_TYPE_LONG, 0);
    return 0;
}

/* Convenient write of small critical vldb header info to the database. */
int write_vital_vlheader(trans)
register struct	ubik_trans  *trans;
{
    long errorcode;

    icl_Trace1(fl_iclSetp, FL_TRACE_WRVITAL, ICL_TYPE_POINTER, trans);
    errorcode = vlwrite(trans, 0, (char *) &cheader.vital_header,
			sizeof(vital_vlheader));
    icl_Trace1(fl_iclSetp, FL_TRACE_WRVITAL_EXIT, ICL_TYPE_LONG, errorcode);

    return errorcode;
}


/*
 * Check that the database has been initialized.  Be careful to fail in a safe
 * manner, to avoid bogusly reinitializing the db.
 */
long CheckInit (trans)
struct ubik_trans   *trans;
{
    register long   errorcode;

    icl_Trace1(fl_iclSetp, FL_TRACE_CHK_INIT, ICL_TYPE_POINTER, trans);
    errorcode = 0;
    /* ubik_CacheUpdate must be called on every transaction.  It returns 0 if
     * the previous transaction would have left the cache fine, and non-zero
     * otherwise.
     * Thus, a local abort or a remote commit will cause this to return non-zero
     * and force a header re-read.  Necessary for a local abort because we may
     * have damaged cheader during the operation.  Necessary for a remote commit
     * since it may have changed cheader.
     */
    icl_Trace0(fl_iclSetp, FL_TRACE_CI_CHK_UPDATE);
    if (ubik_CacheUpdate(trans) != 0) {
	icl_Trace0(fl_iclSetp, FL_TRACE_CI_REREAD);
	/* if version changed (or first call), read the header */
	errorcode = vlread (trans, 0, (char *) &cheader, sizeof(cheader));
	if (errorcode == 0) {
	    lock_ObtainWrite(&vl_fsbLock);
	    errorcode = vlread(trans, ntohl(cheader.vital_header.headersize),
			       (char *)&FirstSiteBlock,
			       sizeof(FirstSiteBlock));
	    if (errorcode == 0) {
		DFSH_MemFromNetHyper(LocalCellInfo.CellID,
				     cheader.vital_header.theCellId);
		FirstSiteBlock.NextPtr = ntohl(FirstSiteBlock.NextPtr);
		FirstSiteBlock.AllocHere = ntohl(FirstSiteBlock.AllocHere);
		FirstSiteBlock.UsedHere = ntohl(FirstSiteBlock.UsedHere);
		if (FirstSiteBlock.Tag != htonl(SITEBLOCKTAG)) errorcode = 1;
	    }
	    lock_ReleaseWrite(&vl_fsbLock);
	}
    }
    /* now, if can't read, or header is wrong, write a new header */
    if (errorcode || ntohl(cheader.vital_header.vldbversion) != VLDBVERSION) {
	/*
	 * if here, we have no version number or the wrong version number
	 * in the file
	 */
        error_status_t st;
        unsigned char   *cmd_explanation;
 
        cmd_explanation = dce_msg_get_msg(lsv_s_initing_FLDB_header, &st);
        Log( (char *) cmd_explanation);
        free(cmd_explanation);
 
	if (flserver_verbose)
	  dce_fprintf(stderr, lsv_s_initing_fldb_header1);
	icl_Trace0(fl_iclSetp, FL_TRACE_CI_WR_HEADER);

	/* try to write a good header */
	bzero((char *)&cheader,sizeof(cheader));
	cheader.vital_header.vldbversion = htonl(VLDBVERSION);
	cheader.vital_header.headersize = htonl(sizeof(cheader));
	{   afs_hyper_t hone = AFS_HINIT(0,1);
	    DFSH_NetFromMemHyper(cheader.vital_header.maxVolumeId, hone);
	}
	cheader.vital_header.eofPtr =
	    htonl(sizeof(cheader) + sizeof(FirstSiteBlock));
	cheader.vital_header.sitesPtr = htonl(sizeof(cheader));
	DFSH_NetFromMemHyper(cheader.vital_header.theCellId, trialCellID);
	errorcode = vlwrite(trans, 0, (char *) &cheader, sizeof(cheader));
	if (errorcode) {
	  if (flserver_verbose)
	    dce_fprintf(stderr, lsv_s_fldb_hdr_init_failed, errorcode,
			 dfs_dceErrTxt(errorcode));

	    cmd_explanation = dce_msg_get_msg(lsv_s_FLDB_hdr_init_failed1, &st);
	    Log( (char *) cmd_explanation, errorcode, dfs_dceErrTxt(errorcode));
	    free(cmd_explanation);

	    icl_Trace1(fl_iclSetp, FL_TRACE_CI_WR_FAILED, ICL_TYPE_LONG, errorcode);
	    return errorcode;
	}
	LocalCellInfo.CellID = trialCellID;
	lock_ObtainWrite(&vl_fsbLock);
	bzero((char *)&FirstSiteBlock, sizeof(FirstSiteBlock));
	FirstSiteBlock.Tag = htonl(SITEBLOCKTAG);
	FirstSiteBlock.AllocHere = htonl(SITESINBLOCK);
	errorcode = vlwrite(trans, sizeof(cheader),
			    (char *) &FirstSiteBlock, sizeof(FirstSiteBlock));
	FirstSiteBlock.AllocHere = SITESINBLOCK;
	lock_ReleaseWrite(&vl_fsbLock);
	if (errorcode) {
	  if (flserver_verbose)
	    dce_fprintf(stderr, lsv_s_fldb_hdr_init_failed2, errorcode,
			 dfs_dceErrTxt(errorcode));

	    cmd_explanation = dce_msg_get_msg(lsv_s_FLDB_hdr_init_failed_2, &st);
	    Log( (char *) cmd_explanation, errorcode, dfs_dceErrTxt(errorcode));
	    free(cmd_explanation);

	    icl_Trace1(fl_iclSetp, FL_TRACE_CI_WR_FAILED, ICL_TYPE_LONG, errorcode);
	    return errorcode;
	}
	if (flserver_verbose)
	  dce_fprintf(stderr, lsv_s_fldb_hdr_initialized, SITESINBLOCK,
		      AFS_HGETBOTH(trialCellID));

	cmd_explanation = dce_msg_get_msg(lsv_s_FLDB_hdr_initialized1, &st);
	Log( (char *) cmd_explanation, SITESINBLOCK, AFS_HGETBOTH(trialCellID));
	free(cmd_explanation);

    }
    /* all done */

    icl_Trace0(fl_iclSetp, FL_TRACE_CI_EXIT);
    return 0;
}


/*
 * Allocate a free block of storage for entry, returning address of a new
 * zeroed entry (or zero if something is wrong).
 */
long AllocBlock (trans, tentry, errp)
register struct ubik_trans  *trans;
struct vlentry		    *tentry;
long *errp;
{
    register long    blockindex;
    long err;

    icl_Trace3(fl_iclSetp, FL_TRACE_ALLOC_BLK, 
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_POINTER, tentry,
	       ICL_TYPE_POINTER, errp);
    if (cheader.vital_header.freePtr != 0) {
	/* allocate this dude */
	blockindex = ntohl(cheader.vital_header.freePtr);
	err = vlentryread(trans, blockindex, (char *) tentry, sizeof(vlentry_t));
	if (err) {
	    *errp = err;
	    icl_Trace1(fl_iclSetp, FL_TRACE_AB_RD_EXIT, ICL_TYPE_LONG, err);
	    return 0;
	}
	cheader.vital_header.freePtr = htonl(tentry->nextIdHash[0]);
    } else {
	/* hosed, nothing on free list, grow file, remember old eof */
	blockindex = ntohl(cheader.vital_header.eofPtr);
	cheader.vital_header.eofPtr = htonl(blockindex + sizeof(vlentry_t));
    }
    cheader.vital_header.allocs = htonl(ntohl(cheader.vital_header.allocs)+1);
    err = write_vital_vlheader(trans);
    if (err) {
	*errp = err;
	icl_Trace1(fl_iclSetp, FL_TRACE_AB_WR_EXIT, ICL_TYPE_LONG, err);
	return 0;
    }
    bzero ((char *)tentry, sizeof(vlentry_t));	/* zero new entry */
    if (blockindex == 0) {
	*errp = VL_INCONSISTENT; 
	icl_Trace0(fl_iclSetp, FL_TRACE_AB_NB_EXIT);
	return 0;
    }
    icl_Trace1(fl_iclSetp, FL_TRACE_AB_EXIT, ICL_TYPE_LONG, blockindex);
    return blockindex;
}


/*
 * Free a block given its index.  It must already have been unthreaded.
 * Returns zero for success or an error code on failure.
 */
int FreeBlock (trans, blockindex)
struct ubik_trans	*trans;
long			blockindex;
{
    struct vlentry	tentry;
    long errorcode;

    icl_Trace2(fl_iclSetp, FL_TRACE_FREE_BLK, 
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, blockindex);
    /* check validity of blockindex just to be on the safe side */
    if (!index_OK(trans, blockindex)) {
	icl_Trace1(fl_iclSetp, FL_TRACE_FB_EXIT, ICL_TYPE_LONG, VL_BADINDEX);
	return VL_BADINDEX;
    }
    bzero ((char *)&tentry, sizeof(vlentry_t));
    /* Don't need htonl in next statement; already in network order */
    tentry.nextIdHash[0] = cheader.vital_header.freePtr;
    tentry.flags = htonl(VLFREE);
    cheader.vital_header.freePtr = htonl(blockindex);
    errorcode = vlwrite (trans, blockindex, (char *)&tentry, sizeof(vlentry_t));
    if (errorcode) {
	icl_Trace1(fl_iclSetp, FL_TRACE_FB_EXIT, ICL_TYPE_LONG, errorcode);
	return errorcode;
    }
    /* cheader.vital_header.frees++; */
    cheader.vital_header.frees = htonl(ntohl(cheader.vital_header.frees)+1);
    errorcode = write_vital_vlheader(trans);
    icl_Trace1(fl_iclSetp, FL_TRACE_FB_EXIT, ICL_TYPE_LONG, errorcode);
    return errorcode;
}


/*
 * Look for a block by volid and voltype (if not known use -1 which searches
 * all 3 volid hash lists. Note that the linked lists are read in first from
 * the database header.  If found read the block's contents into the area
 * pointed to by tentry and return the block's index.  If not found return 0.
 */
long FindByID (trans, volid, voltix, tentry, error)
struct ubik_trans   *trans;
afs_hyper_t	    *volid;
long		    voltix;
struct vlentry	    *tentry;
long		    *error;
{
    register long   typeindex, hashindex, blockindex;
    register long errcode;

    icl_Trace4(fl_iclSetp, FL_TRACE_FIND_ID,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_HYPER, volid,
	       ICL_TYPE_LONG, voltix,
	       ICL_TYPE_POINTER, tentry);
    *error = VL_INCONSISTENT;
    hashindex = IDHash(volid);
    icl_Trace1(fl_iclSetp, FL_TRACE_FI_HASH, ICL_TYPE_LONG, hashindex);
    if (voltix >= MAXTYPES) {
	dce_svc_printf(LSV_S_BUG_VOLTYPE_NOT_CONVERTED_MSG, voltix);
	icl_Trace2(fl_iclSetp, FL_TRACE_FIND_ID_EXIT,
		   ICL_TYPE_LONG, 0, ICL_TYPE_LONG, *error);
	return 0;
    }
    if (voltix == -1) {
	/*
	 * Should we have one big hash table for volids as opposed to
	 * the three ones?
	 */
	for (typeindex = 0; typeindex < MAXTYPES; typeindex++) {
	    for (blockindex = ntohl(cheader.VolidHash[typeindex][hashindex]);
		 blockindex != NULLO;
		 blockindex = tentry->nextIdHash[typeindex]) {
		afs_hyper_t temp;
		errcode = vlentryread(trans, blockindex,
				      (char *) tentry, sizeof(vlentry_t));
		if (errcode) {
		    *error = errcode;
		    icl_Trace2(fl_iclSetp, FL_TRACE_FIND_ID_EXIT,
			       ICL_TYPE_LONG, 0, ICL_TYPE_LONG, *error);
		    return 0;
		}
		DFSH_MemFromNetHyper(temp, tentry->volumeId[typeindex]);
		if (AFS_hsame(*volid, temp)) {
		    icl_Trace2(fl_iclSetp, FL_TRACE_FIND_ID_EXIT,
			       ICL_TYPE_LONG, blockindex, ICL_TYPE_LONG, *error);
		    return blockindex;
		}
	    }
	}
    } else {
	for (blockindex = ntohl(cheader.VolidHash[voltix][hashindex]);
	     blockindex != NULLO;
	     blockindex = tentry->nextIdHash[voltix]) {
	    afs_hyper_t temp;

	    errcode = vlentryread(trans, blockindex,
				  (char *) tentry, sizeof(vlentry_t));
	    if (errcode) {
		*error = errcode;
		icl_Trace2(fl_iclSetp, FL_TRACE_FIND_ID_EXIT,
			   ICL_TYPE_LONG, 0, ICL_TYPE_LONG, *error);
		return 0;
	    }
	    DFSH_MemFromNetHyper(temp, tentry->volumeId[voltix]);
	    if (AFS_hsame(*volid, temp)) {
		icl_Trace2(fl_iclSetp, FL_TRACE_FIND_ID_EXIT,
			   ICL_TYPE_LONG, blockindex, ICL_TYPE_LONG, *error);
		return blockindex;
	    }
	}
    }
    *error = VL_NOENT;
    icl_Trace2(fl_iclSetp, FL_TRACE_FIND_ID_EXIT,
	       ICL_TYPE_LONG, 0, ICL_TYPE_LONG, *error);
    return 0;				/* no such entry */
}


/*
 * Look for a block by volume name. If found read the block's contents into
 * the area pointed to by tentry and return the block's index.  If not found
 * return 0. */
long FindByName (trans, volname, tentry, error)
struct ubik_trans   *trans;
char		    *volname;
struct vlentry	    *tentry;
long		    *error;
{
    register long   hashindex;
    register long   blockindex;
    register long errCode;
    char tname[MAXFTNAMELEN];

    icl_Trace4(fl_iclSetp, FL_TRACE_FIND_NAME,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_STRING, volname,
	       ICL_TYPE_POINTER, tentry,
	       ICL_TYPE_POINTER, error);
    /* remove .backup or .readonly extensions for backwards compatibility */
    strcpy(tname, volname);
    hashindex = strlen(tname);    /* really string length */
    if (hashindex >= 8 && strcmp(tname+hashindex-7, ".backup")==0) {
	/* this is a backup volume */
	tname[hashindex-7] = 0; /* zap extension */
    }
    else if (hashindex >= 10 && strcmp(tname+hashindex-9, ".readonly")==0) {
	/* this is a readonly volume */
	tname[hashindex-9] = 0; /* zap extension */
    }

    *error = 0;
    hashindex = NameHash(tname);
    icl_Trace1(fl_iclSetp, FL_TRACE_FN_HASH, ICL_TYPE_LONG, hashindex);
    for (blockindex = ntohl(cheader.VolnameHash[hashindex]);
	 blockindex != NULLO;
	 blockindex = tentry->nextNameHash) {
	errCode = vlentryread(trans, blockindex, (char *) tentry, sizeof(vlentry_t));
	if (errCode) {
	    *error = errCode;
	    icl_Trace2(fl_iclSetp, FL_TRACE_FIND_NAME_EXIT,
		       ICL_TYPE_LONG, 0, ICL_TYPE_LONG, *error);
	    return 0;
	}
	if (!strcmp(tname, tentry->name)) {
	    icl_Trace2(fl_iclSetp, FL_TRACE_FIND_NAME_EXIT,
		       ICL_TYPE_LONG, blockindex, ICL_TYPE_LONG, *error);
	    return blockindex;
	}
    }
    *error = VL_NOENT;
    icl_Trace2(fl_iclSetp, FL_TRACE_FIND_NAME_EXIT,
	       ICL_TYPE_LONG, 0, ICL_TYPE_LONG, *error);
    return 0;				/* no such entry */
}


/*
 * Add a block to the hash table given a pointer to the block and its index.
 * The block is threaded onto both hash tables and written to disk.
 * The routine returns zero if there were no errors.
 */
int ThreadVLentry (trans, blockindex, tentry)
struct ubik_trans   *trans;
long		    blockindex;
struct vlentry	    *tentry;
{
    int		    errorcode;

    icl_Trace3(fl_iclSetp, FL_TRACE_THR_ENT,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, blockindex,
	       ICL_TYPE_POINTER, tentry);
    if (!index_OK(trans, blockindex)) {
	icl_Trace1(fl_iclSetp, FL_TRACE_THR_ENT_EXIT, ICL_TYPE_LONG, VL_BADINDEX);
	return VL_BADINDEX;
    }
    /* Insert into volid's hash linked list */
    /* should we "assert" than RWVolId isn't zero? -ota 960206 */
    if (errorcode = HashVolid(trans, RWVOL, blockindex, tentry)) {
	icl_Trace1(fl_iclSetp, FL_TRACE_THR_ENT_EXIT, ICL_TYPE_LONG, errorcode);
	return errorcode;
    }

    /*
     * For rw entries we also enter the RO and BACK volume ids (if they exist)
     * in the hash tables; note all there volids (RW, RO, BACK) should not be
     * hashed yet!
     */
    if (errorcode = HashVolid(trans, ROVOL, blockindex, tentry)) {
	icl_Trace1(fl_iclSetp, FL_TRACE_THR_ENT_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return errorcode;
    }   
    if (errorcode = HashVolid(trans, BACKVOL, blockindex, tentry)) {
	icl_Trace1(fl_iclSetp, FL_TRACE_THR_ENT_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return errorcode;
    }

    /* Insert into volname's hash linked list */
    HashVolname(trans, blockindex, tentry);

    /* Update cheader entry */
    errorcode = write_vital_vlheader(trans);
    if (errorcode) {
	icl_Trace1(fl_iclSetp, FL_TRACE_THR_ENT_EXIT, ICL_TYPE_LONG, errorcode);
	return errorcode;
    }

    /* Update hash list pointers in the entry itself */
    errorcode = vlentrywrite(trans, blockindex, (char *)tentry, sizeof(vlentry_t));
    icl_Trace1(fl_iclSetp, FL_TRACE_THR_ENT_EXIT, ICL_TYPE_LONG, errorcode);

    return errorcode;
}


/*
 * Remove a block from both the hash tables.
 * If success return 0, else return an error code.
 */
int UnthreadVLentry (trans, blockindex, aentry)
struct ubik_trans   *trans;
long		    blockindex;
struct vlentry	    *aentry;
{
    register long   errorcode, typeindex;

    icl_Trace3(fl_iclSetp, FL_TRACE_UNTHR_ENT,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, blockindex,
	       ICL_TYPE_POINTER, aentry);
    if (!index_OK(trans, blockindex)) {
	icl_Trace1(fl_iclSetp, FL_TRACE_UNTHR_ENT_EXIT, ICL_TYPE_LONG, VL_BADINDEX);
	return VL_BADINDEX;
    }
    if (errorcode = UnhashVolid(trans, RWVOL, blockindex, aentry)) {
	icl_Trace1(fl_iclSetp, FL_TRACE_UNTHR_ENT_EXIT, ICL_TYPE_LONG, errorcode);
	return errorcode;
    }

    /* Take the RO/RW entries off their respective hash linked lists. */
    for (typeindex = ROVOL; typeindex < MAXTYPES; typeindex++) {
	if (errorcode = UnhashVolid(trans, typeindex, blockindex, aentry)) {
	    icl_Trace1(fl_iclSetp, FL_TRACE_UNTHR_ENT_EXIT, ICL_TYPE_LONG, errorcode);
	    return errorcode;
	}
    }

    /* Take it out of the Volname hash list */
    if (errorcode = UnhashVolname(trans, blockindex, aentry)) {
	icl_Trace1(fl_iclSetp, FL_TRACE_UNTHR_ENT_EXIT, ICL_TYPE_LONG, errorcode);
	return errorcode;
    }

    /* Update cheader entry */
    errorcode = write_vital_vlheader(trans);
    icl_Trace1(fl_iclSetp, FL_TRACE_UNTHR_ENT_EXIT, ICL_TYPE_LONG, errorcode);

    return errorcode;
}

/* cheader must have be read before this routine is called.  Just a no-op if
 * volid is zero. */
int HashVolid(trans, voltix, blockindex, tentry)
struct ubik_trans   *trans;
long		    voltix;
long		    blockindex;
struct vlentry	    *tentry;
{
    long hashindex, errorcode;
    struct vlentry ventry;
    afs_hyper_t currLimit;
    afs_hyper_t volid;

    icl_Trace4(fl_iclSetp, FL_TRACE_HASH_ID,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, voltix,
	       ICL_TYPE_LONG, blockindex,
	       ICL_TYPE_POINTER, tentry);
    DFSH_MemFromNetHyper(volid, tentry->volumeId[voltix]);
    if (AFS_hiszero(volid))
	return 0;
    DFSH_MemFromNetHyper(currLimit, cheader.vital_header.maxVolumeId);
    /* CFE 30 Apr 91: need to reserve fileset IDs before using them */
    if (AFS_hcmp(volid, currLimit) >= 0) {
	icl_Trace1(fl_iclSetp, FL_TRACE_HASH_ID_EXIT, ICL_TYPE_LONG, VL_BADID);
	return VL_BADID;
    }
    /* CFE 30 Apr 91: check all the hash tables, not just the indicated one */
    if (FindByID(trans, &volid, -1 /*voltix*/, &ventry, &errorcode)) {
	icl_Trace1(fl_iclSetp, FL_TRACE_HASH_ID_EXIT,
		   ICL_TYPE_LONG, VL_IDALREADYHASHED);
	return VL_IDALREADYHASHED;
    }
    else if (errorcode != VL_NOENT) {
	icl_Trace1(fl_iclSetp, FL_TRACE_HASH_ID_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return errorcode;
    }
    hashindex = IDHash(&volid);
    tentry->nextIdHash[voltix] = ntohl(cheader.VolidHash[voltix][hashindex]);
    cheader.VolidHash[voltix][hashindex] = htonl(blockindex);
    errorcode =
	vlwrite(trans,
		DOFFSET(0, &cheader, &cheader.VolidHash[voltix][hashindex]),
		(char *) &cheader.VolidHash[voltix][hashindex],
		sizeof(long));
    icl_Trace1(fl_iclSetp, FL_TRACE_HASH_ID_EXIT, ICL_TYPE_LONG, errorcode);
    return errorcode;
}


/* cheader must have be read before this routine is called. */
int UnhashVolid(trans, voltix, blockindex, aentry)
struct ubik_trans   *trans;
long		    voltix;
long		    blockindex;
struct vlentry	    *aentry;
{
    int hashindex, nextblockindex, prevblockindex;
    struct vlentry tentry;
    afs_hyper_t volid;
    long code;
    long temp;

    icl_Trace4(fl_iclSetp, FL_TRACE_UNHASH_ID,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, voltix,
	       ICL_TYPE_LONG, blockindex,
	       ICL_TYPE_POINTER, aentry);
    DFSH_MemFromNetHyper(volid, aentry->volumeId[voltix]);
    if (AFS_hiszero(volid)) {
	/* Assume no volume id */
	icl_Trace1(fl_iclSetp, FL_TRACE_UNHASH_ID_EXIT, ICL_TYPE_LONG, 0);
	return 0;
    }
    /* Take it out of the VolId[voltix] hash list */
    hashindex = IDHash(&volid);
    nextblockindex = ntohl(cheader.VolidHash[voltix][hashindex]);
    if (nextblockindex == blockindex) {
	/* First on the hash list; just adjust pointers */
	cheader.VolidHash[voltix][hashindex] = htonl(aentry->nextIdHash[voltix]);
	code =
	    vlwrite(trans,
		    DOFFSET(0, &cheader, &cheader.VolidHash[voltix][hashindex]),
		    (char *) &cheader.VolidHash[voltix][hashindex],
		    sizeof(long));
	if (code) {
	    icl_Trace1(fl_iclSetp, FL_TRACE_UNHASH_ID_EXIT, ICL_TYPE_LONG, code);
	    return code;
	}
    } else {
	while (nextblockindex != blockindex) {
	    prevblockindex = nextblockindex;	/* always done at least once */
	    code = vlentryread(trans, nextblockindex,
			       (char *)&tentry, sizeof(vlentry_t));
	    if (code) {
		icl_Trace1(fl_iclSetp, FL_TRACE_UNHASH_ID_EXIT, ICL_TYPE_LONG, code);
		return code;
	    }
	    if ((nextblockindex = tentry.nextIdHash[voltix]) == NULLO) {
		icl_Trace1(fl_iclSetp, FL_TRACE_UNHASH_ID_EXIT, ICL_TYPE_LONG, VL_NOENT);
		return VL_NOENT;
	    }
	}
	temp = tentry.nextIdHash[voltix] = aentry->nextIdHash[voltix];
	temp = htonl(temp); /* convert to network byte order before writing */
	code =
	    vlwrite(trans,
		    DOFFSET(prevblockindex, &tentry, &tentry.nextIdHash[voltix]),
		    (char *)&temp,
		    sizeof(long));
	if (code) {
	    icl_Trace1(fl_iclSetp, FL_TRACE_UNHASH_ID_EXIT, ICL_TYPE_LONG, code);
	    return code;
	}
    }
    aentry->nextIdHash[voltix] = 0;
    icl_Trace1(fl_iclSetp, FL_TRACE_UNHASH_ID_EXIT, ICL_TYPE_LONG, 0);
    return 0;
}


int HashVolname(trans, blockindex, aentry)
struct ubik_trans   *trans;
long		    blockindex;
struct vlentry	    *aentry;
{
    register long   hashindex;
    long code;
    struct vlentry ventry;

    icl_Trace3(fl_iclSetp, FL_TRACE_HASH_NAME,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, blockindex,
	       ICL_TYPE_POINTER, aentry);
    if (FindByName(trans, aentry->name, &ventry, &code)) {
	icl_Trace1(fl_iclSetp, FL_TRACE_HASH_NAME_EXIT, ICL_TYPE_LONG, VL_IDALREADYHASHED);
	return VL_IDALREADYHASHED;
    }
    else if (code != VL_NOENT) {
	icl_Trace1(fl_iclSetp, FL_TRACE_HASH_NAME_EXIT, ICL_TYPE_LONG, code);
	return code;
    }
    /* Insert into volname's hash linked list */
    hashindex = NameHash(aentry->name);
    aentry->nextNameHash = ntohl(cheader.VolnameHash[hashindex]);
    cheader.VolnameHash[hashindex] = htonl(blockindex);
    code = vlwrite(trans, DOFFSET(0, &cheader, &cheader.VolnameHash[hashindex]),
		   (char *) &cheader.VolnameHash[hashindex], sizeof(long));
    icl_Trace1(fl_iclSetp, FL_TRACE_HASH_NAME_EXIT, ICL_TYPE_LONG, code);
    return code;
}


int UnhashVolname(trans, blockindex, aentry)
struct ubik_trans   *trans;
long		    blockindex;
struct vlentry	    *aentry;
{
    register long   hashindex, nextblockindex, prevblockindex;
    struct vlentry  tentry;
    long temp, errCode;

    icl_Trace3(fl_iclSetp, FL_TRACE_UNHASH_NAME,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, blockindex,
	       ICL_TYPE_POINTER, aentry);
    /* Take it out of the Volname hash list */
    hashindex = NameHash(aentry->name);
    nextblockindex = ntohl(cheader.VolnameHash[hashindex]);
    if (nextblockindex == blockindex) {
	/* First on the hash list; just adjust pointers */
	cheader.VolnameHash[hashindex] = htonl(aentry->nextNameHash);
	errCode = vlwrite(trans,
			  DOFFSET(0, &cheader, &cheader.VolnameHash[hashindex]),
			  (char *) &cheader.VolnameHash[hashindex], sizeof(long));
	if (errCode) {
	    icl_Trace1(fl_iclSetp, FL_TRACE_UNHASH_NAME_EXIT, ICL_TYPE_LONG, errCode);
	    return errCode;
	}
    } else {
	while (nextblockindex != blockindex) {
	    prevblockindex = nextblockindex;	/* always done at least once */
	    errCode = vlentryread(trans, nextblockindex,
				  (char *)&tentry, sizeof(vlentry_t));
	    if (errCode) {
		icl_Trace1(fl_iclSetp, FL_TRACE_UNHASH_NAME_EXIT, ICL_TYPE_LONG, errCode);
		return errCode;
	    }
	    if ((nextblockindex = tentry.nextNameHash) == NULLO) {
		icl_Trace1(fl_iclSetp, FL_TRACE_UNHASH_NAME_EXIT, ICL_TYPE_LONG, VL_NOENT);
		return VL_NOENT;
	    }
	}
	tentry.nextNameHash = aentry->nextNameHash;
	temp = htonl(tentry.nextNameHash);
	errCode =
	    vlwrite(trans,
		    DOFFSET(prevblockindex, &tentry, &tentry.nextNameHash),
		    (char *)&temp, sizeof(long));
	if (errCode) {
	    icl_Trace1(fl_iclSetp, FL_TRACE_UNHASH_NAME_EXIT, ICL_TYPE_LONG, errCode);
	    return errCode;
	}
    }
    aentry->nextNameHash = 0;
    icl_Trace1(fl_iclSetp, FL_TRACE_UNHASH_NAME_EXIT, ICL_TYPE_LONG, 0);
    return 0;
}


/*
 * Returns the vldb entry tentry at offset index; remaining is the number of
 * entries left; the routine also returns the index of the next sequential
 * entry in the vldb
 */
long NextEntry (trans, blockindex, tentry, remaining)
struct ubik_trans   *trans;
long		    blockindex;
struct vlentry	    *tentry;
long		    *remaining;
{
    register long    lastblockindex;
    afs_hyper_t curMax, volid;

    icl_Trace4(fl_iclSetp, FL_TRACE_NEXT_ENT,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, blockindex,
	       ICL_TYPE_POINTER, tentry,
	       ICL_TYPE_POINTER, remaining);
    if (blockindex == 0) {		/* get first one */
	blockindex = sizeof(cheader) + sizeof(FirstSiteBlock);
    } else {
	if (!index_OK(trans, blockindex)) {
	    *remaining = -1;		/* error */
	    icl_Trace2(fl_iclSetp, FL_TRACE_NXTENT_EXIT, ICL_TYPE_LONG, 0,
		       ICL_TYPE_LONG, -1);
	    return 0;
	}
	blockindex += sizeof(vlentry_t);
    }
    /* can't go beyond vlentry distance from EOFLDB */
    lastblockindex = ntohl(cheader.vital_header.eofPtr);
    /* now search for the first entry that isn't free */
    while (blockindex <= (lastblockindex-sizeof(vlentry_t))) {
      if (vlentryread(trans, blockindex, (char *)tentry, sizeof(vlentry_t))) {
	*remaining = -1;
	icl_Trace2(fl_iclSetp, FL_TRACE_NXTENT_EXIT, ICL_TYPE_LONG, 0,
		   ICL_TYPE_LONG, -1);
	return 0;
      }
      if (tentry->reclaimDally == SITEBLOCKTAG) {
	/* This one was really a siteBlock! */
	blockindex += sizeof(struct siteBlock);
      } else if (tentry->flags == VLFREE) { /* free block */
	blockindex += sizeof(vlentry_t);
      } else {
	/* sanity check on volume id field */
	DFSH_MemFromNetHyper(curMax, cheader.vital_header.maxVolumeId);
	DFSH_MemFromNetHyper(volid, tentry->volumeId[0]);
	if (AFS_hcmp(volid, curMax) >= 0) {
	  *remaining = -1;
	  icl_Trace2(fl_iclSetp, FL_TRACE_NXTENT_EXIT, ICL_TYPE_LONG, 0,
		     ICL_TYPE_LONG, -1);
	  return 0;
	}
	/* estimate remaining number of entries, not including this one */
	*remaining = (lastblockindex - blockindex) / sizeof(vlentry_t) - 1;
	icl_Trace2(fl_iclSetp, FL_TRACE_NXTENT_EXIT, ICL_TYPE_LONG, 
		   blockindex, ICL_TYPE_LONG, *remaining);
	return blockindex;
      }
    }
    *remaining = 0;			/* no more entries */
    icl_Trace2(fl_iclSetp, FL_TRACE_NXTENT_EXIT, ICL_TYPE_LONG, 0,
		       ICL_TYPE_LONG, 0);
    return 0;
}


/* Routine to verify that index is a legal offset to a vldb entry in table */
static int index_OK(trans, blockindex)
struct ubik_trans   *trans;
long		    blockindex;
{
    if ((blockindex < (sizeof(cheader)+sizeof(FirstSiteBlock)))
	|| (blockindex > (ntohl(cheader.vital_header.eofPtr)-sizeof(vlentry_t)))
	 /*|| ((blockindex - sizeof(cheader)) % sizeof(vlentry_t) != 0) */ )
      return 0;
    return 1;
}

/*
 * check that "prefix"/dfs-server exists in the registry
 */
error_status_t
PrincipalExists(prefix)
    char *prefix;
{
   sec_rgy_handle_t sechandle;
   sec_rgy_bind_auth_info_t authinfo;
   sec_rgy_cursor_t  cursor;
   sec_rgy_pgo_item_t pgo_item;
   error_status_t   st, st2;
   sec_rgy_name_t princp;   
   size_t prefixlen;
   
   prefixlen = strlen(prefix);
   if (prefixlen + strlen("/dfs-server") > sec_rgy_name_t_size)
       return(VL_LONGPRINCNAME);
   memcpy(princp, prefix, prefixlen);
   if (princp[prefixlen] != '/')
       strcpy((char *)&princp[prefixlen], "/dfs-server");
   else
       strcpy((char *)&princp[prefixlen], "dfs-server");

   sec_rgy_site_open((unsigned_char_t *) NULL, &sechandle, &st);
   if (st != 0)
       return(st);
   sec_rgy_cursor_reset(&cursor);
   sec_rgy_pgo_get_by_name(sechandle, sec_rgy_domain_person,
                           princp, &cursor, &pgo_item, &st);
   sec_rgy_site_close(sechandle, &st2);
   return(st);
}




