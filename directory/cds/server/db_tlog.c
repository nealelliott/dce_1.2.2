/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_tlog.c,v $
 * Revision 1.1.8.2  1996/02/18  19:36:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:34  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:30:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:08  root]
 * 
 * Revision 1.1.6.4  1994/08/03  19:04:06  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:24:44  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:43:20  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:49  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/14  14:51:15  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:20:04  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:11:12  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:42:00  peckham]
 * 
 * Revision 1.1.4.3  1993/02/08  16:02:51  peckham
 * 	Gen 51, 29-Jan-1993, by Roger Zee
 * 	   Rewrote db_tlog_recover to fix OT bug #6466 and deal with very large
 * 	   transactions in the tlog file.  Also modified db_tlog_create and
 * 	   db_tlog_update to be able to write very large transactions to the
 * 	   tlog file.
 * 	[1993/02/04  19:47:31  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:21:29  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:21:37  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:29:35  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:34:50  weisman]
 * 
 * Revision 1.1  1992/01/19  15:25:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: db_tlog.c
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
#include <time.h>
#include <back.h>
#include <dbdef.h>
#include <dnsnormal.h>
#include <events.h>
#include <find.h>
#include <sets.h>
#include <tlog.h>
#include <uid.h>
#include <dce/assert.h>

/*
 * Tlog recover buffer states
 */
#define TBUF_EXIT    0
#define TBUF_EMPTY   1
#define TBUF_PROCESS 2
#define TBUF_HUGE    3

/*
 *   Local data structure definitions
 */

/*  Playback statistics (instrumentation purposes only) */

typedef struct {
    int    CreDirectory;     /* Directories, replicas created */
    int    CreObject;        /* Objects, links, child pointers created */
    int    CreDoUpdate;      /* DoUpdate playbacks */
    int    UpdDirectory;     /* Updates to directories */
    int    UpdCH;            /* Updates to clearinghouses */
    int    UpdObject;        /* Updates to objects (everything else) */
    time_t Start;            /* When the playback was started */
    time_t Finished;         /* When the playback was completed */
} TLog_statistics_t;

#define deb_ascii_TLog_statistics(b,p) deb_ascii_rec(b,p,"NULL TLog_statistics")
#define DEB_ASCII_LEN_TLOG_STATISTICS DEB_ASCII_LEN_rec
#define DEB_ASCII_BUF_TLog_statistics(b) DEB_ASCII_BUF_rec(b)
#define LOG_ASCII_BUF_TLog_statistics(b) LOG_ASCII_BUF_rec(b)

/*
 * Local prototypes
 */
static void
apply_entry_log (
    db_data_t		*,
    UpdatePacket_t	*,
    int			,
    Timestamp_t		*);

static void
apply_dir_log (
    db_stream_t		*,
    UpdatePacket_t	*);

static void
playback (
    db_stream_t		*,
    tlog_hdr_t		*,
    TLog_statistics_t   *);

/*
 * db_get_checksum
 *	Compute the checksum value for data being written to disk.
 *	Add up all the bytes
 *
 * Input:
 *	next_p = pointer to data
 *	len = number of bytes of data
 */
unsigned int 
db_get_checksum (byte_t        *next_p,
                 unsigned int  len)
{
    unsigned int	checksum = 0;

    if (len)
	do
	    checksum += *next_p++;
	while (--len);

    return(checksum);
}

/*
 * db_tlog_new
 * 	Create a new transaction log file.
 *
 * Input:
 *	file_p = pointer to cache file block
 *
 */
