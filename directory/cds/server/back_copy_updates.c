/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: back_copy_updates.c,v $
 * Revision 1.1.6.2  1996/02/18  19:35:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:32  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:28:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:10  root]
 * 
 * Revision 1.1.4.4  1994/08/03  19:03:25  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:18:08  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:43  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:58  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/14  14:51:02  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:18:20  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:09:39  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:37:16  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:18:36  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:18:42  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:24:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module back_copy_updates.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 */
#include <server.h> 
#include <back.h>
#include <dbdef.h>
#include <clerk.h>
#include <replica.h>
#include <uid.h>

/*
 * external structures
 */                   
extern struct time_quad dns_bkgd_wait_clerk;

/*
 * Local prototypes
 */
static dthread_address_t
send_async_task (
    spread_element_t	*);

static dns_status_t 
send_async (
    update_buffer_t	*,
    copy_context_t	*);


/*
 * send_async_task
 *
 * This routine makes a do_updates clerk call as part of a work crew
 * managed by send_async.
 *
 * Arguments:
 *     element_p (in) - Pointer to spread context.
 */
static dthread_address_t 
send_async_task (spread_element_t *element_p)
{
    DEB_ASCII_BUF_spread_element(elementBuf)
    byte_t		*current_size_p;  /* Pointer to our update packet */
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >send_async_task(%s)",
        deb_ascii_spread_element(elementBuf,element_p)));

    /* Skip over the update buffer prologue */
    current_size_p = SKIP_bytes(element_p->update_buf_p, sizeof(update_buffer_t));

   /*
    * Process each update packet in the update buffer
    */

    while (0 < element_p->spread_update_count)
        {

        element_p->spread_status =
            clerk_do_updates(element_p->context_p->ct_epoch_p, 
			     element_p->spread_hint_p,
                             element_p->spread_hint_len,
			     EXT32(current_size_p),
			     SKIP_longword(current_size_p));

        if (element_p->spread_status == DNS_SUCCESS)
            {
	    --element_p->spread_update_count;
  	    current_size_p = SKIP_bytes(current_size_p,
				sizeof(longword_u) + EXT32(current_size_p));
            }
        else
	    { /* Convert errors */
            if (element_p->spread_status == DNS_RESOURCEERROR) 
    	        element_p->spread_status = DNS_NONSRESOURCES;
            else if (element_p->spread_status <= DNS_NAMESERVERERRORS) 
	        element_p->spread_status = DNS_NAMESERVERBUG;
	    break;  /* If we get an error, don't bother with other packets */
	    }

        }

    if ((thstatus = dthread_lock(&(element_p->crew_context_p->crew_lock))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{
	    --element_p->crew_context_p->tasks_running;

	    if ((thstatus = 
		 dthread_signal(&(element_p->crew_context_p->done))) < 0)
	    {
		dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)thstatus);
	    }

	}
        /* End of the 'protected code' */

    if ((thstatus = 
	 dthread_unlock(&(element_p->crew_context_p->crew_lock))) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <send_async_task(%s) return(%p)",
        deb_ascii_spread_element(elementBuf,element_p),
        (dthread_address_t)0));
    return((dthread_address_t)0);
}

                        
/*                                                                       
 * send_async
 *
 * 	This function sends the update packets necessary for a spread 
 * 	for the directory and all its children to a set of replicas
 *
 * Inputs:         
 *      update_buf_p		  pointer to the buffer of update packets
 *	context_p		  pointer to context buffr
 *
 */                          
