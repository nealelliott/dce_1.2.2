/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * (c) Copyright International Business Machines Corp. 1994, 1996
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 */
/*
 * HISTORY
 * $Log: dmn_rpc_manager.c,v $
 * Revision 1.1.9.1  1996/07/08  18:25:50  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:24 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  00:06:06  marty  1.1.6.2]
 *
 * Revision 1.1.6.2  1996/02/18  00:06:06  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:38  marty]
 * 
 * Revision 1.1.6.1  1995/12/08  17:38:26  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/08/18  19:53 UTC  kline_s
 * 	Merge fix for CHFts16115
 * 
 * 	HP revision /main/HPDCE02/3  1995/08/18  19:52 UTC  kline_s
 * 
 * 	HP revision /main/HPDCE02/kline_s_mothra/2  1995/08/17  21:27 UTC  kline_s
 * 	CHFts16115: Remove DES_HIDDEN conditional - only using integrity
 * 	protection level instead of using the privacy protection level
 * 	for Domestic.  Required to avoid conflicts due to singular
 * 	Domestic/Export release.
 * 
 * 	HP revision /main/HPDCE02/kline_s_mothra/1  1995/07/06  19:53 UTC  kline_s
 * 	Fix international release audit test problems in using rpc_c_authn_level_pkt_privacy
 * 	as the level of protection for the rpc_binding_set/_auth_info() and
 * 	rpc_binding_inq_auth_client() calls.  This uses DES encryption, which is not supported
 * 	in the international version.
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/08  23:18 UTC  jrr
 * 	Fix memory leak by using correct alloc/free for IDL es.
 * 	[1995/06/08  23:16 UTC  jrr  /main/jrr_sec_chfts15431/1]
 * 
 * Revision 1.1.2.18  1994/09/29  21:33:03  weisz
 * 	CR 12304: check for disabled state if in selfaudit mode.
 * 	[1994/09/29  21:32:30  weisz]
 * 
 * Revision 1.1.2.17  1994/09/26  17:10:41  weisz
 * 	CR 9834: fill in audit record header server field.
 * 	[1994/09/26  17:09:47  weisz]
 * 
 * Revision 1.1.2.16  1994/09/22  21:08:59  weisz
 * 	CR 10703: handling of trail full situation.
 * 	[1994/09/22  21:07:18  weisz]
 * 
 * Revision 1.1.2.15  1994/09/19  21:46:56  weisz
 * 	CR 12223: make client permissions checking consistent.
 * 	[1994/09/19  21:45:19  weisz]
 * 
 * Revision 1.1.2.14  1994/08/08  18:52:20  weisz
 * 	CR 10516: remove parameters from messages going accross rpcs as return codes.
 * 	[1994/08/08  18:51:06  weisz]
 * 
 * Revision 1.1.2.13  1994/07/27  19:16:19  weisz
 * 	CR 11372: added status parameter to audit_pickle_dencode_ev_info.
 * 	[1994/07/27  19:15:49  weisz]
 * 
 * Revision 1.1.2.12  1994/06/08  18:36:53  weisz
 * 	CR 10809: changed free(ep) to rpc_sm_client_free.
 * 	[1994/06/08  18:20:48  weisz]
 * 
 * Revision 1.1.2.11  1994/06/06  17:43:50  weisz
 * 	CR 10829: fix some memory leaks.
 * 	[1994/06/06  15:34:54  weisz]
 * 
 * Revision 1.1.2.10  1994/05/28  17:18:31  weisz
 * 	CR 10416: removed unregister_update_interface from audit_control_stop
 * 	[1994/05/28  17:15:14  weisz]
 * 
 * Revision 1.1.2.9  1994/05/10  15:14:32  weisz
 * 	CR 10147: relay back to the caller permission denial return code.
 * 	[1994/05/10  15:09:19  weisz]
 * 
 * Revision 1.1.2.8  1994/04/11  19:23:32  weisz
 * 	CR 10322: add unregister update interface
 * 	[1994/04/11  19:20:44  weisz]
 * 
 * Revision 1.1.2.7  1994/04/08  14:21:37  weisz
 * 	CR 10314 - audit_control_stop:
 * 	           check for NULL trail pointer before calling dce_aud_close
 * 	[1994/04/08  14:18:59  weisz]
 * 
 * Revision 1.1.2.6  1994/03/11  22:48:28  luan
 * 	CR 9775 9776: Changed debug messages to use dce_svc_printf() or DCE_SVC_DEBUG.
 * 	[1994/03/11  22:37:30  luan]
 * 
 * Revision 1.1.2.5  1994/02/23  21:22:02  luan
 * 	CR 9991: Initialized ard's to NULL.
 * 	[1994/02/22  20:21:40  luan]
 * 
 * Revision 1.1.2.4  1994/02/18  21:18:45  luan
 * 	CR 9777: Consolidated include files.
 * 	[1994/02/18  21:18:05  luan]
 * 
 * Revision 1.1.2.3  1994/02/18  19:11:55  luan
 * 	CR 9775: Removed an extra free() call in audit_log_append().
 * 	[1994/02/18  19:11:02  luan]
 * 
 * Revision 1.1.2.2  1994/01/23  20:37:51  luan
 * 	OT# 9796: Changed to allow multiple-guide audfilter add
 * 	and audfilter remove operations.
 * 	[1994/01/23  20:13:06  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:38:54  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:11  zeliff]
 * 
 * $EndLog$
 */

