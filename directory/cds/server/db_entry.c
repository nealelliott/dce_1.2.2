/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_entry.c,v $
 * Revision 1.1.7.2  1996/02/18  19:35:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:25  marty]
 *
 * Revision 1.1.7.1  1995/12/08  15:30:07  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/jrr_1.2_mothra/1  1995/11/16  21:28 UTC  jrr
 * 	Fix for OT 12879 (database index corruption).  See peckham notes in
 * 	db_btree.c.
 * 
 * 	HP revision /main/HPDCE02/1  1995/07/27  04:05 UTC  peckham
 * 	Fix for CHFts14444 - new calling sequences, debug display fix
 * 
 * 	HP revision /main/peckham_14444/2  1995/07/22  00:00 UTC  peckham
 * 	Fix deb_ascii_entry_key() display for terminal entry.
 * 
 * 	HP revision /main/peckham_14444/1  1995/07/21  20:09 UTC  peckham
 * 	db_btree_*_entry() interfaces no longer use entry_insert_ctx_t.
 * 	[1995/12/08  14:49:02  root]
 * 
 * Revision 1.1.4.5  1994/08/03  19:04:01  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:21:51  mccann]
 * 
 * Revision 1.1.4.4  1994/06/30  19:18:13  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:48:25  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:43:14  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:43  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/14  14:51:11  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:19:42  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:11:01  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:41:26  peckham]
 * 
 * Revision 1.1.2.3  1992/12/30  14:21:03  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:21:16  zeliff]
 * 
 * Revision 1.1.2.2  1992/11/13  15:49:21  peckham
 * 	OT#5448: cdsd hangs in (apparent) infinite loop.
 * 	We have had ongoing work to examine how nodes are handled
 * 	which become empty. The case observed has been fixed in our
 * 	DECdns code. What happens is that when a zero-entry node
 * 	is in the list, db_entry_read_next_bucket() goes to an entry,
 * 	advances to the NULL bucket, and does not find a next entry.
 * 	Back to the loop, but at the same place again. The trick is
 * 	to skip over NULL buckets when advancing to the next entry.
 * 
 * 	I took the oportunity to merge several other fixes from
 * 	our version of this module:
 * 
 * 	  6-JUL-1992 09:51  by Dave Griffin
 * 	   The recovery playback stream does not log delete_entries caused by
 * 	   do_updates replacing old entries.  This manifests itself in the
 * 	   playback as duplicate records.  When the db_entry_create_recover
 * 	   code encounters a duplicate entry (EntryExists), then it converts
 * 	   the insert to a replace transaction.
 * 
 * 	 17-JUL-1992 10:29  by Dave Griffin
 * 	   Modified db_entry_read_next_bucket to skip over empty buckets.
 * 
 * 	 21-JUL-1992 20:04  by Dave Griffin
 * 	   db_entry_create_recover has to zero out insert context allupto,
 * 	   or it'll cause purge_data_bucket to go bonkers later on.
 * 	[1992/11/13  15:47:55  peckham]
 * 
 * Revision 1.1  1992/01/19  15:25:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module db_entry.c
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
#include <dbdef.h>
#include <find.h>
#include <names.h>
#include <tlog.h>
#include <dnsnormal.h>
#include <string.h>

/*
 * Local prototypes
 */

static dns_status_t
entry_read (
    db_stream_t		*,
    int                  );

static dns_status_t
entry_read_next (
    db_stream_t		*,
    int                  );

static void
build_return_data (
    db_stream_t		*,
    entry_key_t		*);

static dns_status_t
build_entry_key (
    entry_key_t		*,
    unsigned int	,
    SimpleName_t	*);

static dns_status_t
process_lock (
    db_stream_t		*);

static void
release_lock (
    db_stream_t		*);

/*
 * build_return_data
 *	Return the entry CTS and UID, casemask and name
 *
 * Input:
 *	stream_p 	pointer to strema descriptor
 *	entry_p 	pointer to entry data record
 *
 */
static void 
build_return_data (db_stream_t  *stream_p,
                   entry_key_t  *entry_p)
{
    DBSet_t		*set_p;
    int			idx;

    ASSERT_ACTIVE_STREAM(stream_p);

    if (find_attribute(stream_p->db_entry.data_p,
	    dns_AttributeName(DNS_CTS), &set_p, &idx) == DNS_SUCCESS)
	{
	/*
	 * DNS_CTS is always a single valued set with no history
	 */
	DBSetMember_t *member_p = (DBSetMember_t *)DATA_DBSet(set_p);

        COPY_Timestamp(member_p->dm_data, stream_p->db_entry.data_cts);
	}
    else
	COPY_Timestamp(NullTimestamp, stream_p->db_entry.data_cts);

    if (find_attribute(stream_p->db_entry.data_p,
	    dns_AttributeName(DNS_OBJECTUID), &set_p, &idx) == DNS_SUCCESS)
	{
        DBSetMember_t *member_p = find_last_value(set_p);
	if (member_p)
	  COPY_ObjUID(member_p->dm_data, stream_p->db_entry.data_uid);
	else
	  ZERO_ObjUID(stream_p->db_entry.data_uid);
	}
    else
	ZERO_ObjUID(stream_p->db_entry.data_uid);
    /* X.500 return original name if name is typed */
    switch (EXT8(((SimpleName_t *)entry_p->key_name)->sn_flag)) {
    default:
	COPY_SimpleName(entry_p->key_name, stream_p->db_entry_original_name);
	break;
    case SN_typed:
	COPY_SimpleName(SKIP_SimpleName(entry_p->key_name),
		stream_p->db_entry_original_name);
	break;
    case SN_normal:
    case SN_quoted:
	names_simple_casemask((SimpleName_t *)entry_p->key_name, NULL,
			(SimpleName_t *)stream_p->db_entry_original_name);
	break;
    }

    /* Set up directory uid as name root */
    names_new((ObjUID_t *)stream_p->db_dir_hash_p->dir_uid,
	(SimpleName_t *)entry_p->key_name, (FullName_t *)stream_p->db_key);
}

