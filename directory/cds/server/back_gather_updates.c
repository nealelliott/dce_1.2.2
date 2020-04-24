/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: back_gather_updates.c,v $
 * Revision 1.1.6.2  1996/02/18  19:35:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:37  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:28:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:16  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:03:28  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:18:20  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:42:46  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:01  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:09:47  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:37:37  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:18:48  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:18:57  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:24:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: back_gather_updates.c
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
 */

#include <server.h> 
#include <back.h>
#include <dbdef.h>
#include <find.h>
#include <uid.h>

#define MAX_PACKET_SIZE (UPDATEPACKETMAX - sizeof(VCRequestHeader_u)- sizeof(Timestamp_u))

typedef struct {
    UpdatePacket_t	*pkt_p;		/* Currently being built */
    Timestamp_t		*entryCTS_p;	/*  points to pkt_p->entryCTS */
    byte_t		*updateCount_p;	/*  points to pkt_p->updateCount */
    Update_t		*updateset_p;	/*  points to pkt_p->updateset */
    int			att_index;	/* index of next attribute */
    int			val_index;	/* index of next attribute value */
    byte_t		updateCount;	/* Number of updates in pkt_p */
} up_context_t;

/*
 * Local prototypes
 */
static int
up_header (
    up_context_t	*,
    db_stream_t		*,
    db_data_t		*);

static void
up_delete (
    up_context_t	*,
    byte_t		**,
    byte_t		**,
    update_buffer_t	*,
    Timestamp_t		*);

static dns_status_t
bld_allupto (
    db_data_t		*,
    byte_t		**,
    Update_t		*,
    Timestamp_t		*);

static short
find_update (
    up_context_t	*,
    db_data_t		*,
    Timestamp_t		*,
    Timestamp_t		*);

static void
add_update (
    up_context_t	*,
    Update_t		*,
    db_data_t		*);

/*
 * up_header
 *	This function builds the packet header
 *
 * Input:
 *	up_p = do_updates context block
 *	stream_p = pointer to stream descriptor
 *	rec_p = pointer to data descriptor
 *	new_upd_p = pointer to store header
 *
 * Value:
 *	size of header
 */
static int 
up_header (up_context_t  *up_p,
           db_stream_t   *stream_p,
           db_data_t     *rec_p)
{
    FullName_t		*entryName_p = (FullName_t *)up_p->pkt_p->up_entryName;
    register byte_t	*next_p;

    /* Build directory id */
    if (rec_p->data_type == ET_directory)
	{
	COPY_FullName(NullFullName, entryName_p);
	COPY_ObjUID(rec_p->data_uid, entryName_p->fn_root);
	}
    else
	{
	COPY_FullName(stream_p->db_key, entryName_p);
	db_entry_name_cased(stream_p, (SimpleName_t *)entryName_p->fn_name);	    
	}
    next_p = SKIP_FullName(entryName_p);
    up_p->entryCTS_p = (Timestamp_t *)next_p;

    /* Fill in the static fields in the update packet */
       
    COPY_Timestamp(rec_p->data_cts, next_p);
    next_p = SKIP_Timestamp(next_p);

    PUT8(next_p, rec_p->data_type);

    up_p->updateCount_p = next_p;
    up_p->updateCount = 0;
    PUT8(next_p, up_p->updateCount);

    up_p->updateset_p = (Update_t *)next_p;

    return DIFF_bytes(next_p, up_p->pkt_p);
}

/*
 * up_delete
 *	This routine builds a delete entry and update uts message.
 *
 * Input:
 *	up_p = do_updates context block
 *      current_upd_pp   a pointer to top of update packet buffer 
 *      current_size_pp  a pointer to a place to return the size actually used
 *      update_buf_p     address of beginning of packets of packets
 *	ts_p 		 address of delete timestamp
 *                   
 */                          