/*
** dmn_rpc_manager.c
** RPC manager routines of audit daemon's control and logging interfaces.
**/

#include <stdio.h>

#include <dce/dce.h>
#include <dce/dce_svc.h>
#include <dce/dce_msg.h>
#include <dce/id_base.h>
#include <dce/secidmap.h>
#include <dce/rpc.h>
#include <dce/idl_es.h>
#include <dce/pgo.h>
#include <dce/sec_cred.h>

#include <dce/audit.h>
#include <dce/dceaudmsg.h>
#include <dce/dceaudmac.h>
#include <dce/dceaudsvc.h>

#include <eslp.h>
#include <evtp.h>
#include <error_statusp.h>
#include <dmn_aud_evtsp.h>
#include <esl_filep.h>
#include <audit_alloc.h>

extern boolean32          debug_mode;

extern void             **aud_g_dmn_esl[];
extern aud_esl_guides_p_t aud_g_dmn_world_guides;
extern aud_esl_guides_p_t aud_g_dmn_world_overridable_guides;

extern void              *aud_esl_entry_lookup();
extern int                aud_esl_save_state();
extern void               aud_esl_output_update_entry();
extern dce_aud_trail_t    central_trail;
extern boolean32          audit;

unsigned32                aud_g_dmn_state = aud_c_dmn_state_enabled;
pthread_mutex_t           aud_g_dmn_esl_mutex;
pthread_mutex_t           aud_g_dmn_state_mutex;
pthread_mutex_t           aud_g_dmn_trail_mutex;

extern void               aud_update_announce(aud_esl_type_t);

int                       rpc_count = 0;
#ifdef DCE_DEBUG
#define OUTPUT_RPC_COUNT rpc_count++; \
dce_svc_printf(AUD_S_RPC_COUNT_MSG, rpc_count);
#else 
#define OUTPUT_RPC_COUNT
#endif

/*
 * For international (ie. non-DES) releases, check for a less strict,
 * non-encrypted protection level in rpc_binding_inq_auth_client():
 */

#define CHECK_AUDIT_SECURITY(event) \
   rpc_binding_inq_auth_client(h, &privs, NULL, &authn_level, \
                               &authn_svc, &authz_svc, stp); \
   if (GOOD_STATUS(stp)) { \
      if ((authn_level != rpc_c_protect_level_pkt_integ) || \
           (authz_svc != rpc_c_authz_dce)) { \
         dce_svc_printf(AUD_S_WRONG_PROTECTION_LEVEL_MSG); \
         *stp = aud_s_wrong_protection_level; \
         return; \
      } \
      if (audit) { \
         dce_aud_start(event, h, aud_c_evt_all_info, \
                       aud_c_esl_cond_unknown, &ard, stp); \
         if (BAD_STATUS(stp)) return; \
      } \
   } else { \
        dce_svc_printf(AUD_S_CANNOT_AUTHENTICATE_MSG); \
        return; \
   }

#define CHECK_AUTHORIZATION(func) \
    func(h, &authz_ok, stp); \
    if (BAD_STATUS(stp)) { \
        if (ard != NULL) { \
            dce_aud_commit(central_trail, ard, 0 /* options */ , \
                (unsigned16) 0 /* format */ , aud_c_esl_cond_failure, &lstp); \
            if (BAD_STATUS(&lstp)) return; \
        } \
        return; \
    } \
    if (!authz_ok) { \
        if (ard != NULL) { \
            dce_aud_commit(central_trail, ard, 0 /* options */ , \
                (unsigned16) 0 /* format */ , aud_c_esl_cond_denial, stp); \
            if (BAD_STATUS(stp)) return; \
        } \
        dce_svc_printf(AUD_S_PERMISSION_DENIED_MSG); \
        *stp = aud_s_permission_denied; \
        return; \
    }

#define CHECK_SELFAUDIT_STATE  \
        if ( aud_g_dmn_state == aud_c_dmn_state_disabled) {  \
           dce_aud_discard(ard, &lstp);  \
           *stp = aud_s_dmn_disabled_in_selfaudit;  \
           return;  \
        }

