/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * Queue manager ACL stuff
 *______________________________________________________________________
 *
 * $Log: acl.c,v $
 * Revision 1.4  1996/10/15  16:25:07  bowe
 * reverse sense of status check in acl_for_new_queue() [was wrong].
 *
 * Revision 1.3  1996/10/14  18:00:17  bowe
 * Rework function acl_for_new_queue().
 *
 * Revision 1.2  1996/09/30  20:32:58  bowe
 * In find_initiator(), use *pointer to* status.
 *
 * Revision 1.1  1996/09/27  18:06:07  bowe
 * Initial revision
 *
 * $EndLog$
 * ______________________________________________________________________
 */

#include "mosdefs.h"

#include <dce/pgo.h>
#include <dce/aclif.h>
#include <dce/secidmap.h>
#include <dce/sec_cred.h>

#include "dcemosmsg.h"
#include "dcemosmac.h"
#include "dcemossvc.h"

dce_db_handle_t	acl_db;
dce_db_handle_t	name_db;

extern boolean	skip_authz;		/* XXX - DEBUG */

/* ______________________________________________________________________ */

/* Management object */
uuid_t mgmt_type = { /* d1afe464-b4d7-11cf-aedd-0800092474cc */
  0xd1afe464, 0xb4d7, 0x11cf, 0xae, 0xdd, { 0x08, 0x00, 0x09, 0x24, 0x74, 0xcc }
};
uuid_t mgmt_acl = { /* d1b4dfa0-b4d7-11cf-aedd-0800092474cc */
  0xd1b4dfa0, 0xb4d7, 0x11cf, 0xae, 0xdd, { 0x08, 0x00, 0x09, 0x24, 0x74, 0xcc }
};

/* Initial object */
uuid_t q_iobj_type = { /* 6c29b1cc-b4ca-11cf-b07d-0800092474cc */
  0x6c29b1cc, 0xb4ca, 0x11cf, 0xb0, 0x7d, { 0x08, 0x00, 0x09, 0x24, 0x74, 0xcc }
};
uuid_t q_iobj_acl = { /* 6c29ecb4-b4ca-11cf-b07d-0800092474cc */
  0x6c29ecb4, 0xb4ca, 0x11cf, 0xb0, 0x7d, { 0x08, 0x00, 0x09, 0x24, 0x74, 0xcc }
};

/* queue object */
uuid_t q_obj_type = { /* 6c285b6a-b4ca-11cf-b07d-0800092474cc */
  0x6c285b6a, 0xb4ca, 0x11cf, 0xb0, 0x7d, { 0x08, 0x00, 0x09, 0x24, 0x74, 0xcc }
};

/* ______________________________________________________________________ */

static sec_acl_printstring_t acl_mgr_name = {
    {"mos"},           {"Message-Oriented Services"}
};

/* per queue */
static sec_acl_printstring_t acl_defs[] = {
    { {"D"},	{"delete"},		mos_perm_delete },
    { {"M"},	{"manage"},		mos_perm_manage },
    { {"R"},	{"readqattr"},		mos_perm_rdqattr },

    { {"e"},	{"enqueue"},		mos_perm_enqueue },
    { {"d"},	{"dequeue"},		mos_perm_dequeue },
    { {"r"},	{"readmattr"},		mos_perm_rdmattr },
    { {"p"},	{"purge"},		mos_perm_purge },

    { {"c"},	{"control"},		mos_perm_control },
    { {"t"},	{"test"},		mos_perm_test }
};

static sec_acl_printstring_t mgmtacl_mgr_name = {
    {"mos"},           {"Messaging-Oriented Services Management"}
};

static sec_acl_printstring_t mgmtacl_defs[] = {
    { {"i"},	{"insert"},		mos_perm_create },
    { {"m"},	{"manage"},		mos_perm_manage },
    { {"r"},	{"readattr"},		mos_perm_rdattr },
    { {"c"},	{"control"},		mos_perm_control },
    { {"t"},	{"test"},		mos_perm_test }
};

#define all_queue_perms	\
      ( mos_perm_delete  | mos_perm_manage  | mos_perm_rdqattr | \
	mos_perm_enqueue | mos_perm_dequeue | mos_perm_rdmattr | \
	mos_perm_purge   | mos_perm_control | mos_perm_test )
#define unauth_queue_perms	\
      ( mos_perm_rdqattr | mos_perm_rdmattr )
#define all_mgmt_perms	\
      ( mos_perm_create  | mos_perm_manage | mos_perm_rdattr | \
	mos_perm_control | mos_perm_test )
#define unauth_mgmt_perms	mos_perm_rdattr

