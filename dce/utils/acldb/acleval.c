/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acleval.c,v $
 * Revision 1.1.8.2  1996/10/04  20:05:45  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  20:05:28  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/08/15  20:31 UTC  jrr
 * 	Fix foreign groupset evaluation loop in dce_acl__permset_alg().
 * 	[1996/08/15  20:30 UTC  jrr  /main/DCE_1.2.2/jrr_122_2/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/07/25  16:18 UTC  psn
 * 	OT13278 (foreign groupset check bug).
 * 	[1996/07/24  22:02 UTC  burati  /main/mb_u2u2/1]
 * 
 * Revision 1.1.6.3  1996/02/18  19:19:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:23  marty]
 * 
 * Revision 1.1.6.2  1995/12/08  21:35:16  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/10/20  21:37 UTC  burati
 * 	Merge fix for CHFts16609/OT13148 from mb_mothra7
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/29  20:40 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/11  19:38 UTC  tatsu_s
 * 	Fixed the calculation/use of unauth_mask.
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/25  20:11 UTC  burati
 * 	Merge change for chfts14249
 * 
 * 	HP revision /main/burati_chfts14249/1  1995/03/15  21:13 UTC  burati
 * 	Fix for CHFts14249
 * 	[1995/12/08  18:07:09  root]
 * 
 * Revision 1.1.2.6  1994/09/20  17:34:45  bowe
 * 	Free cred cursor when done with it. [CR 12255]
 * 	[1994/09/20  17:34:26  bowe]
 * 
 * Revision 1.1.2.5  1994/09/15  14:37:07  bowe
 * 	NULL for unused returns from rpc_binding_inq_auth_caller() [CR 12196]
 * 	[1994/09/15  14:36:53  bowe]
 * 
 * Revision 1.1.2.4  1994/07/26  21:07:58  bowe
 * 	Set check_groups flag for sec_acl_e_type_group_obj and its _deleg.
 * 	Removed some unused local variables.  [CR 11378]
 * 	[1994/07/26  21:07:43  bowe]
 * 
 * Revision 1.1.2.3  1994/06/06  15:07:07  rsalz
 * 	Treat NIL UUID as local realm.
 * 	[1994/06/06  15:07:01  rsalz]
 * 
 * Revision 1.1.2.2  1994/05/18  19:17:40  bowe
 * 	Init some pointers in dce_acl__inq_unauthen_perms() [10657]
 * 	[1994/05/16  16:01:21  bowe]
 * 
 * Revision 1.1.2.1  1994/04/26  19:11:26  bowe
 * 	Initial checkin (was part of dceaclimpl.c, now defunct)
 * 	[1994/04/26  19:09:26  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include <aclimpl.h>

/* ______________________________________________________________________ */
/*
 * Macro to mask entry permset against mask_obj if necessary and return
 * the result of that operation.  Note that this macro was ultimately derived
 * from the DEC Security private routine "access_granted" and should track the
 * intended functionality of that routine.  Note also that the choice of ACLE
 * types in the lists below is based on the definition of MASK_OBJ and not on
 * the application requirements.
 */

#define SAET_no_mask_obj	\
    (	(1 << sec_acl_e_type_user_obj) | \
	(1 << sec_acl_e_type_user_obj_deleg) | \
	(1 << sec_acl_e_type_other_obj) | \
	(1 << sec_acl_e_type_other_obj_deleg) | \
	(1 << sec_acl_e_type_mask_obj) | \
	(1 << sec_acl_e_type_unauthenticated) )

#define SAET_use_mask_obj	\
    (	(1 << sec_acl_e_type_group_obj) | \
	(1 << sec_acl_e_type_group_obj_deleg) | \
	(1 << sec_acl_e_type_any_other) | \
	(1 << sec_acl_e_type_any_other_deleg) | \
	(1 << sec_acl_e_type_user) | \
	(1 << sec_acl_e_type_user_deleg) | \
	(1 << sec_acl_e_type_group) | \
	(1 << sec_acl_e_type_group_deleg) | \
	(1 << sec_acl_e_type_foreign_other) | \
	(1 << sec_acl_e_type_for_other_deleg) | \
	(1 << sec_acl_e_type_foreign_user) | \
	(1 << sec_acl_e_type_for_user_deleg) | \
	(1 << sec_acl_e_type_foreign_group) | \
	(1 << sec_acl_e_type_for_group_deleg) | \
	(1 << sec_acl_e_type_extended) )

#define MASKIT(ep, mask_perms) ( \
	    (1 << (int)ep->entry_info.entry_type) & SAET_no_mask_obj \
	    ?  ep->perms \
	    :  (1 << (int)ep->entry_info.entry_type) & SAET_use_mask_obj \
		?  ep->perms & mask_perms : dce_acl_c_no_permissions)