static void
                aud__esl_get_subject_ids
                (
		                 sec_rgy_name_t subject_name,
		                 aud_esl_type_t esl_type,
		                 uuid_t * subject_uuid,
		                 uuid_t * cell_uuid,
		                 error_status_t * stp
) {
    error_status_t   st;
    sec_rgy_handle_t rgy_context;
    sec_rgy_name_t   princ_name, cell_name;
    uuid_t           princ_uuid;

    uuid_create_nil(cell_uuid, stp);
    uuid_create_nil(subject_uuid, stp);
    sec_rgy_site_open(NULL, &rgy_context, stp);
    if (BAD_STATUS(stp)) {
	return;
    }
    switch (esl_type) {
    case aud_e_esl_princ:
	sec_rgy_pgo_name_to_id(rgy_context, sec_rgy_domain_person,
			       subject_name, subject_uuid, stp);
	if (BAD_STATUS(stp)) {
            sec_rgy_site_close(rgy_context, &st);
	    *stp = aud_s_unknown_principal_rc;
	    dce_svc_printf(AUD_S_UNKNOWN_PRINCIPAL_MSG, subject_name);
	    return;
	}
	break;

    case aud_e_esl_foreign_princ:
	sec_id_parse_name(rgy_context, subject_name, cell_name, cell_uuid,
			  princ_name, subject_uuid, stp);
	if (BAD_STATUS(stp)) {
            sec_rgy_site_close(rgy_context, &st);
	    *stp = aud_s_unknown_foreign_principal_rc;
	    dce_svc_printf(AUD_S_UNKNOWN_FOREIGN_PRINCIPAL_MSG, subject_name);
	    return;
	}
	break;

    case aud_e_esl_group:
	sec_rgy_pgo_name_to_id(rgy_context, sec_rgy_domain_group,
			       subject_name, subject_uuid, stp);
	if (BAD_STATUS(stp)) {
            sec_rgy_site_close(rgy_context, &st);
	    *stp = aud_s_unknown_group_rc;
	    dce_svc_printf(AUD_S_UNKNOWN_GROUP_MSG, subject_name);
	    return;
	}
	break;

    case aud_e_esl_foreign_group:
	sec_id_parse_group(rgy_context, subject_name, cell_name, cell_uuid,
			   princ_name, subject_uuid, stp);
	if (BAD_STATUS(stp)) {
            sec_rgy_site_close(rgy_context, &st);
	    *stp = aud_s_unknown_foreign_group_rc;
	    dce_svc_printf(AUD_S_UNKNOWN_FOREIGN_GROUP_MSG, subject_name);
	    return;
	}
	break;

    case aud_e_esl_cell:
    case aud_e_esl_cell_overridable:
	sec_id_parse_name(rgy_context, subject_name, cell_name, subject_uuid,
			  princ_name, &princ_uuid, stp);
	if (BAD_STATUS(stp)) {
            sec_rgy_site_close(rgy_context, &st);
	    *stp = aud_s_unknown_cell_rc;
	    dce_svc_printf(AUD_S_UNKNOWN_CELL_MSG, subject_name);
	    return;
	}
	break;

    default:
        sec_rgy_site_close(rgy_context, &st);
	*stp = aud_s_invalid_esl_type_rc;
	dce_svc_printf(AUD_S_INVALID_ESL_TYPE_MSG, esl_type);
	return;
	break;
    }
}

static          boolean32
                aud__esl_validate_guides
                (
		                 aud_esl_guides_p_t guides
) {
    aud_esl_guides_p_t guide;

    if (guides == NULL) {
	return(FALSE);
    }

    guide = guides;

    while (guide != NULL) {
	if ((guide->audit_condition & ~aud_c_esl_cond_all) != 0
		     ||
	     guide->audit_condition == 0) {
	    dce_svc_printf(AUD_S_INVALID_CONDITION_BIT_SET_MSG, 
		guide->audit_condition, aud_c_esl_cond_all);
	    return (FALSE);
        }
        if ((guide->audit_action & ~aud_c_esl_act_all) != 0) {
	    dce_svc_printf(AUD_S_INVALID_ACTION_BIT_SET_MSG, 
			guide->audit_action, aud_c_esl_act_all);
	    return (FALSE);
	}
        if ((guide)->ec_list == NULL) {
	    dce_svc_printf(AUD_S_EMPTY_EVENT_CLASS_LIST_MSG);
	    return (FALSE);
        }
	guide = guide->next;
    }
    return (TRUE);
}

/*
 * a u d i t _ c o n t r o l _ s h o w _ f i l t e r 
 *
 * This audit daemon's manager routine allows clients to query the guides that
 * are set up for auditing on specific subject.  The subject can belong to a
 * foreign cell.  The remote caller of this routine must have the "r" 
 * permission on the audit daemon's ACL.
 *
 */

