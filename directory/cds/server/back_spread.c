/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: back_spread.c,v $
 * Revision 1.1.8.2  1996/02/18  19:35:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:56  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:29:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:34  root]
 * 
 * Revision 1.1.6.5  1994/09/07  20:02:10  mccann
 * 	fix version handling OT:12004
 * 	[1994/09/07  18:41:37  mccann]
 * 
 * Revision 1.1.6.4  1994/08/23  20:01:05  mccann
 * 	allow root replicas to be removed: OT 11637
 * 	[1994/08/23  20:00:33  mccann]
 * 
 * Revision 1.1.6.3  1994/08/03  19:03:40  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:19:59  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:42:56  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:14  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:10:12  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:38:44  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:19:40  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:19:41  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/30  19:39:28  peckham
 * 	OT#2114: uninitialized variables.
 * 	[1992/06/30  19:38:44  peckham]
 * 
 * Revision 1.1  1992/01/19  15:25:01  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: back_spread.c
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
 * Abstract:
 * 	All functions associated with the SPREAD phase of the skulk.
 *
 */
 
/*
 *  Include files
 */
#include <server.h>
#include <back.h>
#include <clerk.h>
#include <dbdef.h>
#include <events.h>   
#include <find.h>
#include <names.h>
#include <replica.h>
#include <sets.h>
#include <tower.h>
#include <tlog.h>
#include <uid.h>

/*
 * Local prototypes
 */
static dns_status_t
spread_dead_replica (
    db_stream_t		*,
    DBSet_t		*,
    int			,
    ReplicaPointer_t	*);

static dns_status_t
spread_new_replica (
    db_stream_t		*,
    copy_context_t	*,
    Timestamp_t		*,
    update_buffer_t	*);


/*
 * spread_dead_replica
 *	Remove a dying replica, set its state to dead
 *
 * Input:
 *	stream_p     	pointer to stream descriptor
 *	replica_set_p   pointer to set of replicas
 *	workingon_len	length of replica pointer
 *	workingon_p 	pointer to replica
 *
 * Returns:
 *	DNS_SUCCESS
 *	clerk status
 *	other dns error on failure
 */
static dns_status_t 
spread_dead_replica (db_stream_t       *stream_p,
                     DBSet_t           *replica_set_p,
                     int               workingon_len,
                     ReplicaPointer_t  *workingon_p)
{
    Update_u		update;			 /* update block  */
    tlog_dsc_t		tlog;			 /* transaction log update  */
    Timestamp_u		deadts;			 /* timestamp of dead rep  */
    data_record_t	*dir_p;
    int			mecnt, len;
    byte_t		state;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >spread_dead_replica()"));

    /* Update dying replica state to dead */
    state = RS_dead;
    len = update_dns_build((Update_t *)update, UD_present,
		NullTimestamp, 	dns_Attribute(DNS_REPLICASTATE),
		BL_true, sizeof(state), &state);

    status = clerk_modify_replica((ObjUID_t *)stream_p->db_dir.data_uid,
	(Update_t *)update, len, workingon_p, workingon_len);
    if (status != DNS_SUCCESS) return(status);

   /*     
    * build an update packet to change CDS_Replicas set in
    * this replica
    */
    status = ts_new((Timestamp_t *)deadts);
    if (status != DNS_SUCCESS) return(status);
    tlog.len = update_dns_build((Update_t *)update, UD_absent,
		(Timestamp_t *)deadts, dns_Attribute(DNS_REPLICAS),
		BL_true, workingon_len, (byte_t *)workingon_p);

   /*
    * Save directory buffer and re-read directory for update 
    * to remove the replica from the replica set
    */           
    dir_p = stream_p->db_dir.data_p;  
    status = db_dir_read_for_update(stream_p);
    if (status != DNS_SUCCESS)
	{
	stream_p->db_dir.data_p = dir_p;
	return(status);
	}
                       
    status = set_insert(&stream_p->db_dir, (Update_t *)update, 
		dns_Attribute(DNS_REPLICAS), TRUE);
    if (status == DNS_SUCCESS)
 	{               
        /* Fix UTS */
	(void)set_timestamp_set(&stream_p->db_dir,
		(Timestamp_t *)((Update_t *)update)->ud_timestamp,
		dns_Attribute(DNS_UTS), NULL, NULL);

	/* Fix LastUpdate */
	db_dir_set_lastupdate(stream_p->db_dir_hash_p,
		 (Timestamp_t *)((Update_t *)update)->ud_timestamp);
	tlog.cnt = 1;
	tlog.buf_p = update;
	status = db_dir_update(stream_p, &tlog);
	}
    db_dir_release(stream_p);
    stream_p->db_dir.data_p = dir_p;
    return(status);
} 