static void 
up_delete (up_context_t     *up_p,
           byte_t           **current_upd_pp,
           byte_t           **current_size_pp,
           update_buffer_t  *update_buf_p,
           Timestamp_t      *ts_p)
{                                    
    Update_t		*current_p = up_p->updateset_p;

    /* Build delete update */
    (void)update_delete_build (
            current_p,		/* place to build it */
            ts_p);		/* timestamp of the update */

    ++up_p->updateCount;
    INS8(up_p->updateCount_p, up_p->updateCount);
    current_p = (Update_t *)SKIP_Update(current_p);

    /* Build UTS update */
    (void)update_dns_build (
            current_p,			/* place to build it */
            UD_present,			/* operation */
            ts_p,			/* timestamp of the update */
	    dns_Attribute(DNS_UTS),     /* attribute descriptor */
	    BL_true,
            sizeof(*ts_p),		/* value length */
            (byte_t *)ts_p);		/* pointer to the value */

    ++up_p->updateCount;
    INS8(up_p->updateCount_p, up_p->updateCount);
    current_p = (Update_t *)SKIP_Update(current_p);

    INS32(*current_size_pp, EXT32(*current_size_pp) +
		 DIFF_bytes(current_p, up_p->updateset_p));

    ++update_buf_p->ub_count;
    update_buf_p->ub_remaining -= sizeof(longword_u) + EXT32(*current_size_pp);
    *current_size_pp = SKIP_bytes(*current_upd_pp, EXT32(*current_size_pp));
    *current_upd_pp = SKIP_longword(*current_size_pp);
}

           
/*
 * bld_allupto
 *	This function creates an update for the allupto attribute 
 *                    
 * Inputs:
 *	rec_p = address of data descriptor
 *	current_size_pp= pointer to pointer to return size
 *	update_p = pointer to place to put update in
 *	begin_ts_p =lowest possible ts allowed
 *     
 */
static dns_status_t 
bld_allupto (db_data_t    *rec_p,
             byte_t       **current_size_pp,
             Update_t     *update_p,
             Timestamp_t  *begin_ts_p)
{
    DBSet_t		*set_p;
    DBSetMember_t	*value_p;
    int			index;
    dns_status_t	status;
               
    /* Find allupto, any errors have already been handled */
    status = find_attribute(rec_p->data_p,
		dns_AttributeName(DNS_ALLUPTO), &set_p, &index);
          
    if (status != DNS_SUCCESS)	return(DNS_NOUPDATES);

    /*
     * DNS_ALLUPTO is always a single valued set with no history
     */
    value_p = (DBSetMember_t *)DATA_DBSet(set_p);

    if (dnsCmpCTS((dns_cts *)begin_ts_p, (dns_cts *)value_p->dm_data) > 0)
	return(DNS_NOUPDATES);
    
    (void)update_dns_build(update_p, UD_present,
		(Timestamp_t *)set_p->ds_ts, dns_Attribute(DNS_ALLUPTO),
		BL_true, sizeof(Timestamp_u), value_p->dm_data);

    INS32(*current_size_pp, EXT32(*current_size_pp) + LEN_Update(update_p));

    return(DNS_SUCCESS);
}                                             

           
/*      
 * find_update 
 *	This function finds the next update in a database record
 *	it will not find per-replica attributes or DNS$AllUpTo
 *
 * Inputs:
 *	up_p = do_updates context block
 *      rec_p = pointer to data descriptor
 *      begin_ts_p = pointer to the ts to begin the updates
 *      end_ts_p = pointer to the ts to end the updates
 *
 * Value:
 *      update length
 */                          
