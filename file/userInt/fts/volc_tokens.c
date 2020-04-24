/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: volc_tokens.c,v $
 * Revision 1.1.826.1  1996/10/02  21:15:30  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:48  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1991 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/lock.h>
#include <pthread.h>
#include <dcedfs/ftserver_data.h>
#include <dcedfs/ioctl.h>
#include <dcedfs/aggr.h>
#include <dcedfs/fldb_data.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/flserver.h>
#include <dcedfs/rep_data.h>
#include <dcedfs/ftserver.h>
#include <dcedfs/tkn4int.h>
#include <dcedfs/fshs_errs.h>
#include <dcedfs/hs_errs.h>
#include <dcedfs/tkm_errs.h>
#include <dcedfs/osi_dfserrors.h>
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */

#include <volc.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftimac.h>
#include <dfsftisvc.h>
#include <dfsftimsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/userInt/fts/volc_tokens.c,v 1.1.826.1 1996/10/02 21:15:30 damon Exp $")

static unsigned long someStartTime;
static struct afsNetData someRevocationNetData;

/* This module gets and maintains whole-fileset tokens for filesets being copied. */

/* shareable structure */
struct fsconn {
    afsUUID connUuid;			/* uuid for comm. with this server */
    rpc_binding_handle_t Exporter;	/* PX that's granting us tokens. */
    struct afsNetAddr pxAddr;
    unsigned long lastSuccessfulCall;
    unsigned long hostLifeGuarantee;
    int refCount;
    int goneBad;
    unsigned char princName[MAXKPRINCIPALLEN];
};

struct tknList {
    struct tknList *next;		/* chain 'em together */
    struct fsconn *fsp;			/* connection to fileserver */
    afs_token_t tok;			/* put storage in-line here. */
    afs_hyper_t volId;			/* fileset ID for desired token */
    afs_hyper_t tokTypes;		/* types of tokens desired */
    unsigned long flags;
    unsigned long gtFlags;		/* flags for AFS_GetToken */
    int refCount;
    int tokenDescriptor;
};
#define tklFlag_Revoked 0x1
#define tklFlag_GrantInProgress 0x2
#define tklFlag_Granted 0x4

static struct tknList *tkList = NULL;
static osi_dlock_t tkLock;
static pthread_once_t tokensInitedBlock = pthread_once_init;
static int inited = 0;
static int gotGlobalTokenReset = 0;
static int lastTokenDescriptor = 0;
static unsigned long minAdditionalTime = 0;
static unsigned long maxHostLifetime = 2*60;

static void FlushDeleted()
{/* Eliminate unreferenced garbage from the token list. */
 /* MUST be called with tkLock write-locked. */
    register struct tknList *tkl, *Xtkl, *Ptkl;
    error_status_t st;

    for (tkl = tkList, Ptkl = NULL; tkl != NULL; tkl = Xtkl) {
	Xtkl = tkl->next;
	if (tkl->refCount == 0 && ((tkl->flags & tklFlag_Revoked) != 0)) {
	    if (tkl->fsp) {
		--(tkl->fsp->refCount);
		if (tkl->fsp->refCount == 0 && tkl->fsp->Exporter != NULL) {
		    rpc_binding_free(&tkl->fsp->Exporter, &st);
		    tkl->fsp->Exporter = NULL;
		    free((opaque) tkl->fsp);
		}
	    }
	    free((opaque) tkl);
	    if (Ptkl == NULL)
		tkList = Xtkl;
	    else
		Ptkl->next = Xtkl;
	} else {
	    Ptkl = tkl;
	}
    }
}


/*
 * Probe client Token interface call: If we make it to here
 * we're successful enough.
 */
static error_status_t
STKN_Probe(handle_t h)
{
    VOLSERLOG(1, "STKN_Probe() called.\n");
    return error_status_ok;		/* this one's correct */
}


/*
 * Initialize all the token state associated with the server
 */
static error_status_t
STKN_InitTokenState(
  handle_t h,
  unsigned32 Flags,
  unsigned32 hostLifeGuarantee,
  unsigned32 hostRPCGuarantee,
  unsigned32 deadServerTimeout,
  unsigned32 serverRestartEpoch,
  unsigned32 serverSizesAttrs,
  unsigned32 spare2,
  unsigned32 spare3,
  unsigned32 *spare4,
  unsigned32 *spare5,
  unsigned32 *spare6)
{
    register struct tknList *tkl;
    register struct fsconn *fsp;
    int doAll;
    afsUUID connId;
    error_status_t st;

    VOLSERLOG(1, "\nSTKN_InitTokenState called with flags of %#lx\n", Flags);
    doAll = 1;
    rpc_binding_inq_object(h, &connId, &st);
    if (st == rpc_s_ok) {
	if (uuid_is_nil(&connId, (unsigned32 *)&st)) {
	    dce_svc_printf(FTS_S_TOKENS_NIL_SERVER_MSG);
	} else {
	    doAll = 0;
	}
    } else {
	dce_svc_printf(FTS_S_TOKENS_NO_BINDING_UUID_MSG, dfs_dceErrTxt(st));
    }

    /* Free all token state.  This call might race with an AFS_GetToken() call. */
    lock_ObtainWrite(&tkLock);
    if (minAdditionalTime == 0 || minAdditionalTime > hostLifeGuarantee) {
	minAdditionalTime = hostLifeGuarantee;
    }
    if (minAdditionalTime > hostRPCGuarantee)
	minAdditionalTime = hostRPCGuarantee;
    if (minAdditionalTime > deadServerTimeout)
	minAdditionalTime = deadServerTimeout;
    if (maxHostLifetime < hostLifeGuarantee)
	maxHostLifetime = hostLifeGuarantee;
    if (doAll) {
	gotGlobalTokenReset = 1;
	for (tkl = tkList; tkl != NULL; tkl = tkl->next) {
	    tkl->flags |= tklFlag_Revoked;
	}
    } else {
	for (tkl = tkList; tkl != NULL; tkl = tkl->next) {
	    if (tkl->fsp != NULL && uuid_equal(&connId, &tkl->fsp->connUuid,
					       (unsigned32 *)&st)) {
		/* here's one from the given host */
		tkl->fsp->hostLifeGuarantee = hostLifeGuarantee;
		/* ignore serverSizesAttrs */
		tkl->flags |= tklFlag_Revoked;
	    }
	}
    }
    FlushDeleted();
    lock_ReleaseWrite(&tkLock);
    return error_status_ok;
}


/*
 * Revoke token RPC call.
 */

