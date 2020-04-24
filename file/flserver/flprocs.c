/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: flprocs.c,v $
 * Revision 1.1.101.1  1996/10/02  17:46:10  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:37:04  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1991 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/stds.h>
#include <dcedfs/common_data.h>
#include <dcedfs/osi.h>
#include <dcedfs/lock.h>
#ifndef AFS_DEBUG
/* turn it on for d*printf stuff */
#define	AFS_DEBUG   1
#endif /* AFS_DEBUG */
#include <dcedfs/debug.h>

#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>

#include <dcedfs/dfsauth.h>

#include <flserver.h>
#include <fl_opcodes.h>
#include <fldb_proc.h>
#include "flinternal.h"

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfslsvmac.h>
#include <dfslsvsvc.h>
#include <dfslsvmsg.h>

#include <dcedfs/icl.h>
#include <fl_trace.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/flserver/flprocs.c,v 1.1.101.1 1996/10/02 17:46:10 damon Exp $")

extern struct icl_set *fl_iclSetp;

extern struct ubik_dbase *VL_dbase;
struct vlheader cheader;		/* kept in network byte order */
osi_dlock_t vl_fsbLock;
extern struct siteBlock FirstSiteBlock;
extern int flserver_verbose;

osi_dlock_t globalLock;
osi_dlock_t statLock;

error_status_t st;
unsigned char   *cmd_explanation;

char dce_flserver_debug = 0;

/* protect server against thread cancels */
static int savedCancelState, cancelCode;

#define STATLOCK lock_ObtainWrite(&statLock)
#define STATUNLOCK lock_ReleaseWrite(&statLock)

#define COUNT_REQ(op) \
    STATLOCK; dynamic_statistics.requests[(op)-VL_LOWEST_OPCODE]++; STATUNLOCK
#define COUNT_ABO(op) \
    STATLOCK; dynamic_statistics.aborts[(op)-VL_LOWEST_OPCODE]++; STATUNLOCK

#define GLOBALLOCK 	\
MACRO_BEGIN		\
DFS_DISABLE_CANCEL(&savedCancelState, cancelCode);	\
if (cancelCode) 					\
   dlprintf(dce_flserver_debug,1,("fls: DFS_DISABLE_CANCEL: errno %lu\n", \
				  errno));				  \
lock_ObtainWrite(&globalLock);	\
MACRO_END

#define GLOBALUNLOCK 	\
MACRO_BEGIN		\
lock_ReleaseWrite(&globalLock);	\
DFS_ENABLE_CANCEL(savedCancelState, cancelCode);	\
if (cancelCode) 					\
   dlprintf(dce_flserver_debug,1,("fls: DFS_ENABLE_CANCEL: errno %lu\n", \
				  errno));				  \
MACRO_END

/* Forward declarations for ansi C */
static long SiteExists _TAKES((struct ubik_trans *trans,
			       struct vlentry *vlEntry,
			       afsNetAddr *Addr,
			       long Part,
			       int incrOnSuccess,
			       long *wherep));
static long AuthForAddress _TAKES((rpc_binding_handle_t connp,
				   struct ubik_trans *trans,
				   afsNetAddr *addrp,
				   long *okp));
static long AuthForVlEntry _TAKES((rpc_binding_handle_t connp,
				   struct ubik_trans *trans,
				   struct vlentry *vlentryp,
				   long *okp));
static long AuthForSiteDesc _TAKES((rpc_binding_handle_t connp,
				    siteDesc *descp,
				    long *okp));
static long GetSite _TAKES((struct ubik_trans *trans,
			    u_long SitePtr,
			    siteDesc *descBuffP));
static long CarefullyGetSite _TAKES((struct ubik_trans *trans,
				     u_long SitePtr,
				     siteDesc *descBuffP));
static long PutSite _TAKES((struct ubik_trans *trans,
			    u_long SitePtr,
			    siteDesc *descBuffP,
			    int needLock));
static void ntohSiteDesc _TAKES((siteDesc *descp));
static int check_vldbentry _TAKES((vldbentry *aentry,
				   afs_hyper_t *rwvolp,
				   afs_hyper_t *rovolp,
				   afs_hyper_t *backvolp));
static int vldbentry_to_vlentry _TAKES((struct ubik_trans *atrans,
					vldbentry *VldbEntry,
					struct vlentry *VlEntry));
static int NotFlAdmin _TAKES((rpc_binding_handle_t connp));
static int RemoveEntry _TAKES((struct ubik_trans *trans,
			       long entryptr,
			       struct vlentry *tentry));
static int FreeSiteQuota _TAKES((struct ubik_trans *trans,
				 struct vlentry *tentry));
static int VolTypeToVolTix _TAKES((unsigned long voltype));
static int vlentry_to_vldbentry _TAKES((struct ubik_trans *trans,
					struct vlentry *VlEntry,
					vldbentry *VldbEntry));
static int InvalidVolname _TAKES((volumeName volname));
static void ReleaseEntry _TAKES((struct vlentry	*tentry,
				 long releasetype));
static int InvalidReleasetype _TAKES((long releasetype));
static int InvalidOperation _TAKES((long voloper));
static int CheckafsNetAddr _TAKES((afsNetAddr *Addr));
static int EnsureAddrEntry _TAKES((struct ubik_trans *trans,
				   afsNetAddr *Addr,
				   u_long *OutPtr,
				   int DoAlloc,
				   int *WhichAddr,
				   int quotaIncr));
static int vlentry_to_comvldbentry _TAKES((struct ubik_trans *trans,
					   struct vlentry *vlentp,
					   compactvldbentry *comvldbp));


/* This is called to initialize the database, set the appropriate locks and
   make sure that the vldb header is valid.
   For read-only transactions, any representative of the DB may be read.
 */
static long Init_VLdbase (trans, locktype, op)
struct ubik_trans	**trans;
int			locktype;	/* indicate read or write transaction */
int			op;
{
    long errorcode;

    icl_Trace4(fl_iclSetp, FL_TRACE_INIT_DB,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, locktype,
	       ICL_TYPE_LONG, op,
	       ICL_TYPE_LONG, /* readany*/1);
    if (locktype == LOCKREAD)
	errorcode = (long)ubik_BeginTransReadAny(VL_dbase, UBIK_READTRANS,
						 trans);
    else
	errorcode = (long)ubik_BeginTrans(VL_dbase, UBIK_WRITETRANS, trans);
    if (errorcode) {
	icl_Trace1(fl_iclSetp, FL_TRACE_INIT_DB_EXIT, ICL_TYPE_LONG, errorcode);
	return (long)errorcode;
    }
    if (errorcode = (long)ubik_SetLock(*trans, 1, 1, locktype)) {
	COUNT_ABO(op);
	ubik_AbortTrans(*trans);
	icl_Trace1(fl_iclSetp, FL_TRACE_INIT_DB_EXIT, ICL_TYPE_LONG, errorcode);
	return (long)errorcode;
    }
    /* check that dbase is initialized and setup cheader */
    if (errorcode = (long)CheckInit(*trans)) {
	COUNT_ABO(op);
	ubik_AbortTrans(*trans);
	/*
	 * Clarify the nature of the problem if we simply can't create a
	 * new FLDB
	 */
	if (errorcode == UBADTYPE && locktype == LOCKREAD) {
	    errorcode = VL_INITNEEDSWRITE;
	}
	icl_Trace1(fl_iclSetp, FL_TRACE_INIT_DB_EXIT, ICL_TYPE_LONG, errorcode);
	return (long)errorcode;
    }
    icl_Trace1(fl_iclSetp, FL_TRACE_INIT_DB_EXIT, ICL_TYPE_LONG, 0);
    return (long)error_status_ok;
}


/*
 * Create a new vldb entry;
 * both new volume id and name must be unique (non-existant in vldb).
 *
 * access=Admin or serverOwner for all servers referenced
 */