void
audit_control_show_filter(
			   /* [in] */ handle_t h,
			   /* [in] */ aud_esl_type_t esl_type,
			   /* [in] */ sec_rgy_name_t subject_name,
			   /* [out] */ aud_esl_guides_p_t * guides,
			   /* [out] */ error_status_t * stp
)
{
    int                     i;
    aud_esl_evt_classes_p_t ec_list;
    uuid_t                  cell_uuid, subject_uuid;
    unsigned16              probe;
    void                   *entry_ptr;
    rpc_authz_handle_t      privs;
    unsigned32              authn_level, authn_svc, authz_svc;
    dce_aud_ev_info_t       audit_evt_info;
    dce_aud_rec_t           ard = NULL;
    boolean32               authz_ok;
    error_status_t          lstp = 0;

    *guides = NULL;
    OUTPUT_RPC_COUNT;

    CLEAR_STATUS(stp);

    CHECK_AUDIT_SECURITY(EVT_SHOW_FILTER);

    CHECK_AUTHORIZATION(acl_test_read_access);

    if (ard) {
        CHECK_SELFAUDIT_STATE;
	audit_evt_info.format = aud_c_evt_info_long_int;
	audit_evt_info.data.long_int = esl_type;
	dce_aud_put_ev_info(ard, audit_evt_info, stp);
	if (!(WORLD_TYPE(esl_type))) {
	    audit_evt_info.format = aud_c_evt_info_char_string;
	    audit_evt_info.data.char_string = subject_name;
	    dce_aud_put_ev_info(ard, audit_evt_info, stp);
	}
	dce_aud_commit(central_trail, ard, 0 /* options */ , 
		(unsigned16) 0 /* format */ , aud_c_esl_cond_success, stp);
    }
    if (esl_type == aud_e_esl_world) {
	pthread_mutex_lock(&aud_g_dmn_esl_mutex);
	*guides = aud_g_dmn_world_guides;
	pthread_mutex_unlock(&aud_g_dmn_esl_mutex);
	return;
    }
    if (esl_type == aud_e_esl_world_overridable) {
	pthread_mutex_lock(&aud_g_dmn_esl_mutex);
	*guides = aud_g_dmn_world_overridable_guides;
	pthread_mutex_unlock(&aud_g_dmn_esl_mutex);
	return;
    }
    aud__esl_get_subject_ids(subject_name, esl_type, &subject_uuid,
				&cell_uuid, stp);

    if (BAD_STATUS(stp)) {
	return;
    }

    pthread_mutex_lock(&aud_g_dmn_esl_mutex);
    entry_ptr = aud_esl_entry_lookup(esl_type,
					aud_g_dmn_esl,
					subject_uuid,
					cell_uuid, &probe);
    if (entry_ptr != NULL) {
	if (FOREIGN_TYPE(esl_type)) {
	    (*guides) =
		((aud_esl_foreign_entry_p_t) entry_ptr)->guides;
	} else {
	    (*guides) =
		((aud_esl_entry_p_t) entry_ptr)->guides;
	}
    } else {
	(*guides) = NULL;
    }
    pthread_mutex_unlock(&aud_g_dmn_esl_mutex);
    return;
}

/*
 * a u d i t _ c o n t r o l _ l i s t _ f i l t e r 
 *
 * This audit daemon's manager routine allows clients to 
 * list all filters of a speficied type sequentially.  The "r"
 * permission is required for this operation.
 *
 */

void
audit_control_list_filter(
			   /* [in] */ handle_t h,
			   /* [in] */ aud_esl_type_t esl_type,
			   /* [in, out] */ unsigned16 * cursor,
			   /* [out] */ aud_esl_entry_p_t * first_entry,
	     /* [out] */ aud_esl_foreign_entry_p_t * first_foreign_entry,
			   /* [out] */ error_status_t * stp
)
{
    int                i;
    uuid_t             cell_uuid;
    uuid_t             subject_uuid;
    unsigned16         probe;
    void              *entry_ptr;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    dce_aud_rec_t      ard = NULL;
    boolean32          authz_ok;
    error_status_t     lstp = 0;

    *first_entry = NULL;
    *first_foreign_entry = NULL;

    CLEAR_STATUS(stp);

    if (esl_type >= aud_e_esl_world || *cursor >= HASH_TABLE_SIZE
	|| cursor == NULL) {
	return;
    }
    OUTPUT_RPC_COUNT;

    CHECK_AUDIT_SECURITY(EVT_LIST_FILTER);

    CHECK_AUTHORIZATION(acl_test_read_access);

    if (ard) {
        CHECK_SELFAUDIT_STATE;
    }
    pthread_mutex_lock(&aud_g_dmn_esl_mutex);
    while (*cursor < HASH_TABLE_SIZE) {
	if ((aud_g_dmn_esl[esl_type])[(*cursor)] != NULL) {
	    break;
	}
	(*cursor)++;
    }

    if (*cursor < HASH_TABLE_SIZE) {
	if (FOREIGN_TYPE(esl_type)) {
	    (*first_foreign_entry) = ((aud_esl_foreign_entry_p_t *)
			     aud_g_dmn_esl[esl_type])[*cursor];
	} else {
	    (*first_entry) = ((aud_esl_entry_p_t *) 
				aud_g_dmn_esl[esl_type])[*cursor];
	}
	(*cursor)++;
    }
    pthread_mutex_unlock(&aud_g_dmn_esl_mutex);
    if (ard) {
	dce_aud_commit(central_trail, ard, 0 /* options */ , 
		(unsigned16) 0 /* format */ , aud_c_esl_cond_success, stp);
    }
    return;
}

/*
 * a u d i t _ c o n t r o l _ a d d _ f i l t e r 
 *
 * This audit daemon's manager routine allows clients to add a filter for
 * a specified subject.  If a filter for the subject exists, then new
 * combinations of (event_class, audit_condition, audit_action) are added 
 * to the set of existing ones. 
 *
 * For example, adding {(query, success, {log, alarm}} to 
 * {query, {failure, success}, log}  is like adding
 *
 * (query, success, log)
 * (query, success, alarm)
 * 
 * to 
 *
 * (query, failure, log)
 * (query, success, log),
 *
 * and the result is 
 *
 * (query, success, log)
 * (query, success, alarm)
 * (query, failure, log).
 *
 * The remote caller of this routine must have the "w" permission.
 */

