/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dacl_xcred.c,v $
 * Revision 1.1.59.1  1996/10/02  17:24:06  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:31:50  damon]
 *
 * $EndLog$
 */
/*
*/
/*
 * Copyright (C) 1995, 1991 Transarc Corporation
 * All rights reserved.
 */

/*
 *			Episode File System
 *		Interface for Extended Credentials and PACs
 */

#include <dcedfs/episode/dacl_anode.h>
#include <dcedfs/xcred.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/episode/eacl/dacl_xcred.c,v 1.1.59.1 1996/10/02 17:24:06 damon Exp $")
/* OLD CODE */
/*
 * eacl_FindPac -- find or make a PAC appropriate for the caller
 *
 * If the process is in a PAG, we look up the PAG in the xcred database
 * to find a PAC.  Having found one, we return it.
 *
 * If the process is not in a PAG, or if there is no PAC for it in the
 * xcred database, we create a PAC using information from the cred structure.
 *
 * If the xcred package returns some error we don't know how to handle,
 * we panic.
 *
 * PARAMETERS:
 *   If we find a PAC, a pointer to it is copied into pacPP.  If we don't
 *   find one, pacPP is expected to contain a pointer to an uninitialized
 *   PAC structure that we can fill with data from the cred.
 */

void eacl_FindPac(
    IN osi_cred_t *credP,		/* credential structure */
    INOUT sec_id_pac_t **pacPP)		/* pointer to pointer to PAC struct */
{
    xcred_t *xcredP;			/* extended credential structure */
    int code;				/* error return code */

    osi_UnmakePreemptionRight();

    code = xcred_UCredToXCred (credP, &xcredP);
    if (code == 0) {
	long pacBytes;			/* dummy arg. for xcred_GetProp */
	code = xcred_GetProp (xcredP, "DCE_PAC", 7, (char **)pacPP, &pacBytes);
	xcred_Release (xcredP);
    }
    if (code) {
	afsl_Assert (code == XCRED_EC_NO_ENTRY);
	dacl_PacFromUcred (*pacPP, credP);
    }

    osi_MakePreemptionRight();
}

#if (defined(AFS_SUNOS5_ENV) && !defined(AFS_SUNOS55_ENV))
sec_id_pa_t	efsUnauthPA;		/* Episode un-authenticated PA */
#endif /* (defined(AFS_SUNOS5_ENV) && !defined(AFS_SUNOS55_ENV)) */

/*
 * eacl_FindPacList -- find or make a PAC list appropriate for the caller
 *
 * If the process is in a PAG, we look up the PAG in the xcred database
 * to find a PAC.  Having found one, we return it.
 *
 * If the process is not in a PAG, or if there is no EPAC for it in the
 * xcred database, we create an EPAC using information from the cred structure.
 * The needFree flag is set to inform the caller to free the EPAC when
 * done.
 *
 * If the xcred package returns some error we don't know how to handle,
 * we panic.
 *
 * PARAMETERS:
 *   If we find an EPAC, a pointer to it is copied into pacPP.  Otherwise,
 *   an EPAC is allocated and the needsFree flag is set to inform the
 *   caller to free it when done.
 */

void eacl_FindPacList(
    IN osi_cred_t *credP,		/* credential structure */
    INOUT pac_list_t ***pacListPPP,	/* pointer to PAC struct */
    OUT u_int32 *needsFree)		/* tells caller to free pac_list_t */
{
    xcred_t *xcredP;			/* extended credential structure */
    int code;				/* error return code */

    osi_UnmakePreemptionRight();

    code = xcred_UCredToXCred (credP, &xcredP);
    if (code == 0) {
	long pacBytes;			/* dummy arg. for xcred_GetProp */

	code = xcred_GetProp (xcredP, "DCE_EPAC", 8, (char **)pacListPPP,
			      &pacBytes);
	xcred_Release (xcredP);
#if (defined(AFS_SUNOS5_ENV) && !defined(AFS_SUNOS55_ENV))
	/*
	 * If this is the fshost unauth PA, use the Episode flavor.  This will
	 * get us a uid,gid of -2,-1 rather than 65534,65534 as UFS needs,
	 * and as fshs_prutils.c puts there.
	 */
	if (code == 0
	    && (*((int32*)&(**pacListPPP)->pacp->realm.uuid) == -2)
	    && (*((int32*)&(**pacListPPP)->pacp->principal.uuid) == 65534)
	    && (*((int32*)&(**pacListPPP)->pacp->group.uuid) == 65534)) {
	    (**pacListPPP)->pacp = &efsUnauthPA;
	    (**pacListPPP)->lastDelegate = **pacListPPP;
	}
#endif /* (defined(AFS_SUNOS5_ENV) && !defined(AFS_SUNOS55_ENV)) */
    }
    if (code) {
	afsl_Assert (code == XCRED_EC_NO_ENTRY);
	(*needsFree)++;			/* this allocates a paclist */
	dacl_PacListFromUcred (*pacListPPP, credP);
    }

    osi_MakePreemptionRight();
}

void eacl_Init(void) {
#if (defined(AFS_SUNOS5_ENV) && !defined(AFS_SUNOS55_ENV))
    /* Mimic an unauthenticated caller for Episode as vnax_GetOwner says */
    sec_id_pa_t *pa;
    static int32 epiUnauthUid = -2;	/* from efs_access.c */
    static int32 epiUnauthGid = -1;	/* from efs_access.c */
    static int32 unauthRealmID = -2;	/* from fshs_prutils.c */

    pa = &efsUnauthPA;
    bzero((char *)pa, sizeof(sec_id_pa_t));
    *(int32 *)&pa->principal.uuid = epiUnauthUid;
    *(int32 *)&pa->group.uuid = epiUnauthGid;
    *(int32 *)&pa->realm.uuid = unauthRealmID;
#endif /* (defined(AFS_SUNOS5_ENV) && !defined(AFS_SUNOS55_ENV)) */
}
