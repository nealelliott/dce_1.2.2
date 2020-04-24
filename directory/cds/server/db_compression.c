/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_compression.c,v $
 * Revision 1.1.11.2  1996/02/18  19:35:53  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:19  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:29:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:54  root]
 * 
 * Revision 1.1.9.4  1994/08/03  19:03:55  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:21:20  mccann]
 * 
 * Revision 1.1.9.3  1994/06/09  18:43:07  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:34  devsrc]
 * 
 * Revision 1.1.9.2  1994/04/14  14:51:10  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:19:30  peckham]
 * 
 * Revision 1.1.9.1  1994/03/12  22:10:44  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:40:32  peckham]
 * 
 * Revision 1.1.7.1  1993/10/15  17:29:31  peckham
 * 	CR#7265: Document "Compression warning:" messages
 * 	These shouldn't be normally visible. Change LOG_TRACE()
 * 	to DEB_EVENT(). Also added deb_ascii_compression_ctx().
 * 	[1993/10/15  17:29:07  peckham]
 * 
 * Revision 1.1.5.3  1992/12/30  14:20:35  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:20:46  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  19:15:23  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:16:45  weisman]
 * 
 * Revision 1.1.2.2  1992/06/30  19:55:35  peckham
 * 	OT#4477: default compression and alignment.
 * 	[1992/06/30  19:54:39  peckham]
 * 
 * Revision 1.1  1992/01/19  15:25:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: db_compression.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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
 * Module description:
 *
 * This module contains routines for writing compressed forms of the
 * in-memory database and then reading them back in.
 *
 * Two levels of compression are currently supported:
 *
 *    Level 0 - No compression.
 *        The buckets of the B-Tree data level are written out in whatever
 *        state they exist in memory.  The buckets are restored to memory
 *        from the checkpoint file with no modification.
 *
 *    Level 1 - Bucket compression.
 *        The buckets of the B-Tree data level are written out to minimize
 *        the amount of empty space in each bucket - no modifications are
 *        made to the record entries themselves.  The in-memory database
 *        is *NOT* modified.  To gain the effects of this compression, the
 *        database must be purged from memory and read back in from checkpoint
 *        file. The buckets are restored to memory from the checkpoint file
 *        with no modification.
 *
 * Author: Dave Griffin
 * 
 */

#include <server.h>
#include <dce/assert.h>
#include <back.h>
#include <dbdef.h>

/*
 * Compression context buffer (private view)
 */

typedef struct private_compression_ctx {
    int            level;                   /* Compression level */
    file_info_t   *file_p;                  /* Database information */
    dns_status_t   (*btree_start_rtn_p)(struct private_compression_ctx *);  /* Start of BTree handler */
    dns_status_t   (*btree_write_rtn_p)(struct private_compression_ctx *,
					node_header_t                  *); /* BTree bucket handler */
    dns_status_t   (*btree_end_rtn_p)(struct private_compression_ctx *);    /* End of BTree handler */
    unsigned int   next_vbn;                /* Next checkpoint file VBN */
    node_header_t *work_bkt_p;              /* Scratch area */
    node_header_t *last_node_next_p;        /* Last node_next_p written */
    /* Instrumentation cells */
    int          normal_fire_count;         /* L1 NORMAL state fire count */
    int          way_too_big_fire_count;    /* L1 WAY_TOO_BIG fire count */
    int          clean_slate_fire_count;    /* L1 CLEAN_SLATE fire count */
    /* Statistics -- will be exported to file_info_t */
    int          data_bytes_in;             /* Used bytes incoming */
    int          data_bytes_written;        /* Used bytes written  */
    int          free_bytes_in;             /* Free bytes incoming */
    int          free_bytes_written;        /* Free bytes written  */
    int          buckets_in;                /* Buckets incoming    */
    int          buckets_written;           /* Buckets written     */
    int          node_count_in;             /* Entry nodes (records) incoming */
    int          node_count_written;        /* Entry nodes (records) written */
} private_compression_ctx_t;

#define deb_ascii_compression_ctx(b,p) deb_ascii_rec(b,p,"NULL compression_ctx")
#define DEB_ASCII_BUF_compression_ctx(b) DEB_ASCII_BUF_rec(b)
#define LOG_ASCII_BUF_compression_ctx(b) LOG_ASCII_BUF_rec(b)

