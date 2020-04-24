/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: back_hcell_connections.c,v $
 * Revision 1.1.4.2  1996/02/18  19:35:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:43  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:28:35  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:23  root]
 * 
 * Revision 1.1.2.4  1994/08/03  19:03:31  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:18:35  mccann]
 * 
 * Revision 1.1.2.3  1994/07/08  16:16:58  zee
 * 	     HCell inheritance: create back_hcell_inheritance routine.
 * 	[1994/07/07  23:55:02  zee]
 * 
 * Revision 1.1.2.2  1994/06/09  18:42:49  devsrc
 * 	     cr10871 - expand copyright
 * 	     [1994/06/09  18:15:05  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/06  16:04:14  zee
 * 	     Initial version.
 * 	     [1994/05/06  16:03:06  zee]
 * 
 * $EndLog$
 */
/*
 * Module: back_hcell_connections.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1994. ALL RIGHTS RESERVED.
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
#include <sets.h>
#include <tlog.h>
#include <tower.h>
#include <uid.h>
#include <hcell.h>


/*
 * rps_element_t is a local data structure used by the routines in this
 * module only.  It is used to create a simple form of a replica set
 * that can be easily manipulated regardless of its origination.
 *
 * The replica sets are stored as a single linked list with pointers
 * to the actual ReplicaPointer data.
 *
 * The source_buffer_p is a piggyback buffer management optimization:
 * replica sets retrieved by the clerk have to be held in temporary
 * read buffers.  We record the buffers here so we can free them when
 * we deallocate the storage for the set.
 */

typedef struct rps_element {
    struct rps_element *next_p;           /* Next rps_element in this set */
    void               *source_buffer_p;  /* Source buffer (optional) */
    int                 op;               /* Used to mark elements */
    ReplicaPointer_t   *rp_p;             /* Pointer to the replicapointer */
} rps_element_t;


/* ------------------------------------------------------------------------- *\

/*
 * compare_replicapointers
 *
 * This routine compares two replicapointers and returns an integer
 * representing the results of the comparison:
 *
 *   0 - replicapointers are functionally identical
 *   1 - replicapointers are different
 *  -1 - replicapointers are the same CH, but a different address
 */

static int
compare_replicapointers (ReplicaPointer_t *rp1_p,
                         ReplicaPointer_t *rp2_p,
                         void             *work_buffer_p)
{

    ReplicaType_t *rt1_p, *rt2_p;
    dns_opq_fname_t *ch1_p, *ch2_p;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug8,
		   " >compare_replicapointers()"));

    if (NE_ObjUID(rp2_p->rp_CHID, rp1_p->rp_CHID)) {
	return 1;
    }

    /*
     * don't check the clearinghouse's address yet...
     */

    rt2_p = (ReplicaType_t *)SKIP_Set(rp2_p->rp_addressHint);
    rt1_p = (ReplicaType_t *)SKIP_Set(rp1_p->rp_addressHint);
    if (EXT8(rt2_p) != EXT8(rt1_p)) {
	return 1;
    }

    ch1_p = (dns_opq_fname_t *)SKIP_ReplicaType((void *)rt1_p);
    ch2_p = (dns_opq_fname_t *)SKIP_ReplicaType((void *)rt2_p);
    if (dnsCmpFull(ch1_p, ch2_p) != 0) {
	return(1);
    }

    /*
     * We have identical replica pointers.  Now check for network
     * location changes.
     */

    /*
     * Tower comparison is destructive to second set: copy
     * it.  Make sure tower_copy is big enough.
     */

    dce_assert(cds__svc_handle,
	       (LEN_Set(rp2_p->rp_addressHint) < READ_BUF_SIZE));

    COPY_Set(rp2_p->rp_addressHint, work_buffer_p);
    if (towerset_compare_exact((Set_t *)rp1_p->rp_addressHint,
			       (Set_t *)work_buffer_p) == 0) {
        return 0;
    } else {
        return -1;
    }
}

/* ------------------------------------------------------------------------- *\

/*
 * add_rps_element
 *
 * Adds an element to a replica pointer synchronization set.
 *
 * Returns: SUCCESS or NONSRESOURCES
 */

