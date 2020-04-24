/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_pgo.c,v $
 * Revision 1.1.96.3  1996/11/13  18:10:48  arvind
 * 	Handle new status parameter to rsdb_name_util_localize_pgo().
 * 	Add back calls to rsdb_canonicalize_and_convert().
 * 	[1996/10/18  16:55 UTC  jrr  /main/DCE_1.2.2/jrr_122_6/1]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/10/05  16:59 UTC  aha  /main/HPDCE02/5]
 *
 * 	CHFts16436: collapse // to / in pgo names
 * 	HP 	[1995/10/02  20:24 UTC  aha  /main/HPDCE02/aha_mothra10/1]
 *
 * 	CHFts16436: call rsdb_name_canonicalize_components() for non-principal names
 * 	HP 	[1995/07/20  18:39 UTC  aha  /main/HPDCE02/4]
 *
 * 	CHFts15254: prevent addition of ambiguous child cell or principal
 * 	directory names (OT12680)
 * 	HP 	[1995/07/20  13:23 UTC  aha  /main/HPDCE02/aha_mothra8/1]
 *
 * 	CHFts15254: prevent ambiguous child-cell and principal names
 * 	HP 	[1995/07/17  20:32 UTC  aha  /main/HPDCE02/aha_mothra7/6]
 *
 * 	CHFts15254/OT12680: prevent addition of ambiguous principals;
 * 	merge out from mainline
 * 	HP 	[1995/07/17  19:26 UTC  aha  /main/HPDCE02/aha_mothra7/5]
 *
 * 	CHFts15254: prevent ambiguity between child cell and principal directories;
 * 	improve readability based on code review
 * 	HP 	[1995/07/14  20:35 UTC  aha  /main/HPDCE02/aha_mothra7/4]
 *
 * 	CHFts15254: disallow new principal directories that
 * 	conflict with cell names; disallow new cell names
 * 	that conflict with existing principal directories.
 * 	HP 	[1995/07/06  19:46 UTC  aha  /main/HPDCE02/aha_mothra7/3]
 *
 * 	CHFts14236: disallow deletion of primary cell name;
 * 	adjust realm name to match its usage in primary cell principal name
 * 	HP 	[1995/07/06  15:31 UTC  aha  /main/HPDCE02/aha_mothra7/2]
 *
 * 	CHFts14236: disallow deletion of primary cell name;
 * 	check return from malloc; remove unnecessary assignment to cell_name
 * 	HP 	[1995/07/05  20:46 UTC  aha  /main/HPDCE02/aha_mothra7/1]
 *
 * 	CHFts14236: disallow deletion of primary cell name
 * 	HP 	[1995/07/13  21:31 UTC  burati  /main/HPDCE02/3]
 *
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	HP 	[1995/07/13  20:47 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 *
 * 	Alias work
 * 	HP 	[1995/07/07  19:26 UTC  aha  /main/HPDCE02/2]
 *
 * 	CHFts14236: preserve cell's original primary name
 * 	HP 	[1995/05/17  23:55 UTC  rps  /main/HPDCE02/1]
 * 	[1996/11/13  17:51:20  arvind]
 *
 * Revision 1.1.96.2  1996/07/08  18:38:26  arvind
 * 	Call rsdb_name_util_localize_name() instead of
 * 	rsdb_name_canonicalize_and_convert() after calling
 * 	rsdb_name_util_is_foreign_pgo().
 * 	[1996/06/14  06:10 UTC  jrr  /main/DCE_1.2.2/jrr_122_1/1]
 * 
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:47 UTC  hanfei  /main/DCE_1.2/2]
 * 
 * 	add software version check ( migration control ) for activating member operation
 * 	for DCE 1.2.2.
 * 	[1996/03/15  15:33 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/4]
 * 
 * 	add uuids as args to rs_log_pgo_add_member()
 * 	[1996/02/29  19:00 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/3]
 * 
 * 	restructuring in rs_pgo_add_member()
 * 	[1996/02/21  15:41 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/2]
 * 
 * 	multi-cell group work: add codes for handling global group as member.
 * 	[1996/02/06  20:36 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 * 
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:22  marty]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/10/05  16:59 UTC  aha  /main/HPDCE02/5]
 * 
 * 	CHFts16436: collapse // to / in pgo names
 * 	HP 	[1995/10/02  20:24 UTC  aha  /main/HPDCE02/aha_mothra10/1]
 * 
 * 	CHFts16436: call rsdb_name_canonicalize_components() for non-principal names
 * 	HP 	[1995/07/20  18:39 UTC  aha  /main/HPDCE02/4]
 * 
 * 	CHFts15254: prevent addition of ambiguous child cell or principal
 * 	directory names (OT12680)
 * 	HP 	[1995/07/20  13:23 UTC  aha  /main/HPDCE02/aha_mothra8/1]
 * 
 * 	CHFts15254: prevent ambiguous child-cell and principal names
 * 	HP 	[1995/07/17  20:32 UTC  aha  /main/HPDCE02/aha_mothra7/6]
 * 
 * 	CHFts15254/OT12680: prevent addition of ambiguous principals;
 * 	merge out from mainline
 * 	HP 	[1995/07/17  19:26 UTC  aha  /main/HPDCE02/aha_mothra7/5]
 * 
 * 	CHFts15254: prevent ambiguity between child cell and principal directories;
 * 	improve readability based on code review
 * 	HP 	[1995/07/14  20:35 UTC  aha  /main/HPDCE02/aha_mothra7/4]
 * 
 * 	CHFts15254: disallow new principal directories that
 * 	conflict with cell names; disallow new cell names
 * 	that conflict with existing principal directories.
 * 	HP 	[1995/07/06  19:46 UTC  aha  /main/HPDCE02/aha_mothra7/3]
 * 
 * 	CHFts14236: disallow deletion of primary cell name;
 * 	adjust realm name to match its usage in primary cell principal name
 * 	HP 	[1995/07/06  15:31 UTC  aha  /main/HPDCE02/aha_mothra7/2]
 * 
 * 	CHFts14236: disallow deletion of primary cell name;
 * 	check return from malloc; remove unnecessary assignment to cell_name
 * 	HP 	[1995/07/05  20:46 UTC  aha  /main/HPDCE02/aha_mothra7/1]
 * 
 * 	CHFts14236: disallow deletion of primary cell name
 * 	HP 	[1995/07/13  21:31 UTC  burati  /main/HPDCE02/3]
 * 
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	HP 	[1995/07/13  20:47 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 * 
 * 	Alias work
 * 	HP 	[1995/07/07  19:26 UTC  aha  /main/HPDCE02/2]
 * 
 * 	CHFts14236: preserve cell's original primary name
 * 	HP 	[1995/05/17  23:55 UTC  rps  /main/HPDCE02/1]
 * 	[1996/07/08  18:10:29  arvind]
 * 
 * Revision 1.1.96.1  1996/05/10  13:22:17  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:47 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/4  1996/03/15  15:33 UTC  hanfei
 * 	add software version check ( migration control ) for activating member operation
 * 	for DCE 1.2.2.
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/3  1996/02/29  19:00 UTC  hanfei
 * 	add uuids as args to rs_log_pgo_add_member()
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/02/21  15:41 UTC  hanfei
 * 	restructuring in rs_pgo_add_member()
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/06  20:36 UTC  hanfei
 * 	multi-cell group work: add codes for handling global group as member.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:17 UTC  psn
 * 	CHFts16436: collapse // to / in pgo names
 * 	[1995/10/05  16:59 UTC  aha  /main/HPDCE02/5]
 * 
 * 	CHFts16436: call rsdb_name_canonicalize_components() for non-principal names
 * 	[1995/10/02  20:24 UTC  aha  /main/HPDCE02/aha_mothra10/1]
 * 
 * 	CHFts15254: prevent addition of ambiguous child cell or principal
 * 	directory names (OT12680)
 * 	[1995/07/20  18:39 UTC  aha  /main/HPDCE02/4]
 * 
 * 	CHFts15254: prevent ambiguous child-cell and principal names
 * 	[1995/07/20  13:23 UTC  aha  /main/HPDCE02/aha_mothra8/1]
 * 
 * 	CHFts15254/OT12680: prevent addition of ambiguous principals;
 * 	merge out from mainline
 * 	[1995/07/17  20:32 UTC  aha  /main/HPDCE02/aha_mothra7/6]
 * 
 * 	CHFts15254: prevent ambiguity between child cell and principal directories;
 * 	improve readability based on code review
 * 	[1995/07/17  19:26 UTC  aha  /main/HPDCE02/aha_mothra7/5]
 * 
 * 	CHFts15254: disallow new principal directories that
 * 	conflict with cell names; disallow new cell names
 * 	that conflict with existing principal directories.
 * 	[1995/07/14  20:35 UTC  aha  /main/HPDCE02/aha_mothra7/4]
 * 
 * 	CHFts14236: disallow deletion of primary cell name;
 * 	adjust realm name to match its usage in primary cell principal name
 * 	[1995/07/06  19:46 UTC  aha  /main/HPDCE02/aha_mothra7/3]
 * 
 * 	CHFts14236: disallow deletion of primary cell name;
 * 	check return from malloc; remove unnecessary assignment to cell_name
 * 	[1995/07/06  15:31 UTC  aha  /main/HPDCE02/aha_mothra7/2]
 * 
 * 	CHFts14236: disallow deletion of primary cell name
 * 	[1995/07/05  20:46 UTC  aha  /main/HPDCE02/aha_mothra7/1]
 * 
 * 	Merge ACL alias fix (CHFts15622) from mb_mothra5
 * 	[1995/07/13  21:31 UTC  burati  /main/HPDCE02/3]
 * 
 * 	Alias work
 * 	[1995/07/13  20:47 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 * 
 * 	CHFts14236: preserve cell's original primary name
 * 	[1995/07/07  19:26 UTC  aha  /main/HPDCE02/2]
 * 
 * 	migration: conditionally repress alias creation in krbtgt/ tree
 * 	[1995/05/08  17:13 UTC  rps  /main/rps_wp/1]
 * 
 * Revision 1.1.89.5  1994/09/30  20:52:57  kline_s
 * 	CR 12426 - Fix bogus audit event specific info for PGO_Get and
 * 	PGO_KeyTransfer events.
 * 	[1994/09/30  20:51:32  kline_s]
 * 
 * Revision 1.1.89.4  1994/09/09  20:41:08  kline_s
 * 	OT 12028: I18N code cleanup for Security.
 * 	[1994/09/09  20:34:16  kline_s]
 * 
 * Revision 1.1.89.3  1994/08/08  14:23:35  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/08/04  19:23:52  mdf]
 * 
 * Revision 1.1.89.2  1994/07/15  15:01:44  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:20:12  mdf]
 * 
 * Revision 1.1.89.1  1994/06/02  21:22:47  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 
 * 	hp_sec_to_osf_3 drop, fixed segfault.
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/19  13:58:23  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  16:55:30  mdf]
 * 
 * Revision 1.1.86.1  1993/10/07  13:12:17  mccann
 * 	CR8651 64bit porting changes
 * 	[1993/10/07  13:07:08  mccann]
 * 
 * Revision 1.1.6.4  1992/12/29  16:36:07  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:24:58  zeliff]
 * 
 * Revision 1.1.6.3  1992/11/18  17:40:12  sekhar
 * 	[OT 5503]  Drop the following fix from Mike Burati
 * 	[1992/11/18  12:32:48  sekhar]
 * 
 * Revision 1.1.7.2  1992/10/15  19:47:53  burati
 * 	CR5503 Don't path_resolve on null string
 * 
 * Revision 1.1.6.2  1992/10/07  20:34:53  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:17:54  sekhar]
 * 
 * Revision 1.1.4.2  1992/10/06  17:21:15  burati
 * 	Second replication drop: bl5
 * 
 * Revision 1.1.3.3  1992/07/13  20:02:11  ahop
 * 	 3903: Before pgo_rename, call rsdb_acct_store_default_pepper.
 * 	[1992/06/30  22:31:18  ahop]
 * 
 * Revision 1.1.3.2  1992/05/22  20:08:01  ahop
 * 	 CR3427: check for illegal name input before other processing in rs_pgo_add
 * 	[1992/05/22  19:54:08  ahop]
 * 
 * Revision 1.1  1992/01/19  14:44:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991
 * Unpublished work. All Rights Reserved.
 */
