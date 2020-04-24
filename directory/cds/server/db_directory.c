/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_directory.c,v $
 * Revision 1.1.8.2  1996/02/18  19:35:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:23  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:30:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:00  root]
 * 
 * Revision 1.1.6.7  1994/09/07  20:02:16  mccann
 * 	fix version handling OT:12004
 * 	[1994/09/07  18:42:10  mccann]
 * 
 * Revision 1.1.6.6  1994/08/26  21:55:30  zee
 * 	Add fastpath pointer for DirectoryVersion.
 * 	[1994/08/26  21:49:11  zee]
 * 
 * Revision 1.1.6.5  1994/08/03  19:03:59  mccann
 * 	fix assert
 * 	[1994/07/21  20:14:31  mccann]
 * 
 * 	final sams cleanup drop
 * 	[1994/07/19  18:21:40  mccann]
 * 
 * Revision 1.1.6.4  1994/06/09  18:43:13  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:40  devsrc]
 * 
 * Revision 1.1.6.3  1994/05/06  16:04:20  zee
 * 	     HCell BL4 support: Add fastpath pointer for parentcellpointers.
 * 	[1994/05/05  21:19:56  zee]
 * 
 * Revision 1.1.6.2  1994/03/22  20:00:24  griffin
 * 	HCell BL3 support: Support update of fastpath attribute pointers for
 * 	the root directory.
 * 	[1994/03/22  18:06:09  griffin]
 * 
 * Revision 1.1.6.1  1994/03/12  22:10:56  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:41:13  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  14:20:59  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:21:08  zeliff]
 * 
 * Revision 1.1.4.2  1992/11/13  19:43:26  peckham
 * 	OT#5901: propagates not happening. Good sleuthing by John Grober.
 * 	dir_read() was putting the directory CTS in the db_entry.data_cts.
 * 	[1992/11/13  19:41:52  peckham]
 * 
 * Revision 1.1.2.2  1992/01/22  23:02:02  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:17:42  melman]
 * 
 * $EndLog$
 */
/*
 * Module: db_directory.c
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
#include <sets.h>
#include <tlog.h>
#include <uid.h>

static version_t    default_version = {{3}, {0}};

/*
 * Local prototypes
 */
static dns_status_t
db_dir_build (
    db_stream_t         *);

static dns_status_t
dir_read (
    db_stream_t         *,
    unsigned int        ,
    int                  );

static void
refresh_fastpaths (
    dir_hash_entry_t   *);

/* -------------------------------------------------------------------------- */

/*
 * db_dir_release_lock
 *      Release a lock on a directory bucket
 *
 * Input:
 *      stream_p = pointer to data base stream
 *
 */
