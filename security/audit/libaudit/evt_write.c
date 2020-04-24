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
 * $Log: evt_write.c,v $
 * Revision 1.1.9.2  1996/07/08  18:32:19  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:39 UTC  arvind  /main/arvind_audit122/2]
 *
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:26 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:48 UTC  hanfei  /main/DCE_1.2/2]
 *
 * 	Make sure that the pac is initialized properly in
 * 	dce_aud_start_with_uuid().
 * 	[1996/03/01  18:26 UTC  jrr  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:50  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/09/29  14:18 UTC  jrr  /main/HPDCE02/2]
 *
 * 	Recode the client_id loop correctly.
 * 	HP 	[1995/09/20  21:18 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/3]
 *
 * 	clean up compiler warnings.
 * 	delete unused local variable.
 * 	HP 	[1995/08/31  05:59 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/2]
 *
 * 	Rename functions.
 * 	HP 	[1995/08/14  15:24 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 *
 * 	Performance: let secidmap do the rca_site_bind iff needed.
 * 	HP 	[1995/06/08  23:21 UTC  jrr  /main/HPDCE02/1]
 * 	[1996/07/08  18:02:59  arvind]
 *
 * Revision 1.1.9.1  1996/05/10  13:13:11  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:48 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/03/01  18:26 UTC  jrr
 * 	Make sure that the pac is initialized properly in
 * 	dce_aud_start_with_uuid().
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:33 UTC  psn
 * 	Recode the client_id loop correctly.
 * 	[1995/09/29  14:18 UTC  jrr  /main/HPDCE02/2]
 * 
 * 	clean up compiler warnings.
 * 	delete unused local variable.
 * 	[1995/09/20  21:18 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/3]
 * 
 * 	Rename functions.
 * 	[1995/08/31  05:59 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/2]
 * 
 * 	Performance: let secidmap do the rca_site_bind iff needed.
 * 	[1995/08/14  15:24 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 * 
 * 	Fix memory leak by using correct alloc/free for IDL es.
 * 	[1995/06/08  23:21 UTC  jrr  /main/jrr_sec_chfts15431/1]
 * 
 * Revision 1.1.2.22  1994/10/05  13:42:24  weisz
 * 	CR 12493: check for NULL before using pointer.
 * 	[1994/10/05  13:41:58  weisz]
 * 
 * Revision 1.1.2.21  1994/09/26  17:10:46  weisz
 * 	CR 9834: fill in audit record header server field.
 * 	[1994/09/26  17:10:08  weisz]
 * 
 * Revision 1.1.2.20  1994/09/22  21:09:05  weisz
 * 	CR 10703: handling of trail full situation.
 * 	[1994/09/22  21:07:57  weisz]
 * 
 * Revision 1.1.2.19  1994/09/02  14:20:13  weisz
 * 	CR 11965: added more error checking in dce_aud_start_with_server_binding
 * 	[1994/09/02  14:19:38  weisz]
 * 
 * Revision 1.1.2.18  1994/08/30  17:21:37  weisz
 * 	CR 11884: added new API - dce_aud_start_with_uuid
 * 	[1994/08/30  17:18:01  weisz]
 * 
 * Revision 1.1.2.17  1994/08/17  19:56:55  weisz
 * 	CR 11443: added PENDING as a new valid outcome.
 * 	[1994/08/17  19:54:35  weisz]
 * 
 * Revision 1.1.2.16  1994/08/10  13:52:45  weisz
 * 	CR 11603: check for NULL before string operations.
 * 	[1994/08/10  13:52:19  weisz]
 * 
 * Revision 1.1.2.14  1994/08/08  18:52:29  weisz
 * 	CR 10516: remove parameters from messages going accross rpcs as return codes.
 * 	[1994/08/08  18:51:37  weisz]
 * 
 * Revision 1.1.2.13  1994/07/27  14:54:17  weisz
 * 	CR 11372: added status parameter to audit_pickle_dencode_ev_info.
 * 	[1994/07/27  14:53:48  weisz]
 * 
 * Revision 1.1.2.12  1994/06/08  18:36:56  weisz
 * 	CR 10809: changed free(ep) to rpc_sm_client_free.
 * 	[1994/06/08  18:23:21  weisz]
 * 
 * Revision 1.1.2.11  1994/06/06  17:43:54  weisz
 * 	CR 10829: fix some memory leaks.
 * 	[1994/06/06  15:41:37  weisz]
 * 
 * Revision 1.1.2.10  1994/06/02  22:02:12  mdf
 * 	Merged with changes from 1.1.2.9
 * 	[1994/06/02  22:01:29  mdf]
 * 
 * 	Merged with changes from 1.1.2.7
 * 	[1994/06/02  19:08:50  mdf]
 * 
 * 	Fix to prevent secd from dropping core.
 * 	[1994/05/26  15:44:30  mdf]
 * 
 * Revision 1.1.2.7  1994/05/29  19:24:06  weisz
 * 	CR 10735: in dce_aud_start call dce_aud_start_with_name with client_name
 * 	          instead of with v1_privs
 * 	[1994/05/29  19:20:58  weisz]
 * 
 * Revision 1.1.2.9  1994/06/02  20:20:20  mdf
 * 	Merged with changes from 1.1.2.7
 * 	[1994/06/02  19:08:50  mdf]
 * 
 * 	Fix to prevent secd from dropping core.
 * 	[1994/05/26  15:44:30  mdf]
 * 
 * Revision 1.1.2.7  1994/05/29  19:24:06  weisz
 * 	CR 10735: in dce_aud_start call dce_aud_start_with_name with client_name
 * 	          instead of with v1_privs
 * 	[1994/05/29  19:20:58  weisz]
 * 
 * Revision 1.1.2.6  1994/04/25  20:54:15  weisz
 * 	CR 10370: fixed
 * 	[1994/04/25  20:49:54  weisz]
 * 
 * Revision 1.1.2.5  1994/03/11  22:48:44  luan
 * 	CR 9775 9776: Changed debug messages to use dce_svc_printf() or DCE_SVC_DEBUG.
 * 	[1994/03/11  22:32:26  luan]
 * 
 * Revision 1.1.2.4  1994/03/02  16:06:19  luan
 * 	CR 10038: Added delegation information in audit record header.
 * 	[1994/03/02  16:05:44  luan]
 * 
 * Revision 1.1.2.3  1994/02/22  16:47:36  luan
 * 	CR 9775 9776 9860:  Defined DCE_AUD_DEBUG in audit.h.  Removed
 * 	debug_mode and PRE_BETA_DEBUG or replaced it with DCE_DEBUG.
 * 	[1994/02/22  16:42:55  luan]
 * 
 * Revision 1.1.2.2  1994/01/25  04:59:43  luan
 * 	dce_aud_commit()was not taking the "format"
 * 	parameter from the input.  Fixed it.
 * 	[1994/01/25  04:19:13  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:39:37  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:45  zeliff]
 * 
 * $EndLog$
 */

/*
** evt_write.c
** Event logging API routines.
*/

#include <evtp.h>
#include <audit_alloc.h>
#include <dce/sec_cred.h>

void aud__evt_add_id_to_header
(
sec_id_pac_t 	*pac,
unsigned32	options,
dce_aud_rec_t	ardp
)
{
    client_id_t *cidp;
    int		 id_count;
    int 	 i;

    id_count = ardp->header.num_client_ids + 1;
    cidp = (client_id_t *) malloc(sizeof(client_id_t) * id_count);
    if (id_count > 1) {
    	memcpy(cidp, ardp->header.client_id, 
		sizeof(client_id_t) * (id_count-1));
    }
    cidp[id_count-1].client = pac->principal.uuid;
    cidp[id_count-1].cell = pac->realm.uuid;
    if ((options & aud_c_evt_groups_info) != 0) {
	/* Default group and project list groups are all put here. */
	cidp[id_count-1].num_groups = pac->num_groups + 1;
	cidp[id_count-1].groups =
	    (uuid_t *) malloc(sizeof(uuid_t) * (pac->num_groups + 1));
	cidp[id_count-1].groups[0] = pac->group.uuid;
	for (i = 1; i < pac->num_groups + 1; i++) {
	    cidp[id_count-1].groups[i] = pac->groups[i - 1].uuid;
	}
    } else {
	cidp[id_count-1].num_groups = 0;
	cidp[id_count-1].groups = NULL;
    }
    ardp->header.client_id = cidp;
    ardp->header.num_client_ids++;
    return;
}


void            dce_aud_start_with_uuid
                (
                   unsigned32 event,
                   uuid_t     server_uuid,
                   uuid_t     client_uuid,
                   uuid_t     realm_uuid,
                   unsigned_char_t * address,
                   unsigned32 options,
                   unsigned32 outcome,
                   dce_aud_rec_t * ard,
                   unsigned32 * status
                 )
{
    aud_esl_act_t  *as = NULL;
    dce_aud_rec_t   ardp;
    sec_id_pac_t   *pac;
    int             i;

    *status = aud_s_ok;
    *ard = NULL;
    if (aud_g_cli_audit_on == FALSE)
    {
        return;
    }

    pac = (sec_id_pac_t *) malloc(sizeof(sec_id_pac_t));
    pac->principal.uuid = client_uuid;
    pac->realm.uuid = realm_uuid;
    uuid_create_nil(&(pac->group.uuid), status);
    pac->num_groups = 0;
    pac->num_foreign_groups = 0;
    pac->groups = NULL;
    pac->foreign_groups = NULL;

    dce_aud_start_with_pac(event, pac, address, options, outcome, ard, status);
    free(pac);
    return;
}

void            dce_aud_start
                (
		                 unsigned32 event,
		                 rpc_binding_handle_t binding,
		                 unsigned32 options,
		                 unsigned32 outcome,
		                 dce_aud_rec_t * ard,
		                 error_status_t * stp
) {
    rpc_authz_cred_handle_t	callers_identity;
    rpc_authz_handle_t 		v1_privs;
    sec_cred_pa_handle_t        pah;
    sec_cred_cursor_t		cursor;
    unsigned32      		protect_level, authn_svc, authz_svc;
    unsigned_char_t 		*addr;
    int             		i;
    error_status_t		lst;
 
    *stp = aud_s_ok;
    *ard = NULL;
 
    if (aud_g_cli_audit_on == FALSE) {
	return;
    }

    rpc_binding_inq_auth_caller(binding, &callers_identity, NULL,
				&protect_level, &authn_svc, &authz_svc, stp);
    if (*stp != rpc_s_ok) {
	return;
    }
 
 
    if ((options & aud_c_evt_address_info) != 0) {
	rpc_binding_to_string_binding(binding, &addr, stp);
	if (*stp != rpc_s_ok) {
	    return;
	}
    } else {
	addr = NULL;
    }
    if (authz_svc == rpc_c_authz_name) {
	unsigned_char_t *client_name;
	sec_cred_get_client_princ_name( callers_identity, &client_name, stp);
	if (GOOD_STATUS(stp)) {
            dce_aud_start_with_name(event, (unsigned_char_t *) client_name, addr,
				options, outcome, ard, stp);
            if (addr) rpc_string_free(&addr,stp);
	}
	return;
    }
 
    if (authz_svc == rpc_c_authz_dce) {
	pah = sec_cred_get_initiator(callers_identity, stp);
	if (*stp != aud_s_ok) {
	    return;
	}
	v1_privs = sec_cred_get_v1_pac(pah, stp);
	if (*stp != aud_s_ok) {
	    return;
	}
	dce_aud_start_with_pac(event, (sec_id_pac_t *) v1_privs, addr, options,
			       outcome, ard, stp);
        if (addr) rpc_string_free(&addr,stp);
    } /* Only two authorization services are possible for now */
 
    if (*ard == NULL) return;  /* filters say no need to audit */
 
    if ((options & aud_c_evt_delegates_info) == 0)
	return;  /* no need to include delegates information */
 
    /* store the delegation chain in the header */
    sec_login_cred_init_cursor(&cursor, stp);
 
    for(;;) {
	pah = sec_cred_get_delegate(callers_identity, &cursor, stp);
	if (*stp == sec_cred_s_no_more_entries) {
	    *stp = aud_s_ok;
            sec_cred_free_cursor(&cursor, stp);
	    return;
	}
	if (*stp != aud_s_ok) {
	    dce_aud_discard(*ard, &lst);
            *ard = NULL;
            sec_cred_free_cursor(&cursor, stp);
	    return;
	}
	v1_privs = sec_cred_get_v1_pac(pah, stp);
	if (*stp != aud_s_ok) {
	    dce_aud_discard(*ard, &lst);
            sec_cred_free_cursor(&cursor, stp);
            *ard = NULL;
	    return;
	}
	aud__evt_add_id_to_header(v1_privs, options, *ard);
    }
    
    sec_cred_free_cursor(&cursor, stp);
    return;
}
 
void            dce_aud_start_with_server_binding
                (
		                 unsigned32 event,
		                 rpc_binding_handle_t binding,
		                 unsigned32 options,
		                 unsigned32 outcome,
		                 dce_aud_rec_t * ard,
		                 error_status_t * stp
) {
    unsigned_char_t *server_princ;
    unsigned_char_t *addr;
    int              i;
    error_status_t   local_stp;

    *stp = aud_s_ok;
    *ard = NULL;

    if (aud_g_cli_audit_on == FALSE) {
	*ard = NULL;
	return;
    }

    rpc_binding_inq_auth_info(binding, &server_princ, NULL,
		NULL, NULL, NULL, stp);
    if (BAD_STATUS(stp)) return;

    if ((options & aud_c_evt_address_info) != 0) {
	rpc_binding_to_string_binding(binding, &addr, stp);
	if (*stp != rpc_s_ok) {
	    return;
	}
    } else {
	addr = NULL;
    }

    dce_aud_start_with_name(event, server_princ, addr,
				options, outcome, ard, stp);

    rpc_string_free(&server_princ, &local_stp);
    if (addr) rpc_string_free(&addr,&local_stp);
    return;
}


void
                dce_aud_start_with_pac
                (
		                 unsigned32 event,
		                 sec_id_pac_t * pac,
		                 unsigned_char_t * address,
		                 unsigned32 options,
		                 unsigned32 outcome,
		                 dce_aud_rec_t * ard,
		                 unsigned32 * status
) {
    aud_esl_act_t  *as = NULL;
    dce_aud_rec_t   ardp;
    int             i;
    uuid_t          nil_uuid;


    *status = aud_s_ok;
    *ard = NULL;
    if (aud_g_cli_audit_on == FALSE) {
	return;
    }

    if (outcome == 0)
    {
       outcome--;
    }


    if ((options & aud_c_evt_always_log) == 0 &&
	(options & aud_c_evt_always_alarm) == 0 &&
        (aud_g_cli_filter_on)) 
    {
    	pthread_mutex_lock(&esl_mutex);
    	aud_esl_get_actions(event, pac, outcome, &as, status);
    	pthread_mutex_unlock(&esl_mutex);
    	if (as == NULL) {
	    *ard = NULL;
#ifdef DCE_AUD_DEBUG
	    DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
		"dce_aud_start_with_pac: event needs not auditing \n"));
#endif
	    return;
	}
    }
    ardp = (dce_aud_rec_t) malloc(sizeof(struct _dce_aud_rec_handle));
#ifdef DCE_AUD_DEBUG
    DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	"dce_aud_start_with_pac: event may need auditing\n"));
