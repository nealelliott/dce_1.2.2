/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta_modify.c,v $
 * Revision 1.1.8.2  1996/02/18  19:37:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:46  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:33:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:17  root]
 * 
 * Revision 1.1.6.3  1994/08/03  19:04:56  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:32:01  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  18:44:08  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:45  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  22:13:21  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:47:35  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:24:39  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:25:22  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/03  17:31:38  peckham
 * 	OT#3740: forgot to mark freed space as freed
 * 	in ta_modify_replicatype().
 * 	[1992/06/03  16:02:45  peckham]
 * 
 * Revision 1.1  1992/01/19  15:26:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module ta_modify.c
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
 * Modify attribute transaction functions.
 *
 */
#include <server.h>
#include <back.h>
#include <dbdef.h>
#include <find.h>
#include <names.h>
#include <security.h>
#include <tlog.h>
#include <uid.h>

/*
 * Local prototypes
 */
static dns_status_t
ta_modify_replicatype (
    user_descriptor_t	*,
    db_stream_t		*,
    int			 );

/*
 * ta_modify_attribute
 *   This routine locates an entry and applies one udpate
 * to it.
 *
 *
 * Input:
 *      user_p = address of user description
 *      progress_p = address of progress record
 *      type = type of entry to be updated
 *      update_p = address of update structure
 *	updlen = length of update
 *
 *
 * Function value:
 *      status
 */
dns_status_t 
ta_modify_attribute (user_descriptor_t  *user_p,
                     Progress_t         *progress_p,
                     int                type,
                     Update_t           *update_p,
                     int                updlen)
{
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    tlog_dsc_t		tlog;
    AttributeSpecifier_t *const attribute_p =
			    (AttributeSpecifier_t *)update_p->ud_attribute;
    BOOLEAN_t *const	valuePresent_p =
			    (BOOLEAN_t *)SKIP_AttributeSpecifier(attribute_p);
    db_stream_t		*stream_p;
    attribute_descriptor_t *dns_p;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >ta_modify_attribute(%s,%p,%d,%p,%d)",
        deb_ascii_user_descriptor(userBuf,user_p),
        progress_p,
        type,
        update_p,
        updlen));

    status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_ModifyAttribute);
    if (status != DNS_SUCCESS)
	goto leave_ta_modify_attribute;

    status = walktree(user_p, type, WALKTREE_UPDATE_OP, progress_p, stream_p);
    if (status != DNS_SUCCESS)
	goto leave_ta_modify_attribute;

    /* find out what we got */
    if (type == ET_dirOrObj)
	if (FLAG_IS_SET(progress_p->pr_flags, PR_directory))
	    type = ET_directory;
	else
	    type = ET_object;

    /* Get descriptor for global attributes */
    dns_p = find_known_attribute((AttributeName_t *)attribute_p->as_name);

    if (dns_p && !dns_p->visable)
      status = DNS_NOTSUPPORTED;
    else switch (type)
        {
        case ET_object:
	    /* 
	     * Permit the object of the DNS_CHDIRECTORIES attribute to
	     * be modified in this instance.  Required for support of 
	     * DNS$CHDirectories attribute on the clearinghouse object.
	     */
	    if (dns_p)
		if (dns_p->atype != DNS_CHDIRECTORIES)
		    if (!dns_p->change) status = DNS_INVALIDUPDATE;
            break;
        case ET_directory:   
	    if (dns_p)
                if (!dns_p->dir_rec) dns_p = NULL;
                else if (dns_p->rep_rec) status = DNS_NOTSUPPORTED;
                else if (!dns_p->change) status = DNS_INVALIDUPDATE;
            break;
        case ET_softlink:   
	    if (dns_p)		
                if (!dns_p->link_rec) dns_p = NULL;
                else if (!dns_p->change) status = DNS_INVALIDUPDATE;
 	    break;
        case ET_clearinghouse:   
	    if (dns_p)
                if (!dns_p->ch_rec) dns_p = NULL;
                else if (!dns_p->change) status = DNS_INVALIDUPDATE;
            break;
        case ET_childPointer:   
	    if (dns_p)
                if (!dns_p->child_rec) dns_p = NULL;
            break;
        default:
            status = DNS_INVALIDUPDATE;
        }
    if ((status == DNS_SUCCESS) && dns_p)
	if ((EXT8(update_p->ud_operation) != UD_present) && 
		!EXT8(valuePresent_p) && !dns_p->delete)
	    if (dns_p->atype != DNS_CHDIRECTORIES)
		status = DNS_NOTSUPPORTED;

    if (status != DNS_SUCCESS) 
	{
	(void)db_close_stream(stream_p);
	goto leave_ta_modify_attribute;
	}


   /* 
    * BEGIN ACS checking... 
    *
    * Cannot add new attributes to anything but objects and directories.
    * If the 
    * attribute is not a nameserver attribute don't allow it to be added
    */
    if (!dns_p && ((type != ET_object) && (type != ET_directory)))
        status = DNS_NOTSUPPORTED;
    else if (dns_p &&
	    (dns_p->atype == DNS_INCHNAME))
        /* The DNS$ACS and DNS$INCHNAME attributes require CONTROL */
        status = security_check(user_p, stream_p, Super_check, type, 
				NULL);

    else if ((EXT8(update_p->ud_operation) != UD_present)
		&& !EXT8(valuePresent_p))
	/* trying to delete an attribute need DELETE */
	status = security_check(user_p, stream_p, Modify_Del_check, type,
				NULL);

    else   
	/* trying to just modify an attribute need WRITE */
	status = security_check(user_p, stream_p, Modify_check, type,
				NULL);

    /*
     * done with access control checking!
     */
    if (status == DNS_SUCCESS) 
        {
	    /* 
	     * Read the directory so we can check the update timestamp
	     * against last skulk, and allupto and make sure it isn't
	     * in the future.  Want a recent copy of directory to avoid
	     * accepting an old update.
	     * We waited until now to read the directory cause security
	     * check could have gone off node.
	     * Read the directory with no lock, there is a small chance
	     * that the directory could change out from under us.
	     */
	    if (!stream_p->db_dir.data_p)
		status = db_dir_read(stream_p);
	    if (status == DNS_SUCCESS)
	        if (!ts_in_range(stream_p->db_dir.data_p,
			(Timestamp_t *)update_p->ud_timestamp))
		    {
                    if (EQ_Timestamp(update_p->ud_timestamp, NullTimestamp))
			status = ts_new((Timestamp_t *)update_p->ud_timestamp);
		    else 		
			status = DNS_BADCLOCK;
		    }

	    if (status == DNS_SUCCESS)
		{
		tlog.cnt = 1;
		tlog.len = updlen;
		tlog.buf_p = (unsigned char *)update_p;
	   	status = applyupdate(stream_p, type, update_p, dns_p,
				TRUE, &tlog);
		}
	}
	else if (status == DNS_UNKNOWNENTRY)
	    /* Rebuild resolved name for exception */
	    resolved_null(progress_p);

    (void)db_close_stream(stream_p);

