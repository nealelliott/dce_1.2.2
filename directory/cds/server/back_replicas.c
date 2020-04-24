/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: back_replicas.c,v $
 * Revision 1.1.8.2  1996/02/18  19:35:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:49  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:28:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:29  root]
 * 
 * Revision 1.1.6.3  1994/08/03  19:03:35  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:18:50  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:42:52  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:09  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:10:03  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:38:16  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:19:21  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:19:25  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/30  19:37:00  peckham
 * 	OT#2114: uninitialized variables.
 * 	[1992/06/30  19:36:35  peckham]
 * 
 * Revision 1.1  1992/01/19  15:24:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module back_replicas.c
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
#include <clerk.h>
#include <dbdef.h>
#include <find.h>
#include <replica.h>
#include <sets.h>
#include <tlog.h>
#include <uid.h>
#include <back.h>
#include <tower.h>


/*                                            
 * check_own_replica_ptr 
 *	This function checks the replica pointer of a directory
 * and repairs the replica pointer if the addresses don't match.
 *
 * Inputs:                                   
 *      stream_p     = pointer to stream descriptor 
 *      towerset_p   = pointer to the current address of the clearinghouse
 *	upd_p        = address of buffer to return update in
 *	len_p        = address to return update length
 *        
 * Value: 
 *	False if address the same
 *	True if my address has changed
 */                          
int 
check_own_replica_ptr (struct db_stream  *stream_p,
                       Set_t             *towerset_p,
                       Update_t          *upd_p,
                       int               *len_p)
{
    DBSet_t		*replica_set_p;
    int			index, max_replicas, match;
    dns_status_t	status;

    status = find_attribute(stream_p->db_dir.data_p,
        dns_AttributeName(DNS_REPLICAS), &replica_set_p, &index);
    if (status != DNS_SUCCESS) return(FALSE);

    /* Find this clearinghouse in set */
    max_replicas = NUM_DBSet(replica_set_p);
    for (index = 0; index < max_replicas; index++)
        {                       
	Timestamp_u ts;
	DBSetMember_t *value_p = (DBSetMember_t *)INDEX_DBSet(replica_set_p, index);
	ReplicaPointer_t *replica_p;
	Set_t *copy_set_p;
	AtomicValue_t *av_p;

        if (!EXT8(value_p->dm_value_present) || !EXT8(value_p->dm_valid))
	    continue;

        replica_p = (ReplicaPointer_t *)value_p->dm_data;

        /* Is this us? */
        if (NE_ObjUID(replica_p->rp_CHID,
			&stream_p->db_file_p->file_ch_p->ch_uid))
	    continue;

	/* Found this clearinghouse, has it moved? */
	/*
	 * must make a copy of one towerset, cause the compare
	 * exact routine is desctructive to one tower
	 */
	copy_set_p = (Set_t *)dns_malloc(LEN_Set(replica_p->rp_addressHint));
	if (!copy_set_p) return (FALSE); /*can't check now*/
	COPY_Set(replica_p->rp_addressHint, copy_set_p);
	match = towerset_compare_exact(towerset_p, copy_set_p);
	dns_free((char *)copy_set_p);
	if (match) return(FALSE);	/* We didn't move */	

	/* Mark current member absent */
	if (ts_new((Timestamp_t *)ts) != DNS_SUCCESS) return(FALSE);

	av_p = (AtomicValue_t *)SKIP_bytes(upd_p, 
		update_dns_build(upd_p, UD_present, (Timestamp_t *)ts,
		dns_Attribute(DNS_REPLICAS), BL_true, 0, NULL));
	INS16(av_p->av_length,
	    replica_build( (ReplicaPointer_t *)av_p->av_value,
		&stream_p->db_file_p->file_ch_p->ch_uid,
		towerset_p, db_dir_get_replica_type(stream_p->db_dir_hash_p),
		&stream_p->db_file_p->file_ch_p->ch_name));
	*len_p = LEN_Update(upd_p);
	(void)set_insert(&stream_p->db_dir, upd_p,
	    dns_Attribute(DNS_REPLICAS), TRUE);
	/* Update UTS */
	(void)set_timestamp_set(&stream_p->db_dir, (Timestamp_t *)ts, 
	    dns_Attribute(DNS_UTS), NULL, NULL);

	/* Update lastupdate */
	db_dir_set_lastupdate(stream_p->db_dir_hash_p, (Timestamp_t *)ts);
	return(TRUE);
        } /* end of for loop */

    return(FALSE);
}

           
/*                                            
 * fix_replica_ptr 
 *                 
 * This function repairs the replica pointer when an address change has
 * been discovered.  It assumes that the directory is not locked
 * and must be re-read, saving the current directory data.
 *
 * Inputs:                                   
 *      stream_p      pointer to the stream descriptor 
 *      ch_id_p      pointer to the id of the relevant clearinghouse  
 *	addr_p        pointer to the new address of the clearinghouse
 *        
 * Output:
 *	Directory is updated with new replica pointer.
 */
