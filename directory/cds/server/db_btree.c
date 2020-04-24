/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_btree.c,v $
 * Revision 1.1.15.1  1996/08/09  11:59:51  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/08/06  14:52 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	Fix assertion failure in new_root() - don't unlink non-INDEX buckets.
 * 	[1996/02/18  19:35:42  marty  1.1.12.2]
 *
 * Revision 1.1.12.2  1996/02/18  19:35:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:05  marty]
 * 
 * Revision 1.1.12.1  1995/12/08  15:29:23  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/jrr_1.2_mothra/2  1995/11/17  14:28 UTC  jrr
 * 	Add count check in purge_data_bucket().
 * 
 * 	HP revision /main/jrr_1.2_mothra/1  1995/11/16  21:28 UTC  jrr
 * 	Fix for OT 12879 (database index corruption).  See peckham notes.
 * 
 * 	HP revision /main/HPDCE02/3  1995/07/28  15:34 UTC  peckham
 * 	Disable index checker from CHFts14444.
 * 
 * 	HP revision /main/HPDCE02/2  1995/07/27  04:11 UTC  peckham
 * 	Fix for CHFts14444 - rewrite routines for better index maintenance.
 * 	Add index integrity checker.
 * 
 * 	HP revision /main/HPDCE02/peckham_14444/4  1995/07/26  15:52 UTC  peckham
 * 	Supportability: add some comments - more needed
 * 	Added crash if index entry lost to leftmost data node.
 * 
 * 	HP revision /main/HPDCE02/peckham_14444/3  1995/07/23  06:23 UTC  peckham
 * 	Removing an orphan key can delete the sentinel,
 * 	which causes the horizontal chaining to break.
 * 	More rewrite to provide a general solution.
 * 
 * 	HP revision /main/HPDCE02/peckham_14444/2  1995/07/21  20:08 UTC  peckham
 * 	Add recursive node cleanup when entry deletion invalidates index pointers
 * 
 * 	HP revision /main/HPDCE02/peckham_14444/1  1995/07/06  15:15 UTC  peckham
 * 	Verify index consistency - first pass
 * 	[1995/12/08  14:48:42  root]
 * 
 * Revision 1.1.9.3  1994/08/03  19:03:46  mccann
 * 	fix assert
 * 	[1994/08/01  20:11:56  mccann]
 * 
 * 	fix asserts
 * 
 * 	final sams cleanup drop
 * 	[1994/07/19  18:20:41  mccann]
 * 
 * Revision 1.1.9.2  1994/06/09  18:43:00  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:21  devsrc]
 * 
 * Revision 1.1.9.1  1994/03/12  22:10:24  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:39:39  peckham]
 * 
 * Revision 1.1.7.1  1993/10/12  14:57:02  peckham
 * 	CR#7940: btree corruption in split_bucket_for_continuation
 * 	Link in new bucket, not old bucket being released.
 * 	[1993/10/12  14:29:36  peckham]
 * 
 * Revision 1.1.4.5  1993/02/23  19:08:34  peckham
 * 	OT#7329: Getting fixups on CDS from db_btree_build() -
 * 	fix databucket to be properly aligned.
 * 	[1993/02/23  19:06:47  peckham]
 * 
 * Revision 1.1.4.4  1992/12/30  14:20:00  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:20:08  zeliff]
 * 
 * Revision 1.1.4.3  1992/12/28  21:47:15  jd
 * 	Fix supplied by Al Peckham - Reverse the order the insert_records in the
 * 	function new_root.
 * 	[1992/12/28  21:46:41  jd]
 * 
 * Revision 1.1.4.2  1992/12/22  19:34:31  peckham
 * 	OT#4924: While instrumenting dns_malloc() code, I stumbled accross
 * 	  a dns_malloc() failure bug in split_bucket(). If the split1_bkt_p
 * 	  malloc() failed, it would return split2_bkt_p without unlinking
 * 	  it from the list!. Don't commit the btree until all resources
 * 	  have been allocated.
 * 	[1992/12/22  19:32:58  peckham]
 * 
 * Revision 1.1.2.3  1992/06/16  14:44:39  peckham
 * 	OT#3495: newbkt_p used in insert_data() before defined (nit).
 * 	[1992/06/04  14:57:53  peckham]
 * 
 * Revision 1.1.2.2  1992/04/23  13:58:13  peckham
 * 	Fix to db_btree_remove_entry(): MOVE_bytes() referenced
 * 	OFFSET_node_rec() instead of INDEX_node_rec, which corrupted
 * 	the bucket by moving data into the offset vector.
 * 	[1992/04/16  19:58:42  peckham]
 * 
 * Revision 1.1  1992/01/19  15:26:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: db_btree.c
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
/*
 * This module was rewritten by Alan Peckham in response to
 * a major hole in the algorithms. The following are my notes,
 * which still need dome editing to reflect the final form
 * of the solution. More in-line documentation is needed
 * to make this change complete.
 *
 * Main goal:
 *   Eliminate possibility of an inconsistent index.
 * 
 * Index errors are caused by the target of index entries being removed.
 * 
 * The danger occurs when the last entry in the bucket is removed.
 * It is then that the key which points to the bucket is no longer
 * valid, but is not itself removed. If an entry is inserted which
 * is less that that key but later than any other entry in the bucket,
 * the code does not insert it into that bucket, but into a following
 * bucket, thus invalidating the key ordering.
 * 
 * Another issue is that when replacing keys, we must insure that
 * the key to the first bucket is never lost. If this did happen,
 * there would never be any way to get at these records.
 * The short term strategy is to crash under these conditions
 * and let server restart correct the situation, as the system
 * is overloaded anyway. The long-term solution would be to
 * sacrifice other keys to replace that first key.
 * 
 * We end up with several objectives:
 *   Replace keys to buckets which lose their last entry.
 *   Remove buckets which become empty.
 *   Clean up any buckets which are touched during replace.
 *   Recover keys which may be lost during malloc() failures.
 *   Never lose the key to the first bucket.
 *   Do a thorough verification of the index after every operation.
 *   Misc: uppercase all macros.
 * 
 * To provide flexibility, the entry_insert_ctx_t structure was
 * privatized to db_btree.c by removing the need for it in the
 * external interface, and subsetting the recursive portion of
 * the insert/replace/delete routines.
 * 
 * This allowed the insertion of bookkeeping entries with data
 * for the lowest level, plus the addition of a structure for
 * critical recursion parameters. This call overhead is kept to
 * a minimum.
 * 
 * General algorithm:
 *   Top level routine sets up context data and makes top level
 *   call to the recursive routine. On completion, add another
 *   index level if necessary, and verify index consistency.
 * 
 *   The recursion on index entries does a horizontal search
 *   for the next index entry and recurses. On completion it must
 *   recover the index entry if it was orphaned by entry removal.
 *   A one-plus is to re-create any orphaned buckets between here
 *   and the bucket pointed to by the parent's next index entry.
 *   During this process, do what is necessary to insure that the
 *   leftmost index entry is never lost, as the horizontal search
 *   will allow getting to all other entries.
 * 
 *   The recursion will finally reach a data bucket, at which the
 *   data insert, replace or remove will be done.
 * 
 * For index recovery to happen, the following info is needed:
 *   1) whether the child node has lost its last entry.
 *   2) the range of child buckets between this parent index pointer
 *      and the next.
 * 
 * If the parent index entry has been invalidated, remove it.
 * Then, create index entries for all child orphan buckets between
 * here and the child of the next parent index entry. If the index
 * is still valid, exclude the entry it points to.
 * If we cannot recover the deleted index entry, and it is the leftmost
 * entry, keep deleting index entries until we have enough room
 * to create one, or until we hit the sentinal, at which time
 * a crash is in order to trigger restart of the server.
 * 
 * Each recursion level will receive pointers to the child node for
 * the parent's index entry as well as the child node for the following
 * parent index. If there is no sibling node, a null pointer is passed.
 * 
 * Each recursion level will return whether the parent index is to be
 * invalidated, as well as a pointer to the first node to create an
 * index entry for.
 *
 * TODO:
 *
 * In-line documentation.
 *
 * Handle recovery of leftmost index deletion.
 *
 * Some of the debug levels have been set at debug1, and
 * could be relegated to another level.
 *
 * The index dump could go to a local file instead of the
 * servicability log. It should also identify the directory.
 */

#include <server.h>
#include <back.h>
#include <dbdef.h>
#include <find.h>
#include <names.h>
#include <sets.h>

/*
 * Debugging defines
 *
 * CAUTION: As with all useful debug code,
 * sometimes it is hard to turn these off!!!
 */
#define NODE_CHECK
#undef INDEX_CHECK
#undef INDEX_DUMP

/* INDEX_CHECK needs NODE_CHECK */
#if defined(INDEX_CHECK) && !defined(NODE_CHECK)
#define NODE_CHECK 1
#endif

/* INDEX_CHECK needs INDEX_DUMP */
#if defined(INDEX_CHECK) && !defined(INDEX_DUMP)
#define INDEX_DUMP 1
#endif

/*
 * Context and scratchpad to insert new keys
 * into the index as the result of a split
 */
typedef struct btree_ctx {
    dir_hash_entry_t	*ctx_hash_p;        /* directory hash entry          */
    db_stream_t         *ctx_stream_p;      /* stream->data to ins/rep       */
    entry_key_t		*ctx_key_p;         /* key for entry to find         */
    Timestamp_t		*ctx_allupto_p;     /* Used to compress deleted data */
} btree_ctx_t;

#define DEB_ASCII_LEN_btree_ctx \
    (4+DEB_ASCII_LEN_db_stream+DEB_ASCII_LEN_entry_key+DEB_ASCII_LEN_Timestamp)
#define DEB_ASCII_BUF_btree_ctx(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_btree_ctx)
#define LOG_ASCII_BUF_btree_ctx(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_btree_ctx)

/*
 * Parameters for recursion
 */
typedef struct btree_prm {
    entry_key_t         *prm_key_p;         /* (in) parent key pointer       */
    unsigned int	prm_key_pos;        /* (in) position of key_p        */
    node_header_t	*prm_index_p;       /* (inout) start indexing here   */
    node_header_t	*prm_end_p;         /* (in) end of bucket range      */
} btree_prm_t;

#define DEB_ASCII_LEN_btree_prm \
    (5+DEB_ASCII_LEN_entry_key+DEB_ASCII_LEN_u_int+2*DEB_ASCII_LEN_ptr)
#define DEB_ASCII_BUF_btree_prm(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_btree_prm)
#define LOG_ASCII_BUF_btree_prm(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_btree_prm)

/*
 * Bucket pointer handling
 */
#define REPLACE_BKT(oldbkt_p, newbkt_p) \
{\
	/* Copy oldbkt pointer */					\
	newbkt_p->node_next_p = oldbkt_p->node_next_p;			\
	newbkt_p->node_prev_p = oldbkt_p->node_prev_p;			\
	/* Adjust previous buckets next pointer */			\
	if (newbkt_p->node_prev_p)					\
	    newbkt_p->node_prev_p->node_next_p = newbkt_p;		\
	/* Adjust next buckets previous pointer */			\
	if (newbkt_p->node_next_p)					\
	    newbkt_p->node_next_p->node_prev_p = newbkt_p;		\
}

#define LINK_AFTER(bkt_p, newbkt_p)	\
{\
	/* Insert a new bucket after an existing bucket */		\
	newbkt_p->node_next_p = bkt_p->node_next_p;			\
	bkt_p->node_next_p = newbkt_p;					\
	newbkt_p->node_prev_p = bkt_p;					\
	if (newbkt_p->node_next_p)					\
	    newbkt_p->node_next_p->node_prev_p = newbkt_p;		\
}

#define LINK_BEFORE(bkt_p, newbkt_p)	\
{\
	/* Insert a new bucket before an existing bucket */		\
	newbkt_p->node_next_p = bkt_p;					\
	newbkt_p->node_prev_p = bkt_p->node_prev_p;			\
	if (newbkt_p->node_prev_p)					\
	    newbkt_p->node_prev_p->node_next_p = newbkt_p;		\
	bkt_p->node_prev_p = newbkt_p;					\
}

#define UNLINK_BKT(bkt_p) \
{ \
    if (bkt_p->node_prev_p) \
        bkt_p->node_prev_p->node_next_p = bkt_p->node_next_p;  \
    if (bkt_p->node_next_p) \
        bkt_p->node_next_p->node_prev_p = bkt_p->node_prev_p;  \
}

#define THIS_SUBLIST(b,p) (((entry_key_t *)INDEX_node_rec(b,p))->key_down_p)

/*
 * Local prototypes
 */
static int 
make_room (			/* Make rooom in node for new record */
    node_header_t       *const,
    unsigned int        ,
    int                 );

static int
insert_index (			/* Insert index record into node */
    node_header_t	*const,
    unsigned int	,
    node_header_t	*const);

static int
insert_record (			/* Insert data record into node */
    node_header_t	*const,
    unsigned int	,
    entry_key_t		*const,
    DBSet_t	 	*const);

static int
replace_record (		/* Replace data for record in node */
    node_header_t	*const,
    unsigned int	,
    DBSet_t		*const);

static void
remove_record (
    node_header_t	*const,
    unsigned int	);

static dns_status_t
find_entry_pos (		/* Find position for key in record */
    node_header_t	*const,
    entry_key_t		*const,
    unsigned int	*const);

static node_header_t *
next_sublist (			/* find child node for next index entry */
    node_header_t       *const,
    unsigned int        );

static void
split_points (			/* Find points to split node */
    node_header_t	*,
    unsigned int	*,
    unsigned int	*,
    unsigned int	 );

static dns_status_t
split_bucket (			/* Split temporary bucket into suitable */
    btree_prm_t		*const,	/* set of buckets */
    node_header_t	*const,
    node_header_t	*const);

static dns_status_t
new_root (			/* Create new layer of index if necessary */
    dir_hash_entry_t    *const,
    node_header_t	*);

static int
purge_data_bucket (		/* Clean out deleted data from node */
    btree_prm_t		*const,
    node_header_t	*const,
    Timestamp_t		*const);

static dns_status_t
split_bucket_for_continuation (	/* Isolate large data entry from */
    btree_prm_t		*const,	/* surrounding data entries */
    node_header_t	*const,
    node_header_t	*const,
    int			);

static dns_status_t
update_index (			/* Insert keys where necessary */
    btree_ctx_t		*const,
    btree_prm_t		*const,
    btree_prm_t		*const,
    node_header_t	*);

static dns_status_t
build_delete_holder (		/* Include delete holder for old entry */
    db_stream_t		*const,	/* in new entry set */
    entry_key_t		*const,
    DBSet_t		*const);

static dns_status_t
insert_data (			/* Insert new entry in bucket */
    btree_ctx_t		*const,
    btree_prm_t		*const,
    node_header_t	*const);

static dns_status_t
replace_data (			/* Replace old data in entry in bucket */
    btree_ctx_t		*const,
    btree_prm_t		*const,
    node_header_t	*const);

static dns_status_t
remove_data (			/* Remove entry from bucket */
    btree_ctx_t		*const,
    btree_prm_t		*const,
    node_header_t	*const);

static dns_status_t
build_index (			/* Rebuild layer of index */
    dir_hash_entry_t	*const);

static dns_status_t
insert_entry (			/* Traverse index and insert entry */
    btree_ctx_t		*const,
    btree_prm_t		*const,
    node_header_t	*);

static dns_status_t 
replace_entry (			/* Traverse index and replace entry */
    btree_ctx_t		*const,
    btree_prm_t		*const,
    node_header_t	*);

static dns_status_t 
remove_entry (			/* Traverse index and delete entry */
    btree_ctx_t		*const,
    btree_prm_t		*const,
    node_header_t	*);

static void
free_btree_index (		/* Release the entire btree index */
    node_header_t       *);

static void
free_btree_data (		/* Release all data buckets */
    node_header_t       *);


/*
 * Check for node corruption
 */
#ifdef NODE_CHECK

void
node_check (
    node_header_t  *);

/*
 * node_check
 *	Verify as much of this bucket as possible.
 *
 * Inputs:
 *	bkt_p = bucket to check.
 */
void 
node_check (node_header_t *bkt_p)
{
    /* Use this to make sure we stay within the bucket */
    int space = bkt_p->node_blocks*BLOCK_SIZE;

    switch (bkt_p->node_type) { /* main switch */
    case DIRECTORY_BUCKET:
	{
	directory_bucket_t *dir_p = (directory_bucket_t *)bkt_p;

	/* There is a single DBSet in a directory bucket */
	if (space)
	    dce_assert(cds__svc_handle,
	       (SKIP_bytes(dir_p, space) >= SKIP_DBSet(dir_p->dir_data)));

	    /* Verify the DBSet */
	    att_set_check((DBSet_t *)dir_p->dir_data);
	}
	break;
    case DATA_BUCKET:
    case PSEUDO_BUCKET:
    case INDEX_BUCKET:
    {
	/* These contain a DBSet of DBSets */
	register byte_t *next_p =
	    (byte_t *)&bkt_p->node_rec_dsc[bkt_p->node_count];
	register int i;

	for (i=0; i<bkt_p->node_count; i++) 
	{
	    rec_descriptor_t *rec_p = &bkt_p->node_rec_dsc[i];
	    entry_key_t *key_p =
		    (entry_key_t *)SKIP_bytes(bkt_p, rec_p->rec_offset);
	    DBSet_t *set_p;

	    /* Is the key within bounds */
	    dce_assert(cds__svc_handle, ((byte_t *)key_p >= next_p));

	    next_p = SKIP_bytes(key_p, rec_p->rec_length);

	    if (space)
		dce_assert(cds__svc_handle,
			   (SKIP_bytes(bkt_p, space) >= next_p));

	    /* Is the key type valid? */
	    switch (key_p->key_type[0]) {
	    case ET_directory:
	    case ET_object:
	    case ET_childPointer:
	    case ET_softlink:
	    case ET_clearinghouse:
	    case 255:
		break;
	    default:
	        {
		  dce_svc_printf(CDS_S_SERVER_DBKEYTYPE_A_MSG, __LINE__, 
				 __FILE__);
		}
	    }

	    /* Is the key sort valid? */
	    switch (key_p->key_sort[0]) 
	    {
	    case BINARY_SORT:
	    case ASCII_SORT:
	    case TYPED_SORT:
	    case 255:
		break;
	    default:
		  dce_svc_printf(CDS_S_SERVER_DBKEYTYPE_A_MSG, __LINE__, 
				 __FILE__);
	    }
	    switch (bkt_p->node_type) 
	    {
	    case DATA_BUCKET:
	    case PSEUDO_BUCKET:
		/* These are followed by a DBSet ... */
		set_p = (DBSet_t *)DATA_entry_data_record(key_p);

		dce_assert(cds__svc_handle, (next_p >= (byte_t *)set_p));

		/* ... except for the sentinel */
		if (key_p->key_type[0] != 255) {
		  dce_assert(cds__svc_handle, (next_p >= SKIP_DBSet(set_p)));

		  /* Verify the following DBSet */
		  att_set_check(set_p);
		}

		break;
	    case INDEX_BUCKET:
		dce_assert(cds__svc_handle, (next_p >=
			       SKIP_AttributeName(key_p->key_name)));

		break;
	    }
	} /* end for */
    } /* end case INDEX_BUCKET: */
	break;
    default:
	dce_svc_printf(CDS_S_SERVER_DBNODETYPE_A_MSG, __LINE__, __FILE__);
    } /* end main switch */

} /* end function "node_check" */


