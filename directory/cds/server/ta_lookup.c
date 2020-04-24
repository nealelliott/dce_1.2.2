/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: ta_lookup.c,v $
 * Revision 1.1.17.1  1996/08/09  12:01:27  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  15:58 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	Incorporate fix for OT 13085: CDSD memory leaks.
 * 	[1996/02/18  19:36:58  marty  1.1.15.3]
 *
 * Revision 1.1.15.3  1996/02/18  19:36:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:17:42  marty]
 * 
 * Revision 1.1.15.2  1995/12/08  15:33:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:13  root]
 * 
 * Revision 1.1.11.1  1994/10/26  19:22:46  zee
 * 	OT 12653: Add argument to name_in_cell_test calls.
 * 	[1994/10/26  19:22:31  zee]
 * 
 * Revision 1.1.9.4  1994/08/03  19:04:53  mccann
 * 	fix assert
 * 	[1994/07/21  19:28:56  mccann]
 * 
 * 	final sams cleanup drop
 * 	[1994/07/19  18:31:48  mccann]
 * 
 * Revision 1.1.9.3  1994/06/09  18:44:06  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:42  devsrc]
 * 
 * Revision 1.1.9.2  1994/05/06  16:04:30  zee
 * 	     HCell BL4 functionality.
 * 	[1994/05/05  21:21:16  zee]
 * 
 * Revision 1.1.9.1  1994/03/12  22:13:12  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:47:15  peckham]
 * 
 * Revision 1.1.7.2  1993/06/24  19:48:31  hinman
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:37:13  hinman]
 * 
 * Revision 1.1.5.2  1992/12/30  14:24:27  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:25:07  zeliff]
 * 
 * Revision 1.1.2.3  1992/08/05  15:43:38  peckham
 * 	OT#4871: The fix in back_do_updates.c caused another bug to
 * 	be exercised in lookup_entry(), which we had recently discovered
 * 	in our code and were preparing to fix. This routine was the only
 * 	when that seemed not to have to lookup the parent directory before
 * 	the main body of code, but this was an incorrect assumption.
 * 	process_list() presumes that the directory has been loaded, and
 * 	builds the chlist based on non-existant data. This can cause
 * 	the code to drop out with eventual consequences of the stream
 * 	being released twice. Now this will screw up the stream cache on
 * 	the file, with possible subsequent use of the same stream by more
 * 	than one thread. Make sure the directory is loaded!
 * 	Also, the same bug of not unlocking a non-present entry can happen
 * 	in check_exists(). Release the entry when an entry is discovered
 * 	to not be present.
 * 	[1992/08/05  14:23:47  peckham]
 * 
 * Revision 1.1.2.2  1992/01/22  23:02:38  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:18:14  melman]
 * 
 * $EndLog$
 */
/*
 * Module ta_lookup.c
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
#include <replica.h>
#include <security.h>
#include <sets.h>
#include <uid.h>
#include <dnsnormal.h>
#include <hcell.h>


#define	MAX_CHLIST (NSMAXSIZE - sizeof(VCRequestHeader_u)\
    - sizeof(Progress_u) + sizeof(FullName_u) - sizeof(NULLFULLNAME_u)\
    - sizeof(Set_u) - 100)

/*
 * Local data table
 *
 * Used to reduce redundancy in calling parameters
 * and to avoid pointer recomputation.
 */
typedef struct data_s {
    user_descriptor_t	*d_user_p;	/* const */
    Progress_t		*d_progress_p;	/* const */
    db_stream_t		*d_stream_p;	/* const */
    int			d_cnt;		/* currently unresolved names */
    byte_t		d_lookingfor;	/* const below walktree() */
    byte_t		d_optype;	/* const */
    byte_t		d_for_other_cell; /* request is for other cell */
} data_t;

/*
 * Local function prototypes
 */
static dns_status_t
blduppointers (
    data_t *const	,
    Set_t		*);

static void
build_return_name (
    FullName_t		**,
    FullName_t		*,
    ObjUID_t		*,
    int			 );

static void
catenate_name(data_t *const,
              FullName_t *p);

static dns_status_t
build_key (
    data_t *const       );

static dns_status_t
process_list (
    data_t *const       ,
    Set_t               *);

static void
process_dir (
    struct db_data	*,
    Set_t		*);

static dns_status_t
process_child (
    data_t *const       );

static dns_status_t
process_link (
    data_t *const       );

static dns_status_t
process_obj (
    data_t *const       );

static dns_status_t
process_ch (
    data_t *const       );

static dns_status_t
walktree_assist (
    data_t *const       );

static dns_status_t
lookup_any (
    data_t *const       );

static dns_status_t
lookup_entry (
    data_t *const       );

static dns_status_t
lookup_child (
    data_t *const       );

static dns_status_t
lookup_for_create (
    data_t *const       );

static dns_status_t
lookup_dir (
    data_t *const       );


/* -------------------------------------------------------------------------- */

/*
 * check_exists
 *     This routine checks if an entry of the same name exists.
 *
 * Input:
 *     stream_p = pointer to data base stream
 *     optype = whether lock is required
 *
 * Value:
 *     DNS_SUCCESS - entry found
 *     DNS_UNKNOWNENTRY - doesn't exist
 */
dns_status_t
check_exists (db_stream_t  *stream_p,
              int          optype)
{
    DBSet_t		*set_p;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >check_exists(,%d)",
        optype));

    /* Try child pointer first */
    stream_p->db_entry.data_type = ET_childPointer;
    if (optype != WALKTREE_LOOKUP_OP)
      status = db_entry_read_for_update(stream_p);
    else
      status = db_entry_read(stream_p);
    if (status == DNS_SUCCESS)
	{
	set_p = (DBSet_t *)stream_p->db_entry.data_p->dbr_data;
	ALIGN_CHECK(set_p);
	if (EXT8(set_p->ds_flag) == DS_present)
            {
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug4,
                " <check_exists[1] return %d",
                status));

	    return(status);
            }
        db_entry_release(stream_p);
	}

    /* Try object */
    stream_p->db_entry.data_type = ET_object;
    if (optype != WALKTREE_LOOKUP_OP)
      status = db_entry_read_for_update(stream_p);
    else
      status = db_entry_read(stream_p);
    if (status == DNS_SUCCESS)
	{
	set_p = (DBSet_t *)stream_p->db_entry.data_p->dbr_data;
	ALIGN_CHECK(set_p);
	if (EXT8(set_p->ds_flag) == DS_present)
            {
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug4,
                " <check_exists[2] return %d",
                status));

            return(status);
            }

        db_entry_release(stream_p);
	}

    stream_p->db_entry.data_type = ET_softlink;
    if (optype != WALKTREE_LOOKUP_OP)
        status = db_entry_read_for_update(stream_p);
    else
        status = db_entry_read(stream_p);
    if (status == DNS_SUCCESS)
	{
	set_p = (DBSet_t *)stream_p->db_entry.data_p->dbr_data;
	ALIGN_CHECK(set_p);
	if (EXT8(set_p->ds_flag) == DS_present)
            {
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug4,
                " <check_exists[3] return %d",
                status));

            return(status);
            }

        db_entry_release(stream_p);
	status = DNS_UNKNOWNENTRY;
	}

    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug4,
                " <check_exists[4] return %d",
                status));

    return(status);
}


/* -------------------------------------------------------------------------- */

