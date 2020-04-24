/* Copyright (C) 1996, 1994 Transarc Corporation - All rights reserved. */
/*
 * This file implements a program that runs in user space and tests the
 * TKM system for race problems.
 *
 * TKM exports 3 pioctl()s to do tkm_GetToken> tkm_ReturnToken and
 * tkm_GetRightsHeld(). This program will start numerous parallel
 * threads performing these operations on different files and volumes
 * To test the results it will maintain internally a list of all the
 * files and tokens that it has tokens for. Every time that a thread
 * gets or returns a token the program will update its internal lists.
 * After updating it will test to ensure that the state of the system
 * is what it expects it to be. The update and test will be done entirelly
 * while holding a single global lock.
 *
 * It must be noted that the purpose of this program is not to test the
 * correctness of individual routines. These will be tested by tkm_unittests.c
 * The result of that is that this program can use routines from libtkm.a
 */

#include "tkm_systest.h"

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/tkm/RCS/tkm_systest_main.c,v 1.5 1996/03/04 19:13:18 ota Exp $")

void * systest_threadPoolHandle;

osi_dlock_t bigLock; /* covers the following 3*/
struct systest_file *firstFile = NULL;
struct systest_token *raceRevokes = NULL;
int pendingRequests = 0;

/* from tkm_tokens.c */

#define FID_ISVOL(fid)	((fid->Vnode ==TKM_AFSFID_WHOLE_VOL_VNODE) \
                       &&(fid->Unique==TKM_AFSFID_WHOLE_VOL_UNIQUE))

struct QdElement {
    afsFid		*fidP;
    int			host;
    afs_hyper_t		tokenID;
    long		drevoke;
    long		daccept;
    struct QdElement	*next;
};

struct QdElement	*QdTokens = NULL;
osi_dlock_t	QdTokensLock;

AddToList (struct systest_token *element, struct systest_token  **list)
{
    osi_assert(lock_WriteLocked(&bigLock));
    element->next = *list;
    *list = element;
}


/* routines done single threaded (protected by bigLock) */


struct systest_file *FindFile(afsFid *fidP)
{
    struct systest_file *ret;

    for (ret = firstFile; ret != NULL; ret = ret->next)
	if (!FidCmp(ret->fid, fidP))
	    return(ret);
    return(NULL);
}

struct systest_file *AddFile(afsFid *fidP)
{
    struct systest_file *ret;

    if ((ret = FindFile(fidP)) != NULL)
	return(ret);
    ret = (struct systest_file *) osi_Alloc (sizeof(struct systest_file));
    ret->fid = fidP;
    ret->granted = NULL;
    ret->next = firstFile;
    firstFile = ret;
    return(ret);
}

/* Must have the BigLock */
FileConflicts(struct systest_file *fileP)

{
    struct systest_token *token1, *token2, *prev = NULL;
    afsFid volFid;

    /*
     * Remove expired tokens before searching for conflicts
     */
    token1 = fileP->granted;
    while (token1 != NULL) {
	if (token1->info->expirationTime <= osi_Time()) {
	    tkm_dprintf(("EXPIRED %u,,%u\n",
			 AFS_HGETBOTH(token1->info->tokenID)));
	    /*
	     * remove from list just like RemoveToken()
	     */
	    if (prev != NULL)
		prev->next = token1->next;
	    else {
		osi_assert(fileP->granted == token1);
		fileP->granted = token1->next;
	    }
	    osi_Free(token1->info, sizeof(afs_token_t));
	    osi_Free(token1, sizeof(struct systest_token));
	    if (prev != NULL)
		token1 = prev->next;
	    else
		token1 = fileP->granted;
	} else {
	    prev = token1;
	    token1 = token1->next;
	}
    }
    for (token1 = fileP->granted; token1 != NULL; token1 = token1->next){
	for (token2 = fileP->granted; token2 != NULL; token2 = token2->next){
	    if (token1->hostId != token2->hostId) {
		if ((TKM_TYPE_CONFLICTS(token1->noRangeTypes)
		     & token2->noRangeTypes) ||
		    ((TKM_TYPE_CONFLICTS(token1->rangedTypes) & token2->rangedTypes) &&
		     (!TKM_DISJOINT_BYTERANGES(&(token1->range),
					       &(token2->range))))){
		    tkm_dprintf(("Inconsistent tokens %x %x for file %x\n",
			   token1, token2, fileP));
		    break_in_debugger();
		    return(1);
		}
	    }
	}
    }
    if (!(FID_ISVOL(fileP->fid))) {
	struct systest_file *vol;

	FidCopy((&volFid), fileP->fid);
	volFid.Vnode = TKM_AFSFID_WHOLE_VOL_VNODE;
	volFid.Unique = TKM_AFSFID_WHOLE_VOL_UNIQUE;
	vol = FindFile(&volFid);
	osi_assert(vol != NULL);
	return(FileConflicts(vol));
    }	else {
	return(0);
    }
}


