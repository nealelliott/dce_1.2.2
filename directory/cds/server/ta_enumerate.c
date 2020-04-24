/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta_enumerate.c,v $
 * Revision 1.1.6.2  1996/02/18  19:36:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:37  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:32:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:08  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:04:48  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:34:20  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:44:01  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:36  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:12:57  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:46:37  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:24:01  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:24:39  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:25:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: ta_enumerate.c
 * Version: X02.29
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
#include <dbdef.h>
#include <find.h>
#include <names.h>
#include <security.h>


/*
 * build_att_set
 *   This routine returns a set of attribute
 * specifiers for the entry up to the size of the buffer specified by
 * the clerk.  A flag is returned indicating whether all attributes fit.
 *
 * Input:
 *	rec_p = address of record descriptor
 *      context_p = address of attribute name
 *      size = size of user buffer for value set
 * Output:
 *	retlen_p = address to return return length
 *	room_flag_p = address to return flag indicating if anything fit
 *
 *
 * Function value:
 *      status
 */
dns_status_t 
build_att_set (db_data_t     *rec_p,
               SimpleName_t  *context_p,
               Set_t         *retset_p,
               int           size,
               int           *retlen_p,
               int           *room_flag_p)
{
    attribute_descriptor_t *dns_p;
    byte_t		*buf_p;
    MemberValue_t	*mem_p;       
    DBSet_t		*set_p;         
    DBSet_t		*attr_set_p;    
    SimpleName_t	*name_p;
    int			cnt, pos, p_pos;
    byte_t		wholeEntry;
    dns_status_t	status;

    /* Get buffer to store names, until we know how much will fit */
    buf_p = (byte_t *)dns_malloc(size);
    if (!buf_p) return(DNS_NONSRESOURCES);

    /* Get pointers into the set of attribute sets */
    set_p = (DBSet_t *)rec_p->data_p->dbr_data;
    ALIGN_CHECK(set_p);
    cnt = NUM_DBSet(set_p);

   /* Build return set header */
    INS8(retset_p->st_flag, ST_present);
    INS16(retset_p->st_offsetLength, 0);
    INS16(retset_p->st_valueLength, 0);
    size -= sizeof(NULLSET_u);       /* space left for members */
    wholeEntry = BL_true;               /* assume all will fit */
    *room_flag_p = TRUE;	    /* Assume it something will fit */

   /*
    * Locate first attribute to return, they are stored
    * alphabetically in the database
    */  

    status = find_attribute(rec_p->data_p, (AttributeName_t *)context_p,
	&attr_set_p, &pos);
    /* If attribute is there, start at next */
    if (status == DNS_SUCCESS)
        ++pos;    

    mem_p = (MemberValue_t *)buf_p;
    p_pos = 0;
    /* Continue if there are any entries greater then context */
    for ( ; pos < cnt; ++pos)
	{
	name_p = (SimpleName_t *)INDEX_DBSet(set_p, pos);
	attr_set_p = (DBSet_t *)SKIP_SimpleName(name_p);
	ALIGN_CHECK(attr_set_p);
        dns_p = find_known_attribute((AttributeName_t *)name_p);

        if ((EXT8(attr_set_p->ds_flag) == DS_present) &&
	    (!dns_p || dns_p->visable))
            {
	    size -= sizeof(word_u) + sizeof(MemberValue_u)
		- MAXATTRIBUTE + sizeof(AttributeSpecifier_u)
		- sizeof(AttributeName_u) + LEN_SimpleName(name_p);

            /* Make sure this member will fit */
            if (0 < size)
                {
		AttributeSpecifier_t *as_p =
		    (AttributeSpecifier_t *)mem_p->mv_value;

		INS8(mem_p->mv_flag, MV_present);
		COPY_Timestamp(attr_set_p->ds_ts, mem_p->mv_timestamp);
		COPY_AttributeType(attr_set_p->ds_type, as_p->as_type);
		names_simple_casemask(name_p,
		    (SimpleNameMask_t *)attr_set_p->ds_casemask,
		    (SimpleName_t *)as_p->as_name);
		INS8(mem_p->mv_valuetype, VT_byte);
		INS16(mem_p->mv_length, LEN_AttributeSpecifier(as_p));
		mem_p = (MemberValue_t *)SKIP_MemberValue(mem_p);
		p_pos++;
                }
            else
                {
		if (p_pos == 0)
		    *room_flag_p = FALSE;
                wholeEntry = BL_false; /* Cannot return it all */
                break;
                }
            }
        }

    /* Update return set header */
    if (p_pos)
	{
	int valueLength = DIFF_bytes(mem_p, buf_p);

	INS16(retset_p->st_offsetLength, p_pos*sizeof(word_u));
	INS16(retset_p->st_valueLength, valueLength);
	mem_p = (MemberValue_t *)DATA_Set(retset_p);
	COPY_bytes(buf_p, mem_p, valueLength);
	/* Now set the offsets */   
	for (pos = 0; pos < p_pos; pos++)
	    {
	    INS16(OFFSET_Set(retset_p, pos), DIFF_bytes(mem_p, retset_p));
	    mem_p = (MemberValue_t *)SKIP_MemberValue(mem_p);
	    }
	}

    dns_free((char *)buf_p);
    INS8(SKIP_Set(retset_p), wholeEntry);
    *retlen_p += LEN_Set(retset_p) + sizeof(BOOLEAN_u);
    return(DNS_SUCCESS);
}

