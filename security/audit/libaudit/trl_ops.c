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
 * $Log: trl_ops.c,v $
 * Revision 1.1.10.1  1996/07/08  18:32:40  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:27 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  00:06:37  marty  1.1.8.2]
 *
 * Revision 1.1.8.2  1996/02/18  00:06:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:52  marty]
 * 
 * Revision 1.1.8.1  1995/12/08  17:42:16  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/6  1995/09/29  14:23 UTC  jrr
 * 	Remove references to now-defunct own-primary-group-name.
 * 
 * 	HP revision /main/HPDCE02/jrr_dcecp_01/1  1995/09/29  14:21 UTC  jrr
 * 	Remove references to now-defunct own-primary-group-name.
 * 
 * 	HP revision /main/HPDCE02/5  1995/08/18  19:53 UTC  kline_s
 * 	Merge fix for CHFts16115
 * 
 * 	HP revision /main/HPDCE02/5  1995/08/18  19:52 UTC  kline_s
 * 
 * 	HP revision /main/HPDCE02/kline_s_mothra/3  1995/08/17  21:29 UTC  kline_s
 * 	CHFts16115: Remove DES_HIDDEN conditional - only using integrity
 * 	protection level instead of using the privacy protection level
 * 	for Domestic.  Required to avoid conflicts due to singular
 * 	Domestic/Export release.
 * 
 * 	HP revision /main/HPDCE02/kline_s_mothra/2  1995/07/06  19:54 UTC  kline_s
 * 	Fix international release audit test problems in using rpc_c_authn_level_pkt_privacy
 * 	as the level of protection for the rpc_binding_set/_auth_info() and
 * 	rpc_binding_inq_auth_client() calls.  This uses DES encryption, which is not supported
 * 	in the international version.
 * 
 * 	HP revision /main/HPDCE02/3  1995/07/14  18:43 UTC  jrr
 * 	Change dce_printf() to dce_svc_printf().
 * 	[1995/07/14  18:40 UTC  jrr  /main/HPDCE02/jrr_mothra_pure/1]
 * 
 * 	HP revision /main/HPDCE02/2  1995/06/20  17:45 UTC  kline_s
 * 	CHFts15485: Tighten permissions on audit trail and associated index
 * 	files to eliminate security hole.
 * 
 * 	HP revision /main/HPDCE02/kline_s_mothra/1  1995/06/13  20:53 UTC  kline_s
 * 	CHFts15485: Tighten permissions for audit trail and associated index
 * 	files to eliminate security hole.
 * 
 * 	HP revision /main/HPDCE02/1  1994/10/05  11:18 UTC  mothra
 * 	add limits.h
 * 	[1995/12/08  17:18:18  root]
 * 
 * Revision 1.1.2.20  1994/09/27  00:27:19  weisz
 * 	CR 10703: in case of wrap, fopen flags should be wb+
 * 	[1994/09/27  00:26:49  weisz]
 * 
 * Revision 1.1.2.19  1994/09/22  21:09:07  weisz
 * 	CR 10703: handling of trail full situation.
 * 	[1994/09/22  21:08:08  weisz]
 * 
 * Revision 1.1.2.18  1994/08/17  19:56:56  weisz
 * 	CR 11443: added PENDING as a new valid outcome.
 * 	[1994/08/17  19:54:53  weisz]
 * 
 * Revision 1.1.2.15  1994/08/08  18:52:31  weisz
 * 	CR 10516: remove parameters from messages going accross rpcs as return codes.
 * 	[1994/08/08  18:51:45  weisz]
 * 
 * Revision 1.1.2.14  1994/07/27  18:07:33  weisz
 * 	CR 11166: own_primary_group_id.next needs initialization.
 * 	[1994/07/27  18:07:11  weisz]
 * 
 * Revision 1.1.2.13  1994/07/25  14:04:30  weisz
 * 	CR 11166: cache own id, to prevent trail reading loop.
 * 	[1994/07/25  14:03:52  weisz]
 * 
 * Revision 1.1.2.12  1994/07/14  19:48:59  weisz
 * 	CR 11167: DCEAUDITOFF should not affect trail opened for read.
 * 	[1994/07/14  19:48:31  weisz]
 * 
 * Revision 1.1.2.11  1994/06/20  20:24:32  weisz
 * 	CR 10994: fixed.
 * 	[1994/06/20  20:22:28  weisz]
 * 
 * Revision 1.1.2.10  1994/06/06  17:43:55  weisz
 * 	CR 10829: fix some memory leaks.
 * 	[1994/06/06  15:43:59  weisz]
 * 
 * Revision 1.1.2.9  1994/05/29  21:03:35  weisz
 * 	CR 10801: added +1 to strlen in dce_aud_open,
 * 	          and added free(at->trail_file) in dce_aud_close.
 * 	[1994/05/29  20:59:24  weisz]
 * 
 * Revision 1.1.2.8  1994/05/28  17:18:32  weisz
 * 	CR 10416: add unregister update interface to dce_aud_close, and since
 * 	          dce_aud_close could be invoked more than once, ignore the
 * 	          error returned.
 * 	[1994/05/28  17:07:31  weisz]
 * 
 * Revision 1.1.2.7  1994/05/03  13:34:29  weisz
 * 	CR 10501: added initialization *at=NULL in dce_aud_open
 * 	[1994/05/03  13:33:57  weisz]
 * 
 * Revision 1.1.2.6  1994/04/15  15:38:02  weisz
 * 	CR 10353: dce_aud_open flags checking fix
 * 	[1994/04/15  15:37:37  weisz]
 * 
 * Revision 1.1.2.4  1994/03/11  22:48:47  luan
 * 	CR 9775 9776: Changed debug messages to use dce_svc_printf() or DCE_SVC_DEBUG.
 * 	CR 10064: Removed a bogus return in dce_aud_open().
 * 	[1994/03/11  22:31:26  luan]
 * 
 * Revision 1.1.2.3  1994/02/22  16:47:37  luan
 * 	CR 9775 9776 9860:  Defined DCE_AUD_DEBUG in audit.h.  Removed
 * 	debug_mode and PRE_BETA_DEBUG or replaced it with DCE_DEBUG.
 * 	[1994/02/22  16:43:17  luan]
 * 
 * Revision 1.1.2.2  1994/01/25  04:59:44  luan
 * 	CR 9745 9746: File path names consolidated.
 * 	Hard-coded path names replaced with macros
 * 	which generate names dynamically.
 * 	[1994/01/25  04:19:35  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:39:39  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:47  zeliff]
 * 
 * $EndLog$
 */

