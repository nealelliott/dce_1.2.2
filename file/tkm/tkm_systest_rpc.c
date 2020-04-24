/* Copyright (C) 1996, 1994 Transarc Corporation - All rights reserved. */

#include "tkm_systest.h"

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/tkm/RCS/tkm_systest_rpc.c,v 1.5 1996/03/04 19:13:22 ota Exp $")

/* for RPC simulation */
osi_dlock_t asyncComLock;

struct hostops systest_hostops = {
    HoldHost,
    ReleHost,
    LockHost,
    UnLockHost,
    RevokeToken,
    GrantToken
};

struct systest_revoke {
    struct systest_revoke *next;
    afsRevokeDesc *description;
    int flags;
} *revokeQ = NULL;

struct systest_asyncGrant {
    struct systest_asyncGrant *next;
    afs_token_t	*external;

} *asyncGrantQ = NULL;

/* consumer is single threaded */

#define MARK_LAST_REVOKE 1  /* hopefully unused description flag */
#define MARK_PARTIAL_FAILURE 2  /* hopefully unused description flag */

void AsyncQConsumer()
{
    struct systest_revoke *revoke;
    struct systest_asyncGrant *grant;
    int flag = 0;

    while (1) {
	lock_ObtainWrite(&asyncComLock);
	while (revokeQ != NULL){
	    printf(" In here revokeQ is %x\n", revokeQ);
	    revoke = revokeQ;
	    revokeQ = revokeQ->next;
	    if (revoke->flags & MARK_LAST_REVOKE){
		/*
		 * it's the dummy description so wakeup the
		 * thread who's descriptions have all been consumed
		 */
		printf ("processing mark %x. next rq is %x\n",
			revoke, revokeQ);
		revoke->flags = (flag==HS_ERR_PARTIAL)?MARK_PARTIAL_FAILURE:0;
		osi_Wakeup(revoke);
		lock_ReleaseWrite(&asyncComLock);
	    } else {
		printf ("processing rev %x desc %x of %d.%d. next rq is %x\n",
			revoke,
			revoke->description,
			AFS_HGETBOTH(revoke->description->tokenID),
			revokeQ);
		lock_ReleaseWrite(&asyncComLock);
		if ((HandleRevoke(revoke->description)) == HS_ERR_PARTIAL)
		    flag = HS_ERR_PARTIAL;
		printf ("done processing %x. next rq is %x\n",
			revoke, revokeQ);
		osi_Free(revoke,sizeof( struct systest_revoke));
	    }
	    lock_ObtainWrite(&asyncComLock);
	}
	while (asyncGrantQ != NULL){
	    grant = asyncGrantQ;
	    asyncGrantQ = asyncGrantQ->next;
    	    lock_ReleaseWrite(&asyncComLock);
	    /* HandleGrant will preserve the afs_token_t in external */
	    HandleGrant(grant->external);
	    osi_Free(grant,sizeof( struct systest_asyncGrant));
	    lock_ObtainWrite(&asyncComLock);
	}
	osi_SleepW(&revokeQ,&asyncComLock);
	flag = 0;
    }
}

/*
 * the following two are the producers and are called from the tkm
 * server side
 */

int glob;

long RevokeToken(struct hs_host *h, int howMany, afsRevokeDesc *revokes)
{
    struct systest_revoke *thisOne, *mark;
    int i;
    int counter= glob++;

    mark = (struct systest_revoke *)
	    osi_Alloc(sizeof(struct systest_revoke));
    /* put the dummy marker first */
    mark->flags = MARK_LAST_REVOKE;
    lock_ObtainWrite(&asyncComLock);
    mark->next = revokeQ;
    mark->description = NULL;
    revokeQ = mark;
    printf ("%d MarK is %x \n",	counter, mark);
    for (i = 0; i < howMany; i++){
	thisOne = (struct systest_revoke *)
	    osi_Alloc(sizeof(struct systest_revoke));
	thisOne->description = revokes;
	printf ("%d adding to rq %x %d.%d\n",
		counter, revokes, AFS_HGETBOTH(revokes->tokenID));
	thisOne->flags = 0;
	thisOne->next = revokeQ;
	revokeQ = thisOne;
	revokes++;
    }
    /*
     * the async consumer could have already been running and consuming our
     * revokes.  When he's done, he will adjust our Mark token and let us
     * know how well he did.
     */
    osi_Wakeup(&revokeQ);
    /* now we have to wait until they all consumed */
    do {
	osi_SleepW(mark, &asyncComLock);
	lock_ObtainWrite(&asyncComLock);
    } while ((mark->flags & MARK_LAST_REVOKE) != 0);
    i = mark->flags;
    osi_Free(mark, sizeof(struct systest_revoke));
    mark = NULL;
    lock_ReleaseWrite(&asyncComLock);
    printf("%d is done (%x)\n", counter, i);
    if (i & MARK_PARTIAL_FAILURE)
	return(HS_ERR_PARTIAL);

    return(HS_SUCCESS);
}