/*
 * operationallowed
 *   This routine checks the replica type of a directory
 * with the operation requestion to determine if it is valid
 * at this clearinghouse.
 *
 *
 * Input:
 *      op_type = {WALKTREE_CREATE_OP,WALKTREE_DELETE_OP,WALKTREE_UPDATE_OP,
 *                 WALKTREE_SKULK_OP,WALKTREE_LOOKUP_OP}
 *      dir_type = directory type
 *      rec_type = record type
 *
 * Output:
 *      none
 *
 * Function value:
 *      DNS_SUCCESS if allowed
 *      DNS_NOTFND if notallowed
 *      DNS_NAMESERVERBUG if replicatype missing
 */
dns_status_t
operationallowed (int  op_type,
                  int  dir_type,
                  int  rec_type)
{
    int			allowed;

    if (rec_type == ET_clearinghouse)
	return(DNS_SUCCESS);

    switch (op_type)
        {
        case WALKTREE_CREATE_OP:
             if (rec_type == ET_childPointer)
                 allowed = (dir_type == RT_master);
             else
                 allowed = ((dir_type == RT_master) ||
                           (dir_type == RT_secondary));
             break;
        case WALKTREE_UPDATE_OP:
             allowed = ((dir_type == RT_master) ||
			(dir_type == RT_secondary));
             break;
        case WALKTREE_DELETE_OP:
	     if (rec_type == ET_directory)
		allowed = (dir_type == RT_master);
	     else
                allowed = ((dir_type == RT_master) ||
			   (dir_type == RT_secondary));
             break;
        case WALKTREE_SKULK_OP:
             allowed = ((dir_type == RT_master) ||
			(dir_type == RT_secondary));
             break;
        case WALKTREE_LOOKUP_OP:
             allowed = TRUE;
             break;
	case WALKTREE_LINK_OP:
             allowed = (dir_type == RT_master);
	     break;
        default:
             return(DNS_NAMESERVERBUG);
        }

    return((allowed) ? DNS_SUCCESS : DNS_NOTFND);
}


/* -------------------------------------------------------------------------- */

/*
 * blduppointers
 *     This routine builds the replica set of uppointers to return to
 * the clerk.  Uppointers contains clearinghouses closer to the root.
 *
 * Input:
 *	data_p = lookup data
 *	    d_stream_p = pointer to stream descriptor
 *     chlist_p = address to insert replica set
 *
 */
static dns_status_t
blduppointers (data_t *const  data_p,
               Set_t          *chlist_p)
{
    register db_stream_t *stream_p = data_p->d_stream_p; /* cache this */
    register file_info_t *file_p = stream_p->db_file_p;
    DBSet_t		 *set_p;
    int			  index;
    dns_status_t	  status;
    cdsNameRef_t          name_ref;
    FullName_t            test_name;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >blduppointers()"));


    /*
     * Check the name we are referencing and determine if it is a
     * hierarchical cell reference or a GDA reference.
     */

    catenate_name(data_p, &test_name);

    name_ref = name_in_cell_test(stream_p->db_file_p,
                                 &test_name, 0);
    if (name_ref == cdsParentRef) {
        SERVER_LOCK(file_p->file_root_dir_p->dir_mutex) {
            set_p = file_p->file_root_dir_p->dir_cellparent_set_p;
            if (set_p != NULL) {
                if (NUM_DBSet(set_p) > 0) {
                    build_random_set(set_p, chlist_p, 0, MAX_CHLIST);
                    status = DNS_NOTFND;
                } else {
		    DCE_SVC_DEBUG((
                       cds__svc_handle,
                       cds_svc_server,
                       svc_c_debug5,
                       "  blduppointers() CDS_ParentCellPointers empty"));
		    name_ref = cdsGDARef;
		}
            } else {
    	    /* No pointers, root must be here */
                DCE_SVC_DEBUG((
			       cds__svc_handle,
			       cds_svc_server,
			       svc_c_debug4,
			       "  blduppointers() no CDS_ParentCellPointers"));
		name_ref = cdsGDARef;
            }

        } SERVER_END_LOCK(file_p->file_root_dir_p->dir_mutex);

    }

    if (name_ref != cdsParentRef) {

        /*
         * Get pseudo directory
         */

        status = db_pseudo_read(stream_p);

	if ((status == DNS_SUCCESS) && data_p->d_for_other_cell) {
	    /* clear the clearinghouse hash */
	    stream_p->db_dir_hash_p = NULL;
	    COPY_FullName(&stream_p->db_file_p->file_ch_p->ch_cellname,
		stream_p->db_key);
	    if ((db_dir_read(stream_p) == DNS_SUCCESS)
		&& (find_attribute(stream_p->db_dir.data_p,
				dns_AttributeName(DNS_GDAPOINTERS),
				&set_p, &index) == DNS_SUCCESS)
		&& (0 < NUM_DBSet(set_p))) {
                build_random_set(set_p, chlist_p, 0, MAX_CHLIST);
                status = DNS_NOTFND;
            } else {
		status = DNS_ROOTLOST;
	    }
        }
	if (status == DNS_SUCCESS) {
	    if (stream_p->db_file_p->file_root_dir_present) {
		status = DNS_UNKNOWNENTRY;
	    } else {
		status = DNS_ROOTLOST; /* crucial replication chged */
	    }
        }

    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " <blduppointers() return(%d)",
        status));
    return(status);

}


/* -------------------------------------------------------------------------- */

/*
 * build_return_name
 *	This routine builds the resolved and unresolved names
 *	within the progress record.  Since the resolved anme
 *	begins where the unresolved ends, its pointer must
 *	be updated as output.
 * Note: this routine re-arranges the names with the same amount
 * of space that they currently take up.  Anything after the
 * resolvedname on input will still be their on output.
 *
 * Input:
 *	resolved_pp = address of pointer to resolved full name
 *	unresolved_p = pointer to unresolved full name
 *	uid_p = pointer to unresolved name uid
 *	cnt = count of unresolved names still left
 * Output:
 *	resolved_pp = updated as result of smaller unresolved name
 *
 */
static void
build_return_name (FullName_t  **resolved_pp,
                   FullName_t  *unresolved_p,
                   ObjUID_t    *uid_p,
                   int         cnt)
{
    FullName_u		nambuf;
    FullName_t		*newresolved_p;
    SimpleName_t	*name_p;
    int			i, total_len, cur_cnt;

   /*
    * If this is the first time anything has been resolved, copy
    * namespace UID to resolved name
    */
    if (ObjUID_is_nil((*resolved_pp)->fn_root))
	COPY_ObjUID(unresolved_p->fn_root, (*resolved_pp)->fn_root);

    /* Copy uid of last resolved directory to unresolved name */
    COPY_ObjUID(uid_p, unresolved_p->fn_root);

   /* Determine number of names to move */
    cur_cnt = dnsCountSimple((dns_opq_fname *)unresolved_p);
    cur_cnt -= cnt;

    /* Determine bytes to be moved from unresolved to resolved names */
    total_len = 0;
    name_p = (SimpleName_t *)unresolved_p->fn_name;
    for (i = 0; i < cur_cnt; i++)
	{
	total_len += LEN_SimpleName(name_p);
	/* Update pointers */
	name_p = (SimpleName_t *)SKIP_SimpleName(name_p);
	}

   /*
    * Move simple names to temporary area because resolved pointer must
    * be updated to point into the current unresolved name area
    */
    COPY_bytes(unresolved_p->fn_name, nambuf, total_len);

    /* Build new unresolved name */
    INS16(unresolved_p->fn_length, EXT16(unresolved_p->fn_length) - total_len);
    MOVE_bytes(name_p, unresolved_p->fn_name, EXT16(unresolved_p->fn_length));

    /* Update resolved name pointer and build new name */
    newresolved_p = (FullName_t *)SKIP_FullName(unresolved_p);
    COPY_FullName(*resolved_pp, newresolved_p);

    /* Find terminating simple name */
    name_p = (SimpleName_t *)names_pointlast(newresolved_p);
    if (EXT8(name_p->sn_flag) != SN_null)
	name_p = (SimpleName_t *)SKIP_SimpleName(name_p);

    /* Insert new resolved portion in place of terminating name */
    COPY_bytes(nambuf, name_p, total_len);
    INS16(newresolved_p->fn_length,
	EXT16(newresolved_p->fn_length) + total_len);

    /* Add new terminating name */
    name_p = (SimpleName_t *)SKIP_bytes(name_p, total_len);
    COPY_SimpleName(NullSimpleName, name_p);

    /* Update return pointer */
    *resolved_pp = newresolved_p;
}