static error_status_t
STKN_TokenRevoke(handle_t h, afsRevokes *descp)
{
    register struct tknList *tkl, *Xtkl;
    register afsRevokeDesc *tdescp;
    register long code;
    register int i;
    afsUUID connId;
    error_status_t st;
 
    VOLSERLOG(1, "STKN_TokenRevoke called with %d token(s)\n",
	       descp->afsRevokes_len);
    rpc_binding_inq_object(h, &connId, &st);
    if (st == rpc_s_ok) {
	if (uuid_is_nil(&connId, (unsigned32 *)&st)) {
	    dce_svc_printf(FTS_S_TOKENS_NIL_SERVER_UUID_MSG);
	    return EINVAL;
	}
    } else {
	dce_svc_printf(FTS_S_TOKENS_REVOKE_NO_UUID_MSG, dfs_dceErrTxt(st));
	return st;
    }
    code = 0;		/* start out with a complete revocation */
    tdescp = &descp->afsRevokes_val[0];
    for (i = 0; i < descp->afsRevokes_len; ++i, ++tdescp) {
	lock_ObtainWrite(&tkLock);
	for (tkl = tkList; tkl != NULL; tkl = tkl->next) {
	    if (tkl->flags & tklFlag_Granted) {
		if (AFS_hsame(tdescp->tokenID, tkl->tok.tokenID)
		    && tkl->fsp != NULL
		    && ((AFS_hgetlo(tkl->tokTypes) &
			 AFS_hgetlo(tdescp->type)) != 0
			|| (AFS_hgethi(tkl->tokTypes) &
			    AFS_hgethi(tdescp->type)) != 0)
		    && uuid_equal(&connId, &tkl->fsp->connUuid,
				  (unsigned32 *)&st)) {
		    break;	/* this seems to be the token! */
		}
	    } else if (tkl->flags & tklFlag_GrantInProgress) {
		if (tkl->fsp != NULL
		    && ((AFS_hgetlo(tkl->tokTypes) &
			 AFS_hgetlo(tdescp->type)) != 0
			|| (AFS_hgethi(tkl->tokTypes) &
			    AFS_hgethi(tdescp->type)) != 0)
		    && uuid_equal(&connId, &tkl->fsp->connUuid,
				  (unsigned32 *)&st)
		    && AFS_hsame(tdescp->fid.Volume, tkl->volId)) {
		    break;	/* this seems to be the token! */
		}
	    }
	}
	/* Found a match? */
	if (tkl != NULL /* && (tkl->flags & tklFlag_Revoked) == 0 */) {
	    /* revoke nothing--we still need it. */
	    if (tkl->flags & tklFlag_Granted) {
		VOLSERLOG(1, "STKN_TokenRevoke: still need token %lu,,%lu, type %#lx, on %lu,,%lu.\n",
			  AFS_HGETBOTH(tkl->tok.tokenID),
			  AFS_hgetlo(tkl->tok.type),
			  AFS_HGETBOTH(tkl->volId));
		tdescp->errorIDs = tkl->tok.type;
	    } else {
		/* the GrantInProgress case */
		VOLSERLOG(1, "STKN_TokenRevoke: still need pending token of type %#lx for %lu,,%lu.\n",
			  AFS_hgetlo(tkl->tokTypes),
			  AFS_HGETBOTH(tkl->volId));
		tdescp->errorIDs = tkl->tokTypes;
	    }
	    code = HS_ERR_PARTIAL;
	} else {		/* if we found no match */
	    VOLSERLOG(1, "STKN_TokenRevoke: no longer need token %lu,,%lu, type %#lx, on %lu,,%lu.\n",
		      AFS_HGETBOTH(tdescp->tokenID),
		      AFS_hgetlo(tdescp->type),
		      AFS_HGETBOTH(tdescp->fid.Volume));
	    AFS_hzero(tdescp->errorIDs);	/* grant the revocation--we no longer care. */
	}
	lock_ReleaseWrite(&tkLock);
	/* Ensure that we don't appear to accept column A or B choices. */
	AFS_hzero(tdescp->colAChoice);
	AFS_hzero(tdescp->colBChoice);
    }
    return code;	/* either 0 or HS_ERR_PARTIAL */
}


static error_status_t
STKN_GetCellName(handle_t h, afs_hyper_t *cellp, u_char *cellNameP)
{
    VOLSERLOG(1, "STKN_GetCellName() called!\n");
    *cellNameP = '\0';
    return EINVAL;
}


/*
 * These are debugging only RPC calls and might be removed without any warning
 */
static error_status_t
STKN_GetLock(handle_t  h, u_long index, afsDBLock *resultp)
{
    VOLSERLOG(1, "STKN_GetLock() called!\n");
    return EINVAL;
}

static error_status_t
STKN_GetCE(handle_t h, u_long index, afsDBCacheEntry *resultp)
{
    VOLSERLOG(1, "STKN_GetCE() called!\n");
    return EINVAL;
}

static error_status_t
STKN_GetServerInterfaces(handle_t  h, dfs_interfaceList *serverInterfacesP)
{
    register struct dfs_interfaceDescription *idp;
    rpc_if_id_t if_id;
    unsigned long st;

    /* only one interface defined so far */
    /* New interfaces or extensions will add to this list (not replacing the original). */
    serverInterfacesP->dfs_interfaceList_len = 0;
    rpc_if_inq_id (TKN4Int_v4_0_s_ifspec, &if_id, &st);
    if (st != error_status_ok)
	return st;
    serverInterfacesP->dfs_interfaceList_len = 1;
    idp = &serverInterfacesP->dfs_interfaceList_val[0]; /* fill this in */

    bzero((char *)idp, sizeof(struct dfs_interfaceDescription));
    idp->interface_uuid = if_id.uuid;
    idp->vers_major = if_id.vers_major;
    idp->vers_minor = if_id.vers_minor;
    idp->vers_provider = 1;	/* provider_version(1) from tkn4int.idl file */
    strncpy((char *)&idp->spareText[0],
	    "Transarc TKN fts revocation service",
	    MAXSPARETEXT);
    return error_status_ok;
}

static error_status_t
STKN_SetParams(handle_t h, u_long Flags, afsConnParams *paramsP)
{/* NOT IMPLEMENTED YET */
    return EINVAL;
}

globaldef TKN4Int_v4_0_epv_t TKN4Int_v4_0_manager_epv = {
	STKN_Probe,
	STKN_InitTokenState,
	STKN_TokenRevoke,
	STKN_GetCellName,
	STKN_GetLock,
	STKN_GetCE,
	STKN_GetServerInterfaces,
	STKN_SetParams
};