leave_ta_modify_attribute:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <ta_modify_attribute(%s,%p,%d,%p,%d) return(%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        progress_p,
        type,
        update_p,
        updlen,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * ta_modify_replicatype
 *   In order to change the replicatype of a directory,
 * write access to the clearinghouse is required.
 * If the request is only going to re-write the same value
 * we won't bother with the check, since the directory
 * is probably in trouble.
 *
 *
 * Input:
 *      user_p = address of user description
 *      stream_p = address of current stream
 *      update_p = address of update structure
 *
 * Function value:
 *      status
 *
 */
static dns_status_t 
ta_modify_replicatype (user_descriptor_t  *user_p,
                       db_stream_t        *stream_p,
                       int                type)
{
    db_data_t		save_data;
    DBSet_t		*set_p;
    DBSetMember_t	*val_p;
    int			idx;
    dns_status_t	status;
    dir_hash_entry_t    *hash_p;

    /* If the replicatype is being CHANGED, need write to ET_clearinghouse */
    if (find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(DNS_REPLICATYPE),
		&set_p, &idx) == DNS_SUCCESS) 
        if ((val_p = find_last_value(set_p)) != NULL)
	        if (EXT8(val_p->dm_data) != type)
		    {
		    /* Sav directory data */
		    COPY_bytes(&stream_p->db_dir, &save_data, sizeof(db_data_t));
		    hash_p = stream_p->db_dir_hash_p;

		    /* Make sure it reads a new entry and doesn't use this */
		    stream_p->db_dir_hash_p = NULL;
		    ZERO_bytes(&stream_p->db_dir, sizeof(db_data_t));

                    status = db_pseudo_read(stream_p);
            	    if (status == DNS_SUCCESS) 
            	        {
            		status = security_check(user_p, stream_p, 
			    Modify_Rptype_check, ET_clearinghouse, NULL);
            		dns_free((char *)stream_p->db_dir.data_p);
			stream_p->db_dir.data_p = NULL;
			}
		    /* Restore saved directory data */
		    COPY_bytes(&save_data, &stream_p->db_dir, sizeof(db_data_t));
		    stream_p->db_dir_hash_p = hash_p;
           	    if (status != DNS_SUCCESS) return(status);
		    }

    return(db_dir_set_type(stream_p, type, NULL, NULL));
}

