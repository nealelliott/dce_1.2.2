/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: volc_misc.c,v $
 * Revision 1.1.123.1  1996/10/02  21:15:22  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:44  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <fcntl.h>
#include <dcedfs/sysincludes.h>
#include <ctype.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/ftserver_data.h>
#include <dcedfs/ioctl.h>
#include <dcedfs/aggr.h>
#include <dcedfs/fldb_data.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/flserver.h>
#include <dcedfs/rep_data.h>
#include <dcedfs/ftserver.h>
#include <dcedfs/compat.h>

#include <volc.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftimac.h>
#include <dfsftisvc.h>
#include <dfsftimsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/userInt/fts/volc_misc.c,v 1.1.123.1 1996/10/02 21:15:22 damon Exp $")

int useNoAuth = 0;
int useLocalAuth = 0;

/*
 * Extract name of volume <volName> without RO or BACK suffixes 
 */
char *ExtractSuffix(volNamep)
    char *volNamep;
{   
    static char sname[FTSERVER_MAXFSNAME];
    int len = strlen(volNamep);

    strncpy(sname, volNamep, sizeof(sname));
    if (!strcmp(&sname[len - 9], ".readonly"))
	sname[len - 9] = '\0';			/* discard the last 9 chars */
    else if (!strcmp(&sname[len - 7], ".backup"))
	sname[len - 7] = '\0';			/* discard last 7 chars */
    return sname;
}


/*
 * Returns 1 if volume name is too long else 0
 */
NameTooLong(volNamep)
    char *volNamep;
{
    if (strlen(volNamep) >= (unsigned)(FTSERVER_MAXFSNAME - 9)) {
	dce_svc_printf(FTS_S_MISC_NAME_TOO_LONG_MSG, volNamep,
		  (FTSERVER_MAXFSNAME - 9));	
	return 1;
    }
    return 0;
}


/* 
 * fts_IsNumeric -- returns 1 if volNamep is all numeric else 0.  Really, it
 *     acts as a boolean predicate on whether the string argument is
 *     convertable as an afs_hyper_t.  It also returns the value of the
 *     afs_hyper_t in outP.
 *
 * PARAMETERS --
 *     warnFlag -- if true generates a warning if the string is convertable to
 *         a hyper.
 *     doubleOK -- if false requires that the string be converted to a signed
 *         32-bit integer otherwise it fails by returning zero.
 */
fts_IsNumeric(volNamep, warnFlag, doubleOK, outP)
    char *volNamep;
    int warnFlag, doubleOK;
    afs_hyper_t *outP;
{
    int code;
    char *end;

    code = dfsh_StrToHyper(volNamep, outP, &end);
    if (code || *end != '\0')
	return 0;
    if (!doubleOK && AFS_hgethi(*outP) != 0) {
	dce_svc_printf(FTS_S_MISC_NUMBER_TOO_LARGE_MSG, programName, volNamep);
	return 0;
    }
    if (warnFlag)
	dce_svc_printf(FTS_S_MISC_ILLEGAL_NAME_MSG, volNamep);
    return 1;
}


/* 
 * Returns 0 if <volNamep> is a proper fileset name (i.e. doesn't end in ".readonly" or
 * ".backup", it's not numeric, etc.), else 1 
 */
InvalidVolName(volNamep)
    char *volNamep;
{   
    int len = strlen(volNamep);
    afs_hyper_t T;

    if ((len <= 9 || strcmp(&volNamep[len-9], ".readonly") != 0) && (len <= 7 || strcmp(&volNamep[len-7], ".backup") != 0)) {
	if (NameTooLong(volNamep) || fts_IsNumeric(volNamep, 1, 1, &T))
	    return 1;
	else
	    return 0;
    }
    dce_svc_printf(FTS_S_MISC_NAME_ENDING_MSG, volNamep);
    return 1;
}


/*
 * Converts the incoming volume Name to its representive volume Id. If it's an actual
 * name we look in the VLDB to find it's id, else we return the number passed in as
 * volNamep.
 */
