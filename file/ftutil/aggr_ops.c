/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/common_data.h>
#include <dcedfs/queue.h>
#include <dcedfs/syscall.h>
#include <dcedfs/ag_init.h>
#include <dcedfs/cmdAux.h>
#include <dcedfs/ftutil.h>
#include <dcedfs/osi_net.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/tkm_errs.h>
#include <dcedfs/vol_errs.h>
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */
#include <dce/dce_cf.h>

#include "dcedfs/astab.h"
#include "aggrDesc.h"
#include "volDesc.h"
#include <fcntl.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftlmac.h>
#include <dfsftlsvc.h>
#include <dfsftlmsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/ftutil/RCS/aggr_ops.c,v 1.36 1996/10/16 19:41:25 gait Exp $")

#define FSET_STATUS_CHUNK_SIZE	32

struct attlist {
    struct attlist *next;
    struct astab *astabp;
    struct ufs_astab *fstabp;
}; 

int attached _TAKES((struct astab *astabp,
			     struct attlist **attlistpp,
			     int *errp));

int unattachl _TAKES((char *mntp, int parm_all, char * prg_name, char *parm_type, int parm_force));

long detachaggr _TAKES((struct sockaddr *servAddrP, u_long aggrId, char *prg_name, char *principalName, int parm_force));
/*
 * AggrTypeToString - convert the numeric type to a character string
 * Note: this is taken out of newft.c. It should become part of a libray
 * in the next release.
 */
char *AggrTypeToString(type)
    unsigned type;
{
    static char *ag_types[MAX_AG_TYPE] = {
        "unknown",
        "ufs",
        "lfs", /* was "episode" */
        "jfs",
    };

    return (type < MAX_AG_TYPE) ? ag_types[type] : ag_types[0];
}

/*
 * Return 1 if any aggregate is attached (lfs or otherwise)
 * Return 0 on error or if no aggregates are attached.
 */
int any_aggregate_attached()
{
    struct aggrList *aggrs = 0;
    unsigned numAggrs = 0;
    int code;

    code = ftu_ListAggrs(&aggrs, &numAggrs);
    if (code)
      dce_svc_printf(FTL_S_AG_ANY_AGGR_ATTACH_MSG, code);
    return (numAggrs > 0);
}

episode_aggregate_attached()
{
    struct aggrList *aggrs = 0;
    unsigned numAggrs = 0;
    register int  i;
    int code;

    if (code = ftu_ListAggrs(&aggrs, &numAggrs)){
		dce_svc_printf(FTL_S_AG_EPI_AGGR_ATTACH_MSG,code);
		return(0);
    }

    for(i = numAggrs - 1; i >= 0; --i) {
        if (!strcmp (AggrTypeToString(aggrs[i].type), "lfs")) {
    		if (aggrs) osi_Free(aggrs,numAggrs * sizeof(aggrs[0]));
			return(1);
		}
	}
    if (aggrs) osi_Free(aggrs,numAggrs * sizeof(aggrs[0]));
	return(0);
}

/*
 * Searches the dfsatab entry to make sure the aggregate isn't there
 * (i.e. not already attached).  If astabp is null this routine prints out
 * all the attached aggregates/volumes.
 */
