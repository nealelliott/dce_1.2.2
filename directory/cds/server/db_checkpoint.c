/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_checkpoint.c,v $
 * Revision 1.1.10.1  1996/08/09  12:00:07  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/08/06  14:55 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	Clear file_ckpt_force flag at the end of db_checkpoint().
 * 	[1996/02/18  19:35:46  marty  1.1.8.2]
 *
 * Revision 1.1.8.2  1996/02/18  19:35:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:10  marty]
 * 
 * Revision 1.1.8.1  1995/12/08  15:29:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:46  root]
 * 
 * Revision 1.1.6.8  1994/08/19  21:10:41  zee
 * 	Part of BadClock fix - accept this status in db_checkpoint calling ts_new.
 * 	[1994/08/19  18:08:53  zee]
 * 
 * Revision 1.1.6.7  1994/08/03  19:03:48  mccann
 * 	final sams drop
 * 	[1994/08/02  14:08:58  mccann]
 * 
 * Revision 1.1.6.6  1994/08/01  15:07:31  zee
 * 	     Broadcast on file_ckpt_done condition variable when checkpoint is done.
 * 	[1994/07/29  21:03:55  zee]
 * 
 * Revision 1.1.6.5  1994/06/30  19:18:07  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:47:46  mccann]
 * 
 * Revision 1.1.6.4  1994/06/09  18:43:03  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:25  devsrc]
 * 
 * Revision 1.1.6.3  1994/04/14  14:51:06  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:19:01  peckham]
 * 
 * Revision 1.1.6.2  1994/03/22  20:00:21  griffin
 * 	HCell BL3 support: Reorganize checkpointing so that root directory is
 * 	loaded in second.  This permits the CellName and aliases to be exposed and
 * 	allow relative name generation for the rest of the directories in the
 * 	clearinghouse.
 * 	[1994/03/22  18:04:11  griffin]
 * 
 * Revision 1.1.6.1  1994/03/12  22:10:29  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:39:51  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  14:20:08  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:20:19  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/07  15:09:55  keegan
 * 	OT#5678, header corruption status is dropped on floor.
 * 	DB conversion code now only executed if upstream status is O.K.
 * 	[1992/12/07  15:08:11  keegan]
 * 
 * Revision 1.1.2.2  1992/07/17  18:18:21  peckham
 * 	CR#2681: Avoid checkpoint deadly embrace.
 * 	  27-JUN-1992 15:41  by Dave Griffin
 * 	      Modified behavior of file_open -> file_ckpt transition to accomodate
 * 	      special deadlock detection code.
 * 	[1992/07/17  18:17:06  peckham]
 * 
 * Revision 1.1  1992/01/19  15:25:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: db_checkpoint.c
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
 */
#include <server.h>
#include <back.h>
#include <dbdef.h>
#include <find.h>
#include <uid.h>

/*
 * local routines
 */
static dthread_address_t
db_checkpoint_memory (
    file_info_t         *);

/*
 * db_checkpoint
 *      Checkpoint file and re-init transaction log.
 *
 * Input
 *      file_p = pointer to file blocks
 */
