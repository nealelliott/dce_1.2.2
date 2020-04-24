/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dbdef.h,v $
 * Revision 1.1.14.3  1996/02/18  23:33:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:12  marty]
 *
 * Revision 1.1.14.2  1995/12/08  15:15:23  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/jrr_1.2_mothra/1  1995/11/16  21:28 UTC  jrr
 * 	Fix for OT 12879 (database index corruption).  See peckham notes in
 * 	db_btree.c.
 * 
 * 	HP revision /main/HPDCE02/2  1995/07/27  03:56 UTC  peckham
 * 	Merge in fix for CHFts14444 - btree index maintenance.
 * 
 * 	HP revision /main/HPDCE02/peckham_14444/1  1995/07/21  20:11 UTC  peckham
 * 	entry_insert_ctx_t is now private to db_btree.c.
 * 	db_btree_*_entry() interfaces change.
 * 	[1995/12/08  14:42:44  root]
 * 
 * Revision 1.1.10.1  1994/10/25  16:17:32  proulx
 * 	CR#5915 - add file_back_data_p to file_info_t.
 * 	[1994/10/25  15:02:02  proulx]
 * 
 * Revision 1.1.7.8  1994/08/26  21:55:21  zee
 * 	Add fastpath for root dir DirectoryVersion.
 * 	[1994/08/26  21:47:00  zee]
 * 
 * Revision 1.1.7.7  1994/08/19  21:10:32  zee
 * 	Add new file_state for BadClock fixer.
 * 	[1994/08/19  18:06:45  zee]
 * 
 * Revision 1.1.7.6  1994/08/03  19:02:08  mccann
 * 	includes cleanup
 * 	[1994/08/02  14:00:45  mccann]
 * 
 * Revision 1.1.7.5  1994/08/01  15:07:27  zee
 * 	     Added file_back_done and file_ckpt_done to file_info_t.
 * 	[1994/07/29  21:02:24  zee]
 * 
 * Revision 1.1.7.4  1994/06/09  18:38:48  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:19  devsrc]
 * 
 * Revision 1.1.7.3  1994/05/06  16:04:10  zee
 * 	     HCell BL4 support: Add fastpath for CDS_ParentCellPointers attribute.
 * 	[1994/05/05  21:06:37  zee]
 * 
 * Revision 1.1.7.2  1994/03/22  20:00:05  griffin
 * 	HCell BL3 support: Add new attribute fastpath pointers, relative cell
 * 	naming support.
 * 	[1994/03/22  17:52:47  griffin]
 * 
 * Revision 1.1.7.1  1994/03/12  22:02:35  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:13:04  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:29:05  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:46:51  zeliff]
 * 
 * Revision 1.1.2.2  1992/07/17  17:23:58  peckham
 * 	CR#2681: Avoid checkpoint deadly embrace.
 * 	  26-JUN-1992 10:23  by Dave Griffin
 * 	    Add prototypes for db_btree_rebuild_index, ...
 * 	[1992/07/17  17:22:59  peckham]
 * 
 * Revision 1.1  1992/01/19  15:15:26  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _DBDEF_H
#define _DBDEF_H
/*
 * Module dbdef.h
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
 *
 * MODULE DESCRIPTION:
 *
 * Header file for CDS Database data structures and routines
 */
#include <dce/dns_record.h>
#include <threads_lib.h>
#include <tlog.h>
#include <server.h>

/* Database constants */

#define DATABASE_VERSION_MAJOR	4
#define DATABASE_VERSION_MINOR	0

#define BLOCK_SIZE 512
#define DATA_BUCKET_BLOCKS 16
#define INDEX_BUCKET_BLOCKS 8
#define MAX_BUCKET_BLOCKS DATA_BUCKET_BLOCKS
#define DIRECTORY_BUCKET_BLOCKS INDEX_BUCKET_BLOCKS
#define MAX_BUCKET_SIZE (BLOCK_SIZE * MAX_BUCKET_BLOCKS)
#define UID_HASH_LEN 101
#define NAME_HASH_LEN 101

/* Structure IDs */

#define DIRECTORY_BUCKET 1
#define INDEX_BUCKET 2
#define DATA_BUCKET 3
#define PSEUDO_BUCKET 4

/* Structure IDs (used for instrumentation only) */
#define LOCK_BLOCK 5
#define STREAM_BLOCK 6
#define CLEARINGHOUSE_BLOCK 7
#define HASH_BLOCK 8


/* The first block of the data file is reserved.  To make sure we reserve
 * enough space, the following 2 structures are defined.  Code should
 * only have to worry about the file_header structure and
 * use the defined labels.
 */

typedef struct file_hdr_used {		/* longword aligned */
    unsigned long    version;
    unsigned long    sequence;
    unsigned long    dir_count;           /* Count of directories */
    Timestamp_u	     last_cts;            /* Last CTS generated */
    Time_u	     nextback;            /* Next background schedule time */
    VersionNumber_t  db_version;          /* Database Version */
    ObjUID_u         nsuid;               /* NS ID for this clearinghouse */
    unsigned short   align_mask;          /* DBSet alignment mask */
    unsigned long    tlog_threshold;      /* Megabytes of TLog before ckpt. */
    unsigned long    ckpt_interval;       /* Checkpoint interval (hours) */
    unsigned long    ckpt_comp_level;     /* Compression level */
} file_hdr_used_t;