static int
getFSConn(u_char *prinName, struct afsNetAddr *pxaddrp, struct fsconn **fspp)
{
    char pnameBuf[500];
    struct siteDesc site;
    register long code;
    unsigned long st;
    afsUUID someSecUuid; /* dummy sec service uuid */
    register struct fsconn *fsp;
    register struct tknList *tkl;

    lock_ObtainWrite(&tkLock);
    for (tkl = tkList; tkl != NULL; tkl = tkl->next) {
	if (tkl->fsp != NULL && tkl->fsp->refCount > 0 && tkl->fsp->goneBad == 0
	    && bcmp((char *)&tkl->fsp->pxAddr, (char *)pxaddrp, sizeof(afsNetAddr)) ==0)
	    break;
    }
    if (tkl != NULL) {
	fsp = tkl->fsp;
	++(fsp->refCount);
	*fspp = fsp;	/* the one to return */
    } else {
	fsp = NULL;
    }
    lock_ReleaseWrite(&tkLock);
    if (fsp != NULL) {
	return 0;
    }
    fsp = (struct fsconn *) malloc(sizeof(struct fsconn));
    if (fsp == NULL) {
	dce_svc_printf(FTS_S_TOKENS_NO_MEMORY_FOR_FSCONN_MSG);
	return ENOMEM;
    }
    bzero((char *)fsp, sizeof(struct fsconn));
    fsp->refCount = 1;
    *fspp = fsp;	/* the one to return */
    fsp->pxAddr = *pxaddrp;
    if (prinName == NULL) {
	if (fsp->princName[0] != '\0') {
	    prinName = &fsp->princName[0];	/* use copy in fsp if it's there */
	} else {
	    code = vldb_GetSiteInfo(pxaddrp, &site);
	    if (code == 0) {
		prinName = &site.KerbPrin[0];
	    } else {
		dce_svc_printf(FTS_S_TOKENS_NO_PRINCIPAL_MSG, 
			       MapSockAddr(pxaddrp), 
			       dfs_dceErrTxt(code));
		--(fsp->refCount);
		return code;
	    }
	}
    }
    /* store copy in tkl if we need one */
    if (prinName != NULL && fsp->princName[0] == '\0')
	strncpy((char *)&fsp->princName[0], (char *)prinName, sizeof(fsp->princName));

    st = dfsuuid_Create(&fsp->connUuid, /* primary */ 0);
    if (st != error_status_ok) {
	dce_svc_printf(FTS_S_TOKENS_CANT_CREATE_UUID_MSG, 
		       MapSockAddr(pxaddrp), 
		       dfs_dceErrTxt(st));
	--(fsp->refCount);
	return st;
    }
    if (rpcx_binding_from_sockaddr((struct sockaddr *) pxaddrp,
				    &fsp->Exporter) != 0) {
	--(fsp->refCount);
	return FTS_ERR_BADCONN;
    }
    rpc_binding_set_object(fsp->Exporter, &fsp->connUuid, &st);
    if (st != error_status_ok) {
	dce_svc_printf(FTS_S_TOKENS_CANT_SET_OBJECT_ID_MSG, 
		       MapSockAddr(pxaddrp), 
		       dfs_dceErrTxt(st));
	--(fsp->refCount);
	return st;
    }
    if (prinName != NULL) {
	sprintf(pnameBuf, "%s/%s/dfs-server", currentCellName, prinName);
	code = dfsauth_client_InitBindingAuth(&fsp->Exporter, useNoAuth,
					      useLocalAuth, pnameBuf);
	if (code != 0) {
	    dce_svc_printf(FTS_S_TOKENS_SET_AUTH_INFO_ERROR_MSG, 
			   MapSockAddr(pxaddrp), 
			   dfs_dceErrTxt(code));
	    /* return code; *//* DROP THROUGH */
	}
    } else {
	dce_svc_printf(FTS_S_TOKENS_NO_PRINCIPAL_FOR_CONN_MSG,
		       MapSockAddr(pxaddrp));
	/* DROP THROUGH */
    }
    VOLSERLOG(1, "Connecting to file server on %s...", MapSockAddr(pxaddrp));
    st = (unsigned long) osi_Time();
    code = AFS_SetContext(fsp->Exporter, someStartTime, &someRevocationNetData,
			  0, &someSecUuid, OSI_MAX_FILE_PARM_CLIENT, 0);
    code = osi_errDecode(code);
    VOLSERLOG(1, "  ...connection done (result %ld)\n", code);
    if (code != 0) {
	dce_svc_printf(FTS_S_TOKENS_AFS_SETCONTEXT_FAILS_MSG,
		       MapSockAddr(pxaddrp), dfs_dceErrTxt(code));
	return code;
    }
    lock_ObtainWrite(&tkLock);
    if (fsp->lastSuccessfulCall < st)
	fsp->lastSuccessfulCall = st;
    /* pick some small default value */
    if (fsp->hostLifeGuarantee == 0)
	fsp->hostLifeGuarantee = ((minAdditionalTime == 0) ? 45 : minAdditionalTime);
    lock_ReleaseWrite(&tkLock);
    return 0;
}