dns_status_t
db_checkpoint (file_info_t *file_p)
{
    LOG_ASCII_BUF_file_info(fileBuf)
    void                *ckpt_dc_ctx = NULL;
    DBSet_t             *set_p;
    node_header_t       *data_p;
    file_state_t        save_state;
    unsigned int        next_vbn;
    int                 pos, index;
    int                 directories_dumped = 0;
    int                 pass = 1;
    dns_status_t        status;
    void                *comp_ctx = NULL;
    current_op_info_t * const info_p = &file_p->file_op_info;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_checkpoint(%s)",
        deb_ascii_file_info(fileBuf,file_p)));

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        { /* lock */

        /*
         *  Set the file state to the appropriate flush state:
         *
         *    If this is normal checkpoint, then the incoming file state
         *    should be 'file_open'.  If so, then we transition to 'file_ckpt'.
         *
         *    This routine is also called during recovery, but we do not want
         *    to indicate that the file state is 'file_ckpt', since this
         *    implies that the file is open for users. 'file_ckpt_exclusive'
         *    is used instead.
         *
         *    This routine is also called during namespace initialization.
         *    As with recovery a checkpoint is called, but we must prevent
         *    access to the file.
         *
         *    Disabling a clearinghouse forces a checkpoint (if the
         *    clearinghouse wasn't already disabled).  This input state
         *    during this operation is 'file_loaded' (set by the db_disable
         *    code) so we use the 'file_ckpt_exclusive' state to prevent
         *    transactions from occurring.
         *
         *    No other state transitions are recognized, so we return a
         *    nameserver bug in all other cases.
         */


        /*
         * Remember state we entered with, then
         * check states in order of success frequency
         */

            while ((file_p->file_state == file_open) &&
                   (info_p->currop_special > 0))
            {
                if ((thstatus = dthread_wait(&(info_p->currop_sop_completed),
                                             &(file_p->file_mutex))) < 0)
                {
		  dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
                }
            }

            save_state = file_p->file_state;

            switch ((int)file_p->file_state)
            { /* start switch */
            case file_open:
                file_p->file_state = file_ckpt;
                break;
            case file_loaded:
            case file_recover:
            case file_init:
                file_p->file_state = file_ckpt_exclusive;
                break;
            default:

                if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
                {
		  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
                }

                dce_svc_printf(CDS_S_SERVER_CHSTATE_MSG,
			       deb_ascii_file_info(fileBuf,file_p),
			       save_state);

                status = DNS_NAMESERVERBUG;
                goto fail_status;
            } /* end switch */

            if ((thstatus =
                 dthread_broadcast(&(file_p->file_state_changed))) < 0)
            {
	      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
            }


            /* Wait for write count to go to zero */
            while (0 < file_p->file_write_count)
            {
                if ((thstatus = dthread_wait(&(file_p->file_state_changed),
                                             &(file_p->file_mutex))) < 0)
                {
		  dce_svc_printf(CDS_S_PTHREADCONDWAIT_A_MSG, (long)thstatus);
                }
            }
        } /* end lock ! */

        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    /*
     * At this point, the in-memory database is consistent and
     * the hash tables are not changing.  First write out
     * the file header, then start dumping data.
     *
     * Make sure file header is consistant with the server's database
     * version, and also assure that the NS ID for the clearinghouse
     * stored in this file is recorded.  The NS ID is recorded for the
     * X.500 project's use and is not read or used by the DECdns software.
     */

    /* Initialize database compression interface, store level in header */
    status = db_compress_init(file_p, &comp_ctx);
    if (status != DNS_SUCCESS) {
        goto fail_state;
    }

    file_p->file_ckpt_hdr.hdr_ckpt_comp_level = file_p->file_ckpt_comp_level;

    /* Generate the last last_cts and store in header */
    status = ts_new((Timestamp_t *)file_p->file_ckpt_hdr.hdr_last_cts);
    if (status != DNS_SUCCESS && status != DNS_BADCLOCK) {
        goto fail_state;
    }

    COPY_ObjUID(&file_p->file_ch_p->ch_nsuid, file_p->file_ckpt_hdr.hdr_nsuid);
    get_database_version(&file_p->file_ckpt_hdr.hdr_db_version);
    file_p->file_ckpt_hdr.hdr_sequence++;

    status = sys_create_checkpoint(file_p->file_ch_p->ch_filename,
                file_p->file_ckpt_block, file_p->file_ckpt_hdr.hdr_sequence);
    if (status != DNS_SUCCESS) {
        goto fail_sequence;
    }

    /* Write file header as first block */
    status = sys_write(file_p->file_ckpt_block, (byte_t *)&file_p->file_ckpt_hdr, 1, 1);
    if (status != DNS_SUCCESS) {
        goto fail_data;
    }

    /*
     * Record the start time of the checkpoint (when the real work begins).
     * This datum is for informational purposes only and doesn't have to
     * be all that accurate.
     */
    sys_time_from_utc(&file_p->file_ckpt_last_start, NULL);

    /*
     * Start dumping directory contents.
     *
     * The directory dumping order is:
     *     Pseudo-directory
     *     Root Directory
     *     Everything Else
     *
     * This particular ordering is needed by the checkpoint load
     * mechanism to obtain proper cell naming information when the
     * clearinghouse is loaded back into memory.
     *
     */

    next_vbn = 2;

    for (pass = 1; pass < 4; pass++)
    {
      DCE_SVC_DEBUG((
		     cds__svc_handle,
		     cds_svc_database,
		     svc_c_debug8, "  Checkpoint pass %d\n", pass));
        for (index = 0; index < UID_HASH_LEN; index++)
        {
            dir_hash_entry_t *hash_p = file_p->file_uid_table[index];

            for (; hash_p; hash_p = hash_p->dir_uid_collision_p)
            {

                /*
                 * Do clearinghouses first pass, then all else second pass.
                 */

	      DCE_SVC_DEBUG((cds__svc_handle,
		  cds_svc_database, svc_c_debug8, 
		  "  Checkpoint eval 0x%x DT %d, DR %d, pass %d\n",
                  hash_p, hash_p->dir_type, hash_p->dir_root, pass));

                switch (pass) {
                case 1:
                    if (hash_p->dir_type != ET_clearinghouse) continue;
                    break;
                case 2:
                    if (hash_p->dir_type == ET_clearinghouse) continue;
                    if (!hash_p->dir_root) continue;
                    break;
                case 3:
                    if (hash_p->dir_type == ET_clearinghouse) continue;
                    if (hash_p->dir_root) continue;
                    break;
                default:
                    /* assert_abort!!!!!!! ??????????????????????? */
                    break;
                }

                /*
                 * Check the directory state -- if it's dead, don't write
                 * it out to the checkpoint file -- it is a dangling entry
                 * anyway.
                 */

                if (hash_p->dir_replica_state == RS_dead) {
		  DCE_SVC_DEBUG((
				 cds__svc_handle,
				 cds_svc_database,
				 svc_c_debug8, 
		     "  db_checkpoint() - skipping over a dead replica\n"));
                    continue;
                }
                /*
                 * Check the directory state -- if it's dead, don't write
                 * it out to the checkpoint file -- it is a dangling entry
                 * anyway.
                 */
                if (hash_p->dir_replica_state == RS_dead)
                {
                    DCE_SVC_DEBUG((
                        cds__svc_handle,
                        cds_svc_database,
                        svc_c_debug5,
                        "db_checkpoint() - skipping over a dead replica\n"));
    
                    continue;
                }
                /*
                 * Update lastupdate in record if it has changed in hash cache
                 */
                if (hash_p->dir_type == ET_directory)
                {
                    if (NE_Timestamp(hash_p->dir_lastupdate, NullTimestamp))
                    {
                        if (find_attribute(
                               (data_record_t *)hash_p->dir_p->dir_data,
                               dns_AttributeName(DNS_LASTUPDATE),
                               &set_p, &pos) == DNS_SUCCESS)
                        {
                            DBSetMember_t *member_p = (DBSetMember_t *)
                                    DATA_DBSet(set_p);
    
                            COPY_Timestamp(hash_p->dir_lastupdate,
                                    member_p->dm_data);
                            COPY_Timestamp(NullTimestamp, 
                                           hash_p->dir_lastupdate);
                        }
                    }
                }
                /* Write out directory record */
                status = sys_write(file_p->file_ckpt_block, 
                                   (byte_t *)hash_p->dir_p,
                                   next_vbn, hash_p->dir_p->dir_blocks);

                if (status != DNS_SUCCESS) {
                    goto fail_tlog;
                }
                next_vbn += hash_p->dir_p->dir_blocks;
                directories_dumped++;

                /*
                 * Write out B-tree data blocks.  Index blocks are not written,
                 * they are rebuilt during startup.
                 */
                data_p = hash_p->dir_entry_p;
                if (data_p)
                {
                    /* Find data level and write it out */
                    status = db_compress_btree_start(comp_ctx, next_vbn);
                    if (status != DNS_SUCCESS) {
                        goto fail_tlog;
                    }
                    for (data_p = db_btree_get_first_data_bucket(data_p);
                         data_p;
                         data_p = data_p->node_next_p)
                    {
                        status = db_compress_btree_write(comp_ctx, data_p);
                        if (status != DNS_SUCCESS) {
                            goto fail_tlog;
                        }
                    }
                    status = db_compress_btree_end(comp_ctx, &next_vbn);
                    if (status != DNS_SUCCESS) {
                        goto fail_tlog;
                    }
                }

                /*
                 * Here's an optimization for passes 1 & 2: There's only
                 * one entry for each pass, so once we have processed it
                 * there's not much of a reason to walk through the rest
                 * of the hash table entries.  So we break out to the pass
                 * loop.
                 */

                if (pass < 3) {
                    break;
                }

            } /* next hash_p down the collision chain */

        } /* next hash table entry */

    } /* next pass */

    if (status != DNS_SUCCESS)
    {
fail_tlog:
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug2,
            "db_checkpoint(%s) - invalid checkpoint file\n",
            fileBuf));

        /*
         * Cleanup invalid checkpoint
         */
        (void)db_compress_finish(comp_ctx);
        comp_ctx = NULL;
        (void)db_tlog_close(file_p);