u_long GetVolumeID(volNamep, volIdp)
    char *volNamep; 
    afs_hyper_t *volIdp;
{
    struct vldbentry entry;
    int tryname, code, k, len = strlen(volNamep);
    int xl[3];

    tryname = fts_IsNumeric(volNamep, 0, 1, volIdp);
    if (!tryname) {
	AFS_hzero(*volIdp);
	code = vldb_GetEntryByName(ExtractSuffix(volNamep), &entry, 0);
	if (!code) {
	    for (k = 0; k < 3; ++k) xl[k] = -1;
	    for (k = 0; k < MAXVOLTYPES && !AFS_hiszero(entry.VolIDs[k]); ++k) {
		if (entry.VolTypes[k] == VOLTIX_TO_VOLTYPE(RWVOL)) {
		    xl[RWVOL] = k;
		} else if (entry.VolTypes[k] == VOLTIX_TO_VOLTYPE(ROVOL)) {
		    xl[ROVOL] = k;
		} else if (entry.VolTypes[k] == VOLTIX_TO_VOLTYPE(BACKVOL)) { xl[BACKVOL] = k;
		}
	    }
	    if (len > 9 && !strcmp(&volNamep[len-9], ".readonly")) {
		k = ROVOL;
	    } else if (len > 7 && !strcmp(&volNamep[len-7], ".backup")) {
		k = BACKVOL;
	    } else {
		k = RWVOL;
	    }
	    if (xl[k] >= 0) {
		*volIdp = entry.VolIDs[xl[k]];
		return 1;
	    } else {
		return 0;
	    }
	} else {
	    return 0;
	}
    } else {
	return 1;
    }
}


/* 
 * We presently support only the DARPA Internet family (inet): the specified address is
 * either a host name present in some host name data base, or a standard DARPA internet 
 * 'dot' notation.  Returns 1 if the host could be identified, else 0.
 */
int GetServer(serverNamep, servAddrp)
    char *serverNamep;
    struct sockaddr *servAddrp;
{
    register struct hostent *th;
    long addr, code;
    register struct sockaddr_in *sin = (struct sockaddr_in *) servAddrp;
    rpc_binding_handle_t handle;
    error_status_t st;
    long	cacheCode;
    register char *s;
    register int Ix, Dum;
    int Family, AllOK;
    uuid_t nil_uuid;

    bzero((char *)servAddrp, sizeof(struct sockaddr));
    /* give preference (and speed) to the dotted-quad notation */
    for (s = serverNamep; *s != '\0'; ++s) {
	if (*s != '.' && *s != ':' && !isdigit(*s)) break;
    }
    if (*s == '\0') {
	addr = inet_addr(serverNamep);
	if (addr != -1) {
	    sin->sin_family = AF_INET;
	    sin->sin_addr.s_addr = addr;
	    return 1;
	}
	/* try our own ``family:byte.byte.byte.byte...'' form. */
	AllOK = 0;
	s = serverNamep;
	Family = atoi(s);
	if (Family > 0 && Family < 0xffff && *s != '-') {
	    while (isdigit(*s)) ++s;
	    if (*s == ':') {
		++s;
		AllOK = 1;
		for (Ix = 0; Ix < sizeof(servAddrp->sa_data); ++Ix) {
		    if (!isdigit(*s)) {
			AllOK = 0; break;
		    }
		    Dum = 0;
		    while (isdigit(*s)) {
			Dum *= 10; Dum += (*s - '0');
			++s;
		    }
		    if (Dum < 0 || Dum > 0xff || (*s != '.' && *s != '\0')) {
			AllOK = 0; break;
		    }
		    servAddrp->sa_data[Ix] = Dum;
		    if (*s == '\0') break;
		    ++s;		/* skip over the dot */
		}
	    }
	    if (AllOK != 0) {
		servAddrp->sa_family = Family;
		return 1;
	    }
	}
	dce_svc_printf(FTS_S_MISC_MALFORMED_ADDRESS_MSG, serverNamep);
	return 0;
    }
    else {
        uuid_create_nil(&nil_uuid, (unsigned32 *)&st);
	if (st != uuid_s_ok) {
	  dce_svc_printf(FTS_S_MISC_CAN_MAKE_NOTHING_MSG, dfs_dceErrTxt(st));
	  return 0;
	}	  	
	/* we have some hope of doing the right thing in this case */
	dfs_locate_rpc_server((unsigned char *)serverNamep, &handle, 
			      &nil_uuid, useLocalAuth, useNoAuth, &st);

	/* we assume that it's all right to force noauth w/o warnings, in this case */
	if ((st != rpc_s_ok) && (st != COMPAT_ERROR_SHORTNAME_NOAUTH)) {
	    dce_svc_printf(FTS_S_MISC_CANT_TRANSLATE_MSG, serverNamep, dfs_dceErrTxt(st));
	    return 0;
	}
	code = rpcx_binding_to_sockaddr(handle, (struct sockaddr_in *) servAddrp);
	if (code != 0) {
	    dce_svc_printf(FTS_S_MISC_ADDRESS_TRANSLATION_MSG, serverNamep);
	    return 0;
	}

	/* save this server connection and all the binding (& auth) info */
	cacheCode = CacheServerConnection(servAddrp, handle,
					  (*serverNamep != '/'));
	return (cacheCode == 0? 1 : 0);
    }
}