/*
 * release_lock
 *	This routine starts up the next waiting thread
 *
 * Input:
 *	stream_p = owner of lock
 */
static void 
release_lock (db_stream_t *stream_p)
{
    dir_hash_entry_t	*const hash_p = stream_p->db_dir_hash_p;
    lock_block_t	*const lock_p = stream_p->db_entry.data_lock_p;
    int thstatus;


    ASSERT_ACTIVE_STREAM(stream_p);

    if (!lock_p) return;

    if ((thstatus = dthread_lock(&(hash_p->dir_entry_locks_lock))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{

	    stream_p->db_entry.data_lock_p = NULL;
	    lock_p->lock_owner = NULL;

	    if (0 < lock_p->lock_waiters)
	    {
		if ((thstatus = dthread_signal(&(lock_p->lock_ready))) < 0)
		{
		  dce_svc_printf(CDS_S_PTHREADCONDSIGNAL_A_MSG, 
				 (long)thstatus);
		}

		if ((thstatus = dthread_unlock(&(hash_p->dir_entry_locks_lock))) < 0)
		{
		  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, 
				 (long)thstatus);
		}
		return;
	    }

	    /* Queue is empty */
	    remove_list(&lock_p->lock);
	}
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(hash_p->dir_entry_locks_lock))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }
    
    if ((thstatus = dthread_delete_condition(&lock_p->lock_ready)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
    }
    dns_free((char *)lock_p);
}

/*
 * process_lock
 *	Get an update lock on an entry
 *      The hash_p->dir_mutex is locked on entry and exit
 *
 * Input:
 *	stream_p = pointer to data base stream, db_dir and db_key 
 *		   must be initialized
 */
static dns_status_t 
process_lock (db_stream_t *stream_p)
{
    dir_hash_entry_t	*const hash_p = stream_p->db_dir_hash_p;
    dns_status_t	status;
    int thstatus;


    ASSERT_ACTIVE_STREAM(stream_p);

    /* 
     * Now see if there is already an update lock.
     * Couldn't do this before because UID was not known
     */    

    /* Search for existing lock.  If found queue this one */

    if ((thstatus = dthread_lock(&(hash_p->dir_entry_locks_lock))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{ /* b */
	lock_block_t	*newlock_p, *curlock_p;

	for (curlock_p = (lock_block_t *)hash_p->dir_entry_locks.next_p;

	     curlock_p != (lock_block_t *)&hash_p->dir_entry_locks;
	     curlock_p = (lock_block_t *)curlock_p->lock.next_p)
	    {
	    if (EQ_Timestamp(curlock_p->lock_cts, stream_p->db_entry.data_cts))
		{
		/* Release lock while we wait */

		if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
		{
		  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, 
				 (long)thstatus);
		}

		if (curlock_p->lock_owner)
		    {
		    curlock_p->lock_waiters++;
		    db_entry_release(stream_p);
		    while (curlock_p->lock_owner)
		    {
			if ((thstatus = dthread_wait(&(curlock_p->lock_ready),
						     &(hash_p->dir_entry_locks_lock))) < 0)
			{
			  dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, 
					 (long)thstatus);
			}
		    }
		    --curlock_p->lock_waiters;
		}
		/* Flag this stream as owner so doesn't get updated */
		curlock_p->lock_owner = stream_p;

		if ((thstatus = dthread_unlock(&(hash_p->dir_entry_locks_lock))) < 0)
		{
		  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, 
				 (long)thstatus);
		}

		/* Re-read the entry to pick up updates, already have lock */
		status = entry_read(stream_p, NO_LOCK);	    

		/* Get it back again */
		stream_p->db_entry.data_lock_p = curlock_p;

		if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
		{
		  dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
		}
		
		if (status != DNS_SUCCESS)
		    db_entry_release(stream_p);

		return(status);
	    }
	}
	
	/* Allocate lock block */
	newlock_p = (lock_block_t *)dns_malloc_temp(sizeof(lock_block_t));
	if (newlock_p)
	{
	    /* Build lock block */

	    if ((thstatus = dthread_create_condition(&newlock_p->lock_ready)) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
	      }
	    
	    newlock_p->lock_waiters = 0;
	    newlock_p->lock_owner = stream_p;
	    stream_p->db_entry.data_lock_p = newlock_p;
	    COPY_Timestamp(stream_p->db_entry.data_cts, newlock_p->lock_cts);

	    insert_list(&curlock_p->lock, &newlock_p->lock);
	    status = DNS_SUCCESS;
	}
	else
	{
	    status = DNS_NONSRESOURCES;
	}
    }
    /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(hash_p->dir_entry_locks_lock))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    return(status);
}

/*
 * build_entry_key
 * 	This routine build the key for an entry.
 *
 * Input:
 *	entrykey_p = pointer to location to build key into
 *	type = type of entry
 *	name_p = pointer to entry simple name
 */