fail_data:
        (void)sys_delete(file_p->file_ckpt_block);
fail_sequence:
        --file_p->file_ckpt_hdr.hdr_sequence;
fail_state:
        if (comp_ctx) {
            (void)db_compress_finish(comp_ctx);
        }

        /* set save_state to something */
        /* save_state = ; */
    }
    else
    {
        DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug5,
                "db_checkpoint(%s) - valid checkpoint file written\n",
                fileBuf));

        DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug5,
                "db_checkpoint - directories written: %d; header: %d\n",
                directories_dumped,
                file_p->file_ckpt_hdr.hdr_dir_count));

        (void)db_compress_finish(comp_ctx);
        (void)sys_close(file_p->file_ckpt_block);
        (void)db_tlog_close(file_p);
        status = db_tlog_new(file_p);
        if (status != DNS_SUCCESS) {
            goto fail_status;   /* Leave with clearinghouse disabled */
        }
        status = update_version(file_p->file_ch_p->ch_filename,
                                file_p->file_ckpt_hdr.hdr_sequence);
        if (status != DNS_SUCCESS) {
            goto fail_status;   /* Leave with clearinghouse disabled */
        }
        /* Delete old versions, checkpoint now commited */
        sys_purge(file_p->file_ch_p->ch_filename,
                  file_p->file_ckpt_hdr.hdr_sequence - 1);

        if (file_p->file_ckpt_needed > 0) {
            file_p->file_ckpt_needed = 0;
        }

        /*
         * Record the checkpoint finish time.  This datum is used by the
         * periodic checkpoint scheduler to determine how long it has been
         * since the last checkpoint was done.
         */
        sys_time_from_utc(&file_p->file_ckpt_last_finish, NULL);

    }


    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        { /* lock */

            file_p->file_state = save_state;

            if ((thstatus =
                 dthread_broadcast(&(file_p->file_state_changed))) < 0)
            {
	      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
            }

	    /* Clear file_ckpt_force flag if it was set */
	    file_p->file_ckpt_force = 0;

            if ((thstatus =
                 dthread_broadcast(&(file_p->file_ckpt_done))) < 0)
            {
	      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
            }
        }
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