static char *NumericMap(addrp)
struct sockaddr *addrp;
{/* Just do numeric form. */
    static char ansbuf[150];
    int ix, bigIx;
    char *cp;

    sprintf(ansbuf, "%u:%u", addrp->sa_family,
	     (unsigned char) addrp->sa_data[0]);
    for (bigIx = sizeof(addrp->sa_data)-1; bigIx > 0; --bigIx)
	if (addrp->sa_data[bigIx] != 0) break;
    for (ix = 1; ix <= bigIx; ++ix) {
	cp = ansbuf + strlen(ansbuf);
	sprintf(cp, ".%u", (unsigned char) addrp->sa_data[ix]);
    }
    return ansbuf;
}

static int saInit = 0;
#define	NUMSAVEDSA  5
#define	NUMSAVEDNAME	200
static struct saCacheEntry {
    struct sockaddr sa;
    char name[NUMSAVEDNAME];
} saCache[NUMSAVEDSA];
static int saRover = 0;
static struct lock_data saLock;

char *MapSockAddr(addrp)
    struct sockaddr *addrp;
{
    register struct sockaddr_in *sin;
    register int ix;
    char *ans;

    if (saInit == 0) {
	lock_Init(&saLock);
	bzero((char *)saCache, sizeof(saCache));
	saInit = 1;
    }
    /* use cache */
    lock_ObtainWrite(&saLock);
    for (ix = 0; ix < NUMSAVEDSA; ++ix) {
	if (bcmp((char *)&saCache[ix].sa,
		 (char *)addrp,
		 sizeof(struct sockaddr)) == 0) {
	    lock_ReleaseWrite(&saLock);
	    return &saCache[ix].name[0];
	}
    }
    if (addrp->sa_family == AF_INET) {
	sin = (struct sockaddr_in *) addrp;
	ans = (char *) hostutil_GetNameByINet(sin->sin_addr);
    } else {
	ans = (char *) NumericMap(addrp);
    }
    /* fill in cache */
    /* choose an arbitrary entry to overwrite */
    ix = saRover;
    ++saRover;
    if (saRover  == NUMSAVEDSA)
	saRover = 0;
    saCache[ix].sa = *addrp;
    strncpy(&saCache[ix].name[0], ans, NUMSAVEDNAME);
    lock_ReleaseWrite(&saLock);
    return ans;
}


