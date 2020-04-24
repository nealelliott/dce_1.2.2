/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: back_parentpointer.c,v $
 * Revision 1.1.8.2  1996/02/18  19:35:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:45  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:28:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:25  root]
 * 
 * Revision 1.1.6.6  1994/09/07  20:02:09  mccann
 * 	fix version handling OT:12004
 * 	[1994/09/07  18:41:30  mccann]
 * 
 * Revision 1.1.6.5  1994/08/16  18:28:00  jd
 * 	drop from janet
 * 	[1994/08/13  16:07:11  jd]
 * 
 * Revision 1.1.6.4  1994/08/03  19:03:32  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:18:40  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:42:50  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:06  devsrc]
 * 
 * Revision 1.1.6.2  1994/03/22  20:00:18  griffin
 * 	HCell BL3 support: Expose routine so that other modules can get at it.
 * 	Allows parentpointer modification for cell name transformation.
 * 	[1994/03/22  18:01:22  griffin]
 * 
 * Revision 1.1.6.1  1994/03/12  22:09:54  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:37:58  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:19:10  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:19:12  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/30  19:35:11  peckham
 * 	OT#2114: uninitialized variables.
 * 	[1992/06/30  19:34:33  peckham]
 * 
 * Revision 1.1  1992/01/19  15:25:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: back_parentpointer.c
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
#include <events.h>
#include <find.h>
#include <names.h>
#include <sets.h>
#include <tlog.h>
#include <tower.h>
#include <uid.h>

/*
 * Local prototypes
 */
int
update_parentpointer (
    db_stream_t		*,
    ParentPointer_t	*,
    utc_t               *,
    int			);

/*
 * update_parentpointer
 *	This routine extends the timeout of a parentpointer and replaces
 *	the name.  Directory not read on input.
 *
 * Input:
 *	stream_p = address of stream descriptor, directory read
 *	newpp_p = buffer containing directory name.
 *      now_p = address of time to be used to update
 *             the parent pointer expiration 
 *	iamlinked = true if name contains softlinks, false otherwise
 *
 */
int 
update_parentpointer (db_stream_t      *stream_p,
                      ParentPointer_t  *newpp_p,
                      utc_t            *now_p,
                      int              iamlinked)
{
    tlog_dsc_t		tlog;
    bytes_u		upd_buf[2*(sizeof(Update_u) - MAXATTRIBUTE +
				sizeof(ParentPointer_u))];
    Timestamp_u		ts;
    ParentPointer_t	*pp_p;
    dir_hash_entry_t	*hash_p;
    DBSet_t		*set_p;
    DBSetMember_t	*value_p;
    SimpleName_t	*me_p;
    int			index;
    int			updlen;
    dns_status_t	status;

    /* Resolve links in directory name */
    hash_p = stream_p->db_dir_hash_p;
    if (iamlinked)
        {
	/* 
	 * Since names are hashed according to the directory name,
	 * we could end up never resolving this because if we come to this
	 * clearinghouse it will find this directory and claim it is
	 * not linked.  Instead we must lookup up our parent.  If everyone
	 * does this, the links along the way will get resolved and
 	 * propagated down to us 
	 */
	names_stripright((FullName_t *)hash_p->dir_name,
	    (FullName_t *)newpp_p->pp_myName);
	status = clerk_resolve_name((FullName_t *)newpp_p->pp_myName,
	    (FullName_t *)newpp_p->pp_myName);
        if (status != DNS_SUCCESS)
	{            
	    nsgbl_ns_t *ns_p = stream_p->db_file_p->file_ch_p->ch_nameserver;

            if (status == DNS_UNKNOWNENTRY)
	    {
	      evl_BrokenLookupPaths(ns_p, (FullName_t *)hash_p->dir_name);
	    }
            else 
		if (status == DNS_ACCESSDENIED)
		{
		  evl_SecurityFailure(ns_p, (FullName_t *)hash_p->dir_name);
		}

	    COPY_FullName(hash_p->dir_name, newpp_p->pp_myName);
	}         
	else
	    {
	    /* Need to append last simplename back */
	    me_p = (SimpleName_t *)
			names_pointlast((FullName_t *)hash_p->dir_name);
	    names_append(me_p, (FullName_t *)newpp_p->pp_myName);

	    /* Need to add new name to hash table */
	    db_hash_new_name(stream_p->db_file_p, hash_p,
		(FullName_t *)newpp_p->pp_myName);
	    }
        }
    else
	COPY_FullName(hash_p->dir_name, newpp_p->pp_myName);

    /* Access the directory for writting */
    db_dir_release(stream_p);
    status = db_dir_read_for_update(stream_p);
    if (status != DNS_SUCCESS) 
	return(status);

    if (find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(DNS_PARENTPOINTER),
		&set_p, &index) != DNS_SUCCESS) 
	{
	db_dir_release_lock(stream_p);
	return(DNS_SUCCESS);
	}

    value_p = find_last_value(set_p);
    if (value_p)
	{
	Timeout_t *timeout_p = (Timeout_t *)newpp_p->pp_timeout;

	pp_p = (ParentPointer_t *)value_p->dm_data;
	COPY_ObjUID(pp_p->pp_parentID, newpp_p->pp_parentID);
	COPY_Timeout(pp_p->pp_timeout, newpp_p->pp_timeout);

	/* Extent the timeout */
	ADD_utc((byte_t *)now_p, timeout_p->to_extension, 
		timeout_p->to_expiration);

	tlog.cnt = 0;
	tlog.len = 0;
	tlog.buf_p = upd_buf;

	if (ts_new((Timestamp_t *)ts) == DNS_SUCCESS)
	    {
	    ++tlog.cnt;
	    updlen = update_dns_build((Update_t *)&tlog.buf_p[tlog.len],
			UD_present, (Timestamp_t *)ts, 
			dns_Attribute(DNS_PARENTPOINTER), BL_true, 
			LEN_ParentPointer(newpp_p), (byte_t *)newpp_p);
	    (void)set_insert(&stream_p->db_dir, (Update_t *)&tlog.buf_p[tlog.len],
			dns_Attribute(DNS_PARENTPOINTER), TRUE);
	    tlog.len += updlen;
            /* Update timestamps */
            db_dir_set_lastupdate(stream_p->db_dir_hash_p,
			(Timestamp_t *)((Update_t *)upd_buf)->ud_timestamp);
            (void)set_timestamp_set(&stream_p->db_dir, 
			(Timestamp_t *)((Update_t *)upd_buf)->ud_timestamp,
			dns_Attribute(DNS_UTS), NULL, NULL);
	    (void)db_dir_update(stream_p, &tlog);
	    }
        }

    db_dir_release_lock(stream_p);
    return(DNS_SUCCESS);
}

                            
/* 
 * back_parentpointer_check
 *	This routine compares a directories replica set with its child
 * 	pointer set and updates the child.  The directory is read but
 *	not locked on input.
 *
 * Input:
 *     stream_p = pointer to stream descriptor
 *     now_p = address of time to be used to update
 *             the parent pointer expiration 
 *
 */