/*
 * ta_modify_replica
 *   This routine tries to find a directory.  If the entry is found
 * it applies one update to one attribute.  This call is used
 * by the background process to update replicas.
 *
 *
 * Input:
 *      user_p = address of user description
 *      diruid_p = address of directory uid
 *      update_p = address of update structure
 *	updlen = length of update
 *
 *
 * Function value:
 *      status
 *
 */
dns_status_t 
ta_modify_replica (user_descriptor_t  *user_p,
                   ObjUID_t           *diruid_p,
                   Update_t           *update_p,
                   int                updlen)
{
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_ObjUID(diruidBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    tlog_dsc_t		tlog;
    db_stream_t		*stream_p;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >ta_modify_replica(%s,%s,%p,%d)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_ObjUID(diruidBuf,diruid_p),
        update_p,
        updlen));
  
    status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_ModifyAttribute);
    if (status != DNS_SUCCESS) 
	goto leave_ta_modify_replica;

    tlog.cnt = 1;
    tlog.len = updlen;
    tlog.buf_p = (byte_t *)update_p;

    names_new(diruid_p, NULL, (FullName_t *)stream_p->db_key);
    
    /* Get directory to modify */
    status = db_dir_read_for_update(stream_p);

    if (status == DNS_UNKNOWNENTRY)
        {
        status = db_pseudo_read(stream_p);
        if (status == DNS_SUCCESS) 
            {
	    /* Fake type so get answer - special cases CH_REC */
	    stream_p->db_dir.data_type = ET_directory;
#ifdef DCE_SEC
            status = security_dacl(user_p, &stream_p->db_dir, 
				   sec_acl_perm_read, ANYPerms);
#endif
	    stream_p->db_dir.data_type = ET_clearinghouse;
            if ((status == DNS_SUCCESS) || (status == DNS_ACCESSDENIED))
                status = DNS_NOTAREPLICA;
            }
        }
    else if (status == DNS_SUCCESS) 
        {
        status = security_check(user_p, stream_p, Super_check, ET_directory,
				NULL);
        if (status == DNS_SUCCESS) 
            {
	    AttributeSpecifier_t *const attribute_p =
				(AttributeSpecifier_t *)update_p->ud_attribute;
	    attribute_descriptor_t *const dns_p =
		    find_known_attribute((AttributeName_t *)attribute_p->as_name);
	    if (dns_p->atype == DNS_REPLICATYPE)
		{
		AtomicValue_t *const av_p = (AtomicValue_t *)
			    SKIP_BOOLEAN(SKIP_AttributeSpecifier(attribute_p));

		status = ta_modify_replicatype(user_p, stream_p, 
				EXT8(av_p->av_value));
		if (status == DNS_SUCCESS)
		    status = db_dir_update(stream_p, &tlog);
		}	
            else if (dns_p->atype == DNS_REPLICASTATE)
		{
		AtomicValue_t *const av_p = (AtomicValue_t *)
			    SKIP_BOOLEAN(SKIP_AttributeSpecifier(attribute_p));

		status = db_dir_set_state(stream_p, EXT8(av_p->av_value),
			NULL, NULL);
		if (status == DNS_SUCCESS)
		    status = db_dir_update(stream_p, &tlog);
		}
            else 
		status = applyupdate(stream_p, ET_directory, update_p, 
			dns_p, FALSE, &tlog);

            }
        else if (status == DNS_UNKNOWNENTRY)
            status = DNS_NOTAREPLICA;
	else
	    /* DNS_ACCESSDENIED */
	    {
	    status = DNS_UNTRUSTEDCH;
            security_log(user_p,
			(FullName_t *)stream_p->db_dir_hash_p->dir_name,
			stream_p->db_file_p->file_ch_p);
	    }
	}

    (void)db_close_stream(stream_p);

leave_ta_modify_replica:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <ta_modify_replica(%s,%s,%p,%d) return(%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_ObjUID(diruidBuf,diruid_p),
        update_p,
        updlen,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}