AddToken(afs_token_t *atokenP,
	 afsFid *fidP,
	 int hostId,
	 long drevoke,
	 long daccept)
{
    struct systest_file *fileP;
    struct systest_token *token;
    long alltypes;

    fileP = FindFile(fidP);
    osi_assert(fileP!= NULL);
    token = (struct systest_token *) osi_Alloc(sizeof(struct systest_token));
    token->hostId = hostId;
    token->info = atokenP;
    token->range.lowBnd = atokenP->beginRange;
    token->range.upBnd = atokenP->endRange;
    alltypes = AFS_hgetlo(atokenP->type);
    token->rangedTypes = TKM_TYPES_WITH_RANGE(alltypes);
    token->noRangeTypes = TKM_TYPES_WITHOUT_RANGE(alltypes);
    token->dont_revoke = drevoke;
    token->dont_accept = daccept;
    AddToList(token, &(fileP->granted));
    tkm_dprintf(("ADDED %x id %u,,%u types %d for %u.%u.%u.%u for host %d\n",
	   token, AFS_HGETBOTH(atokenP->tokenID),
	   alltypes, AFS_hgetlo(fidP->Cell), AFS_hgetlo(fidP->Volume),
	   fidP->Vnode, fidP->Unique, hostId));
    if (FileConflicts(fileP))
	return(1);
    return(0);
}

struct systest_token *RemoveToken(afsRevokeDesc *revoke)
{
    struct systest_file *fileP;
    struct systest_token *token, *prev = NULL;

    fileP = FindFile(&(revoke->fid));
    if (fileP == NULL)
	break_in_debugger();
    osi_assert(fileP!= NULL);
    for (token = fileP->granted; token != NULL; token = token->next) {
	if (AFS_hsame(token->info->tokenID, revoke->tokenID))
	    break;
	prev = token;
    }
    if (token != NULL){
	if (prev !=NULL)
	    prev->next = token->next;
	else{
	    osi_assert(fileP->granted == token);
	    fileP->granted = token->next;
	}
    }
    return(token);
}
/***** Routines that start without the lock ***/