long GrantToken(struct hs_host *h,
		struct hs_tokenDescription *desc,
		int reqnum)
{
    struct systest_asyncGrant *grant;
    afs_token_t  *external = &desc->token;

    grant = (struct systest_asyncGrant *)
	osi_Alloc(sizeof(struct systest_asyncGrant));

    grant->external = (afs_token_t *) osi_Alloc(sizeof(afs_token_t));
    *grant->external = *external;
    /*
     * for now, toss the fid even though it is hidden under the
     * hs_tokenDescription.  Later, extract it here and stuff into the grantQ.
     */
    lock_ObtainWrite(&asyncComLock);
    grant->next = asyncGrantQ;
    asyncGrantQ = grant;
    lock_ReleaseWrite(&asyncComLock);
    osi_Wakeup(&revokeQ);
    /* no reason to wait for anything */
    return(HS_SUCCESS);
}

long LockHost(struct hs_host *h)
{
    lock_ObtainWrite(&(h->lock));
}

long UnLockHost(struct hs_host *h)
{
    lock_ReleaseWrite(&(h->lock));
}

long HoldHost(struct hs_host *h)
{
    LockHost(h);
    h->refCount++;
    UnLockHost(h);
    tkm_dprintf(("heldhost %x\n", h));
}

long ReleHost(struct hs_host *h)
{
    int freeIt = 0;

    LockHost(h);
    h->refCount--;
    osi_assert(h->refCount >=0);
    tkm_dprintf(("relehost %x rcount %d\n", h, h->refCount));
    if (h->refCount == 0)
	freeIt = 1;
    UnLockHost(h);
    if (freeIt)
	osi_Free(h, sizeof(struct hs_host));
}

int cm_inited = 0;
cm_Init()
{
    int code;

    if (!cm_inited) {
	cm_inited = 1;
	code = osi_ThreadCreate(AsyncQConsumer, (void *)0, 0, 0,
		     "CM listener", code);
    }
}



/*
 * All the 'exotic' errors (HS_SUCCESS and HS_ERR_PARTIAL are the
 * common case). Of those only HS_ERR_TRYAGAIN is something that
 * TKM tries to handle differently. Since we currently do not do
 * anything about grant failures (we always act as if they succeeded)
 * this is not tested
 */

long allHsErrs[] = { HS_ERR_BADTOKEN, HS_ERR_HOSTDOWN,
		   HS_ERR_NOENTRY, HS_ERR_REVFAIL,
		   HS_ERR_JUSTKIDDING, HS_ERR_TRYAGAIN, HS_ERR_OBSOLETE};

long revokeFailure = 0; /*
			 * used only for testing how TKM handles HS_REVOKE failures
			 * The test using it is single threaded
			 */

long grantFailure = 0; /* no need to test this at this point */


int systest_OKerr(long rfail, long code)
{
    if (rfail == 0){
	printf("RFAIL is 0 this test is not applicable\n");
	return(1);
    }

    if (rfail == HS_ERR_TRYAGAIN)
	return (code == TKM_ERROR_TRYAGAIN);
    else
	return (code == TKM_ERROR_HARDCONFLICT);
}



hs_failureTests()
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
    tkm_Init();
    cm_Init();
    AFS_hset64(begin, 0, 1);
    AFS_hset64(end, 0, 2);

    /* setup the fake revoke and async grant functions */
    systest_hostops.hs_revoketoken = FailRevoke;
    systest_hostops.hs_asyncgrant = FailGrant;

    /* first get a token on the file */
    if (GetToken(&fid, 1000 , 1, 0, 2 /* lock_write */
		 , &begin, &end, NULL, 0, 0, NULL))
	return(1);

    /*
     * first get n compatible tokens that will be revoked once the
     * non-compatible one is requested
     */

    for (x = 0; x < (sizeof(allHsErrs)/ sizeof(long)) ; x++) {
	revokeFailure = allHsErrs[x];
	code = GetToken(&fid, x, 1, 0, 1 /* lock_read*/,
		     &begin, &end, NULL, 0, 0, NULL);
	printf("%d %d\n", revokeFailure, code);
	if (!(systest_OKerr(revokeFailure, code))){
	    printf("INCORRECT ERROR HANDLING\n");
	    return(1);
	}

    }
    return(0);
}


long FailRevoke()
{
    return(revokeFailure);
}

long FailGrant()
{
    return(grantFailure);
}
