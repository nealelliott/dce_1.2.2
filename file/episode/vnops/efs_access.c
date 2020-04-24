/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_access.c,v $
 * Revision 1.1.25.1  1996/10/02  17:43:33  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:36  damon]
 *
 * $EndLog$
*/
/*
 * Copyright (C) 1996, 1990 Transarc Corporation
 * All rights reserved.
 */

/*			   Episode File System
		   Access support for Vnode operations
 */
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/lock.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_kvnode.h>
#include <dcedfs/dir_buffer.h>		/* struct dh_ops */
#include <dcedfs/xcred.h>		/* Extended credential defs & ops*/

#include <dcedfs/episode/anode.h>
#include <dcedfs/episode/dacl_anode.h>

#include <efs_evnode.h>
#ifdef AFS_SUNOS54_ENV
#undef agroup		/* XXX FIX ME XXX */
#endif
#include <efs_volvnops.h>
#include <efs_misc.h>
#include <efs_access.h>

#if !defined(KERNEL)
#include "stubs.h"
#endif /* !KERNEL */

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/efs_access.c,v 1.1.25.1 1996/10/02 17:43:33 damon Exp $")

#define DEBUG_THIS_FILE EFS_DEBUG_ACCESS

/* Provide means of turning off superuser privileges for network root
 * under the control of a kernel variable efs_networkRootSuperUser. Turn
 * this feature on by default.
 */
#ifndef NETWORK_ROOT_NOT_SUPERUSER
int efs_networkRootSuperUser = 1;
#endif

/*
 * Access caching control and statistics (for vnax_CanAccess)
 */
int efs_accessCachingEnabled = 1;

static struct accessCacheStats {
    u_int hits;
    u_int total;
    u_int denials;
} efs_accessCacheStats;


/* Even on R/O file systems writes are allowed to special devices. */

#define RO_RESTRICTED_BITS(evp) \
    (!EV_ISREADONLY(evp) ? 0 \
     : ((EV_ISVDEV(evp) ? 0 : dacl_perm_write) | \
	dacl_perm_control | dacl_perm_insert | dacl_perm_delete))

static int vnax_GroupInPAC(epi_principal_id_t *agroup, sec_id_pa_t *apacP);
static int vnax_GidInPAC(u_long *agidP, sec_id_pa_t *apacP);


/*------------------------------------------------------------------------
 * EXPORT vnax_GetOwner()
 *
 * Summary:
 *	Determine the owner and group ID's for a file to be created.
 *
 * Args:
 *	IN credp: pointer to the credential structure from which to derive
 *		  the information we seek
 *	IN pvp: pointer to the vnode representing the parent directory
 *	OUT vap: pointer to a vnode attribute structure in which to store
 *		  the information
 *	OUT realmp: pointer to a uuid into which the principal's ACL is to
 *		    be copied; may be null
 *	OUT authp: pointer to a flag telling whether the principal is
 *		     authenticated (0) or not (1)
 *------------------------------------------------------------------------*/

/* EXPORT */
void vnax_GetOwner (credp, pvp, vap, realmp, authp)
    osi_cred_t *credp;
    struct evnode *pvp;
    struct vattr *vap;
    epi_uuid_t *realmp;
    int *authp;
{
    epi_anode_t pap = EVTOA (pvp);	/* anode handle for pvp */
    long uid, gid;
    pac_list_t **pacListPP;		/* paclist pointer pointer pointer */
    pac_list_t *pacListP, *lastDlgP;    /* pointer to pac list pointer */
    u_int32 needsFree = 0;		/* tells caller to free pac list */

    pacListPP = &pacListP;
    eacl_FindPacList(credp, &pacListPP, &needsFree);
    pacListP = *pacListPP;

    /* Find the last delegate in the chain */
    lastDlgP = pacListP;
    while(lastDlgP->next)
	lastDlgP = lastDlgP->next;

    if (realmp) {
	*realmp = *((epi_uuid_t *)&lastDlgP->pacp->realm.uuid);
    }

    if (authp)
	*authp = lastDlgP->authenticated;

    if (!(lastDlgP->authenticated)) {
	uid = -2;		/* XXX symbolic names?! */
	gid = -1;
    } else {
	uid = *((long *)(&lastDlgP->pacp->principal.uuid));
	/*
	 * Set the gid.  This may be taken from the parent directory, or
	 * from the process (PAC).  Different kernels have different rules
	 * for where to get it, hence the ugly conditional compilation.
	 * XXX Use OSI layer or OS-specific subdirectories for this.
	 */
#ifdef	AFS_SUNOS5_ENV
	/* Solaris NFS clients may set AT_GID */
	if ((vap->va_mask & AT_GID) &&
	    (vap->va_gid == epif_GetGid (pap) ||
	     vnax_GidInPAC((u_long*)&vap->va_gid, pacListP->pacp))) {
	    gid = vap->va_gid;
	} else if (epif_GetMode (pap) & S_ISGID) {
	    gid = epif_GetGid (pap);
	} else
	    gid = *((long *)(&lastDlgP->pacp->group.uuid));
#elif	defined(AFS_AIX31_ENV)
	if (epif_GetMode (pap) & ISGID) {
	    gid = epif_GetGid (pap);
	} else
	    gid = *((long *)(&lastDlgP->pacp->group.uuid));
#elif	defined(AFS_HPUX_ENV)
	if (epif_GetMode (pap) & VSGID) {
	    gid = epif_GetGid (pap);
	} else
	    gid = *((long *)(&lastDlgP->pacp->group.uuid));
#else
	/* Non-SunOS 5, Non-AIX 3 */
	gid = epif_GetGid (pap);
#endif
    }

    if (needsFree) {
	if (pacListP->pacp->groups)
	    osi_Free_r(pacListP->pacp->groups,
		       pacListP->pacp->num_groups * sizeof (sec_id_t));
	osi_Free_r(pacListP->pacp, sizeof (sec_id_pa_t));
	osi_Free_r(pacListP, sizeof (pac_list_t));
    }

    vap->va_uid = (uid_t) uid;
    vap->va_gid = (gid_t) gid;
}

/*
 * vnax_InheritAclMode -- intersect mode bits with corresponding bits from
 *			  inherited ACL
 *
 * Called when creating a file or directory.
 */

