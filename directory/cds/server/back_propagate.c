/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: back_propagate.c,v $
 * Revision 1.1.8.2  1996/02/18  19:35:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:47  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:28:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:27  root]
 * 
 * Revision 1.1.6.3  1994/08/03  19:03:33  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:18:46  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:42:51  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:07  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:09:59  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:38:08  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  14:19:13  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:19:19  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/07  21:12:13  peckham
 * 	 Gen 46, 17-Nov-1992, Alan Peckham
 * 	      OT#6074: directory propagates sometimes ignored. Again, good
 * 	      work by John Grober. Checks in back_propagate_sched() are incorrect.
 * 	      While I am at it, optimize this search by putting the directory or
 * 	      entry CTS in prop_element_cts and keying on this.
 * 
 * 	      Also merge Dave Griffin fix:
 * 
 * 	  17-JUL-1992 09:36  by Dave Griffin
 * 	      Modify stream usage so that it is now legal.  Prior generations
 * 	      stored the propagation stream in the file_info block as a supposed
 * 	      performance optimization.  Obtaining a stream isn't all that
 * 	      expensive, and this is the only place we make such attempts.  By
 * 	      caching the stream, it ends up being used by multiple threads, which
 * 	      breaks a stream usage assertion.   This change removes the cached
 * 	      stream.
 * 	[1992/12/07  21:11:18  peckham]
 * 
 * Revision 1.1.2.2  1992/06/30  20:09:38  peckham
 * 	OT#4483: Made back_propagate() initialize dir_prop_status
 * 	to DNS_SUCCESS.  The routine always sets to failure, but never
 * 	success -- rendering the instrumentation a bit useless.
 * 	[1992/06/30  20:08:59  peckham]
 * 
 * Revision 1.1  1992/01/19  15:25:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * MODULE: back_propagate.c
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
 * MODULE DESCRIPTION:
 *
 * This module contains routines to propagate an update
 * to multiple replicas.  Only one propagate runs at a time, so
 * additional requests are queued.  One stream is permanently 
 * allocated to be used for propagates from a clearinghouse.
 */
#include <server.h> 
#include <back.h>
#include <clerk.h>
#include <dbdef.h>
#include <events.h>   
#include <find.h>
#include <names.h>
#include <replica.h>
#include <uid.h>

/* 
 * Multiple update buffers are allocate at once so we can batch
 * update messages.
 */
#define UPD_BUFFERS 4

/*
 * Local prototypes
 */
static void
back_propagate (
    db_stream_t		*,
    int			,
    Timestamp_t		*);

static dthread_address_t
back_propagate_fork (
    file_info_t		*);

static void
back_propagate_cleanup (
    db_stream_t		*);


/* -------------------------------------------------------------------------- */
           
/*                           
 * back_propagate
 *	This function sends updates for one entry to all replicas
 *	Propagation is an optimization, errors are not returned and
 *	an attempt is made to continue.
 *
 * Inputs:                                   
 *      stream_p  = pointer to the stream descriptor
 *	type = type of record being propagated
 *	begin_p = pointer to beginning ts
 *        
 * Outputs:
 *	none
 */                          