/* -------------------------------------------------------------------------- */

/*
 * catenate_name
 *
 * This routine copies the resolved and unresolved names in the progress
 * record to a fullname.
 */

static void
catenate_name(data_t *const data_p,
              FullName_t *name_p)
{
    register FullName_t *unresolvedName_p = (FullName_t *)
				data_p->d_progress_p->pr_unresolvedName;
    register FullName_t *resolvedName_p = (FullName_t *)
				SKIP_FullName(unresolvedName_p);
    if (ObjUID_is_nil(resolvedName_p->fn_root)) {
	COPY_FullName(unresolvedName_p, name_p);
    } else { 
	/* Copy resolved name first */
	COPY_FullName(resolvedName_p, name_p);
	/* Append unresolved name over termininating simple name */
	MOVE_bytes(unresolvedName_p->fn_name, SKIP_bytes(name_p,
		LEN_FullName(name_p) - sizeof(NULLSIMPLENAME_u)),
		EXT16(unresolvedName_p->fn_length));
	INS16(name_p->fn_length, EXT16(name_p->fn_length) +
		EXT16(unresolvedName_p->fn_length) - sizeof(NULLSIMPLENAME_u));

    }
}


/* -------------------------------------------------------------------------- */


/*
 * build_key
 *	This routine builds the full name key to lookup a record
 *
 * Input:
 *	data_p = lookup data
 *	    d_stream_p = pointer to stream descriptor
 *	    d_progress_p = address of progress record
 *
 */
static dns_status_t
build_key (data_t *const data_p)
{
    register db_stream_t *stream_p = data_p->d_stream_p; /* cache this */
    FullName_t *key_p = (FullName_t *)stream_p->db_key;

    stream_p->db_dir_hash_p = NULL;

    catenate_name(data_p, key_p);

    /* If nothing has been resolved, entire name in unresolved_p */
    /* All keys must be in neutral case */
    /* X.500 changing case isn't sufficient for typed names */
    return dns_normalize_fname(key_p, key_p, sizeof(stream_p->db_key));
}



/* -------------------------------------------------------------------------- */

/*
 * process_list
 *     Build the return chlist.  Only return information we believe will
 * be the most up to date.  Don't return values that
 * won't help the clerk.
 */
static dns_status_t
process_list (data_t *const  data_p,
              Set_t          *chlist_p)
{
    int                 state;
    dns_status_t        status = DNS_SUCCESS;

    DCE_SVC_DEBUG((
             cds__svc_handle,
             cds_svc_server,
             svc_c_debug4,
             " >process_list()"));

    /*
     * Get chlist.  Remember the positon of the list within
     * the progress record remains the same.
     */
    process_dir(&data_p->d_stream_p->db_dir, chlist_p);

    state = db_dir_get_replica_state(data_p->d_stream_p->db_dir_hash_p);

    /* If cannot return anything useful don't overwrite clerks cache
     * Assume 1 entry is for self, and this doesn't help clerk
     * in the case where I don't have the entire set.  I state is not
     * on assume I don't have the entire set if only one entry.
     */
    if ((state != RS_on) && (NUM_Set(chlist_p) == 1))
      COPY_Set(NullSet, chlist_p);

    /* V2.0 allows client to request response irregardless of state */
    if (FLAG_IS_CLEAR(data_p->d_progress_p->pr_flags, PR_ignoreState) &&
	 (state != RS_on))
	status = DNS_NOTFND;

    DCE_SVC_DEBUG((
             cds__svc_handle,
             cds_svc_server,
             svc_c_debug4,
             " <process_list() returned(%d)",
             status));

    return(status);
}


/* -------------------------------------------------------------------------- */

/*
 * process_dir
 *   	Build the replica set from a directory
 *
 * Input:
 *      dir_p = pointer to directory descriptor
 *      chlist_p = pointer to return replica hints
 *
 * Output:
 *
 */
static void
process_dir (db_data_t  *dir_p,
             Set_t      *chlist_p)
{
    DBSet_t		*set_p;
    int			index;

    if (dir_p->data_p && (find_attribute(dir_p->data_p,
	    dns_AttributeName(DNS_REPLICAS), &set_p, &index) == DNS_SUCCESS))
        build_random_set(set_p, chlist_p, 0, MAX_CHLIST);
    else
	COPY_Set(NullSet, chlist_p);

}


/* -------------------------------------------------------------------------- */

/*
 * process_child
 *   Found a child in our path.  If this is not what we expected,
 * it means the directory is not here and a return set must be built
 * for the progress record.
 *
 * Input:
 *	data_p = lookup data
 *	    d_cnt = count of unresolved names
 *	    d_stream_p = pointer to stream descriptor
 *	    d_progress_p = address of progress record
 *	    d_lookingfor = type of record we are looking for
 *
 * Function Value:
 *     DNS_SUCCESS - looking for a child and found it
 *     DNS_NOTFND - need to go to another clearinghouse to find directory
 *     DNS_UNKNOWNENTRY - found a child and didn't expect it
 */
static dns_status_t
process_child (data_t *const data_p)
{
    register db_stream_t *stream_p = data_p->d_stream_p; /* cache this */
    register Progress_t *progress_p = data_p->d_progress_p; /* cache this */
    FullName_t		*unresolvedName_p =
			    (FullName_t *)progress_p->pr_unresolvedName;
    FullName_t		*resolved_p =
			    (FullName_t *)SKIP_FullName(progress_p->pr_unresolvedName);
    dns_status_t	status;

    /* Send clerk to another clearinghouse */
    if ((0 < data_p->d_cnt) || (data_p->d_lookingfor == ET_directory))
        {

	ObjUID_t *id_p = (ObjUID_t *)stream_p->db_entry.data_uid;

        /* For CDS the objectuid is set up in the stream */
	COPY_ObjUID(id_p, unresolvedName_p->fn_root);
	process_dir(&stream_p->db_entry, (Set_t *)SKIP_FullName(resolved_p));
        status = DNS_NOTFND;
        }
    else switch (data_p->d_lookingfor) {
    case ET_childPointer:
    case ET_anyDirectName:
	/* Found what we were looking for */
        status = DNS_SUCCESS;
	break;
    default:
	/* Wasn't expected */
        status = DNS_UNKNOWNENTRY;
	break;
    }

    return(status);
}


/* -------------------------------------------------------------------------- */

/*
 * process_link
 *   Found a soft link.
 *   See if it is what we expected.  If not, expand and return to client.
 *
 * Input:
 *	data_p = lookup data
 *	    d_cnt = count of unresolved names
 *	    d_stream_p = pointer to stream descriptor
 *	    d_user_p = address of user information
 *	    d_progress_p = address of progress record
 *	    d_lookingfor = type of record we are looking for
 *
 *
 * Function Value:
 *      DNS_SUCCESS - looking for a link and found it
 *      DNS_NOTFND - have a new name to return to the clerk
 *      DNS_UNKNOWNENTRY - found a link and didn't expect it
 *      DNS_INVALIDNAME - current unresolved name and linktarget form
 *                          a name too long
 *      DNS_NAMESERVERBUG - dns$linktarget missing from record
 */
