/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta_subs.c,v $
 * Revision 1.1.6.2  1996/02/18  19:37:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:54  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:33:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:26  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:05:03  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:32:26  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:44:14  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:52  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:13:40  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:48:20  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:25:03  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:25:56  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:25:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: ta_subs.c
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

#include <math.h>

#include <server.h>
#include <dbdef.h>
#include <find.h>
#include <names.h>
#include <sets.h>
#include <tlog.h>
#include <uid.h>
#include <back.h>

/* Module-level control variable for randomizing clearinghouse lists */
static int no_random = 0;

/* Module-level control variable for enforcing update operation protocol */
static int strict_update_protocol = 0;




/* -------------------------------------------------------------------------- */

/* 
 * ta_diag_set_random
 *	Diagnostic function which takes in value and uses it to set no_random
 *	variable which determines if clearinghouse list is randomized or not.
 *
 */
int 
ta_diag_set_random (int value)
{
    int prev_value;

    prev_value = no_random;
    no_random = value;
    return (prev_value);
}


/* -------------------------------------------------------------------------- */

/* 
 * ta_diag_set_strict_upd_prot
 *	Diagnostic function which takes in value and uses it to set the
 *	strict_update_protocol variable which determines if applyupdate will
 *      tolerate DNS V1-style update operation codes.
 */
int 
ta_diag_set_strict_upd_prot (int value)
{
    int prev_value;

    prev_value = strict_update_protocol;
    strict_update_protocol = value;
    return (prev_value);
}



/* -------------------------------------------------------------------------- */

/*
 * resolved_null
 *	Set the resolved name in the progress record to NULL
 *	for an unknownentry return variable.  Don't worry about
 *	the rest of the progress record.
 *
 *      as other routines for version processing WILL try to use the
 *      progress record, and special handling would be complex, add an
 *      empty set after the null resolved name for the chlist - which is the
 *      last thing in a progress record.
 *
 * Input:
 *	Progress_p = address of progress record
 */
void 
resolved_null (Progress_t *progress_p)
{
    FullName_t		*resolved_p;
    
    resolved_p = (FullName_t *)SKIP_FullName(progress_p->pr_unresolvedName);
    COPY_FullName(NullFullName, resolved_p);
    COPY_Set(NullSet, SKIP_FullName(resolved_p));
}


/* -------------------------------------------------------------------------- */


/*                                                     
 * build_set
 *   This routine builds a set for over the wire from an internal attribute set.
 *   It copies all present values starting at a user supplied index into the
 *   new set.
 *
 *
 * Input:
 *     in_set_p = address of attribute set
 *     out_set_p = address to build new set
 *     off = entry number to start with, zero based
 *     max_size = size of set
 *
 *
 * Value:
 *  True if whole set fit, false otherwise
 *
 */