/* EXPORT */
int vnax_InheritAclMode (evp, transId)
    struct evnode *evp;			/* Episode vnode */
    buffer_tranRec_t transId;		/* transaction ID */
{
    epi_anode_t ap;			/* anode handle for evp */
    struct epif_status fileStat;	/* status info for evp */
    dacl_t *daclP;			/* parsed ACL */
    u_int32 modeBits;			/* mode bits from file */
    u_int32 permBits;			/* permission bits from ACL */
    int code;				/* error return code */

    ap = EVTOA (evp);
    epif_GetStatus (ap, &fileStat);

    /* if it doesn't have an ACL, do nothing. */
    if (fileStat.acl == 0)
	return 0;

    /* read and parse the ACL */
    eacl_AllocateAcl (&daclP);
    code = eacl_ReadAclFromAnode (daclP, ap, fileStat.acl, EPIF_AUX_IS_ACL,
				  &episodeAclMgmtUuid);
    if (code)
	goto out;

    /* get the conventional permission bits from the ACL */
    code = dacl_ExtractPermBits_r (daclP, &permBits);
    if (code)
	goto out;

    /* if ACL more permissive than mode bits, no action needed */
    modeBits = fileStat.mode & 0777;
    if ((modeBits & permBits) == modeBits)
	goto out;

    /* intersect and reset mode */
    fileStat.mode &= ~0777;
    fileStat.mode |= (modeBits & permBits);
    code = epif_SetStatus (transId, ap, EPIF_SFLAGS_MODE, &fileStat);
    if (code)
	goto out;

    /* clean up and exit */
out:
    eacl_ReleaseAcl (daclP);
    return (code == 0) ? 0 : EIO;
}

/*
 * vnax_GetAnonAccess -- Report permission bits that anyone would get
 *
 * This gives the intersection of permissions that any user could get,
 * even unauthenticated users.
 */

/* EXPORT */
int vnax_GetAnonAccess (evp, fstatP, permP)
    struct evnode *evp;			/* Episode vnode */
    struct epif_status *fstatP;		/* file status info for evp */
    dacl_permset_t *permP;		/* place to put permission bit mask */
{
    int code = 0;

    lock_ObtainWrite_r(&evp->vd_cache.lock);

    if (fstatP->acl == 0) {		/* File does not have an ACL */
	*permP = 0;
    } else {
	dacl_t *aclP = evp->vd_acl;

	if (aclP == NULL) {
	    eacl_AllocateAcl (&aclP);
	    code = eacl_ReadAclFromAnode (aclP, EVTOA (evp), fstatP->acl,
					    EPIF_AUX_IS_ACL,
					    &episodeAclMgmtUuid);
	    if (code == 0)
		evp->vd_acl = aclP;
	    else
		eacl_ReleaseAcl(aclP);
	}
	if (code == 0) {
	    dacl_ExtractMinimumPermSet_r(aclP, permP);
	    *permP &= ~RO_RESTRICTED_BITS(evp);
	}
    }

    lock_ReleaseWrite_r(&evp->vd_cache.lock);
    return (code == 0) ? 0 : EIO;
}

/*------------------------------------------------------------------------
 * EXPORT vnax_GetAccess()
 *
 * Summary:
 *	Report all permission bits that the caller has to a file or directory.
 *
 * This is different from vnax_CanAccess (see below), which gives a yes-or-no
 * answer.
 *
 *------------------------------------------------------------------------*/

