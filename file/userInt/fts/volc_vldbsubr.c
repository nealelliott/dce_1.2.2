/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: volc_vldbsubr.c,v $
 * Revision 1.1.118.1  1996/10/02  21:12:50  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:51  damon]
 *
 * $EndLog$
*/
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/common_data.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/stds.h>
#include <ctype.h>
#include <dcedfs/basicincludes.h>
#include <dcedfs/volume.h>
#include <dcedfs/aggr.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/rep_data.h>
#include <dcedfs/osi_dfserrors.h>
#include <dcedfs/compat.h>

#include <dcedfs/fldb_data.h>
#include <dcedfs/flserver.h>
#include <dcedfs/ftserver_data.h>
#include <dcedfs/ftserver.h>

#include <dce/sec_login.h>
#include <dce/pgo.h>
#include <dce/binding.h>

#include <volc.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftimac.h>
#include <dfsftisvc.h>
#include <dfsftimsg.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/userInt/fts/volc_vldbsubr.c,v 1.1.118.1 1996/10/02 21:12:50 damon Exp $")

/*
 * Useful mappings for standard {RWVOL, ROVOL, BACKVOL} voltypes
 */
char *volTypes[MAXTYPES] = {"readWrite", "readOnly", "backup"};
unsigned long vldbFlags[MAXTYPES] = {VLF_RWEXISTS, VLF_ROEXISTS, VLF_BACKEXISTS};

int volc_noEnumOnVldbEntryCreate = 0;

/*
 * This file includes miscellaneous routines that deal with VLDB entries
 */

/* Return the index of the local replica for release replication, if any. */
long int fts_releaseRepSite(entryp)
register vldbentry *entryp;
{
    register int ix, ix2, rwix;
    int xl[MAXTYPES];
    unsigned long rwflag, roflag;

    /* has to be replicated: */
    if ((entryp->flags & VLF_LAZYREP) == 0) return -1;
    /* has to be release-replicated: */
    if ((entryp->flags & VLF_LAZYRELE) == 0) return -1;
    fts_GetIndices(entryp, xl);
    rwflag = ((unsigned32)VLSF_ZEROIXHERE) >> xl[RWVOL];
    roflag = ((unsigned32)VLSF_ZEROIXHERE) >> xl[ROVOL];
    /* First, look for the RW site; maybe it shares the entry with the R/O. */
    rwix = -1;
    for (ix = 0; ix < entryp->nServers; ++ix) {
	if ((entryp->siteFlags[ix] & (VLSF_SAMEASPREV | VLSF_DEFINED))
	    != (VLSF_DEFINED))
	    continue;
	if (entryp->siteFlags[ix] & rwflag) {
	    if (entryp->siteFlags[ix] & roflag)
		return ix;	/* easy--they share an entry. */
	    if (rwix < 0) rwix = ix;
	}
    }
    if (rwix < 0) return -2;	/* gotta have an R/W site! */
    /* Next, check for a R/O entry on the same server and aggregate. */
    for (ix = 0; ix < entryp->nServers; ++ix) {
	if ((entryp->siteFlags[ix] & (VLSF_SAMEASPREV | VLSF_DEFINED))
	    != (VLSF_DEFINED))
	    continue;
	if (entryp->siteFlags[ix] & roflag) {
	    if (entryp->sitePartition[ix] == entryp->sitePartition[rwix]
		&& bcmp((char *)&entryp->siteAddr[ix], (char *)&entryp->siteAddr[rwix],
			sizeof(afsNetAddr)) == 0)
		return ix;
	}
    }
    /* Last, check for a R/O entry on the same server. */
    for (ix = 0; ix < entryp->nServers; ++ix) {
	if ((entryp->siteFlags[ix] & (VLSF_SAMEASPREV | VLSF_DEFINED))
	    != (VLSF_DEFINED))
	    continue;
	if (entryp->siteFlags[ix] & roflag) {
	    if (bcmp((char *)&entryp->siteAddr[ix], (char *)&entryp->siteAddr[rwix],
		     sizeof(afsNetAddr)) == 0)
		return ix;
	}
    }
    /* There's no R/O staging site for release replication! */
    return -3;
}

static vldbentry oldEntry;

void vldb_SaveOldEntry(entryp)
vldbentry *entryp;
{
    if (entryp == NULL) {
	bzero((char *)&oldEntry, sizeof(oldEntry));
    } else {
	oldEntry = *entryp;
    }
}

/*
 * Get some replication servers to know about any changes in the set of
 * repl params.
 */
long vldb_TellRepAboutNewEntry(entryp, flags, init)
register vldbentry *entryp;
long flags;
int init;
{
    afsNetAddr whomToTell[MAXNSERVERS * 2];
    u_char *princWhomToTell[MAXNSERVERS * 2];
    register int numWhomToTell, numWhatToTell, ix1, ix2;
    int oldStageIx, newStageIx;
    afsNetAddrs howToTell;
    struct afsNetAddr *whatToTell;
    struct sockaddr_in *sinp;
    afs_hyper_t *roVidP, *rwVidP;
    long code;
    rpc_binding_handle_t serverconn;
    extern long REP_AllCheckReplicationConfig();
    u_long oldROFlag, newROFlag;
    u_long newRWFlag;
    extern afs_hyper_t remoteCellID;
    u_long spare4, spare5;
    unsigned long st;
    int tryAgain, timeoutVal, retries;

    fts_InitDecoder();

    /* defense: don't mis-fire */
    if (!AFS_hsame(oldEntry.VolIDs[0], entryp->VolIDs[0]))
	 bzero((char *)&oldEntry, sizeof(oldEntry));

    roVidP = rwVidP = NULL;
    newROFlag = oldROFlag = newRWFlag = 0;
    for (ix1 = 0; ix1 < MAXVOLTYPES && !AFS_hiszero(entryp->VolIDs[ix1]); ++ix1) {
	if (entryp->VolTypes[ix1] == VOLTIX_TO_VOLTYPE(ROVOL)) {
	    newROFlag = ((unsigned32)VLSF_ZEROIXHERE) >> ix1;
	    roVidP = &entryp->VolIDs[ix1];
	} else
	if (entryp->VolTypes[ix1] == VOLTIX_TO_VOLTYPE(RWVOL)) {
	    newRWFlag = ((unsigned32)VLSF_ZEROIXHERE) >> ix1;
	    rwVidP = &entryp->VolIDs[ix1];
	}
    }
    for (ix1 = 0; ix1 < MAXVOLTYPES && !AFS_hiszero(oldEntry.VolIDs[ix1]); ++ix1) {
	if (oldEntry.VolTypes[ix1] == VOLTIX_TO_VOLTYPE(ROVOL)) {
	    oldROFlag = ((unsigned32)VLSF_ZEROIXHERE) >> ix1;
	    if (roVidP == NULL) roVidP = &oldEntry.VolIDs[ix1];
	    break;
	}
    }

    /* don't do anything if neither old nor new is repserver-replicated */
    if ((oldEntry.flags & VLF_LAZYREP) == 0 &&
	 (entryp->flags & VLF_LAZYREP) == 0) {
	bzero((char *)&oldEntry, sizeof(oldEntry));
	return 0;
    }
    if (roVidP == NULL) {
	dce_svc_printf(FTS_S_SUBR_NO_READONLY_FILESET_MSG);
	return -1;
    }
    whatToTell = &howToTell.afsNetAddrs_val[0];
    if (oldROFlag == 0) {
	/* only one set of servers */
	newStageIx = fts_releaseRepSite(entryp);
	ix2 = 0;
	for (ix1 = 0; ix1 < entryp->nServers; ++ix1) {
	    if (ix1 == newStageIx) continue;
	    if (entryp->siteFlags[ix1] & VLSF_SAMEASPREV) continue;
	    if (entryp->siteFlags[ix1] & newROFlag) {
		whomToTell[ix2] = entryp->siteAddr[ix1];
		princWhomToTell[ix2] = (u_char*)&entryp->sitePrincipal[ix1].text[0];
		whatToTell[ix2] = entryp->siteAddr[ix1];
		++ix2;
	    }
	}
	numWhomToTell = numWhatToTell = ix2;
    } else {
	/* compare the sets of servers. */
	oldStageIx = fts_releaseRepSite(&oldEntry);
	newStageIx = fts_releaseRepSite(entryp);
	numWhomToTell = numWhatToTell = 0;
	/* put the changed ones in the targets first, since they're more crucial */
	/* First, the new ones. */
	for (ix1 = 0; ix1 < entryp->nServers; ++ix1) {
	    if (ix1 == newStageIx) continue;
	    if (entryp->siteFlags[ix1] & VLSF_SAMEASPREV) continue;
	    if ((entryp->siteFlags[ix1] & newROFlag) == 0) continue;
	    for (ix2 = 0; ix2 < oldEntry.nServers; ++ix2) {
		if (ix2 == oldStageIx) continue;
		if (oldEntry.siteFlags[ix2] & VLSF_SAMEASPREV) continue;
		if ((oldEntry.siteFlags[ix2] & oldROFlag) == 0) continue;
		if (bcmp((char *)&entryp->siteAddr[ix1], (char *)&oldEntry.siteAddr[ix2],
			 sizeof(afsNetAddr)) == 0) break;
	    }
	    if (ix2 >= oldEntry.nServers) {/* a new one */
		whomToTell[numWhomToTell] = entryp->siteAddr[ix1];
		princWhomToTell[numWhomToTell] =
		  (u_char*)&entryp->sitePrincipal[ix1].text[0];
		++numWhomToTell;
		if (numWhatToTell < REP_ADDRBULKMAX) {
		    whatToTell[numWhatToTell] = entryp->siteAddr[ix1];
		    ++numWhatToTell;
		}
	    }
	}
	/* Next, the deleted ones. */
	for (ix1 = 0; ix1 < oldEntry.nServers; ++ix1) {
	    if (ix1 == oldStageIx) continue;
	    if (oldEntry.siteFlags[ix1] & VLSF_SAMEASPREV) continue;
	    if ((oldEntry.siteFlags[ix1] & oldROFlag) == 0) continue;
	    for (ix2 = 0; ix2 < entryp->nServers; ++ix2) {
		if (ix2 == newStageIx) continue;
		if (entryp->siteFlags[ix2] & VLSF_SAMEASPREV) continue;
		if ((entryp->siteFlags[ix2] & newROFlag) == 0) continue;
		if (bcmp((char *)&oldEntry.siteAddr[ix1], (char *)&entryp->siteAddr[ix2],
			 sizeof(afsNetAddr)) == 0) break;
	    }
	    if (ix2 >= entryp->nServers) {/* a deleted one */
		whomToTell[numWhomToTell] = oldEntry.siteAddr[ix1];
		princWhomToTell[numWhomToTell] =
		  (u_char*)&oldEntry.sitePrincipal[ix1].text[0];
		++numWhomToTell;
		if (numWhatToTell < REP_ADDRBULKMAX) {
		    whatToTell[numWhatToTell] = oldEntry.siteAddr[ix1];
		    ++numWhatToTell;
		}
	    }
	}
	/* Next, the ones that were in both sets.  Either list will have them. */
	for (ix1 = 0; ix1 < oldEntry.nServers; ++ix1) {
	    if (ix1 == oldStageIx) continue;
	    if (oldEntry.siteFlags[ix1] & VLSF_SAMEASPREV) continue;
	    if ((oldEntry.siteFlags[ix1] & oldROFlag) == 0) continue;
	    for (ix2 = 0; ix2 < numWhomToTell; ++ix2) {
		if (bcmp((char *)&oldEntry.siteAddr[ix1], (char *)&whomToTell[ix2],
			 sizeof(afsNetAddr)) == 0) break;
	    }
	    if (ix2 >= numWhomToTell) {/* a constant one */
		whomToTell[numWhomToTell] = oldEntry.siteAddr[ix1];
		princWhomToTell[numWhomToTell] =
		  (u_char*)&oldEntry.sitePrincipal[ix1].text[0];
		++numWhomToTell;
		if (numWhatToTell < REP_ADDRBULKMAX) {
		    whatToTell[numWhatToTell] = oldEntry.siteAddr[ix1];
		    ++numWhatToTell;
		}
	    }
	}
    }
    /* Great: the message to send is in whatToTell, and all we have to
      do is to get it to any one of the guys in whomToTell. */
    if (numWhomToTell <= 0 || numWhatToTell <= 0) {
	if (init == 0) {
	    VOLSERLOG(1, "No RO sites for fileset %lu,,%lu!\n",
		      AFS_HGETBOTH(*roVidP));
	}
	return 0;
    }
    howToTell.afsNetAddrs_len = numWhatToTell;
    (void) vldb_GetRemoteCellID();
    code = -1;	/* for now, will fall through if no AF_INET hosts */
    tryAgain=retries=0;
    timeoutVal=150;
    do {
        if (tryAgain){
           dce_svc_printf(FTS_S_SUBR_RETRYING_CALL_MSG);
           timeoutVal += 30;
           tryAgain=0;
           /* Give up after 3 tries */
           if (retries > 3 ){
              dce_svc_printf(FTS_S_SUBR_FAILED_TO_NOTIFY_REPSERVER_MSG);
              dce_svc_printf(FTS_S_SUBR_MAYBE_RESTART_MSG);
              break;
           }
        }
    for (ix1 = 0; ix1 < numWhomToTell; ++ix1) {
	if (whomToTell[ix1].type == AF_INET) {
	    sinp = (struct sockaddr_in *) &whomToTell[ix1];
	    serverconn = connToServer((struct sockaddr *) sinp,
				      (princWhomToTell[ix1][0])? princWhomToTell[ix1] : NULL,
				      SERVERKIND_REP);
	    if (serverconn != NULL) {
		(void) fts_UseThisTimeout(serverconn, rpc_c_binding_default_timeout);
		rpc_mgmt_set_call_timeout(serverconn, timeoutVal /* sec */, &st);
		if (st != error_status_ok) {
		    dce_svc_printf(FTS_S_SUBR_CANT_SET_TIMEOUT_MSG, MapSockAddr(sinp), dfs_dceErrTxt(st));
		    code = st;
		} else {
		    VOLSERLOG(1, "Calling REP_AllCheckReplicationConfig(%s, {%lu,,%lu}, {%lu,,%lu}, %d, flg=%d...)...\n",
			      MapSockAddr(sinp), AFS_HGETBOTH(remoteCellID),
			      AFS_HGETBOTH(*roVidP), numWhatToTell, flags);
		    code = REP_AllCheckReplicationConfig(serverconn, &remoteCellID,
							 roVidP, numWhatToTell, &howToTell, flags, 0, 0, 0,
							 &spare4, &spare5);
		    code = osi_errDecode(code);
		    if (code != 0){
			dce_svc_printf(FTS_S_SUBR_REPSERVER_NO_TALK_MSG, MapSockAddr(sinp), dfs_dceErrTxt(code));
                        if ((code == rpc_s_call_timeout) || (code == rpc_s_comm_failure))
                            tryAgain=1;
                            retries++;
		      }
		}
		/* rpc_binding_free(&serverconn, (unsigned32 *)&st); */
		if (code == 0) break;
	    }
	}
      }
  } while (code && tryAgain);
    return code;
}