/*
 * ta_enumerate_att
 *   This routine tries to find an entry.  If returns a set of attribute
 * specifiers for the entry up to the size of the buffer specified by
 * the clerk.  A flag is returned indicating whether all attributes fit.
 *
 * Input:
 *      user_p = address of user description
 *      progress_p = address of progress record. This is also the output 
 *                   buffer.  Progress record is modified in place and
 *                   whatever space remains is for output data.
 *      entry_type = {object, directory, subdirectory, clearinghouse, link}
 *      context_p = address of attribute name
 *      max_size = size of user buffer for value set
 *	*retlen_p = size of buffer to receive output data
 *
 * Output:
 *      retlen_p = address to return size of output data 
 *                 This is used only on success.  It includes the size of
 *                 the progress record, return set and wholeentry flag.
 *
 * Function value:
 *      status
 */
dns_status_t 
ta_enumerate_att (user_descriptor_t  *user_p,
                  Progress_t         *progress_p,
                  int                entry_type,
                  SimpleName_t       *context_p,
                  int                max_size,
                  int                *retlen_p)
{
    dns_status_t	status;
    db_stream_t		*stream_p;
    db_data_t		*rec_p;     
    int			retlen = *retlen_p;
    int			chlist_len;
    int			room_flag;
    Set_t		*retset_p;

    status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_ReadAttribute);
    if (status != DNS_SUCCESS) return(status);

    status = walktree(user_p, entry_type, WALKTREE_LOOKUP_OP, progress_p, stream_p);
    if (status != DNS_SUCCESS) return(status);

    if (entry_type == ET_dirOrObj)
	if (FLAG_IS_SET(progress_p->pr_flags, PR_directory))
	    entry_type = ET_directory;
	else
	    entry_type = ET_object;

    status = security_check(user_p, stream_p, Enum_Att_check, entry_type,
			    NULL);

    if (status == DNS_UNKNOWNENTRY)
      /* Rebuild resolved name for exception */
      resolved_null(progress_p);

    if (status == DNS_SUCCESS)
        if (max_size < sizeof(NULLSET_u))
	    status = DNS_NONSRESOURCES;
	else
	    {
            /* Determine what real size of return set can be */
            *retlen_p = LEN_Progress(progress_p);
	    chlist_len = retlen - *retlen_p - sizeof(BOOLEAN_u);

            retset_p = (Set_t *)SKIP_bytes(progress_p, *retlen_p);
	    if ((entry_type == ET_directory) || 
		(entry_type == ET_clearinghouse))
	      rec_p = &stream_p->db_dir;
	    else
	      rec_p = &stream_p->db_entry;

	    if (max_size < chlist_len)
	        status = build_att_set(rec_p, context_p, retset_p,
					max_size, retlen_p, &room_flag);
	    else
		{
	        status = build_att_set(rec_p, context_p, retset_p,
					chlist_len, retlen_p, &room_flag);
	        if (!room_flag)
		    {
		    *retlen_p = progress_make_room(progress_p);
	            chlist_len = retlen - *retlen_p - sizeof(BOOLEAN_u);
                    retset_p = (Set_t *)SKIP_bytes(progress_p, *retlen_p);
	            if (max_size < chlist_len)
	        	status = build_att_set(rec_p, context_p, retset_p,
					max_size, retlen_p, &room_flag);
	    	    else
        	        status = build_att_set(rec_p, context_p, retset_p,
					chlist_len, retlen_p, &room_flag);
		    }
		}
	    }
    db_close_stream(stream_p);
    return(status);
}