dns_status_t 
db_tlog_new (file_info_t *file_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t	status;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_tlog_new(%s)",
        deb_ascii_file_info(fileBuf,file_p)));

    if ((thstatus = dthread_lock(&(file_p->file_tlog_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {

	/* Bug if we're asked to open a TLog file that is already open */
	if (file_p->file_tlog_state == TLOG_FILE_OPEN)
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug1,    
		"db_tlog_new(%s) - bug generated line %d",
                deb_ascii_file_info(fileBuf,file_p), __LINE__));

	    status = DNS_NAMESERVERBUG;
	    goto unlock_db_tlog_new;
	}

	/* Force closed until open */
	file_p->file_tlog_state = TLOG_FILE_CLOSED;
	file_p->file_tlog_next_block = 1;
	/*
	 * This buffer has to be big enough to hold an entry.  Since
	 * the maximum is generated by back_do_updates, that will
	 * determine the maximum buffer size
	 */
	if (!file_p->file_tlog_buf_p)
	    file_p->file_tlog_buf_p = (byte_t *)dns_malloc(
		    (TLOG_BUF_BLOCKS + 1) * BLOCK_SIZE);

	if (file_p->file_tlog_buf_p)
	    {
	    file_p->file_tlog_buf_len = TLOG_BUF_BLOCKS*BLOCK_SIZE;

	    status = sys_create_tlog(file_p->file_ch_p->ch_filename,
                file_p->file_tlog_block,
		file_p->file_ckpt_hdr.hdr_sequence);
            if (status == DNS_SUCCESS)
                {
	        file_p->file_tlog_state = TLOG_FILE_OPEN;
                }
	    }
	else
	    status = DNS_NONSRESOURCES;

    }
unlock_db_tlog_new:

    /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_tlog_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_tlog_new(%s) return(%s)",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * db_tlog_open
 * 	Open existing transaction log file.
 *
 * Input:
 *	file_p = pointer to cache file block
 *
 */
dns_status_t 
db_tlog_open (file_info_t *file_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t status;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_tlog_open(%s)",
        deb_ascii_file_info(fileBuf,file_p)));

    if ((thstatus = dthread_lock(&(file_p->file_tlog_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{

	/* Bug if we're asked to open a TLog file that is already open */
	if (file_p->file_tlog_state == TLOG_FILE_OPEN)
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug1,  
	        "db_tlog_open(%s) - bug generated line %d",
                deb_ascii_file_info(fileBuf,file_p), __LINE__));

	    status = DNS_NAMESERVERBUG;
	    goto unlock_db_tlog_open;
	}

	file_p->file_tlog_state = TLOG_FILE_CLOSED;   /* Force closed until open */
	file_p->file_tlog_next_block = 1;
	/*
	 * This buffer has to be big enough to hold an entry.  Since
	 * the maximum is generated by back_do_updates, that will
	 * determine the maximum buffer size
	 */
	if (!file_p->file_tlog_buf_p)
	    file_p->file_tlog_buf_p = (byte_t *)dns_malloc(
		    (TLOG_BUF_BLOCKS + 1) * BLOCK_SIZE);

	if (file_p->file_tlog_buf_p)
	    {
	    file_p->file_tlog_buf_len = TLOG_BUF_BLOCKS*BLOCK_SIZE;

	    status = sys_open_tlog(file_p->file_ch_p->ch_filename,
		file_p->file_tlog_block);
	    }
	else
	    status = DNS_NONSRESOURCES;

	if (status != DNS_SUCCESS)
	    status = sys_create_tlog(file_p->file_ch_p->ch_filename,
		                 file_p->file_tlog_block,
                                 file_p->file_ckpt_hdr.hdr_sequence);

	if (status == DNS_SUCCESS)
	    file_p->file_tlog_state = TLOG_FILE_OPEN;

	}
unlock_db_tlog_open:
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_tlog_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_tlog_open(%s) return(%s)",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 * db_tlog_create
 *	This routine creates the transaction log entry for a create
 *	entry, directory and pseudo operation.  The entire record
 *	is journalled.  This seems easier then making up transactions
 *	for each attribute value.
 *
 *	There is only one channel assigned to this file, writes must be
 *	synchronized.  Multiple buffer operations must also be synchronous
 *	to maintain order.
 *
 * Input:
 *	file_p = pointer to file descriptor
 * 	function = DNS function which generated create
 *	type = type of record
 *	name_p = full name of entry
 *	uid_p = uid of entry
 *	len = length of data
 *	data_p = pointer to data itself
 */
void 
db_tlog_create (file_info_t  *file_p,
                int          function,
                int          type,
                FullName_t   *name_p,
                ObjUID_t     *uid_p,
                int          len,
                byte_t       *data_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_FullName(nameBuf)
    DEB_ASCII_BUF_ObjUID(uidBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    int			datalen, total_blocks;
    dns_status_t	status = DNS_SUCCESS;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_tlog_create(%s,%d,%d,%s,%s,%d,%p)",
        deb_ascii_file_info(fileBuf,file_p),
        function,
        type,
        deb_ascii_FullName(nameBuf,name_p),
        deb_ascii_ObjUID(uidBuf,uid_p),
        len,
        data_p));

    datalen = ALIGN_LEN(LEN_FullName(name_p)) + len;
    /* round up to the next block */
    total_blocks = (sizeof(tlog_hdr_t) + datalen + (BLOCK_SIZE - 1))/BLOCK_SIZE;
    if ((thstatus = dthread_lock(&(file_p->file_tlog_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{

	if (file_p->file_state != file_recover)
	    {
	    tlog_hdr_t *tlog_buf_p;
	    byte_t *next_p;
	    byte_t *huge_buf_p = 0;	/* Pointer to huge buffer */

	    /*
	     * Need to check if the total_blocks for this transaction
	     * is greater than our "normal" write buffer, and if so,
	     * allocate a buffer large enough.
	     */
	    if (total_blocks > TLOG_BUF_BLOCKS)
	        {
		huge_buf_p = (byte_t *)dns_malloc_temp(total_blocks*BLOCK_SIZE);
		if (huge_buf_p == (byte_t *) NULL) {
		    status = DNS_NONSRESOURCES;
		    goto unlock_db_tlog_create;
		    }
		tlog_buf_p = (tlog_hdr_t *) huge_buf_p;
	        }
	    else
	        tlog_buf_p = (tlog_hdr_t *)file_p->file_tlog_buf_p;

	    /* First set up header */
	    tlog_buf_p->t_len = datalen;
	    tlog_buf_p->t_seq = file_p->file_ckpt_hdr.hdr_sequence;
	    tlog_buf_p->t_type = TLOG_CREATE;
	    tlog_buf_p->t_operation = function;
	    tlog_buf_p->t_rec = type;
	    tlog_buf_p->t_cnt = total_blocks;
	    COPY_ObjUID(uid_p, tlog_buf_p->t_diruid);
	    tlog_buf_p->t_align_mask = ALIGN_MASK;

	    /* pointer after header */
	    COPY_FullName(name_p, &tlog_buf_p[1]);
	    next_p = ALIGN_PTR(SKIP_FullName(&tlog_buf_p[1]));

	    COPY_bytes(data_p, next_p, len);

	    /* Compute checksum */
	    tlog_buf_p->t_checksum = db_get_checksum(
		(byte_t *)&tlog_buf_p[1], tlog_buf_p->t_len);

	    status = sys_safe_write(file_p->file_tlog_block,
			(byte_t *)tlog_buf_p,
			file_p->file_tlog_next_block, total_blocks);

	    if (status == DNS_SUCCESS)
		file_p->file_tlog_next_block += total_blocks;
	    else
		db_checkpoint_schedule(file_p);

	    /* Deallocate the huge buffer if we allocated one */
	    if (huge_buf_p)
	        dns_free(huge_buf_p);
	    }
    }

unlock_db_tlog_create:

    /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_tlog_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_tlog_create(%s,%d,%d,%s,%s,%d,%p) status(%s)",
        deb_ascii_file_info(fileBuf,file_p),
        function,
        type,
        deb_ascii_FullName(nameBuf,name_p),
        deb_ascii_ObjUID(uidBuf,uid_p),
        len,
        data_p,
        deb_ascii_dns_status(statusBuf,status)));
}




/*
 * db_tlog_close
 *
 * This routine properly closes a transaction log file.  The TLog
 * state is checked so that spurious closes are ignored (these can occur
 * during clearinghouse create sequences).
 *
 */
dns_status_t 
db_tlog_close (file_info_t *file_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t	status;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_tlog_close(%s)",
        deb_ascii_file_info(fileBuf,file_p)));

    if ((thstatus = dthread_lock(&(file_p->file_tlog_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

     /* Beginning of the 'protected code' */

	switch (file_p->file_tlog_state) {
	case TLOG_FILE_OPEN:
	    (void)sys_close(file_p->file_tlog_block);
            if (file_p->file_tlog_buf_p) {
                dns_free((char *)file_p->file_tlog_buf_p);
                file_p->file_tlog_buf_p = NULL;
            }
	    file_p->file_tlog_state = TLOG_FILE_CLOSED;
	    /* fall through to set status */
	case TLOG_FILE_CLOSED:
	    status = DNS_SUCCESS;
            break;
	default:
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug1,  
	        "db_tlog_close(%s) - bug generated line %d",
                deb_ascii_file_info(fileBuf,file_p), 
                __LINE__));

	    status = DNS_NAMESERVERBUG;
            break;
	}

    /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_tlog_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_tlog_close(%s) return(%s)",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/*
 * db_tlog_delete
 *
 * This routine deletes the checkpoint file during clearinghouse deletion.
 *
 */
dns_status_t 
db_tlog_delete (file_info_t *file_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t	status;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_tlog_delete(%s)",
        deb_ascii_file_info(fileBuf,file_p)));

    if ((thstatus = dthread_lock(&(file_p->file_tlog_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	
        if (file_p->file_tlog_state == TLOG_FILE_CLOSED)
	    status = sys_delete(file_p->file_tlog_block);
	else
	    status = DNS_NAMESERVERBUG;

        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_tlog_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_tlog_delete(%s) return(%s)",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/*
 * db_tlog_update
 *	This routine creates the transaction log entry for one or more
 *	update operations.  Data is stored as an update packet.
 *
 *	There is only one channel assigned to this file, writes must be
 *	synchronized.  Multiple buffer operations must also be synchronous
 *	to maintain order.
 *
 * Input:
 *	file_p = pointer to file descriptor
 *	function = function generating update
 *	name_p = name of entry being updated
 *	uid_p = uid of directory being updated
 *	type = type of record
 *	tlog_p = address of transaction description
 */
void 
db_tlog_update (file_info_t  *file_p,
                int          function,
                FullName_t   *name_p,
                ObjUID_t     *uid_p,
                int          type,
                tlog_dsc_t   *tlog_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_FullName(nameBuf)
    DEB_ASCII_BUF_ObjUID(uidBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    byte_t		*next_p;
    int			datalen, total_blocks;
    dns_status_t	status = DNS_SUCCESS;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_tlog_update(%s,%d,%s,%s,%d,%p)",
        deb_ascii_file_info(fileBuf,file_p),
        function,
        deb_ascii_FullName(nameBuf,name_p),
        deb_ascii_ObjUID(uidBuf,uid_p),
        type,
        tlog_p));

    if ((thstatus = dthread_lock(&(file_p->file_tlog_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
	{

        if (file_p->file_tlog_state != TLOG_FILE_OPEN)
	{
            /* 
	     * !!! This should be made into a real bugcheck in the mainline !!!
	     */
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug1,  
		"db_tlog_update(%s,%d,%s,%s,%d,%p) file_tlog_state is not open",
		deb_ascii_file_info(fileBuf,file_p), function,
		deb_ascii_FullName(nameBuf,name_p),
		deb_ascii_ObjUID(uidBuf,uid_p), type, tlog_p));
	}

	if (file_p->file_state != file_recover)
	    {
	    tlog_hdr_t *tlog_buf_p;
	    byte_t *huge_buf_p = 0;	/* Pointer to huge buffer */

	    datalen = LEN_FullName(name_p) + sizeof(Timestamp_u) +
		sizeof(EntryType_u) + sizeof(byte_u) + tlog_p->len;
	    /* round up to the next block */
	    total_blocks = (sizeof(tlog_hdr_t) + datalen + (BLOCK_SIZE - 1))/BLOCK_SIZE;

	    /*
	     * Need to check if the total_blocks for this transaction
	     * is greater than our "normal" write buffer, and if so,
	     * allocate a buffer large enough.
	     */
	    if (total_blocks > TLOG_BUF_BLOCKS)
	        {
		huge_buf_p = (byte_t *)dns_malloc_temp(total_blocks*BLOCK_SIZE);
		if (huge_buf_p == (byte_t *) NULL) {
		    status = DNS_NONSRESOURCES;
		    goto unlock_db_tlog_update;
		    }
		tlog_buf_p = (tlog_hdr_t *) huge_buf_p;
	        }
	    else
	        tlog_buf_p = (tlog_hdr_t *)file_p->file_tlog_buf_p;

	    /* First set up header */
	    tlog_buf_p->t_len = datalen;
	    tlog_buf_p->t_seq = file_p->file_ckpt_hdr.hdr_sequence;
	    tlog_buf_p->t_type = TLOG_UPDATE;
	    tlog_buf_p->t_operation = function;
	    tlog_buf_p->t_rec = type;
	    tlog_buf_p->t_cnt = total_blocks;
	    COPY_ObjUID(uid_p, tlog_buf_p->t_diruid);
	    tlog_buf_p->t_align_mask = ALIGN_MASK;

	    /* pointer after header */
	    COPY_FullName(name_p, &tlog_buf_p[1]);
	    next_p = SKIP_Timestamp(SKIP_FullName(&tlog_buf_p[1]));

	    /* Directory uid not entry */
	    PUT8(next_p, type);		/* entrytype: EntryType */
	    PUT8(next_p, tlog_p->cnt);	/* updateCount: [0..255] */

	    COPY_bytes(tlog_p->buf_p, next_p, tlog_p->len);

	    /* Compute checksum */
	    tlog_buf_p->t_checksum = db_get_checksum(
		(byte_t *)&tlog_buf_p[1], tlog_buf_p->t_len);

	    /* Have to keep pieces together, so synchronize writes */
	    status = sys_safe_write(file_p->file_tlog_block,
			(byte_t *)tlog_buf_p,
			file_p->file_tlog_next_block, total_blocks);

	    if (status == DNS_SUCCESS)
		file_p->file_tlog_next_block += total_blocks;
	    else
		db_checkpoint_schedule(file_p);

	    /* Deallocate the huge buffer if we allocated one */
	    if (huge_buf_p)
	        dns_free(huge_buf_p);
	    }
	}

unlock_db_tlog_update:

        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_tlog_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_tlog_update(%s,%d,%s,%s,%d,%p) status(%s)",
        deb_ascii_file_info(fileBuf,file_p),
        function,
        deb_ascii_FullName(nameBuf,name_p),
        deb_ascii_ObjUID(uidBuf,uid_p),
        type,
        tlog_p,
        deb_ascii_dns_status(statusBuf,status)));
}

/*
 * db_tlog_recover
 *	This routine plays back the transaction log file.
 *
 * Input:
 *	file_p = pointer to file info
 *	crash_p = address to return flag = TRUE if there was a crash
 *					 = FALSE otherwise
 */
dns_status_t 
db_tlog_recover (file_info_t   *file_p,
                 unsigned int  *crash_p)
{
    LOG_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    TLog_statistics_t   tlog_statistics;
    db_stream_t	       *stream_p;
    tlog_hdr_t	       *current_trans_p;     /* Current transaction */
    byte_t             *huge_buf_p = 0;	     /* Pointer to large buffer >
					      * TLOG_BUF_BLOCKS
					      */
    unsigned int	initial_file_size,   /* Tlog file size in blocks */
                        remaining_file_size; /* Decreasing running count */
    unsigned int        huge_buf_size = 0;        /* Size of large buffer */
    unsigned int        blocks_left_in_tbuf = 0;  /* Decreasing running cnt */
    unsigned int        buffer_state = TBUF_EMPTY; /* Read buffer state */
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_tlog_recover(%s,%p)",
        deb_ascii_file_info(fileBuf,file_p),
        crash_p));

    /* Get a stream for updates */
    *crash_p = FALSE;
    status = db_tlog_open(file_p);
    if (status != DNS_SUCCESS) {
	goto leave_db_tlog_recover;
    }

    status = db_get_stream_ignore(file_p, &stream_p, OP_ModifyAttribute);
    if (status != DNS_SUCCESS) {
	(void)db_tlog_close(file_p);
	goto leave_db_tlog_recover;
    }

    initial_file_size = sys_filesize(file_p->file_tlog_block);
    remaining_file_size = initial_file_size;

    /* Initialize the statistics block */
    ZERO_bytes(&tlog_statistics, sizeof(tlog_statistics));
    tlog_statistics.Start = time(NULL);

    /*
     * There are four states in the following state machine:
     *     TBUF_EXIT    - Exit the state machine loop.
     *     TBUF_EMPTY   - Empty read buffer (no transactions to process).
     *     TBUF_PROCESS - Process what is currently in the read buffer.
     *     TBUF_HUGE    - The current transaction is larger than our
     *                    usual read buffer.
     *
     * Key variables are:
     *     remaining_file_size - How many blocks left in the tlog file
     *                           that we have yet to read.
     *     blocks_left_in_tbuf - How many blocks left in our read buffer
     *                           that we have yet to process.
     *
     * Summary:
     * Initial state is TBUF_EMPTY.  We typically read in 16K bytes at a
     * time in the read buffer pointed to by file_p->file_tlog_buf_p.  Once
     * read in, we go to the TBUF_PROCESS state, where we process some
     * number of transactions (call to playback() routine for each
     * transaction) until the read buffer is empty (goto TBUF_EMPTY state)
     * or we discover our current transaction is greater than 16K bytes
     * (goto TBUF_HUGE state).  If in TBUF_HUGE state, we allocate a
     * buffer large enough to hold the entire transaction (or reuse the
     * (previous "huge" buffer if it is large enough), and then goto
     * TBUF_PROCESS state.  We exit when there are no more blocks to
     * read from the tlog file and there are no blocks left to process
     * in our read buffer.
     */
    while ((0 < remaining_file_size) || (0 < blocks_left_in_tbuf)) {
	switch (buffer_state) {

	  case TBUF_EXIT:
	    /*
	     * Note: Under normal processing, the end buffer_state will
	     * be TBUF_EXIT.
	     */
	    break;

	  case TBUF_EMPTY:
	    /*
	     * There are no transactions to process in the read buffer.
	     * Check that there are still transactions in the file.
	     */

	    dce_assert(cds__svc_handle,
		       (blocks_left_in_tbuf == 0));
	    blocks_left_in_tbuf = remaining_file_size;
	    if (blocks_left_in_tbuf > TLOG_BUF_BLOCKS)
	        blocks_left_in_tbuf = TLOG_BUF_BLOCKS;
	    remaining_file_size -= blocks_left_in_tbuf;
	    status = sys_read(file_p->file_tlog_block, file_p->file_tlog_buf_p,
			      file_p->file_tlog_next_block,
			      blocks_left_in_tbuf);
	    if (status != DNS_SUCCESS) {
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug5,
                "  db_tlog_recover() - break: sys_read -> %s",
                deb_ascii_dns_status(statusBuf,status)));
		buffer_state = TBUF_EXIT;
		break;
	    }
	    current_trans_p = (tlog_hdr_t *)file_p->file_tlog_buf_p;
	    buffer_state = TBUF_PROCESS;
	    break;

	  case TBUF_PROCESS:
	    /*
	     * There are transactions to process in the read buffer.
	     */

	    /* If sequence doen't match, crashed before writing this block */
	    if (current_trans_p->t_seq != file_p->file_ckpt_hdr.hdr_sequence) {
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug5,
                "  db_tlog_recover() - break: sequence mismatch, current t_seq = %d, file hdr_sequence = %d",
                current_trans_p->t_seq,
                file_p->file_ckpt_hdr.hdr_sequence));
		buffer_state = TBUF_EXIT;
		break;
	    } /* endif sequence check */

	    if (current_trans_p->t_cnt <= blocks_left_in_tbuf) {
		/*
		 * The entire transaction is in the read buffer, so
		 * process it.
		 */

		if (current_trans_p->t_checksum != db_get_checksum(
		      (byte_t *)&current_trans_p[1], current_trans_p->t_len)) {
		    /*
		     * If doesn't match, must have crashed here.
		     * all done recovery
		     */

                DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_database,
                    svc_c_debug5,
                    "  db_tlog_recover() - break: checksum"));
		    buffer_state = TBUF_EXIT;
		    break;
		} /* endif checksum */

		playback(stream_p, current_trans_p, &tlog_statistics);
		*crash_p = TRUE;
		file_p->file_tlog_next_block += current_trans_p->t_cnt;
		blocks_left_in_tbuf -= current_trans_p->t_cnt;
		dce_assert(cds__svc_handle,
			   (blocks_left_in_tbuf <= initial_file_size));
		if (blocks_left_in_tbuf > 0) {
		    current_trans_p = (tlog_hdr_t *)SKIP_bytes(current_trans_p,
					    current_trans_p->t_cnt*BLOCK_SIZE);
		    buffer_state = TBUF_PROCESS;
		    continue;
		}
		else {
		    buffer_state = TBUF_EMPTY;
		    continue;
		}
	    } /* endif t_cnt <= blocks_left_in_tbuf */

	    else {      /* t_cnt > blocks_left_in_tbuf */
		/*
		 * The entire transaction is not in our buffer, so
		 * since we previously reduced remaining_file_size by the
		 * value of blocks_left_in_tbuf, we must add blocks back
		 * into file size if they were not processed.  That will
		 * happen when:  current_trans_p->t_cnt > blocks_left_in_tbuf
		 */

		remaining_file_size += blocks_left_in_tbuf;
		blocks_left_in_tbuf = 0;    /* Reset */
		dce_assert(cds__svc_handle,
			   (remaining_file_size <= initial_file_size));

		if (current_trans_p->t_cnt > TLOG_BUF_BLOCKS) {
		    /*
		     * t_cnt is also greater than our "normal"-sized buffer,
		     * so go to TBUF_HUGE state.
		     */
		    buffer_state = TBUF_HUGE;
		    continue;
		}
		buffer_state = TBUF_EMPTY;
		continue;
	    } /* end else */
	    break;

	  case TBUF_HUGE:
	    /*
	     * The current transaction is larger than TLOG_BUF_BLOCKS,
	     * so we need to allocate a buffer large enough to hold
	     * the entire transaction.  But first check to see if we
	     * already have a huge buffer that will do the job.
	     */

	    if (huge_buf_p == 0) {
		/*
		 * We haven't allocated a huge buffer, so let's allocate
		 * one large enough for the current transaction.
		 */
		huge_buf_size = current_trans_p->t_cnt;
		huge_buf_p = (byte_t *)
		             dns_malloc_temp(huge_buf_size*BLOCK_SIZE);
		if (huge_buf_p == (byte_t *) NULL) {
		    status = DNS_NONSRESOURCES;
		    buffer_state = TBUF_EXIT;
		    break;
		}

		/*
		 * The buffer_state should remain TBUF_HUGE, as we now
		 * have a large enough buffer and will do the read.
		 */
		buffer_state = TBUF_HUGE;
		continue;
	    } /* endif huge_buf_p == 0 */

	    else if (current_trans_p->t_cnt > huge_buf_size) {
		/*
		 * We already have a huge buffer, but it is not large enough.
		 * Deallocate the old huge buffer and then continue.
		 * The buffer_state should remain TBUF_HUGE, as we'll
		 * allocate a huge buffer the next time around.
		 */

		dns_free(huge_buf_p);
		huge_buf_p = 0;
		huge_buf_size = 0;
		buffer_state = TBUF_HUGE;
		continue;
	    } /* end elseif current_trans_p->t_cnt > huge_buf_size */

	    else {
		/*
		 * Our current huge buffer is large enough to hold the
		 * entire current transaction, so read it in.
		 */

		remaining_file_size -= current_trans_p->t_cnt;
		status = sys_read(file_p->file_tlog_block,
				  huge_buf_p,
				  file_p->file_tlog_next_block,
				  current_trans_p->t_cnt);
		if (status != DNS_SUCCESS) {
                DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_database,
                    svc_c_debug5,
                    "  db_tlog_recover() - break: TBUF_HUGE sys_read -> %s",
                    deb_ascii_dns_status(statusBuf,status)));
		    buffer_state = TBUF_EXIT;
		    break;
		}
		current_trans_p = (tlog_hdr_t *) huge_buf_p;
		blocks_left_in_tbuf = current_trans_p->t_cnt;
		buffer_state = TBUF_PROCESS;
		continue;
	    } /* end else */
	    break;

	  default:

	    dce_svc_printf(CDS_S_REPLAY_BADLOG_MSG, buffer_state);

	    buffer_state = TBUF_EXIT;
	    break;

	} /* endswitch */

	/*
	 * Break out of the while loop if we need to exit.
	 */
	if (buffer_state == TBUF_EXIT)
	  break;

    } /* endwhile */

    /* Deallocate our huge read buffer if one exists */
    if (huge_buf_p)
      dns_free(huge_buf_p);

    /* Record playback completion time */
    tlog_statistics.Finished = time(NULL);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        "  db_tlog_recover() initial_file_size: %d  remaining_file_size: %d",
        initial_file_size,
        remaining_file_size));
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        "  db_tlog_recover() tlog_next_block: %d  blocks_left_in_tbuf: %d",
        file_p->file_tlog_next_block,
        blocks_left_in_tbuf));

    db_free_stream(stream_p);

    if (status != DNS_SUCCESS) {
	(void)db_tlog_close(file_p);
    }

    if (*crash_p) 	
    {

        dce_svc_printf(CDS_S_REPLAY_STATS_MSG,
            deb_ascii_file_info(fileBuf,file_p),
            tlog_statistics.CreDirectory,
            tlog_statistics.CreObject,
            tlog_statistics.CreDoUpdate,
            tlog_statistics.UpdDirectory,
            tlog_statistics.UpdObject,
            tlog_statistics.UpdCH);
    }