static dns_status_t 
build_entry_key (entry_key_t   *entrykey_p,
                 unsigned int  type,
                 SimpleName_t  *name_p)
{
    register byte_t *next_p = entrykey_p->key_name;
    register unsigned int sort;

    ALIGN_CHECK(entrykey_p);

    switch (EXT8(name_p->sn_flag)) {
    default:
	COPY_SimpleName(name_p, next_p);
	next_p = SKIP_SimpleName(next_p);
	sort = BINARY_SORT;
	break;
    case SN_cds:
	COPY_SimpleName(name_p, next_p);
	next_p = SKIP_SimpleName(next_p);
	sort = ASCII_SORT;
	break;
    case SN_typed:		/* X.500 typed name */
	{
	dns_status_t status;

	/* X.500 normalize the original simple name for indexing */
	status = dns_normalize_sname(name_p, (SimpleName_t *)next_p,
					sizeof(entrykey_p->key_name));
	if (status != DNS_SUCCESS) return(status);
	next_p = SKIP_SimpleName(next_p);

	/* X.500 Save the orginal simple name */
	COPY_SimpleName(name_p, next_p);
	next_p = SKIP_SimpleName(next_p);

	/* X.500 ASN.1 typed names use typed collating sequence */
	sort = TYPED_SORT;
	break;
	}
    case SN_normal:
    case SN_quoted:
	{
	register SimpleName_t *ename_p = (SimpleName_t *)next_p;
	register int pos = 0;
	register byte_t bits;
	register int bitpos;

        /* Neutralize the case */
        names_s_neutralize_case(name_p, ename_p);
	next_p = (SimpleNameMask_t *)SKIP_SimpleName(ename_p);

        /* Build a casemask indicating which characters were converted */
        for (bitpos = 1, bits = 0; pos < LEN_SimpleName(name_p); pos++)
	    {
            if (name_p->sn_name[pos] != ename_p->sn_name[pos])
                bits |= bitpos;

	    if (0x100 <= (bitpos <<= 1))
		{
		*next_p++ = bits;
		bitpos = 1;
		bits = 0;
		}
	    }
	if (1 < bitpos)
	    *next_p++ = bits;

	/* Force ascii and quoted to sort the same */
	sort = ASCII_SORT;
	break;
        }
    }
    next_p = ALIGN_PTR(next_p);
    INS32( &entrykey_p->key_kd, 0);		 /* Union longword field */
    INS16( &entrykey_p->key_length,		 /* contains key_length  */
	DIFF_bytes(next_p, entrykey_p));
    INS8(entrykey_p->key_type, type);
    INS8(entrykey_p->key_sort, sort);
    return(DNS_SUCCESS);
}

/* 
 * build_record
 *	This routine allocates memory to return a record and
 *	fills it it
 *
 * Input:
 *	stream_p = pointer to db stream
 *	entry_p = pointer to entry data record
 */
dns_status_t 
build_record (db_stream_t  *stream_p,
              entry_key_t  *entry_p)
{
    register data_record_t *data_p;
    register DBSet_t *const set_p = (DBSet_t *)DATA_entry_data_record(entry_p);
    register unsigned int size;
    
    ASSERT_ACTIVE_STREAM(stream_p);
    ALIGN_CHECK(set_p);
    /* Determine blocks to allocate */
    size = sizeof(*data_p) - sizeof(data_p->dbr_data) + LEN_DBSet(set_p);
    size = dns_roundup(size)*BLOCK_SIZE;

    /* Allocate memory */
    data_p = (data_record_t *)dns_malloc_temp(size);
    if (data_p) {
	COPY_DBSet(set_p, data_p->dbr_data);
	stream_p->db_entry.data_p = data_p;
	stream_p->db_entry.data_buf_len = size;
	return(DNS_SUCCESS);
    } else
	return(DNS_NONSRESOURCES);
}

/*
 * db_entry_create
 *	This routines adds an entry to the data base.
 *	The directory should already be locked, and context 
 *	is initialized.
 *
 * Input:
 *	stream_p = pointer to data base stream
 *	name_p = pointer to entry simplename
 *	type = type of record
 */