char *fts_ivToString(intrv)
unsigned long intrv;
{/* Return a pointer to a static string describing the given interval */
    static char res[100];
    char nbuf[12];
    register unsigned long n, h, m, s;

    res[0] = '\0';
    if (intrv >= (60*60*24)) {
	n = intrv / (24*60*60);
	intrv -= (n * (24*60*60));
	sprintf(nbuf, "%lu", n);
	strcat(res, nbuf);
	strcat(res, "d");
    }
    h = intrv / (60*60);
    intrv -= (h * (60*60));
    m = intrv / 60;
    intrv -= (m * 60);
    s = intrv;
    sprintf(nbuf, "%d:%02d:%02d", h, m, s);
    strcat(res, nbuf);
    return (&res[0]);
}

fts_strToIv(strp, ivp)
register char *strp;
unsigned long *ivp;
{/* Parse a string looking for an interval. */
/* Accept a simple grammar:
  [digits"d"]{[[digits"h"]digits"m"]digits["s"] | [[digits":"]digits":"]digits}
  that is, "3d" or "2:00:00" or "3d12h" or "3600" are all OK.
  The somewhat-ambiguous "4:00" is interpreted as mm:ss, not hh:mm.
*/
    unsigned long h, m, s;
    register unsigned long d, x;
    register int err;
    char *oldstr = strp;

    d = h = m = s = 0;
    for (err = 0; err == 0; ) {
	while (*strp && isspace(*strp)) ++strp;
	if (*strp == '\0') break;
	if (!isdigit(*strp)) {err = 1; break;}
	x = 0;
	while (*strp && isdigit(*strp)) {
	    x *= 10;
	    x += (*strp - '0');
	    ++strp;
	}
	while (*strp && isspace(*strp)) ++strp;
	switch (*strp) {
	    case 'D': case 'd':
		if (d != 0) err = 1;
		d = x; break;
	    case 'H': case 'h':
		if (h != 0) err = 1;
		h = x; break;
	    case 'M': case 'm':
		if (m != 0) err = 1;
		m = x; break;
	    case 'S': case 's':
		if (s != 0) err = 1;
		s = x; break;
	    case ':': case '\0':
		if (h != 0) err = 1;
		h = m; m = s; s = x;
		break;
	}
	if (err) break;
	if (*strp == '\0') break;
	++strp;
    }
    if (err) {
	dce_svc_printf(FTS_S_SUBR_INVALID_TIME_INTERVAL_MSG, oldstr);
	return -1;
    }
    d *= 24; d += h;
    d *= 60; d += m;
    d *= 60; d += s;
    *ivp = d;
    return 0;
}

fts_uuidToStr(uuidp, strp, strplen)
afsUUID *uuidp;
char strp[];
int strplen;
{
    register unsigned char *x = (unsigned char *) uuidp;
    register int ctr;
    static char ubuf[2*sizeof(afsUUID)+1];
    register char *p;
    char *uuidDesc;
    static char hexAlphabet[17] = "0123456789abcdef";
    unsigned long st;

    /* XXX this could talk to passwdEtc to translate names to groups */

    for (ctr = sizeof(afsUUID); ctr > 0; --ctr) {
	if (*x++ != 0) break;
    }
    if (ctr != 0) {
	if (uuid_is_nil(uuidp, &st) == true && st == uuid_s_ok)
	    ctr = 0;
    }
    if (ctr == 0) {
	strncpy(strp, "<nil>", strplen);
	return 0;
    } else {
	uuid_to_string(uuidp, (unsigned char **)&uuidDesc, &st);
	if (st == uuid_s_ok) {
	    strncpy(strp, uuidDesc, strplen);
	    rpc_string_free((unsigned char **)&uuidDesc, &st);
	    return 0;
	}
	p = ubuf;
	x = (unsigned char *) uuidp;
	for (ctr = sizeof(afsUUID); ctr > 0; --ctr) {
	    *p++ = hexAlphabet[(*x >> 4) & 0xf];
	    *p++ = hexAlphabet[*x& 0xf];
	    ++x;
	}
	*p = '\0';
	strncpy(strp, &ubuf[0], strplen);
	return 0;
    }
}

fts_strToUuid(strp, uuidp)
char *strp;
afsUUID *uuidp;
{/* convert string to a UUID */
    register char *x = (char *) uuidp;
    register char y;
    register int res, state;
    register unsigned char buf;
    unsigned long st;

    /* XXX this could talk to passwdEtc to translate a group uuid to a group name */

    bzero((char *)uuidp, sizeof(afsUUID));
    if (strp && strcmp(strp, "0"))	/* all-zero ``UUID'' ok here */
	return 0;
    uuid_from_string((unsigned char *)strp, uuidp, &st);
    if (st == uuid_s_ok)	 /* let that translator work */
	return 0;
    bzero((char *)uuidp, sizeof(afsUUID));
    state = 0; buf = 0;	
    for (; *strp != '\0'; ++strp) {
	if (isascii(*strp)) {
	    y = *strp;
	    if (isdigit(y) || isalpha(y)) {
		if (isdigit(y)) {
		    res = y - '0';
		} else {
		    if (isupper(y)) y = tolower(y);
		    if (y >= 'a' && y <= 'f') {
			res = y - 'a' + 0xa;
		    } else res = -1;
		}
		if (res >= 0) {
		    buf <<= 4;
		    buf |= y;
		    ++state;
		    if (state == 2) {
			*x++ = buf;
			buf = 0; state = 0;
		    }
		}
	    }
	}
    }
    return 0;
}

/*
 * Returns the index in the replication list of a volume: the search is by the
 * replicated site's address, its aggregate, and its flag information
 * (wildcarded too).  Return the first index in a sequence of addresses.
 */