#define SAME_FOREIGN_REALM(entry) \
    uuid_equal(&pa_p->realm.uuid, &ep->entry_info.tagged_union.foreign_id.realm.uuid, st)

/* ______________________________________________________________________ */
/*  Checks whether client's credentials are authenticated and that they
 *  grant the desired permissions.
 */
void
dce_acl_is_client_authorized(
  /* [in] */
    handle_t		h,
    uuid_t		*mgr_type,
    uuid_t		*acl_uuid,
    uuid_t		*own_id,
    uuid_t		*grp_id,
    sec_acl_permset_t	desired_perms,
  /* [out] */
    boolean32		*authorized,
    error_status_t	*st
)
{
    sec_acl_permset_t	perms;

    *authorized = FALSE;
    /* Get ACL for this object/mgr type */
    dce_acl_inq_client_permset(h, mgr_type, acl_uuid, own_id, grp_id, &perms, st);
    STAT_CHECK_RET(*st);
    if ( (desired_perms & perms) == desired_perms )
	*authorized = TRUE;
}

/* ______________________________________________________________________ */
/*  Checks whether credentials are those of "unauthenticated".
 *  We declare the caller unauthenticated when rpc_binding_inq_auth_caller()
 *  says no_authen.
 */
static int			s_unauthen_cred_int;
static rpc_authz_cred_handle_t	s_unauthen_cred_h = { 0, &s_unauthen_cred_int };

boolean32
dce_acl_is_unauthenticated(
    rpc_authz_cred_handle_t	cred_h
)
{
    return (cred_h.data == &s_unauthen_cred_int) ? TRUE : FALSE;
}

/* ______________________________________________________________________ */
/*  Check what perms the client has, given the ACL.
 */
void
dce_acl_inq_client_permset(
  /* [in] */
    handle_t		h,
    uuid_t		*mgr_type,
    uuid_t		*acl_uuid,
    uuid_t		*own_id,
    uuid_t		*grp_id,
  /* [out] */
    sec_acl_permset_t	*permset,
    error_status_t	*st
)
{
    rpc_authz_cred_handle_t	cred_h;
    sec_acl_t			acl;
    acl_mgt_t			*am_info;

    *st = error_status_ok;
    *permset = dce_acl_c_no_permissions;

    /* Get database handle */
    am_info = dce_acl__get_info_from_type(mgr_type, st);
    STAT_CHECK_RET(*st);

    /* Read acl from database.  Key is uuid of acl. */
    dce_db_fetch_by_uuid(am_info->db, acl_uuid, &acl, st);
    STAT_CHECK_RET(*st);

    /* check that registered mgr type matches ACL's mgr type */
    if (!uuid_equal(&am_info->mgr_type, &acl.sec_acl_manager_type, st)) {
	*st = acl_s_bad_manager_type;
	return;
    }

    /* Get the client's (caller's) credentials */
    dce_acl_inq_client_creds(h, &cred_h, st);
    STAT_CHECK_RET(*st);

    /* Compare ACL with PAC, determining permissions. */
    dce_acl_inq_permset_for_creds(cred_h, &acl, own_id, grp_id,
	sec_acl_posix_no_semantics, permset, st);
}

/* ______________________________________________________________________ */
/*  Return the client's PAC (Privilege Attribute Certificate).
 *  This is gotten from the RPC binding handle.
 */
