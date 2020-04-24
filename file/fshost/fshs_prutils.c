/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 *      Copyright (C) 1996, 1990 Transarc Corporation
 *      All rights reserved.
 */
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/debug.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_cred.h>
#include <dcedfs/lock.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/xcred.h>
#include <dcedfs/dacl.h>
#include <fshs.h>
#include <fshs_trace.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/fshost/RCS/fshs_prutils.c,v 4.43 1996/02/23 19:30:39 cfe Exp $")

static long initPAC = 0;		/* whether unauthPAC is inited */
static long initPA = 0;			/* whether unauthPA is inited */

#if (defined(AFS_SUNOS5_ENV) && !defined(AFS_SUNOS55_ENV))
/* use short value here, since UFS only stores 16 bits on disk.  Now,
 * it may look like 32 bits are stored, and they are, for a while, but
 * not on disk, and when the UFS inode gets recycled, the bits revert
 * to 65534.  So, start it off there.
 * Actually, SunOS 5 also stores a 32-bit ID in the inode, but before SunOS 5.5
 * the comparison that uses it is signed, so our value of ((int32)-2) was being
 * truncated to 16 bits.  As of SunOS 5.5, that comparison is unsigned and
 * ((int32)-2) will again work.
 */
static int32 unauthID = 65534;		/* Anonymous user ID */
/*
 * UFS won't care about a realm ID, so it doesn't have to be restricted
 * to 16 bits.
 */
static int32 unauthRealmID = -2;
#else /* (defined(AFS_SUNOS5_ENV) && !defined(AFS_SUNOS55_ENV)) */
static int32 unauthID = -2;		/* Anonymous user ID */
/* Non-SunOS uses the same constant for realm, too: */
#define unauthRealmID unauthID
#endif /* (defined(AFS_SUNOS5_ENV) && !defined(AFS_SUNOS55_ENV)) */

static sec_id_pac_t unauthPAC;		/* un-authenticated PAC */
sec_id_pa_t	unauthPA;		/* un-authenticated PA */

/*
 * NOTE: IF uid_t and gid_t are not 32-bits long, creating uids or gids by
 *        using bcopy (below) may create a problem !!
 */

static void
fshs_getcred(
  struct fshs_principal *princp,
  osi_cred_t **credpp)
{
    uid_t uid;
    gid_t gid;
    osi_cred_t *newcred = 0;
    unsigned32 st;
    int i, j;
    sec_id_pa_t *epacp;

    newcred = crget();
    osi_assert(princp->delegationChain);
    epacp = princp->delegationChain->lastDelegate->pacp;
    osi_assert(epacp);
    /* Calling it the local cell will affect only the authn-level checks. */
    if (uuid_equal((uuid_p_t)&dacl_localCellID, &epacp->realm.uuid, &st))
	princp->hostp->flags |= FSHS_HOST_LOCALCELL;
    if (princp->delegationChain->lastDelegate->authenticated &&
	uuid_equal((uuid_p_t)&dacl_localCellID, &epacp->realm.uuid, &st)) {
	/* 
	 * DFS takes the first 32 bits of UUID as UID/GID !
	 */ 
	uid = *((uid_t *)&epacp->principal.uuid);
	gid = *((gid_t *)&epacp->group.uuid);
	osi_SetUID(newcred, uid);
	osi_SetGID(newcred, gid);
	/*
	 * Fill with the first min[OSI_AVAILGROUPS, princp->pacNumGroups] gids
	 */
	for (i = 0, j = 0; i < (int)epacp->num_groups && j < OSI_AVAILGROUPS; i++) {
	    if (uuid_is_nil(&epacp->groups[i].uuid, &st))
	        continue;
	    gid = *((gid_t *)&epacp->groups[i].uuid);
	    newcred->cr_groups[j++] = gid;
	}
	osi_SetNGroups(newcred, j);
	icl_Trace4(fshs_iclSetp, FSHS_GETCREDX,
		   ICL_TYPE_POINTER, princp,
		   ICL_TYPE_LONG, uid,
		   ICL_TYPE_LONG, *((gid_t *)&epacp->group.uuid),
		   ICL_TYPE_LONG, j);
    } else { /* unauthticated */
	osi_SetUID(newcred, unauthID);	/* Make it anonymous */
	osi_SetGID(newcred, unauthID);
	osi_SetNGroups(newcred, 0);
	icl_Trace2(fshs_iclSetp, FSHS_GETCRED, ICL_TYPE_POINTER, princp,
		   ICL_TYPE_LONG, unauthID);
    }
    *credpp = newcred;
}