vldb_FindRepSite(entryp, servAddrp, aggrId, flag)
    register vldbentry *entryp;
    long aggrId, flag;
    struct sockaddr *servAddrp;
{
    register int i, ixToReturn;

    ixToReturn = 0;
    for (i = 0; i < entryp->nServers; i++) {
	if (!(entryp->siteFlags[i] & VLSF_SAMEASPREV))
	    ixToReturn = i;
	if ((!servAddrp || !bcmp(&entryp->siteAddr[i], servAddrp,
				 sizeof(*servAddrp))) &&
	    ((aggrId == -1) || (entryp->sitePartition[i] == aggrId)) &&
	    ((flag == -1) || (entryp->siteFlags[i] & flag))) {
	    return ixToReturn;
	}
    }
    return -1;
}


/* Checks whether the given site (server+aggregate) is in the given set of
 * address duplicates at index Ix in the given vldbentry.
 */
vldb_IsSiteHere(entryp, Ix, servAddrp, aggrId)
    register vldbentry *entryp;
    int Ix;
    long aggrId;
    struct sockaddr *servAddrp;
{
    register int i;

    for (i = Ix; i < entryp->nServers; ++i) {
	if (i != Ix && !(entryp->siteFlags[i] & VLSF_SAMEASPREV))
	    break;	/* didn't find it before hitting next site */
	if (((aggrId == -1) || (aggrId == entryp->sitePartition[i])) &&
	    (!servAddrp || !bcmp(&entryp->siteAddr[i], servAddrp,
				 sizeof(*servAddrp)))) {
	    /* that was a good enough match! */
	    return 1;
	}
    }
    return 0;
}


static void EliminateOldExtras(entryp, startIx, ckStart)
    register struct vldbentry *entryp;
    int startIx;
    int ckStart;
{
    register int i, j;

    i = startIx;
    for (j = i; j < entryp->nServers; ++j) {
	if ((ckStart || (j != i))
	    && (entryp->siteFlags[j] & VLSF_SAMEASPREV) == 0) break;
    }
    if (j == i)
	return;	/* no extra addr entries--nothing to do */
    /* Need to push j down to i */
    /* either j >= nServers or j's SAMEASPREV is off */
    for (; j < entryp->nServers; ++i, ++j) {
	entryp->siteAddr[i] = entryp->siteAddr[j];
	entryp->sitePartition[i] = entryp->sitePartition[j];
	entryp->siteFlags[i] = entryp->siteFlags[j];
	entryp->sitemaxReplicaLatency[i] = entryp->sitemaxReplicaLatency[j];
	entryp->sitePrincipal[i] = entryp->sitePrincipal[j];
	entryp->siteOwner[i] = entryp->siteOwner[j];
	entryp->siteObjID[i] = entryp->siteObjID[j];
    }
    /* Diminish the count of addr entries by what we just cut it by. */
    entryp->nServers -= (j - i);
    }

/*
 * Replaces the occurrence in the replication list of [oservAddrp, oaggrId,
 * flag] with [nservAddrp, naggrId]; as a special case. if nserverAddrp==0 and
 * naggrId==-1, the routine removes the entry instead. It returns 0 if the old
 * entry wasn't found, else 1.  Always removes the BACKUP entry, also!
 */
vldb_ReplaceRepSite(entryp, oservAddrp, oaggrId, nservAddrp, naggrId, flag)
    register vldbentry *entryp;
    struct sockaddr *oservAddrp, *nservAddrp;
    long  oaggrId, naggrId;
    long flag;
{
    register int i, j;
    int xl[MAXTYPES];

    if ((i = vldb_FindRepSite(entryp, oservAddrp, oaggrId, flag)) != -1) {
	if (!nservAddrp && (naggrId == -1)) {
	    /* shuffle the rest down to fill in the gap */
	    EliminateOldExtras(entryp, i, 0);
	} else {
	    fts_GetIndices(entryp, xl);
	    *((struct sockaddr *) &entryp->siteAddr[i]) = *nservAddrp;
	    entryp->sitePartition[i] = naggrId;
	    entryp->sitePrincipal[i].text[0] = '\0';	/* truncate the unknown */
	    EliminateOldExtras(entryp, i+1, 1);
	    if (xl[BACKVOL] >= 0)
		entryp->siteFlags[i] &=
		  ~(((unsigned32)VLSF_ZEROIXHERE) >> xl[BACKVOL]);
	    entryp->flags &= ~(VLF_BACKEXISTS);
	}
	return 1;
    }
    return 0;
}


/*
 * Assigns a replication entry [servAddrp, aggrId, (siteFlag)] to the existing
 * list.  If index == -1, then it's added to the list (nServers is bumped),
 * else overrides the index-th entry. Also note that the site's flags are only
 * set if siteFlag is meaningful (i.e. non -1).
 */
vldb_AssignSite(entryp, flag, siteFlag, servAddrp, aggrId, index, maxreplat)
    register vldbentry *entryp;
    long flag, siteFlag, aggrId;
    register long index;
    u_long maxreplat;
    struct sockaddr *servAddrp;
{
    register long newIx = index;

    entryp->flags |= flag;
    if (newIx == -1) {
	newIx = entryp->nServers;
	if (newIx == MAXNSERVERS) {
	    /* compress out any duplicates */
	    for (newIx = MAXNSERVERS-1; newIx >= 0; --newIx) {
		if (entryp->siteFlags[newIx] & VLSF_SAMEASPREV) {
		    entryp->siteFlags[newIx] = 0;
		    break;	/* overwrite this index */
		}
	    }
	    if (newIx < 0) {
		entryp->nServers += 1;	/* will cause RPC call to fail...should just make local call fail */
		return;
	    }
	} else {
	    entryp->nServers += 1;
	}
	if (siteFlag != -1)
	    entryp->siteFlags[newIx] = siteFlag | VLSF_DEFINED;
	*((struct sockaddr *) &entryp->siteAddr[newIx]) = *servAddrp;
	entryp->sitePartition[newIx] = aggrId;
	if (flag == VLF_ROEXISTS && maxreplat != 0)
	    entryp->sitemaxReplicaLatency[newIx] = maxreplat;
	entryp->sitePrincipal[newIx].text[0] = '\0';	/* truncate the unknown */
    } else {
	if (siteFlag != -1)
	    entryp->siteFlags[newIx] |= siteFlag | VLSF_DEFINED;
	*((struct sockaddr *) &entryp->siteAddr[newIx]) = *servAddrp;
	entryp->sitePartition[newIx] = aggrId;
	if (flag == VLF_ROEXISTS && maxreplat != 0)
	    entryp->sitemaxReplicaLatency[newIx] = maxreplat;
	entryp->sitePrincipal[newIx].text[0] = '\0';	/* truncate the unknown */
	EliminateOldExtras(entryp, newIx+1, 1);
    }
}


/*
 * Assigns a site entry [servAddrp, aggrId, (siteFlag), (maxreplat)] to the
 * existing list.  If index == -1, we try to do it by adding to an existing
 * (servAddrp, aggrId) pair, but if that fails, it's added to the list
 * (nServers is bumped).  If an index is given (>= 0), we add the given data
 * to the index-th entry.  Also note that the site's flags are only set if
 * siteFlag is meaningful (i.e. non -1).
 */
vldb_EnsureSite(entryp, flag, siteFlag, servAddrp, aggrId, index, maxreplat)
    register vldbentry *entryp;
    long flag, siteFlag, aggrId;
    register long index;
    u_long maxreplat;
    struct sockaddr *servAddrp;
{
    register long newIx = index;

    if (newIx == -1) {
	/* Try to find an index that we can use. */
	/* The siteFlag value is purposely made a wild-card match! */
	newIx = vldb_FindRepSite(entryp, servAddrp, aggrId, -1);
    }
    vldb_AssignSite(entryp, flag, siteFlag, servAddrp, aggrId, newIx, maxreplat);
}


/*
 * Convert a volume type from its internal to its external form
 */
vldb_TypeToIndex(volType)
    long volType;
{
    if (volType & 0xffff)
	return -1;
    volType = ((volType >> 16) & 0xffff);
    if (volType < MAXTYPES)
	return volType;
    return -1;
}


/*
 * Setup/Create the vldb entry for the specified volume 
 */
