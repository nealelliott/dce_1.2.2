/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: back_combine.c,v $
 * Revision 1.1.6.2  1996/02/18  19:35:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:31  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:28:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:07  root]
 * 
 * Revision 1.1.4.4  1994/08/03  19:03:24  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:18:03  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:42:42  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:57  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/14  14:51:01  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:18:14  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:09:36  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:37:08  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:18:28  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:18:37  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:24:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module back_combine.c
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
 * Module description:
 *
 * Functions in this module perform the architectural combine
 * module.  It sends all the changes from a secondary (or master) back
 * to the replica performing the skulk.  During a new EPOCH, a readonly
 * replica may be combined.
 *
 */
#include <server.h> 
#include <back.h>
#include <clerk.h>
#include <dbdef.h>
#include <events.h>
#include <find.h>
#include <names.h>
#include <replica.h>
#include <security.h>
#include <sets.h>
#include <tlog.h>
#include <uid.h>

/*
 * internal data protected by dns_server_mutex.
 */                   
#define MAX_COMBINE 5
int gl_forked_combine;

/*
 * Local prototypes
 */
static dns_status_t
get_replica (
    FullName_t		*,
    byte_t	       **);

/*
 *   get_replica
 *	Given a clearinghouse name, this routine builds a replica pointer
 *
 * Input:
 *	ch_fname_p 	pointer to clearinghouse name
 *     	buf_pp		pointer to return malloced storage
 *
 * Value:
 * 	status
 */
static dns_status_t 
get_replica (FullName_t  *ch_fname_p,
             byte_t      **buf_pp)
{
    DEB_ASCII_BUF_FullName(ch_fnameBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    struct buf {
	ReplicaPointer_t rp;
	bytes_u		rp_extra[READ_BUF_SIZE];
	bytes_u		ret[READ_BUF_SIZE];
	bytes_u		ret2[READ_BUF_SIZE];
    } *buf_p;
    MemberValue_t	*set_mem_p;
    Set_t		*retset_p;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >get_replica(%s,%p)",
        deb_ascii_FullName(ch_fnameBuf,ch_fname_p),
        buf_pp));

   /*                                                      
    * get the CDS_ObjectUUID attribute from the clearinghouse obj
    * allocate room for a replica pointer, and a full name.  Also a couple
    * of buffers to put towers in to transform and test them.
    */
    buf_p = (struct buf *)dns_malloc(sizeof(*buf_p));
    if (!(*buf_pp = (byte_t *)buf_p)) {
        status = DNS_NONSRESOURCES;
	goto leave_get_replica;
    }

    /* Output cannot be on the stack */
    /* Read objectuid */
    status = clerk_read_attribute(ch_fname_p, ET_object,
		dns_AttributeName(DNS_OBJECTUID), buf_p->ret,
		NULL, NULL);

    if (status != DNS_SUCCESS)
        {
	dns_free((char *)buf_p);
	goto leave_get_replica;
	}

    /* Save clearinghouse uid */
    set_mem_p = (MemberValue_t *)SKIP_byte(buf_p->ret);
    COPY_ObjUID(set_mem_p->mv_value, buf_p->rp.rp_CHID);

    /* get the address too, as the clerk can't yet, first try towers */
    status = clerk_read_attribute(ch_fname_p, ET_object,
		dns_AttributeName(DNA_TOWERS), buf_p->ret,
		NULL, NULL);
    retset_p = (Set_t *)SKIP_byte(buf_p->ret);
    if (status != DNS_SUCCESS)
      {
	dns_free((char *)buf_p);
	goto leave_get_replica;
      }
    
    (void)replica_build(&buf_p->rp, (ObjUID_t *)buf_p->rp.rp_CHID,
		  retset_p, RT_master, ch_fname_p);
    status = DNS_SUCCESS;

leave_get_replica:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <get_replica(%s,%p) return(%s)",
        deb_ascii_FullName(ch_fnameBuf,ch_fname_p),
        buf_pp,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

           