int attached(astabp, attlistpp, errp)
    register struct astab *astabp;
    struct attlist **attlistpp;
    int *errp;
{
    struct aggrList *aggrs = 0;
    unsigned numAggrs = 0;
    struct astab astab, *tstabp;
    struct ufs_astab fstab, *tfstabp;
    struct stat status;
    static char buffer1[256];
    register int found = 0, i;
    struct attlist *attp;
    int code;
    int headerPrinted = 0;

    if (errp) *errp = 0;
    if (code = ftu_ListAggrs(&aggrs, &numAggrs)){
	dce_svc_printf(FTL_S_AG_ATTACH_MSG,code);
	return(0);
    }
    if (attlistpp)
	*attlistpp = 0;

    for(i = numAggrs - 1; i >= 0; --i) {
        /* fill in the astab */
	strcpy(astab.as_spec,aggrs[i].dvname);
        strcpy(astab.as_aggrName, aggrs[i].name);
        strcpy(astab.as_type, AggrTypeToString(aggrs[i].type));
        astab.as_aggrId = aggrs[i].Id;

        bzero(&fstab, sizeof(struct ufs_astab));

        if (!strcmp (astab.as_type, ASTABTYPE_UFS)){
	    unsigned index = 0;
	    struct vol_status ftStatusArray[2]; /*always 1, just as a sanity check*/
	    long code;
	    unsigned fsetsReturned;

	    code = ftu_AggrEnumerateFsets(astab.as_aggrId, &index,
				      ftStatusArray, 2,
				      &fsetsReturned);
	    if (!code && (fsetsReturned == 1)){
		fstab.uas_volId = ftStatusArray[0].vol_st.volId;
	    } else {
		if (code) {
		    dce_svc_printf(FTL_S_AG_VOLID_MSG, astab.as_spec, code);
		} else {
		    dce_svc_printf(FTL_S_AG_UFS_AGGR_MSG,
			    astab.as_spec, fsetsReturned);
		}
	    }
	}

	if (attlistpp) {
	    /*
	     * First create a copy of the astab entry in stabp; below will add it
	       * to the in-memory linked list of all attached aggregates
	       */
	    tstabp = (struct astab *) malloc(sizeof(struct astab));
	    *tstabp = astab;
	    tfstabp = (struct ufs_astab *) malloc(sizeof(struct ufs_astab));
	    *tfstabp = fstab;

	    attp = (struct attlist *) malloc(sizeof(struct attlist));
	    attp->astabp = tstabp;
	    attp->fstabp = tfstabp;
	    attp->next = *attlistpp;
	    *attlistpp = attp;
	} else {
	    if (astabp) {
		if (!strcmp(astabp->as_spec, astab.as_spec) &&
		    !strcmp(astabp->as_aggrName, astab.as_aggrName) &&
		    !strcmp(astabp->as_type, astab.as_type) &&
		    astabp->as_aggrId == astab.as_aggrId) {
		    found = 1;
		    break;
		}
		if (strcmp(astabp->as_spec, astab.as_spec) == 0
		    || strcmp(astabp->as_aggrName, astab.as_aggrName) == 0
		    || astabp->as_aggrId == astab.as_aggrId) {
		    if (errp) *errp = 1;
		    break;
		}
	    } else {
		if (headerPrinted == 0) {
		    fprintf(stdout, "%-20s %-20s %-7s %-10s %s\n",
			    "Aggregate Device", "Name", "Type", "Aggr ID", 
			    "Non-LFS fileset ID");
		    headerPrinted = 1;
		}
		printastab(&astab, &fstab, stdout);
		fprintf(stdout, "\n");
	    }
	}
    }

    if (aggrs) osi_Free(aggrs,numAggrs * sizeof(aggrs[0]));
    return (found);
}