/*
 * Private prototypes
 */

static dns_status_t
emit_bucket (
    private_compression_ctx_t *,
    node_header_t             *);

static dns_status_t
flush_work_bucket (
    private_compression_ctx_t *);

static dns_status_t
level_0_btree_start (
    private_compression_ctx_t *);

static dns_status_t
level_0_btree_write (
    private_compression_ctx_t *,
    node_header_t             *);

static dns_status_t
level_0_btree_end (
    private_compression_ctx_t *);

static int
assess_transfer_count (
    node_header_t *,
    node_header_t *);

static int
reset_offsets (
    node_header_t *);

static void
copy_starting (
    node_header_t *,
    node_header_t *,
    int            );

static void
copy_remaining (
    node_header_t *,
    node_header_t *,
    int            );

static dns_status_t
level_1_btree_start (
    private_compression_ctx_t *);

static dns_status_t
level_1_btree_write (
    private_compression_ctx_t *,
    node_header_t             *);

static dns_status_t
level_1_btree_end (
    private_compression_ctx_t *);


/*
 *                      Private Routines
 */

/* -------------------------------------------------------------------------- */

/*
 * emit_bucket
 *
 * This routine performs the actual writing of a bucket to disk.  It also
 * updates the written statistics in the context block.  We record the
 * node_next_p in the context block for data consistancy checking when
 * a btree has been completely written.
 *
 * Returns: Result of write.
 */

static dns_status_t 
emit_bucket (private_compression_ctx_t  *ctx_p,
             node_header_t              *data_p)
{

    dns_status_t status;

    status = sys_write(ctx_p->file_p->file_ckpt_block,
                       (byte_t *)data_p,
                       ctx_p->next_vbn,
                       data_p->node_blocks);

    if (status != DNS_SUCCESS) {
        return(status);
    }

    ctx_p->last_node_next_p = data_p->node_next_p;
    ctx_p->next_vbn += data_p->node_blocks;
    ctx_p->data_bytes_written += ((data_p->node_blocks * BLOCK_SIZE) -
                                  data_p->node_space);
    ctx_p->free_bytes_written += data_p->node_space;
    ctx_p->node_count_written += data_p->node_count;
    ctx_p->buckets_written++;

    return(status);
}

/* -------------------------------------------------------------------------- */

/*
 * flush_work_bucket
 *
 * This routine determines if there is any data in the work bucket worth
 * writing, and causes it to be written to disk.   After the routine has
 * completed the bucket is marked as empty (regardless of the result of
 * the write operation).
 *
 * Returns: Result of write (from emit_bucket())
 *          DNS_SUCCESS if no action is taken.
 */

static dns_status_t 
flush_work_bucket (private_compression_ctx_t *ctx_p)
{
    dns_status_t status;

    /*
     * Check node_count of the work bucket.  A positive value indicates
     * that there is data in the bucket.
     */

    if (ctx_p->work_bkt_p->node_count > 0) {
        status = emit_bucket(ctx_p, ctx_p->work_bkt_p);
        ctx_p->work_bkt_p->node_count = 0;
    } else {
        status = DNS_SUCCESS;
    }

    return(status);
}



/*
 *               ----------------------------
 *               Level 0 Compression Routines
 *               ----------------------------
 */

/* -------------------------------------------------------------------------- */

/*
 * level_0_btree_start
 */

static dns_status_t 
level_0_btree_start (private_compression_ctx_t *ctx_p)
{
    return(DNS_SUCCESS);
}

/* -------------------------------------------------------------------------- */

/*
 * level_0_btree_write
 *
 * This routine records the incoming statistics, and then immediately writes
 * the bucket out to disk with no modification.
 *
 * Returns: Result of the write attempt.
 */

static dns_status_t 
level_0_btree_write (private_compression_ctx_t  *ctx_p,
                     node_header_t              *data_p)
{
    ctx_p->data_bytes_in += ((data_p->node_blocks * BLOCK_SIZE) -
                                  data_p->node_space);
    ctx_p->free_bytes_in += data_p->node_space;
    ctx_p->node_count_in += data_p->node_count;
    ctx_p->buckets_in++;

    return(emit_bucket(ctx_p, data_p));
}