void
dce_acl_inq_client_creds(
  /* [in] */
    handle_t			h,
  /* [out] */
    rpc_authz_cred_handle_t	*cred_h,
    error_status_t		*st
)
{
    unsigned32			authz_svc;

    *st = error_status_ok;
    *cred_h = s_unauthen_cred_h;	/* default case */

    /* Get caller's credentials */
    rpc_binding_inq_auth_caller(h, cred_h, NULL, NULL, NULL, &authz_svc, st);
    if (*st == rpc_s_binding_has_no_auth) {
	/* unauthenticated - cred_h is already NULL; just return. */
	*st = error_status_ok;
	return;
    }
    if (*st != rpc_s_ok)
	return;

    /* What kind of authen does client want? */
    if (authz_svc != rpc_c_authz_dce) {
	*st = rpc_s_authn_authz_mismatch;
	return;
    }
/*    *cred_pa = sec_cred_get_initiator(rpc_creds, st);*/
}

/* ______________________________________________________________________ */

/*  This is an outline of "Extended Access Control Algorithm", which
 *  I got from the Security AES/DC (and the Delegation and EPAC spec).
 *	Check initiator:
 *	  Apply standard algorithm
 *	  If access denied, then deny access and return
 *	Check each intermediary (delegate):
 *	  For each extended PAC:
 *	    Apply standard algorithm, but also consider delegate ACLEs
 *	    If access denied, then deny access and return
 *	Grant access.
 *
 *  (A difference is that we need is to retrieve the permissions, not
 *  just say grant/deny. The grant/deny function will call the function
 *  that retrieves permissions, then compare.)
 *
 *  This is the order for matching in the standard algorithm.  In the
 *  extended (with delegates) algorithm the delegate match/tests come
 *  immediately after the corresponding "regular" entry tests.
 *	 1. USER_OBJ
 *	 2. USER
 *	 3. FOREIGN_USER
 *	 4. GROUP_OBJ
 *	 5. GROUP
 *	 6. FOREIGN_GROUP
 *	 7. OTHER_OBJ
 *	 8. FOREIGN_OTHER
 *	 9. ANY_OTHER
 *	10. UNAUTH
 */

/*
 * Determine a principal's "complete extent of access" to some object. This
 * is useful for implementing operations like the conventional UNIX access
 * function.  This routine implements a compatible subset of the DCE ACLs.
 *
 * The principal is represented by its credentials (PAC) and the object is
 * represented by its ACL (ap).  The principle output parameter
 * (access_extent) is a mask containing a 1 for each permission granted by
 * first ACL Entry category (see below) of the ACL which the PAC matches, and
 * 0s elsewhere.  Note that the PAC may be NULL or unauthenticated.
 *
 * All ACL Entry types (sec_acl_e_type_*) are supported by this routine
 * and are in categories checked in the order shown below:
 *
 *	user,foreign_user	At most one match
 *	group,foreign_group	Union of all permissions for each matching group
 *	other_obj		Matching local realm accesses
 *	foreign_other		At most one can match
 *	any_other		Catch-all
 *	unauthenticated		(Note 1)
 *
 * Note: sec_acl_e_type_unauthenticated is a mask used for all matches on
 * unauthenticated PACs.  It is also intersected with sec_acl_e_type_any_other
 * for NULL PACs.
 *
 * NB:	Note that this routine does not care at all about the meaning of any
 *	particular permission bit.
 *
 * IMPLEMENTATION NOTES:
 *
 * Note that this routine does virtually no error checking in the interest of
 * speed.
 */

/* ______________________________________________________________________ */

/* Some notes on ACLE types - sec_acl_e_type_*, that is:
    These can only occur once in an ACL:
	user_obj group_obj other_obj
	mask_obj
	unauthenticated
	any_other
	user_obj_deleg group_obj_deleg other_obj_deleg
	any_other_deleg

    These can occur multiple times, and "take" a sec_id_t:
	user group foreign_other
	user_deleg group_deleg for_other_deleg

    These can occur multiple times, and "take" a sec_id_foreign_t:
	foreign_user foreign_group
	for_user_deleg for_group_deleg

    Only one user, foreign_other, user_deleg, for_other_deleg can match
    what's in the PAC, however.  Same for foreign_user, for_user_deleg.
*/


