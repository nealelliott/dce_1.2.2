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
 * $Log: evt_read.c,v $
 * Revision 1.1.12.1  1996/07/08  18:31:50  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:26 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  00:06:27  marty  1.1.10.3]
 *
 * 	Change dce_printf() to dce_svc_printf().
 * 	[1995/07/14  18:38 UTC	jrr  /main/HPDCE02/jrr_mothra_pure/1]
 * 	HP 	[1995/07/14  18:42 UTC  jrr  /main/HPDCE02/2]
 *
 * 	Fix memory leak by using correct alloc/free for IDL es.
 * 	[1995/06/08  23:22 UTC	jrr  /main/jrr_sec_chfts15431/1]
 * 	HP 	[1995/06/08  23:23 UTC  jrr  /main/HPDCE02/1]
 * 	[1996/07/08  18:02:26  arvind]
 *
 * Revision 1.1.10.3  1996/02/18  00:06:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:48  marty]
 * 
 * Revision 1.1.10.2  1995/12/08  17:41:51  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/09/29  14:17 UTC  jrr
 * 	Recode the client_id loop correctly.
 * 
 * 	HP revision /main/HPDCE02/sommerfeld_dfsperf/3  1995/09/22  14:20 UTC  jrr
 * 	Recode the client_id loop correctly.
 * 
 * 	HP revision /main/HPDCE02/sommerfeld_dfsperf/2  1995/09/20  21:18 UTC  sommerfeld
 * 	Eliminate stupid local cache.
 * 
 * 	HP revision /main/HPDCE02/sommerfeld_dfsperf/1  1995/09/19  22:40 UTC  sommerfeld
 * 	For John Rousseau: avoid explicit binds to rgy.
 * 	clean up all compiler warnings.
 * 	fix some indentation.
 * 
 * 	HP revision /main/HPDCE02/2  1995/07/14  18:42 UTC  jrr
 * 	Change dce_printf() to dce_svc_printf().
 * 	[1995/07/14  18:38 UTC  jrr  /main/HPDCE02/jrr_mothra_pure/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/06/08  23:23 UTC  jrr
 * 	Fix memory leak by using correct alloc/free for IDL es.
 * 	[1995/06/08  23:22 UTC  jrr  /main/jrr_sec_chfts15431/1]
 * 
 * Revision 1.1.6.1  1994/10/26  20:03:07  weisz
 * 	CR 12731: in dce_aud_next, make local copy of incoming predicate.
 * 	[1994/10/26  20:01:03  weisz]
 * 
 * Revision 1.1.2.18  1994/09/26  17:10:43  weisz
 * 	CR 9834: fill in audit record header server field.
 * 	[1994/09/26  17:09:58  weisz]
 * 
 * Revision 1.1.2.17  1994/09/09  18:10:32  rousseau
 * 	Correctly close registry (CR 11950).
 * 	[1994/09/09  15:20:44  rousseau]
 * 
 * Revision 1.1.2.16  1994/09/02  18:19:35  weisz
 * 	CR 12029: fix some format problems in dce_aud_print for some info_items.
 * 	[1994/09/02  18:17:27  weisz]
 * 
 * Revision 1.1.2.15  1994/08/30  17:21:35  weisz
 * 	CR 11884: added new API - dce_aud_start_with_uuid
 * 	[1994/08/30  17:17:52  weisz]
 * 
 * Revision 1.1.2.14  1994/08/18  21:23:09  rousseau
 * 	Ignore warning from sec_login_inquire_net_info. (CR 11764).
 * 	[1994/08/18  21:22:55  rousseau]
 * 
 * Revision 1.1.2.13  1994/08/17  19:56:54  weisz
 * 	CR 11443: added PENDING as a new valid outcome.
 * 	[1994/08/17  19:54:26  weisz]
 * 
 * Revision 1.1.2.10  1994/07/27  14:54:16  weisz
 * 	CR 11372: added status parameter to audit_pickle_dencode_ev_info.
 * 	[1994/07/27  14:53:38  weisz]
 * 
 * Revision 1.1.2.9  1994/07/25  16:24:47  weisz
 * 	CR 11166: initialize own_id_t next pointer to NULL.
 * 	[1994/07/25  16:24:18  weisz]
 * 
 * Revision 1.1.2.8  1994/07/25  14:04:29  weisz
 * 	CR 11166: cache own id, to prevent trail reading loop.
 * 	[1994/07/25  14:03:46  weisz]
 * 
 * Revision 1.1.2.7  1994/06/08  18:36:54  weisz
 * 	CR 10809: changed free(ep) to rpc_sm_client_free.
 * 	[1994/06/08  18:22:05  weisz]
 * 
 * Revision 1.1.2.6  1994/04/25  20:54:14  weisz
 * 	CR 10370: fixed
 * 	[1994/04/25  20:46:33  weisz]
 * 
 * Revision 1.1.2.5  1994/03/11  22:48:42  luan
 * 	CR 9775 9776: Changed debug messages to use dce_svc_printf() or DCE_SVC_DEBUG.
 * 	[1994/03/11  22:29:32  luan]
 * 
 * Revision 1.1.2.4  1994/03/02  16:21:53  luan
 * 	CR 10038: Added delegation information in audit record header.
 * 	[1994/03/02  16:21:35  luan]
 * 
 * Revision 1.1.2.3  1994/02/22  16:47:33  luan
 * 	CR 9775 9776 9860:  Defined DCE_AUD_DEBUG in audit.h.  Removed
 * 	debug_mode and PRE_BETA_DEBUG or replaced it with DCE_DEBUG.
 * 	[1994/02/22  16:42:22  luan]
 * 
 * Revision 1.1.2.2  1994/01/25  04:58:17  luan
 * 	CR 9810: Changed to view audit-record format version
 * 	number "0" as don't care.  dce_aud_next() will return
 * 	matching record with any version number if the input
 * 	"format" parameter is "0".
 * 	[1994/01/25  04:52:42  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:39:33  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:43  zeliff]
 * 
 * $EndLog$
 */

/*
** evt_read.c
** Audit trail analysis API.
*/

#include <evtp.h>
#include <audit_alloc.h>

typedef struct	_index_file {
		unsigned32    sn;   /* the starting position of the record in the index file */
		unsigned32    ln;   /* the length of the record in the index file	     */
	struct _index_file   *next; /* next starting position in the index trail file	     */

} index_file;
index_file  *ifile = NULL;

static char *host = NULL;
static int   record_counter = 0;

void
		dce_aud_next
		(
				 dce_aud_trail_t at,
				 char *predicate,
				 unsigned16 format,
				 dce_aud_rec_t * ard,
				 unsigned32 * status
) {
    idl_es_handle_t  h;
    unsigned32	     i;
    size_t	     prelen;
    idl_byte	    *ep;
    idl_ulong_int    esize;
    aud_esl_act_t    actions;
    dce_aud_hdr_t   *header;
    char	    *commap;
    unsigned32	     authz_status;
    unsigned32	     evt_id, evt_outcome;
    unsigned32	     rec_start_pos;
    utc_t	     before_time, after_time;
    uuid_t	     server_uuid, client_uuid, cell_uuid, group_uuid;
    boolean32	     check_evt, check_outcome, check_authz_status;
    boolean32	     check_time_before, check_time_after, check_server;
    boolean32	     check_client, check_cell, check_group, check_addr;
    unsigned_char_t *addr;
    enum utc_cmptype time_relation;
    char	    *local_predicate = NULL;
    char	    *orig_predicate;

    if (at == NULL) {
	dce_svc_printf(AUD_S_INVALID_TRAIL_DESCRIPTOR_MSG);
	*status = aud_s_invalid_trail_descriptor;
	return;
    }

    *status = aud_s_ok;
    *ard = NULL;

    check_evt = check_outcome = check_authz_status = check_time_before
	= check_time_after = check_server = check_client = check_cell
	= check_group = check_addr = FALSE;

    prelen = strlen(predicate);
    if (predicate != NULL && prelen != 0) {
	local_predicate = malloc(prelen + 1);
	if (local_predicate == NULL) {
	    dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, prelen + 1);
	    *status = aud_s_cannot_allocate_memory;
	    return;
	}
	strcpy(local_predicate, predicate);
	orig_predicate = local_predicate;
    }

    while (local_predicate != NULL && strlen(local_predicate) != 0) {
	commap = (char *) strchr(local_predicate, ',');
	if (commap != NULL) {
	    *commap = '\0';
	}
	if (strncmp(local_predicate, "EVENT:", 6) == 0 ||
	    strncmp(local_predicate, "EVENT=", 6) == 0) {
	    evt_id = strtoul(local_predicate + 6, NULL, 16);
	    check_evt = TRUE;
	} else if (strncmp(local_predicate, "OUTCOME:", 8) == 0 ||
		   strncmp(local_predicate, "OUTCOME=", 8) == 0) {
	    if (strcmp(local_predicate + 8, "SUCCESS") == 0) {
		evt_outcome = aud_c_esl_cond_success;
		check_outcome = TRUE;
	    } else if (strcmp(local_predicate + 8, "FAILURE") == 0) {
		evt_outcome = aud_c_esl_cond_failure;
		check_outcome = TRUE;
	    } else if (strcmp(local_predicate + 8, "DENIAL") == 0) {
		evt_outcome = aud_c_esl_cond_denial;
		check_outcome = TRUE;
	    } else if (strcmp(local_predicate + 8, "PENDING") == 0) {
		evt_outcome = aud_c_esl_cond_pending;
		check_outcome = TRUE;
	    }
	} else if (strncmp(local_predicate, "STATUS:", 7) == 0 ||
		   strncmp(local_predicate, "STATUS=", 7) == 0) {
	    if (strcmp(local_predicate + 7, "DCE") == 0) {
		authz_status = rpc_c_authz_dce;
		check_authz_status = TRUE;
	    } else if (strcmp(local_predicate + 7, "NAME") == 0) {
		authz_status = rpc_c_authz_name;
		check_authz_status = TRUE;
	    }
	} else if (strncmp(local_predicate, "TIME>", 5) == 0) {
	    utc_mkasctime(&after_time, local_predicate + 5);
	    check_time_after = TRUE;
	} else if (strncmp(local_predicate, "TIME:>", 6) == 0 ||
		   strncmp(local_predicate, "TIME=>", 6) == 0) {
	    utc_mkasctime(&after_time, local_predicate + 6);
	    check_time_after = TRUE;
	} else if (strncmp(local_predicate, "TIME<", 5) == 0) {
	    utc_mkasctime(&before_time, local_predicate + 5);
	    check_time_before = TRUE;
	} else if (strncmp(local_predicate, "TIME:<", 6) == 0 ||
		   strncmp(local_predicate, "TIME=<", 6) == 0) {
	    utc_mkasctime(&before_time, local_predicate + 6);
	    check_time_before = TRUE;
	} else if (strncmp(local_predicate, "SERVER:", 7) == 0 ||
		   strncmp(local_predicate, "SERVER=", 7) == 0) {
	    uuid_from_string((unsigned_char_t *)local_predicate + 7, &server_uuid, status);
	    check_server = TRUE;
	} else if (strncmp(local_predicate, "CLIENT:", 7) == 0 ||
		   strncmp(local_predicate, "CLIENT=", 7) == 0) {
	    uuid_from_string((unsigned_char_t *)local_predicate + 7, &client_uuid, status);
	    check_client = TRUE;
	} else if (strncmp(local_predicate, "CELL:", 5) == 0 ||
		   strncmp(local_predicate, "CELL=", 5) == 0) {
	    uuid_from_string((unsigned_char_t *)local_predicate + 5, &cell_uuid, status);
	    check_cell = TRUE;
	} else if (strncmp(local_predicate, "GROUP:", 6) == 0 ||
		   strncmp(local_predicate, "GROUP=", 6) == 0) {
	    uuid_from_string((unsigned_char_t *)local_predicate + 6, &group_uuid, status);
	    check_group = TRUE;
	} else if (strncmp(local_predicate, "ADDR:", 5) == 0 ||
		   strncmp(local_predicate, "ADDR=", 5) == 0) {
	    addr = (unsigned_char_t *) malloc(strlen(local_predicate) - 5 + 1);
	    if (addr == NULL) {
		dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, strlen(local_predicate) - 5 + 1);
		*status = aud_s_cannot_allocate_memory;
		if (local_predicate)
		    free(orig_predicate);
		return;
	    }
	    strcpy((char *)addr, local_predicate + strlen("ADDR"));
	    check_addr = TRUE;
	}
	if (commap != NULL) {
	    local_predicate = commap + 1;
	} else {
	    free(orig_predicate);
	    local_predicate = NULL;
	}
    }

