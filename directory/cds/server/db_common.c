/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_common.c,v $
 * Revision 1.1.16.3  1996/02/18  19:35:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:14  marty]
 *
 * Revision 1.1.16.2  1995/12/08  15:29:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:49  root]
 * 
 * Revision 1.1.12.1  1994/10/25  16:17:40  proulx
 * 	Clearinghouse towers are now checked in the background.
 * 	Remove fix_ch_tower() and write_ch_obj_tower() - no longer used.
 * 	towerset_compare_exact_mx() has moved to back_ground.c
 * 	[1994/10/25  15:04:53  proulx]
 * 
 * Revision 1.1.9.9  1994/08/24  20:14:13  mccann
 * 	fix server shutdown, add annotation strings, start message delay:OT11239,11245 and 11766
 * 	[1994/08/24  20:08:00  mccann]
 * 
 * Revision 1.1.9.8  1994/08/19  21:10:45  zee
 * 	Accommodate new file state for the BadClock Fixer.  Don't allow the
 * 	clearinghouse to be re-enabled during BadClock repair.  Also, moved
 * 	initialization of "found" variable in towerset_compare_exact_mx inside
 * 	of loop to fix a bug where the database will show more towers than
 * 	are actually there.
 * 	[1994/08/19  18:09:10  zee]
 * 
 * Revision 1.1.9.7  1994/08/16  18:28:01  jd
 * 	drop from janet
 * 	[1994/08/13  16:07:29  jd]
 * 
 * Revision 1.1.9.6  1994/08/03  19:03:52  mccann
 * 	final sams drop
 * 	[1994/08/02  14:09:12  mccann]
 * 
 * Revision 1.1.9.5  1994/08/01  15:07:33  zee
 * 	     Add create/delete file_back_done and file_ckpt_done condition variables.
 * 	[1994/07/29  21:04:14  zee]
 * 
 * Revision 1.1.9.4  1994/06/30  19:18:10  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:48:10  mccann]
 * 
 * Revision 1.1.9.3  1994/06/09  18:43:05  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:29  devsrc]
 * 
 * Revision 1.1.9.2  1994/04/14  14:51:08  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:19:18  peckham]
 * 
 * Revision 1.1.9.1  1994/03/12  22:10:37  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:40:18  peckham]
 * 
 * Revision 1.1.6.2  1993/07/30  20:07:10  jd
 * 	Force the close of tlog in db_clear so file descriptors don't get
 * 	all used up.
 * 	[1993/07/30  20:01:37  jd]
 * 
 * Revision 1.1.4.2  1992/12/30  14:20:28  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:20:33  zeliff]
 * 
 * Revision 1.1.2.4  1992/07/17  18:11:33  peckham
 * 	CR#2681: Avoid checkpoint deadly embrace.
 * 	  Add support for file_info_t.file_op_info.
 * 	  Add refresh_chdirectories() and use in db_enable() to insure
 * 	  CHDirectories is reasonable before setting clearinghouse
 * 	  state to open.
 * 	  db_begin_operation() and db_end_operation() bracket each operation
 * 	  in rpc_dispatch.c to avoid deadlocks.
 * 	[1992/07/17  18:10:23  peckham]
 * 
 * Revision 1.1.2.3  1992/06/04  16:02:54  mfox
 * 	Close tlog before deleting it, otherwise it is left around.
 * 	[1992/06/03  17:27:26  mfox]
 * 
 * Revision 1.1.2.2  1992/03/22  22:28:57  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:34:37  weisman]
 * 
 * Revision 1.1  1992/01/19  15:25:45  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*  db_common.c
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
 * Module Description
 *
 * This module contains routines used to open and access the databases
 *
 */

#include <server.h>
#include <dce/assert.h>
#include <back.h>
#include <clerk.h>
#include <dbdef.h>
#include <find.h>
#include <sets.h>
#include <tlog.h>
#include <uid.h>
#include <dce/dce_cf.h>

/* Queue head for circular list, initialize to point at self, empty */
list_header_t db_head = {&db_head, &db_head};


/*
 * Local prototypes
 */
static dns_status_t
get_stream (
    file_info_t         *,
    db_stream_t         **,
    int                 ,
    int                   );
#define NORMAL_MODE   1
#define IGNORE_MODE   2

static void
refresh_chdirectories (
   file_info_t         *);

static void
init_current_op_info  (
    current_op_info_t *);

static void
free_current_op_info  (
    current_op_info_t *);

/* -------------------------------------------------------------------------- */

/*
 *                    Instrumentation Support Routines
 */

#include <string.h>
/*
 * deb_ascii_nsgbl_ns
 *      Produce instrumentation display for nameserver.
 */
char 
*deb_ascii_nsgbl_ns (char *const        buf,
                     const void *const  ptr)
{
    const nsgbl_ns_t *const ns_p = (const nsgbl_ns_t *)ptr;

    if (ns_p) {
        (void)sprintf(buf, DEB_ASCII_FMT_ptr_TO, ns_p);
	return(buf);
    } else
        return("NULL nsgbl_ns");
}

/*
 * deb_ascii_nsgbl_ch
 *      Produce instrumentation display for clearinghouse.
 */
char 
*deb_ascii_nsgbl_ch (char *const        buf,
                     const void *const  ptr)
{
    const nsgbl_ch_t *const ch_p = (const nsgbl_ch_t *)ptr;

    if (ch_p) {
        (void)sprintf(buf, DEB_ASCII_FMT_ptr_TO, ch_p);
        (void)deb_ascii_FullName(strchr(buf, '\0'), &ch_p->ch_name);
	return(buf);
    } else
        return("NULL nsgbl_ch");
}

/*
 * deb_ascii_user_descriptor
 *      Produce instrumentation display for user.
 */
char 
*deb_ascii_user_descriptor (char *const        buf,
                            const void *const  ptr)
{
    const user_descriptor_t *const user_p = (const user_descriptor_t *)ptr;

    if (user_p) {
        (void)sprintf(buf, DEB_ASCII_FMT_ptr_TO, user_p);
	return(buf);
    } else
        return("NULL user_descriptor_t");
}

/*
 * deb_ascii_dir_hash_entry
 *      Produce instrumentation display for directory.
 */
char 
*deb_ascii_dir_hash_entry (char *const        buf,
                           const void *const  ptr)
{
    const dir_hash_entry_t *const hash_p = (const dir_hash_entry_t *)ptr;

    if (hash_p) {
        (void)sprintf(buf, DEB_ASCII_FMT_ptr_TO, hash_p);
        (void)deb_ascii_FullName(strchr(buf, '\0'), hash_p->dir_name);
	return(buf);
    } else
        return("NULL dir_hash_entry_t");
}