static dns_status_t 
send_async (update_buffer_t  *update_buf_p,
            copy_context_t   *context_p)
{
    DEB_ASCII_BUF_update_buffer(update_bufBuf)
    DEB_ASCII_BUF_copy_context(contextBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dthread_t		work_thread_id;
    crew_context	*crew_context_p;
    int			i;
    dns_status_t	status = DNS_SUCCESS;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >send_async(%s,%s)",
        deb_ascii_update_buffer(update_bufBuf,update_buf_p),
        deb_ascii_copy_context(contextBuf,context_p)));

    ++context_p->ct_buffers_sent;	/* Flag that we have been here */


    /*
     * Build the work crew context management block
     */

    crew_context_p = (crew_context *)dns_malloc(sizeof(crew_context));
    if (!crew_context_p) {
        status = DNS_NONSRESOURCES;
	goto leave_send_async;
    }

    crew_context_p->tasks_left = context_p->ct_spread_count;
    crew_context_p->concurrent_limit = MAX_SPREADS;
    {
        if ((thstatus = dthread_create_mutex(&crew_context_p->crew_lock)) < 0)
        {
            dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
        }
    }

    crew_context_p->tasks_running = 0;

    {
        if ((thstatus = dthread_create_condition(&crew_context_p->done)) < 0)
        {
            dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
        }
    }

    /*
     * Initialize static information and start first set of do_updates 
     */

    i = -1;  /* Set iteration variable so it comes up 0 on first loop */

    while (0 < crew_context_p->tasks_left)
    { /* while start */
	spread_element_t *element_p = context_p->ct_spread_list[++i];

	/* If errors already talking to this replica, then skip it */
	if (element_p->spread_status != DNS_SUCCESS)
            {
            crew_context_p->tasks_left--;
            continue;
            }

        element_p->update_buf_p = update_buf_p;
	element_p->spread_update_count = update_buf_p->ub_count;
        element_p->context_p = context_p;
        element_p->crew_context_p = crew_context_p;

        /*
         * Begin work crew management - don't let too many threads start.
         *
         * Examine the work crew context to see how many threads are running
         * with respect to the concurrent limit.  If we are below, start
         * up a thread.  If we are above (or at) the limit, then wait until
         * a crew task is done (they will signal us).
         */

        if ((thstatus = dthread_lock(&(crew_context_p->crew_lock))) < 0)
        {
            dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
        }

            /* Beginning of the 'protected code' */
	    {
	    extern dthread_attr_t send_async_task_attr;

	    if (crew_context_p->concurrent_limit <= crew_context_p->tasks_running)
	    {
                if ((thstatus = dthread_wait(&(crew_context_p->done),
                                             &(crew_context_p->crew_lock))) < 0)
                {
		  dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
                }
	    }

	    /* Create work crew task */
	    if ((thstatus = dthread_create(&work_thread_id,
				       &send_async_task_attr,
				        (dthread_start_routine)send_async_task,
					(dthread_address_t)element_p))  < 0)
	    {
		dce_svc_printf(CDS_S_PTHREADCREATE_A_MSG, (long)thstatus);
	    }
	    (void)dthread_detach(&work_thread_id);
	    --crew_context_p->tasks_left;
	    crew_context_p->tasks_running++;

        }
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(crew_context_p->crew_lock))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

        /* Work crew task has been created */

    }/* while end */

    /*
     *  Wait for the work crew to complete its work
     */

    if ((thstatus = dthread_lock(&(crew_context_p->crew_lock))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{
	    while (0 < crew_context_p->tasks_running)
	    {
		if ((thstatus = dthread_wait(&(crew_context_p->done),
					    &(crew_context_p->crew_lock))) < 0)
		{
		  dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
		}
	    }
	}
       /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(crew_context_p->crew_lock))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    /*
     * Clean up the crew context -- we're all done with it now 
     */
    if ((thstatus = dthread_delete_condition(&crew_context_p->done)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
    }
    
    if ((thstatus = dthread_delete_mutex(&crew_context_p->crew_lock)) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)thstatus);
    }
    dns_free((char *)crew_context_p);

    /* If couldn't talk to anyone, return error */
    for (i = 0; i < context_p->ct_spread_count; ++i)
	{
	spread_element_t *const element_p = context_p->ct_spread_list[i];

	if ((status = element_p->spread_status) == DNS_SUCCESS) 
            break;
	}

    /* Return whatever you got */
leave_send_async:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <send_async(%s,%s) return(%s)",
        deb_ascii_update_buffer(update_bufBuf,update_buf_p),
        deb_ascii_copy_context(contextBuf,context_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

                        
/*                                                                       
 * send_dir 
 *	This function sends the update packets necessary for a spread for the
 * 	directory and all its children to the replica also used by combine
 * 	to send info back to the skulker  
 *
 * Inputs:         
 *      update_buf_p	  pointer to the buffer of update packets
 *	context_p	  pointer to context buffr
 *
 *     
 */                          
dns_status_t 
send_dir (update_buffer_t  *update_buf_p,
          copy_context_t   *context_p)
{
    DEB_ASCII_BUF_update_buffer(update_bufBuf)
    DEB_ASCII_BUF_copy_context(contextBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    byte_t		*current_size_p;
    dns_status_t	status = DNS_SUCCESS;
    int			i, rp_len;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >send_dir(%s,%s)",
        deb_ascii_update_buffer(update_bufBuf,update_buf_p),
        deb_ascii_copy_context(contextBuf,context_p)));

    if (!context_p->ct_replica_p) {
	status = send_async(update_buf_p, context_p);
	goto leave_send_dir;
    }

    replica_parse(context_p->ct_replica_p, &rp_len,
	NULL, NULL, NULL, NULL);
    
    ++context_p->ct_buffers_sent;	/* Flag that we have been here */
    current_size_p = SKIP_bytes(update_buf_p, sizeof(update_buffer_t));

    for (i = 0; i < update_buf_p->ub_count; ++i)
	{
	/* Fill in variable item fields */
	status = clerk_do_updates(context_p->ct_epoch_p, 
		context_p->ct_replica_p, rp_len, EXT32(current_size_p),
		SKIP_longword(current_size_p));

	current_size_p = SKIP_bytes(current_size_p,
		sizeof(longword_u) + EXT32(current_size_p));
            
        if (status != DNS_SUCCESS) break;
                                                      
        } /* End for loop */

leave_send_dir:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <send_dir(%s,%s) return(%s)",
        deb_ascii_update_buffer(update_bufBuf,update_buf_p),
        deb_ascii_copy_context(contextBuf,context_p),
        deb_ascii_dns_status(statusBuf,status)));
    return (status);
}

           
/*                                  
 * copy_updates
 *	This function copies updates to the secondaries and read-only
 * 	replicas.  
 *
 * Inputs:                                                         
 *	stream_p 	pointer to stream descriptor
 *      begin_uid_p     pointer to timestamp begininging the range
 *      skulk_ts_p	pointer to timestamp ending the range
 *      update_buffer_p	pointer to buffer of packets
 *                      replica address in send_dir
 *      context_p       pointer to context datastructure
 *
 * Outputs:
 *                                                                
 */                                                       
