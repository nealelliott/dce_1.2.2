/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_pseudo.c,v $
 * Revision 1.1.8.2  1996/02/18  19:36:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:32  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:30:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:06  root]
 * 
 * Revision 1.1.6.4  1994/08/16  18:28:03  jd
 * 	drop from janet
 * 	[1994/08/13  16:07:52  jd]
 * 
 * Revision 1.1.6.3  1994/08/03  19:04:05  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:24:34  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:43:18  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:47  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:11:09  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:41:44  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:21:21  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:21:29  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/30  19:52:58  peckham
 * 	OT#4476: Changed db_pseudo_chdirectory() to ignore state of
 * 	clearinghousewhen getting a stream -- otherwise convert fails
 * 	to get DNS$CHDirectories.
 * 	[1992/06/30  19:52:25  peckham]
 * 
 * Revision 1.1  1992/01/19  15:25:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: db_pseudo.c
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
 * Functions that pertain to pseudo directory.
 *
 */

#include <server.h>
#include <dbdef.h>
#include <sets.h>
#include <tlog.h>
#include <uid.h>
#include <names.h>

/*
 * db_pseudo_create
 *	This routine creates the pseudo directory record.
 *
 * Input:
 *	stream_p = pointer to stream descriptor
 *	user_p = pointer to user descriptor 
 *	version_p = pointer to buffer containing directory version
 */
dns_status_t 
db_pseudo_create (struct file_info        *file_p,
                  struct user_descriptor  *user_p,
                  version_t               *version_p)
{
    struct db_data dir_data;
#define PCNT 11
    struct entrylist plist[PCNT];
    byte_t statevalue = CS_on;
    struct nsgbl_ch *ch_p;
    struct directory_bucket *pseudo_p;
    struct dir_hash_entry *hash_p;
    int pcnt = 10;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_pseudo_create()"));

    pseudo_p = (directory_bucket_t *)dns_malloc_perm(
		    DIRECTORY_BUCKET_BLOCKS*BLOCK_SIZE);
    if (!pseudo_p) return(DNS_NONSRESOURCES);

    /* Initialize bucket */
    pseudo_p->dir_type = PSEUDO_BUCKET;
    pseudo_p->dir_blocks = DIRECTORY_BUCKET_BLOCKS;

    /* Initialize attributes */

    /* initial address should be a NULL address */
    ch_p = file_p->file_ch_p;
    plist[0].el_type = DNS_ALLUPTO;
    plist[0].el_len = sizeof(NullTimestamp);
    plist[0].el_value_p = (byte_t *)NullTimestamp;

    plist[1].el_type = DNS_OBJECTUID;
    plist[1].el_len = sizeof(ch_p->ch_uid);
    plist[1].el_value_p = (byte_t *)&ch_p->ch_uid;

    plist[2].el_type = DNS_CHLASTADDRESS;
    plist[2].el_value_p = 0;
    plist[2].el_len = 0;

    plist[3].el_type = DNS_CHNAME;
    plist[3].el_len = LEN_FullName(&ch_p->ch_name);
    plist[3].el_value_p = (byte_t *)&ch_p->ch_name;

    plist[4].el_type = DNS_CHSTATE;
    plist[4].el_len = sizeof(statevalue);
    plist[4].el_value_p = &statevalue;

    plist[5].el_type = DNS_CTS;
    plist[5].el_len = sizeof(ch_p->ch_cts);
    plist[5].el_value_p = (byte_t *)&ch_p->ch_cts;

    plist[6].el_type = DNS_DIRECTORYVERSION;
    plist[6].el_len = sizeof(*version_p);
    plist[6].el_value_p = (byte_t *)version_p;

    plist[7].el_type = DNS_REPLICAVERSION;
    plist[7].el_len = sizeof(*version_p);
    plist[7].el_value_p = (byte_t *)version_p;

    if (ch_p->ch_cellname_p) {
	plist[8].el_type = DNS_NSCELLNAME;
	plist[8].el_len = strlen(ch_p->ch_cellname_p);
	plist[8].el_value_p = (byte_t *)ch_p->ch_cellname_p;
	pcnt = 9;
    }

    dir_data.data_buf_len = DIR_DATA_SIZE;
    dir_data.data_type = ET_clearinghouse;
    dir_data.data_p = (data_record_t *)pseudo_p->dir_data;

    /* Build data record */
    status = create_record(&dir_data, pcnt, plist);
    if (status == DNS_SUCCESS) 
	{
	/* Add access control entries */
        status = create_entry_acs(user_p, NULL, &dir_data, 
		NULL, ch_p);
	if (status == DNS_SUCCESS)
	    /* Insert into hash tables */
	    status = db_hash_insert(file_p, pseudo_p, ET_clearinghouse, &hash_p);
	}
    if (status != DNS_SUCCESS)
        {
	/* Release resources on error */
	dns_free((char *)pseudo_p);
        }
    else
	/* Fill in nsgbl_ch structure */
	db_init_ch(hash_p, file_p->file_ch_p);

    return(status);
}