#endif

    uuid_create_nil(&nil_uuid, status);
    ardp->header.server = nil_uuid;

    ardp->header.event = event;
    ardp->header.outcome = outcome;
    ardp->header.authz_st = rpc_c_authz_dce;
    ardp->header.num_client_ids = 0;
    ardp->header.client_id = NULL;
    aud__evt_add_id_to_header(pac, options, ardp);
    utc_gettime(&(ardp->header.time)); 
    if ((options & aud_c_evt_address_info) != 0 && address != NULL) {
	ardp->header.addr = (unsigned_char_t *) malloc(strlen((char *)address) + 1);
	strcpy((char *) ardp->header.addr, (char *) address);
    } else {
	ardp->header.addr = NULL;
    }
    ardp->as = as;
    ardp->options = options;
    ardp->tail = NULL;
    *ard = ardp;
    return;
}

void
                dce_aud_start_with_name
                (
		                 unsigned32 event,
		                 unsigned_char_t * client, /* global name */
		                 unsigned_char_t * address,
		                 unsigned32 options,
		                 unsigned32 outcome,
		                 dce_aud_rec_t * ard,
		                 unsigned32 * status
) {
    sec_rgy_name_t  princ_name, cell_name;
    uuid_t          princ_uuid, cell_uuid;
    sec_id_pac_t   *pac;
    unsigned32	    lst;


    *status = aud_s_ok;
    *ard = NULL;
    if (aud_g_cli_audit_on == FALSE) {
	return;
    }

    sec_id_global_parse_name(NULL, client, cell_name, &cell_uuid, princ_name,
		       &princ_uuid, status);
    if (BAD_STATUS(status)) {
	return;
    }
    pac = (sec_id_pac_t *) malloc(sizeof(sec_id_pac_t));
    pac->principal.uuid = princ_uuid;
    pac->realm.uuid = cell_uuid;
    uuid_create_nil(&(pac->group.uuid), status);
    pac->num_groups = 0;

    dce_aud_start_with_pac(event, pac, address, options, outcome, ard, status);
    if (BAD_STATUS(status)) return;
    if (*ard != NULL)
    {
       (*ard)->header.authz_st = rpc_c_authz_name;
    }
    free(pac);
/*
    Add a server parameter: CR
    sec_rgy_pgo_name_to_id(rgy_context, sec_rgy_domain_person,
                               server, (*ard)->header.server, status);
    if (BAD_STATUS(status)) {
        *status = aud_s_unknown_principal_rc;
        dce_svc_printf(AUD_S_UNKNOWN_PRINCIPAL_MSG, server);
	dce_aud_discard(*ard, &lst);
    }
 */

    return;
}