void
db_dir_release_lock (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    dir_hash_entry_t *const hash_p  = stream_p->db_dir.data_hash_p;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_dir_release_lock(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    ASSERT_ACTIVE_STREAM(stream_p);

    if (hash_p)
    {
        if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
        {
	  dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
        }

           /* Beginning of the 'protected code' */
           {
               if (hash_p->dir_update_access_owner == stream_p)
               {
                   hash_p->dir_update_access_owner = NULL;

                   if ((thstatus = dthread_signal(&(hash_p->dir_update_accessable))) < 0)
                   {
		     dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, 
				    (long)thstatus);
                   }
               }
               else
               {
                   DCE_SVC_DEBUG((
                       cds__svc_handle,
                       cds_svc_database,
                       svc_c_debug3,
                       "db_dir_release_lock(%s) - Redundant/illegal release lock detected (%x)",
                       streamBuf,
                       hash_p->dir_update_access_owner));
               }
               stream_p->db_dir.data_hash_p = NULL;
               --hash_p->dir_users;
           }
           /* End of the 'protected code' */

        if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
        {
	  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
        }
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_dir_release_lock()"));

}


/* -------------------------------------------------------------------------- */

/*
 * db_dir_release
 *      Release a lock on a directory bucket and free buffer
 *
 * Input:
 *      stream_p = pointer to data base stream
 *
 */
void
db_dir_release (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_dir_release(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    ASSERT_ACTIVE_STREAM(stream_p);

    /* Release lock first */
    db_dir_release_lock(stream_p);

    /* Free buffers */
    if (stream_p->db_dir.data_p)
        {
        dns_free((char *)stream_p->db_dir.data_p);
        stream_p->db_dir.data_p = NULL;
        }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_dir_release()"));

}


/* -------------------------------------------------------------------------- */

/*
 * db_dir_get_replica_state
 *      Return replica state value from the hash entry
 *
 * Input:
 *      hash_p = pointer to hash entry
 * Value:
 *      state
 */
dns_status_t
db_dir_get_replica_state (dir_hash_entry_t *hash_p)
{
    return(hash_p->dir_replica_state);
}


/* -------------------------------------------------------------------------- */

/*
 * db_dir_get_replica_type
 *      Locate the directory replica type from the hash table
 *
 * Input:
 *      hash_p = pointer to hash entry
 * Value:
 *      type
 */
dns_status_t
db_dir_get_replica_type (dir_hash_entry_t *hash_p)
{
    return(hash_p->dir_replica_type);
}


/* -------------------------------------------------------------------------- */

/*
 * db_dir_get_version
 *      This routine locates the directory version
 *      and returns it to the caller.
 *      If the attribute is not found, it is defaulted to 1.0
 *
 * Input:
 *      stream_p = address of stream descriptor, directory read
 * Output:
 *      version_p = address to return version
 *
 */
void
db_dir_get_version (db_stream_t  *stream_p,
                    version_t    *version_p)
{
    version_t           *set_ver_p = &default_version;
    DBSet_t             *set_p;
    int                 index;

    ASSERT_ACTIVE_STREAM(stream_p);

    if (find_attribute(stream_p->db_dir.data_p,
                dns_AttributeName(DNS_DIRECTORYVERSION),
                &set_p, &index) == DNS_SUCCESS) {
        DBSetMember_t *const value_p = find_last_value(set_p);

        if (value_p)
            set_ver_p = (version_t *)value_p->dm_data;
    }

    COPY_version(set_ver_p, version_p);
}


/* -------------------------------------------------------------------------- */

/*
 * db_dir_set_lastupdate
 *      Set cached directory lastupdate value in the hash table.  This
 *      prevents us from having to have a lock on the directory
 *      for all updates.
 *      Don't have to build transaction, cause this is the result of
 *      other updates.
 *
 * Input:
 *      hash_p = address of hash descriptor for directory
 *      cts_p  = address of lastupdate value
 *
 * Value:
 *      state
 */
void
db_dir_set_lastupdate (dir_hash_entry_t  *hash_p,
                       Timestamp_t       *cts_p)
{
    DEB_ASCII_BUF_dir_hash_entry(hashBuf)
    DEB_ASCII_BUF_Timestamp(ctsBuf)
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " >db_dir_set_lastupdate(%s,%s)",
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_Timestamp(ctsBuf,cts_p)));

    if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {
            if (dnsCmpCTS((dns_cts *)cts_p,(dns_cts *)hash_p->dir_lastupdate) >= 0)
                COPY_Timestamp(cts_p, hash_p->dir_lastupdate);
        }
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }
}


/* -------------------------------------------------------------------------- */

/*
 * db_dir_set_state
 *      Set directory state in directory record and hash entry.
 *      Return transaction.
 *
 * Input:
 *      stream_p = address of stream descriptor, directory locked
 *      state = new state
 *      upd_p = buffer for update
 *      len_p = address to return length of update
 *
 * Value:
 *      state
 */
dns_status_t
db_dir_set_state (db_stream_t  *stream_p,
                  int          state,
                  Update_t     *upd_p,
                  int          *len_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    Update_u            update;
    byte_t              new_state = state;
    file_info_t         *file_p = stream_p->db_file_p;
    dir_hash_entry_t    *hash_p = stream_p->db_dir_hash_p;
    int                 len;
    dns_status_t        status;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " >db_dir_set_state(%s,%d,,) -- from state %d",
        deb_ascii_db_stream(streamBuf,stream_p),
        state,
        hash_p->dir_replica_state));

    ASSERT_ACTIVE_STREAM(stream_p);

    if (!upd_p)
        {
        upd_p = (Update_t *)update;
        len_p = &len;
        }

    /* Get update structure */
    *len_p = update_dns_build(upd_p, UD_present, NullTimestamp,
        dns_Attribute(DNS_REPLICASTATE), BL_true, sizeof(new_state),
        &new_state);

    /* Replace value in data record */
    status = set_replace_value(&stream_p->db_dir, upd_p,
                dns_Attribute(DNS_REPLICASTATE));
    if (status != DNS_SUCCESS) return(status);

    if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }


    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

       /* Beginning of the 'protected code' */

       /*
        * Properly handle death state transitions.
        * If the directory is dead, then it shouldn't count towards the
        * checkpoint file's directory count.
        */
        if (state == RS_dead) {
            if (hash_p->dir_replica_state != RS_dead) {
                --file_p->file_ckpt_hdr.hdr_dir_count;
            }
        }
        else {
            if (hash_p->dir_replica_state == RS_dead) {
                file_p->file_ckpt_hdr.hdr_dir_count++;
            }
        }

        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    /* Replace value in hash entry */
    hash_p->dir_replica_state = state;

    if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " <db_dir_set_state(%s,%d,,) -- hdr_dir_count = %d",
        streamBuf,
        state,
        file_p->file_ckpt_hdr.hdr_dir_count));

    return(DNS_SUCCESS);
}