/* -------------------------------------------------------------------------- */

/*
 * level_0_btree_end
 */

static dns_status_t 
level_0_btree_end (private_compression_ctx_t *ctx_p)
{
    if (ctx_p->last_node_next_p != NULL) 
    {
       dce_svc_printf(CDS_S_DBCMP_TREENOTNULL_A_MSG, __LINE__, __FILE__);
    }

    return(DNS_SUCCESS);
}



/*
 *               ----------------------------
 *               Level 1 Compression Routines
 *               ----------------------------
 */


/*
 * assess_transfer_count
 *
 * This routine examines the incoming bucket and the state of the work
 * bucket and determines how many records from the incoming bucket can
 * safely be copied to the work bucket without causing an overflow.
 *
 * Returns: Count as defined above.
 */

static int 
assess_transfer_count (node_header_t  *in_p,
                       node_header_t  *work_p)
{
    int count = 0;                            /* How many records fit */
    int in_index;                             /* Iterator */
    int available_space = work_p->node_space; /* Get a copy of the space */

    /*
     * Iterate through the records and calculate space usage.
     */

    for (in_index = 0; in_index < in_p->node_count; in_index++) {
        rec_descriptor_t *rd_p = INDEX_node_rec_dsc(in_p, in_index);
        int space_required;

        /*
         * Each record requires a descriptor, include it in the space
         * calculations.
         */

        space_required = ALIGN_LEN(rd_p->rec_length) + sizeof(rec_descriptor_t);

        /*
         * If we have room for the current record, then include it in the
         * count, and subtract it's space requirements from our projected
         * available space.
         */

        if (available_space > space_required) {
            count++;
            available_space -= space_required;
        } else {
            break;
        }
    }

    return(count);

}

/* -------------------------------------------------------------------------- */

/*
 * reset_offsets
 *
 * This routine resets the record descriptor offsets for an entire
 * bucket.
 *
 * Returns: The amount of free space left in the bucket.
 */

static int 
reset_offsets (node_header_t *work_p)
{
    rec_descriptor_t *rd1_p;    /* Points to "previous" descriptor in bucket */
    rec_descriptor_t *rd2_p;    /* "Current" descriptor being adjusted */
    int x;                      /* Iterator */

    /*
     * The first descriptor's is set based on the number of descriptors in
     * this bucket.
     */

    rd1_p = INDEX_node_rec_dsc(work_p, 0);
    rd1_p->rec_offset = DIFF_bytes(INDEX_node_rec_dsc(work_p, 
                                                      work_p->node_count),
                                   work_p);
    /*
     * In case there is only one descriptor, record it in the current
     * descriptor pointer because we need rd2_p set to the last descriptor
     * when the iteration is done.
     */

    rd2_p = rd1_p;

    /*
     * Iterate through the descriptors.  Adjust offsets based on length in
     * the previous descriptor.
     */

    for (x = 1; x < work_p->node_count; x++) {
        rd1_p = INDEX_node_rec_dsc(work_p, x-1);
        rd2_p = INDEX_node_rec_dsc(work_p, x);
        rd2_p->rec_offset = ALIGN_LEN(rd1_p->rec_offset + rd1_p->rec_length);
    }

    /*
     * Return free space in bucket (bucket size minus the used data space).
     */

    if (ALIGN_LEN(rd2_p->rec_offset + rd2_p->rec_length) >
        (work_p->node_blocks * BLOCK_SIZE)) 
    {
         dce_svc_printf(CDS_S_DBCMP_TOOBIG_A_MSG, 
			__LINE__, __FILE__);
    }

    return ((work_p->node_blocks * BLOCK_SIZE) -
            ALIGN_LEN(rd2_p->rec_offset + rd2_p->rec_length));
}

/* -------------------------------------------------------------------------- */

/*
 * copy_starting
 *
 * This routine copies the initial set of records of the incoming bucket
 * to the work bucket (records 0 through count-1).  It does this in a
 * block fashion (all records moved at one time) for maximum performance.
 */

