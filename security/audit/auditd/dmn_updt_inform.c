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
 * $Log: dmn_updt_inform.c,v $
 * Revision 1.1.13.1  1996/07/08  18:26:14  arvind
 * 	Audit for 1.2.2
 * 	[1996/06/21  20:24 UTC  arvind  /main/arvind_audit122/1]
 *
 * 	Audit for 1.2.2
 * 	[1996/02/18  00:06:10  marty  1.1.10.3]
 *
 * Revision 1.1.10.3  1996/02/18  00:06:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:40  marty]
 * 
 * Revision 1.1.10.2  1995/12/08  17:38:37  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/07/26  18:23 UTC  jrr
 * 	Check for bad returns from open()
 * 	[1995/07/26  18:22 UTC  jrr  /main/HPDCE02/jrr_mothra_pure/1]
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/19  16:41 UTC  jrr
 * 	Merge work for CHFts15152.
 * 
 * 	HP revision /main/jrr_sec_chfts15152/1  1995/05/19  16:40 UTC  jrr
 * 	Create thread for client updates (CHFts15152).
 * 	[1995/12/08  17:17:38  root]
 * 
 * Revision 1.1.6.1  1994/10/26  15:07:05  weisz
 * 	CR 12725: add fclose.
 * 	[1994/10/26  15:06:48  weisz]
 * 
 * Revision 1.1.2.5  1994/08/08  18:52:23  weisz
 * 	CR 10516: remove parameters from messages going accross rpcs as return codes.
 * 	[1994/08/08  18:51:13  weisz]
 * 
 * Revision 1.1.2.4  1994/03/11  22:48:29  luan
 * 	CR 9775 9776: Changed debug messages to use dce_svc_printf() or DCE_SVC_DEBUG.
 * 	Added "rpc_mgmt_is_server_listening()" call to check if an audit client is
 * 	there to receive update notification.
 * 	[1994/03/11  22:36:55  luan]
 * 
 * Revision 1.1.2.3  1994/02/18  19:43:44  luan
 * 	CR 9777: Changed to use <dce/dceaudmsg.h> and <dce/audit.h>
 * 	instead of <dceaudmsg.h> and <audit.h>.
 * 	[1994/02/11  03:00:48  luan]
 * 
 * Revision 1.1.2.2  1994/01/25  04:59:36  luan
 * 	CR 9745 9746: File name paths consolidated.
 * 	Hard-coded file names replaced with macros
 * 	which generate names dynamically.
 * 	[1994/01/25  04:01:49  luan]
 * 
 * Revision 1.1.2.1  1993/12/22  12:38:58  zeliff
 * 	First version of code
 * 	[1993/12/21  15:01:13  zeliff]
 * 
 * $EndLog$
 */

/*
** dmn_update_inform.c
** Informs audit clients about filter updates.
** Audit clients pick up updates from filter files.
*/

#include <stdio.h>
#include <dirent.h>

#include <dce/dce.h>
#include <dce/dce_svc.h>
#include <dce/dce_msg.h>
#include <dce/exc_handling.h>
#include <dce/rpc.h>
#include <pthread.h>

#include <dce/audit.h>
#include <dce/audit_control.h>
#include <dce/dceaudmsg.h>
#include <dce/dceaudsvc.h>
#include <dce/dceaudmac.h>

#include <error_statusp.h>
#include <pathsp.h>

#include "audit_update.h"

#define RETRY_LIMIT 100


/* Internal structures */
typedef struct aud_esl_type_list_s_t {
    aud_esl_type_t                      esl_type;
    struct aud_esl_type_list_s_t        *next;
} aud_esl_type_list_t; 

/* Prototypes */
void *aud_update_client(void *);
void aud_update_announce(aud_esl_type_t);
static void aud__do_client_update(aud_esl_type_t);
static void aud__update_inform_audit_client(char *, 
                                            aud_esl_type_t, 
                                            error_status_t *); 

/* Internal globals */
pthread_mutex_t        aud_g_update_list_lock;
pthread_cond_t         aud_g_update_list_cond;
aud_esl_type_list_t   *aud_g_esl_type_list_head = NULL;
aud_esl_type_list_t   *aud_g_esl_type_list_tail = NULL;


/* 
 * Main loop for the update thread
 *
 * Wait for the control RPC's to inform us of filter changes. When this
 * occurs, attempt to contact each client and inform them of the update.
 */