void 
fix_replica_ptr (struct db_stream  *stream_p,
                 ObjUID_t          *ch_id_p,
                 Set_t             *addr_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_ObjUID(ch_idBuf)
    bytes_u		buffer[sizeof(Update_u) - MAXATTRIBUTE +
				sizeof(ReplicaPointer_u)];
    DBSet_t		*replica_set_p;
    Set_t		*towerset_p = addr_p;
    ReplicaPointer_t	*replica_p;
    FullName_t		*chname_p;
    ReplicaType_u	replica_type;
    Timestamp_u		ts;
    struct db_data	dir_data;
    AtomicValue_t	*av_p;
    struct tlog_dsc	tlog;
    byte_t		*outbuf_p;
    byte_u		linked;
    int			i, index;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >fix_replica_ptr(%s,%s,)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_ObjUID(ch_idBuf,ch_id_p)));

     /*
      * read the clearinghouse object to get the real address
      * this should be in the cache, but it is possible this
      * may cause the thread to block.
      */

     /*
      * first find the name in the old dir record copy, cause we want it
      * to be unlocked when we call the clerk.
      */

    status = find_attribute(stream_p->db_dir.data_p,
        dns_AttributeName(DNS_REPLICAS), &replica_set_p, &index);
    if (status != DNS_SUCCESS) 
    {
	DEB_ASCII_BUF_AttributeName(attBuf)

	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug7,
          " <fix_replica_ptr(%s,%s,) no attribute(%s)",
          deb_ascii_db_stream(streamBuf,stream_p),
          deb_ascii_ObjUID(ch_idBuf,ch_id_p),
          deb_ascii_AttributeName(attBuf,dns_AttributeName(DNS_REPLICAS))));
	return;
    }

    /*
     * get some space to put the tower into
     */
    outbuf_p = (byte_t *)dns_malloc(READ_BUF_SIZE);
    if (!outbuf_p) 
    {
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug7,
          " <fix_replica_ptr(%s,%s,) malloc(%d) failed",
          deb_ascii_db_stream(streamBuf,stream_p),
          deb_ascii_ObjUID(ch_idBuf,ch_id_p),
          READ_BUF_SIZE));

	return;
    }

    /* Find clearinghouse name since we only have the id */
    for (i = 0; i < NUM_DBSet(replica_set_p); i++)
        {
        /* Get next replica pointer */      
	DBSetMember_t *const value_p =
			(DBSetMember_t *)INDEX_DBSet(replica_set_p, i);

        if (!EXT8(value_p->dm_value_present) || !EXT8(value_p->dm_valid))
	    continue;

        replica_p = (ReplicaPointer_t *)value_p->dm_data;

        /* Is this the one we want? */
        if (EQ_ObjUID(replica_p->rp_CHID, ch_id_p))
            {
	    Set_t *const outset_p = (Set_t *)SKIP_byte(outbuf_p);

	    /* prepare to call the clerk */
            replica_parse(replica_p, NULL, NULL, NULL,
		NULL, &chname_p);
	    status = clerk_read_attribute(chname_p, ET_object, 
			  dns_AttributeName(DNA_TOWERS), outbuf_p,
			  NULL, linked);

            /*
             * check for missing attribute... in this case use what the
             * clerk returned on the orginal call, it must be a v1
             * clearinghouse...
             *??? Should we log an error if moredata???  we ignore it
             * here.
             */
            if ((status != DNS_SUCCESS) && (status != DNS_MOREDATA))
	    {
		DEB_ASCII_BUF_FullName(chnameBuf)
		DEB_ASCII_BUF_AttributeName(attBuf)

                dns_free((char *)outbuf_p);

		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_server,
                    svc_c_debug7,
                    " <fix_replica_ptr(%s,%s,) %d=clerk_read_attribute(%s,ET_object,%s)",
                    deb_ascii_db_stream(streamBuf,stream_p),
                    deb_ascii_ObjUID(ch_idBuf,ch_id_p),
		    status,
                    deb_ascii_FullName(chnameBuf,chname_p),
                    deb_ascii_AttributeName(attBuf,dns_AttributeName(DNA_TOWERS))));
                return;
	    }

            if (FLAG_IS_SET(outset_p->st_flag, ST_present))
		towerset_p = outset_p;

	    break;
	}
    } /* end of for loop */


    /*
     * Save directory buffer before re-reading 
     */
    COPY_bytes(&stream_p->db_dir, &dir_data, sizeof(dir_data));

    status = db_dir_read_for_update(stream_p);
    if (status == DNS_SUCCESS)
    {
	status = find_attribute(stream_p->db_dir.data_p,
	    dns_AttributeName(DNS_REPLICAS), &replica_set_p, &index);
	if (status == DNS_SUCCESS)
	{
	    /* Find replica to update */
	    for (i = 0; i < NUM_DBSet(replica_set_p); i++)
	    {
		/* Get next replica pointer */      
		DBSetMember_t *const value_p =
				(DBSetMember_t *)INDEX_DBSet(replica_set_p, i);

		if (!EXT8(value_p->dm_value_present) ||
			!EXT8(value_p->dm_valid))
		    continue;

		replica_p = (ReplicaPointer_t *)value_p->dm_data;

		/* Is this the one we want? */
		if (NE_ObjUID(replica_p->rp_CHID, ch_id_p))
		    continue;

		/* Build an update to fix the address */
		if (ts_new((Timestamp_t *)ts) != DNS_SUCCESS) break;
		av_p = (AtomicValue_t *)SKIP_bytes(buffer,
			update_dns_build((Update_t *)buffer,
			    UD_present, (Timestamp_t *)ts, 
			    dns_Attribute(DNS_REPLICAS),
			    BL_true, 0, NULL));

		replica_parse(replica_p, NULL, NULL, NULL,
		    replica_type, &chname_p);
		INS16(av_p->av_length, replica_build(
			(ReplicaPointer_t *)av_p->av_value, 
			(ObjUID_t *)replica_p->rp_CHID,
			towerset_p, EXT8(replica_type), chname_p));
    
		(void)set_insert(&stream_p->db_dir, (Update_t *)buffer, 
		           dns_Attribute(DNS_REPLICAS), TRUE);

		/* Update UTS */
		(void)set_timestamp_set(&stream_p->db_dir, (Timestamp_t *)ts,
		    dns_Attribute(DNS_UTS), NULL, NULL);
		/* Update lastupdate */
		db_dir_set_lastupdate(stream_p->db_dir_hash_p, 
				      (Timestamp_t *)ts);
		tlog.cnt = 1;
		tlog.buf_p = buffer;
		tlog.len = LEN_Update(buffer);
		(void)db_dir_update(stream_p, &tlog);
		break;
		} /* end of for loop */
	    }    
        
	db_dir_release(stream_p);
	}
    dns_free((char *)outbuf_p);

    /* Restore old record buffer */
    COPY_bytes(&dir_data, &stream_p->db_dir, sizeof(struct db_data));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <fix_replica_ptr(%s,%s,)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_ObjUID(ch_idBuf,ch_id_p)));
}