int 
build_set (DBSet_t  *in_set_p,
           Set_t    *out_set_p,
           int      off,
           int      max_size)
{
    DBSetMember_t	*value_p;
    MemberValue_t	*out_value_p;                
    byte_t		whole_set;
    int			cnt, pos, num_entries;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >build_set"));

    /* Build return set header */
    COPY_byte(in_set_p->ds_flag, out_set_p->st_flag);
    INS16(out_set_p->st_offsetLength, 0);
    INS16(out_set_p->st_valueLength, 0);

    /* If sets is absent, all done */
    if (EXT8(out_set_p->st_flag) != ST_present)
        return(BL_true);

    /* If no members to put in set, all done */
    num_entries = NUM_DBSet(in_set_p);
    if (num_entries <= off)
        return(BL_true);		/* empty-set built */

    /* space left for members */
    max_size -= DIFF_bytes(out_set_p->st_offsets, out_set_p);
    
    whole_set = BL_true;		/* assume success */
    pos = off;				/* init. beginning location */

   /*
    * Determine how many members will be in the final set.
    * Need this information to start building set
    */
    cnt = 0;
    for (; pos < num_entries; pos++)
        {
	/* Get pointer to next member */
	value_p = (DBSetMember_t *)INDEX_DBSet(in_set_p, pos);

        if (EXT8(value_p->dm_value_present) && EXT8(value_p->dm_valid))
            {
            max_size -= sizeof(word_u) + sizeof(MemberValue_u)
		- MAXATTRIBUTE + LEN_DBSetMemberData(value_p);
	    if (max_size < 0)
                {				 /* Couldn't fit all members */
                whole_set = BL_false;		 /* in new copy.  */
                break;
		}
	    ++cnt;				 /* Keep count.  */
            }
	}

    /* If anything was found, fill in members set */
    if (0 < cnt)
        {
        /* 
         * External sets do not have the extra offset position that
         * internal attribute sets have
         */
        INS16(out_set_p->st_offsetLength, cnt*sizeof(word_u));
        out_value_p = (MemberValue_t *)DATA_Set(out_set_p);
        pos = 0;
       /*
        * Now build the set since we have the number of members
        * External offsets are relative to the beginning of the set 
        */
        for (; 0 < cnt; off++)
            {
	    value_p = (DBSetMember_t *)INDEX_DBSet(in_set_p, off);

            if (EXT8(value_p->dm_value_present) && EXT8(value_p->dm_valid))
                {
		int length = LEN_DBSetMemberData(value_p);

		INS16(OFFSET_Set(out_set_p, pos),
		    DIFF_bytes(out_value_p, out_set_p));
                INS8(out_value_p->mv_flag, MV_present);
                COPY_Timestamp(value_p->dm_ts, out_value_p->mv_timestamp);
		INS8(out_value_p->mv_valuetype, EXT8(value_p->dm_valuetype));
                INS16(out_value_p->mv_length, length);
                COPY_bytes(value_p->dm_data, out_value_p->mv_value, length);
                out_value_p = (MemberValue_t *)SKIP_MemberValue(out_value_p);
                pos++;
                --cnt;
                }
            }
	INS16(out_set_p->st_valueLength,
	    DIFF_bytes(out_value_p, DATA_Set(out_set_p)));
        }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <build_set(%d)",
        whole_set));

    return(whole_set);
}


/* -------------------------------------------------------------------------- */


/*                                                     
 * build_random_set
 *   This routine builds a set for over the wire from an internal attribute set.
 *   It copies all present values starting at a user supplied index into the 
 *   new set in a random order.
 *
 *   It also does some value-munging, in that it only passes the ReplicaPointer
 *   part of gdaPointers.
 *
 * Input:
 *   in_set_p = address of attribute set
 *   out_set_p = address to build new set
 *   off = entry number to start with, zero based
 *   max_size = size of set
 *
 *
 * Value:
 *   True if whole set fit, false otherwise
 *
 */