fail_status:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_checkpoint(%s) return(%d)",
        fileBuf,
        status));

    return(status);
}


/* -------------------------------------------------------------------------- */


/*
 * db_checkpoint_schedule
 *      When conditions require it, this routine can be called
 *      to fork off a thread which will perform a checkpoint.
 *
 * Input:
 *      file_p = pointer to address of needy file info block (unlocked)
 */
void
db_checkpoint_schedule (file_info_t *file_p)
{
    int thstatus;


    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {

            if (file_p->file_ckpt_needed >= 0)
            {
                extern dthread_attr_t db_checkpoint_memory_attr;

                file_p->file_ckpt_needed++;

                if (dthread_equal(dthread_null, file_p->file_ckpt_thread))
                    (void)dthread_create(&file_p->file_ckpt_thread,
                                         &db_checkpoint_memory_attr,
                                   (dthread_start_routine)db_checkpoint_memory,
                                         (dthread_address_t)file_p);
            }
        }
       /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }
}

/*
 * db_checkpoint_kill
 *      Cancel a checkpoint task if scheduled. Since this module is
 *      not ready for a dthread_cancel(), this waits until the
 *      checkpoint thread is complete.
 */
void
db_checkpoint_kill (file_info_t *file_p)
{
    /*
     * Stop new checkpoints from being scheduled
     * and keep the current one from being detached.
     */
    int thstatus;


    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {
            file_p->file_ckpt_needed = -1;
        }
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    /*
     * If there is a checkpoint thread, wait for it to quit.
     */
    if (!dthread_equal(dthread_null, file_p->file_ckpt_thread)) {
        dthread_address_t join_status;

        (void)dthread_join(file_p->file_ckpt_thread, &join_status);
        (void)dthread_detach(&file_p->file_ckpt_thread);
        dthread_assign(dthread_null, &file_p->file_ckpt_thread);
    }
    file_p->file_ckpt_needed = 0;
}