void
audit_control_add_filter(
			  /* [in] */ handle_t h,
			  /* [in] */ aud_esl_type_t esl_type,
			  /* [in] */ sec_rgy_name_t subject_name,
			  /* [in] */ aud_esl_guides_p_t * guides,
			  /* [out] */ error_status_t * stp
)
{
    uuid_t                  subject_uuid;
    uuid_t                  cell_uuid;
    aud_esl_evt_classes_p_t ec_ptr;
    unsigned32              evt_class;
    aud_esl_guides_p_t      new_guides;
    char                   *char_p;
    dce_aud_rec_t           ard = NULL;
    rpc_authz_handle_t      privs;
    unsigned32              authn_level, authn_svc, authz_svc;
    boolean32               authz_ok;
    error_status_t          lstp = 0;

    OUTPUT_RPC_COUNT;
    CLEAR_STATUS(stp);

    CHECK_AUDIT_SECURITY(EVT_ADD_FILTER);

    CHECK_AUTHORIZATION(acl_test_write_access);

    if (ard) {
        CHECK_SELFAUDIT_STATE;
    }
    if (aud__esl_validate_guides(*guides) == FALSE) {
	dce_svc_printf(AUD_S_INVALID_GUIDE_MSG);
        *stp = aud_s_invalid_guide;
	return;
    }

    aud_esl_copy_guides(*guides, &new_guides, stp);
    /* get rid of replicated classes */
    aud_esl_compact_ec_list(new_guides);

    if (WORLD_TYPE(esl_type)) {
        pthread_mutex_lock(&aud_g_dmn_esl_mutex);
	if (esl_type == aud_e_esl_world) {
	    aud_esl_update_guides(&aud_g_dmn_world_guides, new_guides);
	    if (debug_mode) {
		printf("filter is added \n");
	        aud_esl_list_guides(aud_g_dmn_world_guides);
	    }
	}
	if (esl_type == aud_e_esl_world_overridable) {
	    aud_esl_update_guides(&aud_g_dmn_world_overridable_guides, new_guides);
	    if (debug_mode) {
		printf("filter is added \n");
	        aud_esl_list_guides(aud_g_dmn_world_overridable_guides);
	    }
	}
	aud_esl_save_state(esl_type);
	aud_update_announce(esl_type);
        pthread_mutex_unlock(&aud_g_dmn_esl_mutex);
	if (ard) {
	    dce_aud_commit(central_trail, ard, 0 /* options */ , 
		(unsigned16) 0 /* format */ , aud_c_esl_cond_success, stp);
	}
	return;
    }
    aud__esl_get_subject_ids(subject_name, esl_type, &subject_uuid,
				&cell_uuid, stp);
    if (BAD_STATUS(stp)) {
	return;
    }
    pthread_mutex_lock(&aud_g_dmn_esl_mutex);
    aud_esl_output_update_entry(ESL_UPDATE_ADD_FILTER, esl_type,
				   subject_uuid, cell_uuid, new_guides);
    aud_esl_entry_add_guides(aud_g_dmn_esl, esl_type,
				subject_uuid, cell_uuid,
				&new_guides, stp);
    if (debug_mode) { 
	printf("filter is added\n");
    	aud_esl_entry_list_guides(aud_g_dmn_esl, esl_type,
				 subject_uuid, cell_uuid);
    }
    pthread_mutex_unlock(&aud_g_dmn_esl_mutex);
    aud_update_announce(esl_type);
    if (ard) {
	dce_aud_commit(central_trail, ard, 0 /* options */ , 
		(unsigned16) 0 /* format */ , aud_c_esl_cond_success, stp);
    }
}

/*
 * a u d i t _ c o n t r o l _ r e m o v e _ f i l t e r 
 *
 * This audit daemon's manager routine allows clients to remove a filter for
 * a specified subject.  This operation has no effect if the subject does not
 * have an existing filter.  If a filter exists for the subject but is not
 * identical to the filter to be removed, then only the matching combinations 
 * of (event_class, audit_condition, audit_action) are removed.
 * For example, removing {(query, success, {log, alarm}} from 
 * {query, {failure, success}, log}  is like removing
 *
 * (query, success, log)
 * (query, success, alarm)
 * 
 * from 
 *
 * (query, failure, log)
 * (query, success, log)
 *
 * and the result is 
 *
 * (query, success, alarm)
 *
 *
 * The remote caller of this routine must have the "w" permission.
 */