static pthread_addr_t
keepAliveThread(pthread_addr_t arg)
{
    struct timeval tv;
    unsigned long lastKASweep;
    unsigned long expTime;
    register struct tknList *tkl;
    register long code;
    long someTimeS, newexpTimeS;
    unsigned long someTime, newexpTime, slopTime, bigSlop;
    int retryOK, retryCount;
    unsigned long st;
    /* for AFS_GetTime probe call */
    unsigned long secs, usecs, syncdist, syncdisp;
    /* for AFS_GetToken call */
    afsFid fid;
    afs_token_t inToken;
    afs_token_t outToken;
    afsFetchStatus outStatus;
    afs_recordLock_t rLock;
    afsVolSync outSync;
    /* for AFS_ReleaseTokens call */
    struct afsReturns Rvk;
    register struct afsReturnDesc *rev;

    for (;;) {
	/* Figure out how long we should sleep. */
	do {
	    lock_ObtainRead(&tkLock);
	    retryCount = 0;	/* use as tag as to whether initialized */
	    for (tkl = tkList; tkl != NULL; tkl = tkl->next) {
		if (tkl->flags & tklFlag_Granted) {
		    if (retryCount == 0) {
			expTime = tkl->tok.expirationTime;
			retryCount = 1;
		    } else if (tkl->tok.expirationTime < expTime) {
			expTime = tkl->tok.expirationTime;
		    }
		    /* Track host keep-alive as well as token expiration */
		    someTime = tkl->fsp->lastSuccessfulCall
		      + tkl->fsp->hostLifeGuarantee;
		    if (someTime < expTime)
			expTime = someTime;
		}
	    }
	    lock_ReleaseRead(&tkLock);
	    if (minAdditionalTime != 0) slopTime = minAdditionalTime >> 2;
	    else slopTime = 20;
	    if (retryCount == 0) {
		/* how long to sleep if nothing yet queued */
		expTime = osi_Time() + slopTime;
	    } else {
		/* Sleep til we reach almost expTime, but allow some slop time. */
		expTime -= slopTime;
	    }
	    bzero(&tv, sizeof(tv));
	    tv.tv_sec = expTime - osi_Time();
	    if (((long) tv.tv_sec) > 0) {
		code = select(0, 0, 0, 0, &tv);
		if (code < 0 && errno != EINTR) {
		    dce_svc_printf(FTS_S_TOKENS_SELECT_ERROR_MSG,
				   tv.tv_sec, errno, strerror(errno));
		    sleep(2); /* prevent tight spin-looping */
		}
	    }
	} while ((osi_Time() + 3) < expTime);
	/* don't sleep again if would be less than 3 secs */

	/* Now, renew all the active tokens. */
	bigSlop = (slopTime << 1) + (slopTime >> 1);	/* slopTime x 2.5 */
	lock_ObtainWrite(&tkLock);
	expTime = osi_Time() + bigSlop;
	for (tkl = tkList; tkl != NULL; tkl = tkl->next) {
	    if (tkl->flags & tklFlag_Granted) {	/* deal only with 'granted' tokens */
		if (expTime >
		    (tkl->fsp->lastSuccessfulCall + tkl->fsp->hostLifeGuarantee)) {
		    ++(tkl->refCount);
		    lock_ReleaseWrite(&tkLock);
		    VOLSERLOG(1, "kathread: Pinging server %s....",
			      MapSockAddr(&tkl->fsp->pxAddr));
		    retryCount = 0;
		    for (retryOK = 1; retryOK;) {
			retryOK = 0;
			++retryCount;
			st = (unsigned long) osi_Time();
			code = AFS_GetTime(tkl->fsp->Exporter,
					   &secs, &usecs, &syncdist, &syncdisp);
			code = osi_errDecode(code);
			if (code != 0 && retryCount < 3) {
			    if (useLocalAuth && code == rpc_s_auth_tkt_expired) {
				code = dfsauth_client_RefreshLocalAuthContext();
				if (code == 0) retryOK = 1;	/* retry the routine */
			    } else if (code == FSHS_ERR_STALEHOST) {
				tkl->fsp->goneBad = 1;
				code = getFSConn(NULL, &tkl->fsp->pxAddr, &tkl->fsp);
				if (code == 0) retryOK = 1;	/* retry it here too */
			    }
			}
		    }
		    VOLSERLOG(1, "done (returned %ld)\n", code);
		    lock_ObtainWrite(&tkLock);
		    --(tkl->refCount);
		    if (code == 0) {
			if (tkl->fsp->lastSuccessfulCall < st)
			    tkl->fsp->lastSuccessfulCall = st;
		    }
		    /* Refresh 'expTime' */
		    expTime = osi_Time() + bigSlop;
		}
		if (expTime > tkl->tok.expirationTime) {
		    bzero((char *)&fid, sizeof(fid));
		    fid.Cell = remoteCellID;
		    fid.Volume = tkl->volId;
		    fid.Vnode = ~(1L);		/* seek a whole-volume token */
		    fid.Unique = 0;		/* seek a whole-volume token */
		    bzero((char *)&inToken, sizeof(afs_token_t));
		    /* Multi-assignment */
		    inToken = tkl->tok;
		    ++(tkl->refCount);
		    tkl->flags |= tklFlag_GrantInProgress;
		    lock_ReleaseWrite(&tkLock);
		    VOLSERLOG(1, "kathread: Renewing token on %s....\n",
			      MapSockAddr(&tkl->fsp->pxAddr));
		    for (;;) {
			retryCount = 0;
			for (retryOK = 1; retryOK;) {
			    retryOK = 0;
			    ++retryCount;
			    st = (unsigned long) osi_Time();
			    code = AFS_GetToken(tkl->fsp->Exporter, &fid, &inToken,
						&osi_hZero, tkl->gtFlags,
						&outToken, &rLock,
						&outStatus, &outSync);
			    code = osi_errDecode(code);
			    if (code != 0 && retryCount < 3) {
				if (useLocalAuth
				    && code == rpc_s_auth_tkt_expired) {
				    code = dfsauth_client_RefreshLocalAuthContext();
				    if (code == 0) retryOK = 1;	/* retry the routine */
				} else if (code == FSHS_ERR_STALEHOST) {
				    /* somebody else might be using? */
				    tkl->fsp->goneBad = 1;
				    code = getFSConn(NULL, &tkl->fsp->pxAddr, &tkl->fsp);
				    if (code == 0) retryOK = 1;	/* retry it here too */
				}
			    }
			}
			lock_ObtainWrite(&tkLock);
			if ((gotGlobalTokenReset == 0)
			    && ((tkl->flags & tklFlag_Revoked) == 0)) {
			    break;	/* leave the ``for (;;)'' loop, lock held */
			}
			/* To resolve the grant/reset race here, try again. */
			gotGlobalTokenReset = 0;
			tkl->flags |= tklFlag_GrantInProgress;
			tkl->flags &= ~tklFlag_Revoked;
			lock_ReleaseWrite(&tkLock);
			/* relinquish the token if we may have gotten one. */
			if (code == 0) {
			    bzero((char *)&Rvk, sizeof(Rvk));
			    Rvk.afsReturns_len = 1;
			    rev = &Rvk.afsReturns_val[0];
			    rev->fid.Cell = remoteCellID;
			    rev->fid.Volume = tkl->volId;
			    rev->fid.Vnode = ~(1L);	/* whole-volume token */
			    rev->fid.Unique = 0;	/* whole-volume token */
			    rev->tokenID = outToken.tokenID;
			    rev->type = outToken.type;
			    VOLSERLOG(1, "Releasing raced-with token on file server...");
			    code = AFS_ReleaseTokens(tkl->fsp->Exporter, &Rvk, 0);
			    code = osi_errDecode(code);
			    if (code == 0) {
				VOLSERLOG(1, "done.\n");
			    } else {
				VOLSERLOG(1, "error:\nCouldn't release token %lu,,%lu on %lu,,%lu (%s): %s\n",
					  AFS_HGETBOTH(rev->tokenID),
					  AFS_HGETBOTH(tkl->volId),
					  MapSockAddr(&tkl->fsp->pxAddr),
					  dfs_dceErrTxt(code));
			    }
			}
			/* In any case, go back and try to get the token again. */
		    }
		    /* we're still globally-locked here */
		    if (code) {
			if (((unsigned long) osi_Time()) >= tkl->tok.expirationTime) {
			    tkl->flags |= tklFlag_Revoked;
			    tkl->flags &= ~(tklFlag_Granted|tklFlag_GrantInProgress);
			}
			lock_ReleaseWrite(&tkLock);
			dce_svc_printf(FTS_S_TOKENS_CANT_REGET_TOKEN_MSG, 
				       AFS_hgetlo(tkl->tok.type), 
				       AFS_HGETBOTH(tkl->volId),
				       MapSockAddr(&tkl->fsp->pxAddr),
				       dfs_dceErrTxt(code));
		    } else {
			/* track the min token lifetime */
			if (minAdditionalTime > outToken.expirationTime)
			    minAdditionalTime = outToken.expirationTime;
			outToken.expirationTime += st;	/* make absolute */
			if (tkl->fsp->lastSuccessfulCall < st)
			    tkl->fsp->lastSuccessfulCall = st;
			tkl->tok.tokenID = outToken.tokenID;
			tkl->tok.expirationTime = outToken.expirationTime;
			tkl->tok.type = outToken.type;
			/* don't really use ranges here, but... */
			tkl->tok.beginRange = outToken.beginRange;
			tkl->tok.endRange = outToken.endRange;
			tkl->flags &= ~tklFlag_GrantInProgress;
			tkl->flags |= tklFlag_Granted;
			lock_ReleaseWrite(&tkLock);
			VOLSERLOG(1, "  ...token %lu,,%lu, type %#lx, re-obtained on fileset %lu,,%lu.\n",
				  AFS_HGETBOTH(tkl->tok.tokenID),
				  AFS_hgetlo(tkl->tok.type),
				  AFS_HGETBOTH(tkl->volId));
		    }
		    lock_ObtainWrite(&tkLock);
		    --(tkl->refCount);
		    /* Refresh 'expTime' */
		    expTime = osi_Time() + bigSlop;
		}
	    }
	}
	/* Now check for obsolete structures. */
	FlushDeleted();
	lock_ReleaseWrite(&tkLock);
    }
}