/* -------------------------------------------------------------------------- */


/*
 * db_checkpoint_memory
 *      This routine initiates a checkpoint operation and creates
 *      a new tlog file.  It is called as a new thread.
 *
 * Input:
 *      file_p = pointer to file info block
 *
 */
static dthread_address_t
db_checkpoint_memory (file_info_t *file_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_checkpoint_memory(%s)",
        deb_ascii_file_info(fileBuf,file_p)));

    if (file_p->file_ckpt_needed > 0)
        (void)db_checkpoint(file_p);

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {
            if (file_p->file_ckpt_needed >= 0)
            {
                (void)dthread_detach(&file_p->file_ckpt_thread);
                dthread_assign(dthread_null, &file_p->file_ckpt_thread);
            }
        }

    /* End of the 'protected code' */
    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }
    return(0);
}


/*
 * db_checkpoint_thresholds_check
 *
 * This routine examines the state of a database and determines if the
 * periodic checkpoint thresholds have been exceeded.  The caller can then
 * decide whether or not to call db_checkpoint_schedule (or whatever other
 * action is necessary).
 *
 * NOTE: This routine does not modify the file info block, nor is it
 * critical that the data read from it be absolutely accurate, so no locking
 * of the structure is done.
 *
 * Input:
 *      file_p = pointer to file info block
 *
 * Returns:
 *     TRUE  - Thresholds are exceeded.
 *     FALSE - Thresholds are not exceeded.
 */
int
db_checkpoint_thresholds_check (file_info_t *file_p)
{

    time_local_t  ckpt_delta;    /* Checkpoint interval in delta time format */
    time_local_t  next_time;     /* The next time a checkpoint should run */
    time_local_t  current_time;  /* Current time */

    DEB_ASCII_BUF_file_info(fileBuf)

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_checkpoint_thresholds_check(%s)",
        deb_ascii_file_info(fileBuf,file_p)));


    /*
     * In support of diagnostics, we check to see if checkpoints are
     * being forced to occur (overriding the thresholds).
     */
    if (file_p->file_ckpt_force)
        {
        return(TRUE);
        }

    /*
     * Now we see if the transaction log has grown past our desired
     * maximum size.
     */
    if ((file_p->file_tlog_next_block * BLOCK_SIZE / 1048587) >
        file_p->file_ckpt_hdr.hdr_tlog_threshold)
        {
        return(TRUE);
        }


    /*
     * Next we check to see if the last time we completed a checkpoint
     * plus the checkpoint interval has expired.
     *
     * First we convert the checkpoint file interval (in hours) to a
     * delta time.  Then we add the delta time to the last checkpoint
     * time and compare against the current time.
     */
    sys_time_from_interval(&ckpt_delta,
                           file_p->file_ckpt_hdr.hdr_ckpt_interval, 0, 0);
    sys_time_from_utc(&current_time, NULL);
    sys_time_add(&next_time, &file_p->file_ckpt_last_finish, &ckpt_delta);
    if (GT_time_local(&current_time, &next_time))
        {
        return(TRUE);
        }

    return(FALSE);
}