static void 
copy_starting (node_header_t  *in_p,
               node_header_t  *work_p,
               int            count)
{
    int copy_length;       /* Calculated length of a byte copy */
    byte_t *to_p;         /* Calculated target of a byte copy */

    /*
     * We have to shift the existing data over to make room for the
     * new descriptors.   Determine how big the existing record set
     * is in bytes.
     */

    copy_length = DIFF_bytes(INDEX_node_rec(work_p, (work_p->node_count-1)),
                             INDEX_node_rec(work_p, 0)) +
                  work_p->node_rec_dsc[work_p->node_count - 1].rec_length;

    /*
     * Overlapping copy from the first record's current location to
     * just beyond the expected placement of the last descriptor.
     * We'll fix up the descriptors later.   Honest.
     */

    to_p = (byte_t *)INDEX_node_rec_dsc(work_p, work_p->node_count + count);
    MOVE_bytes(INDEX_node_rec(work_p, 0), to_p, copy_length);
    to_p = SKIP_bytes(to_p, ALIGN_LEN(copy_length));

    /*
     * Now we copy the incoming records in, same method (only this isn't
     * an overlapping copy).   Record data only.  The descriptors are next.
     */

    copy_length = DIFF_bytes(INDEX_node_rec(in_p, (count-1)),
                             INDEX_node_rec(in_p, 0)) +
                  in_p->node_rec_dsc[count - 1].rec_length;
    COPY_bytes(INDEX_node_rec(in_p, 0), to_p, copy_length);

    /*
     * Copy the incoming record descriptors to the the space we made available.
     */

    COPY_bytes(INDEX_node_rec_dsc(in_p, 0),
               INDEX_node_rec_dsc(work_p, work_p->node_count),
               (count * sizeof(rec_descriptor_t)));

    /*
     * Reset the descriptors in the work bucket, adjust node_count and
     * free space.
     */

    work_p->node_count += count;
    work_p->node_space = reset_offsets(work_p);

}

/* -------------------------------------------------------------------------- */

/*
 * copy_remaining
 *
 * This routine copies the remaining set of records of the incoming bucket
 * as an initialized work bucket.  The records are copied in a single operation
 * to maximize performance.
 */

static void 
copy_remaining (node_header_t  *in_p,
                node_header_t  *work_p,
                int            starting_at)
{
    int count = in_p->node_count - starting_at; /* How many records to copy */
    int copy_length;               /* Calculated length of a byte copy */
    byte_t *to_p;                 /* Calculated target of a byte copy */

    /*
     * Copy over the node header
     */

    COPY_bytes(in_p, work_p, sizeof(node_header_t));

    /*
     * Calculate space (make room) for record descriptors
     */

    to_p = (byte_t*)INDEX_node_rec_dsc(work_p, count);

    /*
     * Copy the incoming record data to the work bucket.
     */

    copy_length = DIFF_bytes(INDEX_node_rec(in_p, (in_p->node_count-1)),
                             INDEX_node_rec(in_p, starting_at)) +
                  in_p->node_rec_dsc[in_p->node_count - 1].rec_length;
    COPY_bytes(INDEX_node_rec(in_p, starting_at), to_p, copy_length);

    /*
     * Copy the incoming record descriptors to the the space we made available.
     */

    COPY_bytes(INDEX_node_rec_dsc(in_p, starting_at),
               INDEX_node_rec_dsc(work_p, 0),
               (count * sizeof(rec_descriptor_t)));

    /*
     * Reset the descriptors in the work bucket, adjust node_count and
     * free space.
     */

    work_p->node_count = count;      /* reset_offsets needs an accurate count */
    work_p->node_space = reset_offsets(work_p);
}

/* -------------------------------------------------------------------------- */

/*
 * level_1_btree_start
 *
 * Initializes the work bucket.
 */

static dns_status_t 
level_1_btree_start (private_compression_ctx_t *ctx_p)
{
    ctx_p->work_bkt_p->node_count = 0;
    return (DNS_SUCCESS);
}

/* -------------------------------------------------------------------------- */

/*
 * level_1_btree_write
 *
 * This routine performs bucket level compression of the incoming data.
 * Free space from adjacent buckets is used so that the resulting btree
 * bucket stream has the minimal amount of free space.
 *
 * A work bucket (hung off the compression context buffer) is used to hold
 * intermediate compression results between incoming buckets.
 *
 * Returns: The result of any write operation that might occur, or DNS_SUCCESS
 *          if no write operation was induced.
 */