#else
# define node_check(rec_p) (rec_p)
#endif

/*
 * INDEX NOTES
 *
 * An index bucket contains at least one sentinal (255,255)
 * at the end of the index.
 * Each index entry points to a bucket which should contain
 * entries <= the index key.
 */
#ifdef INDEX_CHECK

static int
index_check (
    node_header_t  *,
    entry_key_t    *);

static const int
diff_key (
    entry_key_t *,
    entry_key_t *);

/*
 * Verify index consistency from current bucket on down
 *
 * Inputs:
 *	bkt_p = bucket to check
 *	up_p = index entry pointing to bucket,
 *	       or NULL if top level bucket.
 *
 * Returns:
 *	FALSE if no problem found with index.
 *	TRUE is problem found with index.
 *
 * Call index_dump() at top level if problem found.
 */
static int
index_check (node_header_t *bkt_p,
	     entry_key_t *up_p)
{
    DEB_ASCII_BUF_node_header(bktBuf)
    DEB_ASCII_BUF_entry_key(upBuf)
    DEB_ASCII_BUF_entry_key(key1Buf)
    DEB_ASCII_BUF_entry_key(key2Buf)
    entry_key_t *last_p = NULL;
    register int pos;

    /* only index and data buckets */
    if ((bkt_p->node_type != INDEX_BUCKET) && (bkt_p->node_type != DATA_BUCKET))
	return FALSE;

    /* don't touch unless the bucket is good */
    node_check(bkt_p);

    /*
     * Warn about empty buckets
     */
    if (bkt_p->node_count == 0)
    {
	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_database,
	    svc_c_debug1,
	    "W db_btree:index_check(%s,%s) node is empty",
	    deb_ascii_node_header(bktBuf,bkt_p),
	    deb_ascii_entry_key(upBuf,up_p)));
    }
    /*
     * If there is a previous bucket,
     * set last_p to its last entry
     * to verify ordering.
     *
     * Node count is non-zero.
     */
    else if (bkt_p->node_prev_p != NULL)
    {
	/* Get to the last key of the previous bucket */
	node_header_t *const prev_p = bkt_p->node_prev_p;

	node_check(prev_p);

	last_p  = (entry_key_t *)INDEX_node_rec(prev_p, prev_p->node_count-1);
    }
    /*
     * If this is a left border node,
     * make sure that the child is also.
     *
     * Node count is non-zero.
     */
    else if (bkt_p->node_type == INDEX_BUCKET)
    {
	node_header_t *const down_p = THIS_SUBLIST(bkt_p, 0);

	node_check(down_p);

	if (down_p->node_prev_p != NULL)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_database,
		svc_c_debug1,
		"W db_btree:index_check(%s,%s) index entry is missing",
		deb_ascii_node_header(bktBuf,bkt_p),
		deb_ascii_entry_key(upBuf,up_p)));
	}
    }

    /*
     * Go through the current bucket,
     * verifying index order, consistency with down-pointers
     * and calling recursively for lower levels.
     */
    for (pos=0; pos<bkt_p->node_count; pos++) 
    {
	/* Get to the key */
	entry_key_t *const key_p = (entry_key_t *)INDEX_node_rec(bkt_p, pos);

	if ((last_p != NULL) && (diff_key(last_p, key_p) != BEFORE))
	{
	    if (pos == 0)
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_database,
		    svc_c_debug1,
		    "E db_btree:index_check(%s,%s) first entry(%s) <= previous node(%s)",
		    deb_ascii_node_header(bktBuf,bkt_p),
		    deb_ascii_entry_key(upBuf,up_p),
		    deb_ascii_entry_key(key2Buf,key_p),
		    deb_ascii_entry_key(key1Buf,last_p)));
	    else
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_database,
		    svc_c_debug1,
		    "E db_btree:index_check(%s,%s) entry %d(%s) <= previous entry(%s)",
		    deb_ascii_node_header(bktBuf,bkt_p),
		    deb_ascii_entry_key(upBuf,up_p), pos,
		    deb_ascii_entry_key(key2Buf,key_p),
		    deb_ascii_entry_key(key1Buf,last_p)));

	    return TRUE;
	}

	/*
	 * All keys in the bucket must be <= parent key.
	 */
	if ((up_p != NULL) && (diff_key(key_p, up_p) == AFTER))
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_database,
		svc_c_debug1,
		"E db_btree:index_check(%s,%s) entry %d(%s) > index parent(%s)",
		deb_ascii_node_header(bktBuf,bkt_p),
		deb_ascii_entry_key(upBuf,up_p), pos,
		deb_ascii_entry_key(key2Buf,key_p),
		deb_ascii_entry_key(key1Buf,up_p)));

	    return TRUE;
	}

	/*
	 * Recurse if this is an index bucket.
	 *
	 * The down pointer must be there!
	 */
	if (bkt_p->node_type == INDEX_BUCKET)
	    if (key_p->key_down_p != NULL)
	    {
		if (index_check(key_p->key_down_p, key_p))
		    return TRUE;
	    }
	    else
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_database,
		    svc_c_debug1,
		    "E db_btree:index_check(%s,%s) entry %d(%s) -> NULL",
		    deb_ascii_node_header(bktBuf,bkt_p),
		    deb_ascii_entry_key(upBuf,up_p), pos,
		    deb_ascii_entry_key(key2Buf,key_p)));

		return TRUE;
	    }

	/* Keep tack of previous key */
	last_p  = key_p;
    }

    /*
     * If there is a next bucket,
     * verify index ordering against
     * its first entry.
     */
    if (bkt_p->node_next_p != NULL)
    {
	node_header_t *const next_p = bkt_p->node_next_p;

	node_check(next_p);

	if (next_p->node_count > 0)
	{
	    /* Get to the first key of the next bucket */
	    entry_key_t *const key_p = (entry_key_t *)INDEX_node_rec(next_p, 0);

	    /*
	     * Verify key ordering to next bucket.
	     */
	    if ((last_p != NULL) && (diff_key(last_p, key_p) != BEFORE))
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_database,
		    svc_c_debug1,
		    "E db_btree:index_check(%s,%s) last entry(%s) >= next node(%s)",
		    deb_ascii_node_header(bktBuf,bkt_p),
		    deb_ascii_entry_key(upBuf,up_p),
		    deb_ascii_entry_key(key1Buf,last_p),
		    deb_ascii_entry_key(key2Buf,key_p)));

		return TRUE;
	    }

	    /*
	     * The following bucket must not be greater
	     * than the parent's key.
	     */
	    if ((up_p != NULL) && (diff_key(key_p, up_p) != AFTER))
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_database,
		    svc_c_debug1,
		    "E db_btree:index_check(%s,%s) next node(%s) < index parent(%s)",
		    deb_ascii_node_header(bktBuf,bkt_p),
		    deb_ascii_entry_key(upBuf,up_p),
		    deb_ascii_entry_key(key2Buf,key_p),
		    deb_ascii_entry_key(key1Buf,up_p)));

		return TRUE;
	    }
	}
    }
    else if ((pos = bkt_p->node_count) > 0)
    {
	/*
	 * Make sure this last bucket contains a sentinel
	 */
	entry_key_t *const key_p = (entry_key_t *)INDEX_node_rec(bkt_p, pos-1);

	if ((EXT8(key_p->key_type) != 255) ||
	    (EXT8(key_p->key_sort) != 255))
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_database,
		svc_c_debug1,
		"E db_btree:index_check(%s,%s) last entry(%s) not the sentinel",
		deb_ascii_node_header(bktBuf,bkt_p),
		deb_ascii_entry_key(upBuf,up_p),
		deb_ascii_entry_key(key2Buf,key_p)));

	    return TRUE;
	}
    }
    else
    {
	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_database,
	    svc_c_debug1,
	    "E db_btree:index_check(%s,%s) last bucket empty",
	    deb_ascii_node_header(bktBuf,bkt_p),
	    deb_ascii_entry_key(upBuf,up_p)));

	return TRUE;
    }
    return FALSE;
}

/*
 * diff_key
 *
 *	Return to compare two keys and return result.
 *	Copied from find_entry_pos()
 *
 * Returns:
 *	BEFORE	key1 < key2
 *	EQUIV	key1 == key2
 *	AFTER	key1 > key2
 */
static const int
diff_key (entry_key_t *key1_p, entry_key_t *key2_p)
{
    SimpleName_t *const name1_p = (SimpleName_t *)key1_p->key_name;

    if (EQ_byte(key1_p->key_type, key2_p->key_type))
    {
	if (EQ_byte(key1_p->key_sort, key2_p->key_sort))
	{
	    SimpleName_t *const name2_p = (SimpleName_t *)key2_p->key_name;

	    if (LEN_SimpleName(name1_p) == LEN_SimpleName(name2_p))
	    {
		return dns_bcmp_lexical((char *)name1_p->sn_name,
					(char *)name2_p->sn_name,
					EXT8(name1_p->sn_length));
	    }
	    else if (EXT8(name1_p->sn_length) < EXT8(name2_p->sn_length))
	    {
		int ret = dns_bcmp_lexical((char *)name1_p->sn_name,
					   (char *)name2_p->sn_name,
					   EXT8(name1_p->sn_length));

		return (ret == EQUIV) ? BEFORE : ret;
	    }
	    else
	    {
		int ret = dns_bcmp_lexical((char *)name1_p->sn_name,
					   (char *)name2_p->sn_name,
					   EXT8(name2_p->sn_length));

		return (ret == EQUIV) ? AFTER : ret;
	    }
	}
	else
	    return LT_byte(key1_p->key_sort, key2_p->key_sort) ? BEFORE : AFTER;
    }
    else
	return LT_byte(key1_p->key_type, key2_p->key_type) ? BEFORE : AFTER;
}

#else
  /*
   * Null out if not debugging
   */
# define index_check(bkt_p,up_p) (bkt_p,up_p,FALSE)
#endif

/*
 * index_dump
 *	A recursive depth-first dump of the index.
 *
 * Inputs:
 *	bkt_p = current bucket
 *	level = depth of recursion (used to indent display)
 */
#ifdef INDEX_DUMP

static void
index_dump (
    node_header_t  *,
    int);

static void
index_dump (node_header_t *bkt_p, int level)
{
    DEB_ASCII_BUF_node_header(bktBuf)
    DEB_ASCII_BUF_entry_key(keyBuf)
    static const char spaces[] = "                                        ";
    node_header_t *last_p = NULL;
    register int pos;

    /* only index and data buckets */
    if ((bkt_p->node_type != INDEX_BUCKET) && (bkt_p->node_type != DATA_BUCKET))
	return;

    /* don't touch unless the bucket is good */
    node_check(bkt_p);

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_database,
	svc_c_debug1,
	"  db_btree:index_dump > %sbucket(%s)",
	&spaces[sizeof(spaces)-1-level],
	deb_ascii_node_header(bktBuf,bkt_p)));

    /*
     * Go through the current bucket,
     * verifying index order, consistency with down-pointers
     * and calling recursively for lower levels.
     */
    for (pos=0; pos<bkt_p->node_count; pos++) 
    {
	/* Get to the key */
	const entry_key_t *const key_p = (const entry_key_t *)INDEX_node_rec(bkt_p, pos);

	/*
	 * Recurse if this is an index bucket.
	 *
	 * The down pointer must be there!
	 */
	if (bkt_p->node_type == INDEX_BUCKET)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_database,
		svc_c_debug1,
		"  db_btree:index_dump > %s%s(%p)",
		&spaces[sizeof(spaces)-1-level],
		deb_ascii_entry_key(keyBuf,key_p), key_p->key_down_p));

	    if (last_p == NULL)
		last_p = key_p->key_down_p;

	    for (; last_p != NULL; last_p = last_p->node_next_p)
	    {
		index_dump(last_p, level+2);

		if (last_p == key_p->key_down_p)
		{
		    last_p = last_p->node_next_p;
		    break;
		}
	    }

	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_database,
		svc_c_debug1,
		"  db_btree:index_dump < %s%s(%p)",
		&spaces[sizeof(spaces)-1-level],
		deb_ascii_entry_key(keyBuf,key_p), key_p->key_down_p));
	}
	else
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_database,
		svc_c_debug1,
		"  db_btree:index_dump * %s%s",
		&spaces[sizeof(spaces)-1-level],
		deb_ascii_entry_key(keyBuf,key_p)));
	}
    }

    DCE_SVC_DEBUG((
	cds__svc_handle,
	cds_svc_database,
	svc_c_debug1,
	"  db_btree:index_dump < %sbucket(%s)",
	&spaces[sizeof(spaces)-1-level],
	deb_ascii_node_header(bktBuf,bkt_p)));

    dce_assert(cds__svc_handle, (level > 0));
}

#else
  /*
   * Null out if not debugging
   */
# define index_dump(bkt_p,n) (bkt_p,n)
#endif

/*
 * deb_ascii_node_header
 *	format display of node_header_t
 */
char *
deb_ascii_node_header (char           *buf_p,
                        node_header_t  *rec_p)
{
    if (rec_p) {
	(void)sprintf(buf_p, "%p", rec_p);

	return(buf_p);
    } else
	return("NULL node_header_t");
}

/*
 * deb_ascii_btree_ctx
 *	format display of btree_ctx_t
 *	display stream,key,allupto
 */
char *
deb_ascii_btree_ctx (char         *buf_p,
		     btree_ctx_t  *ctx_p)
{
    if (ctx_p) {
	LOG_ASCII_BUF_db_stream(streamBuf)
	LOG_ASCII_BUF_entry_key(keyBuf)
	LOG_ASCII_BUF_Timestamp(alluptoBuf)

	(void)sprintf(buf_p, "{%s,%s,%s}",
		      deb_ascii_db_stream(streamBuf,ctx_p->ctx_stream_p),
		      deb_ascii_entry_key(keyBuf,ctx_p->ctx_key_p),
		      deb_ascii_Timestamp(alluptoBuf,ctx_p->ctx_allupto_p));

	return(buf_p);
    } else
	return("NULL btree_ctx_t");
}

/*
 * deb_ascii_btree_prm
 *	format display of btree_prm_t
 */
char *
deb_ascii_btree_prm (char    *buf_p,
		 btree_prm_t *prm_p)
{
    if (prm_p) {
	LOG_ASCII_BUF_entry_key(keyBuf)

	(void)sprintf(buf_p, "{%s,%u,%p,%p}",
		      deb_ascii_entry_key(keyBuf,prm_p->prm_key_p),
		      prm_p->prm_key_pos,
		      prm_p->prm_index_p,
		      prm_p->prm_end_p);

	return(buf_p);
    } else
	return("NULL btree_prm_t");
}

/*
 * db_btree_copy_keys
 *	This routine copies keys from one bucket to another.
 *	Only a subset of the keys are copied.
 *
 * Input:
 *	bkt_p = pointer to original bucket
 *	newbkt_p = pointer to new bucket to hold subset
 *	size  = blocks in new bucket
 *	start_pos = starting key position for copy
 *	end_pos = ending key position
 */
void 
db_btree_copy_keys (node_header_t  *bkt_p,
                    node_header_t  *newbkt_p,
                    unsigned int   size,
                    unsigned int   start_pos,
                    unsigned int   end_pos)
{
    DEB_ASCII_BUF_node_header(bktBuf)
    DEB_ASCII_BUF_node_header(newbktBuf)
    rec_descriptor_t *rd_p;
    unsigned int adjustment, data_len;
    unsigned int i;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_btree_copy_keys(%s,%s,%u,%u,%u)",
        deb_ascii_node_header(bktBuf,bkt_p),
        deb_ascii_node_header(newbktBuf,newbkt_p),
        size,
        start_pos,
        end_pos));

    node_check(bkt_p);
    /* Set node header new bucket */
    newbkt_p->node_type = bkt_p->node_type;
    newbkt_p->node_blocks = size;
    newbkt_p->node_count = end_pos - start_pos;

    data_len = (bkt_p->node_rec_dsc[end_pos-1].rec_offset +
      	bkt_p->node_rec_dsc[end_pos-1].rec_length) -
	bkt_p->node_rec_dsc[start_pos].rec_offset;


    /* Calculate space left */
    if (bkt_p->node_count == 0)
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug2,
  	    "I db_btree_copy_keys(%s,%s,%u,%u,%u) node_count is zero.",
	    deb_ascii_node_header(bktBuf,bkt_p),
	    deb_ascii_node_header(newbktBuf,newbkt_p),
	    size, start_pos, end_pos));

    /* Copy keys */
    MOVE_bytes(INDEX_node_rec(bkt_p, start_pos),
	INDEX_node_rec_dsc(newbkt_p, newbkt_p->node_count), data_len);

    /* Copy key descriptors */
    MOVE_bytes(INDEX_node_rec_dsc(bkt_p, start_pos),
	INDEX_node_rec_dsc(newbkt_p, 0),
	sizeof(rec_descriptor_t) * newbkt_p->node_count);

    /* Compute difference between what it was and what it will be */
    adjustment = DIFF_bytes(INDEX_node_rec(bkt_p, start_pos), bkt_p) -
	DIFF_bytes(INDEX_node_rec_dsc(newbkt_p, newbkt_p->node_count), newbkt_p);

    /* Now adjust offsets in key descriptors */
    for (i=0; i < newbkt_p->node_count; ++i)
	newbkt_p->node_rec_dsc[i].rec_offset -= adjustment;

    rd_p = &newbkt_p->node_rec_dsc[newbkt_p->node_count-1];

    /* Make sure not a negative value */
    dce_assert(cds__svc_handle, ((rd_p->rec_offset+rd_p->rec_length) <=
				 (newbkt_p->node_blocks*BLOCK_SIZE)));

    newbkt_p->node_space = (newbkt_p->node_blocks*BLOCK_SIZE)
	- (rd_p->rec_offset + rd_p->rec_length);
    node_check(newbkt_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_btree_copy_keys(%s,%s,%u,%u,%u)",
        deb_ascii_node_header(bktBuf,bkt_p),
        deb_ascii_node_header(newbktBuf,newbkt_p),
        size,
        start_pos,
        end_pos));
}