/* 
 *      Registry Server - PGO name management
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
    "$Id: rs_pgo.c,v 1.1.96.3 1996/11/13 18:10:48 arvind Exp $";
#endif

#include <dce/rpc.h>
#include <dce/id_base.h>
#include <rs_pgo.h>

#include <bstring.h>
#include <un_strng.h>

#include <rs_server.h>
#include <rs_log_pgo.h>
#include <rs_auth.h>
#include <rs_ch_lock.h>
#include <rs_ver.h>
#include <rs_ver_pvt.h>
#include <rsdb_pgo.h>
#include <rsdb_name_util.h>
#include <rsdb_util.h>
#include <rsdb_acl.h>
#include <rsdb_misc.h>
#include <rsdb_acct.h>
#include <dce/dce_utils.h>

#if defined(DCE_AUD_POINTS)
#include <dce/audit.h>
#include <sec_aud_evts.h>
#endif	/* defined(DCE_AUD_POINTS) */

#if defined(DCE_AUD_POINTS)
extern dce_aud_trail_t sec_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */

/*
 * Temporary macros to access variant part of query unions.
 * Someday NIDL will support a better mechanism for specifying
 * unions.
 */
#define QUERY_TAG(Q) (Q).query
#define QUERY_NAME(Q) (Q).tagged_union.name
#define QUERY_ID(Q) (Q).tagged_union.id_key
#define QUERY_UNIX_NUM(Q) (Q).tagged_union.unix_num_key
#define QUERY_NEXT_SCOPE(Q) (Q).tagged_union.scope
#define RESULT_ITEM(R) (R).tagged_union.result.item
#define RESULT_NAME(R) (R).tagged_union.result.name

static char *domain_names[3] = {
    SEC_PRINCIPAL_OBJ_NAME,
    SEC_GROUP_OBJ_NAME,
    SEC_ORG_OBJ_NAME,
}; 

static boolean32	    support_global_group = false;

PRIVATE uuid_t  *per_mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_person]);
PRIVATE uuid_t  *dir_mgr_type = &(rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_dir]);



/*
 * Internal Functions
 */

/* valid_scope
 * 
 * checks that the scope in the query key actually exists. Basically an
 * optimization so we don't waste cycles on id_based backend queries 
 * only to discover that the scope doesn't exist.
 *
 * If the scope names an existing directory or pgo in the given domain,
 * return true; otherwise, return false.
 * For query keys not associated with scoping, just return true.
 */
PRIVATE boolean32 valid_scope
  (
    sec_rgy_domain_t    name_domain,
    rs_pgo_query_key_t  *key_p,
    error_status_t      *status_p
  )
{
    sec_rgy_name_t      *scope;                         

    CLEAR_STATUS(status_p);

    switch (key_p->query) {

    case rs_pgo_query_id:      
        scope = (sec_rgy_name_t *)QUERY_ID(*key_p).scope;
        break;
    case rs_pgo_query_unix_num:
        scope = (sec_rgy_name_t *)QUERY_UNIX_NUM(*key_p).scope;
        break;
    case rs_pgo_query_next:    
        scope = (sec_rgy_name_t *)QUERY_NEXT_SCOPE(*key_p);
        break;
    default:
        return true;
    }
    /* Return true if there is no scope. */       
    if((*scope)[0] == '\0') {
        return true;
    }
    /* Return true if scope names an existing directory or pgo. */
    if(rsdb_name_util_item_exists(name_domain, *scope, status_p)) {
        return true;
    } else {
        SET_STATUS(status_p, sec_rgy_bad_scope);
        return false;
    }
}

/* query_in_scope
 * 
 * checks that the result of a valid backend id-based query is within the
 * named scope of the query.
 */
PRIVATE boolean32 query_in_scope
  (
    rs_pgo_query_key_t     *key_p,
    rs_pgo_query_result_t  *result_p,
    error_status_t         *status_p
  )
{
    sec_rgy_name_t      *scope; 
    long                scope_len, result_len; 
    short               prefix_type;

    CLEAR_STATUS(status_p);

    switch (key_p->query) {

    case rs_pgo_query_id:
        scope = (sec_rgy_name_t *)QUERY_ID(*key_p).scope;
        break;
    case rs_pgo_query_unix_num:
        scope = (sec_rgy_name_t *)QUERY_UNIX_NUM(*key_p).scope;
        break;
    case rs_pgo_query_next:
        scope = (sec_rgy_name_t *)QUERY_NEXT_SCOPE(*key_p);
        break;
    default:
        return true;
    }                                         
    scope_len = ustrlen(*scope);
    /* return true if there is no scope */
    if(scope_len == 0) {
        return true;
    }
    result_len = ustrlen(RESULT_NAME(*result_p));
    /* Return true is scope is a prefix of, or identical to, the result name */
    if(rsdb_name_util_is_prefix(*scope, scope_len, RESULT_NAME(*result_p),
                                result_len, &prefix_type)) { 
        if(prefix_type == rsdb_over_qualified) {
            /* scope is longer than result name */ 
            SET_STATUS(status_p, sec_rgy_object_not_in_scope);
            return false;
        } else {
            return true;
        }
    } else {
        /* scope is not a prefix of result name */
        SET_STATUS(status_p, sec_rgy_object_not_in_scope);
        return false;
    }
}