/*
 * spread_new_replica
 *	Send changes to a new replica
 *
 * Input:
 *    	stream_p	pointer to stream descriptor
 *	context_p 	pointer to context block
 *	skulk_ts_p 	pointer to end skulk time
 *	update_buf_p	pointer to update buffer
 *	rab_p 		pointer to rab descriptor
 *
 * Value:
 *	clerk status
 */
static dns_status_t 
spread_new_replica (db_stream_t      *stream_p,
                    copy_context_t   *context_p,
                    Timestamp_t      *skulk_ts_p,
                    update_buffer_t  *update_buf_p)
{
    bytes_u		update[			 /* update storage  */
			    sizeof(Update_u) - MAXATTRIBUTE + 1];
    int			rp_len,			 /* len of replica pointer */
			len;			 /* len of update block  */
    byte_t		state;			 /* state of replica  */
    dns_status_t	status;			 /* current status  */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >spread_new_replica()"));

    /* Must send everything to a new replica */
    status = copy_updates(stream_p, NullTimestamp, skulk_ts_p,
                update_buf_p, context_p);
    if (status != DNS_SUCCESS) return(status);

    /* Get replica length */
    replica_parse(context_p->ct_replica_p, &rp_len, NULL,
	NULL, NULL, NULL);

   /*     
    * Build an update packet to change DNS$ReplicaState
    */
    state = RS_on;
    len = update_dns_build((Update_t *)update, UD_present,
		NullTimestamp, dns_Attribute(DNS_REPLICASTATE),
		BL_true, sizeof(state), &state);

    return(clerk_modify_replica((ObjUID_t *)stream_p->db_dir.data_uid,
		(Update_t *)update, len, 
		context_p->ct_replica_p, rp_len));
}

/*                                  
 *  spread 
 *	This function copies all the updates to the secondaries and read-only
 *	replicas.  It also checks that a replica that is dying is not a
 * 	crucial replica and cleans up dying replicas.
 *
 * Inputs:
 *      stream_p  	   pointer to stream descriptor containing
 *                            directory buffer 
 *      skulk_ts_p         pointer to the proposed timestamp of the skulk
 *                         which goes into LastSkulk before we start.
 *      replica_changed_p  flag return whether replica pointer set
 *                         changed since DNS$LastSkulk
 *
 * Outputs:
 *      min_version_p      Minimized ReplicaVersion of all replicas
 *                                                                
 * Returns:
 *	DNS_SUCCESS
 *      DNS_NONSRESOURCES
 *	DNS_NAMESERVERBUG
 *	other dns error code
 */                                                       
