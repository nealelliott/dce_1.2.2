/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: back_collect.c,v $
 * Revision 1.1.8.2  1996/02/18  19:35:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:29  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:28:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:04  root]
 * 
 * Revision 1.1.6.4  1994/08/03  19:03:23  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:17:58  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:42:41  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:55  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/14  14:51:00  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:18:06  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:09:32  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:37:00  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:18:21  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:18:31  zeliff]
 * 
 * Revision 1.1.2.3  1992/06/30  19:32:53  peckham
 * 	OT#2114: uninitialized variables.
 * 	[1992/06/30  19:32:09  peckham]
 * 
 * Revision 1.1.2.2  1992/03/22  22:25:47  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:33:40  weisman]
 * 
 * Revision 1.1  1992/01/19  15:24:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module - back_collect.c
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
 *
 *
 * Abstract:
 *   All functions associated with the COLLECT phase of the skulk.
 *
 *
 */

/*
 *  Include files
 */
#include <server.h>
#include <back.h>
#include <clerk.h>
#include <dbdef.h>
#include <find.h>
#include <replica.h>
#include <uid.h>

/*
 * local prototypes
 */
static void
check_replicas (
    DBSet_t		*,
    ring_list_t		*);

static dthread_address_t
collect_data_task (
    ring_element_t	*);

static dns_status_t
collect_data (
    struct db_stream	*,
    int			,
    DBSet_t		*,
    ring_list_t		*,
    ring_element_t	*,
    Timestamp_t		*,
    ObjUID_t		*,
    Timestamp_t		*);

/*
 * check_replicas
 *	This routine matches replica clearinghouse uids with elements
 * in the ring list after a combine.  If a replica is not found in
 * the ring list then the set has changed as the result of the combine
 * and more data is out there waiting to be collected.
 *
 * Input
 *	replica_set_p	pointer to current replica set
 *	ring_list_p 	pointer to ring descriptor
 */
static void 
check_replicas (DBSet_t      *replica_set_p,
                ring_list_t  *ring_list_p)
{
    int pos, cnt;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >check_replicas(%p,%p)",
        replica_set_p,
        ring_list_p));

   /* Loop through each replica set member */
    for (pos = 0; pos < NUM_DBSet(replica_set_p); pos++)
	{
	/* Get pointer to replica */
	DBSetMember_t *const value_p =
		(DBSetMember_t *)INDEX_DBSet(replica_set_p, pos);
	ReplicaPointer_t *workingon_p;
	ring_element_t	*ring_element_p;

	if (!EXT8(value_p->dm_value_present) || !EXT8(value_p->dm_valid))
	    continue;

	workingon_p = (ReplicaPointer_t *)value_p->dm_data;

	/* See if we already talked to this replica */
	ring_element_p = ring_list_p->ring_first;

	for (cnt = 0; cnt < ring_list_p->ring_count; ring_element_p++, cnt++)
	    if (EQ_ObjUID(workingon_p->rp_CHID,
			ring_element_p->ring_from_uid))
		{
		/* Flag this replica as done */
		INS8(value_p->dm_valid, BL_false);
		break;
		}
	} /* End looping thru each replica set member  */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <check_replicas(%p,%p)",
        replica_set_p,
        ring_list_p));
}

/*
 * collect_data_task
 *
 * This routine runs as a work crew thread controlled by the collect_data
 * routine.  Its purpose in life is to issue a clerk combine call and
 * wait for its completion, and then update the ring with the status
 * of the combine call.
 *
 * Input
 *	ring_element_p  pointer to current element in ring descriptor
 *
 * Returns:
 *	0
 */