static dns_status_t 
level_1_btree_write (private_compression_ctx_t  *ctx_p,
                     node_header_t              *data_p)
{
    dns_status_t status = DNS_SUCCESS;
    int assessed_count;      /* How many records can moved to the work bucket */

    /*
     * Record incoming statistics
     */

    ctx_p->data_bytes_in += ((data_p->node_blocks * BLOCK_SIZE) -
                                  data_p->node_space);
    ctx_p->free_bytes_in += data_p->node_space;
    ctx_p->node_count_in += data_p->node_count;
    ctx_p->buckets_in++;

    /*
     * There are three states the compression algorithm can enter:
     *
     *    WAY_TOO_BIG - This means that the incoming bucket is an overflow
     *                  bucket and compression is not even attempted.
     *    CLEAN_SLATE - This means that the work bucket is empty, and the
     *                  incoming bucket is transferred intact to the work
     *                  bucket to see if subsequent buckets can be appended
     *                  to it.
     *    NORMAL      - The incoming bucket is analyzed to see how much of
     *                  it can be appended to the work bucket, and the
     *                  appropriate splits and transfers are performed.
     */

    if (data_p->node_blocks > DATA_BUCKET_BLOCKS) {

        /* WAY_TOO_BIG
         *
         * We flush the work bucket to disk and then write out the
         * incoming bucket directly.  After this state completes, the
         * implied state is CLEAN_SLATE.
         */

        ctx_p->way_too_big_fire_count++;

        status = flush_work_bucket(ctx_p);
        if (status == DNS_SUCCESS) {
            status = emit_bucket(ctx_p, data_p);
        }

    } else if (ctx_p->work_bkt_p->node_count == 0) {

        /* CLEAN_SLATE
         *
         * Copy the incoming bucket verbatim to the work bucket.  We
         * know it will fit because we checked for oversized buckets
         * above.
         */

        ctx_p->clean_slate_fire_count++;

        COPY_bytes(data_p, ctx_p->work_bkt_p, (DATA_BUCKET_BLOCKS*BLOCK_SIZE));

    } else {

        /* NORMAL
         *
         * o  Assess how much of the incoming bucket can be transferred
         *    to the work bucket.
         *
         * o  If assessed number is 0, special case for performance:
         *      - Flush work bucket
         *      - Copy incoming bucket verbatim
         *
         * (assuming assessment > 0)
         *
         * o  Copy assessed number of records to work bucket (adjusting
         *    offsets).
         *
         * o  Write out work bucket if there are records left in the
         *    incoming bucket that were not copied (i.e., assessed count <
         *    node_count).  Then initialize work bucket with remaining
         *    records.
         *
         * o  If we appended an entire bucket to the work bucket then we
         *    have to carry the current bucket's next_p into the work
         *    bucket.  next_p is used to signal the end of a directory
         *    bucket stream, and we don't want to lose this information
         *    or it'll screw up the checkpoint load routine.
         */

        ctx_p->normal_fire_count++;

        assessed_count = assess_transfer_count(data_p, ctx_p->work_bkt_p);

        if (assessed_count == 0) {
            status = flush_work_bucket(ctx_p);
            if (status == DNS_SUCCESS) {
                COPY_bytes(data_p, ctx_p->work_bkt_p,
                           (DATA_BUCKET_BLOCKS*BLOCK_SIZE));
            }
        } else {
            copy_starting(data_p, ctx_p->work_bkt_p, assessed_count);
            if (assessed_count < data_p->node_count) {
                status = flush_work_bucket(ctx_p);
                if (status == DNS_SUCCESS) {
                    copy_remaining(data_p, ctx_p->work_bkt_p, assessed_count);
                }
            } else if (assessed_count == data_p->node_count) {
                ctx_p->work_bkt_p->node_next_p = data_p->node_next_p;
            } else {
		dce_svc_printf(CDS_S_DBCMP_1_A_MSG,
				__LINE__,  __FILE__);
            }
        }

    }

    return(status);

}


/* -------------------------------------------------------------------------- */