/*
** trl_ops.c
** Implements audit-trail operation API.
*/

#include <stdlib.h>
#include <stdio.h>
#include <limits.h>
#include <evtp.h>
#include <dceaudmsg.h>
#include <dceaudmac.h>
#include <dceaudsvc.h>

boolean32 aud_g_cli_audit_on  = TRUE;
boolean32 aud_g_cli_filter_on = FALSE;
boolean32 at_mutex_created    = FALSE;
boolean32 esl_mutex_created   = FALSE;

aud_esl_guides_p_t aud_g_cli_world_guides = NULL;
aud_esl_guides_p_t aud_g_cli_world_overridable_guides = NULL;
static aud_esl_entry_p_t cli_esl_princ[HASH_TABLE_SIZE];
static aud_esl_foreign_entry_p_t cli_esl_foreign_princ[HASH_TABLE_SIZE];
static aud_esl_entry_p_t cli_esl_group[HASH_TABLE_SIZE];
static aud_esl_foreign_entry_p_t cli_esl_foreign_group[HASH_TABLE_SIZE];
static aud_esl_entry_p_t cli_esl_cell[HASH_TABLE_SIZE];
static aud_esl_entry_p_t cli_esl_cell_overridable[HASH_TABLE_SIZE];

void          **aud_g_cli_esl[aud_e_esl_cell_overridable + 1] = {
					     (void **) cli_esl_princ,
				     (void **) cli_esl_foreign_princ,
					     (void **) cli_esl_group,
				     (void **) cli_esl_foreign_group,
					      (void **) cli_esl_cell,
				   (void **) cli_esl_cell_overridable
};

time_t               ESL_File_Timestamp[aud_e_esl_max];
time_t               ESL_UPDATE_File_Timestamp[aud_e_esl_max];
boolean32            cell_uuid_known = FALSE;
sec_rgy_handle_t     rgy_context;
sec_rgy_properties_t properties;
unsigned32           aud_g_cli_first_evt_number;
unsigned32           aud_g_cli_num_of_evts;