/* ______________________________________________________________________ */

static void
mos_resolve_object(
    handle_t			h,
    sec_acl_component_name_t	name,
    sec_acl_type_t		sec_acl_type,
    uuid_t			*mgr_type,
    boolean32			writing,
    void			*resolver_arg,
    uuid_t			*acl_uuid,
    error_status_t		*st

)
{
/*
fprintf(stderr, "mos_resolve_object [%s] %d\n", name, sec_acl_type);
Print_uuid("mgr_type: ", mgr_type);
*/

    if (sec_acl_type != sec_acl_type_object) {
	*st = sec_acl_no_acl_found;
	return;
    }

    /* Management ACL? */
    if (name == NULL || name[0] == '\0') {
	*acl_uuid = mgmt_acl;
	*st = error_status_ok;
	return;
    }

    /* find queue with matching UUID */
    if (uuid_equal(mgr_type, &q_obj_type, st)) {
	Queue *q = find_queue_by_name(name, st);
	if (q) {
	    *acl_uuid = q->qattr.acl;
	    *st = error_status_ok;
	    return;
	}
    }
    *st = sec_acl_no_acl_found;
}

static void
mos_resolve_container(
    handle_t			h,
    sec_acl_component_name_t	name,
    sec_acl_type_t		sec_acl_type,
    uuid_t			*mgr_type,
    boolean32			writing,
    void			*resolver_arg,
    uuid_t			*acl_uuid,
    error_status_t		*st

)
{
/*
fprintf(stderr, "mos_resolve_container [%s] %d\n", name, sec_acl_type);
Print_uuid("mgr_type: ", mgr_type);
*/
    *st = error_status_ok;

    if (sec_acl_type == sec_acl_type_default_container) {
	*st = sec_acl_no_acl_found;
	return;
    }

    if (name == NULL || *name == '\0') {
	if (sec_acl_type == sec_acl_type_object &&
		uuid_equal(mgr_type, &mgmt_type, st)) {
	    *acl_uuid = mgmt_acl;
	    return;
	}
	if (sec_acl_type == sec_acl_type_default_object &&
		uuid_equal(mgr_type, &q_iobj_type, st)) {
	    *acl_uuid = q_iobj_acl;
	    return;
	}
    }

    *st = sec_acl_no_acl_found;
}

/* ______________________________________________________________________ */

void
mos_register_acls( void )
{
    error_status_t	st;

    /* management */
    dce_acl_register_object_type(acl_db, &mgmt_type,
	sizeof mgmtacl_defs / sizeof mgmtacl_defs[0],
	mgmtacl_defs, &mgmtacl_mgr_name, mos_perm_control, 0,
	mos_resolve_container, NULL, 0, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"dce_acl_register_object_type (mgmt)", (long)st);

    /* initial object */
    /* XXX - ACL lib bug screws up user_obj handling.
     * Use dce_acl_c_has_owner when fixed.
     */
    dce_acl_register_object_type(acl_db, &q_iobj_type,
	sizeof acl_defs / sizeof acl_defs[0],
	acl_defs, &acl_mgr_name, mos_perm_control, 0,
	mos_resolve_container, NULL, /*dce_acl_c_has_owner*/ 0, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"dce_acl_register_object_type (io)", (long)st);

    /* queue objects */
    dce_acl_register_object_type(acl_db, &q_obj_type,
	sizeof acl_defs / sizeof acl_defs[0],
	acl_defs, &acl_mgr_name, mos_perm_control, 0,
	mos_resolve_object, NULL, 0, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"dce_acl_register_object_type (obj)", (long)st);
}

/* ______________________________________________________________________ */

/* Determine if caller has authorization, based on given perms.
 * (Identity is gotten from handle.)
 */

boolean
mos_authorized(
  /* in */
    handle_t		h,
    uuid_t		*aclmgr,
    uuid_t		*acl,
    sec_acl_permset_t	perms,
  /* out */
    error_status_t	*st
)
{
    uuid_t	prin, grp;
    boolean32	ok;

    *st = error_status_ok;
    if (h == NULL)		/* for 'internal' operations */
	return TRUE;

#ifdef ACL_DEBUG
fprintf(stderr, "======== mos_authorized = %x\n", perms);
Print_uuid("    ==== mgr_type: ", aclmgr);
Print_uuid("    ==== acl uuid: ", acl);
if (skip_authz) {printf("\n"); return TRUE;}
#endif

    dce_acl_inq_prin_and_group(h, &prin, &grp, st);
    if (*st != error_status_ok)
	return FALSE;
    dce_acl_is_client_authorized(h, aclmgr, acl, &prin, &grp, perms, &ok, st);
    if (!ok) {
#ifdef ACL_DEBUG
fprintf(stderr, "\tFAIL\n");
#endif
	*st = sec_acl_invalid_permission;
	return FALSE;
    }
#ifdef ACL_DEBUG
fprintf(stderr, "\tOK\n");
#endif
    return TRUE;
}