/* -------------------------------------------------------------------------- */


/*
 * db_checkpoint_load
 *      Load the database into memory.  Find oldest checkpoint file,
 *      read it into memory, then play back all transaction logs.
 *      If the transaction logs are empty we are done.
 *      If they aren't, the system was shutdown
 *      improperly and we must playback the transactions, checkpoint
 *      the results and delete the old transaction logs.
 *
 *      file_p = pointer to file descriptor
 */
dns_status_t
db_checkpoint_load (file_info_t *file_p)
{
    DEB_ASCII_BUF_file_info(fileBuf)
    dir_hash_entry_t   *hash_p;
    conversion_ctx_t    conversion_ctx;
    unsigned int        next_vbn, was_there_a_crash;
    int                 i;
    dns_status_t        status;
    int thstatus;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " >db_checkpoint_load(%s)",
        deb_ascii_file_info(fileBuf,file_p)));

    /* Find matching checkpoint and log files and open them */
    status = sys_open_checkpoint(file_p->file_ch_p->ch_filename,
                file_p->file_ckpt_block);
    if (status != DNS_SUCCESS)
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug3,
            "db_checkpoint_load: checkpoint file open failed"));

        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug2,
            " <db_checkpoint_load(%s) return(%d)",
            deb_ascii_file_info(fileBuf,file_p),
            status));

        return(status);
    }

    status = sys_read(file_p->file_ckpt_block, (byte_t *)&file_p->file_ckpt_hdr, 1, 1);
    if (status != DNS_SUCCESS)
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug3,
            "db_checkpoint_load: checkpoint file read header failed"));

        (void)sys_close(file_p->file_ckpt_block);
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug2,
            " <db_checkpoint_load(%s) return(%d)",
            deb_ascii_file_info(fileBuf,file_p),
            status));
        return(status);
        }

    /* Maximize last_cts generated */
    ts_update_last_ts((Timestamp_t *)file_p->file_ckpt_hdr.hdr_last_cts);

    /*
     * If we've got a pre-V4 checkpoint file, fix it up so we can read it.
     * If we've got a V4+ file, then make sure the alignment is compatible.
     */

    if (EXT8(file_p->file_ckpt_hdr.hdr_db_version.vn_major) < 4)
    {
        unsigned long tlog_threshold;
        unsigned long ckpt_interval;
        char *work_p = (char *)file_p->file_ckpt_hdr.hdr_nsuid;
        work_p = (char *)SKIP_ObjUID(work_p);
        tlog_threshold = EXT32(work_p);
        ckpt_interval =  EXT32(SKIP_longword(work_p));
        file_p->file_ckpt_hdr.hdr_align_mask = ALIGN_MASK;
        file_p->file_ckpt_hdr.hdr_tlog_threshold  = tlog_threshold;
        file_p->file_ckpt_hdr.hdr_ckpt_interval   = ckpt_interval;
        file_p->file_ckpt_hdr.hdr_ckpt_comp_level = 0;
    }
    else
    {
        /*
         * Data must be aligned at an acceptable level
         */
        if (file_p->file_ckpt_hdr.hdr_align_mask < ALIGN_MASK) {
            status = DNS_DATACORRUPTION;
        }
    }


    /*
     * We've successfully opened up the checkpoint file.
     * Begin processing contents.
     */


    file_p->file_ckpt_hdr.hdr_align_mask = ALIGN_MASK;
    next_vbn = 2;


    /*
     * Load each directory (including pseudo directory)
     */

    for (i = 0; i < file_p->file_ckpt_hdr.hdr_dir_count; ++i)
    {
        /*
         * Read directory bucket
         * Allocate in memory structures for directory,
         * read continuation buckets and insert in hash table
         */
        status = db_dir_build_memory_record(file_p, &next_vbn, &hash_p,
                                            conversion_ctx);
        if (status != DNS_SUCCESS) break;

        /*
         * Normal directories are followed by their contents (btree data),
         * but pseudo-directories only have the directory bucket.  Handle
         * each case accordingly.
         */

        if (hash_p->dir_type == ET_directory)
        {
            /* Allocate and build in memory structures for b-tree */
            status = db_btree_build(file_p, hash_p, &next_vbn, conversion_ctx);
            if (status != DNS_SUCCESS) break;
        }
        else
        {
            /* Fill in pseudo directory data in ch_p */
            db_init_ch(hash_p, file_p->file_ch_p);
        }
    }

    /*
     * We have now processed all the directories (or an error was detected
     * whilst reading them in).
     *
     * Close the checkpoint file, return if there was an error.
     * !!! I think the conversion context should be checked here as well,
     * !!! but some checking of db_convert_finish is required to see that
     * !!! the side-effects of doing that under error conditions aren't
     * !!! going to make matters worse.   Right now there's a fatal condition
     * !!! here anyway -- a small memory leak is the least of our problems.
     */

    (void)sys_close(file_p->file_ckpt_block);
    if (status != DNS_SUCCESS)
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug2,
            " <db_checkpoint_load(%s) return(%d)",
            deb_ascii_file_info(fileBuf,file_p),
            status));
        return(status);
    }

    /*
     * Dummy up a checkpoint finish time for the periodic checkpoint
     * scheduler.
     */

    sys_time_from_utc(&file_p->file_ckpt_last_start, NULL);
    sys_time_from_utc(&file_p->file_ckpt_last_finish, NULL);


    /*
     * Determine if recovery is necessary and play back the transaction
     * log.  Set the file_state to recover while this is happening.
     * The TLog recovery routine will let us know if any recovery actions
     * were taken.
     */

    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {
            file_p->file_state = file_recover;

            if ((thstatus =
                 dthread_broadcast(&(file_p->file_state_changed))) < 0)
            {
	      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
            }
        }
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    status = db_tlog_recover(file_p, &was_there_a_crash);
    if (status != DNS_SUCCESS)
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug1,
            "db_checkpoint_load: db_tlog_recover error: x%x\n",
            status));

        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug2,
            " <db_checkpoint_load(%s) return(%d)",
            deb_ascii_file_info(fileBuf,file_p),
            status));

        return(status);
    }


    /*
     * If recovery took place, re-checkpoint the file
     */

    if (was_there_a_crash)
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_database,
            svc_c_debug5,
            "  db_checkpoint_load: db_tlog_recover said there was a crash"));
        status = db_checkpoint(file_p);
        if (status != DNS_SUCCESS)
        {
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug1,
                "db_checkpoint_load: db_checkpoint error: x%x\n",
                status));

            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_database,
                svc_c_debug2,
                " <db_checkpoint_load(%s) return(%d)",
                deb_ascii_file_info(fileBuf,file_p),
                status));

            return(status);
        }
    }

    /*
     * Change the file state to "loaded".
     */
    if ((thstatus = dthread_lock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_A_MSG, (long)thstatus);
    }

        /* Beginning of the 'protected code' */
        {

            file_p->file_state = file_loaded;

            if ((thstatus =
                 dthread_broadcast(&(file_p->file_state_changed))) < 0)
            {
	      dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
            }
        }
        /* End of the 'protected code' */

    if ((thstatus = dthread_unlock(&(file_p->file_mutex))) < 0)
    {
        dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_database,
        svc_c_debug2,
        " <db_checkpoint_load(%s) return(%d)",
        deb_ascii_file_info(fileBuf,file_p),
        status));

    return(status);
}