leave_db_tlog_recover:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_tlog_recover(%s,%p->%d) return(%s)",
        deb_ascii_file_info(fileBuf,file_p),
        crash_p,
        *crash_p,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
} /* end db_tlog_recover */


/*
 * apply_entry_log
 *	Apply the update packet to an entry.  Update last_uid
 *	if not a do_updates operation.  Do-update uids were not
 *	generated on this node.
 *
 * Input:
 *	rec_p = pointer to data record
 *	update_packet_p = pointer to update packet
 *	flag = TRUE if UTS should be updated
 *	max_p = address to return maximum UID
 */
static void 
apply_entry_log (db_data_t       *rec_p,
                 UpdatePacket_t  *update_packet_p,
                 int             flag,
                 Timestamp_t     *max_p)
{
    DEB_ASCII_BUF_db_data(recBuf)
    DEB_ASCII_BUF_UpdatePacket(update_packetBuf)
    DEB_ASCII_BUF_Timestamp(maxBuf)
    Update_t		*update_p = (Update_t *)SKIP_EntryType(
				SKIP_Timestamp(SKIP_FullName(update_packet_p)));
    int			i, updateCount;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >apply_entry_log(%s,%s,%d,%p)",
        deb_ascii_db_data(recBuf,rec_p),
        deb_ascii_UpdatePacket(update_packetBuf,update_packet_p),
        flag,
        max_p));

    /* loop thru the update packet and apply each update */
    COPY_Timestamp(NullTimestamp, max_p);

    updateCount = EXT8(update_p);
    update_p = (Update_t *)SKIP_byte(update_p);

    for (i = 0; i < updateCount; i++)
	{
	AttributeSpecifier_t *as_p =
		(AttributeSpecifier_t *)update_p->ud_attribute;

	/* Save maximum UID */
	if (dnsCmpCTS((dns_cts *)max_p, (dns_cts *)update_p->ud_timestamp) < 0)
	    COPY_Timestamp(update_p->ud_timestamp, max_p);

	/* Check for delete entry */
	if (EXT8(as_p->as_type) == AT_null)
            {
	    DBSet_t *main_set_p = (DBSet_t *)rec_p->data_p->dbr_data;

	    ALIGN_CHECK(main_set_p);
            CLEAR_FLAG(main_set_p->ds_flag, DS_present);
            COPY_Timestamp(update_p->ud_timestamp, main_set_p->ds_ts);
	    }
	else
	    (void)set_insert(rec_p, update_p,
		find_known_attribute((AttributeName_t *)as_p->as_name), TRUE);

	/* Get next update */
	update_p = (Update_t *)SKIP_Update(update_p);
	}

    if (flag)
	{
	ts_update_last_ts(max_p);
 	(void)set_timestamp_set(rec_p, max_p, dns_Attribute(DNS_UTS),
			    NULL, NULL);
	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <apply_entry_log(%s,%s,%d,%p->%s)",
        deb_ascii_db_data(recBuf,rec_p),
        deb_ascii_UpdatePacket(update_packetBuf,update_packet_p),
        flag,
        max_p,
        deb_ascii_Timestamp(maxBuf,max_p)));
}