vldb_CreateVldbEntry(volNamep, servAddrp, aggrId, flags, volIdp, roIdp, backIdp, special, quietp)
    char *volNamep;
    struct sockaddr *servAddrp;
    long aggrId, flags;
    afs_hyper_t *volIdp, *roIdp, *backIdp;
    long special;
    int quietp;
{
    vldbentry entry, altEntry;
    long code;
    unsigned long ROflag, RWflag, BACKflag, FLAG;
    int typeIx;
    int siteIx;

    VOLSERLOG(1, "------- Creating a new FLDB entry for %s ------- \n", volNamep);
    bzero((char *)&entry, sizeof(entry));
    strncpy((char *)entry.name, volNamep, FTSERVER_MAXFSNAME);
    entry.volumeType = VOLTIX_TO_VOLTYPE(RWVOL);
    FLAG = (unsigned32)VLSF_ZEROIXHERE;
    RWflag = ROflag = BACKflag = 0;
    typeIx = 0;
    if (special) {	/* from syncfldb to make up lots of attributes */
	if (!AFS_hiszero(*volIdp)) {
	    RWflag = FLAG;
	    FLAG >>= 1;
	    entry.VolTypes[typeIx] = VOLTIX_TO_VOLTYPE(RWVOL);
	    entry.VolIDs[typeIx] = *volIdp;
	    ++typeIx;
	}
	if (!AFS_hiszero(*roIdp)) {
	    ROflag = FLAG;
	    FLAG >>= 1;
	    entry.VolTypes[typeIx] = VOLTIX_TO_VOLTYPE(ROVOL);
	    entry.VolIDs[typeIx] = *roIdp;
	    ++typeIx;
	}
	if (!AFS_hiszero(*backIdp)) {
	    BACKflag = FLAG;
	    FLAG >>= 1;
	    entry.VolTypes[typeIx] = VOLTIX_TO_VOLTYPE(BACKVOL);
	    entry.VolIDs[typeIx] = *backIdp;
	    ++typeIx;
	}
	AFS_hzero(entry.VolIDs[typeIx]);	/* terminate the list */
	if (flags & VLF_RWEXISTS) {
	    vldb_EnsureSite(&entry, VLF_RWEXISTS, RWflag,
			    servAddrp, aggrId, -1, 0);
	}
	if (flags & VLF_ROEXISTS) {
	    vldb_EnsureSite(&entry, VLF_ROEXISTS, ROflag,
			    servAddrp, aggrId, -1, 0);
	}
	if (flags & VLF_BACKEXISTS) {
	    entry.flags |= VLF_BACKEXISTS;
#if 0
	    if (!(entry.flags & VLF_RWEXISTS) && RWflag != 0) {
		vldb_EnsureSite(&entry, VLF_RWEXISTS, RWflag,
				servAddrp, aggrId, -1, 0);
	    }
#endif /* 0 */
	    if (!AFS_hiszero(*backIdp)) {
		vldb_EnsureSite(&entry, VLF_BACKEXISTS, BACKflag,
				servAddrp, aggrId, -1, 0);
	    }
	}
    } else {
	if (RWflag == 0) {
	    RWflag = FLAG;
	    FLAG >>= 1;
	}
	entry.nServers = 0;
	entry.VolTypes[typeIx] = VOLTIX_TO_VOLTYPE(RWVOL);
	entry.VolIDs[typeIx] = *volIdp;
	++typeIx;
	vldb_AssignSite(&entry, flags, RWflag, servAddrp, aggrId, -1, 0);
	if (!AFS_hiszero(*roIdp)) {
	    entry.VolTypes[typeIx] = VOLTIX_TO_VOLTYPE(ROVOL);
	    entry.VolIDs[typeIx] = *roIdp;
	    ++typeIx;
	}
	if (!AFS_hiszero(*backIdp)) {
	    entry.VolTypes[typeIx] = VOLTIX_TO_VOLTYPE(BACKVOL);
	    entry.VolIDs[typeIx] = *backIdp;
	    ++typeIx;
	}
	AFS_hzero(entry.VolIDs[typeIx]);	/* terminate the list */
    }
    if (code = vldb_CreateEntry(&entry, volNamep))
	return code;
    VOLSERLOG(1, "Created the FLDB entry for fileset %s\n", volNamep);
    code = vldb_GetEntryByID(volIdp, RWVOL, &altEntry, 0);

    if (volc_noEnumOnVldbEntryCreate == 0) {
	if (!code)
	    vldb_EnumerateEntry(&altEntry, "", quietp);
	else
	    vldb_EnumerateEntry(&entry, "", quietp);
    }
    
    return 0;
}


/*
 * Call VLDB to replace the contents of an entry (create it if necessary)
 */
vldb_UpdateEntry(volIdp, volNamep, servAddrp, aggrId, statusp)
    afs_hyper_t *volIdp;
    char *volNamep;
    struct sockaddr *servAddrp;
    long aggrId;
    struct ftserver_status *statusp;
{
    register long code, tcode, i, j, RWix;
    vldbentry entry;

    code = vldb_GetEntryByID(volIdp, RWVOL, &entry, 1);
    if (code && code != VL_NOENT) {
	dce_svc_printf(FTS_S_SUBR_CANT_FETCH_ENTRY_MSG,
		       AFS_HGETBOTH(*volIdp), code);
    } else if (code == VL_NOENT) {
	code = vldb_CreateVldbEntry(volNamep, servAddrp, aggrId, VLF_RWEXISTS,
				    volIdp, &statusp->vsd.cloneId,
				    &statusp->vsd.backupId, 0, 0);
    } else {
	if (code = vldb_SetLock(volIdp, RWVOL, VLOP_RESTORE))
	    return code;
	strcpy((char *)entry.name, volNamep);
	RWix = -1;
	for (j = 0; j < MAXVOLTYPES && !AFS_hiszero(entry.VolIDs[j]); ++j) {
	    if (entry.VolTypes[j] == VOLTIX_TO_VOLTYPE(RWVOL)) {
		RWix = j;
		break;
	    }
	}
	if (RWix < 0) {
	    dce_svc_printf(FTS_S_SUBR_NO_RW_ID_MSG,
			   volNamep, AFS_HGETBOTH(*volIdp));
	    tcode = vldb_ReleaseLock(volIdp, RWVOL, LOCKREL_ALL);			   
	    return VL_NOENT;
	}
	i = vldb_FindRepSite(&entry, (struct sockaddr *)0,
					 -1, ((unsigned32)VLSF_ZEROIXHERE) >> RWix);
	if (i != -1) {
	    if (!vldb_IsSiteHere(&entry, i, servAddrp, aggrId)) {
		/*
		 * The volume is deleted on the old site if it's different
		 * from new 
		 */
		code = fts_DeleteVolume(0, entry.sitePartition[i], volIdp, 0,
				    (struct sockaddr *)&entry.siteAddr[i]);
		if (code != ENOENT && code != ENODEV && 
		    code != FTSERVER_ERR_NOVOL) {
		    tcode = vldb_ReleaseLock(volIdp, RWVOL, LOCKREL_ALL);
		    if (!code)
			code = tcode;
		    return code;
		}
		vldb_AssignSite(&entry, VLF_RWEXISTS,
				((unsigned32)VLSF_ZEROIXHERE) >> RWix,
				servAddrp, aggrId, i, 0);
	    }
	} else {
	    /* we need to establish a RW site now. */
	    vldb_AssignSite(&entry, VLF_RWEXISTS,
			    ((unsigned32)VLSF_ZEROIXHERE) >> RWix,
			    servAddrp, aggrId, -1, 0);
	}
	if (code = vldb_ReplaceEntry(&entry, volIdp, RWVOL, LOCKREL_ALL))
	    return code;
	if (verbose) vldb_EnumerateEntry(&entry, "", 0);	/* XXXX */
    }
    return code;
}


vldb_AddVolume(entryp, servAddrp, aggrId, volType, volIdp)
    vldbentry *entryp;
    register struct sockaddr *servAddrp;
    long aggrId, volType;
    afs_hyper_t *volIdp;
{
    register long code = 0;
    int xl[MAXTYPES];
    register int ix;
    register unsigned long RWmask, BUmask;

    fts_GetIndices(entryp, xl);

    if (volType == BACKVOL) {
	if (!(entryp->flags & VLF_BACKEXISTS)) {
	    entryp->flags |= VLF_BACKEXISTS;
	    if (xl[BACKVOL] < 0 || xl[RWVOL] < 0)
		return ESRCH;
	    RWmask = ((unsigned32)VLSF_ZEROIXHERE) >> xl[RWVOL];
	    BUmask = ((unsigned32)VLSF_ZEROIXHERE) >> xl[BACKVOL];
	    for (ix = 0; ix < entryp->nServers; ++ix) {
		/* flag backup on same aggr as r/w on all copies */
		if (entryp->siteFlags[ix] & RWmask) {
		    entryp->siteFlags[ix] |= BUmask;
		}
	    }
	    code = vldb_ReplaceEntry(entryp, volIdp, BACKVOL, 0);
	}
    } else
	return EINVAL;		/* XXX Just Backup is used for now! XXX */
    return code;
}


static void vldb_FixEntryFlags(entryp)
    vldbentry *entryp;
{
    int stillUsed[MAXTYPES];
    register int ix, jx;
    unsigned long masks[MAXTYPES];

    bzero((char *)&masks[0], sizeof(masks));
    for (ix = 0; ix < MAXVOLTYPES && !AFS_hiszero(entryp->VolIDs[ix]); ++ix) {
	for (jx = 0; jx < MAXTYPES; ++jx) {
	    if (entryp->VolTypes[ix] == VOLTIX_TO_VOLTYPE(jx)) {
		masks[jx] = ((unsigned32)VLSF_ZEROIXHERE) >> ix;
		break;
	    }
	}
    }
    bzero((char *)&stillUsed[0], sizeof(stillUsed));
    for (ix = 0; ix < entryp->nServers; ++ix) {
	for (jx = 0; jx < MAXTYPES; ++jx) {
	    if (entryp->siteFlags[ix] & VLSF_SAMEASPREV) continue;
	    if (entryp->siteFlags[ix] & masks[jx])
		stillUsed[jx] = 1;
	}
    }
    for (jx = 0; jx < MAXTYPES; ++jx) {
	if (stillUsed[jx]) {
	    entryp->flags |= vldbFlags[jx];
	} else {
	    entryp->flags &= ~vldbFlags[jx];
	}
    }
}


/*
 * Replaces the occurrence in the replication list of [oservAddrp, oaggrId,
 * flag] with [nservAddrp, naggrId]; as a special case. if nserverAddrp==0 and
 * naggrId==-1, the routine removes the entry instead. It returns 0 if the old
 * entry wasn't found, else 1.
 */
vldb_MoveSite(entryp, oservAddrp, oaggrId, nservAddrp, naggrId, mytype)
    register vldbentry *entryp;
    struct sockaddr *oservAddrp, *nservAddrp;
    long  oaggrId, naggrId;
    int mytype;
{
    register int i, j;
    int xl[MAXTYPES];

    fts_GetIndices(entryp, xl);
    /* Bounds-check the input. */
    if (mytype < 0 || mytype >= MAXTYPES || xl[mytype] < 0)
	return 0;
    /* Find where this site is. */
    i = vldb_FindRepSite(entryp, oservAddrp, oaggrId,
			 (((unsigned32)VLSF_ZEROIXHERE)>>xl[mytype]));
    if (i < 0)
	return 0;	/* no such old site */
    if (!nservAddrp && (naggrId == -1)) {
	/* shuffle the rest down to fill in the gap */
	EliminateOldExtras(entryp, i, 0);
    } else {
	*((struct sockaddr *) &entryp->siteAddr[i]) = *nservAddrp;
	entryp->sitePartition[i] = naggrId;
	entryp->sitePrincipal[i].text[0] = '\0';	/* truncate the unknown */
	EliminateOldExtras(entryp, i+1, 1);
    }
    return 1;
}


