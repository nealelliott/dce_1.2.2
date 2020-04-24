/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta_read.c,v $
 * Revision 1.1.10.2  1996/02/18  19:37:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:48  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:33:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:18  root]
 * 
 * Revision 1.1.8.3  1994/08/03  19:04:57  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:32:09  mccann]
 * 
 * Revision 1.1.8.2  1994/06/09  18:44:10  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:47  devsrc]
 * 
 * Revision 1.1.8.1  1994/03/12  22:13:25  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:47:47  peckham]
 * 
 * Revision 1.1.4.5  1992/12/30  14:24:43  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:25:30  zeliff]
 * 
 * Revision 1.1.4.4  1992/12/22  21:11:42  peckham
 * 	Gen 45, 11-Dec-1992, Alan Peckham (CDS only)
 * 	     OT#6418: 'show clearinghouses' crashes cdsd.
 * 	     Change read_chdirectory() to calculate the length right for
 * 	     the DNS_CDS alternative.
 * 	[1992/12/22  21:10:52  peckham]
 * 
 * Revision 1.1.4.3  1992/12/21  20:04:39  jd
 * 	Backed out Allen's last change untill we fix all the places that are
 * 	relying on the old behavior.
 * 	[1992/12/21  18:46:23  jd]
 * 
 * Revision 1.1.4.2  1992/12/18  00:29:38  jd
 * 	    Alan Peckham, 17-Dec-1992
 * 	    OT#3523: Invalid attributes shown for entries
 * 	    This problem is many-fold. The initial architecture says
 * 	    that unknown attributes will return as a Set with the
 * 	    "present" flag off. 1) the handle routines don't
 * 	    distinguish this case. 2) There is no error code for this.
 * 	    3) during the conversion between internal and rpc formats
 * 	    the "present" flag is not maintained, always being reconstituted
 * 	    as being "on".
 * 	    Fix this by creating a new error "UnknownAttribute" and passing
 * 	    this back as an exception instead of the NullSet.
 * 	[1992/12/18  00:14:17  jd]
 * 
 * Revision 1.1.2.2  1992/01/22  23:02:57  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:18:43  melman]
 * 
 * $EndLog$
 */
/*
 * Module: ta_read.c
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
 *	Functions associated with reading entry.
 *
 */
#include <server.h>
#include <dbdef.h>
#include <find.h>
#include <names.h>
#include <security.h>

static bytes_u rpc_uuid[] = {0x7, 0x7, 0x6, 0x5, 0x2B, 0x16, 0x01, 0x01, 0x02};

DBSet_t *
net_get_rpc_towers (
    void *);

/*
 * Local prototypes
 */

static dns_status_t
readentry (
    db_data_t		*,
    db_stream_t		*,
    AttributeName_t	*,
    byte_t		*,
    int			,
    int			*);

static dns_status_t
read_chdirectory (
    data_record_t	*,
    db_stream_t		*,
    Timestamp_t		*,
    byte_t		*,
    int			,     
    int			*,
    int			*);


static DBSet_t *
build_rpcuuid (
    db_data_t		*);

/*
 * readentry
 *   This routine returns the entire entry to the user as an array
 * of attribute name, attribute contents.
 *
 * Input:
 *   rec_p = address of db data
 *   stream_p = address of db stream
 *   buf_p = return buffer
 *   max_size = length of buffer
 *
 * Value: length of inserted data
 */