void
audit_control_remove_filter(
			     /* [in] */ handle_t h,
			     /* [in] */ aud_esl_type_t esl_type,
			     /* [in] */ sec_rgy_name_t subject_name,
			     /* [in] */ aud_esl_guides_p_t * guides,
			     /* [out] */ error_status_t * stp
)
{
    uuid_t             subject_uuid, cell_uuid;
    dce_aud_rec_t      ard = NULL;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    boolean32          authz_ok;
    error_status_t     lstp = 0;
    CLEAR_STATUS(stp);

    OUTPUT_RPC_COUNT;

    CHECK_AUDIT_SECURITY(EVT_REMOVE_FILTER);

    CHECK_AUTHORIZATION(acl_test_write_access);

    if (ard) {
        CHECK_SELFAUDIT_STATE;
    }
    if (aud__esl_validate_guides(*guides) == FALSE) {
	dce_svc_printf(AUD_S_INVALID_GUIDE_MSG);
        *stp = aud_s_invalid_guide;
	return;
    }

    if (WORLD_TYPE(esl_type)) {
        pthread_mutex_lock(&aud_g_dmn_esl_mutex);
	if (esl_type == aud_e_esl_world) {
	    aud_esl_remove_guides(&aud_g_dmn_world_guides, *guides);
	    if (debug_mode) {
		printf("filter is removed\n");
	        aud_esl_list_guides(aud_g_dmn_world_guides);
	    }
	}
	if (esl_type == aud_e_esl_world_overridable) {
	    aud_esl_remove_guides(&aud_g_dmn_world_overridable_guides,
				     *guides);
	    if (debug_mode) {
		printf("filter is removed\n");
	        aud_esl_list_guides(aud_g_dmn_world_overridable_guides);
	    }
	}
	aud_esl_save_state(esl_type);
        pthread_mutex_unlock(&aud_g_dmn_esl_mutex);
	aud_update_announce(esl_type);
	if (ard) {
	    dce_aud_commit(central_trail, ard, 0 /* options */ , 
		(unsigned16) 0 /* format */ , aud_c_esl_cond_success, stp);
	    if (BAD_STATUS(stp)) {
		dce_svc_printf(AUD_S_CANNOT_AUDIT_MSG);
	    }
	}
	return;
    }
    aud__esl_get_subject_ids(subject_name, esl_type, &subject_uuid,
				&cell_uuid, stp);
    if (BAD_STATUS(stp) ||
	(uuid_is_nil(&subject_uuid, stp) &&
	 uuid_is_nil(&cell_uuid, stp))
	) {
	return;
    }
    pthread_mutex_lock(&aud_g_dmn_esl_mutex);
    aud_esl_entry_remove_guides(aud_g_dmn_esl, esl_type,
				   subject_uuid, cell_uuid, *guides);
    if (debug_mode) {
	printf("filter is removed\n");
        aud_esl_entry_list_guides(aud_g_dmn_esl, esl_type,
				 subject_uuid, cell_uuid);
    }

    aud_esl_output_update_entry(ESL_UPDATE_REMOVE_FILTER, esl_type,
				   subject_uuid, cell_uuid, *guides);
    pthread_mutex_unlock(&aud_g_dmn_esl_mutex);
    aud_update_announce(esl_type);
    if (ard) {
	dce_aud_commit(central_trail, ard, 0 /* options */ , 
		(unsigned16) 0 /* format */ , aud_c_esl_cond_success, stp);
    }
}

void
audit_control_delete_filter(
			     /* [in] */ handle_t h,
			     /* [in] */ aud_esl_type_t esl_type,
			     /* [in] */ sec_rgy_name_t subject_name,
			     /* [out] */ error_status_t * stp
)
{
    uuid_t             subject_uuid, cell_uuid;
    void              *entry_ptr;
    unsigned16         probe;
    dce_aud_rec_t      ard = NULL;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    boolean32          authz_ok;
    aud_esl_guides_p_t guides_ptr = NULL;
    error_status_t     lstp = 0;
    CLEAR_STATUS(stp);

    OUTPUT_RPC_COUNT;

    CHECK_AUDIT_SECURITY(EVT_DELETE_FILTER);

    CHECK_AUTHORIZATION(acl_test_write_access);

    if (ard) {
        CHECK_SELFAUDIT_STATE;
    }
    if (WORLD_TYPE(esl_type)) {
        pthread_mutex_lock(&aud_g_dmn_esl_mutex);
	if (esl_type == aud_e_esl_world) {
	    aud_esl_free_guides(aud_g_dmn_world_guides);
	    aud_g_dmn_world_guides = NULL;
	}
	if (esl_type == aud_e_esl_world_overridable) {
	    aud_esl_free_guides(aud_g_dmn_world_overridable_guides);
	    aud_g_dmn_world_overridable_guides = NULL;
	}
	aud_esl_save_state(esl_type);
        pthread_mutex_unlock(&aud_g_dmn_esl_mutex);
	aud_update_announce(esl_type);
	if (ard) {
	    dce_aud_commit(central_trail, ard, 0 /* options */ , 
		(unsigned16) 0 /* format */ , aud_c_esl_cond_success, stp);
	}
	return;
    }
    aud__esl_get_subject_ids(subject_name, esl_type, &subject_uuid,
				&cell_uuid, stp);
    if (BAD_STATUS(stp) ||
	(uuid_is_nil(&subject_uuid, stp) &&
	 uuid_is_nil(&cell_uuid, stp))
	) {
	return;
    }
    pthread_mutex_lock(&aud_g_dmn_esl_mutex);
    aud_esl_entry_delete_filter(esl_type, aud_g_dmn_esl,
				   subject_uuid, cell_uuid);

    if (debug_mode) {
	printf("filter is deleted\n");
        aud_esl_entry_list_guides(aud_g_dmn_esl, esl_type,
				 subject_uuid, cell_uuid);
    }
    aud_esl_output_update_entry(ESL_UPDATE_DELETE_FILTER, esl_type,
				   subject_uuid, cell_uuid, NULL);
    pthread_mutex_unlock(&aud_g_dmn_esl_mutex);
    aud_update_announce(esl_type);
    if (ard) {
	dce_aud_commit(central_trail, ard, 0 /* options */ , 
		(unsigned16) 0 /* format */ , aud_c_esl_cond_success, stp);
    }
}

