/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: volc_vldbint.c,v $
 * Revision 1.1.828.1  1996/10/02  21:15:34  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:49  damon]
 *
 * $EndLog$
*/

/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/common_data.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <dce/rpc.h>
#include <dce/secsts.h>

#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <dcedfs/fldb_proc.h>
#include <dcedfs/flserver.h>
#include <dcedfs/ftserver.h>

#include <volc.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftimac.h>
#include <dfsftisvc.h>
#include <dfsftimsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/userInt/fts/volc_vldbint.c,v 1.1.828.1 1996/10/02 21:15:34 damon Exp $")

static struct ubik_client *cstruct = NULL;
char currentCellName[400] = "";		/* cell whose fldb is currently open */
char localCellName[400] = "";		/* name of the local cell */
afs_hyper_t localCellID = AFS_HINIT(0,0);
char remoteCellName[400] = "";		/* a lot like currentCellName */
afs_hyper_t remoteCellID = AFS_HINIT(0,0); /* cell ID from remote cell's fls */

void getLocalCellInfo()
{
    long code;
    long i;
    unsigned long st;
    char *cellNameP = 0;
    static int done = 0;

    if (done) return;
    dce_cf_get_cell_name(&cellNameP, &st);
    if (st != 0) {
	dce_svc_printf(FTS_S_VLDBINT_LOCAL_CELL_NAME_MSG, dfs_dceErrTxt(st));
	exit(1);
    } else if ((cellNameP == 0) || (cellNameP[0] != '/')) {
	dce_svc_printf(FTS_S_VLDBINT_CELL_NAME_UNUSABLE_MSG, cellNameP);
	exit(1);
    } else {
	/* successfully got cell name */
	strcpy(localCellName, cellNameP);
    }
    if (cellNameP != 0) free(cellNameP);
    done = 1;
}

/* vldb_SetRemoteCellName
 *	Fill in remoteCellName with remote cell name
 * entry:
 *	cellname - remote cell name
 */
vldb_SetRemoteCellName(cellname)
char *cellname;
{
    if (!cellname) return;
    if (*cellname == '/')
	(void) strcpy(remoteCellName, cellname);
    else
	(void) sprintf(remoteCellName, "/.../%s", cellname);
}

/* vldb_InitUbikHandle
 *	Initialize ubik structures
 * entry:
 *	cellName - a CDS cell name, without any /fs suffix (so far)
 */
int vldb_InitUbikHandle(cellName)
char *cellName;
{
    long code;
    static int done = 0;
    int cnlen;
    char cellWithFS[450];
    unsigned long st, oldAuth;

    if (cellName == NULL) {
	getLocalCellInfo();
	cellName = localCellName;
    }
    if (done && (strcmp(cellName, currentCellName) == 0)) return 0;

    strncpy(cellWithFS, cellName, sizeof(cellWithFS));
    cnlen = strlen(cellWithFS);
    dfs_GetJunctionName(cellName, 0, &cellWithFS[cnlen],
			 (sizeof(cellWithFS) - cnlen - 1), &st);
    if (st != 0) {
      if ( st == sec_login_s_no_current_context || st == rpc_s_auth_tkt_expired || st == sec_rgy_server_unavailable ) {
	dce_svc_printf(FTS_S_VLDBINT_UNAUTHENTICATED_CONNECTION_MSG, programName, cellName, dfs_dceErrTxt(st));
	useNoAuth = 1;
	rpc_ns_set_authn(0, &oldAuth, &st); /* turn off authentication */
	if (st != rpc_s_ok) {
	  dce_svc_printf(FTS_S_VLDBINT_USER_UNAUTHENTICATED_MSG,
			 programName,
			 dfs_dceErrTxt(st));
	  return st;
	}
	dfs_GetJunctionName(cellName, 0, &cellWithFS[cnlen],
			    (sizeof(cellWithFS) -cnlen -1), &st);
	if (st != 0) {
	    dce_svc_printf(FTS_S_VLDBINT_NO_DFS_JUNCTION_MSG, programName, cellName, dfs_dceErrTxt(st));
	    return st;
	  }
      }
      else {
	dce_svc_printf(FTS_S_VLDBINT_NO_DFS_JUNCTION_MSG, programName, cellName, dfs_dceErrTxt(st));
	return st;
      }
    }

    code = ubik_ClientInit(NULL, (unsigned_char_t *)cellWithFS,
			   (unsigned_char_t *)"subsys/dce/dfs-fs-servers",
			   useNoAuth, useLocalAuth, 0,
			   (struct ubik_client **)&cstruct); /* don't init auth again */
    if (code) {
	if (code == rpc_s_entry_not_found) {
	    dce_svc_printf(FTS_S_VLDBINT_CELL_NOT_FOUND_MSG, programName, cellName, cellWithFS);
	} else {
	    dce_svc_printf(FTS_S_VLDBINT_UBIK_INIT_FAILED_MSG, cellWithFS, dfs_dceErrTxt(code));
	}
	return code;
    }
    done = 1;
    strncpy(currentCellName, cellName, sizeof(currentCellName));
    return 0;
}

