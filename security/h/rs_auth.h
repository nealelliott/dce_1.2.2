/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_auth.h,v $
 * Revision 1.1.9.2  1996/02/18  22:58:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:24  marty]
 *
 * Revision 1.1.9.1  1995/12/13  16:24:59  root
 * 	Submit
 * 
 * 	HP revision /main/HPDCE02/1  1995/03/27  19:31 UTC  cuti
 * 	CHFts14466 (ot12799) bug fix
 * 
 * 	HP revision /main/cuti_mothra_bug/1  1995/03/24  20:50 UTC  cuti
 * 	Change dummy_handle to unauth_handle for better naming
 * 	[1995/12/11  15:14:36  root]
 * 
 * Revision 1.1.6.4  1994/07/15  14:59:10  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:16:00  mdf]
 * 
 * Revision 1.1.6.3  1994/06/02  20:59:08  mdf
 * 	Merged with changes from 1.1.6.2
 * 	[1994/06/02  20:58:47  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:14:47  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.6.2  1994/06/02  20:21:37  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:14:47  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.4.3  1992/12/29  13:07:37  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:02  zeliff]
 * 
 * Revision 1.1.4.2  1992/08/31  18:17:21  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  14:56:56  sekhar]
 * 
 * Revision 1.1.2.2  1992/03/30  16:31:29  ahop
 * 	 support locksmith mode
 * 	[1992/03/27  23:40:47  ahop]
 * 
 * Revision 1.1  1992/01/19  14:43:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1994
** Unpublished work. All Rights Reserved.
*/

#ifndef rs_auth__included
#define rs_auth__included
 
#include <dce/rpc.h>
#include <dce/rgynbase.h>
#include <dce/id_base.h>
#include <dce/aclbase.h>
#include <dce/id_epac.h>
#include <rsdb_pvt.h>

                                      
PUBLIC void rs_auth_init_unauth_cred_handle(

    error_status_t  *stp
);

PUBLIC void rs_auth_init (
    error_status_t  *st
);

PUBLIC boolean32 rs_is_authorized_by_epac_data (
    sec_id_epac_data_t      *epac_data,
    sec_acl_permset_t       requested_perms,
    sec_acl_permset_t       parent_perms,
    sec_rgy_name_t          object_name,
    uuid_t                  *mgr_type,
    sec_rgy_foreign_id_t    *client_ids,
    error_status_t          *stp
);

PRIVATE void epac_data_to_cred_handle(
    sec_id_epac_data_t      *epac_data,
    boolean32               authenticated,
    rpc_authz_cred_handle_t *cred_h,
    error_status_t          *stp
);

PUBLIC boolean32 rs_is_authorized_by_creds_no_locksmith (
    rpc_authz_cred_handle_t cred_h,
    sec_acl_permset_t       requested_perms,
    sec_acl_permset_t       parent_perms,
    sec_rgy_name_t          object_name,
    uuid_t                  *mgr_type,
    sec_rgy_foreign_id_t    *client_ids,
    error_status_t          *stp
);


PUBLIC boolean32 rs_is_authorized_by_creds (
    handle_t                h,
    rpc_authz_cred_handle_t cred_h,
    sec_acl_permset_t       requested_perms,
    sec_acl_permset_t       parent_perms,
    sec_rgy_name_t          object_name,
    uuid_t                  *mgr_type,
    boolean32               check_quota,
    sec_rgy_foreign_id_t    *client_ids,
    error_status_t          *st
);

                                      
PUBLIC boolean32 rs_is_authorized(
    handle_t                h,
    sec_acl_permset_t       requested_perms,
    sec_acl_permset_t       parent_perms,
    sec_rgy_name_t          object_name,
    uuid_t                  *mgr_type,
    boolean32               check_quota,
    sec_rgy_foreign_id_t    *client_ids,
    error_status_t          *st
);
                                   

PUBLIC boolean32 rs_auth_is_authorized(
    handle_t                h,
    sec_acl_permset_t       requested_perms,
    sec_acl_permset_t       parent_perms,
    sec_rgy_domain_t        domain,
    sec_rgy_name_t          object_name,
    uuid_t                  *mgr_type,
    boolean32               check_quota,
    sec_rgy_foreign_id_t    *client_ids,
    error_status_t          *st
);

PUBLIC boolean32 rs_auth_is_auth_by_name_ok (
    handle_t                h,
    sec_acl_permset_t       requested_perms,
    sec_acl_permset_t       parent_perms,
    sec_rgy_domain_t        domain,
    sec_rgy_name_t          object_name,
    uuid_t                  *mgr_type,
    boolean32               check_quota,
    sec_rgy_foreign_id_t    *client_ids,
    error_status_t          *st
);

PUBLIC boolean32 rs_auth_get_creds_from_handle (
    handle_t                h,
    rpc_authz_cred_handle_t *cred_h,
    error_status_t          *st
);
 
PRIVATE boolean32 rs_auth_get_creds_from_hdl_name_ok (
    handle_t                h,
    boolean32               name_ok,
    rpc_authz_cred_handle_t *cred_h,
    boolean32               *free_cred_h,
    error_status_t          *st
);
                                    
PUBLIC rpc_auth_key_retrieval_fn_t rs_get_auth_key (
    void                    *arg,
    unsigned_char_p_t       svr_princ_name,
    unsigned32              key_type,
    unsigned32              key_version,
    void                    **key,
    unsigned32              *st
);


PUBLIC void rs_auth_pgo_update_check (
    sec_rgy_pgo_item_t      *old_pgo,
    sec_rgy_pgo_item_t      *new_pgo,
    boolean32               is_cell,
    sec_acl_permset_t       *required_perms,
    error_status_t          *st
);

PUBLIC void rs_auth_acct_admin_update_check  (
    sec_rgy_acct_admin_t    *old_admin_part,
    sec_rgy_acct_admin_t    *new_admin_part,
    sec_acl_permset_t       *required_perms,
    error_status_t          *st
);

PUBLIC void rs_auth_setup_locksmith (
    rsdb_locksmith_info_t   *locksmith_info_p,
    unsigned_char_p_t       locksmith_name_p,
    boolean32               prompt_for_locksmith_key,
    error_status_t          *st
);


#endif