/* Determine and return the perms for the completely unauthenticated
 * (no credentials sent at all) user.
 * 
 * Without credentials, you can only match against the any_other entry
 * type, and the unauthenticated mask must be applied (mask_perms apply
 * too if a mask entry exists).
 */
static void
dce_acl__inq_unauthen_perms(
  /* [in] */
    sec_acl_t			*ap,
  /* [out] */
    sec_acl_permset_t		*perms
)
{
    sec_acl_entry_t	*any_other_ep, *unauth_ep, *other_obj_ep, *ep;
    sec_acl_permset_t	mask_perms, tmp1, tmp2;
    int			i;

    mask_perms = MAX_PERMISSIONS;
    any_other_ep = unauth_ep = other_obj_ep = NULL;
    for (i=ap->num_entries, ep=ap->sec_acl_entries; --i>=0; ep++) {
	switch (ep->entry_info.entry_type) {
	default:
	    break;
	case sec_acl_e_type_any_other:
	    any_other_ep = ep;
	    break;
	case sec_acl_e_type_unauthenticated:
	    unauth_ep = ep;
	    break;
	case sec_acl_e_type_mask_obj:
	    mask_perms = ep->perms;
	    break;
	}
    }

    if (any_other_ep && unauth_ep) {
	*perms = (MASKIT(any_other_ep, mask_perms)) & unauth_ep->perms;
    } else {
	*perms = dce_acl_c_no_permissions;
    }
    return;
}


/*
 * The access algorithm.
 * 
 * Find entries that match the caller and return the
 * permissions granted to that caller.
 */