/* This is the thread that spawns the RPC listener. */
static pthread_addr_t
rpcThread(pthread_addr_t arg)
{
    unsigned long st;

    /* Listen to the network for incoming calls */
    VOLSERLOG(1, "Listening for net calls (calling rpc_server_listen)\n");
    rpc_mgmt_set_server_com_timeout(rpc_c_binding_default_timeout+2, &st);
    if (rpc_mgmt_is_server_listening((rpc_binding_handle_t)NULL, &st) && 
       st == rpc_s_ok) /* already listening */
      return ((pthread_addr_t) 0);
    rpc_server_listen((int) arg, &st);
    VOLSERLOG(1, "Returned from rpc_server_listen: %s\n", dfs_dceErrTxt(st));
    if (st != error_status_ok && st != rpc_s_already_listening) {
	dce_svc_printf(FTS_S_TOKENS_RPC_NOT_LISTENING_MSG, dfs_dceErrTxt(st));
	return ((pthread_addr_t) 1);
    }
    return ((pthread_addr_t) 0);
}

static void initVolcTokens()
{
    pthread_t keepAliveThreadPtr, rpcListenerThreadPtr;
    pthread_attr_t subthreadAttr;
    rpc_binding_vector_t *bvecP;
    rpc_binding_handle_t binding;
    rpc_binding_handle_t *bindingP;
    register int ix;
    register long code;
    unsigned long st;
    int foundSelf;
    unsigned_char_t *stringBinding, *strUuid, *strProtseq, *strNetaddr, *strEndpoint;
    char *cellNamep, *hostNamep, *suffixp, *princNamep;
    struct sockaddr_in myRevoke;
    uuid_t nil_uuid;

    lock_Init(&tkLock);
    gotGlobalTokenReset = 0;
    minAdditionalTime = 0;
    someStartTime = (unsigned long) osi_Time();
    bzero((char *)&someRevocationNetData, sizeof(someRevocationNetData));
    /* Initialize the table used for decoding the error returned by DFS calls */
    fts_InitDecoder();

    /* Create the keep-alive thread. */
    pthread_attr_create(&subthreadAttr);
    pthread_attr_setstacksize(&subthreadAttr, 10000);
    VOLSERLOG(1, "Creating token keep-alive thread.\n");
    code = pthread_create(&keepAliveThreadPtr, subthreadAttr, keepAliveThread,
			  (pthread_addr_t)NULL);

    if (code) { /* thread creation failed */
	dce_svc_printf(FTS_S_TOKENS_KEEPALIVE_THREAD_MSG, errno, strerror(code));
	exit(1);
    }
    uuid_create_nil(&nil_uuid, (unsigned32 *)&st);
    if (st != uuid_s_ok) {
      dce_svc_printf(FTS_S_TOKENS_NO_NIL_UUID_MSG, code, dfs_dceErrTxt(code));
      exit(1);
    }
    /* Export our token-revocation interface. */
    st = rpc_s_ok;
#if 0
    dfs_register_rpc_server(TKN4Int_v4_0_s_ifspec,
			    (rpc_mgr_epv_t) &TKN4Int_v4_0_manager_epv,
			    &nil_uuid, &nil_uuid,
			     4, "admin.fts.tkn", "DFS fts tkn4", &st);
#else /* 0 */
    /* Need to be noauth, since we can't claim the machine's identity without being root */
    /* If we could be the running user, that would be great, too. */
    dfs_register_rpc_server_noauth(TKN4Int_v4_0_s_ifspec,
			    (rpc_mgr_epv_t) &TKN4Int_v4_0_manager_epv,
			    &nil_uuid, &nil_uuid,
			     4, "DFS fts tkn4", &st);
#endif /* 0 */
    if (st != error_status_ok) {
	dce_svc_printf(FTS_S_TOKENS_CANT_EXPORT_INTERFACE_MSG, dfs_dceErrTxt(st));
	exit(1);
    }
    /*
     * Inquire all protocol sequences supported by both the RPC runtime and
     * the underlying operating system.
     */
    rpc_server_inq_bindings(&bvecP, &st);
    if (st != error_status_ok) {
	dce_svc_printf(FTS_S_TOKENS_CANT_LIST_BINDINGS_MSG, dfs_dceErrTxt(st));
	exit(1);
    }
    foundSelf = 0;
    for (ix = 0, bindingP = &bvecP->binding_h[0]; ix < bvecP->count; ++ix, ++bindingP) {
	if (*bindingP == NULL)
	    continue;
	rpc_binding_to_string_binding(*bindingP, &stringBinding, &st);
	if (st != error_status_ok) {
	    dce_svc_printf(FTS_S_TOKENS_CANT_GET_BINDING_NUMBER_MSG, ix, dfs_dceErrTxt(st));
	    exit(1);
	}
	rpc_string_binding_parse(stringBinding, &strUuid, &strProtseq,
				 &strNetaddr, &strEndpoint, NULL, &st);
	if (st != error_status_ok) {
	    dce_svc_printf(FTS_S_TOKENS_CANT_PARSE_NUMBER_MSG, ix, stringBinding, dfs_dceErrTxt(st));
	    exit(1);
	}
	if (strcmp("ncadg_ip_udp", (char *)strProtseq) == 0
	    || strcmp("ip", (char *)strProtseq) == 0) {
	    foundSelf = 1;
	    bzero((char *)&myRevoke, sizeof(myRevoke));
	    myRevoke.sin_family = AF_INET;
	    myRevoke.sin_addr.s_addr = inet_addr(strNetaddr);
	    myRevoke.sin_port = (unsigned short)htons((unsigned short)atoi((char *)strEndpoint));
	    VOLSERLOG(1, "Revocation socket is: inet/%s/%d\n",
		      inet_ntoa(myRevoke.sin_addr),
		      myRevoke.sin_port);
	    someRevocationNetData.sockAddr = *((struct afsNetAddr *) &myRevoke);
	}
	if (stringBinding) rpc_string_free(&stringBinding, &st);
	if (strUuid) rpc_string_free(&strUuid, &st);
	if (strProtseq) rpc_string_free(&strProtseq, &st);
	if (strNetaddr) rpc_string_free(&strNetaddr, &st);
	if (strEndpoint) rpc_string_free(&strEndpoint, &st);

	/* Quit if we're done. */
	if (foundSelf != 0) break;
    }
    rpc_binding_vector_free(&bvecP, &st);
    if (st != error_status_ok) {
	dce_svc_printf(FTS_S_TOKENS_CANT_FREE_VECTOR_MSG, dfs_dceErrTxt(st));
	exit(1);
    }

    if (geteuid() == 0 && !useNoAuth) {
	/* Figure the local principal name for fx's revocation calls. */
	dce_cf_get_cell_name(&cellNamep, &st);
	if (st) {
	    dce_svc_printf(FTS_S_TOKENS_NO_LOCALCELL_NAME_MSG,
			   dfs_dceErrTxt(st));
	    exit(1);
	}
	/* Get the name of the machine principal, too */
	dce_cf_get_host_name(&hostNamep, &st);
	if (st) {
	    dce_svc_printf(FTS_S_TOKENS_NO_LOCALHOST_NAME_MSG,
			   dfs_dceErrTxt(st));
	    exit(1);
	}
	if (useLocalAuth)
	    suffixp = "dfs-server";
	else
	    suffixp = "self";
	if ((strlen(cellNamep)+strlen(hostNamep)+strlen(suffixp)+4) >=
	    sizeof(someRevocationNetData.principalName)) {
	    dce_svc_printf(FTS_S_TOKENS_PRINCNAME_TOO_BIG_MSG,
			   cellNamep, hostNamep);
	} else {
	    sprintf((char*)&someRevocationNetData.principalName[0], "%s/%s/%s",
		    cellNamep, hostNamep, suffixp);
	    VOLSERLOG(1, "Revocation principal name is '%s'\n",
		      &someRevocationNetData.principalName[0]);
	}
	free(cellNamep);
	free(hostNamep);

	rpc_server_register_auth_info(someRevocationNetData.principalName,
				      rpc_c_authn_default,
				      (rpc_auth_key_retrieval_fn_t)NULL,
				      (void *)NULL,
				      &st);
	if (st != error_status_ok) {
	    dce_svc_printf(FTS_S_TOKENS_BAD_REGISTERAUTH_MSG,
			   dfs_dceErrTxt(st));
	    someRevocationNetData.principalName[0] = '\0';
	}
    }

    /* Create the thread that will take the RPC requests. */
    VOLSERLOG(1, "Creating RPC listener thread.\n");
    code = pthread_create(&rpcListenerThreadPtr, subthreadAttr, rpcThread,
			  (pthread_addr_t)2);	/* number of sub-threads to use */

    if (code) { /* thread creation failed */
	dce_svc_printf(FTS_S_TOKENS_CANT_CREATE_LISTENER_MSG,
		       errno, strerror(errno));
	exit(1);
    }

    inited = 1;
}