dns_status_t 
copy_updates (struct db_stream  *stream_p,
              Timestamp_t       *begin_uid_p,
              Timestamp_t       *skulk_ts_p,
              update_buffer_t   *update_buf_p,
              copy_context_t    *context_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_Timestamp(begin_uidBuf)
    DEB_ASCII_BUF_Timestamp(skulk_tsBuf)
    DEB_ASCII_BUF_update_buffer(update_bufBuf)
    DEB_ASCII_BUF_copy_context(contextBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    byte_t		*current_upd_p;
    struct node_header	*bkt_p;
    byte_t		*current_size_p;
    int			count;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >copy_updates(%s,%s,%s,%s,%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_Timestamp(begin_uidBuf,begin_uid_p),
        deb_ascii_Timestamp(skulk_tsBuf,skulk_ts_p),
        deb_ascii_update_buffer(update_bufBuf,update_buf_p),
        deb_ascii_copy_context(contextBuf,context_p)));

    /*
     *  see if updates collected last time are still useful
     */                                                    
    if ((context_p->ct_buffers_sent == 1) &&
        EQ_Timestamp(begin_uid_p, context_p->ct_begin_uid))
        {                                         
        status = send_dir(update_buf_p, context_p);
	context_p->ct_buffers_sent = 1;
	goto leave_copy_updates;
        }
        

    /* Initialize the context */
    COPY_Timestamp(begin_uid_p, context_p->ct_begin_uid);

    context_p->ct_buffers_sent = 0;
    update_buf_p->ub_count = 0;
    update_buf_p->ub_remaining = update_buf_p->ub_total;

    current_size_p = SKIP_bytes(update_buf_p, sizeof(update_buffer_t));
    current_upd_p = SKIP_longword(current_size_p);
                                       
    /* Get changes to directory itself */
    status = gather_updates(stream_p, &stream_p->db_dir,
		begin_uid_p, skulk_ts_p,  &current_upd_p,
		&current_size_p, update_buf_p, context_p);

    if ((status != DNS_SUCCESS) && (status != DNS_NOUPDATES))
	goto leave_copy_updates;
    
    bkt_p = NULL;
    stream_p->db_entry.data_p = NULL;
    
    /* 
     * Read the entry key data level and collect all of our updates 
     * key in stream must equal directory uid 
     */
    for (;;)
	{
	/* 
	 * We are returned our own copy of the bucket and will 
	 * change each record in the bucket to look like 
	 * a data_record.  This is being done so we don't have
	 * to copy the data each time.
	 */
	status = db_entry_read_next_bucket(stream_p, &bkt_p);
	if (status != DNS_SUCCESS) 
	    {
	    if (status == DNS_UNKNOWNENTRY) break;
	    if (bkt_p) dns_free((char *)bkt_p);		
	    db_entry_release(stream_p);
	    goto leave_copy_updates;
	    }

	for (count = 0; count < bkt_p->node_count; ++count)
	    {
	    status = db_entry_build_record(stream_p, count, bkt_p);
	    if (status != DNS_SUCCESS)
		{
		if (status == DNS_UNKNOWNENTRY) break;
		if (bkt_p) dns_free((char *)bkt_p);		
		db_entry_release(stream_p);
		goto leave_copy_updates;
		}
	    status = gather_updates(stream_p, &stream_p->db_entry,
		begin_uid_p, skulk_ts_p,  &current_upd_p,
		&current_size_p, update_buf_p, context_p);

	    if ((status != DNS_SUCCESS) && (status != DNS_NOUPDATES))
		{
		if (bkt_p) dns_free((char *)bkt_p);		
		db_entry_release(stream_p);
		goto leave_copy_updates;
		}
	    }
	/* 
	 * Fork so another thread can run. Move us to the back of the
	 * queue
 	 */
	if (status == DNS_UNKNOWNENTRY) break;
        (void)dthread_yield();

	}
    if (bkt_p) dns_free((char *)bkt_p);		
    db_entry_release(stream_p);

    status = send_dir(update_buf_p, context_p);

leave_copy_updates:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <copy_updates(%s,%s,%s,%s,%s) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_Timestamp(begin_uidBuf,begin_uid_p),
        deb_ascii_Timestamp(skulk_tsBuf,skulk_ts_p),
        deb_ascii_update_buffer(update_bufBuf,update_buf_p),
        deb_ascii_copy_context(contextBuf,context_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}