#ifdef DCE_AUD_DEBUG
    if (check_evt) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_next: Check event\n"));
    }
    if (check_outcome) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_next: Check outcome\n"));
    }
    if (check_authz_status) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_next: Check authz_status\n"));
    }
    if (check_time_before) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_next: Check time_before\n"));
    }
    if (check_time_after) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_next: Check time after\n"));
    }
    if (check_server) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_next: Check server\n"));
    }
    if (check_client) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_next: Check client\n"));
    }
    if (check_cell) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_next: Check cell\n"));
    }
    if (check_group) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_next: Check group\n"));
    }
    if (check_addr) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_next: Check addr\n"));
    }
#endif /* DCE_AUD_DEBUG */

next_record:
    if (*ard != NULL) {
	dce_aud_discard(*ard, status);
    };
    /* Use the index_cursor to traverse throught the index file.  Also */
    /* reset the file pointers when trails have been wrapped around.   */
    for (;;) {
	if (fread((void *)&rec_start_pos, sizeof(idl_byte),
		  sizeof(rec_start_pos), at->md_index_fp)
	    != sizeof(rec_start_pos))
	{
	    if (local_predicate != NULL)
		free( orig_predicate);
	    if (check_addr)
		free(addr);
	    *ard = NULL;
	    *status = aud_s_ok;
	    return;
	} else {
	    if (rec_start_pos >= at->index_cursor) {
		if (fread((void *)&esize, sizeof(idl_byte), sizeof(esize),
		    at->md_index_fp) != sizeof(esize))
		{
		    if (local_predicate != NULL)
			free(orig_predicate);
		    if (check_addr)
			free(addr);
		    *ard = NULL;
		    *status = aud_s_ok;
		    return;
		} else {
		    if (at->index_cursor != rec_start_pos) {
			fseek(at->trail_fp, rec_start_pos, SEEK_SET);
		    }

		    /*********************************************************/
		    /* the record could not be read probably because it is   */
		    /* still in the writer's buffer. Rewind the file pointer */
		    /* so that the next read will resume on this record.     */
		    /*********************************************************/

		    if (at->index_cursor + esize > at->file_size_limit_value) {
		       aud_trl_file_flush(at);
		       fseek(at->md_index_fp, 0-(2*sizeof(esize)), SEEK_CUR);
		       *ard = NULL;
		       *status = aud_s_ok;
		       if (local_predicate != NULL)
			  free(orig_predicate);
		       if (check_addr)
			  free(addr);
		       return;
		    } /* endif */
		    at->index_cursor = rec_start_pos + esize;
		    break;
		}
	    } else {
		    fseek(at->md_index_fp, sizeof(esize), SEEK_CUR);
	    }
	}
    }