static void
dce_acl__permset_alg(
  /* [in] */
    boolean			do_deleg,
    sec_id_pa_t			*pa_p,
    sec_acl_t			*ap,
    uuid_t			*own_id,
    uuid_t			*grp_id,
  /* [out] */
    sec_acl_permset_t		*unauth_mask,
  /* [in] */
    sec_acl_posix_semantics_t	posix_semantics,
  /* [out] */
    sec_acl_permset_t		*perms,
    error_status_t		*st
)
{
    boolean		chk_loc_groups, in_def_realm, one_group_found;
    boolean		check_groups;
    sec_acl_entry_t	*user_obj_ep, *user_ep, *for_user_ep;
    sec_acl_entry_t	*other_obj_ep, *for_other_ep, *any_other_ep;
    sec_acl_entry_t	*user_obj_del_ep, *user_del_ep, *for_user_del_ep;
    sec_acl_entry_t	*other_obj_del_ep, *for_other_del_ep;
    sec_acl_entry_t	*any_other_del_ep, *ep;
    sec_acl_permset_t	group_access, mask_perms, tmp1;
    sec_id_t		*gp;
    uuid_t		*gr_uuid;
    sec_id_t		*realm;
    int			i, j;

    *st = error_status_ok;
    *perms = dce_acl_c_no_permissions;

    /* If the mask isn't explicitly set, then it should have no effect when
     * masking, so turn all perm bits on by default. */
    mask_perms  = MAX_PERMISSIONS;

    user_obj_ep = user_ep = for_user_ep =
	other_obj_ep = for_other_ep = any_other_ep =
	user_obj_del_ep = user_del_ep = for_user_del_ep =
	other_obj_del_ep = for_other_del_ep = any_other_del_ep = NULL;

    /* We have a PAC.  Pre-process the acl entries so we don't have to loop
     * through the entries as many times.  Note, the accessor may be a member
     * of multiple [foreign_]group[_deleg]. Therefore, the group checks
     * must be done afterwords, in a separate pass through the entry list.
     */

    /* Set this just once now we know we have a PAC */
    in_def_realm = uuid_equal(&pa_p->realm.uuid, &ap->default_realm.uuid, st)
	|| uuid_is_nil(&ap->default_realm.uuid, st);

    check_groups = FALSE;
    for (i=ap->num_entries, ep=ap->sec_acl_entries; --i>=0; ep++) {

	/* Check for existence of each type of entry, and keep track
         * of where each type was found in the entry list. */
	switch (ep->entry_info.entry_type) {
	case sec_acl_e_type_user:
	    if (in_def_realm && uuid_equal(&pa_p->principal.uuid,
			&ep->entry_info.tagged_union.id.uuid, st))
		user_ep = ep;
	    break;
	case sec_acl_e_type_foreign_user:
	    if (uuid_equal(&pa_p->principal.uuid,
		    &ep->entry_info.tagged_union.foreign_id.id.uuid, st)
	     && SAME_FOREIGN_REALM(ep))
		for_user_ep = ep;
	    break;
	case sec_acl_e_type_other_obj:
	    if (in_def_realm)
		other_obj_ep = ep;
	    break;
	case sec_acl_e_type_foreign_other:
	    if (uuid_equal(&pa_p->realm.uuid,
		    &ep->entry_info.tagged_union.id.uuid, st))
		for_other_ep = ep;
	    break;
	case sec_acl_e_type_unauthenticated:
	    *unauth_mask = ep->perms;
	    break;
	case sec_acl_e_type_group:
	case sec_acl_e_type_foreign_group:
	case sec_acl_e_type_group_obj:
	    /* These are ignored by this pass over the acl.  Just flag that
	     * we should check groups later.
	     */
	    check_groups = TRUE;
	    break;
	case sec_acl_e_type_any_other:
	    any_other_ep = ep;
	    break;
	case sec_acl_e_type_mask_obj:
	    mask_perms = ep->perms;
	    break;

	case sec_acl_e_type_user_obj:
	    /* If user_obj entry exists, owner cannot be NULL */
	    if (own_id == NULL) {
	        *st = sec_acl_expected_user_obj;
	        return;
	    }
	    if (in_def_realm && uuid_equal(own_id, &pa_p->principal.uuid,
                st))
		user_obj_ep = ep;
	    break;
	    /* Unsupported ACL entry types. */
	default:
	    break;
	}

	/* with delegation */
	if (do_deleg) {
	    switch (ep->entry_info.entry_type) {
	    case sec_acl_e_type_user_obj_deleg:
	        /* If user_obj entry exists, owner cannot be NULL */
	        if (own_id == NULL) {
	            *st = sec_acl_expected_user_obj;
	            return;
	        }
	        if (in_def_realm && uuid_equal(own_id, &pa_p->principal.uuid,
                    st))
		    user_obj_del_ep = ep;
		break;
	    case sec_acl_e_type_user_deleg:
		if (in_def_realm && uuid_equal(&pa_p->principal.uuid,
			    &ep->entry_info.tagged_union.id.uuid, st))
		    user_del_ep = ep;
		break;
	    case sec_acl_e_type_for_user_deleg:
		if (uuid_equal(&pa_p->principal.uuid,
			&ep->entry_info.tagged_union.foreign_id.id.uuid, st)
		 && SAME_FOREIGN_REALM(ep))
		    for_user_del_ep = ep;
		break;
	    case sec_acl_e_type_group_deleg:
	    case sec_acl_e_type_for_group_deleg:
	    case sec_acl_e_type_group_obj_deleg:
		check_groups = TRUE;
		break;
	    case sec_acl_e_type_other_obj_deleg:
		if (in_def_realm)
		    other_obj_del_ep = ep;
		break;
	    case sec_acl_e_type_for_other_deleg:
		if (uuid_equal(&pa_p->realm.uuid,
			&ep->entry_info.tagged_union.id.uuid, st))
		    for_other_del_ep = ep;
		break;
	    case sec_acl_e_type_any_other_deleg:
		any_other_del_ep = ep;
		break;
	    default:
		break;
	    }
	}
    }


    /* Now that we know which entries match the user described in the PAC,
     * check the permissions corresponding to each entry until access is
     * granted by one of them. */

    /* USER_OBJ check (masking is differnt than the others) */
    if (user_obj_ep) {
	*perms = user_obj_ep->perms;
	if (!(do_deleg && user_obj_del_ep)) return;
    }

    /* USER_OBJ_DELEG check (masking is differnt than the others) */
    if (do_deleg && user_obj_del_ep) {
	*perms |= user_obj_del_ep->perms;
	return;
    }

    /* USER check */
    if (user_ep) {
	*perms = MASKIT(user_ep, mask_perms);
	if (!(do_deleg && user_del_ep)) return;
    }

    /* USER_DELEG check */
    if (do_deleg && user_del_ep) {
	*perms |= MASKIT(user_del_ep, mask_perms);
	return;
    }

    /* FOREIGN_USER check */
    if (for_user_ep) {
	*perms = MASKIT(for_user_ep, mask_perms);
	if (!(do_deleg && for_user_del_ep)) return;
    }

    /* FOREIGN_USER_DELEG check */
    if (do_deleg && for_user_del_ep) {
	*perms |= MASKIT(for_user_del_ep, mask_perms);
	return;
    }

    /* GROUP checks */
    one_group_found = FALSE;
    if (check_groups) {
	sec_acl_entry_type_t	e_type;
	group_access = dce_acl_c_no_permissions;
	chk_loc_groups  = FALSE;
	for (i=ap->num_entries, ep=ap->sec_acl_entries; --i>=0; ep++) {
	    e_type = ep->entry_info.entry_type;
	    switch (e_type) {
	    default:
		break;
	    case sec_acl_e_type_group_obj:
	    case sec_acl_e_type_group_obj_deleg:
	    case sec_acl_e_type_group:
	    case sec_acl_e_type_group_deleg:
	    case sec_acl_e_type_foreign_group:
	    case sec_acl_e_type_for_group_deleg:
		/* GROUP_OBJ, GROUP_OBJ_DELEG */
		if (e_type == sec_acl_e_type_group_obj ||
				e_type == sec_acl_e_type_group_obj_deleg) {
		    /* If group_obj entry exists then group_obj can't be NULL */
		    if (grp_id == NULL) {
			*st = sec_acl_expected_group_obj;
			return;
		    }
		    chk_loc_groups = in_def_realm;
		    gr_uuid = grp_id;
		    realm   = &ap->default_realm;
		}
		/* GROUP, GROUP_DELEG */
		else if (e_type == sec_acl_e_type_group ||
			    e_type == sec_acl_e_type_group_deleg) {
		    chk_loc_groups = in_def_realm;
		    gr_uuid = &ep->entry_info.tagged_union.id.uuid;
		    realm   = &ap->default_realm;
		}
		/* FOREIGN_GROUP, FOREIGN_GROUP_DELEG */
		else {		/* foreign groups */
		    chk_loc_groups = SAME_FOREIGN_REALM(ep);
		    gr_uuid = &ep->entry_info.tagged_union.foreign_id.id.uuid;
		    realm   = &ep->entry_info.tagged_union.foreign_id.realm;
		}

		/* Check either the local groups or the foreign groups */
		if (chk_loc_groups) {
		    if (uuid_equal(&pa_p->group.uuid, gr_uuid, st)) {
			one_group_found = TRUE;
			tmp1 = MASKIT(ep, mask_perms);
			group_access |= tmp1;
		    }

		    /* CHECK LOCAL GROUPS */
		    for (gp=pa_p->groups, j=pa_p->num_groups; --j>=0; gp++)
			if (uuid_equal(&gp->uuid, gr_uuid, st)) {
			    one_group_found = TRUE;
			    tmp1 = MASKIT(ep, mask_perms);
			    group_access |= tmp1;
			}
		}
		else {
		    int	fg_i, fgs_i;
		    sec_id_foreign_groupset_t *fgs;

		    /* CHECK FOREIGN GROUPS (which are in groupsets) */
		    fgs = pa_p->foreign_groupsets;
		    for (fgs_i=pa_p->num_foreign_groupsets; --fgs_i>=0; fgs++) {
			if (!uuid_equal(&fgs->realm.uuid, &realm->uuid, st))
			    continue;
			gp = fgs->groups;
			for (fg_i=fgs->num_groups; --fg_i>=0; gp++) {
			    if (uuid_equal(&gp->uuid, gr_uuid, st)) {
				one_group_found = TRUE;
				tmp1 = MASKIT(ep, mask_perms);
				group_access |= tmp1;
			    }
			}
		    }
		}
		break;
	    }
	}
    }

    /* If one or more groups found then we take the union of all matching
     * group's permissions and call that the access extent. */
    if (one_group_found) {
	*perms = group_access;
	return;
    }

    /* OTHER_OBJ check */
    if (other_obj_ep) {
	*perms = MASKIT(other_obj_ep, mask_perms);
	if (!(do_deleg && other_obj_del_ep)) return;
    }

    /* OTHER_OBJ_DELEG check */
    if (do_deleg && other_obj_del_ep) {
	*perms |= MASKIT(other_obj_del_ep, mask_perms);
	return;
    }

    /* FOREIGN_OTHER check */
    if (for_other_ep) {
	*perms = MASKIT(for_other_ep, mask_perms);
	if (!(do_deleg && for_other_del_ep)) return;
    }

    /* FOREIGN_OTHER_DELEG check */
    if (do_deleg && for_other_del_ep) {
	*perms |= MASKIT(for_other_del_ep, mask_perms);
	return;
    }

    /* ANY_OTHER check */
    if (any_other_ep) {
	*perms = MASKIT(any_other_ep, mask_perms);
	if (!(do_deleg && any_other_del_ep)) return;
    }

    /* ANY_OTHER_DELEG check */
    if (do_deleg && any_other_del_ep) {
	*perms |= MASKIT(any_other_del_ep, mask_perms);
	return;
    }

    /* No ACL entry gave any access. */
    *perms = dce_acl_c_no_permissions;
    return;
}