/*
 * deb_ascii_db_stream
 *      Produce instrumentation display for stream.
 */
char 
*deb_ascii_db_stream (char *const        buf,
                      const void *const  ptr)
{
    const db_stream_t *const stream_p = (const db_stream_t *)ptr;

    if (stream_p) {
        (void)sprintf(buf, DEB_ASCII_FMT_ptr_TO, stream_p);
        (void)deb_ascii_FullName(strchr(buf, '\0'), stream_p->db_key);
	return(buf);
    } else
        return("NULL db_stream_t");
}

/*
 * deb_ascii_file_info
 *      Produce instrumentation display for clearinghouse file.
 */
char 
*deb_ascii_file_info (char *const        buf,
                      const void *const  ptr)
{
    const file_info_t *const file_p = (const file_info_t *)ptr;

    if (file_p) {
        (void)sprintf(buf, DEB_ASCII_FMT_ptr_TO, file_p);
        if (file_p->file_ch_p)
            (void)deb_ascii_ObjUID(strchr(buf, '\0'),
					&file_p->file_ch_p->ch_uid);
        else
            (void)strcat(buf, "*no CHID*");

	return(buf);
    } else
        return("NULL file_info_t");
}

/* -------------------------------------------------------------------------- */

/*
 * get_stream
 *      Allocate a stream descriptor and initialize it.   The state
 *      of the clearinghouse can be ignored if desired.
 *      Is is called by internal processing
 *      routines, transactions will call open stream and only
 *      call this if there is a need for an additional stream.
 *
 * Input:
 *      file_p = pointer to file_info (locked)
 *      stream_pp = address of pointer to return stream
 *      function = calling function
 *              Non-writing function are set to READ_ATT_OP
 *              Most writes are converted to MODIFY_ATT_OP
 *                exceptions are do_updates, creates and add_replica
 *                because they require special handling in recovery
 *
 * Side Effects:
 *      Streams are freed by db_free_stream
 */
static dns_status_t 
get_stream (file_info_t  *file_p,
            db_stream_t  **stream_pp,
            int          function,
            int          usage_mode)
{
    db_stream_t *stream_p = NULL;
    dns_status_t status = DNS_NAMESERVERBUG;
    int thstatus;


    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {

        /*
         * If currently checkpointing this file, writers must be stalled
         * though reading is permitted.   Any other state (besides open and
         * ckpt) results in a ClearinghouseDown exception - UNLESS this
         * open was issued during a database recovery (recovery opens have
         * a direct path in regardless of the database state).
         */

        while ((file_p->file_state != file_open) &&
               (usage_mode == NORMAL_MODE))
            {
            if ((file_p->file_state == file_ckpt) &&
                (function == OP_ReadAttribute))
                {
                break;
                }
            if (file_p->file_state != file_ckpt)
                {
                status = DNS_CLEARINGHOUSEDOWN;
		goto unlock_db_get_stream;
                }
            {
                if ((thstatus = dthread_wait(&(file_p->file_state_changed),
                                             &(file_p->file_mutex))) < 0)
                {
		  dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
                }
            }
            }

        /*
         * At this point either the file_state is open, or it is
         * checkpointing and we have a Read transaction requested.
         */

        /* Check if there are any free streams currently available */
        stream_p = file_p->file_free_stream_p;
        if (stream_p)
            {
            /* Stream exists, re-use it */
            file_p->file_free_stream_p = stream_p->db_next_p;
            }
        else
            {
            /* None free, allocate stream descriptor for caller */
            stream_p = (db_stream_t *)dns_malloc_temp(sizeof(db_stream_t));
            }

        *stream_pp = stream_p;

        if (stream_p)
            {
            /* Initialize stream */
            ZERO_bytes(stream_p, sizeof(db_stream_t));
            COPY_FullName(NullFullName, stream_p->db_key);
            stream_p->db_owner = dthread_self();
            stream_p->db_function = function;
            stream_p->db_file_p = file_p;
            file_p->file_users++;
            /*
             * Need to maintain write count so we know when
             * to safely initiate a checkpoint operation
             */
            switch (function)
                {
                default:
                    file_p->file_write_count++;
                case OP_ReadAttribute:
                    break;
                }
            status = DNS_SUCCESS;
            }
        else
            {
            status = DNS_NONSRESOURCES;
            }
        }
unlock_db_get_stream:

       /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
}

    return (status);
}


dns_status_t 
db_get_stream (file_info_t  *file_p,
               db_stream_t  **stream_pp,
               int          function)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_get_stream(%s,%p,%d)",
        deb_ascii_file_info(fileBuf,file_p),
        stream_pp,
        function));

    status = get_stream(file_p, stream_pp, function, NORMAL_MODE);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_get_stream(%s,%p,%d) return(%s)",
        deb_ascii_file_info(fileBuf,file_p),
        stream_pp,
        function,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

dns_status_t 
db_get_stream_ignore (file_info_t  *file_p,
                      db_stream_t  **stream_pp,
                      int          function)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_get_stream_ignore(%s,%p,%d)",
        deb_ascii_file_info(fileBuf,file_p),
        stream_pp,
        function));

    status = get_stream(file_p, stream_pp, function, IGNORE_MODE);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_get_stream_ignore(%s,%p,%d) return(%s)",
        deb_ascii_file_info(fileBuf,file_p),
        stream_pp,
        function,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}



/* -------------------------------------------------------------------------- */

/*
 * db_free_stream
 *      Return a stream descriptor to the list of free streams
 *      ready for another caller to use.  This routine should
 *      parellel calls to db_get_stream.
 *
 *
 * Input:
 *      stream_p = pointer to stream
 *
 * Side Effects
 *      Streams are allocated by db_get_stream
 */
void 
db_free_stream (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    file_info_t *const file_p = stream_p->db_file_p;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_free_stream(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    dthread_assign(dthread_null, &stream_p->db_owner);

    if (file_p)
    {

        if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
        {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
        }

         /* Beginning of the 'protected code' */
            {
            /* Insert this stream in list of freed streams */
            stream_p->db_next_p = file_p->file_free_stream_p;
            file_p->file_free_stream_p = stream_p;

            /* If this was not a read operation, decrement writer count */
            switch (stream_p->db_function) {
            default:
                if (--file_p->file_write_count == 0)
                    {
                        if ((thstatus = dthread_broadcast(&(file_p->file_state_changed))) < 0)
                        {
			  dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, 
					 (long)thstatus);
                        }
                    }
            case OP_ReadAttribute:
                break;
            }

            /* Indicate one less use of the file */
            if (--file_p->file_users == 0)
                {
                    if ((thstatus = dthread_broadcast(&(file_p->file_state_changed))) < 0)
                    {
		      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, 
				     (long)thstatus);
                    }
                }
            }
            /* End of the 'protected code' */

	if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
	{
	  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	}
    }
    else
        dns_free((char *)stream_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_free_stream(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));
}