dns_status_t 
db_entry_create (db_stream_t   *stream_p,
                 SimpleName_t  *name_p,
                 int           type)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_SimpleName(nameBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    ALIGN_DECL(entry_key_t, entry_data_max);
    dir_hash_entry_t	*hash_p;
    dns_status_t	status;
    int thstatus;

  
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_entry_create(%s,%s,%d)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_SimpleName(nameBuf,name_p),
        type));
 
    ASSERT_ACTIVE_STREAM(stream_p);

    stream_p->db_entry.data_lock_p = NULL;

    hash_p = stream_p->db_dir_hash_p;
    if (!hash_p) {
	status = DNS_UNKNOWNENTRY;
	goto leave_db_entry_create;
    }

    status = build_entry_key(&entry_data_max, type, name_p);
    if (status != DNS_SUCCESS)
	goto leave_db_entry_create;

    if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{
	    status = db_btree_insert_entry(stream_p,
					   hash_p, 
					   &entry_data_max);
	}

        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }
    
    build_return_data(stream_p, &entry_data_max);
    stream_p->db_entry.data_type = type;

    /* Build return data sets up entry fullname in key field */
    if (status == DNS_SUCCESS)
    {
        FullName_u entry_key;      /* Holds properly-cased name */
        /*
         * By this time the db_key in the stream has been case neutralized,
         * so we have to get the original case for the transaction log.
         */
        COPY_FullName((FullName_t *)stream_p->db_key, (FullName_t *)entry_key);

	{ /* X.500 - getting original name may change length! */
	    FullName_t *the_fname;
	    SimpleName_t *the_sname;
	    int original_length;

	    the_sname = names_pointlast((FullName_t *)entry_key);
	    the_fname = (FullName_t *)entry_key;
	    original_length = EXT8(the_sname->sn_length);
	    db_entry_name_cased(stream_p, the_sname);
	    if (original_length != EXT8(the_sname->sn_length))
		INS16(the_fname->fn_length, EXT16(the_fname->fn_length) +
		      EXT8(the_sname->sn_length) - original_length);
        }
	
	db_tlog_create(stream_p->db_file_p,
			stream_p->db_function, stream_p->db_entry.data_type,
			(FullName_t *)entry_key,
			(ObjUID_t *)stream_p->db_dir.data_uid,
			LEN_DBSet(stream_p->db_entry.data_p->dbr_data),
			stream_p->db_entry.data_p->dbr_data);
    }
    
leave_db_entry_create:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_entry_create(%s,%s,%d) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_SimpleName(nameBuf,name_p),
        type,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * db_entry_create_recover
 *	This routines adds an entry to the data base.  This is used
 *	to rebuild the database after a crash.
 *
 * Input:
 *	stream_p = pointer to data base stream
 *	myname_p = pointer to entry fullname
 *	type = type of record
 *	diruid_p = pointer to directory uid
 */
dns_status_t 
db_entry_create_recover (db_stream_t  *stream_p,
                         FullName_t   *myname_p,
                         int          type,
                         ObjUID_t     *diruid_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_FullName(mynameBuf)
    DEB_ASCII_BUF_ObjUID(diruidBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    ALIGN_DECL(entry_key_t, entry_data_max);
    /* Hash uid to get entry index */
    dir_hash_entry_t *const hash_p = db_hash_uid(stream_p->db_file_p, diruid_p);
    dns_status_t	status = DNS_UNKNOWNENTRY;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_entry_create_recover(%s,%s,%d,%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_FullName(mynameBuf,myname_p),
        type,
        deb_ascii_ObjUID(diruidBuf,diruid_p)));
 
    ASSERT_ACTIVE_STREAM(stream_p);

    if (hash_p)
    { /* h */
	DBSet_t *const set_p = (DBSet_t *)stream_p->db_entry.data_p->dbr_data;

	stream_p->db_dir_hash_p = hash_p;
	/* 
         * When a create is logged it contains any delete holders created
         * after a successfull entry_create.  We must remove them now,
	 * cause the are already accounted for in the database.
	 */
	ALIGN_CHECK(set_p);

	if ((0 < set_p->ds_hcnt) && (0 < set_p->ds_ocnt))
	    {
	    /* This is where the data is */
	    byte_t *const from_p = DATA_DBSet(set_p);
	    /* This is the earliest place the data could start */
	    byte_t *const to_p = (byte_t *)&set_p->ds_offp[set_p->ds_ocnt];
	    /* Figure adjustment to overlay delete holders */
	    const int len = (from_p - to_p) & ~ALIGN_MASK;

	    if (0 < len)
		{
		unsigned int pos;

		/* Shift record to remove old data */
		MOVE_bytes(from_p, SKIP_bytes(from_p, -len), 
			    SKIP_DBSet(set_p) - from_p);
	
		/* Adjust offsets */
		for (pos = 0; pos < set_p->ds_ocnt; pos++)
		    set_p->ds_offp[pos] -= len;

		set_p->ds_setl -= len;
		}
	    set_p->ds_hcnt = 0;
	    }

	/* Build key */
	status = build_entry_key(&entry_data_max, type,
		names_pointlast(myname_p));
	if (status == DNS_SUCCESS)
	{

	    if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
	    }
	    
                /* Beginning of the 'protected code' */
	        {
		    status = db_btree_insert_entry(stream_p,
						   hash_p,
						   &entry_data_max);
		    if (status == DNS_ENTRYEXISTS)
		    {
			DCE_SVC_DEBUG((
    		            cds__svc_handle,
                            cds_svc_database,
                            svc_c_debug1,
                            "db_entry_create_recover(%s,%s,%d,%s) ins->rep",
 	                    deb_ascii_db_stream(streamBuf,stream_p),
	                    deb_ascii_FullName(mynameBuf,myname_p), 
                            type,
	                    deb_ascii_ObjUID(diruidBuf,diruid_p)));

			status = db_btree_replace_entry(stream_p, 
						    hash_p, 
						    &entry_data_max,
						    NULL);
		    }
		}
                /* End of the 'protected code' */

	    if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	    }
	} 
		
    } /* end if (hash_p) */
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_entry_create_recover(%s,%s,%d,%s) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_FullName(mynameBuf,myname_p),
        type,
        deb_ascii_ObjUID(diruidBuf,diruid_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * entry_read
 *	Read an entry from the data base
 *
 * Input:
 *	stream_p = pointer to data base stream, db_dir and db_key 
 *		   must be initialized
 *      lock = update or no lock
 */
static dns_status_t 
entry_read (db_stream_t  *stream_p,
            int          lock)
{
    node_header_t	*bkt_p;
    dir_hash_entry_t	*hash_p;
    unsigned int	pos;
    dns_status_t	status;
    int thstatus;


    ASSERT_ACTIVE_STREAM(stream_p);

    stream_p->db_entry.data_p = NULL;
    stream_p->db_entry.data_lock_p = NULL;

    /* Neutralize the case of the db_key (in place) for proper lookup */
    /* X.500 - Changing case isn't good enough for typed names */
    status = dns_normalize_fname((FullName_t *)stream_p->db_key,
		(FullName_t *)stream_p->db_key, sizeof(stream_p->db_key));
    if (status != DNS_SUCCESS) return(status);

    /* Find hash entry for directory */
    hash_p = stream_p->db_dir_hash_p;
    if (!hash_p) 
	{
	hash_p = db_hash_locate(stream_p->db_file_p,
			(FullName_t *)stream_p->db_key);
	if (!hash_p) return(DNS_UNKNOWNENTRY);
	stream_p->db_dir_hash_p = hash_p;
	}


    if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{
	    status = db_btree_locate_entry_bkt(stream_p, hash_p->dir_entry_p, 
					       hash_p, &bkt_p, &pos);

	    if (status == DNS_SUCCESS) 
	    {
		entry_key_t *entry_p = (entry_key_t *)INDEX_node_rec(bkt_p, pos);

		/* Have the entry bucket, now build record to return */
		status = build_record(stream_p, entry_p);
		if (status == DNS_SUCCESS) {
		    build_return_data(stream_p, entry_p);
		    if (lock == UPDATE_LOCK) 
			/* This call may release and re-aquire the dir_mutex */
			status = process_lock(stream_p);
		}
	    }
	    
	}
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }
    return(status);
}