static int agnaInit = 0;
#define	NUMSAVEDAGNA  40
#define	NUMSAVEDAGNAME	50
static struct agnaCacheEntry {
    struct sockaddr sa;
    unsigned long agId;
    char agName[NUMSAVEDAGNAME];
    char agDevName[NUMSAVEDAGNAME];
    long code;
} agnaCache[NUMSAVEDAGNA];
static int agnaRover = 0;
static struct lock_data agnaLock;

static void addAgCache(sap, id, nam, devnam, code)
struct sockaddr *sap;
unsigned long id;
char *nam, *devnam;
{
    register int ix;

    ix = agnaRover;
    ++agnaRover;
    if (agnaRover == NUMSAVEDAGNA)
	agnaRover = 0;
    agnaCache[ix].sa = *sap;
    agnaCache[ix].agId = id;
    strncpy(&agnaCache[ix].agName[0], nam, NUMSAVEDAGNAME);
    strncpy(&agnaCache[ix].agDevName[0], devnam, NUMSAVEDAGNAME);
    agnaCache[ix].code = code;
}

/*
 * Returns the aggregate's Id if the aggregate name (or Id) parameter is a valid one in
 * the server passed in; if no validation is required (validateFlag == 0) only numeric
 * aggregate param is accepted.  Otherwise we actually do an rpc to the server to verify
 * this; return -1 if something went wrong (like the aggregate isn't found).
 */