/* -------------------------------------------------------------------------- */

/*
 * db_dir_set_type
 *      Set directory type in directory record and hash entry.
 *      Return transaction.
 *
 * Input:
 *      stream_p = address of stream descriptor, directory locked
 *      type = new type
 *      upd_p = buffer for update
 *      len_p = address to return length of update
 *
 * Value:
 *      state
 */
dns_status_t
db_dir_set_type (db_stream_t  *stream_p,
                 int          type,
                 Update_t     *upd_p,
                 int          *len_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    Update_u            update;
    byte_t              new_type = type;
    dir_hash_entry_t    *hash_p = stream_p->db_dir_hash_p;
    int                 len;
    dns_status_t        status;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " >db_dir_set_type(%s,%d,,)",
        deb_ascii_db_stream(streamBuf,stream_p),
        type));

    ASSERT_ACTIVE_STREAM(stream_p);

    if (!upd_p)
        {
        upd_p = (Update_t *)update;
        len_p = &len;
        }

    /* Get update structure */
    *len_p = update_dns_build(upd_p, UD_present, NullTimestamp,
        dns_Attribute(DNS_REPLICATYPE), BL_true, sizeof(new_type),
        &new_type);

    /* Replace value in data record */
    status = set_replace_value(&stream_p->db_dir, upd_p,
                dns_Attribute(DNS_REPLICATYPE));
    if (status != DNS_SUCCESS) return(status);

    if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */

        /* Replace value in hash entry */
        hash_p->dir_replica_type = type;

        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    return(DNS_SUCCESS);
}


/* -------------------------------------------------------------------------- */

/*
 *
 * db_dir_build_memory_record
 *      Read directory bucket and allocate in memory structures.
 *      Insert in hash table.
 *
 * Input:
 *      file_p = pointer to file info block
 *      next_vbn_p = address of next VBN to read
 *      ckpt_major_version - Version of the checkpoint file supplying
 *      ckpt_minor_version - the data for this directory.
 *
 * Output:
 *      next_vbn_p = updated
 *      hash_pp = address to return pointer to directory hash entry created
 */
dns_status_t
db_dir_build_memory_record (file_info_t       *file_p,
                            unsigned int      *next_vbn_p,
                            dir_hash_entry_t  **hash_pp,
                            conversion_ctx_t  conversion_ctx)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    /* NOTE: we don't have to read in DIRECTORY_BUCKET_BLOCKS of data */
    union {
        directory_bucket_t      s;
        bytes_u                 b[BLOCK_SIZE];
    }                   bucket;
    directory_bucket_t  *bkt_p = &bucket.s, *dir_bkt_p;
    dns_status_t         status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >db_dir_build_memory_record(%s)",
        deb_ascii_file_info(fileBuf,file_p)));

    /* Read first bucket of directory data */
    status = sys_read(file_p->file_ckpt_block, bucket.b, *next_vbn_p,
                (sizeof(bucket)/BLOCK_SIZE));
    if (status != DNS_SUCCESS) return(status);

    dir_bkt_p = (directory_bucket_t *)dns_malloc_perm(
                bkt_p->dir_blocks*BLOCK_SIZE);
    if (!dir_bkt_p) return(DNS_NONSRESOURCES);

    /* Copy what we have to in-memory record */
    COPY_bytes(bucket.b, dir_bkt_p, sizeof(bucket));

    *next_vbn_p += sizeof(bucket)/BLOCK_SIZE;

    /* See if directory spans multiple buckets */
    if ((sizeof(bucket)/BLOCK_SIZE) < dir_bkt_p->dir_blocks)
        {
        /* Read remaining blocks of directory into in-memory record */
        status = sys_read(file_p->file_ckpt_block,
                SKIP_bytes(dir_bkt_p, sizeof(bucket)),
                *next_vbn_p,
                dir_bkt_p->dir_blocks - (sizeof(bucket)/BLOCK_SIZE));

        *next_vbn_p += dir_bkt_p->dir_blocks - (sizeof(bucket)/BLOCK_SIZE);

        if (status != DNS_SUCCESS)
            {
            dns_free((char *)dir_bkt_p);
            return(DNS_NONSRESOURCES);
            }
        }


    /*
     * Insert the directory into the hash tables
     */

    if (dir_bkt_p->dir_type == DIRECTORY_BUCKET)
    {
        dir_hash_entry_t    *hash_p;       /* Local copy of derefed hash_pp */

        status = db_hash_insert(file_p, dir_bkt_p, ET_directory, hash_pp);

        /*
         * Make sure attribute fastpaths are secure if this is the root
         * directory.
         */

        hash_p = *hash_pp;
        if (hash_p->dir_root) {
            refresh_fastpaths(hash_p);
        }

    } 
    else {
        status = db_hash_insert(file_p, dir_bkt_p, ET_clearinghouse, hash_pp);
    }


    return(status);
}