/*
 * db_entry_read
 *	Read an entry from the data base
 *
 * Input:
 *	stream_p = pointer to data base stream, db_dir and db_key 
 *		   must be initialized
 */
dns_status_t 
db_entry_read (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_entry_read(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));
 
    status = entry_read(stream_p, NO_LOCK);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_entry_read(%s) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

/*
 * db_entry_read_for_update
 *	Read an entry from the data base and lock it. 
 *
 * Input:
 *	stream_p = pointer to data base stream, db_dir and db_key 
 *		   must be initialized
 */
dns_status_t 
db_entry_read_for_update (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_entry_read_for_update(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));
 
    status = entry_read(stream_p, UPDATE_LOCK);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_entry_read_for_update(%s) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * entry_read_next
 *	Read the next sequenctial entry from the data base
 *
 * Input:
 *	stream_p = pointer to data base stream, db_dir and db_key 
 *		   must be initialized
 *      lock = update or no lock
 */
static dns_status_t 
entry_read_next (db_stream_t  *stream_p,
                 int          lock)
{
    dir_hash_entry_t	*hash_p;
    node_header_t	*bkt_p;
    unsigned int	pos;
    dns_status_t	status;
    int thstatus;


    ASSERT_ACTIVE_STREAM(stream_p);

    stream_p->db_entry.data_p = NULL;
    stream_p->db_entry.data_lock_p = NULL;

    /* Neutralize the case of the db_key (in place) for proper lookup */
    /* X.500 - changing case isn't sufficient for typed names */
    status = dns_normalize_fname((FullName_t *)stream_p->db_key,
		(FullName_t *)stream_p->db_key, sizeof(stream_p->db_key));
    if (status != DNS_SUCCESS) return(status);

    if (!(hash_p = stream_p->db_dir_hash_p))
	{
	FullName_t *fname_p = (FullName_t *)stream_p->db_key;

        hash_p = db_hash_uid(stream_p->db_file_p, (ObjUID_t *)fname_p->fn_root);
	if (!(stream_p->db_dir_hash_p = hash_p)) return(DNS_UNKNOWNENTRY);
	}
    
    if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

     /* Beginning of the 'protected code' */
	{
	status = db_btree_locate_entry_bkt(stream_p, hash_p->dir_entry_p, 
			hash_p, &bkt_p, &pos);
	if (status == DNS_SUCCESS)
	    pos++;
	else if ((status == DNS_UNKNOWNENTRY) && bkt_p)
	    status = DNS_SUCCESS;

	if (status == DNS_SUCCESS)
	    /* At end of bucket or at dummy name in last data bucket */
	    if (bkt_p->node_count <= pos) 
		/* Moved into another bucket */
		{
		bkt_p = bkt_p->node_next_p;
		pos = 0;
		}
	    else if (!bkt_p->node_next_p && (pos == bkt_p->node_count-1))
		{
		status = DNS_UNKNOWNENTRY;
		}

	if (status == DNS_SUCCESS)
	    {
	    entry_key_t *entry_p = (entry_key_t *)INDEX_node_rec(bkt_p, pos);
	    /* Have the entry bucket, now build record to return */

	    if (EXT8(entry_p->key_type) != stream_p->db_entry.data_type)
		/* If not the same type, all done read next */
		status = DNS_UNKNOWNENTRY;
	    else
		{
		status = build_record(stream_p, entry_p);
		if (status == DNS_SUCCESS) {
		    build_return_data(stream_p, entry_p);
		    if (lock == UPDATE_LOCK)
		      /* dir_mutex may be freed and allocted here */
		      status = process_lock(stream_p);
		  }
		}
	    }
	}
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    return(status);
}