int 
build_random_set (DBSet_t  *in_set_p,
                  Set_t    *out_set_p,
                  int      off,
                  int      max_size)
{
    MemberValue_t	*out_value_p;                
    byte_t		whole_set;
    int			cnt, inpos, outpos, num_entries;
    int			*workbuf_p = 0;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " >build_random_set(,,%d,%d)",
        off,
        max_size));

    /* Build return set header */
    COPY_byte(in_set_p->ds_flag, out_set_p->st_flag);
    INS16(out_set_p->st_offsetLength, 0);
    INS16(out_set_p->st_valueLength, 0);

    /* If sets are absent, nothing to copy, all done */
    if (EXT8(out_set_p->st_flag) != ST_present)
        return(BL_true);

    /* If no members to put in set, nothing to copy, all done */
    num_entries = NUM_DBSet(in_set_p);
    if (num_entries <= off)
        return(BL_true);		/* empty-set built */

    if (!no_random)
	{
	/*
	 * Here generate a random order of the input set.
	 */
	const int multiple = 48;
	const int workbuf_bytes = 	/* Get a 'multiple'-sized buffer */
	    (num_entries * sizeof(*workbuf_p) / multiple + 1) * multiple;
	const int work_range = num_entries - off;
	int workndx;

	workbuf_p = (int *)dns_get_buffer( ta_random_bt, workbuf_bytes);
	for (workndx = off; workndx < num_entries; workndx++)
	    workbuf_p[workndx] = workndx;

	for (workndx = off; workndx < num_entries; workndx++)
	    {
	    const int random = off + (rand() % work_range);
	    const int temp = workbuf_p[random];

	    workbuf_p[random] = workbuf_p[workndx];
	    workbuf_p[workndx] = temp;
	    }
	}

    whole_set = BL_true;	/* Assume complete list can be copied */
    max_size -= 		/* Get space left for members */
	DIFF_bytes(out_set_p->st_offsets, out_set_p);

   /*
    * Determine how many members will be in the final set.
    * Need this information to start building set
    */
    cnt = 0;
    for (inpos = off; inpos < num_entries; inpos++)
        {
	const int work_pos = workbuf_p ? workbuf_p[inpos] : inpos;
	const DBSetMember_t *const value_p =
			(DBSetMember_t *)INDEX_DBSet(in_set_p, work_pos);

        if (!(EXT8(value_p->dm_value_present) && EXT8(value_p->dm_valid)))
	    continue;

	max_size -= sizeof(word_u) + sizeof(MemberValue_u) - MAXATTRIBUTE;
	if (EXT8(value_p->dm_valuetype) == VT_gdaPointer) {
	    const gdaPointer_t *const gp_p = (gdaPointer_t *)value_p->dm_data;

	    max_size -= LEN_ReplicaPointer(gp_p->gp_replica);
	} else
	    max_size -= LEN_DBSetMemberData(value_p);

	if (max_size < 0)
	    {				 /* Couldn't fit all members */
	    whole_set = BL_false;	 /* in output copy.  */
	    break;
	    }
	cnt++;				 /* Keep count of to be copied*/
	}

    /* Found no valid set members, just return */
    if (cnt <= 0)
	{
	if (workbuf_p)
	    dns_free_buffer((char *)workbuf_p);

      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug5,
          " <build_random_set(,,%d,%d) return(%d), no members copied.",
          off,
          max_size,
          whole_set));
	return(whole_set);
	}

    INS16(out_set_p->st_offsetLength, cnt*sizeof(word_u));
    out_value_p = (MemberValue_t *)DATA_Set(out_set_p);

    /*
     * Now build the set since we have the number of members
     * External offsets are relative to the beginning of the set 
     */
    outpos = 0;
    for (inpos = off; inpos < num_entries; inpos++)
	{					 /* Begin copying members */
	/* Point to next potential input member */
	const int work_pos = workbuf_p ? workbuf_p[inpos] : inpos;
	const DBSetMember_t *const value_p =
			(DBSetMember_t *)INDEX_DBSet(in_set_p, work_pos);

	if (!(EXT8(value_p->dm_value_present) && EXT8(value_p->dm_valid)))
	    continue;

	/*
	 * Here have valid and present data.
	 */
	INS16(OFFSET_Set(out_set_p, outpos),
		DIFF_bytes(out_value_p, out_set_p));
	INS8(out_value_p->mv_flag, MV_present);
	COPY_Timestamp(value_p->dm_ts, out_value_p->mv_timestamp);
	if (EXT8(value_p->dm_valuetype) == VT_gdaPointer) {
	    const gdaPointer_t *const gp_p = (gdaPointer_t *)value_p->dm_data;
	    const int no_ovhd_len = LEN_ReplicaPointer(gp_p->gp_replica);

	    INS8(out_value_p->mv_valuetype, VT_ReplicaPointer);
	    INS16(out_value_p->mv_length, no_ovhd_len);
	    COPY_bytes(gp_p->gp_replica, out_value_p->mv_value, no_ovhd_len);
	} else {
	    const int no_ovhd_len = LEN_DBSetMemberData(value_p);

	    COPY_byte(value_p->dm_valuetype, out_value_p->mv_valuetype);
	    INS16(out_value_p->mv_length, no_ovhd_len);
	    COPY_bytes(value_p->dm_data, out_value_p->mv_value, no_ovhd_len);
	}
	out_value_p = (MemberValue_t *)SKIP_MemberValue(out_value_p);
	outpos++;				/* Count output number  */
	if (--cnt <= 0)
	    break;				/* should not happen */
	}					/* End copying members  */
    INS16(out_set_p->st_valueLength,
	DIFF_bytes(out_value_p, DATA_Set(out_set_p)));

    if (workbuf_p)
	dns_free_buffer((char *)workbuf_p);

    if (cnt)					 /* Had some problem  */
	{
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_server,
          svc_c_debug5,
          " <build_random_set(,,%d,%d) return(%d)",
          off,
          max_size,
          BL_false));
	return (BL_false);			 /* not clear copied full set */
	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug5,
        " <build_random_set(,,%d,%d) return(%d)",
        off,
        max_size,
        whole_set));
    return(whole_set);
}