/* EXPORT */
int vnax_GetAccess (evp, fstatP, permP, credP)
    struct evnode *evp;			/* Episode vnode */
    struct epif_status *fstatP;		/* file status info for evp */
    dacl_permset_t *permP;		/* place to put permission bit mask */
    osi_cred_t *credP;			/* credential structure */
{
    long code = 0;			/* dummy return code */
    dacl_permset_t dummyperm;		/* dummy argument to Check...Pac */
    u_int32 grantedAccessBits;		/* permissions bits granted */
    u_int32 alignmode;			/* mode bits on long boundary */
    epi_principal_id_t userObj;		/* USER_OBJ principal */
    epi_principal_id_t groupObj;	/* GROUP_OBJ principal */
    dacl_t * aclP;			/* ACL structure */
    epi_uuid_t sysAdminGroupID;		/* privileged ID */
    epi_uuid_t networkRootID;		/* privileged ID */
    epi_uuid_t localCellID;
    dacl_permset_t roBits;		/* clear these bits before return */
    pac_list_t **pacListPP;
    pac_list_t *pacListP, *tmpPacListP, *lastDlgP;
    u_int32 needsFree = 0;

    dacl_GetLocalCellID_r((afsUUID*)&localCellID);
    roBits = RO_RESTRICTED_BITS(evp);

    pacListPP = &pacListP;
    eacl_FindPacList(credP, &pacListPP, &needsFree);
    pacListP = *pacListPP;

    /*
     * If called on behalf of local user (indicated by needsFree being set),
     * check for local superuser.
     *
     * Currently, vnax_GetAccess is not called on behalf of local users.
     * This code is being added for uniformity with vnax_CanAccess, the
     * aim being to combine the 2 functions someday soon.
     */
    if (needsFree && (osi_GetUID(credP) == 0)) {
	/* local access by local superuser */
	*permP = (dacl_perm_control | dacl_perm_read |
		  dacl_perm_execute | dacl_perm_write |
		  dacl_perm_insert | dacl_perm_delete);
	goto cleanup;
    }

    lock_ObtainWrite_r(&evp->vd_cache.lock);

    /*
     * If there is an ACL, the DACL package will check for system
     * administrator.  If there is no ACL, this function will check.
     */

    if (fstatP->acl > 0) {		/* File has an ACL */
	int releaseAcl = 0;

	/*
	 * Prepare to find out the caller's access.  Set up the
	 * USER_OBJ and GROUP_OBJ identities (simply the owner
	 * and group fields from the stat block) beforehand.
	 */
	bzero ((caddr_t)&userObj, sizeof (epi_principal_id_t));
	bzero ((caddr_t)&groupObj, sizeof (epi_principal_id_t));
	bcopy ((caddr_t)&fstatP->oid, (caddr_t)&userObj, sizeof (fstatP->oid));
	bcopy ((caddr_t)&fstatP->gid, (caddr_t)&groupObj, sizeof (fstatP->gid));
	aclP = evp->vd_acl;
	if (!aclP) {
	    eacl_AllocateAcl (&aclP);
	    code = eacl_ReadAclFromAnode (aclP, EVTOA (evp), fstatP->acl,
					    EPIF_AUX_IS_ACL,
					    &episodeAclMgmtUuid);
	    if (!code)
		evp->vd_acl = aclP;
	    else
		eacl_ReleaseAcl (aclP);
	}
	if (!code) {
	    int networkRootCheck;

#ifndef NETWORK_ROOT_NOT_SUPERUSER
	    networkRootCheck = (needsFree ? 0 :
				efs_networkRootSuperUser);
#else
	    networkRootCheck = 0;

#endif
	    dummyperm = 0;
	    alignmode = fstatP->mode;
	    code = dacl_epi_CheckAccessAllowedPac_r(aclP, &alignmode,
						    &dummyperm, pacListP,
						    &userObj, &groupObj,
						    networkRootCheck,
						    permP);
	}
	if (code)
	    switch (code) {
	      case DACL_ERROR_ACCESS_DENIED:
	      case DACL_ERROR_ACCESS_EXPLICITLY_DENIED:
		code = 0; break;
	      default:
		code = EIO;
	    }
	*permP &= ~roBits;
    } else {
	/* File does not have an ACL */

	u_int32 bits;
	int not_owner = 0;
	int not_group = 0;
	int systemAdminGroupMember = 1;
	int networkRoot = 1;

	*permP = 0;

	/* No bits set for unauthenticated principal */
	if (!(pacListP->authenticated)) {
	    lock_ReleaseWrite_r(&evp->vd_cache.lock);
	    icl_Trace2(efs_iclSet, EFS_TRACE_GETACCESS_UNAUTH,
		       ICL_TYPE_POINTER, evp,
		       ICL_TYPE_LONG, *((uid_t *)
					(&(pacListP->pacp->principal.uuid))));
	    goto cleanup;
	}

	/* First set Unix standard access bits, plus C if we are owner */

	/*
	 * Requests from a different cell receive no rights in the null ACL
	 * case since by default there are not any_other rights.
	 * This includes authenticated requests from a foreign
	 * cell and anomymous requests.
	 */
	if (bcmp((char *) &pacListP->pacp->realm.uuid,
		(char *) &localCellID, sizeof (epi_uuid_t)) != 0) {
	    lock_ReleaseWrite_r(&evp->vd_cache.lock);
	    icl_Trace2(efs_iclSet, EFS_TRACE_GETACCESS_NOACL_FOREIGNACCESS,
		       ICL_TYPE_POINTER, evp,
		       ICL_TYPE_POINTER, pacListP);
	    goto cleanup;
	}

	tmpPacListP = pacListP;
	dacl_GetNetworkRootID_r((afsUUID *)&networkRootID);
	while (tmpPacListP) {
	    lastDlgP = tmpPacListP;
	    if (networkRoot &&
		(Epi_PrinId_Cmp(&tmpPacListP->pacp->principal.uuid,
				&networkRootID) != 0)) {
		networkRoot = 0;
	    }
	    if (Epi_PrinId_Cmp (&tmpPacListP->pacp->principal.uuid,
				&fstatP->oid) != 0) {
		not_owner = 1;
		if (!vnax_GidInPAC(&fstatP->gid, tmpPacListP->pacp))
		    not_group = 1;
	    }
	    tmpPacListP = tmpPacListP->next;
	}

	if (networkRoot) {
	    *permP = (dacl_perm_control | dacl_perm_read |
		      dacl_perm_execute | dacl_perm_write |
		      dacl_perm_insert | dacl_perm_delete);
	} else {
	    bits = fstatP->mode;
	    if (not_owner) {
		bits <<= 3;
		if (not_group)
		    bits <<= 3;
	    }

	    bits &= S_IRWXU;
	    dacl_ChmodOnePermset_r(bits, permP, EV_ISDIR(evp));
	    if (Epi_PrinId_Cmp (&lastDlgP->pacp->principal.uuid,
				&fstatP->oid) == 0)
		*permP |= dacl_perm_control;

	    /* Add C access if we are system administrator */
	    if (!(*permP & dacl_perm_control)) {
		dacl_GetSysAdminGroupID_r((afsUUID*)&sysAdminGroupID);
		tmpPacListP = pacListP;
		while (tmpPacListP) {
		    if (!vnax_GroupInPAC((epi_principal_id_t *)
					 &sysAdminGroupID,
					 tmpPacListP->pacp)) {
			systemAdminGroupMember = 0;
			break;
		    }
		    tmpPacListP = tmpPacListP->next;
		}
		if (systemAdminGroupMember)
		    *permP |= dacl_perm_control;
	    }
	}

	*permP &= ~roBits;
	code = 0;
    }

    lock_ReleaseWrite_r(&evp->vd_cache.lock);

cleanup:

    /*
     * Only need to free in the stand-alone case, so there's
     * only one pac_list_t to worry about.
     */
    if (needsFree) {
	if (pacListP->pacp->groups)
	    osi_Free_r(pacListP->pacp->groups,
		       pacListP->pacp->num_groups * sizeof (sec_id_t));
	osi_Free_r(pacListP->pacp, sizeof (sec_id_pa_t));
	osi_Free_r(pacListP, sizeof (pac_list_t));
    }
    return code;
}

/*------------------------------------------------------------------------
 * EXPORT vnax_CanAccess()
 *
 * Summary:
 *	Check the caller's access rights to a file or directory.
 *	Should not be used to check for ``c'' access (see vnax_CanAdminister).
 *
 * Args:
 *	IN aevP    : Vnode pointer for file/directory being accessed.
 *	IN afstatP : Vnode's associated status.
 *	IN aperm   : Desired access permission(s).
 *	IN aucredP : Pointer to the caller's Unix cred.
 *
 * Returns:
 *	0	  if the caller has the desired access.
 *	EACCES	  if the caller's access has been denied.
 *      EROFS	  if file system cannot be modified
 *	Otherwise the ACL error code mapped to ``nearest'' Unix
 *		  equivalent.
 *
 * Locks:
 *	Caller has read-locked the given vnode.  We don't set or
 *	release locks ourselves in this routine.
 *
 * Note:
 *      The access rights for the last user of a file are now cached
 *	in the vnode along with the user's cred. The cred acts as the
 *	comparision tag. The cache is invalidated at events involving
 *	change in the permissions/acl of the file.
 *
 *	Previously, we took a few explicit steps to avoid disturbing the
 *	cache for access by local and remote root users. But with
 *	support for delegation, some changes need to be made to
 *	dacl routines and interfaces which do seem really warranted. Combined
 *	with the observation that simultaneous access of a file by regular
 *	users and root should be rare, we no longer take any steps to prevent
 *	access caching for superusers.
 *
 *------------------------------------------------------------------------*/