/*
 * build_name_set
 *   This routine builds a set of entries of a specific type
 * which are under a directory.  The set is up the the size specified
 * by the user.
 *
 *
 * Input:
 *      stream_p = address of stream descriptor
 *      user_p = address of user description
 *      wildcard_p = address of wildcarded simple name
 *      context_p = address of name to start after
 *      class_p = address of class filter, may be wildcarded
 *      size = size of user buffer for value set
 *	check_access = flag indicating whether access to the entry is required
 * 	retset_p = address to return name set
 *      return_class = flag saying if class should be returned witn object name
 *
 * Output:
 *      retlen_p = address to return length of output including progress record
 *                 but not packet overhead
 *	room_flag_p = address to return flag indicating if anything fit
 * Function value:
 *      status
 *
 */
dns_status_t 
build_name_set (db_stream_t        *stream_p,
                user_descriptor_t  *user_p,
                SimpleName_t       *wildcard_p,
                SimpleName_t       *context_p,
                SimpleName_t       *class_p,
                int                size,
                int                check_access,
                Set_t              *retset_p,
                int                *retlen_p,
                int                *room_flag_p,
                int                return_class)
{
    DBSet_t		*attr_set_p; /* pointer to attribute in record */
    DBSet_t		*set_p;
    MemberValue_t	*cur_value_p;
    FullName_t		*key_p;
    SimpleName_t	*name_p;
    byte_t		*buf_p;
    int			len,index,cnt;
    byte_t		wholeEntry;
    dns_status_t	status;

    /* Make sure inoutReturnClass is false if not looking at object */
    if (stream_p->db_entry.data_type != ET_object)
	return_class = BL_false;

   /* Allocate enough memory so we can save names while we build
    * the offset positions in the return buffer.  
    * The number of entries is not known initally.
    */
    buf_p = (byte_t *)dns_malloc(size);
    if (!buf_p)
        return(DNS_NONSRESOURCES);

    /* Build return set header */
    INS8(retset_p->st_flag, ST_present);
    INS16(retset_p->st_offsetLength, 0);
    INS16(retset_p->st_valueLength, 0);
    size -= sizeof(NULLSET_u);        /* space left for members */
    wholeEntry = BL_true;
    *room_flag_p = TRUE;

   /*
    * Locate first record to start with based on context.
    * They are stored alphabetically in the database
    */  
    key_p = (FullName_t *)stream_p->db_key;
    COPY_ObjUID(stream_p->db_dir.data_uid, key_p->fn_root);
    name_p = (SimpleName_t *)key_p->fn_name;
    COPY_SimpleName(context_p, name_p);
    if (NE_bytes(name_p, NullSimpleName, sizeof(NULLSIMPLENAME_u)))
	{
	name_p = (SimpleName_t *)SKIP_SimpleName(name_p);
	COPY_SimpleName(NullSimpleName, name_p);
	}
    INS16(key_p->fn_length, SKIP_SimpleName(name_p) - key_p->fn_name);

    status = db_entry_read_next(stream_p);
    cur_value_p = (MemberValue_t *)buf_p;
    cnt = 0;
    while (status == DNS_SUCCESS)
        {
	key_p = (FullName_t *)stream_p->db_key;
	name_p = (SimpleName_t *)key_p->fn_name;
	set_p = (DBSet_t *)stream_p->db_entry.data_p->dbr_data;
	ALIGN_CHECK(set_p);
        if (EXT8(set_p->ds_flag) == DS_present)
            {
            /* Have to check access if not an enumerate child or */
            /* didn't have control to parent dir - check made in caller */
            if (check_access)
                status = security_check(user_p, stream_p, Enum_Entry_check,
				stream_p->db_entry.data_type, NULL);
            if (status == DNS_SUCCESS)
                {
                /* See if matches wildcard */
                if (names_s_compare(name_p, wildcard_p))
                    {
		    /* default ClassName to NullSimpleName */
		    SimpleName_t *class_name_p = (SimpleName_t *)NullSimpleName;

                    if (stream_p->db_entry.data_type == ET_object)
                    /* Additional wildcarding needed */
                        {
                        status = find_attribute(stream_p->db_entry.data_p,
			    dns_AttributeName(DNS_CLASS), &attr_set_p, &index);

                        if (status == DNS_SUCCESS)
			    {
			    DBSetMember_t *dm_p =
				(DBSetMember_t *)DATA_DBSet(attr_set_p);

			    class_name_p = (SimpleName_t *)dm_p->dm_data;
			    }
			/* Allow null class to match '*' */
		        if (!names_s_compare(class_name_p, class_p))
				status = DNS_UNKNOWNENTRY;
			else status = DNS_SUCCESS;
                        }
                    if (status == DNS_SUCCESS)
                        {
			size -= sizeof(word_u) + sizeof(MemberValue_u)
				- MAXATTRIBUTE + LEN_SimpleName(name_p);

                        if (return_class)
			    size -= LEN_ClassName(class_name_p);

                        if (0 < size)
                            {
			    byte_t *value_p = cur_value_p->mv_value;

                            INS8(cur_value_p->mv_flag, MV_present);
                            COPY_Timestamp(stream_p->db_entry.data_cts,
                                      cur_value_p->mv_timestamp);
			    db_entry_name_cased(stream_p,
				(SimpleName_t *)value_p);
			    INS8(cur_value_p->mv_valuetype, 
				     VT_byte);
			    value_p = SKIP_SimpleName(value_p);
                            if (return_class)
                                {
                                /*
                                 * copy the class name in after the obj name
                                 */
                                COPY_ClassName(class_name_p, value_p);
				value_p = SKIP_ClassName(value_p);
				INS8(cur_value_p->mv_valuetype, 
				     VT_byte);
                                }
			    INS16(cur_value_p->mv_length,
				value_p - cur_value_p->mv_value);

                            cur_value_p =
				(MemberValue_t *)SKIP_MemberValue(cur_value_p);
                            cnt++;
                            }
                        else
                            {
			    if (cnt == 0)
				*room_flag_p = FALSE;
                            wholeEntry = BL_false;
			    dns_free((char *)stream_p->db_entry.data_p);
                            stream_p->db_entry.data_p = NULL;
                            break;
                            }
                        }
                    } /* End if matchwild */ 
                } /* End of have access */
            } /* End if present */
	dns_free((char *)stream_p->db_entry.data_p);
        stream_p->db_entry.data_p = NULL;
        status = db_entry_read_next(stream_p);
        } /* end while */

    /* The number of entries are known, so values can be copied
     * to the end of the offset array
     */
    if (cnt)
	{
	int valueLength = DIFF_bytes(cur_value_p, buf_p);

	INS16(retset_p->st_offsetLength, cnt*sizeof(word_u));
	INS16(retset_p->st_valueLength, valueLength);
	cur_value_p = (MemberValue_t *)DATA_Set(retset_p);
	COPY_bytes(buf_p, cur_value_p, valueLength);
	/* Adjust offsets to start from beginning of set */
	for (len = 0; len < cnt; len++)
	    {
            INS16(OFFSET_Set(retset_p, len), DIFF_bytes(cur_value_p, retset_p));
	    cur_value_p = (MemberValue_t *)SKIP_MemberValue(cur_value_p);
	    }
	}

    dns_free((char *)buf_p);
    buf_p = SKIP_Set(retset_p);
    PUT8(buf_p, wholeEntry);
    INS8(buf_p, return_class);
    *retlen_p += LEN_Set(retset_p) + 2*sizeof(BOOLEAN_u);
    return(DNS_SUCCESS);
}