void
                dce_aud_put_ev_info
                (
		                 dce_aud_rec_t ard,
		                 dce_aud_ev_info_t info,
		                 error_status_t * status
) {
    aud_log_ev_info_t      *evip;
    int                     i;
    aud_log_ev_info_list_t *tail_list_p;

    if (ard == NULL) {
	dce_svc_printf(AUD_S_INVALID_RECORD_DESCRIPTOR_MSG);
	*status = aud_s_invalid_record_descriptor;
	return;
    }

    if ((info.format == aud_c_evt_info_acl) &&
         ((sec_acl_t *)(info.data.acl) == NULL))
       return;
    if ((info.format == aud_c_evt_info_byte_string) &&
         ((idl_byte *)(info.data.byte_string) == NULL))
       return;
    if ((info.format == aud_c_evt_info_char_string) &&
         ((idl_char *)(info.data.char_string) == NULL))
       return;

    *status = aud_s_ok;
    if (ard->tail == NULL) {
	ard->tail = (aud_log_ev_info_list_t *)
	    malloc(sizeof(aud_log_ev_info_list_t) +
		   SMALL_SIZE * sizeof(aud_log_ev_info_t));
	ard->tail->item_count = 1;
    } else if (ard->tail->item_count < SMALL_SIZE) {
	ard->tail->item_count++;
    } else if (ard->tail->item_count < MEDIUM_SIZE) {
	if (ard->tail->item_count == SMALL_SIZE) {
	    tail_list_p = (aud_log_ev_info_list_t *) malloc(
					    sizeof(aud_log_ev_info_list_t) +
				   MEDIUM_SIZE * sizeof(aud_log_ev_info_t));
	    for (i = 0; i < SMALL_SIZE; i++) {
		tail_list_p->info_item[i] = ard->tail->info_item[i];
	    }
	    free(ard->tail);
	    ard->tail = tail_list_p;
	    ard->tail->item_count = SMALL_SIZE + 1;
	} else {
	    ard->tail->item_count++;
	}
    } else if (ard->tail->item_count < LARGE_SIZE) {
	if (ard->tail->item_count == MEDIUM_SIZE) {
	    tail_list_p = (aud_log_ev_info_list_t *) malloc(
					    sizeof(aud_log_ev_info_list_t) +
				    LARGE_SIZE * sizeof(aud_log_ev_info_t));
	    for (i = 0; i < MEDIUM_SIZE; i++) {
		tail_list_p->info_item[i] = ard->tail->info_item[i];
	    }
	    free(ard->tail);
	    ard->tail = tail_list_p;
	    ard->tail->item_count = MEDIUM_SIZE + 1;
	} else {
	    ard->tail->item_count++;
	}
    } else {
	dce_svc_printf(AUD_S_EVT_TAIL_INFO_EXCEEDS_LIMIT_MSG);
	*status = aud_s_evt_tail_info_exceeds_limit;
	return;
    }

    evip = &ard->tail->info_item[ard->tail->item_count - 1];
    evip->format = info.format;
    switch (info.format) {

    case aud_c_evt_info_small_int:
	evip->tagged_union.small_int =
	    info.data.small_int;
	break;

    case aud_c_evt_info_short_int:
	evip->tagged_union.short_int =
	    info.data.short_int;
	break;

    case aud_c_evt_info_long_int:
	evip->tagged_union.long_int =
	    info.data.long_int;
	break;

    case aud_c_evt_info_hyper_int:
	evip->tagged_union.hyper_int = (idl_hyper_int *)
	    malloc(sizeof(idl_hyper_int));
	*evip->tagged_union.hyper_int = info.data.hyper_int;
	break;

    case aud_c_evt_info_usmall_int:
	evip->tagged_union.usmall_int = info.data.usmall_int;
	break;

    case aud_c_evt_info_ushort_int:
	evip->tagged_union.ushort_int = info.data.ushort_int;
	break;

    case aud_c_evt_info_ulong_int:
	evip->tagged_union.ulong_int = info.data.ulong_int;
	break;

    case aud_c_evt_info_uhyper_int:
	evip->tagged_union.uhyper_int = (idl_uhyper_int *)
	    malloc(sizeof(idl_uhyper_int));
	*evip->tagged_union.uhyper_int = info.data.uhyper_int;
	break;

    case aud_c_evt_info_short_float:
	evip->tagged_union.short_float = info.data.short_float;
	break;

    case aud_c_evt_info_long_float:
	evip->tagged_union.long_float = (idl_long_float *)
	    malloc(sizeof(idl_long_float));
	*evip->tagged_union.long_float = info.data.long_float;
	break;

    case aud_c_evt_info_boolean:
	evip->tagged_union.bool = info.data.boolean;
	break;

    case aud_c_evt_info_uuid:
	evip->tagged_union.uuid = (uuid_t *) malloc(sizeof(uuid_t));
	*evip->tagged_union.uuid = info.data.uuid;
	break;

    case aud_c_evt_info_utc:
	evip->tagged_union.utc = (utc_t *) malloc(sizeof(utc_t));
	*evip->tagged_union.utc = info.data.utc;
	break;

    case aud_c_evt_info_acl:
	{
	    sec_acl_t      *new_acl;
	    aud__log_dup_acl(info.data.acl, &new_acl);
	    evip->tagged_union.acl = new_acl;
	}
	break;

    case aud_c_evt_info_byte_string:
	{
	    idl_byte       *cp = (idl_byte *)
	    malloc(strlen((char *)info.data.byte_string) + 1);
	    strcpy((char *) cp, (char *) info.data.byte_string);
	    evip->tagged_union.byte_string = cp;
	}
	break;

    case aud_c_evt_info_char_string:
	{
	    idl_char       *cp = (idl_char *)
	    malloc(strlen((char *) info.data.char_string) + 1);
	    strcpy((char *) cp, (char *) info.data.char_string);
	    evip->tagged_union.char_string = cp;
	}
	break;

    default:
	;
    }
    return;
}

