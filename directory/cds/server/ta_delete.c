/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta_delete.c,v $
 * Revision 1.1.8.2  1996/02/18  19:36:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:36  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:32:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:06  root]
 * 
 * Revision 1.1.6.3  1994/08/03  19:04:47  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:31:27  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:44:00  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:34  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:12:54  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:46:29  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:23:54  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:24:33  zeliff]
 * 
 * Revision 1.1.2.3  1992/06/04  19:06:41  mfox
 * 	Fix hang that occurs then background thread starts during directory deletion.
 * 	[1992/06/04  13:45:02  mfox]
 * 
 * Revision 1.1.2.2  1992/03/22  22:36:17  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:36:47  weisman]
 * 
 * Revision 1.1  1992/01/19  15:25:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module ta_delete.c
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
#include <sets.h>
#include <security.h>
#include <tlog.h>
#include <names.h>
#include <uid.h>
#include <back.h>
#include <find.h>

/*
 * Local prototypes
 */
static dns_status_t
empty (
    db_stream_t		*);

static dns_status_t
markabsent (
    ObjUID_t		*,
    db_stream_t		*);

/*
 * empty
 *     Determine if a directory is empty. Must do synchronous reads
 * cause a lock is held in this thread.
 *
 * Input:
 *     stream_p = address of stream
 *
 * Output:
 *     DNS_NOTEMPTY
 *     DNS_SUCCESS
 */
static dns_status_t 
empty (db_stream_t *stream_p)
{
    DBSet_t		*set_p;
    dns_status_t	status;

    stream_p->db_entry.data_type = ET_object;
    for (;;)
        {
	names_new((ObjUID_t *)stream_p->db_dir.data_uid, NULL, (FullName_t *)stream_p->db_key);
        status = db_entry_read_next(stream_p);
        while (status == DNS_SUCCESS)
            {
	    set_p = (DBSet_t *)stream_p->db_entry.data_p->dbr_data;            
            if (EXT8(set_p->ds_flag) == DS_present)
                {
		dns_free((char *)stream_p->db_entry.data_p);
                stream_p->db_entry.data_p = NULL;
                return(DNS_NOTEMPTY);
                }
	    dns_free((char *)stream_p->db_entry.data_p);
	    stream_p->db_entry.data_p = NULL;
            status = db_entry_read_next(stream_p);
            }
        if (status != DNS_UNKNOWNENTRY)
            return(status);
        if (stream_p->db_entry.data_type == ET_object)
            stream_p->db_entry.data_type = ET_softlink;
        else if (stream_p->db_entry.data_type == ET_softlink)
            stream_p->db_entry.data_type = ET_childPointer;
        else return(DNS_SUCCESS);
        }
}     

/*
 * markabsent
 *     This routine marks an entry absent, updates DNS$UTS, writes
 * out the entry and updates dns$lastupdate of the parent and writes
 * that out.
 *
 * Input:
 *     chid_p = address of clearinghouse id
 *     stream_p = address of stream descriptor
 *
 * Value:
 *     status = result of write
 *
 */
static dns_status_t 
markabsent (ObjUID_t     *chid_p,
            db_stream_t  *stream_p)
{
    tlog_dsc_t		tlog;
    bytes_u		upd_buf[sizeof(Update_u) - MAXATTRIBUTE + 1];
    Update_t		*upd_p;
    DBSet_t		*set_p;
    dns_status_t	status;
    
    set_p = (DBSet_t *)stream_p->db_entry.data_p->dbr_data;
    INS8(set_p->ds_flag, DSX_absent);

    /* This will be the deletion timestamp */
    status = ts_new((Timestamp_t *)set_p->ds_ts);

    if (status != DNS_SUCCESS) return(status);
    set_timestamp_set(&stream_p->db_entry, (Timestamp_t *)set_p->ds_ts,
	    		dns_Attribute(DNS_UTS), NULL, NULL);
    upd_p = (Update_t *)upd_buf;
    INS8(upd_p->ud_operation, UD_absent);
    COPY_Timestamp(set_p->ds_ts, upd_p->ud_timestamp);
    INS8(upd_p->ud_attribute, AT_null);
    INS8(SKIP_AttributeSpecifier(upd_p->ud_attribute), BL_false);
    tlog.cnt = 1;
    tlog.buf_p = (byte_t *)upd_p;
    tlog.len = LEN_Update(upd_p);
    status = db_entry_update(stream_p, &tlog);
    if (status == DNS_SUCCESS)
	{
	/* 
	 * The directory is not locked.  The update to lastupdate
	 * is made into the hash table.  Not locking the directory
	 * allows us to process multiple updates/directory
	 */
	db_dir_set_lastupdate(stream_p->db_dir_hash_p,
		(Timestamp_t *)set_p->ds_ts);
	if (!low_convergence(stream_p->db_dir.data_p))
	    back_propagate_sched(stream_p, stream_p->db_entry.data_type, 
		 	(Timestamp_t *)set_p->ds_ts);
	}
    return(status);
}

/*
 * ta_delete
 *   This routine tries to find an entry.  If the entry is found
 * it marks it as absent.   Background processing will actually
 * remove it from the database.
 * The progress record is updated.
 *
 *
 * Input:
 *      user_p = address of user description
 *      progress_p = address of progress record
 *      type = entry type
 *
 * Output:
 *
 * Function value:
 *      status
 *
 * Side effects: 
 *      Progress record updated
 */