/* -------------------------------------------------------------------------- */

/*
 * db_open_stream
 *      Locate already enabled clearinghouse return a stream to it.
 *      This routine is called whenever a new transaction is
 *      initiated.  It will return errors if the clearinghouse
 *      is not enabled.
 *
 * Input:
 *      chid_p = pointer to clearinghouse id
 *      stream_pp = address of pointer to return stream
 *      function = calling function
 *
 */
dns_status_t 
db_open_stream (ObjUID_t     *chid_p,
                db_stream_t  **stream_pp,
                int          function)
{
    DEB_ASCII_BUF_ObjUID(chidBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    file_info_t         *file_p;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_open_stream(%s,%p,%d)",
        deb_ascii_ObjUID(chidBuf,chid_p),
        stream_pp,
        function));

    /* If match was not found, clearinghouse doesn't exist */
    if ((file_p = db_find_file(chid_p)) != NULL) {
	status = db_get_stream(file_p, stream_pp, function);
    } else {
        status = DNS_UNKNOWNCLEARINGHOUSE;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_open_stream(%s,%p,%d) return(%s)",
        deb_ascii_ObjUID(chidBuf,chid_p),
        stream_pp,
        function,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/* -------------------------------------------------------------------------- */

/*
 * db_close_stream
 *      Free a stream previously gotten by db_open_stream.
 *      Release dynamic memory still held by stream.
 *
 * Input:
 *      stream_p = pointer to stream descriptor
 *
 * Side Effects:
 *      memory released that was allocated by the caller thread.
 */
dns_status_t 
db_close_stream (db_stream_t *stream_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_close_stream(%s)",
        deb_ascii_db_stream(streamBuf,stream_p)));

    /* Cleanup remaining data */
    db_dir_release(stream_p);
    db_entry_release(stream_p);
    db_free_stream(stream_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_close_stream(%s) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_dns_status(statusBuf,DNS_SUCCESS)));
    return(DNS_SUCCESS);
}

/* -------------------------------------------------------------------------- */

/*
 * db_modify_stream_function
 *
 * This routine changes the function of a stream, and modifies the writer
 * count appropriately.   It abides by get_stream rules for write access
 * during checkpoints.
 *
 * Returns:
 *    DNS_SUCCESS - function changed (old one returned via old_function_p);
 *    DNS_CLEARINGHOUSEDOWN
 */
dns_status_t 
db_modify_stream_function (db_stream_t  *stream_p,
                           int          new_function,
                           int          *old_function_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    file_info_t *const file_p = stream_p->db_file_p;
    dns_status_t status = DNS_SUCCESS;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_modify_stream_function(%s,%d,%p)",
        deb_ascii_db_stream(streamBuf,stream_p),
        new_function,
        old_function_p));

    if (file_p) {

        if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
        {
	  dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
        }

         /* Beginning of the 'protected code' */

            /*
             * If the old function was not a read operation, decrement
             * writer count
             */
            switch (stream_p->db_function) {
            default:
                file_p->file_write_count--;
            case OP_ReadAttribute:
                break;
            }


            /*
             * If currently checkpointing this file, writers must be stalled
             * though reading is permitted.   Any other state (besides open and
             * ckpt) results in a ClearinghouseDown exception.
             */
            while (file_p->file_state != file_open) 
	    {
                if ((file_p->file_state == file_ckpt) &&
                    (new_function == OP_ReadAttribute)) {
                    break;
                }
                if (file_p->file_state != file_ckpt) {
                    status = DNS_CLEARINGHOUSEDOWN;
		    goto unlock_db_modify_stream_function;
                }
		
		if ((thstatus = dthread_wait(&(file_p->file_state_changed),
					     &(file_p->file_mutex))) < 0)
		{
		  dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
                }
            }

            /*
             * If new function is not read, increment writer count.
             */
            switch (new_function) {
            default:
                file_p->file_write_count++;
            case OP_ReadAttribute:
                break;
            }

            *old_function_p = stream_p->db_function;
            stream_p->db_function = new_function;

            if (file_p->file_write_count == 0) 
	    {
		if ((thstatus = dthread_broadcast(&(file_p->file_state_changed))) < 0)
		{
		  dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, 
				 (long)thstatus);
                }
            }

unlock_db_modify_stream_function:

	/* End of the 'protected code' */
	
	if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
	{
	  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	}
    } 
    else 
    {
        *old_function_p = stream_p->db_function;
        stream_p->db_function = new_function;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_modify_stream_function(%s,%d,%p->%d) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        new_function,
        old_function_p,
        *old_function_p,
        deb_ascii_dns_status(statusBuf,status)));

    return (status);
}


/* -------------------------------------------------------------------------- */

/*
 * db_assert_active_stream
 *
 * Checks that the stream is active and executing under the proper thread.
 *
 */

void 
db_assert_active_stream (db_stream_t  *stream_p,
                         char         *module_p,
                         int          line_number)
{
    LOG_ASCII_BUF_db_stream(streamBuf)

    if (dthread_equal(dthread_null, stream_p->db_owner))
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug1,
            "Active stream: %s - NULL owner at %s/%d",
            deb_ascii_db_stream(streamBuf,stream_p),
            module_p,
            line_number));
    } 
    else
	if (!dthread_equal(stream_p->db_owner, dthread_self()))
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug1,
                "Active stream: %s - MIXED owner(s:%x/ct:%x) at %s/%d",
                deb_ascii_db_stream(streamBuf,stream_p),
                stream_p->db_owner,
                dthread_self(),
                module_p,
                line_number));
	}
}




/* -------------------------------------------------------------------------- */

/*
 * db_find_file
 *      This routine locates the file_info structure for a
 *      clearinghouse in the list of known files.
 *
 * Input:
 *      id_p = pointer to clearinghouse ID
 *
 */