void vldb_PrintLocker(entryp, level, pfx)
vldbentry *entryp;
int level;
char *pfx;
{
    long bogusSignedTime;
    char dateBuf[28];
    static struct {unsigned long mask; char *name;} lockBitNames[] = {
	{VLOP_ADDSITE, "addsite"},
	{VLOP_RESTORE, "restore"},
	{VLOP_DUMP, "dump"},
	{VLOP_DELETE, "delete"},
	{VLOP_BACKUP, "backup"},
	{VLOP_RELEASE, "release"},
	{VLOP_MOVE, "move"},
    };
    char thisLock[130];
    unsigned long theseBits;
    register int ix;

    theseBits = entryp->flags & VLOP_ALLOPERS;
    thisLock[0] = '\0';
    if (theseBits == VLOP_ALLOPERS) {
	strcpy(thisLock, "everything");
    } else {
	for (ix = (sizeof(lockBitNames) / sizeof(lockBitNames[0]))-1; ix >= 0; --ix) {
	    if ((theseBits & lockBitNames[ix].mask) != 0) {
		if (thisLock[0] != '\0') strcat(thisLock, "&");
		strcat(thisLock, lockBitNames[ix].name);
	    }
	}
    }
    bogusSignedTime = entryp->WhenLocked;
    strncpy(&dateBuf[0], (char *) ctime(&bogusSignedTime), sizeof(dateBuf));
    dateBuf[24] = '\0';
    if (level < 0) {
	dce_printf(fts_s_vldbint_for_on, pfx, thisLock, dateBuf);
	if (entryp->LockerName[0] != '\0') {
	  dce_printf(fts_s_vldbint_by);
	  fputs((char *)entryp->LockerName, stdout);
	}
	fputc('\n', stdout);
    } else {
	VOLSERLOG(level, "%s for %s on %s", pfx, thisLock, dateBuf);
	if (entryp->LockerName[0] != '\0') {
	    VOLSERLOG(level, " by %s", entryp->LockerName);
	}
	VOLSERLOG(level, "\n");
    }
}

/*
 * This file includes the RPC interface calls to the VOLUME LOCATION SERVER
 * (VLDB).  We have this additional layer mainly so that we can isolate
 * debugging/logging messages in a single place. Note that additional
 * processing is done in some of the calls so be careful if want to get rid of
 * this file.
 */

vldb_CreateEntry(entryp, volNamep)
    vldbentry *entryp;
    char *volNamep;
{
    register long code = 0;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    if ((code = ubik_Call(VL_CreateEntry, cstruct, 0, entryp))) {
	dce_svc_printf(FTS_S_VLDBINT_CANT_CREATE_FLDB_ENTRY_MSG, volNamep);
	(void)PrintVLError(0, code);
    }
    return code;
}


vldb_DeleteEntry(volIdp, volType)
    afs_hyper_t *volIdp;
    long volType;
{
    register long code;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    code = ubik_Call(VL_DeleteEntry, cstruct, 0, volIdp,
		     (volType == -1 ? -1 : VOLTIX_TO_VOLTYPE(volType)));
    if (code != error_status_ok) {
	dce_svc_printf(FTS_S_VLDBINT_CANT_DELETE_FLDB_ENTRY_MSG,
		       AFS_HGETBOTH(*volIdp), volType);
	(void)PrintVLError(0, code);
    }
    return code;
}


vldb_GetEntryByID(volIdp, volType, entryp, quiet)
    afs_hyper_t *volIdp;
    long volType;
    vldbentry *entryp;
    int quiet;
{
    register long code;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    if ((code = ubik_Call(VL_GetEntryByID, cstruct, 0, volIdp,
			  (volType == -1 ? -1 : VOLTIX_TO_VOLTYPE(volType)),
			  entryp))) {
	if (quiet == 0) {
	    dce_svc_printf(FTS_S_VLDBINT_CANT_FETCH_FLDB_ENTRY_MSG,
			   AFS_HGETBOTH(*volIdp), volType);
	    (void)PrintVLError(0, code);
	}
    }
    return code;
}


vldb_GetEntryByName(volNamep, entryp, quiet)
    char *volNamep;
    vldbentry *entryp;
    int quiet;
{
    register long code;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    if ((code = ubik_Call(VL_GetEntryByName, cstruct, 0, volNamep, entryp))) {
	if (quiet == 0) {
	    dce_svc_printf(FTS_S_VLDBINT_CANT_FETCH_FLDB_MSG, volNamep);
	    (void)PrintVLError(0, code);
	}
    }
    return code;
}