static void 
back_propagate (db_stream_t  *stream_p,
                int          type,
                Timestamp_t  *begin_p)
{   
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_Timestamp(beginBuf)
    Timestamp_u		end_ts;
    dir_hash_entry_t	*hash_p = stream_p->db_dir_hash_p;
    nsgbl_ch_t		*ch_p = stream_p->db_file_p->file_ch_p;
    nsgbl_ns_t		*ns_p = ch_p->ch_nameserver;
    Timestamp_t		*allupto_ts_p;
    struct update_buffer *update_buf_p;
    byte_t		*current_size_p;
    byte_t		*current_upd_p;
    DBSet_t		*set_p;
    DBSetMember_t	*value_p;
    ObjUID_t		*epoch_p;
    db_data_t		*data_p;
    int			i, index;
    int			rp_len;
    dns_status_t	status;               

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >back_propagate(%s,%d,%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        type,
        deb_ascii_Timestamp(beginBuf,begin_p)));

    hash_p->dir_prop_status = DNS_SUCCESS;   /* Assume a successful propagate */

    /* Get a ts to use as end of propagate range */
    if (ts_new((Timestamp_t *)end_ts) != DNS_SUCCESS) 
	{
	hash_p->dir_prop_status = DNS_PROPAGATE_FAIL;	/* Remember failure */
	return;
	}

    /* Remember time of this attempt to propagate */
    COPY_Timestamp( end_ts, hash_p->dir_prop_time);

    /* If the end time is not past the beginning timestamp 
     * generate a new end timestamp.  This can happen if
     * the beginning timestamp was generated on another system
     */
    if (dnsCmpCTS((dns_cts *)begin_p, (dns_cts *)end_ts) >= 0)
        {
	time_quad_t this_time;

        /* Make up another timestamp as ending point
         * add future skew to beginning time and use that
         */                         
	COPY_Time(begin_p->ts_time, &this_time);
	ADD_time_quad(&this_time, &ns_p->ns_future_skew, &this_time);
	COPY_Time(&this_time, ((Timestamp_t *)end_ts)->ts_time);
        }

    /* 
     * Allocate one large buffer in the hope that all updates will fit
     * and we can send them all at once to each replica.  The alternative
     * is to create links to each replica, send some of the
     * updates, return to the database to find the rest and re-create
     * the links to send the rest.  More efficient to do it in one batch.
     */
    if (!(update_buf_p =
	(update_buffer_t *)dns_get_buffer(update_buffer_bt, 
                                          (UPDATEPACKETMAX * UPD_BUFFERS))))
	{
	hash_p->dir_prop_status = DNS_PROPAGATE_FAIL;	/* Remember failure */	
    	return;
	}

    /* Initialize buffer */
    update_buf_p->ub_count = 0;
    update_buf_p->ub_total =
	(UPDATEPACKETMAX*UPD_BUFFERS)-sizeof(update_buffer_t);
    update_buf_p->ub_remaining = update_buf_p->ub_total;
    current_size_p = (byte_t *)&update_buf_p[1];
    current_upd_p = SKIP_longword(current_size_p);

    /* Find allupto */                          
    status = find_attribute(stream_p->db_dir.data_p, 
        dns_AttributeName(DNS_ALLUPTO), &set_p, &index);

    if (status != DNS_SUCCESS)
        {    
	evl_AttributeMissing(ch_p, (FullName_t *)hash_p->dir_name,
	    dns_AttributeName(DNS_ALLUPTO));
	hash_p->dir_prop_status = DNS_PROPAGATE_FAIL;	/* Remember failure */
	goto free_buffer;
        }
        
    /* Point at only member of set */
    value_p = (DBSetMember_t *)DATA_DBSet(set_p);
    allupto_ts_p = (Timestamp_t *)value_p->dm_data;

    /* If the update is less then allupto, propagation has already 
     * occurred so we do not need to update; this is the propagate 
     * of allupto just after a skulk.
     */
    if (dnsCmpCTS((dns_cts *)begin_p, (dns_cts *)allupto_ts_p) < 0)
        /* don't bother to send anything */
	goto free_buffer;

    /* find the epoch */
    status = find_attribute(stream_p->db_dir.data_p, 
        dns_AttributeName(DNS_EPOCH), &set_p, &index);

    if (status != DNS_SUCCESS)
        {    
	evl_AttributeMissing(ch_p, (FullName_t *)hash_p->dir_name,
	    dns_AttributeName(DNS_EPOCH));
	hash_p->dir_prop_status = DNS_PROPAGATE_FAIL;	/* Remember failure */
	goto free_buffer;
        }        

    /* Get pointer to only value */
    value_p = (DBSetMember_t *)DATA_DBSet(set_p);
    epoch_p = (ObjUID_t *)value_p->dm_data;

    /* Find the replica set */
    status = find_attribute(stream_p->db_dir.data_p, 
        dns_AttributeName(DNS_REPLICAS), &set_p, &index);

    if (status != DNS_SUCCESS)
        {    
	evl_AttributeMissing(ch_p, (FullName_t *)hash_p->dir_name,
	    dns_AttributeName(DNS_REPLICAS));
	hash_p->dir_prop_status = DNS_PROPAGATE_FAIL;	/* Remember failure */
	goto free_buffer;
        }              

     /* Propagating a directory or an entry */
    if (type == ET_directory)
	data_p = &stream_p->db_dir;
    else
	data_p = &stream_p->db_entry;
	
    status = gather_updates(stream_p, data_p, begin_p, (Timestamp_t *)end_ts,
	&current_upd_p, &current_size_p, update_buf_p, NULL);

    if (status != DNS_SUCCESS)
	{
	hash_p->dir_prop_status = DNS_PROPAGATE_FAIL;	/* Remember failure */
        goto free_buffer;
	}

    /* Send updates to replicas, continue on errors */ 
    for (index = 0; index < NUM_DBSet(set_p); index++)
        {
	ReplicaPointer_t *replica_p;

	/* Get pointer to replica value */
        value_p = (DBSetMember_t *)INDEX_DBSet(set_p, index);

	/* If not in use, move onto the next */
        if (!EXT8(value_p->dm_value_present) || !EXT8(value_p->dm_valid))
	    continue;

        replica_p = (ReplicaPointer_t *)value_p->dm_data;
        replica_parse(replica_p, &rp_len,
		NULL, NULL, NULL, NULL);
        
        /* Continue if replica I am pointing at is not myself */
        if (NE_ObjUID(replica_p->rp_CHID, &ch_p->ch_uid))
            {            
	    current_size_p = (byte_t *)&update_buf_p[1];
	    for (i = 0; i < update_buf_p->ub_count; i++)
	        {
		/* Send updates to replica.  */
                if ((status = clerk_do_updates(epoch_p, replica_p, rp_len, 
			EXT32(current_size_p), SKIP_longword(current_size_p)))
			 != DNS_SUCCESS)
		    {		/* Remember failure of propagate */	
		    hash_p->dir_prop_status = DNS_PROPAGATE_FAIL;
		    }
		
		/* Unpack buffer */
	        current_size_p = SKIP_bytes(SKIP_longword(current_size_p),
			EXT32(current_size_p));
            
	        } /* End of do_update loop */
                      
            } /* end of skiping the matching replica */

        }/* end of looping thru the replicas */