static void ensureTokensInited() {
    if (inited == 0) {
	if (pthread_once(&tokensInitedBlock, &initVolcTokens) != 0)
	    VOLSERLOG(0, "Cannot init token module: %d\n", errno);
    }
}

SHARED int fts_GetToken(servAddrp, prinName, volIdp, desiredTokens, gtFlags, descriptorP)
struct afsNetAddr *servAddrp;
unsigned char *prinName;
afs_hyper_t *volIdp;
long desiredTokens;
unsigned long gtFlags;
int *descriptorP;
{
    register struct tknList *tkl;
    afsFid fid;
    afs_token_t inToken;
    afs_token_t outToken;
    afsFetchStatus outStatus;
    afs_recordLock_t rLock;
    afsVolSync outSync;
    struct afsReturns Rvk;
    register struct afsReturnDesc *rev;
    register long code;
    unsigned long st;
    int retryOK, retryCount, sleepHere;
    unsigned long loopStartTime, loopLifeTime;

    ensureTokensInited();

    VOLSERLOG(1, "About to get token %lx for fileset %lu,,%lu from %s.\n",
	       desiredTokens, AFS_HGETBOTH(*volIdp), MapSockAddr(servAddrp));
    tkl = (struct tknList *) malloc(sizeof(struct tknList));
    if (tkl == NULL) {
	dce_svc_printf(FTS_S_TOKENS_NO_MEMORY_FOR_TOKEN_MSG);
	return ENOMEM;
    }
    (void) vldb_GetRemoteCellID();	/* (void) for now */
    bzero((char *)tkl, sizeof(struct tknList));
    tkl->volId = *volIdp;
    ++lastTokenDescriptor;
    tkl->tokenDescriptor = lastTokenDescriptor;
    tkl->refCount = 1;
    tkl->flags = tklFlag_GrantInProgress;
    tkl->gtFlags = gtFlags;
    AFS_hset64(tkl->tokTypes, 0, desiredTokens);
   /* Make this 'tkl' structure visible globally. */
    lock_ObtainWrite(&tkLock);
    tkl->next = tkList;
    tkList = tkl;
    lock_ReleaseWrite(&tkLock);
    code = getFSConn(prinName, servAddrp, &tkl->fsp);
    lock_ObtainWrite(&tkLock);
    if (code != 0) {
	tkl->flags |= tklFlag_Revoked;
	--(tkl->refCount);
	FlushDeleted();
	lock_ReleaseWrite(&tkLock);
	return code;
    }
    /* Kill the tklFlag_Revoked bit from the InitTokenState since we don't have one yet! */
    tkl->flags &= ~tklFlag_Revoked;
    lock_ReleaseWrite(&tkLock);
    loopStartTime = 0;
    for (;;) {
	bzero((char *)&fid, sizeof(fid));
	fid.Cell = remoteCellID;
	fid.Volume = *volIdp;
	fid.Vnode = ~(1L);		/* seek a whole-volume token */
	fid.Unique = 0;		/* seek a whole-volume token */
	bzero((char *)&inToken, sizeof(afs_token_t));
	AFS_hset64(inToken.type, 0, desiredTokens);
	inToken.endRange = osi_hMaxint64;
	retryCount = 0;
	for (retryOK = 1; retryOK;) {
	    retryOK = 0;
	    ++retryCount;
	    st = (unsigned long) osi_Time();
	    if (loopStartTime == 0) loopStartTime = st;
	    code = AFS_GetToken(tkl->fsp->Exporter, &fid, &inToken, &osi_hZero,
				tkl->gtFlags,
				&outToken, &rLock, &outStatus, &outSync);
	    code = osi_errDecode(code);
	    if (code != 0 && retryCount < 3) {
		if (useLocalAuth && code == rpc_s_auth_tkt_expired) {
		    code = dfsauth_client_RefreshLocalAuthContext();
		    if (code == 0) retryOK = 1;	/* retry the routine */
		} else if (code == FSHS_ERR_STALEHOST) {
		    tkl->fsp->goneBad = 1;
		    code = getFSConn(prinName, servAddrp, &tkl->fsp);
		    if (code == 0) retryOK = 1;	/* retry it here too */
		}
	    }
	}
	sleepHere = 0;
	lock_ObtainWrite(&tkLock);
	if ((gotGlobalTokenReset == 0) && ((tkl->flags & tklFlag_Revoked) == 0)) {
	    /* Got the token, or an error code, without a conflicting revocation.
	     See if we should try again. */
	    if (code != TKM_ERROR_TRYAGAIN && code != TKM_ERROR_TIMEDOUT
		&& code != TKM_ERROR_NOMEM)
		break;	/* nothing to try again */
	    loopLifeTime = tkl->fsp->hostLifeGuarantee;
	    if (loopLifeTime < maxHostLifetime) loopLifeTime = maxHostLifetime;
	    if (loopLifeTime < 60) loopLifeTime = 60;
	    loopLifeTime *= 30;
	    if ((loopStartTime + loopLifeTime) < osi_Time()) {
		dce_svc_printf(FTS_S_TOKENS_NO_WAITING_FOR_TOKEN_MSG, 
			       desiredTokens, AFS_HGETBOTH(tkl->volId),
			       MapSockAddr(servAddrp), dfs_dceErrTxt(code));
		break;
	    }
	    /* Plan to sleep after possibly returning a raced token */
	    sleepHere = 1;
	    VOLSERLOG(1, "Waiting for token %lx on %lu,,%lu (%s): %s\n",
		      desiredTokens, AFS_HGETBOTH(tkl->volId),
		      MapSockAddr(servAddrp), dfs_dceErrTxt(code));
	}
	gotGlobalTokenReset = 0;	/* To resolve the grant/reset race here, try again. */
	tkl->flags |= tklFlag_GrantInProgress;
	tkl->flags &= ~tklFlag_Revoked;
	lock_ReleaseWrite(&tkLock);
	if (code == 0) {	/* relinquish the token if we may have gotten one. */
	    bzero((char *)&Rvk, sizeof(Rvk));
	    Rvk.afsReturns_len = 1;
	    rev = &Rvk.afsReturns_val[0];
	    rev->fid.Cell = remoteCellID;
	    rev->fid.Volume = *volIdp;
	    rev->fid.Vnode = ~(1L);		/* whole-volume token */
	    rev->fid.Unique = 0;		/* whole-volume token */
	    rev->tokenID = outToken.tokenID;
	    rev->type = outToken.type;
	    VOLSERLOG(1, "Releasing raced-with token on file server...");
	    code = AFS_ReleaseTokens(tkl->fsp->Exporter, &Rvk, 0);
	    code = osi_errDecode(code);
	    if (code == 0) {
		VOLSERLOG(1, "done.\n");
	    } else {
		VOLSERLOG(1, "error:\nCouldn't release the token on %lu,,%lu (%s): %s\n",
			  AFS_HGETBOTH(tkl->volId),
			  MapSockAddr(servAddrp), dfs_dceErrTxt(code));
	    }
	}
	if (sleepHere) {
	    struct timeval TV;
	    bzero((char *)&TV, sizeof(TV));
	    TV.tv_sec = 10;
	    if (select(0, 0, 0, 0, &TV) < 0)
		sleep(10);
	}
    }
    if (code) {
	tkl->flags |= tklFlag_Revoked;
	--(tkl->refCount);
	FlushDeleted();
	lock_ReleaseWrite(&tkLock);
	dce_svc_printf(FTS_S_TOKENS_CANT_GET_TOKEN_MSG, 
		       desiredTokens, AFS_HGETBOTH(*volIdp),
		       MapSockAddr(servAddrp), dfs_dceErrTxt(code));
	return code;
    }
    if (tkl->fsp->lastSuccessfulCall < st)
	tkl->fsp->lastSuccessfulCall = st;
    tkl->tok.tokenID = outToken.tokenID;
    /* track the minimum token lifetime */
    if (minAdditionalTime > outToken.expirationTime)
	minAdditionalTime = outToken.expirationTime;
    /* make expiration time absolute */
    outToken.expirationTime += st;
    tkl->tok.expirationTime = outToken.expirationTime - 15;	/* we don't get a reference time */
    tkl->tok.type = outToken.type;
    /* don't really use ranges here, but... */
    tkl->tok.beginRange = outToken.beginRange;
    tkl->tok.endRange = outToken.endRange;
    tkl->flags &= ~tklFlag_GrantInProgress;
    tkl->flags |= tklFlag_Granted;
    --(tkl->refCount);
    lock_ReleaseWrite(&tkLock);
    VOLSERLOG(1, "Token %lu,,%lu, type %#lx, obtained on fileset %lu,,%lu.\n",
	       AFS_HGETBOTH(tkl->tok.tokenID),
	       AFS_hgetlo(tkl->tok.type),
	       AFS_HGETBOTH(tkl->volId));
    *descriptorP = tkl->tokenDescriptor;
    return 0;
}