dns_status_t 
spread (db_stream_t      *stream_p,
        Timestamp_t      *skulk_ts_p,
        int              *replica_changed_p,
        VersionNumber_t  *min_version_p)
{                                    
    dir_hash_entry_t	*hash_p 		 /* directory hash entry ptr  */
			    = stream_p->db_dir_hash_p;
    nsgbl_ch_t		*ch_p 			 /* clearinghouse information */
			    = stream_p->db_file_p->file_ch_p;
    Timestamp_t		*allupto_ts_p;		 /* allupto timestamp  */
    ObjUID_t            *epoch_p;		 /* epoch timestamp  */
    spread_list_t	*spread_list_p;		 /* entire element list  */
    spread_element_t	*element_p;		 /* current element  */
    DBSet_t		*set_p, 		 /* current set  */
			*replica_set_p;		 /* current replica set  */
    DBSetMember_t	*value_p;		 /* current value  */
    ReplicaPointer_t	*returned_replica_p;	 /* replica hint  */
    FullName_t		*workingon_chname_p;	 /* name of clearinghouse  */
    update_buffer_t	*update_buf_p;		 /* update storage ptr  */
    byte_t		*buff_p; 		 /* work buffer for read  */
    MemberValue_t	*member_p;  		 /* set member ptr  */
    copy_context_t	*context_p;		 /* context ptr  */
    int			nonerootier;		 /* closest to root flag  */
    int			max_replicas, 		 /* maximum replicas  */
			cnt, index;		 /* temp indices  */
    int                 alloc_chunk;		 /* # replicas need storage */
    dns_status_t	status;			 /* current status  */
    dns_status_t        version_status 		 /* reading replicaversion */
			    = DNS_SUCCESS;	 /* status  */
    LOG_ASCII_BUF_FullName(namebuf)
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >spread()"));
 
    /*                                 
     * Now we have a copy of the directory record, but no lock on it.
     * find the allupto_ts_p
     */
    status = find_attribute(stream_p->db_dir.data_p,
        dns_AttributeName(DNS_ALLUPTO), &set_p, &index);

    if (status != DNS_SUCCESS)
    {   
	evl_AttributeMissing(ch_p, (FullName_t *)hash_p->dir_name,
	    dns_AttributeName(DNS_ALLUPTO));

        return(DNS_NAMESERVERBUG);
    }        

    /*
     * ALLUPTO is always a single valued set with no history
     */
    value_p = (DBSetMember_t *)DATA_DBSet(set_p);
    allupto_ts_p = (Timestamp_t *)value_p->dm_data;
    
    /* find the epoch */
    status = find_attribute(stream_p->db_dir.data_p,
        dns_AttributeName(DNS_EPOCH), &set_p, &index);

    if (status != DNS_SUCCESS)
    {    
	evl_AttributeMissing(ch_p, (FullName_t *)hash_p->dir_name,
	    dns_AttributeName(DNS_EPOCH));

        return(DNS_NAMESERVERBUG);
        }        
        
    /*
     * EPOCH is always a single valued set with no history
     */
    value_p = (DBSetMember_t *)DATA_DBSet(set_p);
    epoch_p = (ObjUID_t *)value_p->dm_data;
    
    /* Find the replica set */
    status = find_attribute(stream_p->db_dir.data_p,
        dns_AttributeName(DNS_REPLICAS), &replica_set_p, &index);

    if (status != DNS_SUCCESS)
    {

	evl_AttributeMissing(ch_p, (FullName_t *)hash_p->dir_name,
	    dns_AttributeName(DNS_REPLICAS));

        return(DNS_NAMESERVERBUG);
    }
        
    /* Find the replica version */
    status = find_attribute(stream_p->db_dir.data_p,
        dns_AttributeName(DNS_REPLICAVERSION), &set_p, &index);

    if (status == DNS_SUCCESS)
    {
        DBSetMember_t    *member_p;                    /* Work pointer */
        VersionNumber_t  *replica_version_p;
	/*
	 * DNS_REPLICAVERSION is always a single valued set with no history
	 */
        member_p = (DBSetMember_t *)DATA_DBSet(set_p);
        replica_version_p = (VersionNumber_t *)member_p->dm_data;
        COPY_VersionNumber(replica_version_p, min_version_p);
    }
    else
    {
        get_directory_version(min_version_p);
    }
        
    /* Record if replica pointer has changed */
    if (dnsCmpCTS((dns_cts *)replica_set_p->ds_ts, 
		  (dns_cts *)allupto_ts_p) >=0 )
        *replica_changed_p = TRUE;
    else
	*replica_changed_p = FALSE;

    max_replicas = NUM_DBSet(replica_set_p);

    if (max_replicas == 1)
        {
        /*
         * the only member in the replica set is ourselves. 
         * no use in going thru the rest of this routine,
         * all it will do is allocate some memory and then de-allocate it.
         */
        return (DNS_SUCCESS);
        }
                      
    /*
     * Allocate our context buffer out of the buffer farm.
     * Since a part of our buffer is dependent on the number of replicas,
     * we want to avoid fragmentation of the farm lookaside list.  So
     * we'll allocate memory based on increments of 100 replicas.
     *
     * There is one spread_element_t in spread_list_t.
     */

    alloc_chunk = ((max_replicas + 100 - 1) / 100) * 100; /* Round up by 100s */

    update_buf_p = (update_buffer_t *)dns_get_buffer(update_buffer_bt,
         (10 * UPDATEPACKETMAX + READ_BUF_SIZE + sizeof(ReplicaPointer_u)
	  + sizeof(spread_list_t) + (alloc_chunk - 1)*sizeof(spread_element_t)));

    if (!update_buf_p) 
	{
	skulk_log(hash_p, cds_s_server_spreadphase, cds_s_server_failcontext,
		  NULL, DNS_NONSRESOURCES);
	return(DNS_NONSRESOURCES);
	}

    spread_list_p = (spread_list_t *)
	SKIP_bytes(update_buf_p, 10*UPDATEPACKETMAX);

    buff_p = SKIP_bytes(spread_list_p, sizeof(spread_list_t) +
			(max_replicas-1)*sizeof(spread_element_t));

    returned_replica_p = (ReplicaPointer_t *)SKIP_bytes(buff_p, READ_BUF_SIZE);

    update_buf_p->ub_total = (UPDATEPACKETMAX*10) - sizeof(update_buffer_t);
    spread_list_p->spread_count = 0;
    
    /* Set up static context variables */
    context_p = (copy_context_t *)dns_malloc(sizeof(copy_context_t));

    context_p->ct_buffers_sent = 0;
    context_p->ct_epoch_p = epoch_p;
    context_p->ct_spread_count = 0;

   /*
    * Loop through replica set sending updates.  Do 5 replicas
    * at a time.  Everytime a new or dying replica is encountered,
    * process it.
    */
    status = DNS_SUCCESS;  			/* Changes for fatal errors */
    /*
     * the element array follows the spread_list in the buffer space that
     * was allocated all together above in update_buf_p, 
     * skip over spread_list to get at the first element
     */ 
    element_p = spread_list_p->spread_first;
    for (index = 0; index < max_replicas; ++index)
        {
	value_p = (DBSetMember_t *)INDEX_DBSet(replica_set_p, index);
	if (!EXT8(value_p->dm_value_present) || !EXT8(value_p->dm_valid))
	    continue;
	element_p->spread_hint_p = (ReplicaPointer_t *)value_p->dm_data;

	/* Skip self */
        if (EQ_ObjUID(element_p->spread_hint_p->rp_CHID, &ch_p->ch_uid))
	    continue;

	replica_parse(element_p->spread_hint_p, &element_p->spread_hint_len,
		NULL, NULL, NULL, &workingon_chname_p);

        /*                                      
         * call the clerk to find out what replica state is
         */ 
	element_p->spread_status = 
		clerk_read_replica((ObjUID_t *)stream_p->db_dir.data_uid, 
		(FullName_t *)stream_p->db_dir_hash_p->dir_name,
		dns_AttributeName(DNS_REPLICASTATE),
		(byte_t *)buff_p, 
		element_p->spread_hint_p, element_p->spread_hint_len,
		returned_replica_p);

	/* Keep trying other replicas so information gets out there */
	if (element_p->spread_status != DNS_SUCCESS)
	    {
	    skulk_log(hash_p, cds_s_server_spreadphase, 
		      cds_s_server_failreadrs,
		      deb_ascii_FullName(namebuf, workingon_chname_p),
		      element_p->spread_status);
	    ++spread_list_p->spread_count;
	    ++element_p;
	    continue;
	    }
            

        /*
         * Minimize the ReplicaVersion of all replicas by reading the
         * replica version from each replica.  If we cannot reach a
         * replica, then we set the minimized ReplicaVersion to 0.0
         * (this is essentially a "floor" value: below the
         * architecture domain of valid version numbers).  This will
         * cause the upgrade to not occur (since the upgrade will not
         * work unless MIN(ReplicaVersion) >= UpgradeTo.
         *
         * We optimize the lookups by not checking other replicas, if we
         * fail to obtain the replica version from just one of them.
         * (version_status is initialized to DNS_SUCCESS for this reason).
         */

        if (version_status == DNS_SUCCESS)
            {
            /*                                      
             * While we're here, find out what replica version is too
             */ 
	    version_status = 
		clerk_read_replica((ObjUID_t *)stream_p->db_dir.data_uid, 
  		    (FullName_t *)stream_p->db_dir_hash_p->dir_name,
		    dns_AttributeName(DNS_REPLICAVERSION),
		    (byte_t *)context_p->ct_rv_buf,
		    element_p->spread_hint_p, element_p->spread_hint_len,
		    NULL);

            if (version_status == DNS_SUCCESS)
                {
                VersionNumber_t *this_replica_version_p;
  	        /* Retrieve returned replica version */
                member_p = (MemberValue_t *)
			    SKIP_AttributeType(context_p->ct_rv_buf);
                this_replica_version_p = (VersionNumber_t *)member_p->mv_value;
                if (LT_VersionNumber(this_replica_version_p, min_version_p))
                    {
                    COPY_VersionNumber(this_replica_version_p, min_version_p);
                    }
                }
            else
                {
                ZERO_VersionNumber(min_version_p);
		skulk_log(hash_p, cds_s_server_spreadphase, 
			  cds_s_server_failreadv,
			  deb_ascii_FullName(namebuf, workingon_chname_p),
			  version_status);
                }
            } /* End if found dns_relicaversion attribute */


        /* Fix hints if returned address doesn't match hint */
        if (!towerset_compare_contained(
		(Set_t *)returned_replica_p->rp_addressHint,
		(Set_t *)element_p->spread_hint_p->rp_addressHint))
            fix_replica_ptr(stream_p,
		(ObjUID_t *)element_p->spread_hint_p->rp_CHID, 
		(Set_t *)returned_replica_p->rp_addressHint);
                                    
	/* Retrieve returned replica state */
        member_p = (MemberValue_t *)SKIP_byte(buff_p);
        element_p->spread_replica_state = EXT8(member_p->mv_value);
            
        switch (element_p->spread_replica_state)
	{          
            case RS_newDir:                                
		/*
		 * Not allowed to happen, cleanup & return bug 
		 */
		evl_WrongState(ch_p, (FullName_t *)hash_p->dir_name,
				element_p->spread_replica_state);

		status = DNS_NAMESERVERBUG;
            	break;
	    case RS_dyingDir:
		/*	
		 * not allowed to happen, cleanup & return bug 
		 */ 
		evl_WrongState(ch_p, (FullName_t *)hash_p->dir_name,
				element_p->spread_replica_state);

		status = DNS_NAMESERVERBUG;
		break;
	    case RS_dead:     
		/* not allowed to happen, cleanup & return bug */            

		evl_WrongState(ch_p, (FullName_t *)hash_p->dir_name,
				element_p->spread_replica_state);

		status = DNS_NAMESERVERBUG;
            	break;
	    case RS_newReplica:
		*replica_changed_p = TRUE;
		context_p->ct_replica_p = element_p->spread_hint_p;
	        element_p->spread_status =
			spread_new_replica(stream_p,
			context_p, skulk_ts_p, update_buf_p);
		if (element_p->spread_status != DNS_SUCCESS)
		    {
		    skulk_log(hash_p, cds_s_server_spreadphase,
			cds_s_server_failnewreplica,
			deb_ascii_FullName(namebuf, workingon_chname_p),
			element_p->spread_status);
		    }
		break;
	    case RS_dyingReplica:
		*replica_changed_p = TRUE;
		/*      
		 * crucial replica is handled in remove replica
		 * keeps the root unless deleting clearinghouse
                 */
		element_p->spread_status =
		    spread_dead_replica(stream_p, 
			       replica_set_p, element_p->spread_hint_len, 
			       element_p->spread_hint_p);
		/* 
		 * If it worked remove it from the in memory set so
		 * it doesn't get used by another dying replica 
		 */
		if (element_p->spread_status == DNS_SUCCESS)
		    {
		      value_p = (DBSetMember_t *)SKIP_bytes(
					      element_p->spread_hint_p,
					       -sizeof(DBSetMember_overhead));
		      INS8(value_p->dm_valid, FALSE);
		    }
		else
		    {
		      skulk_log(hash_p, cds_s_server_spreadphase, 
			  cds_s_server_faildeadreplica,
			  deb_ascii_FullName(namebuf, workingon_chname_p),
			  element_p->spread_status);
		    }
		break;

	    case RS_on:
		/* Add to spread list */
	    	context_p->ct_spread_list[context_p->ct_spread_count] = 
								    element_p;
	    	if (++context_p->ct_spread_count >= MAX_SPREADS)	
		    {
		    /* Signal have a list */
		    context_p->ct_replica_p = NULL;
                    status = copy_updates(stream_p, allupto_ts_p, skulk_ts_p,
                    	update_buf_p, context_p);
		    context_p->ct_spread_count = 0;
		    }
   		break;
	    }  /* end of case */
        element_p++;
        spread_list_p->spread_count++;
        if (status != DNS_SUCCESS) break;	
        } /* Loop through each element */

    if (status == DNS_SUCCESS)
        if (context_p->ct_spread_count != 0)	
	    {
	    /* Signal have a list */
	    context_p->ct_replica_p = NULL;
            status = copy_updates(stream_p, allupto_ts_p, skulk_ts_p,
                        update_buf_p, context_p);
	    if (status != DNS_SUCCESS)
		{
		skulk_log(hash_p, cds_s_server_spreadphase,
			  cds_s_server_failcopyupd, 
			  deb_ascii_FullName(namebuf, workingon_chname_p),
			  status);
		}
	    }

    /* If still successfull, see if we talked to all replicas */
    if (status == DNS_SUCCESS)
	{
	element_p = spread_list_p->spread_first;
        for (cnt = 0; cnt < spread_list_p->spread_count; element_p++, cnt++)
	    if (element_p->spread_status != DNS_SUCCESS)
		{
		status = element_p->spread_status;
		if (status != DNS_SUCCESS)
		    {
		    replica_parse(element_p->spread_hint_p, NULL, 
				NULL, NULL, NULL, 
				&workingon_chname_p);
		    skulk_log(hash_p, cds_s_server_spreadphase,
			  cds_s_server_failcopyupd, 
			  deb_ascii_FullName(namebuf, workingon_chname_p),
			  status);
		    }
		break;
		}
	}

    dns_free_buffer((char *)update_buf_p);
    dns_free((char *)context_p);
    return(status);
}