static dns_status_t 
readentry (db_data_t        *rec_p,
           db_stream_t      *stream_p,
           AttributeName_t  *attribute_p,
           byte_t           *buf_p,
           int              max_size,
           int              *retlen_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_AttributeName(attributeBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    DBSet_t	*const	main_p 			 /* set of data  */
			    = (DBSet_t *)rec_p->data_p->dbr_data;
    WholeEntry_t *const wh_p = (WholeEntry_t *)buf_p;
    DBSet_t *objectuid_set_p = NULL;
    byte_t		*next_p = wh_p->wh_entry; /* ptr into buffer  */
    dns_status_t	status;
    const int		cnt = NUM_DBSet(main_p); /* # members in set  */
    int			numberOfAttributes = 0;
    int			pos;			 /* temp index  */
    int			found = 0;
#define FOUND_ATTRIBUTE		(1<<0)
#define FOUND_RPC_UUID		(1<<1)
#define FOUND_TOWERS		(1<<2)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >readentry(,%s,%s,,%d,%d)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_AttributeName(attributeBuf,attribute_p),
        max_size,
        *retlen_p));

    /* Point at set of sets */
    ALIGN_CHECK(main_p);

    if ((max_size -= sizeof(byte_t)) < 0)
	{
	status = DNS_NOROOM;
	goto leave_readentry;
	}

    for (pos = 0; pos < cnt; ++pos)
        {
	AttributeName_t *const name_p = 
			    (AttributeName_t *)INDEX_DBSet(main_p, pos);
	attribute_descriptor_t *const dns_p = find_known_attribute(name_p);
	DBSet_t *const set_p = (DBSet_t *)SKIP_AttributeName(name_p);

	ALIGN_CHECK(set_p);
	if (EXT8(set_p->ds_flag) != DS_present)
	    continue;

	/*
	 * Root out all attributes which require special handling
	 */
	if (dns_p) {
	    if ((dns_p->atype == DNS_OBJECTUID) && find_last_value(set_p))
		objectuid_set_p = set_p;
	    else if (dns_p->atype == DNA_TOWERS)
		found |= FOUND_TOWERS;
	    else if ((dns_p->atype == DNS_CHDIRECTORIES)
		&& (rec_p->data_type == ET_clearinghouse))
		continue;
	    if (!dns_p->visable)
		continue;
	}
	++numberOfAttributes;

	/* Check if this is the desired attribute */
	if (EQ_SimpleName(name_p, attribute_p)) found |= FOUND_ATTRIBUTE;
	if (EQ_SimpleName(name_p, rpc_uuid)) found |= FOUND_RPC_UUID;

	/* Copy name */
	if ((max_size -= LEN_AttributeName(name_p)) < 0)
	    {
	    status = DNS_NOROOM;
	    goto leave_readentry;
	    }
	COPY_AttributeName(name_p, next_p);
	next_p = SKIP_AttributeName(next_p);

	if (EXT8(set_p->ds_type) == AT_single)
	    {
	    const int numentries = NUM_DBSet(set_p);
	    DBSetMember_t *const value_p =
		    (DBSetMember_t *)INDEX_DBSet(set_p, numentries-1);
	    const int value_len = LEN_DBSetMemberData(value_p);
	    AttributeContents_t *const ac_p = (AttributeContents_t *)next_p;
	    MemberValue_t *const mv_p = (MemberValue_t *)ac_p->ac_single;

	    if ((max_size -= DIFF_bytes(&mv_p->mv_value[value_len], ac_p)) < 0)
		{
		status = DNS_NOROOM;
		goto leave_readentry;
		}
	    INS8(ac_p->ac_type, AT_single);

	    /* Build return value */
	    INS8(mv_p->mv_flag, MV_present);
	    COPY_Timestamp(value_p->dm_ts, mv_p->mv_timestamp);
	    INS8(mv_p->mv_valuetype, EXT8(value_p->dm_valuetype));
	    INS16(mv_p->mv_length, value_len);
	    COPY_bytes(value_p->dm_data, mv_p->mv_value, value_len);
	    next_p = SKIP_MemberValue(mv_p);
	    }
	else
	    {
	    AttributeContents_t *const ac_p = (AttributeContents_t *)next_p;
	    Set_t *const st_p = (Set_t *)ac_p->ac_set;

	    if ((max_size -= DIFF_bytes(st_p, ac_p)) < 0)
		{
		status = DNS_NOROOM;
		goto leave_readentry;
		}
	    INS8(ac_p->ac_type, AT_set);
	    /* Build return set. */
	    if (build_set(set_p, st_p, 0, max_size) == BL_false)
		{
		status = DNS_NOROOM;
		goto leave_readentry;
		}
	    max_size -= LEN_Set(st_p);
	    next_p = SKIP_Set(st_p);
	    }

	} /* End looping through set */

    /*
     * If attribute not found yet, assume DACL attributes wanted
     */
    if (!(found&FOUND_ATTRIBUTE)
	&& ((rec_p->data_type == ET_directory)
	    || (rec_p->data_type == ET_clearinghouse)))
	{
	/*
	 * If RPC_UUID not found, construct it.
	 */
	if (!(found&FOUND_RPC_UUID) && objectuid_set_p)
	    {
	    uuid_t uuid;
	    DBSetMember_t *const dm_p = find_last_value(objectuid_set_p);
	    AttributeName_t *const name_p = (AttributeName_t *)rpc_uuid;
	    AttributeContents_t *const ac_p = (AttributeContents_t *)
		    SKIP_bytes(next_p, LEN_AttributeName(name_p));
	    Set_t *const st_p = (Set_t *)ac_p->ac_set;
	    MemberValue_t *const mv_p = (MemberValue_t *)
		    SKIP_word(st_p->st_offsets);

	    COPY_bytes(dm_p->dm_data, &uuid, sizeof(uuid));
# if BYTE_ORDER == BIG_ENDIAN
	    uuid.time_low = _swapQ(uuid.time_low);
	    uuid.time_mid = _swapS(uuid.time_mid);
	    uuid.time_hi_and_version = _swapS(uuid.time_hi_and_version);
# endif

	    numberOfAttributes++;
	    max_size -= DIFF_bytes(&mv_p->mv_value[sizeof(uuid)], next_p);
	    if (max_size < 0)
		{
		status = DNS_NOROOM;
		goto leave_readentry;
		}
	    COPY_AttributeName(name_p, next_p);
	    INS8(ac_p->ac_type, AT_set);
	    /* Build return set. */
	    INS8(st_p->st_flag, ST_present);
	    INS16(st_p->st_offsetLength, sizeof(word_u));
	    INS16(st_p->st_offsets, DIFF_bytes(mv_p, st_p));
	    INS8(mv_p->mv_flag, MV_present);
	    COPY_Timestamp(dm_p->dm_ts, mv_p->mv_timestamp);
	    INS8(mv_p->mv_valuetype, VT_byte);
	    /* (this ordering helps optimization) */
	    INS16(mv_p->mv_length, sizeof(uuid));
	    next_p = SKIP_MemberValue(mv_p);
	    INS16(st_p->st_valueLength, DIFF_bytes(next_p, mv_p));
	    COPY_bytes(&uuid, mv_p->mv_value, sizeof(uuid));
	    /* (end optimization) */

	    if (EQ_SimpleName(attribute_p, name_p))
		found |= FOUND_ATTRIBUTE;
	    }

	/*
	 * If DNA_TOWERS not found, construct it.
	 */
	if (!(found&FOUND_TOWERS))
	    {
	    nsgbl_ns_t *const ns_p = stream_p->db_file_p->file_ch_p->ch_nameserver;
	    DBSet_t *const set_p = net_get_rpc_towers(ns_p->ns_ta_id);

	    if (set_p)
		{
		AttributeName_t *const name_p = dns_AttributeName(DNA_TOWERS);
		AttributeContents_t *const ac_p = (AttributeContents_t *)
			SKIP_bytes(next_p, LEN_AttributeName(name_p));
		Set_t *const st_p = (Set_t *)ac_p->ac_set;

		numberOfAttributes++;
		if ((max_size -= DIFF_bytes(st_p, next_p)) < 0)
		    {
		    status = DNS_NOROOM;
		    goto leave_readentry;
		    }
		COPY_AttributeName(name_p, next_p);
		INS8(ac_p->ac_type, AT_set);
		/* Build return set. */
		if (build_set(set_p, st_p, 0, max_size) == BL_false)
		    {
		    status = DNS_NOROOM;
		    goto leave_readentry;
		    }
		max_size -= LEN_Set(st_p);
		next_p = SKIP_Set(st_p);

		if (EQ_SimpleName(attribute_p, name_p))
		    found |= FOUND_ATTRIBUTE;
		}
	    }
	}

    if (!(found&FOUND_ATTRIBUTE))
	{
	status = DNS_UNKNOWNATTRIBUTE;
	goto leave_readentry;
	}

    if (255 < numberOfAttributes)
	{
	status = DNS_NOROOM;
	goto leave_readentry;
	}
    INS8(wh_p->wh_numberOfAttribtues, numberOfAttributes);
    *retlen_p += DIFF_bytes(next_p, buf_p);
    status = DNS_SUCCESS;