/*
 * Creates a xcred (extra cred) structure, holds it and fills in all cred info
 * dereived from PAC of each RPC packet.
 */
int fshs_GetPAC(cookie, princp)
    struct context *cookie;
    struct fshs_principal *princp;
{

    long code, groupsSize;
    osi_cred_t *credp;
    struct xcred *xcredp;
    sec_id_t *groups1;
    sec_id_foreign_t *groups2;

    if (code = xcred_Create(&xcredp)) {
	printf("fshs: xcred_Create failed!\n");
	return code;
    }

    if (code = xcred_PutProp (xcredp, "DCE_EPAC", 8,
                             (char *)&princp->delegationChain,
                             sizeof(pac_list_t *), 0, 0)) {
        return code;
    }

    fshs_getcred(princp, &credp);
    if (code = xcred_AssociateCreds(xcredp, credp, 0))
	return code;

    princp->xcredp = xcredp;

    return 0;

}

/*
 * Derive some authn/authz information from rpc binding. PX will use this
 * to locate the appropriate context for the remote caller.
 */
int fshs_InqContext(h, cookiePtr)
     handle_t h;
     struct context **cookiePtr;
{
    struct context *cookie;
    unsigned32 authnLevel, authnSvc, authzSvc;
    unsigned32 st;
    error_status_t tmp_st;

    *cookiePtr = NULL;
    cookie = (struct context *) osi_Alloc(sizeof (struct context));
    bzero((char *)cookie, sizeof (struct context));
    /*
     * Get caller's host objectID and host network address.
     * Note, Must extract the network address directly from the rpc binding !
     */
    rpc_binding_inq_object(h, &cookie->clientID, &st);
    if (st != rpc_s_ok) {
	goto bad;
    }
    /*
     * reject the call if the binding contains a nil uuid
     */
    if (uuid_is_nil(&cookie->clientID, &st)) {
	st =  FSHS_ERR_NULLUUID;
	goto bad;
    }
    cookie->uuidIndex = uuid_hash(&cookie->clientID, &st) % FSHS_NHOSTS;

    rpc_binding_inq_auth_caller(h, &cookie->cred_ptr, NULL,
		&authnLevel, &authnSvc, &authzSvc, &tmp_st);
    cookie->isAuthn = 0;
    cookie->authnLevel = rpc_c_authn_level_none;
    icl_Trace4(fshs_iclSetp, FSHS_INQCTX,
	       ICL_TYPE_LONG, tmp_st,
	       ICL_TYPE_LONG, authnLevel,
	       ICL_TYPE_LONG, authnSvc,
	       ICL_TYPE_LONG, authzSvc);
    if (tmp_st == error_status_ok) {
	if (authnSvc != rpc_c_authn_none
	    && authnLevel != rpc_c_authn_level_none
	    && (authzSvc == rpc_c_authz_dce || authzSvc == rpc_c_authz_name)) {
	    /*
	     * We have some kind of authentication.  Screen authz_dce with
	     * sec_cred_is_authenticated, since that's the only authz that DFS
	     * will treat as reliable.  Don't bother screening authz_name.
	     */
	    if (authzSvc == rpc_c_authz_dce
		&& sec_cred_is_authenticated(cookie->cred_ptr, &tmp_st))
		cookie->isAuthn = 1;
	    /* either authz_dce or authz_name is OK for an authnLevel, tho. */
	    cookie->authnLevel = authnLevel;
	}
    } else if (tmp_st != rpc_s_binding_has_no_auth) {
	st = tmp_st;
	goto bad;
    }
    *cookiePtr = cookie;
    return error_status_ok;

bad:
    osi_Free((char *)cookie, sizeof (struct context));
    return st;
}

int
fshs_InitUnAuthPA()
{
    if (initPA == 0) {
	sec_id_pa_t *pa;

	pa = &unauthPA;
	bzero((char *)pa, sizeof(sec_id_pa_t));
	*(int32 *)&pa->principal.uuid = unauthID;
	*(int32 *)&pa->group.uuid = unauthID;
	*(int32 *)&pa->realm.uuid = unauthRealmID;
	initPA = 1;
    }
}