file_info_t 
*db_find_file (ObjUID_t *id_p)
{
    DEB_ASCII_BUF_ObjUID(idBuf)
    DEB_ASCII_BUF_file_info(fileBuf)
    file_info_t         *file_p;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_find_file(%s)",
        deb_ascii_ObjUID(idBuf,id_p)));

    if ((thstatus = dthread_lock(&(dns_server_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {

        /*
         * Search circular list for a match.  Done when find a match or
         * there are not more entries in the queue to check
         */
        for (file_p = (file_info_t *)db_head.next_p;
             file_p != (file_info_t *)&db_head;
             file_p = (file_info_t *)file_p->file_queue.next_p)
            {
            /*
             * First we check the against the CHID of the clearinghouse.
             * If that fails, we see if it matches any of the alternate
             * CHCTS's for this clearinghouse (generated when the clearinghouse
             * object is accidently deleted).
             */
            if (EQ_ObjUID(&file_p->file_ch_p->ch_uid, id_p))
                break;
           }

        }
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(dns_server_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    if (file_p == (file_info_t *)&db_head)
        file_p = NULL;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_find_file(%s) return(%s)",
        deb_ascii_ObjUID(idBuf,id_p),
        deb_ascii_file_info(fileBuf,file_p)));
    return(file_p);
}


/* -------------------------------------------------------------------------- */

/*
 * db_disable
 *      Release resources allocated to an enabled clearinhgouse.  Leave
 *      the data in memory and set the state to db_loaded
 *
 * Input:
 *      file_p = pointer to clearinghouse file info
 *
 */
dns_status_t 
db_disable (ObjUID_t *chid_p)
{
    DEB_ASCII_BUF_ObjUID(chidBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    file_info_t         *file_p;
    dns_status_t	status;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_disable(%s)",
        deb_ascii_ObjUID(chidBuf,chid_p)));

    /*
     * Locate the clearinghouse in the list of known clearinghouses.
     * Wait for it to out of checkpoint or recovery mode
     */
    for (;;)
        {
        /*
         * Have to keep finding file incase it was closed and
         * cleaned up  by another thread
         */
        file_p = db_find_file(chid_p);
        if (!file_p) {
	    status = DNS_UNKNOWNCLEARINGHOUSE;
	    goto leave_db_disable;
	}

        if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
        {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
        }

         /* Beginning of the 'protected code' */
            {
            if (file_p->file_state == file_open)
                {
                /* Prevent further transactions from starting */
                file_p->file_state = file_loaded;
                {
                    if ((thstatus = dthread_broadcast(&(file_p->file_state_changed))) < 0)
                    {
		      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, 
				     (long)thstatus);
                    }
                }
                }
            }
 	    /* End of the 'protected code' */

	if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
	{
	  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	}

        /* make sure background is inactive */
        (void)background_stop(file_p);

        if (file_p->file_state == file_loaded) break;

        /* Wait a second to let file state change by another thread */
	
	if ((thstatus = dthread_delay(1.0)) < 0)
	{
	  dce_svc_printf(CDS_S_PTHREADDELAY_A_MSG, (long)thstatus);
        }
    }

    /*
     * The stream allocated to propagates prevents the open_count from
     * going to 0.  Free that stream if there are no propagates
     * queued.  If there are propagates queued, they will notice
     * the file state and close the stream which will force the count
     * to be decremented and this operation to complete
     */
    (void)back_propagate_stop(file_p);

    /* Wait until this thread is the only one */

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }
        /* Beginning of the 'protected code' */
        {
	    while (0 < file_p->file_users)
            {
                if ((thstatus = dthread_wait(&(file_p->file_state_changed),
                                             &(file_p->file_mutex))) < 0)
                {
		  dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
                }
            }
        }
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    /* UnRegister with clerkserver interface */
    (void)rpc_unregister_ch(file_p->file_ch_p->ch_nameserver->ns_ta_id, 
			    chid_p);

    /* Save in-memory database on stable storage */
    status = db_checkpoint(file_p);

leave_db_disable:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_disable(%s) return(%s)",
        deb_ascii_ObjUID(chidBuf,chid_p),
        deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

/* -------------------------------------------------------------------------- */

/*
 * db_exit
 *      This routine releases virtual memory resources used by
 *      all known clearinghouses.
 *
 * Input:
 *      ns_p = namespace global structure
 * Output:
 *      none explicit
 * Side Effects:
 *      db_head queue is emptied
 */
void 
db_exit (nsgbl_ns_t *ns_p)
{
    DEB_ASCII_BUF_nsgbl_ns(nsBuf)
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_exit(%s)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p)));

    /* Loop through list of files until queue is empty */
    while (TRUE)
    {
        file_info_t             *file_p;

        if ((thstatus = dthread_lock(&(dns_server_mutex))) < 0)
        {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
        }
            /* Beginning of the 'protected code' */

            file_p = (file_info_t *)LIST_FIRST(&db_head);

	    /* End of the 'protected code' */

	if ((thstatus = dthread_unlock(&(dns_server_mutex))) < 0)
	{
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	}

        if (!file_p) break;

        /* Make sure it is disabled */
        if (file_p->file_state != file_loaded)
            (void)db_disable(&file_p->file_ch_p->ch_uid);

        /* Close transaction log file */
        (void)db_tlog_close(file_p);

        /* Free memory resources used by clearinghouse */
        file_info_free(file_p);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_exit(%s)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p)));
}

/* -------------------------------------------------------------------------- */

/*
 * db_init_ch
 *      This routine initialize the nsgbl_ch structure representing
 *      a clearinghouse with the ch_uid, ch_name, ch_nsnickname
 *      and ch_nsuid all found in the pseudo directory.  This
 *      information is kept in a structure available to
 *      entity management.
 *
 * Input:
 *      hash_p = pointer to hash entry for pseudo directory
 *      ch_p = pointer to nsgbl_ch structure to fill in.
 *
 */