/*
 * make_room
 *	Make room for a record in a bucket at pos.
 *
 * Input:
 *	bkt_p = pointer to bucket
 *	pos = index to insertion point for record in bucket
 *	len = space needed for record
 *
 * Returns:
 *	if insufficient space, returns additional space needed,
 *	otherwise returns zero.
 */
static int 
make_room (node_header_t  *const bkt_p,
	   unsigned int   pos,
	   int		  len)
{
    DEB_ASCII_BUF_node_header(bktBuf)
    rec_descriptor_t *rd_p;
    int move_len;
    unsigned int i, size;
    int space;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >db_btree:make_room(%s,%u,%d)",
        deb_ascii_node_header(bktBuf,bkt_p),
	pos, len));

    node_check(bkt_p);

    if (pos < bkt_p->node_count)
	{
	/* keep everything aligned */
	move_len = ALIGN_LEN(len);

	/* reject if not enough space */
	if (bkt_p->node_space < (i = sizeof(rec_descriptor_t) + move_len))
	    {
	    space = i - bkt_p->node_space;
	    goto leave_make_room;
	    }

	/* Shift end */
	size = (bkt_p->node_rec_dsc[bkt_p->node_count-1].rec_offset +
		bkt_p->node_rec_dsc[bkt_p->node_count-1].rec_length) -
		bkt_p->node_rec_dsc[pos].rec_offset;
	MOVE_bytes(INDEX_node_rec(bkt_p, pos),
	  	   SKIP_bytes(INDEX_node_rec(bkt_p, pos),
			sizeof(rec_descriptor_t) + move_len),
		   size);

	/* Shift front */
	size = sizeof(rec_descriptor_t)*(bkt_p->node_count-pos) +
	  	    bkt_p->node_rec_dsc[pos].rec_offset -
		    bkt_p->node_rec_dsc[0].rec_offset;
	MOVE_bytes(INDEX_node_rec_dsc(bkt_p, pos),
	    INDEX_node_rec_dsc(bkt_p, pos+1), size);
        }
    else if (pos != 0)
	{
	/* Adding to the end, only descriptors to shift */
	rd_p = &bkt_p->node_rec_dsc[pos-1];
	i = rd_p->rec_offset + rd_p->rec_length;
	/* reject if not enough space */
	if (bkt_p->node_space < (i = sizeof(rec_descriptor_t)
					+ (ALIGN_LEN(i) - i) + len))
	    {
	    space = i - bkt_p->node_space;
	    goto leave_make_room;
	    }

	MOVE_bytes(INDEX_node_rec_dsc(bkt_p, bkt_p->node_count),
	       	INDEX_node_rec_dsc(bkt_p, bkt_p->node_count+1),
		(rd_p->rec_offset + rd_p->rec_length)
			- bkt_p->node_rec_dsc[0].rec_offset);

	/* make sure address is aligned */
	bkt_p->node_rec_dsc[pos].rec_offset =
	    ALIGN_LEN(rd_p->rec_offset + rd_p->rec_length);

	move_len = len;
	}
    else
	{
	/* This is the only entry */
	/* reject if not enough space */
	if (bkt_p->node_space < (i = sizeof(rec_descriptor_t) + len))
	    {
	    space = i - bkt_p->node_space;
	    goto leave_make_room;
	    }

	bkt_p->node_rec_dsc[0].rec_offset =
	    DIFF_bytes(&bkt_p->node_rec_dsc[0], bkt_p);

	move_len = len;
	}
    bkt_p->node_rec_dsc[pos].rec_length = len;

    /* Fix up offsets */
    bkt_p->node_count++;

    /* Update offsets */
    for (i=0; i <= pos; ++i)
	bkt_p->node_rec_dsc[i].rec_offset += sizeof(rec_descriptor_t);

    /* Complete adjustments */
    for (; i < bkt_p->node_count; ++i)
       bkt_p->node_rec_dsc[i].rec_offset += sizeof(rec_descriptor_t) + move_len;

    /* Update node space */
    rd_p = &bkt_p->node_rec_dsc[bkt_p->node_count-1];
    bkt_p->node_space = (bkt_p->node_blocks*BLOCK_SIZE)
	- (rd_p->rec_offset + rd_p->rec_length);
    space = 0;

leave_make_room:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <db_btree:make_room(%s,%u,%d) return(%d)",
        deb_ascii_node_header(bktBuf,bkt_p),
	pos, len, space));
    return(space);
}

/*
 * insert_index
 *	Insert entry into an index bucket. The index entry
 *	to insert is for the last entry in the provided down_p.
 *
 * Input:
 *	bkt_p = pointer to bucket
 *	pos = index to insert index entry into bucket
 *	down_p = pointer to target bucket
 *
 * Returns:
 *	if insufficient space, returns additional space needed,
 *	otherwise returns zero.
 */
static int 
insert_index (node_header_t  *const bkt_p,
              unsigned int   pos,
              node_header_t  *const down_p)
{
    DEB_ASCII_BUF_node_header(bktBuf)
    DEB_ASCII_BUF_node_header(downBuf)
    DEB_ASCII_BUF_entry_key(srcBuf)
    entry_key_t *const src_p = (entry_key_t *)
		INDEX_node_rec(down_p, down_p->node_count-1);
    int space;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " >db_btree:insert_index(%s,%u,%s) key(%s)",
        deb_ascii_node_header(bktBuf,bkt_p), pos,
        deb_ascii_node_header(downBuf,down_p),
	deb_ascii_entry_key(srcBuf,src_p)));

    space = make_room(bkt_p, pos, LEN_entry_key(src_p));
    if (space == 0)
	{
	entry_key_t *const dst_p = (entry_key_t *)INDEX_node_rec(bkt_p, pos);

	MOVE_entry_key(src_p, dst_p);
	dst_p->key_down_p = down_p;
	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " <db_btree:insert_index(%s,%u,%s) return(%d)",
        deb_ascii_node_header(bktBuf,bkt_p), pos,
        deb_ascii_node_header(downBuf,down_p), space));
    return(space);
}

/*
 * insert_record
 *	Insert data into a bucket. set_p contains the
 *	attribute set to be appended to the key_p.
 *
 * Input:
 *	bkt_p = pointer to bucket
 *	pos = index to insert data into bucket
 *	key_p = pointer to data key to insert
 *	set_p = pointer to set for data bucket
 *
 * Returns:
 *	if insufficient space, returns additional space needed,
 *	otherwise returns zero.
 */
static int 
insert_record (node_header_t  *const bkt_p,
               unsigned int   pos,
               entry_key_t    *const key_p,
               DBSet_t        *const set_p)
{
    DEB_ASCII_BUF_node_header(bktBuf)
    DEB_ASCII_BUF_entry_key(keyBuf)
    DEB_ASCII_BUF_DBSet(setBuf)
    int space;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " >db_btree:insert_record(%s,%u,%s,%s)",
        deb_ascii_node_header(bktBuf,bkt_p),
        pos,
        deb_ascii_entry_key(keyBuf,key_p),
        deb_ascii_DBSet(setBuf,set_p)));

    att_set_check(set_p);
    space = make_room(bkt_p, pos, key_p->key_length + LEN_DBSet(set_p));
    if (space == 0)
	{
	entry_key_t *const dst_p = (entry_key_t *)INDEX_node_rec(bkt_p, pos);

        MOVE_bytes(key_p, dst_p, key_p->key_length);
        MOVE_DBSet(set_p, DATA_entry_data_record(dst_p));
	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " <db_btree:insert_record(%s,%u,%s,%s) return(%d)",
        deb_ascii_node_header(bktBuf,bkt_p),
        pos,
        deb_ascii_entry_key(keyBuf,key_p),
        deb_ascii_DBSet(setBuf,set_p),
        space));
    return(space);
}

/*
 * replace_record
 *	This routine replaces a data record.
 *	To do this it must shift the data
 *	that is already in the bucket.
 *
 * Input:
 *	bkt_p = pointer to node containing data
 *	pos = position in bkt of entry
 *	new_data_p = pointer to new data.
 *
 * Returns:
 *	if insufficient space, returns additional space needed,
 *	otherwise returns zero.
 */
static int 
replace_record (node_header_t  *const bkt_p,
                unsigned int   pos,
                DBSet_t        *const new_data_p)
{
    DEB_ASCII_BUF_node_header(bktBuf)
    DEB_ASCII_BUF_DBSet(new_dataBuf)
    register entry_key_t *data_p = (entry_key_t *)INDEX_node_rec(bkt_p, pos);
    register DBSet_t *set_p = (DBSet_t *)DATA_entry_data_record(data_p);
    rec_descriptor_t *rd_p;
    register int i = LEN_DBSet(set_p);
    register int shiftsize = LEN_DBSet(new_data_p);
    int space;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " >db_btree:replace_record(%s,%d,%p) of(%d)",
        deb_ascii_node_header(bktBuf,bkt_p),
        pos,
        deb_ascii_DBSet(new_dataBuf,new_data_p),
        bkt_p->node_count));

    node_check(bkt_p);

    /* if not the last record, the entry is padded */
    if ((pos+1) < bkt_p->node_count)
	{
	i = ALIGN_LEN(i);
	shiftsize = ALIGN_LEN(shiftsize);
	}

    shiftsize -= i;
    if (shiftsize)
	if (0 < shiftsize)
	    if (bkt_p->node_space < shiftsize)
		{
		space = shiftsize - bkt_p->node_space;
		goto leave_replace_record;
		}
    /* Only need to shift data if not last record */
    if ((pos+1) < bkt_p->node_count)
	{
	/* Shift data following data record */
	register byte_t *start_p = INDEX_node_rec(bkt_p, pos+1);

	/* keep following records aligned */
	shiftsize = ALIGN_LEN(shiftsize);
	MOVE_bytes(start_p, SKIP_bytes(start_p, shiftsize),
	     	(bkt_p->node_rec_dsc[bkt_p->node_count-1].rec_offset +
	     	bkt_p->node_rec_dsc[bkt_p->node_count-1].rec_length) -
		bkt_p->node_rec_dsc[pos+1].rec_offset);

	/* Adjust offsets */
	for (i = pos+1; i < bkt_p->node_count; i++)
	    bkt_p->node_rec_dsc[i].rec_offset += shiftsize;
	}

    /* re-compute pointers */
    data_p = (entry_key_t *)INDEX_node_rec(bkt_p, pos);
    set_p = (DBSet_t *)DATA_entry_data_record(data_p);
    ALIGN_CHECK(set_p);
    att_set_check(new_data_p);
    MOVE_DBSet(new_data_p, set_p);
    bkt_p->node_rec_dsc[pos].rec_length = DIFF_bytes(SKIP_DBSet(set_p), data_p);

    rd_p = &bkt_p->node_rec_dsc[bkt_p->node_count-1];
    bkt_p->node_space = (bkt_p->node_blocks*BLOCK_SIZE)
	- (rd_p->rec_offset + rd_p->rec_length);
    node_check(bkt_p);
    space = 0;

leave_replace_record:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " <db_btree:replace_record(%s,%d,%s) return(%d)",
        deb_ascii_node_header(bktBuf,bkt_p),
        pos,
        deb_ascii_DBSet(new_dataBuf,new_data_p),
        space));
    return(space);
}

/*
 * remove_record
 *
 * This routine excises a record from a bucket -- shifting data and adjusting
 * descriptors as needed.  It also maintains the bucket's node count and
 * free space cells.
 */
static void
remove_record (node_header_t  *const bkt_p,
               unsigned int   pos)
{
    DEB_ASCII_BUF_node_header(bktBuf)
    rec_descriptor_t *rd_p;
    unsigned int count = bkt_p->node_count;

    /*
     * A few assertion checks:
     *   - Can't remove a record that isn't here
     *   - Can't remove a record from an empty bucket
     */

    dce_assert(cds__svc_handle, (pos <= count));
    dce_assert(cds__svc_handle, (count != 0));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >remove_record(%s,%u)",
        deb_ascii_node_header(bktBuf,bkt_p), pos));
    
    if (--count > 0)
    {
	/* Save my length */
	unsigned int len = bkt_p->node_rec_dsc[pos].rec_length;
	unsigned int offset;

	/* Remove data only if not last entry */
	if (pos < count) {
	    /* leave following records aligned */
	    len = ALIGN_LEN(len);

	    MOVE_bytes(INDEX_node_rec(bkt_p, pos+1), INDEX_node_rec(bkt_p, pos),
		    (bkt_p->node_rec_dsc[count].rec_offset +
		    bkt_p->node_rec_dsc[count].rec_length) -
		    bkt_p->node_rec_dsc[pos+1].rec_offset);
	}

	/* Remove descriptor */
	MOVE_bytes(INDEX_node_rec_dsc(bkt_p, pos+1),
	    INDEX_node_rec_dsc(bkt_p, pos),
	    (bkt_p->node_rec_dsc[count].rec_offset +
	    bkt_p->node_rec_dsc[count].rec_length) -
	    len - DIFF_bytes(INDEX_node_rec_dsc(bkt_p, pos+1), bkt_p));

	bkt_p->node_count = count;

	for (offset=0; offset < pos; ++offset) {
	    bkt_p->node_rec_dsc[offset].rec_offset -= sizeof(rec_descriptor_t);
	}

	for (offset=pos; offset < count; ++offset) {
	    bkt_p->node_rec_dsc[offset].rec_offset -=
		    (len + sizeof(rec_descriptor_t));
	}

	rd_p = &bkt_p->node_rec_dsc[count-1];
	bkt_p->node_space = (bkt_p->node_blocks*BLOCK_SIZE)
	    - (rd_p->rec_offset + rd_p->rec_length);

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_database,
	    svc_c_debug4,
	    "I remove_record(%s,%u) count(%u) space(%u)",
	    deb_ascii_node_header(bktBuf,bkt_p), pos,
	    count, bkt_p->node_space));

	node_check(bkt_p);
    }
    else
    {
	bkt_p->node_count = 0;
	bkt_p->node_space = (bkt_p->node_blocks*BLOCK_SIZE)
	    - DIFF_bytes(INDEX_node_rec_dsc(bkt_p,0), bkt_p);

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_database,
	    svc_c_debug1,
	    "I remove_record(%s,%u) node is empty",
	    deb_ascii_node_header(bktBuf,bkt_p), pos,
	    count, bkt_p->node_space));
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <remove_record(%p,%u)", bkt_p, pos));
}

/*
 * find_entry_pos
 *	Search a bucket and return the position the first key
 *	greater then or equal to the input key
 *
 * Input:
 *	rec_p = pointer to a key bucket
 *	key_p  = pointer to entry key
 *
 * Output:
 *	pos_p = pointer to return position
 *
 * Value:
 *	DNS_SUCCESS - found match
 *	DNS_UNKNOWNENTRY - returning position for insert
 *
 */
static dns_status_t 
find_entry_pos (node_header_t  *const rec_p,
                entry_key_t    *const key_p,
                unsigned int   *const pos_p)
{
    DEB_ASCII_BUF_node_header(recBuf)
    DEB_ASCII_BUF_entry_key(keyBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    SimpleName_t *const name_p = (SimpleName_t *)key_p->key_name;
    int upper, lower, curpos;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >db_btree:find_entry_pos(%s,%s,%p)",
        deb_ascii_node_header(recBuf,rec_p),
        deb_ascii_entry_key(keyBuf,key_p),
        pos_p));

    node_check(rec_p);
   /* Now do binary search looking for the best insert position */

    upper = rec_p->node_count;
    curpos = upper/2;
    lower = -1;

    /* Binary search through an ordered list */
    while ((curpos < upper) && (lower < curpos))
        {
	entry_key_t *const cur_key_p = (entry_key_t *)INDEX_node_rec(rec_p, curpos);
	int cmp_status;

	if (EQ_byte(key_p->key_type, cur_key_p->key_type))
	    {
	    if (EQ_byte(key_p->key_sort, cur_key_p->key_sort))
	        {
		SimpleName_t *const cur_name_p = (SimpleName_t *)cur_key_p->key_name;

	 	if (LEN_SimpleName(key_p->key_name) ==
			LEN_SimpleName(cur_key_p->key_name))
		    cmp_status = dns_bcmp_lexical((char *)name_p->sn_name,
			    (char *)cur_name_p->sn_name, EXT8(name_p->sn_length));
		else if (EXT8(name_p->sn_length) < EXT8(cur_name_p->sn_length))
		    {
		    cmp_status = dns_bcmp_lexical((char *)name_p->sn_name,
				(char *)cur_name_p->sn_name, EXT8(name_p->sn_length));
		    if (cmp_status == EQUIV) cmp_status = BEFORE;
		    }
	 	else
		    {
		    cmp_status = dns_bcmp_lexical((char *)name_p->sn_name,
			    (char *)cur_name_p->sn_name, EXT8(cur_name_p->sn_length));
		    if (cmp_status == EQUIV) cmp_status = AFTER;
		    }
		}
	    else if (LT_byte(key_p->key_sort, cur_key_p->key_sort))
		cmp_status = BEFORE;
	    else cmp_status = AFTER;
	    }
    	else if (LT_byte(key_p->key_type, cur_key_p->key_type))
	    cmp_status = BEFORE;
	else cmp_status = AFTER;

        if (cmp_status == AFTER)    /* Name is greater then current position */
            {
            lower = curpos;
            curpos += (upper-lower+1)/2;
            }
        else if (cmp_status == BEFORE)
            {                   /* name is less then current position */
            upper =curpos;
            curpos -= (upper-lower+1)/2;
            }
        else /* Equal */
            {
            *pos_p = curpos;
            status = DNS_SUCCESS;
	    goto leave_find_entry_pos;
            }
        }

    /* Return where new value should go */
    *pos_p = upper;
    status = DNS_UNKNOWNENTRY;