dns_status_t 
ta_delete (user_descriptor_t  *user_p,
           Progress_t         *progress_p,
           int                type)
{
    db_stream_t		*stream_p;
    dns_status_t	status;

    status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_ModifyAttribute);

    if (status != DNS_SUCCESS) return(status);

    status = walktree(user_p, type, WALKTREE_DELETE_OP, progress_p, stream_p);
    if (status != DNS_SUCCESS) return(status);
    
    status = security_check(user_p, stream_p, Delete_Entry_check, type,
			    NULL);

    if (status == DNS_SUCCESS) {
	/* Get directory for convergence check */
	if (!stream_p->db_dir.data_p)
	  status = db_dir_read(stream_p);
	if (status == DNS_SUCCESS)
	    status = markabsent(user_p->ud_chuid_p, stream_p);

    } else if (type == ET_childPointer) {
	/* This is an nsclerk call so caller is server itself */
        status = DNS_UNTRUSTEDCH;
        security_log(user_p,
		(FullName_t *)SKIP_FullName(progress_p->pr_unresolvedName),
		stream_p->db_file_p->file_ch_p);
    } else if (status == DNS_UNKNOWNENTRY)  
      /* Rebuild resolved name for exception */
      resolved_null(progress_p);
    
    db_close_stream(stream_p);
    return(status);
}

/*
 * ta_delete_dir
 *   This routine tries find a directory.  If the directory is found
 * it marks it as a dying directory. Background processing will actually
 * remove it from the database.
 * The progress record is updated.
 *
 *
 * Input:
 *      user_p = address of user description
 *      progress_p = address of progress record
 *
 * Output:
 *
 * Function value:
 *      status
 *
 * Side effects: 
 *      Progress record updated
 */
dns_status_t 
ta_delete_dir (user_descriptor_t  *user_p,
               Progress_t         *progress_p)
{
    bytes_u		updbuf[sizeof(Update_u) - MAXATTRIBUTE + 1];
    tlog_dsc_t		tlog;
    db_stream_t		*stream_p;
    db_stream_t		*ch_stream_p;
    DBSet_t		*set_p;
    DBSetMember_t	*value_p;
    int			index, cnt;
    dns_status_t	status;
    
    status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_ModifyAttribute);
    if (status != DNS_SUCCESS)
        return(status);

    status = walktree(user_p, ET_directory, WALKTREE_DELETE_OP, progress_p, stream_p);
    if (status != DNS_SUCCESS) return(status);

    status = security_check(user_p, stream_p, Delete_Dir_check, ET_directory,
			    NULL); 
    if (status == DNS_SUCCESS)
        {
        /*
         * We're probing the clearinghouse for information, so we open the
         * stream for READ access, which bypasses deadlock problems if a
         * checkpoint was initiated since the open_stream above.
         */
        status = db_open_stream(user_p->ud_chuid_p, &ch_stream_p, OP_ReadAttribute);
        if (status != DNS_SUCCESS)
            {
            db_close_stream(stream_p);
            return(status);
            }

        /* Check write access to clearinghouse */
        status = db_pseudo_read(ch_stream_p);
        if (status == DNS_SUCCESS)
            status = security_check(user_p, ch_stream_p, Delete_Dir_check,
				    ET_clearinghouse, NULL);
	db_close_stream(ch_stream_p);

        if (status == DNS_SUCCESS)
            /* make sure the directory is empty */
	    {
            status = empty(stream_p);
	    if (status == DNS_SUCCESS)
		{
        	if (find_attribute(stream_p->db_dir.data_p, 
			dns_AttributeName(DNS_REPLICAS),
			&set_p, &index) == DNS_SUCCESS)
            	    {
		    cnt = 0;
                    for (index = 0; index < NUM_DBSet(set_p); index++)
                	{
                	value_p = (DBSetMember_t *)INDEX_DBSet(set_p, index);
                	if (EXT8(value_p->dm_value_present) &&
				EXT8(value_p->dm_valid))
			    cnt++;
                 	}
            	    if (cnt > 1)
                	status = DNS_MORETHANONEREPLICA;
                    else
			{
			tlog.cnt = 1;
			tlog.buf_p = updbuf;
			db_dir_set_state(stream_p, RS_dyingDir,
				(Update_t *)updbuf, &tlog.len);
			status = db_dir_update(stream_p, &tlog);
		        /* Update database */
		        if (status == DNS_SUCCESS)
			    skulk_sched(stream_p);
			}
		    }
		} /* End directory empty */	
	    } /* End access to clearinghouse */
	} /* End access to directory */
    else if (status == DNS_UNKNOWNENTRY)
    /* Rebuild resolved name for exception */
        resolved_null(progress_p);

    db_close_stream(stream_p);
    return(status);
}

/*
 * ta_delete_replica 
 *    Remove a replica as long as it is not considered crucial.
 *
 * Input:
 *    user_p = address of user descriptor
 *    dir_p = address of directory full name
 */
dns_status_t 
ta_delete_replica (user_descriptor_t  *user_p,
                   FullName_t         *dir_p)
{
    byte_t		*retset_p;
    db_stream_t		*stream_p;
    dns_status_t	status;

    /* Output cannot be on the stack */
    retset_p = (byte_t *)dns_malloc(READ_BUF_SIZE);
    if (!retset_p) return(DNS_NONSRESOURCES);

    status = clerk_read_attribute_conf(dir_p, ET_directory,
		dns_AttributeName(DNS_OBJECTUID), retset_p,
		NULL, NULL, dnsConfMed);
    if ((status == DNS_SUCCESS) || (status == DNS_MOREDATA))
	{
	status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_ModifyAttribute);
	if (status == DNS_SUCCESS)
	    {
	    MemberValue_t *mv_p = (MemberValue_t *) SKIP_byte(retset_p);

	    status = ta_replica_remove(user_p, (ObjUID_t *)mv_p->mv_value,
		FALSE, stream_p);
	    db_close_stream(stream_p);
	    }
	}

    dns_free((char *)retset_p);
    return(status);
}