/* ______________________________________________________________________ */

/* Initialize ACL database.
 */

static void
write_acl(
    uuid_t		*acl_uuid,
    uuid_t		*mgr_type,
    sec_acl_permset_t	all_perms,
    sec_acl_permset_t	other_perms,
    boolean		want_user_obj,
    error_status_t	*st
)
{
    static sec_acl_t	nil_acl;
    sec_acl_t		acl;
    sec_acl_entry_t	entries[4];

    entries[0].entry_info.entry_type = sec_acl_e_type_user;
    entries[0].entry_info.tagged_union.id.uuid = qmgr_prin_uuid;
    entries[0].entry_info.tagged_union.id.name = NULL;
    entries[0].perms = all_perms;
    entries[1].entry_info.entry_type = sec_acl_e_type_any_other;
    entries[1].perms = other_perms;
    entries[2].entry_info.entry_type = sec_acl_e_type_unauthenticated;
    entries[2].entry_info.entry_type = sec_acl_e_type_unauthenticated;
    entries[2].perms = other_perms;

#if 0
    if (want_user_obj) {
	entries[3].entry_info.entry_type = sec_acl_e_type_user_obj;
	entries[3].perms = all_perms;
    }
#endif

    acl = nil_acl;
    acl.sec_acl_manager_type = *mgr_type;
    acl.default_realm.uuid = qmgr_cell_uuid;
    /* XXX - there's a bug in the ACL library that screws up use of user_obj */
#if 0
    acl.num_entries = want_user_obj ? 4 : 3;
#else
    acl.num_entries = 3;
#endif
    acl.sec_acl_entries = entries;
    dce_db_store_by_uuid(acl_db, acl_uuid, &acl, st);
    if (*st != error_status_ok)
	dce_svc_printf(MOS_S_CANT_STORE_ACL_MSG, "write_acl", (long)*st);

}

/* Write default permissions to ACL database.  We want:
 *	Management obj: qmgr prin gets all, unauthen gets read
 *	Initial obj: qmgr prin gets all, unauthen gets read qattr, user_obj all
 */
void
initialize_acls(void)
{
    error_status_t	st;

    if (uuid_is_nil(&qmgr_prin_uuid, &st))
	dce_svc_printf(MOS_S_NO_SERVER_PRIN_MSG);

    /* XXX - Not sure what's best for unauth perms.  Give all for now. */
    write_acl(&mgmt_acl, &mgmt_type, all_mgmt_perms,
	/*unauth_mgmt_perms*/ all_mgmt_perms, FALSE, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"initialize_acls/write_acl (mgmt)", (long)st);

    write_acl(&q_iobj_acl, &q_iobj_type, all_queue_perms, unauth_queue_perms,
	TRUE, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		"initialize_acls/write_acl (io)", (long)st);
}

/* ______________________________________________________________________ */
/* Get a new ACL for the new queue -- copy the io ACL, set UUID, set the
 * type to "obj", change user_obj to user, then write it to the ACL databse.
 */