/* EXPORT */
int vnax_CanAccess(aevP, afstatP, aperm, aucredP)
    struct evnode *aevP;		/* Vnode to check */
    struct epif_status *afstatP;	/* File status info */
    dacl_permset_t aperm;		/* Desired permissions */
    osi_cred_t *aucredP;		/* Unix credential structure */
{
    long code = 0;			/* Return code */
    struct epif_status fstat;		/* local copy */
    u_int32 grantedAccessBits;		/* permissions bits granted */
    u_int32 alignmode;			/* mode bits on long boundary */
    epi_principal_id_t userObj;		/* USER_OBJ principal */
    epi_principal_id_t groupObj;	/* GROUP_OBJ principal */
    dacl_t *aclP;			/* ACL structure */
    epi_uuid_t localCellID;
    pac_list_t **pacListPP;
    pac_list_t *pacListP, *tmpPacListP, *lastDlgP;
    u_int32 needsFree = 0;
    dacl_permset_t allowedAccess = 0;
    epi_uuid_t networkRootID;		/* privileged ID */

    if (aperm & RO_RESTRICTED_BITS(aevP)) {
	/* Check for read-only file system if an update is contemplated. */
	return (EROFS);
    }

    lock_ObtainWrite_r(&aevP->vd_cache.lock);
    efs_accessCacheStats.total++;

#ifdef LFS_USE_LOCAL_ACCESS_CACHE
    /* First check access cache */
    if (efs_accessCachingEnabled && aevP->vd_cred &&
	osi_crequal(aucredP, aevP->vd_cred)) {
	efs_accessCacheStats.hits++;
	icl_Trace4(efs_iclSet2, EFS_TRACE_ACCESS_CACHE_HIT,
		   ICL_TYPE_POINTER, aevP,
		   ICL_TYPE_LONG, osi_GetUID(aucredP),
		   ICL_TYPE_LONG, aevP->vd_access,
		   ICL_TYPE_LONG, aperm);
	if ((aperm & aevP->vd_access) == aperm) {
	    code = 0;
	} else {
	    code = EACCES;
	}
	lock_ReleaseWrite_r(&aevP->vd_cache.lock);
	return code;
    }
    icl_Trace3(efs_iclSet2, EFS_TRACE_ACCESS_CACHE_MISS,
	       ICL_TYPE_POINTER, aevP,
	       ICL_TYPE_LONG, osi_GetUID(aucredP),
	       ICL_TYPE_LONG, aperm);
#endif /* LFS_USE_LOCAL_ACCESS_CACHE */


#if !defined(AFS_SUNOS5_ENV) && !defined(AFS_AIX31_ENV)
    if (aperm & dacl_perm_write) {

	/*
	 * Ancient code to handle shared text.  Perhaps obsolete?
	 */
	if ((EVTOV(aevP))->v_flag & VTEXT)
	    xrele(aevP);
	if ((EVTOV(aevP))->v_flag & VTEXT) {
	    lock_ReleaseWrite_r(&evp->vd_cache.lock);
	    return (ETXTBSY);
	}
    }
#endif /* AFS_SUNOS5_ENV, AFS_AIX31_ENV */

    /* If the caller didn't pass us status info, get it ourselves */
    if (!afstatP) {
	afstatP = &fstat;
	code = epif_GetStatus (EVTOA(aevP), &fstat);
	if (code) {
	    icl_Trace3(efs_iclSet, EFS_TRACE_CANACCESS_GETSTATUS,
		       ICL_TYPE_POINTER, aevP,
		       ICL_TYPE_LONG, aevP->vd_fid.index,
		       ICL_TYPE_LONG, code);
	    lock_ReleaseWrite_r(&aevP->vd_cache.lock);
	    return code;
	}
    }

    dacl_GetLocalCellID_r((afsUUID*)&localCellID);

    /*
     * Pull out the caller's PAC information before doing the
     * true access check.
     */
    pacListPP = &pacListP;
    eacl_FindPacList(aucredP, &pacListPP, &needsFree);
    pacListP = *pacListPP;

    /*
     * Check for superuser
     *   local always
     *   network root if NETWORK_ROOT_NOT_SUPERUSER macro not defined
     */
    if (needsFree) {			/* If called on behalf of local user */
	/*
	 * Check for local superuser
	 */
	if (osi_GetUID(aucredP) == 0) {
	    allowedAccess = (dacl_perm_control | dacl_perm_read |
			     dacl_perm_execute | dacl_perm_write |
			     dacl_perm_insert | dacl_perm_delete);
	    goto cleanup;
	}
    }

    /*
     * We don't check for system administrator, since aperm can be
     * guaranteed not to request C access.  (Callers that need C access
     * should use vnax_CanAdminister.)
     */

    /*
     * We do different things, depending on whether there's an
     * ACL attached to the file in question.  We determine this
     * by the ACL's length, as recorded in the stat information.
     */
    if (afstatP->acl > 0) {		/* File has an ACL */
	int releaseAcl = 0;
	/*
	 * Prepare to find out the caller's access.  Set up the
	 * USER_OBJ and GROUP_OBJ identities (simply the owner
	 * and group fields from the stat block) beforehand.
	 */
	bzero((caddr_t)&userObj, sizeof (epi_principal_id_t));
	bzero((caddr_t)&groupObj, sizeof (epi_principal_id_t));
	bcopy((caddr_t)&afstatP->oid, (caddr_t)&userObj, sizeof (afstatP->oid));
	bcopy((caddr_t)&afstatP->gid, (caddr_t)&groupObj, sizeof (afstatP->gid));

	code = 0;
	aclP = aevP->vd_acl;
	if (!aclP) {
	    eacl_AllocateAcl (&aclP);

	    code = eacl_ReadAclFromAnode (aclP, EVTOA (aevP), afstatP->acl,
					    EPIF_AUX_IS_ACL,
					    &episodeAclMgmtUuid);
	    if (!code)
		aevP->vd_acl = aclP;
	    else
		eacl_ReleaseAcl(aclP);
	}
	if (!code) {
	    int networkRootCheck;

#ifndef NETWORK_ROOT_NOT_SUPERUSER
	    networkRootCheck = (needsFree ? 0 :
				efs_networkRootSuperUser);
#else
	    networkRootCheck = 0;
#endif

	    alignmode = afstatP->mode;
	    code =
		dacl_epi_CheckAccessAllowedPac_r(aclP, &alignmode, &aperm,
						 pacListP,
						 (epi_principal_id_t *)&userObj,
						 (epi_principal_id_t *)&groupObj,
						 networkRootCheck,
						 &allowedAccess);
	    switch (code) {
	      case 0:
		break;
	      case DACL_ERROR_ACCESS_DENIED:
	      case DACL_ERROR_ACCESS_EXPLICITLY_DENIED:
		code = EACCES;
		efs_accessCacheStats.denials++;
		break;
	      default:
		code = EIO;
	    }
	}
    } else {				/* File does not have an ACL */
	int notowner = 0;
	int notgroup = 0;
	int networkRoot = 1;

	if (!(pacListP->authenticated)) {
	    code = EACCES;
	    efs_accessCacheStats.denials++;
	    goto cleanup;
	}

	/*
	 * Requests from a different cell are denied access in the null ACL
	 * case since by default there are not any_other rights.
	 * This includes authenticated requests from a foreign
	 * cell and anonymous requests.
	 */
	if (bcmp((char *) &pacListP->pacp->realm.uuid, (char *) &localCellID,
	    sizeof (epi_uuid_t)) != 0) {
	    code = EACCES;
	    efs_accessCacheStats.denials++;
	    goto cleanup;
	}

	tmpPacListP = pacListP;
	dacl_GetNetworkRootID_r((afsUUID *)&networkRootID);
	while (tmpPacListP) {
	    lastDlgP = tmpPacListP;
	    if (networkRoot &&
		(Epi_PrinId_Cmp(&tmpPacListP->pacp->principal.uuid,
				&networkRootID) != 0)) {
		networkRoot = 0;
	    }
	    if (Epi_PrinId_Cmp (&tmpPacListP->pacp->principal.uuid,
				&afstatP->oid) != 0) {
		notowner = 1;
		if (!vnax_GidInPAC(&afstatP->gid, tmpPacListP->pacp))
			notgroup = 1;
	    }
	    tmpPacListP = tmpPacListP->next;
	}

	if (networkRoot) {
	    allowedAccess = (dacl_perm_read | dacl_perm_write |
			     dacl_perm_execute | dacl_perm_control |
			     dacl_perm_insert | dacl_perm_delete);
	    code = 0;
	} else {
	    grantedAccessBits = afstatP->mode;
	    if (notowner) {
		grantedAccessBits <<= 3;
		if (notgroup)
		    grantedAccessBits <<= 3;
	    }
	    grantedAccessBits &= S_IRWXU;
	    dacl_ChmodOnePermset_r(grantedAccessBits, &allowedAccess,
				   EV_ISDIR(aevP));

	    if (Epi_PrinId_Cmp (&lastDlgP->pacp->principal.uuid,
				&afstatP->oid) == 0)
		allowedAccess |= dacl_perm_control;

	    /* Add C access if we are system administrator */
	    if (!(allowedAccess & dacl_perm_control)) {
		epi_uuid_t sysAdminGroupID;		/* privileged ID */
		int systemAdminGroupMember = 1;

		dacl_GetSysAdminGroupID_r((afsUUID*)&sysAdminGroupID);
		tmpPacListP = pacListP;
		while (tmpPacListP) {
		    if (!vnax_GroupInPAC((epi_principal_id_t *)
					 &sysAdminGroupID,
					 tmpPacListP->pacp)) {
			systemAdminGroupMember = 0;
			break;
		    }
		    tmpPacListP = tmpPacListP->next;
		}
		if (systemAdminGroupMember)
		    allowedAccess |= dacl_perm_control;
	    }
	    if ((aperm & allowedAccess) == aperm) {
		code = 0;
	    } else {
		code = EACCES;
		efs_accessCacheStats.denials++;
	    }
	}
    } /*Vnode doesn't have an ACL*/

cleanup:

#ifdef LFS_USE_LOCAL_ACCESS_CACHE
    if (code == 0 && efs_accessCachingEnabled) {
	if (aevP->vd_cred)
	    crfree(aevP->vd_cred);
	aevP->vd_cred = crdup(aucredP);
	aevP->vd_access = allowedAccess;
	icl_Trace4(efs_iclSet2, EFS_TRACE_ACCESS_CACHE_UPDATE,
		  ICL_TYPE_POINTER, aevP,
		  ICL_TYPE_LONG, osi_GetUID(aucredP),
		  ICL_TYPE_LONG, allowedAccess,
		  ICL_TYPE_LONG, aperm);
    }
#endif /* LFS_USE_LOCAL_ACCESS_CACHE */

    lock_ReleaseWrite_r(&aevP->vd_cache.lock);

    /*
     * Only need to free in the stand-alone case, so there's
     * only one pac_list_t to worry about.
     */
    if (needsFree) {
	if (pacListP->pacp->groups)
	    osi_Free_r(pacListP->pacp->groups,
		       pacListP->pacp->num_groups * sizeof (sec_id_t));
	osi_Free_r(pacListP->pacp, sizeof (sec_id_pa_t));
	osi_Free_r(pacListP, sizeof (pac_list_t));
    }

    return (code);
} /*vnax_CanAccess*/