GetAggr(aggrnamep, servAddrp, validateFlag, aggrIdp, aggrNamep)
    char *aggrnamep;
    struct sockaddr *servAddrp;
    long validateFlag;
    unsigned long *aggrIdp;
    char *aggrNamep;
{   
    struct ftserver_aggrList *aggrListp;
    register struct ftserver_aggrList *aggrp;
    register int i;
    int numeric = 0, thisIx;
    afs_hyper_t T;
    long code, numAggrs, tcode;
    int samei;

    if (fts_IsNumeric(aggrnamep, 0, 0, &T)) {
	numeric = 1;
	*aggrIdp = AFS_hgetlo(T);
    }
    if (validateFlag == GETAGGR_NUMERICONLY) {
	if (!numeric) {
	    dce_svc_printf(FTS_S_MISC_ILLEGAL_AGGREGATE_ID_MSG, aggrnamep);
	    PrintError("GetAggr", FTS_ERR_BADAGGREGATE);
	    return 0;
	}
	return 1;
    }

    if (agnaInit == 0) {
	lock_Init(&agnaLock);
	bzero((char *)agnaCache, sizeof(agnaCache));
	agnaInit = 1;
    }
    lock_ObtainWrite(&agnaLock);
    samei = -1;
    for (i = 0; i < NUMSAVEDAGNA; ++i) {
	if (bcmp((char *)servAddrp,
		 (char *)&agnaCache[i].sa,
		 sizeof(struct sockaddr)) == 0) {
	    /* Check for any RPC failure from the same server. */
	    tcode = agnaCache[i].code;
	    if ((tcode >= rpc_s_mod) && (tcode <= (rpc_s_mod+4096)))
		samei = i;
	    if (numeric) {
		if (agnaCache[i].agId == *aggrIdp) break;
	    } else {
	        if (strcmp(&agnaCache[i].agName[0], aggrnamep) == 0) break;
		if (strcmp(&agnaCache[i].agDevName[0], aggrnamep) == 0) break;
	    }
	}
    }
    /* Use the exact match if we have one, else the same-server RPC failure, if any. */
    if (i >= NUMSAVEDAGNA && samei >= 0)
	i = samei;
    if (i < NUMSAVEDAGNA) {
	/* found in cache. */
	/* See if it was failure or not. */
	if (agnaCache[i].code) {
	    /* a cached failure */
	    /* Even failed validation is OK if a number was given and validateFlag agrees */
	    lock_ReleaseWrite(&agnaLock);
	    return ((validateFlag == GETAGGR_NUMERICOK && numeric) ? 1 : 0);
	} else {
	    if (aggrNamep)
		strcpy(aggrNamep, &agnaCache[i].agName[0]);
	    *aggrIdp = agnaCache[i].agId;
	    lock_ReleaseWrite(&agnaLock);
	    return 1;
	}
    }
    if ((servAddrp->sa_family == ((unsigned short) -1))
	 || servAddrp->sa_family == 0) {
	lock_ReleaseWrite(&agnaLock);
	dce_svc_printf(FTS_S_MISC_EVALUATE_AGGREGATE_MSG, aggrnamep);
	PrintError("GetAggr", FTS_ERR_BADSERVADDR);
	return 0;
    }
    /* Make the RPC. */
    code = volc_Call(fts_GetAllAggrs, servAddrp, NULL, &numAggrs, &aggrListp);
    if (!code) {
	/* add 'em all to the cache */
	thisIx = -1;
	for (i = 0, aggrp = aggrListp; i < numAggrs; ++i, ++aggrp) {
	    addAgCache(servAddrp, aggrp->Id, aggrp->name, aggrp->dvname, 0);
	    /* Look again, and stop when we find the desired one, which might
	     not have been in the cache, if it overflowed. */
	    if (thisIx < 0) {
		if (numeric) {
		    if (aggrp->Id == *aggrIdp) thisIx = i;
		} else {
		    if (strcmp((char *)aggrp->name, aggrnamep) == 0) thisIx = i;
		    if (strcmp((char *)aggrp->dvname, aggrnamep) == 0) thisIx = i;
		}
		if (thisIx >= 0) {
		    /* Found it.  Return its official Id and also copy its name if the
		    * parameter is there. */
		    *aggrIdp = aggrListp[thisIx].Id;
		    if (aggrNamep)
			strcpy(aggrNamep, (caddr_t)aggrListp[thisIx].name);
		}
	    }
	}
	if (aggrListp)
	    free(aggrListp);
	if (thisIx >= 0) {
	    lock_ReleaseWrite(&agnaLock);
	    return 1;
	}
    }
    if (code) 
	PrintError(0, code);
    else {
	if (numeric) {
	    dce_svc_printf(FTS_S_MISC_AGGREGATE_ID_NOT_EXPORTED_MSG, *aggrIdp);
	}
	else {
	    dce_svc_printf(FTS_S_MISC_AGGREGATE_NAME_NOT_EXPORTED_MSG, aggrnamep);
	}
	code = ENOENT;
    }
    if (numeric)
	addAgCache(servAddrp, *aggrIdp, "", "", code);
    else
	addAgCache(servAddrp, 0, aggrnamep, "", code);
    lock_ReleaseWrite(&agnaLock);
    /* Even failed validation is OK if a number was given and validateFlag agrees */
    return ((validateFlag == GETAGGR_NUMERICOK && numeric) ? 1 : 0);
}

/* Always return some kind of name for the given aggregate. */
char *getAggrName(sap, agid, quietp)
struct sockaddr *sap;
unsigned long agid;
int quietp;
{
    static char agName[24];
    unsigned long realAgId;
    static char realAgName[NUMSAVEDAGNAME + 15];
    int code;

    sprintf(agName, "%lu", agid);
    realAgName[0] = '\0';
    code = GetAggr(agName, sap, (quietp ? 0 : 1), &realAgId, &realAgName[0]);
    if (realAgName[0] == '\0')
	return agName;
    else
	return realAgName;
}