#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_next: next record is %ld bytes\n", esize));
#endif
    ep = (idl_byte *) malloc(esize);
    if (ep == NULL) {
	dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, esize);
	*status = aud_s_cannot_allocate_memory;
	if (local_predicate)
	    free(orig_predicate);
	if (check_addr)
	    free(addr);
	return;
    }
    if (fread(ep, sizeof(idl_byte), esize, at->trail_fp) != esize) {
	/****************************************************************/
	/* The record could not be read probably because it is still in */
	/* writer's bufer.  Rewind the file pointers, so the next read  */
	/* can resume on this record					*/
	/****************************************************************/
	aud_trl_file_flush(at);
	fseek(at->trail_fp, 0-esize, SEEK_CUR);
	fseek(at->md_index_fp, 0-(2*sizeof(esize)), SEEK_CUR);
	*ard = NULL;
	*status = aud_s_ok;
	dce_svc_printf(AUD_S_TRAIL_FILE_CORRUPTED_MSG, at->trail_file);
	*status = aud_s_trail_file_corrupted;
	free(ep);
	if (local_predicate != NULL)
	    free(orig_predicate);
	if (check_addr)
	    free(addr);
	return;
    }
    idl_es_decode_buffer(ep, esize, &h, status);
    if (BAD_STATUS(status)) {
#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_next: idl_es_decode_buffer failed\n"));
#endif
	if (local_predicate != NULL)
	    free(orig_predicate);
	if (check_addr)
	    free(addr);
	free(ep);
	return;
    }
    *ard = (dce_aud_rec_t) malloc(sizeof(struct _dce_aud_rec_handle));
    if (*ard == NULL) {
	idl_es_handle_free(&h, status);
	if (BAD_STATUS(status)) {
	    if (local_predicate != NULL)
		free(orig_predicate);
	    if (check_addr)
		free(addr);
	    free(ep);
	    return;
	}
	if (local_predicate != NULL)
	    free(orig_predicate);
	if (check_addr)
	    free(addr);
	free(ep);
	dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG,
		sizeof(struct _dce_aud_rec_handle));
	*status = aud_s_cannot_allocate_memory;
	return;
    }
    (*ard)->as = NULL;

    audit_pickle_dencode_ev_info(h, &(*ard)->header, &(*ard)->tail, status);

    if (BAD_STATUS(status)) {
	if (local_predicate != NULL)
	    free(orig_predicate);
	if (check_addr)
	    free(addr);
	idl_es_handle_free(&h, status);
	free(ep);
	return;
    }

    idl_es_handle_free(&h, status);
    if (BAD_STATUS(status)) {
	if (local_predicate != NULL)
	    free(orig_predicate);
	if (check_addr)
	    free(addr);
	free(ep);
	return;
    }

    free(ep);

    header = &((*ard)->header);
    if (format != 0 && format != header->format) {
	goto next_record;
    }
    if ((check_evt && evt_id != header->event) ||
	(check_outcome && evt_outcome != header->outcome) ||
	(check_authz_status && authz_status != header->authz_st) ||
	(check_server && !uuid_equal(&server_uuid, &header->server, status)) ||
	(check_client && !uuid_equal(&client_uuid, &header->client_id[0].client, status)) ||
	(check_cell   && !uuid_equal(&cell_uuid, &header->client_id[0].cell, status)) ||
	(check_addr   && strcmp((char *)addr, (char *)header->addr) == 0)) {
	goto next_record;
    }
    if (check_time_before) {
	utc_cmpintervaltime(&time_relation, &header->time, &before_time);
	if (time_relation == utc_greaterThan) {
	    goto next_record;
	}
    }
    if (check_time_after) {
	utc_cmpintervaltime(&time_relation, &header->time, &after_time);
	if (time_relation == utc_lessThan) {
	    goto next_record;
	}
    }
    if (check_group) {
	for (i = 0; i < header->client_id[0].num_groups; i++) {
	    if (uuid_equal(&header->client_id[0].groups[i],
				&group_uuid, status)) {
		break;
	    }
	}
	if (i == header->client_id[0].num_groups) {
	    goto next_record;
	}
    }
#ifdef DCE_AUD_DEBUG
    DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	"dce_aud_next: found a matching record\n"));
#endif
    (*ard)->tail_read_cursor = 0;
    *status = aud_s_ok;
    return;
}