/*------------------------------------------------------------------------
 * PRIVATE vnax_GroupInPAC()
 *
 * Summary:
 *	See if the given group is listed in the PAC.
 *
 * Args:
 *	IN agroup : Group we're looking for.
 *	IN apacP : Ptr to PAC to search.
 *
 * Returns:
 *	1 if the given group appears in the PAC and PAC is in local cell,
 *	0 otherwise.
 *
 * Note:
 *	Perhaps modify this to take explicit realm?
 *------------------------------------------------------------------------*/

static int vnax_GroupInPAC(
    epi_principal_id_t *agroup,	/*Group we're looking for*/
    sec_id_pa_t *apacP)		/*Ptr to PAC to search*/
{ /*vnax_GroupInPAC*/
    int i;				/*Group index in PAC*/
    epi_uuid_t localCellID;

    dacl_GetLocalCellID_r((afsUUID*)&localCellID);
    if (bcmp ((char *) &localCellID,
	      (char *) &apacP->realm.uuid,
	      sizeof (epi_uuid_t)) != 0) {
	return (0);
    }

    if (Epi_PrinId_Cmp (&apacP->group.uuid, agroup) == 0)
	return (1);

    for (i = 0; i < (int) apacP->num_groups; i++) {
	if (Epi_PrinId_Cmp (&apacP->groups[i].uuid, agroup) == 0)
	    return  (1);
    }

    return (0);

} /*vnax_GroupInPAC*/