static dns_status_t
process_link (data_t *const data_p)
{
    FullName_u		oldname;
    register db_stream_t *stream_p = data_p->d_stream_p; /* cache this */
    register Progress_t *progress_p = data_p->d_progress_p; /* cache this */
    FullName_t		*unresolvedName_p =
			    (FullName_t *)progress_p->pr_unresolvedName;
    FullName_t		*resolvedName_p =
			    (FullName_t *)SKIP_FullName(progress_p->pr_unresolvedName);
    DBSet_t		*set_p;
    DBSetMember_t	*value_p;
    int			len, index;
    dns_status_t	status;

    /* If this what we expected we are done */
    if ((data_p->d_cnt == 0) && (data_p->d_lookingfor == ET_softlink))
        return(DNS_SUCCESS);

    /* Must have read access to follow links */
    status = security_check(data_p->d_user_p, stream_p, Resolve_check, ET_softlink,
			    NULL);

    /* Set resolved name to null if we didn't find the link name */
    if (status == DNS_UNKNOWNENTRY)
      resolved_null(progress_p);

    if (status != DNS_SUCCESS) return(status);

    /* Get target name */
    status = find_attribute(stream_p->db_entry.data_p,
		dns_AttributeName(DNS_LINKTARGET), &set_p, &index);
    if (status != DNS_SUCCESS) return(DNS_NAMESERVERBUG);

    /* Rebuild progress record to start over */
    SET_FLAG(progress_p->pr_flags, PR_up);
    SET_FLAG(progress_p->pr_flags, PR_linked);
    SET_FLAG(progress_p->pr_flags, PR_hitLink);


    /* New name will be target + current unresolved name */
    len = EXT16(unresolvedName_p->fn_length);
    COPY_bytes(unresolvedName_p->fn_name, oldname, len);

    /* Point at full name of target, which is last member.*/
    value_p = find_last_value(set_p);
    if (!value_p) return(DNS_NAMESERVERBUG);

    COPY_FullName(value_p->dm_data, unresolvedName_p);

    /* Terminating simple name is part of oldname */
    INS16(unresolvedName_p->fn_length,
	EXT16(unresolvedName_p->fn_length) - sizeof(NULLSIMPLENAME_u));

    if (sizeof(unresolvedName_p->fn_name)
			< EXT16(unresolvedName_p->fn_length) + len)
       return(DNS_INVALIDNAME);

    /* Copy unresolved name to the end of target */
    COPY_bytes(oldname, SKIP_FullName(unresolvedName_p), len);
    INS16(unresolvedName_p->fn_length,
	EXT16(unresolvedName_p->fn_length) + len);

    /* Build null resolved name */
    resolvedName_p = (FullName_t *)SKIP_FullName(unresolvedName_p);

    COPY_FullName(NullFullName, resolvedName_p);

    /* Build set, nothing to give clerk */
    COPY_Set(NullSet, SKIP_FullName(resolvedName_p));

    /* Set up timeout in progress record to be the minumum of what is
     * there now and the linktimeout
     */
    status = find_attribute(stream_p->db_entry.data_p,
		dns_AttributeName(DNS_LINKTIMEOUT), &set_p, &index);
    if ((status == DNS_SUCCESS) && (value_p = find_last_value(set_p)))
        {
	COPY_Timeout(min_timeout((Timeout_t *)progress_p->pr_timeout,
				 (Timeout_t *)value_p->dm_data),
		     progress_p->pr_timeout);

        }

    return(DNS_NOTFND);
}


/* -------------------------------------------------------------------------- */

/*
 * process_obj
 *   Found an object.  See if it is what we expected.
 *
 * Input:
 *	data_p = lookup data
 *	    d_cnt = count of unresolved names
 *	    d_stream_p = pointer to stream descriptor
 *	    d_progress_p = address of progress record
 *	    d_lookingfor = type of record we are looking for
 *
 * Function Value:
 *        DNS_SUCCESS - found what we where looking for
 *        DNS_UNKNOWNENTRY - found object, not looking for it
 *        DNS_NOTFND - need to go to another clearinghouse forET_clearinghouse
 *
 */
static dns_status_t
process_obj (data_t *const data_p)
{
    register db_stream_t *stream_p = data_p->d_stream_p; /* cache this */
    register Progress_t *progress_p = data_p->d_progress_p; /* cache this */
    FullName_t		*unresolvedName_p =
			    (FullName_t *)progress_p->pr_unresolvedName;
    FullName_t		*resolvedName_p =
                             (FullName_t *)SKIP_FullName(progress_p->pr_unresolvedName);
    DBSet_t		*set_p;
    Set_t		*user_set_p;
    MemberValue_t	*replica_member_p;
    ReplicaPointer_t	*rp_p;
    byte_t		*next_p;
    Set_t		*chlist_p;
    int			index;

    if (data_p->d_cnt == 0)
        switch (data_p->d_lookingfor) {
	case ET_object:
	case ET_anyDirectName:
            return(DNS_SUCCESS);

        case ET_clearinghouse:
            if (!set_dnstest(stream_p->db_entry.data_p,
		    LEN_ClassName(cds_classlist[DNS_CLEARINGHOUSE].cname),
		    cds_classlist[DNS_CLEARINGHOUSE].cname,
		    dns_Attribute(DNS_CLASS)))
                return(DNS_UNKNOWNENTRY);

            /*
	     * Point clerk at pseudo directory for this clearinghouse object
             */
            COPY_ObjUID(stream_p->db_entry.data_uid,
		unresolvedName_p->fn_root);

	    /*  must use tower here not just a single address */
            /* Point clerk at clearinghouse with pseudo directory */
	    chlist_p = (Set_t *)SKIP_FullName(resolvedName_p);
            INS8(chlist_p->st_flag, ST_present);
            INS16(chlist_p->st_offsetLength, sizeof(word_u));

            replica_member_p = (MemberValue_t *)DATA_Set(chlist_p);
            INS8(replica_member_p->mv_flag, MV_present);
	    /* what about replica_member_p->mv_timestamp ? */
            INS16(OFFSET_Set(chlist_p, 0),
		DIFF_bytes(DATA_Set(chlist_p), chlist_p));

            rp_p = (ReplicaPointer_t *)replica_member_p->mv_value;
            COPY_ObjUID(stream_p->db_entry.data_uid, rp_p->rp_CHID);

            if (find_attribute(stream_p->db_entry.data_p,
		  dns_AttributeName(DNA_TOWERS), &set_p, &index) != DNS_SUCCESS)
                {
		  /* stuff null set for tower */
		  COPY_Set(NullSet, rp_p->rp_addressHint);
		  next_p = SKIP_Set(rp_p->rp_addressHint);
		}
            else
                {
                /*
                 * got tower, convert to userset and copy it in
                 */
                user_set_p = (Set_t *)rp_p->rp_addressHint;
                /*
                 * note that the tower can't be bigger than one attribute
                 * value... or it won't fit into the replica pointer
                 */
                build_random_set(set_p, user_set_p, 0, MAXATTRIBUTE);
                next_p = SKIP_Set(rp_p->rp_addressHint);
                }

            PUT8(next_p, RT_master);
            COPY_FullName(resolvedName_p, next_p);
            next_p = SKIP_FullName(next_p);

            INS16(replica_member_p->mv_length,
		next_p - replica_member_p->mv_value);
	    INS8(replica_member_p->mv_valuetype, VT_ReplicaPointer);
            INS16(chlist_p->st_valueLength, LEN_SetMember(replica_member_p));
            SET_FLAG(progress_p->pr_flags, PR_up);
            return(DNS_NOTFND);
	default:
	    return(DNS_UNKNOWNENTRY);
        }
    else
	return(DNS_UNKNOWNENTRY);
}