/*
 * a u d i t _ c o n t r o l _ m o d i f y _ s t o s t r a t e g y 
 */

void
audit_control_modify_sstrategy(
			        /* [in] */ handle_t h,
			        /* [in] */ unsigned32 strgy,
			        /* [out] */ error_status_t * stp
)
{
    boolean32          authz_ok;
    dce_aud_rec_t      ard = NULL;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    error_status_t     lstp = 0;

    OUTPUT_RPC_COUNT;

    CLEAR_STATUS(stp);

    CHECK_AUDIT_SECURITY(EVT_MODIFY_SSTRATEGY);

    CHECK_AUTHORIZATION(acl_test_control_access);

    if (ard) {
        CHECK_SELFAUDIT_STATE;
    }

    dce_aud_modify_sstrategy(central_trail, strgy, stp);

    if (ard) {
	dce_aud_commit(central_trail, ard, 0 /* options */ , 
		(unsigned16) 0 /* format */ , aud_c_esl_cond_success, stp);
    }
}

void
audit_control_show_sstrategy(
			      /* [in] */ handle_t h,
			      /* [in] */ unsigned32 * strgy,
			      /* [out] */ error_status_t * stp
)
{
    boolean32          authz_ok;
    dce_aud_rec_t      ard = NULL;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    error_status_t     lstp = 0;

    OUTPUT_RPC_COUNT;
    CLEAR_STATUS(stp);

    CHECK_AUDIT_SECURITY(EVT_SHOW_SSTRATEGY);

    CHECK_AUTHORIZATION(acl_test_read_access);

    if (ard) {
        CHECK_SELFAUDIT_STATE;
    }
    pthread_mutex_lock(&central_trail->mutex);
    *strgy = central_trail->flags & aud_c_trl_ss_bits;
    pthread_mutex_unlock(&central_trail->mutex);
    if (ard) {
	dce_aud_commit(central_trail, ard, 0 /* options */ , 
		(unsigned16) 0 /* format */ , aud_c_esl_cond_success, stp);
    }
}


/*
 * a u d i t _ c o n t r o l _ m o d i f y _ s t a t e 
 *
 * This manager routine allows the authorized administrators to notify audit
 * daemon to stop logging service. 
 *
 */

void
audit_control_modify_state(
			    /* [in] */ handle_t h,
			    /* [in] */ unsigned32 state,
			    /* [out] */ error_status_t * stp
)
{
    boolean32          authz_ok;
    dce_aud_rec_t      ard = NULL;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    error_status_t     lstp = 0;

    OUTPUT_RPC_COUNT;
    CLEAR_STATUS(stp);

    CHECK_AUDIT_SECURITY(EVT_MODIFY_STATE);

    CHECK_AUTHORIZATION(acl_test_control_access);

    if (ard) 
    {
       if ( aud_g_dmn_state == aud_c_dmn_state_disabled &&
            state == aud_c_dmn_state_enabled)
       {
          pthread_mutex_lock(&aud_g_dmn_state_mutex);
          aud_g_dmn_state = state;
          pthread_mutex_unlock(&aud_g_dmn_state_mutex);
          dce_aud_commit(central_trail, ard, 0 /* options */ , 
                      (unsigned16) 0 /* format */ , aud_c_esl_cond_success, stp);
       }

       if ( aud_g_dmn_state == aud_c_dmn_state_enabled &&
            state == aud_c_dmn_state_disabled)
       {
          dce_aud_commit(central_trail, ard, 0 /* options */ , 
                         (unsigned16) 0 /* format */ , aud_c_esl_cond_success, stp);
          pthread_mutex_lock(&aud_g_dmn_state_mutex);
          aud_g_dmn_state = state;
          pthread_mutex_unlock(&aud_g_dmn_state_mutex);
       }
    }
    else 
    {
          pthread_mutex_lock(&aud_g_dmn_state_mutex);
          aud_g_dmn_state = state;
          pthread_mutex_unlock(&aud_g_dmn_state_mutex);
    }

    if (state == aud_c_dmn_state_disabled) {
	aud_trl_file_flush(central_trail);
    }
}

void
audit_control_show_state(
			  /* [in] */ handle_t h,
			  /* [in] */ unsigned32 * state,
			  /* [out] */ error_status_t * stp
)
{
    boolean32          authz_ok;
    dce_aud_rec_t      ard = NULL;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    error_status_t     lstp = 0;

    OUTPUT_RPC_COUNT;
    CLEAR_STATUS(stp);

    CHECK_AUDIT_SECURITY(EVT_SHOW_STATE);

    CHECK_AUTHORIZATION(acl_test_read_access);

    if (ard) {
        CHECK_SELFAUDIT_STATE;
    }
    pthread_mutex_lock(&aud_g_dmn_state_mutex);
    *state = aud_g_dmn_state;
    pthread_mutex_unlock(&aud_g_dmn_state_mutex);
    if (ard) {
	dce_aud_commit(central_trail, ard, 0 /* options */ , 
		(unsigned16) 0 /* format */ , aud_c_esl_cond_success, stp);
    }
}

