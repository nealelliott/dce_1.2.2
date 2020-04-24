/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta_replica.c,v $
 * Revision 1.1.9.2  1996/02/18  19:37:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:50  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:33:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:20  root]
 * 
 * Revision 1.1.7.7  1994/08/26  21:55:38  zee
 * 	In ta_replica_add, stick V4.0 replica id in unused timestamp area.
 * 	[1994/08/26  21:50:01  zee]
 * 
 * Revision 1.1.7.6  1994/08/23  20:01:07  mccann
 * 	allow root replicas to be removed: OT 11637
 * 	[1994/08/23  20:00:39  mccann]
 * 
 * Revision 1.1.7.5  1994/08/16  18:28:12  jd
 * 	drop from janet
 * 	[1994/08/13  16:07:24  jd]
 * 
 * Revision 1.1.7.4  1994/08/03  19:04:59  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:32:17  mccann]
 * 
 * Revision 1.1.7.3  1994/06/09  18:44:11  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:49  devsrc]
 * 
 * Revision 1.1.7.2  1994/05/12  21:12:29  peckham
 * 	LOG_ASCII_BUF_user_descriptor() is defined.
 * 	Accomodate SNI_DCOSX conditional.
 * 	[1994/05/12  19:18:47  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  22:13:29  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:47:59  peckham]
 * 
 * Revision 1.1.5.3  1992/12/30  14:24:47  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:25:37  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  19:16:26  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:17:32  weisman]
 * 
 * Revision 1.1.2.4  1992/06/01  21:17:09  peckham
 * 	Fix OT#3736: endian problem in create_replica_record().
 * 	[1992/06/01  16:00:07  peckham]
 * 
 * Revision 1.1.2.3  1992/04/24  13:05:30  keegan
 * 	Server thread dies (in 'free') when creating replicas causing cdscp to hang.
 * 	[1992/04/22  18:18:14  keegan]
 * 
 * Revision 1.1.2.2  1992/03/22  22:38:10  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:37:19  weisman]
 * 
 * Revision 1.1  1992/01/19  15:26:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * MODULEID ta_replica.c
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
#include <clerk.h>
#include <dbdef.h>
#include <find.h>
#include <names.h>
#include <replica.h>
#include <security.h>
#include <sets.h>
#include <tlog.h>

# include <dce/uuid.h>

#define INITIAL_RECORD_SIZE 4096

/*
 * Local prototypes
 */
static dns_status_t
read_replica_acl (
    FullName_t		*,
    db_stream_t		*);

static dns_status_t
fixup_replica (
    db_stream_t		*,
    ObjUID_t		*,
    VersionNumber_t     *);
#if !defined(DCE_SEC)

/*
 * read_replica_acl
 *     This routine builds the DNS$ACS attribute for a new replica
 * by reading the contents of DNS$ACS from another replica.
 * It sets the timestamps = NULL so these copies will be updated during
 * a skulk.
 *
 * Input:
 *     dirname_p = address of directory full name
 *     dir_p = address of record descriptor
 *
 * Value:
 *     status
 */
static dns_status_t 
read_replica_acl (FullName_t   *dirname_p,
                  db_stream_t  *stream_p)
{
    return(DNS_SUCCESS);
}
#else /* DCE_SEC */
/*
 * read_replica_acl
 *     This routine builds the DNS_OBJECTACL attribute for a new replica
 * by reading the contents of DNS_OBJECTACL from another replica.
 * Note this in a single valued attribute.
 * It sets the timestamps = NULL so these copies will be updated during
 * a skulk.
 *
 * Input:
 *     dirname_p = address of directory full name
 *     dir_p = address of record descriptor
 *
 * Value:
 *     status
 */