void
dce_acl_inq_permset_for_creds(
  /* [in] */
    rpc_authz_cred_handle_t	cred_h,
    sec_acl_t			*ap,
    uuid_t			*own_id,
    uuid_t			*grp_id,
    sec_acl_posix_semantics_t	posix_semantics,
  /* [out] */
    sec_acl_permset_t		*perms,
    error_status_t		*st
)
{
    boolean			auth, ret;
    sec_acl_permset_t		unauth_mask;
    sec_id_pa_t			*pa_p;
    sec_cred_pa_handle_t	cred_pa;
    sec_cred_cursor_t		del_cursor;
    error_status_t		xst;

    *st = error_status_ok;
    *perms = dce_acl_c_no_permissions;
    unauth_mask = dce_acl_c_no_permissions;

    /* If there was no credential information at all from the runtime,
     * request access permissions for unauthenticated any_other.
     */
    if (dce_acl_is_unauthenticated(cred_h)) {
	dce_acl__inq_unauthen_perms(ap, perms);
	return;
    }

    cred_pa = sec_cred_get_initiator(cred_h, st);
    STAT_CHECK_RET(*st);
    pa_p = sec_cred_get_pa_data(cred_pa, st);
    STAT_CHECK_RET(*st);

    auth = sec_cred_is_authenticated(cred_h, st);

    dce_acl__permset_alg(FALSE, pa_p, ap, own_id, grp_id,
		&unauth_mask, posix_semantics, perms, st);
    if (st != error_status_ok) {
	return;
    }

    /* Now, loop through the delegates, doing the same check. */
    sec_cred_initialize_cursor(&del_cursor, st);
    STAT_CHECK_RET(*st);
    for (;;) {
	if (*perms == dce_acl_c_no_permissions) {
	    sec_cred_free_cursor(&del_cursor, &xst);
	    return;
	}
	cred_pa = sec_cred_get_delegate(cred_h, &del_cursor, st);
	if (*st == sec_cred_s_no_more_entries) {
	    *st = error_status_ok;
	    break;
	}
	STAT_CHECK_RET(*st);
	pa_p = sec_cred_get_pa_data(cred_pa, st);
	if (*st != error_status_ok) {
	    sec_cred_free_cursor(&del_cursor, &xst);
	    *perms = dce_acl_c_no_permissions;
	    return;
	}

	dce_acl__permset_alg(TRUE, pa_p, ap, own_id, grp_id,
		&unauth_mask, posix_semantics, perms, st);
	if (*st != error_status_ok) {
	    sec_cred_free_cursor(&del_cursor, &xst);
	    *perms = dce_acl_c_no_permissions;
	    return;
	}
    }
    sec_cred_free_cursor(&del_cursor, &xst);

    /* If creds were not authenticated then apply the unauth mask perms */
    if (!auth) {
	*perms &= unauth_mask;
    }
}