void vldb_DeleteVolume(entryp, servAddrp, aggrId, volType, volIdp, whatNextp)
    vldbentry *entryp;
    register struct sockaddr *servAddrp;
    long aggrId, volType;
    afs_hyper_t *volIdp;
    int* whatNextp;
{
    register unsigned long offSiteFlags, allFlags;
    register int ix, jx;
    unsigned long mask;

    /* First do the usual to figure out the RW, RO, BK flags for this entry,
	plus the bit-OR of all three such flags. */
    offSiteFlags = 0;
    allFlags = 0;
    for (ix = 0; ix < MAXVOLTYPES && !AFS_hiszero(entryp->VolIDs[ix]); ++ix) {
	for (jx = 0; jx < MAXTYPES; ++jx) {
	    if (entryp->VolTypes[ix] == VOLTIX_TO_VOLTYPE(jx)) {
		mask = ((unsigned32)VLSF_ZEROIXHERE) >> ix;
		allFlags |= mask;
		/* Find the flag that corresponds to the desired type */
		if (jx == volType)
		    offSiteFlags = mask;
		break;
	    }
	}
    }
    /* Now find the index of the site we're trying to eliminate. */
    ix = vldb_FindRepSite(entryp, servAddrp, aggrId, offSiteFlags);
    if (ix < 0) return;		/* nothing to do */
    /* Turn off that flag in the site's siteFlags */
    entryp->siteFlags[ix] &= ~(offSiteFlags);
    /* Turn it off in all duplicate lines, too. */
    for (jx = ix+1; jx < entryp->nServers; ++jx) {
	if ((entryp->siteFlags[jx] & VLSF_SAMEASPREV) == 0)
	    break;
	entryp->siteFlags[jx] &= ~(offSiteFlags);
    }
    if (entryp->siteFlags[ix] & allFlags) {
	/* still some uses left for this entry */
	vldb_FixEntryFlags(entryp);
	if (whatNextp)
	    *whatNextp = 1;
    } else {
	/* Check whether any site entry is still in use. */
	for (jx = 0; jx < entryp->nServers; ++jx) {
	    if (entryp->siteFlags[jx] & VLSF_SAMEASPREV) continue;
	    if (entryp->siteFlags[jx] & allFlags) break;
	}
	if (jx < entryp->nServers) {
	    /* still some uses for the entry as a whole */
	    EliminateOldExtras(entryp, ix, 0);
	    vldb_FixEntryFlags(entryp);
	    if (whatNextp)
		*whatNextp = 1;
	} else {
	    /* the whole entry is useless: all site flags are clear. */
	    entryp->nServers = 0;
	    if (whatNextp)
		*whatNextp = 2;
	}
    }
}


vldb_RemoveVolume(entryp, servAddrp, aggrId, volType, volIdp)
    vldbentry *entryp;
    register struct sockaddr *servAddrp;
    long aggrId, volType;
    afs_hyper_t *volIdp;
{
    register long code = 0;
    int whatNext = 0;

    vldb_DeleteVolume(entryp, servAddrp, aggrId, volType, volIdp, &whatNext);

    if (whatNext == 1) {
	code = vldb_ReplaceEntry(entryp, volIdp, -1, LOCKREL_ALL);
    } else if (whatNext == 2) {
	VOLSERLOG(1,
		  "Last reference to the FLDB entry for %lu,,%lu--deleting it.\n",
		  AFS_HGETBOTH(*volIdp));
	code = vldb_DeleteEntry(volIdp, -1);
    } /* else no FLDB update necessary */

    return code;
}


/*
 * Adds <servAddrp> and <aggrId> as a readonly replication site for <volIdp>
 * in vldb 
 */
vldb_AddSite(servAddrp, aggrId, volIdp, maxreplat)
    register struct sockaddr *servAddrp;
    long aggrId;
    afs_hyper_t *volIdp;
    u_long maxreplat;
{
    long code, tcode;
    vldbentry entry;
    int ROix, RWix, stageIx, rwIx, thisIsStage;
    register int j, ix;
    int roExists;
    unsigned long roMask, rwMask;

    if (code = vldb_SetLock(volIdp, RWVOL, VLOP_ADDSITE))
	return code;

    code = vldb_GetEntryByID(volIdp, RWVOL, &entry, 0);
    if (!code) {
	roExists = entry.flags & VLF_ROEXISTS;
	if ((entry.flags & VLF_LAZYREP) == 0) {
	    dce_svc_printf(FTS_S_SUBR_SET_REPLICATION_MSG);
	    code = -1;
	    goto addsite_end;
	}
	ROix = RWix = -1;
	for (j = 0; j < MAXVOLTYPES && !AFS_hiszero(entry.VolIDs[j]); ++j) {
	    if (entry.VolTypes[j] == VOLTIX_TO_VOLTYPE(ROVOL)) {
		ROix = j;
	    }
	    if (entry.VolTypes[j] == VOLTIX_TO_VOLTYPE(RWVOL)) {
		RWix = j;
	    }
	}
	if (ROix < 0 || RWix < 0) {
	    dce_svc_printf(FTS_S_SUBR_NO_FILESET_IDS_MSG);
	    code = -1;
	    goto addsite_end;
	}
	roMask = ((unsigned32)VLSF_ZEROIXHERE) >> ROix;
	rwMask = ((unsigned32)VLSF_ZEROIXHERE) >> RWix;
	rwIx = -1;
	for (j = 0; j < entry.nServers; ++j) {
	    if (entry.siteFlags[j] & VLSF_SAMEASPREV) continue;
	    if (entry.siteFlags[j] & rwMask) {
		code = fts_NeedAggrType(&entry.siteAddr[j], entry.sitePartition[j],
					AG_TYPE_ANY_EFS_MASK, "Replication is");
		if (code) goto addsite_end;
		rwIx = j;
		break;
	    }
	}
	if (rwIx < 0) {
	    dce_svc_printf(FTS_S_SUBR_NO_READWRITE_SITE_MSG, entry.name);
	    code = -1;
	    goto addsite_end;
	}

	/* complain if we really need a maxreplat */
	if (maxreplat == 0) maxreplat = entry.defaultMaxReplicaLatency;
	if ((entry.flags & VLF_LAZYRELE) == 0) {
	    if (maxreplat == 0) {
		dce_svc_printf(FTS_S_SUBR_NEED_MAXSITEAGE_VALUE_MSG );
		code = -1;
		goto addsite_end;
	    }
	    if (maxreplat <= entry.minimumPounceDally) {
		dce_svc_printf(FTS_S_SUBR_MAXSITEAGE_TOO_SMALL_MSG,
			       fts_ivToString(maxreplat));
		dce_svc_printf(FTS_S_SUBR_MINIMUM_MAXSITEAGE_MSG,
			       fts_ivToString(entry.minimumPounceDally));
		code = -1;
		goto addsite_end;
	    }
	    if (maxreplat >= entry.maxTotalLatency) {
		dce_svc_printf(FTS_S_SUBR_MAXSITEAGE_TOO_BIG_MSG,
			       fts_ivToString(maxreplat));
		dce_svc_printf(FTS_S_SUBR_MAXIMUM_MAXSITEAGE_MSG,
			       fts_ivToString(entry.maxTotalLatency));
		code = -1;
		goto addsite_end;
	    }
	}
	stageIx = fts_releaseRepSite(&entry);
	thisIsStage = 0;
	if ((entry.flags & VLF_LAZYRELE) && (stageIx < 0)) {
	    /* if release-replication and no staging replica yet */
	    if (bcmp((char *)&entry.siteAddr[rwIx], (char *)servAddrp,
		     sizeof(afsNetAddr)) != 0) {
		/* creating on the R/W site? */
		for (ix = rwIx+1;
		     ix < entry.nServers
		       && (entry.siteFlags[ix] & VLSF_SAMEASPREV);
		     ++ix) {
		    if (bcmp((char *)&entry.siteAddr[ix], (char *)servAddrp,
			     sizeof(afsNetAddr)) == 0)
			break;
		}
		if (ix >= entry.nServers
		    || ((entry.siteFlags[ix] & VLSF_SAMEASPREV) == 0)) {
		    /* Nope.  Must create the first one on the R/W site. */
		    dce_svc_printf(FTS_S_SUBR_NEED_STAGING_REPLICA_MSG,
				   MapSockAddr(&entry.siteAddr[rwIx]));
		    code = -1;
		    goto addsite_end;
		}
	    }
	    thisIsStage = 1;
	}
	/* Be sure that we're not trying to create another replica */
	/* on the same server as any existing replica, even the staging one. */
	ix = vldb_FindRepSite(&entry, servAddrp, -1, roMask);
	if (ix >= 0) {
	    if (aggrId == entry.sitePartition[ix]) {
		dce_svc_printf(FTS_S_SUBR_ALREADY_RD_REPLICA_MSG,
			       MapSockAddr(servAddrp),
			  getAggrName(servAddrp, aggrId, 0));
	    } else {
		dce_svc_printf(FTS_S_SUBR_NOT_ANOTHER_RD_REPLICA_MSG,
			       MapSockAddr(servAddrp),
			       getAggrName(servAddrp, aggrId, 0));
		dce_svc_printf(FTS_S_SUBR_IS_ALREADY_MSG,
			       MapSockAddr(&entry.siteAddr[ix]),
			       getAggrName(&entry.siteAddr[ix], entry.sitePartition[ix], 0));
	    }
	    code = -1;
	    goto addsite_end;
	}
	vldb_SaveOldEntry(&entry);
	/* Use an existing site if we can. */
	vldb_EnsureSite(&entry, VLF_ROEXISTS, roMask,
			servAddrp, aggrId, -1, maxreplat);
	code = vldb_ReplaceEntry(&entry, volIdp, RWVOL, LOCKREL_ALL);
	if (!code && !roExists)
	{
	    /* set VOL_IS_REPLICATED in fileset */
	    code = volc_Call(VC_SetStateBits, &entry.siteAddr[rwIx],
			     (u_char *)&entry.sitePrincipal[rwIx].text[0],
			     entry.sitePartition[rwIx], volIdp, 
			     VOL_IS_REPLICATED,/*set*/1);
	}
	if (!code) {
	    vldb_TellRepAboutNewEntry(&entry, 0, thisIsStage);
	}
    }
    addsite_end:
    tcode = vldb_ReleaseLock(volIdp, RWVOL, LOCKREL_ALL);
    if (!code)
	code = tcode;
    return code;
}


/*
 * Removes [serverAddr, aggrId] as a readonly site for <volIdp> from vldb 
 */