HandleRevoke(afsRevokeDesc *description)
{
    struct systest_token *revoked;
    int delete = 1;
    long code = HS_SUCCESS;

    lock_ObtainWrite(&bigLock);
    revoked = RemoveToken(description);
    if (revoked == NULL) {
	/*
	 * Handle Racing Revokes like the cache manager
	 */
	if (pendingRequests > 0){
	    /* simulate a racing revokes handling */
	    revoked = (struct systest_token *)
		osi_Alloc (sizeof (struct systest_token));
	    /*** add revoked->info ***/
	    revoked->info = osi_Alloc(sizeof(afs_token_t));
	    revoked->info->tokenID = description->tokenID;
	    AddToList(revoked, &raceRevokes);
	} else {
	    struct systest_token *this;
	    /*
	     * to simulate the CM behavior clean up the racing
	     * revokes history
	     */
	    while (raceRevokes != NULL) {
		this = raceRevokes;
		raceRevokes = raceRevokes->next;
		osi_Free(this, sizeof(struct systest_token));
	    }
	}
	lock_ReleaseWrite(&bigLock);
	return (HS_SUCCESS);
    } else {
	long revokedTypes;

	/*
	 * Are there any types we should not return because the token
	 * insisted on keeping them? (asked & cant_have)
	 */
	AFS_hset64(description->errorIDs, 0,
		   AFS_hgetlo(description->type) & revoked->dont_revoke);

	/*
	 * Here is what we _were_ able to revoke. (asked & can have)
	 */
	revokedTypes = AFS_hgetlo(description->type)
	    & ~(AFS_hgetlo(description->errorIDs));

	/*
	 * May still have some rights left in token.  (had & not_revoked)
	 */
	AFS_hset64(revoked->info->type, 0,
		   AFS_hgetlo(revoked->info->type) & ~revokedTypes);
	tkm_dprintf(("REVOKED %u,,%u types %d\n",
		     AFS_HGETBOTH(revoked->info->tokenID), revokedTypes));
	tkm_dprintf(("KEEPING %u,,%u types %u\n",
		     AFS_HGETBOTH(revoked->info->tokenID),
		     AFS_hgetlo(revoked->info->type)));
	/*
	 * If anything left in the token, add it back to the file
	 */
	if (!AFS_hiszero(revoked->info->type)) {
	    struct systest_file *fileP;

	    fileP = FindFile(&(description->fid));
	    osi_assert(fileP!= NULL);
	    revoked->rangedTypes =
		TKM_TYPES_WITH_RANGE(AFS_hgetlo(revoked->info->type));
	    revoked->noRangeTypes =
		TKM_TYPES_WITHOUT_RANGE(AFS_hgetlo(revoked->info->type));
	    AddToList(revoked, &(fileP->granted));
	    delete = 0;
	}
	if (!AFS_hiszero(description->errorIDs))
	    code = HS_ERR_PARTIAL;
    }
    /*
     * Check if offered a smaller range to replace revoked token
     */
    if (description->flags & AFS_REVOKE_COL_A_VALID) {
	afs_token_t *atokenP;

	atokenP = (afs_token_t *) osi_Alloc (sizeof(afs_token_t));
	*atokenP = description->columnA;
	tkm_dprintf(("\t\t\tOffered Column A Adding 0x%x\n",atokenP));
	AddToken(atokenP, &(description->fid), revoked->hostId,
		 revoked->dont_revoke, revoked->dont_accept);
	/*
	 * Inform TKM of choice to keep token by scribbling in Choice
	 */
	description->colAChoice = description->columnA.type;
	tkm_dprintf(("-- %x.colA accepted %u,,%u ---\n", description,
		     AFS_HGETBOTH(description->colAChoice)));
    }

    if (description->flags & AFS_REVOKE_COL_B_VALID) {
	afs_token_t *atokenP;

	atokenP = (afs_token_t *) osi_Alloc (sizeof(afs_token_t));
	*atokenP = description->columnB;
	tkm_dprintf(("\t\t\tOffered Column B Adding 0x%x\n",atokenP));
	AddToken(atokenP, &(description->fid), revoked->hostId,
		 revoked->dont_revoke, revoked->dont_accept);
	/*
	 * Inform TKM of choice to keep token by scribbling in Choice
	 */
	description->colBChoice = description->columnB.type;
	tkm_dprintf(("-- %x.colB accepted %u,,%u ---\n", description,
		     AFS_HGETBOTH(description->colBChoice)));
    }
    lock_ReleaseWrite(&bigLock);
    if (delete) {
	osi_Free(revoked->info, sizeof(afs_token_t));
	osi_Free(revoked, sizeof(struct systest_token));
    }
    return(code);
}

/*
 * These two functions keep track of those token IDs which will be
 * granted asynchronously.
 */