static dns_status_t
add_rps_element (rps_element_t   **set_pp,
                 void             *source_buffer_p,
                 int               op,
                 ReplicaPointer_t *rp_p)
{
    rps_element_t *work_p;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug7,
		   " >add_rps_element()"));

    work_p = (rps_element_t *)dns_malloc(sizeof(rps_element_t));
    if (work_p == NULL) {
        return DNS_NONSRESOURCES;
    }
    work_p->next_p = *set_pp;
    *set_pp = work_p;
    work_p->source_buffer_p = source_buffer_p;
    work_p->op = op;
    work_p->rp_p = rp_p;
    return DNS_SUCCESS;
}

/* ------------------------------------------------------------------------- *\

/*
 * purge_rps_set
 *
 * This routine purges an rps set from memory.  It also will free up
 * any source buffers associated with the set.
 */

static void
purge_rps_set (rps_element_t **set_pp)
{
    rps_element_t *work_p;
    rps_element_t *flush_p;

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug8,
		   " >purge_rps_set()"));

    work_p = *set_pp;
    while (work_p) {
        if (work_p->source_buffer_p) {
            for (flush_p = work_p->next_p;
                 flush_p != NULL;
                 flush_p = flush_p->next_p) {
                if (flush_p->source_buffer_p == work_p->source_buffer_p) {
                    flush_p->source_buffer_p = NULL;
                }
            }
            dns_free(work_p->source_buffer_p);
        }
        flush_p = work_p;
        work_p = work_p->next_p;
        dns_free(flush_p);
    }

    *set_pp = NULL;
}

/* ------------------------------------------------------------------------- *\

/*
 * mark_differences
 *
 * This routine compares the target set with a shadow set and designates
 * which replicapointers are different and which are the same.  The resulting
 * sets will have op != 1 where there is a difference between the two
 * replica sets.  Identical replica pointers are "removed" from both sets,
 * while a replica pointer with a new address will only "remove" the shadow's
 * pointer.   The resulting sets can be used to generate add and remove
 * operations on the shadow set.
 */

static void
mark_differences (rps_element_t *target_set_p,
                  rps_element_t *shadow_set_p,
                  void          *work_buffer_p)
{
    rps_element_t *target_p;
    rps_element_t *shadow_p;
    int rp_match;                    /* holds result of RP comparison */

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug8,
		   " >mark_differences()"));

    for (target_p = target_set_p;
         target_p != NULL;
         target_p = target_p->next_p) {
        for (shadow_p = shadow_set_p;
             shadow_p != NULL;
             shadow_p = shadow_p->next_p) {
            rp_match = compare_replicapointers(target_p->rp_p,
                                               shadow_p->rp_p,
                                               work_buffer_p);
            if (rp_match == 0) {
                shadow_p->op = 1;
                target_p->op = 1;
            } else if (rp_match < 0) {
                shadow_p->op = 1;
            }
        }
    }
}

/* ------------------------------------------------------------------------- */

/*
 * load_set_local
 *
 * This routine loads an rps set with the active replica pointers of a
 * specified database attribute (from the root directory).
 */
                            
static dns_status_t
load_set_local (rps_element_t **set_pp,         /* What set to load */
                db_stream_t    *stream_p,       /* Access to directory */
                int             attribute_id)   /* Which attribute to use */
{

    dir_hash_entry_t	*hash_p = stream_p->db_dir_hash_p;
    DBSet_t		*set_p;                 /* The attribute set */ 
    int                  index;                 /* Work variable */
    dns_status_t         status;
    DBSetMember_t	*member_p;              /* Attribute value */


    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug8,
		   " >load_set_local()"));

    /*
     * If this isn't the root directory for the cell, something is amiss
     */

    if (!hash_p->dir_root) {
	return(DNS_NAMESERVERBUG);
    }

    status = find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(attribute_id), &set_p, &index);
    if (status != DNS_SUCCESS) {
	evl_AttributeMissing(stream_p->db_file_p->file_ch_p,
			     (FullName_t *)hash_p->dir_name, 
			     dns_AttributeName(attribute_id));
        return(DNS_NAMESERVERBUG);
    }               

    for (index = 0; index < set_p->ds_ocnt; index++) {
        member_p = (DBSetMember_t *)INDEX_DBSet(set_p, index);
        if (!EXT8(member_p->dm_value_present)) continue;
	if (EXT8(member_p->dm_valuetype) != VT_ReplicaPointer) continue;
        if (!EXT8(member_p->dm_valid)) continue;

        status = add_rps_element(set_pp, NULL, 0,
                                 (ReplicaPointer_t *)member_p->dm_data);
        if (status != DNS_SUCCESS) {
            break;
        }
    }                             

    return status;
}