leave_find_entry_pos:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <db_btree:find_entry_pos(%s,%s,%p->%u) return(%s)",
        deb_ascii_node_header(recBuf,rec_p),
        deb_ascii_entry_key(keyBuf,key_p),
        pos_p,
        *pos_p,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * next_sublist
 *	This routine determines the beginning bucket of the next sublist
 *	in the BTree.
 *
 * Inputs:
 *	bkt_p = parent bucket
 *	pos = current position in parent bucket
 *
 * Returns:
 *	Down pointer for next index entry
 *	or NULL if no next entry.
 */

static node_header_t *
next_sublist (node_header_t  *const bkt_p,
	      unsigned int   pos)
{
    /*
     * First try to obtain the next down_p in the same index bucket.  If
     * we're at the end of the bucket, then get the first one of the next
     * bucket (if any).
     */

    if (++pos < bkt_p->node_count) {
	return (((entry_key_t *)INDEX_node_rec(bkt_p, pos))->key_down_p);
    } else {
	node_header_t *const next_p = bkt_p->node_next_p;
	if (next_p) {
	    return (((entry_key_t *)INDEX_node_rec(next_p, 0))->key_down_p);
	} else {
	    return (NULL);
	}
    }
}

/*
 * split_points
 *	Determine the split points in the expanded bucket.
 *	We already know at least one bucket is needed.  Attempt
 *	to keep it to only 1.
 *
 * Inputs:
 *	bkt_p = pointer to expanded bucket
 *	point1_p = address to return first split point
 *	point2_p = address to return second split point if needed
 *	max_size = maximum size of bucket
 */
static void
split_points (node_header_t  *bkt_p,
	      unsigned int   *point1_p,
	      unsigned int   *point2_p,
              unsigned int   max_size)
{
    DEB_ASCII_BUF_node_header(bktBuf)
    unsigned int point1, point2;
    unsigned int top, middle, bottom;
    unsigned int top_len, bottom_len;
    unsigned int i;
    int ret;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " >db_btree:split_points(%s,%p,%p,%u)",
	deb_ascii_node_header(bktBuf,bkt_p),
        point1_p, point2_p, max_size));

    node_check(bkt_p);
    /* Remove overhead from bucket space */
    max_size -= DIFF_bytes(bkt_p->node_rec_dsc, bkt_p);

    middle = bkt_p->node_count/2;

    /* Will half fit in the first bucket */
    if ( ((middle * sizeof(rec_descriptor_t)) +
	bkt_p->node_rec_dsc[middle].rec_offset -
	bkt_p->node_rec_dsc[0].rec_offset) < max_size)

        /* Will remaining fit in second bucket? */
        if ( (((bkt_p->node_count-middle) * sizeof(rec_descriptor_t)) +
		(bkt_p->node_rec_dsc[bkt_p->node_count-1].rec_offset +
		bkt_p->node_rec_dsc[bkt_p->node_count-1].rec_length) -
	        bkt_p->node_rec_dsc[middle].rec_offset) < max_size)
	    {
	    point1 = middle;
	    point2 = middle;
	    goto leave_split_points;
	    }

    /* Find best position */
    top_len = 0;
    if (bkt_p->node_count == 0)
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug3,
	    "W db_btree:split_points(%s,%p,%p,%u) node_count is zero",
	    deb_ascii_node_header(bktBuf,bkt_p), 
	    point1_p, point2_p, max_size));

    top = bkt_p->node_count-1;

    bottom_len = 0;
    bottom = 0;
    for (i=0; i < bkt_p->node_count; ++i)
	{
	/* Try to get one for the left bucket */
	if ((bottom_len + sizeof(rec_descriptor_t) +
		bkt_p->node_rec_dsc[bottom].rec_length) < max_size)
	    {
	    bottom_len += sizeof(rec_descriptor_t) +
		bkt_p->node_rec_dsc[bottom].rec_length;
	    ++bottom;
	    }

	/* Try to get one for the right bucket */
	if ((top_len + sizeof(rec_descriptor_t) +
			 bkt_p->node_rec_dsc[top].rec_length) < max_size)
	    {
	    top_len += sizeof(rec_descriptor_t) +
			bkt_p->node_rec_dsc[top].rec_length;
	    --top;
	    }

	if (bottom > top)
    	/* fit in 2 buckets */
	    {
	    point1 = bottom;
	    point2 = bottom;
	    goto leave_split_points;
	    }
	}

    /* Keys won't fit in 2 buckets */

    bottom = bkt_p->node_count/3;

    /* Will first third fit in the first bucket */
    if ( ((bottom * sizeof(rec_descriptor_t)) +
	bkt_p->node_rec_dsc[bottom].rec_offset -
	bkt_p->node_rec_dsc[0].rec_offset) < max_size)

        /* Will next third fit in second bucket? */
	{
	middle = bottom*2;
        if ( (((middle-bottom) * sizeof(rec_descriptor_t)) +
		bkt_p->node_rec_dsc[middle].rec_offset -
	        bkt_p->node_rec_dsc[bottom].rec_offset) < max_size)

            /* Will remaining third fit in lastbucket? */
            if ( (((bkt_p->node_count - middle) * sizeof(rec_descriptor_t)) +
		(bkt_p->node_rec_dsc[bkt_p->node_count-1].rec_offset +
		bkt_p->node_rec_dsc[bkt_p->node_count-1].rec_length) -
	        bkt_p->node_rec_dsc[middle].rec_offset) < max_size)
		{
		point1 = bottom;
		point2 = middle;
		goto leave_split_points;
		}
	}

    /* Just fill them up */
    bottom_len = 0;
    for (i=0; i < bkt_p->node_count; ++i)
	if ((bottom_len + sizeof(rec_descriptor_t) +
		bkt_p->node_rec_dsc[i].rec_length) < max_size)
	    /* Have another one */
	    bottom_len += sizeof(rec_descriptor_t) +
			bkt_p->node_rec_dsc[i].rec_length;
	else
	    {
	    point1 = i;
	    bottom_len = 0;
	    for (; i < bkt_p->node_count; ++i)
		if ((bottom_len + sizeof(rec_descriptor_t) +
			bkt_p->node_rec_dsc[i].rec_length) < max_size)
		    /* Have another one */
		    bottom_len += sizeof(rec_descriptor_t) +
			bkt_p->node_rec_dsc[i].rec_length;
		else
		    {
		    /* The rest must fit in the last bucket */
		    *point2_p = i;
		    /* Calculate size of the rest */
		    for (bottom_len=0; i < bkt_p->node_count; ++i)
			bottom_len += sizeof(rec_descriptor_t) +
			    bkt_p->node_rec_dsc[i].rec_length;
		    if (max_size < bottom_len)
			DCE_SVC_DEBUG((
                            cds__svc_handle,
                            cds_svc_database,
                            svc_c_debug2,
			    "W db_btree: split_points(%s,%p,%p,%u) last bucket too big (%u bytes)",
			    deb_ascii_node_header(bktBuf,bkt_p),
                            point1_p, point2_p, max_size, bottom_len));

		    goto leave_split_points;
		    }
	    }

    /*
     * Could we REALLY get here ? 
     */

leave_split_points:
    if (point1_p != NULL) *point1_p = point1;
    if (point2_p != NULL) *point2_p = point2;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " <db_btree:split_points(%s,%p->%u,%p->%u,%u)",
        deb_ascii_node_header(bktBuf,bkt_p),
        point1_p, point1, point2_p, point2, max_size));
}

/*
 * split_bucket
 *	This routine does the splitting of either a data or index bucket.
 *	No record within the bucket must be larger than
 *	DATA_BUCKET_BLOCKS * BLOCK_SIZE.
 *
 *	Original bucket will still be first bucket in chain.
 *
 * Input:
 *	prm_p = context
 *	expand_p = pointer to expanded data bucket
 *	bkt_p = original bucket
 *
 * Returns:
 *	DNS_SUCCESS = split successful
 *	DNS_NONSRESOURCES = malloc() failure
 */
static dns_status_t 
split_bucket (btree_prm_t   *const prm_p,
              node_header_t *const expand_p,
              node_header_t *const bkt_p)
{
    DEB_ASCII_BUF_btree_prm(prmBuf)
    DEB_ASCII_BUF_node_header(expandBuf)
    DEB_ASCII_BUF_node_header(bktBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    node_header_t *split1_bkt_p, *split2_bkt_p;
    unsigned int split1, split2;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " >db_btree:split_bucket(%s,%s,%s)",
	deb_ascii_btree_prm(prmBuf,prm_p),
        deb_ascii_node_header(expandBuf,expand_p),
        deb_ascii_node_header(bktBuf,bkt_p)));

    node_check(expand_p);
    node_check(bkt_p);

   /* Need to do some splitting, determine split points */
    split_points(expand_p, &split1, &split2, bkt_p->node_blocks*BLOCK_SIZE);

   /*
    * Since we could run out of resources here, we don't want to
    * destroy the original bucket until we know everything will succeed.
    * This means working our ways backwards.
    */

    /* Get new bucket for split */
    split2_bkt_p = (node_header_t *)dns_malloc_perm(bkt_p->node_blocks*BLOCK_SIZE);
    if (!split2_bkt_p)
	{
	status = DNS_NONSRESOURCES;
	goto leave_split_bucket;
	}

    split2_bkt_p->node_blocks = bkt_p->node_blocks;
    /* Insert records into end bucket */
    db_btree_copy_keys(expand_p, split2_bkt_p, bkt_p->node_blocks,
			split2, expand_p->node_count);

    if (split1 < split2)
	{
	split1_bkt_p = (node_header_t *)dns_malloc_perm(
		    bkt_p->node_blocks*BLOCK_SIZE);

	if (!split1_bkt_p)
	    {
	    dns_free((char *)split2_bkt_p);
	    status = DNS_NONSRESOURCES;
	    goto leave_split_bucket;
	    }

	split1_bkt_p->node_blocks = bkt_p->node_blocks;
	/* Insert records into middle bucket */
	db_btree_copy_keys(expand_p, split1_bkt_p, bkt_p->node_blocks, 
			    split1, split2);

	}

    /*** commit point ***/

    /* Insert records into low end bucket */
    db_btree_copy_keys(expand_p, bkt_p, bkt_p->node_blocks, 0, split1);

    /* Link other buckets horizontally after bkt_p */
    LINK_AFTER(bkt_p, split2_bkt_p);

    if (split1 < split2)
    {
	LINK_AFTER(bkt_p, split1_bkt_p);
    }

    /* If we have inserted after end_p, extend it */
    if (prm_p->prm_end_p == bkt_p)
	prm_p->prm_end_p = split2_bkt_p;

    /* If this bucket is pointed by an index, the last key has changed */
    if (prm_p->prm_key_p != NULL)
    {
	entry_key_t *const key_p = prm_p->prm_key_p;

	if (key_p->key_down_p == bkt_p)
	    key_p->key_down_p = split2_bkt_p;
    }

    status = DNS_SUCCESS;

leave_split_bucket:
    dns_free((char *)expand_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " <db_btree:split_bucket(%s,%p,%s) return(%s)",
	deb_ascii_btree_prm(prmBuf,prm_p),
        expand_p,
        deb_ascii_node_header(bktBuf,bkt_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/*
 * new_root
 *	Create a new root for the Btree.
 *	This routine checks to see if the top layer
 *	has expanded enough to create another layer.
 *
 *	It also insures that hash_p is updated
 *	in case the root bucket has been replaced.
 *
 * Inputs:
 *	hash_p = the hash table pointer
 *	start_p = beginning of replacement buckets
 *
 * Returns:
 *	DNS_SUCCESS = hash contains good root pointer
 *	DNS_NONSRESOURCES = malloc() failure
 */
static dns_status_t 
new_root (dir_hash_entry_t    *const hash_p,
	  node_header_t       *start_p)
{
    DEB_ASCII_BUF_dir_hash_entry(hashBuf)
    DEB_ASCII_BUF_node_header(startBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    int status = DNS_SUCCESS;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_btree:new_root(%s,%s)",
	deb_ascii_dir_hash_entry(hashBuf,hash_p),
	deb_ascii_node_header(startBuf,start_p)));

    /* If there is more than one bucket, time to add another layer */
    if (start_p->node_next_p != NULL)
    {
	/* Need to allocate a new root */
	node_header_t *const root_bkt_p = (node_header_t *)
			dns_malloc_perm(INDEX_BUCKET_BLOCKS*BLOCK_SIZE);

	if (root_bkt_p)
	{
	    node_header_t *bkt_p = start_p;
	    int pos = 0;

	    /* Initialize index bucket */
	    root_bkt_p->node_type = INDEX_BUCKET;
	    root_bkt_p->node_blocks =  INDEX_BUCKET_BLOCKS;
	    root_bkt_p->node_next_p = NULL;
	    root_bkt_p->node_prev_p = NULL;
	    root_bkt_p->node_count = 0;
	    root_bkt_p->node_space = INDEX_BUCKET_BLOCKS*BLOCK_SIZE
		- DIFF_bytes(root_bkt_p->node_rec_dsc, root_bkt_p);

	    /* Add index entries to all those buckets */
	    do {
		status = insert_index(root_bkt_p, pos++, bkt_p);
		/*
		 * NOTE: we could avoid the assert() by
		 * doing a back-off on failure to insure
		 * that the last sentinel entry gets included.
		 */
		dce_assert(cds__svc_handle, (0 == status));
	    } while ((bkt_p = bkt_p->node_next_p) != NULL);

	    node_check(root_bkt_p);
	    start_p = root_bkt_p;
	}
	else
	{
	    /* Keep what we have and try again later */
	    status = DNS_NONSRESOURCES;
	}
    }
    else while (start_p->node_count == 1)
    {
	/* Eliminate useless index layers */
	node_header_t *const down_p = THIS_SUBLIST(start_p,0);

	dce_assert(cds__svc_handle, (down_p->node_next_p == NULL));
	dce_assert(cds__svc_handle, (down_p->node_prev_p == NULL));

	if (start_p->node_type != INDEX_BUCKET) {
	    break;
	}
	UNLINK_BKT(start_p);
	dns_free((char *)start_p);
	start_p = down_p;
    }

    /* Update root index bucket vbn in directory bucket */
    hash_p->dir_entry_p = start_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_btree:new_root(%s,%s) return(%s)",
	deb_ascii_dir_hash_entry(hashBuf,hash_p),
	deb_ascii_node_header(startBuf,start_p),
	deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/*
 * purge_data_bucket
 *	Remove records which were deleted before current allupto
 *	Do not remove empty buckets at this point!
 *
 * Inputs:
 *	bkt_p = pointer to bucket to purge
 *	allupto_p = pointer to directory CDS_AllUpTo
 *
 * Returns: TRUE if last entry deleted, otherwise FALSE.
 */
static int
purge_data_bucket (btree_prm_t		*const prm_p,
		   node_header_t	*const bkt_p,
		   Timestamp_t		*const allupto_p)
{
    DEB_ASCII_BUF_btree_prm(prmBuf)
    DEB_ASCII_BUF_node_header(bktBuf)
    DEB_ASCII_BUF_Timestamp(alluptoBuf)
    rec_descriptor_t *rd_p;
    unsigned int count = bkt_p->node_count;
    unsigned int len;
    int offset, pos;
    int ret = FALSE;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >db_btree:purge_data_bucket(%s,%s,%s)",
	deb_ascii_btree_prm(prmBuf,prm_p),
        deb_ascii_node_header(bktBuf,bkt_p),
        deb_ascii_Timestamp(alluptoBuf,allupto_p)));

    /* We must have CDS_AllUpTo to get here */
    dce_assert(cds__svc_handle, (allupto_p != NULL));

    node_check(bkt_p);

    /* No more empty buckets */
    dce_assert(cds__svc_handle, (bkt_p->node_count > 0));

    for (pos = 0; pos < count; ++pos)
    {
	/* Get pointer to entry data record */
	register entry_key_t *data_p = (entry_key_t *)INDEX_node_rec(bkt_p, pos);
	DBSet_t *set_p;

	/* End of the line flag, no set following this */
	if (EXT8(data_p->key_type) == 255) continue;

	set_p = (DBSet_t *)DATA_entry_data_record(data_p);
	ALIGN_CHECK(set_p);

	/* Is it absent ? */
	if (EXT8(set_p->ds_flag) == DS_present) continue;

	/* Check if this delete has been skulked */
	if (dnsCmpCTS((dns_cts *)allupto_p, (dns_cts *)set_p->ds_ts) <=0) continue;

	/* Save my length */
	len = bkt_p->node_rec_dsc[pos].rec_length;

	/* Remove data if not last record */
	if (pos < --count)
	{
	    len = ALIGN_LEN(len);
	    MOVE_bytes(INDEX_node_rec(bkt_p, pos+1), INDEX_node_rec(bkt_p, pos),
		(bkt_p->node_rec_dsc[count].rec_offset +
		bkt_p->node_rec_dsc[count].rec_length) -
		bkt_p->node_rec_dsc[pos+1].rec_offset);

	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_database,
		svc_c_debug4,
		"I db_btree:purge_data_bucket(%s,%p,%s) remove(%d) of (%u)",
		deb_ascii_btree_prm(prmBuf,prm_p), bkt_p,
		deb_ascii_Timestamp(alluptoBuf,allupto_p), pos, count));
	}
	else
	{
	    entry_key_t *const key_p = prm_p->prm_key_p;

	    if (count > 0)
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_database,
		    svc_c_debug1,
		    "I db_btree:purge_data_bucket(%s,%p,%s) last entry(%d) deleted",
		    deb_ascii_btree_prm(prmBuf,prm_p), bkt_p,
		    deb_ascii_Timestamp(alluptoBuf,allupto_p), pos));
	    }
	    else
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_database,
		    svc_c_debug1,
		    "I db_btree:purge_data_bucket(%s,%p,%s) bucket emptied",
		    deb_ascii_btree_prm(prmBuf,prm_p), bkt_p,
		    deb_ascii_Timestamp(alluptoBuf,allupto_p)));
	    }

	    /* Parent key is no longer valid */
	    if ((key_p != NULL) && (bkt_p == key_p->key_down_p))
		key_p->key_down_p = NULL;
	}

	/* Remove descriptor */
	MOVE_bytes(INDEX_node_rec_dsc(bkt_p, pos+1),
		INDEX_node_rec_dsc(bkt_p, pos),
		(bkt_p->node_rec_dsc[count].rec_offset +
		bkt_p->node_rec_dsc[count].rec_length) - len
		- DIFF_bytes(INDEX_node_rec_dsc(bkt_p, pos+1), bkt_p));

	for (offset=0; offset < pos; ++offset)
	    bkt_p->node_rec_dsc[offset].rec_offset -= sizeof(rec_descriptor_t);

	for (offset=pos; offset < count; ++offset)
	    bkt_p->node_rec_dsc[offset].rec_offset -=
		    (len + sizeof(rec_descriptor_t));
	--pos;
    }  /* end for */

    if (count > 0)
    {
	    rd_p = &bkt_p->node_rec_dsc[count-1];
	    bkt_p->node_space = (bkt_p->node_blocks*BLOCK_SIZE)
	      - (rd_p->rec_offset + rd_p->rec_length);
    }
    else
    {
	    bkt_p->node_space = (bkt_p->node_blocks*BLOCK_SIZE)
	      - DIFF_bytes(bkt_p->node_rec_dsc, bkt_p);
    }

    bkt_p->node_count = count;
    node_check(bkt_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <db_btree:purge_data_bucket(%s,%s,%s) return(%d)",
	deb_ascii_btree_prm(prmBuf,prm_p),
        deb_ascii_node_header(bktBuf,bkt_p),
        deb_ascii_Timestamp(alluptoBuf,allupto_p), ret));
    return(ret);
}