AddQdToken(afs_hyper_t *tokenID,
	   afsFid *fidP,
	   int host,
	   long dontrevoke,
	   long dontaccept)
{
    struct QdElement *new;

    tkm_dprintf(("Qd %u,,%u  host %d\n", AFS_HGETBOTH(*tokenID), host));
    new = osi_Alloc(sizeof(struct QdElement));
    new->fidP = fidP;
    new->host = host;
    new->drevoke = dontrevoke;
    new->daccept = dontaccept;
    new->tokenID = *tokenID;
    lock_ObtainWrite(&QdTokensLock);
    new->next = QdTokens;
    QdTokens = new;
    lock_ReleaseWrite(&QdTokensLock);
}


struct QdElement *RemoveQdToken(afs_hyper_t *tokenID)
{
    struct QdElement	*Qep, *prev = NULL;

    lock_ObtainWrite(&QdTokensLock);
    for(Qep=QdTokens; Qep; Qep=Qep->next) {
	if (AFS_hsame((*tokenID), Qep->tokenID)) {
	    tkm_dprintf(("Remove Qd %u,,%u  host %d\n",
			 AFS_HGETBOTH(Qep->tokenID), Qep->host));
	    if (prev == NULL) {
		osi_assert(Qep == QdTokens);
		QdTokens = Qep->next;
	    } else {
		prev->next = Qep->next;
	    }
	    lock_ReleaseWrite(&QdTokensLock);
	    return(Qep);
	}
	prev = Qep;
    }
    lock_ReleaseWrite(&QdTokensLock);
    return(NULL);
}

/*
 * Handle the asynchronous grant request from rpc
 */
HandleGrant(afs_token_t *atokenP)
{
    int 	code;
    struct QdElement	*Qep;

    lock_ObtainWrite(&bigLock);
    Qep = RemoveQdToken(&atokenP->tokenID);
    osi_assert(Qep != NULL);
    tkm_dprintf(("HANDLEGRANT: Adding async grant to fileP %x host %x\n",
		 Qep->fidP, Qep->host));
    code = AddToken(atokenP, Qep->fidP, Qep->host, Qep->drevoke, Qep->daccept);
    osi_Free(Qep, sizeof(struct QdElement));
    lock_ReleaseWrite(&bigLock);
    return(code);
}


long GetToken(afsFid *fidP,
	 int hostId,
	 int reqNum,
	 long flags,
	 long types,
	 afs_hyper_t *beginRange,
	 afs_hyper_t *endRange,
	 afs_recordLock_t *lockDescriptor,
	 long dontrevoke,
	 long dontaccept,
	 afs_hyper_t *IdP)
{
    struct hs_host *host;
    afs_token_t *atokenP;
    long code;


    host = (struct hs_host *) osi_Alloc(sizeof(struct hs_host));
    bzero((caddr_t) host, sizeof(struct hs_host));
    host->hstopsp = &(systest_hostops);
    atokenP = (afs_token_t *) osi_Alloc (sizeof(afs_token_t));
    bzero(atokenP, sizeof(afs_token_t));
    AFS_hset64(atokenP->type, 0, types);
    atokenP->beginRange = *beginRange;
    atokenP->endRange = *endRange;
    lock_ObtainWrite(&bigLock);
    pendingRequests++;
    lock_ReleaseWrite(&bigLock);
    if ((code= tkm_GetToken(fidP, flags, atokenP,
			    host, reqNum, lockDescriptor)) == TKM_SUCCESS) {
	int alreadyRevoked = 0;
	struct systest_token *revoked, *prev = NULL;

	/*
	 * Save the ID of granted token
	 */
	if (IdP != NULL)
	    *IdP = atokenP->tokenID;

	lock_ObtainWrite(&bigLock);
	/* check for racing revokes */
	revoked = raceRevokes;
	while (revoked != NULL) {
	    if (AFS_hsame(revoked->info->tokenID, atokenP->tokenID)){
		tkm_dprintf(("RACE REVOKED %u,,%u\n",
			     AFS_HGETBOTH(atokenP->tokenID)));
		if (prev != NULL)
		    prev->next = revoked->next;
		else {
		    osi_assert(revoked == raceRevokes);
		    raceRevokes = raceRevokes->next;
		}
		alreadyRevoked = 1;
		osi_Free(atokenP, sizeof(afs_token_t));
		break;
	    }
	    prev = revoked;
	    revoked = revoked->next;
	    code = HS_SUCCESS;
	}
	if (!alreadyRevoked) {
	    code = AddToken(atokenP, fidP, hostId, dontrevoke, dontaccept);
	}
	pendingRequests--;
	lock_ReleaseWrite(&bigLock);
	return(code);
    } else if (code == TKM_ERROR_REQUESTQUEUED) {
	lock_ObtainWrite(&bigLock);
	pendingRequests--;
	lock_ReleaseWrite(&bigLock);
	/*
	 * Save the ID of granted (queued) token
	 */
	if (IdP != NULL)
	    *IdP = atokenP->tokenID;

	AddQdToken(&atokenP->tokenID, fidP, hostId, dontrevoke, dontaccept);
	return (0);
    } else {
	/* XXX Don't know how else to fail */
	lock_ObtainWrite(&bigLock);
	pendingRequests--;
	lock_ReleaseWrite(&bigLock);
	tkm_dprintf( ("tkm_GetToken FAILED (%d)\n", code));
	if (code == EINVAL)
	    /* it's OK we tried to get byte range token on a volume */
	    code = 0;
	return (code);
    }
}