static dns_status_t 
read_replica_acl (FullName_t   *dirname_p,
                  db_stream_t  *stream_p)
{
    DEB_ASCII_BUF_FullName(dirnameBuf)
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    Update_u            upd;
    Update_t		*update_p = (Update_t *)upd;
    dns_status_t	status;
    byte_t              *flat_p;
    int                 len;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >read_replica_acl(%s,%s)",
        deb_ascii_FullName(dirnameBuf,dirname_p),
        deb_ascii_db_stream(streamBuf,stream_p)));

    
    status = dacl_lookup(dirname_p, &flat_p, &len, sec_acl_type_object);
    if (status == DNS_SUCCESS) {
	
	(void)update_dns_build(update_p, UD_present, 
			 NullTimestamp,
			 dns_Attribute(DNS_OBJECTACL), BL_true,
			 len, flat_p);

	status = set_insert(&stream_p->db_dir, update_p, 
			    dns_Attribute(DNS_OBJECTACL), FALSE);
	
    }
	
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " <read_replica_acl(%s,%s) return(%s)",
        deb_ascii_FullName(dirnameBuf,dirname_p),
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

#endif /* !DCE_SEC */

/*
 * ROUTINE - fixup_replica                
 *
 * This routine sets the Epoch and ReplicaVersion values. 
 * It is called after a successful return from LinkReplica.
 *
 * Arguments:
 *
 *   dir_p             - pointer to directory record descriptor
 *                        *rd_ekey_p set up
 *   epoch_p           - pointer to epoch returned from LinkReplica
 *   replicaversion_p  - pointer to ReplicaVersion returned from LinkReplica
 */
static dns_status_t 
fixup_replica (db_stream_t      *stream_p,
               ObjUID_t         *epoch_p,
               VersionNumber_t  *replicaversion_p)
{
    tlog_dsc_t		tlog;
    bytes_u		updbuf[(2*(sizeof(Update_u) - MAXATTRIBUTE +
				sizeof(ObjUID_u))) +
                               (sizeof(Update_u) - MAXATTRIBUTE + 
                                sizeof(VersionNumber_u))];
    int			len;

    /* Set up epoch and replicaversion */
    tlog.cnt = 1;
    tlog.buf_p = updbuf;
    tlog.len = update_dns_build((Update_t *)updbuf,
                                UD_present, NullTimestamp,
                                dns_Attribute(DNS_EPOCH),
                                BL_true,
                                sizeof(*epoch_p), (byte_t *)epoch_p);
    (void)set_replace_value(&stream_p->db_dir, (Update_t *)updbuf, 
		      dns_Attribute(DNS_EPOCH));

    len = update_dns_build((Update_t *)SKIP_bytes(updbuf, tlog.len),
                           UD_present, NULL,
                           dns_Attribute(DNS_REPLICAVERSION),
                           BL_true,
                           sizeof(VersionNumber_u),
                           (byte_t *)replicaversion_p);
    (void)set_replace_value(&stream_p->db_dir,
                      (Update_t *)SKIP_bytes(updbuf, tlog.len),
                      dns_Attribute(DNS_REPLICAVERSION));
    tlog.cnt++;
    tlog.len += len;

    (void)db_dir_update(stream_p, &tlog);

    return (DNS_SUCCESS);
}

/*
 * create_replica_record
 *	Build the directory replica
 *
 * Input:
 *	stream_p = address of stream descriptor
 *	cts_p = address of replica cts
 *	uid_p = address of replica uid
 *	name_p = address of replica name
 * 	epoch_p = address of replica epoch
 *	dir_type  = replica type to create
 *	rp_p = replica len and value
 */