void 
db_init_ch (dir_hash_entry_t  *hash_p,
            nsgbl_ch_t        *ch_p)
{
    DEB_ASCII_BUF_dir_hash_entry(hashBuf)
    DEB_ASCII_BUF_nsgbl_ch(chBuf)
    DBSet_t             *set_p;
    SimpleName_t        *sn_p = (SimpleName_t *)NullSimpleName;
    int                 idx;
    int                 len;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_init_ch(%s,%s)",
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_nsgbl_ch(chBuf,ch_p)));

    /* Copy clearinghouse id */
    COPY_ObjUID(hash_p->dir_uid, &ch_p->ch_uid);

    /* Copy clearinghouse name */
    COPY_FullName(hash_p->dir_name, &ch_p->ch_name);

    /* Locate cellname */
    if (find_attribute((data_record_t *)hash_p->dir_p->dir_data,
            dns_AttributeName(DNS_NSCELLNAME), &set_p, &idx) == DNS_SUCCESS) {
        /* Find last valid value */
	DBSetMember_t *const val_p = find_last_value(set_p);

        if (val_p)
            switch (EXT8(val_p->dm_valuetype)) {
            case VT_char:
                /* Move cellname to nsgbl_ch structure */
                ch_p->ch_cellname_p = (char *)malloc(val_p->dm_length+1);
                if (ch_p->ch_cellname_p) {
                    COPY_bytes(val_p->dm_data,
                            ch_p->ch_cellname_p, val_p->dm_length);
                    ch_p->ch_cellname_p[val_p->dm_length] = '\0';
                    /* Create the opaque version */
                    idx = sizeof(ch_p->ch_cellname);
                    (void)dnsCvtCDSFullToOpq(
				(unsigned char *)ch_p->ch_cellname_p,
				(dns_full_name_t *)&ch_p->ch_cellname, &idx);
                }
                break;
            case VT_FullName:
                COPY_FullName(val_p->dm_data, &ch_p->ch_cellname);
                idx = 511;
                while (TRUE) {
                    ch_p->ch_cellname_p = (char *)malloc(idx+1);
                    if (!ch_p->ch_cellname_p) break;
                    if (dnsCvtCDSFullToStr((dns_full_name_t *)
                                                        &ch_p->ch_cellname,
                                (unsigned char *)ch_p->ch_cellname_p, &idx)
                                                                == DNS_SUCCESS)
                        break;
                    free(ch_p->ch_cellname_p);
                }
                break;
            }
    } else {
        error_status_t dce_status;
        /* Migration: supply new cellname if not in database */

        dce_cf_get_cell_name(&ch_p->ch_cellname_p, &dce_status);
        if (dce_status == dce_cf_st_ok) {
            idx = sizeof(ch_p->ch_cellname);
            (void)dnsCvtCDSFullToOpq((unsigned char *)ch_p->ch_cellname_p,
                        (dns_full_name_t *)&ch_p->ch_cellname, &idx);
        } else
            ch_p->ch_cellname_p = NULL;
    }

    /* the HACK NSNICKNAME is the cellname */
    INS8(ch_p->ch_nsnickname.sn_flag, SN_cds);
    INS8(ch_p->ch_nsnickname.sn_length,
		len = strlen(ch_p->ch_cellname_p));
    COPY_bytes(ch_p->ch_cellname_p, ch_p->ch_nsnickname.sn_name, len);

    /* Namespace id is root of clearinghouse fullname */
    COPY_ObjUID(ch_p->ch_cellname.fn_root, &ch_p->ch_nsuid);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_init_ch(%s,%s)",
        deb_ascii_dir_hash_entry(hashBuf,hash_p),
        deb_ascii_nsgbl_ch(chBuf,ch_p)));
}

/* -------------------------------------------------------------------------- */

/*
 * file_info_malloc
 *      Allocate and initialize file descriptor structure
 *
 * Input:
 *      ch_p = address of clearinghouse descriptor
 */
file_info_t 
*file_info_malloc (nsgbl_ch_t *ch_p)
{
    DEB_ASCII_BUF_nsgbl_ch(chBuf)
    DEB_ASCII_BUF_file_info(fileBuf)
    file_info_t         *file_p;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >file_info_malloc(%s)",
        deb_ascii_nsgbl_ch(chBuf,ch_p)));

    /* Allocate file descriptor */
    file_p = (file_info_t *)dns_malloc_perm(sizeof(file_info_t));
    if (file_p) {
        /* Initalize all fields to zero */
        ZERO_bytes(file_p, sizeof(*file_p));

        init_list(&file_p->file_prop_wait);
        {
            if ((thstatus = dthread_create_mutex(&file_p->file_mutex)) < 0)
            {
	      dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
            }
        }
        file_p->file_state = file_init;

	if ((thstatus = dthread_create_condition(&file_p->file_state_changed)) < 0)
	{
	  dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
        }
	if ((thstatus = dthread_create_condition(&file_p->file_back_done)) < 0)
	{
	  dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
        }
	if ((thstatus = dthread_create_condition(&file_p->file_ckpt_done)) < 0)
	{
	  dce_svc_printf(CDS_S_PTHREADCONDINIT_A_MSG, (long)thstatus);
        }
        dthread_assign(dthread_null, &file_p->file_propagator);
        dthread_assign(dthread_null, &file_p->file_back_activator);
        dthread_assign(dthread_null, &file_p->file_back_thread);
        file_p->file_ch_p = ch_p;
        dthread_assign(dthread_null, &file_p->file_ckpt_thread);

	if ((thstatus = dthread_create_mutex(&file_p->file_tlog_mutex)) < 0)
	{
	  dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)thstatus);
        }
        init_current_op_info(&file_p->file_op_info);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <file_info_malloc(%s) return(%s)",
        deb_ascii_nsgbl_ch(chBuf,ch_p),
        deb_ascii_file_info(fileBuf,file_p)));
    return(file_p);
}

/* -------------------------------------------------------------------------- */

/*
 * file_info_free
 *      This routine release memory allocated for file_info structure
 *      and hash tables.
 *
 * Input:
 *      file_p = pointer to file_info block
 *
 */