/*
 * apply_dir_log
 *	Apply the update packet to a directory.  Update dns_last_uid
 *	as part of recovery.
 *
 * Input:
 *	stream_p = pointer to db stream
 *	update_packet_p = pointer to update packet
 */
static void 
apply_dir_log (db_stream_t     *stream_p,
               UpdatePacket_t  *update_packet_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_UpdatePacket(update_packetBuf)
    Timestamp_u		maxcts;
    Update_t		*update_p = (Update_t *)SKIP_EntryType(
				SKIP_Timestamp(SKIP_FullName(update_packet_p)));
    int			i, updateCount;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >apply_dir_log(%s,%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_UpdatePacket(update_packetBuf,update_packet_p)));

    /* Loop thru the update packet and apply each update */
    COPY_Timestamp(NullTimestamp, maxcts); /* Init maxcts to Null */

    updateCount = EXT8(update_p);
    update_p = (Update_t *)SKIP_byte(update_p);

    for (i = 0; i < updateCount; i++)
	{
	AttributeSpecifier_t *as_p =
		(AttributeSpecifier_t *)update_p->ud_attribute;
	attribute_descriptor_t *dns_p =
		find_known_attribute((AttributeName_t *)as_p->as_name);

	/* Save maximum UID */
	if (!dns_p || (!dns_p->rep_rec && (dns_p->atype != DNS_ALLUPTO)))
	    if (dnsCmpCTS((dns_cts *)maxcts,
			    (dns_cts *)update_p->ud_timestamp) < 0)
		    COPY_Timestamp(update_p->ud_timestamp, maxcts);

        if (dns_p)
            {
	    AtomicValue_t *av_p = (AtomicValue_t *)
		SKIP_BOOLEAN(SKIP_AttributeSpecifier(update_p->ud_attribute));

	    switch (dns_p->atype)
		{
		case DNS_REPLICASTATE:
		    (void)db_dir_set_state(stream_p, EXT8(av_p->av_value),
			NULL, NULL);
		    break;
		case DNS_REPLICATYPE:
		    (void)db_dir_set_type(stream_p, EXT8(av_p->av_value),
			NULL, NULL);
		    break;
		default:
		    (void)set_insert(&stream_p->db_dir, update_p, dns_p, TRUE);
		    break;
		}
            } else
	      (void)set_insert(&stream_p->db_dir, update_p, dns_p, TRUE);


	/* Get next update */
	update_p = (Update_t *)SKIP_Update(update_p);
	}

    if (NE_Timestamp(maxcts, NullTimestamp))
	{
	ts_update_last_ts((Timestamp_t *)maxcts);
 	(void)set_timestamp_set(&stream_p->db_dir, (Timestamp_t *)maxcts,
			dns_Attribute(DNS_UTS), NULL, NULL);
	db_dir_set_lastupdate(stream_p->db_dir_hash_p, (Timestamp_t *)maxcts);
	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <apply_dir_log(%s,%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_UpdatePacket(update_packetBuf,update_packet_p)));
}