vldb_RemoveSite(servAddrp, aggrId, volIdp)
    register struct sockaddr *servAddrp;
    long aggrId;
    afs_hyper_t *volIdp;
{
    long code, tcode;
    vldbentry entry, oldentry;
    int ROix, j;
    int stageIx, thisIx, isDeleted, isDifferent;
    int roExists;
    afs_hyper_t roVolid;
    int anyRO, thisRO;
    int xl[MAXTYPES];
    unsigned long ROFLAG;


    if (code = vldb_SetLock(volIdp, -1, VLOP_ADDSITE))
	return code;

    isDeleted = isDifferent = 0;
    code = vldb_GetEntryByID(volIdp, -1, &entry, 0);
    if (code) {
	(void) vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
	return code;
    }

    /* Save a bunch of info before we dink around with the fldbentry. */
    fts_GetIndices(&entry, xl);
    if (xl[ROVOL] >= 0)
	ROFLAG = ((unsigned32)VLSF_ZEROIXHERE) >> xl[ROVOL];
    else
	ROFLAG = 0;
    stageIx = fts_releaseRepSite(&entry);
    thisRO = vldb_FindRepSite(&entry, servAddrp, aggrId, ROFLAG);
    oldentry = entry;
    roExists = entry.flags & VLF_ROEXISTS;
    roVolid = entry.VolIDs[xl[ROVOL]];

    /*
     * Disallow rmsite of distinguished replica in release
     * replication whenever some other replica exists.
     */
    if (stageIx >= 0 && thisRO >= 0 &&
	stageIx == thisRO) {
	VOLSERLOG(1, "trying to remove distinguished RO\n");

	/* look for any other RO */

	for (anyRO = 0; anyRO < entry.nServers; ++anyRO) {
	    /* skip over same entry at different NW address */
	    if (entry.siteFlags[anyRO] & VLSF_SAMEASPREV)
		continue;

	    if (entry.siteFlags[anyRO] & ROFLAG) {
		/* here is one */
		if (anyRO != stageIx) {
		    /* not allowed */
		    VOLSERLOG(0, "There is another readonly for %s,\n",
			      entry.name);
		    VOLSERLOG(0, "therefore, this rmsite operation is not allowed.\n");
		    vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL);
		    return -1;
		}
	    }
	}
    }

    code = FTS_ERR_BADOP;
    if (xl[ROVOL] < 0) {
	dce_svc_printf(FTS_S_SUBR_NO_RO_FILESET_ID_MSG, entry.name);
    } else {
	thisIx = vldb_FindRepSite(&entry, servAddrp, aggrId, ROFLAG);
	if (thisIx < 0) {
	    dce_svc_printf(FTS_S_SUBR_NO_READONLY_COPY_MSG,
			   entry.name, MapSockAddr(servAddrp), aggrId);
	} else {
	    vldb_SaveOldEntry(&entry);
	    code = vldb_RemoveVolume(&entry, servAddrp, aggrId, ROVOL, volIdp);
	    isDifferent = 1;
	    if (code == 0 && entry.nServers == 0)
		isDeleted = 1;
	}
    }
    if (code == 0) {
	/* If this is the distinguished replica for release rep, we have to delete it ourselves. */
	if (stageIx >= 0) {
	    if (bcmp((char *)&oldentry.siteAddr[stageIx], (char *)&oldentry.siteAddr[thisIx],
		     sizeof(afsNetAddr)) == 0) {
		code = volc_Call(VC_VolumeZap, &oldentry.siteAddr[thisIx],
				 (u_char *)&oldentry.sitePrincipal[thisIx].text[0],
				 oldentry.sitePartition[thisIx], &roVolid);
		if (code == ENOENT || code == ENODEV  ||
		    code == FTSERVER_ERR_NOVOL) 
		    code = 0;
		if (code != 0) {
		    dce_svc_printf(FTS_S_SUBR_ERROR_DELETING_DISTINGUISHED_MSG,
				   entry.name, dfs_dceErrTxt(code));
		}
	    }
	}
	if (((entry.flags & VLF_ROEXISTS) == 0) && roExists) {
	    /* last read-only was removed -- update rw */
	     if (xl[RWVOL] < 0) {
		 dce_svc_printf(FTS_S_SUBR_NO_RW_FOR_MSG, entry.name);
	     } else {
		 long code1;
		 unsigned long rwMask = ((unsigned32)VLSF_ZEROIXHERE) >> xl[RWVOL];

		 for (j = 0; j < entry.nServers; ++j) 
		 {
		     if (entry.siteFlags[j] & VLSF_SAMEASPREV) continue;
		     if (entry.siteFlags[j] & rwMask) 
		     {
			/* clear VOL_IS_REPLICATED in fileset */
			 code1 = volc_Call(VC_SetStateBits, &entry.siteAddr[j],
					   (u_char *)&entry.sitePrincipal[j].text[0],
					   entry.sitePartition[j], volIdp, 
					   VOL_IS_REPLICATED,/*!set*/0);
			 if (code == 0) code = code1;
			 break;
		     }
		 }
	     }
	}
    }

    tcode = (isDeleted? 0 : vldb_ReleaseLock(volIdp, -1, LOCKREL_ALL));
    if (isDifferent)
	vldb_TellRepAboutNewEntry(&entry, 0, 0);
    if (!code) code = tcode;
    return code;
}


vldb_GetVolumeId(volIdp, newVolIdp, volNamep, newFlag, srvAddrp)
    afs_hyper_t *volIdp;
    afs_hyper_t *newVolIdp;
    char *volNamep;
    long *newFlag;
    afsNetAddr *srvAddrp;
{
    vldbentry entry;
    register long code = 0;
    int RWix, k;
    bulkIDs newIDs;

    /* Initialize the output storage */
    *newFlag = 0;
    AFS_hzero(*newVolIdp);

    if (volNamep) {
	if (AFS_hiszero(*volIdp)) {
	    if ((code = vldb_GetEntryByName(volNamep, &entry, 1)) != 0) {
		code = vldb_GetNewVolumeIds(1, srvAddrp, &newIDs);
		if (code == 0)
		  *newVolIdp = newIDs.bulkIDs_val[0];
	    } else {
		RWix = -1;
		for (k = 0; k < MAXVOLTYPES && !AFS_hiszero(entry.VolIDs[k]); ++k) {
		    if (entry.VolTypes[k] == VOLTIX_TO_VOLTYPE(RWVOL)) {
			RWix = k;
			break;
		    }
		}
		if (RWix >= 0) {
		    *newVolIdp = entry.VolIDs[RWix];
		} else {
		    code = vldb_GetNewVolumeIds(1, srvAddrp, &newIDs);
		    if (code == 0)
		      *newVolIdp = newIDs.bulkIDs_val[0];
		}
	    }
	} else {
	    *newVolIdp = *volIdp;
	}
    } else {
	if (AFS_hiszero(*volIdp)) {
	    code = vldb_GetNewVolumeIds(1, srvAddrp, &newIDs);
	    if (code == 0) {
	      *newVolIdp = newIDs.bulkIDs_val[0];
	      *volIdp = *newVolIdp;
	      *newFlag = 1;
	    }
	} else 
	    *newVolIdp = *volIdp;
    }
    return code;
}


static int GetVldbInfo(volIdp, servAddrp, aggrIdp, voltypep, entryp, anyserver)
    afs_hyper_t *volIdp;
    struct sockaddr *servAddrp;
    long *aggrIdp, *voltypep;
    vldbentry *entryp;
    int anyserver;
{
    long code, type, ourtype, index;

    if (code = vldb_GetEntryByID(volIdp, -1, entryp, 0))
	return (code);
    for (type = 0 ; type < MAXVOLTYPES && !AFS_hiszero(entryp->VolIDs[type]); ++type) {
	if (AFS_hsame(*volIdp, entryp->VolIDs[type])) {
	    ourtype = vldb_TypeToIndex(entryp->VolTypes[type]);
	    if (ourtype < 0) {
		dce_svc_printf(FTS_S_SUBR_UNKNOWN_FILESET_TYPE_MSG, entryp->VolTypes[type]);
		return ENOENT;
	    }
	    *voltypep = ourtype;
	    index = vldb_FindRepSite(entryp, 
				     anyserver ? (struct sockaddr *)0 : servAddrp, 
				     -1, ((unsigned32)VLSF_ZEROIXHERE) >> type);
	    if (index == -1) {
		dce_svc_printf(FTS_S_SUBR_NO_SITE_FOR_FILESET_MSG, volTypes[ourtype], entryp->name,
			  AFS_HGETBOTH(*volIdp));
		return ENOENT;		/* XXX */
	    }
	    *servAddrp = *((struct sockaddr *) &entryp->siteAddr[index]);
	    *aggrIdp = entryp->sitePartition[index];
	    break;
	}
    }
    if (NameTooLong(entryp->name))
	return -1;
    return 0;
}


vldb_GetVldbInfo(volIdp, servAddrp, aggrIdp, voltypep, entryp)
    afs_hyper_t *volIdp;
    struct sockaddr *servAddrp;
    long *aggrIdp, *voltypep;
    vldbentry *entryp;
{
    int retVal;

    retVal = GetVldbInfo(volIdp, servAddrp, aggrIdp, voltypep, entryp, 
			 /* anyserver? */1);
    return retVal;
}

vldb_GetVldbInfoByServer(volIdp, servAddrp, aggrIdp, voltypep, entryp)
    afs_hyper_t *volIdp;
    struct sockaddr *servAddrp;
    long *aggrIdp, *voltypep;
    vldbentry *entryp;
{
    int retVal;

    retVal = GetVldbInfo(volIdp, servAddrp, aggrIdp, voltypep, entryp, 
			 /* anyserver? */0);
    return retVal;
}

/* 
 * Set <servAddrp> and <aggrId> to the correct values depending on
 * <voltype> and <entryp> 
 */
vldb_GetServerAndAggr(entryp, voltype, servAddrp, aggrIdp)
    vldbentry *entryp;
    struct sockaddr *servAddrp;
    long *aggrIdp;
    int voltype;
{
    int i, wantedIx;
    long wantedType;

    *aggrIdp = -1;
    wantedIx = 0;
    wantedType = (voltype == BACKVOL ? RWVOL : voltype);
    wantedType = VOLTIX_TO_VOLTYPE(wantedType);
    for (i = 0; i < MAXVOLTYPES && !AFS_hiszero(entryp->VolIDs[i]); ++i) {
	if (entryp->VolTypes[i] == wantedType) {
	    wantedIx = i;
	    break;
	}
    }
    if (wantedIx >= 0) {
	for (i = 0; i < entryp->nServers ; i++) {
	    if ((entryp->siteFlags[i] && (((unsigned32)VLSF_ZEROIXHERE) >> wantedIx)) != 0) {
		*servAddrp = *((struct sockaddr *) &entryp->siteAddr[i]);
		*aggrIdp = entryp->sitePartition[i];
		return 0;
	    }
	}
    }
    dce_svc_printf(FTS_S_SUBR_NO_TYPE_IN_FLDB_MSG, volTypes[voltype], entryp->name);
    return 1;
}