/* ------------------------------------------------------------------------- */

/*
 * load_set_remote
 *
 * This routine loads a specified rps set with the active members of
 * a replica set which is retrieved from the cell namespace (via the
 * clerk).   Unlike the load case (above) this routine also dynamically
 * allocates read buffers for the clerk transactions.  We record the
 * buffers in the rps set, so they can be freed later (when not needed
 * anymore).
 */

static dns_status_t
load_set_remote (rps_element_t **set_pp,        /* What set to load */
                 FullName_t     *name_p,        /* Cell entry to examine */
                 int             entry_type)    /* Cell entry type */
{
    byte_t      *return_buffer_p;            /* Allocated buffer for read */
    Set_t       *return_set_p;               /* Access to data inside buffer */
    int          pos, cnt;                   /* Work variables */
    byte_t       iamlinked;                  /* Junk -- returned by read */
    dns_status_t status;


    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug2,
		   " >load_set_remote()"));

    do  {
	Timestamp_t *ctx = NullTimestamp;

	/*
	 * Allocate space for return set, update packet and possible
	 * resolved name, also an extra space for comparing towers
	 */

	return_buffer_p = dns_malloc(READ_BUF_SIZE);
	if (!return_buffer_p) {
	    return(DNS_NONSRESOURCES);
	}

	return_set_p = (Set_t *)SKIP_AttributeType(return_buffer_p);

	status = clerk_read_attribute(name_p, entry_type,
				      dns_AttributeName(DNS_REPLICAS),
				      return_buffer_p, ctx, &iamlinked);
	
        if (!((status == DNS_SUCCESS) || (status == DNS_MOREDATA))) {
            dns_free(return_buffer_p);	/* Didn't need it */
            break;
        }

        /*
         * For each returned member, add it to the replicapointer sync set.
 	 */

	cnt = NUM_Set(return_set_p);
	for (pos = 0; pos < cnt; ++pos) {
            dns_status_t add_status;
	    MemberValue_t *const member_p =
	      (MemberValue_t *)INDEX_Set(return_set_p, pos);
            add_status = add_rps_element(set_pp, return_buffer_p, 0,
				     (ReplicaPointer_t *)member_p->mv_value);
            if (add_status != DNS_SUCCESS) {
                status = add_status;
                break;
            }
            ctx = (Timestamp_t *)member_p->mv_timestamp;
	}

    } while (status == DNS_MOREDATA);

    return status;
}


/* ------------------------------------------------------------------------- */

/* 
 * back_hcell_connections
 *
 * This routine performs the following functions:
 *
 *  1. It synchronizes the childpointer (in the parent cell) with
 *     our root directory's replica set.    This makes sure that access
 *     to our cell from a cell higher in the hierarchy can reach any of
 *     the local cell's clearinghouses.
 *
 *  2. It synchronizes the local CDS_ParentCellPointers with the replica
 *     set for the childpointer (located in the parent cell).  This assures
 *     that non-GDA inter-cell references can be made to the appropriate
 *     clearinghouses in the parent cell.
 *
 * This routine uses a common set of routines for both tasks.
 * The routines operate on a convention of "target" sets and "shadow"
 * sets.  Target sets are considered to be the "master" information, and
 * the point of all the gibberish below is to determine the differences
 * between the two sets and generate a set of updates (either remote or
 * local) that brings the shadow set in synchronization with the target
 * set.
 */