/* query_is_dcepcs
 *
 * checks the inputted string for any occurrences of non-ASCII 
 * (or more concisely, non-PCS) characters and returns false
 * if any are found; true otherwise.
 */
PRIVATE int query_is_dcepcs
  (
    sec_rgy_name_t         name
  )
{
    char *tname;

    tname = (char *)name;
    while (*tname != '\0')
        if (!isdcepcs(*tname++))  {
            return false;
        }
    return true;
}

/*
 * End of Internal Functions
 */

PUBLIC void rs_pgo_add
  (
    handle_t             h,            /* [in] */
    sec_rgy_domain_t     name_domain,  /* [in] */
    sec_rgy_name_t       pgo_name,     /* [in] */
    sec_rgy_pgo_item_t   *(pgo_item),  /* [in] */
    rs_cache_data_t      *cache_info,  /* [out] */
    error_status_t       *(status)
  )
{
    sec_rgy_name_t          parent_name;
    char                    buf[64];
    sec_acl_permset_t       parent_perms;
    sec_acl_permset_t       null_perms = (sec_acl_permset_t) NULL;
    boolean32               check_quota = true;
    sec_rgy_foreign_id_t    client_ids;                   
    error_status_t          tmp_status;
    boolean32               prop_acl;
    boolean32               is_cell = FALSE;
    rs_pgo_query_key_t      key = {rs_pgo_query_next, { 0 }};
    sec_rgy_name_t          cell_dir  = { 0 };

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    dce_aud_ev_info_t         ev_info;
    error_status_t            aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);

    /* Check name for legal syntax before processing */
    if ( !LEGAL_PGO_NAME(pgo_name) || !query_is_dcepcs(pgo_name))
        SET_STATUS(status, sec_rgy_bad_name);
                  
    if (STATUS_OK(status)) {

      if (name_domain == sec_rgy_domain_person) {

        rsdb_name_canonicalize_and_convert(pgo_name, status);
        if (is_cell = rsdb_name_util_is_cell(pgo_name, status)) {
            /*
             * OT12680: get pointer to "X" in "krbtgt/X" for
             * use in checking for naming ambiguity under lock
             */
            strcpy((char *) QUERY_NEXT_SCOPE(key), (char *)pgo_name + KRB_DIR_ROOT_LEN);
        } else {
            /*
             * OT12680: extract any initial directory from the principal
             * name for use in checking for naming ambiguity under lock
             */
            char                *name_ptr;

            if ((name_ptr=u_strchr(pgo_name, '/')) != NULL) {
                ustrncat(
                  ustrcpy(cell_dir, KRB_DIR_ROOT),
                  pgo_name,
                  name_ptr - (char *)pgo_name
                );
            }
        }
      } else {

        /* CHFts16436: collapse any "//" to "/" */
        rsdb_name_canonicalize_components(pgo_name);

      }
    }

    if (STATUS_OK(status)) { 

        /* parent directory of new pgo must provide insert rights */
        parent_perms = sec_acl_perm_insert;

        /* pgo_name checked again in case canonicalization increased its length */
        if ( !check_domain(name_domain) || !CHECK_PGO_NAME(pgo_name) || 
             !CHECK_PGO_FULLNAME(pgo_item->fullname) ) {
            SET_STATUS(status, sec_rgy_bad_data);
        } else if ( !is_cell ) {
            rsdb_unix_uuid_check((int *)&(pgo_item->unix_num), &(pgo_item->id), status);
	} else if ( is_cell && IS_AN_ALIAS(pgo_item)) {
	    rs_sw_feature_test_support(rs_c_sw_feature_cell_alias, status);
	}
    }

    if (BAD_STATUS(status)) {
        rs_util_bad_client(h, "rs_pgo_add", status);
    }