/*
 * db_entry_read_next
 *	Read the next sequenctial entry from the data base and
 *	return it unlocked.
 *
 * Input:
 *	stream_p = pointer to data base stream, db_dir and db_key 
 *		   must be initialized
 */
dns_status_t 
db_entry_read_next (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_entry_read_next(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));
 
    status = entry_read_next(stream_p, NO_LOCK);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_entry_read_next(%s) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * db_entry_read_next_for_update
 *	Read the next sequential entry from the data base and 
 *	return it locked.
 *
 * Input:
 *	stream_p = pointer to data base stream, db_dir and db_key 
 *		   must be initialized
 */
dns_status_t 
db_entry_read_next_for_update (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_entry_read_next_for_update(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));
 
    status = entry_read_next(stream_p, UPDATE_LOCK);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_entry_read_next_for_update(%s) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * db_entry_delete
 *	Delete an entry from the database
 *	Record is locked on input.
 *
 * Input:
 *	stream_p = pointer to data base stream, db_dir and db_key 
 *		   must be initialized
 */
dns_status_t 
db_entry_delete (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    LOG_ASCII_BUF_dns_status(statusBuf)
    entry_key_t		ekey;
    dir_hash_entry_t	*hash_p;
    SimpleName_t	*name_p;
    unsigned int	sort;
    dns_status_t	status;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_entry_delete(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));
 
    ASSERT_ACTIVE_STREAM(stream_p);

    if (!stream_p->db_entry.data_lock_p)
    {
       DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_database,
           svc_c_debug1, 
	   "db_entry_delete(%s) bug line %d",
	   deb_ascii_db_stream(streamBuf,stream_p), 
	   __LINE__));
       
       status = DNS_NAMESERVERBUG;
       goto leave_db_entry_delete;
    }

    /* Find hash entry for directory */
    hash_p = stream_p->db_dir_hash_p;
    if (!hash_p) 
	{
	hash_p = db_hash_locate(stream_p->db_file_p,
			(FullName_t *)stream_p->db_key);
	if (!hash_p)
	    {
	    status = DNS_UNKNOWNENTRY;
	    goto leave_db_entry_delete;
	    }
	}

    /* Build entry key to locate it */
    name_p = names_pointlast((FullName_t *)stream_p->db_key);
    switch (EXT8(name_p->sn_flag)) {
    case SN_null:
    case SN_binary:
	sort = BINARY_SORT;
	break;
/*  case SN_normal:		*/
/*  case SN_quoted:		*/
/*  case SN_wildcard:		*/
/*  case SN_cds:                */
    default:
	sort = ASCII_SORT;
	break;
    case SN_typed:
	sort = TYPED_SORT;
	break;
    }
    INS8(ekey.key_type, stream_p->db_entry.data_type);
    INS8(ekey.key_sort, sort);
    COPY_SimpleName(name_p, ekey.key_name);

    if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{

	status = db_btree_remove_entry(stream_p, hash_p, &ekey);
        if (status == DNS_DATACORRUPTION)
            {
            status = db_btree_rebuild_index(hash_p);
            if (status != DNS_SUCCESS)
	    {
                dce_svc_printf(CDS_S_SERVER_REBUILDINDEX_A_MSG,
                    deb_ascii_dns_status(statusBuf,status));
	    }
            
  	    status = db_btree_remove_entry(stream_p, hash_p, &ekey);
	    
	} /* endif - DATACORRUPTION */
	
    }
    /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    /* Startup next thread waiting for this record */
    release_lock(stream_p);

leave_db_entry_delete:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_entry_delete(%s) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

/*
 * db_entry_update
 *	Read an entry from the data base and update it.  Must
 *	already be locked.
 *
 * Input:
 *	stream_p = pointer to data base stream, db_dir and db_key 
 *		   must be initialized
 *	tlog_p = pointer to transaction description
 *
 */
dns_status_t 
db_entry_update (db_stream_t  *stream_p,
                 tlog_dsc_t   *tlog_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    ALIGN_DECL(entry_key_t, ekey);
    dir_hash_entry_t	*hash_p;
    Timestamp_t		*allupto_p = NULL;
    int			idx;
    dns_status_t	status;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_entry_update(%s,%p)",
        deb_ascii_db_stream(streamBuf,stream_p),
        tlog_p));
 
    ASSERT_ACTIVE_STREAM(stream_p);

    if (!stream_p->db_entry.data_lock_p)
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug1,
	    "db_entry_update(%s,%p) bug line %d",
	    deb_ascii_db_stream(streamBuf,stream_p), 
	    tlog_p, 	
            __LINE__));

	status = DNS_NAMESERVERBUG;
	goto leave_db_entry_update;
    }

    /* Cleanup old data */
    if (stream_p->db_dir.data_p)
	{
	DBSet_t *set_p;

	if (find_attribute(stream_p->db_dir.data_p,
		dns_AttributeName(DNS_ALLUPTO), &set_p, &idx) == DNS_SUCCESS)
	    {
	    DBSetMember_t *val_p = find_last_value(set_p);
	    if (val_p)
		{
		db_cleanup_record(&stream_p->db_entry,
			(Timestamp_t *)val_p->dm_data);
		allupto_p = (Timestamp_t *)val_p->dm_data;
		}
	    }
	}

    /* Find hash entry for directory */
    hash_p = stream_p->db_dir_hash_p;
    if (!hash_p) 
	{
	hash_p = db_hash_locate(stream_p->db_file_p,
			(FullName_t *)stream_p->db_key);
	if (!hash_p)
	    {
	    status = DNS_UNKNOWNENTRY;
	    goto leave_db_entry_update;
	    }
	}


    /* Build entry key to locate it */
    (void)build_entry_key(&ekey, stream_p->db_entry.data_type,
	names_pointlast((FullName_t *)stream_p->db_key));

    if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{

	    status = db_btree_replace_entry(stream_p, hash_p, &ekey, allupto_p);
	}
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    if (status == DNS_SUCCESS)
	if (tlog_p)
	     db_tlog_update(stream_p->db_file_p, stream_p->db_function,
		(FullName_t *)stream_p->db_key,
		(ObjUID_t *)hash_p->dir_uid,
		stream_p->db_entry.data_type, tlog_p);

    /* Startup next thread waiting for this record */
    release_lock(stream_p);