/* Release a previously-granted token. */
SHARED int fts_ReleaseToken(descriptor)
int descriptor;
{
    register struct tknList *tkl, *Xtkl;
    afsFid fid;
    register long code;
    struct afsReturns Rvk;
    register struct afsReturnDesc *rev;
    unsigned long st;
    int retryOK, retryCount;

    lock_ObtainShared(&tkLock);
    for (tkl = tkList; tkl != NULL; tkl = tkl->next) {
	if (tkl->tokenDescriptor == descriptor) break;
    }
    if (tkl == NULL) {
	lock_ReleaseShared(&tkLock);
	dce_svc_printf(FTS_S_TOKENS_CANT_FIND_RIGHT_TOKEN_MSG, descriptor);
	return ENOENT;
    }
    /* Found the token structure. */
    VOLSERLOG(1, "Releasing token (%d) for fileset %lu,,%lu from %s.\n",
	       descriptor,
	       AFS_HGETBOTH(tkl->volId), MapSockAddr(&tkl->fsp->pxAddr));
    /* First, remove tkl from the list being kept alive. */
    lock_UpgradeSToW(&tkLock);
    ++(tkl->refCount);
    tkl->flags |= tklFlag_Revoked;
    lock_ReleaseWrite(&tkLock);

    /* Now, tell the remote PX that we don't need this token any more. */
    bzero((char *)&Rvk, sizeof(Rvk));
    Rvk.afsReturns_len = 1;
    rev = &Rvk.afsReturns_val[0];
    rev->fid.Cell = remoteCellID;
    rev->fid.Volume = tkl->volId;
    rev->fid.Vnode = ~(1L);		/* whole-volume token */
    rev->fid.Unique = 0;		/* whole-volume token */
    rev->tokenID = tkl->tok.tokenID;
    rev->type = tkl->tok.type;
    VOLSERLOG(1, "Releasing token on file server...");
    retryCount = 0;
    for (retryOK = 1; retryOK;) {
	retryOK = 0;
	++retryCount;
	st = (unsigned long) osi_Time();
	code = AFS_ReleaseTokens(tkl->fsp->Exporter, &Rvk, 0);
	code = osi_errDecode(code);
	if (code != 0 && retryCount < 3) {
	    if (useLocalAuth && code == rpc_s_auth_tkt_expired) {
		code = dfsauth_client_RefreshLocalAuthContext();
		if (code == 0) retryOK = 1;	/* retry the routine */
	    } else if (code == FSHS_ERR_STALEHOST) {
		tkl->fsp->goneBad = 1;
		code = getFSConn(NULL, &tkl->fsp->pxAddr, &tkl->fsp);
		if (code == 0) retryOK = 1;	/* retry it here too */
	    }
	}
    }
    if (code == 0) {
	VOLSERLOG(1, "done.\n");
    } else {
	VOLSERLOG(1, "***failure\n");
	dce_svc_printf(FTS_S_TOKENS_CANT_RELEASE_TOKEN_MSG,
		  AFS_HGETBOTH(tkl->volId),
		  MapSockAddr(&tkl->fsp->pxAddr),
		  dfs_dceErrTxt(code));
    }
    lock_ObtainWrite(&tkLock);
    if (code == 0) {
	if (tkl->fsp->lastSuccessfulCall < st)
	    tkl->fsp->lastSuccessfulCall = st;
    }
    --(tkl->refCount);
    tkl->flags &= ~(tklFlag_Granted | tklFlag_GrantInProgress);
    tkl->flags |= tklFlag_Revoked;
    FlushDeleted();
    lock_ReleaseWrite(&tkLock);
    return 0;
}