static dthread_address_t 
collect_data_task (ring_element_t *ring_element_p)
{
    crew_context *const crew_context_p = ring_element_p->crew_context_p;
    int thstatus;
    ObjUID_t return_uuid;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >collect_data_task(%p)",
        ring_element_p));

    /* Call the clerk with the parameters needed for a combine */

    ring_element_p->ring_status = clerk_combine(
		(ObjUID_t *)ring_element_p->stream_p->db_dir.data_uid,
                ring_element_p->all_p,
                ring_element_p->epoch_p,
                ring_element_p->sk_p,
		&ring_element_p->stream_p->db_file_p->file_ch_p->ch_name,
		ring_element_p->ring_hint_p,
		ring_element_p->ring_hint_len,
                &return_uuid,  /* no longer used */
		ring_element_p->ring_actual_p);

    /* Check combine status - set ring_status appropriately */

    if (ring_element_p->ring_status != DNS_SUCCESS)
	{
        if (ring_element_p->ring_status == DNS_RESOURCEERROR)
    	    ring_element_p->ring_status = DNS_NONSRESOURCES;
        else if (ring_element_p->ring_status <= DNS_NAMESERVERERRORS)
	    ring_element_p->ring_status = DNS_NAMESERVERBUG;
	}

    /* (This is where we removed the fix_replica_ptr call) */

    /* Signal the work crew manager that we're done */

    if (crew_context_p)
    {
	if ((thstatus = dthread_lock(&(crew_context_p->crew_lock))) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
	}

	/* Beginning of the 'protected code' */
        {
  	    --crew_context_p->tasks_running;

	    if ((thstatus = dthread_signal(&(crew_context_p->done))) < 0)
	    {
                dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)thstatus);
	    }

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug6,
                "collect_data_task(%p) - Signalling done, tasks_running=%d\n",
                ring_element_p, 
		crew_context_p->tasks_running));
       }

      /* End of the 'protected code' */
      if ((thstatus = dthread_unlock(&(crew_context_p->crew_lock))) < 0)
      {
	  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
      }
  }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <collect_data_task(%p) return(0)",
        ring_element_p));

    return((dthread_address_t)0);
}

/*
 * collect_data
 *	This routine reads through the replica set collecting changes
 * from the directory copies.  There are a maximum of 5 combines
 * outstanding at any time.
 *
 * Input
 *	stream_p	pointer to stream descriptor
 *	max_replicas	count of replicas
 *	replica_set_p	pointer to current replica set
 *	ring_list_p 	pointer to ring descriptor
 *	ring_element_p  pointer to current element in ring descriptor
 *	all_p		pointer to allupto value
 *	epoch_p 	pointer to epoch
 *	sk_p 		pointer to skulktime
 * 
 * Returns:
 *	DNS_SUCCESS
 * 	DNS_NONSRESOURCES
 */