static short 
find_update (up_context_t  *up_p,
             db_data_t     *rec_p,
             Timestamp_t   *begin_ts_p,
             Timestamp_t   *end_ts_p)
{                                                                 
    DBSet_t		*main_p;
    
    main_p = (DBSet_t *)rec_p->data_p->dbr_data;
    ALIGN_CHECK(main_p);

    while (up_p->att_index < NUM_DBSet(main_p))
	{
	AttributeName_t *name_p = (AttributeName_t *)
			INDEX_DBSet(main_p, up_p->att_index);
	DBSet_t *set_p = (DBSet_t *)SKIP_AttributeName(name_p);
	attribute_descriptor_t *dns_p = find_known_attribute(name_p);

	ALIGN_CHECK(set_p);

	/* See if this is a DNS attribute */
	if (dns_p)
	    if (((rec_p->data_type == ET_directory) && 
		    (dns_p->rep_rec || (dns_p->atype == DNS_ALLUPTO))) ||
		    (dns_p->atype == DNS_CTS))
		goto next_set;

	if (up_p->val_index == 0)
	    {
	    if (dnsCmpCTS((dns_cts *)set_p->ds_ts, (dns_cts *)begin_ts_p) < 0)
		goto next_set;
	    }
	else if (NUM_DBSet(set_p) <= up_p->val_index)
	    goto next_set;

	/* Account for empty sets */
	if (NUM_DBSet(set_p) == 0)
	    {
	    if (dnsCmpCTS((dns_cts *)set_p->ds_ts, (dns_cts *)end_ts_p) > 0)
		goto next_set;

	    return(sizeof(Update_u) - sizeof(AtomicValue_u)
		- sizeof(AttributeName_u) + LEN_AttributeName(name_p));
	    }

	/*
	 * find a value within the set with a timestamp greater than or equal
	 * to the beginning timestamp and less than or equal to the ending
	 * timestamp
	 */                                
	for (; up_p->val_index < NUM_DBSet(set_p); up_p->val_index++)
	    {
	    DBSetMember_t *value_p = (DBSetMember_t *)
			INDEX_DBSet(set_p, up_p->val_index);

	    if (dnsCmpCTS((dns_cts *)value_p->dm_ts, (dns_cts *)begin_ts_p)<0)
		continue;
	    if (dnsCmpCTS((dns_cts *)value_p->dm_ts, (dns_cts *)end_ts_p)>0)
		continue;

	    if (EXT8(value_p->dm_value_present))
		return(sizeof(Update_u) - MAXATTRIBUTE
		    - sizeof(AttributeName_u) + LEN_AttributeName(name_p)
		    + LEN_DBSetMemberData(value_p));
	    else
		return(sizeof(Update_u) - sizeof(AtomicValue_u)
		    - sizeof(AttributeName_u) + LEN_AttributeName(name_p));
	    } 
next_set:
	up_p->val_index = 0;
	up_p->att_index++;
	}

    return(0);
}

/* 
 * add_update 
 *	This routine determines the paremeters for update_build
 *
 * Input:
 *	up_p = do_updates context block
 *	update_p = pointer to update buffer
 *	rec_p = pointer to data descriptor
 */
static void 
add_update (up_context_t  *up_p,
            Update_t      *update_p,
            db_data_t     *rec_p)
{
    DBSet_t		*main_p, *set_p;
    SimpleName_t	*name_p;
    DBSetMember_t	*val_p;

    main_p = (DBSet_t *)rec_p->data_p->dbr_data;
    ALIGN_CHECK(main_p);
    name_p = (SimpleName_t *)INDEX_DBSet(main_p, up_p->att_index);
    set_p = (DBSet_t *)SKIP_AttributeName(name_p);
    ALIGN_CHECK(set_p);


    if ( (up_p->val_index == 0) && (NUM_DBSet(set_p) == 0) ) {
        (void)update_build(update_p, EXT8(set_p->ds_flag),
		(Timestamp_t *)set_p->ds_ts, EXT8(set_p->ds_type),
		(AttributeName_t *)name_p, (unsigned int *)set_p->ds_casemask,
		BL_false, 0, VT_none, NULL);
    } else {

        val_p = (DBSetMember_t *)INDEX_DBSet(set_p, up_p->val_index);
    
        if (EXT8(val_p->dm_value_present)) {
	    (void)update_build(update_p, EXT8(val_p->dm_flag),
	    	    (Timestamp_t *)val_p->dm_ts, EXT8(val_p->dm_type),
	    	    (AttributeName_t *)name_p, 
                    (unsigned int *)set_p->ds_casemask,
		    BL_true, LEN_DBSetMemberData(val_p), 
		    EXT8(val_p->dm_valuetype),
		    val_p->dm_data);
        } else {
	    (void)update_build(update_p, EXT8(val_p->dm_flag),
		    (Timestamp_t *)val_p->dm_ts, EXT8(val_p->dm_type),
		    (AttributeName_t *)name_p, 
                    (unsigned int *)set_p->ds_casemask,
		    BL_false, 0, VT_none, NULL);
        }
    }
}

           
/*
 * gather_updates 
 *	This function creates one or more update packets for an entry.
 *                                        
 * Inputs:                       
 *	stream_p     pointer to stream descriptor 
 *      rec_p        a pointer to the data descriptor 
 *      begin_ts_p  a pointer to a time. All updates after this time and
 *                   before end_ts will be included in the update packet
 *                   messages.
 *      end_ts_p    a pointer to the end of the time for the update.
 *      current_upd_pp   a pointer to pointer of the update packet buffer 
 *      current_size_pp
 *                   a pointer to a pointer to return the size of the packet
 *      update_buf_p Address of beginning of packets of packets
 *      context_p    pointer to a context structure that is used to
 *                   determine whare to start gathering updates from the entry.
 *                   
 * Outputs:                                                         
 *      creates one or more update messages and saves the context
 *      (when available) so can continue later from the ending place
 *      context_p    Updated for next call
 *
 */                          