PrintSiteInfo(descrp, nam)
struct siteDesc *descrp;
char *nam;
{
    register int ix;
    char *nump;
    char ownerbuf[100];
    long code;

    if (nam) dce_svc_printf(FTS_S_MISC_SITE_DESCRIPTION_MSG, nam);
    for (ix = 0; ix < ADDRSINSITE; ++ix) {
	if (descrp->Addr[ix].type == (unsigned short) -1) break;
	dce_printf(fts_s_misc_sock_addr, 
		   MapSockAddr(&descrp->Addr[ix]),
		   NumericMap(&descrp->Addr[ix]));
    }
    dce_printf(fts_s_misc_fldb_quota, descrp->CreationQuota, descrp->CreationUses, &descrp->KerbPrin[0]);
    code = fts_UuidToGroup(&descrp->Owner, &ownerbuf[0]);
    /* if we can't lookup up the group name, try to print the uuid */
    if (code)
	code = fts_uuidToStr(&descrp->Owner, &ownerbuf[0], sizeof(ownerbuf));
    if (code) sprintf(ownerbuf, "<<%s>>", dfs_dceErrTxt(code));
    dce_printf(fts_s_misc_owner, ownerbuf);
}

PrintRepStatus(rsp, servAddrp)
struct repStatus *rsp;
struct afsNetAddr *servAddrp;
{
    u_long nowSecs;
    long bogusSignedTime;

    nowSecs = osi_Time();
    dce_printf(fts_s_misc_cell, rsp->volName, AFS_HGETBOTH(rsp->CellId),
	    AFS_HGETBOTH(rsp->srcVolId),
	    getAggrName(&rsp->srcAddr, rsp->srcAggr, 0));
    dce_printf(fts_s_misc_sockaddr, MapSockAddr(&rsp->srcAddr));
    dce_printf(fts_s_misc_greater_than);
    dce_printf(fts_s_misc_sockaddr, MapSockAddr(servAddrp));
    dce_printf(fts_s_misc_uu, AFS_HGETBOTH(rsp->volId),
	    getAggrName(servAddrp, rsp->curAggr, 0));
    bogusSignedTime = (long) rsp->lastKASweep.sec;
    dce_printf(fts_s_misc_flags_volstates, rsp->flags, rsp->volStates,
	    rsp->numKAs, ctime(&bogusSignedTime));
    dce_printf(fts_s_misc_wvt_id, AFS_HGETBOTH(rsp->srcVV),
	       AFS_HGETBOTH(rsp->curVV), AFS_HGETBOTH(rsp->WVT_ID));
    dce_printf(fts_s_misc_lost_token, nowSecs - rsp->tokenLossTime.sec,
	    rsp->tokenExpireTime.sec - nowSecs,
	    nowSecs - rsp->lastReplicaPublish.sec);
    dce_printf(fts_s_misc_vvcurr, rsp->vvCurr.sec, rsp->vvCurr.usec, nowSecs - rsp->vvCurr.sec,
	    rsp->vvPingCurr.sec, rsp->vvPingCurr.usec, nowSecs - rsp->vvPingCurr.sec);
    dce_printf(fts_s_misc_last_update_attempt, rsp->whenTried.sec, rsp->whenTried.usec,
	    nowSecs - rsp->whenTried.sec,
	    rsp->whenToTryAgain.sec, rsp->whenToTryAgain.usec,
	    rsp->whenToTryAgain.sec - nowSecs);
    dce_printf(fts_s_misc_status_msg, (rsp->RepMsg[0] == '\0' ? "<<empty>>" : (char *)rsp->RepMsg));
    return 0;
}

static void display_nums(what, trackp)
char *what;
register struct repNumTrack *trackp;
{
    dce_svc_printf(FTS_S_MISC_DONE_OVERSIZE_MSG, what, trackp->Count,
		   trackp->OverCount, trackp->SizeOverCount,
		   AFS_HGETBOTH(trackp->SizeOverCountSq));
    if (trackp->OverCount > 0) {
	dce_svc_printf(FTS_S_MISC_OVERSIZE_AVG_MSG, trackp->SizeOverCount / trackp->OverCount);
    }
    /* someday can calculate approx std dev with 64-bit quantity SizeOverCountSq. */
}