/*
 * split_bucket_for_continuation
 *	An entry has grown large enough to require a continuation bucket
 *
 * Inputs:
 *	prm_p = context contains:
 *	expand_p = pointer to expanded bucket containing all data
 *	pos = position of entry in old bkt
 *
 * Returns:
 *	DNS_SUCCESS = split successful
 *	DNS_NONSRESOURCES = malloc() failure
 */
static dns_status_t 
split_bucket_for_continuation (btree_prm_t   *const prm_p,
                               node_header_t *const expand_p,
                               node_header_t *const bkt_p,
                               int           pos)
{
    DEB_ASCII_BUF_btree_prm(prmBuf)
    DEB_ASCII_BUF_node_header(expandBuf)
    DEB_ASCII_BUF_node_header(bktBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    node_header_t *new_bkt_p;
    node_header_t *split1_bkt_p = NULL;
    node_header_t *split2_bkt_p;
    entry_key_t *data_p;
    DBSet_t *set_p;
    int size, split1, split2;
    int new_size;
    int split1_size = 0;
    int split2_size = 0;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " >db_btree:split_bucket_for_continuation(%s,%s,%s,%d)",
	deb_ascii_btree_prm(prmBuf,prm_p),
        deb_ascii_node_header(expandBuf,expand_p),
        deb_ascii_node_header(bktBuf,bkt_p), pos));

    node_check(expand_p);
    /* Determine blocks needed for large record */
    data_p = (entry_key_t *)INDEX_node_rec(expand_p, pos);
    set_p = (DBSet_t *)DATA_entry_data_record(data_p);
    ALIGN_CHECK(set_p);
    size = DIFF_bytes(&expand_p->node_rec_dsc[1], expand_p)
		+ DIFF_bytes(SKIP_DBSet(set_p), data_p);
    size = dns_roundup(size);			 /* Get number of buckets  */

    /* First entry is 1 bucket split */
    if (pos == 0)
	{
	/* First bucket contains large record */
	new_size = size;

	split2_size = DATA_BUCKET_BLOCKS;
	split2 = 1;
        split1 = 1;
	}
    /* Last entry is 1 bucket split */
    else if (pos == (expand_p->node_count - 1))
 	{
	/* Allocate a bucket */
	new_size = DATA_BUCKET_BLOCKS;

	/* Last bucket contains large record */
	split2_size = size;
	split2 = expand_p->node_count-1;
        split1 = 1;
	}
    else
	{
	/* Allocate a bucket */
	new_size = DATA_BUCKET_BLOCKS;

	/* Middle bucket contains large record */
	split1_size = size;

	/* Allocate a bucket */
	split2_size = DATA_BUCKET_BLOCKS;

	split2 = pos+1;
	split1 = pos;
	}

    /* We have to replace the existing bucket, so allocate it now */
    new_bkt_p = (node_header_t *)dns_malloc_perm(new_size*BLOCK_SIZE);
    if (!new_bkt_p)
	{
	status = DNS_NONSRESOURCES;
	goto leave_split_bucket_for_continuation;
	}
    new_bkt_p->node_blocks = new_size;

    /* Allocate last bucket */
    split2_bkt_p = (node_header_t *)dns_malloc_perm(
			split2_size*BLOCK_SIZE);
    if (!split2_bkt_p)
	{
	dns_free((char *)new_bkt_p);
	status = DNS_NONSRESOURCES;
	goto leave_split_bucket_for_continuation;
	}
    split2_bkt_p->node_blocks = split2_size;

    /* Insert records into middle bucket */
    db_btree_copy_keys(expand_p, split2_bkt_p, split2_size,
		split2, expand_p->node_count);

    if (split1_size > 0)
	{
	/* Middle bucket contains large record */
	split1_bkt_p = (node_header_t *)dns_malloc_perm(
			split1_size*BLOCK_SIZE);
	if (!split1_bkt_p)
	    {
	    dns_free((char *)split2_bkt_p);
	    dns_free((char *)new_bkt_p);
	    status = DNS_NONSRESOURCES;
	    goto leave_split_bucket_for_continuation;
	    }
	split1_bkt_p->node_blocks = split1_size;

	/* Insert records into middle bucket */
	db_btree_copy_keys(expand_p, split1_bkt_p, split1_size,
			   split1, split2);
	}

    /*** commit point ***/

    /* Insert records into low end bucket */
    db_btree_copy_keys(expand_p, new_bkt_p, new_bkt_p->node_blocks, 0, split1);

    REPLACE_BKT(expand_p, new_bkt_p);
    node_check(new_bkt_p);

    LINK_AFTER(new_bkt_p, split2_bkt_p);
    node_check(split2_bkt_p);

    if (split1_size > 0)
    {
	LINK_AFTER(new_bkt_p, split1_bkt_p);
	node_check(split1_bkt_p);
    }

    /* If bkt_p is old key, new start for re-indexing */
    if (prm_p->prm_index_p == bkt_p)
	prm_p->prm_index_p = new_bkt_p;

    /* If bkt_p is at the end of the chain, change to the new end */
    if (prm_p->prm_end_p == bkt_p)
      prm_p->prm_end_p = split2_bkt_p;

    /* Retarget index pointer */
    if (prm_p->prm_key_p != NULL)
    {
	entry_key_t *const down_p = prm_p->prm_key_p;

	if (down_p->key_down_p == bkt_p)
	    down_p->key_down_p = split2_bkt_p;
    }

    dns_free((char *)bkt_p);
    status = DNS_SUCCESS;

leave_split_bucket_for_continuation:
    dns_free((char *)expand_p);

    /* Warning: expand_p may have been released */
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " <db_btree:split_bucket_for_continuation(%s,%p,%s,%d) return(%s)",
	deb_ascii_btree_prm(prmBuf,prm_p), expand_p,
        deb_ascii_node_header(bktBuf,bkt_p), pos,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * update_index
 *	Insert keys into the btree
 *
 * Inputs:
 *	ctx_p = context contains:
 *
 * Returns:
 */
static dns_status_t 
update_index (btree_ctx_t	*const ctx_p,
	     btree_prm_t	*const child_p,
	     btree_prm_t	*const parent_p,
	     node_header_t	*bkt_p)
{
    DEB_ASCII_BUF_btree_ctx(ctxBuf)
    DEB_ASCII_BUF_btree_prm(childBuf)
    DEB_ASCII_BUF_btree_prm(parentBuf)
    DEB_ASCII_BUF_node_header(bktBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    entry_key_t *start_key_p = NULL;
    node_header_t *next_p;
    node_header_t *skip_p = NULL;
    node_header_t *expand_p;
    unsigned int pos;
    int new_size = 0;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >db_btree:update_index(%s,%s,%s,%s)",
	deb_ascii_btree_ctx(ctxBuf,ctx_p),
	deb_ascii_btree_prm(childBuf,child_p),
	deb_ascii_btree_prm(parentBuf,parent_p),
	deb_ascii_node_header(bktBuf,bkt_p)));

    /* Find out right away if this was not set */
    dce_assert(cds__svc_handle, (child_p->prm_index_p != NULL));
    dce_assert(cds__svc_handle, (parent_p->prm_index_p != NULL));

    /*
     * If the child node has gone, remove the index pointer
     * before we start other index surgery, but don't check
     * if this bucket is empty yet.
     */
    if (child_p->prm_key_p != NULL)
    {
	DEB_ASCII_BUF_entry_key(child_keyBuf)
	entry_key_t *const child_key_p = child_p->prm_key_p;
	skip_p = child_key_p->key_down_p;

	if (skip_p == NULL)
	{
	    entry_key_t *const parent_key_p = parent_p->prm_key_p;
	    pos = child_p->prm_key_pos;

	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_database,
		svc_c_debug1,
		"I db_btree:update_index(%s,%s,%s,%s) key(%s) removed",
		deb_ascii_btree_ctx(ctxBuf,ctx_p),
		deb_ascii_btree_prm(childBuf,child_p),
		deb_ascii_btree_prm(parentBuf,parent_p),
		deb_ascii_node_header(bktBuf,bkt_p),
		deb_ascii_entry_key(child_keyBuf,child_key_p)));

	    remove_record(bkt_p, pos);
	    child_p->prm_key_p = NULL;

	    /* If parent's key pnts here & last entry rmved, orphaned again */
	    if ((bkt_p->node_count == pos) &&
		(parent_key_p != NULL) &&
		(bkt_p == parent_key_p->key_down_p))
	    {
		parent_key_p->key_down_p = NULL;
	    }
	}
    }

    /*
     * Go through the child nodes, weeding out the empty buckets,
     * getting the first key, and figuring the space needed
     * for all the new keys.
     */
    for (next_p = child_p->prm_index_p;
	 next_p != child_p->prm_end_p; )
    {
	DEB_ASCII_BUF_node_header(thisBuf)
	node_header_t *const this_p = next_p;

	if (this_p == NULL)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_database,
		svc_c_debug1,
		"E db_btree:update_index(%s,%s,%s,%s) broken index level",
		deb_ascii_btree_ctx(ctxBuf,ctx_p),
		deb_ascii_btree_prm(childBuf,child_p),
		deb_ascii_btree_prm(parentBuf,parent_p),
		deb_ascii_node_header(bktBuf,bkt_p)));

	    dce_assert(cds__svc_handle, (0 == 1));

	    status = DNS_DATACORRUPTION;
	    goto leave_update_index;
	}
	next_p = next_p->node_next_p;

	/* Don't include an already indexed node */
	if (this_p == skip_p)
	{
	    continue;
	}
	/* Remove purged buckets now */
	if (this_p->node_count == 0)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_database,
		svc_c_debug1,
		"I db_btree:update_index(%s,%s,%s,%s) empty node(%s) removed",
		deb_ascii_btree_ctx(ctxBuf,ctx_p),
		deb_ascii_btree_prm(childBuf,child_p),
		deb_ascii_btree_prm(parentBuf,parent_p),
		deb_ascii_node_header(bktBuf,bkt_p),
		deb_ascii_node_header(thisBuf,this_p)));

	    /* Bump the re-index node */
	    if (child_p->prm_index_p == this_p)
		child_p->prm_index_p = next_p;

	    /* Bump the re-index end node */
	    if (child_p->prm_end_p == this_p)
		child_p->prm_end_p = next_p;

	    UNLINK_BKT(this_p);
	    dns_free((char *)this_p);
	}
	else
	{
	    entry_key_t *const key_p = (entry_key_t *)
			INDEX_node_rec(this_p, this_p->node_count-1);

	    new_size += ALIGN_SIZE + sizeof(rec_descriptor_t)
				   + LEN_entry_key(key_p);

	    if (start_key_p == NULL)
		start_key_p = key_p;
	}
    }

    /* No entries to index ? */
    if (start_key_p == NULL)
    {
	status = DNS_SUCCESS;
	goto leave_update_index;
    }

    /* Make sure we have the correct bucket */
    for (;;)
    {
	status = find_entry_pos(bkt_p, start_key_p, &pos);

	if (pos < bkt_p->node_count)
	    break;

	if (bkt_p->node_next_p == parent_p->prm_end_p)
	    break;

	bkt_p = bkt_p->node_next_p;
	if (!bkt_p)
	{
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug4,
		"E db_btree: update_index(%s,%s,%s,%s) find_entry_pos(,,%u) invalid",
		deb_ascii_btree_ctx(ctxBuf,ctx_p),
		deb_ascii_btree_prm(childBuf,child_p),
		deb_ascii_btree_prm(parentBuf,parent_p),
		deb_ascii_node_header(bktBuf,bkt_p), pos));

            status = DNS_NAMESERVERBUG;
	    goto leave_update_index;
	}
    }

    /* If start_key_p index defined we have a bug */
    dce_assert(cds__svc_handle, (status != DNS_SUCCESS));

    /* If all will fit we are all set */
    if (new_size <= bkt_p->node_space)
    {
	/* Load up the keys */
	for (next_p = child_p->prm_index_p;
	     next_p != child_p->prm_end_p;
	     pos++, next_p = next_p->node_next_p)
	{
	    if (next_p == skip_p)
		continue;

	    status = insert_index(bkt_p, pos, next_p);
	    /* We made sure above that they would all fit */
	    dce_assert(cds__svc_handle, (0 == status));
	}

	status = DNS_SUCCESS;
	goto leave_update_index;
    }

    /* Data won't fit in current bucket.  First extend bucket so it
     * will fit, then worry about splitting it
     */
    expand_p = (node_header_t *)dns_malloc_perm(
		INDEX_BUCKET_BLOCKS*BLOCK_SIZE + new_size);
    if (!expand_p)
    {
	status = DNS_NONSRESOURCES;
	goto check_update_index;
    }

    MOVE_bytes(bkt_p, expand_p, INDEX_BUCKET_BLOCKS*BLOCK_SIZE);
    expand_p->node_blocks = 0;		/* temporary bucket */
    expand_p->node_space += new_size;

    /* find the insertion point in the expansion bucket */
    (void)find_entry_pos(expand_p, start_key_p, &pos);

    /* Load up the expanded bucket with the keys */
    for (next_p = child_p->prm_index_p;
	 next_p != child_p->prm_end_p;
	 pos++, next_p = next_p->node_next_p)
    {
	if (next_p == skip_p)
	    continue;

	status = insert_index(expand_p, pos, next_p);
	/* We made sure above that they would all fit */
	dce_assert(cds__svc_handle, (0 == status));
    }

    /* Split the expansion bucket back into bkt_p and more... */
    status = split_bucket(parent_p, expand_p, bkt_p);

    if (status != DNS_SUCCESS)
    {
check_update_index:
	/*
	 * If this is a left-most parent, and does not point
	 * to a left-most child, kill the server
	 */
	if (bkt_p->node_prev_p == NULL)
	{
	    node_header_t *const down_p = THIS_SUBLIST(bkt_p, 0);

	    if (down_p->node_prev_p != NULL)
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_database,
		    svc_c_debug1,
		    "F db_btree:update_index(%s,%s,%s,%s) status(%s) left-most key lost!!",
		    deb_ascii_btree_ctx(ctxBuf,ctx_p),
		    deb_ascii_btree_prm(childBuf,child_p),
		    deb_ascii_btree_prm(parentBuf,parent_p),
		    deb_ascii_node_header(bktBuf,bkt_p),
		    deb_ascii_dns_status(statusBuf,status)));

		index_dump(ctx_p->ctx_hash_p->dir_entry_p, 0);
		dce_assert(cds__svc_handle, (down_p->node_prev_p == NULL));
	    }
	}
    }