dns_status_t 
back_hcell_connections (db_stream_t  *stream_p)
{
    bytes_u	      update[sizeof(Update_u) - MAXATTRIBUTE +
				sizeof(ReplicaPointer_u)];
    bytes_u	      tower_copy[READ_BUF_SIZE];
    AtomicValue_t    *av_p;
    int		      cnt, pos, index;
    dns_status_t      status;
    int               deltas = 0;           /* Deltas detected */
    byte_t           *tlog_buf_p;           /* Alloc'd tlog updates */
    tlog_dsc_t        tlog;                 /* Transaction log update  */
    Update_t         *update_p;             /* Update work buffer */
    FullName_t        parent_name;          /* Parent of cell_name */
    FullName_t       *cell_name_p;          /* Current cell name */
    rps_element_t    *target_set_p = NULL;  /* Replica set to sync to */
    rps_element_t    *shadow_set_p = NULL;  /* Replica set needing syncing */
    rps_element_t    *work_p;               /* Work index */
    Timestamp_u       ts;                   /* New attribute value stamp */

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug2,
		   " >back_hcell_connections()"));

    /*
     * Read with intent to write
     */

    db_dir_release(stream_p);
    status = db_dir_read_for_update(stream_p);
    if (status != DNS_SUCCESS) {
        return(status);
   }


    /*
     * Set up and verify cell name, parent name, etc.
     */

    cell_name_p = (FullName_t *)stream_p->db_dir_hash_p->dir_name;
    if (cell_alias_test(stream_p->db_file_p, cell_name_p) != 1) {
        status = DNS_NAMESERVERBUG;
        goto exit_back_hcell_connections;
    }

    names_stripright(cell_name_p, &parent_name);

    /*
     *
     * First we reconcile the replicaset located in the childpointer
     * residing in the parent cell with our cell's root directory replicaset.
     *
     */

    status = load_set_local(&target_set_p, stream_p, DNS_REPLICAS);
    if (status != DNS_SUCCESS) {
        goto exit_back_hcell_connections;
    }

    status = load_set_remote(&shadow_set_p, cell_name_p, ET_childPointer);
    if (status != DNS_SUCCESS) {
        goto exit_back_hcell_connections;
    }

    mark_differences(target_set_p, shadow_set_p, tower_copy);


    /*
     * Apply the results of the set comparison as a series of update
     * transactions.
     * First we build an update structure that we can reuse for each
     * update.  We start of with deletes, and will poke the structure
     * to perform adds later.
     *
     * We ignore update failures as they will be caught later on (which
     * usually translates to "eventually").  At most this is a warning
     * condition, but we have no way of conveying that information.
     */

    av_p = (AtomicValue_t *)SKIP_bytes(update, 
                update_dns_build((Update_t *)update, UD_absent, NullTimestamp, 
                dns_Attribute(DNS_REPLICAS), BL_true, 0, NULL));

    for (work_p = shadow_set_p; work_p != NULL; work_p = work_p->next_p) {
        if (work_p->op == 1) continue;
        INS16(av_p->av_length, LEN_ReplicaPointer(work_p->rp_p));
        INS8(av_p->av_valuetype, VT_ReplicaPointer);
        COPY_bytes(work_p->rp_p, av_p->av_value, EXT16(av_p->av_length));

        (void)clerk_modify_attribute(cell_name_p,
		ET_childPointer, (Update_t *)update, LEN_Update(update));
    }

    /*
     * Switch update buffer to add mode.
     */

    INS8(((Update_t *)update)->ud_operation, UD_present);

    for (work_p = target_set_p; work_p != NULL; work_p = work_p->next_p) {
        if (work_p->op == 1) continue;
        INS16(av_p->av_length, LEN_ReplicaPointer(work_p->rp_p));
        INS8(av_p->av_valuetype, VT_ReplicaPointer);
        COPY_bytes(work_p->rp_p, av_p->av_value, EXT16(av_p->av_length));

        (void)clerk_modify_attribute(cell_name_p,
		ET_childPointer, (Update_t *)update, LEN_Update(update));
    }


    purge_rps_set(&target_set_p);
    purge_rps_set(&shadow_set_p);

    /*
     *
     * Next we reconcile our local CellParentPointers with the replicaset
     * for our intercell childpointer.
     *
     */

    status = load_set_local(&shadow_set_p, stream_p, DNS_PARENTCELLPOINTERS);
    if (status != DNS_SUCCESS) {
        goto exit_back_hcell_connections;
    }

    status = load_set_remote(&target_set_p, &parent_name, ET_directory);
    if (status != DNS_SUCCESS) {
        goto exit_back_hcell_connections;
    }

    mark_differences(target_set_p, shadow_set_p, tower_copy);

    /*
     * Calculate the number of deltas we have to make to the directory.
     * We need this number to allocate enough transaction log buffer
     * space for the directory updates.
     */

    for (work_p = shadow_set_p; work_p != NULL; work_p = work_p->next_p) {
        if (work_p->op == 1) continue;
        deltas++;
    }
    for (work_p = target_set_p; work_p != NULL; work_p = work_p->next_p) {
        if (work_p->op == 1) continue;
        deltas++;
    }

    /*
     * If the sets are the same (no deltas detected) then we're done...
     */

    if (deltas == 0) {
        goto exit_back_hcell_connections;
    }

    /*
     * Allocate enough space to store all the updates for transaction
     * logging.
     */

    tlog_buf_p = dns_malloc(sizeof(Update_t) * deltas);
    if (!tlog_buf_p) {
        status = DNS_NONSRESOURCES;
        goto exit_back_hcell_connections;
    }

    /* 
     * Set up update buffer
     */

    tlog.cnt = 0;
    tlog.len = 0;
    tlog.buf_p = tlog_buf_p;
    update_p = (Update_t *)tlog_buf_p;
    
    /*
     * Apply the results of the set comparison as a series of update
     * transactions.
     */

    for (work_p = shadow_set_p; work_p != NULL; work_p = work_p->next_p) {
        if (work_p->op == 1) continue;

	status = ts_new((Timestamp_t *)ts);
	if (status != DNS_SUCCESS) {
	    break;
	}
        av_p = (AtomicValue_t *)SKIP_bytes(update_p, 
                update_dns_build((Update_t *)update_p, UD_absent, 
				 (Timestamp_t *)ts,
				 dns_Attribute(DNS_PARENTCELLPOINTERS),
				 BL_true, 0, NULL));
        INS16(av_p->av_length, LEN_ReplicaPointer(work_p->rp_p));
        INS8(av_p->av_valuetype, VT_ReplicaPointer);
        COPY_bytes(work_p->rp_p, av_p->av_value, EXT16(av_p->av_length));
	(void)set_insert(&stream_p->db_dir, update_p,
			 dns_Attribute(DNS_PARENTCELLPOINTERS), TRUE);
	tlog.cnt++;
        tlog.len += LEN_Update(update_p);
	update_p = (Update_t *)SKIP_Update(update_p);
    }

    for (work_p = target_set_p; work_p != NULL; work_p = work_p->next_p) {
        if (work_p->op == 1) continue;

	status = ts_new((Timestamp_t *)ts);
	if (status != DNS_SUCCESS) {
	    break;
	}
        av_p = (AtomicValue_t *)SKIP_bytes(update_p, 
                update_dns_build((Update_t *)update_p, UD_present,
				 (Timestamp_t *)ts, 
				 dns_Attribute(DNS_PARENTCELLPOINTERS), 
				 BL_true, 0, NULL));
        INS16(av_p->av_length, LEN_ReplicaPointer(work_p->rp_p));
        INS8(av_p->av_valuetype, VT_ReplicaPointer);
        COPY_bytes(work_p->rp_p, av_p->av_value, EXT16(av_p->av_length));
	(void)set_insert(&stream_p->db_dir, update_p,
			 dns_Attribute(DNS_PARENTCELLPOINTERS), TRUE);
	tlog.cnt++;
        tlog.len += LEN_Update(update_p);
	update_p = (Update_t *)SKIP_Update(update_p);
    }


    /*
     * Apply the updates to the database (committed here).
     */

    if (tlog.cnt > 0) {
        if (status == DNS_SUCCESS) {
            status = db_dir_update(stream_p, &tlog);
        }
    }

    if (deltas > 0) {
	dns_free(tlog_buf_p);
    }