void
		dce_aud_prev
		(
				 dce_aud_trail_t at,
				 char *predicate,
				 unsigned16 format,
				 dce_aud_rec_t * ard,
				 unsigned32 * status
) {
    idl_es_handle_t  h;
    int 	     i;
    size_t	     prelen;
    idl_byte	    *ep;
    idl_ulong_int    esize;
    dce_aud_hdr_t   *header;
    char	    *commap;
    unsigned32	     authz_status;
    unsigned32	     evt_id, evt_outcome;
    unsigned32	     rec_start_pos;
    utc_t	     before_time, after_time;
    uuid_t	     server_uuid, client_uuid, cell_uuid, group_uuid;
    boolean32	     check_evt, check_outcome, check_authz_status;
    boolean32	     check_time_before, check_time_after, check_server;
    boolean32	     check_client, check_cell, check_group, check_addr;
    unsigned_char_t *addr;
    enum utc_cmptype time_relation;
    char	    *local_predicate = NULL;
    char	    *orig_predicate;
    index_file	    *tfile = NULL;

    if (at == NULL) {
	dce_svc_printf(AUD_S_INVALID_TRAIL_DESCRIPTOR_MSG);
	*status = aud_s_invalid_trail_descriptor;
	return;
    }

    *status = aud_s_ok;
    *ard = NULL;

    check_evt = check_outcome = check_authz_status = check_time_before
	= check_time_after = check_server = check_client = check_cell
	= check_group = check_addr = FALSE;

    prelen = strlen(predicate);
    if (predicate != NULL && prelen != 0) {
	local_predicate = malloc(prelen + 1);
	if (local_predicate == NULL) {
	    dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, prelen + 1);
	    *status = aud_s_cannot_allocate_memory;
	    return;
	}
	strcpy(local_predicate, predicate);
	orig_predicate = local_predicate;
    }

    /* parse the predicate values */
    while (local_predicate != NULL && strlen(local_predicate) != 0) {
	commap = (char *) strchr(local_predicate, ',');
	if (commap != NULL) {
	    *commap = '\0';
	}
	if (strncmp(local_predicate, "EVENT:", 6) == 0 ||
	    strncmp(local_predicate, "EVENT=", 6) == 0) {
	    evt_id = strtol(local_predicate + 6, NULL, 16);
	    check_evt = TRUE;
	} else if (strncmp(local_predicate, "OUTCOME:", 8) == 0 ||
		   strncmp(local_predicate, "OUTCOME=", 8) == 0) {
	    if (strcmp(local_predicate + 8, "SUCCESS") == 0) {
		evt_outcome = aud_c_esl_cond_success;
		check_outcome = TRUE;
	    } else if (strcmp(local_predicate + 8, "FAILURE") == 0) {
		evt_outcome = aud_c_esl_cond_failure;
		check_outcome = TRUE;
	    } else if (strcmp(local_predicate + 8, "DENIAL") == 0) {
		evt_outcome = aud_c_esl_cond_denial;
		check_outcome = TRUE;
	    } else if (strcmp(local_predicate + 8, "PENDING") == 0) {
		evt_outcome = aud_c_esl_cond_pending;
		check_outcome = TRUE;
	    }
	} else if (strncmp(local_predicate, "STATUS:", 7) == 0 ||
		   strncmp(local_predicate, "STATUS=", 7) == 0) {
	    if (strcmp(local_predicate + 7, "DCE") == 0) {
		authz_status = rpc_c_authz_dce;
		check_authz_status = TRUE;
	    } else if (strcmp(local_predicate + 7, "NAME") == 0) {
		authz_status = rpc_c_authz_name;
		check_authz_status = TRUE;
	    }
	} else if (strncmp(local_predicate, "TIME>", 5) == 0) {
	    utc_mkasctime(&after_time, local_predicate + 5);
	    check_time_after = TRUE;
	} else if (strncmp(local_predicate, "TIME:>", 6) == 0 ||
		   strncmp(local_predicate, "TIME=>", 6) == 0) {
	    utc_mkasctime(&after_time, local_predicate + 6);
	    check_time_after = TRUE;
	} else if (strncmp(local_predicate, "TIME<", 5) == 0) {
	    utc_mkasctime(&before_time, local_predicate + 5);
	    check_time_before = TRUE;
	} else if (strncmp(local_predicate, "TIME:<", 6) == 0 ||
		   strncmp(local_predicate, "TIME=<", 6) == 0) {
	    utc_mkasctime(&before_time, local_predicate + 6);
	    check_time_before = TRUE;
	} else if (strncmp(local_predicate, "SERVER:", 7) == 0 ||
		   strncmp(local_predicate, "SERVER=", 7) == 0) {
	    uuid_from_string((unsigned_char_t *)local_predicate + 7, &server_uuid, status);
	    check_server = TRUE;
	} else if (strncmp(local_predicate, "CLIENT:", 7) == 0 ||
		   strncmp(local_predicate, "CLIENT=", 7) == 0) {
	    uuid_from_string((unsigned_char_t *)local_predicate + 7, &client_uuid, status);
	    check_client = TRUE;
	} else if (strncmp(local_predicate, "CELL:", 5) == 0 ||
		   strncmp(local_predicate, "CELL=", 5) == 0) {
	    uuid_from_string((unsigned_char_t *)local_predicate + 5, &cell_uuid, status);
	    check_cell = TRUE;
	} else if (strncmp(local_predicate, "GROUP:", 6) == 0 ||
		   strncmp(local_predicate, "GROUP=", 6) == 0) {
	    uuid_from_string((unsigned_char_t *)local_predicate + 6, &group_uuid, status);
	    check_group = TRUE;
	} else if (strncmp(local_predicate, "ADDR:", 5) == 0 ||
		   strncmp(local_predicate, "ADDR=", 5) == 0) {
	    addr = (unsigned_char_t *) malloc(strlen(local_predicate) - 5 + 1);
	    if (addr == NULL) {
		dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, strlen(local_predicate) - 5 + 1);
		*status = aud_s_cannot_allocate_memory;
		if (local_predicate)
		    free(orig_predicate);
		return;
	    }
	    strcpy((char *)addr, local_predicate + strlen("ADDR"));
	    check_addr = TRUE;
	}
	if (commap != NULL) {
	    local_predicate = commap + 1;
	} else {
	    free(orig_predicate);
	    local_predicate = NULL;
	}
    }

#ifdef DCE_AUD_DEBUG
    if (check_evt) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_prev: Check event\n"));
    }
    if (check_outcome) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_prev: Check outcome\n"));
    }
    if (check_authz_status) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_prev: Check authz_status\n"));
    }
    if (check_time_before) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_prev: Check time_before\n"));
    }
    if (check_time_after) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_prev: Check time after\n"));
    }
    if (check_server) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_prev: Check server\n"));
    }
    if (check_client) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_prev: Check client\n"));
    }
    if (check_cell) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_prev: Check cell\n"));
    }
    if (check_group) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_prev: Check group\n"));
    }
    if (check_addr) {
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_prev: Check addr\n"));
    }
#endif /* DCE_AUD_DEBUG */

prev_record:
    if (*ard != NULL) {
	dce_aud_discard(*ard, status);
    };

    /* Since the md_index file contains (s1, l1),(s2, l2),...,(sn, ln) where si is   */
    /* the starting point of the event record and li is the length of the event      */
    /* record, we must reverse the index and store them in a linked list to retrieve */
    /* them in LIFO order, i.e (sn, ln),(sn-1, ln-1),...,(s2, l2),(s1, l1).	     */

    if (!ifile) {
        /* Use the index_cursor to traverse throught the index file.  Also */
        /* reset the file pointers when trails have been wrapped around.   */
	while (1) {
	    fread((void *)&rec_start_pos, sizeof(idl_byte), sizeof(rec_start_pos), at->md_index_fp);
            /* terminate the loop if eof or error on reading */
	    if (ferror(at->md_index_fp) || feof(at->md_index_fp))
		break;
	    if (rec_start_pos >= at->index_cursor) {
		fread((void *)&esize, sizeof(idl_byte), sizeof(esize), at->md_index_fp);
		if (ferror(at->md_index_fp) || feof(at->md_index_fp))
		    break;
		if (at->index_cursor != rec_start_pos) {
		    fseek(at->trail_fp, rec_start_pos, SEEK_SET);
		}
		at->index_cursor = rec_start_pos + esize;

		tfile = (index_file *) malloc(sizeof(index_file));
		if (tfile == NULL) {
		    dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, sizeof(index_file));
		    *status = aud_s_cannot_allocate_memory;
		    if (local_predicate)
			free(orig_predicate);
		    if (check_addr)
			free(addr);
		    return;
		}
		/* add into the linked list */
		tfile->sn = rec_start_pos;
		tfile->ln = esize;
		tfile->next = ifile;
		ifile = tfile;

	    } else {
		fseek(at->md_index_fp, sizeof(esize), SEEK_CUR);
	    }
	}
	free(tfile);
    }

    /* check to make sure the linked list is not empty */
    if (ifile == NULL) {
	if (local_predicate)
	    free(orig_predicate);
	if (check_addr)
	    free(addr);
	*ard = NULL;
	*status = aud_s_ok;
	return;
    }

    /* set the starting pointer to the starting point of */
    /* the previous last written audit record		 */
    at->trail_cursor = ifile->sn;
    esize	     = ifile->ln;
    ifile	     = ifile->next;
    fseek(at->trail_fp, at->trail_cursor, SEEK_SET);