struct act_cond_text audit_condition[] = {
					  {"success", 0x00000001},
					  {"failure", 0x00000002},
					  {"denial", 0x00000004},
					  {"pending", 0x00000008},
					  {"all", 0x0000000F}
};
struct act_cond_text audit_action[] = {
				       {"none", 0x0},
				       {"log", 0x00000001},
				       {"alarm", 0x00000002},
				       {"all", 0x00000003}
};

void            dce_aud_reset
                (
                                 dce_aud_trail_t *at,
                                 unsigned32 * stp
) {

    if (at == NULL) {
        dce_svc_printf(AUD_S_INVALID_TRAIL_DESCRIPTOR_MSG);
        *stp = aud_s_invalid_trail_descriptor;
        return;
    }

    *stp = aud_s_ok;

    (*at)->trail_cursor = 0;                 /* reset the trail file cursor  */
    (*at)->index_cursor = 0;                 /* reset the index file cursor  */
    fseek((*at)->trail_fp, 0, SEEK_SET);     /* reset the trail file pointer */
    fseek((*at)->md_index_fp, 0, SEEK_SET);  /* reset the index file pointer */
    return;
}

void            dce_aud_rewind
                (
                                 dce_aud_trail_t at,
                                 unsigned32 * stp
) {

    if (at == NULL) {
        dce_svc_printf(AUD_S_INVALID_TRAIL_DESCRIPTOR_MSG);
        *stp = aud_s_invalid_trail_descriptor;
        return;
    }

    /* lock the mutex */
    pthread_mutex_lock(&at->mutex);

    /* rewind the trail file */
    if (ftruncate(at->trail_fd, 0) == -1) {        /* truncate the trail file'size */
        dce_svc_printf(AUD_S_REWIND_TRAIL_FILE_MSG, at->trail_file);
        *stp = aud_s_rewind_trail_file;
        pthread_mutex_unlock(&at->mutex);
        return;
    }

    /* rewind the index file */
    if (ftruncate(at->md_index_fd, 0) == -1) {     /* truncate the index file'size */
        dce_svc_printf(AUD_S_REWIND_INDEX_FILE_MSG, at->index_file);
        *stp = aud_s_rewind_index_file;
        pthread_mutex_unlock(&at->mutex);
        return;
    }

    /* reset the file pointers */
    dce_aud_reset(&at, stp);

    /* unlock the mutex */
    pthread_mutex_unlock(&at->mutex);
    return;

}