/*
 * level_1_btree_end
 *
 * This routine flushes the work bucket to disk, assuring that any data
 * left over from the compression isn't lost.  It also asserts that the last
 * node_next_p written was null.
 *
 * Returns: The result of the flush (write) request.
 */

static dns_status_t 
level_1_btree_end (private_compression_ctx_t *ctx_p)
{
    dns_status_t status;

    status = flush_work_bucket(ctx_p);

    if (ctx_p->last_node_next_p != NULL) 
    {
	dce_svc_printf(CDS_S_DBCMP_NONNULLPTR_A_MSG,
			__LINE__, __FILE__);      
    }

    return(status);
}



/*
 *                       ----------------
 *                       Public Interface
 *                       ----------------
 */

/* -------------------------------------------------------------------------- */

/*
 * db_compress_init
 *
 * Public view:
 *
 * This routine is called before you start checkpointing the database and some
 * form of data compression is desired.
 *
 * Private view:
 *
 * This routine initializes the data structures required to support a
 * particular level of compression, creates a context structure for
 * subsequent compression-related calls.  The desired compression level
 * is obtained from the FIB.
 *
 * Returns: DNS_SUCCESS       - Compression routines ready.
 *          DNS_NONSRESOURCES - Couldn't allocate memory required by the
 *                              requested compression algorithm (level).
 */

dns_status_t 
db_compress_init (file_info_t        *file_p,
                  compression_ctx_t  *comp_ctx_pp)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_compression_ctx(ctxBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    private_compression_ctx_t *ctx_p;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_compress_init(%s,%p) level(%d)",
        deb_ascii_file_info(fileBuf,file_p),
        comp_ctx_pp,
        file_p->file_ckpt_comp_level));

    if (file_p == NULL) 
    {
	dce_svc_printf(CDS_S_DBCMP_NULLFP_A_MSG,
			__LINE__, __FILE__);
    }

    /*
     * Force checkpoint compression level to 1 if diagnostics aren't
     * overriding us.
     */

    if (!file_p->file_ckpt_force) {
        if (file_p->file_ckpt_comp_level < 1) {
            file_p->file_ckpt_comp_level = 1;
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug5,
                "  db_compress_init(%s,%p) forcing level 1",
                deb_ascii_file_info(fileBuf,file_p),
                comp_ctx_pp));
        }
    } 
    /*
     * Allocate a compression context buffer.
     */

    ctx_p = (private_compression_ctx_t *)dns_malloc(sizeof(private_compression_ctx_t));
    if (ctx_p == NULL) {
        status = DNS_NONSRESOURCES;
	goto leave_db_compress_init;
    }

    ZERO_bytes(ctx_p, sizeof(*ctx_p));

    /*
     * Fill in the the various fields of the compression context buffer.
     * The compression level determines which compression callback routines
     * to utilize, plus any additional resources required.
     */

    ctx_p->level = file_p->file_ckpt_comp_level;
    ctx_p->file_p = file_p;

    switch (ctx_p->level) {
    case 0:
        ctx_p->btree_start_rtn_p = level_0_btree_start;
        ctx_p->btree_write_rtn_p = level_0_btree_write;
        ctx_p->btree_end_rtn_p   = level_0_btree_end;
        break;

    case 1:
        ctx_p->btree_start_rtn_p = level_1_btree_start;
        ctx_p->btree_write_rtn_p = level_1_btree_write;
        ctx_p->btree_end_rtn_p   = level_1_btree_end;
        ctx_p->work_bkt_p = (node_header_t *)dns_malloc(DATA_BUCKET_BLOCKS *
                                                        BLOCK_SIZE);
        if (ctx_p->work_bkt_p == NULL) {
            status = DNS_NONSRESOURCES;
            dns_free(ctx_p);
	    goto leave_db_compress_init;
        }
        ZERO_bytes(ctx_p->work_bkt_p, sizeof(node_header_t));
        break;

    default:
        dce_svc_printf(CDS_S_DBCMP_LEVELBAD_A_MSG,
		       __LINE__, __FILE__);
    }

    /*
     * Return the compression context to the caller.  It's the address of
     * the context buffer, but they don't know that.
     */

    *comp_ctx_pp = (compression_ctx_t)ctx_p;
    status = DNS_SUCCESS;