/*
 * playback
 *	This routine replays the transactions.  Decisions are made
 *	based on the funcion which generated the log entry and the
 *	type of log entry.  The value of last_uid must be recreated
 *	based on updates in the transaction log.  Only those
 *	not the result of a do_updates operation are considered.
 *	Do_updates timestamps are generated on other nodes and
 *	are not of value to lastuid.
 *      All buffers should be freed on exit.
 *
 *
 * Input:
 *	stream_p = pointer to db_stream
 *	tlog_p = pointer to tlog_hdr
 */
static void 
playback (db_stream_t        *stream_p,
          tlog_hdr_t         *tlog_p,
          TLog_statistics_t  *stat_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_TLog_statistics(statBuf)
    LOG_ASCII_BUF_FullName(nameBuf)
    LOG_ASCII_BUF_dns_status(statusBuf)
    nsgbl_ch_t		*ch_p = stream_p->db_file_p->file_ch_p;
    Timestamp_u		maxcts;
    data_record_t	*data_p, *spare_p = NULL;
    FullName_t		*name_p;
    int			idx, spare_length;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >playback(%s,%p,%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        tlog_p,
        deb_ascii_TLog_statistics(statBuf,stat_p)));

    COPY_Timestamp(NullTimestamp, maxcts);
    name_p = (FullName_t *)&tlog_p[1];
    stream_p->db_dir_hash_p = NULL;

    /* Was a new record created? */
    if (tlog_p->t_type == TLOG_CREATE) {
	switch (tlog_p->t_operation) {
	case OP_CreateDirectory:
	case OP_AddReplica:
            stat_p->CreDirectory++;           /* Bump playback statistics */
	    /* Fill in data pointer */
	    data_p = (data_record_t *)ALIGN_PTR(SKIP_FullName(name_p));
            spare_length = sizeof(*spare_p) - sizeof(spare_p->dbr_data)
			+ LEN_DBSet(data_p->dbr_data);
            stream_p->db_dir.data_buf_len = 
		dns_roundup(spare_length) * BLOCK_SIZE;
            if (!(spare_p = 
                  (data_record_t*)dns_malloc(
		    stream_p->db_dir.data_buf_len))) 
	    {
                dce_svc_printf(CDS_S_REPLAY_DIRMEM_A_MSG, name_p);

                stream_p->db_dir.data_p = NULL;
                stream_p->db_dir.data_buf_len = 0;
		break;
            }
	    COPY_bytes(data_p, spare_p, spare_length);
	    stream_p->db_dir.data_p = spare_p;
	    /* RE-build record */
	    status = db_dir_create_recover(stream_p);
	    if (status == DNS_SUCCESS) {
		/* Update last_uid */
		DBSet_t *set_p;

		if (find_attribute(stream_p->db_dir.data_p,
	             dns_AttributeName(DNS_UTS),
		      &set_p, &idx) == DNS_SUCCESS) {
		    DBSetMember_t *dm_p = (DBSetMember_t *)DATA_DBSet(set_p);

		    /*
		     * DNS_UTS is always a single valued set with no history
		     */
		    ts_update_last_ts((Timestamp_t *)dm_p->dm_data);
		}
	    } else {
                dce_svc_printf(CDS_S_REPLAY_DIRINTERNAL_A_MSG,
                    deb_ascii_FullName(nameBuf,name_p),
                    deb_ascii_dns_status(statusBuf,status));
            }

	    dns_free((char *)stream_p->db_dir.data_p);
            stream_p->db_dir.data_p = NULL;
	    break;

	case OP_CreateObject:
	case OP_CreateLink:
	case OP_CreateChildEntry:
            stat_p->CreObject++;              /* Bump playback statistics */
	    /* Fill in data pointer */
	    data_p = (data_record_t *)ALIGN_PTR(SKIP_FullName(name_p));
	    spare_length = sizeof(*spare_p) - sizeof(spare_p->dbr_data)
			+ LEN_DBSet(data_p->dbr_data);
            stream_p->db_entry.data_buf_len = 
		dns_roundup(spare_length) * BLOCK_SIZE;
	    if (!(spare_p = (data_record_t *)dns_malloc(
		    stream_p->db_entry.data_buf_len))) 	
	    {
                dce_svc_printf(CDS_S_REPLAY_OBJMEM_A_MSG,
			       deb_ascii_FullName(nameBuf,name_p));

                stream_p->db_entry.data_p = NULL;
                stream_p->db_entry.data_buf_len = 0;
		break;
	    }
	    COPY_bytes(data_p, spare_p, spare_length);
	    stream_p->db_entry.data_p = spare_p;
	    /* Re-build record */
	    status = db_entry_create_recover(stream_p, name_p, 
	     		tlog_p->t_rec, (ObjUID_t *)tlog_p->t_diruid);
	    if (status == DNS_SUCCESS) {
		/* Update lastupdate and last_uid */
		DBSet_t *set_p;

		if (find_attribute(stream_p->db_entry.data_p,
			    dns_AttributeName(DNS_UTS),
			    &set_p, &idx) == DNS_SUCCESS) {
		    DBSetMember_t *dm_p = (DBSetMember_t *)DATA_DBSet(set_p);

		    /*
		     * DNS_UTS is always a single valued set with no history
		     */
		    ts_update_last_ts((Timestamp_t *)dm_p->dm_data);
	  	    db_dir_set_lastupdate(stream_p->db_dir_hash_p,
				(Timestamp_t *)dm_p->dm_data);
		}
	    } 
	    else 
	    {
                dce_svc_printf(CDS_S_REPLAY_OBJINTERNAL_A_MSG,
                    deb_ascii_FullName(nameBuf,name_p),
                    deb_ascii_dns_status(statusBuf,status));
            }
	    dns_free((char *)stream_p->db_entry.data_p);
            stream_p->db_entry.data_p = NULL;
	    break;

	case OP_DoUpdate:
            stat_p->CreDoUpdate++;            /* Bump playback statistics */
	    /* Fill in data pointer */
	    data_p = (data_record_t *)ALIGN_PTR(SKIP_FullName(name_p));
	    spare_length = sizeof(*spare_p) - sizeof(spare_p->dbr_data)
			+ LEN_DBSet(data_p->dbr_data);
            stream_p->db_entry.data_buf_len = 
		dns_roundup(spare_length) * BLOCK_SIZE;
	    if (!(spare_p = (data_record_t *)dns_malloc(
		    stream_p->db_entry.data_buf_len))) 
	    {
		/* ABORT */
                dce_svc_printf(CDS_S_REPLAY_OBJMEM_A_MSG, name_p);

                stream_p->db_entry.data_p = NULL;
                stream_p->db_entry.data_buf_len = 0;
		break;
	    }
	    COPY_bytes(data_p, spare_p, spare_length);
	    stream_p->db_entry.data_p = spare_p;
	    /* RE-build record */
	    status = db_entry_create_recover(stream_p, name_p, 
			tlog_p->t_rec, (ObjUID_t *)tlog_p->t_diruid);
	    if (status != DNS_SUCCESS) 	
	    {
                dce_svc_printf(CDS_S_REPLAY_OBJINTERNAL_A_MSG,
                    deb_ascii_FullName(nameBuf,name_p),
                    deb_ascii_dns_status(statusBuf,status));

            }
	    dns_free((char *)stream_p->db_entry.data_p);
            stream_p->db_entry.data_p = NULL;
	    break;

	default:
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug1,  
		"playback(%s) bug generated line %d", 
                streamBuf, 
                __LINE__));

	    status = DNS_NAMESERVERBUG;
	}
    } else if (tlog_p->t_type == TLOG_UPDATE) {
	switch (tlog_p->t_rec) {
	case ET_directory:
            stat_p->UpdDirectory++;           /* Bump playback statistics */
	    /* X.500 changing case isn't sufficient for typed names */
	    status = dns_normalize_fname(name_p,
			(FullName_t *)stream_p->db_key,
			sizeof(stream_p->db_key));
	    if (status == DNS_SUCCESS) {
		stream_p->db_dir.data_type = ET_directory;
		status = db_dir_read_for_update(stream_p);
	    }
	    if (status != DNS_SUCCESS) 
	    {
		dce_svc_printf(CDS_S_REPLAY_DIRINTERNAL_A_MSG,
                    deb_ascii_FullName(nameBuf,name_p),
                    deb_ascii_dns_status(statusBuf,status));

            } else {
		/* 
                 * Treat as a directory entry, no per replica attributes
		 * are passed in a doupdate operation
		 */
		if (tlog_p->t_operation == OP_DoUpdate) {
		        apply_entry_log(&stream_p->db_dir,
			(UpdatePacket_t *)name_p, FALSE,
			(Timestamp_t *)maxcts);
		 } else {
		    /* Need to special case attributes */
		    apply_dir_log(stream_p, (UpdatePacket_t *)name_p);
		 }
		 (void)db_dir_update(stream_p, NULL);
	    }
	    db_dir_release(stream_p);
	    break;

	case ET_clearinghouse:
            stat_p->UpdCH++;                  /* Bump playback statistics */
	    /* X.500 changing case isn't sufficient for typed names */
	    status = dns_normalize_fname(name_p,
			(FullName_t *)stream_p->db_key,
			sizeof(stream_p->db_key));
	    if (status == DNS_SUCCESS) {
		stream_p->db_dir.data_type = ET_clearinghouse;
		status = db_pseudo_read_for_update(stream_p);
	    }
	    if (status != DNS_SUCCESS) 
	    {
		dce_svc_printf(CDS_S_REPLAY_DIRINTERNAL_A_MSG,
                    deb_ascii_FullName(nameBuf,name_p),
                    deb_ascii_dns_status(statusBuf,status));

	    } else {
		apply_entry_log(&stream_p->db_dir,
			        (UpdatePacket_t *)name_p, TRUE,
			        (Timestamp_t *)maxcts);
		(void)db_pseudo_update(stream_p, NULL);
	    }

	    db_dir_release(stream_p);
	    break;

	default:
            stat_p->UpdObject++;              /* Bump playback statistics */
	    /* X.500 changing case isn't sufficient for typed names */
	    status = dns_normalize_fname(name_p,
			(FullName_t *)stream_p->db_key,
			sizeof(stream_p->db_key));
	    if (status == DNS_SUCCESS) {
		stream_p->db_entry.data_type = tlog_p->t_rec;
		status = db_entry_read_for_update(stream_p);
	    }
	    if (status != DNS_SUCCESS) 
	    {
		dce_svc_printf(CDS_S_REPLAY_OBJINTERNAL_A_MSG,
                    deb_ascii_FullName(nameBuf,name_p),
                    deb_ascii_dns_status(statusBuf,status));

	    } else if (tlog_p->t_operation != OP_DoUpdate) {
		apply_entry_log(&stream_p->db_entry,
		 	        (UpdatePacket_t *)name_p, TRUE,
			        (Timestamp_t *)maxcts);
		(void)db_entry_update(stream_p, NULL);
		db_dir_set_lastupdate(stream_p->db_dir_hash_p,
			              (Timestamp_t *)maxcts);
            } else {
		apply_entry_log(&stream_p->db_entry,
			        (UpdatePacket_t *)name_p, FALSE,
			        (Timestamp_t *)maxcts);
		(void)db_entry_update(stream_p, NULL);
	    }

	    db_entry_release(stream_p);
	    break;
	}
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <playback(%s,%p,%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        tlog_p,
        deb_ascii_TLog_statistics(statBuf,stat_p)));
}