/*
 * This structure ensures the space required for the first block is reserverd
 */

typedef struct file_header {		/* longword aligned */
    union {
        bytes_u		hdr_max[BLOCK_SIZE];
	file_hdr_used_t	used;
    } hdr;
} file_header_t;

#define hdr_version             hdr.used.version
#define hdr_sequence            hdr.used.sequence
#define hdr_dir_count           hdr.used.dir_count
#define hdr_last_cts            hdr.used.last_cts
#define hdr_nextback            hdr.used.nextback
#define hdr_db_version          hdr.used.db_version
#define hdr_nsuid               hdr.used.nsuid
#define hdr_tlog_threshold      hdr.used.tlog_threshold
#define hdr_ckpt_interval       hdr.used.ckpt_interval
#define hdr_align_mask          hdr.used.align_mask
#define hdr_ckpt_comp_level     hdr.used.ckpt_comp_level


/*
 *             Directory Bucket
 */

/* Space left in bucket for data */
#define DIRECTORY_BUCKET_OVERHEAD (sizeof(int) + sizeof(unsigned int))
#define DIR_DATA_SIZE (BLOCK_SIZE*DIRECTORY_BUCKET_BLOCKS - \
	DIRECTORY_BUCKET_OVERHEAD)

typedef struct directory_bucket {	/* longword aligned */
    int			dir_type;	/* DIR_REC_TYPE or CLE_REC_TYPE */
    unsigned int	dir_blocks;	/* Length of bucket in 512 blocks */

    /*
     * The dir_data array contains the set of attribute sets
     * making up the directory record.  A copy of this array
     * is returned to callers reading the directory.  The array
     * is at least as large of dir_data_size.  It may be
     * larger if the directory requires more data.
     */
    bytes_u		dir_data[DIR_DATA_SIZE];
} directory_bucket_t;

#define deb_ascii_directory_bucket(b,p) \
    deb_ascii_rec(b,p,"NULL directory_bucket_t")
#define DEB_ASCII_LEN_directory_bucket DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_directory_bucket(b) DEB_ASCII_BUF_rec(b)
#define LOG_ASCII_BUF_directory_bucket(b) LOG_ASCII_BUF_rec(b)


/*
 *        Record Descriptor
 */

typedef struct rec_descriptor {		/* longword aligned */
    unsigned int	rec_offset;	/* Byte offset from beginning of bucket
					 * to record */
    unsigned int	rec_length;	/* Length of record */
    } rec_descriptor_t;

/*
 *         Header on all index and data buckets
 */

typedef struct node_header {		/* longword aligned */
    int			node_type;	/* Index or data bucket */
    unsigned int	node_blocks;	/* Blocks in bucket */
    /*
     * Buckets are linked horizontally so that they can be searched
     * sequentially.  A double linked list is used so that
     * data buckets can be replaced when they expand the 16 block
     * mimimum size
     */
    struct node_header	*node_next_p;
    struct node_header	*node_prev_p;

    unsigned int	node_space;	/* Free bytes in bucket */
    unsigned int	node_count;	/* Number of rec_descriptors */
    rec_descriptor_t	node_rec_dsc[1];
} node_header_t;
#define INDEX_node_rec_dsc(ptr,idx) (&ptr->node_rec_dsc[idx])
#define INDEX_node_rec(ptr,idx) \
	SKIP_bytes(ptr,ptr->node_rec_dsc[idx].rec_offset)

#define BINARY_SORT	0	/* Sort order for binary simple names */
#define ASCII_SORT	1	/* Sort order for ascii/quoted simple names */
#define TYPED_SORT	2	/* Sort order for X.500 typed names */

char *
deb_ascii_node_header (
    char		*,
    node_header_t	*);
#define DEB_ASCII_LEN_node_header DEB_ASCII_LEN_ptr
#define DEB_ASCII_BUF_node_header(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_node_header)
#define LOG_ASCII_BUF_node_header(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_node_header)

/*
 *      Key to lookup up an entry in a directory
 */

typedef struct entry_key {		/* NOT aligned */
    union {
	unsigned short	kd_length;	/* full length of key_name */
	node_header_t	*kd_down_p;	/* down pointer (address) for index */
    } key_kd;
#define	key_length key_kd.kd_length
#define key_down_p key_kd.kd_down_p
    byte_u		key_type;	/* Object, softlink or childpointer */
    byte_u		key_sort;	/* binary_sort or ascii_sort */
    SimpleName_u	key_name;
    /* Followed by SimpleNameMask when (sn_type == SN_normal|SN_quoted) */
    /* Followed by original SimpleName when (sn_type == SN_typed) */
    SimpleName_u	key_orgname;	/* space for original simplename */
} entry_key_t;
#define	LEN_entry_key(ptr) \
    DIFF_bytes(SKIP_SimpleName(((entry_key_t *)(ptr))->key_name), ptr)