void 
file_info_free (file_info_t *file_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    int thstatus;
    

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >file_info_free(%s)",
        deb_ascii_file_info(fileBuf,file_p)));

    if ((thstatus = dthread_lock(&(dns_server_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {

	    /* Remove clearinghouse from list of known files */
	    remove_list(&file_p->file_queue);
        }
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(dns_server_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {
	    file_p->file_state = file_rundown;

            if ((thstatus = dthread_broadcast(&(file_p->file_state_changed))) < 0)
            {
	      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
	    }
        }
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    background_kill(file_p);
    db_checkpoint_kill(file_p);
    back_propagate_kill(file_p);

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {
	    unsigned int            idx;

	    /*
	     * Free directory id hash table entries, since these are the
	     * same entries as the name hash, both links will be removed.
	     */
	    for (idx = 0; idx < UID_HASH_LEN; idx++)
		while (TRUE)
		{
		    dir_hash_entry_t *const hash_p = file_p->file_uid_table[idx];
		    if (!hash_p) break;
		    (void)db_hash_free(hash_p, file_p);
		}
        }
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    /* Release memory used by inactive stream descriptors */
    while (TRUE)
        {
	db_stream_t *const stream_p = file_p->file_free_stream_p;
	if (!stream_p) break;
        /* Remove stream from list */
        file_p->file_free_stream_p = stream_p->db_next_p;
        dns_free((char *)stream_p);
        }
    
    if ((thstatus = dthread_delete_mutex(&file_p->file_tlog_mutex)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)thstatus);
    }

    if ((thstatus = dthread_delete_condition(&file_p->file_state_changed)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
    }

    if ((thstatus = dthread_delete_condition(&file_p->file_back_done)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
    }

    if ((thstatus = dthread_delete_condition(&file_p->file_ckpt_done)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
    }

    if ((thstatus = dthread_delete_mutex(&file_p->file_mutex)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXDESTROY_A_MSG, (long)thstatus);
    }

    free_current_op_info(&file_p->file_op_info);
    dns_free((char *)file_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <file_info_free(%p)",
        file_p));
}

/* -------------------------------------------------------------------------- */

/*
 * db_setup
 *      Allocate and initialize the file descriptor.
 *      Load the file into memory.
 *      Initialize the clearinghouse descriptor
 *
 * Input:
 *      ch_p = pointer to clearinghouse descriptor
 *                      file name only field filled in
 *
 */
dns_status_t 
db_setup (nsgbl_ch_t *ch_p)
{
    DEB_ASCII_BUF_nsgbl_ch(chBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    file_info_t         *file_p;
    dns_status_t        status;
    int thstatus;
    

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_setup(%s)",
        deb_ascii_nsgbl_ch(chBuf,ch_p)));

    /* Allocate file descriptor and initialize */
    file_p = file_info_malloc(ch_p);
    if (!file_p) {
	status = DNS_NONSRESOURCES;
	goto leave_db_setup;
    }

    /* Load database into memory */
    status = db_checkpoint_load(file_p);
    if (status != DNS_SUCCESS)
        {
        /* Release memory resources */
        file_info_free(file_p);
        }
    else
        {
	    if ((thstatus = dthread_lock(&(dns_server_mutex))) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
	    }
	    
	        /* Beginning of the 'protected code' */
	        {
		    /* Link into list of known files */
		    insert_list(&db_head, &file_p->file_queue);
                }
	        /* End of the 'protected code' */

	    if ((thstatus = dthread_unlock(&(dns_server_mutex))) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	    }
        }

leave_db_setup:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_setup(%s) return(%s)",
        deb_ascii_nsgbl_ch(chBuf,ch_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/* ------------------------------------------------------------------------- */

/*
 * db_enable
 *      Allocate and initialize the file descriptor and load the file.
 *      If checkaddr is TRUE and the clearinghouse address has changed,
 *      we checkpoint to save the address change.
 *
 * Input:
 *      ch_p = pointer to clearinghouse descriptor
 *      checkaddr = TRUE if addresses should be checked, FALSE otherwise
 *
 */
dns_status_t 
db_enable (nsgbl_ch_t  *ch_p,
           int         checkaddr)
{
    DEB_ASCII_BUF_nsgbl_ch(chBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    bytes_u             mytower[READ_BUF_SIZE];
    nsgbl_ns_t *const   ns_p = ch_p->ch_nameserver;
    file_info_t *const  file_p = db_find_file(&ch_p->ch_uid);
    dns_status_t        status = DNS_SUCCESS;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_enable(%s,%d)",
        deb_ascii_nsgbl_ch(chBuf,ch_p),
        checkaddr));

    if (!file_p) {
	status = DNS_UNKNOWNENTRY;
	goto leave_db_enable;
    }

    if (file_p->file_state == file_open) {
	goto leave_db_enable;
    }

    /*
     * Before we open things up, verify that the associated pseduodirectory's
     * CHDirectories attribute is a superset of the directories entered
     * in our hash table.  (Background will take care of the inverse problem).
     */

    refresh_chdirectories(file_p);

    /*
     * We are open for business
     */
    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {

	if ((file_p->file_state == file_badclk_repair))
	    {
	    status = DNS_WRONGSTATE;
	    }
	else
	    {
	    file_p->file_state = file_open;

            if ((thstatus = dthread_broadcast(&(file_p->file_state_changed))) < 0)
                {
		dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
                }
	    }
        }
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    if (status != DNS_SUCCESS)
        goto leave_db_enable;

    /* Register with clerkserver interface */
    (void)rpc_register_ch(ch_p->ch_nameserver->ns_ta_id, ch_p);

    /* Schedule background thread */
    (void)background_start(file_p);

    /*
     * Restart propagation if it was interrupted.
     */
    (void)back_propagate_start(file_p);

leave_db_enable:
   
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_enable(%s,%d) return(%s)",
        deb_ascii_nsgbl_ch(chBuf,ch_p),
        checkaddr,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/* -------------------------------------------------------------------------- */

/*
 * write_chlastaddress
 *      writes a tower to chlastaddress
 *
 * Input:
 *      stream_p = pointer to open clearinghouse pseudo dir record
 *      mytower_p = pointer to buffer containing local address, if destroyed
 *                  re-read it.
 *
 * Value:
 *      status
 */
dns_status_t 
write_chlastaddress (db_stream_t  *stream_p,
                     Set_t        *mytower_p)
{
    DEB_ASCII_BUF_db_stream(streamBuf)
    DEB_ASCII_BUF_Set(mytowerBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    Timestamp_u         ts;
    Update_t            *upd_p;
    int                  num_mem;
    int                  i;
    dns_status_t        status = DNS_SUCCESS;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >write_chlastaddress(%s,%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_Set(mytowerBuf,mytower_p)));

    upd_p = (Update_t *)dns_malloc(sizeof(Update_u));
    if (!upd_p) {
	status = DNS_NONSRESOURCES;
	goto leave_write_chlastaddress;
    }
    /* Remove dns$chlastaddress attribute from the ch pseudo dir */
    (void)ts_new((Timestamp_t *)ts);
    (void)update_dns_build(upd_p, UD_absent, (Timestamp_t *)ts,
        dns_Attribute(DNS_CHLASTADDRESS),
        BL_false, 0, NULL);  /* no value */
    (void)set_insert(&stream_p->db_dir, upd_p,
        dns_Attribute(DNS_CHLASTADDRESS), TRUE);
    (void)set_timestamp_set(&stream_p->db_dir,
	(Timestamp_t *)upd_p->ud_timestamp,
        dns_Attribute(DNS_UTS), NULL, NULL);

    num_mem = NUM_Set(mytower_p);

    for (i = 0; i < num_mem; i++)
        {
	MemberValue_t *const towermem_p =
			(MemberValue_t *)INDEX_Set(mytower_p, i);

        if (EXT8(towermem_p->mv_flag) != MV_present) continue;

        /* add the new tower
         */
        (void)ts_new((Timestamp_t *)ts);
        (void)update_dns_build(upd_p, UD_present, (Timestamp_t *)ts,
            dns_Attribute(DNS_CHLASTADDRESS),
            BL_true, EXT16(towermem_p->mv_length), towermem_p->mv_value);
        (void)set_insert(&stream_p->db_dir, upd_p,
            dns_Attribute(DNS_CHLASTADDRESS), TRUE);
        (void)set_timestamp_set(&stream_p->db_dir,
		(Timestamp_t *)upd_p->ud_timestamp,
                dns_Attribute(DNS_UTS), NULL, NULL);
        }

    (void)db_pseudo_update(stream_p, NULL);
    dns_free((char *)upd_p);

leave_write_chlastaddress:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <write_chlastaddress(%s,%s) return(%s)",
        deb_ascii_db_stream(streamBuf,stream_p),
        deb_ascii_Set(mytowerBuf,mytower_p),
        deb_ascii_dns_status(statusBuf,status)));
    return (status);
}

/* -------------------------------------------------------------------------- */

/*
 * db_create
 *      Allocate and initialize a file descriptor.  The file is
 *      not actually created here.  Create pseudo directory.
 *
 * Input:
 *      ch_p = pointer to global section entry
 *      username_p = pointer to user full name
 *      version_p = pointer to directory version
 *
 * Value:
 *      status
 */
dns_status_t 
db_create (nsgbl_ch_t         *ch_p,
           user_descriptor_t  *username_p,
           version_t          *version_p)
{
    DEB_ASCII_BUF_nsgbl_ch(chBuf)
    DEB_ASCII_BUF_user_descriptor(usernameBuf)
    DEB_ASCII_BUF_version(versionBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    time_quad_t         nextback;
    file_info_t         *file_p;
    dns_status_t        status;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_create(%s,%s,%s)",
        deb_ascii_nsgbl_ch(chBuf,ch_p),
        deb_ascii_user_descriptor(usernameBuf,username_p),
        deb_ascii_version(versionBuf,version_p)));

    file_p = file_info_malloc(ch_p);
    if (!file_p) {
	status = DNS_NONSRESOURCES;
	goto leave_db_create;
    }
    /* Load in default checkpoint interval values */
    get_ckpt_parameters(&file_p->file_ckpt_hdr.hdr_tlog_threshold,
                        &file_p->file_ckpt_hdr.hdr_ckpt_interval);

    file_p->file_ckpt_hdr.hdr_align_mask = ALIGN_MASK;

    /* Get current time as time for nextback to run */
    sys_time_to_utc(NULL, &nextback);
    COPY_Time(&nextback, file_p->file_ckpt_hdr.hdr_nextback);

    /* Build pseudo directory, and insert in hash tables */
    status = db_pseudo_create(file_p, username_p, version_p);
    if (status == DNS_SUCCESS)
        {
        file_p->file_ckpt_hdr.hdr_dir_count = 1;
        status = db_checkpoint(file_p);
        }

    if (status == DNS_SUCCESS)
    { /* if */
        if ((thstatus = dthread_lock(&(dns_server_mutex))) < 0)
        {
	  dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
        }
            /* Beginning of the 'protected code' */
            {
                /* Link into list of opened files */
		insert_list(&db_head, &file_p->file_queue);
            }
	    /* End of the 'protected code' */

	if ((thstatus = dthread_unlock(&(dns_server_mutex))) < 0)
	{
	  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	}

	if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
        {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
        }

	    /* Beginning of the 'protected code' */
            {
		file_p->file_state = file_loaded;

                if ((thstatus = dthread_broadcast(&(file_p->file_state_changed))) < 0)
                {
		  dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, 
				 (long)thstatus);
                }
            }
            /* End of the 'protected code' */

	if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
	{
	  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
        }
    }
    else
	file_info_free(file_p);
    
leave_db_create:
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_create(%s,%s,%s) return(%s)",
        deb_ascii_nsgbl_ch(chBuf,ch_p),
        deb_ascii_user_descriptor(usernameBuf,username_p),
        deb_ascii_version(versionBuf,version_p),
        deb_ascii_dns_status(statusBuf,status)));

    return(status);
    
}