/* -------------------------------------------------------------------------- */

/*
 * db_pseudo_read
 *	This routine read the pseudo directory record.  It is a special case
 *	of read directory, we set up the key.
 *	Returns the record without a lock.
 *
 * Input:
 *	stream_p = pointer to stream descriptor
 */
dns_status_t 
db_pseudo_read (struct db_stream *stream_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_pseudo_read()"));

    /* Build pseudo directory name */
    names_new(&stream_p->db_file_p->file_ch_p->ch_uid,
	      NULL, 
	      (FullName_t *)stream_p->db_key);

    return(db_pdir_read(stream_p));
}

/* -------------------------------------------------------------------------- */


/*
 * db_pseudo_read_for_update
 *	This routine read the pseudo directory record for update.
 *	It is a special case db_pdir_read_for_update, we set up the key.
 *	Returns the record locked.
 *
 * Input:
 *	stream_p = pointer to stream descriptor
 */
dns_status_t 
db_pseudo_read_for_update (struct db_stream *stream_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_pseudo_read_for_update()"));

    /* Build pseudo directory name */
    names_new(&stream_p->db_file_p->file_ch_p->ch_uid,
	      NULL, 
	      (FullName_t *)stream_p->db_key);

    return(db_pdir_read_for_update(stream_p));
}

/* -------------------------------------------------------------------------- */

/*
 * db_pseudo_update
 *	This routine updates the pseudo directory record. Lock
 *	must already be held.
 *
 * Input:
 *	stream_p = pointer to stream descriptor
 *	tlog_p = pointer to transaction description
 */
dns_status_t 
db_pseudo_update (struct db_stream  *stream_p,
                  struct tlog_dsc   *tlog_p)
{
  
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_pseudo_update()"));

    return(db_dir_update(stream_p, tlog_p));
}

/* -------------------------------------------------------------------------- */


/*
 * db_pseudo_chdirectory
 *
 * Add or remove a directory ID to the set of all directories
 * in this clearinghouse.
 *
 * Arguments:
 *     file_p    - (in) pointer to file information
 *     chdir_p   - (in) pointer to the ID of the diretory being added.
 *     operation - (in) PRESENT or ABSENT
 *
 */
dns_status_t 
db_pseudo_chdirectory (struct file_info  *file_p,
                       ObjUID_t          *chdir_p,
                       int               operation)
{
    Update_u update;
    Timestamp_u ts;
    struct db_stream *stream_p;
    dns_status_t status;
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_pseudo_chdirectory()"));

    /* Get independent stream */
    status = db_get_stream_ignore(file_p, &stream_p, OP_ModifyAttribute);
    if (status != DNS_SUCCESS) return(status);

    status = db_pseudo_read_for_update(stream_p);
    if (status != DNS_SUCCESS) 
	{
	db_free_stream(stream_p);
	return(status);
	}
                                     
    /* Get update to insert/remove a new CHDirectories member */
    ts_new((Timestamp_t *)ts);
    update_dns_build((Update_t *)update, operation, (Timestamp_t *)ts, 
                 dns_Attribute(DNS_CHDIRECTORIES), BL_true,
                 LEN_ObjUID(chdir_p), (byte_t *)chdir_p);

    status = set_insert(&stream_p->db_dir, (Update_t *)update,
		dns_Attribute(DNS_CHDIRECTORIES), TRUE);

    if (status == DNS_SUCCESS)
        db_pseudo_update(stream_p, NULL);

    db_dir_release(stream_p);
    db_free_stream(stream_p);
    return(status); 
}