/* -------------------------------------------------------------------------- */

/*
 * ts_in_range
 *     This routine makes sure a proposed uid is not less than
 * allupto or lastskulk.
 *
 * Input:
 *     dir_p = address of directory
 *     cts_p = cts to check
 *
 * Value:
 *     True if in range
 *     False otherwise
 */
int 
ts_in_range (data_record_t  *dir_p,
             Timestamp_t    *cts_p)
{
    DBSet_t		*set_p;
    DBSetMember_t	*member_p;
    int			index;

    if (!ts_future_skew(cts_p))
        return(FALSE);

    if (find_attribute(dir_p,
		dns_AttributeName(DNS_ALLUPTO), &set_p, &index) != DNS_SUCCESS)
        return(FALSE);

    member_p = (DBSetMember_t *)DATA_DBSet(set_p);

    if (dnsCmpCTS((dns_cts *)cts_p, (dns_cts_t *)member_p->dm_data) < 0)
        return(FALSE);

    if (find_attribute(dir_p,
	    dns_AttributeName(DNS_LASTSKULK), &set_p, &index) != DNS_SUCCESS)
        return(FALSE);

    member_p = (DBSetMember_t *)DATA_DBSet(set_p);
    return (0 <= dnsCmpCTS((dns_cts *)cts_p, (dns_cts *)member_p->dm_data));
}


/* -------------------------------------------------------------------------- */


/*
 * progress_size
 *     This routine determines the size of a progress record after
 * a successful walktree.  
 *
 * Input:
 *     progress_p = address of progress record
 *
 * Value:
 *     size of record.
 */
int 
progress_size (Progress_t *progress_p)
{
    return(LEN_Progress(progress_p));
}


/* -------------------------------------------------------------------------- */


/*
 * progress_make_room
 *     This routine removes the chlist from the return packet so there
 * is room to return information to the client.
 *
 * Input:
 *     progress_p = address of progress record
 *
 * Value:
 *     size of record.
 */
int 
progress_make_room (Progress_t *progress_p)
{
    FullName_t		*resolved_p;
    Set_t		*chlist_p;

    /* We made it through walktree so the unresolved name is nullname */
    resolved_p = (FullName_t *)SKIP_FullName(progress_p->pr_unresolvedName);

    /* the chlist will be an empty set */

    /* Free up replica pointers */
    chlist_p = (Set_t *)SKIP_FullName(progress_p);
    COPY_Set(NullSet, chlist_p);

    return(LEN_Progress(progress_p));
}


/* -------------------------------------------------------------------------- */


/*
 * low_convergence
 *	This routine finds the dns$convergence attribute of
 * a directory and tests is value against low.
 *
 * Input:
 *	dir_p = address of directory record
 *
 * Value:
 *	TRUE if convergence is low
 *	FALSE otherwise
 */
int 
low_convergence (data_record_t *dir_p)
{
    DBSet_t		*set_p;
    DBSetMember_t	*value_p;
    int			index;

    if (find_attribute(dir_p,
	    dns_AttributeName(DNS_CONVERGENCE), &set_p,  &index) == DNS_SUCCESS)
        {
        value_p = find_last_value(set_p);
        if (value_p)
            if (EXT8(value_p->dm_data) == CG_low)
		return(TRUE);
	}
    return(FALSE);
}


/* -------------------------------------------------------------------------- */


/*
 * applyupdate
 *     This routine is called to do an update and invoke the background
 * process to propagate the change.  If applicable the parent Last$update
 * attribute is also update.
 *
 * Input:
 *     stream_p = pointer to stream descriptor
 *     type = type of record to update
 *     update_p = update data
 *     dns_p = address of attribute in internal list of known attributes
 *     prop_flag = TRUE or FALSE, determines whether propagtion is notified.
 *                  and uts-lastupdate adjusted.
 *     tlog_p = address of transaction description for journalling
 *
 * Value:
 *     status 
 */