dns_status_t 
create_replica_record (db_stream_t    *stream_p,
                       Timestamp_t    *cts_p,
                       ObjUID_t       *uid_p,
                       FullName_t     *name_p,
                       ObjUID_t       *epoch_p,
                       int            dir_type,
                       AtomicValue_t  *rp_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_Timestamp(ctsBuf)
    DEB_ASCII_BUF_ObjUID(uidBuf)
    DEB_ASCII_BUF_FullName(nameBuf)
    DEB_ASCII_BUF_ObjUID(epochBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    bytes_u		updbuf[sizeof(Update_u) - MAXATTRIBUTE +
				sizeof(ParentPointer_u)];
    ParentPointer_t	parent;
    struct entrylist	elist[11];
    byte_t		replicaType = dir_type;
    ReplicaState_u	replicaState;
    dns_status_t	status;
    int cnt = 0;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >create_replica_record(%s,%s,%s,%s,%s,%d,%p)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_Timestamp(ctsBuf,cts_p),
        deb_ascii_ObjUID(uidBuf,uid_p),
        deb_ascii_FullName(nameBuf,name_p),
        deb_ascii_ObjUID(epochBuf,epoch_p),
        dir_type,
        rp_p));

    /* See if replica already exists */
    stream_p->db_dir_hash_p = NULL;
    names_new(uid_p, NULL, (FullName_t *)stream_p->db_key);

    status = db_dir_read(stream_p);

    if (status == DNS_SUCCESS) {
        status = DNS_ALREADYREPLICA;
	goto leave_create_replica_record;
    }

    /* Build temporary parent pointer */
    ZERO_ObjUID(parent.pp_parentID);
    COPY_Timeout(NullTimeout, parent.pp_timeout);
    COPY_FullName(name_p, parent.pp_myName);

    elist[cnt].el_type = DNS_ALLUPTO;
    elist[cnt].el_len = sizeof(NullTimestamp);
    elist[cnt++].el_value_p = (unsigned char *)NullTimestamp;

    elist[cnt].el_type = DNS_OBJECTUID;
    elist[cnt].el_len = sizeof(*uid_p);
    elist[cnt++].el_value_p = (unsigned char *)uid_p;


    elist[cnt].el_type = DNS_CTS;
    elist[cnt].el_len = sizeof(*cts_p);
    elist[cnt++].el_value_p = (unsigned char *)cts_p;

    elist[cnt].el_type = DNS_EPOCH;
    elist[cnt].el_len = sizeof(*epoch_p);
    elist[cnt++].el_value_p = (unsigned char *)epoch_p;
    
    elist[cnt].el_type = DNS_LASTSKULK;
    elist[cnt].el_len = sizeof(NullTimestamp);
    elist[cnt++].el_value_p = (unsigned char *)NullTimestamp;

    elist[cnt].el_type = DNS_LASTUPDATE;
    elist[cnt].el_len = sizeof(NullTimestamp);
    elist[cnt++].el_value_p = (unsigned char *)NullTimestamp;
  
    elist[cnt].el_type = DNS_REPLICAS;
    elist[cnt].el_value_p = rp_p->av_value;
    elist[cnt++].el_len = EXT16(rp_p->av_length);

    INS8(replicaState, RS_newReplica);
    elist[cnt].el_type = DNS_REPLICASTATE;
    elist[cnt].el_len = sizeof(replicaState);
    elist[cnt++].el_value_p = (unsigned char *)replicaState;
  
    elist[cnt].el_type = DNS_REPLICATYPE;
    elist[cnt].el_len = sizeof(replicaType);
    elist[cnt++].el_value_p = (unsigned char *)&replicaType;

    stream_p->db_dir.data_p = (data_record_t *)dns_malloc_temp(INITIAL_RECORD_SIZE);
    if (!stream_p->db_dir.data_p) {
	status = DNS_NONSRESOURCES;
	goto leave_create_replica_record;
    }
    stream_p->db_dir.data_buf_len = INITIAL_RECORD_SIZE;
    status = create_record(&stream_p->db_dir, cnt, elist);
    if (status == DNS_SUCCESS)
    	{
	/* If this is the root directory, don't insert a parentpointer */
	if (NE_FullName(name_p, &stream_p->db_file_p->file_ch_p->ch_cellname))
	    {
	    /* Insert parentpointer with null time stamp so it will be replaced */
	    (void)update_dns_build((Update_t *)updbuf, UD_present,
			NullTimestamp, dns_Attribute(DNS_PARENTPOINTER),
			BL_true, LEN_ParentPointer(&parent), (byte_t *)&parent);
	    (void)set_insert(&stream_p->db_dir, (Update_t *)updbuf, 
			dns_Attribute(DNS_PARENTPOINTER), TRUE);
	    }
        status = read_replica_acl(name_p, stream_p);
        if (status == DNS_SUCCESS)
            status = db_dir_create(stream_p);
		
        }