static int vldb_CompareEntryByName(p1, p2)
    const void *p1, *p2;
{
    vldbentry *arg1,*arg2;

    arg1 = (vldbentry *)p1;
    arg2 = (vldbentry *)p2;
    return(strcmp((char *)arg1->name, (char *)arg2->name));
}

static int vldb_CompareEntryByRWAddr(p1, p2)
    const void *p1, *p2;
{
    vldbentry *arg1,*arg2;
    register int ix;
    unsigned long rwMask1, rwMask2;
    register int rwIx1, rwIx2;

    arg1 = (vldbentry *)p1;
    arg2 = (vldbentry *)p2;
    rwMask1 = rwMask2 = 0;
    for (ix = 0; ix < MAXVOLTYPES && !AFS_hiszero(arg1->VolIDs[ix]); ++ix) {
	if (arg1->VolTypes[ix] == VOLTIX_TO_VOLTYPE(RWVOL)) {
	    rwMask1 = ((unsigned32)VLSF_ZEROIXHERE) >> ix;
	    break;
	}
    }
    for (ix = 0; ix < MAXVOLTYPES && !AFS_hiszero(arg2->VolIDs[ix]); ++ix) {
	if (arg2->VolTypes[ix] == VOLTIX_TO_VOLTYPE(RWVOL)) {
	    rwMask2 = ((unsigned32)VLSF_ZEROIXHERE) >> ix;
	    break;
	}
    }
    rwIx1 = rwIx2 = 0;
    for (ix = 0; ix < arg1->nServers; ++ix) {
	if (arg1->siteFlags[ix] & rwMask1) {
	    rwIx1 = ix;
	    break;
	}
    }
    for (ix = 0; ix < arg2->nServers; ++ix) {
	if (arg2->siteFlags[ix] & rwMask2) {
	    rwIx2 = ix;
	    break;
	}
    }
    ix = bcmp((char *)&arg1->siteAddr[rwIx1], (char *)&arg2->siteAddr[rwIx2],
	      sizeof(afsNetAddr));
    if (ix != 0) return ix;
    return(arg1->sitePartition[rwIx1] - arg2->sitePartition[rwIx2]);
}


static void vldb_HandleBulkEntries(nentries, Entriesp, prefixp, cmd, succp, failp, softfailp)
    long nentries;
    bulkentries *Entriesp;
    char *prefixp;
    long cmd;
    long *succp, *failp, *softfailp;
{
    register vldbentry *entryp;
    afs_hyper_t volId;
    struct sockaddr servAddr, oldServAddr;
    long aggrId, oldAggrId;
    register int k;
    int RWix, vtype, isSoft;
    long i, code = 0;
    unsigned long rwmask;

    if (nentries > 1) {
	if (cmd == 4/*BULK_LIST*/)
	    qsort((char *)Entriesp->bulkentries_val, nentries,
		  sizeof(vldbentry), vldb_CompareEntryByName);
	if (cmd == 2/*BULK_BACKUP*/)
	    qsort((char *)Entriesp->bulkentries_val, nentries,
		  sizeof(vldbentry), vldb_CompareEntryByRWAddr);
    }
    bzero((char *)&oldServAddr, sizeof(oldServAddr));
    aggrId = -1;
    for (i = 0; i < nentries; i++) {	
	entryp = &(Entriesp->bulkentries_val[i]);
	if (prefixp && strncmp((char *)entryp->name, prefixp, strlen(prefixp)))
	    continue;
	RWix = -1;
	rwmask = 0;
	for (k = 0; k < MAXVOLTYPES && !AFS_hiszero(entryp->VolIDs[k]); ++k) {
	    if (entryp->VolTypes[k] == VOLTIX_TO_VOLTYPE(RWVOL)) {
		RWix = k;
		rwmask = ((unsigned32)VLSF_ZEROIXHERE) >> k;
		break;
	    }
	}
	volId = entryp->VolIDs[RWix >= 0 ? RWix : 0];
	vtype = ((RWix >= 0) ? RWVOL : -1);
	isSoft = 0;
	switch (cmd) {
	    case 1: /* BULK_DELETE */
		vldb_SaveOldEntry(NULL);
		code = vldb_DeleteEntry(&volId, vtype);
		(void) vldb_TellRepAboutNewEntry(entryp, 0, 1);
		break;
	    case 2: /* BULK_BACKUP */
		if (!(entryp->flags & VLF_RWEXISTS)) {
		    VOLSERLOG(1, "Not performing backup on non-RW fileset %s\n", entryp->name);
		    continue;
		}
		for (k = 0; k < entryp->nServers; ++k) {
		    if ((entryp->siteFlags[k] && rwmask) != 0) {
			servAddr = *((struct sockaddr *) &entryp->siteAddr[k]);
			aggrId = entryp->sitePartition[k];
			break;
		    }
		}
		if (k >= entryp->nServers) {
		    dce_svc_printf(FTS_S_SUBR_NO_BACKUP_INVALID_FLDB_MSG, entryp->name);
		    ++(*failp);
		    continue;
		}
		if (bcmp((char *)&servAddr, (char *)&oldServAddr, sizeof(servAddr)) != 0 || aggrId != oldAggrId) {
		    /* Treat wrong-aggregate-type as a ``soft'' failure. */
		    code = fts_NeedAggrType(&servAddr, aggrId, AG_TYPE_ANY_EFS_MASK,
					"Backup is");
		    if (code) {
			if (code == -1) /* means that type was wrong */
			    isSoft = 1; 
			else {
			  /* Print out an error message */
			  VOLSERLOG(0, "Cannot clone fileset %s on server %s aggregate %s : %s\n", 
				    entryp->name,
				    MapSockAddr(&servAddr),
				    getAggrName(&servAddr, aggrId, 0), 
				    dfs_dceErrTxt(code));

			  vldb_EnumerateEntry(entryp, "FLDB Information", 1);
			}
			break;
		    }
		    oldServAddr = servAddr;
		    oldAggrId = aggrId;
		}
		code = volc_Call(VC_BackupVolume, &servAddr, NULL, aggrId, &volId);
		break;
	    case 3: /* BULK_UNLOCK */
		code = vldb_ReleaseLock(&volId, vtype, LOCKREL_ALL);
		break;
	    case 4: /* BULK_LIST */
		vldb_EnumerateEntry(entryp, " ", 0);
		break;
	}
	if (code) {
	    if (isSoft)
		++(*softfailp);
	    else
		++(*failp);
	} else {
	    ++(*succp);
	}
    }
}


vldb_ProcessByAttrs(serverNamep, aggrNamep, locked, prefix, cmdix)
    char *serverNamep;
    char *aggrNamep;
    long locked;
    char *prefix;
    long cmdix;
{
    long code, succs, fails, softfails;
    struct sockaddr servAddr;
    struct VldbListByAttributes attributes;
    bulkentries Entries;
    u_long aggrId, nentries, cookie, nextcookie, flg;
    static char *cmdName[4] = 
    {"deleted", "backed up", "unlocked", "enumerated"};

    if (cmdix < 1 || cmdix > 4) {
	dce_svc_printf(FTS_S_SUBR_BAD_COMMAND_INDEX_MSG, cmdix);
	return -1;
    }
    bzero((char *)&attributes, sizeof(struct VldbListByAttributes));
    attributes.Mask = 0;
    if (serverNamep) {				
	if (!GetServer(serverNamep, &servAddr))
	    return -1;
	*((struct sockaddr *) &attributes.site) = servAddr;
	attributes.Mask |= VLLIST_SITE;
    }
    if (aggrNamep) {	
	if (serverNamep) {
	    if (!GetAggr(aggrNamep, &servAddr, 1, &aggrId, (char *)0))
		exit(1);
	    attributes.partition = aggrId;
	    attributes.Mask |= VLLIST_PARTITION;
	} else {
	    return FTS_ERR_BADAGGREGATE;
	}
    }
    if (locked) {
	attributes.flag = VLOP_ALLOPERS;
	attributes.Mask |=  VLLIST_FLAG;
    }
    succs = fails = softfails = 0;
    for (cookie = flg = 0; (flg & 1) == 0; cookie = nextcookie) {
	bzero((char *)&Entries, sizeof(bulkentries));
	if (code = vldb_ListByAttributes(&attributes, cookie, &nentries,
					 &Entries, &nextcookie, &flg)) {
	    if (code == VL_ENDOFLIST) break;
	    return code;
	}
	vldb_HandleBulkEntries(nentries, &Entries, prefix, cmdix, &succs,
			       &fails, &softfails);
    }
    dce_svc_printf(FTS_S_SUBR_DASH_DASH_MSG);
    dce_svc_printf(FTS_S_SUBR_TOTAL_FLDB_ENTRIES_MSG, cmdName[cmdix-1], succs, fails, softfails);
    /* Don't let soft-failures screw up the return code unless there were
		     no successes. */
    /* Catch the no-match case (succs==0) so that failure to match any names is
	an error. */
    if (succs != 0 && fails == 0)
	return 0;
    else
	return 1;
}


vldb_DisplayVolId(volIdp)
    afs_hyper_t *volIdp;
{
    register long code;
    vldbentry entry;

    if ((code = vldb_GetEntryByID(volIdp, -1 ,&entry, 0)))
	return code;
    vldb_EnumerateEntry(&entry, "", 0);
    return 0;
}

/*
 * Enumerate the vldb entry corresponding to <entryp> 
 */