free_buffer:
    dns_free_buffer((char *)update_buf_p);
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <back_propagate status(%d)",
        hash_p->dir_prop_status));
}


/* -------------------------------------------------------------------------- */

/*
 * back_propagate_fork
 *	This routine is run when the propagate queue goes from
 *	empty to not empty.  Only one thread runs at a time.
 *	This routine is the entry point for a new thread scheduled
 *	from back_propagate_sched.  
 *	Since propagates are an optimization, errors are not returned
 *	to the caller, and we try to continue if appropriate.
 *
 * Input:
 *	file_p = The file that needs propagation.
 *	
 * Output:
 *
 * Side Effects:
 *	Frees storage allocated in back_propagate_sched
 *	Closes stream opened by db_enable
 */
static dthread_address_t 
back_propagate_fork (file_info_t *file_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    db_stream_t *stream_p;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >back_propagate_fork(%s)",
        deb_ascii_file_info(fileBuf,file_p)));

    /* Delay start of propagation for 1 minute */

    if ((thstatus = dthread_delay(60.0)) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADDELAY_A_MSG, (long)thstatus);
    }

    if (db_get_stream(file_p, &stream_p, OP_ReadAttribute) != DNS_SUCCESS)
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug7,
            " >back_propagate_fork: unable to get stream\n"));

	return((dthread_address_t)-1);
    }

    dthread_cleanup_push(back_propagate_cleanup, stream_p);

    /* 
     * Process all propagates waiting, one at time.  Propagates
     * are queued in a circular list.  Loop until
     * queue is empty, points at itself.
     */
    while (TRUE)
	{
	prop_queue_element_t *prop_p;
	int old_cancel;
	dns_status_t status;

	/* 
	 * See if a management request to close this clearinghouse
         * arrived while this thread was being scheduled or while
	 * this thread processed another propagate
	 */
	if (file_p->file_state != file_open)
	    {
	    break;	
	    }

	dthread_testcancel();

	if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
	}

	    /* Beginning of the 'protected code' */
	    {
		prop_p = (prop_queue_element_t *)LIST_FIRST(&file_p->file_prop_wait);
		if (prop_p)
		    remove_list(prop_p);
	    }

	    /* End of the 'protected code' */

	if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	}

	if (!prop_p) break;

	old_cancel = dthread_setcancel(CANCEL_OFF);
	stream_p->db_dir_hash_p = NULL;

	/* Set up keys for directory lookup */
	/* Build a fullname containing directory ID and null simple name */
	names_new((ObjUID_t *)prop_p->prop_diruid, NULL,
		(FullName_t *)stream_p->db_key);	

	status = db_dir_read(stream_p);

        if (status == DNS_SUCCESS) 
	    {
	    /* If propagate is for an entry in the directory, need to read it */
	    if (prop_p->prop_type != ET_directory)
		{
		/* Set up entry fullname */
		names_new((ObjUID_t *)stream_p->db_dir.data_uid, 
			(SimpleName_t *)prop_p->prop_name,
			(FullName_t *)stream_p->db_key);
		/* Set up entry record type */
	    	stream_p->db_entry.data_type = prop_p->prop_type;
	    	status = db_entry_read(stream_p);
		if (status == DNS_SUCCESS)
		    back_propagate(stream_p, prop_p->prop_type, 
			(Timestamp_t *)prop_p->prop_begin);
		db_entry_release(stream_p);
	        }
	    else
		back_propagate(stream_p, prop_p->prop_type, 
			(Timestamp_t *)prop_p->prop_begin);

	    /* Release storage used by directory */
	    db_dir_release(stream_p);	
	    } /* End read directory successfully */

	dns_free((char *)prop_p);
	(void)dthread_setcancel(old_cancel);
	} /* End for loop checking queue */

    dthread_cleanup_pop(1);

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{
	    if (file_p->file_prop_state == file_prop_On)
		file_p->file_prop_state = file_prop_Off;

	    if (file_p->file_prop_state != file_prop_Rundown)
	    {
		(void)dthread_detach(&file_p->file_propagator);
		dthread_assign(dthread_null, &file_p->file_propagator);
	    }
	}

        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }
    
    return(0);
}