int unattachl(mntp,parm_all,prg_name, parm_type,parm_force)
    char *mntp, *prg_name, *parm_type;
	int	 parm_all, parm_force;
{
    struct astab *astabp;
    struct ufs_astab *fstabp;
    struct attlist *attp, *tattp;
    char tmpatab[500];
    int found, error = 0;
    long code;
    unsigned long status;
    char* cellName = 0;
    extern char currentCellName[];
    char servName[OSI_MAXHOSTNAMELEN + 1];
    struct sockaddr_in servAddr;
    struct hostent *hostentP;
	char* principalName = 0;

    if (gethostname(servName, OSI_MAXHOSTNAMELEN + 1) == -1) {
	dce_svc_printf(FTL_S_AG_NO_HOST_NAME_MSG, prg_name,
		strerror(errno));
	return(1);
    }

    if ((hostentP = gethostbyname(servName)) == NULL) {
	dce_svc_printf(FTL_S_AG_NO_HOST_ENTRY_MSG, prg_name,
		strerror(errno));
	return(1);
    }

    bzero(&servAddr, sizeof servAddr);
    servAddr.sin_family = AF_INET;
    servAddr.sin_addr = *((struct in_addr *) hostentP->h_addr);

    dce_cf_get_cell_name(&cellName, &status);
    if (status != dce_cf_st_ok) {
	dce_svc_printf(FTL_S_AG_NO_CELL_NAME_MSG, prg_name,
		dfs_dceErrTxt(status));
	return(1);
    }
    strcpy(currentCellName, cellName);
    free(cellName);

    dce_cf_get_host_name(&principalName, &status);
    if (status != dce_cf_st_ok) {
	dce_svc_printf(FTL_S_AG_NO_DCE_HOST_MSG, prg_name,
		dfs_dceErrTxt(status));
	return(1);
    }

    attached((struct astab *)0, &tattp, 0);
    found = 0;
    for (attp = tattp; attp; attp = attp->next) { 
	astabp = attp->astabp;
	fstabp = attp->fstabp;
	if (!parm_all) {
	    if (strcmp(astabp->as_spec, mntp)
		&& strcmp(astabp->as_aggrName, mntp)
		&& strcmp(fstabp->uas_mountedon, mntp))
		continue;
	}
	if (parm_type[0] && strcmp(astabp->as_type, parm_type))
	    continue;	
	found = 1;
	code = detachaggr((struct sockaddr *)&servAddr, astabp->as_aggrId,prg_name, principalName,parm_force);
	if (!code) {
	    attp->astabp = (struct astab *)0;	/* Indicates "unattached" */
	} else {
	    dce_svc_printf(FTL_S_AG_DETACH_FAILED_MSG, prg_name,
		    astabp->as_spec, astabp->as_aggrName, dfs_dceErrTxt(code));
	    error = 1;
	}
    }

    free(principalName);

    if (!found) {
	if (parm_all) {
	    dce_svc_printf(FTL_S_AG_NO_DETACH_MSG, prg_name);
	    return(2);
	} else {
	    dce_svc_printf(FTL_S_AG_AGGR_NOT_FOUND_MSG, prg_name, mntp);
	    return(1);
	}
    }
    return(error);
}