static dns_status_t 
collect_data (struct db_stream  *stream_p,
              int               max_replicas,
              DBSet_t           *replica_set_p,
              ring_list_t       *ring_list_p,
              ring_element_t    *ring_element_p,
              Timestamp_t       *all_p,
              ObjUID_t          *epoch_p,
              Timestamp_t       *sk_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dthread_t work_thread_id;			 /* thread in use  */
    crew_context *crew_context_p;		 /* Used by thread  */
    ReplicaPointer_t *replica_p;		 /* Replica attribute set ptr*/
    int cnt;					 /* counter  */
    dns_status_t status = DNS_SUCCESS;		 /* stat to be returned  */
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >collect_data(%s,%d,%p,%p,%p)",
        deb_ascii_db_stream(streamBuf,stream_p),
        max_replicas,
        replica_set_p,
        ring_list_p,
        ring_element_p));

        

    /*
     * Build the work crew context management block
     */

    crew_context_p = (crew_context *)dns_malloc(sizeof(crew_context));
    if (!crew_context_p) {
        status = DNS_NONSRESOURCES;
	goto leave_collect_data;
    }

    crew_context_p->tasks_left = max_replicas;
    crew_context_p->concurrent_limit = COLLECT_CONCURRENT_LIMIT;
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

    replica_p = NULL;

    /* Loop through the replica set */
    for (cnt = 0; (cnt < NUM_DBSet(replica_set_p)) &&
			(0 < crew_context_p->tasks_left); cnt++)
	{
	DBSetMember_t *value_p = (DBSetMember_t *)
		    INDEX_DBSet(replica_set_p, cnt);
	ReplicaType_u replicaType;

        if (!EXT8(value_p->dm_value_present) || !EXT8(value_p->dm_valid))
            {
            crew_context_p->tasks_left--;
            continue;
            }
	ring_element_p->ring_hint_p = (ReplicaPointer_t *)value_p->dm_data;

	/* Skip self */
        if (EQ_ObjUID(ring_element_p->ring_hint_p->rp_CHID,
		         &stream_p->db_file_p->file_ch_p->ch_uid))
            {
            crew_context_p->tasks_left--;
            continue;
            }

	/* Get replica pieces */
        replica_parse(ring_element_p->ring_hint_p,
  	    &ring_element_p->ring_hint_len, NULL, NULL,
	    replicaType, NULL);


        /* Allocate buffer for clerk output - must not be on stack */
        if (!replica_p)
	    {
            replica_p = (ReplicaPointer_t *)
		    dns_malloc(sizeof(ReplicaPointer_u));
            if (!replica_p)
		{
		status = DNS_NONSRESOURCES;
		break;
		}
	    }

        /*
         * Finish loading the ring_element with needed data, collect has
         * loaded the other ring_element component variables.
         * Note that we're passing a stream pointer to multiple threads.
         * This is legal here because we're only using data within the
         * stream block itself, and not performing database operations.
         * This is kinda dangerous, and it would be much better if the
         * required fields were extracted here and placed in the ring_element
         * structure -- removing the temptation to use the stream for database
         * operations in the worker threads.   **!!** Fix this someday!!
         */

        ring_element_p->crew_context_p = crew_context_p;
        ring_element_p->stream_p = stream_p;
        ring_element_p->all_p = (Timestamp_t *)all_p;
        ring_element_p->epoch_p = (ObjUID_t *)epoch_p;
        ring_element_p->sk_p = (Timestamp_t *)sk_p;
        ring_element_p->ring_actual_p = replica_p;


        /*
         * Begin work crew management - don't let too many threads start.
         *
         * Examine the work crew context to see how many threads are running
         * with respect to the concurrent limit.  If we are below, start
         * up a thread.  If we are above ( or at) the limit, then wait until
         * a crew task is done (they will signal us).
         */
        if ((thstatus = dthread_lock(&(crew_context_p->crew_lock))) < 0)
        {
            dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
        }

	/* Beginning of the 'protected code' */
        {
	    extern dthread_attr_t collect_data_task_attr;

	    if (crew_context_p->concurrent_limit <= 
		crew_context_p->tasks_running)
	    {
		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_server,
                    svc_c_debug5,
		    "collect_data(%s,...) - throttling work crew: run=%d, limit=%d\n",
		    deb_ascii_db_stream(streamBuf,stream_p),
		    crew_context_p->tasks_running,
		    crew_context_p->concurrent_limit));

                if ((thstatus = dthread_wait(&(crew_context_p->done),
                                             &(crew_context_p->crew_lock))) < 0)
                {
		  dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
                }

	    }

	    /*
	     * Create work crew task (clerk combine call) 
	     */
	    if ((thstatus = dthread_create(&work_thread_id, 
				     &collect_data_task_attr,
				      (dthread_start_routine)collect_data_task,
				      (dthread_address_t)ring_element_p)) <0 )
	    {
		dce_svc_printf(CDS_S_PTHREADCREATE_A_MSG, (long)thstatus);
	    }
	    
	    (void)dthread_detach(&work_thread_id);
	    --crew_context_p->tasks_left;
	    crew_context_p->tasks_running++;

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug5,
		"collect_data(%s,...) -  tasks_left=%d, tasks_running=%d\n",
		deb_ascii_db_stream(streamBuf,stream_p),
		crew_context_p->tasks_left,
		crew_context_p->tasks_running));
	}
	/* End of the 'protected code' */
	if ((thstatus = dthread_unlock(&(crew_context_p->crew_lock))) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	}

        /*
	 * Work crew task has been created 
	 */

	/*	
	 * Save uid for later recreation of ring 
	 */
	COPY_ObjUID(ring_element_p->ring_hint_p->rp_CHID,
		  ring_element_p->ring_from_uid);

	/* Move on to next element without waiting */
	++ring_list_p->ring_count;
	ring_element_p++;
	replica_p = NULL;
	} 				/* End loop through the replica set */

    if (replica_p) dns_free((char *)replica_p);

    /*
     * Wait till all outstanding combines complete 
     */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        "I collect_data(%s,...) - waiting for combines\n",
	deb_ascii_db_stream(streamBuf,stream_p)));

    if ((thstatus = dthread_lock(&(crew_context_p->crew_lock))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }
        /* Beginning of the 'protected code' */
	{
	    while (crew_context_p->tasks_running > 0)
	    {
	        DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_server,
                    svc_c_debug5, 
                    "I collect_data(%s,...) - %d combines still running\n",
                    deb_ascii_db_stream(streamBuf,stream_p),
                    crew_context_p->tasks_running));

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

    /* Clean up the work crew context */
    if ((thstatus = dthread_delete_condition(&crew_context_p->done)) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
    }

    if ((thstatus = dthread_delete_mutex(&crew_context_p->crew_lock)) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)thstatus);
    }
    
    dns_free((char *)crew_context_p);