dns_status_t 
applyupdate (db_stream_t             *stream_p,
             int                     type,
             Update_t                *update_p,
             attribute_descriptor_t  *dns_p,
             int                     prop_flag,
             tlog_dsc_t              *tlog_p)
{
    dns_status_t	status;

    if (type == ET_clearinghouse)
	{
        status = set_insert(&stream_p->db_dir, update_p, dns_p, FALSE);
	if (status != DNS_SUCCESS) return(status);
	set_timestamp_set(&stream_p->db_dir,
		(Timestamp_t *)update_p->ud_timestamp, 
		dns_Attribute(DNS_UTS), NULL, NULL);
	return(db_pseudo_update(stream_p, tlog_p));
	}
    if (type == ET_directory)
	{
        status = set_insert(&stream_p->db_dir, update_p, dns_p, FALSE);
	if (status != DNS_SUCCESS) return(status);
        if (!prop_flag) return(db_dir_update(stream_p, tlog_p));
	set_timestamp_set(&stream_p->db_dir,
		(Timestamp_t *)update_p->ud_timestamp, 
		dns_Attribute(DNS_UTS), NULL, NULL);
	db_dir_set_lastupdate(stream_p->db_dir_hash_p,
		(Timestamp_t *)update_p->ud_timestamp);
	status = db_dir_update(stream_p, tlog_p);
	if (status == DNS_SUCCESS)
	    {
            if (!low_convergence(stream_p->db_dir.data_p))
	        back_propagate_sched(stream_p, ET_directory,
			(Timestamp_t *)update_p->ud_timestamp);
	    }
	return(status);
	}
    else
	{
        status = set_insert(&stream_p->db_entry, update_p, dns_p, FALSE);
	if (status != DNS_SUCCESS) return(status);
        if (!prop_flag) return(db_entry_update(stream_p, tlog_p));
	set_timestamp_set(&stream_p->db_entry,
		(Timestamp_t *)update_p->ud_timestamp, 
		dns_Attribute(DNS_UTS), NULL, NULL);
	db_dir_set_lastupdate(stream_p->db_dir_hash_p,
		(Timestamp_t *)update_p->ud_timestamp);
	status = db_entry_update(stream_p, tlog_p);
	if (status != DNS_SUCCESS) return(status);
	if (!low_convergence(stream_p->db_dir.data_p))
	    back_propagate_sched(stream_p, type,
		(Timestamp_t *)update_p->ud_timestamp);
	return(status);
	}
}


/* -------------------------------------------------------------------------- */


/*
 * Compare 2 grouprevoke timeouts and return the one which will timeout
 * first.  Routine treats a value of all zeros as infinite.
 */
Timeout_t 
*min_timeout (Timeout_t  *t1_p,
              Timeout_t  *t2_p)
{
    utc_t now,t1_new,t2_new;
    utc_gettime(&now);
	    
    /* 
     * All zeros => value not supplied.  Since this is not a valid utc,
     * must be byte compares here.
     */
    if (EQ_bytes(NullTimeout, t1_p, sizeof(NullTimeout))) {
        return(t2_p);
    }

    if (EQ_bytes(NullTimeout, t2_p, sizeof(NullTimeout))) {
        return(t1_p);
    }

    /* Find Next absolute timeout for t1 */
    if (EQ_bytes(Nullutc, t1_p->to_expiration, LEN_utc(Nullutc))) {
        ADD_utc((byte_t *)&now, t1_p->to_extension, (byte_t *)&t1_new);
    } else if (LT_utc(t1_p->to_expiration, (byte_t *)&now)) {
        ADD_utc((byte_t *)&now, t1_p->to_extension, (byte_t *)&t1_new);
    } else {
        COPY_utc(t1_p->to_expiration, (byte_t *)&t1_new);
    }

    /* Find Next absolute timeout for t2 */
    if (EQ_bytes(Nullutc, t2_p->to_expiration, LEN_utc(Nullutc))) {
        ADD_utc((byte_t *)&now, t2_p->to_extension, (byte_t *)&t2_new);
    } else if (LT_utc(t2_p->to_expiration, (byte_t *)&now)) {
        ADD_utc((byte_t *)&now, t2_p->to_extension, (byte_t *)&t2_new);
    } else {
        COPY_utc(t2_p->to_expiration, (byte_t *)&t2_new);
    }

    /* Find minimum */
    if (LT_utc((byte_t *)&t1_new, (byte_t *)&t2_new)) {
        return(t1_p);
    }

    return(t2_p);
}