leave_update_index:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <db_btree:update_index(%s,%s,%s,%s) return(%s)",
	deb_ascii_btree_ctx(ctxBuf,ctx_p),
	deb_ascii_btree_prm(childBuf,child_p),
	deb_ascii_btree_prm(parentBuf,parent_p),
	deb_ascii_node_header(bktBuf,bkt_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * build_delete_holder
 *	When a record is created and a record of the same name already
 *	exists but is marked for delete, the new record is created
 *	with special place holders for the deleted record.  The
 *	combine and spread phases of a skulk must recognize these updates.
 *
 * Input:
 *	stream_p = pointer to stream descriptor
 *	data_p = pointer to entry key (unused)
 *	main_p = pointer to main set in record
 *
 */
static dns_status_t 
build_delete_holder (db_stream_t  *const stream_p,
                     entry_key_t  *const data_p,
                     DBSet_t      *const main_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_entry_key(dataBuf)
    DEB_ASCII_BUF_DBSet(mainBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    DBSet_t *set_p = (DBSet_t *)stream_p->db_entry.data_p;
    DBSetMember_t *value_p;
    delete_holder_t *delete_p;
    int idx, pos, len, move_len;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_btree:build_delete_holder(%s,%s,%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_entry_key(dataBuf,data_p),
        deb_ascii_DBSet(mainBuf,main_p)));

    att_set_check(main_p);
    ALIGN_CHECK(set_p);

    /* The new set must NOT have any delete holders */
    dce_assert(cds__svc_handle,
	       (set_p->ds_hcnt == 0));
    /*
     * Get new length needed to support new record (in stream_p),
     * a new delete_holder for this record (in stream_p)
     * and any delete_holders already kept for this record (in rec_p)
     *
     * If the space needed to align the delete holders is already
     * available in the padding to align the DBSet of the first entry,
     * then use the pad space.
     */
    move_len = DATA_DBSet_Delete(set_p) - DATA_DBSet(set_p);
    len = INDEX_DBSet(set_p, 0) - DATA_DBSet(set_p);	/* pad space */
    if (move_len < len) move_len = 0;
    move_len = ALIGN_LEN(move_len + sizeof(*delete_p) + LEN_DBSet_Delete(main_p));

    len = LEN_DBSet(set_p) + move_len;

    /* If current buffer is too small allocate a new one */
    if (stream_p->db_entry.data_buf_len < len)
	{
	DBSet_t *newset_p;

	len = dns_roundup(len)*BLOCK_SIZE;
	newset_p = (DBSet_t *)dns_malloc_perm(len);
	if (!newset_p)
	    {
	    status = DNS_NONSRESOURCES;
	    goto leave_build_delete_holder;
	    }
	att_set_check(set_p);
	COPY_DBSet(set_p, newset_p);
	dns_free((char *)set_p);
	stream_p->db_entry.data_p = (data_record_t *)newset_p;
	stream_p->db_entry.data_buf_len = len;
	set_p = newset_p;
	ALIGN_CHECK(set_p);
	}

    /* Shift new record to store this data */
    if (0 < set_p->ds_ocnt)
	{
	byte_t *const from_p = INDEX_DBSet(set_p,0);

	MOVE_bytes(from_p, SKIP_bytes(from_p, move_len),
			SKIP_DBSet(set_p) - from_p);

	/* Adjust offsets */
	for (pos = 0; pos < set_p->ds_ocnt; pos++)
	    set_p->ds_offp[pos] += move_len;
	}

    set_p->ds_setl += move_len;
    set_p->ds_hcnt = 1 + main_p->ds_hcnt;

    /*
     * NOTE: new delete holder being put on the front of the list
     * instead of the back !!!
     */

    /* Move in holder for this record */
    delete_p = (delete_holder_t *)DATA_DBSet_Delete(set_p);
    status = find_attribute((data_record_t *)main_p,
		dns_AttributeName(DNS_CTS), &set_p, &idx);
    if (status != DNS_SUCCESS)
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug5,
	    "E db_btree:build_delete_holder(%s,%s,%s) %s = find_attribute(DNS$CTS)",
	    deb_ascii_db_stream(streamBuf,stream_p),
	    deb_ascii_entry_key(dataBuf,data_p),
	    deb_ascii_DBSet(mainBuf,main_p),
	    deb_ascii_dns_status(statusBuf,status)));

	status = DNS_NAMESERVERBUG;
	goto leave_build_delete_holder;
    }

    /*
     * DNS_CTS is always a single valued set with no history
     */
    value_p = (DBSetMember_t *)DATA_DBSet(set_p);
    COPY_Timestamp(value_p->dm_data, delete_p->del_uid);

    /* Copy deletion timestamp */
    COPY_Timestamp(main_p->ds_ts, delete_p->del_ts);

    /* Bump pointer for next one */
    delete_p++;

    /* Copy any holders from the old record to new */
    if (0 < main_p->ds_hcnt)
	COPY_bytes(DATA_DBSet_Delete(main_p), delete_p, LEN_DBSet_Delete(main_p));

    att_set_check((DBSet_t *)stream_p->db_entry.data_p);
    status = DNS_SUCCESS;

leave_build_delete_holder:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_btree:build_delete_holder(%s,%s,%s) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_entry_key(dataBuf,data_p),
        deb_ascii_DBSet(mainBuf,main_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/*
 * insert_data
 *	Insert a data record into an entry data bucket
 *
 * Input:
 *	ctx_p = pointer to return insert context
 *	bkt_p = pointer to data bucket
 *
 */
static dns_status_t 
insert_data (btree_ctx_t	*const ctx_p,
	     btree_prm_t	*const prm_p,
	     node_header_t	*const start_p)
{
    DEB_ASCII_BUF_btree_ctx(ctxBuf)
    DEB_ASCII_BUF_btree_prm(prmBuf)
    DEB_ASCII_BUF_node_header(startBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    db_stream_t *const stream_p = ctx_p->ctx_stream_p;
    entry_key_t *const key_p = ctx_p->ctx_key_p;
    node_header_t *bkt_p = start_p;
    node_header_t *newbkt_p,*expand_p;
    unsigned int pos, new_size;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >db_btree:insert_data(%s,%s,%s)",
        deb_ascii_btree_ctx(ctxBuf,ctx_p),
        deb_ascii_btree_prm(prmBuf,prm_p),
        deb_ascii_node_header(startBuf,start_p)));

    node_check(bkt_p);
    /*
     * It is possible that a new bucket did not make it into the
     * index structure.  If this is the case, we may have to read
     * adjacent buckets to find the right position for the new data.
     */
    status = find_entry_pos(bkt_p, key_p, &pos);
    while (bkt_p->node_count <= pos)
	{
	if (bkt_p->node_next_p == prm_p->prm_end_p)
	    break;

	bkt_p = bkt_p->node_next_p;
	if (!bkt_p)
	{
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug4,
		"E db_btree:insert_data(%s,%s,%s) find_entry_pos(,,%u) invalid",
		deb_ascii_btree_ctx(ctxBuf,ctx_p),
		deb_ascii_btree_prm(prmBuf,prm_p),
		deb_ascii_node_header(startBuf,start_p), pos));

            status = DNS_NAMESERVERBUG;
	    goto leave_insert_data;
	}

	/* Assert a valid index structure. */
	if (bkt_p == prm_p->prm_end_p)
	{
	    dce_svc_printf(CDS_S_SERVER_DBINDEX_MSG);
	    status = DNS_DATACORRUPTION;
	    goto leave_insert_data;
	}

	status = find_entry_pos(bkt_p, key_p, &pos);
    }

    /* Compute size needed in current bucket */
    new_size = DIFF_bytes(DATA_entry_data_record(key_p), key_p)
		+ LEN_DBSet(stream_p->db_entry.data_p);

    /* Found a match */
    if (status == DNS_SUCCESS)
	{
	/* Pointer to entry_data_record */
	entry_key_t *data_p = (entry_key_t *)INDEX_node_rec(bkt_p, pos);

	/* Get pointer to actual data itself */
	DBSet_t *const set_p = (DBSet_t *)DATA_entry_data_record(data_p);
	ALIGN_CHECK(set_p);

	/* If entry is present, cannot create another */
	if (EXT8(set_p->ds_flag) == DS_present)
	    {
	    status = DNS_ENTRYEXISTS;
	    goto leave_insert_data;
	    }

	status = build_delete_holder(stream_p, data_p, set_p);

	if (status != DNS_SUCCESS)
	    goto leave_insert_data;

	/* This operations now becomes a replace */
	ctx_p->ctx_allupto_p = NULL;
	status = replace_data(ctx_p, prm_p, start_p);
	goto leave_insert_data;
	}

    /*
     * If this bucket contains a record which is larger
     * then a standard data bucket, the new record cannot be stored here.
     * Must allocate a new bucket.
     */
    if (DATA_BUCKET_BLOCKS < bkt_p->node_blocks)
	{
	unsigned int size =
	    DIFF_bytes(&bkt_p->node_rec_dsc[1], bkt_p) + new_size;

	/* missing history - keep this (this is always true anyway) */
	dce_assert(cds__svc_handle, (bkt_p->node_count > 0));

        if ((DATA_BUCKET_BLOCKS * BLOCK_SIZE) < size) {
    	    size = dns_roundup(size);		 /* Get the NUMBER OF BLOCKS  */
	    newbkt_p = (node_header_t *)dns_malloc_perm(size*BLOCK_SIZE);
	    if (!newbkt_p)
		{
		status = DNS_NONSRESOURCES;
		goto leave_insert_data;
		}

	    newbkt_p->node_blocks = size;
	  } else {
	    newbkt_p = (node_header_t *)dns_malloc_perm(
		DATA_BUCKET_BLOCKS*BLOCK_SIZE);
	    if (!newbkt_p)
		{
		status = DNS_NONSRESOURCES;
		goto leave_insert_data;
		}

	    newbkt_p->node_blocks = DATA_BUCKET_BLOCKS;
	  }

	newbkt_p->node_type = DATA_BUCKET;
	newbkt_p->node_count = 0;
	newbkt_p->node_space = (newbkt_p->node_blocks*BLOCK_SIZE)
		- DIFF_bytes(newbkt_p->node_rec_dsc, newbkt_p);

	status = insert_record(newbkt_p, 0, key_p, 
			       (DBSet_t *)stream_p->db_entry.data_p);
	dce_assert(cds__svc_handle, (0 == status));

	/* Link new bucket into chain */
	LINK_BEFORE(bkt_p, newbkt_p);
	node_check(newbkt_p);

	/* If this is before the key, new start for re-indexing */
	if (prm_p->prm_index_p == bkt_p)
	    prm_p->prm_index_p = newbkt_p;

	status = DNS_SUCCESS;
	goto leave_insert_data;
	}

    /* If it will fit we are all set */
    if (insert_record(bkt_p, pos,
		key_p, (DBSet_t *)stream_p->db_entry.data_p) == 0)
	{
	status = DNS_SUCCESS;
	goto leave_insert_data;
	}

    /*
     * Data won't fit in current bucket.  First extend bucket so it
     * will fit, then worry about splitting it
     */
    expand_p = (node_header_t *)dns_malloc_perm(
		DATA_BUCKET_BLOCKS*BLOCK_SIZE +
		ALIGN_LEN(new_size + sizeof(rec_descriptor_t)));
    if (!expand_p)
	{
	status = DNS_NONSRESOURCES;
	goto leave_insert_data;
	}

    MOVE_bytes(bkt_p, expand_p, DATA_BUCKET_BLOCKS*BLOCK_SIZE);
    expand_p->node_blocks = 0;		/* temporary bucket */
    expand_p->node_space += ALIGN_LEN(new_size + sizeof(rec_descriptor_t));

    status = insert_record(expand_p, pos, key_p, 
			   (DBSet_t *)stream_p->db_entry.data_p);
    dce_assert(cds__svc_handle, (0 == status));

    /* See if this record requires a continuation bucket */
    if ((DATA_BUCKET_BLOCKS * BLOCK_SIZE) <=
	    (DIFF_bytes(&bkt_p->node_rec_dsc[1], bkt_p) + new_size))
	status = split_bucket_for_continuation(prm_p, expand_p, bkt_p, pos);
    else
	status = split_bucket(prm_p, expand_p, bkt_p);

leave_insert_data:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <db_btree:insert_data(%s,%s,%s) return(%s)",
	deb_ascii_btree_ctx(ctxBuf,ctx_p),
        deb_ascii_btree_prm(prmBuf,prm_p),
        deb_ascii_node_header(startBuf,start_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * replace_data
 *	Replace a data record in an entry data bucket
 *
 * Input:
 *	ctx_p = pointer to return insert context
 *	bkt_p = pointer to data bucket
 *
 */
static dns_status_t 
replace_data (btree_ctx_t	*const ctx_p,
	      btree_prm_t	*const prm_p,
	      node_header_t	*const start_p)
{
    DEB_ASCII_BUF_btree_ctx(ctxBuf)
    DEB_ASCII_BUF_btree_prm(prmBuf)
    DEB_ASCII_BUF_node_header(startBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    DBSet_t *const set_p = (DBSet_t *)ctx_p->ctx_stream_p->db_entry.data_p;
    entry_key_t *const key_p = ctx_p->ctx_key_p;
    node_header_t *bkt_p = start_p;
    node_header_t *expand_p;
    unsigned int pos, new_size;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >db_btree:replace_data(%s,%s,%s)",
	deb_ascii_btree_ctx(ctxBuf,ctx_p),
        deb_ascii_btree_prm(prmBuf,prm_p),
        deb_ascii_node_header(startBuf,start_p)));

    node_check(bkt_p);

    /*
     * It is possible that a new bucket did not make it into the
     * index structure.  If this is the case, we may have to read
     * adjacent buckets to find right position for the new data.
     */

    for (;;)
    {
	/* Purge out deleted records from this bucket */
	if (ctx_p->ctx_allupto_p != NULL)
	    purge_data_bucket(prm_p, bkt_p, ctx_p->ctx_allupto_p);

	status = find_entry_pos(bkt_p, key_p, &pos);

	if (pos < bkt_p->node_count)
	    break;

	if (bkt_p->node_next_p == prm_p->prm_end_p)
	    break;

	bkt_p = bkt_p->node_next_p;
	if (!bkt_p)
	{
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug4,
	        "E db_btree:replace_data(%s,%s,%s) find_entry_pos(,,%u) invalid",
		deb_ascii_btree_ctx(ctxBuf,ctx_p),
		deb_ascii_btree_prm(prmBuf,prm_p),
		deb_ascii_node_header(startBuf,start_p), pos));

	    status = DNS_NAMESERVERBUG;
	    goto leave_replace_data;
	}

	/* Assert a valid index structure. */
	if (bkt_p == prm_p->prm_end_p)
	{
	    dce_svc_printf(CDS_S_SERVER_DBINDEX_MSG);
	    status = DNS_DATACORRUPTION;
	    goto leave_replace_data;
	}
    }

    /* If match not found, cannot replace it */
    if (status != DNS_SUCCESS)
	goto leave_replace_data;

    /* Compute size needed in current bucket */
    new_size = DIFF_bytes(DATA_entry_data_record(key_p), key_p)
			+ LEN_DBSet(set_p);

    /*
     * Special handling if this record is a continuation bucket
     * or it is the only record in the bucket
     */
    if ((bkt_p->node_count == 1) || (DATA_BUCKET_BLOCKS < bkt_p->node_blocks))
	{
	/* Cheat to get the entry address */
	entry_key_t *newentry_p = (entry_key_t *)&bkt_p->node_rec_dsc[1];
	node_header_t *newbkt_p;
	unsigned int this_bkt;

	/* Will record fit in current space */
        if (new_size <=
	      ((bkt_p->node_blocks*BLOCK_SIZE) - DIFF_bytes(newentry_p, bkt_p)))
	    {
	    rec_descriptor_t *rd_p = &bkt_p->node_rec_dsc[0];

	    rd_p->rec_length =  new_size;
	    att_set_check(set_p);
	    MOVE_DBSet(set_p, DATA_entry_data_record(newentry_p));
	    /* Set space left in bucket */
	    bkt_p->node_space = (bkt_p->node_blocks*BLOCK_SIZE)
		- (rd_p->rec_offset + new_size);

	    status = DNS_SUCCESS;
	    goto leave_replace_data;
	    }

	/*
	 * Allocate a new bucket because new data does not fit where
	 * old data was.
	 */
	this_bkt = dns_roundup(DIFF_bytes(newentry_p, bkt_p) + new_size);
	newbkt_p = (node_header_t *)dns_malloc_perm(this_bkt * BLOCK_SIZE);
	if (!newbkt_p)
	    {
	    status = DNS_NONSRESOURCES;
	    goto leave_replace_data;
	    }
	newbkt_p->node_type = DATA_BUCKET;
	newbkt_p->node_blocks = this_bkt;
	newbkt_p->node_space = (this_bkt*BLOCK_SIZE)
		- DIFF_bytes(newbkt_p->node_rec_dsc, newbkt_p);
	newbkt_p->node_count = 0;

	/* Re-use old header in bkt_p */
	status = insert_record(newbkt_p, 0, newentry_p, set_p);
	dce_assert(cds__svc_handle, (0 == status));

	REPLACE_BKT(bkt_p, newbkt_p);
	node_check(newbkt_p);

	/* If bkt_p is old key, new start for re-indexing */
	if (prm_p->prm_index_p == bkt_p)
	    prm_p->prm_index_p = newbkt_p;

	/* Retarget parent index pointer */
	if (prm_p->prm_key_p->key_down_p == bkt_p)
	    prm_p->prm_key_p->key_down_p = newbkt_p;

	/* Free after insert because newentry_p points here */
       	dns_free((char *)bkt_p);
	status = DNS_SUCCESS;
	goto leave_replace_data;
	}

    /* If it will fit we are all set */
    if (replace_record(bkt_p, pos, set_p) == 0)
	{
	status = DNS_SUCCESS;
	goto leave_replace_data;
	}

    /*
     * Data won't fit in current bucket.  First extend bucket so it
     * will fit, then worry about splitting it
     */
    expand_p = (node_header_t *)dns_malloc_perm(
		DATA_BUCKET_BLOCKS*BLOCK_SIZE + ALIGN_LEN(new_size));
    if (!expand_p)
	{
	status = DNS_NONSRESOURCES;
	goto leave_replace_data;
	}

    MOVE_bytes(bkt_p, expand_p, DATA_BUCKET_BLOCKS*BLOCK_SIZE);
    expand_p->node_blocks = 0;		/* temporary bucket */
    expand_p->node_space += ALIGN_LEN(new_size);

    status = replace_record(expand_p, pos, set_p);
    dce_assert(cds__svc_handle, (0 == status));

    /* See if this record requires a continuation bucket */
    if ((DATA_BUCKET_BLOCKS * BLOCK_SIZE) <=
	    (new_size + DIFF_bytes(&expand_p->node_rec_dsc[1], expand_p)))
	status = split_bucket_for_continuation(prm_p, expand_p, bkt_p, pos);
    else
	status = split_bucket(prm_p, expand_p, bkt_p);