void            dce_aud_open
                (
		                 unsigned32 flags,
		                 char *description,
		                 unsigned32 first_evt_num,
		                 unsigned32 total_evts,
		                 dce_aud_trail_t * at,
		                 unsigned32 * stp
) {
    aud_esl_type_t       esl_type;
    idl_es_handle_t      h;
    rpc_binding_handle_t daemon_binding;
    FILE                *trail_fp, *md_index_fp;
    char                *open_flags;
    int                  i;
    char                 buf[FILE_NAME_LENGTH];
    boolean32            open_for_read = FALSE;
    boolean32            storage_strategy_wrap = FALSE;
    struct stat          stat_buf;
    int                  size;
    unsigned32	         lst;
    char                *value_string_ptr;
    unsigned32           size_limit_value;

    dce_aud_init_svc();
    *stp = aud_s_ok;
    *at  = NULL;

    if ((flags == 0)
             ||
       (((flags & aud_c_trl_open_read) != 0) && ((flags & aud_c_trl_open_write) != 0)))
    {
       *stp = aud_s_trl_invalid_open_flags;
       return;
    }

    if ((flags & aud_c_trl_open_read) != 0) {
	open_for_read = TRUE;
    }

    if ((flags & aud_c_trl_ss_wrap) != 0) {
	storage_strategy_wrap = TRUE;
    }
    if ((char *) getenv("DCEAUDITOFF") != NULL) {
	aud_g_cli_audit_on = FALSE;
	if (!open_for_read) return;
    }
    if ((char *) getenv("DCEAUDITFILTERON") != NULL) {
	aud_g_cli_filter_on = TRUE;
    }

    if (description == NULL) {
	aud_cli_bind_to_dmn(&daemon_binding, stp);
	if (BAD_STATUS(stp)) {
	    return;
	}
    } else {
	if (open_for_read) {
	    open_flags = "rb";
	} else {
	    if (storage_strategy_wrap) {
               open_flags = "wb+";
	    } else {
               open_flags = "ab+";
	    }
	}
	trail_fp = fopen(description, open_flags);
	if (trail_fp == NULL) {
	    dce_svc_printf(AUD_S_CANNOT_OPEN_TRAIL_FILE_MSG, description);
	    *stp = aud_s_cannot_open_trail_file_rc;
	    return;
	}
	strcpy(buf, description);
	strcat(buf, ".md_index");
	if ((md_index_fp = fopen(buf, open_flags)) == NULL) {
	    dce_svc_printf(AUD_S_CANNOT_OPEN_TRAIL_FILE_MSG, description);
	    *stp = aud_s_cannot_open_trail_file_rc;
	    return;
	}
	if (!open_for_read) {
	    fseek(trail_fp, 0, SEEK_END);     /* go to the end of the file */
	    fseek(md_index_fp, 0, SEEK_END);  /* go to the end of the file */

            /* CHFts15485: Tighten permissions on audit trail file and index */
            chmod(description, 0640);
            chmod(buf, 0640);
	}
    }

    *at = (struct _dce_aud_trail_handle *)
	malloc(sizeof(struct _dce_aud_trail_handle));
    if (*at == NULL) {
	dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, 
			sizeof(struct _dce_aud_trail_handle));
	*stp = aud_s_cannot_allocate_memory;
        return;
    } else {
        (*at)->auditd_binding = NULL;
	if (description == NULL) {
	    (*at)->auditd_binding = daemon_binding;
	    (*at)->location = TRAIL_DAEMON;
	} else {
	    (*at)->location = TRAIL_FILE;
	    (*at)->trail_file = (char *) malloc(strlen(description) + 1);
            if ((*at)->trail_file == NULL) {
	        dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, strlen(description) + 1); 
	        *stp = aud_s_cannot_allocate_memory;
                return;
            }
	    strcpy((*at)->trail_file, description);
            (*at)->index_file = (char *) malloc(strlen(buf) + 1);
            if ((*at)->index_file == NULL) {
	        dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG,  strlen(buf) + 1);
	        *stp = aud_s_cannot_allocate_memory;
                return;
            }
            strcpy((*at)->index_file, buf);
	    (*at)->trail_fp = trail_fp;
	    (*at)->trail_fd = fileno(trail_fp);
	    (*at)->md_index_fp = md_index_fp;
	    (*at)->md_index_fd = fileno(md_index_fp);
	    if (!open_for_read) {
	        fstat((*at)->trail_fd, &stat_buf); 
	        (*at)->trail_cursor = stat_buf.st_size;
	    } else {
	        (*at)->trail_cursor = 0;
	        (*at)->index_cursor = 0;
	    }

            value_string_ptr = (char *) getenv("DCEAUDITTRAILSIZE");
            if (value_string_ptr == NULL)
            {
               (*at)->file_size_limit_value = DEFAULT_TRAIL_SIZE_LIMIT;
               (*at)->file_size_limit = FALSE;
            }
            else
            {
               size_limit_value = strtoul(value_string_ptr, (char **)NULL, 10);

               if ((size_limit_value == 0) || (size_limit_value == ULONG_MAX))
               {
		  dce_svc_printf(AUD_S_INVALID_AUDIT_TRAIL_SIZE_LIMIT_MSG);
		  *stp = aud_s_invalid_audit_trail_size_limit;
		  return;
               }
                   
               (*at)->file_size_limit_value = size_limit_value;
               (*at)->file_size_limit = TRUE;
            }

	    if (pthread_mutex_init(&(*at)->mutex, 
				pthread_mutexattr_default)<0){
		dce_svc_printf(AUD_S_CANNOT_INIT_TRAIL_MUTEX_MSG);
		*stp = aud_s_cannot_init_trail_mutex;
		return;
   	    }
            at_mutex_created = TRUE;
	}
	(*at)->flags = flags;
    }

    /* open read, or open write by auditd as the central trail
     * NO need of inputing event table in either case
     * because no filter is going to be used
     */

    if (open_for_read || (flags & aud_c_trl_open_write_no_filter) != 0 ||
	!aud_g_cli_filter_on) {
	return;
    }
    aud_g_cli_first_evt_number = first_evt_num;
    aud_g_cli_num_of_evts = total_evts;
    input_evt_table();

    /*
     * initialize the esl hash tables, one for local principals and local
     * groups, one for foreign principals and foreign groups, and one for
     * cells. 
     */

    if (aud_g_cli_num_of_ecs != 0) {
	for (esl_type = aud_e_esl_princ;
	     esl_type < aud_e_esl_max; esl_type++) {
	    ESL_File_Timestamp[esl_type] =
		aud_esl_input_file(esl_type, aud_g_cli_num_of_ecs,
				      aud_g_cli_evt_classes,
				      aud_g_cli_esl,
				      &aud_g_cli_world_guides,
				      &aud_g_cli_world_overridable_guides);
	    if (esl_type <= aud_e_esl_cell_overridable) {
		ESL_UPDATE_File_Timestamp[esl_type] =
		    aud_esl_input_update_file(esl_type,
						 aud_g_cli_num_of_ecs,
						 aud_g_cli_evt_classes,
						 aud_g_cli_esl);
	    }
	}
    }				

    if (pthread_mutex_init(&esl_mutex, pthread_mutexattr_default) < 0) {
	dce_svc_printf(AUD_S_CLIENT_ESL_MUTEX_MSG);
	*stp = aud_s_client_esl_mutex;
    }
    else
    {
        esl_mutex_created = TRUE;
    }
    /*  init_update_module(); */
    register_update_interface(stp);
    return;
}