leave_readentry:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " <readentry(,%s,%s,,%d,%d) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_AttributeName(attributeBuf,attribute_p),
        max_size,
        *retlen_p,
        deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

/*
 * read_chdirectory
 *     This routine builds the DNS$CH_DIRECTORIES attribute set
 * by reading the actual directory records in this database.
 *
 * Input:
 *     data_p = address of record descriptor
 *     context_p = directory uid to start with
 *     buf_p = address of return buffer
 *     max_size = size of user buffer
 *     retlen_p = address to return total length 
 *     room_flag_p = address to return true if enough room
 * 
 * Returns:
 * 	DNS_SUCCESS 
 *	DNS_NONSRESOURCES on malloc failure
 */
static dns_status_t 
read_chdirectory (data_record_t  *data_p,
                  db_stream_t    *stream_p,
                  Timestamp_t    *context_p,
                  byte_t         *buf_p,
                  int            max_size,
                  int            *retlen_p,
                  int            *room_flag_p)
{
    AttributeContents_t	*const ac_p = 		 /* attribute data  */
			    (AttributeContents_t *)buf_p;
    Set_t		*const retset_p =	 /* set to be returned ptr  */
			    (Set_t *)ac_p->ac_set;
    db_stream_t		*dirstream_p;
    DBSet_t		*set_p, 		 /* DNS$PARENTPOINTER data  */
			*chset_p;		 /* DNS$CHDIRECTORIES data  */
    DBSetMember_t	*tmpchvalue_p;
    MemberValue_t	*cur_value_p;		 /* current value ptr  */
    char		*work_p;		 /* work buffer ptr  */
    int			cnt = 0, 		 /* # chdirectories  */
			pos, index;		 /* temp indices  */
    byte_t		wholeEntry = BL_true;	 /* set if have whole entry  */
    dns_status_t	status;			 /* current status  */

   /*
    * Locate first record to start with based on context.
    */  
    status = find_attribute(data_p,
		dns_AttributeName(DNS_CHDIRECTORIES), &chset_p, &index);
    if (status != DNS_SUCCESS)
	{
	status = DNS_UNKNOWNATTRIBUTE;
	goto leave_read_chdirectory;
	}

    /* Account for att type and set header in user buffer count*/    
    max_size -= sizeof(AttributeType_u) + sizeof(NULLSET_u) + sizeof(BOOLEAN_u);
    if (max_size <= 0)
	{
	status = DNS_NOROOM;
	goto leave_read_chdirectory;
	}

    /* Allocate enough memory so we can save names while we build
     * the offset positions in the return buffer.  
     * The number of entries is not known initally.
     */
    work_p = dns_malloc(max_size);
    if (!work_p)
	{
        status = DNS_NONSRESOURCES;
	goto leave_read_chdirectory;
	}

    /*
     * Get a stream for looking at the individual directories
     */
    status = db_get_stream(stream_p->db_file_p, &dirstream_p, OP_ReadAttribute);
    if (status != DNS_SUCCESS)
	goto free_work_and_leave_read_chdirectory;

    find_ts_position(chset_p, context_p, &tmpchvalue_p, &pos);
    cur_value_p = (MemberValue_t *)work_p;

    for (; pos < NUM_DBSet(chset_p); ++pos)
	{
	DBSetMember_t *const chvalue_p =
				(DBSetMember_t *)INDEX_DBSet(chset_p, pos);

	if (!EXT8(chvalue_p->dm_value_present) || 
	    !EXT8(chvalue_p->dm_valid))
	    {
	    continue;
	    }
	/* Read directory record by UID */
	dirstream_p->db_dir_hash_p = NULL;
	names_new((ObjUID_t *)chvalue_p->dm_data, NULL,
	    (FullName_t *)dirstream_p->db_key);
	status = db_dir_read(dirstream_p);
	if (status != DNS_SUCCESS) continue;
	if (db_dir_get_replica_state(dirstream_p->db_dir_hash_p) != RS_on)
	    {
	    db_dir_release(dirstream_p);
	    continue;
	    }
	/* Check to see if this is the root */
	if ((find_attribute(dirstream_p->db_dir.data_p,
		    dns_AttributeName(DNS_PARENTPOINTER),
		    &set_p, &index) != DNS_SUCCESS)
		|| dirstream_p->db_dir_hash_p->dir_root)
	    {
	    const int len = sizeof(MemberValue_u) - MAXATTRIBUTE +
		sizeof(CHDirectoryPointer_u) - sizeof(FullName_u) +
		LEN_FullName(&dirstream_p->db_file_p->file_ch_p->ch_cellname);

	    if (0 < (max_size -= sizeof(word_u) + len))
		{
		CHDirectoryPointer_t *const cp_p =
		    (CHDirectoryPointer_t *)cur_value_p->mv_value;

		INS8(cur_value_p->mv_flag, MV_present);
		COPY_Timestamp(chvalue_p->dm_ts, cur_value_p->mv_timestamp);
		COPY_ObjUID(dirstream_p->db_dir.data_uid, cp_p->cp_dirID);
		/* Uid of root is namespace uid */
		INS8(cur_value_p->mv_valuetype, VT_CHDirectory);
		COPY_FullName(&dirstream_p->db_file_p->file_ch_p->ch_cellname,
		    cp_p->cp_directory);
		INS16(cur_value_p->mv_length, LEN_CHDirectoryPointer(cp_p));
		cur_value_p = (MemberValue_t *)SKIP_MemberValue(cur_value_p);
		cnt++;
		}
	    else
		{
		wholeEntry = BL_false;
		db_dir_release(dirstream_p);
		break;
		}
	    } /* End if found parentpointer attribute */
	else /* Not root directory */
	    {
	    DBSetMember_t *const dm_p = find_last_value(set_p);

	    if (dm_p)
		{
		ParentPointer_t *const pp_p =
		    (ParentPointer_t *)dm_p->dm_data;
		const int len = sizeof(MemberValue_u) - MAXATTRIBUTE +
		    sizeof(CHDirectoryPointer_u) - sizeof(FullName_u) +
		    LEN_FullName(pp_p->pp_myName);

		if (0 < (max_size -= sizeof(word_u) + len))
		    {
		    CHDirectoryPointer_t *const cp_p =
			(CHDirectoryPointer_t *)cur_value_p->mv_value;

		    INS8(cur_value_p->mv_flag, MV_present);
		    COPY_Timestamp(chvalue_p->dm_ts,
			cur_value_p->mv_timestamp);
		    COPY_ObjUID(dirstream_p->db_dir.data_uid,
			cp_p->cp_dirID);
		    COPY_FullName(pp_p->pp_myName, cp_p->cp_directory);
		    INS8(cur_value_p->mv_valuetype, VT_CHDirectory);
		    INS16(cur_value_p->mv_length,
			LEN_CHDirectoryPointer(cp_p));
		    cur_value_p = (MemberValue_t *)
				SKIP_MemberValue(cur_value_p);
		    cnt++;
		    }
		else
		    {
		    if (cnt == 0)
			*room_flag_p = FALSE;
		    wholeEntry = BL_false;
		    db_dir_release(dirstream_p);
		    break;
		    }
		} /* End found parent pointer */
	    } /* End not root directory */
	db_dir_release(dirstream_p);
	} /* End for loop */

    /* The number of entries are known, so values can be copied
     * to the end of the offset array
     */
    INS8(ac_p->ac_type, AT_set);  /* attribute type */
    INS8(retset_p->st_flag, ST_present);
    INS16(retset_p->st_offsetLength, cnt*sizeof(word_u));

    if (cnt)
	{
	MemberValue_t *mv_p = (MemberValue_t *)DATA_Set(retset_p);
	const int valueLength = DIFF_bytes(cur_value_p, work_p);
	int i;

	COPY_bytes(work_p, mv_p, valueLength);

	/* Adjust offsets to start from beginning of set */
	for (i = 0; i < cnt; i++)
	    {
	    INS16(OFFSET_Set(retset_p, i), DIFF_bytes(mv_p, retset_p));

	    mv_p = (MemberValue_t *)SKIP_MemberValue(mv_p);
	    }

	INS16(retset_p->st_valueLength, valueLength);
	}
    else
	{
	INS16(retset_p->st_valueLength, 0);
	}

    INS8(SKIP_Set(retset_p), wholeEntry);
    /*
     * Update return count to include AttributeContents
     * and wholeEntry flag.
     */
    *retlen_p += LEN_AttributeContents(ac_p) + sizeof(BOOLEAN_u);
    status = DNS_SUCCESS;

    db_free_stream(dirstream_p);

free_work_and_leave_read_chdirectory:
    dns_free(work_p);

leave_read_chdirectory:
    return(status);
}