/* -------------------------------------------------------------------------- */

/*
 * db_dir_build
 *      Allocate a directory bucket, fill it in and
 *      initialize btree.
 *
 * Input:
 *      stream_p = pointer to user stream with record data
 */
static dns_status_t
db_dir_build (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    static max_name[2] = {255, 0};
    file_info_t         *file_p = stream_p->db_file_p;
    directory_bucket_t  *dir_p;
    node_header_t       *data_bkt_p;
    entry_key_t         *rec_p;
    int                 size;
    dns_status_t        status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_dir_build(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    ASSERT_ACTIVE_STREAM(stream_p);

    /*
     * Allocate either DIRECTORY_BUCKET_BLOCKS or input length
     * rounded to a multiple of a block, whichever is larger
     */
    size = LEN_DBSet(stream_p->db_dir.data_p->dbr_data);
    size = dns_roundup(size);
    if (size < DIRECTORY_BUCKET_BLOCKS)
        size = DIRECTORY_BUCKET_BLOCKS;

    dir_p = (directory_bucket_t *)dns_malloc_perm(size * BLOCK_SIZE);
    if (!dir_p) return(DNS_NONSRESOURCES);

    /* Initialize directory bucket */
    dir_p->dir_type = DIRECTORY_BUCKET;
    dir_p->dir_blocks = size;
    COPY_DBSet(stream_p->db_dir.data_p->dbr_data, dir_p->dir_data);

   /*
    * The Btree starts out with a data bucket.  Only when
    * this buckets splits do we introduce a real index bucket.  This
    * is an optimization for small directories.  No sense in adding an
    * index level if it isn't necessary.
    */
    data_bkt_p = (node_header_t *)dns_malloc_perm(
                DATA_BUCKET_BLOCKS*BLOCK_SIZE);
    if (!data_bkt_p)
        {
        dns_free((char *)dir_p);
        return(DNS_NONSRESOURCES);
        }

    /* Now initalize data bucket */
    data_bkt_p->node_type = DATA_BUCKET;
    data_bkt_p->node_blocks = DATA_BUCKET_BLOCKS;
    data_bkt_p->node_next_p = NULL;
    data_bkt_p->node_prev_p = NULL;
    data_bkt_p->node_count = 1;


    /*
     * We add a dummy record which will always sort last,
     * so inserts will always be before it
     * and buckets cannot be added to the end.
     */
    rec_p = (entry_key_t *)INDEX_node_rec_dsc(data_bkt_p, 1);
    INS8(rec_p->key_type, 255);
    INS8(rec_p->key_sort, 255);
    COPY_AttributeName(max_name, rec_p->key_name);
    rec_p->key_length = DIFF_bytes(SKIP_AttributeName(rec_p->key_name), rec_p);

    data_bkt_p->node_rec_dsc[0].rec_offset = DIFF_bytes(rec_p, data_bkt_p);
    data_bkt_p->node_rec_dsc[0].rec_length =
        DIFF_bytes(SKIP_AttributeName(rec_p->key_name), rec_p);
    data_bkt_p->node_space = DATA_BUCKET_BLOCKS*BLOCK_SIZE -
        DIFF_bytes(SKIP_AttributeName(rec_p->key_name), data_bkt_p);

    status = db_hash_insert(file_p, dir_p, ET_directory,
                &stream_p->db_dir_hash_p);

    if (status == DNS_SUCCESS) {
        stream_p->db_dir_hash_p->dir_entry_p = data_bkt_p;
        /*
         * Make sure attribute fastpaths are secure if this is the root
         * directory.
         */
        if (stream_p->db_dir_hash_p->dir_root) {
            refresh_fastpaths(stream_p->db_dir_hash_p);
        }
    } else {
        dns_free((char *)data_bkt_p);
        dns_free((char *)dir_p);
    }

    return(status);
}



/* -------------------------------------------------------------------------- */

/*
 * db_dir_create
 *      Create a new directory.
 *
 * Input:
 *      stream_p = pointer to stream descriptor
 */
dns_status_t
db_dir_create (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    ObjUID_t            chdir;
    file_info_t         *file_p = stream_p->db_file_p;
    dir_hash_entry_t    *hash_p;
    dns_status_t        status;
    DBSet_t             *set_p;
    int                 index;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_dir_create(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    ASSERT_ACTIVE_STREAM(stream_p);

    /* Allocate and build directory bucket and entry index */
    status = db_dir_build(stream_p);
    if (status != DNS_SUCCESS) return(status);

    hash_p = stream_p->db_dir_hash_p;
    /*
     * Add directory to chdirectories.
     *
     * We optimize a bit over the architecture here.  Only the directory
     * ID is stored in the database (the architecture directory pointer
     * structure is dynamically created when requested through the TA).
     * This optimization save a bit of space and it also will ease directory
     * renaming in the future.
     */
    COPY_ObjUID(hash_p->dir_uid, &chdir);
    (void)db_pseudo_chdirectory(file_p, &chdir, UD_present);

    /* Log creation in transaction log */
    db_tlog_create(file_p, stream_p->db_function, ET_directory,
                (FullName_t *)hash_p->dir_name,
                (ObjUID_t *)hash_p->dir_uid,
                LEN_DBSet(stream_p->db_dir.data_p->dbr_data),
                stream_p->db_dir.data_p->dbr_data);

    /* Flag directory as locked */

    if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */

        while (hash_p->dir_update_access_owner)
          {
              if ((thstatus = dthread_wait(&(hash_p->dir_update_accessable),
                                           &(hash_p->dir_mutex))) < 0)
              {
		dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
              }
          }

        hash_p->dir_update_access_owner = stream_p;
        stream_p->db_dir.data_hash_p = hash_p;
        hash_p->dir_users++;

        /* End of the 'protected code' */

        if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
        {
	  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
        }

    /* Complete filling in stream */
    stream_p->db_dir.data_type = ET_directory;
    COPY_ObjUID(hash_p->dir_uid, stream_p->db_dir.data_uid);
    COPY_FullName(hash_p->dir_name, stream_p->db_key);

    if (find_attribute(stream_p->db_dir.data_p,
            dns_AttributeName(DNS_CTS), &set_p, &index) == DNS_SUCCESS)
        {
        /*
         * DNS_CTS is always a single valued set with no history
         */
        DBSetMember_t *member_p = (DBSetMember_t *)DATA_DBSet(set_p);

        COPY_Timestamp(member_p->dm_data, stream_p->db_dir.data_cts);
        }
    else
        COPY_Timestamp(NullTimestamp, stream_p->db_dir.data_cts);

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

         /* Beginning of the 'protected code' */

         file_p->file_ckpt_hdr.hdr_dir_count++;

         /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    return(DNS_SUCCESS);
}


/* -------------------------------------------------------------------------- */

/*
 * db_dir_create_recover
 *      This routine is called during recover.  Create or replace
 *      a directory from data in the transaction log file.
 *      Create the btree for the entry index.
 *      This is different then dir_create because the directory is not
 *      return locked.
 *
 * Input:
 *      stream_p = pointer to stream descriptor
 */
dns_status_t
db_dir_create_recover (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    ObjUID_t            chdir;
    file_info_t         *file_p = stream_p->db_file_p;
    dir_hash_entry_t    *hash_p;
    dns_status_t        status;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " >db_dir_create_recover(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    ASSERT_ACTIVE_STREAM(stream_p);

    /* Allocate and build directory bucket and entry index */
    status = db_dir_build(stream_p);
    if (status != DNS_SUCCESS) return(status);

    hash_p = stream_p->db_dir_hash_p;

    /* Add directory to chdirectories */
    COPY_ObjUID(hash_p->dir_uid, &chdir);
    (void)db_pseudo_chdirectory(file_p, &chdir, UD_present);

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */

        file_p->file_ckpt_hdr.hdr_dir_count++;

        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    return(DNS_SUCCESS);
}


/* -------------------------------------------------------------------------- */

/*
 * dir_read
 *      This routine reads a directory record.
 *
 * Arguments:
 *      stream_p       - (in) pointer to stream descriptor
 *      lock_state     - (in) lock state desired
 *      directory_type - (in) type of directory (normal, pseudo)
 *
 */
static dns_status_t
dir_read (db_stream_t   *stream_p,
          unsigned int  lock_state,
          int           directory_type)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    file_info_t         *file_p = stream_p->db_file_p;
    dir_hash_entry_t    *hash_p;
    dns_status_t        status = DNS_UNKNOWNENTRY;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " >dir_read(%s,%d,%d)",
        deb_ascii_db_stream(streamBuf,stream_p),
        lock_state,
        directory_type));


    ASSERT_ACTIVE_STREAM(stream_p);

    if (stream_p->db_dir.data_p)
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug3,
            "dir_read(%s,%d,%d) data_p is non-NULL (0x%x)",
            deb_ascii_db_stream(streamBuf,stream_p),
            lock_state,
            directory_type, stream_p->db_dir.data_p));
    }

    stream_p->db_dir.data_p = NULL;

    /* Get hash entry, may already be set up. */
    if (!(hash_p = stream_p->db_dir_hash_p))
        {
        if (EMPTY_FullName(stream_p->db_key))
            {
            FullName_t *fname_p = (FullName_t *)stream_p->db_key;

            hash_p = db_hash_uid(file_p, (ObjUID_t *)fname_p->fn_root);
            }
        else
            hash_p = db_hash_name(file_p, (FullName_t *)stream_p->db_key);

        if (!hash_p)
            {
          DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_database,
              svc_c_debug3,
              " <dir_read(%s,%d,%d) return(%d) no hash table",
              streamBuf,
              lock_state,
              directory_type,
              status));

            return(status);
            }
        if (hash_p->dir_type != directory_type)
            {
          DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_database,
              svc_c_debug3,
              " <dir_read(%s,%d,%d) return(%d) wrong hash dir_type(%d)",
              streamBuf,
              lock_state,
              directory_type,
              status,
              hash_p->dir_type));

            return (status);
            }
        stream_p->db_dir_hash_p = hash_p;
        }

    /*
     * Get update access lock if requested
     * When we get the lock, mark the directory as locked, and record this
     * thread as the owner (useful for diagnostics).
     */
    if (lock_state == UPDATE_LOCK)
    {
        if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
        {
	  dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
        }

            /* Beginning of the 'protected code' */
            {

                while (hash_p->dir_update_access_owner)
                {
                    if ((thstatus = dthread_wait(&(hash_p->dir_update_accessable),
                                                 &(hash_p->dir_mutex))) < 0)
                    {
		      dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, 
				     (long)thstatus);
                    }
                }

                hash_p->dir_update_access_owner = stream_p;
                stream_p->db_dir.data_hash_p = hash_p;
                hash_p->dir_users++;
            }
            /* End of the 'protected code' */

        if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
        {
	  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
        }
    }

    /*
     * We must do this under lock to insure a consistent copy
     */

    if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

         /* Beginning of the 'protected code' */
         {  /* b */

             /* Make sure directory hasn't been deleted, hash allways valid */

          if (hash_p->dir_p) {
          register DBSet_t *set_p = (DBSet_t *)hash_p->dir_p->dir_data;
          register data_record_t *rec_p;
          int size;
          DBSet_t             *cts_p;
          int                 index;

          ALIGN_CHECK(set_p);
          /* Get bytes needed */
          size = sizeof(*rec_p) - sizeof(rec_p->dbr_data) + LEN_DBSet(set_p);

          /* Round up to a multiple of a block */
          size = dns_roundup(size)*BLOCK_SIZE;

          /* Set up return data */
          rec_p = (data_record_t *)dns_malloc_temp(size);
          if (rec_p) {
            COPY_DBSet(set_p, rec_p->dbr_data);
            stream_p->db_dir.data_type = hash_p->dir_type;
            stream_p->db_dir.data_buf_len = size;
            stream_p->db_dir.data_p = rec_p;
            COPY_ObjUID(hash_p->dir_uid, stream_p->db_dir.data_uid);
            if (find_attribute(stream_p->db_dir.data_p,
                    dns_AttributeName(DNS_CTS), &cts_p, &index) == DNS_SUCCESS)
              {
                /*
                 * DNS_CTS is always a single valued set with no history
                 */
                DBSetMember_t *member_p = (DBSetMember_t *)DATA_DBSet(cts_p);

                COPY_Timestamp(member_p->dm_data, stream_p->db_dir.data_cts);
              }
            else
              COPY_Timestamp(NullTimestamp, stream_p->db_dir.data_cts);

            /*
             * Performance optimization:
             * Lastupdate is kept in the hash table so we don't have
             * to lock the directory on every update operation.
             * This means when the directory is read, we must update
             * lastupdate within the directory record returned
             * to the caller so they see the most recent value.
             */
            if (hash_p->dir_type == ET_directory) {
              (void)set_timestamp_set(&stream_p->db_dir,
                          (Timestamp_t *)hash_p->dir_lastupdate,
                          dns_Attribute(DNS_LASTUPDATE), NULL, NULL);
            }
            status = DNS_SUCCESS;
          } else
            status = DNS_NONSRESOURCES;
        } else
          status = DNS_UNKNOWNENTRY;

      }
      /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    if ((status != DNS_SUCCESS) && (lock_state == UPDATE_LOCK))
      db_dir_release_lock(stream_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " <dir_read(%s,%d,%d) return(%d)",
        streamBuf,
        lock_state,
        directory_type,
        status));

    return(status);
}