leave_collect_data:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <collect_data(%s,...) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dns_status(statusBuf,status)));
    return (status);
}


/*
 * cleanup_ring_list
 * This function loops thru the ring list and clears out any replica
 * pointers stored in
 *
 * Inputs:
 *	ring_list_p pointer to ring of replicas
 */
void 
cleanup_ring_list (ring_list_t *ring_list_p)
{
    ring_element_t      *ring_element_p = ring_list_p->ring_first;
    int                  i;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >cleanup_ring_list(%p)",
        ring_list_p));

    if (ring_list_p) {
	for (i=0; i<ring_list_p->ring_count; ring_element_p++, i++)
	    if (ring_element_p->ring_actual_p != 0)
		dns_free((char *)(ring_element_p->ring_actual_p));


	dns_free((char *)ring_list_p);
      }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <cleanup_ring_list(%p)",
        ring_list_p));
    return;
}


/*
 * collect
 * CDS does not support writable copies (secondaries) so only need to collect
 * if this skulk is the result of a newepoch (which may have changed the
 * master).  If so this function contacts all replicas of the directory and
 * has them send all the updates they have for the directory.
 * It does not alter the database.
 *
 * Inputs:
 *      stream_p	pointer to stream descriptor
 *      skulktime_p     pointer to the proposed timestamp of the skulk
 * 
 * Returns:
 *	DNS_SUCCESS
 *	DNS_OLDSKULK      if another skulk in progress/completed
 *	DNS_NAMESERVERBUG on software error and/or missing required attribute
 *	other DECdns failure code returned from db_get_stream, db_dir_read, etc.
 */