/* -------------------------------------------------------------------------- */

/*
 * process_ch
 *   	Looking for the clearinghouse pseudo directory.
 *	See if this is the correct place to be.
 *
 * Input:
 *	data_p = lookup data
 *	    d_cnt = count of unresolved names
 *	    d_stream_p = pointer to stream descriptor
 *	    d_progress_p = pointer to progress record
 *	    d_optype = type of request
 *
 * Output:
 *
 * Function Value:
 *        DNS_SUCCESS - found what we where looking for
 *        DNS_UNKNOWNENTRY - found object, not looking for it
 *
 */
static dns_status_t
process_ch (data_t *const data_p)
{
    register db_stream_t *stream_p = data_p->d_stream_p; /* cache this */
    register Progress_t *progress_p = data_p->d_progress_p; /* cache this */
    FullName_t		*unresolvedName_p =
			    (FullName_t *)progress_p->pr_unresolvedName;
    FullName_t		*resolvedName_p =
			    (FullName_t *)SKIP_FullName(unresolvedName_p);
    nsgbl_ch_t		*ch_p = stream_p->db_file_p->file_ch_p;
    Set_t		*chlist_p;
    MemberValue_t	*member_p;
    dns_status_t	status;

    if (data_p->d_cnt == 0)
	{
        /* There can be only one here */
        if (NE_ObjUID(&ch_p->ch_uid, unresolvedName_p->fn_root))
            return(DNS_UNKNOWNENTRY);
        else
            {
	    /* This is the right clearinghouse  */
	    if (data_p->d_optype == WALKTREE_LOOKUP_OP)
                status = db_pseudo_read(stream_p);
	    else
                status = db_pseudo_read_for_update(stream_p);
	    if (status != DNS_SUCCESS) return(status);
            }
        }
    else
        {
	/* See if the names match */
	status = build_key(data_p);
	if (status != DNS_SUCCESS) return(status);

        if (dnsCmpFull((dns_opq_fname_t *)stream_p->db_key,
		       (dns_opq_fname_t *)&ch_p->ch_name) != 0)
	    /* Names don't match, so walktree looking for object */
	    return(DNS_SUCCESS);

	/* This is the right clearinghouse */
	if (data_p->d_optype == WALKTREE_LOOKUP_OP)
            status = db_pseudo_read(stream_p);
	else
            status = db_pseudo_read_for_update(stream_p);
        if (status != DNS_SUCCESS) return(DNS_SUCCESS);

        data_p->d_cnt = 0;
	build_return_name(&resolvedName_p, unresolvedName_p, &ch_p->ch_uid, data_p->d_cnt);
        }

    CLEAR_FLAG(progress_p->pr_flags, PR_up);
    SET_FLAG(progress_p->pr_flags, PR_directory);

    /* Build hint */
    chlist_p = (Set_t *)SKIP_FullName(resolvedName_p);
    INS8(chlist_p->st_flag, ST_present);
    INS16(chlist_p->st_offsetLength, sizeof(word_u));
    member_p = (MemberValue_t *)DATA_Set(chlist_p);
    INS16(OFFSET_Set(chlist_p, 0), DIFF_bytes(member_p, chlist_p));
    INS8(member_p->mv_flag, MV_present);
    /* what about member_p->mv_timestamp ? */
    INS8(member_p->mv_valuetype, VT_ReplicaPointer);
    INS16(member_p->mv_length, replica_build(
			(ReplicaPointer_t *)member_p->mv_value,
			&ch_p->ch_uid, NULL,
			RT_master, &ch_p->ch_name));
    INS16(chlist_p->st_valueLength, LEN_MemberValue(member_p));
    return(DNS_SUCCESS);
}


/* -------------------------------------------------------------------------- */

/*
 * walktree_assist
 *	Try to find some parent directory in this clearinghouse
 *
 * Input:
 *	data_p = lookup data
 *	    d_cnt = count of unresolved names
 *	    d_stream_p = pointer to stream descriptor
 *	    d_progress_p = pointer to progress record
 *
 */
static dns_status_t
walktree_assist (data_t *const data_p)
{
    register db_stream_t *const stream_p = data_p->d_stream_p;
    register Progress_t *const progress_p = data_p->d_progress_p;
    FullName_t	*const	key_p = (FullName_t *)stream_p->db_key;
    int			i;
    Set_t               *chlist_p;
    dns_status_t        status;
    FullName_t          test_name;
    cdsNameRef_t        name_ref;
    

    catenate_name(data_p, &test_name);

    name_ref = name_in_cell_test(stream_p->db_file_p,
                                 &test_name, 1);

    if (name_ref != cdsLocalRef) {
        data_p->d_for_other_cell = TRUE;
        return(DNS_UNKNOWNENTRY);
    }

    for (i = data_p->d_cnt; 0 < i; --i) {
	SimpleName_t *const last_p = (SimpleName_t *)names_pointlast(key_p);

	stream_p->db_dir_hash_p = NULL;
	INS16(key_p->fn_length,
	    EXT16(key_p->fn_length) - LEN_SimpleName(last_p));
	COPY_SimpleName(NullSimpleName, last_p);
	if (db_dir_read(stream_p) == DNS_SUCCESS) {
	    FullName_t *const unresolvedName_p =
		(FullName_t *)progress_p->pr_unresolvedName;
	    FullName_t *resolvedName_p =
		(FullName_t *)SKIP_FullName(unresolvedName_p);


	    /* Get CHlist if their is one */
	    chlist_p = (Set_t *)SKIP_FullName(resolvedName_p);
	    status = process_list(data_p, chlist_p);

	    /* If we can use the directory or have a useful chlist_p
             * then we are done.   Otherwise ignore this directory and try
             * for another.  If we return an empty set or a ptr to ourselves
	     * and don't complete
             * the request, the clerk will have no where to go.
	     * Better to return without resolving anything.
             */

	    if ((status == DNS_SUCCESS) || !EMPTY_Set(chlist_p)) {
		--i;
		data_p->d_cnt -= i;
		SET_FLAG(progress_p->pr_flags, PR_directory);
		build_return_name(&resolvedName_p, unresolvedName_p,
				  (ObjUID_t *)stream_p->db_dir.data_uid,
				  data_p->d_cnt);
		CLEAR_FLAG(progress_p->pr_flags, PR_up);
		return(status);

	    } else {
	        db_dir_release(stream_p);
            }
	}
    }

    data_p->d_for_other_cell = TRUE;
    return(DNS_UNKNOWNENTRY);
}


/* -------------------------------------------------------------------------- */

/*
 * lookup_any
 *	Lookup either an object, link or child pointer
 *
 * Input:
 *	data_p = lookup data
 *	    d_cnt = count of unresolved names
 *	    d_stream_p = pointer to stream descriptor
 *	    d_progress_p = pointer to progress record
 *
 */