static void 
back_propagate_cleanup (db_stream_t *stream_p)
{
    (void)db_close_stream(stream_p);
}


/* -------------------------------------------------------------------------- */

/*
 * back_propagate_sched
 *	This routines inserts a propagate request in a queue,
 *	which will run later.  The request is ignored if
 *	one already exists which covers this timestamp.
 *
 * Input:
 *	stream_p = pointer to stream descriptor
 *	type = record type for propagate
 *	ts_p = pointer to timestamp of update to propagate
 *
 * Output:
 *	none
 *
 * Side Effects:
 *	Allocate storage which is freed by back_propagate_fork
 */
void 
back_propagate_sched (db_stream_t  *stream_p,
                      int          type,
                      Timestamp_t  *ts_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_Timestamp(tsBuf)
    file_info_t *const file_p = stream_p->db_file_p;
    Timestamp_t *const cts_p = (Timestamp_t *)((type == ET_directory)
				? stream_p->db_dir.data_cts
				: stream_p->db_entry.data_cts);
    prop_queue_element_t *prop_p;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >back_propagate_sched(%s,%d,%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        type,
        deb_ascii_Timestamp(tsBuf,ts_p)));


    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{
	    /* 
	     * Check all propagates waiting to see if one of them will cover
	     * this update. Propagates are queued in a circular list.  Loop 
	     * until points back at queue head or find a match.
	     */
	    for (prop_p =(prop_queue_element_t *)file_p->file_prop_wait.next_p;
		 prop_p != (prop_queue_element_t *)&file_p->file_prop_wait;
		 prop_p = (prop_queue_element_t *)prop_p->prop_wait.next_p)
	    {
		/* Check if this queued propagate the right one */
		if (EQ_Timestamp(cts_p, prop_p->prop_element_cts))
		{
		    /* 
		     * If we have a matching propagate, see if beginning
		     * timestamp should be adjusted.  May already be covered.
		     * No need to schedule another propagate.
		     */
		    if (dnsCmpCTS((dns_cts *)ts_p, 
				  (dns_cts *)prop_p->prop_begin) < 0)
			COPY_Timestamp(ts_p, prop_p->prop_begin);

		    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
		    {
			dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG,
				       (long)thstatus);
		    }
		    
		    /* Otherwise this timestamp is after, will be picked up */
		    return;
		}
	    }
	}
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    /* No match was found, allocate a queue element */
    prop_p = (prop_queue_element_t *)dns_malloc(sizeof(*prop_p));
    if (!prop_p) return;

    /* Initialize new queue element */
    COPY_ObjUID(stream_p->db_dir.data_uid, prop_p->prop_diruid);
    COPY_Timestamp(cts_p, prop_p->prop_element_cts);
    COPY_Timestamp(ts_p, prop_p->prop_begin);
    prop_p->prop_type = type;

    /* If propagate for a directory entry, fill in remaining thread data */
    if (type != ET_directory)
	{
	FullName_t *fn_p = (FullName_t *)stream_p->db_key;

	COPY_SimpleName(fn_p->fn_name, prop_p->prop_name);
	}

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{
	    /* Add thread element to end of queue */
	    insert_list(&file_p->file_prop_wait, &prop_p->prop_wait);
	}
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }
    
    if (file_p->file_prop_state == file_prop_Off)
	(void)back_propagate_start(file_p);
}