void
*aud_update_client
(
    void        *arg
)
{
    aud_esl_type_t              esl_type;
    aud_esl_type_list_t         *tmp_list;
    struct timespec             now;
    struct timespec             waketime;

    /* Take the lock */
    if (pthread_mutex_lock(&aud_g_update_list_lock) != 0) {
        dce_svc_printf(AUD_S_CLIENT_UPDT_MUTEX_LOCK_FAIL_MSG);
    }
        
    for ( ; ; ) {
        /* Do we have anything to do? */
        if (aud_g_esl_type_list_head == NULL) {
            dce_gettimeofday(&now);

            /* Sleep for 10 minutes */
            waketime.tv_sec = now.tv_sec + 600;
	    waketime.tv_nsec = 0;

            /*
             * Wake up if there are changes to do or 10 mins expire,
             * whichever comes first.
             */
	    pthread_cond_timedwait(&aud_g_update_list_cond,
				   &aud_g_update_list_lock, &waketime);
        }
        else {
            esl_type = aud_g_esl_type_list_head->esl_type;

            /* 
             * If this is the last element in the queue, be careful
             * to reset the _last pointer.
             */
            if (aud_g_esl_type_list_head == aud_g_esl_type_list_tail) {
                aud_g_esl_type_list_tail = NULL;
            }
            tmp_list = aud_g_esl_type_list_head;
            aud_g_esl_type_list_head = aud_g_esl_type_list_head->next;
            free(tmp_list);

            /* Release the lock so clients can continue to update the list */
            if (pthread_mutex_unlock(&aud_g_update_list_lock) != 0) {
                dce_svc_printf(AUD_S_CLIENT_UPDT_MUTEX_UNLOCK_FAIL_MSG);
            }

            /* Do some real work */
            aud__do_client_update(esl_type);

            if (pthread_mutex_lock(&aud_g_update_list_lock) != 0) {
                dce_svc_printf(AUD_S_CLIENT_UPDT_MUTEX_LOCK_FAIL_MSG);
            }
        }
    }
}


/* 
 * This routine is called by any control RPC that updates any of the 
 * filters. 
 *
 * All we do here is tack the request to send updates for a specific
 * esl_type onto the queue for the client update thread and return.
 */

void
aud_update_announce
(
    aud_esl_type_t esl_type
)
{
    aud_esl_type_list_t         *new_list;
    aud_esl_type_list_t         *tmp_ptr;

    if (pthread_mutex_lock(&aud_g_update_list_lock) != 0) {
        dce_svc_printf(AUD_S_CLIENT_UPDT_MUTEX_LOCK_FAIL_MSG);
    }
    
    /* 
     * As an optimization, search through the list of esl_types that
     * are queued for update. If our esl_type is already in the queue,
     * we're done.
     */
    tmp_ptr = aud_g_esl_type_list_head;
    while (tmp_ptr != NULL) {
        if (tmp_ptr->esl_type == esl_type) {
            if (pthread_mutex_unlock(&aud_g_update_list_lock) != 0) {
                dce_svc_printf(AUD_S_CLIENT_UPDT_MUTEX_UNLOCK_FAIL_MSG);
            }
            return;
        }
        tmp_ptr = tmp_ptr->next;
    }

    /* Create a new list element */
    new_list = (aud_esl_type_list_t *)malloc(sizeof(aud_esl_type_list_t));
    if (new_list == NULL) {
        dce_svc_printf(AUD_S_CANNOT_ALLOCATE_MEMORY_MSG,
                       sizeof(aud_esl_type_list_t));
    }
    
    /* Fill in the new element and tack it onto the tail of the queue */
    new_list->esl_type = esl_type;
    new_list->next = NULL;

    /* If the list is empty, make the head and tail point to the right place */
    if (aud_g_esl_type_list_tail != NULL) {
        aud_g_esl_type_list_tail->next = new_list;
    }
    else {
        aud_g_esl_type_list_head = new_list;
    }
    aud_g_esl_type_list_tail = new_list;
    
    if (pthread_mutex_unlock(&aud_g_update_list_lock) != 0) {
        dce_svc_printf(AUD_S_CLIENT_UPDT_MUTEX_UNLOCK_FAIL_MSG);
    }

    pthread_cond_signal(&aud_g_update_list_cond);
    
    return;
}