error_status_t VL_CreateEntry(connp, newentry)
rpc_binding_handle_t	connp;
vldbentry		*newentry;
{
    struct ubik_trans	*trans;
    long		errorcode, blockindex, ok;
    struct vlentry	tentry;
    afs_hyper_t RW, RO, BACK;
    int isAdmin;

    icl_Trace2(fl_iclSetp, FL_TRACE_CRENT,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_POINTER, newentry);
    COUNT_REQ(VLCREATEENTRY);
    newentry->volumeType = VOLTIX_TO_VOLTYPE(RWVOL);	/* always want this */
    dlprintf(dce_flserver_debug,1,("fls: in VL_CreateEntry\n"));
    GLOBALLOCK;

    isAdmin = (NotFlAdmin(connp) == 0);
    if (errorcode = check_vldbentry(newentry, &RW, &RO, &BACK)) {
	/* Do some validity tests on new entry */
	COUNT_ABO(VLCREATEENTRY);
	GLOBALUNLOCK;
	dlprintf(dce_flserver_debug,1,
		 ("fls: CreateEntry: check_vldbentry code %lu\n", errorcode));
	icl_Trace1(fl_iclSetp, FL_TRACE_CRENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    if (errorcode = Init_VLdbase(&trans, LOCKWRITE, VLCREATEENTRY)) {
	COUNT_ABO(VLCREATEENTRY);
	GLOBALUNLOCK;
	dlprintf(dce_flserver_debug, 1,
		 ("fls: CreateEntry: Init_VLdbase code %lu\n", errorcode));
	icl_Trace1(fl_iclSetp, FL_TRACE_CRENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    /* XXX shouldn't we check to see if the r/o volume is duplicated? */
    if (AFS_hiszero(RW) || FindByID(trans, &RW, RWVOL, &tentry, &errorcode)) {
	/* entry already exists; we fail */
	COUNT_ABO(VLCREATEENTRY);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	dlprintf(dce_flserver_debug,1,("fls: CreateEntry: FindByID code %lu\n",
				       VL_IDEXIST));
	icl_Trace1(fl_iclSetp, FL_TRACE_CRENT_EXIT, ICL_TYPE_LONG, VL_NOENT);
	return (error_status_t)VL_IDEXIST;
    } else if (errorcode != VL_NOENT) {
	COUNT_ABO(VLCREATEENTRY);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	dlprintf(dce_flserver_debug,1,("fls: CreateEntry: FindByID code %lu\n",
				       errorcode));
	icl_Trace1(fl_iclSetp, FL_TRACE_CRENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    /*
     * Is the following check (by volume name) necessary??
     * CFE thinks it's a good idea 4/19/90
     */
    if (FindByName(trans, newentry->name, &tentry, &errorcode)) {
	/* entry already exists; we fail */
	COUNT_ABO(VLCREATEENTRY);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	dlprintf(dce_flserver_debug, 1,
		 ("fls: CreateEntry: FindByName code %lu\n", VL_IDEXIST));
	icl_Trace1(fl_iclSetp, FL_TRACE_CRENT_EXIT,
		   ICL_TYPE_LONG, VL_NAMEEXIST);
	return (error_status_t)VL_NAMEEXIST;
    } else if (errorcode != VL_NOENT) {
	COUNT_ABO(VLCREATEENTRY);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	dlprintf(dce_flserver_debug, 1,
		 ("fls: CreateEntry: FindByName code %lu\n", errorcode));
	icl_Trace1(fl_iclSetp, FL_TRACE_CRENT_EXIT, ICL_TYPE_LONG, errorcode);
	return(error_status_t)errorcode;
    }
    dlprintf(dce_flserver_debug,1,("fls: CreateEntry: about to AllocBlock\n"));
    blockindex = AllocBlock(trans, &tentry, &errorcode);
    if (blockindex == 0) {
	COUNT_ABO(VLCREATEENTRY);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	if (errorcode == 0) errorcode = VL_CREATEFAIL;
	dlprintf(dce_flserver_debug, 1,
		 ("fls: CreateEntry: AllocBlock code %lu\n", errorcode));
	icl_Trace1(fl_iclSetp, FL_TRACE_CRENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)(errorcode);
    }
    dlprintf(dce_flserver_debug,1,("fls: CreateEntry: AllocBlock OK (%lu)\n",
				   blockindex));
    bzero((char *)&tentry, sizeof(struct vlentry));
    /* Convert to its internal representation; both in host byte order */
    if (errorcode = vldbentry_to_vlentry(trans, newentry, &tentry))	{
	FreeBlock(trans, blockindex);
	COUNT_ABO(VLCREATEENTRY);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	dlprintf(dce_flserver_debug, 1,
		 ("fls: CreateEntry: vldbentry_to_vlentry code %lu\n",
		  errorcode));
	icl_Trace1(fl_iclSetp, FL_TRACE_CRENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    if (!isAdmin) {
	errorcode = AuthForVlEntry(connp, trans, &tentry, &ok);
	if (errorcode || !ok) {
	    FreeBlock(trans, blockindex);
	    COUNT_ABO(VLCREATEENTRY);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    dlprintf(dce_flserver_debug, 1,
		     ("fls: CreateEntry: AuthForVlEntry code %lu\n",
		      errorcode));
	    icl_Trace1(fl_iclSetp, FL_TRACE_CRENT_EXIT, ICL_TYPE_LONG,
		       (errorcode ? errorcode : VL_PERM));
	    return (error_status_t)(errorcode ? errorcode : VL_PERM);
	}
    }

    dlprintf(dce_flserver_debug, 1,
	     ("fls: CreateEntry: about to ThreadVLentry\n"));

    /* Actually insert the entry in vldb */
    errorcode = ThreadVLentry(trans, blockindex, &tentry);
    if (errorcode) {
	FreeBlock(trans, blockindex);
	COUNT_ABO(VLCREATEENTRY);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	dlprintf(dce_flserver_debug, 1,
		 ("fls: CreateEntry: ThreadVLentry code %lu\n", errorcode));
	icl_Trace1(fl_iclSetp, FL_TRACE_CRENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = ubik_EndTrans(trans);
    GLOBALUNLOCK;
    dlprintf(dce_flserver_debug, 1,
	     ("fls: CreateEntry: about to ubik_EndTrans\n"));
    icl_Trace1(fl_iclSetp, FL_TRACE_CRENT_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}


/*
 * Delete a vldb entry given the volume id.
 *
 * access=Admin or serverOwner for all servers referenced
 */
error_status_t VL_DeleteEntry(connp, volid, voltype)
rpc_binding_handle_t	connp;
afs_hyper_t		*volid;
u_long			voltype;
{
    struct ubik_trans	*trans;
    long		blockindex, errorcode, ok;
    struct vlentry	tentry;
    int isAdmin;

    icl_Trace3(fl_iclSetp, FL_TRACE_DELENT,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_HYPER, volid,
	       ICL_TYPE_LONG, voltype);
    COUNT_REQ(VLDELETEENTRY);
    GLOBALLOCK;

    isAdmin = (NotFlAdmin(connp) == 0);
    if (voltype != (unsigned long) -1) {
	voltype = VolTypeToVolTix(voltype);
	if (voltype == (unsigned long) -1) {
	    COUNT_ABO(VLDELETEENTRY);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_DELENT_EXIT,
		       ICL_TYPE_LONG, VL_BADVOLTYPE);
	    return (error_status_t)VL_BADVOLTYPE;
	}
    }
    if (errorcode = Init_VLdbase (&trans, LOCKWRITE, VLDELETEENTRY)) {
	COUNT_ABO(VLDELETEENTRY);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_DELENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    blockindex = FindByID(trans, volid, voltype, &tentry, &errorcode);
    if (blockindex == 0) {			/* volid not found */
	COUNT_ABO(VLDELETEENTRY);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_DELENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    if (!isAdmin) {
	errorcode = AuthForVlEntry(connp, trans, &tentry, &ok);
	if (errorcode || !ok) {
	    COUNT_ABO(VLDELETEENTRY);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_DELENT_EXIT, ICL_TYPE_LONG,
		       (errorcode ? errorcode : VL_PERM));
	    return (error_status_t)(errorcode ? errorcode : VL_PERM);
	}
    }
    if (errorcode = FreeSiteQuota(trans, &tentry)) {
	COUNT_ABO(VLDELETEENTRY);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_DELENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    if (errorcode = RemoveEntry(trans, blockindex, &tentry)) {
	COUNT_ABO(VLDELETEENTRY);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_DELENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = ubik_EndTrans(trans);
    GLOBALUNLOCK;
    icl_Trace1(fl_iclSetp, FL_TRACE_DELENT_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}


/*
 * Get a vldb entry given its volume id; make sure it's not a deleted entry.
 *
 * access=OPEN
 */
error_status_t VL_GetEntryByID (connp, volid, voltype, aentry)
rpc_binding_handle_t	connp;
afs_hyper_t		*volid;
u_long			voltype;
vldbentry		*aentry;		/* entry data copied here */
{
    struct ubik_trans	*trans;
    long		blockindex, errorcode;
    struct vlentry	tentry;

    icl_Trace4(fl_iclSetp, FL_TRACE_GETENT_ID,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_HYPER, volid,
	       ICL_TYPE_LONG, voltype,
	       ICL_TYPE_POINTER, aentry);
    COUNT_REQ(VLGETENTRYBYID);
    GLOBALLOCK;
    if (voltype != (unsigned long) -1) {
	voltype = VolTypeToVolTix(voltype);
	if (voltype == (unsigned long) -1) {
	    COUNT_ABO(VLGETENTRYBYID);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_GETENT_ID_EXIT,
		       ICL_TYPE_LONG, VL_BADVOLTYPE);
	    return (error_status_t)VL_BADVOLTYPE;
	}
    }
    if (errorcode = Init_VLdbase (&trans, LOCKREAD, VLGETENTRYBYID)) {
	COUNT_ABO(VLGETENTRYBYID);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETENT_ID_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    blockindex = FindByID(trans, volid, voltype, &tentry, &errorcode);
    if (blockindex == 0) {		/* entry not found */
	COUNT_ABO(VLGETENTRYBYID);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETENT_ID_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    /* Convert from the internal to external form */
    errorcode = vlentry_to_vldbentry(trans, &tentry, aentry);
    if (errorcode != 0) {
	COUNT_ABO(VLGETENTRYBYID);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETENT_ID_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = ubik_EndTrans(trans);
    GLOBALUNLOCK;
    icl_Trace1(fl_iclSetp, FL_TRACE_GETENT_ID_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}


/*
 * Get a vldb entry given the volume's name;
 * of course, very similar to VLGetEntryByID() above.
 *
 * access=OPEN
 */
error_status_t VL_GetEntryByName (connp, volname, aentry)
rpc_binding_handle_t	connp;
volumeName		volname;
vldbentry		*aentry;	/* entry data copied here */
{
    struct ubik_trans	*trans;
    long		blockindex, errorcode;
    struct vlentry	tentry;
    afs_hyper_t Imputed;

    icl_Trace3(fl_iclSetp, FL_TRACE_GETENT_NAME,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_STRING, volname,
	       ICL_TYPE_POINTER, aentry);
    COUNT_REQ(VLGETENTRYBYNAME);
    GLOBALLOCK;
    if (dfsh_StrToHyper(volname, &Imputed, NULL) == 0) {
	COUNT_ABO(VLGETENTRYBYNAME);
	GLOBALUNLOCK;
	errorcode = VL_GetEntryByID(connp, &Imputed, -1, aentry);
	icl_Trace1(fl_iclSetp, FL_TRACE_GETENT_NAME_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    if (InvalidVolname(volname)) {
	COUNT_ABO(VLGETENTRYBYNAME);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETENT_NAME_EXIT,
		   ICL_TYPE_LONG, VL_BADNAME);
	return (error_status_t)VL_BADNAME;
    }
    if (errorcode = Init_VLdbase(&trans, LOCKREAD, VLGETENTRYBYNAME)) {
	COUNT_ABO(VLGETENTRYBYNAME);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETENT_NAME_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    blockindex = FindByName(trans, volname, &tentry, &errorcode);
    if (blockindex == 0) {			/* entry not found */
	COUNT_ABO(VLGETENTRYBYNAME);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETENT_NAME_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    /* Convert to external entry representation */
    errorcode = vlentry_to_vldbentry(trans, &tentry, aentry);
    if (errorcode != 0) {
	COUNT_ABO(VLGETENTRYBYNAME);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETENT_NAME_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = ubik_EndTrans(trans);
    GLOBALUNLOCK;
    icl_Trace1(fl_iclSetp, FL_TRACE_GETENT_NAME_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}


/*
 * Get the current value of the maximum volume id
 * and bump the volume id counter by Maxvolidbump.
 *
 * access=Admin or serverOwner for named server
 */
error_status_t VL_GetNewVolumeId (connp, Maxvolidbump, Addrp, newvolumeid)
rpc_binding_handle_t	connp;
u_long			Maxvolidbump;
afsNetAddr	*Addrp;		/* for permission checking if not super-user */
afs_hyper_t		*newvolumeid;
{
    register long      errorcode;
    long ok;
    afs_hyper_t CurrMax;
    struct ubik_trans *trans;
    int isAdmin;

    icl_Trace4(fl_iclSetp, FL_TRACE_GETNEWVOL,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_LONG, Maxvolidbump,
	       ICL_TYPE_POINTER, Addrp,
	       ICL_TYPE_POINTER, newvolumeid);
    COUNT_REQ(VLGETNEWVOLUMEID);
    GLOBALLOCK;

    isAdmin = (NotFlAdmin(connp) == 0);

    if (Maxvolidbump < 0 || Maxvolidbump > MAXBUMPCOUNT) {
	COUNT_ABO(VLGETNEWVOLUMEID);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETNEWVOL_EXIT,
		   ICL_TYPE_LONG, VL_BADVOLIDBUMP);
	return (error_status_t)VL_BADVOLIDBUMP;
    }
    if (errorcode = Init_VLdbase(&trans, LOCKWRITE, VLGETNEWVOLUMEID)) {
	COUNT_ABO(VLGETNEWVOLUMEID);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETNEWVOL_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    if (!isAdmin) {
	if (Addrp == 0) {ok = 0; errorcode = 0;}
	else errorcode = AuthForAddress(connp, trans, Addrp, &ok);
	if (errorcode || !ok) {
	    COUNT_ABO(VLGETNEWVOLUMEID);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_GETNEWVOL_EXIT, ICL_TYPE_LONG,
		       (errorcode ? errorcode : VL_PERM));
	    return (error_status_t)(errorcode ? errorcode : VL_PERM);
	}
    }
    DFSH_MemFromNetHyper(CurrMax, cheader.vital_header.maxVolumeId);
    *newvolumeid = CurrMax;
    AFS_hadd32(CurrMax, Maxvolidbump);
    DFSH_NetFromMemHyper(cheader.vital_header.maxVolumeId, CurrMax);
    errorcode = write_vital_vlheader(trans);
    if (errorcode) {
	COUNT_ABO(VLGETNEWVOLUMEID);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETNEWVOL_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = ubik_EndTrans(trans);
    GLOBALUNLOCK;
    icl_Trace1(fl_iclSetp, FL_TRACE_GNV_RES, ICL_TYPE_HYPER, newvolumeid);
    icl_Trace1(fl_iclSetp, FL_TRACE_GETNEWVOL_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}


/*
 * Simply replace the contents of the vldb entry, volid, with newentry.
 *
 * access=Admin or serverOwner for all servers referenced in old and new
 */
error_status_t VL_ReplaceEntry (connp, volid, voltype, newentry, releasetype)
rpc_binding_handle_t	connp;
afs_hyper_t		*volid;
u_long			voltype;
vldbentry		*newentry;
u_long			releasetype;
{
    struct ubik_trans	*trans;
    long		blockindex, errorcode, typeindex, ok;
    int			hashnewname;
    char			hashVol[MAXTYPES];
    struct vlentry	tentry;
    afs_hyper_t VIDS[MAXTYPES];
    afs_hyper_t temp;
    int isAdmin;

    icl_Trace4(fl_iclSetp, FL_TRACE_REPLENT,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_HYPER, volid,
	       ICL_TYPE_LONG, voltype,
	       ICL_TYPE_POINTER, newentry);
    COUNT_REQ(VLREPLACEENTRY);
    bzero(hashVol, sizeof(hashVol));
    hashnewname = 0;
    GLOBALLOCK;


    isAdmin = (NotFlAdmin(connp) == 0);

    if (errorcode = check_vldbentry(newentry, &VIDS[RWVOL], &VIDS[ROVOL],
				    &VIDS[BACKVOL])) {
	COUNT_ABO(VLREPLACEENTRY);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_REPLENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    if (voltype != (unsigned long) -1) {
	voltype = VolTypeToVolTix(voltype);
	if (voltype == (unsigned long) -1) {
	    COUNT_ABO(VLREPLACEENTRY);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_REPLENT_EXIT,
		       ICL_TYPE_LONG, VL_BADVOLTYPE);
	    return (error_status_t)VL_BADVOLTYPE;
	}
    }
    if (releasetype && InvalidReleasetype(releasetype)) {
	COUNT_ABO(VLREPLACEENTRY);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_REPLENT_EXIT,
		   ICL_TYPE_LONG, VL_BADRELLOCKTYPE);
	return (error_status_t)VL_BADRELLOCKTYPE;
    }
    if (errorcode = Init_VLdbase(&trans, LOCKWRITE, VLREPLACEENTRY)) {
	COUNT_ABO(VLREPLACEENTRY);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_REPLENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }

    /* find vlentry we're changing */
    blockindex = FindByID(trans, volid, voltype, &tentry, &errorcode);
    if (blockindex == 0) {			/* entry not found */
	COUNT_ABO(VLREPLACEENTRY);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_REPLENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    /* check that all the old servers are owned */
    if (!isAdmin) {
	errorcode = AuthForVlEntry(connp, trans, &tentry, &ok);
	if (errorcode || !ok) {
	    COUNT_ABO(VLREPLACEENTRY);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_REPLENT_EXIT, ICL_TYPE_LONG,
		       (errorcode ? errorcode : VL_PERM));
	    return (error_status_t)(errorcode ? errorcode : VL_PERM);
	}
    }

    /* check that we're not trying to change the RW vol ID */
    DFSH_MemFromNetHyper(temp, tentry.volumeId[RWVOL]);
    if (!AFS_hsame(VIDS[RWVOL], temp)) {
	COUNT_ABO(VLREPLACEENTRY);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_REPLENT_EXIT,
		   ICL_TYPE_LONG, VL_BADENTRY);
	return (error_status_t)VL_BADENTRY;
    }

    /*
     * unhash volid entries if they're disappearing or changing.
     * Remember if we need to hash in the new value
     * (we don't have to rehash if volid stays the same)
     */
    for (typeindex = ROVOL; typeindex < MAXTYPES; typeindex++) {
	DFSH_MemFromNetHyper(temp, tentry.volumeId[typeindex]);
	if (!AFS_hsame(temp, VIDS[typeindex])) {
	    if (!AFS_hiszero(temp)) {
		if (errorcode = UnhashVolid(trans, typeindex, blockindex,
					    &tentry)) {
		    COUNT_ABO(VLREPLACEENTRY);
		    ubik_AbortTrans(trans);
		    GLOBALUNLOCK;
		    icl_Trace1(fl_iclSetp, FL_TRACE_REPLENT_EXIT,
			       ICL_TYPE_LONG, errorcode);
		    return (error_status_t)errorcode;
		}
	    }
	    /*
	     * we must rehash new id if the id is different and the ID
	     * is nonzero
	     */
	    hashVol[typeindex] = 1;	/* must rehash this guy if he exists */
	}
    }

    /* Rehash volname if it changes */
    if (strcmp((char *)newentry->name, tentry.name)) {
	/* Name changes: redo name hashing */
	if (errorcode = UnhashVolname(trans, blockindex, &tentry)) {
	    COUNT_ABO(VLREPLACEENTRY);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_REPLENT_EXIT,
		       ICL_TYPE_LONG, errorcode);
	    return (error_status_t)errorcode;
	}
	hashnewname = 1;
    }
    /* Bump site quota for all sites pointed to by the old entry */
    if (errorcode = FreeSiteQuota(trans, &tentry)) {
	COUNT_ABO(VLDELETEENTRY);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_REPLENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }

    /*
     * after this, tentry is new entry, not old one.
     * vldbentry_to_vlentry doesn't touch hash chains
     */
    if (errorcode = vldbentry_to_vlentry(trans, newentry, &tentry)) {
	COUNT_ABO(VLREPLACEENTRY);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_REPLENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }

    /* check that all the new servers are owned */
    if (!isAdmin) {
	errorcode = AuthForVlEntry(connp, trans, &tentry, &ok);
	if (errorcode || !ok) {
	    COUNT_ABO(VLREPLACEENTRY);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_REPLENT_EXIT, ICL_TYPE_LONG,
		       (errorcode ? errorcode : VL_PERM));
	    return (error_status_t)(errorcode ? errorcode : VL_PERM);
	}
    }

    for (typeindex = ROVOL; typeindex < MAXTYPES; typeindex++) {
	if (hashVol[typeindex]) {
	    if (errorcode = HashVolid(trans, typeindex, blockindex, &tentry)) {
		COUNT_ABO(VLREPLACEENTRY);
		ubik_AbortTrans(trans);
		GLOBALUNLOCK;
		icl_Trace1(fl_iclSetp, FL_TRACE_REPLENT_EXIT,
			   ICL_TYPE_LONG, errorcode);
		return (error_status_t)errorcode;
	    }
	}
    }

    if (hashnewname) {
	errorcode = HashVolname(trans, blockindex, &tentry);
	if (errorcode != 0) {
		COUNT_ABO(VLREPLACEENTRY);
		ubik_AbortTrans(trans);
		GLOBALUNLOCK;
		icl_Trace1(fl_iclSetp, FL_TRACE_REPLENT_EXIT,
			   ICL_TYPE_LONG, errorcode);
		return (error_status_t)errorcode;
	}
    }

    if (releasetype)
	ReleaseEntry(&tentry, releasetype);	/* Unlock entry if necessary */
    errorcode = vlentrywrite(trans, blockindex, &tentry, sizeof(tentry));
    if (errorcode) {
	COUNT_ABO(VLREPLACEENTRY);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_REPLENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = ubik_EndTrans(trans);
    GLOBALUNLOCK;
    icl_Trace1(fl_iclSetp, FL_TRACE_REPLENT_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}


/*
 * Set a lock to the vldb entry for volid (of type voltype if not -1).
 *
 * access=Admin or serverOwner for all servers referenced
 */
error_status_t VL_SetLock (connp, volid, voltype, voloper)
rpc_binding_handle_t	connp;
afs_hyper_t		*volid;
u_long			voltype;
u_long			voloper;
{
    long		timestamp, blockindex, errorcode, ok;
    struct ubik_trans	*trans;
    struct vlentry	tentry;
    char LockPersonName[300];
    int isAdmin;

    icl_Trace4(fl_iclSetp, FL_TRACE_SETLK,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_HYPER, volid,
	       ICL_TYPE_LONG, voltype,
	       ICL_TYPE_LONG, voloper);
    COUNT_REQ(VLSETLOCK);
    GLOBALLOCK;

    isAdmin = (NotFlAdmin(connp) == 0);

    if (voltype != (unsigned long) -1) {
	voltype = VolTypeToVolTix(voltype);
	if (voltype == (unsigned long) -1) {
	    COUNT_ABO(VLSETLOCK);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_SETLK_EXIT,
		       ICL_TYPE_LONG, VL_BADVOLTYPE);
	    return (error_status_t)VL_BADVOLTYPE;
	}
    }
    if (InvalidOperation(voloper)) {
	COUNT_ABO(VLSETLOCK);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_SETLK_EXIT,
		   ICL_TYPE_LONG, VL_BADVOLOPER);
	return (error_status_t)VL_BADVOLOPER;
    }
    if (errorcode = Init_VLdbase(&trans, LOCKWRITE, VLSETLOCK)) {
	COUNT_ABO(VLSETLOCK);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_SETLK_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    blockindex = FindByID(trans, volid, voltype, &tentry, &errorcode);
    if (blockindex == NULLO) {
	COUNT_ABO(VLSETLOCK);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_SETLK_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    /* check that all the servers are owned */
    if (!isAdmin) {
	errorcode = AuthForVlEntry(connp, trans, &tentry, &ok);
	if (errorcode || !ok) {
	    COUNT_ABO(VLSETLOCK);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_SETLK_EXIT, ICL_TYPE_LONG,
		       (errorcode ? errorcode : VL_PERM));
	    return (error_status_t)(errorcode ? errorcode : VL_PERM);
	}
    }
    timestamp = osi_Time();
    /*
     * Check if entry is already locked;
     * note that we unlock any entry locked more than MAXLOCKTIME seconds
     */
    if ((tentry.LockTimestamp) &&
	((timestamp - tentry.LockTimestamp) < MAXLOCKTIME)) {
	COUNT_ABO(VLSETLOCK);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_SETLK_EXIT,
		   ICL_TYPE_LONG, VL_ENTRYLOCKED);
	return (error_status_t)VL_ENTRYLOCKED;
    }
    /*
     * Consider it an unlocked entry:
     * set current timestamp, caller and active vol operation
     */
    tentry.LockTimestamp = timestamp;
    tentry.NameOfLocker[0] = '\0';
#if 0
    if (GetConnUsername(connp, LockPersonName) == 0)
	strncpy(tentry.NameOfLocker,LockPersonName,sizeof(tentry.NameOfLocker));
#endif /*0 */
    if (tentry.flags & VLOP_RELEASE) {
	COUNT_ABO(VLSETLOCK);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_SETLK_EXIT,
		   ICL_TYPE_LONG, VL_RERELEASE);
	return (error_status_t)VL_RERELEASE;
    }
    /* Clear any possible older operation bits */
    tentry.flags &= ~VLOP_ALLOPERS;
    tentry.flags |= voloper;

    errorcode = vlentrywrite(trans, blockindex, &tentry, sizeof(tentry));
    if (errorcode) {
	COUNT_ABO(VLSETLOCK);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_SETLK_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = ubik_EndTrans(trans);
    GLOBALUNLOCK;
    icl_Trace1(fl_iclSetp, FL_TRACE_SETLK_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}


/*
 * Release an already locked vldb entry.
 * Releasetype determines what fields (afsid and/or volume operation)
 * will be cleared along with the lock time stamp.
 *
 * access=Admin or serverOwner for all servers referenced
 */
error_status_t VL_ReleaseLock (connp, volid, voltype, releasetype)
rpc_binding_handle_t	connp;
afs_hyper_t		*volid;
u_long			voltype;
u_long			releasetype;
{
    long		blockindex, errorcode, ok;
    struct ubik_trans	*trans;
    struct vlentry	tentry;
    int isAdmin;

    icl_Trace4(fl_iclSetp, FL_TRACE_RELLK,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_HYPER, volid,
	       ICL_TYPE_LONG, voltype,
	       ICL_TYPE_LONG, releasetype);
    COUNT_REQ(VLRELEASELOCK);
    GLOBALLOCK;


    isAdmin = (NotFlAdmin(connp) == 0);

    if (voltype != (unsigned long) -1) {
	voltype = VolTypeToVolTix(voltype);
	if (voltype == (unsigned long) -1) {
	    COUNT_ABO(VLRELEASELOCK);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_RELLK_EXIT,
		       ICL_TYPE_LONG, VL_BADVOLTYPE);
	    return (error_status_t)VL_BADVOLTYPE;
	}
    }
    if (releasetype && InvalidReleasetype(releasetype)) {
	COUNT_ABO(VLRELEASELOCK);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_RELLK_EXIT,
		   ICL_TYPE_LONG, VL_BADRELLOCKTYPE);
	return (error_status_t)VL_BADRELLOCKTYPE;
    }
    if (errorcode = Init_VLdbase(&trans, LOCKWRITE, VLRELEASELOCK)) {
	COUNT_ABO(VLRELEASELOCK);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_RELLK_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    blockindex = FindByID(trans, volid, voltype, &tentry, &errorcode);
    if (blockindex == NULLO) {
	COUNT_ABO(VLRELEASELOCK);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_RELLK_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    /* check that all the servers are owned */
    if (!isAdmin) {
	errorcode = AuthForVlEntry(connp, trans, &tentry, &ok);
	if (errorcode || !ok) {
	    COUNT_ABO(VLRELEASELOCK);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_RELLK_EXIT, ICL_TYPE_LONG,
		       (errorcode ? errorcode : VL_PERM));
	    return (error_status_t)(errorcode ? errorcode : VL_PERM);
	}
    }
    if (releasetype) {
	/* Unlock the appropriate fields */
	ReleaseEntry(&tentry, releasetype);
    }
    errorcode = vlentrywrite(trans, blockindex, &tentry, sizeof(tentry));
    if (errorcode) {
	COUNT_ABO(VLRELEASELOCK);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_RELLK_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = ubik_EndTrans(trans);
    GLOBALUNLOCK;
    icl_Trace1(fl_iclSetp, FL_TRACE_RELLK_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}


/*
 * ListEntry returns a single vldb entry, aentry, with offset previous_index;
 * the remaining parameters (i.e. next_index) are used so that sequential
 * calls to this routine will get the next (all) vldb entries.
 *
 * access=OPEN
 */
error_status_t VL_ListEntry (connp, previous_index, count, next_index, aentry)
rpc_binding_handle_t	connp;
unsigned long		previous_index;
unsigned long		*count;
unsigned long		*next_index;
vldbentry		*aentry;
{
    int			errorcode;
    struct ubik_trans	*trans;
    struct vlentry	tentry;

    icl_Trace4(fl_iclSetp, FL_TRACE_LSTENT,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_LONG, previous_index,
	       ICL_TYPE_POINTER, count,
	       ICL_TYPE_POINTER, next_index);
    COUNT_REQ(VLLISTENTRY);
    GLOBALLOCK;
    if (errorcode = Init_VLdbase(&trans, LOCKREAD, VLLISTENTRY)) {
	COUNT_ABO(VLLISTENTRY);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_LSTENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    *next_index = NextEntry(trans, previous_index, &tentry, count);
    if (*next_index) {
	errorcode = vlentry_to_vldbentry(trans, &tentry, aentry);
	if (errorcode != 0) {
	    COUNT_ABO(VLLISTENTRY);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_LSTENT_EXIT,
		       ICL_TYPE_LONG, errorcode);
	    return (error_status_t)errorcode;
	}
    }
    errorcode = ubik_EndTrans(trans);
    GLOBALUNLOCK;
    icl_Trace1(fl_iclSetp, FL_TRACE_LSTENT_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}

/*
 * Retrieves in vldbentries all vldb entries that match the specified
 * attributes (by server number, partition, volume type, and flag);
 * if volume id is specified then the associated list for that entry is
 * returned.
 *
 * CAUTION: This could be a very expensive call since in most cases sequential
 * search of all vldb entries is performed.
 *
 * access=OPEN
 */
error_status_t VL_ListByAttributes(connp, attributes, cookie, nentries,
				   vldbentries, nextCookieP, flagsP)
rpc_binding_handle_t	    connp;
VldbListByAttributes 	    *attributes;
u_long			    cookie;
u_long			    *nentries;
bulkentries		    *vldbentries;
u_long			    *nextCookieP;
u_long			    *flagsP;
{
    int			i;
    unsigned long	errorcode;
    int			AddrIx, Site, VolTIx;
    char		Sites[MAXNSERVERS];
    siteDesc		DescBuff;
    struct ubik_trans	*trans;
    struct vlentry	tentry;
    vldbentry		*Vldbentry = 0;
    u_long		numStored;

    icl_Trace4(fl_iclSetp, FL_TRACE_LSTATTR,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_POINTER, attributes,
	       ICL_TYPE_LONG, cookie,
	       ICL_TYPE_POINTER, nentries);
    COUNT_REQ(VLLISTBYATTRIBUTES);
    GLOBALLOCK;
    *flagsP = 0;
    *nextCookieP = ~(0L);
    vldbentries->bulkentries_len = *nentries = 0;	/* prevent faults */
    VolTIx = -1;
    if (attributes->Mask & VLLIST_VOLUMETYPE) {
	VolTIx = VolTypeToVolTix(attributes->volumetype);
	if (VolTIx == -1) {
	    COUNT_ABO(VLLISTBYATTRIBUTES);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_LSTATTR_EXIT, ICL_TYPE_LONG,
		       VL_BADVOLTYPE);
	    return (error_status_t)VL_BADVOLTYPE;
	}
    }
    if (errorcode = Init_VLdbase(&trans, LOCKREAD, VLLISTBYATTRIBUTES)) {
	COUNT_ABO(VLLISTBYATTRIBUTES);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_LSTATTR_EXIT, ICL_TYPE_LONG,
		   errorcode);
	return (error_status_t)errorcode;
    }
    Vldbentry = &vldbentries->bulkentries_val[0];
    numStored = 0;
    /* Handle the attribute by volume id totally separate of the rest
     * (thus additional Mask values are ignored if VLLIST_VOLUMEID is set!)
     *
     * -- actually VLLIST_VOLUMETYPE is not ignored if VLLIST_VOLUMEID is set.
     * cookie's are not useful here because only one entry will be returned,
     * and the initial cookie by definition has to be zero. The flag indicating
     * that this is the last entry will always be set for this attribute.
     */
    if (attributes->Mask & VLLIST_VOLUMEID) {
      u_long	blockindex, chain;
      struct vlentry tempentry;
      blockindex = FindByID(trans, &(attributes->volumeid), VolTIx, &tentry,
			    &errorcode);
      if (blockindex == 0) {
	COUNT_ABO(VLLISTBYATTRIBUTES);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_LSTATTR_EXIT, ICL_TYPE_LONG,
		   errorcode);
	return (error_status_t)errorcode;
      }
      if (cookie == 0) {
	errorcode = (error_status_t)vlentry_to_vldbentry(trans,
							 &tentry,
							 Vldbentry);
	if (errorcode != 0) {
	  COUNT_ABO(VLLISTBYATTRIBUTES);
	  ubik_AbortTrans(trans);
	  GLOBALUNLOCK;
	  icl_Trace1(fl_iclSetp, FL_TRACE_LSTATTR_EXIT, ICL_TYPE_LONG,
		     errorcode);
	  return (error_status_t)errorcode;
	}
	++numStored;
	++Vldbentry;
	*nextCookieP = ++cookie;
      }
    }
    else {
      long int count=0;
      long blockindex = (long)cookie;
      while(numStored < MAXBULKLEN &&
	    (blockindex=NextEntry(trans, blockindex, &tentry, &count))) {
	Site = -1;
	bzero((char *)Sites, sizeof(Sites));
	if (attributes->Mask & VLLIST_SITE) {
	  for (i = 0; i < MAXNSERVERS && tentry.serverSite[i] != 0; ++i) {
	    errorcode = (error_status_t)GetSite(trans,
						tentry.serverSite[i],
						&DescBuff);
	    if (errorcode != 0) {
	      COUNT_ABO(VLLISTBYATTRIBUTES);
	      ubik_AbortTrans(trans);
	      GLOBALUNLOCK;
	      icl_Trace1(fl_iclSetp, FL_TRACE_LSTATTR_EXIT, ICL_TYPE_LONG,
			 errorcode);
	      return (error_status_t)errorcode;
	    }
	    for (AddrIx = 0; AddrIx < ADDRSINSITE; ++AddrIx) {
	      if (DescBuff.Addr[AddrIx].type == (unsigned short) -1) break;
	      if (bcmp((char *)&(DescBuff.Addr[AddrIx]),
		       (char *)&(attributes->site),
		       sizeof(afsNetAddr)) == 0) {
		Site = i;
		Sites[i] = 1;
		break;
	      }
	    }
	  }
	  if (Site < 0)
	    continue;
	}

	if (attributes->Mask & VLLIST_PARTITION) {
	  if (Site < 0)
	    continue;
	  Site = -1;
	  for (i = 0; i < MAXNSERVERS && tentry.serverSite[i] != 0; ++i) {
	    if (Sites[i] &&
		tentry.serverPartition[i] == attributes->partition) {
	      Site = i;
	      break;
	    }
	  }
	  if (Site < 0)
	    continue;
	}

	if (attributes->Mask & VLLIST_VOLUMETYPE) {
	  if (tentry.volumeTIx != VolTIx) continue;
	}
	if (attributes->Mask & VLLIST_FLAG) {
	  if (!(tentry.flags & attributes->flag)) continue;
	}

	errorcode=(error_status_t)vlentry_to_vldbentry(trans,
						       &tentry,
						       Vldbentry);
	if (errorcode != 0) {
	  COUNT_ABO(VLLISTBYATTRIBUTES);
	  ubik_AbortTrans(trans);
	  GLOBALUNLOCK;
	  icl_Trace1(fl_iclSetp, FL_TRACE_LSTATTR_EXIT, ICL_TYPE_LONG,
		     errorcode);
	  return (error_status_t)errorcode;
	}
	++numStored;
	++Vldbentry;
      } /*while*/
      if (blockindex)
	*nextCookieP = blockindex;
    }

    if (numStored == 0) {
      *flagsP = 1;			/* flag that this was the last entry */
      COUNT_ABO(VLLISTBYATTRIBUTES);
      ubik_AbortTrans(trans);
      GLOBALUNLOCK;
      icl_Trace1(fl_iclSetp, FL_TRACE_LSTATTR_EXIT, ICL_TYPE_LONG,
		 VL_ENDOFLIST);
      return (error_status_t)VL_ENDOFLIST;
    }
    else {
      if (numStored < MAXBULKLEN)
	*flagsP = 1;			/* flag that this was the last entry */
      vldbentries->bulkentries_len = *nentries = numStored;
      errorcode = ubik_EndTrans(trans);
      GLOBALUNLOCK;
      icl_Trace1(fl_iclSetp, FL_TRACE_LSTATTR_EXIT, ICL_TYPE_LONG, errorcode);
      return (error_status_t)errorcode;
    }
}


/*
 * Get back vldb header statistics (allocs, frees, maxvolumeid, totalentries,
 * etc.) and dynamic statistics (number of requests and/or aborts per remote
 * procedure call, etc.)
 *
 * access=Admin
 */
error_status_t VL_GetStats(connp, stats, vital_header)
rpc_binding_handle_t	connp;
vldstats		*stats;
vital_vlheader		*vital_header;
{
    register long	errorcode;
    struct ubik_trans	*trans;
    register int Ix;
    register u_long *ulp;

    icl_Trace3(fl_iclSetp, FL_TRACE_GETSTAT,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_POINTER, stats,
	       ICL_TYPE_POINTER, vital_header);
    dlprintf(dce_flserver_debug,1,("enter GetStats\n"));
    COUNT_REQ(VLGETSTATS);
    GLOBALLOCK;

    if (NotFlAdmin(connp)) {
	COUNT_ABO(VLGETSTATS);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETSTAT_EXIT, ICL_TYPE_LONG, VL_PERM);
	return (error_status_t)VL_PERM;
    }

    if (errorcode = Init_VLdbase(&trans, LOCKREAD, VLGETSTATS)) {
	COUNT_ABO(VLGETSTATS);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETSTAT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }

    vital_header->vldbversion = ntohl(cheader.vital_header.vldbversion);
    vital_header->headersize = ntohl(cheader.vital_header.headersize);
    vital_header->freePtr = ntohl(cheader.vital_header.freePtr);
    vital_header->eofPtr = ntohl(cheader.vital_header.eofPtr);
    vital_header->allocs = ntohl(cheader.vital_header.allocs);
    vital_header->frees = ntohl(cheader.vital_header.frees);
    DFSH_MemFromNetHyper(vital_header->MaxVolumeId,
			 cheader.vital_header.maxVolumeId);
    vital_header->sitesPtr = ntohl(cheader.vital_header.sitesPtr);
    vital_header->numSites = ntohl(cheader.vital_header.numSites);
    for (Ix=0; Ix<MAXVOLTYPES; Ix++)
	vital_header->totalEntries[Ix] =
	    ntohl(cheader.vital_header.totalEntries[Ix]);
    DFSH_MemFromNetHyper(vital_header->theCellID,
			 cheader.vital_header.theCellId);
    vital_header->spare0 = ntohl(cheader.vital_header.spare0);
    vital_header->spare1 = ntohl(cheader.vital_header.spare1);
    vital_header->spare2 = ntohl(cheader.vital_header.spare2);
    vital_header->spare3 = ntohl(cheader.vital_header.spare3);
    vital_header->spare4 = ntohl(cheader.vital_header.spare4);
    vital_header->spare5 = ntohl(cheader.vital_header.spare5);
    vital_header->spare6 = ntohl(cheader.vital_header.spare6);
    vital_header->spare7 = ntohl(cheader.vital_header.spare7);
    vital_header->spare8 = ntohl(cheader.vital_header.spare8);
    vital_header->spare9 = ntohl(cheader.vital_header.spare9);
    vital_header->spare10 = ntohl(cheader.vital_header.spare10);
    vital_header->spare11 = ntohl(cheader.vital_header.spare11);
    vital_header->spare12 = ntohl(cheader.vital_header.spare12);

    *stats = dynamic_statistics;
    errorcode = ubik_EndTrans(trans);
    GLOBALUNLOCK;
    icl_Trace1(fl_iclSetp, FL_TRACE_GETSTAT_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}

/*
 * Add the given new address to the site descriptor specified via OldAddr.
 *
 * access=Admin or serverOwner
 */
error_status_t VL_AddAddress(connp, OldAddr, AddrToAdd)
rpc_binding_handle_t	connp;
afsNetAddr		*OldAddr;
afsNetAddr		*AddrToAdd;
{
    u_long DescAddr;
    siteDesc DescBuff;
    struct ubik_trans *trans;
    long errorcode, ok;
    register int Ix;
    int isAdmin;

    icl_Trace3(fl_iclSetp, FL_TRACE_ADDADDR,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_POINTER, OldAddr,
	       ICL_TYPE_POINTER, AddrToAdd);
    COUNT_REQ(VLADDADDRESS);
    GLOBALLOCK;

    isAdmin = (NotFlAdmin(connp) == 0);

    if (CheckafsNetAddr(AddrToAdd) != 0) {
	COUNT_ABO(VLADDADDRESS);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_ADDADDR_EXIT,
		   ICL_TYPE_LONG, VL_BADENTRY);
	return (error_status_t)VL_BADENTRY;
    }
    if (errorcode = Init_VLdbase(&trans, LOCKWRITE, VLADDADDRESS)) {
	COUNT_ABO(VLADDADDRESS);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_ADDADDR_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = EnsureAddrEntry(trans, AddrToAdd, &DescAddr, 0, 0, 0);
    if (errorcode != VL_NOSERVER) {
	COUNT_ABO(VLADDADDRESS);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_ADDADDR_EXIT, ICL_TYPE_LONG,
		   (errorcode ? errorcode : VL_ADDRESSELSEWHERE));
	return (error_status_t)(errorcode ? errorcode : VL_ADDRESSELSEWHERE);
    }
    errorcode = EnsureAddrEntry(trans, OldAddr, &DescAddr, 0, 0, 0);
    if (errorcode != 0) {
	COUNT_ABO(VLADDADDRESS);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_ADDADDR_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = GetSite(trans, DescAddr, &DescBuff);
    if (errorcode == 0 && DescBuff.DeletedFlag == SITEDELETEDSP1)
      errorcode = VL_DELETEDSITE;
    if (errorcode != 0) {
	COUNT_ABO(VLADDADDRESS);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_ADDADDR_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    /* check that the server is owned */
    if (!isAdmin) {
	errorcode = AuthForSiteDesc(connp, &DescBuff, &ok);
	if (errorcode || !ok) {
	    COUNT_ABO(VLADDADDRESS);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_ADDADDR_EXIT, ICL_TYPE_LONG,
		       (errorcode ? errorcode : VL_PERM));
	    return (error_status_t)(errorcode ? errorcode : VL_PERM);
	}
    }
    for (Ix = 0; Ix < ADDRSINSITE; ++Ix) {
	if (DescBuff.Addr[Ix].type == (unsigned short) -1) break;
	if (bcmp((char *)&(DescBuff.Addr[Ix]),
		 (char *)AddrToAdd,
		 sizeof(afsNetAddr)) == 0) {
	    COUNT_ABO(VLADDADDRESS);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_ADDADDR_EXIT,
		       ICL_TYPE_LONG, VL_ADDRESSEXISTS);
	    return (error_status_t)VL_ADDRESSEXISTS;
	}
    }
    if (Ix >= ADDRSINSITE) {
	COUNT_ABO(VLADDADDRESS);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_ADDADDR_EXIT,
		   ICL_TYPE_LONG, VL_ADDRESSFULL);
	return (error_status_t)VL_ADDRESSFULL;
    }
    DescBuff.Addr[Ix] = *AddrToAdd;
    errorcode = PutSite(trans, DescAddr, &DescBuff, 1);
    if (errorcode != 0) {
	COUNT_ABO(VLADDADDRESS);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_ADDADDR_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode=ubik_EndTrans(trans);
    GLOBALUNLOCK;
    icl_Trace1(fl_iclSetp, FL_TRACE_ADDADDR_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}

/*
 * Remove the given address from the site descriptor for which it is a member.
 *
 * access=Admin or serverOwner
 */
error_status_t VL_RemoveAddress(connp, AddrToRemove)
rpc_binding_handle_t	connp;
afsNetAddr		*AddrToRemove;
{
    struct ubik_trans *trans;
    long errorcode, ok;
    siteDesc DescBuff;
    u_long DescAddr;
    int WhichAddr, isAdmin;
    register int Ix;

    icl_Trace2(fl_iclSetp, FL_TRACE_REMADDR,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_POINTER, AddrToRemove);
    COUNT_REQ(VLREMOVEADDRESS);
    GLOBALLOCK;

    isAdmin = (NotFlAdmin(connp) == 0);

    if (errorcode = Init_VLdbase(&trans, LOCKWRITE, VLREMOVEADDRESS)) {
	COUNT_ABO(VLREMOVEADDRESS);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_REMADDR_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = EnsureAddrEntry(trans, AddrToRemove, &DescAddr, 0,
				&WhichAddr, 0);
    if (errorcode != 0) {
	COUNT_ABO(VLREMOVEADDRESS);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_REMADDR_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = GetSite(trans, DescAddr, &DescBuff);
    if (errorcode == 0 && DescBuff.DeletedFlag == SITEDELETEDSP1)
	errorcode = VL_DELETEDSITE;
    if (errorcode != 0) {
	COUNT_ABO(VLREMOVEADDRESS);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_REMADDR_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    /* check that the server is owned */
    if (!isAdmin) {
	errorcode = AuthForSiteDesc(connp, &DescBuff, &ok);
	if (errorcode || !ok) {
	    COUNT_ABO(VLREMOVEADDRESS);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_REMADDR_EXIT, ICL_TYPE_LONG,
		       (errorcode ? errorcode : VL_PERM));
	    return (error_status_t)(errorcode ? errorcode : VL_PERM);
	}
    }
    if (WhichAddr == 0 && DescBuff.Addr[1].type == (unsigned short) -1) {
	COUNT_ABO(VLREMOVEADDRESS);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_REMADDR_EXIT,
		   ICL_TYPE_LONG, VL_LASTADDRESS);
	return (error_status_t)VL_LASTADDRESS;
    }
    for (Ix = WhichAddr; Ix < (ADDRSINSITE-1); ++Ix) {
	DescBuff.Addr[Ix] = DescBuff.Addr[Ix+1];
	if (DescBuff.Addr[Ix].type == (unsigned short) -1) break;
    }
    DescBuff.Addr[Ix].type = (unsigned short) -1;
    errorcode = PutSite(trans, DescAddr, &DescBuff, 1);
    if (errorcode != 0) {
	COUNT_ABO(VLREMOVEADDRESS);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_REMADDR_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode=ubik_EndTrans(trans);
    GLOBALUNLOCK;
    icl_Trace1(fl_iclSetp, FL_TRACE_REMADDR_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}

/*
 * Change the OldAddr address, in the site descriptor for which it's a member,
 * to the NewAddr address.
 *
 * access=Admin or serverOwner
 */
error_status_t VL_ChangeAddress(connp, OldAddr, NewAddr)
rpc_binding_handle_t	connp;
afsNetAddr		*OldAddr;
afsNetAddr		*NewAddr;
{
    struct ubik_trans *trans;
    siteDesc DescBuff;
    long errorcode, ok;
    int WhichAddr, isAdmin;
    u_long DescAddr;

    icl_Trace3(fl_iclSetp, FL_TRACE_CHGADDR,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_POINTER, OldAddr,
	       ICL_TYPE_POINTER, NewAddr);
    COUNT_REQ(VLCHANGEADDRESS);
    GLOBALLOCK;

    isAdmin = (NotFlAdmin(connp) == 0);

    if (CheckafsNetAddr(NewAddr) != 0) {
	COUNT_ABO(VLCHANGEADDRESS);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_CHGADDR_EXIT,
		   ICL_TYPE_LONG, VL_BADENTRY);
	return (error_status_t)VL_BADENTRY;
    }
    if (errorcode = Init_VLdbase(&trans, LOCKWRITE, VLCHANGEADDRESS)) {
	COUNT_ABO(VLCHANGEADDRESS);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_CHGADDR_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = EnsureAddrEntry(trans, NewAddr, &DescAddr, 0, 0, 0);
    if (errorcode != VL_NOSERVER) {
	COUNT_ABO(VLADDADDRESS);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_CHGADDR_EXIT, ICL_TYPE_LONG,
		   (errorcode ? errorcode : VL_ADDRESSELSEWHERE));
	return (error_status_t)(errorcode ? errorcode : VL_ADDRESSELSEWHERE);
    }
    errorcode = EnsureAddrEntry(trans, OldAddr, &DescAddr, 0, &WhichAddr, 0);
    if (errorcode != 0) {
	COUNT_ABO(VLCHANGEADDRESS);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_CHGADDR_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = GetSite(trans, DescAddr, &DescBuff);
    if (errorcode == 0 && DescBuff.DeletedFlag == SITEDELETEDSP1)
      errorcode = VL_DELETEDSITE;
    if (errorcode != 0) {
	COUNT_ABO(VLCHANGEADDRESS);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_CHGADDR_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    /* check that the server is owned */
    if (!isAdmin) {
	errorcode = AuthForSiteDesc(connp, &DescBuff, &ok);
	if (errorcode || !ok) {
	    COUNT_ABO(VLCHANGEADDRESS);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_CHGADDR_EXIT, ICL_TYPE_LONG,
		       (errorcode ? errorcode : VL_PERM));
	    return (error_status_t)(errorcode ? errorcode : VL_PERM);
	}
    }
    DescBuff.Addr[WhichAddr] = *NewAddr;
    errorcode = PutSite(trans, DescAddr, &DescBuff, 1);
    if (errorcode != 0) {
	COUNT_ABO(VLCHANGEADDRESS);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_CHGADDR_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = ubik_EndTrans(trans);
    GLOBALUNLOCK;
    icl_Trace1(fl_iclSetp, FL_TRACE_CHGADDR_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}

/*
 * Get the complete information for this guy's cell, remotely.
 *
 * access=OPEN
 */
error_status_t VL_GetCellInfo(connp, MyCell)
rpc_binding_handle_t	connp;
vlconf_cell		*MyCell;
{
    extern struct vlconf_cell LocalCellInfo;
    extern int LocalCellNames;
    register int ix;
    u_long DescAddr;
    siteDesc DescBuff;
    struct ubik_trans *trans;
    long errorcode, err2;
    afsNetAddr oneAddr;

    icl_Trace2(fl_iclSetp, FL_TRACE_GETCELL,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_POINTER, MyCell);
    COUNT_REQ(VLGETCELLINFO);
    errorcode = error_status_ok;
    if (LocalCellNames == 0) {
	/* Fill in the principal names, once, that correspond to DB sites. */
	GLOBALLOCK;
	/*
	 * Called only from CM, don't bother about quorum
	 */
	errorcode = Init_VLdbase(&trans, LOCKREAD, VLGETCELLINFO);
	if (errorcode == 0) {
	    if (flserver_verbose)
		dce_fprintf(stderr, lsv_s_flserver_filling_princ_name,
			    LocalCellInfo.numServers);
	    cmd_explanation = dce_msg_get_msg(
					lsv_s_log_flserver_filling_princ_name,
					      &st);
	    Log( (char *) cmd_explanation, LocalCellInfo.numServers);
	    free(cmd_explanation);
	    for (ix = 0; ix < LocalCellInfo.numServers; ++ix) {
		oneAddr = LocalCellInfo.hostAddr[ix];
		if (oneAddr.type == AF_INET) {
		    ((struct sockaddr_in *) &oneAddr)->sin_port = 0;
		}
		err2 = EnsureAddrEntry(trans, &oneAddr, &DescAddr, 0, 0, 0);
		if (errorcode == 0) errorcode = err2;
		if (err2 == 0) {
		    err2 = GetSite(trans, DescAddr, &DescBuff);
		    if (errorcode == 0) errorcode = err2;
		    if (err2 == 0) {
			strncpy((char *)&LocalCellInfo.hostName[ix].hstnam[0],
			      (char *)&DescBuff.KerbPrin[0], MAXVLHOSTCHARS-1);
			if (flserver_verbose)
			    dce_fprintf(stderr, lsv_s_site_has_principal, ix,
					((struct sockaddr_in *)
					 &oneAddr)->sin_addr.s_addr,
					&DescBuff.KerbPrin[0]);
			cmd_explanation = dce_msg_get_msg(
						   lsv_s_log_site_has_principal,
							  &st);
			Log( (char *) cmd_explanation, ix,
			     ((struct sockaddr_in *) &oneAddr)->sin_addr.s_addr,
			     &DescBuff.KerbPrin[0]);
			free(cmd_explanation);
		    } else {
			if (flserver_verbose)
			    dce_fprintf(stderr, lsv_s_flserver_no_site_descr,
					ix, ((struct sockaddr_in *)
					     &oneAddr)->sin_addr.s_addr, err2,
					dfs_dceErrTxt(err2));
			cmd_explanation = dce_msg_get_msg(
					      lsv_s_log_flserver_no_site_descr,
							  &st);
			Log( (char *) cmd_explanation, ix,
			     ((struct sockaddr_in *) &oneAddr)->sin_addr.s_addr,
			     err2, dfs_dceErrTxt(err2));
			free(cmd_explanation);
		    }
		} else {
		    if (flserver_verbose)
			dce_fprintf(stderr, lsv_s_flserver_cant_get_entry, ix,
				    ((struct sockaddr_in *)
				     &oneAddr)->sin_addr.s_addr,
				    err2, dfs_dceErrTxt(err2));
		    cmd_explanation = dce_msg_get_msg(
					     lsv_s_log_flserver_cant_get_entry,
						      &st);
		    Log( (char *) cmd_explanation, ix,
			 ((struct sockaddr_in *) &oneAddr)->sin_addr.s_addr,
			 err2, dfs_dceErrTxt(err2));
		    free(cmd_explanation);
		}
	    }
	    (void) ubik_EndTrans(trans);
	    if (errorcode == 0) LocalCellNames = 1;
	}
	GLOBALUNLOCK;
    }
    *MyCell = LocalCellInfo;
    icl_Trace1(fl_iclSetp, FL_TRACE_GETCELL_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}

/*
 * Get a vldb entry given its volume id
 *
 * This gets the servers starting with the one numbered ``startHere'', unlike
 * GetEntryByID, which gets only the first few.  This procedure is really here
 * for future expansion without changing the representation on the wire.
 *
 * access=OPEN
 */
error_status_t VL_GetNextServersByID (connp, volid, voltype, startHere,
				      nextStartP, aentry, flagsP)
rpc_binding_handle_t	connp;
afs_hyper_t		*volid;
u_long			voltype;
u_long			startHere;
u_long			*nextStartP;
vldbentry		*aentry;	/* entry data copied here */
u_long			*flagsP;
{
    long errorcode;

    icl_Trace4(fl_iclSetp, FL_TRACE_GETNXT_ID,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_HYPER, volid,
	       ICL_TYPE_LONG, voltype,
	       ICL_TYPE_LONG, startHere);
    COUNT_REQ(VLGETNEXTSRVBYID);
    GLOBALLOCK;
    if (startHere == 0) {
	*nextStartP = 1;
	*flagsP = 1;	/* indicate that this call returns the final values */
	GLOBALUNLOCK;
	errorcode = VL_GetEntryByID(connp, volid, voltype, aentry);
	icl_Trace1(fl_iclSetp, FL_TRACE_GETNXT_ID_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    } else if (startHere == 1) {
	COUNT_ABO(VLGETNEXTSRVBYID);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETNXT_ID_EXIT,
		   ICL_TYPE_LONG, VL_ENDOFLIST);
	return (error_status_t)VL_ENDOFLIST;
    } else {
	COUNT_ABO(VLGETNEXTSRVBYID);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETNXT_ID_EXIT,
		   ICL_TYPE_LONG, VL_NOTYETIMPLEMENTED);
	return (error_status_t)VL_NOTYETIMPLEMENTED;
    }
}


/*
 * Get a vldb entry given the volume's name
 *
 * This gets the servers starting with the one numbered ``startHere'', unlike
 * GetEntryByName, which gets only the first few.  This procedure is really
 * here for future expansion without changing the representation on the wire.
 *
 * access=OPEN
 */
error_status_t VL_GetNextServersByName (connp, volname, startHere,
					nextStartP, aentry, flagsP)
rpc_binding_handle_t	connp;
volumeName 		volname;
u_long			startHere;
u_long			*nextStartP;
vldbentry		*aentry;	/* entry data copied here */
u_long			*flagsP;
{
    long errorcode;

    icl_Trace3(fl_iclSetp, FL_TRACE_GETNXT_NAME,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_STRING, volname,
	       ICL_TYPE_LONG, startHere);
    COUNT_REQ(VLGETNEXTSRVBYNAME);
    GLOBALLOCK;
    if (startHere == 0) {
	*nextStartP = 1;
	*flagsP = 1;	/* indicate that this call returns the final values */
	GLOBALUNLOCK;
	errorcode = VL_GetEntryByName(connp, volname, aentry);
	icl_Trace1(fl_iclSetp, FL_TRACE_GETNXT_NAME_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    } else if (startHere == 1) {
	COUNT_ABO(VLGETNEXTSRVBYNAME);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETNXT_NAME_EXIT,
		   ICL_TYPE_LONG, VL_ENDOFLIST);
	return (error_status_t)VL_ENDOFLIST;
    } else {
	COUNT_ABO(VLGETNEXTSRVBYNAME);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETNXT_NAME_EXIT,
		   ICL_TYPE_LONG, VL_NOTYETIMPLEMENTED);
	return (error_status_t)VL_NOTYETIMPLEMENTED;
    }
}

/*
 * Get all the information about a site, given one of its sockaddr's.
 *
 * access=OPEN
 */
error_status_t VL_GetSiteInfo(connp, OldAddr, FullSiteInfo)
rpc_binding_handle_t	connp;
afsNetAddr		*OldAddr;
siteDesc		*FullSiteInfo;
{
    u_long DescAddr;
    siteDesc DescBuff;
    struct ubik_trans *trans;
    long errorcode;

    icl_Trace3(fl_iclSetp, FL_TRACE_GETSITE,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_POINTER, OldAddr,
	       ICL_TYPE_POINTER, FullSiteInfo);
    COUNT_REQ(VLGETSITEINFO);
    GLOBALLOCK;
    if (errorcode = Init_VLdbase(&trans, LOCKREAD, VLGETSITEINFO)) {
	COUNT_ABO(VLGETSITEINFO);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETSITE_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = EnsureAddrEntry(trans, OldAddr, &DescAddr, 0, 0, 0);
    if (errorcode != 0) {
	COUNT_ABO(VLGETSITEINFO);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETSITE_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = GetSite(trans, DescAddr, &DescBuff);
    if (errorcode == 0 && DescBuff.DeletedFlag == SITEDELETEDSP1)
      errorcode = VL_DELETEDSITE;
    if (errorcode != 0) {
	COUNT_ABO(VLGETSITEINFO);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETSITE_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    *FullSiteInfo = DescBuff;
    errorcode = ubik_EndTrans(trans);
    GLOBALUNLOCK;
    icl_Trace1(fl_iclSetp, FL_TRACE_GETSITE_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}

/*
 * Generate all known sites known to the cell's VLDB.
 *
 * access=OPEN
 */
error_status_t VL_GenerateSites(connp, startHere, nextStartP, TheseSites,
				nSites)
rpc_binding_handle_t	connp;
u_long			startHere;
u_long			*nextStartP;
bulkSites		*TheseSites;
u_long			*nSites;
{
    u_long DiskAddr;
    int numSites;
    struct ubik_trans *trans;
    struct siteBlock Bk;
    u_long BkAddr;
    long errorcode;

    icl_Trace4(fl_iclSetp, FL_TRACE_GENSITE,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_LONG, startHere,
	       ICL_TYPE_POINTER, TheseSites,
	       ICL_TYPE_POINTER, nSites);
    COUNT_REQ(VLGENERATESITES);
    GLOBALLOCK;
    if (errorcode = Init_VLdbase(&trans, LOCKREAD, VLGENERATESITES)) {
	COUNT_ABO(VLGENERATESITES);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GENSITE_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    if (startHere == 0)
	DiskAddr = ntohl(cheader.vital_header.sitesPtr)
		     + offsetof (struct siteBlock, Sites);
    else DiskAddr = startHere;
    if (DiskAddr >= ntohl(cheader.vital_header.eofPtr)) {
	COUNT_ABO(VLGENERATESITES);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GENSITE_EXIT,
		   ICL_TYPE_LONG, VL_ENDOFLIST);
	return (error_status_t)VL_ENDOFLIST;
    }
    if (DiskAddr < ntohl(cheader.vital_header.sitesPtr)) {
	COUNT_ABO(VLGENERATESITES);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GENSITE_EXIT,
		   ICL_TYPE_LONG, VL_BADINDEX);
	return (error_status_t)VL_BADINDEX;
    }
    lock_ObtainRead(&vl_fsbLock);
    Bk.UsedHere = FirstSiteBlock.UsedHere;
    lock_ReleaseRead(&vl_fsbLock);
    for (numSites = 0;
	 (numSites < (sizeof(TheseSites->Sites)/sizeof(TheseSites->Sites[0])));
	 ++numSites) {
	if (DiskAddr <
	     (ntohl(cheader.vital_header.sitesPtr)
		+ offsetof (struct siteBlock, Sites)
		+ Bk.UsedHere * sizeof(siteDesc))) {
	    errorcode = GetSite(trans, DiskAddr,&(TheseSites->Sites[numSites]));
	    if (errorcode != 0) {
		COUNT_ABO(VLGENERATESITES);
		ubik_AbortTrans(trans);
		GLOBALUNLOCK;
		icl_Trace1(fl_iclSetp, FL_TRACE_GENSITE_EXIT,
			   ICL_TYPE_LONG, errorcode);
		return (error_status_t)errorcode;
	    }
	    else {
	      if (TheseSites->Sites[numSites].DeletedFlag == SITEDELETEDSP1)
		--numSites;
		DiskAddr += sizeof(siteDesc);
		continue; /* copied a site description, in network byte order */
	    }
	}
	break;	/* past the FirstSiteBlock range */
    }
    if (numSites < (sizeof(TheseSites->Sites)/sizeof(TheseSites->Sites[0]))) {
	lock_ObtainRead(&vl_fsbLock);
	Bk.NextPtr = FirstSiteBlock.NextPtr;
	Bk.UsedHere = FirstSiteBlock.UsedHere;
	lock_ReleaseRead(&vl_fsbLock);
	for (;;) {
	    if (Bk.NextPtr == (u_long)NULL) break;
	    BkAddr = Bk.NextPtr;
	    if (DiskAddr <= BkAddr)
		DiskAddr = BkAddr + offsetof (struct siteBlock, Sites);
	    errorcode = vlread(trans, BkAddr, (char *) &Bk, sizeof(Bk));
	    if (errorcode != 0) {
		COUNT_ABO(VLGENERATESITES);
		ubik_AbortTrans(trans);
		GLOBALUNLOCK;
		icl_Trace1(fl_iclSetp, FL_TRACE_GENSITE_EXIT,
			   ICL_TYPE_LONG, errorcode);
		return (error_status_t)errorcode;
	    }
	    if (Bk.Tag != htonl(SITEBLOCKTAG)) {
		COUNT_ABO(VLGENERATESITES);
		ubik_AbortTrans(trans);
		GLOBALUNLOCK;
		icl_Trace1(fl_iclSetp, FL_TRACE_GENSITE_EXIT,
			   ICL_TYPE_LONG, VL_SITESCORRUPTED);
		return (error_status_t)VL_SITESCORRUPTED;
	    }
	    Bk.NextPtr = ntohl(Bk.NextPtr);
	    Bk.UsedHere = ntohl(Bk.UsedHere);
	    while ((DiskAddr < (BkAddr + offsetof (struct siteBlock, Sites)
				+ Bk.UsedHere * sizeof(siteDesc))) &&
		   (numSites <
		    (sizeof(TheseSites->Sites)/sizeof(TheseSites->Sites[0])))) {
		errorcode = GetSite(trans, DiskAddr,
				    &(TheseSites->Sites[numSites]));
		if (errorcode != 0) {
		    COUNT_ABO(VLGENERATESITES);
		    ubik_AbortTrans(trans);
		    GLOBALUNLOCK;
		    icl_Trace1(fl_iclSetp, FL_TRACE_GENSITE_EXIT,
			       ICL_TYPE_LONG, errorcode);
		    return (error_status_t)errorcode;
		}
		if (TheseSites->Sites[numSites].DeletedFlag
		    != htonl(SITEDELETEDSP1)) {
		    ++numSites;
		}
		DiskAddr += sizeof(siteDesc);
	    }
	    if (numSites >=
		(sizeof(TheseSites->Sites)/sizeof(TheseSites->Sites[0])))
		break;
	}
    }
    *nSites = numSites;
    *nextStartP = DiskAddr;
    icl_Trace2(fl_iclSetp, FL_TRACE_GS_RES, ICL_TYPE_LONG, numSites,
	       ICL_TYPE_POINTER, DiskAddr);
    errorcode = ubik_EndTrans(trans);
    GLOBALUNLOCK;
    icl_Trace1(fl_iclSetp, FL_TRACE_GENSITE_EXIT, ICL_TYPE_LONG,
	       (numSites > 0 ? errorcode : VL_ENDOFLIST));
    return(numSites > 0 ? errorcode : VL_ENDOFLIST);
}

/*
 * Allocate numWanted volume IDs and return them, explicitly, via newIDs.
 *
 * access=Admin or serverOwner for the named server
 */
error_status_t VL_GetNewVolumeIds (connp, numWanted, Addrp, newIDs)
rpc_binding_handle_t	connp;
u_long			numWanted;
afsNetAddr		*Addrp;	/* for permission checking if not super-user */
bulkIDs			*newIDs;
{
    register long errorcode;
    long ok;
    afs_hyper_t CurrMax;
    struct ubik_trans *trans;
    register int i;
    int isAdmin;


    icl_Trace4(fl_iclSetp, FL_TRACE_GETIDS,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_LONG, numWanted,
	       ICL_TYPE_POINTER, Addrp,
	       ICL_TYPE_POINTER, newIDs);
    COUNT_REQ(VLGETNEWVOLUMEIDS);
    GLOBALLOCK;

    newIDs->bulkIDs_len = 0;
    isAdmin = (NotFlAdmin(connp) == 0);
    if (numWanted == 0 || numWanted > MAXBULKLEN) {
	COUNT_ABO(VLGETNEWVOLUMEIDS);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETIDS_EXIT,
		   ICL_TYPE_LONG, VL_BADVOLIDBUMP);
	return (error_status_t)VL_BADVOLIDBUMP;
    }
    if (errorcode = Init_VLdbase(&trans, LOCKWRITE, VLGETNEWVOLUMEIDS)) {
	COUNT_ABO(VLGETNEWVOLUMEIDS);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETIDS_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    if (!isAdmin) {
	if (Addrp == 0) {ok = 0; errorcode = 0;}
	else errorcode = AuthForAddress(connp, trans, Addrp, &ok);
	if (errorcode || !ok) {
	    COUNT_ABO(VLGETNEWVOLUMEIDS);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_GETIDS_EXIT, ICL_TYPE_LONG,
		       (errorcode ? errorcode : VL_PERM));
	    return (error_status_t)(errorcode ? errorcode : VL_PERM);
	}
    }
    DFSH_MemFromNetHyper(newIDs->bulkIDs_val[0],
			 cheader.vital_header.maxVolumeId);
    for (i = 1; i < numWanted; ++i) {
	newIDs->bulkIDs_val[i] = newIDs->bulkIDs_val[i-1];
	AFS_hadd32(newIDs->bulkIDs_val[i], 1);
    }
    i = numWanted-1;
    CurrMax = newIDs->bulkIDs_val[i];
    AFS_hadd32(CurrMax, 1);
    DFSH_NetFromMemHyper(cheader.vital_header.maxVolumeId, CurrMax);
    errorcode = write_vital_vlheader(trans);
    if (errorcode) {
	COUNT_ABO(VLGETNEWVOLUMEIDS);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETIDS_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = ubik_EndTrans(trans);
    GLOBALUNLOCK;
    newIDs->bulkIDs_len = numWanted;
    icl_Trace1(fl_iclSetp, FL_TRACE_GETIDS_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}

/*
 * Create a new site (server) in the FLDB.
 *
 * access=Admin
 */
error_status_t VL_CreateServer (connp, FullSiteInfo)
rpc_binding_handle_t	connp;
siteDesc		*FullSiteInfo;
{
    register long errorcode;
    afs_hyper_t CurrMax;
    u_long DescAddr;
    struct ubik_trans *trans;
    register int Ix;

    icl_Trace2(fl_iclSetp, FL_TRACE_CRSRV,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_POINTER, FullSiteInfo);
    COUNT_REQ(VLCREATESERVER);
    GLOBALLOCK;

    if (NotFlAdmin(connp)) {
	COUNT_ABO(VLCREATESERVER);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_CRSRV_EXIT, ICL_TYPE_LONG, VL_PERM);
	return (error_status_t)VL_PERM;
    }

    for (Ix = 0; Ix < ADDRSINSITE; ++Ix) {
	if (Ix > 0 && FullSiteInfo->Addr[Ix].type == ((unsigned short) -1))
	    break;
	if (CheckafsNetAddr(&FullSiteInfo->Addr[Ix])) {
	    COUNT_ABO(VLCREATESERVER);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_CRSRV_EXIT,
		       ICL_TYPE_LONG, VL_BADENTRY);
	    return (error_status_t)VL_BADENTRY;
	}
    }
    if (FullSiteInfo->DeletedFlag == SITEDELETEDSP1) {
	COUNT_ABO(VLCREATESERVER);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_CRSRV_EXIT, ICL_TYPE_LONG, VL_BADENTRY);
	return (error_status_t)VL_BADENTRY;
    }
    if ((errorcode = PrincipalExists(&FullSiteInfo->KerbPrin[0]))){
	COUNT_ABO(VLCREATESERVER);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_CRSRV_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    if (errorcode = Init_VLdbase(&trans, LOCKWRITE, VLCREATESERVER)) {
	COUNT_ABO(VLCREATESERVER);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_CRSRV_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    /* Make sure none of the given addresses is yet known */
    for (Ix = 0; Ix < ADDRSINSITE; ++Ix) {
	if (FullSiteInfo->Addr[Ix].type == ((unsigned short) -1)) break;
	errorcode = EnsureAddrEntry(trans, &FullSiteInfo->Addr[Ix], &DescAddr,
				    0, 0, 0);
	if (errorcode != VL_NOSERVER) {
	    COUNT_ABO(VLCREATESERVER);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_CRSRV_EXIT, ICL_TYPE_LONG,
		       (errorcode ? errorcode : VL_ADDRESSELSEWHERE));
	    return (error_status_t)
	      (errorcode ? errorcode : VL_ADDRESSELSEWHERE);
	}
    }
    /* Now create the server entry. */
    errorcode = EnsureAddrEntry(trans, &FullSiteInfo->Addr[0], &DescAddr,
				1, 0, 0);
    if (errorcode != 0) {
	COUNT_ABO(VLCREATESERVER);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_CRSRV_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    /* Overwrite it with the information we have */
    errorcode = PutSite(trans, DescAddr, FullSiteInfo, 1);
    if (errorcode != 0) {
	COUNT_ABO(VLCREATESERVER);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_CRSRV_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    --Ix;	/* use the last address given */
    errorcode=ubik_EndTrans(trans);
    GLOBALUNLOCK;
    if (flserver_verbose)
	dce_fprintf(stderr, lsv_s_flserver_created_site,
		    inet_ntoa(((struct sockaddr_in *)
			       &FullSiteInfo->Addr[Ix])->sin_addr),
		    ntohl(((struct sockaddr_in *)
			   &FullSiteInfo->Addr[Ix])->sin_addr.s_addr),
		    &FullSiteInfo->KerbPrin[0],
		    ntohl(FullSiteInfo->CreationQuota));
    cmd_explanation = dce_msg_get_msg(lsv_s_log_flserver_created_site, &st);
    Log( (char *) cmd_explanation,
	 inet_ntoa(((struct sockaddr_in *)&FullSiteInfo->Addr[Ix])->sin_addr),
	 ntohl(((struct sockaddr_in *)
		&FullSiteInfo->Addr[Ix])->sin_addr.s_addr),
	 &FullSiteInfo->KerbPrin[0], ntohl(FullSiteInfo->CreationQuota));
    free(cmd_explanation);
    icl_Trace1(fl_iclSetp, FL_TRACE_CRSRV_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}

/*
 * see if the vlserver is back yet
 *
 * access=OPEN
 */
error_status_t VL_Probe(connp)
rpc_binding_handle_t	connp;
{
    icl_Trace1(fl_iclSetp, FL_TRACE_PROBE, ICL_TYPE_POINTER, connp);
    COUNT_REQ(VLPROBE);
    return error_status_ok;
}

/*
 * Get a vldb entry given its volume id; make sure it's not a deleted entry.
 *
 * access=OPEN
 */
error_status_t VL_GetCEntryByID (connp, volid, voltype, centryp)
rpc_binding_handle_t	connp;
afs_hyper_t		*volid;
u_long			voltype;
compactvldbentry	*centryp;	/* entry data copied here */
{
    struct ubik_trans	*trans;
    long		blockindex, errorcode;
    struct vlentry	tentry;

    icl_Trace4(fl_iclSetp, FL_TRACE_GETCENT_ID,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_HYPER, volid,
	       ICL_TYPE_LONG, voltype,
	       ICL_TYPE_POINTER, centryp);
    dlprintf(dce_flserver_debug,1,("fls: in VL_GetCEntryByID\n"));
    COUNT_REQ(VLGETCENTRYBYID);
    GLOBALLOCK;
    if (voltype != (unsigned long) -1) {
	voltype = VolTypeToVolTix(voltype);
	if (voltype == (unsigned long) -1) {
	    COUNT_ABO(VLGETCENTRYBYID);
	    GLOBALUNLOCK;
	    dlprintf(dce_flserver_debug,1,("fls: GetCEntryByID: bad type %lu\n",
					   voltype));
	    icl_Trace1(fl_iclSetp, FL_TRACE_GETCENT_ID_EXIT,
		       ICL_TYPE_LONG, VL_BADVOLTYPE);
	    return (error_status_t)VL_BADVOLTYPE;
	}
    }
    if (errorcode = Init_VLdbase (&trans, LOCKREAD, VLGETCENTRYBYID)) {
	COUNT_ABO(VLGETCENTRYBYID);
	GLOBALUNLOCK;
	dlprintf(dce_flserver_debug, 1,
		 ("fls: GetCEntryByID: Init_VLdbase returns %lu\n", errorcode));
	icl_Trace1(fl_iclSetp, FL_TRACE_GETCENT_ID_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    blockindex = FindByID(trans, volid, voltype, &tentry, &errorcode);
    if (blockindex == 0) {		/* entry not found */
	COUNT_ABO(VLGETCENTRYBYID);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	dlprintf(dce_flserver_debug, 1,
		 ("fls: GetCEntryByID: FindByID returns %lu\n", errorcode));
	icl_Trace1(fl_iclSetp, FL_TRACE_GETCENT_ID_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    dlprintf(dce_flserver_debug, 1,
	     ("fls: GetCEntryByID: calling vl_to_comvldb\n"));
    /* Convert from the internal to external form */
    errorcode = vlentry_to_comvldbentry(trans, &tentry, centryp);
    if (errorcode != 0) {
	COUNT_ABO(VLGETCENTRYBYID);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	dlprintf(dce_flserver_debug, 1,
		 ("fls: GetCEntryByID: vl_to_comvldb returns %lu\n",
		  errorcode));
	icl_Trace1(fl_iclSetp, FL_TRACE_GETCENT_ID_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    dlprintf(dce_flserver_debug, 1,
	     ("fls: GetCEntryByID: calling ubik_EndTrans()\n"));
    errorcode = ubik_EndTrans(trans);
    GLOBALUNLOCK;
    dlprintf(dce_flserver_debug,1,("fls: GetCEntryByID returning %lu\n",
				   errorcode));
    icl_Trace1(fl_iclSetp, FL_TRACE_GETCENT_ID_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}

/*
 * Get a vldb entry given the volume's name;
 * of course, very similar to VLGetEntryByID() above.
 *
 * access=OPEN
 */
error_status_t VL_GetCEntryByName (connp, volname, centryp)
rpc_binding_handle_t	connp;
volumeName		volname;
compactvldbentry	*centryp;	/* entry data copied here */
{
    struct ubik_trans	*trans;
    long		blockindex, errorcode;
    struct vlentry	tentry;
    afs_hyper_t Imputed;

    icl_Trace3(fl_iclSetp, FL_TRACE_GETCENT_NAME,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_STRING, volname,
	       ICL_TYPE_POINTER, centryp);
    COUNT_REQ(VLGETCENTRYBYNAME);
    GLOBALLOCK;
    if (dfsh_StrToHyper(volname, &Imputed, NULL) == 0) {
	COUNT_ABO(VLGETCENTRYBYNAME);
	GLOBALUNLOCK;
	errorcode = VL_GetCEntryByID(connp, &Imputed, -1, centryp);
	icl_Trace1(fl_iclSetp, FL_TRACE_GETCENT_NAME_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    if (InvalidVolname(volname)) {
	COUNT_ABO(VLGETCENTRYBYNAME);
	GLOBALUNLOCK;
	return (error_status_t)VL_BADNAME;
    }
    if (errorcode = Init_VLdbase(&trans, LOCKREAD, VLGETCENTRYBYNAME)) {
	COUNT_ABO(VLGETCENTRYBYNAME);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETCENT_NAME_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    blockindex = FindByName(trans, volname, &tentry, &errorcode);
    if (blockindex == 0) {		/* entry not found */
	COUNT_ABO(VLGETCENTRYBYNAME);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETCENT_NAME_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    /* Convert to external entry representation */
    errorcode = vlentry_to_comvldbentry(trans, &tentry, centryp);
    if (errorcode != 0) {
	COUNT_ABO(VLGETCENTRYBYNAME);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETCENT_NAME_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = ubik_EndTrans(trans);
    GLOBALUNLOCK;
    icl_Trace1(fl_iclSetp, FL_TRACE_GETCENT_NAME_EXIT,
	       ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}

/*
 * Get a vldb entry given its volume id
 *
 * This gets the servers starting with the one numbered ``startHere'', unlike
 * GetEntryByID, which gets only the first few.  This procedure is really here
 * for future expansion without changing the representation on the wire.
 *
 * access=OPEN
 */
error_status_t VL_GetCNextServersByID (connp, volid, voltype, startHere,
				       nextStartP, centryp, flagsP)
rpc_binding_handle_t	connp;
afs_hyper_t		*volid;
u_long			voltype;
u_long			startHere;
u_long			*nextStartP;
compactvldbentry	*centryp;	/* entry data copied here */
u_long			*flagsP;
{
    long errorcode;

    icl_Trace4(fl_iclSetp, FL_TRACE_GETCNXT_ID,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_HYPER, volid,
	       ICL_TYPE_LONG, voltype,
	       ICL_TYPE_LONG, startHere);
    icl_Trace2(fl_iclSetp, FL_TRACE_GETCNXT_ID_STR,
	       ICL_TYPE_POINTER, centryp,
	       ICL_TYPE_POINTER, flagsP);
    COUNT_REQ(VLGETCNEXTSRVBYID);
    GLOBALLOCK;
    if (startHere == 0) {
	*nextStartP = 1;
	*flagsP = 1;	/* indicate that this call returns the final values */
	GLOBALUNLOCK;
	errorcode = VL_GetCEntryByID(connp, volid, voltype, centryp);
	icl_Trace1(fl_iclSetp, FL_TRACE_GETCNXT_ID_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    } else if (startHere == 1) {
	COUNT_ABO(VLGETCNEXTSRVBYID);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETCNXT_ID_EXIT,
		   ICL_TYPE_LONG, VL_ENDOFLIST);
	return (error_status_t)VL_ENDOFLIST;
    } else {
	COUNT_ABO(VLGETCNEXTSRVBYID);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETCNXT_ID_EXIT,
		   ICL_TYPE_LONG, VL_NOTYETIMPLEMENTED);
	return (error_status_t)VL_NOTYETIMPLEMENTED;
    }
}


/*
 * Get a vldb entry given the volume's name
 *
 * This gets the servers starting with the one numbered ``startHere'', unlike
 * GetEntryByName, which gets only the first few.  This procedure is really
 * here for future expansion without changing the representation on the wire.
 *
 * access=OPEN
 */
error_status_t VL_GetCNextServersByName (connp, volname, startHere,
					 nextStartP, centryp, flagsP)
rpc_binding_handle_t	connp;
volumeName		volname;
u_long			startHere;
u_long			*nextStartP;
compactvldbentry	*centryp;	/* entry data copied here */
u_long			*flagsP;
{
    long errorcode;

    icl_Trace3(fl_iclSetp, FL_TRACE_GETCNXT_NAME,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_STRING, volname,
	       ICL_TYPE_LONG, startHere);
    icl_Trace2(fl_iclSetp, FL_TRACE_GETCNXT_NAME_STR,
	       ICL_TYPE_POINTER, centryp,
	       ICL_TYPE_POINTER, flagsP);
    COUNT_REQ(VLGETCNEXTSRVBYNAME);
    GLOBALLOCK;
    if (startHere == 0) {
	*nextStartP = 1;
	*flagsP = 1;	/* indicate that this call returns the final values */
	GLOBALUNLOCK;
	errorcode = VL_GetCEntryByName(connp, volname, centryp);
	icl_Trace1(fl_iclSetp, FL_TRACE_GETCNXT_NAME_EXIT,
		   ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    } else if (startHere == 1) {
	COUNT_ABO(VLGETCNEXTSRVBYNAME);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETCNXT_NAME_EXIT,
		   ICL_TYPE_LONG, VL_ENDOFLIST);
	return (error_status_t)VL_ENDOFLIST;
    } else {
	COUNT_ABO(VLGETCNEXTSRVBYNAME);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_GETCNXT_NAME_EXIT,
		   ICL_TYPE_LONG, VL_NOTYETIMPLEMENTED);
	return (error_status_t)VL_NOTYETIMPLEMENTED;
    }
}

/*
 * Alter a site (server) in the FLDB.
 * Set the name of the authoritative Kerberos principal for this site to
 * the given name.
 *
 * access=Admin
 */
error_status_t VL_AlterServer(connp, AddrOfHost, AttrsP)
rpc_binding_handle_t	connp;
afsNetAddr		*AddrOfHost;
struct siteAlter	*AttrsP;
{
    struct ubik_trans *trans;
    siteDesc DescBuff;
    siteDesc altDescBuff;	/* a copy of DescBuff made before deletion */
    long errorcode;
    u_long DescAddr;

    icl_Trace3(fl_iclSetp, FL_TRACE_ALTS,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_POINTER, AddrOfHost,
	       ICL_TYPE_POINTER, AttrsP);
    COUNT_REQ(VLALTERSERVER);
    GLOBALLOCK;

    if (NotFlAdmin(connp)) {
	COUNT_ABO(VLALTERSERVER);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_ALTS_EXIT, ICL_TYPE_LONG, VL_PERM);
	return (error_status_t)VL_PERM;
    }

    if (errorcode = Init_VLdbase(&trans, LOCKWRITE, VLALTERSERVER)) {
	COUNT_ABO(VLALTERSERVER);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_ALTS_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = EnsureAddrEntry(trans, AddrOfHost, &DescAddr, 0, 0, 0);
    if (errorcode != 0) {
	COUNT_ABO(VLALTERSERVER);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_ALTS_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = GetSite(trans, DescAddr, &DescBuff);
    if (errorcode == 0 && DescBuff.DeletedFlag == SITEDELETEDSP1)
	errorcode = VL_DELETEDSITE;
    if (errorcode != 0) {
	COUNT_ABO(VLALTERSERVER);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_ALTS_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    if (AttrsP->Mask & SITEALTER_DELETEME) {
	register int i;
	struct vlentry tentry;
	long int nextblockindex, count;

	if (AttrsP->Mask != SITEALTER_DELETEME) {
	    /* must be those bits and no others! */
	    COUNT_ABO(VLALTERSERVER);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_ALTS_EXIT,
		       ICL_TYPE_LONG, VL_BADINDEX);
	    return (error_status_t)VL_BADINDEX;
	}
	if (DescBuff.CreationUses != 0) {
	    COUNT_ABO(VLALTERSERVER);
	    ubik_AbortTrans(trans);
	    GLOBALUNLOCK;
	    icl_Trace1(fl_iclSetp, FL_TRACE_ALTS_EXIT,
		       ICL_TYPE_LONG, VL_SITEINUSE);
	    return (error_status_t)VL_SITEINUSE;
	}
	/* Search the FLDB, just to make sure this entry isn't used. */
	nextblockindex = 0; count = 0;
	while (nextblockindex = NextEntry(trans, nextblockindex,
					  &tentry, &count)) {
	    for (i = 0; i < MAXNSERVERS && tentry.serverSite[i] != 0; ++i) {
		if (tentry.serverSite[i] == DescAddr) {
		    COUNT_ABO(VLALTERSERVER);
		    ubik_AbortTrans(trans);
		    GLOBALUNLOCK;
		    icl_Trace1(fl_iclSetp, FL_TRACE_ALTS_EXIT,
			       ICL_TYPE_LONG, VL_INCONSISTENT);
		    return (error_status_t)VL_INCONSISTENT;
		}
	    }
	}

	/*
	 * Before deleting the site, save the site info so we can append
	 * a note to the log about this deletion. We save the entire siteDesc
	 * structure instead of just the ones we want now because logging the
	 * other fields of the siteDesc structure can be done at a later date
	 * without worrying about whether the fields were zeroed out.
	 */
	altDescBuff = DescBuff;

	/* OK, just delete the silly thing. */
	bzero((char *)&DescBuff, sizeof(DescBuff));
	for (i = 0; i < ADDRSINSITE; i++) {
	    DescBuff.Addr[i].type = ((unsigned short) -1);
	}
	DescBuff.DeletedFlag = SITEDELETEDSP1;
	/* adjust the counters in the site block */

    } else {
	if (AttrsP->Mask & SITEALTER_PRINCIPAL) {
            if ((errorcode = PrincipalExists(&AttrsP->KerbPrin[0])) == 0)
	        bcopy((char *)&AttrsP->KerbPrin[0],
		      (char *)&DescBuff.KerbPrin[0], MAXKPRINCIPALLEN);
	}
	if (AttrsP->Mask & SITEALTER_OWNER) {
	    DescBuff.Owner = AttrsP->Owner;
	}
	if (AttrsP->Mask & SITEALTER_OBJID) {
	    DescBuff.ObjID = AttrsP->ObjID;
	}
	if (AttrsP->Mask & SITEALTER_CREATIONQUOTA) {
	    DescBuff.CreationQuota = AttrsP->CreationQuota;
	}
	/* for fts's syncfldb operation */
	if (AttrsP->Mask & SITEALTER_CREATIONUSES) {
	    DescBuff.CreationUses = AttrsP->CreationUses;
	}
    }
    if (errorcode == 0)
        errorcode = PutSite(trans, DescAddr, &DescBuff, 1);
    if (errorcode != 0) {
	COUNT_ABO(VLALTERSERVER);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_ALTS_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode=ubik_EndTrans(trans);
    GLOBALUNLOCK;

    if (DescBuff.DeletedFlag == htonl(SITEDELETEDSP1)) {
	if (flserver_verbose)
	    dce_fprintf(stderr, lsv_s_flserver_deleted_these, "Deleted",
			inet_ntoa(((struct sockaddr_in *)
				   &altDescBuff.Addr[0])->sin_addr),
			ntohl(((struct sockaddr_in *)
			       &altDescBuff.Addr[0])->sin_addr.s_addr),
			&altDescBuff.KerbPrin[0],
			ntohl(altDescBuff.CreationQuota));
	cmd_explanation = dce_msg_get_msg(lsv_s_log_flserver_deleted_these,
					  &st);
	Log( (char *) cmd_explanation, "Deleted",
	     inet_ntoa(((struct sockaddr_in *)&altDescBuff.Addr[0])->sin_addr),
	     ntohl(((struct sockaddr_in *)
		    &altDescBuff.Addr[0])->sin_addr.s_addr),
	     &altDescBuff.KerbPrin[0],  ntohl(altDescBuff.CreationQuota));
	free(cmd_explanation);
    }
    else {
	if (flserver_verbose)
	    dce_fprintf(stderr, lsv_s_flserver_edited_these, "Edited",
			inet_ntoa(((struct sockaddr_in *)
				   &DescBuff.Addr[0])->sin_addr),
			ntohl(((struct sockaddr_in *)
			       &DescBuff.Addr[0])->sin_addr.s_addr),
			&DescBuff.KerbPrin[0],  ntohl(DescBuff.CreationQuota));
	cmd_explanation = dce_msg_get_msg(lsv_s_log_flserver_edited_these, &st);
	Log( (char *) cmd_explanation, "Edited",
	     inet_ntoa(((struct sockaddr_in *) &DescBuff.Addr[0])->sin_addr),
	     ntohl(((struct sockaddr_in *) &DescBuff.Addr[0])->sin_addr.s_addr),
	     &DescBuff.KerbPrin[0],  ntohl(DescBuff.CreationQuota));
	free(cmd_explanation);
    }
    icl_Trace1(fl_iclSetp, FL_TRACE_ALTS_EXIT, ICL_TYPE_LONG, errorcode);
    return (error_status_t)errorcode;
}

/*
 * Expand an abbreviated site description found in a compactvldbentry.
 *
 * access=OPEN
 */
error_status_t VL_ExpandSiteCookie (connp, Cookie, FullSiteInfo)
rpc_binding_handle_t	connp;
unsigned long		Cookie;
siteDesc		*FullSiteInfo;
{
    register long      errorcode;
    afs_hyper_t CurrMax;
    struct ubik_trans *trans;

    icl_Trace3(fl_iclSetp, FL_TRACE_EXPS,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_LONG, Cookie,
	       ICL_TYPE_POINTER, FullSiteInfo);
    COUNT_REQ(VLEXPANDSITECOOKIE);
    GLOBALLOCK;
    if (errorcode = Init_VLdbase(&trans, LOCKREAD, VLEXPANDSITECOOKIE)) {
	COUNT_ABO(VLEXPANDSITECOOKIE);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_EXPS_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode = CarefullyGetSite(trans, Cookie, FullSiteInfo);
    if (errorcode == 0 && FullSiteInfo->DeletedFlag == SITEDELETEDSP1)
      errorcode = VL_DELETEDSITE;
    if (errorcode != 0) {
	COUNT_ABO(VLGETSITEINFO);
	ubik_AbortTrans(trans);
	GLOBALUNLOCK;
	icl_Trace1(fl_iclSetp, FL_TRACE_EXPS_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    errorcode=ubik_EndTrans(trans);
    GLOBALUNLOCK;
    icl_Trace1(fl_iclSetp, FL_TRACE_EXPS_EXIT, ICL_TYPE_LONG, errorcode);

    return (error_status_t)errorcode;
}

error_status_t VL_GetServerInterfaces (connp, serverInterfacesP)
rpc_binding_handle_t	connp;
dfs_interfaceList	*serverInterfacesP;
{
/*    error_status_t st;    */

    icl_Trace2(fl_iclSetp, FL_TRACE_GIF,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_POINTER, serverInterfacesP);
    COUNT_REQ(VLGETSERVERINTERFACES);
    dfs_GetServerInterfaces(VL__v4_0_s_ifspec, serverInterfacesP, &st);
    icl_Trace1(fl_iclSetp, FL_TRACE_GIF_EXIT, ICL_TYPE_LONG, st);
    return st;
}

/* ============> End of Exported vldb RPC functions <============= */

#if 0
/*
 * Return the username associated with the given connection, or a non-zero code.
 */
static int GetConnUsername(acall, whereP)
struct rx_call *acall;
char *whereP; {
    register struct rx_connection *tconn;
    register long code;
    struct rxvab_conn *tc;
    long viceID;

    if (!vldb_confdir) return 1;
    tconn = rx_ConnectionOf(acall);
    /* check for call from magic localhost reserved address */
    if (rx_HostOf(rx_PeerOf(tconn)) == htonl(0x7f000001)) return 1;
    code = rx_SecurityClassOf(tconn);
    if (code == 0) return 1;	    /* not authenticated at all; no name */
    else if (code == 1) {
	/* bcrypt tokens */
	tc = (struct rxvab_conn *) tconn->securityData;
	if (!tc) return 1;	    /* not handled properly */
	viceID = ntohl(tc->viceID);
	{
	char *dcesptr;
	dcesptr = dce_sprintf(lsv_s_afs_vice_id, viceID);
	strcpy(whereP , dcesptr);
	free(dcesptr);
	}
	return 0;
    }
    else if (code == 2) {
#ifdef notdef /* security */
	char tname[KTC_MAXNAMELEN];	/* authentication from ticket */
	char tinst[KTC_MAXNAMELEN];
	char tcell[KTC_MAXREALMLEN];
	char uname[KTC_MAXNAMELEN];	/* name.instance */
	int  ilen;			/* length of instance */
	unsigned long exp;
	static char localcellname[MAXVLCELLCHARS] = "";
#ifdef AFS_ATHENA_STDENV
	char local_realm[REALM_SZ];
#endif /* AFS_ATHENA_STDENV */

	/* des tokens */
	code = rxkad_GetServerInfo
	    (acall->conn, (long *) 0, &exp, tname, tinst, tcell, (long *) 0);
	if (code) return 1; /* bogus */

	if (strlen (tcell)) {
	    if (!localcellname[0])
		afsconf_GetLocalCell
		    (vldb_confdir, localcellname, sizeof(localcellname));
#ifdef AFS_ATHENA_STDENV
	    if (krb_get_lrealm(local_realm, 0) != KSUCCESS)
		strncpy(local_realm, localcellname, REALM_SZ);
	    if (strcasecmp(local_realm, tcell))
#else /* AFS_ATHENA_STDENV */
	    if (strcasecmp(localcellname, tcell))
#endif /* AFS_ATHENA_STDENV */
		{
		    return 1;
		}
	}
	ilen = strlen(tinst);
	strncpy (uname, tname, sizeof(uname));
	if (ilen) {
	    if (strlen(uname) + 1 + ilen >= sizeof(uname)) return 1;
	    strcat (uname, ".");
	    strcat (uname, tinst);
	}

	if (exp < osi_Time()) return 1;	/* expired tix */
	if (strcmp("afs", uname) == 0) return 1;
	else strcpy(whereP, uname);
#endif /* notdef */
	return 0;
    }
    else return 1;	    /* mysterious, just say no */
}
#endif /* 0 */

/* Common code to actually remove a vldb entry from the database. */
static int RemoveEntry(trans, entryptr, tentry)
struct ubik_trans   *trans;
long		    entryptr;
struct vlentry	    *tentry;
{
    register long errorcode;

    icl_Trace3(fl_iclSetp, FL_TRACE_REMENT,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, entryptr,
	       ICL_TYPE_POINTER, tentry);
    if (errorcode = UnthreadVLentry(trans, entryptr, tentry)) {
	icl_Trace1(fl_iclSetp, FL_TRACE_REMENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    if (errorcode = FreeBlock(trans, entryptr)) {
	icl_Trace1(fl_iclSetp, FL_TRACE_REMENT_EXIT, ICL_TYPE_LONG, errorcode);
	return (error_status_t)errorcode;
    }
    icl_Trace1(fl_iclSetp, FL_TRACE_REMENT_EXIT, ICL_TYPE_LONG, 0);
    return 0;
}

/* Bump the site quota for all sites that this entry points to. */
static int FreeSiteQuota(trans, tentry)
struct ubik_trans   *trans;
struct vlentry	    *tentry;
{
    int Ix;
    long i;
    struct siteDesc DescBuff;

    icl_Trace2(fl_iclSetp, FL_TRACE_FREES,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_POINTER, tentry);
    for (Ix = 0;
	 Ix < MAXNSERVERS && (tentry->serverFlags[Ix] & VLSF_DEFINED) != 0;
	 ++Ix) {
	i = GetSite(trans, tentry->serverSite[Ix], &DescBuff);
	if (i != 0) return i;
	--DescBuff.CreationUses;
	i = PutSite(trans, tentry->serverSite[Ix], &DescBuff, 1);
	if (i != 0) {
	    icl_Trace1(fl_iclSetp, FL_TRACE_FREES_EXIT, ICL_TYPE_LONG, 0);
	    return i;
	}
    }
    icl_Trace1(fl_iclSetp, FL_TRACE_FREES_EXIT, ICL_TYPE_LONG, 0);
    return 0;
}

static void ReleaseEntry(tentry, releasetype)
struct vlentry	*tentry;
long		releasetype;
{
    if (releasetype & LOCKREL_TIMESTAMP)
	tentry->LockTimestamp = 0;
    if (releasetype & LOCKREL_OPCODE)
	tentry->flags &= ~VLOP_ALLOPERS;
    if (releasetype & LOCKREL_AFSID)
	tentry->NameOfLocker[0] = '\0';
}


/*
 * Verify that the incoming vldb entry is valid;
 * multi type of error codes are returned.
 */
static int
check_vldbentry(aentry, rwvolp, rovolp, backvolp)
vldbentry	*aentry;
afs_hyper_t	*rwvolp, *rovolp, *backvolp;
{
    long	i, Type;

    if (InvalidVolname(aentry->name))
	return (error_status_t)VL_BADNAME;
    if (aentry->volumeType != (unsigned long) -1) {
	if (VolTypeToVolTix(aentry->volumeType) == -1)
	    return (error_status_t)VL_BADVOLTYPE;
    }
    if (aentry->nServers <= 0 || aentry->nServers > MAXNSERVERS)
	return (error_status_t)VL_BADSERVER;
    AFS_hzero(*rwvolp);
    AFS_hzero(*rovolp);
    AFS_hzero(*backvolp);
    for (i = 0; i < MAXVOLTYPES && !AFS_hiszero(aentry->VolIDs[i]); ++i) {
	Type = VolTypeToVolTix(aentry->VolTypes[i]);
	if (Type == -1) {
	    return (error_status_t)VL_BADVOLTYPE;
	}
	if (Type == RWVOL) *rwvolp = aentry->VolIDs[i];
	else if (Type == ROVOL) *rovolp = aentry->VolIDs[i];
	else if (Type == BACKVOL) *backvolp = aentry->VolIDs[i];
	else return (error_status_t)VL_BADVOLTYPE;
    }
    return 0;
}


#define	LocVLSF_ROVOL	    0x02
#define	LocVLSF_RWVOL	    0x04
#define	LocVLSF_BACKVOL	    0x08
static long OldSFFromIx[MAXTYPES] = {
    LocVLSF_RWVOL,
    LocVLSF_ROVOL,
    LocVLSF_BACKVOL};

/*
 * Convert from the external vldb entry representation to its internal
 * (more compact) form.  This call should not change the hash chains!
 */
static int
vldbentry_to_vlentry(atrans, VldbEntry, VlEntry)
struct ubik_trans	*atrans;
vldbentry		*VldbEntry;
struct vlentry		*VlEntry;
{
    int i, j, k, index;
    u_long msk;
    u_int32 nextIdHash[MAXTYPES];
    u_int32 LockTimestamp;
    u_int32 nextNameHash;
    int trans[MAXVOLTYPES];
    char    NameOfLocker[MAXLOCKNAMELEN];

    icl_Trace3(fl_iclSetp, FL_TRACE_VDTOVE,
	       ICL_TYPE_POINTER, atrans,
	       ICL_TYPE_POINTER, VldbEntry,
	       ICL_TYPE_POINTER, VlEntry);
    dlprintf(dce_flserver_debug,1,("fls: vldb2vl: entered\n"));
    /* save everything we aren't going to overwrite before we blast the entry */
    for(i=0; i < MAXTYPES; i++)
	nextIdHash[i] = VlEntry->nextIdHash[i];
    nextNameHash = VlEntry->nextNameHash;
    (void) strncpy(NameOfLocker, VlEntry->NameOfLocker, MAXLOCKNAMELEN);
    LockTimestamp = VlEntry->LockTimestamp;

    bzero((char *)VlEntry, sizeof(struct vlentry));

    /* stick it all back in (I wanted to just remove the bzero, but...) */
    for(i=0; i < MAXTYPES; i++)
	VlEntry->nextIdHash[i] = nextIdHash[i];
    VlEntry->nextNameHash = nextNameHash;
    (void) strncpy(VlEntry->NameOfLocker, NameOfLocker, MAXLOCKNAMELEN);
    VlEntry->LockTimestamp = LockTimestamp;
    VlEntry->vlent_min_lcl_authn = VldbEntry->charSpares[VLAUTHN_MINLCLLEVEL];
    VlEntry->vlent_max_lcl_authn = VldbEntry->charSpares[VLAUTHN_MAXLCLLEVEL];
    VlEntry->vlent_min_rmt_authn = VldbEntry->charSpares[VLAUTHN_MINRMTLEVEL];
    VlEntry->vlent_max_rmt_authn = VldbEntry->charSpares[VLAUTHN_MAXRMTLEVEL];

    strncpy(VlEntry->name, (char *)VldbEntry->name, sizeof(VlEntry->name));
    i = VolTypeToVolTix(VldbEntry->volumeType);
    if (i == -1) return (error_status_t)VL_BADVOLTYPE;
    VlEntry->volumeTIx = i;
    bzero((char *)&VlEntry->volumeId[0], sizeof(VlEntry->volumeId));
    for (index = 0; index < MAXVOLTYPES; ++index) trans[index] = -1;
    for (index = 0; index < MAXVOLTYPES; ++index) {
	if (AFS_hiszero(VldbEntry->VolIDs[index])) break;
	i = VolTypeToVolTix(VldbEntry->VolTypes[index]);
	if (i == -1) return (error_status_t)VL_BADVOLTYPE;
	DFSH_NetFromMemHyper(VlEntry->volumeId[i], VldbEntry->VolIDs[index]);
	trans[index] = i;
    }
    dlprintf(dce_flserver_debug, 1,
	     ("fls: vldb2vl: trans is %d %d %d %d %d %d %d %d\n",
	      trans[0], trans[1], trans[2], trans[3], trans[4], trans[5],
	      trans[6], trans[7]));
    if (VldbEntry->nServers > MAXNSERVERS) return (error_status_t)VL_REPSFULL;
    k = 0;
    for (i = 0; i < VldbEntry->nServers; ++i) {
	/* skip sites that we've already added to the VlEntry */
	if (VldbEntry->siteFlags[i] & VLSF_SAMEASPREV) continue;
	/* 6th param says to decrement site's creation quota on success */
	dlprintf(dce_flserver_debug, 1,
		 ("fls: vldb2vl: about to EnsureAddrEntry (index %d/%d)\n",
		  i, VldbEntry->nServers));
	j = EnsureAddrEntry(atrans, &(VldbEntry->siteAddr[i]),
			    (u_long *)&index, 0 /* 1 */, 0, -1);
	dlprintf(dce_flserver_debug, 1,
		 ("fls: vldb2vl: EnsureAddrEntry returns %lu\n", j));
	if (j != 0) return j;
	dlprintf(dce_flserver_debug, 1,
		 ("fls: vldb2vl: EnsureAddrEntry about to store ix %d\n", k));
	VlEntry->serverSite[k] = index;
	VlEntry->serverPartition[k] = VldbEntry->sitePartition[i];
	VlEntry->sitemaxReplicaLatency[k] = VldbEntry->sitemaxReplicaLatency[i];
	VlEntry->serverFlags[k] =
	  0xff & ((VldbEntry->siteFlags[i] | VLSF_DEFINED) & ~VLSF_SAMEASPREV);
	VlEntry->serverFlags[k] &=
	  ~(LocVLSF_ROVOL | LocVLSF_RWVOL | LocVLSF_BACKVOL);
	/* now translate the which-vols-on-this-server flags */
	msk = VLSF_ZEROIXHERE;
	dlprintf(dce_flserver_debug, 1, ("fls: vldb2vl: (%d) old flags %x\n",
					 k, VlEntry->serverFlags[k]));
	for (index = 0; index < MAXVOLTYPES; ++index) {
	    if ((msk & VldbEntry->siteFlags[i]) != 0 && trans[index] >= 0) {
		VlEntry->serverFlags[k] |= OldSFFromIx[trans[index]];
	    }
	    msk >>= 1;
	}
	dlprintf(dce_flserver_debug, 1, ("fls: vldb2vl: (%d) new flags %x\n",
					 k, VlEntry->serverFlags[k]));
	++k;
    }
    /* clear out the rest of the VlEntry server slots, if any */
    dlprintf(dce_flserver_debug, 1,("fls: vldb2vl: clearing from %d up to %d\n",
				     k, MAXNSERVERS));
    for ( ; k < MAXNSERVERS; ++k) {
	VlEntry->serverSite[k] = 0;
	VlEntry->serverPartition[k] = 0;
	VlEntry->sitemaxReplicaLatency[k] = 0;
	VlEntry->serverFlags[k] = 0;
    }
    VlEntry->flags = VldbEntry->flags;
    VlEntry->maxTotalLatency = VldbEntry->maxTotalLatency;
    VlEntry->hardMaxTotalLatency = VldbEntry->hardMaxTotalLatency;
    VlEntry->minimumPounceDally = VldbEntry->minimumPounceDally;
    VlEntry->defaultMaxReplicaLatency = VldbEntry->defaultMaxReplicaLatency;
    VlEntry->reclaimDally = VldbEntry->reclaimDally;
    DFSH_NetFromMemHyper(VlEntry->cloneId, VldbEntry->cloneId);
    dlprintf(dce_flserver_debug,1,("fls: vldb2vl: returning all-OK\n"));
    icl_Trace0(fl_iclSetp, FL_TRACE_VDTOVE_EXIT);
    return 0;
}


#if 0
/*
 * Check if the given afsNetAddr appears in the given VLDB entry.
 * If so, return its index via wherep, or else -1.
 */
static long SiteExists(trans, vlEntry, Addr, Part, incrOnSuccess, wherep)
struct ubik_trans	*trans;
struct vlentry		*vlEntry;
afsNetAddr		*Addr;
long			Part;
int			incrOnSuccess;
long			*wherep;
{
    register long Ix, errCode;
    register int XIx;
    siteDesc SiteBuff;

    for (Ix = 0;
	 Ix < MAXNSERVERS && (vlEntry->serverFlags[Ix] & VLSF_DEFINED) != 0;
	 ++Ix) {
	if (Part == vlEntry->serverPartition[Ix]) {
	    errCode = GetSite(trans, vlEntry->serverSite[Ix], &SiteBuff);
	    if (errCode != 0) return errCode;
	    for (XIx = 0;
		 XIx < ADDRSINSITE
		   && SiteBuff.Addr[XIx].type != (unsigned short) -1;
		 ++XIx) {
		if (bcmp(&(SiteBuff.Addr[XIx]), Addr, sizeof(afsNetAddr)) == 0){
		    if (incrOnSuccess != 0) {
			SiteBuff.CreationUses -= incrOnSuccess;
			errCode = PutSite(trans, vlEntry->serverSite[Ix],
					  &SiteBuff, 1);
			if (errCode != 0) return errCode;
		    }
		    *wherep = Ix;
		    return 0;
		}
	    }
	}
    }
    return (error_status_t)VL_NOREPSERVER;
}
#endif /* 0 */

/*
 * Convert from the internal (compacted) vldb entry to the external
 * representation used by the interface.
 */
static int
vlentry_to_vldbentry(trans, VlEntry, VldbEntry)
struct ubik_trans	*trans;
struct vlentry		*VlEntry;
vldbentry		*VldbEntry;
{
    int i, j, SiteIx, AddrIx, DestIx;
    siteDesc DescBuff;
    int xl[MAXTYPES];
    int histSrvs[ADDRSINSITE],
	srvsIxOrMore[ADDRSINSITE],
	addrsLessIx[ADDRSINSITE];
    int addrAllotment, addrExtra;

    icl_Trace3(fl_iclSetp, FL_TRACE_VETOVD,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_POINTER, VlEntry,
	       ICL_TYPE_POINTER, VldbEntry);
    /*
     * This gets a bit more complicated when we have up to MAXNSERVERS servers
     * for an entry, each with up to ADDRSINSITE addresses, and a vldbentry has
     * room for only MAXNSERVERS addresses.
     * What we do is fill up the MAXNSERVERS address slots with at least one
     * address for each server.
     */
    bzero((char *)VldbEntry, sizeof(vldbentry));
    strncpy((char *)VldbEntry->name, VlEntry->name, sizeof(VldbEntry->name));
    VldbEntry->reclaimDally = VlEntry->reclaimDally;
    VldbEntry->maxTotalLatency = VlEntry->maxTotalLatency;
    VldbEntry->hardMaxTotalLatency = VlEntry->hardMaxTotalLatency;
    VldbEntry->minimumPounceDally = VlEntry->minimumPounceDally;
    VldbEntry->defaultMaxReplicaLatency = VlEntry->defaultMaxReplicaLatency;
    VldbEntry->WhenLocked = VlEntry->LockTimestamp;
    strncpy((char *)VldbEntry->LockerName,
	    VlEntry->NameOfLocker,
	    sizeof(VldbEntry->LockerName));
    VldbEntry->charSpares[VLAUTHN_MINLCLLEVEL] = VlEntry->vlent_min_lcl_authn;
    VldbEntry->charSpares[VLAUTHN_MAXLCLLEVEL] = VlEntry->vlent_max_lcl_authn;
    VldbEntry->charSpares[VLAUTHN_MINRMTLEVEL] = VlEntry->vlent_min_rmt_authn;
    VldbEntry->charSpares[VLAUTHN_MAXRMTLEVEL] = VlEntry->vlent_max_rmt_authn;
    for (i = 0; i < (sizeof(xl)/sizeof(xl[0])); ++i) xl[i] = -1;
    for (i = RWVOL, j = 0; i < MAXTYPES; ++i) {
	afs_hyper_t temp;
	DFSH_MemFromNetHyper(temp, VlEntry->volumeId[i]);
	if (!AFS_hiszero(temp)) {
	    VldbEntry->VolIDs[j] = temp;
	    VldbEntry->VolTypes[j] = VOLTIX_TO_VOLTYPE(i);
	    xl[i] = j;
	    ++j;
	}
    }
    for (; j < MAXVOLTYPES; ++j) AFS_hzero(VldbEntry->VolIDs[j]);
    VldbEntry->volumeType = VOLTIX_TO_VOLTYPE(VlEntry->volumeTIx);
    DFSH_MemFromNetHyper(VldbEntry->cloneId, VlEntry->cloneId);
    VldbEntry->flags = VlEntry->flags;
    bzero((char *)&histSrvs[0], sizeof(histSrvs));
    bzero((char *)&srvsIxOrMore[0], sizeof(srvsIxOrMore));
    bzero((char *)&addrsLessIx[0], sizeof(addrsLessIx));
    for (SiteIx = 0;
	 SiteIx < MAXNSERVERS
	   && (VlEntry->serverFlags[SiteIx] & VLSF_DEFINED) != 0;
	 ++SiteIx) {
	i = GetSite(trans, VlEntry->serverSite[SiteIx], &DescBuff);
	if (i == 0 && DescBuff.DeletedFlag == SITEDELETEDSP1)
	    i = VL_DELETEDSITE;
	if (i != 0) return i;
	for (AddrIx = 0; AddrIx < ADDRSINSITE; ++AddrIx) {
	    if (DescBuff.Addr[AddrIx].type == (unsigned short) -1) break;
	}
	if (AddrIx > 0) ++histSrvs[AddrIx-1];
    }
    /*
     * (the following description is complicated by C's zero-origin arrays)
     * Put in srvsIxOrMore[x-1] the number of servers with
     * that many or more addresses
     */
    srvsIxOrMore[ADDRSINSITE-1] = histSrvs[ADDRSINSITE-1];
    for (AddrIx = (ADDRSINSITE - 2); AddrIx >= 0; --AddrIx) {
	srvsIxOrMore[AddrIx] = srvsIxOrMore[AddrIx+1] + histSrvs[AddrIx];
    }
    /*
     * Put in addrsLessIx[x-1] the total number of addresses used by all
     * servers with fewer than x net addresses.
     */
    addrsLessIx[0] = 0;
    for (AddrIx = 1; AddrIx < ADDRSINSITE; ++AddrIx) {
	addrsLessIx[AddrIx] =
	    addrsLessIx[AddrIx-1] + (histSrvs[AddrIx-1] * AddrIx);
    }
    /*
     * Convert srvsIxOrMore[x-1] to be the number of addresses that would be
     * needed to describe the hosts at this and subsequent levels, if they were
     * each given a quota of x addresses.
     * Then add in the number of addresses necessary to describe all the
     * servers with fewer than x addresses.
     */
    for (AddrIx = 0; AddrIx < ADDRSINSITE; ++AddrIx) {
	srvsIxOrMore[AddrIx] *= (AddrIx+1);
	srvsIxOrMore[AddrIx] += addrsLessIx[AddrIx];
    }
    /*
     * Now, find a minimum quota for every server,
     * and the remainder to be sprinkled.
     */
    for (AddrIx = 0; ; ++AddrIx) {
	if (AddrIx >= ADDRSINSITE) {
	    addrAllotment = ADDRSINSITE;
	    addrExtra = 0;
	    break;
	}
	if (srvsIxOrMore[AddrIx] == 0) {
	    addrAllotment = AddrIx;
	    addrExtra = 0;
	    break;
	}
	if (srvsIxOrMore[AddrIx] > MAXNSERVERS) {
	    addrAllotment = AddrIx;
	    addrExtra = MAXNSERVERS - srvsIxOrMore[AddrIx-1];
	    break;
	}
    }
    /*
     * addrAllotment represents the quota of addresses given to each server.
     * addrExtra represents the number of extra addresses for which there is
     * room in the pkt.
     * We grant these addrExtra addresses as an increment on the addrAllotment
     * for the first few servers listed in the vlentry.
     */
    if (addrExtra) ++addrAllotment;
    DestIx = 0;
    for (SiteIx = 0;
	 SiteIx < MAXNSERVERS
	   && (VlEntry->serverFlags[SiteIx] & VLSF_DEFINED) != 0;
	 ++SiteIx) {
	i = GetSite(trans, VlEntry->serverSite[SiteIx], &DescBuff);
	if (i == 0 && DescBuff.DeletedFlag == SITEDELETEDSP1)
	    i = VL_DELETEDSITE;
	if (i != 0) return i;
	/* figure whether VLSF_PARTIALADDRS should be set in this site */
	j = 0;
	if ((addrAllotment < ADDRSINSITE)
	    && (DescBuff.Addr[addrAllotment].type != ((unsigned short) -1))) {
	    j = VLSF_PARTIALADDRS;
	}
	for (AddrIx = 0; AddrIx < addrAllotment; ++AddrIx) {
	    if (DescBuff.Addr[AddrIx].type == (unsigned short) -1) break;
	    VldbEntry->siteAddr[DestIx] = DescBuff.Addr[AddrIx];
	    VldbEntry->sitePartition[DestIx] = VlEntry->serverPartition[SiteIx];
	    VldbEntry->sitemaxReplicaLatency[DestIx] =
		VlEntry->sitemaxReplicaLatency[SiteIx];
	    VldbEntry->siteFlags[DestIx] =
		VlEntry->serverFlags[SiteIx] | VLSF_DEFINED;
	    VldbEntry->siteFlags[DestIx] &=
		~(LocVLSF_ROVOL | LocVLSF_RWVOL | LocVLSF_BACKVOL);
	    VldbEntry->siteFlags[DestIx] |= j;
	    if (AddrIx == 0) {
		VldbEntry->siteFlags[DestIx] &= ~VLSF_SAMEASPREV;
	    } else {
		VldbEntry->siteFlags[DestIx] |= VLSF_SAMEASPREV;
	    }
	    if ((VlEntry->serverFlags[SiteIx] & LocVLSF_ROVOL) != 0
		&& xl[ROVOL] >= 0) {
		VldbEntry->siteFlags[DestIx] |=
		  ((unsigned)VLSF_ZEROIXHERE >> xl[ROVOL]);
	    }
	    if ((VlEntry->serverFlags[SiteIx] & LocVLSF_RWVOL) != 0
		&& xl[RWVOL] >= 0) {
		VldbEntry->siteFlags[DestIx] |=
		  ((unsigned)VLSF_ZEROIXHERE >> xl[RWVOL]);
	    }
	    if ((VlEntry->serverFlags[SiteIx] & LocVLSF_BACKVOL) != 0
		&& xl[BACKVOL] >= 0) {
		VldbEntry->siteFlags[DestIx] |=
		  ((unsigned)VLSF_ZEROIXHERE >> xl[BACKVOL]);
	    }
	    bcopy((char *)DescBuff.KerbPrin,
		  (char *)VldbEntry->sitePrincipal[DestIx].text,
		  MAXKPRINCIPALLEN);
	    VldbEntry->siteOwner[DestIx] = DescBuff.Owner;
	    VldbEntry->siteObjID[DestIx] = DescBuff.ObjID;
	    /* insurance against bad counts */
	    ++DestIx;
	    if (DestIx >= MAXNSERVERS) goto break_two_loops;
	}
	if (addrExtra > 0 && AddrIx >= addrAllotment) {
	    --addrExtra;
	    /* see if it's time to back off the per-server address allotment */
	    if (addrExtra == 0) --addrAllotment;
	}
    }
    break_two_loops:;
    VldbEntry->nServers = DestIx;
    for (; DestIx < MAXNSERVERS; ++DestIx) VldbEntry->siteFlags[DestIx] = 0;
    icl_Trace0(fl_iclSetp, FL_TRACE_VETOVD_EXIT);
    return 0;
}

/*
 * Convert from the internal (compacted) vldb entry to the external
 * representation used by the interface (in external-compact form).
 */
static int
vlentry_to_comvldbentry(trans, vlentp, comvldbp)
struct ubik_trans	*trans;
struct vlentry		*vlentp;
compactvldbentry	*comvldbp;
{
    int i, j, SiteIx, AddrIx, DestIx;
    int xl[MAXTYPES];

    icl_Trace3(fl_iclSetp, FL_TRACE_VETOCVD,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_POINTER, vlentp,
	       ICL_TYPE_POINTER, comvldbp);
    dlprintf(dce_flserver_debug,1,("fls: vl_to_comvldb: entered\n"));
    bzero((char *)comvldbp, sizeof(compactvldbentry));
    strncpy((char *)comvldbp->name, vlentp->name, sizeof(comvldbp->name));
    comvldbp->reclaimDally = vlentp->reclaimDally;
    comvldbp->maxTotalLatency = vlentp->maxTotalLatency;
    comvldbp->hardMaxTotalLatency = vlentp->hardMaxTotalLatency;
    comvldbp->minimumPounceDally = vlentp->minimumPounceDally;
    comvldbp->defaultMaxReplicaLatency = vlentp->defaultMaxReplicaLatency;
    comvldbp->WhenLocked = vlentp->LockTimestamp;
    strncpy((char *)comvldbp->LockerName,
	    vlentp->NameOfLocker,
	    sizeof(comvldbp->LockerName));
    comvldbp->charSpares[VLAUTHN_MINLCLLEVEL] = vlentp->vlent_min_lcl_authn;
    comvldbp->charSpares[VLAUTHN_MAXLCLLEVEL] = vlentp->vlent_max_lcl_authn;
    comvldbp->charSpares[VLAUTHN_MINRMTLEVEL] = vlentp->vlent_min_rmt_authn;
    comvldbp->charSpares[VLAUTHN_MAXRMTLEVEL] = vlentp->vlent_max_rmt_authn;
    for (i = 0; i < (sizeof(xl)/sizeof(xl[0])); ++i) xl[i] = -1;
    for (i = RWVOL, j = 0; i < MAXTYPES; ++i) {
	afs_hyper_t temp;
	DFSH_MemFromNetHyper(temp, vlentp->volumeId[i]);
	if (!AFS_hiszero(temp)) {
	    comvldbp->VolIDs[j] = temp;
	    comvldbp->VolTypes[j] = VOLTIX_TO_VOLTYPE(i);
	    xl[i] = j;
	    ++j;
	}
    }
    dlprintf(dce_flserver_debug, 1, ("fls: vl_to_comvldb: xl is %d %d %d\n",
				     xl[0], xl[1], xl[2]));
    /* for (; j < MAXVOLTYPES; ++j) AFS_hzero(comvldbp->VolIDs[j]); */
    comvldbp->volumeType = VOLTIX_TO_VOLTYPE(vlentp->volumeTIx);
    DFSH_MemFromNetHyper(comvldbp->cloneId, vlentp->cloneId);
    comvldbp->flags = vlentp->flags;
    DestIx = 0;
    for (SiteIx = 0;
	 SiteIx < MAXNSERVERS
	   && (vlentp->serverFlags[SiteIx] & VLSF_DEFINED) != 0;
	 ++SiteIx) {
	dlprintf(dce_flserver_debug, 1, ("fls: vl_to_comvldb: storing ix %d\n",
					 SiteIx));
	comvldbp->siteCookies[SiteIx] = vlentp->serverSite[SiteIx];
	comvldbp->sitePartition[SiteIx] = vlentp->serverPartition[SiteIx];
	comvldbp->sitemaxReplicaLatency[SiteIx] =
	    vlentp->sitemaxReplicaLatency[SiteIx];
	comvldbp->siteFlags[SiteIx] =
	    vlentp->serverFlags[SiteIx] | VLSF_DEFINED;
	dlprintf(dce_flserver_debug, 1,
		 ("fls: vl_to_comvldb: flags[%d] starts %x\n",
		  SiteIx, comvldbp->siteFlags[SiteIx]));
	comvldbp->siteFlags[SiteIx] &=
	  ~(LocVLSF_ROVOL | LocVLSF_RWVOL | LocVLSF_BACKVOL | VLSF_SAMEASPREV);
	dlprintf(dce_flserver_debug, 1,
		 ("fls: vl_to_comvldb: flags[%d] next %x\n",
		  SiteIx, comvldbp->siteFlags[SiteIx]));
	if ((vlentp->serverFlags[SiteIx] & LocVLSF_ROVOL) != 0
	    && xl[ROVOL] >= 0) {
	    comvldbp->siteFlags[SiteIx] |=
	      ((unsigned)VLSF_ZEROIXHERE >> xl[ROVOL]);
	}
	if ((vlentp->serverFlags[SiteIx] & LocVLSF_RWVOL) != 0
	    && xl[RWVOL] >= 0) {
	    comvldbp->siteFlags[SiteIx] |=
	      ((unsigned)VLSF_ZEROIXHERE >> xl[RWVOL]);
	}
	if ((vlentp->serverFlags[SiteIx] & LocVLSF_BACKVOL) != 0
	    && xl[BACKVOL] >= 0) {
	    comvldbp->siteFlags[SiteIx] |=
	      ((unsigned)VLSF_ZEROIXHERE >> xl[BACKVOL]);
	}
	dlprintf(dce_flserver_debug, 1,
		 ("fls: vl_to_comvldb: flags[%d] ends %x\n",
		  SiteIx, comvldbp->siteFlags[SiteIx]));
	++DestIx;
    }
    comvldbp->nServers = DestIx;
    dlprintf(dce_flserver_debug,1,("fls: vl_to_comvldb: clearing %d to %d\n",
				    DestIx, MAXNSERVERS));
    for (; DestIx < MAXNSERVERS; ++DestIx)
	comvldbp->siteFlags[DestIx] = 0;
    icl_Trace0(fl_iclSetp, FL_TRACE_VETOCVD_EXIT);
    return 0;
}

/* Verify that the volname is a valid volume name. */
static int
InvalidVolname(volname)
volumeName volname;
{
    static char LEGALCHARS[] =
	".ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";
    static char DIGITS[] = "0123456789.,-";
    int slen;
    char *name = (char *)volname;

    slen = strlen(name);
    if (slen >= MAXFTNAMELEN) return 1;
    if (slen != strspn(name, LEGALCHARS)) return 1;
    if (slen == strspn(name, DIGITS)) return 1;
    return 0;
}


/*
 * Convert a long VolType to a volume type index, or to -1 if it's unrecognized.
 */
static int VolTypeToVolTix(voltype)
unsigned long voltype;
{
    if ((voltype & 0xffff) != 0) return -1;
    voltype = ((voltype >> 16) & 0xffff);
    if (voltype < MAXTYPES) return voltype;
    return -1;
}

static int
InvalidOperation(voloper)
long voloper;
{
    if (voloper != VLOP_MOVE
	  && voloper != VLOP_RELEASE
	  && voloper != VLOP_BACKUP
	  && voloper != VLOP_DELETE
	  && voloper != VLOP_DUMP
	  && voloper != VLOP_RESTORE
	  && voloper != VLOP_ADDSITE) {
	icl_Trace1(fl_iclSetp, FL_TRACE_INVALOP, ICL_TYPE_LONG, voloper);
	return 1;
    }
    return 0;
}

static int
InvalidReleasetype(releasetype)
long	releasetype;
{
    if ((releasetype & LOCKREL_TIMESTAMP)
	  || (releasetype & LOCKREL_OPCODE)
	  || (releasetype & LOCKREL_AFSID))
	return 0;
    icl_Trace1(fl_iclSetp, FL_TRACE_INVALREL, ICL_TYPE_LONG, releasetype);
    return 1;
}

/*
 * Check whether the given connection has global FL administrator permissions.
 */
static int NotFlAdmin(connp)
rpc_binding_handle_t connp;
{
    long errorcode;

    icl_Trace1(fl_iclSetp, FL_TRACE_FLCHK, ICL_TYPE_POINTER, connp);
    errorcode = dfsauth_server_CheckAuthorization(&connp,
						  (dacl_permset_t *)NULL,
						 (char *)NULL, (uuid_t *)NULL);
    icl_Trace1(fl_iclSetp, FL_TRACE_FLCHK, ICL_TYPE_LONG, errorcode);

    return errorcode;
}

/* Write in *okp whether the caller ``owns'' the server described by *descp. */

static long AuthForSiteDesc(connp, descp, okp)
rpc_binding_handle_t	connp;
siteDesc		*descp;
long			*okp;
{
    long  rtnVal = 0;

    icl_Trace2(fl_iclSetp, FL_TRACE_SITECHK,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_POINTER, descp);
    if ((rtnVal = dfsauth_server_CheckAuthorizationGroup(&connp,
				(uuid_t *)&(descp->Owner)))
	 == 0) {
	*okp = 1;
    } else
	*okp = 0;
    icl_Trace2(fl_iclSetp, FL_TRACE_SITECHK_EXIT,
	       ICL_TYPE_LONG, rtnVal,
	       ICL_TYPE_LONG, *okp);
    return rtnVal;
}


/*
 * Put TRUE (!= 0) in *okp iff the caller owns the server that has
 * address *addrp.
 * Put FALSE (0) in *okp if the caller doesn't own it.
 */
static long AuthForAddress(connp, trans, addrp, okp)
rpc_binding_handle_t	connp;
struct ubik_trans	*trans;
afsNetAddr		*addrp;
long			*okp;
{
    siteDesc desc;
    u_long ptr;
    long code;

    icl_Trace3(fl_iclSetp, FL_TRACE_ADDRCHK,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_POINTER, addrp);
    code = EnsureAddrEntry(trans, addrp, &ptr, 0, 0, 0);
    if (code) {
	icl_Trace2(fl_iclSetp, FL_TRACE_ADDRCHK_EXIT,
		   ICL_TYPE_LONG, code, ICL_TYPE_LONG, 0);
	return code;
    }
    code = GetSite(trans, ptr, &desc);
    if (code) {
	icl_Trace2(fl_iclSetp, FL_TRACE_ADDRCHK_EXIT,
		   ICL_TYPE_LONG, code, ICL_TYPE_LONG, 0);
	return code;
    }
    code = AuthForSiteDesc(connp, &desc, okp);
    icl_Trace2(fl_iclSetp, FL_TRACE_ADDRCHK_EXIT,
	       ICL_TYPE_LONG, code, ICL_TYPE_LONG, *okp);
    return code;
}



/*
 * Put TRUE (!= 0) in *okp if the caller owns all servers pointed to by
 * the VlEntry.
 * Put FALSE (0) in *okp if the caller doesn't own them all.
 */
static long AuthForVlEntry(connp, trans, vlentryp, okp)
rpc_binding_handle_t	connp;
struct ubik_trans	*trans;
struct vlentry		*vlentryp;
long			*okp;
{
    register int ix;
    long code;
    siteDesc desc;
    long OK;

    icl_Trace3(fl_iclSetp, FL_TRACE_VLCHK,
	       ICL_TYPE_POINTER, connp,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_POINTER, vlentryp);
    for (ix = 0;
	 ix < MAXNSERVERS && (vlentryp->serverFlags[ix] & VLSF_DEFINED) != 0;
	 ++ix) {
	code = GetSite(trans, vlentryp->serverSite[ix], &desc);
	if (code != 0) {
	    icl_Trace2(fl_iclSetp, FL_TRACE_VLCHK_EXIT,
		       ICL_TYPE_LONG, code, ICL_TYPE_LONG, 0);
	    return code;
	}
	code = AuthForSiteDesc(connp, &desc, &OK);
	if (code != 0) {
	    icl_Trace2(fl_iclSetp, FL_TRACE_VLCHK_EXIT,
		       ICL_TYPE_LONG, code, ICL_TYPE_LONG, 0);
	    return code;
	}
	if (OK == 0) {	/* permission failure */
	    *okp = 0;
	    return 0;
	}
    }
    *okp = 1;
    icl_Trace2(fl_iclSetp, FL_TRACE_VLCHK_EXIT,
	       ICL_TYPE_LONG, code, ICL_TYPE_LONG, 1);
    return 0;
}

/* Check for random bad data in the ``afsNetAddr'' *Addr.  0 for OK. */
static int CheckafsNetAddr(Addr)
afsNetAddr *Addr;
{
    unsigned int Family;

    Family = (unsigned short) Addr->type;
    if (Family == 0 || Family > AF_MAX) return -1;
    return 0;
}

/* Convert *descp from network to host order. */
static void ntohSiteDesc(descp)
siteDesc *descp;
{
    register int Ix;

    ntohuuid(&descp->Owner);
    ntohuuid(&descp->ObjID);
    descp->CreationQuota = ntohl(descp->CreationQuota);
    descp->CreationUses = ntohl(descp->CreationUses);
    descp->DeletedFlag = ntohl(descp->DeletedFlag);
    descp->spare2 = ntohl(descp->spare2);
    descp->spare3 = ntohl(descp->spare3);
    descp->spare4 = ntohl(descp->spare4);
    descp->spare5 = ntohl(descp->spare5);
    for (Ix = 0; Ix < ADDRSINSITE; ++Ix)
	descp->Addr[Ix].type = ntohs(descp->Addr[Ix].type);
}

/* Convert *descp from host to network order. */
static void htonSiteDesc(descp)
siteDesc *descp;
{
    register int Ix;

    htonuuid(&descp->Owner);
    htonuuid(&descp->ObjID);
    descp->CreationQuota = htonl(descp->CreationQuota);
    descp->CreationUses = htonl(descp->CreationUses);
    descp->DeletedFlag = htonl(descp->DeletedFlag);
    descp->spare2 = htonl(descp->spare2);
    descp->spare3 = htonl(descp->spare3);
    descp->spare4 = htonl(descp->spare4);
    descp->spare5 = htonl(descp->spare5);
    for (Ix = 0; Ix < ADDRSINSITE; ++Ix)
	descp->Addr[Ix].type = htons(descp->Addr[Ix].type);
}

static struct siteBlock NextBlock;
static long NextBlockAddr = 0;

/* Return a copy of the siteDesc information pointed to by SitePtr. */
static long GetSite(trans, SitePtr, descBuffP)
struct ubik_trans *trans;
u_long SitePtr;
siteDesc *descBuffP;
{
    long errorcode;
    register int Ix;

    icl_Trace3(fl_iclSetp, FL_TRACE_GSITE,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, SitePtr,
	       ICL_TYPE_POINTER, descBuffP);
    lock_ObtainRead(&vl_fsbLock);
    if (SitePtr < (sizeof(cheader) + sizeof(FirstSiteBlock))) {
	bcopy(((char *)&FirstSiteBlock) + SitePtr - sizeof(cheader),
	      (char *) descBuffP,
	      sizeof(siteDesc));
	lock_ReleaseRead(&vl_fsbLock);
    } else if (NextBlockAddr != 0
	       && SitePtr > NextBlockAddr
	       && SitePtr < (NextBlockAddr + sizeof(struct siteBlock))) {
	bcopy(((char *) &NextBlock) + SitePtr - NextBlockAddr,
	      (char *) descBuffP,
	      sizeof(siteDesc));
	lock_ReleaseRead(&vl_fsbLock);
    } else {
	lock_ReleaseRead(&vl_fsbLock);
	errorcode = vlread(trans, SitePtr, (char *)descBuffP, sizeof(siteDesc));
	if (errorcode != 0) {
	    icl_Trace1(fl_iclSetp, FL_TRACE_GSITE_EXIT,
		       ICL_TYPE_LONG, errorcode);
	    return (error_status_t)errorcode;
	}
    }
    /* convert entry from network to host order */
    ntohSiteDesc(descBuffP);
    icl_Trace1(fl_iclSetp, FL_TRACE_GSITE_EXIT, ICL_TYPE_LONG, 0);
    return 0;
}

/*
 * Return a copy of the siteDesc information pointed to by SitePtr.
 * Check the SitePtr pretty carefully.
 */
static long CarefullyGetSite(trans, SitePtr, descBuffP)
struct ubik_trans *trans;
u_long SitePtr;
siteDesc *descBuffP;
{
    long errorcode;
    int Ix;

    icl_Trace3(fl_iclSetp, FL_TRACE_CGSITE,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, SitePtr,
	       ICL_TYPE_POINTER, descBuffP);
    lock_ObtainRead(&vl_fsbLock);
    if (SitePtr < (sizeof(cheader) + sizeof(FirstSiteBlock))) {
	/* does this point before the first siteDesc? */
	if (SitePtr < (sizeof(cheader) + offsetof(struct siteBlock, Sites))) {
	    lock_ReleaseRead(&vl_fsbLock);
	    icl_Trace1(fl_iclSetp, FL_TRACE_CGSITE_EXIT,
		       ICL_TYPE_LONG, VL_BADCOOKIE);
	    return (error_status_t)VL_BADCOOKIE;
	}
	/* does SitePtr point at a real siteDesc? */
	if ((SitePtr - (sizeof(cheader) + offsetof(struct siteBlock, Sites)))
		% sizeof(siteDesc) != 0) {
	    lock_ReleaseRead(&vl_fsbLock);
	    icl_Trace1(fl_iclSetp, FL_TRACE_CGSITE_EXIT,
		       ICL_TYPE_LONG, VL_BADCOOKIE);
	    return (error_status_t)VL_BADCOOKIE;
	}
	bcopy(((char *)&FirstSiteBlock) + SitePtr - sizeof(cheader),
	      (char *) descBuffP,
	      sizeof(siteDesc));
	lock_ReleaseRead(&vl_fsbLock);
    } else if (NextBlockAddr != 0
		&& SitePtr > (u_long)(NextBlockAddr +
				      offsetof(struct siteBlock, Sites))
		&& SitePtr < (NextBlockAddr + sizeof(struct siteBlock))) {
	if ((SitePtr - (NextBlockAddr + offsetof(struct siteBlock, Sites)))
		% sizeof(siteDesc) != 0) {
	    lock_ReleaseRead(&vl_fsbLock);
	    icl_Trace1(fl_iclSetp, FL_TRACE_CGSITE_EXIT,
		       ICL_TYPE_LONG, VL_BADCOOKIE);
	    return (error_status_t)VL_BADCOOKIE;
	}
	bcopy(((char *) &NextBlock) + (SitePtr - NextBlockAddr),
	      (char *) descBuffP,
	      sizeof(siteDesc));
	lock_ReleaseRead(&vl_fsbLock);
    } else {
	/* Perhaps we could validate the SitePtr a bit more here, but... */
	lock_ReleaseRead(&vl_fsbLock);
	errorcode = vlread(trans, SitePtr, (char *)descBuffP, sizeof(siteDesc));
	if (errorcode != 0) {
	    icl_Trace1(fl_iclSetp, FL_TRACE_CGSITE_EXIT,
		       ICL_TYPE_LONG, errorcode);
	    return (error_status_t)errorcode;
	}
    }
    /* convert entry from network to host order */
    ntohSiteDesc(descBuffP);
    icl_Trace1(fl_iclSetp, FL_TRACE_CGSITE_EXIT, ICL_TYPE_LONG, 0);
    return 0;
}

/*
 * Put the modified, host-byte-order descBuffP entry back at address SitePtr.
 * Trash descBuffP buffer.
 */
static long PutSite(trans, SitePtr, descBuffP, needLock)
struct ubik_trans	*trans;
u_long			SitePtr;
siteDesc		*descBuffP;
int			needLock;
{
    u_long BlockAddr;
    struct siteBlock *Blk;
    int ErrCode;
    int Ix;

    icl_Trace4(fl_iclSetp, FL_TRACE_PUTS,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, SitePtr,
	       ICL_TYPE_POINTER, descBuffP,
	       ICL_TYPE_LONG, needLock);
    dlprintf(dce_flserver_debug,1,("fls: Putsite: entered\n"));
    if (needLock) lock_ObtainWrite(&vl_fsbLock);
    if (SitePtr < (sizeof(cheader) + sizeof(struct siteBlock))) {
	Blk = &FirstSiteBlock;
	BlockAddr = sizeof(cheader);
	dlprintf(dce_flserver_debug,1,("fls: Putsite: in first block\n"));
    } else if (NextBlockAddr != 0
		 && SitePtr > NextBlockAddr
		 && SitePtr < (NextBlockAddr + sizeof(struct siteBlock))) {
	Blk = &NextBlock;
	BlockAddr = NextBlockAddr;
	dlprintf(dce_flserver_debug,1,("fls: Putsite: in next block\n"));
    } else {
	/*
	 * Yes, you could read the right block back,
	 * but who would be breaking our write lock and changing NextBlock?
	 */
	if (needLock) lock_ReleaseWrite(&vl_fsbLock);
	dlprintf(dce_flserver_debug, 1,
		 ("PutSite: Cannot put site back: site ptr %#x or block \
addr %#x moved.\n",
		  SitePtr, NextBlockAddr));
	icl_Trace1(fl_iclSetp, FL_TRACE_PUTS_EXIT,
		   ICL_TYPE_LONG, VL_INCONSISTENT);
	return (error_status_t)VL_INCONSISTENT;
    }
    /* convert entry from host to network order, in place. */
    htonSiteDesc(descBuffP);
    bcopy((char *)descBuffP,
	  ((char *) Blk) + (SitePtr - BlockAddr),
	  sizeof(siteDesc));
    Blk->NextPtr = htonl(Blk->NextPtr);
    Blk->AllocHere = htonl(Blk->AllocHere);
    Blk->UsedHere = htonl(Blk->UsedHere);
    ErrCode = vlwrite(trans, BlockAddr, (char *) Blk, sizeof(struct siteBlock));
    Blk->NextPtr = ntohl(Blk->NextPtr);
    Blk->AllocHere = ntohl(Blk->AllocHere);
    Blk->UsedHere = ntohl(Blk->UsedHere);
    if (ErrCode != 0 && Blk == &NextBlock) NextBlockAddr = 0;
    if (needLock) lock_ReleaseWrite(&vl_fsbLock);
    dlprintf(dce_flserver_debug, 1,
	     ("fls: Putsite: returning %lu (from vlwrite)\n", ErrCode));
    icl_Trace1(fl_iclSetp, FL_TRACE_PUTS_EXIT, ICL_TYPE_LONG, ErrCode);
    return ErrCode;
}

/*
 * Check the quota on the given site, after adding quotaIncr to it.
 * Descp is in NETWORK order.
 */
static int quotaCheck(trans, descaddr, descp, quotaIncr)
struct ubik_trans	*trans;
u_long			descaddr;
struct siteDesc		*descp;
int			quotaIncr;
{
    long code;
    struct siteDesc DescBuff;

    icl_Trace4(fl_iclSetp, FL_TRACE_QCHK,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_LONG, descaddr,
	       ICL_TYPE_POINTER, descp,
	       ICL_TYPE_LONG, quotaIncr);
    if (quotaIncr == 0) {
	icl_Trace1(fl_iclSetp, FL_TRACE_QCHK_EXIT, ICL_TYPE_LONG, 0);
	return 0;
    }
    DescBuff = *descp;
    ntohSiteDesc(&DescBuff);
    if (quotaIncr < 0) { /* allocating => decreasing quota */
	quotaIncr = (-quotaIncr);
	dlprintf(dce_flserver_debug,1,("fls: quotaCheck: consuming %d of %d\n",
				       quotaIncr,
				       DescBuff.CreationQuota
				       - DescBuff.CreationUses));
	if (
	    (DescBuff.CreationQuota != 0)
	    && ((DescBuff.CreationUses + quotaIncr) > DescBuff.CreationQuota)
	    ) {
	    icl_Trace1(fl_iclSetp, FL_TRACE_QCHK_EXIT,
		       ICL_TYPE_LONG, VL_OUTOFQUOTA);
	    return (error_status_t)VL_OUTOFQUOTA;
	}
	DescBuff.CreationUses += quotaIncr;
    } else {
	dlprintf(dce_flserver_debug, 1, ("fls: quotaCheck: releasing %d\n",
					 quotaIncr));
	DescBuff.CreationUses -= quotaIncr;
    }
    code = PutSite(trans, descaddr, &DescBuff, 0);
    icl_Trace1(fl_iclSetp, FL_TRACE_QCHK_EXIT, ICL_TYPE_LONG, code);
    return code;
}

/*
 * Return a host-byte-order pointer (or -1) to a siteDesc structure in the
 * VLDB file that matches the given Addr.  If none matches, allocate one.
 */
static int EnsureAddrEntry(trans, Addr, OutPtr, DoAlloc, WhichAddr, quotaIncr)
struct ubik_trans	*trans;
afsNetAddr		*Addr;
u_long			*OutPtr;
int	DoAlloc;	/* != 0 if we're permitted to create a new one */
int	*WhichAddr;	/* if != 0, use to say where the given Addr
			 * was found in the siteDesc. */
int			quotaIncr;
{
    register int Ix, XIx;
    register afsNetAddr *SA;
    long BlockAddr, NewAddr, errCode;
    register struct siteBlock *Blk;
    siteDesc *firstEmpty = (siteDesc *)0;
    int firstEmptyIndex = 0;
    struct siteBlock *firstEmptyBlock;
    long firstBlockAddr;

    icl_Trace3(fl_iclSetp, FL_TRACE_ENSADDR,
	       ICL_TYPE_POINTER, trans,
	       ICL_TYPE_POINTER, Addr,
	       ICL_TYPE_POINTER, OutPtr);
    icl_Trace3(fl_iclSetp, FL_TRACE_ENSADDRA,
	       ICL_TYPE_LONG, DoAlloc,
	       ICL_TYPE_POINTER, WhichAddr,
	       ICL_TYPE_LONG, quotaIncr);
    BlockAddr = sizeof(cheader);

    dlprintf(dce_flserver_debug,1,("fls: EnsAdEnt: entered\n"));
    Blk = &FirstSiteBlock;
    lock_ObtainWrite(&vl_fsbLock);
    for (;;) {
	for (Ix = 0; Ix < Blk->UsedHere; ++Ix) {
	    if (Blk->Sites[Ix].DeletedFlag == htonl(SITEDELETEDSP1)) {
		if (!firstEmptyIndex) {
		    firstEmptyIndex = Ix;
		    firstEmptyBlock = Blk;
		    firstBlockAddr = BlockAddr;
		}
		continue;
	    }
	    SA = Blk->Sites[Ix].Addr;
	    for (XIx = 0; XIx < ADDRSINSITE; ++XIx,++SA) {
		if (ntohs(SA->type) == ((unsigned short) -1)) break;
		if (ntohs(SA->type) == Addr->type &&
		    (bcmp((char *)Addr->data,
			  (char *)SA->data,
			  sizeof(SA->data)) == 0)) {
		    if (WhichAddr != 0) *WhichAddr = XIx;
		    *OutPtr = (u_long) BlockAddr
				+ offsetof (struct siteBlock, Sites)
				+ Ix * sizeof(siteDesc);
		    errCode = quotaCheck(trans, *OutPtr, &(Blk->Sites[Ix]),
					 quotaIncr);
		    lock_ReleaseWrite(&vl_fsbLock);
		    dlprintf(dce_flserver_debug, 1,
			     ("fls: EnsAdEnt: (ok) returning %lu\n", errCode));
		    icl_Trace1(fl_iclSetp, FL_TRACE_ENSADDR_EXIT,
			       ICL_TYPE_LONG, errCode);
		    return errCode;
		}
	    }
	}
	if (Blk->NextPtr == 0) break;

	if (Blk->NextPtr >= ntohl(cheader.vital_header.eofPtr)) {
	    lock_ReleaseWrite(&vl_fsbLock);
	    dlprintf(dce_flserver_debug, 1,
		     ("fls: EnsAdEnt: (badindex) returning %lu\n",
		      VL_BADINDEX));
	    icl_Trace1(fl_iclSetp, FL_TRACE_ENSADDR_EXIT,
		       ICL_TYPE_LONG, VL_BADINDEX);
	    return (error_status_t)VL_BADINDEX;
	}
	BlockAddr = Blk->NextPtr;
	Blk = &NextBlock;
	if ((NextBlockAddr == 0) || (NextBlockAddr != BlockAddr)) {
	    NextBlockAddr = 0;
	    errCode = vlread(trans, BlockAddr, (char *) Blk, sizeof(NextBlock));
	    if (errCode != 0) {
		lock_ReleaseWrite(&vl_fsbLock);
		dlprintf(dce_flserver_debug, 1,
			 ("fls: EnsAdEnt: (bad vlread) returning %lu\n",
			  errCode));
		icl_Trace1(fl_iclSetp, FL_TRACE_ENSADDR_EXIT,
			   ICL_TYPE_LONG, errCode);
		return errCode;
	    }
	    NextBlock.NextPtr = ntohl(NextBlock.NextPtr);
	    NextBlock.AllocHere = ntohl(NextBlock.AllocHere);
	    NextBlock.UsedHere = ntohl(NextBlock.UsedHere);
	    NextBlockAddr = BlockAddr;
	}
	if (NextBlock.Tag != htonl(SITEBLOCKTAG)) {
	    NextBlockAddr = 0;
	    lock_ReleaseWrite(&vl_fsbLock);
	    dlprintf(dce_flserver_debug, 1,
		     ("fls: EnsAdEnt: (bad sig) returning %lu\n",
		      VL_SITESCORRUPTED));
	    icl_Trace1(fl_iclSetp, FL_TRACE_ENSADDR_EXIT,
		       ICL_TYPE_LONG, VL_SITESCORRUPTED);
	    return (error_status_t)VL_SITESCORRUPTED;
	}
    }
    if (DoAlloc == 0) {
	lock_ReleaseWrite(&vl_fsbLock);
	dlprintf(dce_flserver_debug, 1,
		 ("fls: EnsAdEnt: (no alloc) returning %lu\n", VL_NOSERVER));
	icl_Trace1(fl_iclSetp, FL_TRACE_ENSADDR_EXIT,
		   ICL_TYPE_LONG, VL_NOSERVER);
	return (error_status_t)VL_NOSERVER;
    }
    /* OK, so there isn't an entry.  Allocate one. */
    /* if we don't have an empty siteDesc slot to reuse, we allocate */
    if (!firstEmptyIndex) {
	if (Blk->UsedHere >= Blk->AllocHere) {
	    /* fooey.  Have to allocate a new block, too. */
	    if (flserver_verbose)
		dce_fprintf(stderr, lsv_s_flserver_alloc_new_block );
	    cmd_explanation =dce_msg_get_msg(lsv_s_log_flserver_alloc_new_block,
					     &st);
	    Log( (char *) cmd_explanation);
	    free(cmd_explanation);
	    /* remember this guy */
	    NewAddr = ntohl(cheader.vital_header.eofPtr);
	    cheader.vital_header.eofPtr =
	      htonl(NewAddr + sizeof(struct siteBlock));
	    cheader.vital_header.numSites =
	      htonl(ntohl(cheader.vital_header.numSites)+1);
	    errCode = write_vital_vlheader(trans);
	    if (errCode != 0) {
		lock_ReleaseWrite(&vl_fsbLock);
		icl_Trace1(fl_iclSetp, FL_TRACE_ENSADDR_EXIT,
			   ICL_TYPE_LONG, errCode);
		return errCode;
	    }
	    /* point to the newly-allocated block */
	    Blk->NextPtr = htonl(NewAddr);
	    Blk->AllocHere = htonl(Blk->AllocHere);
	    Blk->UsedHere = htonl(Blk->UsedHere);
	    errCode = vlwrite(trans, BlockAddr, (char *) Blk,
			      sizeof(struct siteBlock));
	    Blk->NextPtr = ntohl(Blk->NextPtr);
	    Blk->AllocHere = ntohl(Blk->AllocHere);
	    Blk->UsedHere = ntohl(Blk->UsedHere);
	    if (errCode != 0) {
		if (Blk == &NextBlock) NextBlockAddr = 0;
		lock_ReleaseWrite(&vl_fsbLock);
		icl_Trace1(fl_iclSetp, FL_TRACE_ENSADDR_EXIT,
			   ICL_TYPE_LONG, errCode);
		return errCode;
	    }
	    bzero((char *)&NextBlock, sizeof(struct siteBlock));
	    Blk = &NextBlock;
	    BlockAddr = NewAddr;
	    NextBlockAddr = NewAddr;
	    Blk->Tag = htonl(SITEBLOCKTAG);
	    Blk->AllocHere = SITESINBLOCK;
	} else {
	    cheader.vital_header.numSites =
	      htonl(ntohl(cheader.vital_header.numSites)+1);
	    errCode = write_vital_vlheader(trans);
	    if (errCode != 0) {
		lock_ReleaseWrite(&vl_fsbLock);
		icl_Trace1(fl_iclSetp, FL_TRACE_ENSADDR_EXIT,
			   ICL_TYPE_LONG, errCode);
		return errCode;
	    }
	}
    }
    if (firstEmptyIndex) {	/* reusing the first empty siteDesc slot */
	if (firstBlockAddr == sizeof(cheader)) { /* use FirstSiteBlock */
	    firstEmptyBlock = &FirstSiteBlock;
	    firstEmpty = &(firstEmptyBlock->Sites[firstEmptyIndex]);
	}
	else if (firstBlockAddr == NextBlockAddr) { /* use NextBlock */
	    firstEmptyBlock = &NextBlock;
	    firstEmpty = &(firstEmptyBlock->Sites[firstEmptyIndex]);
	}
	else {				/* got to read it from FLDB */
	    errCode = vlread(trans, firstBlockAddr, (char *)&NextBlock,
			     sizeof(NextBlock));
	    if (errCode != 0) {
		NextBlockAddr = 0;
		lock_ReleaseWrite(&vl_fsbLock);
		dlprintf(dce_flserver_debug, 1,
			 ("fls: EndAdEnt: (bad vlread) returning %lu\n",
			  errCode));
		icl_Trace1(fl_iclSetp, FL_TRACE_ENSADDR_EXIT,
			   ICL_TYPE_LONG, errCode);
		return errCode;
	    }
	    firstEmptyBlock = &NextBlock;
	    firstEmpty = &(firstEmptyBlock->Sites[firstEmptyIndex]);
	    firstEmptyBlock->NextPtr = ntohl(firstEmptyBlock->NextPtr);
	    firstEmptyBlock->AllocHere = ntohl(firstEmptyBlock->AllocHere);
	    firstEmptyBlock->UsedHere = ntohl(firstEmptyBlock->UsedHere);
	    NextBlockAddr = firstBlockAddr;
	}

	firstEmpty->Addr[0] = *Addr;
	firstEmpty->Addr[0].type = htons(firstEmpty->Addr[0].type);
	firstEmpty->DeletedFlag = htonl(0L);
	for (XIx = 1; XIx < ADDRSINSITE; ++XIx) {
	    firstEmpty->Addr[XIx].type = htons((unsigned short) -1);
	}
	firstEmpty->CreationQuota = htonl(1 + quotaIncr);
	firstEmpty->CreationUses = 0;
	firstEmptyBlock->NextPtr = htonl(firstEmptyBlock->NextPtr);
	firstEmptyBlock->AllocHere = htonl(firstEmptyBlock->AllocHere);
	firstEmptyBlock->UsedHere = htonl(firstEmptyBlock->UsedHere);
	XIx = vlwrite(trans, firstBlockAddr, (char *) firstEmptyBlock,
		      sizeof(struct siteBlock));
	firstEmptyBlock->NextPtr = ntohl(firstEmptyBlock->NextPtr);
	firstEmptyBlock->AllocHere = ntohl(firstEmptyBlock->AllocHere);
	firstEmptyBlock->UsedHere = ntohl(firstEmptyBlock->UsedHere);
	if (XIx != 0) {
	    lock_ReleaseWrite(&vl_fsbLock);
	    icl_Trace1(fl_iclSetp, FL_TRACE_ENSADDR_EXIT, ICL_TYPE_LONG, XIx);
	    return XIx;
	}
	*OutPtr = firstBlockAddr+((char *)firstEmpty-(char *)firstEmptyBlock);
    } else { /* using the new siteDesc we allocated */
	Ix = Blk->UsedHere;
	bzero((char *)&(Blk->Sites[Ix]), sizeof(struct siteDesc));
	Blk->Sites[Ix].Addr[0] = *Addr;
	Blk->Sites[Ix].Addr[0].type = htons(Blk->Sites[Ix].Addr[0].type);
	for (XIx = 1; XIx < ADDRSINSITE; ++XIx) {
	    Blk->Sites[Ix].Addr[XIx].type = htons((unsigned short) -1);
	}
	/* Just default the creation quota for now */
	Blk->Sites[Ix].CreationQuota = htonl(1 + quotaIncr);
	Blk->Sites[Ix].CreationUses = htonl(0);
	++(Blk->UsedHere);
	/* Now write the bitsies. */
	Blk->NextPtr = htonl(Blk->NextPtr);
	Blk->AllocHere = htonl(Blk->AllocHere);
	Blk->UsedHere = htonl(Blk->UsedHere);
	XIx = vlwrite(trans, BlockAddr, (char *) Blk, sizeof(struct siteBlock));
	Blk->NextPtr = ntohl(Blk->NextPtr);
	Blk->AllocHere = ntohl(Blk->AllocHere);
	Blk->UsedHere = ntohl(Blk->UsedHere);
	if (XIx != 0) {
	    if (Blk == &NextBlock) NextBlockAddr = 0;
	    lock_ReleaseWrite(&vl_fsbLock);
	    icl_Trace1(fl_iclSetp, FL_TRACE_ENSADDR_EXIT,
		       ICL_TYPE_LONG, XIx);
	    return XIx;
	}
	if (WhichAddr != 0) *WhichAddr = 0;
	*OutPtr = (BlockAddr + (((char *) &(Blk->Sites[Ix])) - ((char *) Blk)));
    }
    lock_ReleaseWrite(&vl_fsbLock);
    icl_Trace1(fl_iclSetp, FL_TRACE_ENSADDR_EXIT, ICL_TYPE_LONG, 0);
    return 0;
}


/* init local stuff */
vlserver_Init()
{
    lock_Init(&vl_fsbLock);
    return 0;
}