static dns_status_t
lookup_any (data_t *const data_p)
{
    register db_stream_t *stream_p = data_p->d_stream_p; /* cache this */
    register Progress_t *progress_p = data_p->d_progress_p; /* cache this */
    FullName_t		*unresolvedName_p =
			    (FullName_t *)progress_p->pr_unresolvedName;
    FullName_t		*resolvedName_p =
			    (FullName_t *)SKIP_FullName(unresolvedName_p);
    dns_status_t	status;
    Set_t               *chlist_p;

    status = build_key(data_p);
    if (status != DNS_SUCCESS) return(status);

    status = db_dir_read(stream_p);
    if (status == DNS_SUCCESS)
	{
	/* Get CHlist if their is one */
	chlist_p = (Set_t *)SKIP_FullName(resolvedName_p);
	status = process_list(data_p, chlist_p);

	/* If we can use the directory or have a useful chlist_p
	 * then we are done.   Otherwise ignore this directory and try
	 * for another.  If we return an empty set or a ptr to ourselves
	 * and don't complete
	 * the request, the clerk will have no where to go.
	 * Better to return without resolving anything.
	 */
	if ((status == DNS_SUCCESS) || !EMPTY_Set(chlist_p))
	    {
	    data_p->d_cnt = 0;
	    SET_FLAG(progress_p->pr_flags, PR_directory);
	    build_return_name(&resolvedName_p, unresolvedName_p,
			      (ObjUID_t *)stream_p->db_dir.data_uid,
			      data_p->d_cnt);
	    CLEAR_FLAG(progress_p->pr_flags, PR_up);

	    return(status);
	    }
	else
	    db_dir_release(stream_p);

	}

    return(walktree_assist(data_p));
}


/* -------------------------------------------------------------------------- */

/*
 * lookup_entry
 *	Lookup up an entry in the directory.  Either object or softlink
 *    (or childptr for CDS)
 * Input:
 *	data_p = lookup data
 *	    d_cnt = count of unresolved names
 *	    d_stream_p = address of stream descriptor
 *	    d_progress_p = pointer to progress record
 *	    d_optype = type of operation
 *	    d_lookingfor = type of entry
 */
static dns_status_t
lookup_entry (data_t *const data_p)
{
    register db_stream_t *stream_p = data_p->d_stream_p; /* cache this */
    register Progress_t *progress_p = data_p->d_progress_p; /* cache this */
    FullName_t		*unresolvedName_p =
			    (FullName_t *)progress_p->pr_unresolvedName;
    FullName_t		*resolvedName_p =
			    (FullName_t *)SKIP_FullName(unresolvedName_p);
    DBSet_t		*set_p;
    dns_status_t	status;
    Set_t               *chlist_p;

    status = build_key(data_p);
    if (status != DNS_SUCCESS) return(status);

    stream_p->db_entry.data_type = data_p->d_lookingfor;
    /* Read operation will handle reading the directory */
    if (data_p->d_optype == WALKTREE_LOOKUP_OP)
	status = db_entry_read(stream_p);
    else
	status = db_entry_read_for_update(stream_p);

    if (status == DNS_SUCCESS)
	{
	set_p = (DBSet_t *)stream_p->db_entry.data_p->dbr_data;
	ALIGN_CHECK(set_p);
	if (EXT8(set_p->ds_flag) != DS_present)
	    {
	    status = DNS_UNKNOWNENTRY;
	    db_entry_release(stream_p);
	    }
	}
    else
        {
        db_entry_release(stream_p);
        }

    if (status == DNS_SUCCESS)
	{
        dns_status_t dir_status    ;        /* Holds directory read status */
	/* Get CHlist if their is one */
	chlist_p = (Set_t *)SKIP_FullName(resolvedName_p);
        dir_status = db_dir_read(stream_p);
        if (dir_status != DNS_SUCCESS)
	{
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug4,
                "lookup_entry() - db_dir_read->0x%x",
                dir_status));
        }
	status = process_list(data_p, chlist_p);

	if (status == DNS_SUCCESS)
	    status = operationallowed(data_p->d_optype,
		db_dir_get_replica_type(stream_p->db_dir_hash_p),
		data_p->d_lookingfor);


	/* If we can use the directory or have a useful chlist_p
	 * then we are done.   Otherwise ignore this directory and try
	 * for another.  If we return an empty set or a ptr to ourselves
	 * and don't complete
	 * the request, the clerk will have no where to go.
	 * Better to return without resolving anything.
	 */
	if ((status == DNS_SUCCESS) || !EMPTY_Set(chlist_p))
	    {
	    /* If directory type and state is okay, resolve entire name */
	    if (status == DNS_SUCCESS)
	      data_p->d_cnt = 0;
	    else {
		data_p->d_cnt = 1;
		SET_FLAG(progress_p->pr_flags, PR_directory);
	        }

	    build_return_name(&resolvedName_p, unresolvedName_p,
			      (ObjUID_t *)stream_p->db_dir.data_uid,
			      data_p->d_cnt);

	    CLEAR_FLAG(progress_p->pr_flags, PR_up);
	    return(status);
	    }
	else
	    {
	    db_dir_release(stream_p);
	    db_entry_release(stream_p);
	    }
	}

    return(walktree_assist(data_p));
}


/* -------------------------------------------------------------------------- */

/*
 * lookup_child
 *	Since a child doesn't have and access control entry, we must
 * 	also lookup the parent directory.
 *
 * Input:
 *	data_p = lookup data
 *	    d_cnt = count of unresolved names
 *	    d_stream_p = pointer to stream descriptor
 *	    d_progess_p = pointer to progress record
 *	    d_optype = type of operation
 */
static dns_status_t
lookup_child (data_t *const data_p)
{
    /* In CDS, child entries have their own access. */
    dce_assert(cds__svc_handle,
	       (data_p->d_lookingfor == ET_childPointer));
    return(lookup_entry(data_p));
}


/* -------------------------------------------------------------------------- */

/*
 * lookup_for_create
 *	Lookup a directory with the intention of creating
 *	an entry in the directory.  Make sure an entry of the
 *	same name doesn't already exist.
 *
 * Input:
 *	data_p = lookup data
 *	    d_cnt = count of unresolved names
 *	    d_stream_p = pointer to stream descriptor
 *	    d_progress_p = pointer to progress record
 *	    d_lookingfor = type of record
 */
static dns_status_t
lookup_for_create (data_t *const data_p)
{
    register db_stream_t *stream_p = data_p->d_stream_p; /* cache this */
    register Progress_t *progress_p = data_p->d_progress_p; /* cache this */
    FullName_t		*unresolvedName_p =
			    (FullName_t *)progress_p->pr_unresolvedName;
    FullName_t		*resolvedName_p =
			    (FullName_t *)SKIP_FullName(unresolvedName_p);
    FullName_t		*key_p = (FullName_t *)stream_p->db_key;
    SimpleName_t	*name_p;
    dns_status_t	status;
    Set_t               *chlist_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug4,
        " >lookup_for_create(%p)",
        data_p));

    dce_assert(cds__svc_handle,
	       (data_p->d_optype == WALKTREE_CREATE_OP));

    status = build_key(data_p);
    if (status != DNS_SUCCESS)
         {
         DCE_SVC_DEBUG((
             cds__svc_handle,
             cds_svc_server,
             svc_c_debug4,
             " <lookup_for_create() returned %d",
             status));
         return(status);
         }

    /* Strip off last simple name and find directory */
    name_p = (SimpleName_t *)names_pointlast(key_p);
    INS16(key_p->fn_length,
	EXT16(key_p->fn_length) - LEN_SimpleName(name_p));
    COPY_SimpleName(NullSimpleName, name_p);
    status = db_dir_read_for_update(stream_p);
    if (status == DNS_SUCCESS)
	{
	/* Get CHlist if their is one */
	chlist_p = (Set_t *)SKIP_FullName(resolvedName_p);
	process_list(data_p, chlist_p);

	if (status == DNS_SUCCESS)
	    status = operationallowed(data_p->d_optype,
			db_dir_get_replica_type(stream_p->db_dir_hash_p),
			data_p->d_lookingfor);

	/* If we can use the directory or have a useful chlist_p
	 * then we are done.   Otherwise ignore this directory and try
	 * for another.  If we return an empty set or a ptr to ourselves
	 * and don't complete
	 * the request, the clerk will have no where to go.
	 * Better to return without resolving anything.
	 */
	if ((status == DNS_SUCCESS) || !EMPTY_Set(chlist_p))
	    {
	    data_p->d_cnt = 1;
	    SET_FLAG(progress_p->pr_flags, PR_directory);
	    build_return_name(&resolvedName_p, unresolvedName_p,
			      (ObjUID_t *)stream_p->db_dir.data_uid,
			      data_p->d_cnt);
	    CLEAR_FLAG(progress_p->pr_flags, PR_up);
	    if (status != DNS_SUCCESS)
                {
                DCE_SVC_DEBUG((
                         cds__svc_handle,
                         cds_svc_server,
                         svc_c_debug4,
                         " <lookup_for_create() returned %d",
                         status));
                return(status);
                }

	    /* Now see if the entry exists */
	    COPY_FullName(unresolvedName_p, key_p);
	    status = check_exists(stream_p, WALKTREE_LOOKUP_OP);
	    if (status == DNS_SUCCESS)      status = DNS_ENTRYEXISTS;
	    if (status == DNS_UNKNOWNENTRY) status = DNS_SUCCESS;
            DCE_SVC_DEBUG((
                         cds__svc_handle,
                         cds_svc_server,
                         svc_c_debug4,
                         " <lookup_for_create() returned %d",
                         status));

	    return(status);
	    }
	else
	  db_dir_release(stream_p);
	}

    /* Rebuild key */
    status = build_key(data_p);
    if (status != DNS_SUCCESS)
         {
         DCE_SVC_DEBUG((
             cds__svc_handle,
             cds_svc_server,
             svc_c_debug4,
             " <lookup_for_create() returned %d",
               status));
         return(status);
         }

    status = walktree_assist(data_p);

    DCE_SVC_DEBUG((
             cds__svc_handle,
             cds_svc_server,
             svc_c_debug4,
             " <lookup_for_create() returned %d",
               status));

    return(status);
}