#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_prev: sn is %ld bytes\n", at->index_cursor));
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_prev: ln is %ld bytes\n", esize));
#endif
    ep = (idl_byte *) malloc(esize);
    if (ep == NULL) {
	dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, esize);
	*status = aud_s_cannot_allocate_memory;
	if (local_predicate)
	    free(orig_predicate);
	if (check_addr)
	    free(addr);
	ifile = NULL;
	return;
    }
    if (fread(ep, sizeof(idl_byte), esize, at->trail_fp) != esize) {
	dce_svc_printf(AUD_S_TRAIL_FILE_CORRUPTED_MSG, at->trail_file);
	*status = aud_s_trail_file_corrupted;
	free(ep);
	if (local_predicate)
	    free(orig_predicate);
	if (check_addr)
	    free(addr);
	ifile = NULL;
	return;
    }
    idl_es_decode_buffer(ep, esize, &h, status);
    if (BAD_STATUS(status)) {
#ifdef DCE_AUD_DEBUG
	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	    "dce_aud_prev: idl_es_decode_buffer failed\n"));
#endif
	if (local_predicate)
	    free(orig_predicate);
	if (check_addr)
	    free(addr);
	ifile = NULL;
	free(ep);
	return;
    }
    *ard = (dce_aud_rec_t) malloc(sizeof(struct _dce_aud_rec_handle));
    if (*ard == NULL) {
	idl_es_handle_free(&h, status);
	if (BAD_STATUS(status)) {
	    if (local_predicate)
		free(orig_predicate);
	    if (check_addr)
		free(addr);
	    ifile = NULL;
	    free(ep);
	    return;
	}
	if (local_predicate)
	    free(orig_predicate);
	if (check_addr)
	    free(addr);
	ifile = NULL;
	free(ep);
	dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG,
		sizeof(struct _dce_aud_rec_handle));
	*status = aud_s_cannot_allocate_memory;
	return;
    }
    (*ard)->as = NULL;
    audit_pickle_dencode_ev_info(h, &(*ard)->header, &(*ard)->tail, status);
    if (BAD_STATUS(status)) {
	if (local_predicate)
	    free(orig_predicate);
	if (check_addr)
	    free(addr);
	ifile = NULL;
	idl_es_handle_free(&h, status);
	free(ep);
	return;
    }

    idl_es_handle_free(&h, status);
    if (BAD_STATUS(status)) {
	if (local_predicate)
	    free(orig_predicate);
	if (check_addr)
	    free(addr);
	ifile = NULL;
	free(ep);
	return;
    }

    free(ep);

    header = &((*ard)->header);
    if (format != 0 && format != header->format) {
	goto prev_record;
    }
    if ((check_evt && evt_id != header->event) ||
	(check_outcome && evt_outcome != header->outcome) ||
	(check_authz_status && authz_status != header->authz_st) ||
	(check_server && !uuid_equal(&server_uuid, &header->server, status)) ||
	(check_client && !uuid_equal(&client_uuid, &header->client_id[0].client, status)) ||
	(check_cell && !uuid_equal(&cell_uuid, &header->client_id[0].cell, status)) ||
	(check_addr && strcmp((char *)addr, (char *)header->addr) == 0)) {
	goto prev_record;
    }
    if (check_time_before) {
	utc_cmpintervaltime(&time_relation, &header->time, &before_time);
	if (time_relation == utc_greaterThan) {
	    goto prev_record;
	}
    }
    if (check_time_after) {
	utc_cmpintervaltime(&time_relation, &header->time, &after_time);
	if (time_relation == utc_lessThan) {
	    goto prev_record;
	}
    }
    if (check_group) {
	for (i = 0; i < header->client_id[0].num_groups; i++) {
	    if (uuid_equal(&header->client_id[0].groups[i],
				&group_uuid, status)) {
		break;
	    }
	}
	if (i == header->client_id[0].num_groups) {
	    goto prev_record;
	}
    }
#ifdef DCE_AUD_DEBUG
    DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug1,
	"dce_aud_prev: found a matching record\n"));
#endif
    if (local_predicate)
	free(orig_predicate);
    if (check_addr)
	free(addr);
    (*ard)->tail_read_cursor = 0;
    *status = aud_s_ok;
    return;
}

void
		dce_aud_get_header
		(
				 dce_aud_rec_t ard,
				 dce_aud_hdr_t ** header,
				 unsigned32 * status
) {
    int 	    id_cnt, group_cnt;

    if (ard == NULL) {
	*status = aud_s_invalid_record_descriptor;
	return;
    }
    *header = (dce_aud_hdr_t *) malloc(sizeof(dce_aud_hdr_t));
    if (*header == NULL) {
       dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG,sizeof(dce_aud_hdr_t));
       *status = aud_s_cannot_allocate_memory;
       return;
    }

    **header = ard->header;
    id_cnt = ard->header.num_client_ids;
    (*header)->num_client_ids = id_cnt;
    (*header)->client_id = (client_id_t *) malloc(sizeof(client_id_t) * id_cnt);
    if ((*header)->client_id == NULL) {
       dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG,sizeof(client_id_t) * id_cnt);
       *status = aud_s_cannot_allocate_memory;
       return;
    }

    for (; id_cnt-- > 0; ) {
	(*header)->client_id[id_cnt] = ard->header.client_id[id_cnt];
	group_cnt = ard->header.client_id[id_cnt].num_groups;
	(*header)->client_id[id_cnt].num_groups = group_cnt;
	(*header)->client_id[id_cnt].groups =
		(uuid_t *) malloc(sizeof(uuid_t) * group_cnt);
	for (; group_cnt-- > 0; ) {
	    (*header)->client_id[id_cnt].groups[group_cnt] =
		ard->header.client_id[id_cnt].groups[group_cnt];
	}
    }

    if (ard->header.addr != NULL) {
	(*header)->addr = (unsigned_char_t *) malloc(strlen((char *)ard->header.addr) + 1);
	if ((*header)->addr == NULL) {
	   dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG,strlen((char *)ard->header.addr)+1);
	   *status = aud_s_cannot_allocate_memory;
	   return;
	}
	strcpy((char *) (*header)->addr, (char *) ard->header.addr);
    }
    *status = aud_s_ok;
    return;
}