leave_db_compress_init:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_compress_init(%s,%p->%s) return(%s)",
        deb_ascii_file_info(fileBuf,file_p),
        comp_ctx_pp,
        deb_ascii_compression_ctx(ctxBuf,ctx_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/* -------------------------------------------------------------------------- */

/*
 * db_compress_btree_start
 *
 * Public view:
 *
 * This routine is called just prior to checkpointing a BTree (directory
 * contents).   The VBN (virtual block number) of the checkpoint file must
 * be passed in so the compression routines know where to begin writing
 * the data on disk.  The updated VBN is returned when btree compression is
 * completed.
 *
 * Private view:
 *
 * This routine maps the public compression context identifier to the
 * private compression context buffer, then it sets up the compression
 * context block and calls the compression-level-specific initialization in
 * support of subsequent calls to the db_compress_btree_* routines.
 *
 * Returns: Result of compression-specific startup.
 */

dns_status_t 
db_compress_btree_start (compression_ctx_t  comp_ctx_p,
                         unsigned int       next_vbn)
{
    private_compression_ctx_t *ctx_p = (private_compression_ctx_t *)comp_ctx_p;

    if (ctx_p == NULL) 
    {
	dce_svc_printf(CDS_S_DBCMP_NULLCTXSTART_A_MSG,
			__LINE__, __FILE__);
    }

    /*
     * Record the checkpoint routine's next_vbn variable for compression
     * write routine's use.
     */

    ctx_p->next_vbn = next_vbn;

    return ((*ctx_p->btree_start_rtn_p)(ctx_p));
}

/* -------------------------------------------------------------------------- */

/*
 * db_compress_btree_write
 *
 * Public view:
 *
 * This routine is called once for every bucket in a BTree that is being
 * checkpointed.
 *
 * Private view:
 *
 * This routine maps the public compression context identifier to the
 * private compression context buffer, then it calls the
 * compression-level-specific write routine.
 *
 * Returns: Result of compression-level-specific write routine.
 */

dns_status_t 
db_compress_btree_write (compression_ctx_t  comp_ctx_p,
                         node_header_t      *data_p)
{
    private_compression_ctx_t *ctx_p = (private_compression_ctx_t *)comp_ctx_p;

    if (ctx_p == NULL) 
    {
	dce_svc_printf(CDS_S_DBCMP_NULLCTXWRITE_A_MSG,
			__LINE__, __FILE__);
    }

    return ((*ctx_p->btree_write_rtn_p)(ctx_p, data_p));
}

/* -------------------------------------------------------------------------- */

/*
 * db_compress_btree_end
 *
 * Public view:
 *
 * This routine is called after the last bucket in a BTree has been handed
 * to the db_compress_btree_write routine.  It also returns what next VBN
 * of the checkpoint file to begin writing at.
 *
 * Private view:
 *
 * This routine maps the public compression context identifier to the
 * private compression context buffer, then it calls the compression-level-
 * specific end routine.  Finally it writes the next VBN (virtual block
 * number) of the checkpoint file to the caller's location.
 *
 * Returns: Result of the compression-level-specific end routine.
 */

dns_status_t 
db_compress_btree_end (compression_ctx_t  comp_ctx_p,
                       unsigned int       *next_vbn_p)
{
    private_compression_ctx_t *ctx_p = (private_compression_ctx_t *)comp_ctx_p;
    dns_status_t status;

    if (ctx_p == NULL) 
    {
	dce_svc_printf(CDS_S_DBCMP_NULLCTXEND_A_MSG,
			__LINE__,  __FILE__); 
    }

    status = (*ctx_p->btree_end_rtn_p)(ctx_p);

    /*
     * Return the updated VBN outside routines should now use.
     */

    *next_vbn_p = ctx_p->next_vbn;

    return(status);
}

/* -------------------------------------------------------------------------- */

/*
 * db_compress_finish
 *
 * Public view:
 *
 * This routine is called when a checkpoint has completed (prior to closing
 * the checkpoint file).  The compression context is invalid after this call.
 *
 * Private view:
 *
 * This routine frees the compression context buffer, work buffers, and
 * (someday) transfers statistics to the file_info block.
 *
 * Returns: DNS_SUCCESS       - Compression context successfully released.
 */

dns_status_t 
db_compress_finish (compression_ctx_t comp_ctx_p)
{
    DEB_ASCII_BUF_compression_ctx(ctxBuf)
    private_compression_ctx_t *ctx_p = (private_compression_ctx_t *)comp_ctx_p;


    if (ctx_p == NULL) 
    {
	dce_svc_printf(CDS_S_DBCMP_NULLCTXFINISH_A_MSG,
			__LINE__, __FILE__);
    }

    /*
     * Free the work bucket if it has been allocated.
     */

    if (ctx_p->work_bkt_p) {
        dns_free(ctx_p->work_bkt_p);
    }


    /*
     * Post-Compression Checks
     */

    dce_assert(cds__svc_handle, 
		(ctx_p->node_count_in == ctx_p->node_count_written));

    if (ctx_p->data_bytes_written > ctx_p->data_bytes_in) {
	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_database,
	    svc_c_debug5,
	    "W db_compress_finish(%s) data written (%d) > data in (%d)",
	    deb_ascii_compression_ctx(ctxBuf,comp_ctx_p),
	    ctx_p->data_bytes_written,
	    ctx_p->data_bytes_in));
    }
    if (ctx_p->free_bytes_written > ctx_p->free_bytes_in) {
	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_database,
	    svc_c_debug5,
	    "W db_compress_finish(%s) free written (%d) > free in (%d)",
	    deb_ascii_compression_ctx(ctxBuf,comp_ctx_p),
	    ctx_p->data_bytes_written,
	    ctx_p->data_bytes_in));
    }

    /*
     * Transfer statistics to the file_info block.
     */

    ctx_p->file_p->file_ckpt_dbytes_in   = ctx_p->data_bytes_in;
    ctx_p->file_p->file_ckpt_fbytes_in   = ctx_p->free_bytes_in;
    ctx_p->file_p->file_ckpt_dbytes_out  = ctx_p->data_bytes_written;
    ctx_p->file_p->file_ckpt_fbytes_out  = ctx_p->free_bytes_written;
    ctx_p->file_p->file_ckpt_node_count  = ctx_p->node_count_in;
    ctx_p->file_p->file_ckpt_buckets_in  = ctx_p->buckets_in;
    ctx_p->file_p->file_ckpt_buckets_out  = ctx_p->buckets_written;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        "  db_compress_finish(%s) - Data bytes in .... %8d",
        deb_ascii_compression_ctx(ctxBuf,comp_ctx_p),
        ctx_p->data_bytes_in));
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        "  db_compress_finish(%s) - Free bytes in .... %8d",
        deb_ascii_compression_ctx(ctxBuf,comp_ctx_p),
        ctx_p->free_bytes_in));
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        "  db_compress_finish(%s) - Buckets in ....... %8d",
        deb_ascii_compression_ctx(ctxBuf,comp_ctx_p),
        ctx_p->buckets_in));
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        "  db_compress_finish(%s) - Node count in .... %8d",
        deb_ascii_compression_ctx(ctxBuf,comp_ctx_p),
        ctx_p->node_count_in));
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        "  db_compress_finish(%s) - Data bytes written %8d",
        deb_ascii_compression_ctx(ctxBuf,comp_ctx_p),
        ctx_p->data_bytes_written));
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        "  db_compress_finish(%s) - Free bytes written %8d",
        deb_ascii_compression_ctx(ctxBuf,comp_ctx_p),
        ctx_p->free_bytes_written));
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        "  db_compress_finish(%s) - Buckets written .. %8d",
        deb_ascii_compression_ctx(ctxBuf,comp_ctx_p),
        ctx_p->buckets_written));
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        "  db_compress_finish(%s) - Node count written %8d",
        deb_ascii_compression_ctx(ctxBuf,comp_ctx_p),
        ctx_p->node_count_written));
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        "  db_compress_finish(%s) - L1: norm %d, cs %d, wtb %d",
        deb_ascii_compression_ctx(ctxBuf,comp_ctx_p),
        ctx_p->normal_fire_count,
        ctx_p->clean_slate_fire_count,
        ctx_p->way_too_big_fire_count));

    /*
     * Free the compression context block.
     */

    dns_free(ctx_p);

    return(DNS_SUCCESS);
}