vldb_GetNewVolumeId(bumpCount, serverAddrp, volIdp)
    long bumpCount;
    afsNetAddr *serverAddrp;
    afs_hyper_t *volIdp;
{
    register long code;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    if ((code = ubik_Call(VL_GetNewVolumeId, cstruct, 0, bumpCount,
			  serverAddrp, volIdp))) {
	if (!bumpCount)
	    dce_svc_printf(FTS_S_VLDBINT_CANT_GET_HIGHEST_ID_MSG);
	else
	    dce_svc_printf(FTS_S_VLDBINT_CAN_GET_NEW_IDS_MSG, bumpCount);
	(void)PrintVLError(0, code);
    }
    return code;
}


vldb_ReplaceEntry(entryp, volIdp, volType, lockrelFlag)
    vldbentry *entryp;
    afs_hyper_t *volIdp;
    long volType;
    long lockrelFlag;
{
    register long code;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    code = ubik_Call(VL_ReplaceEntry, cstruct, 0, volIdp,
		     (volType == -1 ? -1 : VOLTIX_TO_VOLTYPE(volType)),
		     entryp, lockrelFlag);
    if (code != error_status_ok) {
      dce_svc_printf(FTS_S_VLDBINT_CANT_UPDATE_FLDB_ENTRY_MSG,
		     AFS_HGETBOTH(*volIdp));
      (void)PrintVLError(0, code);
    }
    return code;
}


/* volOper of zero is flag to say not to complain */
vldb_SetLock(volIdp, volType, volOper)
    afs_hyper_t *volIdp;
    long volType;
    long volOper;
{
    register long code, xcode;
    vldbentry entry;
    char ctm[30];
    long bogusSignedTime;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    if ((code = ubik_Call(VL_SetLock, cstruct, 0, volIdp,
			 (volType == -1 ? -1 : VOLTIX_TO_VOLTYPE(volType)),
			  (volOper ? volOper : VLOP_DELETE)))) {
	if (((xcode = ubik_Call(VL_GetEntryByID, cstruct, 0, volIdp,
		 	(volType == -1 ? -1 : VOLTIX_TO_VOLTYPE(volType)),
			 &entry)) != 0)){
	    entry.LockerName[0] = '\0';
	    entry.WhenLocked = 0;
	}
	if (volOper) {
	    dce_svc_printf(FTS_S_VLDBINT_CANT_LOCK_FLDB_ENTRY_MSG,
			   AFS_HGETBOTH(*volIdp), volType, volOper);
	    if (entry.WhenLocked != 0) {
		vldb_PrintLocker(&entry, 0, "...locked");
	    }
	    (void)PrintVLError(0, code);
	}
    }
    return code;
}


vldb_ReleaseLock(volIdp, volType, relType)
    afs_hyper_t *volIdp;
    long volType;
    long relType;
{
    register long code;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    if ((code = ubik_Call(VL_ReleaseLock, cstruct, 0, volIdp,
			 (volType == -1 ? -1 : VOLTIX_TO_VOLTYPE(volType)),
			  relType))) {
	dce_svc_printf(FTS_S_VLDBINT_CANT_UNLOCK_FLDB_ENTRY_MSG,
		       AFS_HGETBOTH(*volIdp), volType);
	PrintVLError(0, code);
    }
    return code;
}


vldb_ListByAttributes(attribp, cookie, nentriesp, arrayEntriesp, nextCookieP, flagsP)
    VldbListByAttributes *attribp;
    u_long cookie;
    long *nentriesp;
    bulkentries *arrayEntriesp;
    u_long *nextCookieP;
    u_long *flagsP;
{
    register long code;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    if ((code = ubik_Call(VL_ListByAttributes, cstruct, 0, attribp, cookie,
			  nentriesp, arrayEntriesp, nextCookieP, flagsP))) {
	if (code != VL_ENDOFLIST) {
	    dce_svc_printf(FTS_S_VLDBINT_CANT_ACCESS_FLDB_MSG);
	    (void)PrintVLError(0, code);
	}
    }
    return code;
}


vldb_CreateServer(siteinfo)
siteDesc *siteinfo;
{
    register long code;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    code = ubik_Call(VL_CreateServer, cstruct, 0, siteinfo);
    if (code != error_status_ok) {
	dce_svc_printf(FTS_S_VLDBINT_CANT_CREATE_SERVER_ENTRY_MSG, dfs_dceErrTxt(code));
	(void)PrintVLError(0, code);
    }
    return code;
}