vldb_EnumerateEntry(entryp, headerText, quietp)
    vldbentry *entryp;
    char *headerText;
    int quietp;
{
    register int i, type;
    int numRealSites;
    int xl[MAXTYPES];
    long code;
    unsigned long existsflags[MAXTYPES];
    static char *flagname[MAXTYPES] = {"RW", "RO", "BK"};
    static struct {unsigned long flg; char *nm;} flgNames[] = {
	{VLSF_SAMEASPREV, "aa"},
	{VLSF_DEFINED, "df"},
	{VLSF_PARTIALADDRS, "pt"}
    };
    unsigned long sf;
    unsigned32 protectLevel;
    char datebuf[28];
    char nbuf[12];
    char hnbuf[32];
    char authnbuf[50];
    char ownerbuf[100];
    long bogusSignedTime;

    if (headerText) {
	if (headerText[0] == '\0') {
	    dce_svc_printf(FTS_S_SUBR_ENTRYP_NAME_NONL_MSG, entryp->name);
	} else {
	    dce_svc_printf(FTS_S_SUBR_ENTRYP_NAME_MSG,
			   entryp->name, headerText);
	}
    }
    for (i = 0; i < MAXTYPES; ++i) {xl[i] = -1; existsflags[i] = 0;}
    for (i = 0; i < MAXVOLTYPES && !AFS_hiszero(entryp->VolIDs[i]); i++) {
	if ((type = vldb_TypeToIndex(entryp->VolTypes[i])) == -1) {
	    dce_svc_printf(FTS_S_SUBR_ILLEGAL_UNKNOWN_TYPE_MSG,
			   entryp->VolTypes[i]);
	    continue;
	}
	xl[type] = i;
	existsflags[type] = ((unsigned32)VLSF_ZEROIXHERE) >> i;
	dce_printf(fts_s_subr_voltypes_sub_type, volTypes[type],
		  AFS_HGETBOTH(entryp->VolIDs[xl[type]]));
	if (entryp->flags & vldbFlags[type]) 	
	    dce_svc_printf(FTS_S_SUBR_VALIE_MSG);
	else 
	    dce_svc_printf(FTS_S_SUBR_INVALIE_MSG);
    }
    if (!AFS_hiszero(entryp->cloneId) && (entryp->flags & VLF_ROEXISTS))
	dce_svc_printf(FTS_S_SUBR_RELEASECLONE_MSG,
		       AFS_HGETBOTH(entryp->cloneId));
    numRealSites = 0;
    for (i = 0; i < entryp->nServers && (entryp->siteFlags[i] &VLSF_DEFINED); ++i) {
	if ((entryp->siteFlags[i] & VLSF_SAMEASPREV) == 0) {
	    ++numRealSites;
	}
    }
    /* Lock status */
    if ((entryp->flags & VLOP_ALLOPERS) || entryp->WhenLocked) {
	vldb_PrintLocker(entryp, -1, "  Locked");
    }
    /* Replication parameters */
    if (entryp->flags & VLF_LAZYREP) {
	if (entryp->flags & VLF_LAZYRELE) {
	    dce_printf(fts_s_subr_release_repl, fts_ivToString(entryp->maxTotalLatency));
	    dce_printf(fts_s_subr_failage, fts_ivToString(entryp->hardMaxTotalLatency));
	    dce_printf(fts_s_subr_reclaimwait, fts_ivToString(entryp->reclaimDally));
	} else {
	    dce_printf(fts_s_subr_sched_repl, fts_ivToString(entryp->maxTotalLatency));
	    dce_printf(fts_s_subr_failage, fts_ivToString(entryp->hardMaxTotalLatency));
	    dce_printf(fts_s_subr_reclaimwait, fts_ivToString(entryp->reclaimDally));
	    dce_printf(fts_s_subr_minrepdelay,
		       fts_ivToString(entryp->minimumPounceDally));
	    if (entryp->defaultMaxReplicaLatency != 0) {
		dce_printf(fts_s_subr_defaultsiteage,
			   fts_ivToString(entryp->defaultMaxReplicaLatency));
	    }
	    printf("\n");
	}
    }

    /* Protection-level bounds */
    protectLevel = ((unsigned char)entryp->charSpares[VLAUTHN_MINLCLLEVEL]);
    if (protectLevel != rpc_c_protect_level_default) {
	dce_printf(fts_s_subr_minlclprotectlev,
		   compat_authnToString(protectLevel, authnbuf, sizeof(authnbuf)));
    }
    protectLevel = ((unsigned char)entryp->charSpares[VLAUTHN_MAXLCLLEVEL]);
    if (protectLevel != rpc_c_protect_level_default) {
	dce_printf(fts_s_subr_maxlclprotectlev,
		   compat_authnToString(protectLevel, authnbuf, sizeof(authnbuf)));
    }
    protectLevel = ((unsigned char)entryp->charSpares[VLAUTHN_MINRMTLEVEL]);
    if (protectLevel != rpc_c_protect_level_default) {
	dce_printf(fts_s_subr_minrmtprotectlev,
		   compat_authnToString(protectLevel, authnbuf, sizeof(authnbuf)));
    }
    protectLevel = ((unsigned char)entryp->charSpares[VLAUTHN_MAXRMTLEVEL]);
    if (protectLevel != rpc_c_protect_level_default) {
	dce_printf(fts_s_subr_maxrmtprotectlev,
		   compat_authnToString(protectLevel, authnbuf, sizeof(authnbuf)));
    }

    if (numRealSites == entryp->nServers) {
	dce_svc_printf(FTS_S_SUBR_NUMBER_SITES_MSG, numRealSites);
    } else {
	dce_svc_printf(FTS_S_SUBR_NUMBER_ADDRESSES_MSG,
		       numRealSites, entryp->nServers);
    }
    /* Header for sites. */

    dce_printf(fts_s_subr_format);

    for (i = 0; i < entryp->nServers; i++) {
	datebuf[0] = '\0';
	sf = entryp->siteFlags[i] & ~(VLSF_DEFINED);
	for (type = 0; type < MAXTYPES; ++type) {
	    if (sf & existsflags[type]) {
		if (datebuf[0] != '\0') strcat(datebuf, ",");
		strcat(datebuf, flagname[type]);
		sf &= ~existsflags[type];
	    }
	}
	for (type = 0; type < (sizeof(flgNames)/sizeof(flgNames[0])); ++type) {
	    if (sf & flgNames[type].flg) {
		if (datebuf[0] != '\0') strcat(datebuf, ",");
		strcat(datebuf, flgNames[type].nm);
		sf &= ~flgNames[type].flg;
	    }
	}
	if (sf != 0) {
	    if (datebuf[0] != 0) strcat(datebuf, ",");
	    sprintf(nbuf, "%lx", sf);
	    strcat(datebuf, nbuf);
	}
	if (datebuf[0] == '\0')
	    strcpy(datebuf, "*none*");
	strncpy(&hnbuf[0], (char *)MapSockAddr(&entryp->siteAddr[i]), 32);
	hnbuf[19] = '\0';	/* truncate the name if it's long */
	dce_printf(fts_s_subr_another_format, hnbuf, datebuf,
	       getAggrName(&entryp->siteAddr[i], entryp->sitePartition[i], quietp),
	       fts_ivToString(entryp->sitemaxReplicaLatency[i]),
	       &entryp->sitePrincipal[i].text[0]);
	code = fts_UuidToGroup(&entryp->siteOwner[i], &ownerbuf[0]);
	/* if we can't lookup up the group name, try to print the uuid */
	if (code)
	    code = fts_uuidToStr(&entryp->siteOwner[i], &ownerbuf[0], sizeof(ownerbuf));
	if (code) sprintf(ownerbuf, "<<%s>>", dfs_dceErrTxt(code));
	dce_svc_printf(FTS_S_SUBR_YET_ANOTHER_FORMAT_MSG, ownerbuf);
    }
    return;
}


vldb_PrintRepSites(entryp, flags, localOnly, localHerep)
    vldbentry *entryp;
    long flags;
    int localOnly;
    struct sockaddr *localHerep;
{
    register int i;

    dce_svc_printf(FTS_S_SUBR_REPLICATION_SITES_ARE_MSG);
    for (i = 0; i < entryp->nServers; i++) {
	if (localOnly && localHerep) {
	    if (bcmp((char *)&entryp->siteAddr[i], (char *)localHerep, sizeof(struct sockaddr)) != 0)
		continue;
	}
	if (entryp->siteFlags[i] & flags) {
	    dce_svc_printf(FTS_S_SUBR_SERVER_AGGREGATE_MSG, MapSockAddr(&entryp->siteAddr[i]), 
		   getAggrName(&entryp->siteAddr[i], entryp->sitePartition[i], 0));
	}
    }    
}

/* define functions to convert group names into uuids and back again */
int fts_GroupToUuid(groupname, uuidp)
  char *groupname;
  uuid_t *uuidp;
{
    sec_rgy_handle_t context;
    sec_rgy_bind_auth_info_t bindingAuthInfo;
    sec_rgy_name_t cellDirName;
    sec_rgy_name_t fullGroupName;
    sec_rgy_name_t secGroupName;
    error_status_t code;
    char *cellname;

    /* bind to registry unauthenticated */
    bindingAuthInfo.info_type = sec_rgy_bind_auth_none;
    (void) strcpy((char *)fullGroupName, groupname);
    sec_id_parse_name(sec_rgy_default_handle, fullGroupName, 
		      cellDirName, NULL, secGroupName, NULL, &code);
    if (code != error_status_ok)
	return code;

    /* 
     * Require that the cell name part of the group name
     * is in the cell specified by '-cellname'
     */
    getLocalCellInfo();
    cellname = *remoteCellName ? remoteCellName : localCellName;
    if (strcmp((caddr_t)cellDirName, cellname) != 0)
    {
	/* invalid cell name */
	return FTS_ERR_BADGROUP;
    }

    sec_rgy_site_bind(cellDirName, &bindingAuthInfo, &context, &code);
    if (code != error_status_ok)
	return code;

    sec_rgy_pgo_name_to_id(context, sec_rgy_domain_group, 
			   secGroupName, 
			   uuidp,
			   &code);

    return (int)code;
}

int fts_UuidToGroup(uuidp, groupname)
  uuid_t *uuidp;
  char *groupname;
{
    error_status_t code;
    sec_rgy_name_t secGroupName;
    sec_rgy_name_t fullGroupName;
    sec_rgy_handle_t context;
    sec_rgy_bind_auth_info_t bindingAuthInfo;
    sec_rgy_name_t cellDirName;

    if (uuid_is_nil(uuidp, &code) == true && code == uuid_s_ok) {
	strcpy(groupname, "<nil>");
	return 0;
    }

    /* run an unauthenticated request */
    getLocalCellInfo();
    bindingAuthInfo.info_type = sec_rgy_bind_auth_none;
    (void) strcpy((caddr_t)cellDirName, (*remoteCellName ? remoteCellName : localCellName));

    sec_rgy_site_bind(cellDirName, &bindingAuthInfo, &context, &code);
    if (code != error_status_ok)
	return code;

    sec_rgy_pgo_id_to_name(context, sec_rgy_domain_group, 
			   uuidp, secGroupName, &code);
    if (code == error_status_ok)
	(void) strcpy(groupname, (char *)secGroupName);

    return code;
}