void
		dce_aud_get_ev_info
		(
				 dce_aud_rec_t ard,
				 dce_aud_ev_info_t ** ev_info,
				 unsigned32 * status
) {
    aud_log_ev_info_t *evip;
    dce_aud_ev_info_t *info;

    if (ard == NULL) {
	*status = aud_s_invalid_record_descriptor;
	dce_svc_printf(AUD_S_INVALID_RECORD_DESCRIPTOR_MSG);
	return;
    }

    *status = aud_s_ok;

    if (ard->tail == NULL) {
	*ev_info = NULL;
	return;
    }

    /* return if it reaches the last item */
    if (ard->tail_read_cursor >= ard->tail->item_count) {
	*ev_info = NULL;
	return;
    }
    *ev_info = (dce_aud_ev_info_t *) malloc(sizeof(dce_aud_ev_info_t));
    if (*ev_info == NULL) {
        dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, sizeof(dce_aud_ev_info_t));
        *status = aud_s_cannot_allocate_memory;
        return;
    }
    info = *ev_info;

    evip = &ard->tail->info_item[ard->tail_read_cursor++];
    info->format = evip->format;
    switch (info->format) {

    case aud_c_evt_info_small_int:
	info->data.small_int = evip->tagged_union.small_int;
	break;

    case aud_c_evt_info_short_int:
	info->data.short_int = evip->tagged_union.short_int;
	break;

    case aud_c_evt_info_long_int:
	info->data.long_int = evip->tagged_union.long_int;
	break;

    case aud_c_evt_info_hyper_int:
	info->data.hyper_int = *evip->tagged_union.hyper_int;
	break;

    case aud_c_evt_info_usmall_int:
	info->data.usmall_int = evip->tagged_union.usmall_int;
	break;

    case aud_c_evt_info_ushort_int:
	info->data.ushort_int = evip->tagged_union.ushort_int;
	break;

    case aud_c_evt_info_ulong_int:
	info->data.ulong_int = evip->tagged_union.ulong_int;
	break;

    case aud_c_evt_info_uhyper_int:
	info->data.uhyper_int = *evip->tagged_union.uhyper_int;
	break;

    case aud_c_evt_info_short_float:
	info->data.short_float = evip->tagged_union.short_float;
	break;

    case aud_c_evt_info_long_float:
	info->data.long_float = *evip->tagged_union.long_float;
	break;

    case aud_c_evt_info_boolean:
	info->data.boolean = evip->tagged_union.bool;
	break;

    case aud_c_evt_info_uuid:
	info->data.uuid = *evip->tagged_union.uuid;
	break;

    case aud_c_evt_info_utc:
	info->data.utc = *evip->tagged_union.utc;
	break;

    case aud_c_evt_info_acl:
	{
	    sec_acl_t	   *new_acl;
	    aud__log_dup_acl(evip->tagged_union.acl, &new_acl);
	    info->data.acl = new_acl;
	}
	break;


    case aud_c_evt_info_byte_string: /* XXX This looks like a bug */
	{
	    idl_byte	   *cp = (idl_byte *)
	    malloc(strlen((char *)evip->tagged_union.byte_string) + 1);
	    strcpy((char *)cp, (char *)evip->tagged_union.byte_string);
	    info->data.byte_string = cp;
	}
	break;

    case aud_c_evt_info_char_string:
	{
	    idl_char	   *cp = (idl_char *)
	    malloc(strlen((char *)evip->tagged_union.char_string) + 1);
	    strcpy((char *)cp, (char *)evip->tagged_union.char_string);
	    info->data.char_string = cp;
	}
	break;

    default:
	;
    }
    return;
}

unsigned32
		dce_aud_length
		(
				 dce_aud_rec_t ard,
				 unsigned32 * status
) {
    idl_es_handle_t h;
    idl_byte	   *ep;
    idl_ulong_int   esize;
    boolean32	    enabled_allocator;

    if (ard == NULL) {
	*status = aud_s_invalid_record_descriptor;
	dce_svc_printf(AUD_S_INVALID_RECORD_DESCRIPTOR_MSG);
	return 0;
    }

    idl_es_encode_dyn_buffer(&ep, &esize, &h, status);
    if (BAD_STATUS(status)) return 0;

    AUD_SET_ALLOC_FREE(enabled_allocator);
    audit_pickle_dencode_ev_info(h, &ard->header, &ard->tail, status);
    AUD_RESET_ALLOC_FREE(enabled_allocator, ep);

    if (BAD_STATUS(status)) {
        idl_es_handle_free(&h, status);
        return 0;
    }

    idl_es_handle_free(&h, status);
    if (BAD_STATUS(status)) return 0;

    return (esize);
}