/* -------------------------------------------------------------------------- */

/*
 * db_delete
 *      This routine deletes the checkpoint and transaction log files.
 *      It also rolls the version file back one.  Transation log
 *      is open on entry.
 *
 *
 * Input:
 *      ns_p = namespace global structure
 *      id_p = pointer to clearinghouse ID
 */
void 
db_delete (nsgbl_ns_t  *ns_p,
           ObjUID_t    *id_p)
{
    DEB_ASCII_BUF_nsgbl_ns(nsBuf)
    DEB_ASCII_BUF_ObjUID(idBuf)
    file_info_t *const file_p = db_find_file(id_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_delete(%s,%s)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p),
        deb_ascii_ObjUID(idBuf,id_p)));

    if (file_p)
        {
        /* Keep structure from disappearing */
        if (file_p->file_state != file_loaded)
            (void)db_disable(id_p);

        /*
         * Get rid of transaction log which is currently opened,
         * then the checkpoint file, and finally the version file.
         *
         * After we have deleted the on-disk version of the database,
         * we release it from memory, remove the clearinghouse from
         * our internal list, and free up the file block itself.
         */

	(void)db_tlog_close(file_p);	/* must first close */
        (void)db_tlog_delete(file_p);
        (void)sys_delete_checkpoint(file_p->file_ch_p->ch_filename);
        (void)sys_delete_version(file_p->file_ch_p->ch_filename);

        file_info_free(file_p);
        }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_delete(%s,%s)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p),
        deb_ascii_ObjUID(idBuf,id_p)));
}

/* -------------------------------------------------------------------------- */

/*
 * db_clear
 *      This routine clears the checkpoint and transaction log files.
 *      It also rolls the version file back one.  Transation log
 *      is open on entry.
 *
 *
 * Input:
 *      id_p = pointer to clearinghouse ID
 */
void 
db_clear (ObjUID_t *id_p)
{
    DEB_ASCII_BUF_ObjUID(idBuf)
    file_info_t *const file_p = db_find_file(id_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_clear(%s)",
        deb_ascii_ObjUID(idBuf,id_p)));

    if (file_p)
        {
        /* Keep structure from disappearing */
        if (file_p->file_state != file_loaded)
            (void)db_disable(id_p);

	/*
	 * Bugfix for remaining open channel on tlog file -
	 *  Close current tlog file
	 */
	(void)db_tlog_close(file_p);

        /* Now release clearinghouse from memory. */
        file_info_free(file_p);
        }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_clear(%s)",
        deb_ascii_ObjUID(idBuf,id_p)));
}

/* -------------------------------------------------------------------------- */

/*
 * refresh_ch_directories
 *
 * This routine maintains a constraint that the CHDirectories attribute
 * should contain at least a superset of the directories maintained in
 * the clearinghouse.   If external circumstances brings these two
 * items out of balance, this function works towards restoring lost
 * references.   The background process supplies the other side of this
 * balance by removing entries from CHDirectories that are superfluous.
 */