/*
 * vnax_GidInPAC -- Convert a group ID to a UUID, then call vnax_GroupInPAC
 *
 * Perhaps modify this to take explicit realm?
 */

static int
vnax_GidInPAC(u_long *agidP, sec_id_pa_t *apacP)
{
    epi_principal_id_t group;

    bzero ((caddr_t)&group, sizeof (group));
    bcopy ((caddr_t)agidP, (caddr_t)&group, sizeof (*agidP));
    return vnax_GroupInPAC (&group, apacP);
}

/*
 * vnax_CanAdminister -- Check rights to administer a file;
 *			 also optionally check access to a group
 *
 * Return 0 if can administer the file (i.e. if we are its owner or
 * we are superuser), otherwise return error code (such as EPERM).  If
 * we return 0, we set an out-parameter to indicate whether the given file
 * primary group is in the process' group list, and another out-parameter to
 * indicate if the process has super-user rights.
 */

/* EXPORT */
int vnax_CanAdminister (evP, fstatP, gidP, cred, issuperP, ingroupsP)
    struct evnode *evP;			/* Episode vnode */
    struct epif_status *fstatP;		/* File status info */
    u_long *gidP;			/* group id */
    osi_cred_t *cred;			/* credential structure */
    int *issuperP;			/* Is process super-user? */
    int *ingroupsP;			/* Is file's group one of user's? */
{
    u_int32 modebits;			/* mode bits on long boundary */
    epi_principal_id_t userObj;		/* USER_OBJ principal */
    epi_principal_id_t groupObj;	/* GROUP_OBJ principal */
    dacl_t *aclP;			/* ACL structure */
    int code = 0;			/* error return code */
    dacl_permset_t perm;		/* Temporary permset */
    epi_uuid_t sysAdminGroupID;		/* privilaged ID */
    epi_uuid_t networkRootID;		/* privileged ID */
    epi_uuid_t localCellID;
    pac_list_t **pacListPP;		/* paclist pointer pointer pointer */
    pac_list_t *pacListP;		/* pointer to pac list pointer */
    pac_list_t *lastDlgP = NULL;	/* pointer to pac list pointer */
    pac_list_t *tmpPacListP;		/* tmp pointer to pac list */
    pac_list_t PacList;			/* a bona-fide pac list */
    u_int32 needsFree = 0;		/* tells caller to free pac list */
    int superGroupMember;		/* set - dlg chain not in supergroup */
    int networkRoot;			/* set - network root */
    int checkDone = 0;

    if (dacl_perm_control & RO_RESTRICTED_BITS(evP)) {
	/* Check for read-only file system if an update is contemplated. */
	return (EROFS);
    }

    dacl_GetLocalCellID_r((afsUUID*)&localCellID);

    pacListPP = &pacListP;
    eacl_FindPacList(cred, &pacListPP, &needsFree);
    pacListP = *pacListPP;

    /*
     * XXX - This function needs to be reworked to avoid duplicate
     * checking of system admin group membership.
     */

    /*
     * Check for superuser. Two mutually exclusive cases:
     *	 Local access by local root
     *   DFS access by network root. Check only if NETWORK_ROOT_NOT_SUPERUSER
     *     macro not defined
     *
     */
    if (needsFree) {
	if (osi_GetUID(cred) == 0) {
	    *issuperP = 1;
	    *ingroupsP = 1;
	    goto cleanup;
	}
    } else {

	/*
	 * If called on behalf of remote user, check for network
	 * system administrator or network root
	 */
	superGroupMember = 1;
#ifndef NETWORK_ROOT_NOT_SUPERUSER
	networkRoot = efs_networkRootSuperUser;
#else
	networkRoot = 0;
#endif
	tmpPacListP = pacListP;
	dacl_GetSysAdminGroupID_r((afsUUID*)&sysAdminGroupID);
	dacl_GetNetworkRootID_r((afsUUID *)&networkRootID);
	while (tmpPacListP) {
	    lastDlgP = tmpPacListP;
	    if (networkRoot &&
		(Epi_PrinId_Cmp(&tmpPacListP->pacp->principal.uuid,
				&networkRootID) != 0)) {
		networkRoot = 0;
	    }
	    if (!vnax_GroupInPAC((epi_principal_id_t *)&sysAdminGroupID,
				 tmpPacListP->pacp)) {
		superGroupMember = 0;
	    }
	    if (!networkRoot && !superGroupMember)
		break;
	    tmpPacListP = tmpPacListP->next;
	}

	if (networkRoot || superGroupMember) {
	    *issuperP = 1;
	    *ingroupsP = 1;
	    goto cleanup;
	}
    }

    lock_ObtainWrite_r(&evP->vd_cache.lock);
    if (fstatP->acl > 0) {
	/* There's an ACL */
	bzero ((caddr_t)&userObj, sizeof (epi_principal_id_t));
	bzero ((caddr_t)&groupObj, sizeof (epi_principal_id_t));
	bcopy ((caddr_t)&fstatP->oid, (caddr_t)&userObj,
	       sizeof(fstatP->oid));
	bcopy ((caddr_t)&fstatP->gid, (caddr_t)&groupObj,
	       sizeof(fstatP->gid));

	code = 0;
	aclP = evP->vd_acl;
	if (!aclP) {
	    eacl_AllocateAcl (&aclP);
	    code = eacl_ReadAclFromAnode (aclP, EVTOA (evP), fstatP->acl,
					  EPIF_AUX_IS_ACL,
					  &episodeAclMgmtUuid);
	    if (!code)
		evP->vd_acl = aclP;
	    else
		eacl_ReleaseAcl (aclP);
	}
	if (!code) {
	    modebits = fstatP->mode & 0777;
	    perm = dacl_perm_control;
	    /*
	     * avoid super user / system admin check here as it has already
	     * been done above
	     */
	    code = dacl_epi_CheckAccessPac_r(aclP, &modebits, &perm,
					     pacListP, &userObj, &groupObj,
					     0 /* avoid super user check */);
	}
	if (code) {
	    switch (code) {
	      case DACL_ERROR_ACCESS_DENIED:
	      case DACL_ERROR_ACCESS_EXPLICITLY_DENIED:
		code = EPERM;
		break;
	      default:
		code = EIO;
	    }
	    lock_ReleaseWrite_r(&evP->vd_cache.lock);
	    goto cleanup;
	}
    } else {
	/* There's no ACL; just check for owner (in the local cell). */
	tmpPacListP = pacListP;
	while (tmpPacListP) {
	    lastDlgP = tmpPacListP;
	    tmpPacListP = tmpPacListP->next;
	}
	/*
	 * Requests from a different cell are denied access in the null ACL
	 * case since by default there are not any_other rights.
	 * This includes authenticated requests from a foreign
	 * cell and anonymous requests.
	 */
	if (!(lastDlgP->authenticated) ||
	    Epi_PrinId_Cmp (&lastDlgP->pacp->principal.uuid,
			    &fstatP->oid) != 0 ||
	    bcmp((char *)&pacListP->pacp->realm.uuid, (char *)&localCellID,
		 sizeof (epi_uuid_t)) != 0) {
	    code = EPERM;
	    lock_ReleaseWrite_r(&evP->vd_cache.lock);
	    goto cleanup;
	}
    }
    lock_ReleaseWrite_r(&evP->vd_cache.lock);

    *issuperP = 0;
    if (gidP)
	*ingroupsP = vnax_GidInPAC(gidP, pacListP->pacp);

cleanup:
    /*
     * Only need to free in the stand-alone case, so there's
     * only one pac_list_t to worry about.
     */
    if (needsFree) {
	if (pacListP->pacp->groups)
	    osi_Free_r(pacListP->pacp->groups,
		       pacListP->pacp->num_groups * sizeof (sec_id_t));
	osi_Free_r(pacListP->pacp, sizeof (sec_id_pa_t));
	osi_Free_r(pacListP, sizeof (pac_list_t));
    }

    return code;
}