long detachaggr(servAddrP, aggrId, prg_name,principalName, parm_force)
    struct sockaddr *servAddrP;
    u_long aggrId;
	char	*prg_name, *principalName;
	int		parm_force;
{
    long code = 0, code2 = 0;
    unsigned fsetIndex = 0;
    struct vol_status fsetStatusArray[FSET_STATUS_CHUNK_SIZE];
    unsigned numFsetsReturned;
    unsigned i;
    struct tokenInfo {
	afs_hyper_t fsetId;
	int tokenDesc;
	int readOnly;
    };
    struct tokenListChunk {
	struct tokenListChunk *next;
	unsigned tokenArrayLength;
	struct tokenInfo *tokenArray;
    } *tokenList = NULL, *tlp, *next;
    unsigned arraySize;
    u_long tokens;
    u_long gtFlags;
    u_long ftuFlags;
    int printDone = 1;
    char* noTokens = getenv("DFSEXPORT_NO_TOKENS");

    if (!noTokens) {
	dce_svc_printf(FTL_S_AG_TOKEN_REVOKE_MSG, prg_name, aggrId);
	
	do {
	    code = ftu_AggrEnumerateFsets(aggrId, &fsetIndex,
					  fsetStatusArray,
					  FSET_STATUS_CHUNK_SIZE,
					  &numFsetsReturned);
	    if (code)
		if (parm_force) {
		    dce_svc_printf(FTL_S_AG_FAIL_TO_ENUM_MSG, prg_name,
			    aggrId);
		    code = 0;
		    break;
		} else
		    goto done;
	  
	    tlp = (struct tokenListChunk *)osi_Alloc(sizeof *tlp);
	    tlp->tokenArrayLength = numFsetsReturned;
	    arraySize = numFsetsReturned * sizeof *tlp->tokenArray;
	    tlp->tokenArray = (struct tokenInfo *)osi_Alloc(arraySize);
	    tlp->next = tokenList;
	    tokenList = tlp;

	    for (i = 0; i < numFsetsReturned; i++) {
		tlp->tokenArray[i].fsetId = fsetStatusArray[i].vol_st.volId;
		tlp->tokenArray[i].tokenDesc = 0;
		/* Don't attempt to revoke tokens for readonly filesets */
		/* Also ignore non-exported filesets */
		tlp->tokenArray[i].readOnly =
		    (fsetStatusArray[i].vol_st.states
		     & (VOL_READONLY | VOL_NOEXPORT));
	    }
	} while (numFsetsReturned == FSET_STATUS_CHUNK_SIZE);
	
	tokens = TKN_DATA_READ|TKN_DATA_WRITE|TKN_STATUS_READ|TKN_STATUS_WRITE;
	if (!parm_force)
	    tokens |= TKN_OPEN_DELETE;
	gtFlags = AFS_FLAG_FORCEVOLQUIESCE;
	if (parm_force)
	    gtFlags |= AFS_FLAG_FORCEREVOCATIONS;
	
	for (tlp = tokenList; tlp != NULL; tlp = tlp->next) {
	    for (i = 0; i < tlp->tokenArrayLength; i++) {
		if (tlp->tokenArray[i].readOnly)
		    continue;

		code = fts_GetToken((afsNetAddr *)servAddrP, principalName,
				    &tlp->tokenArray[i].fsetId, tokens,
				    gtFlags, &tlp->tokenArray[i].tokenDesc);
		/* Ignore failures to revoke tokens on deleted/moved fsets */
		if (code && code != VOLERR_PERS_DELETED) {
		    dce_svc_printf(FTL_S_AG_COULD_NOT_REVOKE_MSG,
			    AFS_hgethi(tlp->tokenArray[i].fsetId),
			    AFS_hgetlo(tlp->tokenArray[i].fsetId),
			    dfs_dceErrTxt(code));

		    if (parm_force){
			dce_svc_printf(FTL_S_AG_PROCEED_WITH_DETACH_MSG);
			code = 0;
		    } else {
			if (code == TKM_ERROR_TOKENCONFLICT)
			    code = EBUSY;
			goto done;
		    }
		}
	    }
	}
    }

    ftuFlags = FTU_ATTACH_FLAGS_MOUNTAGGR;
    if (parm_force)
	ftuFlags |= FTU_ATTACH_FLAGS_FORCEDETACH;
    code = ftu_DetachAggr(aggrId, ftuFlags);

done:
    if (!noTokens) {
	for (tlp = tokenList; tlp != NULL; tlp = next) {
	    for (i = 0; i < tlp->tokenArrayLength; i++)
		if (tlp->tokenArray[i].tokenDesc != 0 &&
		    !tlp->tokenArray[i].readOnly &&
		    (code2 = fts_ReleaseToken(tlp->tokenArray[i].tokenDesc))) {
		    dce_svc_printf(FTL_S_AG_FAIL_TO_REL_TOK_MSG,
			    AFS_hgethi(tlp->tokenArray[i].fsetId),
			    AFS_hgetlo(tlp->tokenArray[i].fsetId),
			    dfs_dceErrTxt(code2));

		    if (code == 0) code = code2;
		}
	
	    arraySize = tlp->tokenArrayLength * sizeof *tlp->tokenArray;
	    osi_Free(tlp->tokenArray, arraySize);
	    next = tlp->next;		/* Save next pointer before freeing */
	    osi_Free(tlp, sizeof *tlp);
	}
    }

    return code;
}