static void 
refresh_chdirectories (file_info_t *file_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    LOG_ASCII_BUF_dns_status(statusBuf)
    LOG_ASCII_BUF_ObjUID(diridBuf)
    int x;
    dir_hash_entry_t *hash_p;
    db_stream_t *stream_p;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >refresh_chdirectories(%s)",
        deb_ascii_file_info(fileBuf,file_p)));

    status = db_get_stream_ignore(file_p, &stream_p, OP_ReadAttribute);

    if (status != DNS_SUCCESS) {
        goto leave_refresh_chdirectories;
    }

    status = db_pseudo_read(stream_p);
    if (status != DNS_SUCCESS) {
        db_free_stream(stream_p);
        goto leave_refresh_chdirectories;
    }


    for (x = 0; x < UID_HASH_LEN; x++) {
        if (file_p->file_uid_table[x]) {
            hash_p = file_p->file_uid_table[x];
            do {
                if ((hash_p->dir_type == ET_directory) &&
                    (!set_dnstest(stream_p->db_dir.data_p,
                                  sizeof(hash_p->dir_uid),
                                  hash_p->dir_uid,
                                  dns_Attribute(DNS_CHDIRECTORIES)))) {
                    status = db_pseudo_chdirectory(file_p, 
                                                   (ObjUID_t *)hash_p->dir_uid,
                                                   UD_present);
                    if (status != DNS_SUCCESS) 
		    {
                        dce_svc_printf(CDS_S_SERVER_FIXCHDIR_MSG,
                            deb_ascii_ObjUID(diridBuf,hash_p->dir_uid),
                            deb_ascii_dns_status(statusBuf,status));
                    } 
                }
		hash_p = hash_p->dir_uid_collision_p;
            } while (hash_p);
        } /* endif */
    } /* endfor */

    db_free_stream(stream_p);

leave_refresh_chdirectories:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <refresh_chdirectories(%s) [status is %s]",
        deb_ascii_file_info(fileBuf,file_p),
        deb_ascii_dns_status(statusBuf,status)));
    return;
}

/* -------------------------------------------------------------------------- */

static void 
init_current_op_info (current_op_info_t *info_p)
{
    int thstatus;


    /*
     *  We assume it has been initialized to 0 from file_info_malloc 
     */
    dce_assert(cds__svc_handle,
	       (info_p->currop_current == 0));       /* Just in case */
    dce_assert(cds__svc_handle,
	       (info_p->currop_special == 0));       /* Just in case */

    if ((thstatus = 
	 dthread_create_condition(&info_p->currop_sop_completed)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
    }
}



/* -------------------------------------------------------------------------- */

static void 
free_current_op_info (current_op_info_t *info_p)
{
    int thstatus;

    
    if ((thstatus = 
	 dthread_delete_condition(&info_p->currop_sop_completed)) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADCONDDESTROY_A_MSG, (long)thstatus);
    }
}


/* -------------------------------------------------------------------------- */

dns_status_t 
db_begin_operation (user_descriptor_t *user_p)
{
    DEB_ASCII_BUF_ObjUID(chidBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    ObjUID_t * const chid_p = user_p->ud_chuid_p;      /* ObjUID of CH */
    const int       operation = user_p->ud_operation; /* Operation PDU value */
    file_info_t    *file_p;                           /* Database info */
    dns_status_t    status = DNS_SUCCESS;
    current_op_info_t *info_p;
    int             special_op;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_begin_operation(%p->(%s,%d))", 
	user_p,
	deb_ascii_ObjUID(chidBuf,chid_p), 
        operation));

    /* If match was not found, clearinghouse doesn't exist */
    file_p = db_find_file(chid_p);
    if (file_p == NULL) {
        status = DNS_UNKNOWNCLEARINGHOUSE;
        goto leave_db_begin_operation;
    }

    info_p = &file_p->file_op_info;

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */

        special_op = ((operation == OP_CreateDirectory) ||
                      (operation == OP_DeleteDirectory) ||
                      (operation == OP_AddReplica) ||
                      (operation == OP_RemoveReplica));
        if (special_op) {
            while ((file_p->file_ckpt_needed > 0) &&
                   (info_p->currop_special > 0)) 
	    {
                DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_database,
                    svc_c_debug5,
                    "  Special operation deadlock avoidance triggered."));

                DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_database,
                    svc_c_debug5,
                    "  Op: %d, CHID: %s",
                    operation,
                    deb_ascii_ObjUID(chidBuf,chid_p)));

                {
                    if ((thstatus = dthread_wait(&(info_p->currop_sop_completed),
                                                 &(file_p->file_mutex))) < 0)
                    {
		      dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, 
				     (long)thstatus);
                    }
                }
            }
            info_p->currop_special++;
        }
        info_p->currop_current++;

        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }


leave_db_begin_operation:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_begin_operation(%p->(%s,%d)) return(%s)", 
        user_p,
        deb_ascii_ObjUID(chidBuf,chid_p), 
        operation,
	deb_ascii_dns_status(statusBuf,status)));

    return(status);

}


/* -------------------------------------------------------------------------- */

dns_status_t 
db_end_operation (user_descriptor_t *user_p)
{
    DEB_ASCII_BUF_ObjUID(chidBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    ObjUID_t * const chid_p = user_p->ud_chuid_p;      /* ObjUID of CH */
    const int       operation = user_p->ud_operation; /* Operation PDU value */
    file_info_t    *file_p;
    dns_status_t    status = DNS_SUCCESS;
    current_op_info_t *info_p;
    int             special_op;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " >db_end_operation(%p->(%s,%d))", 
        user_p,
        deb_ascii_ObjUID(chidBuf,chid_p), 
        operation));

    /* If match was not found, clearinghouse doesn't exist */
    file_p = db_find_file(chid_p);
    if (file_p == NULL) {
        status = DNS_UNKNOWNCLEARINGHOUSE;
        goto leave_db_end_operation;
    }

    info_p = &file_p->file_op_info;

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */

        special_op = ((operation == OP_CreateDirectory) ||
                      (operation == OP_DeleteDirectory) ||
                      (operation == OP_AddReplica) ||
                      (operation == OP_RemoveReplica));
        info_p->currop_count[operation]++;
        info_p->currop_current--;
        if (special_op) {
            info_p->currop_special--;
            if (info_p->currop_special == 0) 
	    {
		if ((thstatus = 
		     dthread_broadcast(&(info_p->currop_sop_completed))) < 0)
		{
		  dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, 
				 (long)thstatus);
                }
            }
        }
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

leave_db_end_operation:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug5,
        " <db_end_operation(%p->(%s,%d)) return(%s)",
        user_p,
        deb_ascii_ObjUID(chidBuf,chid_p), 
        operation,
        deb_ascii_dns_status(statusBuf,status)));

    return(status);
}