/*
 * vnax_StickyCheck -- check whether sticky bit forbids us to unlink
 */

/* EXPORT */
int
vnax_StickyCheck (devP, evP, cred)
    struct evnode *devP;		/* Episode directory vnode */
    struct evnode *evP;			/* Episode vnode for object to be
					 * unlinked from devP */
    osi_cred_t *cred;			/* credential structure */
{
    epi_anode_t dap;			/* anode handle for devP */
    epi_anode_t ap;			/* anode handle for evP */
    struct epif_status dfstat;		/* file status info for devP */
    struct epif_status fstat;		/* file status info for evP */
    int issuper, ingroups;		/* dummy arguments for vnax_CanAdmin */
    int ccode;

    dap = EVTOA (devP);

    /* Check if directory is sticky */
    if (!(epif_GetMode (dap) & S_ISVTX))
	return 0;

    /* Set up for directory check */
    ccode = epif_GetStatus (dap, &dfstat);
    MBZ (ccode);

    /* Check if we own directory */
    if (vnax_CanAdminister (devP, &dfstat, 0, cred, &issuper, &ingroups) == 0)
	return 0;

    /* Set up for object checks */
    ap = EVTOA (evP);
    ccode = epif_GetStatus (ap, &fstat);
    MBZ (ccode);

    /* Check if we own object */
    if (vnax_CanAdminister (evP, &fstat, 0, cred, &issuper, &ingroups) == 0)
	return 0;

    /* SunOS only:  check if we can write object */
#ifdef AFS_SUNOS5_ENV
    return vnax_CanAccess (evP, &fstat, dacl_perm_write, cred);
#else
    return EPERM;
#endif
}

/*
 * vnax_DoChown -- change owner of a file
 */

/* EXPORT */
int
vnax_DoChown (evP, mask, afstatP, cred, uid, volop)
    struct evnode *evP;			/* Episode vnode */
    long *mask;				/* status fields for caller to set */
    struct epif_status *afstatP;	/* file status info for evp */
    osi_cred_t *cred;			/* credential structure */
    uid_t uid;				/* user id */
    int volop;				/* vol_efsSetattr path */
{
    int code = 0;
    int issuper;
    int ingroups;

    if (!volop) {
	/*
	 * error if not super-user and:
	 *	1) trying to change owner, or
	 *  2) not current owner, or
	 */
	code = vnax_CanAdminister (evP, afstatP, 0, cred,
				   &issuper, &ingroups);
	if (code)
	    return code;
	if (afstatP->oid != uid && !issuper)
	    return EPERM;
	if (!issuper) {
	    afstatP->mode &= ~(VSUID | VSGID);
	    *mask |= EPIF_SFLAGS_MODE;
	}
    }

    /* Change the uid */
    *mask |= EPIF_SFLAGS_OID;
    afstatP->oid = uid;

    /* FINIS */

    return (0);
}

/*
 * vnax_DoChgrp -- change group of a file
 */

/* EXPORT */
int
vnax_DoChgrp (evP, mask, afstatP, cred, gid, volop)
    struct evnode *evP;			/* Episode vnode */
    long *mask;				/* status fields for caller to set */
    struct epif_status *afstatP;	/* file status info for evp */
    osi_cred_t *cred;			/* credential structure */
    gid_t gid;				/* group id */
    int volop;				/* vol_efsSetattr path */
{
    int code = 0;
    int issuper;
    int ingroups;
    u_long temp_gid;

    if (!volop) {
	/* Use another variable even though not strictly required
	 * to be defensive */
	temp_gid = gid;
	/*
	 * error if not super-user and:
	 *  1) new group is not a member of process group set
	 */
	code = vnax_CanAdminister (evP, afstatP, &temp_gid, cred,
				   &issuper, &ingroups);
	if (code)
	    return code;
	if (afstatP->gid != gid && !issuper && !ingroups)
	    return EPERM;
	if (!issuper) {
	    afstatP->mode &= ~(VSUID | VSGID);
	    *mask |= EPIF_SFLAGS_MODE;
	}
    }
    /* Change the gid */
    *mask |= EPIF_SFLAGS_GID;
    afstatP->gid = gid;

    /* FINIS */

    return (0);
}

/*
 * vnax_CorrectAclRealm -- check if realm of acl matches owner's realm
 * and change acl realm if not. If file/dir has no acl and owner's
 * realm does not match realm of dummy regular acl, then sets
 * regular acl with owner's realm. If dummy/explicit regular acl for dir
 * does not have same realm as owner, and explicit initial acls exist
 * for dir, correct the realm in these acls to the owner's realm.
 */