leave_replace_data:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <db_btree:replace_data(%s,%s,%s) return(%s)",
	deb_ascii_btree_ctx(ctxBuf,ctx_p),
        deb_ascii_btree_prm(prmBuf,prm_p),
        deb_ascii_node_header(startBuf,start_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * remove_data
 *	Remove a data record from an entry data bucket
 *
 * Input:
 *	ctx_p = pointer to return insert context
 *	bkt_p = pointer to data bucket
 *
 */
static dns_status_t 
remove_data (btree_ctx_t	*const ctx_p,
	     btree_prm_t	*const prm_p,
	     node_header_t	*const start_p)
{
    DEB_ASCII_BUF_btree_ctx(ctxBuf)
    DEB_ASCII_BUF_btree_prm(prmBuf)
    DEB_ASCII_BUF_node_header(startBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    entry_key_t *const key_p = ctx_p->ctx_key_p;
    node_header_t *bkt_p = start_p;
    unsigned int pos;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >db_btree:remove_data(%s,%s,%s)",
	deb_ascii_btree_ctx(ctxBuf,ctx_p),
        deb_ascii_btree_prm(prmBuf,prm_p),
        deb_ascii_node_header(startBuf,start_p)));

    node_check(bkt_p);

    /*
     * It is possible that a new bucket did not make it into the
     * index structure.  If this is the case, we may have to read
     * adjacent buckets to find right position for the new data.
     */

    for (;;)
    {
	status = find_entry_pos(bkt_p, key_p, &pos);

	if (status != DNS_SUCCESS) 
	    goto leave_remove_data;

	if (pos < bkt_p->node_count)
	    break;

	if (bkt_p->node_next_p == prm_p->prm_end_p)
	    break;

	bkt_p = bkt_p->node_next_p;
	if (!bkt_p)
	{
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug4,
	        "E db_btree:remove_data(%s,%s,%s) find_entry_pos(,,%u) invalid",
		deb_ascii_btree_ctx(ctxBuf,ctx_p),
		deb_ascii_btree_prm(prmBuf,prm_p),
		deb_ascii_node_header(startBuf,start_p), pos));

	    status = DNS_NAMESERVERBUG;
	    goto leave_remove_data;
	}

	/* Assert a valid index structure. */
	if (bkt_p == prm_p->prm_end_p)
	{
	    dce_svc_printf(CDS_S_SERVER_DBINDEX_MSG);
	    status = DNS_DATACORRUPTION;
	    goto leave_remove_data;
	}
    }

    remove_record(bkt_p, pos);

    /* If parent's key points here and last entry removed, we are orphaned */
    if (pos == bkt_p->node_count)
    {
	entry_key_t *const key_p = prm_p->prm_key_p;

	if ((key_p != NULL) && (bkt_p == key_p->key_down_p))
	    key_p->key_down_p = NULL;
    }

leave_remove_data:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <db_btree:remove_data(%s,%s,%s) return(%s)",
	deb_ascii_btree_ctx(ctxBuf,ctx_p),
        deb_ascii_btree_prm(prmBuf,prm_p),
        deb_ascii_node_header(startBuf,start_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * build_index
 *	This routine generates a level of index buckets
 *	for the btree.
 *
 * Input:
 *	hash_p = pointer to hash entry for a directory.
 *		 dir_entry_p contains pointer to first bucket in next level.
 */
static dns_status_t 
build_index (dir_hash_entry_t *const hash_p)
{
    DEB_ASCII_BUF_dir_hash_entry(hashBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    node_header_t *next_p = hash_p->dir_entry_p;
    node_header_t *index_bkt_p;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_btree:build_index(%s)",
        deb_ascii_dir_hash_entry(hashBuf,hash_p)));

    index_bkt_p = (node_header_t *)dns_malloc_perm(INDEX_BUCKET_BLOCKS*BLOCK_SIZE);
    if (!index_bkt_p)
	{
	status = DNS_NONSRESOURCES;
	goto leave_build_index;
	}

    /* Set up header */
    index_bkt_p->node_type = INDEX_BUCKET;
    index_bkt_p->node_blocks = INDEX_BUCKET_BLOCKS;
    index_bkt_p->node_next_p = NULL;
    index_bkt_p->node_prev_p = NULL;
    index_bkt_p->node_space = INDEX_BUCKET_BLOCKS*BLOCK_SIZE
	- DIFF_bytes(index_bkt_p->node_rec_dsc, index_bkt_p);
    index_bkt_p->node_count = 0;

    hash_p->dir_entry_p = index_bkt_p;

    while (next_p)
	{

        /*
         * If we've stumbled upon an empty bucket -- now's a darn ideal
         * time to get rid of it.
         */
        if (next_p->node_count == 0) 
	{
            node_header_t *empty_bkt_p = next_p;  /* Save empty bucket ptr */
            next_p = empty_bkt_p->node_next_p;
            UNLINK_BKT(empty_bkt_p);
            dns_free((char *) empty_bkt_p);

	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug5,
		"I build_index(%s): Empty bucket at %p deleted",
		deb_ascii_dir_hash_entry(hashBuf,hash_p),
                empty_bkt_p));
 
	    continue;                            /* Re-evaluate loop */
        }

	/* Get pointer to last record */
        dce_assert(cds__svc_handle, (next_p->node_count > 0));

	/* Determine if we need a new bucket */
	if (0 < insert_index(index_bkt_p, index_bkt_p->node_count, next_p))
	{
	    node_header_t *new_index_bkt_p = (node_header_t *)
			dns_malloc_perm(INDEX_BUCKET_BLOCKS*BLOCK_SIZE);

	    if (!new_index_bkt_p)
		{
	    	status = DNS_NONSRESOURCES;
		goto leave_build_index;
		}

	    /* Set up header */
	    new_index_bkt_p->node_type = INDEX_BUCKET;
	    new_index_bkt_p->node_blocks = INDEX_BUCKET_BLOCKS;
	    new_index_bkt_p->node_space = INDEX_BUCKET_BLOCKS*BLOCK_SIZE -
		DIFF_bytes(new_index_bkt_p->node_rec_dsc, new_index_bkt_p);
	    new_index_bkt_p->node_count = 0;
	    LINK_AFTER(index_bkt_p, new_index_bkt_p);
	    index_bkt_p = new_index_bkt_p;
	    status = insert_index(index_bkt_p, index_bkt_p->node_count, next_p);
	    dce_assert(cds__svc_handle, (0 == status));
	}
	next_p = next_p->node_next_p;
    }
    status = DNS_SUCCESS;

leave_build_index:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_btree:build_index(%s) return(%s)",
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * db_btree_build
 *	Read entry data buckets, allocate in memory structures and
 *	build btree index buckets.
 *
 * Input:
 *	file_p = pointer to file info block
 *	hash_p = pointer to directory hash entry
 *	next_vbn_p = address of next VBN to read
 *      conversion_ctx = database conversion context handle
 *
 * Output:
 *	next_vbn_p = updated
 */
dns_status_t 
db_btree_build (file_info_t       *file_p,
                dir_hash_entry_t  *hash_p,
                unsigned int      *next_vbn_p,
                conversion_ctx_t  conversion_ctx)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_dir_hash_entry(hashBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    static union {
	node_header_t	node;
	bytes_u		buffer[DATA_BUCKET_BLOCKS*BLOCK_SIZE];
    } databucket;
    node_header_t *const bkt_p = &databucket.node;
    node_header_t *data_bkt_p, *prev_bkt_p;
    dns_status_t status;

    DCE_SVC_DEBUG((
	cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_btree_build(%s,%s,%p->%d,%p)",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        next_vbn_p,
        *next_vbn_p,
        conversion_ctx));

    /* Build chain of data buckets first, then go back and build index */
    status = sys_read(file_p->data_block, databucket.buffer, *next_vbn_p,
		sizeof(databucket.buffer)/BLOCK_SIZE);
    if (status != DNS_SUCCESS)
	goto leave_db_btree_build;
    
    data_bkt_p = (node_header_t *)dns_malloc_perm(
		bkt_p->node_blocks*BLOCK_SIZE);
    if (!data_bkt_p)
	{
	status = DNS_NONSRESOURCES;
	goto leave_db_btree_build;
	}

    data_bkt_p->node_blocks = bkt_p->node_blocks;
    /* Set up pointer to first bucket */
    hash_p->dir_entry_p = data_bkt_p;
    prev_bkt_p = NULL;
    for (;;)
	{
	/* Copy what we have to in-memory record */
	MOVE_bytes(databucket.buffer, data_bkt_p, sizeof(databucket.buffer));

	*next_vbn_p = *next_vbn_p + DATA_BUCKET_BLOCKS;

	/* See if directory spans multiple buckets */
	if (data_bkt_p->node_blocks > DATA_BUCKET_BLOCKS)
	    {
	    /* Read remaining blocks of directory into in-memory record */
	    status = sys_read(file_p->data_block,
		SKIP_bytes(data_bkt_p, DATA_BUCKET_BLOCKS*BLOCK_SIZE),
		*next_vbn_p,
		data_bkt_p->node_blocks - DATA_BUCKET_BLOCKS);

	    *next_vbn_p = *next_vbn_p + data_bkt_p->node_blocks - DATA_BUCKET_BLOCKS;
	    if (status != DNS_SUCCESS)
	     	{
	    	status = DNS_NONSRESOURCES;
	    	dns_free((char *)data_bkt_p);
		goto leave_db_btree_build;
		}
	    }
	/* Insert bucket in new list */
	if (prev_bkt_p)
	    /* Link new bucket into chain */
	    LINK_AFTER(prev_bkt_p, data_bkt_p)
	else
	    {
	    data_bkt_p->node_next_p = NULL;
	    data_bkt_p->node_prev_p = NULL;
	    }

	/* Save pointer to current bucket and read int next */
	prev_bkt_p = data_bkt_p;

	/* See if bkt we just processed is last for this directory */
	if (!bkt_p->node_next_p)
	    break;

	status = sys_read(file_p->data_block, databucket.buffer, *next_vbn_p,
		sizeof(databucket.buffer)/BLOCK_SIZE);
	if (status != DNS_SUCCESS)
	    goto leave_db_btree_build;

	data_bkt_p = (node_header_t *)dns_malloc_perm(
		bkt_p->node_blocks*BLOCK_SIZE);
	if (!data_bkt_p)
	    {
	    status = DNS_NONSRESOURCES;
	    goto leave_db_btree_build;
	    }

	data_bkt_p->node_blocks = bkt_p->node_blocks;
	}


    /*
     * At this point we have a chain of data buckets.
     * Need to build rest of index one level at a time.
     */
    while ((status == DNS_SUCCESS) &&
	   (hash_p->dir_entry_p->node_next_p != NULL))
	status = build_index(hash_p);

    /*
     * Verify index consistency if successful
     */
    if (status == DNS_SUCCESS)
	if (index_check(hash_p->dir_entry_p, NULL))
	    index_dump(hash_p->dir_entry_p, 0);

leave_db_btree_build:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_btree_build(%s,%s,%p->%d,%p) return(%s)",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        next_vbn_p,
        *next_vbn_p,
        conversion_ctx,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/*
 * db_btree_insert_entry
 *	This routine inserts an entry into the entry b_tree.
 *
 * Input:
 *	stream_p = pointer to database stream
 *	hash_p = pointer to directory hash entry
 *	ekey_p = pointer to data level record header to insert
 */
dns_status_t 
db_btree_insert_entry (db_stream_t         *stream_p,
                       dir_hash_entry_t    *hash_p,
                       entry_key_t         *ekey_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dir_hash_entry(hashBuf)
    DEB_ASCII_BUF_entry_key(ekeyBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_btree_insert_entry(%s,%s,%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_entry_key(ekeyBuf,ekey_p)));

    /* Cache window where directory went to RS_dead */
    if (hash_p->dir_entry_p != NULL)
    {
	btree_ctx_t ctx;
	btree_prm_t child;

	ctx.ctx_hash_p = hash_p;
	ctx.ctx_key_p = ekey_p;
	ctx.ctx_stream_p = stream_p;
	ctx.ctx_allupto_p = NULL;

	child.prm_key_p = NULL;
	child.prm_index_p = hash_p->dir_entry_p;
	child.prm_end_p = NULL;

	status = insert_entry (&ctx, &child, hash_p->dir_entry_p);

	/*
	 * Verify index consistency if a change was made and at top level
	 */
	if ((status == DNS_SUCCESS) && (child.prm_index_p != NULL))
	{
	    (void)new_root(hash_p, child.prm_index_p);

	    if (index_check(hash_p->dir_entry_p, NULL))
		index_dump(hash_p->dir_entry_p, 0);
	}
    }
    else
	status = DNS_UNKNOWNENTRY;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_btree_insert_entry(%s,%s,%s) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_entry_key(ekeyBuf,ekey_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * insert_entry
 *	This routine inserts an entry into the entry b_tree.
 *
 * Input:
 *	ctx_p = pointer to split context
 *	parent_p = input/output parameter block
 *
 * Usage note:  This routine is called recursively, so don't be a pig
 * with the stack or you'll blow some thread away.
 */
static dns_status_t 
insert_entry (btree_ctx_t	*const ctx_p,
	      btree_prm_t	*const parent_p,
	      node_header_t	*bkt_p)
{
    DEB_ASCII_BUF_btree_ctx(ctxBuf)
    DEB_ASCII_BUF_btree_prm(parentBuf)
    DEB_ASCII_BUF_node_header(bktBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_btree:insert_entry(%s,%s,%s)",
	deb_ascii_btree_ctx(ctxBuf,ctx_p),
        deb_ascii_btree_prm(parentBuf,parent_p),
	deb_ascii_node_header(bktBuf,bkt_p)));

    /* Is this the bottom? */
    if (bkt_p->node_type == INDEX_BUCKET)
    {
	btree_prm_t child;
	unsigned int pos;
	dns_status_t istatus;

	(void)find_entry_pos(bkt_p, ctx_p->ctx_key_p, &pos);
	while (bkt_p->node_count <= pos)
	{
	    bkt_p = bkt_p->node_next_p;
	    if (!bkt_p)
	    {
		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_database,
                    svc_c_debug2,
		    "E db_btree:insert_entry(%s,%s,%s) find_entry_pos(,,%u) invalid",
		    deb_ascii_btree_ctx(ctxBuf,ctx_p),
		    deb_ascii_btree_prm(parentBuf,parent_p),
		    deb_ascii_node_header(bktBuf,bkt_p), pos));

	        status = DNS_NAMESERVERBUG;
		goto leave_insert_entry;
	    }

            /* Assert a valid index structure. */
            if (bkt_p == parent_p->prm_end_p)
	    {
                dce_svc_printf(CDS_S_SERVER_DBINDEX_MSG);
                status = DNS_DATACORRUPTION;
		goto leave_insert_entry;
            }

	    (void)find_entry_pos(bkt_p, ctx_p->ctx_key_p, &pos);
	}

	/* Process next level */
	child.prm_key_p = (entry_key_t *)INDEX_node_rec(bkt_p, pos);
	child.prm_key_pos = pos;
	child.prm_index_p = child.prm_key_p->key_down_p;
	child.prm_end_p = next_sublist(bkt_p, pos);

	status = insert_entry(ctx_p, &child, child.prm_index_p);
	istatus = update_index(ctx_p, &child, parent_p, bkt_p);

	if (status == DNS_SUCCESS)
	    status = istatus;

	/* TODO: check if leftmost bucket accessable */
    }
    else
    {
	status = insert_data(ctx_p, parent_p, bkt_p);
    }

leave_insert_entry:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_btree:insert_entry(%s,%s,%s) return(%s)",
	deb_ascii_btree_ctx(ctxBuf,ctx_p),
        deb_ascii_btree_prm(parentBuf,parent_p),
	deb_ascii_node_header(bktBuf,bkt_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * db_btree_replace_entry
 *	This routine locates an entry and replaces it with a new data record
 *
 * Input:
 *	ctx_p = pointer to split context
 *	stream_p = pointer to stream descriptor
 *	bkt_p = pointer to root index bucket
 *	ekey_p = pointer to entry key
 *
 */
dns_status_t 
db_btree_replace_entry (db_stream_t         *stream_p,
                        dir_hash_entry_t    *hash_p,
                        entry_key_t         *ekey_p,
			Timestamp_t         *allupto_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dir_hash_entry(hashBuf)
    DEB_ASCII_BUF_entry_key(ekeyBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_btree_replace_entry(%s,%s,%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_entry_key(ekeyBuf,ekey_p)));

    /* Cache window where directory went to RS_dead */
    if (hash_p->dir_entry_p != NULL)
    {
	btree_ctx_t ctx;
	btree_prm_t child;

	ctx.ctx_hash_p = hash_p;
	ctx.ctx_key_p = ekey_p;
	ctx.ctx_stream_p = stream_p;
	ctx.ctx_allupto_p = allupto_p;

	child.prm_key_p = NULL;
	child.prm_index_p = hash_p->dir_entry_p;
	child.prm_end_p = NULL;

	status = replace_entry(&ctx, &child, hash_p->dir_entry_p);

	/*
	 * Verify index consistency if a change was made and at top level
	 */
	if ((status == DNS_SUCCESS) && (child.prm_index_p != NULL))
	{
	    (void)new_root(hash_p, child.prm_index_p);

	    if (index_check(hash_p->dir_entry_p, NULL))
		index_dump(hash_p->dir_entry_p, 0);
	}
    }
    else
    {
	status = DNS_UNKNOWNENTRY;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_btree_replace_entry(%s,%s,%s) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_entry_key(ekeyBuf,ekey_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * replace_entry
 *	This routine locates an entry and replaces it with a new data record
 *
 * Input:
 *	ctx_p = pointer to split context
 *	parent_p = input/output parameter block
 *
 * Usage note:  This routine is called recursively, so don't be a pig
 * with the stack or you'll blow some thread away.
 */
static dns_status_t 
replace_entry (btree_ctx_t	*const ctx_p,
	       btree_prm_t	*const parent_p,
	       node_header_t	*bkt_p)
{
    DEB_ASCII_BUF_btree_ctx(ctxBuf)
    DEB_ASCII_BUF_btree_prm(parentBuf)
    DEB_ASCII_BUF_node_header(bktBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_btree:replace_entry(%s,%s,%s)",
	deb_ascii_btree_ctx(ctxBuf,ctx_p),
        deb_ascii_btree_prm(parentBuf,parent_p),
	deb_ascii_node_header(bktBuf,bkt_p)));

    /*
     * Is this the bottom? (Recursing now to find the correct place in the
     * database tree to put the data.)
     */
    if (bkt_p->node_type == INDEX_BUCKET)
    {
	btree_prm_t child;
	unsigned int pos;
	dns_status_t istatus;

	status = find_entry_pos(bkt_p, ctx_p->ctx_key_p, &pos);
	while (bkt_p->node_count <= pos)
	{
	    bkt_p = bkt_p->node_next_p;
	    if (!bkt_p)
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
                    cds_svc_database,
                    svc_c_debug2,
		    "E db_btree:replace_entry(%s,%s,%s) find_entry_pos(,,%d) invalid",
		    deb_ascii_btree_ctx(ctxBuf,ctx_p),
		    deb_ascii_btree_prm(parentBuf,parent_p),
		    deb_ascii_node_header(bktBuf,bkt_p), pos));

                status = DNS_NAMESERVERBUG;
		goto leave_replace_entry;
	    }

            /* Assert a valid index structure. */
            if (bkt_p == parent_p->prm_end_p)
	    {
                dce_svc_printf(CDS_S_SERVER_DBINDEX_MSG);
                status = DNS_DATACORRUPTION;
		goto leave_replace_entry;
            }

	    status = find_entry_pos(bkt_p, ctx_p->ctx_key_p, &pos);
	}

	/* Process next level */
	child.prm_key_p = (entry_key_t *)INDEX_node_rec(bkt_p, pos);
	child.prm_key_pos = pos;
	child.prm_index_p = child.prm_key_p->key_down_p;
	child.prm_end_p = next_sublist(bkt_p, pos);

	status = replace_entry(ctx_p, &child, child.prm_index_p);
	istatus = update_index(ctx_p, &child, parent_p, bkt_p);

	if (status == DNS_SUCCESS)
	    status = istatus;

	/* TODO: check if rightmost bucket accessable */
	}
    else
	{
	status = replace_data(ctx_p, parent_p, bkt_p);
	}

leave_replace_entry:

    /* NOTE: bkt_p may no longer be valid */
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_btree:replace_entry(%s,%s,%s) return(%s)",
	deb_ascii_btree_ctx(ctxBuf,ctx_p),
        deb_ascii_btree_prm(parentBuf,parent_p),
	deb_ascii_node_header(bktBuf,bkt_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * db_btree_remove_entry
 *      This routine locates an entry and removes it from the bucket
 *
 * Inputs:
 *	stream_p = (unused, here for interface consistency)
 *	hash_p = hash entry for directory to remove entry from
 *	EKEy_p = key of entry to remove
 *
 * Returns:
 *	DNS_SUCCESS = if entry removed
 *	DNS_UNKNOWNENTRY = if entry not found
 */

dns_status_t 
db_btree_remove_entry (db_stream_t         *stream_p,
                       dir_hash_entry_t    *hash_p,
                       struct entry_key    *ekey_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dir_hash_entry(hashBuf)
    DEB_ASCII_BUF_entry_key(ekeyBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_btree_remove_entry(%s,%s,%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_entry_key(ekeyBuf,ekey_p)));

    /* Cache window where directory went to RS_dead */
    if (hash_p->dir_entry_p != NULL)
    {
	btree_ctx_t ctx;
	btree_prm_t child;

	ctx.ctx_hash_p = hash_p;
	ctx.ctx_key_p = ekey_p;
	ctx.ctx_stream_p = stream_p;

	child.prm_key_p = NULL;
	child.prm_index_p = hash_p->dir_entry_p;
	child.prm_end_p = NULL;

        status = remove_entry(&ctx, &child, hash_p->dir_entry_p);

	/*
	 * Verify index consistency if a change was made and at top level
	 */
	if ((status == DNS_SUCCESS) && (child.prm_index_p != NULL))
	{
	    (void)new_root(hash_p, child.prm_index_p);

	    if (index_check(hash_p->dir_entry_p, NULL))
		index_dump(hash_p->dir_entry_p, 0);
	}
    }
    else
    {
	status = DNS_UNKNOWNENTRY;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_btree_remove_entry(%s,%s,%s) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_entry_key(ekeyBuf,ekey_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * remove_entry
 *      This routine locates an entry and removes it from the bucket
 *
 * Inputs:
 *	ctx_p = context contains:
 *	start_p = bucket to check for entry
 *
 * Returns:
 *	DNS_SUCCESS = entry deleted
 *	DNS_EMPTY = start_p deleted
 *	DNS_DATACORRUPTION = entry not betweek start_p and end_p
 *	DNS_NAMESERVERBUG = sentinel missing
 *
 * Usage note:  This routine is called recursively, so don't be a pig
 * with the stack or you'll blow some thread away.
 */

static dns_status_t 
remove_entry (btree_ctx_t	*const ctx_p,
	      btree_prm_t	*const parent_p,
	      node_header_t	*bkt_p)
{
    DEB_ASCII_BUF_btree_ctx(ctxBuf)
    DEB_ASCII_BUF_btree_prm(parentBuf)
    DEB_ASCII_BUF_node_header(bktBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    DEB_ASCII_BUF_node_header(down_bktBuf)
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_btree:remove_entry(%s,%s,%s)",
	deb_ascii_btree_ctx(ctxBuf,ctx_p),
        deb_ascii_btree_prm(parentBuf,parent_p),
	deb_ascii_node_header(bktBuf,bkt_p)));

    /* Is this the bottom? */
    if (bkt_p->node_type == INDEX_BUCKET)
    {
	btree_prm_t child;
	unsigned int pos;
	dns_status_t istatus;

	(void) find_entry_pos(bkt_p, ctx_p->ctx_key_p, &pos);
	while (pos >= bkt_p->node_count) {

            bkt_p = bkt_p->node_next_p;
            if (!bkt_p) 
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
                    cds_svc_database,
                    svc_c_debug2,
		    "E db_btree:remove_entry(%s,%s,%s) find_entry_pos(,,%d) invalid",
		    deb_ascii_btree_ctx(ctxBuf,ctx_p),
		    deb_ascii_btree_prm(parentBuf,parent_p),
		    deb_ascii_node_header(bktBuf,bkt_p), pos));

                status = DNS_NAMESERVERBUG;
		goto leave_remove_entry;
            }

            /* Assert a valid index structure. */
            if (bkt_p == parent_p->prm_end_p)
	    {
                dce_svc_printf(CDS_S_SERVER_DBINDEX_MSG);
                status = DNS_DATACORRUPTION;
		goto leave_remove_entry;
            }

	    (void) find_entry_pos(bkt_p, ctx_p->ctx_key_p, &pos);
        }

	child.prm_key_p = (entry_key_t *)INDEX_node_rec(bkt_p, pos);
	child.prm_key_pos = pos;
	child.prm_index_p = child.prm_key_p->key_down_p;
	child.prm_end_p = next_sublist(bkt_p, pos);

        status = remove_entry(ctx_p, &child, child.prm_index_p);
	istatus = update_index(ctx_p, &child, parent_p, bkt_p);

	if (status == DNS_SUCCESS)
	    status = istatus;

	/* TODO: check if rightmost bucket accessable */
    }
    else
    {
	status = remove_data(ctx_p, parent_p, bkt_p);
    }

leave_remove_entry:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_btree:remove_entry(%s,%s,%s) return(%s)",
	deb_ascii_btree_ctx(ctxBuf,ctx_p),
        deb_ascii_btree_prm(parentBuf,parent_p),
	deb_ascii_node_header(bktBuf,bkt_p),
        deb_ascii_dns_status(statusBuf,status)));

    return(status);
}