leave_create_replica_record:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " <create_replica_record(%s,%s,%s,%s,%s,%d,%p) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_Timestamp(ctsBuf,cts_p),
        deb_ascii_ObjUID(uidBuf,uid_p),
        deb_ascii_FullName(nameBuf,name_p),
        deb_ascii_ObjUID(epochBuf,epoch_p),
        dir_type,
        rp_p,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * ta_add_replica_check
 *	This routine makes sure the link_replica call completed during
 *	an add_replica.  This would not happen if the system crashed
 *	before the link completed.  If not, we will attempt to link it here.
 *	The directory is read in and locked on input.
 *
 * Input:
 *	stream_p = pointer to stream descriptor, hash entry valid
 *
 */
void 
ta_add_replica_check (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    Update_t		update;
    AuthenticationInfo_t onbehalfof;
    DBSet_t		*set_p;
    DBSetMember_t	*member_p;
    ObjUID_t		*epoch_p;
    VersionNumber_t     *replicaversion_p;
    AtomicValue_t	*av_p;
    unsigned int	updlen;
    int			idx;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >ta_add_replica_check(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    /* For CDS, the onbehalfof field is not passed.  This check was done 
     * by a seperate check prior to writing this record to disk 
     *
     * Set up a new onbehalfof for DNS
     */
    ZERO_FullName(onbehalfof.ai_principal);
    /* Build update structure for replica pointer */
    av_p = (AtomicValue_t *)SKIP_bytes(&update, 
		       update_dns_build(&update, UD_present, NullTimestamp,
			dns_Attribute(DNS_REPLICAS), BL_true, 0, NULL));

    INS16(av_p->av_length, replica_build((ReplicaPointer_t *)av_p->av_value,
		&stream_p->db_file_p->file_ch_p->ch_uid, NULL, 
		db_dir_get_replica_type(stream_p->db_dir_hash_p),
                &stream_p->db_file_p->file_ch_p->ch_name));

    updlen = SKIP_AtomicValue(av_p) - (byte_t *)&update;

    /* Find epoch in record and use as output buffers */
    (void)find_attribute(stream_p->db_dir.data_p,
		   dns_AttributeName(DNS_EPOCH), &set_p, &idx);
    member_p = (DBSetMember_t *)DATA_DBSet(set_p);
    epoch_p = (ObjUID_t *)member_p->dm_data;

    replicaversion_p = (VersionNumber_t *)dns_malloc_temp(sizeof(VersionNumber_u));

    status = clerk_link_replica((byte_t *)&onbehalfof, 
		(FullName_t *)stream_p->db_dir_hash_p->dir_name,
		(ObjUID_t *)stream_p->db_dir.data_uid,
		&update, updlen,
		epoch_p, replicaversion_p);

    if ((status == DNS_UNKNOWNENTRY) || (status == DNS_ACCESSDENIED))
	{
	/* Save next pointer, cause delete frees current */
	db_dir_delete(stream_p);
	stream_p->db_dir_hash_p = NULL;
 	db_dir_release(stream_p);
	}
    else if (status == DNS_SUCCESS)
        {
        (void)update_dns_build(&update,
                         UD_present, NULL,
                         dns_Attribute(DNS_REPLICAVERSION), 
                         BL_true,
                         sizeof(VersionNumber_u),
                         (byte_t *)replicaversion_p);
        (void)set_replace_value(&stream_p->db_dir,
                          &update,
                          dns_Attribute(DNS_REPLICAVERSION));
	(void)db_dir_update(stream_p, NULL);
        }


    dns_free((char *)replicaversion_p);
    /* Assume it is some communication problem, background will check later */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " <ta_add_replica_check(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));
}

/*
 * ta_replica_add
 *   This routine creates a directory replica and writes it to the database.
 * It is used by clerk initiated transactions and management when
 * a new clearinghouse is created.  This routine is called with its
 * own stream.
 *
 * Input:
 *      user_p = address of user descriptor
 *      directory_p = address of directory full name
 *      replicatype = type of replica to create
 *
 * Function value:
 *      status
 *
 */
dns_status_t 
ta_replica_add (user_descriptor_t  *user_p,
                FullName_t         *directory_p,
                int                replicatype)
{
    LOG_ASCII_BUF_user_descriptor(userBuf)
    LOG_ASCII_BUF_FullName(directoryBuf)
    LOG_ASCII_BUF_dns_status(statusBuf)
    ObjUID_u		dir_uid, epoch;
    Timestamp_u         dir_cts;
    db_stream_t		*stream_p;
    AuthenticationInfo_t *behalf_p;
    Update_t		*update_p;
    FullName_t		*pname_p;
    FullName_t		*dirname_p;
    MemberValue_t	*retset_p;
    byte_t		*type_p;
    ObjUID_t		*epoch_p;
    VersionNumber_t     *replicaversion_p;
    byte_t		linked;
    Timestamp_t         v4replica_id;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >ta_replica_add(%s,%s,%d)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_FullName(directoryBuf,directory_p),
        replicatype));

    dirname_p = NULL;

    if (replicatype != RT_readOnly) {
        status = DNS_NOTSUPPORTED;
	goto leave_ta_replica_add;
    }

    status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_AddReplica);
    if (status != DNS_SUCCESS) 
	goto leave_ta_replica_add;

    status = db_pseudo_read(stream_p);
    if (status != DNS_SUCCESS) goto cleanup_add;

    status = security_check(user_p, stream_p, Replica_Add_check,
			    ET_clearinghouse, NULL );
    if (status != DNS_SUCCESS) goto cleanup_add;

    /* Clean out stream for next read */
    db_dir_release(stream_p);
    stream_p->db_dir_hash_p = NULL;

    /* Allocate enough memory for replica pointer update structure.
     * Memory for 3 full names (plus the one from the replica
     * pointer) and memory for the return set member and flag
     * from read_att, return epoch  from link_replica,
     * and flag for onhalfof
     */

    if (!(dirname_p = (FullName_t *)dns_malloc_temp(3 * sizeof(FullName_u) +
		sizeof(Update_u) - MAXATTRIBUTE + sizeof(ReplicaPointer_u) +
		READ_BUF_SIZE + sizeof(VersionNumber_u))))
	{
        status = DNS_NONSRESOURCES;
	goto cleanup_add;
	}

    /* Set up pointers */
    pname_p = (FullName_t *)SKIP_bytes(dirname_p, sizeof(FullName_u));
    behalf_p = (AuthenticationInfo_t *)SKIP_bytes(pname_p, sizeof(FullName_u));
    type_p = SKIP_bytes(behalf_p, sizeof(AuthenticationInfo_u));
    retset_p = (MemberValue_t *)SKIP_byte(type_p);
    epoch_p = (ObjUID_t *)type_p;
    replicaversion_p = (VersionNumber_t *)SKIP_ObjUID(epoch_p);
    update_p = (Update_t *)SKIP_VersionNumber(replicaversion_p);

    status = clerk_read_attribute_conf(directory_p, ET_directory, 
		dns_AttributeName(DNS_CTS),
		type_p, NULL, &linked, dnsConfMed);
    if ((status != DNS_SUCCESS) && (status != DNS_MOREDATA)) 
	goto cleanup_add;

    /* save data */
    COPY_Timestamp(retset_p->mv_value, dir_cts);

    status = clerk_read_attribute_conf(directory_p, ET_directory, 
		dns_AttributeName(DNS_OBJECTUID),
		type_p, NULL, &linked, dnsConfMed);
    if ((status != DNS_SUCCESS) && (status != DNS_MOREDATA)) 
	goto cleanup_add;

    /* save data */
    COPY_ObjUID(retset_p->mv_value, dir_uid);


    /* Get epoch */
    status = clerk_read_attribute_conf(directory_p, ET_directory, 
		dns_AttributeName(DNS_EPOCH),
		type_p, NULL, &linked, dnsConfMed);
    if ((status != DNS_SUCCESS) && (status != DNS_MOREDATA)) 
	goto cleanup_add;

    COPY_ObjUID(retset_p->mv_value, epoch);

    if (linked)
        {
	status = clerk_resolve_name(directory_p, dirname_p);
        if (status != DNS_SUCCESS) 
            COPY_FullName(directory_p, dirname_p);
        }
    else
        COPY_FullName(directory_p, dirname_p);

    status = DNS_SUCCESS;