/* -------------------------------------------------------------------------- */

/*
 * lookup_dir
 *	Looking for a directory, see if it or some parent exists here.
 *
 * Input:
 *	data_p = lookup data
 *	    d_cnt = count of unresolved names
 *	    d_stream_p = pointer to stream descriptor
 *	    d_progress_p = pointer to progress record
 *	    d_optype = operation
 */
static dns_status_t
lookup_dir (data_t *const data_p)
{
    register db_stream_t *stream_p = data_p->d_stream_p; /* cache this */
    register Progress_t *progress_p = data_p->d_progress_p; /* cache this */
    FullName_t		*unresolvedName_p =
			    (FullName_t *)progress_p->pr_unresolvedName;
    FullName_t		*resolvedName_p =
			    (FullName_t *)SKIP_FullName(unresolvedName_p);
    dns_status_t	status;
    int                 state;
    Set_t               *chlist_p;

    dce_assert(cds__svc_handle,
	       (data_p->d_lookingfor == ET_directory));

    if (data_p->d_cnt == 0)
	{
	stream_p->db_dir_hash_p = NULL;
	/* Use UID to build full name */
	names_new((ObjUID_t *)unresolvedName_p->fn_root, NULL,
	    (FullName_t *)stream_p->db_key);

	if (data_p->d_optype == WALKTREE_LOOKUP_OP)
	    status = db_dir_read(stream_p);
	else
	    status = db_dir_read_for_update(stream_p);

	if (status != DNS_SUCCESS) return(status);

	/* V2.0 allows client to request response irregardless of state */
	if (FLAG_IS_CLEAR(progress_p->pr_flags, PR_ignoreState)) {
	  state = db_dir_get_replica_state(stream_p->db_dir_hash_p);
	  if (data_p->d_optype != WALKTREE_SKULK_OP) {
	    if (state != RS_on)
	      return(DNS_NOTFND);
	  } else
	    /* Skulk is more liberal then most */
	    if ((state != RS_on) && (state != RS_newDir) &&
		(state != RS_dyingDir))
	      return(DNS_NOTFND);
	}

	status = operationallowed(data_p->d_optype,
		db_dir_get_replica_type(stream_p->db_dir_hash_p),
		data_p->d_lookingfor);

	if (status != DNS_SUCCESS) return(status);
	CLEAR_FLAG(progress_p->pr_flags, PR_up);
	SET_FLAG(progress_p->pr_flags, PR_directory);
	/* If looking for root and found it, copy id to the resolved name */
	if (EMPTY_FullName(resolvedName_p))
	  COPY_ObjUID(unresolvedName_p->fn_root, resolvedName_p->fn_root);

	return(status);
	}
    else
	{
	/*
	 * Start with the longest name and backup until we find a
	 * directory
	 */
	status = build_key(data_p);
	if (status != DNS_SUCCESS) return(status);

	if (data_p->d_optype == WALKTREE_LOOKUP_OP)
	    status = db_dir_read(stream_p);
	else
	    status = db_dir_read_for_update(stream_p);

	/* If the directory is here, we are done */
	if (status == DNS_SUCCESS)
	    {
	    /* V2.0 allows client to request response irregardless of state */
	    if (FLAG_IS_CLEAR(progress_p->pr_flags, PR_ignoreState)) {
	      state = db_dir_get_replica_state(stream_p->db_dir_hash_p);
	      if (data_p->d_optype != WALKTREE_SKULK_OP) {
		if (state != RS_on)
		  status = DNS_NOTFND;
	       } else
		 /* Skulk is more liberal then most */
		 if ((state != RS_on) && (state != RS_newDir) &&
		     (state != RS_dyingDir))
		   status = DNS_NOTFND;
	         }

	    /* Make sure replica type here can perform operation */
	    if (status == DNS_SUCCESS)
	        status = operationallowed(data_p->d_optype,
			  db_dir_get_replica_type(stream_p->db_dir_hash_p),
					  data_p->d_lookingfor);

	    /* Get CHlist if their is one */
	    chlist_p = (Set_t *)SKIP_FullName(resolvedName_p);
	    process_list(data_p, chlist_p);

	    /* If we can use the directory or have a useful chlist_p
	     * then we are done.   Otherwise ignore this directory and try
             * for another.  If we return an empty set or a ptr to ourselves
	     * and don't complete
	     * the request, the clerk will have no where to go.
	     * Better to return without resolving anything.
	     */
	    if ((status == DNS_SUCCESS) || !EMPTY_Set(chlist_p))
	        {
		data_p->d_cnt = 0;
		SET_FLAG(progress_p->pr_flags, PR_directory);
		build_return_name(&resolvedName_p, unresolvedName_p,
				  (ObjUID_t *)stream_p->db_dir.data_uid,
				  data_p->d_cnt);

		CLEAR_FLAG(progress_p->pr_flags, PR_up);
		return(status);
	        }
	    else
	        db_dir_release(stream_p);
            }
	/* Look for a parent directory */
	return(walktree_assist(data_p));
        }
}


/* -------------------------------------------------------------------------- */

/*
 * walktree
 *   This routine searches for an entry at a particular clearinghouse.
 * It progresses as far as possible.  The input rab and key buffers must be
 * used only by this thread, and must not be on the stack because
 * read asynchronous operations will be done.  The routine must
 * not be called when there are any locks held by this thread.
 *
 *
 * Input:
 *      user_p = address of client authentication information
 *      lookingfor = type of entry we are looking for
 *      optype = flag to indicate whether a create is in progress
 *      progress_p = address of progress_record
 *      stream_p = address of stream descriptor
 *
 * Output:
 *     *progress_p updated
 *     *rec_p updated
 *     *dir_p updated
 *
 * Function value:
 *      DNS_SUCCESS - found entry
 *      DNS_NOTFND - entry not found here, but progress probably made
 *      DNS_NAMESERVERBUG - invalid clerk input
 *      DNS_UNKNOWNENTRY - didn't find what we wanted
 */