void
acl_for_new_queue(
    handle_t		h,
    uuid_t		*aclid,
    uuid_t		*user,
    error_status_t	*st
)
{
    sec_acl_t		acl, newacl;
    sec_acl_entry_t	*ep;
    int			i;
    rpc_authz_cred_handle_t	cred_h;

    dce_db_fetch_by_uuid(acl_db, &q_iobj_acl, &acl, st);
    if (*st != error_status_ok)
	dce_svc_printf(MOS_S_CANT_FETCH_ACL_MSG, "acl_for_new_queue",(long)*st);
    acl.sec_acl_manager_type = q_obj_type;

    dce_acl_copy_acl(&acl, &newacl, st);
    if (*st != error_status_ok) {
	dce_svc_printf(MOS_S_MISC_ACL_ERROR_MSG, "dce_acl_copy_acl", (long)*st);
	return;
    }

    /* Check for unauthenticated */
    rpc_binding_inq_auth_caller(h, &cred_h, NULL, NULL, NULL, NULL, st);
    if (*st == rpc_s_binding_has_no_auth) {
	boolean has_any_other=FALSE, has_unauth=FALSE;

	/* If any_other/unauthen entries are there, use them. */
	for (i=newacl.num_entries, ep=newacl.sec_acl_entries; --i>=0; ep++) {
	    /* If any_other/unauthen are already in the ACL, adjust perms */
	    if (ep->entry_info.entry_type == sec_acl_e_type_unauthenticated)
		has_unauth = TRUE;
	    else if (ep->entry_info.entry_type == sec_acl_e_type_any_other)
		has_any_other = TRUE;
	}
	if (!has_unauth) {
	    dce_acl_obj_add_unauth_entry(&newacl, all_queue_perms, st);
	    if (*st != error_status_ok) {
		dce_svc_printf(MOS_S_MISC_ACL_ERROR_MSG,
			"dce_acl_obj_add_unauth_entry", (long)*st);
		return;
	    }
	}
	if (!has_any_other) {
	    dce_acl_obj_add_any_other_entry(&newacl, all_queue_perms, st);
	    if (*st != error_status_ok) {
		dce_svc_printf(MOS_S_MISC_ACL_ERROR_MSG,
			"dce_acl_obj_add_any_other_entry", (long)*st);
		return;
	    }
	}
    }
    else if (*st != error_status_ok) {
	dce_svc_printf(MOS_S_MISC_ACL_ERROR_MSG,
		"rpc_binding_inq_auth_caller", (long)*st);
    }
    else {

/* XXX - there's a bug in the ACL library with user_obj handling.
 * For now, give caller full rights if he's not in the iobj acl.
 */
#if 0
    /* Now, if there's a user_obj entry, turn it into a user entry,
     * putting the caller's prin in there.
     */
	/* XXX - we should check whether the user already has an ACL.
	 * If so, use it, removing the user_obj one.  */

	for (i=newacl.num_entries, ep=newacl.sec_acl_entries; --i>=0; ep++) {
	    if (ep->entry_info.entry_type == sec_acl_e_type_user_obj) {
		ep->entry_info.entry_type = sec_acl_e_type_user;
		ep->entry_info.tagged_union.id.uuid = *user;
		ep->entry_info.tagged_union.id.name = NULL;
		break;
	    }
	}
#else
	boolean has_user=FALSE;
	/* If there's a user entry, leave it. */
	for (i=newacl.num_entries, ep=newacl.sec_acl_entries; --i>=0; ep++) {
	    if (ep->entry_info.entry_type == sec_acl_e_type_user &&
		uuid_equal(&ep->entry_info.tagged_union.id.uuid, user, st)) {
		has_user = TRUE;
		break;
	    }
	}
	if (!has_user) {
	    dce_acl_obj_add_user_entry(&newacl, all_queue_perms, user, st);
	    if (*st != error_status_ok) {
		dce_svc_printf(MOS_S_MISC_ACL_ERROR_MSG,
			"dce_acl_obj_add_user_entry", (long)*st);
		return;
	    }
	}
#endif
    }

    dce_db_store_by_uuid(acl_db, aclid, &newacl, st);
    if (*st != error_status_ok)
	dce_svc_printf(MOS_S_CANT_STORE_ACL_MSG, "acl_for_new_queue",(long)*st);

    dce_acl_obj_free_entries(&newacl, st);
    if (*st != error_status_ok)
	dce_svc_printf(MOS_S_MISC_ACL_ERROR_MSG, "dce_acl_obj_free_entries",
		(long)*st);
}

/* ______________________________________________________________________ */
/* Find PAC of caller. */

void
find_initiator(
    handle_t		h,
    sec_id_pa_t		*pa,
    error_status_t	*st
)
{
    rpc_authz_cred_handle_t	cred_h;
    sec_cred_pa_handle_t	*cred_pa;
    sec_id_pa_t			*pa_p;
    unsigned32			authz_svc;

    *st = error_status_ok;

    /* Get the caller's credentials */
    rpc_binding_inq_auth_caller(h, &cred_h, NULL, NULL, NULL, &authz_svc, st);
    if (*st != error_status_ok) {
	dce_svc_printf(MOS_S_SEC_FUNCTION_FAIL_MSG,
		"rpc_binding_inq_auth_caller", (long)*st);
    }

    cred_pa = sec_cred_get_initiator(cred_h, st);
    if (*st != error_status_ok) {
	dce_svc_printf(MOS_S_SEC_FUNCTION_FAIL_MSG,
		"sec_cred_get_initiator", (long)*st);
    }
    pa_p = sec_cred_get_pa_data(cred_pa, st);
    if (*st != error_status_ok) {
	dce_svc_printf(MOS_S_SEC_FUNCTION_FAIL_MSG,
		"sec_cred_get_pa_data", (long)*st);
    }
    *pa = *pa_p;
}

/* ______________________________________________________________________ */