/*
 * combine 
 * 	This function copies all updates within a time period back to the
 *      requesting clearinghouse for one directory.
 *                                        
 * Inputs:                  
 *      user_p       pointer to user descriptor
 *      dir_uid_p    pointer to directory uid
 *      skulktime_p  pointer to the proposed skulktime
 *      allupto_p    pointer to oldest update wanted
 *      epoch_p      pointer to the epoch uid
 *      ch_fname_p   pointer to the full name of the ch to return to
 *      next_uid_p   pointer to the next ring pointer uid to return
 *
 *                   
 * Outputs:                                                         
 *                                                                       
 *      gathers up the updates for all the entries in a directory between
 *      two times, and returns them to the caller using the local clerk to
 *      the remote do_updates function.
 *
 * Notes:                                                      
 *     
 */                          
dns_status_t 
combine (user_descriptor_t  *user_p,
         ObjUID_t           *dir_uid_p,
         Timestamp_t        *skulktime_p,
         Timestamp_t        *allupto_p,
         ObjUID_t           *epoch_p,
         FullName_t         *ch_fname_p,
         ObjUID_t           *next_uid_p)
{
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_ObjUID(dir_uidBuf)
    DEB_ASCII_BUF_Timestamp(skulktimeBuf)
    DEB_ASCII_BUF_Timestamp(alluptoBuf)
    DEB_ASCII_BUF_ObjUID(epochBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    struct tlog_dsc	tlog;
    bytes_u 		updbuf[2*(sizeof(Update_u) - MAXATTRIBUTE +
				sizeof(Timestamp_u))];
    Timestamp_u		current_time;
    DBSet_t		*set_p;
    DBSetMember_t	*value_p;
    Timestamp_t		*allupto_uid_p;
    FullName_t		*name_p;
    copy_context_t	*context_p;
    ReplicaPointer_t	*rp,*replica_p;
    update_buffer_t	*update_buf_p;
    db_stream_t		*stream_p;
    nsgbl_ch_t		*ch_p;
    int			size,idx;
    int			max_replicas;
    int			i;
    byte_t		replica_state;
    dns_status_t	status;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >combine(%s,%s,%s,%s,%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_ObjUID(dir_uidBuf,dir_uid_p),
        deb_ascii_Timestamp(skulktimeBuf,skulktime_p),
        deb_ascii_Timestamp(alluptoBuf,allupto_p),
        deb_ascii_ObjUID(epochBuf,epoch_p)));

    /*          
     * check to see that skulktime is greater than current time -
     * if greater than future skew, release lock and return error,
     * if less than future skew, release the lock and wait.
     *
     * THIS IMPLEMENTATION IS A MINOR HACK!!!  Please note that
     * wait is done by looping, whereas calculating the exact time to
     * wait would be much better.  The threads package doesn't have such
     * a feature, and there's no time to write one right now.
     */
    status = ts_new((Timestamp_t *)current_time);
    if (status != DNS_SUCCESS)
	goto leave_combine;


    while (dnsCmpCTS((dns_cts *)current_time, (dns_cts *)skulktime_p) <=0)
        {
        if (!ts_future_skew(skulktime_p)) {
            status = DNS_BADCLOCK;
	    goto leave_combine;
	}
        (void)dthread_delay(10.0);	/* Re-evaluate every 10 seconds */
	/* Re-fetch the current time */
        status = ts_new((Timestamp_t *)current_time);
        if (status != DNS_SUCCESS)
	    goto leave_combine;
	}

    status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_ModifyAttribute);
    if (status != DNS_SUCCESS)
	goto leave_combine;

    ch_p = stream_p->db_file_p->file_ch_p;

    /* Set up key */
    name_p = (FullName_t *)stream_p->db_key;
    COPY_ObjUID(dir_uid_p, name_p->fn_root);

    status = db_dir_read_for_update(stream_p);
    if (status != DNS_SUCCESS)
	{
	(void)db_close_stream(stream_p);
	if (status == DNS_UNKNOWNENTRY)
	    status = DNS_NOTAREPLICA;
	goto leave_combine;
	}

    /* Check acs */
    status = security_check(user_p, stream_p, Super_check, ET_directory,
			    NULL);
    if (status != DNS_SUCCESS)
        {                                                            
        security_log(user_p, (FullName_t *)stream_p->db_dir_hash_p->dir_name,
                     ch_p);
	(void)db_close_stream(stream_p);
        if (status == DNS_ACCESSDENIED)
	    status = DNS_UNTRUSTEDCH;
	goto leave_combine;
        }


    /* find the CDS_ReplicaState */
    status = find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(DNS_REPLICASTATE),
		&set_p, &idx);
    if (status != DNS_SUCCESS)
        {        
	evl_AttributeMissing(ch_p,
	    (FullName_t *)stream_p->db_dir_hash_p->dir_name,
	    dns_AttributeName(DNS_REPLICASTATE));
	(void)db_close_stream(stream_p);
        status = DNS_NAMESERVERBUG;
	goto leave_combine;
        }                 

    value_p = (DBSetMember_t *)DATA_DBSet(set_p);
    replica_state = EXT8(value_p->dm_data);
    if (replica_state == RS_dead)
        {
        (void)db_close_stream(stream_p);
        status = DNS_NAMESERVERBUG;
	goto leave_combine;
        }                         
        
    /* check for correct EPOCH */
    status = find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(DNS_EPOCH), &set_p, &idx);
    if (status != DNS_SUCCESS)
        { 
	evl_AttributeMissing(ch_p,
	    (FullName_t *)stream_p->db_dir_hash_p->dir_name,
	    dns_AttributeName(DNS_EPOCH));
	(void)db_close_stream(stream_p);
        status = DNS_NAMESERVERBUG;
	goto leave_combine;
        }    

    value_p = (DBSetMember_t *)DATA_DBSet(set_p);
    if (NE_ObjUID(value_p->dm_data, epoch_p))
        {
        (void)db_close_stream(stream_p);
        status = DNS_BADEPOCH;
	goto leave_combine;
        }

    /* Make sure resources exist to continue */

    if ((thstatus = dthread_lock(&(dns_server_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{
	    if (gl_forked_combine < MAX_COMBINE)
		gl_forked_combine++;	/* NOTE: value != NULL */
	    else
		value_p = NULL;		/* Use value_p as a flag... */
	}
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(dns_server_mutex))) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    if (!value_p)		/* too many ? */
        {
        (void)db_close_stream(stream_p);
        status = DNS_NONSRESOURCES;
	goto leave_combine;
        }

    /* Check for old skulk */
    status = find_attribute(stream_p->db_dir.data_p, 
		dns_AttributeName(DNS_LASTSKULK), &set_p, &idx);

    if (status == DNS_SUCCESS)
    {
	value_p = (DBSetMember_t *)DATA_DBSet(set_p);
	if (dnsCmpCTS((dns_cts *)skulktime_p, (dns_cts *)value_p->dm_data) < 0)
	{
            (void)db_close_stream(stream_p);
	    
	    if ((thstatus = dthread_lock(&(dns_server_mutex))) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
	    }
	    
   	        /* Beginning of the 'protected code' */
 		
	        --gl_forked_combine;
        
	        /* End of the 'protected code' */

	    if ((thstatus = dthread_unlock(&(dns_server_mutex))) < 0)
	    {
		dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	    }
            
	    status = DNS_OLDSKULK;
	    goto leave_combine;
	}
    }

    /* Update lastskulk */
    (void)set_timestamp_set(&stream_p->db_dir, skulktime_p, 
		dns_Attribute(DNS_LASTSKULK), (Update_t *)updbuf, &size);
    tlog.cnt = 1;
    tlog.len = size;
    tlog.buf_p = updbuf;

    if (replica_state != RS_newReplica)
	{
        (void)set_timestamp_set(&stream_p->db_dir, allupto_p,
			dns_Attribute(DNS_ALLUPTO),
			(Update_t *)&updbuf[size], &size);
	++tlog.cnt;
	tlog.len += size;
	}
                     
    status = db_dir_update(stream_p, &tlog);
    if (status != DNS_SUCCESS)
    { 
        (void)db_close_stream(stream_p);

	if ((thstatus = dthread_lock(&(dns_server_mutex))) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
	}

           /* Beginning of the 'protected code' */

	   --gl_forked_combine;

          /* End of the 'protected code' */

	if ((thstatus = dthread_unlock(&(dns_server_mutex))) < 0)
	{
	  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);

	}

	goto leave_combine;
    }
    
    /* find the AllUpTo attribute */   
    status = find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(DNS_ALLUPTO), &set_p, &idx);
    if (status != DNS_SUCCESS)
    {                
	evl_AttributeMissing(ch_p,
	    (FullName_t *)stream_p->db_dir_hash_p->dir_name,
	    dns_AttributeName(DNS_ALLUPTO));
	(void)db_close_stream(stream_p);

	if ((thstatus = dthread_lock(&(dns_server_mutex))) < 0)
	{
            dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
	}

	    /* Beginning of the 'protected code' */

	    --gl_forked_combine;

	    /* End of the 'protected code' */

	if ((thstatus = dthread_unlock(&(dns_server_mutex))) < 0)
	{
	    dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	}
        status = DNS_NAMESERVERBUG;
	goto leave_combine;
    }

    value_p = (DBSetMember_t *)DATA_DBSet(set_p);
    allupto_uid_p = (Timestamp_t *)value_p->dm_data;

    /* Ringpointer is obsolete, just return NULL */
    ZERO_ObjUID(next_uid_p);

    /* Only sending updates to one replica so no need to batch them */
    size = UPDATEPACKETMAX + sizeof(update_buffer_t) + sizeof(int);
    update_buf_p = (update_buffer_t *)dns_malloc(size);
    if (!update_buf_p)
	status = DNS_NONSRESOURCES;
    else
        {    
        update_buf_p->ub_total = size - sizeof(update_buffer_t);
        
        /* Process replica based on state */                     
        switch (replica_state)
            {                   
	    case RS_newDir:                                
	    case RS_dyingDir:
	    case RS_dead:     
		evl_WrongState(ch_p,
			(FullName_t *)stream_p->db_dir_hash_p->dir_name,
			replica_state);
		status = DNS_NAMESERVERBUG;
		break;
	    case RS_newReplica:
		/* nothing to return but ringpointer, already setup */
		status = DNS_SUCCESS;
	        break;
	    case RS_on:
	    case RS_dyingReplica:
		/* Locate replica identified by ch_name_p */
		replica_p = NULL;
		rp = NULL;
		status = find_attribute(stream_p->db_dir.data_p,
			dns_AttributeName(DNS_REPLICAS), &set_p, &idx);
		if (status == DNS_SUCCESS)
		    {        
		    max_replicas = NUM_DBSet(set_p);
		    for (i = 0; i < max_replicas; i++)
			{
			value_p = (DBSetMember_t *)INDEX_DBSet(set_p, i);            
			if (!EXT8(value_p->dm_value_present) ||
				!EXT8(value_p->dm_valid))
			    continue; 

			(void)replica_parse(
				(ReplicaPointer_t *)value_p->dm_data, NULL,
				NULL, NULL, NULL, &name_p);
			if (names_fullmatch(ch_fname_p, name_p))
			    {   
			    replica_p = (ReplicaPointer_t *)value_p->dm_data;
			    break; /* leave the for loop */
			    }
			} /* end of for loop */
		    } /* end of if success on finding replica set */

		if (!replica_p)
		    {
		    /* Lookup the clearinghouse and build a replica */
		    status = get_replica(ch_fname_p, (byte_t **)&rp);
		    if (status != DNS_SUCCESS) break;
		    replica_p = rp;
		    }
            
	       /*
		* find all the updates in the directory 
		* after allupto and before skulktime
		* and send them back to the requesting clearinghouse
		*/
                context_p =(copy_context_t *)dns_malloc(sizeof(copy_context_t));
		context_p->ct_buffers_sent = 0;
		context_p->ct_replica_p = replica_p;
		context_p->ct_epoch_p = epoch_p;
		context_p->ct_spread_count = 0;
	
		status = copy_updates(stream_p, allupto_uid_p, skulktime_p,
			update_buf_p, context_p);

                dns_free((char *)context_p);
		if (rp) dns_free((char *)rp);
		break;                                  
        
	    } /* end of case */
	dns_free((char *)update_buf_p);
	} /* End if update_buf_p */

    /* clean some things up */                                          

    if ((thstatus = dthread_lock(&(dns_server_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */

	--gl_forked_combine;

        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(dns_server_mutex))) < 0)
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }
    
    (void)db_close_stream(stream_p);

leave_combine:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <combine(%s,%s,%s,%s,%s) return(%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_ObjUID(dir_uidBuf,dir_uid_p),
        deb_ascii_Timestamp(skulktimeBuf,skulktime_p),
        deb_ascii_Timestamp(alluptoBuf,allupto_p),
        deb_ascii_ObjUID(epochBuf,epoch_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}