break_in_debugger()
{
    int i = 0;

    char s[10];

    gets(s);
    i = atoi(s) +1;
    return(i);
}



int howManyFiles = 500, howManyVols = 100;

struct systest_file *GetRandomFile()
{
    int i, fnum;
    struct systest_file *fileP = firstFile;

    fnum = lrand48() % (howManyFiles + howManyVols);
    for (i = 0; i < fnum; i++) {
	if (fileP->next != NULL)
	    fileP = fileP->next;
	else
	    fileP = firstFile;
    }
    return(fileP);
}


long GetRandomToken()
{
    afs_hyper_t begin;
    afs_hyper_t end;

    static struct systest_file *rfileP = NULL;


    if (rfileP == NULL)
	rfileP = firstFile;
    AFS_hset64(begin, 0, 1);
    AFS_hset64(end, 0, 2);

    if (rfileP->next != NULL)
	rfileP = rfileP->next;
    else
	rfileP = firstFile;

    return(GetToken(rfileP->fid, 1, 1, 0, 1 /* lock_read*/,
		     &begin, &end, NULL, 0, 0, NULL));

}

/* create n files and their corresponding volumes */

MakeFiles(int files, int volumes)
{
    afsFid *fidP;
    int i;

    fidP = (afsFid *) osi_Alloc ((files + volumes) * sizeof(afsFid));

    for (i = 1; i < files; i++){
	fidP->Cell = tkm_localCellID;
	AFS_hset64(fidP->Volume, 0, (i % volumes));
	fidP->Vnode = i + 3;
	fidP->Unique = 2*i;
	AddFile(fidP);
	fidP++;
    }
    for (i = 0; i < volumes; i++){
	fidP->Cell = tkm_localCellID;
	AFS_hset64(fidP->Volume, 0, i);
	fidP->Vnode = TKM_AFSFID_WHOLE_VOL_VNODE;
	fidP->Unique = TKM_AFSFID_WHOLE_VOL_UNIQUE;
	AddFile(fidP);
	fidP++;
    }
}