/*
 * build_rpcuuid
 *    Convert the CDS attribute DNS_OBJECTUID to look like an
 * RPC_ObjectUuid.  This attribute is an object uid stored in little
 * endian format.  
 * This call is needed so the dacl editor can bind to the directory object.
 *
 * Parameters:
 *	rec_p	(in)	- address of record descriptor
 *
 * Returns:
 * 	The output set is a in the for of a DBSet.
 */
static DBSet_t 
*build_rpcuuid (db_data_t *rec_p)
{
    dns_status_t  status;			 /* current status  */
    int           index;			 /* used by find_attribute  */
    DBSet_t       *set_p,			 /* work set ptr  */
		  *newset_p = NULL; 	 /* new set  */
    DBSetMember_t *val_p;			 /* current set member  */
    uuid_t        uuid;				 /* object uid  */

    status = find_attribute(rec_p->data_p, dns_AttributeName(DNS_OBJECTUID),
			    &set_p, &index);
    /* If it isn't their, return empty set */
    if (status != DNS_SUCCESS)
	return(newset_p);
    
    /* If their is a value, convert it into a set with little endian value */
    if (val_p = find_last_value(set_p)) {
	newset_p = (DBSet_t *)dns_malloc(sizeof(DBSet_t) + 
			    sizeof(DBSetMember_t) + val_p->dm_length);
	if (newset_p) {
	    COPY_bytes(val_p->dm_data, &uuid, sizeof(uuid_t));
	    /* RPC stores its uuids in little endian format */
# if BYTE_ORDER == BIG_ENDIAN
	    uuid.time_low = _swapQ(uuid.time_low);
	    uuid.time_mid = _swapS(uuid.time_mid);
	    uuid.time_hi_and_version = _swapS(uuid.time_hi_and_version);
#endif
	    INS8(newset_p->ds_flag, DS_present);
	    INS8(newset_p->ds_type, AT_set);
	    COPY_Timestamp(val_p->dm_ts, newset_p->ds_ts);
	    INS32(newset_p->ds_casemask, 0);
	    newset_p->ds_hcnt = 0;			/* no delete holders */
	    newset_p->ds_ocnt = 1;

	    /* Build value header */
	    val_p = (DBSetMember_t *)ALIGN_PTR(DATA_DBSet(newset_p));
	    newset_p->ds_offp[0] = DIFF_bytes(val_p, newset_p);
	    INS8(val_p->dm_flag, DM_present);
	    INS8(val_p->dm_valid, BL_true);
	    INS8(val_p->dm_type, AT_set);
	    INS8(val_p->dm_value_present, BL_true);
	    COPY_Timestamp(newset_p->ds_ts, val_p->dm_ts);
	    INS8(val_p->dm_valuetype, VT_byte);
	    val_p->dm_length = sizeof(uuid);
	    COPY_bytes(&uuid, val_p->dm_data, sizeof(uuid));
        }
    }
    return(newset_p);
}