exit_back_hcell_connections:

    /*
     * Clean up and return.
     */

    purge_rps_set(&target_set_p);
    purge_rps_set(&shadow_set_p);

    db_dir_release(stream_p);

    DCE_SVC_DEBUG((cds__svc_handle, cds_svc_server, svc_c_debug2,
		   " <back_hcell_connections() returns %d", status));

    return status;
}



/* ------------------------------------------------------------------------- */

/*
 * back_hcell_inheritance
 *
 * This routine checks the name of a parent cell (if we are in a hierarchy)
 * and determines if the name has changed.  If so, we issue an add_cellname
 * as inherit command implicitly to the system.
 *
 */

dns_status_t 
back_hcell_inheritance (db_stream_t  *stream_p)
{

    dns_status_t   status;
    dir_hash_entry_t *hash_p;
    int            pos, num_mem;
    DBSet_t       *work_set_p;
    int            in_a_hierarchy = FALSE;
    int            is_root = FALSE;
    ObjUID_t       ch_uid;
    DBSetMember_t *preferred_member_p;  /* Pointer to preferred cell name */
    FullName_t     local_cell_name;     /* The local cell's name */
    SimpleName_t   local_cell_rhs;      /* Last simplename in the local cell */
    FullName_t     assumed_parent_name;
    FullName_t     current_parent_name;
    FullName_t     new_cell_name;

    /*
     * Access directory directly.
     */

    db_dir_release(stream_p);

    /*
     *  Lock down the root directory and retrieve all the information
     *  we need -- we'll evaluate it and act on that information after
     *  releasing the lock.
     *
     *  Walk through PARENTCELLPOINTER and check for valid entries
     *  to see if this cell is a valid child cell.
     *
     *  Then retrieve the local cell name.
     */

    hash_p = stream_p->db_dir_hash_p;
    SERVER_LOCK(hash_p->dir_mutex) {
        is_root = hash_p->dir_root;
	work_set_p = hash_p->dir_cellparent_set_p;
	if (work_set_p) {
	    num_mem = NUM_DBSet(work_set_p);
	    for (pos = 0; pos < num_mem; pos++) {
		DBSetMember_t *member_p;

		member_p = (DBSetMember_t *) INDEX_DBSet(work_set_p, pos);
		if (EXT8(member_p->dm_valid) &&
		    EXT8(member_p->dm_value_present)) {
		    in_a_hierarchy = TRUE;
		}
	    }
	}
	work_set_p = hash_p->dir_cellalias_set_p;
	preferred_member_p = NULL;
	if (work_set_p) {
	    status = get_preferred_member(work_set_p, &preferred_member_p);
	    if (status == CDS_SUCCESS) {
		/* Copy the preferred name (after the header byte) */
		COPY_FullName((FullName_t *)SKIP_byte(preferred_member_p->dm_data),
			      &local_cell_name);
	    }
	}
	COPY_ObjUID(&hash_p->dir_file_info_p->file_ch_p->ch_uid, &ch_uid);

    } SERVER_END_LOCK(stream_p->db_dir_hash_p->dir_mutex);


    /*
     * If we weren't pointed at the proper directory, it's a bug.
     */

    if (!is_root) {
	return DNS_NAMESERVERBUG;
    }

    /*
     * If we're not in a hierarchy, there's nothing left to do here.
     */

    if (!in_a_hierarchy) {
        return CDS_SUCCESS;
    }

    /*
     * If there is no preferred alias, it's a bug.
     */

    if (preferred_member_p == NULL) {
	return DNS_NAMESERVERBUG;
    }

    COPY_SimpleName(names_pointlast(&local_cell_name), &local_cell_rhs);
    names_stripright(&local_cell_name, &assumed_parent_name);

    status = clerk_get_preferred_alias(&assumed_parent_name,
				       &current_parent_name);
    if (status != CDS_SUCCESS) {
	return status;
    }

    /*
     * If the names match, then the parent cellname has not changed.
     * We can leave.
     */

    if (names_fullmatch(&assumed_parent_name, &current_parent_name)) {
	return CDS_SUCCESS;
    }

    COPY_FullName(&current_parent_name, &new_cell_name);
    names_append(&local_cell_rhs, &new_cell_name);
    status = add_cell_alias(NULL, &ch_uid, AsInherit, &new_cell_name);

    if (status == CDS_SUCCESS) {
	/*
	 * Servicability message???
	 */
    }

    return status;

}