leave_db_entry_update:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_entry_update(%s,%p) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        tlog_p,
        deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

/*
 * db_entry_release
 *	Cleanup up entry resources
 *
 * Input: 
 *	stream_p = pointer to data base stream
 *
 */
void 
db_entry_release (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_entry_release(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));
 
    ASSERT_ACTIVE_STREAM(stream_p);

    if (stream_p->db_entry.data_p)
	{
	dns_free((char *)stream_p->db_entry.data_p);
	stream_p->db_entry.data_p = NULL;
	}

    /* Startup next thread waiting for this record */
    release_lock(stream_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_entry_release(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));
}

/*
 * db_entry_read_next_bucket
 *	This routine returns a bucket full of records.  It is called
 *	by copy_updates().  It is an optimization so that we don't
 *	have to lookup up each entry individually, we can batch them.
 *
 * Input:
 *	stream_p	pointer to stream descriptor
 *	bkt_pp		address of pointer to bucket	
 *
 * Output:
 *	bkt_pp contains new bucket or NULL
 *
 */
dns_status_t 
db_entry_read_next_bucket (db_stream_t    *stream_p,
                           node_header_t  **bkt_pp)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dir_hash_entry_t	*hash_p = stream_p->db_dir_hash_p;
    node_header_t	*next_p;
    unsigned int	pos;
    dns_status_t	status;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_entry_read_next_bucket(%s,)",
        deb_ascii_db_stream(streamBuf,stream_p)));
 
    ASSERT_ACTIVE_STREAM(stream_p);

    stream_p->db_entry.data_lock_p = NULL;

    /* Neutralize the case of the db_key (in place) for proper lookup */
    /* X.500 - changing case isn't sufficient for typed names */
    status = dns_normalize_fname((FullName_t *)stream_p->db_key,
		(FullName_t *)stream_p->db_key, sizeof(stream_p->db_key));
    if (status != DNS_SUCCESS)
	goto leave_db_entry_read_next_bucket;
	
    if ((thstatus = dthread_lock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{

	node_header_t *bkt_p = *bkt_pp;

	/* No bucket allocated first time through */
	if (!bkt_p)
	    {	
	    next_p = db_btree_get_first_data_bucket(hash_p->dir_entry_p);
	    pos = 0;
	    if (next_p->node_next_p || (next_p->node_count != 1))
		status = DNS_SUCCESS;
	    else
		status = DNS_UNKNOWNENTRY;
	    }
	else
	    /* 
	     * Find the next bucket by first finding the bucket containing 
	     * the last key processed
	     */
	    {
	    status = db_btree_locate_entry_bkt(stream_p,
		hash_p->dir_entry_p, hash_p, &next_p, &pos);
	    if (status == DNS_SUCCESS)
		pos++;
	    else if ((status == DNS_UNKNOWNENTRY) && next_p)
		status = DNS_SUCCESS;

	    if (status == DNS_SUCCESS)
		{
		/* 
                 * At end of bucket or at dummy name in last data bucket.
                 * Advance to the next bucket that has nodes in it.
                 */

		if (next_p->node_count <= pos) 
		    {
                    do
                        {
		        next_p = next_p->node_next_p;
                        } while (next_p && (next_p->node_count == 0));
                    if (!next_p)
                        {
                        status = DNS_UNKNOWNENTRY;
                        }
		    pos = 0;
		    }
		else if (!next_p->node_next_p && (pos == next_p->node_count-1))
		    status = DNS_UNKNOWNENTRY;
		}
	    }

	if (status == DNS_SUCCESS)
	    {
	    /* Make sure we have a buffer to return the bucket */
	    if (!bkt_p)
		{
		bkt_p = (node_header_t *)dns_malloc_temp(
		    next_p->node_blocks*BLOCK_SIZE);
		}
	    else if (bkt_p->node_blocks < next_p->node_blocks)
		{
		dns_free((char *)bkt_p);
		bkt_p = (node_header_t *)dns_malloc_temp(
		    next_p->node_blocks*BLOCK_SIZE);
		}

	    if (bkt_p) 
		{
		db_btree_copy_keys(next_p, bkt_p, next_p->node_blocks,
			pos, next_p->node_count);
		bkt_p->node_next_p = next_p->node_next_p;
		bkt_p->node_prev_p = next_p->node_prev_p;
		}
	    else
		status = DNS_NONSRESOURCES;
	    }
	else if (status == DNS_UNKNOWNENTRY)
	    {
	    if (bkt_p)
		{
		dns_free((char *)bkt_p);
		bkt_p = NULL;
		}
	    }

	*bkt_pp = bkt_p;

	}
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(hash_p->dir_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

leave_db_entry_read_next_bucket:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_entry_read_next_bucket(%s,) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * db_entry_build_record
 *	Used in association with db_entry_read_next_bucket().
 *
 *
 * Input:
 *	stream_p	pointer to stream descriptor
 *	pos		record position to build
 *	bkt_p		address of pointer to bucket (locked)
 *
 * Output:
 *
 */
dns_status_t 
db_entry_build_record (db_stream_t    *stream_p,
                       int            pos,
                       node_header_t  *bkt_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_node_header(bktBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    register entry_key_t *entry_p;
    register data_record_t *data_p;
    register byte_t	*start_p;
    int			size, data_len;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " >db_entry_build_record(%s,%d,%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        pos,
        deb_ascii_node_header(bktBuf,bkt_p)));
 
    ASSERT_ACTIVE_STREAM(stream_p);

    /* See if this is dummy record */
    if (!bkt_p->node_next_p && (pos == bkt_p->node_count-1))
	{
	status = DNS_UNKNOWNENTRY;
	goto leave_db_entry_build_record;
	}

    /* Get pointer to data record in data bucket */
    entry_p = (entry_key_t *)INDEX_node_rec(bkt_p, pos);
    start_p = DATA_entry_data_record(entry_p);
    data_len = sizeof(*data_p) - sizeof(data_p->dbr_data) + LEN_DBSet(start_p);

    /* Get block size */
    size = dns_roundup(data_len)*BLOCK_SIZE;
    if (size < DATA_BUCKET_BLOCKS*BLOCK_SIZE)
	size = DATA_BUCKET_BLOCKS*BLOCK_SIZE;

    if (!(data_p = stream_p->db_entry.data_p))
	{
	/* 
	 * Allocate a large buffer to cover most cases so it can
	 * be re-used 
	 */
	data_p = (data_record_t *)dns_malloc_temp(size);
	if (!data_p)
	    {
	    status = DNS_NONSRESOURCES;
	    goto leave_db_entry_build_record;
	    }
	stream_p->db_entry.data_p = data_p;
	stream_p->db_entry.data_buf_len = size;
	}
    else if (stream_p->db_entry.data_buf_len < data_len)
	{
	/* Allocate biggest buffer so it can be re-used */
	dns_free((char *)data_p);
	stream_p->db_entry.data_p = NULL;

	data_p = (data_record_t *)dns_malloc_temp(size);
	if (!data_p)
	    {
	    status = DNS_NONSRESOURCES;
	    goto leave_db_entry_build_record;
	    }
	stream_p->db_entry.data_p = data_p;
	stream_p->db_entry.data_buf_len = size;
	}	

    COPY_DBSet(start_p, data_p->dbr_data);
    stream_p->db_entry.data_type = EXT8(entry_p->key_type);

    build_return_data(stream_p, entry_p);
    status = DNS_SUCCESS;

leave_db_entry_build_record:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug3,
        " <db_entry_build_record(%s,%d,%s) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        pos,
        deb_ascii_node_header(bktBuf,bkt_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * db_entry_name_cased
 *	Construct the entry simplename with its original case
 *
 * Input:
 *	stream_p 	pointer to stream descriptor
 *	name_p 		pointer to buffer to return cased simple name
 *
 */
void 
db_entry_name_cased (db_stream_t   *stream_p,
                     SimpleName_t  *name_p)
{
    if (EMPTY_FullName(stream_p->db_key))
	{
	COPY_SimpleName(NullSimpleName, name_p);
	}
    else
	{
        /* Restore the case of the name */
	COPY_SimpleName(stream_p->db_entry_original_name, name_p);
	}
}

/*
 * deb_ascii_entrySort
 *	Decode sort field of entry_key_t
 */
char *
deb_ascii_entrySort (char *const        buf,
                      const void *const  ptr)
{
    const byte_t *const sort_p = (const byte_t *)ptr;

    if (sort_p)
	switch (*sort_p) {
	case BINARY_SORT:
	    return("binary");
	case ASCII_SORT:
	    return("ascii");
	case TYPED_SORT:
	    return("typed");
	default:
	    (void)sprintf(buf, "sort(%d)", *sort_p);
	    return(buf);
	}
    else
	return("NULL entrySort");
}
/*
 * deb_ascii_entry_key
 */
char *
deb_ascii_entry_key (char *const        buf,
                      const void *const  ptr)
{
    entry_key_t *const key_p = (entry_key_t *)ptr;

    if (key_p) {
	(void)sprintf(buf, DEB_ASCII_FMT_ptr_TO, key_p);

	if ((EXT8(key_p->key_type) != 255) && (EXT8(key_p->key_sort) != 255)) {
	    LOG_ASCII_BUF_EntryType(typeBuf)
	    LOG_ASCII_BUF_entrySort(sortBuf)
	    LOG_ASCII_BUF_SimpleName(nameBuf)

	    (void)sprintf(strchr(buf, '\0'), "{%s,%s,%s}",
		deb_ascii_EntryType(typeBuf,key_p->key_type),
		deb_ascii_entrySort(sortBuf,key_p->key_sort),
		deb_ascii_SimpleName(nameBuf,key_p->key_name));
	} else {
	    (void)sprintf(strchr(buf, '\0'), "{255,255}");
	}

	return(buf);
    } else
	return("NULL entry_key_t");
}