static void
aud__do_client_update
(
    aud_esl_type_t esl_type
)
{
    DIR            *lvl1_dir = NULL, *lvl2_dir = NULL;
    FILE           *file;
    struct dirent  *dir_lvl1_entry;
    struct dirent  *dir_lvl2_entry;
    char            binding_dname[256];
    char            fname[256];
    error_status_t  st;
    error_status_t *stp = &st;

    lvl1_dir = opendir(CLIENT_BINDING_DIR);
    if (lvl1_dir == NULL) {
        /* Abort and try again later */
        dce_svc_printf(AUD_S_UPDT_OPENDIR_FAIL_MSG, CLIENT_BINDING_DIR, errno);
        return;
    }

    while ((dir_lvl1_entry = readdir(lvl1_dir)) != NULL) {
	if (strcmp(".", dir_lvl1_entry->d_name)
	    &&
	    strcmp("..", dir_lvl1_entry->d_name)) {
	    strcpy(binding_dname, CLIENT_BINDING_DIR);
	    strcat(binding_dname, dir_lvl1_entry->d_name);
	    strcat(binding_dname, "/");
	    lvl2_dir = opendir(binding_dname);
            if (lvl2_dir == NULL) {
                /* Abort and try again later */
                dce_svc_printf(AUD_S_UPDT_OPENDIR_FAIL_MSG, binding_dname, errno);
                closedir(lvl1_dir);
                return;
            }

	    while ((dir_lvl2_entry = readdir(lvl2_dir)) != NULL) {
		if (strcmp(".", dir_lvl2_entry->d_name)
		    &&
		    strcmp("..", dir_lvl2_entry->d_name)) {
		    strcpy(fname, binding_dname);
		    strcat(fname, dir_lvl2_entry->d_name);
		    aud__update_inform_audit_client(fname, esl_type, stp);
		    if (BAD_STATUS(stp)) {
			dce_svc_printf(AUD_S_UNABLE_TO_SYNC_CLIENT_MSG, fname);
		    }
		}
	    }
	    closedir(lvl2_dir);
	}
    }
    closedir(lvl1_dir);
    return;
}

static void
aud__update_inform_audit_client
(
    char           *fname,
    aud_esl_type_t esl_type,
    error_status_t * stp
) 
{
    FILE                *fptr;
    char                 buff[1024];

    unsigned_char_p_t    obj = NULL;
    unsigned_char_p_t    protseq = NULL;
    unsigned_char_p_t    netaddr = NULL;
    unsigned_char_p_t    endpoint = NULL;
    unsigned_char_p_t    options = NULL;
    unsigned_char_p_t    binding = NULL;
    unsigned_char_p_t    protseq_real, endpoint_real, binding_real, hostname;
    rpc_binding_handle_t audit_client_handle;
    int                  counter = 0;

    CLEAR_STATUS(stp);

    if ((fptr = fopen(fname, "r")) != NULL) {
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
		if (GOOD_STATUS(stp)) {
		    binding_real = binding;
		    rpc_binding_from_string_binding(binding_real,
						 &audit_client_handle, stp);
		}
	    }			/* end of if rpc_string_binding_parse */
	}			/* end of if fgets */

        fclose(fptr);
    }
    /* end of if fopen */
    else {			/* fopen() returns NULL */
	dce_svc_printf(AUD_S_CANT_OPEN_CLIENT_BINDING_FILE_MSG, fname);
	*stp = aud_s_cant_open_client_binding_file_rc;
	return;
    }

    if (rpc_mgmt_is_server_listening(audit_client_handle, stp)) {
#ifdef DCE_DEBUG
        DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug9,
	    "Calling update_esl_check no. %d\n", counter));
#endif
        for ( ; ; ) {
            update_v1_0_c_epv.update_esl_check(audit_client_handle, esl_type, stp);
            if (GOOD_STATUS(stp)) {
#ifdef DCE_DEBUG
        	DCE_SVC_DEBUG((aud_svc_handle, aud_s_msgs, svc_c_debug9,
	    	    "update_esl_check no. %d succeeded\n", counter));
#endif
		break;
	    }
	    if (++counter > RETRY_LIMIT) {
		dce_svc_printf(AUD_S_UNABLE_TO_SYNC_CLIENT_MSG, fname);
		break;
	    } 
	}
    } else {
	dce_svc_printf(AUD_S_UNABLE_TO_SYNC_CLIENT_MSG, fname);
    }

    /*
     * Free the strings that may have been allocated by the RPC runtime 
     */

    if (obj != NULL) {
	rpc_string_free(&obj, stp);
    }
    if (protseq != NULL) {
	rpc_string_free(&protseq, stp);
    }
    if (netaddr != NULL) {
	rpc_string_free(&netaddr, stp);
    }
    if (endpoint != NULL) {
	rpc_string_free(&endpoint, stp);
    }
    if (options != NULL) {
	rpc_string_free(&options, stp);
    }
    if (binding != NULL) {
	rpc_string_free(&binding, stp);
    }
    return;
}