/*
 * readone 
 *     This routine reads one attribute.
 *
 * Input:
 *      rec_p = address of record descriptor
 *	stream_p = address of db stream
 *      attribute_p = address of attribute to read
 *      context_p = address of timestamp of value to begin after
 *      buf_p = address to return output
 *      max_size = size of user buffer for value set
 *
 * Output:
 *      retlen_p = address to return length of output
 *      room_flag_p = address to return true if enough room
 *
 * Value:
 *	DNS_SUCCESS on successful read
 *	DNS_NOTSUPPORTED
 *	or status from read_skulkstatus or read_chdirectory
 *
 */
dns_status_t 
readone (db_data_t     *rec_p,
         db_stream_t   *stream_p,
         SimpleName_t  *attribute_p,
         Timestamp_t   *context_p,
         byte_t        *buf_p,
         int           max_size,
         int           *retlen_p,
         int           *room_flag_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_AttributeName(attributeBuf)
    DEB_ASCII_BUF_Timestamp(contextBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    AttributeContents_t	*ac_p 			 /* attribute data  */
			    = (AttributeContents_t *)buf_p;
    DBSet_t		*set_p;			 /* work set  */
    DBSetMember_t	*value_p;		 /* work value  */
    int			pos, index, 		 /* temporary indices  */
			numentries;		 /* # entries in set  */
    byte_t		wholeEntry;		 /* flag if have whole entry  */
    dns_status_t	status;			 /* current status  */
    attribute_descriptor_t *const dns_p =	 /* attribute data  */
			find_known_attribute((AttributeName_t *)attribute_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >readone(,%s,%s,%s,,%d,%d,)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_AttributeName(attributeBuf,attribute_p),
        deb_ascii_Timestamp(contextBuf,context_p),
        max_size,
        *retlen_p));

    *room_flag_p = TRUE;

    if (dns_p && !dns_p->visable)
	{
	status = DNS_NOTSUPPORTED;
	goto leave_readone;
	}

    /* Find attribute in record */
    if ((rec_p->data_type == ET_clearinghouse) && dns_p &&
       (dns_p->atype == DNS_CHDIRECTORIES))
	{
	status = read_chdirectory(rec_p->data_p, stream_p, context_p,
				buf_p, max_size, retlen_p, room_flag_p);
	goto leave_readone;
	}


    status = find_attribute(rec_p->data_p, (AttributeName_t *)attribute_p,
		&set_p, &index);
    
    /* Update return count to include type of attribute and
     * whole entry flag.
     */
    max_size -= sizeof(char);     /* Account for att type in user buf count */
    if (status != DNS_SUCCESS) 
	{			  /* Attribute absent or unknown */
	status = DNS_UNKNOWNATTRIBUTE;

	/* Fake DACL attributes for binding */
	if ((rec_p->data_type == ET_directory) ||
	    (rec_p->data_type == ET_clearinghouse)) 
	    {
	    /* turn objectuid into set */
	    if (dnsCmpSimple((dns_simple_name_t *)attribute_p,
			     (dns_simple_name_t *)rpc_uuid) == 0) 
		set_p = build_rpcuuid(rec_p);
	    else if (dns_p && (dns_p->atype == DNA_TOWERS))
		{
		set_p = net_get_rpc_towers(
		    stream_p->db_file_p->file_ch_p->ch_nameserver->ns_ta_id);
		}
	    else
		set_p = NULL;

	    if (set_p) 
		{
		/* Find where to begin returning values */
		find_ts_position(set_p, context_p, &value_p, &pos);
		wholeEntry = build_set(set_p,
				       (Set_t *)ac_p->ac_set, pos, max_size);
		INS8(ac_p->ac_type, AT_set);
		INS8(SKIP_Set(ac_p->ac_set), wholeEntry);
		if (!wholeEntry && (NUM_Set((Set_t *)ac_p->ac_set) == 0))
		    *room_flag_p = FALSE;
		if (dnsCmpSimple((dns_simple_name_t *)attribute_p,
				 (dns_simple_name_t *)rpc_uuid) == 0) 
		    dns_free((char *)set_p);
		status = DNS_SUCCESS;
		}
	    } /* End is a directory or clearinghose */
	} /* End find_attribute != success */
    else  
	{                         /* Build return packet */
	COPY_AttributeType(set_p->ds_type, ac_p->ac_type);
        if (EXT8(set_p->ds_type) == AT_set)
            {
            /* Find where to begin returning values */
            find_ts_position(set_p, context_p, &value_p, &pos);
            wholeEntry = build_set(set_p,(Set_t *)ac_p->ac_set, pos, max_size);
	    INS8(SKIP_Set(ac_p->ac_set), wholeEntry);
	    if (!wholeEntry && (NUM_Set((Set_t *)ac_p->ac_set) == 0))
		*room_flag_p = FALSE;
            }
        else
            {
            /* Build single value, ignore context and return only value */
	    MemberValue_t *member_p = (MemberValue_t *)ac_p->ac_single;

	    numentries = NUM_DBSet(set_p);
	    value_p = (DBSetMember_t *)INDEX_DBSet(set_p, numentries-1);

            if (max_size < (sizeof(MemberValue_u) - MAXATTRIBUTE +
		    LEN_DBSetMemberData(value_p)))
                { /* Only member won't fit */
                INS8(member_p->mv_flag, MV_present);
                INS16(member_p->mv_length, 0);
		INS8(member_p->mv_valuetype, VT_none);
		INS8(SKIP_SetMember(ac_p->ac_single), BL_false);
		*room_flag_p = FALSE;
                }
            else /* Return single value */
                {
                INS8(member_p->mv_flag, ST_present);
                COPY_Timestamp(value_p->dm_ts, member_p->mv_timestamp);
                INS16(member_p->mv_length, LEN_DBSetMemberData(value_p));
		INS8(member_p->mv_valuetype, EXT8(value_p->dm_valuetype));
                COPY_bytes(value_p->dm_data, member_p->mv_value,
		    EXT16(member_p->mv_length));
		INS8(SKIP_SetMember(ac_p->ac_single), BL_true);
                }
            } /* End if set type isn't AT_set */
        } /* End if set type is AT_set */
    *retlen_p += LEN_AttributeContents(ac_p) + sizeof(BOOLEAN_u);

leave_readone:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " <readone(,%s,%s,%s,,%d,%d,%d) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_AttributeName(attributeBuf,attribute_p),
        deb_ascii_Timestamp(contextBuf,context_p),
        max_size,
        *retlen_p,
        *room_flag_p,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * ta_read_att
 *   This routine tries to find an entry.  If the entry is found
 * the output data is build.  If the user requests the entire entry
 * and it will fit in max_size a wholeentry data structure is returned.
 * Otherwise either a set or single valued attribute is returned with a
 * flag indicating whether there is more data for this attribute.
 *
 * Input:
 *      user_p = address of user information
 *      progress_p = address of progress record.  This is also the
 *                   output buffer.  The progress record is modified
 *                   in place and whatever is left is used to return
 *                   output data.
 *      entry_type = {object, directory, subdirectory, clearinghouse, link}
 *      maybemore = flag indicating whether to try to fit entire entry in
 *      attribute_name_p = address of attribute to read
 *      context_p = address of timestamp of value to begin after
 *      max_size = size of user buffer for value set
 *	*retlen_p = size of buffer to receive output data
 *
 * Output:
 *      retlen_p = address to return length of output
 *                 This is valid only on success.  It contains the
 *                 length of the progress record, type of output
 *                 and either the whole entry structure or a (set/single)
 *                 data structure and the wholeentry flag.
 *
 * Function value:
 *	DNS_SUCCESS on successful completion
 *     	other dns error code otherwise
 */
dns_status_t 
ta_read_att (user_descriptor_t  *user_p,
             Progress_t         *progress_p,
             int                entry_type,
             unsigned int       maybemore,
             SimpleName_t       *attribute_p,
             Timestamp_t        *context_p,
             int                max_size,
             int                *retlen_p)
{
    DEB_ASCII_BUF_user_descriptor(userBuf)
    DEB_ASCII_BUF_Progress(progressBuf)
    DEB_ASCII_BUF_EntryType(entryTypeBuf)
    DEB_ASCII_BUF_AttributeName(attributeBuf)
    DEB_ASCII_BUF_Timestamp(contextBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    const byte_t	entryType = entry_type;
    db_stream_t		*stream_p;		 /* stream info pointer  */
    db_data_t		*rec_p;			 /* record pointer  */
    byte_t		*buf_p;			 /* ptr into progress record  */
    int			retlen = *retlen_p;	 /* buffer size  */
    int			chlist_len, 		 /* chlist bytes  */
			nochlist_len;		 /* bytes data without chlist */
    int			room_flag;		 /* set if enough room in buf */
    dns_status_t	status;			 /* current & exit status  */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >ta_read_att(%s,%s,%s,%d,%s,%s,%d,)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_Progress(progressBuf,progress_p),
        deb_ascii_EntryType(entryTypeBuf,&entryType),
        maybemore,
        deb_ascii_AttributeName(attributeBuf,attribute_p),
        deb_ascii_Timestamp(contextBuf,context_p),
        max_size));

    status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_ReadAttribute);
    if (status != DNS_SUCCESS)
	goto leave_ta_read_att;

    status = walktree(user_p, entry_type, WALKTREE_LOOKUP_OP, 
		      progress_p, stream_p);

    if (entry_type == ET_dirOrObj)
	if (FLAG_IS_SET(progress_p->pr_flags, PR_directory))
	    entry_type = ET_directory;
	else
	    entry_type = ET_object;

    if (status != DNS_SUCCESS) 
	goto leave_ta_read_att;
    status = security_check(user_p, stream_p, Read_check, entry_type, 
			    NULL);
    if (status == DNS_UNKNOWNENTRY)
	/* Rebuild resolved name for exception */
	resolved_null(progress_p);

    if (status == DNS_SUCCESS)
        {
	FullName_t *resolvedName_p = (FullName_t *)
				SKIP_FullName(progress_p->pr_unresolvedName);
	Set_t *CHList_p = (Set_t *)SKIP_FullName(resolvedName_p);


        if ((entry_type == ET_directory) || (entry_type == ET_clearinghouse))
	    rec_p = &stream_p->db_dir;
	else 
	    rec_p = &stream_p->db_entry;
	
        /* Determine real maximum of return set.  Include byte for
         * return_type.
         */
        *retlen_p = LEN_Progress(progress_p);  /* length of progress record */

        /*
	 * Calculate space left for user buffer 
         * Remove return type and all fits flags from user count
         */
        chlist_len = retlen - *retlen_p - sizeof(byte_u) - sizeof(BOOLEAN_u);
        /* Determine length without chlist */
        nochlist_len = chlist_len - LEN_Set(CHList_p) + sizeof(NULLSET_u);

        /* See if caller wants entire entry returned.
         * If the entry will fit, ignore the context
         */
        if (maybemore)
            {
	    buf_p = SKIP_Progress(progress_p);
	    *buf_p++ = RC_WHOLE_ENTRY;  /* Return type */
	    *retlen_p = DIFF_bytes(buf_p, progress_p);
	    status = readentry(rec_p, stream_p, (AttributeName_t *)attribute_p,
			       buf_p, max_size, retlen_p);
	    if (status != DNS_SUCCESS)
		maybemore = FALSE;

	    /*
	     * Trying w/o the chlist is not an option
	     * since we could not recover it for oneAttribute.
	     */
            } /* End if maybemore */

	if (!maybemore)
            {
            if (max_size < (sizeof(NULLSET_u) + sizeof(BOOLEAN_u)))
                status = DNS_NONSRESOURCES;
            else 
                {
                /* Only returning one attribute */
                buf_p = SKIP_Progress(progress_p);
                *buf_p++ = RC_ONE_ATTRIBUTE;
		*retlen_p = DIFF_bytes(buf_p, progress_p);
	        if (max_size > chlist_len)
                    status = readone(rec_p, stream_p, attribute_p, context_p,
			buf_p, chlist_len, retlen_p, &room_flag);
		else
                    status = readone(rec_p, stream_p, attribute_p, context_p,
			buf_p, max_size, retlen_p, &room_flag);
		if (!room_flag && (max_size > chlist_len))
		    {
		    if (max_size > nochlist_len) max_size = nochlist_len;
                    buf_p = SKIP_Progress(progress_p);
                    *buf_p++ = RC_ONE_ATTRIBUTE;
		    *retlen_p = DIFF_bytes(buf_p, progress_p);
                    status = readone(rec_p, stream_p, attribute_p, context_p, 
                                     buf_p, max_size, retlen_p, &room_flag);
		    }		
                }
            }
	    /*
	     * Earlier versions cannot handle DNS_UNKNOWNATTRIBUTE,
	     * so pass through DNS_SUCCESS with NullSet.
	     */
	    if (status != DNS_SUCCESS)
		{
		buf_p = SKIP_Progress(progress_p);
		*buf_p++ = RC_ONE_ATTRIBUTE;
                *buf_p++ = AT_set;
		COPY_bytes(&NullSet, buf_p, sizeof(NullSet));
		buf_p = SKIP_bytes(buf_p, sizeof(NullSet));
		*buf_p++ = TRUE;
		*retlen_p = DIFF_bytes(buf_p, progress_p);
		if (status == DNS_UNKNOWNATTRIBUTE)
		    status = DNS_SUCCESS;
		else if (status == DNS_NOROOM)
		    status = DNS_NAMESERVERBUG;
		}
        } /* if security check was successful */
    db_close_stream(stream_p);

