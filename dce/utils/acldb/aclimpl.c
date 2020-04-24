/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: aclimpl.c,v $
 * Revision 1.1.6.1  1996/08/09  11:56:14  arvind
 * 	Fix OTs 12929: The acldb library uses rpc_sm_client_free instead of
 * 	rpc_sm_free.
 * 	[1996/07/23  15:58 UTC  psn  /main/psn_bugfix/1]
 *
 * Revision 1.1.4.2  1996/02/18  19:20:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:25  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  21:35:20  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  20:40 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/4  1995/09/23  00:14 UTC  tatsu_s
 * 	Cleaned up error path.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/3  1995/09/20  13:20 UTC  tatsu_s
 * 	Do not check authorization in dce__rdacl_lookup() if called locally
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/2  1995/09/19  13:09 UTC  tatsu_s
 * 	Added dce_acl_unregister_object_type().
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/11  19:36 UTC  tatsu_s
 * 	Call resolver with writing:true in dce_rdacl_replace().
 * 	[1995/12/08  18:07:13  root]
 * 
 * Revision 1.1.2.4  1994/07/27  21:26:33  bowe
 * 	Handle ACL entries for delegation (1.0 vs 1.1 issues) [CR 10001]
 * 	Check return status of resolver in dce__rdacl_lookup. [CR 11008]
 * 	[1994/07/27  21:26:12  bowe]
 * 
 * Revision 1.1.2.3  1994/06/10  22:26:16  cbrooks
 * 	CR 10897
 * 	[1994/06/10  21:11:36  cbrooks]
 * 
 * Revision 1.1.2.2  1994/05/05  15:18:55  bowe
 * 	Fix typos in epv variable names. [CR 10001]
 * 	[1994/05/05  13:33:13  bowe]
 * 
 * Revision 1.1.2.1  1994/04/26  19:11:28  bowe
 * 	Initial checkin (was part of dceaclimpl.c, now defunct).
 * 	[1994/04/26  19:08:42  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#include <stdio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

#include <aclimpl.h>
#include <dce/sec_acl_encode.h>
#include <dce/pgo.h>
#include <pthread.h>

static acl_mgt_t	*acl_mgt;
static int		amgt_count;
static pthread_mutex_t	dce_acl_s_mutex;
static pthread_once_t	dce_acl_s_once_block = pthread_once_init;
static boolean		dce_acl_s_setup;

#define LOCK	pthread_mutex_lock(&dce_acl_s_mutex);
#define UNLOCK	pthread_mutex_unlock(&dce_acl_s_mutex);
#define STAT_CHECK_RET_UNLOCK(st) \
  { if (st != error_status_ok) { UNLOCK; return; } }

/* ______________________________________________________________________ */

/*  Register an object with the ACL management package.  The library uses
 *  this information to find the actual database, printstrings, component
 *  resolution, etc.
 */

/*static pthread_initroutine_t*/
static void
dce_acl__initialize(void)
{
    if (pthread_mutex_init(&dce_acl_s_mutex, pthread_mutexattr_default) != 0) {
	/* report the error somehow */
    }
    dce_acl_s_setup = 1;
}

void
dce_acl_register_object_type(
    dce_db_handle_t		db,
    uuid_t			*mgr_type,
    unsigned32			printstring_size,
    sec_acl_printstring_t	*printstring,
    sec_acl_printstring_t	*mgr_info,
    sec_acl_permset_t		control_perm,
    sec_acl_permset_t		test_perm,
    dce_acl_resolve_func_t	resolver,
    void			*resolver_arg,
    unsigned32			flags,
    error_status_t		*st
)
{
    static int		amgt_slots;
    sec_acl_permset_t	max_perm;
    unsigned32		i;

    *st = error_status_ok;
    if (!mgr_type) {
	*st = acl_s_bad_manager_type;
	return;
    }
    if (!dce_acl_s_setup) {
	pthread_once(&dce_acl_s_once_block,
		(pthread_initroutine_t)dce_acl__initialize);
    }

    LOCK;
    if (amgt_slots == 0) {
	amgt_slots = 8;	/* arbitrary size */
	acl_mgt = (acl_mgt_t *)malloc(sizeof(acl_mgt_t) * amgt_slots);
    }
    else if (amgt_count >= amgt_slots) {
	/* If we ever allow a way to "free" a slot, we should fist search
	 * empty slots, and, if one is found, use it. */
	amgt_slots += 8;
	acl_mgt = (acl_mgt_t *)realloc(acl_mgt, sizeof(acl_mgt_t) * amgt_slots);
    }

    acl_mgt[amgt_count].mgr_type	= *mgr_type;
    acl_mgt[amgt_count].db		= db;
    acl_mgt[amgt_count].printstring_size = printstring_size;
    acl_mgt[amgt_count].printstring	= printstring;
    acl_mgt[amgt_count].mgr_info	= mgr_info;
    acl_mgt[amgt_count].resolver	= resolver;
    acl_mgt[amgt_count].resolver_arg	= resolver_arg;
    acl_mgt[amgt_count].control_perm	= control_perm;
    acl_mgt[amgt_count].test_perm	= test_perm;
    acl_mgt[amgt_count].has_owner	=
	(flags & dce_acl_c_has_owner) ? TRUE : FALSE;
    acl_mgt[amgt_count].has_groups	=
	(flags & dce_acl_c_has_groups) ? TRUE : FALSE;
    acl_mgt[amgt_count].orphans_ok	=
	(flags & dce_acl_c_orphans_ok) ? TRUE : FALSE;
    for (i=0,max_perm=dce_acl_c_no_permissions; i<printstring_size; i++) {
	max_perm |= printstring[i].permissions;
    }
    acl_mgt[amgt_count].max_perm	= max_perm;
    amgt_count++;
    UNLOCK;
}

#ifndef	HPDCE_ACLMGR_FIX_ALGORITHM
void
dce_acl_unregister_object_type(
    uuid_t		*mgr_type,
    error_status_t	*st
)
{
    int			i;

    *st = error_status_ok;
    LOCK;
    for (i=0; i<amgt_count; i++) {
	if (uuid_equal(mgr_type, &acl_mgt[i].mgr_type, st)) {
	    /* Move the last one to the slot found and decrement the count.
	     * Don't realloc down -- not worth the cost, since we will
	     * probably just add an element soon anyway. */
	    amgt_count--;
	    acl_mgt[i] = acl_mgt[amgt_count];
	    UNLOCK;
	    return;
	}
    }
    UNLOCK;
    *st = acl_s_type_not_registered;
}
#endif	/* HPDCE_ACLMGR_FIX_ALGORITHM */

/*  Given a manager type, search the list of registered types for the
 *  matching "slot" in the library's list.
 */
acl_mgt_t *
dce_acl__get_info_from_type(
    uuid_t		*mgr_type,
    error_status_t	*st
)
{
    int			i;

    *st = error_status_ok;
    for (i=0; i<amgt_count; i++) {
	if (uuid_equal(mgr_type, &acl_mgt[i].mgr_type, st)) {
	    return &acl_mgt[i];
	}
    }
    *st = acl_s_type_not_registered;
    return NULL;
}

/* ______________________________________________________________________ */
/*  Retrieve ACLs associated with the object referred to in the handle.
 *  component_name is used to further identify the entity desired.
 *  Note that there is only 1 ACL in the list of ACLs returned.
 */

/* Dummy uuid - to spoof ACL permset algorithm */
static uuid_t	dummy_id = { /* 0021fade-0a6f-1dbc-89e8-0000c09ce054 */
 0x0021fade, 0x0a6f, 0x1dbc, 0x89, 0xe8, {0x00, 0x00, 0xc0, 0x9c, 0xe0, 0x54} };

static void
dce__rdacl_lookup(
  /* [in] */
    handle_t			h,
    sec_acl_component_name_t	component_name,
    uuid_t			*mgr_type,
    sec_acl_type_t		sec_acl_type,
  /* [out] */
    sec_acl_result_t		*result
)
{
    uuid_t			auuid;
    sec_acl_t			acl;
    acl_mgt_t			*am_info;
    sec_acl_list_t		*sal;
    rpc_authz_cred_handle_t	cred_h;
    sec_acl_permset_t		net_rights;
    error_status_t		*st;


    st = &result->st;			/* for convenience */
    *st = error_status_ok;
#ifndef	HPDCE_ACLMGR_FIX_ALGORITHM
    result->tagged_union.sec_acl_list = NULL;
#endif	/* HPDCE_ACLMGR_FIX_ALGORITHM */

    if (!(sal = (sec_acl_list_t *)rpc_sm_allocate(sizeof(sec_acl_list_t), st)))
	return;
#ifdef	HPDCE_ACLMGR_FIX_ALGORITHM
    result->tagged_union.sec_acl_list = sal;
#endif	/* HPDCE_ACLMGR_FIX_ALGORITHM */

    sal->num_acls = 0;

    /* Get database handle */
    am_info = dce_acl__get_info_from_type(mgr_type, st);
    STAT_CHECK_RET(*st);

    /* get component uuid from component name */
    if (!am_info->resolver) {
	/* error -- no resolution function */
#ifndef	HPDCE_ACLMGR_FIX_ALGORITHM
	*st = sec_acl_no_acl_found;
#endif	/* HPDCE_ACLMGR_FIX_ALGORITHM */
	return;
    }
    (am_info->resolver) (h, component_name, sec_acl_type, mgr_type,
		0, am_info->resolver_arg, &auuid, st);
    STAT_CHECK_RET(*st);

    /* Read acl from database.  Key is uuid of acl. */
    dce_db_fetch_by_uuid(am_info->db, &auuid, &acl, st);
    STAT_CHECK_RET(*st);

    /* check that registered mgr type matches ACL's mgr type */
    if (!uuid_equal(&am_info->mgr_type, &acl.sec_acl_manager_type, st)) {
	*st = acl_s_bad_manager_type;
	return;
    }

#ifndef	HPDCE_ACLMGR_FIX_ALGORITHM
    if (h) {
#endif	/* HPDCE_ACLMGR_FIX_ALGORITHM */
    /* First, check that at least one ACLE bit is set. */
    dce_acl_inq_client_creds(h, &cred_h, st);
    STAT_CHECK_RET(*st);
    dce_acl_inq_permset_for_creds(cred_h, &acl, &dummy_id, &dummy_id,
	sec_acl_posix_no_semantics, &net_rights, st);
    STAT_CHECK_RET(*st);
    /* Now, check that caller has some - any - access. */
    if (net_rights == dce_acl_c_no_permissions) {
	/* No rights. Return an error. */
	*st = sec_acl_invalid_permission;
	return;
    }
#ifndef	HPDCE_ACLMGR_FIX_ALGORITHM
    }
#endif	/* HPDCE_ACLMGR_FIX_ALGORITHM */

    if ( (sal->sec_acls[0] =
	(sec_acl_t *)rpc_sm_allocate(sizeof(sec_acl_t), st)) == NULL ) {
	return;
    }
    dce_acl_copy_acl(&acl, sal->sec_acls[0], st);
    if (*st != error_status_ok) {
	rpc_sm_free(sal, st);
	return;
    }
    sal->num_acls = 1;
#ifndef	HPDCE_ACLMGR_FIX_ALGORITHM
    result->tagged_union.sec_acl_list = sal;
#endif	/* HPDCE_ACLMGR_FIX_ALGORITHM */
}

void
dce_rdacl_lookup(
  /* [in] */
    handle_t			h,
    sec_acl_component_name_t	component_name,
    uuid_t			*mgr_type,
    sec_acl_type_t		sec_acl_type,
  /* [out] */
    sec_acl_result_t		*result
)
{
    LOCK;
    dce__rdacl_lookup(h, component_name, mgr_type, sec_acl_type, result);
    UNLOCK;
}

void
dce_rdacl_lookup_v0(
  /* [in] */
    handle_t			h,
    sec_acl_component_name_t	component_name,
    uuid_t			*mgr_type,
    sec_acl_type_t		sec_acl_type,
  /* [out] */
    sec_acl_result_t		*result
)
{
    unsigned32			i, j;
    sec_acl_t			*ap;
    error_status_t		st;
    sec_acl_extend_info_t	ext_info_p;
    static sec_acl_extend_info_t ext_info;

    LOCK;
    dce__rdacl_lookup(h, component_name, mgr_type, sec_acl_type, result);
    UNLOCK;
    if (result->st != error_status_ok)
	return;

    /* Convert the delegate entries to extended ones (where apropriate). */
    sec_acl_encode_v0_entries(rpc_ss_allocate, rpc_ss_free, FALSE,
	result->tagged_union.sec_acl_list, &result->st);
}

/* ______________________________________________________________________ */
/*  Replace ACL associated with obj referred to by handle with the given one.
 *  Actually replaces ACL list - of length 1.
 */

void
dce_rdacl_replace(
  /* [in] */
    handle_t			h,
    sec_acl_component_name_t	component_name,
    uuid_t			*mgr_type,
    sec_acl_type_t		sec_acl_type,
    sec_acl_list_t		*sec_acl_list,
  /* [out] */
    error_status_t		*st
)
{
    uuid_t		auuid;
    acl_mgt_t		*am_info;
    boolean32		ok;
    sec_acl_t		*ap;
    unsigned32		ai, ei;

    *st = error_status_ok;

    /* Get database handle and resolver */
    am_info = dce_acl__get_info_from_type(mgr_type, st);
    STAT_CHECK_RET(*st);

    /* get component uuid from component name */
    if (!am_info->resolver) {
	/* error -- no resolution function */
#ifndef	HPDCE_ACLMGR_FIX_ALGORITHM
	*st = sec_acl_no_acl_found;
#endif	/* HPDCE_ACLMGR_FIX_ALGORITHM */
	return;
    }

    /* Get existing ACL uuid of thing to see if caller has permission. */
#ifndef	HPDCE_ACLMGR_FIX_ALGORITHM
    (am_info->resolver) (h, component_name, sec_acl_type, mgr_type, TRUE,
		am_info->resolver_arg, &auuid, st);
#else
    (am_info->resolver) (h, component_name, sec_acl_type, mgr_type, 0,
		am_info->resolver_arg, &auuid, st);
#endif	/* HPDCE_ACLMGR_FIX_ALGORITHM */
    STAT_CHECK_RET(*st);

    /* If this obj does NOT do owners/groups, scan the ACL lists for
     * entries of those types.  If found, report an error.
     */
    if (!am_info->has_owner) {
	for (ai=0; ai<sec_acl_list->num_acls; ai++) {	/* always 1 now */
	    ap = sec_acl_list->sec_acls[ai];
	    for (ei=0; ei<ap->num_entries; ei++) {
		if (ap->sec_acl_entries[ei].entry_info.entry_type ==
			sec_acl_e_type_user_obj ||
		    ap->sec_acl_entries[ei].entry_info.entry_type ==
			sec_acl_e_type_user_obj_deleg) {
		    *st = acl_s_owner_not_allowed;
		    return;
		}
	    }
	}
    }
    if (!am_info->has_groups) {
	for (ai=0; ai<sec_acl_list->num_acls; ai++) {	/* always 1 now */
	    ap = sec_acl_list->sec_acls[ai];
	    for (ei=0; ei<ap->num_entries; ei++) {
		if (ap->sec_acl_entries[ei].entry_info.entry_type ==
			sec_acl_e_type_group_obj ||
		    ap->sec_acl_entries[ei].entry_info.entry_type ==
			sec_acl_e_type_group_obj_deleg) {
		    *st = acl_s_group_not_allowed;
		    return;
		}
	    }
	}
    }

    LOCK;
    if (h) {
	dce_acl_is_client_authorized(h, mgr_type, &auuid, NULL, NULL,
		am_info->control_perm, &ok, st);
	if (!ok) {
	    UNLOCK;
	    *st = sec_acl_invalid_permission;
	    return;
	}
    }

    /* Store ACL in database.  Key is uuid of the ACL. */
    dce_db_store_by_uuid(am_info->db, &auuid, sec_acl_list->sec_acls[0], st);
    UNLOCK;
    STAT_CHECK_RET(*st);
}

void
dce_rdacl_replace_v0(
  /* [in] */
    handle_t			h,
    sec_acl_component_name_t	component_name,
    uuid_t			*mgr_type,
    sec_acl_type_t		sec_acl_type,
    sec_acl_list_t		*sec_acl_list,
  /* [out] */
    error_status_t		*st
)
{
    unsigned32	i;

    /* Convert the extended entries to delegate ones (where apropriate). */
    sec_acl_decode_v0_entries(rpc_ss_allocate, rpc_ss_free, FALSE,
	sec_acl_list, st);

    dce_rdacl_replace(h, component_name, mgr_type, sec_acl_type,
	sec_acl_list, st);
}

/* ______________________________________________________________________ */
/*  Read a privilege attribute certificate (PAC) and return caller's
 *  rights granted to specified object.
 */

void
dce_rdacl_get_access(
  /* [in] */
    handle_t			h,
    sec_acl_component_name_t	component_name,
    uuid_t			*mgr_type,
  /* [out] */
    sec_acl_permset_t		*net_rights,
    error_status_t		*st
)
{
    rpc_authz_cred_handle_t	cred_h;
    acl_mgt_t			*am_info;
    sec_acl_result_t		result;
    sec_cred_pa_handle_t	*cred_pa;
    sec_id_pa_t			*pa_p;

    *st = error_status_ok;
    *net_rights = dce_acl_c_no_permissions;	/* default perms - none */

    /* Get the caller's credentials */
    dce_acl_inq_client_creds(h, &cred_h, st);
    STAT_CHECK_RET(*st);

    LOCK;
    dce__rdacl_lookup(h, component_name, mgr_type, sec_acl_type_object,
	    &result);
    if (result.st != error_status_ok) {
	*st = result.st;
	UNLOCK;
	return;
    }
    if (result.tagged_union.sec_acl_list->num_acls == 0) {
	*st = acl_s_no_acl_entries;
	UNLOCK;
	return;
    }

    /* Find who the caller is - we need the prin/group in case there are
     * user_obj and group_obj ACLEs */
    cred_pa = sec_cred_get_initiator(cred_h, st);
    STAT_CHECK_RET_UNLOCK(*st);
    pa_p = sec_cred_get_pa_data(cred_pa, st);
    STAT_CHECK_RET_UNLOCK(*st);

    /* Compare ACL with PAC, determining permissions. */
    dce_acl_inq_permset_for_creds(cred_h,
	    result.tagged_union.sec_acl_list->sec_acls[0],
	    &pa_p->principal.uuid, &pa_p->group.uuid,
	    sec_acl_posix_no_semantics, net_rights, st);
    STAT_CHECK_RET_UNLOCK(*st);

    /* Get info about this mgr type */
    am_info = dce_acl__get_info_from_type(mgr_type, st);
    STAT_CHECK_RET_UNLOCK(*st);

    /* Now, check that caller has "test" access. */
    if (am_info->test_perm && !(*net_rights & am_info->test_perm)) {
	/* No test rights, blank out the rights (returned val), and
	 * return an error. */
	*net_rights = dce_acl_c_no_permissions;
	*st = sec_acl_invalid_permission;
    }
    UNLOCK;
}

/* ______________________________________________________________________ */
/*  Test access to an object, returning (true or false) whether the caller
 *  has the asked-for access rights.
 */

boolean32
dce_rdacl_test_access(
  /* [in] */
    handle_t			h,
    sec_acl_component_name_t	component_name,
    uuid_t			*mgr_type,
    sec_acl_permset_t		desired_permset,
  /* [out] */
    error_status_t		*st
)
{
    sec_acl_permset_t		net_rights;

    *st = error_status_ok;
    /* the next call does a lock */
    dce_rdacl_get_access(h, component_name, mgr_type, &net_rights, st);
    STAT_CHECK_RET_FALSE(*st);

    return ( (net_rights & desired_permset) == desired_permset );
}

/* ______________________________________________________________________ */
/*  Test access to an object on behalf of another process.  Returns true
 *  if the accecss is available to both the caller and the subject identified
 *  in the call.
 */

boolean32
dce_rdacl_test_access_on_behalf(
  /* [in] */
    handle_t			h,
    sec_acl_component_name_t	component_name,
    uuid_t			*mgr_type,
    sec_id_pac_t		*subject,
    sec_acl_permset_t		desired_permset,
  /* [out] */
    error_status_t		*st
)
{
    *st = sec_acl_not_implemented;
    return FALSE;
#if 0
    sec_acl_result_t		result;
    sec_acl_permset_t		perm;
    acl_mgt_t			*am_info;
    rpc_authz_cred_handle_t	cred_h;

    *st = error_status_ok;
    am_info = dce_acl__get_info_from_type(mgr_type, st);
    STAT_CHECK_RET_FALSE(*st);

    LOCK;

    /* Find the ACL that goes with the object */
    dce__rdacl_lookup(h, component_name, mgr_type, sec_acl_type_object, &result);
    if (result.st != error_status_ok) {
	*st = result.st;
	UNLOCK;
	return FALSE;
    }
    if (result.tagged_union.sec_acl_list->num_acls == 0) {
	*st = acl_s_no_acl_entries;
	UNLOCK;
	return FALSE;
    }

    /* Get the caller's credentials */
    dce_acl_inq_client_creds(h, &cred_h, st);
    STAT_CHECK_RET_FALSE(*st);

    /* Check authorization. */
    dce_acl_inq_permset_for_creds(cred_h,
	result.tagged_union.sec_acl_list->sec_acls[0], NULL, NULL,
	sec_acl_posix_no_semantics, &perm, st);
    STAT_CHECK_RET_FALSE(*st);
    if ( (perm & am_info->test_perm) != am_info->test_perm ) {
	*st = sec_acl_invalid_permission;
	return FALSE;
    }

    /* Compare ACL with PAC, determining permissions. */
/* XXX subject ???? -- use the cred_h for now, to make compiler happy */
    dce_acl_inq_permset_for_creds(cred_h,
	result.tagged_union.sec_acl_list->sec_acls[0], NULL, NULL,
	sec_acl_posix_no_semantics, &perm, st);
    UNLOCK;
    STAT_CHECK_RET_FALSE(*st);
    return ( (perm & desired_permset) == desired_permset );
#endif
}

/* ______________________________________________________________________ */
/*  Determine the types of ACLs protecting an object.  ACL editors/browsers
 *  use this operation to determine the sec_acl manager types that a
 *  particular reference monitor is using to protect a selected entity.
 */

void
dce_rdacl_get_manager_types(
  /* [in] */
    handle_t			h,
    sec_acl_component_name_t	component_name,
    sec_acl_type_t		sec_acl_type,
    unsigned32			size_avail,
  /* [out] */
    unsigned32			*size_used,
    unsigned32			*num_types,
    uuid_t			manager_types[],
    error_status_t		*st
)
{
    int			i;
    acl_mgt_t		*am_info;
    sec_acl_t		acl;
    uuid_t		auuid;

    *st = error_status_ok;
    *size_used = *num_types = 0;
    if (size_avail < 1) return;
    LOCK;
    /* Find the first matching manager type.  (We only support 1.) */
    for (i=0,am_info=acl_mgt; i<amgt_count; i++,am_info++) {
	
	/* Get existing ACL uuid of thing to see if caller has permission. */
	(am_info->resolver) (h, component_name, sec_acl_type,
		&am_info->mgr_type, 0, am_info->resolver_arg, &auuid, st);

	if (*st == error_status_ok) {
	    /* OK, we resolved ACL uuid.  Now be sure that mgr type registered
	     * matches the mgr type that lives with the ACL in the database.  */
	    dce_db_fetch_by_uuid(am_info->db, &auuid, &acl, st);
	    STAT_CHECK_RET_UNLOCK(*st);
	    if (uuid_equal(&am_info->mgr_type, &acl.sec_acl_manager_type, st)) {
		manager_types[0] = am_info->mgr_type;
		*num_types = *size_used = 1;
	    }
	    else
		*st = acl_s_bad_manager_type;
	    break;
	}
    }
#ifndef	HPDCE_ACLMGR_FIX_ALGORITHM
    if (i == amgt_count && *st == error_status_ok)
	*st = sec_acl_no_acl_found;
#endif	/* HPDCE_ACLMGR_FIX_ALGORITHM */
    UNLOCK;
}

/* ______________________________________________________________________ */
/*  Return printable ACL strings for a given manager type.
 */

void
dce_rdacl_get_printstring(
  /* [in] */
    handle_t			h,
    uuid_t			*mgr_type,
    unsigned32			size_avail,
  /* [out] */
    uuid_t			*manager_type_chain,
    sec_acl_printstring_t	*manager_info,
    boolean32			*tokenize,
    unsigned32			*total_num_printstrings,
    unsigned32			*size_used,
    sec_acl_printstring_t	printstrings[],
    error_status_t		*st
)
{
    acl_mgt_t	*am_info;
    int		i;

    *st = error_status_ok;
#ifndef	HPDCE_ACLMGR_FIX_ALGORITHM
    /* what to do with tokenize arg?? */
    *tokenize = FALSE;
    *total_num_printstrings = 0;
    *size_used = 0;
#endif	/* HPDCE_ACLMGR_FIX_ALGORITHM */

    /* We're not implementing chaining yet... */
    uuid_create_nil(manager_type_chain, st);

    if (size_avail < 1) {
	*total_num_printstrings = 0;
	*size_used = 0;
	return;
    }
    am_info = dce_acl__get_info_from_type(mgr_type, st);
    if (*st != error_status_ok) {
	*total_num_printstrings = 0;
	*size_used = 0;
	return;
    }
    LOCK;
    *total_num_printstrings	= am_info->printstring_size;
    manager_info[0]		= am_info->mgr_info[0];
    for (i=0; i<am_info->printstring_size && i<size_avail; i++) {
	printstrings[i] = am_info->printstring[i];
    }
    *size_used = i;
    UNLOCK;
#ifdef	HPDCE_ACLMGR_FIX_ALGORITHM
    /* what to do with tokenize arg?? */
    *tokenize = FALSE;
#endif	/* HPDCE_ACLMGR_FIX_ALGORITHM */
}

/* ______________________________________________________________________ */
/*  Gets a referral to an ACL update site.  (Used for replicated objects,
 *  where the client is bound to the manager of a copy (not the master).)
 *
 *	NOTE:  This will not be implemented.
 */

void
dce_rdacl_get_referral(
  /* [in] */
    handle_t			h,
    sec_acl_component_name_t	component_name,
    uuid_t			*mgr_type,
    sec_acl_type_t		sec_acl_type,
  /* [out] */
    sec_acl_tower_set_t		*towers,
    error_status_t		*st
)
{
    *towers = (sec_acl_tower_set_t )NULL;
    *st = sec_acl_not_implemented;
}

/* ______________________________________________________________________ */
/*  Determine the types of ACLs protecting an object.
 */

void
dce_rdacl_get_mgr_types_semantics(
  /* [in] */
    handle_t			h,
    sec_acl_component_name_t	component_name,
    sec_acl_type_t		sec_acl_type,
    unsigned32			size_avail,
  /* [out] */
    unsigned32			*size_used,
    unsigned32			*num_types,
    uuid_t			manager_types[],
    sec_acl_posix_semantics_t	posix_semantics[],
    error_status_t		*st
)
{
    *st = error_status_ok;
    *size_used = *num_types = 0;

    /* The function below is almost exactly like this one.  Let it do
     * the work, then set the posix_semantics return, if apropriate.
     */
    dce_rdacl_get_manager_types(h, component_name, sec_acl_type, size_avail,
	size_used, num_types, manager_types, st);
    if (*st == error_status_ok && *size_used == 1)
	posix_semantics[0] = sec_acl_posix_no_semantics;
}

/* ______________________________________________________________________ */
/*
    ==================================================================

    NOTE: these two will not work until dce_db_fetch_header() works.
    (Waiting to resolve some idl encoding services issues/enhancements.)
    Change these when dce_db_fetch_header() is ready.

    ==================================================================
*/

void
dce_acl_resolve_by_name(
  /* in */
    handle_t			h,
    sec_acl_component_name_t	component_name,
    sec_acl_type_t		sec_acl_type,
    uuid_t			*manager_type,
    boolean32			writing,
    void			*resolver_arg,
  /* out */
    uuid_t			*acl_uuid,
    error_status_t		*st
)
{
    dce_db_handle_t             db_h;
/*    error_status_t              st2;*/
    dce_db_header_t             dbh;

    /* Get object header using the object database.  The handle was passed
     * in as the resolver_arg in the dce_acl_register_object_type call.
     */
    db_h = (dce_db_handle_t)resolver_arg;
/*    dce_db_fetch_header(db_h, &component_name, &dbh, st);*/
    dce_db_fetch(db_h, &component_name, &dbh, st);
    STAT_CHECK_RET(*st);

    /* Get the appropriate ACL based on the ACL type. */
    dce_acl_inq_acl_from_header(dbh, sec_acl_type, acl_uuid, st);
    STAT_CHECK_RET(*st);

    /* Release the object header. */
/*    dce_db_free_header(dbh, &st2);*/

    /* If "writing" was true, we could retrieve the header of the ACL object
     * and return a newly-generated UUID if its reference count was greater
     * than one.  We assume each object has its own ACL object and don't
     * bother.
     */
}

void
dce_acl_resolve_by_uuid(
  /* in */
    handle_t			h,
    sec_acl_component_name_t	component_name,
    sec_acl_type_t		sec_acl_type,
    uuid_t			*manager_type,
    boolean32			writing,
    void			*resolver_arg,
  /* out */
    uuid_t			*acl_uuid,
    error_status_t		*st
)
{
    dce_db_handle_t             db_h;
/*    error_status_t              st2;*/
    dce_db_header_t             dbh;
    uuid_t                      obj;

    /* Get the object. */
    rpc_binding_inq_object(h, &obj, st);
    STAT_CHECK_RET(*st);

    /* Get object header using the object database.  The handle was passed
     * in as the resolver_arg in the dce_acl_register_object_type call.
     */
    db_h = (dce_db_handle_t)resolver_arg;
/*    dce_db_fetch_header(db_h, &obj, &dbh, st);*/
    dce_db_fetch(db_h, &obj, &dbh, st);
    STAT_CHECK_RET(*st);

    /* Get the appropriate ACL based on the ACL type. */
    dce_acl_inq_acl_from_header(dbh, sec_acl_type, acl_uuid, st);
    STAT_CHECK_RET(*st);

    /* Release the object header. */
/*    dce_db_free_header(dbh, &st2);*/

    /* If "writing" was true, we could retrieve the header of the ACL object
     * and return a newly-generated UUID if its reference count was greater
     * than one.  We assume each object has its own ACL object and don't
     * bother.
     */
}

/* ______________________________________________________________________ */
/*  Application servers pass this to rpc_server_register_if() */

/* global */
rdaclif_v1_0_epv_t dce_acl_v1_0_epv = {
    dce_rdacl_lookup,
    dce_rdacl_replace,
    dce_rdacl_get_access,
    dce_rdacl_test_access,
    dce_rdacl_test_access_on_behalf,
    dce_rdacl_get_manager_types,
    dce_rdacl_get_printstring,
    dce_rdacl_get_referral,
    dce_rdacl_get_mgr_types_semantics
};

rdaclif_v0_0_epv_t dce_acl_v0_0_epv = {
    dce_rdacl_lookup_v0,
    dce_rdacl_replace_v0,
    dce_rdacl_get_access,
    dce_rdacl_test_access,
    dce_rdacl_test_access_on_behalf,
    dce_rdacl_get_manager_types,
    dce_rdacl_get_printstring,
    dce_rdacl_get_referral,
    dce_rdacl_get_mgr_types_semantics
};

/* ______________________________________________________________________ */