void
audit_control_rewind(
		    /* [in] */ handle_t h,
		    /* [out] */ error_status_t * stp
)
{
    boolean32          authz_ok = FALSE;
    dce_aud_rec_t      ard = NULL;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    error_status_t     lstp = 0;

    OUTPUT_RPC_COUNT;
    CLEAR_STATUS(stp);

    CHECK_AUDIT_SECURITY(EVT_REWIND);

    CHECK_AUTHORIZATION(acl_test_control_access);

    dce_aud_rewind(central_trail, stp);
    if (ard) {
	dce_aud_commit(central_trail, ard, 0 /* options */ , 
		(unsigned16) 0 /* format */ , aud_c_esl_cond_success, stp);
    }
    return;
}

void
audit_control_stop(
		    /* [in] */ handle_t h,
		    /* [out] */ error_status_t * stp
)
{
    boolean32          authz_ok = FALSE;
    dce_aud_rec_t      ard = NULL;
    rpc_authz_handle_t privs;
    unsigned32         authn_level, authn_svc, authz_svc;
    error_status_t     lstp = 0;

    OUTPUT_RPC_COUNT;
    CLEAR_STATUS(stp);

    CHECK_AUDIT_SECURITY(EVT_STOP);

    CHECK_AUTHORIZATION(acl_test_control_access);

    if (ard) {
	dce_aud_commit(central_trail, ard, 0 /* options */ , 
		(unsigned16) 0 /* format */ , aud_c_esl_cond_success, stp);
    }

    if (central_trail != NULL)
    {
       dce_aud_close(central_trail, stp);
    }
    rpc_mgmt_stop_server_listening(NULL, stp);
}

void
audit_log_append(
		  /* [in] */ handle_t h,
		  /* [in] */ unsigned32 options,
		  /* [in] */ dce_aud_hdr_t * header_info,
		  /* [in] */ aud_log_ev_info_list_t ** tail_info,
		  /* [out] */ error_status_t * stp
)
{
    idl_es_handle_t             es_h;
    idl_byte                    *ep;
    idl_ulong_int               esize;
    boolean32                   authz_ok;
    unsigned32                  authn_level, authn_svc, authz_svc;
    error_status_t              lstp = 0;
    rpc_authz_cred_handle_t     callers_identity;
    sec_cred_pa_handle_t        pah;
    sec_id_pa_t                 *cred;
    boolean32                   enabled_allocator;

    CLEAR_STATUS(stp);

    rpc_binding_inq_auth_caller(h, &callers_identity, NULL, &authn_level,
				&authn_svc, &authz_svc, stp);
    if (GOOD_STATUS(stp)) {
	if ((authn_level == rpc_c_protect_level_pkt_integ) &
	    (authz_svc == rpc_c_authz_dce)) {
	} else {
	    dce_svc_printf(AUD_S_WRONG_PROTECTION_LEVEL_MSG);
	    *stp = aud_s_wrong_protection_level;
	}
    } else {
	dce_svc_printf(AUD_S_CANNOT_AUTHENTICATE_MSG);
	return;
    }

    if (aud_g_dmn_state == aud_c_dmn_state_disabled) {
	*stp = aud_s_dmn_disabled;
	return;
    }

    acl_test_log_access(h, &authz_ok, stp);

    if (BAD_STATUS(stp)) {
        return;
    }

    if (!authz_ok) {
        dce_svc_printf(AUD_S_LOG_ACCESS_DENIED_MSG);
        *stp = aud_s_log_access_denied;
        return;
    }

    pah = sec_cred_get_initiator(callers_identity, stp);
    cred = sec_cred_get_pa_data(pah, stp);

    header_info->server = cred->principal.uuid;

    idl_es_encode_dyn_buffer(&ep, &esize, &es_h, stp);

    AUD_SET_ALLOC_FREE(enabled_allocator);
    audit_pickle_dencode_ev_info(es_h, header_info, tail_info, stp);

    if (BAD_STATUS(stp)) {
        idl_es_handle_free(&es_h, stp);
        AUD_RESET_ALLOC_FREE(enabled_allocator, ep);
        return;
    }

#ifdef DCE_DEBUG
    DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
                   "pickle size = %ld\n", esize));
    DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
                   "pickle address = %ld\n", ep));
#endif
    aud_trl_file_write(central_trail, ep, esize, options, &lstp);
    AUD_RESET_ALLOC_FREE(enabled_allocator, ep);

    idl_es_handle_free(&es_h, stp);

    if (BAD_STATUS(stp)) {
	dce_svc_printf(AUD_S_FREE_ENCODING_SERVICE_HANDLE_MSG);
    }
    if (BAD_STATUS(&lstp)) {  
        *stp = lstp;  /* serviceability msg sent from aud_trl_file_write */
    }
}
