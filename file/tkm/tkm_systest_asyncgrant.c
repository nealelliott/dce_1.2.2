/* Copyright (C) 1996, 1994 Transarc Corporation - All rights reserved. */

#include "tkm_systest.h"

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/tkm/RCS/tkm_systest_asyncgrant.c,v 1.4 1996/03/04 19:13:17 ota Exp $")

extern osi_dlock_t bigLock; /* covers the following 3*/

/*
 * Return a token to TKM, after removing it from its file.
 */
ReturnToken(struct systest_file *fileP, afs_hyper_t *IdP)
{
    struct systest_token *tokenP, *retP, *prev = NULL;
    long code;

    lock_ObtainWrite(&bigLock);
    for (tokenP = fileP->granted; tokenP != NULL; tokenP = tokenP->next) {
	if (AFS_hsame(tokenP->info->tokenID, *IdP)) {
	    tkm_dprintf(("RETURNING %u,,%u\n",
			 AFS_HGETBOTH(tokenP->info->tokenID)));
	    /*
	     * remove from list just like RemoveToken()
	     */
	    if (prev != NULL) {
		prev->next = tokenP->next;
	    } else {
		osi_assert(fileP->granted == tokenP);
		fileP->granted = tokenP->next;
	    }
	    retP = tokenP;
	    break;
	} else {
	    prev = tokenP;
	}
    }
    code = tkm_ReturnToken(fileP->fid, (afs_hyper_t *)IdP,
			   (afs_hyper_t *)&retP->info->type, 0);
    lock_ReleaseWrite(&bigLock);
    osi_Free(retP->info, sizeof(afs_token_t));
    osi_Free(retP, sizeof(struct systest_token));
    return(code);
}

FindTokenInFile(struct systest_file *fileP, afs_hyper_t *IdP)
{
    struct systest_token *tokenP;

    for (tokenP = fileP->granted; tokenP != NULL; tokenP = tokenP->next) {
	if (AFS_hsame(tokenP->info->tokenID, *IdP)) {
	    return(1);
	}
    }
    return(0);
}

/****************************************************
  The following code is needed to generate sequential conflicting
  token requests, with revoke refusal and asynch grant.
  *****************************************************/

AsyncGrantTest()
{
    long t1, t2;
    afs_hyper_t b1, b2, e1, e2;
    struct systest_file *fileP ;
    afs_hyper_t Id, AsyncID;
    int h = 0, code, found = 0, i;

    /*
     * Choose a file, any file
     */
    fileP = GetRandomFile();
    MakeConflictingTypes(&t1, &t2);
    MakeOverlappingRanges(&b1, &e1, &b2, &e2);


    code = GetToken(fileP->fid, 1, 0, 0,
		    t1, &b1, &e1, NULL, t1, 0, &Id);
    if (code != TKM_SUCCESS) {
	tkm_dprintf(("\tGetToken 1 FAILED\n"));
	return(1);
    }

    code = GetToken(fileP->fid, 2, 0, TKM_FLAGS_QUEUEREQUEST,
		    t2, &b2, &e2, NULL, 0, 0, &AsyncID);
    if (code != TKM_ERROR_REQUESTQUEUED) {
	tkm_dprintf(("\tGetToken 2 FAILED\n"));
	return(1);
    }

    /*
     * Expect an asynchronous
     * grant right after we return the conflicting token!
     */
    code =  ReturnToken(fileP, &Id);
    if (code != TKM_SUCCESS)
	break_in_debugger();

    /*
     * Wait for the grant to come down
     */
    i = 0;
    while (!found && i < 30) {
	lock_ObtainWrite(&bigLock);
	found = FindTokenInFile(fileP, &AsyncID);
	lock_ReleaseWrite(&bigLock);
	if (!found)
	    osi_Pause(1);
    }
    if (!found) {
	printf("\t\t\tFAILURE\n");
	return(-1);
    }
    printf("\t\t\tSUCCESS\n");
    return(0);
}