/*
 * ta_enumerate
 *   This routine builds a set of entries of a specific type
 * which are under a directory.  The set is up the the size specified
 * by the user.
 *
 *
 * Input:
 *      user_p = address of user description
 *      progress_p = address of progress record which is modified in place
 *                   Following the progress record is the buffer for returning
 *                   the set and whole entry flag.  This saves some
 *                   copying time.
 *      wildcard_p = address of wildcarded simple name
 *      context_p = address of name to start after
 *      class_p = address of class filter, may be wildcarded
 *      max_size = size of user buffer for value set
 *      entry_type = {object, link, subdirectory}
 *	*retlen_p = size of buffer to receive output data
 *
 * Output:
 *      retlen_p = address to return length of output including progress record
 *                 but not packet overhead
 * Function value:
 *      status
 *
 */
dns_status_t 
ta_enumerate (user_descriptor_t  *user_p,
              Progress_t         *progress_p,
              SimpleName_t       *wildcard_p,
              SimpleName_t       *context_p,
              SimpleName_t       *class_p,
              int                max_size,
              int                entry_type,
              int                *retlen_p,
              int                return_class)
{
    db_stream_t		*stream_p;
    Set_t		*retset_p;
    int			retlen = *retlen_p;
    int			list_len;
    int			room_flag;
    int			check_entry_access;
    dns_status_t	status;

    status = db_open_stream(user_p->ud_chuid_p, &stream_p, OP_ReadAttribute);

    if (status != DNS_SUCCESS) return(status);

    status = walktree(user_p, ET_directory, WALKTREE_LOOKUP_OP, progress_p, stream_p);
    if (status != DNS_SUCCESS) return(status);

    if (entry_type == ET_dirOrObj)
	if (FLAG_IS_SET(progress_p->pr_flags, PR_directory))
	    entry_type = ET_directory;
	else
	    entry_type = ET_object;

    /*
     * this next block of code is a little dense...
     * first we check if the user possibly has CONTROL to the directory,
     * that is the simplest case, and the fastest.  If the user has
     * CONTROL to the parent, no more access checking on the individual
     * entries need be done. (when building name sets to return)
     *
     * If the user doesn't have CONTROL then they need at least READ
     * access to the directory.  If they have only READ, then they must check
     * the access on individual entries, except for child pointer entries.
     * The status from the check for READ on the parent directory is actually
     * checked in the next large block of code.  Basically if the READ
     * access doesn't exist, you fall out the bottom. \sjm
     */
	{
	status = security_check(user_p, stream_p, Enum_Entry_check, 
				ET_directory, NULL);
	/* IN CDS name belongs to directory, only need read to directory */
	check_entry_access = FALSE;
	}
	
    if (status == DNS_UNKNOWNENTRY)
        /* Rebuild resolved name for exception */
        resolved_null(progress_p);
    else if (status == DNS_SUCCESS)
        if (max_size < (sizeof(NULLSET_u) + 2*sizeof(BOOLEAN_u)))
	    status = DNS_NONSRESOURCES;
	else
            {
	    stream_p->db_entry.data_type = entry_type;
            *retlen_p = LEN_Progress(progress_p); /* size of progress record */
            retset_p = (Set_t *)SKIP_bytes(progress_p, *retlen_p);
 
            /* Determine what real size of return set can be */
            list_len = retlen - (*retlen_p + 2*sizeof(BOOLEAN_u));
	    if (max_size < list_len)
	        status = build_name_set(stream_p, user_p, wildcard_p, context_p,
				        class_p, max_size, check_entry_access,
					retset_p, retlen_p, &room_flag,
					return_class);
	    else
		{
	        status = build_name_set(stream_p, user_p, wildcard_p, context_p,
				        class_p, list_len, check_entry_access,
					retset_p, retlen_p, &room_flag,
					return_class);
		if (!room_flag)
	   	    {
                    *retlen_p = progress_make_room(progress_p); 
                    retset_p = (Set_t *)SKIP_bytes(progress_p, *retlen_p);
 
                    /* Determine what real size of return set can be */
                    list_len = retlen - (*retlen_p - 2*sizeof(BOOLEAN_u));
	            if (max_size < list_len) list_len = max_size;
		    status = build_name_set(stream_p, user_p, wildcard_p,
			context_p, class_p, list_len, check_entry_access,
			retset_p, retlen_p, &room_flag, return_class);

		    }
		}
	    }

    db_close_stream(stream_p);
    return(status);
}