/* -------------------------------------------------------------------------- */

dns_status_t 
back_propagate_start (file_info_t *file_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    dns_status_t	status = DNS_SUCCESS;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >back_propagate_start(%s)",
        deb_ascii_file_info(fileBuf,file_p)));

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{
	    if (file_p->file_prop_state != file_prop_On)
	    {
		file_p->file_prop_state = file_prop_Off;
		/* 
		 * If the prop_stream is available, schedule propagate
		 * to run.  It will start processing in about 1 minute.  This 
		 * allows other updates to the same entry to be batched.
		 */
		if (LIST_FIRST(&file_p->file_prop_wait))
		{
		    extern dthread_attr_t back_propagate_fork_attr;

		    /* Fork a new thread to start processing propagates */
		    if (0 <= dthread_create(&file_p->file_propagator,
				    &back_propagate_fork_attr,
				    (dthread_start_routine)back_propagate_fork,
			            (dthread_address_t)file_p))
			file_p->file_prop_state = file_prop_On;
		    else
			status = DNS_NONSRESOURCES;
		}
	    }
	}
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <back_propagate_start(%s) return(%d)",
        deb_ascii_file_info(fileBuf,file_p),
        status));

    return(status);
}


/* ------------------------------------------------------------------------- */

dns_status_t 
back_propagate_stop (file_info_t *file_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >back_propagate_stop(%s)",
        deb_ascii_file_info(fileBuf,file_p)));

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{
	    if (file_p->file_prop_state == file_prop_On)
		file_p->file_prop_state = file_prop_Rundown;
	}
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    if (!dthread_equal(file_p->file_propagator, dthread_null))
	{
	dthread_address_t join_status;

	(void)dthread_join(file_p->file_propagator, &join_status);
	(void)dthread_detach(&file_p->file_propagator);
	dthread_assign(dthread_null, &file_p->file_propagator);
	}

    file_p->file_prop_state = file_prop_Stopped;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <back_propagate_stop(%s) return(%d)",
        deb_ascii_file_info(fileBuf,file_p),
        DNS_SUCCESS));

    return(DNS_SUCCESS);
}


/* -------------------------------------------------------------------------- */

void 
back_propagate_kill (file_info_t *file_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >back_propagate_kill(%s)",
        deb_ascii_file_info(fileBuf,file_p)));

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{
	    file_p->file_prop_state = file_prop_Cancel;

	    if (!dthread_equal(file_p->file_propagator, dthread_null))
		(void)dthread_cancel(file_p->file_propagator);

	    while (TRUE)
	    {
		prop_queue_element_t *const prop_p =
		   (prop_queue_element_t *)LIST_FIRST(&file_p->file_prop_wait);

		if (!prop_p) break;

		remove_list(&prop_p->prop_wait);
		dns_free((char *)prop_p);
	    }

	}
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    (void)back_propagate_stop(file_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <back_propagate_kill(%s)",
        deb_ascii_file_info(fileBuf,file_p)));
}