void
		dce_aud_print
		(
				 dce_aud_rec_t ard,
				 unsigned32 flags,
				 unsigned_char_t **buffer,
				 unsigned32 * status
)
{
    error_status_t	    stp;
    dce_aud_hdr_t	   *header_p;
    aud_log_ev_info_list_t *tail;
    unsigned32		    i, j;
    client_id_t 	   *id_p;
    int 		    id_count, num_entries;
    unsigned_char_t	   *sp;
    char                    event_name[80];
    char		    buf[100];
    sec_rgy_name_t	    subject_name;
    boolean32		    output_uuids = TRUE;
    char		   *strbuf;
    int 		    index;
    sec_login_handle_t	    login_context;
    sec_login_net_info_t    net_info;
    boolean32		    found = FALSE;

    if (ard == NULL) {
	*status = aud_s_invalid_record_descriptor;
	dce_svc_printf(AUD_S_INVALID_RECORD_DESCRIPTOR_MSG);
	return;
    }

    strbuf = (char *) malloc(EVENT_BUF_SIZE);
    if (strbuf == NULL) {
	dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, EVENT_BUF_SIZE);
	*buffer = NULL;
	*status = aud_s_cannot_allocate_memory;
	return;
    }

    if ((flags & aud_c_evt_raw_info) == 0) {
	output_uuids = FALSE;
    }

    /* get the hostname of the auditd */
    if (host == NULL)
	dce_cf_get_host_name(&host, &stp);

    header_p = &ard->header;

    /* AIX sprintf seems to have a bug.  It returns the starting address of
       the string, instead of the number of characters in the output string.
       Using strlen() here as a get around, until AIX is fixed.
     */

    record_counter++;
    sprintf(strbuf, "\n--- Event Record number %i ---\n", record_counter);
    index = strlen(strbuf);
    sprintf(strbuf+index, "o Event Information:\n");
    index = strlen(strbuf);
    sprintf(strbuf+index, "   %-27s %#lx /* %ld */\n",
	    "- Event Number:", header_p->event, header_p->event);
    index = strlen(strbuf);
    get_event_name_from_number(header_p->event, event_name);
    sprintf(strbuf+index, "   %-27s %s\n",
	    "- Event Name:", event_name);
    index = strlen(strbuf); 
    sprintf(strbuf+index, "   %-27s","- Event Outcome:");
    for (i = 0; i < MAX_CONDITIONS - 1; i++) {
	if ((audit_condition[i].number & header_p->outcome) != 0) {
	    index = strlen(strbuf);
	    sprintf(strbuf+index, " %s ", audit_condition[i].name);
	}
    }
    index = strlen(strbuf);
    sprintf(strbuf+index, "\n");
    index = strlen(strbuf);

    if (!output_uuids)
    {
       if (uuid_is_nil(&header_p->server, status))
       {
	   if (host == NULL)
	       sprintf(strbuf+index, "%-30s\n", "o Server: NULL");
	   else
	       sprintf(strbuf+index, "%-30s /.:/%s\n", "o Server:", host);
       }
       else
       {
	   /* We should never get here! */
	   unsigned_char_t 	*suuid;
	   uuid_to_string(&header_p->server, &suuid, status);
	   dce_svc_printf(AUD_S_GOT_SERVER_UUID_MSG, suuid);
       }
    }
    else
    {
	uuid_to_string(&header_p->server, &sp, status);
	sprintf(strbuf+index, "%-30s %s\n", "o Server UUID:", sp);
	rpc_string_free(&sp, status);
    }

    index = strlen(strbuf);

    for (id_count = 0; id_count < header_p->num_client_ids; id_count++) {
	id_p = &(header_p->client_id[id_count]);
	if (!output_uuids) {
	    sec_id_gen_name(NULL, &id_p->cell, &id_p->client,
			    subject_name, NULL, NULL, status);
	    if (BAD_STATUS(status)) {
		sprintf(strbuf+index, "%-30s %s\n",
			"o Client:", "Unknown client and cell uuids");
	    } else {
		sprintf(strbuf+index, "%-30s %s\n", "o Client:", subject_name);
	    }
	} else {
	    uuid_to_string(&id_p->client, &sp, status);
	    sprintf(strbuf+index, "%-30s %s\n", "o Client UUID", sp);
	    rpc_string_free(&sp, status);
	    uuid_to_string(&id_p->cell, &sp, status);
	    index = strlen(strbuf);
	    sprintf(strbuf+index, "%-30s %s\n", "o Cell UUID", sp);
	    rpc_string_free(&sp, status);
	}
	index = strlen(strbuf);
	if (!((id_p->num_groups == 1) && (uuid_is_nil(&id_p->groups[0], status))))
	{

	   if ((flags & aud_c_evt_groups_info) != 0) {
	       sprintf(strbuf+index, "%-30s %ld\n",
		       "o Number of groups:", id_p->num_groups);
	   }
	   if (id_p->num_groups != 0 && (flags & aud_c_evt_groups_info) != 0) {
	       if (output_uuids) {
		   for (i = 0; i < id_p->num_groups; i++) {
		       uuid_to_string(&id_p->groups[i], &sp, status);
		       index = strlen(strbuf);
		       sprintf(strbuf+index, "   %-27s %s\n", "o Group UUID:", sp);
		       rpc_string_free(&sp, status);
		   }
	       } else {
		   for (i = 0; i < id_p->num_groups; i++) {
		       found = FALSE;
		       index = strlen(strbuf);
		       sec_id_gen_group(NULL, &id_p->cell, &id_p->groups[i],
					subject_name, NULL, NULL, status);
		       if (BAD_STATUS(status)) {
			   sprintf(strbuf+index, "   - Unknown group uuid \n");
		       } else {
			   sprintf(strbuf+index, "   - Group %ld:                  %s\n", i, subject_name);
		       }
		   }
	       }
	   }
	}
	else
	{
	    sprintf(strbuf+index, "o Number of groups:            Nil\n");
	}

	/* It's enough to get the first id (initiator) in the chain
	   if the delegates info is not needed. */

	if ((flags & aud_c_evt_delegates_info) == 0) break;
    }
    index = strlen(strbuf);
    sprintf(strbuf+index, "%-30s", "o Authorization Status:");
    index = strlen(strbuf);
    if (header_p->authz_st == rpc_c_authz_name) {
	sprintf(strbuf+index, " Authorized with a name\n");
    } else if (header_p->authz_st == rpc_c_authz_dce) {
	sprintf(strbuf+index, " Authorized with a pac\n");
    }
    utc_asclocaltime(buf, 100, &header_p->time);
    index = strlen(strbuf);
    sprintf(strbuf+index, "%-30s %s\n", "o Date and Time recorded:", buf);
    index = strlen(strbuf);
    if (header_p->addr != NULL && (flags & aud_c_evt_address_info) != 0) {
	sprintf(strbuf+index, "%-30s %s\n", "o Client Address:", header_p->addr);
    }
    index = strlen(strbuf);
    if (ard->tail != NULL && (flags & aud_c_evt_specific_info) != 0) {
	tail = ard->tail;
	sprintf(strbuf+index, "o %ld Event(s) specific:\n", tail->item_count);
	for (i = 0; i < tail->item_count; i++) {
	    index = strlen(strbuf);
	    sprintf(strbuf+index, "   - item number %d %s", i + 1, "            ");
	    index = strlen(strbuf);
	    switch (tail->info_item[i].format) {
	    case aud_c_evt_info_small_int:
		sprintf(strbuf+index, "small int %d\n",
		       tail->info_item[i].tagged_union.small_int);
		break;
	    case aud_c_evt_info_short_int:
		sprintf(strbuf+index, "short int %d\n",
		       tail->info_item[i].tagged_union.short_int);
		break;
	    case aud_c_evt_info_long_int:
		sprintf(strbuf+index, "long int %ld\n",
		       tail->info_item[i].tagged_union.long_int);
		break;
	    case aud_c_evt_info_hyper_int:
		sprintf(strbuf+strlen(strbuf), "hyper int [1] %ld\n",
		       tail->info_item[i].tagged_union.hyper_int->high);
		sprintf(strbuf+strlen(strbuf), "hyper int [2] %ld\n",
		       tail->info_item[i].tagged_union.hyper_int->low);
		break;
	    case aud_c_evt_info_usmall_int:
		sprintf(strbuf+index, "usmall int %d\n",
		       tail->info_item[i].tagged_union.usmall_int);
		break;
	    case aud_c_evt_info_ushort_int:
		sprintf(strbuf+index, "ushort int %d\n",
		       tail->info_item[i].tagged_union.ushort_int);
		break;
	    case aud_c_evt_info_ulong_int:
		sprintf(strbuf+index, "ulong int %ld\n",
		       tail->info_item[i].tagged_union.ulong_int);
		break;
	    case aud_c_evt_info_uhyper_int:
		sprintf(strbuf+index, "uhyper int [1] %ld\n",
		     tail->info_item[i].tagged_union.uhyper_int->high);
		sprintf(strbuf+strlen(strbuf), "uhyper int [2] %ld\n",
		       tail->info_item[i].tagged_union.uhyper_int->low);
		break;
	    case aud_c_evt_info_short_float:
		sprintf(strbuf+index, "short float %f\n",
		       tail->info_item[i].tagged_union.short_float);
		break;
	    case aud_c_evt_info_long_float:
		sprintf(strbuf+index, "long float %lf\n",
		       *tail->info_item[i].tagged_union.long_float);
		break;
	    case aud_c_evt_info_boolean:
		sprintf(strbuf+index, "boolean info ");
		if (tail->info_item[i].tagged_union.bool == TRUE) {
		    sprintf(strbuf+strlen(strbuf), "TRUE\n");
		} else {
		    sprintf(strbuf+strlen(strbuf), "FALSE\n");
		}
		break;
	    case aud_c_evt_info_uuid:
		sprintf(strbuf+index, "uuid info ");
		uuid_to_string(tail->info_item[i].tagged_union.uuid,
			       &sp, status);
		if (sp != NULL) {
		    sprintf(strbuf+strlen(strbuf), "%s\n", sp);
		}
		rpc_string_free(&sp, status);
		break;
	    case aud_c_evt_info_utc:
		sprintf(strbuf+index, "utc info ");
		utc_asclocaltime(buf, 100,
				 tail->info_item[i].tagged_union.utc);
		sprintf(strbuf+strlen(strbuf), "%s\n", buf);
		break;
	    case aud_c_evt_info_acl:
		sprintf(strbuf+index, "acl info\n");
		uuid_to_string(&(tail->info_item[i].tagged_union.acl->default_realm.uuid),
			       &sp, status);
		if (sp != NULL) {
		    sprintf(strbuf+strlen(strbuf), "default realm uuid: %s\n", sp);
		    rpc_string_free(&sp, status);
		}

		sp = tail->info_item[i].tagged_union.acl->default_realm.name;
		if (sp != NULL) {
		    sprintf(strbuf+strlen(strbuf), "default realm name: %s\n", sp);
		}

		uuid_to_string(&(tail->info_item[i].tagged_union.acl->sec_acl_manager_type),
			       &sp, status);
		if (sp != NULL) {
		    sprintf(strbuf+strlen(strbuf), "acl manager type: %s\n", sp);
		    rpc_string_free(&sp, status);
		}

		num_entries = tail->info_item[i].tagged_union.acl->num_entries;
		sprintf(strbuf+strlen(strbuf), "number of entries: %d\n", num_entries);

		for (j = 0; j < num_entries; j++) {
		   switch (tail->info_item[i].tagged_union.acl->sec_acl_entries[j].entry_info.entry_type)
		{

		       case sec_acl_e_type_user_obj:
			  sprintf(strbuf+strlen(strbuf), "type user obj\n");
		       break;

		       case sec_acl_e_type_group_obj:
			  sprintf(strbuf+strlen(strbuf), "type group obj\n");
		       break;

		       case sec_acl_e_type_other_obj:
			  sprintf(strbuf+strlen(strbuf), "type other obj\n");
		       break;

		       case sec_acl_e_type_unauthenticated:
			  sprintf(strbuf+strlen(strbuf), "type unauthenticated\n");
		       break;

		       case sec_acl_e_type_any_other:
			  sprintf(strbuf+strlen(strbuf), "type any other\n");
		       break;

		       case sec_acl_e_type_user:
			  sprintf(strbuf+strlen(strbuf), "type user\n");

			  uuid_to_string(&(tail->info_item[i].tagged_union.acl->sec_acl_entries[j].entry_info.tagged_union.id.uuid),
					 &sp, status);
			  if (sp != NULL) {
			     sprintf(strbuf+strlen(strbuf), "uuid: %s\n", sp);
			     rpc_string_free(&sp, status);
			  }

			  sp = tail->info_item[i].tagged_union.acl->sec_acl_entries[j].entry_info.tagged_union.id.name;
			  if (sp != NULL) {
			     sprintf(strbuf+strlen(strbuf), "name: %s\n", sp);
			  }
		       break;

		       case sec_acl_e_type_group:
			  sprintf(strbuf+strlen(strbuf), "type group\n");
			  uuid_to_string(&(tail->info_item[i].tagged_union.acl->sec_acl_entries[j].entry_info.tagged_union.id.uuid),
					 &sp, status);
			  if (sp != NULL) {
			     sprintf(strbuf+strlen(strbuf), "uuid: %s\n", sp);
			     rpc_string_free(&sp, status);
			  }

			  sp = tail->info_item[i].tagged_union.acl->sec_acl_entries[j].entry_info.tagged_union.id.name;
			  if (sp != NULL) {
			     sprintf(strbuf+strlen(strbuf), "name: %s\n", sp);
			  }
		       break;

		       case sec_acl_e_type_foreign_other:
			  sprintf(strbuf+strlen(strbuf), "type foreign other\n");
			  uuid_to_string(&(tail->info_item[i].tagged_union.acl->sec_acl_entries[j].entry_info.tagged_union.id.uuid),
					 &sp, status);
			  if (sp != NULL) {
			     sprintf(strbuf+strlen(strbuf), "uuid: %s\n", sp);
			     rpc_string_free(&sp, status);
			  }

			  sp = tail->info_item[i].tagged_union.acl->sec_acl_entries[j].entry_info.tagged_union.id.name;
			  if (sp != NULL) {
			     sprintf(strbuf+strlen(strbuf), "name: %s\n", sp);
			  }
		       break;

		       case sec_acl_e_type_foreign_user:
			  sprintf(strbuf+strlen(strbuf), "type foreign user\n");
			  uuid_to_string(&(tail->info_item[i].tagged_union.acl->sec_acl_entries[j].entry_info.tagged_union.foreign_id.id.uuid),
					 &sp, status);
			  if (sp != NULL) {
			     sprintf(strbuf+strlen(strbuf), "uuid: %s\n", sp);
			     rpc_string_free(&sp, status);
			  }

			  sp = tail->info_item[i].tagged_union.acl->sec_acl_entries[j].entry_info.tagged_union.foreign_id.id.name;
			  if (sp != NULL) {
			     sprintf(strbuf+strlen(strbuf), "name: %s\n", sp);
			  }
		       break;

		       case sec_acl_e_type_foreign_group:
			  sprintf(strbuf+strlen(strbuf), "type foreign group\n");
			  uuid_to_string(&(tail->info_item[i].tagged_union.acl->sec_acl_entries[j].entry_info.tagged_union.foreign_id.id.uuid),
					 &sp, status);
			  if (sp != NULL) {
			     sprintf(strbuf+strlen(strbuf), "uuid: %s\n", sp);
			     rpc_string_free(&sp, status);
			  }

			  sp = tail->info_item[i].tagged_union.acl->sec_acl_entries[j].entry_info.tagged_union.foreign_id.id.name;
			  if (sp != NULL) {
			     sprintf(strbuf+strlen(strbuf), "name: %s\n", sp);
			  }
			  uuid_to_string(&(tail->info_item[i].tagged_union.acl->sec_acl_entries[j].entry_info.tagged_union.foreign_id.realm.uuid),
					 &sp, status);
			  if (sp != NULL) {
			     sprintf(strbuf+strlen(strbuf), "realm uuid: %s\n", sp);
			     rpc_string_free(&sp, status);
			  }

			  sp = tail->info_item[i].tagged_union.acl->sec_acl_entries[j].entry_info.tagged_union.foreign_id.realm.name;
			  if (sp != NULL) {
			     sprintf(strbuf+strlen(strbuf), "realm name: %s\n", sp);
			  }
		       break;

		       case sec_acl_e_type_extended:
		       default:
		       break;
		  }			  /* end of switch */
		  sprintf(strbuf+strlen(strbuf), "permission bits: %lX\n",
			  tail->info_item[i].tagged_union.acl->sec_acl_entries[j].perms);
		}			    /* end of for loop */

		break;
		break;
	    case aud_c_evt_info_byte_string:
		sprintf(strbuf+index, "byte string %s\n",
			tail->info_item[i].tagged_union.char_string);
		break;
	    case aud_c_evt_info_char_string:
		sprintf(strbuf+index, "char string %s\n",
		        tail->info_item[i].tagged_union.char_string);
		break;

	    default:
		break;
	    }
	}
    }
    index = strlen(strbuf);
    sprintf(strbuf+index, "--- End of Event record number %i --- \n\n\n", record_counter);
    *buffer = (unsigned_char_t *) malloc(strlen(strbuf)+1);
    if (*buffer == NULL) {
	dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG, strlen(strbuf)+1);
	*status = aud_s_cannot_allocate_memory;
	return;
    }
    memcpy((char *)*buffer, strbuf, strlen(strbuf)+1);
    free(strbuf);
    *status = aud_s_ok;
    return;
}