void
                dce_aud_commit
                (
		                 dce_aud_trail_t at,
		                 dce_aud_rec_t ard,
		                 unsigned32 options,
		                 unsigned16 format,
		                 unsigned32 outcome,
		                 unsigned32 * status
) {

    idl_es_handle_t         h;
    int                     i, j;
    idl_byte               *ep;
    idl_ulong_int           esize;
    aud_esl_act_t           actions = aud_c_esl_act_none;
    aud_log_ev_info_list_t *tail_list;
    aud_log_ev_info_list_t *retrieved_ev_info;
    unsigned_char_t        *alarm_message;
    unsigned32              enabled_allocator;
    unsigned32              st;

    if (ard == NULL) {
	*status = aud_s_invalid_record_descriptor;
	return;
    }

    i = 0;
    if (outcome & aud_c_esl_cond_success)
       i++;
    if (outcome & aud_c_esl_cond_denial)
       i++;
    if (outcome & aud_c_esl_cond_failure)
       i++;
    if (outcome & aud_c_esl_cond_pending)
       i++;

    if ((i != 1) || ((outcome & ~aud_c_esl_cond_all) != 0))
    {
	*status = aud_s_invalid_outcome;
        dce_svc_printf(AUD_S_INVALID_OUTCOME_MSG, outcome);
	dce_aud_discard(ard, &st);
	return;
    }

    if ((ard->header.outcome & outcome) == 0)
    {
	*status = aud_s_outcomes_inconsistent;
        dce_svc_printf(AUD_S_OUTCOMES_INCONSISTENT_MSG, ard->header.outcome, outcome);
	dce_aud_discard(ard, &st);
	return;
    }

    *status = aud_s_ok;
#ifdef DCE_AUD_DEBUG
    if (ard->as != NULL) {
	for (i = 0; i < MAX_CONDITIONS - 1; i++) {
	    DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
		"audit actions on %s:\n", audit_condition[i].name));
	    actions = ard->as[i];
	    if (actions == 0) {
	        DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
			"No action"));
	    } else {
	        for (j = 0; j < MAX_ACTIONS - 1; j++) {
		    if ((audit_action[j].number & actions) != 0) {
			DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, 
			    svc_c_debug1, "%s ", audit_action[j].name));
		    }
	        }
	    }
	    if (audit_action[MAX_ACTIONS - 1].number == actions) {
	        DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
		    " (all)"));
	    }
	    DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1, "\n"));
        }
    }