void            dce_aud_set_trail_size_limit
                (
                                 dce_aud_trail_t at,
                                 unsigned32 file_size_limit_value,
                                 unsigned32 * stp
) {

    if (at == NULL) {
        dce_svc_printf(AUD_S_INVALID_TRAIL_DESCRIPTOR_MSG);
        *stp = aud_s_invalid_trail_descriptor;
        return;
    }

    /*
     * if the file size limit was set in dce_aud_open from the DCEAUDITTRAILSIZE 
     * environment variable then don't do here anything, else overwrite the 
     * default size set in dce_aud_open, by the value passed in here. 
     */

    pthread_mutex_lock(&at->mutex);

    if (at->file_size_limit == FALSE)
    {
       if (file_size_limit_value != 0)
       {
          at->file_size_limit_value = file_size_limit_value;
       }
    }
    pthread_mutex_unlock(&at->mutex);
    *stp = aud_s_ok;
    return;
}


void            dce_aud_close
                (
                                 dce_aud_trail_t at,
                                 unsigned32 * stp
) {

    if (at == NULL) {
        dce_svc_printf(AUD_S_INVALID_TRAIL_DESCRIPTOR_MSG);
       *stp = aud_s_invalid_trail_descriptor;
        return;
    }

    *stp = aud_s_ok;

    if (
	( ( (at->flags & aud_c_trl_open_write) != 0 
			||
	    (at->flags & aud_c_trl_open_write_no_filter) != 0
	  ) 
	 		&& 
	     at->location == TRAIL_FILE
	 )
					||
	((at->flags & aud_c_trl_open_read) != 0)
    ) {

	/*
	 * flush trail buffer to trail file 
	 */
#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_trl, svc_c_debug1, 
	    "dce_aud_close: closing trail and trail index files\n"));
#endif
	fclose(at->trail_fp);
	fclose(at->md_index_fp);
    }
    if (at->location == TRAIL_FILE) {
        free(at->trail_file);
        free(at->index_file);
    }
    if (at->auditd_binding != NULL)
        rpc_binding_free(&(at->auditd_binding), stp);
    /* destroy the mutexes if they were created */
    if (at_mutex_created) {
        pthread_mutex_destroy(&at->mutex);
        at_mutex_created = FALSE;
    }
    if (esl_mutex_created) {
        pthread_mutex_destroy(&esl_mutex);
        esl_mutex_created = FALSE;
    }
    free(at);
    unregister_update_interface(stp);
    if (*stp != aud_s_ok)
        *stp = aud_s_ok;
    return;
}


void            aud_cli_bind_to_dmn
                (
		                 rpc_binding_handle_t * auditd_handle,
		                 error_status_t * stp
) {
    sec_login_handle_t  login_context;
    FILE               *fptr;
    char                buff[1024];
    unsigned_char_p_t   obj      = NULL;
    unsigned_char_p_t   protseq  = NULL;
    unsigned_char_p_t   netaddr  = NULL;
    unsigned_char_p_t   endpoint = NULL;
    unsigned_char_p_t   options  = NULL;
    unsigned_char_p_t   binding  = NULL;
    aud_esl_guides_p_t  guide    = NULL;
    unsigned_char_p_t   protseq_real, endpoint_real, binding_real, hostname;
    error_status_t      lst;
    int                 i;

    if ((fptr = fopen
	 (DAEMON_BINDING_FILE, "r")) != NULL) {
#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_general, svc_c_debug1,
	    "aud_cli_bind_to_dmn: daemon_bindings file %s open ok\n", 
	    DAEMON_BINDING_FILE));