vldb_AlterServer(siteaddr, alterAttrs)
afsNetAddr *siteaddr;
siteAlter *alterAttrs;
{
    register long code;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    code = ubik_Call(VL_AlterServer, cstruct, 0, siteaddr, alterAttrs);
    if (code != error_status_ok) {
	dce_svc_printf(FTS_S_VLDBINT_CANT_ALTER_SERVER_ENTRY_MSG, dfs_dceErrTxt(code));
	(void)PrintVLError(0, code);
    }
    return code;
}


vldb_AddAddress(oldAddrp, newAddrp)
afsNetAddr *oldAddrp;
afsNetAddr *newAddrp;
{
    register long code;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    code = ubik_Call(VL_AddAddress, cstruct, 0, oldAddrp, newAddrp);
    if (code != error_status_ok) {
	dce_svc_printf(FTS_S_VLDBINT_CANT_ADD_ADDRESS_MSG, dfs_dceErrTxt(code));
	(void)PrintVLError(0, code);
    }
    return code;
}


vldb_RemoveAddress(oldAddrp)
afsNetAddr *oldAddrp;
{
    register long code;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    code = ubik_Call(VL_RemoveAddress, cstruct, 0, oldAddrp);
    if (code != error_status_ok) {
	dce_svc_printf(FTS_S_VLDBINT_CANT_REMOVE_ADDRESS_MSG, dfs_dceErrTxt(code));
	(void)PrintVLError(0, code);
    }
    return code;
}


vldb_ChangeAddress(oldAddrp, newAddrp)
afsNetAddr *oldAddrp;
afsNetAddr *newAddrp;
{
    register long code;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    code = ubik_Call(VL_ChangeAddress, cstruct, 0, oldAddrp, newAddrp);
    if (code != error_status_ok) {
	dce_svc_printf(FTS_S_VLDBINT_CANT_CHANGE_ADDRESS_MSG, dfs_dceErrTxt(code));
	(void)PrintVLError(0, code);
    }
    return code;
}


vldb_GetCellInfo(celldescp)
vlconf_cell *celldescp;
{
    register long code;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    code = ubik_Call(VL_GetCellInfo, cstruct, 0, celldescp);
    if (code != error_status_ok) {
	dce_svc_printf(FTS_S_VLDBINT_NO_BASIC_CELL_INFO_MSG, dfs_dceErrTxt(code));
	(void)PrintVLError(0, code);
    }
    return code;
}


vldb_GetSiteInfo(addrp, siteinfop)
afsNetAddr *addrp;
siteDesc *siteinfop;
{
    register long code;
    afsNetAddr addrCopy;

    addrCopy = *addrp;
    /* Need some generic way of removing the port information */
    if (addrCopy.type == AF_INET) {
	((struct sockaddr_in *) &addrCopy)->sin_port = 0;
    }
    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    code = ubik_Call(VL_GetSiteInfo, cstruct, 0, &addrCopy, siteinfop);
    if (code != error_status_ok) {
	dce_svc_printf(FTS_S_VLDBINT_NO_SITE_INFO_MSG, MapSockAddr(addrp), dfs_dceErrTxt(code));
	(void)PrintVLError(0, code);
    }
    return code;
}


vldb_GenerateSites(startHere, nextStartP, TheseSites, nSites)
unsigned long startHere;
unsigned long *nextStartP;
bulkSites *TheseSites;
u_long *nSites;
{
    register long code;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    code = ubik_Call(VL_GenerateSites, cstruct, 0, startHere, nextStartP, TheseSites, nSites);
    if ((code != 0 && code != VL_ENDOFLIST)) {
	dce_svc_printf(FTS_S_VLDBINT_NO_SITE_LIST_MSG, dfs_dceErrTxt(code));
	(void)PrintVLError(0, code);
    }
    return code;
}


vldb_GetNewVolumeIds(numWanted, ServerAddr, newIDs)
unsigned long numWanted;
afsNetAddr *ServerAddr;
bulkIDs *newIDs;
{
    register long code;

    if (cstruct == NULL) return FTS_ERR_INTERNAL_FLDB;
    code = ubik_Call(VL_GetNewVolumeIds, cstruct, 0, numWanted, ServerAddr, newIDs);
    if (code != error_status_ok) {
	dce_svc_printf(FTS_S_VLDBINT_CANT_RESERVE_FILESET_MSG, numWanted, dfs_dceErrTxt(code));
	(void)PrintVLError(0, code);
    }
    return code;
}

vldb_GetRemoteCellID()
{
    static int done = 0;
    vlconf_cell celldesc;
    int ret;

    if (done == 0) {
	getLocalCellInfo();
	if (!AFS_hiszero(localCellID) && strcmp(localCellName, remoteCellName) == 0) {
	    remoteCellID = localCellID;
	} else {
	    ret = vldb_GetCellInfo(&celldesc);
	    if (ret != 0) return ret;
	    remoteCellID = celldesc.CellID;
	}
	done = 1;
    }
    return 0;
}