leave_ta_read_att:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <ta_read_att(%s,%s,%s,%d,%s,%s,%d,%d) return(%s)",
        deb_ascii_user_descriptor(userBuf,user_p),
        deb_ascii_Progress(progressBuf,progress_p),
        deb_ascii_EntryType(entryTypeBuf,&entryType),
        maybemore,
        deb_ascii_AttributeName(attributeBuf,attribute_p),
        deb_ascii_Timestamp(contextBuf,context_p),
        max_size,
        *retlen_p,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * ta_read_replica
 *     This routine reads/replica attributes of a directory.
 * It allows a nameserver component to read attributes which are
 * not visable to other clients.
 *
 * Input:
 *     user_p = address of client descriptor
 *     directory_p = address of directory UDI
 *     attribute_p = address of attribute name
 *     context_p = address of context UID
 *     max_size = maximum length of return user buffer
 *	*retlen_p = size of buffer to receive output data
 *     
 * Output:
 *     buf_p = address to return set 
 *     retlen_p = address to return length of output
 *
 * Returns:
 *	DNS_SUCCESS on successful completion
 *      DNS_NONSRESOURCES
 *	DNS_NOTAREPLICA
 *      other dns error code otherwise
 */
dns_status_t 
ta_read_replica (user_descriptor_t  *user_p,
                 ObjUID_t           *directory_p,
                 SimpleName_t       *attribute_p,
                 Timestamp_t        *context_p,
                 int                max_size,
                 byte_t             *buf_p,
                 int                *retlen_p)
{
    db_stream_t		*stream_p;		 /* stream information  */
    DBSet_t		*set_p;			 /* parent pointer set  */
    DBSetMember_t	*val_p;			 /* parent pointer data  */
    int			retlen = *retlen_p;	 /* buffer size  */
    int			index;			 /* used for find_attribute  */
    int			room_flag;		 /* set if enough room in buf */
    dns_status_t	status;			 /* current & exit status  */

    status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_ReadAttribute);
    if (status != DNS_SUCCESS)
	goto leave_ta_read_replica;

    /* Build directory name */
    names_new(directory_p, NULL, (FullName_t *)stream_p->db_key);

    /* Read the directory */
    status = db_dir_read(stream_p);
    if (status == DNS_UNKNOWNENTRY)
        {
        status = db_pseudo_read(stream_p);
        if (status == DNS_SUCCESS)
            {
	    /* Fake type so get real error status back - special cases ch_rec*/
	    stream_p->db_dir.data_type = ET_directory;
#if defined(DCE_SEC)
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
        status = security_check(user_p, stream_p, Read_check, 
				ET_directory, NULL);
	if (status != DNS_SUCCESS)
            {
            if (find_attribute(stream_p->db_dir.data_p,
			dns_AttributeName(DNS_PARENTPOINTER),
			&set_p, &index) == DNS_SUCCESS)
                if (val_p = find_last_value(set_p))
		    {
		    ParentPointer_t *pp_p = (ParentPointer_t *)val_p->dm_data;

                    security_log(user_p, (FullName_t *)pp_p->pp_myName,
			stream_p->db_file_p->file_ch_p);
		    }
            }
        else
            {
            /* 
	     * Calculate space left in packet for user buffer.
             * Remove whole entry flag for user count
             */
            if (max_size > (retlen - sizeof(BOOLEAN_u)))
                max_size = (retlen - sizeof(BOOLEAN_u));
            if (max_size < (sizeof(NULLSET_u) + sizeof(BOOLEAN_u)))
                status = DNS_NONSRESOURCES;
            else
                {
                *retlen_p = 0;
                status = readone(&stream_p->db_dir, stream_p, attribute_p,
			context_p, buf_p, max_size, retlen_p, &room_flag);
                }
            }
	    /*
	     * Earlier versions cannot handle DNS_UNKNOWNATTRIBUTE,
	     * so pass through DNS_SUCCESS with NullSet.
	     */
	    if (status != DNS_SUCCESS)
		{
                *buf_p++ = AT_set;
		COPY_bytes(&NullSet, buf_p, sizeof(NULLSET_u));
		*retlen_p = sizeof(byte_u) + sizeof(NULLSET_u);
		if (status == DNS_UNKNOWNATTRIBUTE)
		    status = DNS_SUCCESS;
		else if (status == DNS_NOROOM)
		    status = DNS_NAMESERVERBUG;
		}
        } /* End if status == DNS_SUCCESS */

    db_close_stream(stream_p);

leave_ta_read_replica:
    return(status);
}