#endif /* DCE_AUD_DEBUG */

    if (ard->as != NULL) {
	for (i = 0; i < MAX_CONDITIONS - 1; i++) 
        {
	    if (audit_condition[i].number == outcome) {
	        actions = ard->as[i];
	        break;
	    }
        }
    }

    ard->options |= options;

    if ((ard->options & aud_c_evt_always_log) != 0 ||
        !aud_g_cli_filter_on) 
    {
	actions |= aud_c_esl_act_log;
    }
    if ((ard->options & aud_c_evt_always_alarm) != 0) 
    {
	actions |= aud_c_esl_act_alarm;
    }

    if (actions == aud_c_esl_act_none) {
	dce_aud_discard(ard, status);
	return;
    }
    ard->header.outcome = outcome;
    if (actions & aud_c_esl_act_alarm) {
        dce_aud_print(ard, 0, &alarm_message, status);
	printf("%s", alarm_message);
	free(alarm_message);
    } 

    ard->header.format = format;

    if (actions & aud_c_esl_act_log) { 
	if (at->location == TRAIL_DAEMON) {
	    (*audit_log_v1_0_c_epv.audit_log_append) (at->auditd_binding, 
		ard->options, &ard->header, &ard->tail, status);
	} else {
	    idl_es_encode_dyn_buffer(&ep, &esize, &h, status);

            AUD_SET_ALLOC_FREE(enabled_allocator);
            audit_pickle_dencode_ev_info(h, &ard->header, &ard->tail, status);
                
            if (BAD_STATUS(status)) {
                idl_es_handle_free(&h, &st);
                AUD_RESET_ALLOC_FREE(enabled_allocator, ep);
                return;
            }

#ifdef DCE_AUD_DEBUG
	    DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
		"pickle size = %ld\n", esize));
#endif
	    aud_trl_file_write(at, ep, esize, ard->options, status);

	    idl_es_handle_free(&h, &st);
            AUD_RESET_ALLOC_FREE(enabled_allocator, ep);
	}
    }
    dce_aud_discard(ard, &st);
    return;
}