dns_status_t
walktree (user_descriptor_t  *user_p,
          int                lookingfor,
          int                optype,
          Progress_t         *progress_p,
          db_stream_t        *stream_p)
{
    DEB_ASCII_BUF_EntryType(etypeBuf)
    data_t		data;
    FullName_t		*unresolvedName_p =
			    (FullName_t *)progress_p->pr_unresolvedName;
    dns_status_t	status;

    data.d_stream_p = stream_p;
    data.d_user_p = user_p;
    data.d_progress_p = progress_p;
    data.d_lookingfor = lookingfor;
    data.d_optype = optype;
    data.d_for_other_cell = FALSE;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " >walktree(,%s,%d)",
        deb_ascii_EntryType(etypeBuf,&data.d_lookingfor),
        optype));

    CLEAR_FLAG(progress_p->pr_flags, PR_hitLink | PR_directory);

    /* Determine number of names still unresolved */
    data.d_cnt = dnsCountSimple((dns_opq_fname *)unresolvedName_p);

    if (lookingfor == ET_clearinghouse)
        {
	status = process_ch(&data);
	if (status == DNS_SUCCESS)
	    {
	    if (data.d_cnt == 0)
	        {
	        if (optype == WALKTREE_LOOKUP_OP)
	            {
                    INC_SRV_CTR(user_p->ud_ns_p, ns_ReadAccesses);
                    INC_CH_CTR(stream_p->db_file_p->file_ch_p, ch_ReadAccesses);
	            }
	        else
	            {
                    INC_SRV_CTR(user_p->ud_ns_p, ns_WriteAccesses);
                    INC_CH_CTR(stream_p->db_file_p->file_ch_p, ch_WriteAccesses);
	            }
		goto exit_walktree; /* status == DNS_SUCCESS */
		}

	    data.d_lookingfor = ET_object; /* masquerades as object */
	    status = lookup_entry(&data);
	    data.d_lookingfor = lookingfor;
	    if ((status == DNS_SUCCESS) && (data.d_cnt == 0))
		status = process_obj(&data);
	    }
	}
    else if (lookingfor == ET_directory)
	status = lookup_dir(&data);
    else if (optype == WALKTREE_CREATE_OP)
	{
	status = lookup_for_create(&data);
	if (status == DNS_ENTRYEXISTS)
	    {
            INC_SRV_CTR(user_p->ud_ns_p, ns_WriteAccesses);
            INC_CH_CTR(stream_p->db_file_p->file_ch_p, ch_WriteAccesses);
            goto exit_walktree;
            }

	/* Entry doesn't exist, return to create it */
	if ((status == DNS_SUCCESS) && (data.d_cnt == 1))
	    {
            INC_SRV_CTR(user_p->ud_ns_p, ns_WriteAccesses);
            INC_CH_CTR(stream_p->db_file_p->file_ch_p, ch_WriteAccesses);
	    CLEAR_FLAG(progress_p->pr_flags, PR_directory);
            goto exit_walktree;
	    }
	}
    else switch (lookingfor) {
    case ET_childPointer:
	status = lookup_child(&data);
	break;
    case ET_anyDirectName:
	status = lookup_any(&data);
	break;
    case ET_dirOrObj:
	/* Check for object first since this is the most common */
	data.d_lookingfor = ET_object;
	status = lookup_entry(&data);
	if ((status == DNS_SUCCESS) && (data.d_cnt == 0))
	    break;

	/* At this point we have resolved all but that last name.  This means
	 * we have already gone through walktree_assist.  Now just check
	 * to see if the last name is a directory
	 * Clear out stream, so no hash_p left to mess up what actually
	 * gets read.
	 */
	db_dir_release(stream_p);
	db_entry_release(stream_p);
	data.d_lookingfor = ET_directory;
	status = lookup_dir(&data);
	break;
    default:
	status = lookup_entry(&data);
	break;
    }

    if (status == DNS_UNKNOWNENTRY)
	{
        /* Point clerk back up tree */
	FullName_t *resolvedName_p =
	    (FullName_t *)SKIP_FullName(unresolvedName_p);

        if (FLAG_IS_SET(progress_p->pr_flags, PR_up) &&
		EMPTY_FullName(resolvedName_p))
             status = blduppointers(&data,
		(Set_t *)SKIP_FullName(resolvedName_p));
	else
	    status = DNS_NOTFND;
	}

    /* If didn't find anything, stop here */
    if (status != DNS_SUCCESS)
	{
	if (status == DNS_NOTFND)
	    {
            INC_CH_CTR(stream_p->db_file_p->file_ch_p, ch_ReferencesReturned);
            }
        else if (optype == WALKTREE_LOOKUP_OP)
	    {
            INC_SRV_CTR(user_p->ud_ns_p, ns_ReadAccesses);
            INC_CH_CTR(stream_p->db_file_p->file_ch_p, ch_ReadAccesses);
	    }
	else
	    {
            INC_SRV_CTR(user_p->ud_ns_p, ns_WriteAccesses);
            INC_CH_CTR(stream_p->db_file_p->file_ch_p, ch_WriteAccesses);
	    }

	db_close_stream(stream_p);
	goto exit_walktree;
	}

    /* If entire name is now successfully resolved, return */
    if (data.d_cnt == 0)
        {
        if (optype == WALKTREE_LOOKUP_OP)
	    {
            INC_SRV_CTR(user_p->ud_ns_p, ns_ReadAccesses);
            INC_CH_CTR(stream_p->db_file_p->file_ch_p, ch_ReadAccesses);
	    }
	else
	    {
            INC_SRV_CTR(user_p->ud_ns_p, ns_WriteAccesses);
            INC_CH_CTR(stream_p->db_file_p->file_ch_p, ch_WriteAccesses);
	    }
	goto exit_walktree; /* status == DNS_SUCCESS */
        }

    stream_p->db_dir_hash_p = NULL;
    /* Build key of unresolved uid and first simple name */
    names_new((ObjUID_t *)unresolvedName_p->fn_root,
	(SimpleName_t *)unresolvedName_p->fn_name,
	(FullName_t *)stream_p->db_key);
    status = check_exists(stream_p, WALKTREE_LOOKUP_OP);
    if (status == DNS_SUCCESS)
	{
	--data.d_cnt;
	/* Resolve name now */
	names_swap(unresolvedName_p);
	CLEAR_FLAG(progress_p->pr_flags, PR_directory);
	switch (stream_p->db_entry.data_type)
	    {
	    case ET_object:
		status = process_obj(&data);
		break;
	    case ET_childPointer:
		status = process_child(&data);
		break;
	    case ET_softlink:
		status = process_link(&data);
		break;
	    }
	}

    if (status == DNS_NOTFND)
        {
        INC_CH_CTR(stream_p->db_file_p->file_ch_p, ch_ReferencesReturned);
        }
    else if (optype == WALKTREE_LOOKUP_OP)
        {
        INC_SRV_CTR(user_p->ud_ns_p, ns_ReadAccesses);
        INC_CH_CTR(stream_p->db_file_p->file_ch_p, ch_ReadAccesses);
	}
    else
	{
        INC_SRV_CTR(user_p->ud_ns_p, ns_WriteAccesses);
        INC_CH_CTR(stream_p->db_file_p->file_ch_p, ch_WriteAccesses);
	}

    if (status != DNS_SUCCESS)
	db_close_stream(stream_p);

exit_walktree:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug3,
        " <walktree(,%s,%d) return(%d)",
        deb_ascii_EntryType(etypeBuf,&data.d_lookingfor),
        optype,
        status));

    return(status);
}