TestParallelRevokes(int howmany)
{
    afsFid fid, volFid;
    afs_hyper_t begin;
    afs_hyper_t end;
    afsRevokeDesc desc;
    struct systest_file * fileP;
    struct systest_file *vol;
    struct systest_token *revoked;
    long code;
    int x;

    FidCopy((&volFid), (&fid));
    volFid.Vnode = TKM_AFSFID_WHOLE_VOL_VNODE;
    volFid.Unique = TKM_AFSFID_WHOLE_VOL_UNIQUE;
    AddFile(&fid);
    AddFile(&volFid);
    AFS_hset64(begin, 0, 1);
    AFS_hset64(end, 0, 2);
    /*
     * first get n compatible tokens that will be revoked once the
     * non-compatible one is requested
     */

    for (x = 0; x < howmany ; x++) {
	if (GetToken(&fid, x, 1, 0, 1 /* lock_read*/,
		     &begin, &end, NULL, 0, 0, NULL))
	    return(1);
    }
    fileP = FindFile(&fid);
    osi_assert(fileP!= NULL);
    /* now cause all those revocations to happen */
    if (GetToken(&fid, howmany , 1, 0, 2 /* lock_write */
		 , &begin, &end, NULL, 0, 0, NULL))
	return(1);
    return(0);
}

TestManyParallelRevokes()
{
    TestParallelRevokes(1);
    TestParallelRevokes(TKM_MAXPARALLELRPC -1 );
    TestParallelRevokes(TKM_MAXPARALLELRPC);
    TestParallelRevokes(TKM_MAXPARALLELRPC + 1 );
}

long systest_globError = 0;

void tkm_AsyncTest(long *reps)
{
    int i;
    long code;

    for (i=0;i<*reps;i++)
	if ( code = AsyncGrantTest()){
	    systest_globError = code;
	    return;
	}
}

void tkm_ParallelReqTest(long *reps)
{
    int i;
    long code;

    systest_threadPoolHandle = tpq_Init(2,2,2,30, "systest_parconf");

    for (i=0;i<*reps;i++)
	if ( code = ParallelConflict()){
	    systest_globError = code;
	    return;
	}
}


main(int argc, char **argv)
{
    int code, i;
    char	*pname;
    long *aReps, *pReps;
    long reps = 20000;
    int try = 0;

    tkm_Init();
    cm_Init();
    MakeFiles(howManyFiles, howManyVols);
    printf("made the files\n");

    pname = *argv++;
    argc--;
    if (argc < 1) {
	fprintf(stderr, "%s: must specify test to run [combo|pconf|async|revokes|fail]\n",
		pname);
	exit(1);
    }
    switch(*argv[0]) {
      case 'c':
	/*
	 * this is the big test where a thread is continiously exercising the
	 * parallel conflicting requests, one is exercising the asyc grants
	 * and the main thread is exhausting the system resources as fast as
	 * it can by randomly asking for tokens for every single file and volume
	 * in the system
	 */
	aReps = (long *) osi_Alloc(sizeof(long));
	pReps = (long *) osi_Alloc(sizeof(long));
	*aReps = reps;
	*pReps = reps;
	/*osi_ThreadCreate(tkm_AsyncTest,       (void *) aReps, 0, 0,"atest", code);*/
	osi_ThreadCreate(tkm_ParallelReqTest, (void *) pReps, 0, 0,"ptest", code);
	for (i = 0; i <reps; i++) {
	    if (code = GetRandomToken()){
		if (code == ENOMEM) {
		    /*
		     * this could have have happened because the async
		     * grant attempts queued are holding too many files
		     * so we'll give the system a chance to clean this
		     * queue
		     */
		    if (try < 10) {
			try++;
			osi_Pause(10);
			continue;
		    }
		}
		break_in_debugger();
		return(code);
	    } else
		try = 0;
	}
	break;



      /* forever running tests */
      case 'p':
	systest_threadPoolHandle = tpq_Init(2,2,2,30, "systest_parconf");
	while(1)
	    ParallelConflict();
	break;

      case 'a':
	while (1)
	    AsyncGrantTest();
	break;

      /* once running tests */
      case 'r':
	code = TestManyParallelRevokes();
	break;

      case 'f':
	code = hs_failureTests();
	break;

      default:
	fprintf(stderr, "Must specify a legal test name\n");
	exit(1);
    }
    return(code);
}