dns_status_t 
back_parentpointer_check (db_stream_t  *stream_p,
                          utc_t        *now_p)
{
    bytes_u		update[sizeof(Update_u) - MAXATTRIBUTE +
				sizeof(ReplicaPointer_u)];
    struct buf {
	bytes_u		retbuf[READ_BUF_SIZE];
	bytes_u		tower_copy[READ_BUF_SIZE];
    } *buf_p;
    dir_hash_entry_t	*hash_p = stream_p->db_dir_hash_p;
    Set_t		*retset_p;
    AtomicValue_t	*av_p;
    DBSet_t		*set_p;
    DBSetMember_t	*value_p;
    ParentPointer_t	*newpp_p;
    ReplicaPointer_t	*rp1_p, *rp2_p;
    byte_t		iamlinked;
    int			cnt, pos, index;
    int                 prev_func;               /* old stream access type */
    dns_status_t	status;

    /* If this is the root, no child to update */
    if (hash_p->dir_root)
	return(DNS_SUCCESS);
                  
    /* Get local directories replica set and compare it with the childs */
    status = find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(DNS_REPLICAS), &set_p, &index);
    if (status != DNS_SUCCESS)
    {
	evl_AttributeMissing(stream_p->db_file_p->file_ch_p,
			     (FullName_t *)hash_p->dir_name, 
			     dns_AttributeName(DNS_REPLICAS));

        return(DNS_NAMESERVERBUG);
    }               

    /* Build update structure */
    av_p = (AtomicValue_t *)SKIP_bytes(update, 
                update_dns_build((Update_t *)update, UD_absent, NullTimestamp, 
                dns_Attribute(DNS_REPLICAS), BL_true, 0, NULL));

    /* Allocate space for return set, update packet and possible
     * resolved name, also an extra space for comparing towers
     */
    buf_p = (struct buf *)dns_malloc(sizeof(*buf_p));
    if (!buf_p)
        return(DNS_NONSRESOURCES);

    retset_p = (Set_t *)SKIP_AttributeType(buf_p->retbuf);

    /*
     * Change our database access to read-only for the duration of
     * the check.   This allows checkpoints to occur and prevents deadlocks
     * if this code calls modifyattribute while a checkpoint
     * is closing out writers.
     */

    status = db_modify_stream_function(stream_p, OP_ReadAttribute, &prev_func);

    status = clerk_read_attribute((FullName_t *)hash_p->dir_name,
		ET_childPointer,
		dns_AttributeName(DNS_REPLICAS),
		buf_p->retbuf, NullTimestamp, &iamlinked);
    
    /* Assume if we can read it once, we will continue to */
    while ((status == DNS_SUCCESS) || (status == DNS_MOREDATA))
	{
	MemberValue_t *last_p = NULL;
	const dns_status_t retstatus = status;

       /* For each returned member, make sure it is also a member of the 
	* directories replica set 
	*/

	cnt = NUM_Set(retset_p);
	for (pos = 0; pos < cnt; ++pos)
	    {
	    MemberValue_t *const member_p =
			(MemberValue_t *)INDEX_Set(retset_p, pos);

            for (index = 0; ; index++)
            	{
		value_p = set_valuetest(set_p, EXT16(member_p->mv_length),
                     member_p->mv_value, EXT8(member_p->mv_valuetype), &index, 
		     dns_Attribute(DNS_REPLICAS));
		if (!value_p) break;
		if (!EXT8(value_p->dm_valid)) continue;
		/* Make sure address are the same,  Set test doesn't do this */
		rp1_p = (ReplicaPointer_t *)value_p->dm_data;
		rp2_p = (ReplicaPointer_t *)member_p->mv_value;
		/* tower comparison is desctructive to second set: copy */
		/* if tower_copy is too small, nameserver bug...*/
		if (READ_BUF_SIZE < LEN_Set(rp2_p->rp_addressHint))
		    {
		    /* log problem description ?????
		     * and return nameserver bug
		     */
		    status = DNS_NAMESERVERBUG;
		    break; 
		    }
		COPY_Set(rp2_p->rp_addressHint, buf_p->tower_copy);
		if (towerset_compare_exact((Set_t *)rp1_p->rp_addressHint,
					   (Set_t *)buf_p->tower_copy))
		    {
		    /* If a value is in both sets, ignore it */
		    INS8(value_p->dm_valid, FALSE);
		    INS8(member_p->mv_flag, MVX_absent);
		    break;
		    }
            	}
       	    }
       	    
        if (status != DNS_SUCCESS) break;

	/* For each returned member which is still PRESENT, remove it from
	 * the child because it was not found in the directory
 	 */
	cnt = NUM_Set(retset_p);
	for (pos = 0; pos < cnt; ++pos)
            {
            last_p = (MemberValue_t *)INDEX_Set(retset_p, pos);
            if (EXT8(last_p->mv_flag) != MV_present) continue;
	    COPY_AtomicValue(last_p->mv_length, av_p);

            status = clerk_modify_attribute((FullName_t *)hash_p->dir_name, 
		ET_childPointer, (Update_t *)update, LEN_Update(update));
            if (status != DNS_SUCCESS) break;
            }

        if (status != DNS_SUCCESS) break;
	if (retstatus == DNS_SUCCESS) break;
	if (!last_p) break;

        /* Set up item list, first 2 items are already set up */
	status = clerk_read_attribute((FullName_t *)hash_p->dir_name,
		ET_childPointer,
		dns_AttributeName(DNS_REPLICAS), buf_p->retbuf,
		(Timestamp_t *)last_p->mv_timestamp, NULL);
      }

    if (status != DNS_SUCCESS)
    {
	nsgbl_ns_t *ns_p = stream_p->db_file_p->file_ch_p->ch_nameserver;

	dns_free((char *)buf_p);
	if (status == DNS_UNKNOWNENTRY)
	{
	    evl_BrokenLookupPaths(ns_p, (FullName_t *)hash_p->dir_name);
	}
        else
	    if (status == DNS_ACCESSDENIED)
	    {
	      evl_SecurityFailure(ns_p, (FullName_t *)hash_p->dir_name);
	    }
	

	/*
	 * Revert to the original stream access mode.
	 */

	(void)db_modify_stream_function(stream_p, prev_func, &prev_func);

        return(status);
      }         


    /* For each member which is still PRESENT in the directory set add it to
     * the child because it was not returned from read_att.
     */
    INS8(((Update_t *)update)->ud_operation, UD_present);
    cnt = NUM_DBSet(set_p);
    for (pos = 0; pos < cnt; ++pos)
        {
        value_p = (DBSetMember_t *)INDEX_DBSet(set_p, pos);
        if (!EXT8(value_p->dm_valid)) continue;
        INS16(av_p->av_length, LEN_DBSetMemberData(value_p));
        INS8(av_p->av_valuetype, EXT8(value_p->dm_valuetype));
        COPY_bytes(value_p->dm_data, av_p->av_value, EXT16(av_p->av_length));

        (void)clerk_modify_attribute((FullName_t *)hash_p->dir_name,
		ET_childPointer, (Update_t *)update, LEN_Update(update));
        }
          
    /*
     * Modify phase is done.  Revert to the original stream access mode.
     */

    (void)db_modify_stream_function(stream_p, prev_func, &prev_func);

    /* Resolve directory name if linked */
    newpp_p = (ParentPointer_t *)buf_p->retbuf;
    status = update_parentpointer(stream_p, newpp_p, now_p, iamlinked);

    dns_free((char *)buf_p);
    return(status);
}