dns_status_t 
gather_updates (db_stream_t      *stream_p,
                db_data_t        *rec_p,
                Timestamp_t      *begin_ts_p,
                Timestamp_t      *end_ts_p,
                byte_t           **current_upd_pp,
                byte_t           **current_size_pp,
                update_buffer_t  *update_buf_p,
                copy_context_t   *context_p)
{                                    
    up_context_t	up;
    Timestamp_u		entry_uts;      
    Update_t		*current_p;
    DBSet_t		*set_p, *main_set_p;
    int			cnt, packet_size, static_hdr_size;
    short		update_size;
    char		done;
    dns_status_t	status;
    DEB_ASCII_BUF_Timestamp(begin_ts_buf)
    DEB_ASCII_BUF_Timestamp(end_ts_buf)
    int			temp;
                             
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >gather_updates(%x, %x, %s, %s, ...) - (remaining: %d)",
        stream_p,
        rec_p,
        deb_ascii_Timestamp(begin_ts_buf,begin_ts_p),
        deb_ascii_Timestamp(end_ts_buf,end_ts_p),
        update_buf_p->ub_remaining));

    main_set_p = (DBSet_t *)rec_p->data_p->dbr_data;
    ALIGN_CHECK(main_set_p);

    /* Make sure header and delete or allupto will fit */
    if (update_buf_p->ub_remaining < 1000)
	{
	if (!context_p) return(DNS_NONSRESOURCES);
	status = send_dir(update_buf_p, context_p);
	if (status != DNS_SUCCESS) return(status);
	update_buf_p->ub_remaining = update_buf_p->ub_total;
	update_buf_p->ub_count = 0;
        *current_size_pp = SKIP_bytes(update_buf_p, sizeof(update_buffer_t));
	*current_upd_pp = SKIP_longword(*current_size_pp);
	}

    up.pkt_p = (UpdatePacket_t *)*current_upd_pp;

    /* Build packet header */
    static_hdr_size = up_header(&up, stream_p, rec_p);
    INS32(*current_size_pp, static_hdr_size);
    current_p = up.updateset_p;
	
    /* Check for deleted records of the same name */
    if (0 < (cnt = NUM_DBSet_Delete(main_set_p)))
	{
	delete_holder_t *delete_p =
	    (delete_holder_t *)DATA_DBSet_Delete(main_set_p);

	do
	    {
	    if ((dnsCmpCTS((dns_cts *)delete_p->del_ts, 
			  (dns_cts *)begin_ts_p) >=0) &&
	    	(dnsCmpCTS((dns_cts *)delete_p->del_ts, 
			  (dns_cts *)end_ts_p) <=0))
		{
		COPY_Timestamp(delete_p->del_uid, up.entryCTS_p);
		/* A delete will always fit */
		up_delete(&up, current_upd_pp, current_size_pp,
			     update_buf_p, (Timestamp_t *)delete_p->del_ts);

		/* Make sure header and delete or allupto will fit */
		if (update_buf_p->ub_remaining < 1000)
		    {
		    if (!context_p) return(DNS_NONSRESOURCES);
	  	    status = send_dir(update_buf_p, context_p);
	   	    if (status != DNS_SUCCESS) return(status);
		    update_buf_p->ub_remaining = update_buf_p->ub_total;
		    update_buf_p->ub_count = 0;
        	    *current_size_pp = SKIP_bytes(update_buf_p,
						sizeof(update_buffer_t));
		    *current_upd_pp = SKIP_longword(*current_size_pp);
		    }

		up.pkt_p = (UpdatePacket_t *)*current_upd_pp;

		/* Build packet header */
		static_hdr_size = up_header(&up, stream_p, rec_p);
		INS32(*current_size_pp, static_hdr_size);
		current_p = up.updateset_p;
		}
	    }
	while (delete_p++, --cnt);
	}


    /* If this is a directory, get allupto */
    if (rec_p->data_type == ET_directory)
	{
        if (bld_allupto(rec_p, current_size_pp,
		current_p, begin_ts_p) == DNS_NOUPDATES)
	    {
	    /* Won't be any delete on a directory, since they are not
	     * accumlated the same way.
	     * See if any changes to send  
	     */
	    if (find_attribute(rec_p->data_p, dns_AttributeName(DNS_UTS),
			&set_p, &up.att_index) == DNS_SUCCESS)
		if (dnsCmpCTS((dns_cts *)set_p->ds_ts, 
			     (dns_cts *)begin_ts_p) < 0)
		    return(DNS_NOUPDATES);
	    }
	else
	    {
	    up.updateCount = 1;
	    INS8(up.updateCount_p, up.updateCount);
	    current_p = (Update_t *)
		SKIP_bytes(*current_upd_pp, EXT32(*current_size_pp));
	    }
	}
    else 
        /* See if any changes to send  */
	if (find_attribute(rec_p->data_p, dns_AttributeName(DNS_UTS),
		&set_p, &up.att_index) == DNS_SUCCESS)
	    if (dnsCmpCTS((dns_cts *)set_p->ds_ts, 
			 (dns_cts *)begin_ts_p) < 0)
		return(DNS_NOUPDATES);

    /* If record is marked absent, special case since there is not attribute */
    if (EXT8(main_set_p->ds_flag) != DS_present)
        {
	/* Make sure delete is in this skulk time */
	if ((dnsCmpCTS((dns_cts *)main_set_p->ds_ts, 
		      (dns_cts *)begin_ts_p) >= 0) &&
	    (dnsCmpCTS((dns_cts *)main_set_p->ds_ts, 
		      (dns_cts *)end_ts_p) <=0))
	    {
	    /* A delete will always fit */
 	    up_delete(&up, current_upd_pp, current_size_pp,
			     update_buf_p, (Timestamp_t *)main_set_p->ds_ts);
	    return(DNS_SUCCESS);
	    }
	}
    
    done = FALSE;
    up.att_index = 0;
    up.val_index = 0;               
    COPY_Timestamp(NullTimestamp, entry_uts);  
    
    if ((update_buf_p->ub_remaining - (int)2*sizeof(longword_u)) < MAX_PACKET_SIZE)
	packet_size = update_buf_p->ub_remaining - (int)sizeof(longword_u) - 
		(int)EXT32(*current_size_pp);
    else
        packet_size = MAX_PACKET_SIZE - (int)EXT32(*current_size_pp);

    /* Find updates and add them to the packet */                
    while (!done) 
        {
        update_size = find_update(&up, rec_p, begin_ts_p, end_ts_p);

        /* Although the UTS is an attribute, we don't maintain all of
	 * its values, only the latest.  This means we must calculate the
	 * UTS based on the other updates we are sending 
	 */
	if (update_size == 0)
            {
	    done = TRUE;
	    if (dnsCmpCTS((dns_cts *) begin_ts_p, (dns_cts *)entry_uts) > 0)
	        break;

	    update_size = sizeof(Update_u) - MAXATTRIBUTE + sizeof(Timestamp_u)
	    	- sizeof(AttributeName_u)
		+ LEN_AttributeName(dns_AttributeName(DNS_UTS));
            }
	temp = MAX_PACKET_SIZE - (2*sizeof(longword_u));
	dce_assert(cds__svc_handle,
		   (update_size < temp));
        /* Check for overflow */
	/*	packet_size represents the remaining space for this packet.  
	 *	Since it may be (and usually is) constrained by the remaining
	 *	buffer space, allow for the trailing longword of zeros that
	 *	terminates the buffer contents.
	 */
        if ( ( packet_size - sizeof(longword_u) ) < update_size )
            {   
            /* Close out this packet first */
	    if (0 < up.updateCount)
	        {
	        ++update_buf_p->ub_count;
	        update_buf_p->ub_remaining -= (EXT32(*current_size_pp) +
			sizeof(longword_u));
	        *current_size_pp = SKIP_bytes(*current_upd_pp,
			EXT32(*current_size_pp));
	        *current_upd_pp = SKIP_longword(*current_size_pp);
		}

	    /* Check for room for another packet */
            if ((update_buf_p->ub_remaining - (int)(static_hdr_size + 
				   2*sizeof(longword_u))) < update_size)
	      {
                if (context_p)
		  {                                                       
		    status = send_dir(update_buf_p, context_p);
		    if (status != DNS_SUCCESS) return(status);
		    update_buf_p->ub_remaining = update_buf_p->ub_total;
		    update_buf_p->ub_count = 0;
        	    *current_size_pp = SKIP_bytes(update_buf_p,
					sizeof(update_buffer_t));
		    *current_upd_pp = SKIP_longword(*current_size_pp);
                    }
                else                                                
                    /* Return what we have for propagates */
                    return(DNS_SUCCESS);
                }                    
                 
            if ((update_buf_p->ub_remaining - (int)2*sizeof(longword_u)) < MAX_PACKET_SIZE)
	      packet_size = update_buf_p->ub_remaining - 
                              (int)sizeof(longword_u) - static_hdr_size;
            else
	      packet_size = MAX_PACKET_SIZE - static_hdr_size;

            /*
             * copy the header portions of the data to the new buffer
             * and fix the current_p and the entryCTS_p
             */
	    up.pkt_p = (UpdatePacket_t *)*current_upd_pp;

	    /* Build packet header */
	    static_hdr_size = up_header(&up, stream_p, rec_p);
	    INS32(*current_size_pp, static_hdr_size);
	    current_p = up.updateset_p;
	  } 

	/* Add the update to the buffer */
	if (done)
	    /* This is the UTS update */
	    (void)update_dns_build(current_p, UD_present,
			(Timestamp_t *)entry_uts, dns_Attribute(DNS_UTS),
			BL_true, sizeof(Timestamp_u), entry_uts);
	else
	    {
	    add_update(&up, current_p, rec_p);
	    up.val_index++;
	    /* Recalculate the uts */                   
	    if (dnsCmpCTS((dns_cts *)current_p->ud_timestamp,
				(dns_cts *)entry_uts) > 0)
            	COPY_Timestamp(current_p->ud_timestamp, entry_uts);
	    }
            
        /* Update buffers */
	++up.updateCount;
	INS8(up.updateCount_p, up.updateCount);
        current_p = (Update_t *)SKIP_bytes(current_p, update_size);
        INS32(*current_size_pp, EXT32(*current_size_pp) + update_size);
        packet_size -= update_size;
        }  

    /* If we got out of the main loop the entire record has been processed */

    if ((update_buf_p->ub_count == 0) && (up.updateCount == 0))
	return (DNS_NOUPDATES); 

    if (up.updateCount != 0)
	{
	++update_buf_p->ub_count;
	update_buf_p->ub_remaining -= (EXT32(*current_size_pp) +
		sizeof(longword_u));
	*current_size_pp = SKIP_bytes(*current_upd_pp,
		EXT32(*current_size_pp));
	INS32(*current_size_pp, 0);
	*current_upd_pp = SKIP_longword(*current_size_pp);
	}
    return(DNS_SUCCESS);
}