dns_status_t 
collect (struct db_stream  *stream_p,
         Timestamp_t       *skulktime_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_Timestamp(skulktimeBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    LOG_ASCII_BUF_FullName( namebuf)
    dir_hash_entry_t	*const hash_p 		 /* Dir hash entry pointer  */
			    = stream_p->db_dir_hash_p;
    nsgbl_ch_t 		*const ch_p  		 /* Clearinghouse info ptr */
			    = stream_p->db_file_p->file_ch_p;
    ring_list_t		*ring_list_p = NULL;	 /* Ptr to ring of replicas  */
    ring_element_t	*ring_element_p;
    DBSet_t		*replica_set_p;		 /* Replica attribute ptr  */
    DBSetMember_t	*value_p;
    DBSet_t		*set_p;
    int			cnt, max_replicas, idx;
    ObjUID_t		*epoch_uid_p;		 /* Ts of this epoch  */
    Timestamp_t         *allupto_uid_p;		 /* Ts of allupto  */
    db_stream_t		*newstream_p;		 /* Stream information ptr  */
    int			found;			 /* Checks ring validity  */
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >collect(%s,%s,)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_Timestamp(skulktimeBuf,skulktime_p)));

    /* Get allupto */
    status = find_attribute(stream_p->db_dir.data_p,
	dns_AttributeName(DNS_ALLUPTO), &set_p, &idx);

    if (status != DNS_SUCCESS)
        {
	evl_AttributeMissing(ch_p, (FullName_t *)hash_p->dir_name,
	    dns_AttributeName(DNS_ALLUPTO));
        status = DNS_NAMESERVERBUG;
	goto leave_collect;
        }

    value_p = (DBSetMember_t *)DATA_DBSet(set_p);
    allupto_uid_p = (Timestamp_t *)value_p->dm_data;

   /*
    * Only combine from read_onlys when following a newepoch
    * Replica type may have changed
    */
    if (NE_Timestamp(allupto_uid_p, NullTimestamp))
        {
        status = DNS_SUCCESS;
	goto leave_collect;
        }

    /* Get the current epoch value */
    status = find_attribute(stream_p->db_dir.data_p,
        dns_AttributeName(DNS_EPOCH), &set_p, &idx);

    if (status != DNS_SUCCESS)
        {
	evl_AttributeMissing(ch_p, (FullName_t *)hash_p->dir_name,
	    dns_AttributeName(DNS_EPOCH));
        status = DNS_NAMESERVERBUG;
	goto leave_collect;
        }

    value_p = (DBSetMember_t *)DATA_DBSet(set_p);
    epoch_uid_p = (ObjUID_t *)value_p->dm_data;

    /* Get the current replica set, may change as result of combine */
    status = find_attribute(stream_p->db_dir.data_p,
        dns_AttributeName(DNS_REPLICAS), &replica_set_p, &idx);

    if (status != DNS_SUCCESS)
        {
	evl_AttributeMissing(ch_p, (FullName_t *)hash_p->dir_name,
	    dns_AttributeName(DNS_REPLICAS));
        status = DNS_NAMESERVERBUG;
	goto leave_collect;
        }

    max_replicas = NUM_DBSet(replica_set_p);

    /*
     * Shortcut if there is only one replica of this directory.
     * There's no point in doing the collect_data phase for directories
     * with single replicas, so we'll just say everything worked dandy
     * and leave right away.
     */

    if (max_replicas == 1)
        {
        status = DNS_SUCCESS;
	goto leave_collect;
        }

   /*
    * More than one replica exists and this skulk is after a newepoch 
    * so do collect phase.
    */

    /*
     * Allocate a structure to maintain state across mulitple asynchronous
     * combine operations
     */
    dce_assert(cds__svc_handle,
	       (0 < max_replicas));
    ring_list_p = (ring_list_t *)dns_malloc(sizeof(ring_list_t)
	+ ((max_replicas - 1)*sizeof(ring_element_t)));
    if (!ring_list_p) {
        status = DNS_NONSRESOURCES;
	skulk_log(hash_p, cds_s_server_collectphase, 0, NULL, status);
	goto leave_collect;
    }

    /*
     * clear out the ring element list, so that later we can
     * tell if a replica pointer is left in the structure to be freed
     */
    ZERO_bytes(ring_list_p, sizeof(ring_list_t)
	+ ((max_replicas-1)*sizeof(ring_element_t)));

    /* First one is for this clearinghouse */
    ring_list_p->ring_count = 1;
    ring_element_p = ring_list_p->ring_first;

    ring_element_p->ring_status = DNS_SUCCESS;
    COPY_ObjUID(&ch_p->ch_uid, ring_element_p->ring_from_uid);

    status = collect_data(stream_p, max_replicas, replica_set_p, ring_list_p,
		&ring_element_p[1],  /* Skip over the local CH copy */
		allupto_uid_p, epoch_uid_p, skulktime_p);
    if (status != DNS_SUCCESS)
	{
	skulk_log(hash_p, cds_s_server_collectphase, cds_s_server_failcollect,
		  NULL, status);
	goto leave_collect;
	}

    /*
     * The previous collect_data may have changed the replica
     * set on disk.  Re_read the directory and find out.  If
     * the replica set doesn't match the combined list return OldSkulk
     * and let the other skulk finish.  (Adding or removing a replica
     * both cause a skulk to occur from the master.)
     */
    status = db_get_stream(stream_p->db_file_p, &newstream_p, OP_ReadAttribute);
    if (status != DNS_SUCCESS)
	{
	/* get rid of any return replica pointers stored in structure
	 */
	goto leave_collect;
	}

    newstream_p->db_dir_hash_p = hash_p;

    status = db_dir_read(newstream_p);
    if (status != DNS_SUCCESS)
        {
	skulk_log(hash_p, cds_s_server_collectphase, cds_s_server_failread,
		  NULL, status);

	/* get rid of any return replica pointers stored in structure
	 */
	db_free_stream(newstream_p);
	goto leave_collect;
	}

    /* Get new replica set */
    status = find_attribute(newstream_p->db_dir.data_p,
        dns_AttributeName(DNS_REPLICAS), &replica_set_p, &idx);

    if (status != DNS_SUCCESS)
        {
	evl_AttributeMissing(ch_p, (FullName_t *)hash_p->dir_name,
	    dns_AttributeName(DNS_REPLICAS));
        dns_free((char *)newstream_p->db_dir.data_p);
	/* get rid of any return replica pointers stored in structure
	 */
        status = DNS_NAMESERVERBUG;
	db_free_stream(newstream_p);
	goto leave_collect;
        }

    max_replicas = NUM_DBSet(replica_set_p);
    check_replicas(replica_set_p, ring_list_p);

    /* If there aren't any left, no replicas were added that we need
     * to talk to while completing this collect phase
     */
    if (find_last_value(replica_set_p))
        {
        /* the replica set changed - let the other skulk finish */
        dns_free((char *)newstream_p->db_dir.data_p);
	/* get rid of any return replica pointers stored in structure
	 */
        status = DNS_OLDSKULK;
	db_free_stream(newstream_p);
	goto leave_collect;
        }

    /*
     * done with the replicaset we just read
     */
    dns_free((char *)newstream_p->db_dir.data_p);
    db_free_stream(newstream_p);


   /*
    * See if we successfully contacted everyone in the ring
    */
    status = DNS_SUCCESS;
    ring_element_p = ring_list_p->ring_first;
    for (cnt = 0; cnt < ring_list_p->ring_count; ring_element_p++, cnt++)
	if (ring_element_p->ring_status != DNS_SUCCESS)
	    {
	    FullName_t *chname_p;
	    /*
	     * Remember error
	     */
	    status = ring_element_p->ring_status;

	    replica_parse(ring_element_p->ring_hint_p,
		NULL, NULL, NULL, NULL, &chname_p);

	    skulk_log(hash_p, cds_s_server_collectphase, 
		      cds_s_server_failcollectch, 
		      deb_ascii_FullName(namebuf,chname_p),
		      status);
            /* 
	     * get rid of any return replica pointers stored in structure
             */
	    goto leave_collect;
	    }


leave_collect:
    cleanup_ring_list (ring_list_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <collect(%s,%s,) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_Timestamp(skulktimeBuf,skulktime_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}




