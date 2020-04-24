/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: aclif.h,v $
 * Revision 1.1.4.2  1996/02/18  23:32:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:24  marty]
 *
 * Revision 1.1.4.1  1995/12/08  21:35:17  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/29  20:40 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/tatsu_s.dced_local.b0/1  1995/09/19  13:09 UTC  tatsu_s
 * 	Added dce_acl_unregister_object_type().
 * 	[1995/12/08  18:07:11  root]
 * 
 * Revision 1.1.2.5  1994/07/15  17:27:05  bowe
 * 	Remove dce_acl_g_helpstring. [CR 11308]
 * 	[1994/07/15  17:26:21  bowe]
 * 
 * Revision 1.1.2.4  1994/07/13  22:09:52  bowe
 * 	Declare rdacl EPVs.  [CR 10678]
 * 	Include size of dce_acl_g_printstring. [CR 11284]
 * 	[1994/07/13  22:09:34  bowe]
 * 
 * Revision 1.1.2.3  1994/05/05  15:18:54  bowe
 * 	Added prototypes for dce_acl_obj_init() and
 * 	dce_acl_obj_free_entries() [CR 10001]
 * 	[1994/05/05  13:29:28  bowe]
 * 
 * Revision 1.1.2.2  1994/04/26  19:11:27  bowe
 * 	Adjust prototypes to reflect new reality.
 * 	Added new globals: dce_acl_g_printstring, dce_acl_g_helpstring.
 * 	#define flags for dce_acl_register_object_type().
 * 	[1994/04/26  17:45:45  bowe]
 * 
 * Revision 1.1.2.1  1994/01/24  22:01:33  bowe
 * 	Initial checkin
 * 	[1994/01/24  21:59:24  bowe]
 * 
 * Revision 1.1.2.1  1993/12/20  13:43:54  bowe
 * 	Added another param (for mgr_type) to dce_acl_is_client_authorized()
 * 	and dce_acl_inq_client_permset().
 * 	[1993/12/17  22:05:50  bowe]
 * 
 * 	Initial revision.
 * 	[1993/12/16  20:40:05  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________
 *
 *  ACL management library routines.
 */

#include <dce/aclbase.h>
#include <dce/id_epac.h>
#include <dce/sec_cred.h>
#include <dce/dbif.h>
#include <dce/dcelibmsg.h>
#include <dce/dceacl.h>
#include <dce/rdaclifv0.h>
#include <dce/rdaclif.h>

/* constants */
#define dce_acl_c_no_permissions	((sec_acl_permset_t)0)
/* flags for dce_acl_register_object_type() */
#define dce_acl_c_orphans_ok	0x01
#define dce_acl_c_has_owner	0x02
#define dce_acl_c_has_groups	0x04

/* type definitions */
typedef void (*dce_acl_resolve_func_t)(
    /* [in] parameters */
	handle_t, sec_acl_component_name_t, sec_acl_type_t,
	uuid_t *, boolean32, void *,
    /* [out] parameters */
	uuid_t *, error_status_t *
);

/*
 * Global definition for common printstring.
 * The size must match what is defined in aclconv.c.
 */
extern sec_acl_printstring_t	dce_acl_g_printstring[7];

extern rdaclif_v0_0_epv_t dce_acl_v0_0_epv;
extern rdaclif_v1_0_epv_t dce_acl_v1_0_epv;

/* ACL lib function prototypes */
/*
dce_acl_resolve_func_t dce_acl_resolve_by_uuid();
dce_acl_resolve_func_t dce_acl_resolve_by_name();
*/
void dce_acl_resolve_by_uuid(handle_t, sec_acl_component_name_t, sec_acl_type_t,
	uuid_t *, boolean32, void *, uuid_t *, error_status_t *);
void dce_acl_resolve_by_name(handle_t, sec_acl_component_name_t, sec_acl_type_t,
	uuid_t *, boolean32, void *, uuid_t *, error_status_t *);

/* called by server once for each managed obj type */
void dce_acl_register_object_type(dce_db_handle_t, uuid_t *, unsigned32,
	sec_acl_printstring_t *, sec_acl_printstring_t *,
	sec_acl_permset_t, sec_acl_permset_t, dce_acl_resolve_func_t, void *,
	unsigned32, error_status_t *);
#ifndef	HPDCE_ACLMGR_FIX_ALGORITHM
void dce_acl_unregister_object_type(uuid_t *, error_status_t *);
#endif	/* HPDCE_ACLMGR_FIX_ALGORITHM */

void dce_acl_inq_acl_from_header(dce_db_header_t, sec_acl_type_t, uuid_t *,
	error_status_t *);

/* for creating and adding to an ACL */
void dce_acl_obj_init(uuid_t *, sec_acl_t *, error_status_t *);
void dce_acl_obj_free_entries(sec_acl_t *, error_status_t*);
void dce_acl_obj_add_user_entry(sec_acl_t *, sec_acl_permset_t, uuid_t *,
	error_status_t *);
void dce_acl_obj_add_group_entry(sec_acl_t *, sec_acl_permset_t, uuid_t *,
	error_status_t *);
void dce_acl_obj_add_id_entry(sec_acl_t *, sec_acl_entry_type_t,
	sec_acl_permset_t, uuid_t *, error_status_t *);
void dce_acl_obj_add_unauth_entry(sec_acl_t *, sec_acl_permset_t,
	error_status_t *);
void dce_acl_obj_add_any_other_entry(sec_acl_t *, sec_acl_permset_t,
	error_status_t *);
void dce_acl_obj_add_obj_entry(sec_acl_t *, sec_acl_entry_type_t,
	sec_acl_permset_t, error_status_t *);
void dce_acl_obj_add_foreign_entry(sec_acl_t *, sec_acl_entry_type_t,
	sec_acl_permset_t, uuid_t *, uuid_t *, error_status_t *);
void dce_acl_copy_acl(sec_acl_t *, sec_acl_t *, error_status_t *);

/* various convenience routines */

void dce_acl_is_client_authorized(handle_t, uuid_t *, uuid_t *,
	uuid_t *, uuid_t *, sec_acl_permset_t, boolean32 *, error_status_t *);
void dce_acl_inq_client_permset(handle_t, uuid_t *, uuid_t *,
	uuid_t *, uuid_t *, sec_acl_permset_t *, error_status_t *);
void dce_acl_inq_client_creds(handle_t, rpc_authz_cred_handle_t *,
	error_status_t *);
void dce_acl_inq_permset_for_creds(rpc_authz_cred_handle_t, sec_acl_t *,
	uuid_t *, uuid_t *, sec_acl_posix_semantics_t,
	sec_acl_permset_t *, error_status_t *);
boolean32 dce_acl_is_unauthenticated(rpc_authz_cred_handle_t);
void dce_acl_inq_prin_and_group(handle_t, uuid_t *, uuid_t *, error_status_t *);