#if 0
/* don't need to do this if we're using VOL_IS_REPLICATED */
/* Make an AFS_BulkKeepAlive call to refresh the exporter's cache. */
SHARED int fts_FreshenDally(servAddrp, prinName, volIdp)
struct afsNetAddr *servAddrp;
unsigned char *prinName;
afs_hyper_t *volIdp;
{
    afsFid fid;
    afs_token_t inToken;
    afs_token_t outToken;
    afsFetchStatus outStatus;
    afsVolSync outSync;
    register long code;
    unsigned long st;
    int retryOK, retryCount;
    struct fsconn *fsp;
    struct afsBulkFEX fexArg;

    ensureTokensInited();

    VOLSERLOG(1, "About to refresh KnowDally on server %s, id %lu,,%lu.\n",
	       MapSockAddr(servAddrp), AFS_HGETBOTH(*volIdp));
    (void) vldb_GetRemoteCellID();	/* (void) for now */
    fsp = NULL;
    (void) vldb_GetRemoteCellID();	/* (void) for now */
    code = getFSConn(prinName, servAddrp, &fsp);
    if (code != 0) {
	return code;
    }
    bzero(&fexArg, sizeof(fexArg));
    fexArg.afsBulkFEX_len = 1;
    fexArg.afsBulkFEX_val[0].fid.Cell = remoteCellID;
    fexArg.afsBulkFEX_val[0].fid.Volume = *volIdp;
    /* The actual vnode/uniquifier don't matter. */
    fexArg.afsBulkFEX_val[0].fid.Vnode = 1;
    fexArg.afsBulkFEX_val[0].fid.Unique = 1;
    fexArg.afsBulkFEX_val[0].keepAliveTime = 15;

    retryCount = 0;
    for (retryOK = 1; retryOK;) {
	retryOK = 0;
	++retryCount;
	code = AFS_BulkKeepAlive(fsp->Exporter, &fexArg, /* numExec: */0,
				 /* flags: */ 0, /* spares: */ 0, 0, &st);
	code = osi_errDecode(code);
	if (code != 0 && retryCount < 3) {
	    if (useLocalAuth && code == rpc_s_auth_tkt_expired) {
		code = dfsauth_client_RefreshLocalAuthContext();
		if (code == 0) retryOK = 1;	/* retry the routine */
	    } else if (code == FSHS_ERR_STALEHOST) {
		fsp->goneBad = 1;
		code = getFSConn(prinName, servAddrp, &fsp);
		if (code == 0) retryOK = 1;	/* retry it here too */
	    }
	}
    }
    --(fsp->refCount);
    if (fsp->refCount == 0) {
	rpc_binding_free(&fsp->Exporter, &st);
	free((opaque) fsp);
    }
    if (code) {
	dce_svc_printf(FTS_S_TOKENS_CANT_REFRESH_KNOWDALLY_MSG, 
		       AFS_HGETBOTH(*volIdp),
		       MapSockAddr(servAddrp), dfs_dceErrTxt(code));
	return code;
    }
    VOLSERLOG(1, "Refreshed knowDally on fileset %lu,,%lu, server %s.\n",
	       AFS_HGETBOTH(*volIdp), MapSockAddr(servAddrp));
    return 0;
}
#endif /* 0 */