#if defined(DCE_SEC)
    /* 
     * Onbehalf of check is done seperatly from the link replica
     * call in CDS.  Servers trust each other to have done this
     * check.  do it before record is written. 
     */
    status = dacl_test_access(user_p,  (FullName_t *)dirname_p,
		  sec_acl_perm_admin, 
		  sec_acl_perm_admin | sec_acl_perm_read | sec_acl_perm_write);
#endif

    get_v4_replica_ts(&v4replica_id);
    if (status == DNS_SUCCESS) {
	/* Build update structure for replica pointer */
	AtomicValue_t *const av_p = (AtomicValue_t *)SKIP_bytes(update_p, 
			update_dns_build(update_p, UD_present, &v4replica_id,
			dns_Attribute(DNS_REPLICAS), BL_true, 0, NULL));

	INS16(av_p->av_length, replica_build(
			(ReplicaPointer_t *)av_p->av_value,
			&stream_p->db_file_p->file_ch_p->ch_uid,
			NULL, replicatype, 
			&stream_p->db_file_p->file_ch_p->ch_name));

	status = create_replica_record(stream_p, (Timestamp_t *)dir_cts,
				   (ObjUID_t *)dir_uid, dirname_p,
				   (ObjUID_t *)epoch, replicatype, av_p);

    }

    /* Link this replica into master-replica */
    if (status == DNS_SUCCESS)
        {
	const int updateLen = LEN_Update(update_p);

        COPY_FullName(NullFullName, behalf_p->ai_principal);

	status = clerk_link_replica((byte_t *)behalf_p,
		dirname_p, (ObjUID_t *)dir_uid,
		update_p, updateLen, epoch_p, replicaversion_p);

	/* Lock still held on directory */
        if (status == DNS_SUCCESS)
            /* COMMIT POINT */
	    {
            int skulk_status;
            (void)fixup_replica(stream_p, epoch_p, replicaversion_p);
	    if (NE_FullName(dirname_p, 
			    &stream_p->db_file_p->file_ch_p->ch_cellname))
            	/* Root doesn't have a child */
	        /* Ignore errors, check_parentpointer will fix it */
		(void)clerk_modify_attribute(dirname_p, ET_childPointer,
			update_p, updateLen);
            (void)db_close_stream(stream_p); /* Free up write locks */
            skulk_status = clerk_skulk(dirname_p);
            if (skulk_status != DNS_SUCCESS)
	    {
                DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_server,
                    svc_c_debug3,
                    "ta_replica_add(%s,%s,%d) - ignored skulk failure(%s)",
                    deb_ascii_user_descriptor(userBuf,user_p),
                    deb_ascii_FullName(directoryBuf,directory_p),
                    replicatype,
                    deb_ascii_dns_status(statusBuf,skulk_status)));
	    }
            dns_free((char *)dirname_p);
	    goto leave_ta_replica_add;
	    }
        else
	    db_dir_delete(stream_p);
        }