#define	MOVE_entry_key(from,to) MOVE_bytes(from,to,LEN_entry_key(from))

char *
deb_ascii_entrySort (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_entrySort (DEB_ASCII_LEN_byte+6)
#define DEB_ASCII_BUF_entrySort(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_entrySort)
#define LOG_ASCII_BUF_entrySort(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_entrySort)

char *
deb_ascii_entry_key (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_entry_key (DEB_ASCII_LEN_ptr_TO+DEB_ASCII_LEN_EntryType+DEB_ASCII_LEN_entrySort+DEB_ASCII_LEN_SimpleName+2)
#define DEB_ASCII_BUF_entry_key(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_entry_key)
#define LOG_ASCII_BUF_entry_key(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_entry_key)

/*
 *       Data Bucket
 *
 * Format of a record in the data bucket.
 * After entry_key_t is DBSet_t.
 */

#define CASEMASK_SIZE ((SIMPLENAMEMAX+7)/8)
#define LEN_entry_data_record(ptr) DIFF_bytes(SKIP_entry_data_record(ptr), ptr)
#define SKIP_entry_data_record(ptr) SKIP_DBSet(DATA_entry_data_record(ptr))
#define	DATA_entry_data_record(ptr) \
    SKIP_bytes(ptr, ((entry_key_t *)(ptr))->key_length)



/*
 * Deleted record holder when new record with same name is created
 */

typedef struct delete_holder {		/* NOT aligned */
    Timestamp_u		del_uid;	/* old entry uid */
    Timestamp_u		del_ts;  	/* timestamp of delete */
} delete_holder_t;
#define LEN_delete_holder(ptr) LEN_bytes(ptr,sizeof(delete_holder_t))
#define SKIP_delete_holder(ptr) SKIP_bytes((ptr),LEN_delete_holder(ptr))
#define COPY_delete_holder(from,to) \
    COPY_bytes((from),(to),LEN_delete_holder(from))
#define MOVE_delete_holder(from,to) \
    MOVE_bytes((from),(to),LEN_delete_holder(from))


/*
 * Lock Block
 * This structure describes a record currently locked
 * and which stream locked it.
 * It is protected by the hash_p->dir_entry_locks_lock mutex.
 */

typedef struct lock_block {		/* longword aligned */
    list_header_t	lock;		/* List of records current locked */
    dthread_cond_t	lock_ready;	/* Transfer ownership of lock */
    struct db_stream	*lock_owner;	/* Who owns this lock */
    unsigned short	lock_waiters;	/* Count of waiters on this lock */
    Timestamp_u		lock_cts;	/* Timestamp of entry locked */
} lock_block_t;


/*
 * Directory Hash Entry
 * It is created and hung off of the structure pointed to by dir_file_info_p.
 * The file_info_t has two pointers to it through file_(uid|name)_table.
 * The unique directory_bucket_t is from the checkpoint file.
 * The node_header_t is the root index of the b-tree.
 * Data buckets at the leaves of the b-tree are locked by lock_block_t,
 * and a copy of that data is pointed to by data_p.
 */
typedef enum {
    dir_init = 0,
    dir_on,
    dir_rundown
} dir_state_t;

typedef struct dir_hash_entry {		/* longword aligned */
    /*
     * Pointer to other dir_hash_entries that hashed to
     * the same index value. List must be searched sequentially
     * to locate the correct directory.
     */
    struct dir_hash_entry *dir_uid_collision_p;
    struct dir_hash_entry *dir_name_collision_p;
    unsigned short	dir_uid_index;	    /* uid hash index */
    unsigned short	dir_name_index;	    /* name hash index */

    dthread_mutex_t	dir_mutex;	    /* structure mutex */
    dir_state_t		dir_state;
    unsigned int	dir_users;	    /* number of users of this struct */
    dthread_cond_t	dir_state_changed;  /* if the above changes */
    struct file_info	*dir_file_info_p;   /* Points to file info */

    directory_bucket_t	*dir_p;		    /* Memory resident directory */
    int			dir_type;	    /* ET_directory or ET_clearinghouse */
    dthread_cond_t	dir_update_accessable;
    struct db_stream	*dir_update_access_owner; /* when locked for update */

    node_header_t	*dir_entry_p;	        /* Root bucket of btree */
    dthread_mutex_t	dir_entry_locks_lock;   /* Entry locks list lock */
    list_header_t	dir_entry_locks;        /* List of entry locks held */

    dthread_mutex_t	dir_skulk_lock;
    int			dir_skulk_in_progress;
    dthread_cond_t	dir_skulk_completed;
    unsigned int	dir_skulk_last_status;	 /* Remember failure of skulk */
    ErrSet_t		*dir_skulkinfo_p;	 /* Skulk stage/fail info  */

    unsigned int	dir_prop_status;	/* Reset after any propagate */
    Timestamp_u		dir_prop_time;		/* Time of last propagate */

    /* 
     * This is fast-path information for root directories
     * only.  It is maintained by directory updates and is meant
     * to be READ ONLY, but dir_mutex is assumed to be locked before
     * dereferencing.
     */

    char		dir_root;		/* non-zero if root directory */
    Timestamp_t        *dir_allupto_p;          /* AllUpTo attribute value */
    DBSet_t            *dir_cellalias_set_p;    /* Cell Aliases set */
    DBSet_t            *dir_cellparent_set_p;   /* Parent Cell pointers set */
    VersionNumber_t    *dir_version_p;	        /* DirectoryVersion of root */

    /*
     * General attributes:
     *	DNS$CTS (Single R) ->		(DBSet) dir_uid
     *	DNS$UTS (Single R) ->		(DBSet)
     *	DNS$ACS (Set RW) ->		(DBSet)
     *
     * Replica attributes:
     *	DNS$ReplicaState (Single) ->	(DBSet) dir_replica_state
     *	DNS$ReplicaType (Single) ->	(DBSet) dir_replica_type
     *	DNS$LastSkulk (Single) ->	(DBSet)
     *	DNS$LastUpdate (Single) ->	dir_lastupdate
     *	DNS$RingPointer (Single) ->	(DBSet)
     *	DNS$Epoch (Single) ->		(DBSet)
     *	DNS$ReplicaVersion (Single) ->	(DBSet)
     */

    /*
     * The following fields are single-valued Replica attributes
     * commonly accessed by transactions. Instead of reading
     * the directory record just to access these fields,
     * they shadowed here for easy retrival.
     * This is an optimization.
     */
    int			dir_replica_state;	/* read-write */
    int			dir_replica_type;	/* read-write */

    /*
     * Every update of an object or softlink must update the
     * directory attribute DNS$LastUpdate.  Instead of reading and
     * locking the directory record all the time, this field
     * is kept and updated here.  Then when a directory is read
     * the directory record is updated.  This field can always
     * be recreated from the transaction log, so a record of
     * of its latest value exists in the log.
     */
    Timestamp_u		dir_lastupdate;		/* read-write */

    ObjUID_u		dir_uid;		/* read-only */
    FullName_u		dir_name;		/* read-write */
    FullName_u          dir_relative_name;      /* read-write ? */
} dir_hash_entry_t;

char *
deb_ascii_dir_hash_entry (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_dir_hash_entry (DEB_ASCII_LEN_ptr_TO+DEB_ASCII_LEN_FullName)
#define DEB_ASCII_BUF_dir_hash_entry(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_dir_hash_entry)
#define LOG_ASCII_BUF_dir_hash_entry(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_dir_hash_entry)

#define NO_LOCK		 0		    /*  Record not locked */
#define UPDATE_LOCK	 1<<1		    /*  Update lock held */


/*
 * Data Record
 *
 * This structure is the same as the last two fields of directory_bucket
 */

typedef struct data_record {		/* longword aligned */
    bytes_u		dbr_data[1];	/* Array of data, variable length */
} data_record_t;
#define DR_OVERHEAD 0



/*
 * db_data
 */

typedef struct db_data {		/* longword aligned */
    int                 data_type;	/* type of record described here */
    union {
	lock_block_t	*entry_lock_p;	/* Lock block if update_lock held */
	dir_hash_entry_t *dir_hash_p;	/* hash entry if directory locked */
    } data_lock;
    unsigned int        data_buf_len;	/* Length of data_p */
    data_record_t	*data_p;
    Timestamp_u         data_cts;	/* DNS$CTS of record for easy access */
    ObjUID_u            data_uid;	/* DNS_ObjUID of record for easy access */
} db_data_t;

#define	data_lock_p data_lock.entry_lock_p
#define	data_hash_p data_lock.dir_hash_p

#define deb_ascii_db_data(b,p) deb_ascii_rec(b,p,"NULL db_data_t")
#define DEB_ASCII_LEN_db_data DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_db_data(b) DEB_ASCII_BUF_rec(b)
#define LOG_ASCII_BUF_db_data(b) LOG_ASCII_BUF_rec(b)

/*
 * Database stream (db_stream)
 *
 * Stream descriptor returned by open file or get_stream
 *
 * Accessed exclusively by one thread, so doesn't need mutex.
 */

typedef struct db_stream {		/* longword aligned */
    /* When a stream is free, it is linked in a list */
    struct db_stream	*db_next_p;

    /* Only one thread owns the stream at a time */
    dthread_t		db_owner;

    /* Clearinghouse this stream belongs to */
    struct file_info	*db_file_p;

    /* Transaction initiating request, used in transaction log */
    int			db_function;

    /* Data for directory or pseudo directory read */
    db_data_t		db_dir;

    /* Hash entry for this directory or NULL */
    dir_hash_entry_t	*db_dir_hash_p;

    /* Data for object, softlink or childpointer read */
    db_data_t		db_entry;

    /* unnormalized copy of last SimpleName in db_key */
    SimpleName_u	db_entry_original_name;
    /*
     * When looking for a directory the key can contain the
     * directory uid and nullsimplename which results in a uid
     * hash of the directory full name from the root which
     * results in a name hash.  It is ignored if the hash entry
     * is non-null.
     *
     * When looking for an object, softlink of childpointer
     * the directory is located first.  If the hash is non-null
     * the directory is known.  Otherwise the last simplename
     * of the key is removed and the directory is looked up.
     * The key is then replaced with the directory uid and
     * the entry simplename and the entry name is searched for
     * in the btree
     */
    FullName_u		db_key;
} db_stream_t;

/* Use this macro to assure stream use is legal */
#define ASSERT_ACTIVE_STREAM(stream) \
    db_assert_active_stream(stream, __FILE__, __LINE__)

char *
deb_ascii_db_stream (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_db_stream (DEB_ASCII_LEN_ptr_TO+DEB_ASCII_LEN_FullName)
#define DEB_ASCII_BUF_db_stream(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_db_stream)
#define LOG_ASCII_BUF_db_stream(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_db_stream)

/*
 *    Clearinghouse Database Structures and Constants
 */


/*
 * Memory resident description of file
 * There are one of these per clearinghouse, representing
 * the clearinghouse checkpoint and transaction files.
 */

typedef enum {
    file_init = 0,		/* creating, no users */
    file_recover,		/* reading tlog, no users */
    file_loaded,		/* loaded, no users */
    file_open,			/* loaded, users allowed */
    file_ckpt,			/* writing chkp, no writers */
    file_ckpt_exclusive,	/* writing chkp, no users */
    file_rundown,		/* unloading users */
    file_badclk_repair		/* BadClock repair, no users */
} file_state_t;

typedef enum {
    file_prop_Off = 0,		/* inactive */
    file_prop_On,		/* thread running */
    file_prop_Rundown,		/* waiting for thread to stop */
    file_prop_Stopped,		/* propagation stopped */
    file_prop_Cancel		/* waiting for thread cancel to complete */
} file_prop_state_t;

#define OS_DATA_SIZE 600		/* Size of OS file data area */

#define OP_CARDINALITY 36

typedef struct current_op_info {
    dthread_cond_t   currop_sop_completed;  /* SpecialOp completed bell */
    int              currop_count[OP_CARDINALITY]; /* Stats of each type */
    int              currop_current;        /* Count of ops running */
    int              currop_special;        /* Count of special ops running */
} current_op_info_t;


typedef struct file_info {		    /* longword aligned */
    LIST_ELEMENT(struct file_info, file_queue);
					    /* List of other clearinghouses */
    dthread_mutex_t  file_mutex;            /* General structure mutex */
    file_state_t     file_state;            /* Database state */
    unsigned int     file_users;	    /* Number of users of this struct */
    int		     file_write_count;      /* Count of writers to file */
    dthread_cond_t   file_state_changed;    /* If the above changes */
    current_op_info_t file_op_info;         /* Operation information */

    nsgbl_ch_t      *file_ch_p;             /* Mgmt data for clearinghouse */
    /*
     * General attributes:
     *	DNS$CTS (Single R) ->		(DBSet) &file_ch_p->ch_uid
     *	DNS$UTS (Single R) ->		(DBSet)
     *	DNS$ACS (Set RW) ->		(DBSet)
     *
     * Clearinghouse attributes:
     *	DNS$Address (Set RWD) ->	(DBSet)
     *	DNS$CHName (Set) ->		(DBSet) &file_ch_p->ch_name
     *	DNS$CHCTS (Single) ->		(DBSet) &file_ch_p->ch_uid
     *	DNS$CHLastAddress (Single) ->	(DBSet)
     *	DNS$CHUpPointers (Set) ->	(DBSet)
     *	DNS$CHState (Single) ->		(DBSet)
     *	DNS$CHDirectories (Set) ->	(DBSet)
     *	DNS$NSNickName (Single) ->	(DBSet) &file_ch_p->ch_nsnickname
     *	DNS$NSCTS (Single) ->		(DBSet) &file_ch_p->ch_nsuid
     */

    db_stream_t     *file_free_stream_p; /* Cached free streams */

    /* Directory UID Hash array */
    dir_hash_entry_t *file_uid_table[UID_HASH_LEN];

    /* Directory name Hash array */
    dir_hash_entry_t *file_name_table[NAME_HASH_LEN];

    short            file_root_dir_present; /* Do we store a root replica? */
    dir_hash_entry_t *file_root_dir_p;

    file_prop_state_t file_prop_state;
    LIST_HEADER(struct prop_queue_element, file_prop_wait);
					    /* List of directories to propagate */
    db_stream_t	    *file_prop_stream_p;    /* Pre-allocated stream to use on propagates */
    dthread_t	     file_propagator;       /* propagate thread */

    dthread_t	     file_back_activator;   /* background activator thread */
    void            *file_back_data_p;      /* background private data */
    short	     file_back_force;  	    /* force background thread to run */
    short            file_back_inhibit;     /* inhibit background from running */
    short            file_back_once;        /* for guaranteed background pass */
    dthread_t	     file_back_thread;      /* background thread */
    dthread_cond_t   file_back_done;        /* when the background is done */

    short	     file_ckpt_needed;      /* checkpointed needed */
    short            file_ckpt_force;       /* Force checkpoint thresholds */
    dthread_t	     file_ckpt_thread;      /* active checkpoint thread */
    time_local_t     file_ckpt_last_start;  /* last checkpoint start time */
    time_local_t     file_ckpt_last_finish; /* last checkpoint finish time */
    dthread_cond_t   file_ckpt_done;        /* when checkpointing is done */
    file_header_t    file_ckpt_hdr;         /* Copy of 1st blk of chkpt file */
    bytes_u	     file_ckpt_block[OS_DATA_SIZE]; /* OS specific data for chkpt */
    short            file_ckpt_comp_level;  /* Compression level */
    int              file_ckpt_dbytes_in;   /* Data bytes into compression */
    int              file_ckpt_fbytes_in;   /* Free bytes into compression */
    int              file_ckpt_buckets_in;  /* Buckets into compression */
    int              file_ckpt_dbytes_out;  /* Data bytes out of compression */
    int              file_ckpt_fbytes_out;  /* Free bytes out of compression */
    int              file_ckpt_buckets_out; /* Buckets out of compression */
    int              file_ckpt_node_count;  /* Nodes compressed */

    dthread_mutex_t  file_tlog_mutex;       /* mutex for following tlog data */
    int              file_tlog_state;       /* If the TLog file open or closed */
    byte_t	    *file_tlog_buf_p;       /* Buffer for transaction writes */
    unsigned int     file_tlog_buf_len;     /* Length of tlog_buf_p */
    unsigned int     file_tlog_next_block;  /* Next block to write in log */
    bytes_u	     file_tlog_block[OS_DATA_SIZE]; /* Os specific data for log */
} file_info_t;

char *
deb_ascii_file_info (
    char *const       ,
    const void *const );
#define DEB_ASCII_LEN_file_info (DEB_ASCII_LEN_ptr_TO+DEB_ASCII_LEN_ObjUID)
#define DEB_ASCII_BUF_file_info(nam) \
	DEB_ASCII_BUF(nam,DEB_ASCII_LEN_file_info)
#define LOG_ASCII_BUF_file_info(nam) \
	LOG_ASCII_BUF(nam,DEB_ASCII_LEN_file_info)

#define TLOG_FILE_OPEN 1
#define TLOG_FILE_CLOSED 0


/*
 * Database compression routines context id
 */

typedef void *compression_ctx_t; /* Database compression context id */

/*
 * Database conversion routines context id
 */

typedef void *conversion_ctx_t; /* Database conversion context id */

/*
 * definitions being phased out
 */
#define DB_OPEN		file_open
#define DB_LOADED	file_loaded
#define	data_block	file_ckpt_block


/*
 * db_btree.c
 */
void
db_btree_copy_keys (
    node_header_t	*,
    node_header_t	*,
    unsigned int	,
    unsigned int	,
    unsigned int	);

dns_status_t
db_btree_build (
    file_info_t		*,
    dir_hash_entry_t    *,
    unsigned int	*,
    conversion_ctx_t     );

dns_status_t
db_btree_insert_entry (
    db_stream_t		*,
    dir_hash_entry_t    *,
    entry_key_t		*);

dns_status_t
db_btree_remove_entry (
    db_stream_t		*,
    dir_hash_entry_t    *,
    entry_key_t		*);

dns_status_t
db_btree_replace_entry (
    db_stream_t		*,
    dir_hash_entry_t    *,
    entry_key_t		*,
    Timestamp_t         *);

dns_status_t
db_btree_locate_entry_bkt (
    db_stream_t		*,
    node_header_t	*,
    dir_hash_entry_t    *,
    node_header_t	**,
    unsigned int	*);

node_header_t *
db_btree_get_next_level (
    node_header_t	*);

node_header_t *
db_btree_get_first_data_bucket (
    node_header_t	*);

void
db_btree_free (
    node_header_t	*);

dns_status_t
db_btree_rebuild_index (
    dir_hash_entry_t    *);


/*
 * db_checkpoint.c
 */
dns_status_t
db_checkpoint (
    file_info_t		*);

void
db_checkpoint_schedule (
    file_info_t		*);

void
db_checkpoint_kill (
    file_info_t		*);

int
db_checkpoint_thresholds_check (
    file_info_t		*);

dns_status_t
db_checkpoint_load (
    file_info_t		*);

/*
 * db_cleanup.c
 */
void
db_cleanup_record (
    db_data_t		*,
    Timestamp_t		*);

/*
 * db_common.c
 */
dns_status_t
db_get_stream (
    file_info_t		*,
    db_stream_t		**,
    int			);

dns_status_t
db_get_stream_ignore (
    file_info_t		*,
    db_stream_t		**,
    int			);

void
db_free_stream (
    db_stream_t		*);

dns_status_t
db_open_stream (
    ObjUID_t		*,
    db_stream_t		**,
    int			);

dns_status_t
db_close_stream (
    db_stream_t		*);

dns_status_t
db_modify_stream_function (
    db_stream_t		*,
    int			,
    int			*);

void
db_assert_active_stream  (
    db_stream_t *,
    char        *,
    int          );

void
db_insert_alt_chcts (
    file_info_t		*,
    Timestamp_t		*);

file_info_t *
db_find_file (
    ObjUID_t		*);

dns_status_t
db_disable (
    ObjUID_t		*);

void
db_exit (
    nsgbl_ns_t         *);

void
db_init_ch (
    dir_hash_entry_t	*,
    nsgbl_ch_t		*);

file_info_t *
file_info_malloc (
    nsgbl_ch_t		*);

void
file_info_free (
    file_info_t		*);

dns_status_t
db_setup (
    nsgbl_ch_t		*);

dns_status_t
db_enable (
    nsgbl_ch_t		*,
    int                 );

dns_status_t
fix_ch_tower (
    db_stream_t		*,
    Set_t		*);

dns_status_t
fix_ch_obj_addr (
    db_stream_t		*,
    unsigned char	*);

dns_status_t
write_ch_obj_tower (
    db_stream_t		*,
    Set_t		*);

dns_status_t
write_chlastaddress (
    db_stream_t		*,
    Set_t		*);

dns_status_t
db_create (
    nsgbl_ch_t		*,
    user_descriptor_t	*,
    version_t		*);

void
db_delete (
    nsgbl_ns_t          *,
    ObjUID_t		*);

void
db_clear (
    ObjUID_t		*);

dns_status_t
db_begin_operation (
    user_descriptor_t   *);

dns_status_t
db_end_operation (
    user_descriptor_t   *);


/*
 * db_compression.c
 */

dns_status_t
db_compress_init (
    file_info_t       *,
    compression_ctx_t *);

dns_status_t
db_compress_btree_start (
    compression_ctx_t  ,
    unsigned int       );

dns_status_t
db_compress_btree_write (
    compression_ctx_t  ,
    node_header_t      *);

dns_status_t
db_compress_btree_end (
    compression_ctx_t  ,
    unsigned int      *);

dns_status_t
db_compress_finish (
    compression_ctx_t  );

/*
 * db_conversion.c
 */

dns_status_t
db_convert_init (
    unsigned int      ,
    unsigned int      ,
    conversion_ctx_t *);

dns_status_t
db_convert_finish (
    conversion_ctx_t );

dns_status_t
db_convert_directory_bucket (
    conversion_ctx_t     ,
    directory_bucket_t  *,
    directory_bucket_t **);

dns_status_t
db_convert_data_bucket (
    conversion_ctx_t ,
    node_header_t   *,
    node_header_t  **);

/*
 * db_diags.c
 */

int
db_diag_dump_file_info (
    int           ,
    file_info_t   *,
    int           (*)(char	*,
                      void	*),
    void          *);

int
db_diag_dump_file_info_list (
    int           ,
    int           (*)(char	*,
                      void	*),
    void          *);

int
db_diag_dump_dir_hash_entry (
    int              ,
    dir_hash_entry_t *,
    int              (*)(char	*,
                         void	*),
    void             *);

int
db_diag_dump_dir_hash_table (
    int          ,
    file_info_t  *,
    int          (*)(char	*,
                     void	*),
    void         *);

int
db_diag_dump_nsgbl_ns (
    int               ,
    nsgbl_ns_t       *,
    int              (*)(char	*,
                         void	*),
    void             *);

int
db_diag_dump_nsgbl_ch (
    int              ,
    nsgbl_ch_t       *,
    int              (*)(char	*,
                         void	*),
    void             *);

int
db_diag_dump_stream (
    int              ,
    db_stream_t      *,
    int              (*)(char	*,
                         void	*),
    void             *);

int
db_diag_set_force_back (
    file_info_t *,
    int          );

int
db_diag_set_force_ckpt (
    file_info_t *,
    int          );


int
db_diag_set_inhibit_back (
    file_info_t *,
    int          );

int
db_diag_set_ckpt_comp_level (
    file_info_t *,
    int          );



/*
 * db_directory.c
 */

void
db_dir_release_lock (
    db_stream_t		*);

void
db_dir_release (
    db_stream_t		*);

dns_status_t
db_dir_get_replica_state (
    dir_hash_entry_t	*);

dns_status_t
db_dir_get_replica_type (
    dir_hash_entry_t	*);

void
db_dir_get_version (
    db_stream_t		*,
    version_t		*);

void
db_dir_set_lastupdate (
    dir_hash_entry_t	*,
    Timestamp_t		*);

dns_status_t
db_dir_set_state (
    db_stream_t		*,
    int			 ,
    Update_t		*,
    int			*);

dns_status_t
db_dir_set_type (
    db_stream_t		*,
    int			 ,
    Update_t		*,
    int			*);

dns_status_t
db_dir_build_memory_record (
    file_info_t		   *,
    unsigned int	   *,
    dir_hash_entry_t	  **,
    conversion_ctx_t        );

dns_status_t
db_dir_create (
    db_stream_t		*);

dns_status_t
db_dir_create_recover (
    db_stream_t		*);

dns_status_t
db_dir_read (
    db_stream_t		*);

dns_status_t
db_dir_read_for_update (
    db_stream_t		*);

dns_status_t
db_pdir_read (
    db_stream_t		*);

dns_status_t
db_pdir_read_for_update (
    db_stream_t		*);

dns_status_t
db_dir_update (
    db_stream_t		*,
    struct tlog_dsc	*);

void
db_dir_delete (
    db_stream_t		*);

/*
 * db_entry.c
 */

dns_status_t
build_record (
    db_stream_t		*,
    entry_key_t		*);

dns_status_t
db_entry_create (
    db_stream_t		*,
    SimpleName_t	*,
    int			);

dns_status_t
db_entry_create_recover (
    db_stream_t		*,
    FullName_t		*,
    int			,
    ObjUID_t		*);

dns_status_t
db_entry_read (
    db_stream_t		*);

dns_status_t
db_entry_read_for_update (
    db_stream_t		*);

dns_status_t
db_entry_read_next (
    db_stream_t		*);

dns_status_t
db_entry_read_next_for_update (
    db_stream_t		*);

dns_status_t
db_entry_delete (
    db_stream_t		*);

dns_status_t
db_entry_update (
    db_stream_t		*,
    struct tlog_dsc	*);

void
db_entry_release (
    db_stream_t		*);

dns_status_t
db_entry_read_next_bucket (
    db_stream_t		*,
    node_header_t	**);

dns_status_t
db_entry_build_record (
    db_stream_t		*,
    int			,
    node_header_t	*);

void
db_entry_name_cased (
    db_stream_t		*,
    SimpleName_t	*);

/*
 * db_hash.c
 */

dns_status_t
db_hash_free (
    dir_hash_entry_t	*,
    file_info_t		*);

unsigned int
db_find_name_hash_index (
    FullName_t		*);

void
db_hash_new_name (
    file_info_t		*,
    dir_hash_entry_t	*,
    FullName_t		*);

dir_hash_entry_t *
db_hash_name (
    file_info_t		*,
    FullName_t		*);

unsigned int
db_find_uid_hash_index (
    ObjUID_t		*);

dir_hash_entry_t *
db_hash_uid (
    file_info_t		*,
    ObjUID_t		*);

dns_status_t
db_hash_insert (
    file_info_t		*,
    directory_bucket_t	*,
    unsigned int	,
    dir_hash_entry_t	**);

dir_hash_entry_t *
db_hash_locate (
    file_info_t		*,
    FullName_t		*);

dir_hash_entry_t *
db_hash_uid_ignore_state (
    file_info_t		*,
    ObjUID_t		*);

/*
 * db_pseudo.c
 */

dns_status_t
db_pseudo_create (
    file_info_t		*,
    user_descriptor_t	*,
    version_t		*);

dns_status_t
db_pseudo_read (
    db_stream_t		*);

dns_status_t
db_pseudo_read_for_update (
    db_stream_t		*);

dns_status_t
db_pseudo_update (
    db_stream_t		*,
    struct tlog_dsc	*);

dns_status_t
db_pseudo_chdirectory (
    file_info_t		*,
    ObjUID_t		*,
    int			);

/*
 * db_tlog.c
 */

unsigned int
db_get_checksum (
    unsigned char	*,
    unsigned int	);

dns_status_t
db_tlog_new (
    file_info_t		*);

dns_status_t
db_tlog_open (
    file_info_t		*);

void
db_tlog_create (
    file_info_t		*,
    int			,
    int			,
    FullName_t		*,
    ObjUID_t		*,
    int			,
    unsigned char	*);

dns_status_t
db_tlog_close (
    file_info_t		*);

dns_status_t
db_tlog_delete (
    file_info_t		*);

void
db_tlog_update (
    file_info_t		*,
    int			,
    FullName_t		*,
    ObjUID_t		*,
    int			,
    struct tlog_dsc	*);

dns_status_t
db_tlog_recover (
    file_info_t		*,
    unsigned int	*);

/*
 * db_unix.c
 * db_vms.c
 */

dns_status_t
sys_write (
    byte_t		*,
    unsigned char	*,
    unsigned int	,
    unsigned int	);

dns_status_t
sys_safe_write (
    byte_t		*,
    unsigned char	*,
    unsigned int	,
    unsigned int	);

dns_status_t
sys_read (
    byte_t		*,
    unsigned char	*,
    unsigned int	,
    int			);

dns_status_t
sys_close (
    byte_t		*);

dns_status_t
sys_delete (
    byte_t		*);

dns_status_t
update_version (
    char		*,
    unsigned int	);

dns_status_t
sys_create_checkpoint (
    char		*,
    byte_t		*,
    unsigned int	);

dns_status_t
sys_delete_version (
    char		*);

dns_status_t
sys_delete_checkpoint (
    char		*);

dns_status_t
sys_create_tlog (
    char		*,
    byte_t		*,
    unsigned int	);

dns_status_t
sys_open_checkpoint (
    char		*,
    byte_t		*);

dns_status_t
sys_open_tlog (
    char		*,
    byte_t		*);

dns_status_t
sys_extend (
    byte_t		*);

void
sys_purge (
    char		*,
    unsigned int	);

int
sys_filesize (
    byte_t		*);

dns_status_t
sys_find_versionfile (
    char		*);

#endif  /* #ifndef _DBDEF_H */