int vnax_CorrectAclRealm (evp, realmp, auth, transId)
    struct evnode *evp;			/* newly created Episode vnode */
    epi_uuid_t *realmp;			/* realm of principal */
    int auth;				/* is principal authenticated? */
    logTranRec_t transId;		/* transaction ID */
{
    int code = 0;			/* error return code */
    struct epif_status fstat;		/* status info for evp */
    dacl_t *aclp;			/* acl pointer */
    epi_uuid_t aclRealm;
    u_long explicitRegularAcl;

    epif_GetStatus (EVTOA (evp), &fstat);
    explicitRegularAcl = fstat.acl;

    lock_ObtainWrite_r(&evp->vd_cache.lock);
    /* Split on whether object has explicit regular acl */
    if (explicitRegularAcl) {
	/* Read and cache acl if not already cached */
	if (!evp->vd_acl) {
	    eacl_AllocateAcl(&aclp);
	    /* file already has ACL (inherited) */
	    code = eacl_ReadAclFromAnode (aclp, EVTOA (evp), fstat.acl,
					  EPIF_AUX_IS_ACL,
					  &episodeAclMgmtUuid);
	    if (code) {
		eacl_ReleaseAcl (aclp);
		VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);
		return code;
	    }
	    evp->vd_acl = aclp;
	}
    } else {
	u_int32 mode = fstat.mode;
	eacl_AllocateAcl(&aclp);
	dacl_InitEpiAcl_r (aclp, &mode, EV_ISDIR (evp), (epi_uuid_t *)NULL);
	evp->vd_acl = aclp;
    }

    afsl_Assert(evp->vd_acl != NULL);

    dacl_GetDefaultRealm_r(evp->vd_acl, &aclRealm);
    if (bcmp((char *)realmp, (char *)&aclRealm, sizeof (epi_uuid_t)) == 0) {
	/* Do not cache a dummy acl */
	if (!explicitRegularAcl) {
	    eacl_ReleaseAcl (evp->vd_acl);
	    evp->vd_acl = 0;
	}
	lock_ReleaseWrite_r(&evp->vd_cache.lock);
	return 0;
    }

    /* ACL realm is not the owner's realm, change acl realms */
    code = vnax_SetForeignRegularAcl(evp, realmp, auth, transId);
    if (!code  && EV_ISDIR(evp)) {
	code = vnax_SetForeignInitialAcl(evp, realmp, auth, transId,
					 fstat.initialDirAcl,
					 EPIF_AUX_IS_INITIAL_DIR_ACL);
	if (!code) {
	    code = vnax_SetForeignInitialAcl(evp, realmp, auth, transId,
					     fstat.initialFileAcl,
					     EPIF_AUX_IS_INITIAL_FILE_ACL);
	}
    }
    if (code) {
	VNM_MAP_COMMON_ERROR(code, DEBUG_THIS_FILE);
	eacl_ReleaseAcl(evp->vd_acl);
	evp->vd_acl = 0;
    }
    lock_ReleaseWrite_r(&evp->vd_cache.lock);
    return code;
}

/*
 * vnax_SetForeignRegularAcl -- set regular ACL for a file/dir created by a
 * foreign user
 *
 * LOCKING: cache lock held by caller
 */
int vnax_SetForeignRegularAcl(evp, realmp, auth, transId)
    struct evnode *evp;			/* newly created Episode vnode */
    epi_uuid_t *realmp;			/* realm of principal */
    int auth;				/* is principal authenticated? */
    logTranRec_t transId;		/* transaction ID */
{
    int code = 0;			/* error return code */
    char *bbP = 0;			/* buffer for flattened ACL */
    u_int len = 0;			/* length of flattened ACL */
    dacl_t *aclp;

    aclp = evp->vd_acl;
    afsl_Assert(aclp != NULL);
    dacl_ChangeRealm_r (aclp, realmp);

    if (!auth)
	dacl_ChangeUnauthMask_r (aclp, ~0L);

    osi_UnmakePreemptionRight();
    code = dacl_FlattenAcl (aclp, &bbP, &len);
    osi_MakePreemptionRight();
    afsl_MBZ (code);

    code = epif_WriteAux (transId, EVTOA (evp),
		EPIF_AUX_IS_ACL | EPIF_FLAGS_NEW_AUX | EPIF_FLAGS_TRUNCATE_AUX,
			  0, len, bbP);

    osi_Free_r (bbP, (long) len);
    return code;
}

/*
 * vnax_SetForeignInitialAcl -- change realms of initial ACLs for a dir
 * created by a foreign user.
 */
int vnax_SetForeignInitialAcl(evp, realmp, auth, transId, alength, flag)
    struct evnode *evp;			/* newly created Episode vnode */
    epi_uuid_t *realmp;			/* realm of principal */
    int auth;				/* is principal authenticated? */
    logTranRec_t transId;		/* transaction ID */
    int alength;			/* length of initial acl */
    int flag;				/* which initial ACL */
{
    int code = 0;			/* error return code */
    dacl_t *aclp;			/* acl pointer */
    char *bbP = 0;			/* buffer for flattened ACL */
    u_int len = 0;			/* length of flattened ACL */


    afsl_Assert(flag == EPIF_AUX_IS_INITIAL_DIR_ACL ||
		flag == EPIF_AUX_IS_INITIAL_FILE_ACL);

    if (!alength) return 0;

    /* Initial acl is present. Convert realms */
    aclp = osi_Alloc_r (sizeof (dacl_t));
    code = eacl_ReadAclFromAnode (aclp, EVTOA (evp), alength,
				  flag, &episodeAclMgmtUuid);
    if (!code) {
	dacl_ChangeRealm_r (aclp, realmp);

	if (!auth)
	    dacl_ChangeUnauthMask_r (aclp, ~0L);

	osi_UnmakePreemptionRight();
	code = dacl_FlattenAcl (aclp, &bbP, &len);
	osi_MakePreemptionRight();
	afsl_MBZ(code);

	code = epif_WriteAux (transId, EVTOA (evp),
			      flag | EPIF_FLAGS_NEW_AUX |
			      EPIF_FLAGS_TRUNCATE_AUX,
			      0, len, bbP);
    }

    eacl_ReleaseAcl (aclp);
    osi_Free_r (bbP, (long) len);

    return code;
}