#if defined(DCE_AUD_POINTS)
    dce_aud_start(PGO_Add, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_long_int;
        ev_info.data.long_int = name_domain;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = pgo_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if (STATUS_OK(status)) { 
        BEGIN_MASTER_RIW(lock_db, status) {
            sprintf(buf, "Attempt PGO add (%s)", domain_names[name_domain]);
            rs_audit_trail_log(h, buf, (char *) pgo_name);

            if (*QUERY_NEXT_SCOPE(key) != '\0' ) {
                error_status_t      query_status;
                sec_rgy_cursor_t    item_cursor;
                rs_pgo_query_result_t result;

                /* OT12680: if new pgo is a cell name "krbtgt/X", then need
                 * to disallow if there is an existing "principal/X/"
                 * directory to avoid naming ambiguity.
                 */

                rs_util_clear_cursor(&item_cursor);
                CLEAR_STATUS(&tmp_status);
                while (STATUS_OK(&tmp_status)) {
                    rsdb_pgo_get_next(sec_rgy_domain_person,
                                      &item_cursor, 
                                      (RESULT_NAME(result)),
                                      &(RESULT_ITEM(result)),
                                      &tmp_status);
                    if (STATUS_OK(&tmp_status)
                    && (query_in_scope(
                      &key,
                      &result,
                      &query_status))) {
                        /* DISALLOW */
                        SET_STATUS(status, sec_rgy_bad_name);
                        break;
                    }
                }

            } else if (*cell_dir != '\0') {

                /* OT12680: if new pgo includes a new "principal/X/" directory,
                 * then disallow if there is an existing cell name
                 * "principal/krbtgt/X" that would create naming ambiguity.
                 */

                if (rsdb_name_util_item_exists(sec_rgy_domain_person, cell_dir, &tmp_status)) {
                    /* DISALLOW */
                    SET_STATUS(status, sec_rgy_bad_name);
                }
            }

            if (STATUS_OK(status)) {
                /* check for insert permissions on parent */
                rsdb_name_util_get_parent(pgo_name, parent_name);

                /* Until secd exports a directory interface, pgo_add creates new
                 * directories if required; thus, some parent directories in
                 * pgo_name may not yet exist.
                 * 
                 * Get nearest existing parent for auth check.
                 */
                rsdb_name_util_get_exist_parent(name_domain, parent_name);
                if (rs_auth_is_authorized(h, parent_perms, null_perms,
                    name_domain, parent_name, dir_mgr_type, 
                    check_quota, &client_ids, status)) {

                    /* If cell name, don't embed unix id in uuid. */
                    if(rsdb_unix_uuid_gen_check(name_domain, !is_cell, 
                       (int *)&(pgo_item->unix_num), &(pgo_item->id), status)) {

                        RIW_PROMOTE(lock_db); 
                        rs_util_pgo_add(name_domain, pgo_name, pgo_item, NULL_PTR,
                                        &client_ids, true, &prop_acl, status);

                        if (STATUS_OK(status)) {
                            rsdb_util_quota_decrement(&client_ids.principal,
                                &client_ids.cell, &tmp_status);
                            sprintf(buf, "PGO add (%s)", 
                                    domain_names[name_domain]);
                            rs_audit_trail_log(h, buf,(char *) pgo_name);
                            rs_log_pgo_add(name_domain, pgo_name, pgo_item,
                                           &client_ids, NULL_PTR);

                            /* Propagate ACL if necessary, to avoid old alias
                             * bug where new creator entries were added by mistake
                             */
                            if (prop_acl) {
                                rs_rdacl_prop_acl(name_domain, pgo_name,
                                    &tmp_status);
                            }
                        }
                    }
                }
            }
        } END_MASTER_RIW
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    rs_util_get_cache_data(cache_info);
}


PUBLIC void rs_pgo_delete
  (
    handle_t          h,            /* [in] */
    sec_rgy_domain_t  name_domain,  /* [in] */
    sec_rgy_name_t    pgo_name,     /* [in] */
    rs_cache_data_t   *cache_info,  /* [out] */
    error_status_t    *(status)
  )
{
    char                    buf[64];
    sec_rgy_name_t          parent_name;
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms;
    uuid_t                  *mgr_type;         
    sec_rgy_foreign_id_t    client_ids;                   
    sec_rgy_name_t          realm;
    char                    *realm_p;
    char                    *cell_name;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    dce_aud_ev_info_t         ev_info;
    error_status_t            aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);              
    mgr_type = &(rsdb_acl_mgr_uuids[GET_MGR_BY_DOMAIN(name_domain)]);

    /* pgo must provide delete rights */
    requested_perms = sec_acl_perm_Delete_object;
    /* parent directory of pgo must provide delete rights */
    parent_perms = sec_acl_perm_delete;

    if (!check_domain(name_domain) || !CHECK_PGO_NAME(pgo_name)
            || !query_is_dcepcs(pgo_name)) {
        SET_STATUS(status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_pgo_delete", status);
    }

    if (GOOD_STATUS(status)) {

      if (name_domain == sec_rgy_domain_person) {

        rsdb_name_canonicalize_and_convert(pgo_name, status);
        if (GOOD_STATUS(status)) {
            /* get /.../cell_name */
            rsdb_properties_get_realm_name(realm);
            realm_p=(char *)realm;
            /* skip over the /.../ prefix */
            realm_p += GLOBAL_DIR_ROOT_LEN;
            cell_name=malloc(KRB_DIR_ROOT_LEN + strlen(realm_p) + 1);
            if (cell_name == NULL) {
                SET_STATUS(status, sec_rgy_cant_allocate_memory);
            } else {
                /* initialize cell_name with "krbtgt/" */
                strcpy(cell_name,KRB_DIR_ROOT);
                /* compare krbtgt/cell_name with principal pgo_name being deleted */
                if (ustrcmp(strcat(cell_name, realm_p), pgo_name) == 0) {
                    /* CHFts14236: can't delete original primary cell name
                     * because cell account is associated with it.
                     */
                    SET_STATUS(status, sec_rgy_s_pgo_is_required);
                    rs_util_bad_client(h, "rs_pgo_delete", status);
                }
                free(cell_name);
            }
        }
      } else {

        /* CHFts16436: collapse any "//" to "/" */
        rsdb_name_canonicalize_components(pgo_name);

      }
    }

#if defined(DCE_AUD_POINTS)
    dce_aud_start(PGO_Delete, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_long_int;
        ev_info.data.long_int = name_domain;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = pgo_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if (STATUS_OK(status)) {
        BEGIN_MASTER_RIW(lock_db, status) {
            sprintf(buf, "Attempt PGO delete (%s)", domain_names[name_domain]);
            rs_audit_trail_log(h, buf, (char *) pgo_name);

            if (rs_auth_is_authorized(h, requested_perms, parent_perms,
                                name_domain, pgo_name, mgr_type, false, 
                                &client_ids, status)) {
                if (rs_util_pgo_is_required(name_domain, pgo_name, status)) {
                    SET_STATUS(status, sec_rgy_s_pgo_is_required);
                } else {
                    RIW_PROMOTE(lock_db);
                                      
                    rs_util_pgo_delete(name_domain, pgo_name, status);
                    if (STATUS_OK(status)) {
                        rs_util_update_cache_info(name_domain);
                        sprintf(buf, "PGO delete (%s)", domain_names[name_domain]);
                        rs_audit_trail_log(h, buf, (char *) pgo_name);
                        rs_log_pgo_delete(name_domain, pgo_name, NULL_PTR);
                    }
                }
            }
        } END_MASTER_RIW
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    rs_util_get_cache_data(cache_info);
}

                                                                             

PUBLIC void rs_pgo_replace
  (
    handle_t            h,            /* [in] */
    sec_rgy_domain_t    name_domain,  /* [in] */
    sec_rgy_name_t      pgo_name,     /* [in] */
    sec_rgy_pgo_item_t  *(pgo_item),  /* [in] */
    rs_cache_data_t     *cache_info,  /* [out] */
    error_status_t      *(status)
  )
{
    sec_rgy_cursor_t        cursor;
    sec_rgy_pgo_item_t      existing_item;
    char                    buf[64];
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = (sec_acl_permset_t) NULL;
    uuid_t                  *mgr_type;         
    sec_rgy_foreign_id_t    client_ids;                             
    boolean32               authorized = false;
    boolean32               is_cell;
    error_status_t          tmp_status;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    dce_aud_ev_info_t         ev_info;
    error_status_t            aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);
    mgr_type = &(rsdb_acl_mgr_uuids[GET_MGR_BY_DOMAIN(name_domain)]);
    
    if (!check_domain(name_domain) || !CHECK_PGO_NAME(pgo_name) 
            || !CHECK_PGO_FULLNAME(pgo_item->fullname) 
            || !query_is_dcepcs(pgo_name)) {
        SET_STATUS(status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_pgo_replace", status);
    }

    is_cell = rsdb_name_util_is_cell(pgo_name, &tmp_status);

    if (GOOD_STATUS(status)) {
      if (name_domain == sec_rgy_domain_person) {
        rsdb_name_canonicalize_and_convert(pgo_name, status);
      } else {
        /* CHFts16436: collapse any "//" to "/" */
        rsdb_name_canonicalize_components(pgo_name);
      }
    }

#if defined(DCE_AUD_POINTS)
    dce_aud_start(PGO_Replace, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_long_int;
        ev_info.data.long_int = name_domain;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = pgo_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if (STATUS_OK(status)) {
        BEGIN_MASTER_RIW(lock_db, status) {
            sprintf(buf, "Attempt PGO replace (%s)", domain_names[name_domain]);
            rs_audit_trail_log(h, buf, (char *) pgo_name);

            rsdb_pgo_get_by_name(name_domain, pgo_name, &cursor, 
                                 &existing_item, status);
            if (STATUS_OK(status)) {
                rs_auth_pgo_update_check(&existing_item, pgo_item, is_cell,
                                         &requested_perms, status); 
                if (STATUS_OK(status)) {
                    if (rs_auth_is_authorized(h, requested_perms,
                             parent_perms, name_domain, pgo_name, 
                             mgr_type, false, &client_ids, status)) {
                        authorized = true;
                    }
                }
            }
            if (authorized) {
                RIW_PROMOTE(lock_db);
                rsdb_pgo_replace(name_domain, pgo_name, pgo_item, status);

                if (STATUS_OK(status)) {
                    /*
                    * Check if cache state has changed...
                    */
                    if (pgo_item->unix_num != existing_item.unix_num) {
                        rs_util_update_cache_info(name_domain);
                    }

                    sprintf(buf, "PGO replace (%s)",
                            domain_names[name_domain]);
                    rs_audit_trail_log(h, buf, (char *) pgo_name);
                    rs_log_pgo_replace(name_domain, pgo_name, pgo_item,
                                       NULL_PTR);
                }
            }
        } END_MASTER_RIW
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    rs_util_get_cache_data(cache_info);
}


/*  r s _ p g o _ r e n a m e 
 *
 *  Rename or move a pgo from old_name to new_name within
 *  the given name_domain.  
 *  For rename: If the leaf components of old_name
 *  and new_name are in the same directory (e.g., person/anne and
 *  person/ahop), the name (n bit) permission is required on the
 *  old leaf.  
 *  For move: If the leaf components are in different directories
 *  (e.g., person/anne and person/ccd/anne), the n bit is required
 *  on the old leaf, the d bit is required on the old parent,
 *  and the i bit is required on the new parent directory.
 */
PUBLIC void rs_pgo_rename
  (
    handle_t          h,            /* [in] */
    sec_rgy_domain_t  name_domain,  /* [in] */
    sec_rgy_name_t    old_name,     /* [in] */
    sec_rgy_name_t    new_name,     /* [in] */
    rs_cache_data_t   *cache_info,  /* [out] */
    error_status_t    *(status)
  )
{
    uuid_t                  *mgr_type;
    sec_rgy_foreign_id_t    client_ids;
    boolean32               old_auth, new_auth;
    sec_rgy_name_t          old_par_name;
    sec_rgy_name_t          new_par_name;
    long                    new_par_len;
    sec_rgy_name_t          new_leaf;
    long                    new_leaf_len;
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms;
    char                    buf[64];
    boolean32               move_required = true;
    error_status_t          st1, st2;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    dce_aud_ev_info_t         ev_info;
    error_status_t            aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(PGO_Rename, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_long_int;
        ev_info.data.long_int = name_domain;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = old_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = new_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if (!check_domain(name_domain) || !CHECK_PGO_NAME(old_name) 
           || !CHECK_PGO_NAME(new_name) || !query_is_dcepcs(new_name)) {
        SET_STATUS(status, sec_rgy_bad_data);
    } else if (name_domain == sec_rgy_domain_person) {
        rsdb_name_canonicalize_and_convert(new_name, status);
        if (STATUS_OK(status)) {
            rsdb_name_canonicalize_and_convert(old_name, status);
        }
    } else {
        /* CHFts16436: collapse any "//" to "/" */
        rsdb_name_canonicalize_components(new_name);
        rsdb_name_canonicalize_components(old_name);
    }

    if (STATUS_OK(status)) {

        mgr_type = &(rsdb_acl_mgr_uuids[GET_MGR_BY_DOMAIN(name_domain)]);
    
        old_par_name[0] = '\0';
        new_par_name[0] = '\0';
        rsdb_name_util_get_parent(old_name, old_par_name);
        /* Use strip leaf on new_name to get both parent and leaf */
        ustrcpy(new_par_name, new_name);  
        new_par_len = ustrlen(new_par_name);
        rsdb_name_util_strip_leaf(new_par_name, &new_par_len,
                                  new_leaf, &new_leaf_len);
        if( ustrcmp(old_par_name, new_par_name) == 0 ) {
                move_required = false;                   
        }
    }

    if (BAD_STATUS(status)) {
        rs_util_bad_client(h, "rs_pgo_rename", status);
    }

    if (STATUS_OK(status)) {
        BEGIN_MASTER_RIW(lock_db, status) {
            sprintf(buf, "Attempt PGO rename (%s):",
                    domain_names[name_domain]);
            rs_audit_trail_log(h, buf, (char *) old_name);

            if( move_required ) {
                /* check permissions on old_name */
                requested_perms = sec_acl_perm_name;
                parent_perms = sec_acl_perm_delete;
                old_auth = rs_auth_is_authorized(h, requested_perms, 
                           parent_perms, name_domain, old_name, mgr_type,
                           false, &client_ids, &st1);
                /* check permissions on new name */
                requested_perms = sec_acl_perm_insert;
                parent_perms = (sec_acl_permset_t)NULL;
                rsdb_name_util_get_exist_parent(name_domain, 
                                                new_par_name);
                new_auth = rs_auth_is_authorized(h, requested_perms, 
                           parent_perms, name_domain, new_par_name,
                           dir_mgr_type, false, &client_ids, &st2);
                if(old_auth && new_auth) {
                    RIW_PROMOTE(lock_db);
                    if(name_domain == sec_rgy_domain_person) {
                        rsdb_acct_store_default_pepper(old_name, status);
                    }
                    if(STATUS_OK(status))
                        rsdb_pgo_move(name_domain, old_name, new_name, status);
                }
                else { 
                    /* If either old or new name didn't allow
                     * any access, return not_found.
                     */
                    if(STATUS_EQUAL(&st1, sec_rgy_object_not_found) ||
                       STATUS_EQUAL(&st2, sec_rgy_object_not_found)) {
                        SET_STATUS(status, sec_rgy_object_not_found);
                    } else
                        SET_STATUS(status, sec_rgy_not_authorized);
                }
            } else {       /* if rename only (!move_required) */
                requested_perms = sec_acl_perm_name;
                parent_perms = (sec_acl_permset_t)NULL;
                if(rs_auth_is_authorized(h, requested_perms, 
                           parent_perms, name_domain, old_name, mgr_type,
                           false, &client_ids, status)) {
                    RIW_PROMOTE(lock_db);
                    if(name_domain == sec_rgy_domain_person) {
                        rsdb_acct_store_default_pepper(old_name, status);
                    }
                    if(STATUS_OK(status))
                        rsdb_pgo_rename(name_domain, old_name, new_leaf, status);
                }
            }
            if(STATUS_OK(status)) {
                rs_util_update_cache_info(name_domain);
                sprintf(buf, "PGO rename (%s)",
                        domain_names[name_domain]);
                rs_audit_trail_log(h, buf, (char *) new_name);
                rs_log_pgo_rename(name_domain, old_name, new_name, NULL_PTR);                   
            }
        } END_MASTER_RIW
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    rs_util_get_cache_data(cache_info);
}


PUBLIC void rs_pgo_get
  (
    handle_t               h,              /* [in] */
    sec_rgy_domain_t       name_domain,    /* [in] */
    rs_pgo_query_key_t     *(key),         /* [in] */
    boolean32              allow_aliases,  /* [in] */
    sec_rgy_cursor_t       *(item_cursor), /* [in, out] */
    rs_cache_data_t        *cache_info,    /* [out] */
    rs_pgo_query_result_t  *(result)
  )
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = (sec_acl_permset_t) NULL;
    uuid_t                  *mgr_type;         
    sec_rgy_foreign_id_t    client_ids;     
    sec_rgy_name_t          *auth_name_p;
    error_status_t          status;
    boolean32               done  = false;  /* so we can iterate over out-of-scope
                                             * get_next queries
                                             */
#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    dce_aud_ev_info_t         ev_info;
    error_status_t            aud_status;
    error_status_t            aud_status2;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(&status);
    mgr_type = &(rsdb_acl_mgr_uuids[GET_MGR_BY_DOMAIN(name_domain)]);

    /* retrieved PGO must provide read permission. */
    requested_perms = sec_acl_perm_read;

    rs_util_check_cursor(item_cursor);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(PGO_Get, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_long_int;
        ev_info.data.long_int = name_domain;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        /* Fix for OT 12426  - just display the key in the format as 
         * it is received from the caller.  This avoids making
         * expensive registry lookup and conversion calls to acquire
         * the uuid value of the key.  In some instances, the uuid
         * may not even be available in the registry, so this also 
         * precludes bogus/trashed uuid values from being included 
         * for this audit event.
         */
        ev_info.format = aud_c_evt_info_ulong_int;
        ev_info.data.ulong_int = QUERY_TAG(*key);
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        switch(QUERY_TAG(*key)){
        case rs_pgo_query_name:
            ev_info.format = aud_c_evt_info_char_string;
            ev_info.data.char_string = QUERY_NAME(*key);
            break;
        case rs_pgo_query_id: 
            ev_info.format = aud_c_evt_info_uuid;
            ev_info.data.uuid = QUERY_ID(*key).id;
            break;
        case rs_pgo_query_unix_num:
            ev_info.format = aud_c_evt_info_long_int;
            ev_info.data.long_int = QUERY_UNIX_NUM(*key).unix_num;
            break;
        case rs_pgo_query_next:
            ev_info.format = aud_c_evt_info_char_string;
            ev_info.data.char_string = QUERY_NEXT_SCOPE(*key);
            break;
        default:   /* Handle extreme error case gracefully */
            ev_info.format = aud_c_evt_info_uuid;
            uuid_create_nil(&ev_info.data.uuid, &aud_status2);
            break;
         }
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    if (!check_domain(name_domain)) {
        SET_STATUS(&status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_pgo_get: domain", &status);
    } else if (valid_scope(name_domain, key, &status)) {
        BEGIN_READ(lock_db, &status) {
            while (!done) {
                switch (QUERY_TAG(*key)) {
                    
                case rs_pgo_query_name:
                    done = true;
                    if (!CHECK_PGO_NAME(QUERY_NAME(*key)) 
                            || !query_is_dcepcs(QUERY_NAME(*key))) {
                        SET_STATUS(&status, sec_rgy_bad_data);
                        rs_util_bad_client(h, "rs_pgo_get", &status);
                        break;
                    }
                           
                    if (name_domain == sec_rgy_domain_person) {
                        rsdb_name_canonicalize_and_convert(QUERY_NAME(*key), &status);
                    } else {
                        rsdb_name_canonicalize_components(QUERY_NAME(*key));
                    }

                    if (STATUS_OK(&status)) {
                        rsdb_pgo_get_by_name(name_domain, QUERY_NAME(*key),
                                             item_cursor, &(RESULT_ITEM(*result)),
                                             &status);
                        auth_name_p = (sec_rgy_name_t *)QUERY_NAME(*key);
                    }
                    break;
                    
                case rs_pgo_query_id: 
                    done = true;
                    rsdb_pgo_get_by_uid(name_domain, &(QUERY_ID(*key).id),
                                        allow_aliases, item_cursor,
                                        RESULT_NAME(*result),
                                        &(RESULT_ITEM(*result)), &status); 
                    
                    if (STATUS_OK(&status)) {
                        (void)query_in_scope(key, result, &status);
                        auth_name_p = (sec_rgy_name_t *)RESULT_NAME(*result);
                    }
                    break;
                    
                case rs_pgo_query_unix_num:
                    done = true;
                    rsdb_pgo_get_by_unix_num(name_domain,
                                             QUERY_UNIX_NUM(*key).unix_num,
                                             allow_aliases, item_cursor, 
                                             RESULT_NAME(*result),
                                             &(RESULT_ITEM(*result)),
                                             &status);
                    
                    if (STATUS_OK(&status)) {
                        (void)query_in_scope(key, result, &status);
                        auth_name_p = (sec_rgy_name_t *)RESULT_NAME(*result);
                    }      
                    break;

                case rs_pgo_query_next:
                    rsdb_pgo_get_next(name_domain, item_cursor, 
                                      (RESULT_NAME(*result)),
                                      &(RESULT_ITEM(*result)), &status);
                    if (STATUS_OK(&status)) {
                        done = query_in_scope(key, result, &status); 
                        auth_name_p = (sec_rgy_name_t *)RESULT_NAME(*result);
                    } else {
                        done = true;
                    }
                    break;
                    
                default:
                    SET_STATUS(&status, sec_rgy_not_implemented);
                    break;
                }   

                if (done && STATUS_OK(&status)) {
                /*
                 * Make sure we are authorized to view the data.  If not
                 * we silently go to the next item that we can access.
                 * If we have a query_by_name key - there is no "next"
                 * item to read, so we pass back the authorization failure.
                 */
                    done = rs_auth_is_authorized(h, requested_perms,
                            parent_perms, name_domain, *auth_name_p, 
                            mgr_type, false, &client_ids, &status);

                    if (QUERY_TAG(*key) == rs_pgo_query_name) {
                        done = true;
                    }
                }
            }
        } END_READ
    }

    SET_STATUS( (error_status_t *) &(result->status), STATUS(&status));

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(&status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    rs_util_get_cache_data(cache_info);
}


/* 
 *  An access check is performed for id -> name and unix_num -> name
 *  transfers only.
 */
PUBLIC void rs_pgo_key_transfer
  (
    handle_t            h,                      /* [in] */
    sec_rgy_domain_t    name_domain,            /* [in] */
    rs_pgo_query_t      requested_result_type,  /* [in] */
    rs_pgo_query_key_t  *(key),                 /* [in, out] */
    rs_cache_data_t     *cache_info,            /* [out] */
    error_status_t      *(status)
  )
{
    uuid_t                  id;
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = (sec_acl_permset_t) NULL;
    uuid_t                  *mgr_type;         
    sec_rgy_foreign_id_t    client_ids;     
    
#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    dce_aud_ev_info_t         ev_info;
    error_status_t            aud_status;
    error_status_t            aud_status2;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);
    mgr_type = &(rsdb_acl_mgr_uuids[GET_MGR_BY_DOMAIN(name_domain)]);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(PGO_KeyTransfer, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_long_int;
        ev_info.data.long_int = name_domain;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        /* Fix for OT 12426 - just display the key in the format as 
         * it is received from the caller.  This avoids making
         * expensive registry lookup and conversion calls to acquire
         * the uuid value of the key.  In some instances, the uuid
         * may not even be available in the registry, so this also 
         * precludes bogus/trashed uuid values from being included 
         * for this audit event.
         */

        ev_info.format = aud_c_evt_info_ulong_int;
        ev_info.data.ulong_int = QUERY_TAG(*key);
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        switch(QUERY_TAG(*key)){
        case rs_pgo_query_name:
            ev_info.format = aud_c_evt_info_char_string;
            ev_info.data.char_string = QUERY_NAME(*key);
            break;
        case rs_pgo_query_id: 
            ev_info.format = aud_c_evt_info_uuid;
            ev_info.data.uuid = QUERY_ID(*key).id;
            break;
        case rs_pgo_query_unix_num:
            ev_info.format = aud_c_evt_info_long_int;
            ev_info.data.long_int = QUERY_UNIX_NUM(*key).unix_num;
            break;
        default:   /* Handle extreme error case gracefully */
            ev_info.format = aud_c_evt_info_uuid;
            uuid_create_nil(&ev_info.data.uuid, &aud_status2);
            break;
         }
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_ulong_int;
        ev_info.data.ulong_int = requested_result_type;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* Although no specific permission is required (requested_perms = NULL), 
     * client must have some access to the named object.
     */
    requested_perms = (sec_acl_permset_t) NULL;

    if (!check_domain(name_domain)) {
        QUERY_TAG(*key) = rs_pgo_query_none;
        SET_STATUS(status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_pgo_key_transfer: domain", status);
    } else {
        BEGIN_READ(lock_db, status) {
            switch (QUERY_TAG(*key)) {

            case rs_pgo_query_name:  

                if (!CHECK_PGO_NAME(QUERY_NAME(*key))
                        || !query_is_dcepcs(QUERY_NAME(*key))) {
                    QUERY_TAG(*key) = rs_pgo_query_none;
                    SET_STATUS(status, sec_rgy_bad_data);
                    rs_util_bad_client(h, "rs_pgo_key_transfer", status);
                    break;
                }
                if(rs_auth_is_authorized(h, requested_perms,
                               parent_perms, name_domain, QUERY_NAME(*key),
                               mgr_type, false, &client_ids, status)) {

                    switch(requested_result_type) {

                    case rs_pgo_query_id:   /* name --> id */
                      
                        if (name_domain == sec_rgy_domain_person) {
                            rsdb_name_canonicalize_and_convert(QUERY_NAME(*key), status);
                        } else {
                            rsdb_name_canonicalize_components(QUERY_NAME(*key));
                        }
                              
                        if (STATUS_OK(status)) {
                            rsdb_pgo_name_to_uid(name_domain, QUERY_NAME(*key), &id, 
                                             status);
                            QUERY_TAG(*key) = rs_pgo_query_id;
                            QUERY_ID(*key).id = id;
                        }
                        break;
                    
                    case rs_pgo_query_unix_num:  /* name --> unix_num */
                        {
                            long    unix_num;

                            rsdb_pgo_name_to_unix_num(name_domain,
                                QUERY_NAME(*key), &unix_num, status);
                            QUERY_TAG(*key) = rs_pgo_query_unix_num;
                            QUERY_UNIX_NUM(*key).unix_num = unix_num;
                            break;
                        }

                    default:
                        QUERY_TAG(*key) = rs_pgo_query_none;
                        SET_STATUS(status, sec_rgy_not_implemented);
                        break;
                    }  
                }
                break;

            case rs_pgo_query_id: 

                switch (requested_result_type) {

                case rs_pgo_query_name:  /* id --> name */
                    {
                        sec_rgy_name_t name;
                        
                        rsdb_pgo_uid_to_name(name_domain, &(QUERY_ID(*key).id),
                            name, status);
                        if (STATUS_OK(status)) {
                            if(rs_auth_is_authorized(h, requested_perms,
                               parent_perms, name_domain, name, 
                               mgr_type, false, &client_ids, status)) {

                                QUERY_TAG(*key) = rs_pgo_query_name;
                                strcpy((char *)QUERY_NAME(*key), (char *)name);
                            }
                        }
                        break;
                    }
                
                case rs_pgo_query_unix_num:  /* id --> unix_num */
                    {
                        long    unix_num;
                        
                        rsdb_pgo_uid_to_unix_num(name_domain,
                            &(QUERY_ID(*key).id), &unix_num, status);
                        QUERY_TAG(*key) = rs_pgo_query_unix_num;
                        QUERY_UNIX_NUM(*key).unix_num = unix_num;
                        break;
                    }

                default:
                    QUERY_TAG(*key) = rs_pgo_query_none;
                    SET_STATUS(status, sec_rgy_not_implemented);
                    break;
                }
                break;
                
            case rs_pgo_query_unix_num:

                switch(requested_result_type) {

                case rs_pgo_query_id:  /* unix_num --> id */
                    {
                        rsdb_pgo_unix_num_to_uid(name_domain,
                            QUERY_UNIX_NUM(*key).unix_num, &id, status);
                        QUERY_TAG(*key) = rs_pgo_query_id;
                        QUERY_ID(*key).id = id;
                        break;
                    }
                
                case rs_pgo_query_name:  /* unix_num --> name */
                    {
                        sec_rgy_name_t name;
                        
                        rsdb_pgo_unix_num_to_name(name_domain,
                            QUERY_UNIX_NUM(*key).unix_num, name, status);
                        if (STATUS_OK(status)) {
                            if (rs_auth_is_authorized(h, requested_perms,
                                parent_perms, name_domain, name, 
                                mgr_type, false, &client_ids, status)) {

                            QUERY_TAG(*key) = rs_pgo_query_name;
                            strcpy((char *) QUERY_NAME(*key), (char *) name);
                            }
                        }
                        break;
                    }

                default:
                    QUERY_TAG(*key) = rs_pgo_query_none;
                    SET_STATUS(status, sec_rgy_not_implemented);
                    break;
                }
                break;

            default:
                QUERY_TAG(*key) = rs_pgo_query_none;
                SET_STATUS(status, sec_rgy_not_implemented);
                break;
            }

        } END_READ
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                1, aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard,
                    aud_c_evt_commit_sync_no_wait, 1, aud_c_esl_cond_denial,
                    &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard,
                    aud_c_evt_commit_sync_no_wait, 1, aud_c_esl_cond_failure,
                    &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    rs_util_get_cache_data(cache_info);
}


/*
 * r s _ p g o _ a d d _ m e m b e r
 *
 * Add the member person_name to the group or org, go_name.
 *
 * To add person_name to go_name, the client must have
 * Member_list (M) permission on go_name.
 * Additionally, if go_name names a group, the client must
 * have groups (g) permission on person_name.
 *
 * If person_name is a global name and name_domain is for group,
 * a local pgo is created under domain foreign_person ( detail
 * description in rsdb_pvt.h ) to hold information for this 
 * foreign principal.
 *
 */
PUBLIC void rs_pgo_add_member
  (
    handle_t          h,              /* [in] */
    sec_rgy_domain_t  name_domain,    /* [in] */
    sec_rgy_name_t    go_name,        /* [in] */
    sec_rgy_name_t    person_name,    /* [in] */
    rs_cache_data_t   *cache_info,    /* [out] */
    error_status_t    *(status)
  )
{
    char                    buf[128];
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = (sec_acl_permset_t) NULL;
    uuid_t                  *mgr_type;         
    sec_rgy_foreign_id_t    client_ids;                             
    boolean32               go_auth;
    boolean32               person_auth = true; 
    sec_rgy_name_t          cell_name;
    uuid_t                  cell_uuid;
    sec_rgy_name_t          princ_name;
    uuid_t                  princ_uuid;
    uuid_p_t		    member_cell_uuid_p = NULL_PTR;
    uuid_p_t		    member_princ_uuid_p = NULL_PTR;
    boolean32		    is_local=true;
    error_status_t          st1, st2;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    dce_aud_ev_info_t         ev_info;
    error_status_t            aud_status;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);
    CLEAR_STATUS(&st1);
    CLEAR_STATUS(&st2);
    mgr_type = &(rsdb_acl_mgr_uuids[GET_MGR_BY_DOMAIN(name_domain)]);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(PGO_AddMember, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_long_int;
        ev_info.data.long_int = name_domain;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = person_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = go_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* group/org must provide Member_list permission. */
    requested_perms = sec_acl_perm_Member_list;

    if (!check_go_domain(name_domain)) {
        SET_STATUS(status, sec_rgy_bad_domain);
    } else if (!CHECK_PGO_NAME(go_name) || !CHECK_PGO_NAME(person_name)
            || !query_is_dcepcs(go_name) || !query_is_dcepcs(person_name)) {
        SET_STATUS(status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_pgo_add_member", status);
    }

    if (STATUS_OK(status)) {
 	if (!rsdb_name_util_is_foreign_pgo(person_name) ) {
            rsdb_name_util_localize_pgo(person_name, status);
            if (STATUS_OK(status)) {
                rsdb_name_canonicalize_and_convert(person_name, status);
            }
        } else { /* is a global name for group membership */
	    is_local = false;
	    if ( ! support_global_group ) {
		/* 
		 * since once software version set forward, cannot
		 * be set back, use this check to save future calls.
		 */
            	support_global_group = rs_sw_feature_test_support
			(rs_c_sw_feature_global_group, status);
	    }

	    /* this operation only valid on group membership */
	    if ( STATUS_OK(status) ) {
	    	if ( name_domain != sec_rgy_domain_group ) {
	            SET_STATUS(status, sec_rgy_bad_domain);
	    	}
	    }

	    if ( STATUS_OK(status) ) {
	        sec_id_parse_name(NULL, person_name,
		     cell_name, &cell_uuid, princ_name, &princ_uuid, status);
		member_cell_uuid_p = &cell_uuid;
		member_princ_uuid_p = &princ_uuid;
	    }
	}
    }

    if (STATUS_OK(status)) {
        BEGIN_MASTER_RIW(lock_db, status) {
            sprintf(buf, "Attempt PGO add member (%s/%s)",
                    domain_names[name_domain], go_name);
            rs_audit_trail_log(h, buf, (char *) person_name);

            /* Check Member_list permissions on group/org. */
            go_auth = rs_auth_is_authorized(h, requested_perms,
                        parent_perms, name_domain, go_name, mgr_type,
                        false, &client_ids, &st1);
            /* Check access to person_name */
            if(name_domain == sec_rgy_domain_group) {        
                /* Check groups permission on person_name */
                requested_perms = sec_acl_perm_groups;
		if ( is_local ) {
		    /* only check acl access for local principal as member */
                    person_auth = rs_auth_is_authorized(h, requested_perms,
                        parent_perms, sec_rgy_domain_person, 
                        person_name, per_mgr_type,
                        false, &client_ids, &st2);
		}
            }
            if(go_auth && person_auth) {
                RIW_PROMOTE(lock_db);
		if ( ! is_local ) {
		    /* create a local principal for this foreign principal,
		     * if not yet existed 
		     */
		    rs_util_pgo_add_foreign_principal(person_name, 
				member_cell_uuid_p, member_princ_uuid_p,
				status);
		}
	    	if ( STATUS_OK(status) ) {
                    rsdb_pgo_add_member(name_domain, go_name, person_name, 
                                    status);
	    	}
                if (STATUS_OK(status)) {
                    sprintf(buf, "PGO add member (%s/%s)",
                                domain_names[name_domain], go_name);
                    rs_audit_trail_log(h, buf, (char *) person_name);
                    rs_log_pgo_add_member(name_domain, go_name, person_name,
					  member_cell_uuid_p, 
					  member_princ_uuid_p, NULL_PTR);
                }
            }
            else { 
                /* If either go_name or person_name didn't allow
                 * any access, return not_found.
                 */
                if(STATUS_EQUAL(&st1, sec_rgy_object_not_found) ||
                   STATUS_EQUAL(&st2, sec_rgy_object_not_found)) {
                    SET_STATUS(status, sec_rgy_object_not_found);
                } else {
                    SET_STATUS(status, sec_rgy_not_authorized);
                }
            }
        } END_MASTER_RIW
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                           aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_denial, &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                               aud_c_esl_cond_failure, &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    rs_util_get_cache_data(cache_info);
}


/*
 * r s _ p g o _ d e l e t e _ m e m b e r
 *
 * Delete the member person_name from the group or org, go_name.
 *
 * To delete person_name from go_name, the client must have
 * Member_list (M) permission on go_name.
 *
 * If person_name is a global name and name_domain is group,
 * when this global name is no longer a member of any group in this
 * cell, the associated pgo created in doman foreign_person 
 * then is removed in rsdb_pgo_delete_member() call.
 * 
 */
PUBLIC void rs_pgo_delete_member
  (
    handle_t          h,             /* [in] */
    sec_rgy_domain_t  name_domain,   /* [in] */
    sec_rgy_name_t    go_name,       /* [in] */
    sec_rgy_name_t    person_name,   /* [in] */
    rs_cache_data_t   *cache_info,   /* [out] */
    error_status_t    *(status)
  )
{
    char                    buf[128];
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = (sec_acl_permset_t) NULL;
    uuid_t                  *mgr_type;         
    sec_rgy_foreign_id_t    client_ids;                   

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    dce_aud_ev_info_t         ev_info;
    error_status_t            aud_status;
    error_status_t            aud_status2;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);
    mgr_type = &(rsdb_acl_mgr_uuids[GET_MGR_BY_DOMAIN(name_domain)]);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(PGO_DeleteMember, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_long_int;
        ev_info.data.long_int = name_domain;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = person_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = go_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* group/org must provide Member_list permission. */
    requested_perms = sec_acl_perm_Member_list;

    if (!check_go_domain(name_domain)) {
        SET_STATUS(status, sec_rgy_bad_domain);
    } else if (!CHECK_PGO_NAME(go_name) || !CHECK_PGO_NAME(person_name)
            || !query_is_dcepcs(go_name) || !query_is_dcepcs(person_name)) {
        SET_STATUS(status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_pgo_delete_member", status);
    }

    if (STATUS_OK(status)) {
 	if (!rsdb_name_util_is_foreign_pgo(person_name) ) {
            rsdb_name_util_localize_pgo(person_name, status);
            if (STATUS_OK(status)) {
                rsdb_name_canonicalize_and_convert(person_name, status);
            }
        } else { /* is a global name for group membership */
	    /* 
	     * since once software version set forward, cannot
	     * be set back, use this check to save future calls.
	     */
	    if ( ! support_global_group ) {
		/* 
		 * since once software version set forward, cannot
		 * be set back, use this check to save future calls.
		 */
            	support_global_group = rs_sw_feature_test_support
			(rs_c_sw_feature_global_group, status);
	    }
	    
	    if (STATUS_OK(status)) {
	    	if ( name_domain != sec_rgy_domain_group ) {
	            SET_STATUS(status, sec_rgy_bad_domain);
	    	}
	    }
	}
    }

    if (STATUS_OK(status)) {
        BEGIN_MASTER_RIW(lock_db, status) {
            sprintf(buf, "Attempt PGO del member (%s/%s)",
                    domain_names[name_domain],
                    go_name);
            rs_audit_trail_log(h, buf, (char *) person_name);

            if(rs_auth_is_authorized(h, requested_perms,
                        parent_perms, name_domain, go_name, mgr_type,
                        false, &client_ids, status)) {
                RIW_PROMOTE(lock_db);
                rsdb_pgo_delete_member(name_domain, go_name, person_name, 
                                       status);

                if (STATUS_OK(status)) {
                    sprintf(buf, "PGO delete member (%s/%s)",
                            domain_names[name_domain], go_name);
                    rs_audit_trail_log(h, buf, (char *) person_name);
                    rs_log_pgo_delete_member(name_domain, go_name,
                                             person_name, NULL_PTR);
                }
            } 
        } END_MASTER_RIW
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                1, aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard,
                    aud_c_evt_commit_sync_no_wait, 1, aud_c_esl_cond_denial,
                    &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard,
                    aud_c_evt_commit_sync_no_wait, 1, aud_c_esl_cond_failure,
                    &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    rs_util_get_cache_data(cache_info);
}


/*
 * r s _ p g o _ g e t _ m e m b e r s
 *
 * If domain is group or org, list the members of the group or org, go_name.
 * If domain is person, list the groups in which person is a member.
 * Note: comments in the code generally assume domain is group or org;
 * although the operation works for domain person as well.
 *
 * To list members of go_name, the client must have
 * read (r) rights on go_name. 
 *
 */
PUBLIC void rs_pgo_get_members
  (
    handle_t           h,                   /* [in] */
    sec_rgy_domain_t   name_domain,         /* [in] */
    sec_rgy_name_t     go_name,             /* [in] */
    sec_rgy_cursor_t   *(member_cursor),    /* [in, out] */
    signed32           max_members,         /* [in] */
    sec_rgy_member_t   member_list[],       /* [out] */
    signed32           *(number_supplied),  /* [out] */
    signed32           *(number_members),   /* [out] */
    rs_cache_data_t    *(cache_info),       /* [out] */
    error_status_t     *(status)
  )
{
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = (sec_acl_permset_t) NULL;
    uuid_t                  *mgr_type;         
    sec_rgy_foreign_id_t    client_ids;
    int                     i, j;
    error_status_t          tmp_status;
    sec_rgy_domain_t        member_domain;

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    dce_aud_ev_info_t         ev_info;
    error_status_t            aud_status;
    error_status_t            aud_status2;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);
    *number_supplied = 0;
    mgr_type = &(rsdb_acl_mgr_uuids[GET_MGR_BY_DOMAIN(name_domain)]);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(PGO_GetMembers, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_long_int;
        ev_info.data.long_int = name_domain;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = go_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* group/org must provide read permission. */
    requested_perms = sec_acl_perm_read;

    if (!check_domain(name_domain) || !CHECK_PGO_NAME(go_name)
            || !query_is_dcepcs(go_name)) { 
        SET_STATUS(status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_pgo_get_members", status);
    }

    if (STATUS_OK(status)) {
        if (name_domain == sec_rgy_domain_person) {
            rsdb_name_canonicalize_and_convert(go_name, status);
        } else {
            rsdb_name_canonicalize_components(go_name);
        }
    }

    if (STATUS_OK(status)) {
        rs_util_check_cursor(member_cursor);

        BEGIN_READ(lock_db, status) {
            if(rs_auth_is_authorized(h, requested_perms,
                        parent_perms, name_domain, go_name, mgr_type,
                        false, &client_ids, status)) {
                rsdb_pgo_get_members(false, name_domain, go_name, 
                        member_cursor, max_members, member_list,
                        number_supplied, number_members, status); 
            }
        } END_READ
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                1, aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard,
                    aud_c_evt_commit_sync_no_wait, 1, aud_c_esl_cond_denial,
                    &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard,
                    aud_c_evt_commit_sync_no_wait, 1, aud_c_esl_cond_failure,
                    &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    rs_util_get_cache_data(cache_info);
}

/*
 * r s _ p g o _ i s _ m e m b e r
 *
 * Test whether person_name is a member of the group/org go_name.
 *
 * Client must have test permission on go_name.
 */
PUBLIC boolean32 rs_pgo_is_member
  (
    handle_t          h,              /* [in] */
    sec_rgy_domain_t  name_domain,    /* [in] */
    sec_rgy_name_t    go_name,        /* [in] */
    sec_rgy_name_t    person_name,    /* [in] */
    rs_cache_data_t   *(cache_info),  /* [out] */
    error_status_t    *(status)
  )
{
    boolean32               is_member = false;
    sec_acl_permset_t       requested_perms;
    sec_acl_permset_t       parent_perms = (sec_acl_permset_t) NULL;
    uuid_t                  *mgr_type;         
    sec_rgy_foreign_id_t    client_ids;                   

#if defined(DCE_AUD_POINTS)
    dce_aud_rec_t             ard;
    dce_aud_ev_info_t         ev_info;
    error_status_t            aud_status;
    error_status_t            aud_status2;
#endif	/* defined(DCE_AUD_POINTS) */

    CLEAR_STATUS(status);
    mgr_type = &(rsdb_acl_mgr_uuids[GET_MGR_BY_DOMAIN(name_domain)]);

#if defined(DCE_AUD_POINTS)
    dce_aud_start(PGO_IsMember, h, 0, aud_c_esl_cond_unknown, 
                  &ard, &aud_status);
    if (ard) {
        ev_info.format = aud_c_evt_info_long_int;
        ev_info.data.long_int = name_domain;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = person_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);

        ev_info.format = aud_c_evt_info_char_string;
        ev_info.data.char_string = go_name;
        dce_aud_put_ev_info(ard, ev_info, &aud_status);
    }
#endif	/* defined(DCE_AUD_POINTS) */

    /* group/org must provide read permission. */
    requested_perms = sec_acl_perm_test;

    if (!check_domain(name_domain) || !CHECK_PGO_NAME(go_name) 
        || !CHECK_PGO_NAME(person_name) || !query_is_dcepcs(go_name)
        || !query_is_dcepcs(person_name)) {
        SET_STATUS(status, sec_rgy_bad_data);
        rs_util_bad_client(h, "rs_pgo_is_member", status);
    }

    if (STATUS_OK(status)) {
 	if (!rsdb_name_util_is_foreign_pgo(person_name) ) {
            rsdb_name_util_localize_pgo(person_name, status);
            if (STATUS_OK(status)) {
                rsdb_name_canonicalize_and_convert(person_name, status);
            }
        } else { /* is a global name for group membership */
	    if ( ! support_global_group ) {
		/* 
		 * since once software version set forward, cannot
		 * be set back, use this check to save future calls.
		 */
            	support_global_group = rs_sw_feature_test_support
			(rs_c_sw_feature_global_group, status);
	    }

	    if (STATUS_OK(status)) {
	    	if ( name_domain != sec_rgy_domain_group ) {
	            SET_STATUS(status, sec_rgy_bad_domain);
	    	}
	    }

	}
    }

    if (STATUS_OK(status)) {
        BEGIN_READ(lock_db, status) {
            if(rs_auth_is_authorized(h, requested_perms,
                        parent_perms, name_domain, go_name, mgr_type,
                        false, &client_ids, status)) {
                is_member = rsdb_pgo_is_member(name_domain, go_name, 
                                               person_name, status);
            }
        } END_READ
    }

#if defined(DCE_AUD_POINTS)
    if (ard) {    
        if (GOOD_STATUS(status)) {
            dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait,
                1, aud_c_esl_cond_success, &aud_status); 
        } else {  
            if (rs_util_status_is_denial(*status)) {
                dce_aud_commit(sec_aud_trail, ard,
                    aud_c_evt_commit_sync_no_wait, 1, aud_c_esl_cond_denial,
                    &aud_status);
            } else {
                dce_aud_commit(sec_aud_trail, ard,
                    aud_c_evt_commit_sync_no_wait, 1, aud_c_esl_cond_failure,
                    &aud_status);
            } 
        }
    }
#endif	/* defined(DCE_AUD_POINTS) */

    rs_util_get_cache_data(cache_info);

    return is_member;
}