/* -------------------------------------------------------------------------- */

/*
 * db_dir_read
 *      This routine reads a normal directory record without locking it.
 *
 * Input:
 *      stream_p = pointer to stream descriptor
 *
 */
dns_status_t
db_dir_read (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_dir_read(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    return(dir_read(stream_p, NO_LOCK, ET_directory));
}


/* -------------------------------------------------------------------------- */

/*
 * db_dir_read_for_update
 *      This routine reads a normal directory record and locks it
 *      for update.  This allows other readers by no writers.
 *
 * Input:
 *      stream_p = pointer to stream descriptor
 *
 */
dns_status_t
db_dir_read_for_update (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_dir_read_for_update(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    return(dir_read(stream_p, UPDATE_LOCK, ET_directory));
}


/* -------------------------------------------------------------------------- */

/*
 * db_pdir_read
 *      This routine reads a pseudo-directory record without locking it.
 *
 * Input:
 *      stream_p = pointer to stream descriptor
 *
 */
dns_status_t
db_pdir_read (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_pdir_read(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    return(dir_read(stream_p, NO_LOCK, ET_clearinghouse));

}


/* -------------------------------------------------------------------------- */

/*
 * db_pdir_read_for_update
 *      This routine reads a pseudo-directory record and locks it
 *      for update.  This allows other readers by no writers.
 *
 * Input:
 *      stream_p = pointer to stream descriptor
 *
 */
dns_status_t
db_pdir_read_for_update (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_pdir_read_for_update(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    return(dir_read(stream_p, UPDATE_LOCK, ET_clearinghouse));
}


/* -------------------------------------------------------------------------- */

/*
 * refresh_fastpaths
 *
 * This routine is called only when the directory being modified is the
 * root directory.  It is only called by db_dir_update().
 *
 * We locate the following attributes and update pointers to their values
 * in the DHE (hash_p):
 *                      cds_AllUpTo
 *                      cds_CellAliases
 *                      cds_ParentCellPointers
 *                      cds_DirectoryVersion
 *
 * This routine asserts that cds_AllUpTo and cds_DirectoryVersion are
 * always present, but allows cds_CellAliases and cds_ParentCellPointers
 * to be absent.
 */

static void
refresh_fastpaths (
    dir_hash_entry_t   *hash_p)

{
    DBSet_t             *set_p;        /* Temp database set pointer */
    DBSetMember_t       *value_p;      /* Temp value pointer */
    dns_status_t         status;       /* Temp status holder */
    int                  idx;          /* Not used: set index */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " >refresh_fastpaths()"));

    dce_assert(cds__svc_handle, (hash_p->dir_root != 0));

    /*
     * AllUpTo
     */

    status = find_attribute((data_record_t *)hash_p->dir_p->dir_data,
                dns_AttributeName(DNS_ALLUPTO), &set_p, &idx);
    dce_assert(cds__svc_handle, (status == DNS_SUCCESS));
    value_p = (DBSetMember_t *)DATA_DBSet(set_p);
    hash_p->dir_allupto_p = (Timestamp_t *)value_p->dm_data;


    /*
     * CellAliases
     */

    status = find_attribute((data_record_t *)hash_p->dir_p->dir_data,
                dns_AttributeName(DNS_CELLALIASES), &set_p, &idx);
    if (status == DNS_SUCCESS) {
        hash_p->dir_cellalias_set_p = set_p;
    } else {
        hash_p->dir_cellalias_set_p = NULL;
    }

    /*
     * ParentCellPointers
     */

    status = find_attribute((data_record_t *)hash_p->dir_p->dir_data,
                dns_AttributeName(DNS_PARENTCELLPOINTERS), &set_p, &idx);
    if (status == DNS_SUCCESS) {
        hash_p->dir_cellparent_set_p = set_p;
    } else {
        hash_p->dir_cellparent_set_p = NULL;
    }

    /*
     * DirectoryVersion
     */

    status = find_attribute((data_record_t *)hash_p->dir_p->dir_data,
                dns_AttributeName(DNS_DIRECTORYVERSION), &set_p, &idx);
    if (status == DNS_SUCCESS) {
      value_p = (DBSetMember_t *)DATA_DBSet(set_p);
      hash_p->dir_version_p = (VersionNumber_t *)value_p->dm_data;
    } else {
      hash_p->dir_version_p = (VersionNumber_t *)&default_version;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug4,
        " <refresh_fastpaths() -> AUT %x, CA %x\n",
        hash_p->dir_allupto_p, hash_p->dir_cellalias_set_p));

}


/* -------------------------------------------------------------------------- */

/*
 * db_dir_update
 *      This routine updates a directory record and releases
 *      its lock.  Must already have the lock.
 *      Release data buckets if possible if state is RS_dead
 *
 * Input:
 *      stream_p = pointer to stream descriptor
 *      tlog_p = pointer to transaction descriptor
 *
 */
dns_status_t
db_dir_update (db_stream_t  *stream_p,
               tlog_dsc_t   *tlog_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    dir_hash_entry_t    *hash_p = stream_p->db_dir_hash_p;
    int                 needed_blocks;
    dns_status_t        status = DNS_SUCCESS;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_dir_update(%s, %p)",
        deb_ascii_db_stream(streamBuf,stream_p),
        tlog_p));

    ASSERT_ACTIVE_STREAM(stream_p);

    /*
     * Since this is preceeded by a read_for_update()
     * the hash has already been done and the directory
     * must already be locked by this thread
     */
    if (!hash_p || (hash_p != stream_p->db_dir.data_hash_p) ||
        (hash_p->dir_update_access_owner != stream_p))
        {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug2,
            "db_dir_update"));
        return(DNS_NAMESERVERBUG);
        }

    if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

    {   /* Beginning of the 'protected code' */

        /* Get pointer to directory bucket */
        directory_bucket_t *dir_p = hash_p->dir_p;

        if (hash_p->dir_replica_state == RS_dead) {
            if (hash_p->dir_p) {
                dns_free((char *)hash_p->dir_p);
                hash_p->dir_p = NULL;
            }

            /*
             * Release btree memory
             * Since we have the hash entry locked, no one is logging at this.
             */
            if (hash_p->dir_entry_p) {
                db_btree_free(hash_p->dir_entry_p);
                hash_p->dir_entry_p = NULL;
            }

        }
        else {

            /* Determine space needed for new record, round up */
            needed_blocks = DIFF_bytes(dir_p->dir_data, dir_p)
                    + LEN_DBSet(stream_p->db_dir.data_p->dbr_data);
            needed_blocks = dns_roundup(needed_blocks);

            /* If don't have enough space, allocate a new block */
            if (dir_p->dir_blocks < needed_blocks)
            {
                directory_bucket_t *newdir_p;

                newdir_p = (directory_bucket_t *)dns_malloc_perm(
                    needed_blocks * BLOCK_SIZE);
                if (newdir_p)
                {
                    newdir_p->dir_type = dir_p->dir_type;
                    newdir_p->dir_blocks = needed_blocks;
                    dns_free((char *)dir_p);
                    hash_p->dir_p = newdir_p;
                    dir_p = newdir_p;
                }
                else
                {
                    status = DNS_NONSRESOURCES;
                }
            }

            if (status == DNS_SUCCESS)
            {
                /* Copy new record */
                COPY_DBSet(stream_p->db_dir.data_p->dbr_data, dir_p->dir_data);
                /*
                 * Refresh fast path to attributes for root directory only.
                 */
                if (hash_p->dir_root) {
                    refresh_fastpaths(hash_p);
                }
                hash_p->dir_update_access_owner = NULL;
                if ((thstatus = dthread_signal(&(hash_p->dir_update_accessable))) < 0)
                {
		  dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, (long)thstatus);
                }
            }
            stream_p->db_dir.data_hash_p = NULL;
            --hash_p->dir_users;
        }

    } /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    /* Record update in transaction log */
    if ((status == DNS_SUCCESS) && tlog_p)
    {
        db_tlog_update(stream_p->db_file_p, stream_p->db_function,
                (FullName_t *)stream_p->db_dir_hash_p->dir_name,
                (ObjUID_t *)stream_p->db_dir.data_uid,
                stream_p->db_dir.data_type, tlog_p);
    }

    return(status);
}

/* -------------------------------------------------------------------------- */

/*
 * db_dir_delete
 *      This routine deletes a directory as the result of
 *      an incomplete create_dir or add_replica call.  The directory
 *      is actually marked DEAD.  It is not removed because
 *      outstanding pointer to the hash entry may still exist.
 *
 * Input:
 *      stream_p = pointer to stream descriptor with directory locked
 */
void
db_dir_delete (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    Update_u            update;
    tlog_dsc_t          tlog;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_dir_delete(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    ASSERT_ACTIVE_STREAM(stream_p);

    /* Mark it dead for later cleanup */
    tlog.cnt = 1;
    tlog.buf_p = (byte_t *)update;
    (void)db_dir_set_state(stream_p, RS_dead, (Update_t *)tlog.buf_p, &tlog.len);
    /* Record update */
    (void)db_dir_update(stream_p, &tlog);
}