#endif
	if (fgets(buff, 1024, fptr)) {
	    /* fgets keeps newlines, so get rid of it manually */
	    if (buff[strlen(buff) - 1] == '\n')
		buff[strlen(buff) - 1] = '\0';
	    binding_real = (unsigned_char_p_t) buff;
	    rpc_string_binding_parse((unsigned_char_p_t) buff, &obj,
			      &protseq, &netaddr, &endpoint, &options, stp);
	    if (GOOD_STATUS(stp)) {
		if (protseq == NULL || *protseq == 0) {
		    protseq_real = (unsigned_char_p_t) "ip";
		} else
		    protseq_real = protseq;

		if (endpoint == NULL || *endpoint == 0) {
		    endpoint_real = (unsigned_char_p_t) "";
		} else
		    endpoint_real = endpoint;

		rpc_string_binding_compose(obj, protseq_real, netaddr,
					   endpoint_real, options,
					   &binding, stp);
#ifdef DCE_AUD_DEBUG
		DCE_SVC_DEBUG((aud_svc_handle, aud_s_general, svc_c_debug1,
	 	    "(aud_cli_bind_to_dmn: binding =  %s\n", binding));	
#endif
		if (GOOD_STATUS(stp)) {
		    binding_real = binding;
		    rpc_binding_from_string_binding(binding_real,
						    auditd_handle, stp);
		}
	    }			/* end of if rpc_string_binding_parse */
	}			/* end of if fgets */
    }
    /* end of if fopen */
    else {			/* fopen() returns NULL */
	dce_svc_printf(AUD_S_CANNOT_OPEN_DMN_BINDING_FILE_MSG);
	*stp = aud_s_cannot_open_dmn_binding_file;
	return;
    }

    sec_login_get_current_context(&login_context, stp);
    if (BAD_STATUS(stp)) {
	dce_svc_printf(AUD_S_NO_CURRENT_LOGIN_CONTEXT_MSG);
	return;
    }
    if ((fptr = fopen(DAEMON_IDENTITY_FILE, "r")) != NULL) {
#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_general, svc_c_debug1,
"aud_cli_bind_to_dmn: daemon_identity file open ok\n"));
#endif
	if (fgets(buff, 1024, fptr)) {
	    /* fgets keeps newlines, so get rid of it manually */
	    if (buff[strlen(buff) - 1] == '\n')
		buff[strlen(buff) - 1] = '\0';
	}
    } else {
	dce_svc_printf(AUD_S_CANNOT_OPEN_DMN_IDENTITY_FILE_MSG);
	*stp = aud_s_cannot_open_dmn_identity_file;
	return;
    }
    /* Setup the auth info on the handle */
    if (GOOD_STATUS(stp)) {

        /*
         * For international (ie. non-DES) releases, use a less strict,
         * non-encrypted protection level in rpc_binding_set_auth_info():
         */

	rpc_binding_set_auth_info(*auditd_handle, (unsigned_char_t *) buff,
				  rpc_c_authn_level_pkt_integrity,
				  rpc_c_authn_dce_private,
				  (rpc_auth_identity_handle_t) login_context,
				  rpc_c_authz_dce, stp);
	if (BAD_STATUS(stp)) {
	    return;
	}
    }
    /*
     * Free the strings that may have been allocated by the RPC runtime 
     */
    if (obj != NULL) {
	rpc_string_free(&obj, &lst);
    }
    if (protseq != NULL) {
	rpc_string_free(&protseq, &lst);
    }
    if (netaddr != NULL) {
	rpc_string_free(&netaddr, &lst);
    }
    if (endpoint != NULL) {
	rpc_string_free(&endpoint, &lst);
    }
    if (options != NULL) {
	rpc_string_free(&options, &lst);
    }
    if (binding != NULL) {
	rpc_string_free(&binding, &lst);
    }
    return;
}