/*
 * db_btree_locate_entry_bkt
 *	Find the bucket either containing the entry record or
 *	which should contain the entry bucket
 *	Root index bucket already read.
 *
 * Input:
 *	stream_p = pointer to data base stream
 *	root_p = pointer to root bucket
 *	hash_p = pointer to hash table entry
 *	entry_pp = address to return pointer to data bucket
 *	pos_p = address to return entry position in data bucket
 */
dns_status_t 
db_btree_locate_entry_bkt (db_stream_t       *stream_p,
                           node_header_t     *root_p,
                           dir_hash_entry_t  *hash_p,
                           node_header_t     **entry_pp,
                           unsigned int      *pos_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_node_header(rootBuf)
    DEB_ASCII_BUF_dir_hash_entry(hashBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    entry_key_t ekey;
    node_header_t *bkt_p;
    dns_status_t status;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_btree_locate_entry_bkt(%s,%s,%s,%p,%p)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_node_header(rootBuf,root_p),
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        entry_pp,
        pos_p));

    *entry_pp = NULL;

    /*
     * Cache window where directory went to RS_dead 
     */
    if (!root_p)
	{
	status = DNS_UNKNOWNENTRY;
	goto leave_db_btree_locate_entry_bkt;
	}

    /* Build key */
    INS8(ekey.key_type, stream_p->db_entry.data_type);
    if (EMPTY_FullName(stream_p->db_key))
	{
	INS8(ekey.key_sort, BINARY_SORT);
	COPY_SimpleName(NullSimpleName, ekey.key_name);
	}
    else
	{
	SimpleName_t *name_p = names_pointlast((FullName_t *)stream_p->db_key);

	switch(EXT8(name_p->sn_flag)) {
/*	case SN_normal:		*/
/*	case SN_quoted:		*/
/*	case SN_cds:    	*/
	default:
	    INS8(ekey.key_sort, ASCII_SORT);
	    break;
	case SN_null:
	case SN_binary:
	    INS8(ekey.key_sort, BINARY_SORT);
	    break;
	case SN_typed:
	    INS8(ekey.key_sort, TYPED_SORT);
	    break;
	}

	COPY_SimpleName(name_p, ekey.key_name);
	}

    bkt_p = root_p;
    while (bkt_p->node_type == INDEX_BUCKET)
    {
	while (TRUE)
	{
	    (void)find_entry_pos(bkt_p, &ekey, pos_p);

	    if (*pos_p < bkt_p->node_count)
		break;

	    bkt_p = bkt_p->node_next_p;
	    if (!bkt_p)
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
                    cds_svc_database,
                    svc_c_debug2,
		    "E db_btree_locate_entry_bkt(%s,%s,%s,%p,%p) find_entry_pos() returned invalid position: %u",
		    deb_ascii_db_stream(streamBuf,stream_p),
		    deb_ascii_node_header(rootBuf,root_p),
		    deb_ascii_dir_hash_entry(hashBuf,hash_p),
		    entry_pp, pos_p, *pos_p));

		status = DNS_NAMESERVERBUG;
		goto leave_db_btree_locate_entry_bkt;
	    }
	}

	/* Go down another level in the tree*/
	bkt_p = THIS_SUBLIST(bkt_p, *pos_p);
    }

    while (TRUE)
    {
	status = find_entry_pos(bkt_p, &ekey, pos_p);

	if ((*pos_p != bkt_p->node_count) || (status == DNS_SUCCESS))
	    break;

	bkt_p = bkt_p->node_next_p;
	if (!bkt_p)
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
                cds_svc_database,
                svc_c_debug2,
		"E db_btree_locate_entry_bkt(%s,%s,%s,%p,%p) find_entry_pos() returned invalid position: %u",
		deb_ascii_db_stream(streamBuf,stream_p),
		deb_ascii_node_header(rootBuf,root_p),
		deb_ascii_dir_hash_entry(hashBuf,hash_p),
		entry_pp, pos_p, *pos_p));

            status = DNS_NAMESERVERBUG;
	    goto leave_db_btree_locate_entry_bkt;
	}
    }

    *entry_pp = bkt_p;

leave_db_btree_locate_entry_bkt:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_btree_locate_entry_bkt(%s,%s,%s,%p->%s,%p->%u) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_node_header(rootBuf,root_p),
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        entry_pp,
        *entry_pp,
        pos_p,
        *pos_p,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/*
 * db_btree_get_next_level
 *	Return a pointer to the node header for the first bucket
 *	pointed to by this node header.
 *
 * Input:
 *	data_p = address of current node header
 *
 */

node_header_t *
db_btree_get_next_level (node_header_t *data_p)
{
    DEB_ASCII_BUF_node_header(dataBuf)
    DEB_ASCII_BUF_node_header(returnBuf)
    register node_header_t *return_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >db_btree_get_next_level(%s)",
        deb_ascii_node_header(dataBuf,data_p)));

    /* Cache window where directory went to RS_dead */
    if (!data_p) {
	return_p = NULL;
    } else {
        /* If already at bottom of btree, return null */
        if (data_p->node_type == DATA_BUCKET) {
  	    return_p = NULL;
        } else {
	    return_p = THIS_SUBLIST(data_p, 0);
        }
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <db_btree_get_next_level(%s) return(%s)",
        deb_ascii_node_header(dataBuf,data_p),
        deb_ascii_node_header(returnBuf,return_p)));
    return(return_p);
}



/* -------------------------------------------------------------------------- */

/*
 * db_btree_get_first_data_bucket
 *	Return a pointer to the first data bucket
 *	pointed to by this node header.
 *
 * Input:
 *	data_p = address of current node header
 *
 */
node_header_t *
db_btree_get_first_data_bucket (node_header_t *data_p)
{
    DEB_ASCII_BUF_node_header(dataBuf)
    DEB_ASCII_BUF_node_header(returnBuf)
    register node_header_t *return_p = data_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_btree_get_first_data_bucket(%s)",
        deb_ascii_node_header(dataBuf,data_p)));

    /* Cache window where directory went to RS_dead */
    if (return_p)
	{
	/* Loop till at bottom of tree */
	while (return_p->node_type != DATA_BUCKET)
	    return_p = THIS_SUBLIST(return_p, 0);
	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_btree_get_first_data_bucket(%s) return(%s)",
        deb_ascii_node_header(dataBuf,data_p),
        deb_ascii_node_header(returnBuf,return_p)));
    return(return_p);
}



/* -------------------------------------------------------------------------- */

/*
 * free_btree_index
 *
 * This routine, when passed the root bucket (node) of a BTree, will free
 * up any index level nodes that exist in the BTree, leaving an unconnected
 * list of data nodes.
 */

static void 
free_btree_index (node_header_t *data_p)
{
    DEB_ASCII_BUF_node_header(dataBuf)
    register node_header_t *this_p = data_p;  /* Iterator */
    register int levels_processed = 0;        /* How many levels down we went */
    register int nodes_freed = 0;             /* Count of buckets freed */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >free_btree_index(%s)",
        deb_ascii_node_header(dataBuf,data_p)));

    while (this_p) {
	node_header_t *down_p;

        /*
         *  If we've arrived at the data level, then we stop here.
         */

        if (this_p->node_type == DATA_BUCKET) {
	    break;
        }

	/* 
         * Get first node of next level.
         */

        down_p = db_btree_get_next_level(this_p);

        /*
         * Free all nodes at this level.
         */

	while (this_p->node_next_p) {
	    node_header_t *const next_p = this_p->node_next_p;

	    dns_free((char *)this_p);
            nodes_freed++;
	    this_p = next_p;
	}

	/* Complete freeing resources for this level */
	dns_free((char *)this_p);
        nodes_freed++;
        levels_processed++;

        /* Move to the next level down */
  	this_p = down_p;

    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <free_btree_index(%p) - %d nodes, %d levels",
        data_p,
        nodes_freed,
        levels_processed));

}

/* -------------------------------------------------------------------------- */

/*
 * free_btree_data
 *
 * This routine, given the leftmost data node in the BTree, will free up
 * all siblings to the right of it.
 */

static void 
free_btree_data (node_header_t *data_p)
{
    DEB_ASCII_BUF_node_header(dataBuf)
    register node_header_t *this_p = data_p;  /* Iterator */
    register int nodes_freed = 0;             /* Count of buckets freed */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >free_btree_data(%s)",
        deb_ascii_node_header(dataBuf,data_p)));

    while (this_p->node_next_p) {
	node_header_t *const next_p = this_p->node_next_p;

	dns_free((char *)this_p);
        nodes_freed++;
	this_p = next_p;
    }

    dns_free((char *)this_p);
    nodes_freed++;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <free_btree_data(%p) - %d nodes",
        data_p,
        nodes_freed));
}



/* -------------------------------------------------------------------------- */

/*
 * db_btree_free
 *	This routine free's memory used by a btree
 *
 */
void 
db_btree_free (node_header_t *data_p)
{
    DEB_ASCII_BUF_node_header(dataBuf)
    node_header_t *data_level_p;     /* Holds first bucket of data level */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_btree_free(%s)",
        deb_ascii_node_header(dataBuf,data_p)));

    /*
     * First we obtain the leftmost data bucket, then free the index followed
     * by the data level.
     */

    data_level_p = db_btree_get_first_data_bucket (data_p);
    free_btree_index(data_p);
    free_btree_data(data_level_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_btree_free(%p)",
        data_p));
}



/* -------------------------------------------------------------------------- */

/*
 * db_btree_rebuild_index
 *
 * This routine will rebuild a BTree index in memory.   The primary use
 * for this routine is to correct minor problems with the index due to
 * bucket splitting, etc.  It can also be used to improve performance
 * of database lookups.
 *
 * This routine assumes that the directory is locked and that nobody is
 * accessing the BTree.   Data buckets are left intact (and not relocated
 * in memory).  Index buckets are deleted and then rebuilt, so any improper
 * pointers to them will be invalid.
 */
dns_status_t 
db_btree_rebuild_index (dir_hash_entry_t *hash_p)
{
    DEB_ASCII_BUF_dir_hash_entry(hashBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t status = DNS_SUCCESS;
    node_header_t *data_level_p;          /* Leftmost data-level node */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_btree_rebuild_index(%s)",
        deb_ascii_dir_hash_entry(hashBuf,hash_p)));

    /*
     * Squirrel away the data level for later, then free the index.
     */

    data_level_p = db_btree_get_first_data_bucket(hash_p->dir_entry_p);
    free_btree_index(hash_p->dir_entry_p);

    /*
     * The free_btree_index routine leaves the data level disconnected, so
     * the hash entry has an invalid root bucket at this point.  We must
     * fix this up before rebuilding the index.
     */

    hash_p->dir_entry_p = data_level_p;

    while ((status == DNS_SUCCESS) &&
           (hash_p->dir_entry_p->node_next_p != NULL)) {
        status = build_index(hash_p);
    }

    /*
     * Verify index consistency if successful
     */
    if (status == DNS_SUCCESS)
	if (index_check(hash_p->dir_entry_p, NULL))
	    index_dump(hash_p->dir_entry_p, 0);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_btree_rebuild_index(%s) return(%s)",
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_dns_status(statusBuf,status)));

    return (status);
}