PrintRepServerStatus(ssp, sname, addrp)
repserverStatus *ssp;
char *sname;
afsNetAddr *addrp;
{
    unsigned long nowSecs;
    unsigned long someTime;
    long bogusSignedTime;

    nowSecs = osi_Time();
    bogusSignedTime = nowSecs;
    dce_printf(fts_s_misc_status_of_rep_server, sname, MapSockAddr(addrp), ctime(&bogusSignedTime));
    dce_printf(fts_s_misc_replicas_managed, ssp->numReplicas, ssp->numHosts);
    dce_printf(fts_s_misc_ids_allocated, ssp->numAllocVIDs, ssp->numUsedVIDs, ssp->numReusedVIDs);
    bogusSignedTime = ssp->nextForceKA;
    dce_printf(fts_s_misc_next_forced, ssp->nextForceKA - nowSecs, ctime(&bogusSignedTime));
    display_nums("Primary comm blocks", &ssp->Attns);
    display_nums("Primary keep-alive blocks", &ssp->KAs);
    return 0;
}


fts_GetAggrType(servAddrp, aggrId, typep, quietp)
struct sockaddr *servAddrp;
unsigned long aggrId;
unsigned long *typep;
int quietp;
{/* Get the type of the aggregate. */
    struct ftserver_aggrInfo aggInfo;
    long code;

    code = volc_Call(vols_AggregateInfo, servAddrp, NULL, aggrId,
		      &aggInfo, quietp);
    *typep = aggInfo.type;
    return code;
}

fts_NeedAggrType(servAddrp, aggrId, typeOkMask, errtext)
struct sockaddr *servAddrp;
unsigned long aggrId;
unsigned long typeOkMask;
char *errtext;
{/* Determine the type of the aggregate and complain if it's the wrong type. */
    unsigned long agtype, agmask;
    register long code, c2;
    static struct typename {
	long typemask; char *typen;
    } agTypeNames[] = {
	{(1 << AG_TYPE_EPI), "LFS"},
	{(1 << AG_TYPE_VXFS), "VXFS"},
	{(1 << AG_TYPE_DMEPI), "DMEPI"},
#if 0
	{(1 << AG_TYPE_UFS), "UFS"},
	{(1 << AG_TYPE_AIX3), "AIX3"}
#else /* 0 */
	{(1 << AG_TYPE_UFS), "non-LFS"},
#endif /* 0 */
    };
    char thistypeBuf[20];
    char neededTypeBuf[200];
    char xbuf[20];

    if (typeOkMask == 0) return -1;
    code = fts_GetAggrType(servAddrp, aggrId, &agtype, 1);
    if (code) {
	dce_svc_printf(FTS_S_MISC_NOT_AGGREGATE_TYPE_MSG,aggrId);
	return code;
    }
    if (agtype < (8 * sizeof(agtype))) {
	agmask = (1 << agtype);
	if (agmask & typeOkMask) return 0;
	for (code = (sizeof(agTypeNames)/sizeof(agTypeNames[0])) - 1; code >= 0; --code) {
	    if (agmask == agTypeNames[code].typemask) {
		strcpy(thistypeBuf, agTypeNames[code].typen);
		break;
	    }
	}
    } else code = -1;
    if (code < 0) sprintf(thistypeBuf, "type %#lx", agtype);
    neededTypeBuf[0] = '\0';
    for (agmask = 1, c2 = 0; c2 < 8*sizeof(agmask); agmask <<= 1, ++c2) {
	if (typeOkMask & agmask) {
	    if (neededTypeBuf[0] != '\0') strcat(neededTypeBuf, " or ");
	    for (code = (sizeof(agTypeNames)/sizeof(agTypeNames[0])) - 1; code >= 0; --code) {
		if (agmask == agTypeNames[code].typemask) {
		    strcat(neededTypeBuf, agTypeNames[code].typen);
		    break;
		}
	    }
	    if (code < 0) {
		sprintf(xbuf, "type-%#lx", c2);
		strcat(neededTypeBuf, xbuf);
	    }
	}
    }
    dce_svc_printf(FTS_S_MISC_NO_ON_THIS_ONE_MSG, errtext, neededTypeBuf, thistypeBuf);
    return -1;
}