cleanup_add:

    if (dirname_p)
        dns_free((char *)dirname_p);
    (void)db_close_stream(stream_p);

leave_ta_replica_add:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <ta_replica_add(%s,%s,%d) return(%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_FullName(directoryBuf,directory_p),
        replicatype,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * ta_replica_remove
 *    Remove a replica as long as it is not considered crucial.  This
 *    routine is used by management and the ta.  Three clearinghouses
 *    are involved, the one containing the replica to remove, the
 *    one with the master and the one with the master of the parent.
 *
 * Input:
 *    user_p = address of user descriptor
 *    uid_p = address of directory uid
 *    from_mgmt = TRUE if called by management
 *    stream_p = pointer to stream descriptor
 *
 */
dns_status_t 
ta_replica_remove (user_descriptor_t  *user_p,
                   ObjUID_t           *uid_p,
                   int                from_mgmt,
                   db_stream_t        *stream_p)
{
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_ObjUID(uidBuf)
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    tlog_dsc_t		tlog;
    bytes_u		updbuf[sizeof(Update_u) - MAXATTRIBUTE + 1];
    dir_hash_entry_t	*hash_p;
    FullName_t		*myname_p;
    int			state;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >ta_replica_remove(%s,%s,%d,%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_ObjUID(uidBuf,uid_p),
        from_mgmt,
        deb_ascii_db_stream(streamBuf,stream_p)));

    status = db_pseudo_read(stream_p);
    if (status != DNS_SUCCESS)
	goto leave_ta_replica_remove;

    status = security_check(user_p, stream_p, Replica_Remove_check, 
			    ET_clearinghouse, NULL);
    if (status != DNS_SUCCESS)
	goto leave_ta_replica_remove;

    /* Clear out stream for new read */
    db_dir_release(stream_p);
    stream_p->db_dir_hash_p = NULL;

    /* Build full name */
    names_new(uid_p, NULL, (FullName_t *)stream_p->db_key);

    status = db_dir_read_for_update(stream_p);
    if (status == DNS_UNKNOWNENTRY)
	{
 	status = DNS_NOTAREPLICA;
	goto leave_ta_replica_remove;
	}
    else if (status == DNS_SUCCESS)
	{
	status = security_check(user_p, stream_p, Super_check, ET_directory,
				NULL);
	if (status == DNS_UNKNOWNENTRY)
	    {
            status = DNS_NOTAREPLICA;
	    goto leave_ta_replica_remove;
	    }
	else if (status == DNS_SUCCESS) {
	    if ((db_dir_get_replica_type(stream_p->db_dir_hash_p) == 
		 RT_master) ||
		((!from_mgmt) && (stream_p->db_dir_hash_p->dir_root)))
		{
		  status = DNS_CRUCIALREPLICA;
		  goto leave_ta_replica_remove;
		}
	  }
      }

    if (status != DNS_SUCCESS)
	goto leave_ta_replica_remove;

    hash_p = stream_p->db_dir_hash_p;
    myname_p = (FullName_t *)hash_p->dir_name;
    state = db_dir_get_replica_state(hash_p);
    if (state == RS_dead)
	{
	status = DNS_SUCCESS;
	goto leave_ta_replica_remove;
	}
    else if (state == RS_dyingReplica)
	{
        db_dir_release_lock(stream_p);
	(void)clerk_skulk(myname_p);
	status = DNS_SUCCESS;
	goto leave_ta_replica_remove;
	}
    else if (state == RS_newReplica)
	{
	status = DNS_NOTAREPLICA;
	goto leave_ta_replica_remove;
	}

    /* Set up as dying, update happens later */
    tlog.cnt = 1;
    tlog.buf_p = updbuf;
    (void)db_dir_set_state(stream_p, RS_dyingReplica,
			   (Update_t *)updbuf, &tlog.len);
    status = db_dir_update(stream_p, &tlog);
    if (status == DNS_SUCCESS) 
	(void)clerk_skulk(myname_p);

leave_ta_replica_remove:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <ta_replica_remove(%s,%s,%d,%s) return(%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_ObjUID(uidBuf,uid_p),
        from_mgmt,
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}
