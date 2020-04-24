/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: db_diags.c,v $
 * Revision 1.1.10.1  1996/08/09  12:00:19  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  15:50 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	Use %lx to display addresses instead of %x.
 * 	*
 * 	Part of fix for OT 13208: Change use of file_back_inhibit to require signal.
 * 	[1996/02/18  19:35:54  marty  1.1.8.2]
 *
 * Revision 1.1.8.2  1996/02/18  19:35:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:20  marty]
 * 
 * Revision 1.1.8.1  1995/12/08  15:29:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:58  root]
 * 
 * Revision 1.1.6.6  1994/09/06  17:37:06  proulx
 * 	fix timestamps
 * 	[1994/09/06  17:30:26  proulx]
 * 
 * Revision 1.1.6.5  1994/08/24  20:14:18  mccann
 * 	fix server shutdown, add annotation strings, start message delay:OT11239,11245 and 11766
 * 	[1994/08/24  20:08:14  mccann]
 * 
 * Revision 1.1.6.4  1994/08/03  19:03:58  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:21:30  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:43:11  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:15:38  devsrc]
 * 
 * Revision 1.1.6.2  1994/03/22  20:00:22  griffin
 * 	HCell BL3 support: Return new attributes (fastpath, root directory pointer)
 * 	to diagnostic displays.
 * 	[1994/03/22  18:05:12  griffin]
 * 
 * Revision 1.1.6.1  1994/03/12  22:10:52  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:40:55  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:20:55  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:21:00  zeliff]
 * 
 * Revision 1.1.2.2  1992/07/17  18:54:24  peckham
 * 	CR#2681: Avoid checkpoint deadly embrace.
 * 	  Display new current_op_info_t structure in file_info_t.
 * 	[1992/07/17  18:52:57  peckham]
 * 
 * Revision 1.1  1992/01/19  15:25:09  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module db_diags.c
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
 * Module description:
 *
 * This module provides diagnostic support for the server database.  The
 * following routines are available:
 *
 * db_diag_dump_file_info_list   - summary of active file_info_t structures
 * db_diag_dump_file_info        - file_info_t contents
 * db_diag_dump_dir_hash_table   - file_info_t hash table contents
 * db_diag_dump_dir_hash_entry   - dir_hash_entry_t contents
 * db_diag_dump_nsgbl_ch         - nsgbl_ch_t contents
 * db_diag_dump_nsgbl_ns         - nsgbl_ns_t contents
 * db_diag_dump_stream           - db_stream_t contents
 *
 */

#include <server.h>
#include <back.h>
#include <dbdef.h>

/* Diagnostics output buffer must handle insanely long names */
#define DIAG_OUTPUT_BUFFER_SIZE 700


extern list_header_t db_head;

/*
 * The EMIT macro is set up to send the output string to the
 * emit_routine callback for processing.  If the emit_routine returns
 * a negative number (error of some sort), the EMIT macro will force an
 * exit from the routine.  CONTEXT_EMIT is used where errors must just
 * be ignored because of some context handling.  BREAK_EMIT will issue
 * a break statement instead of a return when an error is detected.
 */

#define EMIT if ((*emit_routine)(out,emit_arg) < 0) return(1)
#define CONTEXT_EMIT (*emit_routine)(out,emit_arg)
#define BREAK_EMIT if ((*emit_routine)(out,emit_arg) < 0) break

/*
 * Local prototypes
 */

static char *
replica_state_to_string (
    int                  ,
    char                 *);

static char *
replica_type_to_string (
    int                 ,
    char                *);

static char *
entry_type_to_string (
    int               ,
    char              *);


static char 
*replica_state_to_string (int   replica_state,
                          char  *RS_string)
{
    switch (replica_state) {
    case RS_newDir:       (void)strcpy(RS_string, "New Directory"); break;
    case RS_newReplica:   (void)strcpy(RS_string, "New Replica"); break;
    case RS_on:           (void)strcpy(RS_string, "On"); break;
    case RS_dyingDir:     (void)strcpy(RS_string, "Dying Directory"); break;
    case RS_dyingReplica: (void)strcpy(RS_string, "Dying Replica"); break;
    case RS_dead:         (void)strcpy(RS_string, "Dead"); break;
    default: (void)sprintf(RS_string, "*UNKNOWN* (%d)", replica_state);
    }

    return(RS_string);
}


static char 
*replica_type_to_string (int   replica_type,
                         char  *RT_string)
{
    switch (replica_type) {
    case RT_master:    (void)strcpy(RT_string, "Master"); break;
    case RT_secondary: (void)strcpy(RT_string, "Secondary"); break;
    case RT_readOnly:  (void)strcpy(RT_string, "Readonly"); break;
#ifdef RT_gda
    case RT_gda:       (void)strcpy(RT_string, "GDA"); break;
#endif
    default: (void)sprintf(RT_string, "*UNKNOWN* (%d)", replica_type);
    }

    return(RT_string);
}

static char 
*entry_type_to_string (int   entry_type,
                       char  *ET_string)
{
    switch (entry_type) {
    case ET_directory:     (void)strcpy(ET_string, "Directory"); break;
    case ET_object:        (void)strcpy(ET_string, "Object"); break;
    case ET_childPointer:  (void)strcpy(ET_string, "ChildPointer"); break;
    case ET_softlink:      (void)strcpy(ET_string, "Softlink"); break;
    case ET_clearinghouse: (void)strcpy(ET_string, "PseudoDirectory"); break;
    default: (void)sprintf(ET_string, "*UNKNOWN* (%d)", entry_type);
    }

    return(ET_string);
}





int 
db_diag_dump_file_info (int                 detail_level,
                        file_info_t         *file_p,
                        int (*emit_routine  )(char *, void *),
                        void *            emit_arg)
{
    LOG_ASCII_BUF_ObjUID(objuidBuf)
    LOG_ASCII_BUF_time_local(timelocalBuf)
    char out[DIAG_OUTPUT_BUFFER_SIZE];


    (void)sprintf(out,
	"___ File Info Block: 0x%lx __________________________________",
            file_p); EMIT;

    if (file_p == NULL) {
        (void)sprintf(out,
		"** Invalid file information block referenced"); EMIT;
        return (-1);
    }

    (void)sprintf(out, "File:  %s", file_p->file_ch_p->ch_filename); EMIT;

    (void)sprintf(out, "CHID: %s", deb_ascii_ObjUID(objuidBuf,
            &file_p->file_ch_p->ch_uid)); EMIT;

    (void)sprintf(out, "List:  (Next: 0x%lx    Prev: 0x%lx)",
            file_p->file_queue.next_p, file_p->file_queue.prev_p); EMIT;

    (void)sprintf(out, "Mutex: 0x%lx      State changed cv: 0x%lx",
            file_p->file_mutex, file_p->file_state_changed); EMIT;

    switch (file_p->file_state) {
    case file_init:    (void)sprintf(out, "State: Initialized"); break;
    case file_recover: (void)sprintf(out, "State: Recovering"); break;
    case file_loaded:  (void)sprintf(out, "State: Loaded (no users)"); break;
    case file_open:    (void)sprintf(out, "State: Open (users allowed)"); break;
    case file_ckpt:    (void)sprintf(out, "State: Checkpointing"); break;
    case file_ckpt_exclusive: (void)sprintf(out, "State: Checkpointing (exclusive)"); break;
    case file_rundown: (void)sprintf(out, "State: Rundown"); break;
    default: (void)sprintf(out, "State: *UNKNOWN* (%d)", file_p->file_state); break;
    }
    EMIT;

    (void)sprintf(out, "Users: %d        Writers: %d",
            file_p->file_users, file_p->file_write_count); EMIT;

    if (detail_level == 1) {
        const current_op_info_t *info_p = &file_p->file_op_info;
        char                     work[50];
        int                      idx;

        (void)sprintf(out, "Current Op Info:             (SOp completed: 0x%lx)",
                      info_p->currop_sop_completed); EMIT;
        (void)sprintf(out, "   Current ops: %d     Special ops: %d",
                      info_p->currop_current, info_p->currop_special); EMIT;
        (void)sprintf(out, "   Op Histogram:"); EMIT;
        (void)strcpy(out, "      ");
        for (idx = 0; idx < OP_CARDINALITY; idx++) {
            (void)sprintf(work, "%d/%d ", idx, info_p->currop_count[idx]);
            (void)strcat(out, work);
            if (strlen(out) > 64) {
                EMIT;
                (void)strcpy(out, "      ");
            }
        }
        EMIT;
    }

    (void)sprintf(out, "Management block: 0x%lx", file_p->file_ch_p); EMIT;

    (void)sprintf(out, "Free stream list: 0x%lx", file_p->file_free_stream_p); EMIT;

    if (file_p->file_root_dir_present) {
        (void)sprintf(out, "Root directory is present in this clearinghouse"); EMIT;
    } else {
        (void)sprintf(out, "Root directory is not present in this clearinghouse"); EMIT;
    }

    (void)sprintf(out, "Root directory DHE: 0x%lx", file_p->file_root_dir_p); 
    EMIT;

    (void)sprintf(out, "Propagation"); EMIT;

    switch(file_p->file_prop_state) {
    case file_prop_Off:     (void)sprintf(out, "   State:     Off");     break;
    case file_prop_On:      (void)sprintf(out, "   State:     On");      break;
    case file_prop_Rundown: (void)sprintf(out, "   State:     Rundown"); break;
    case file_prop_Stopped: (void)sprintf(out, "   State:     Stopped"); break;
    case file_prop_Cancel:  (void)sprintf(out, "   State:     Cancel");  break;
    default: (void)sprintf(out, "   State:     *UNKNOWN* (%d)", file_p->file_prop_state);
    }
    EMIT;

    (void)sprintf(out, "   Stream:    0x%lx       Thread: 0x%lx",
            file_p->file_prop_stream_p, file_p->file_propagator); EMIT;

    (void)sprintf(out, "   Wait list: (Next: 0x%lx   Prev: 0x%lx)",
            file_p->file_prop_wait.next_p, file_p->file_prop_wait.prev_p);
    EMIT;

    (void)sprintf(out, "Background"); EMIT;

    (void)sprintf(out,
      "   Activator thread: 0x%lx  Process thread: 0x%lx  Force: %d  Inhibit: %d",
            file_p->file_back_activator, file_p->file_back_thread,
            file_p->file_back_force, file_p->file_back_inhibit); EMIT;

    (void)sprintf(out, "Checkpoint"); EMIT;

    (void)sprintf(out,
	"   Needed: %d   Thread: 0x%lx   Force: %d   Compression level: %d",
            file_p->file_ckpt_needed,  file_p->file_ckpt_thread,
            file_p->file_ckpt_force, file_p->file_ckpt_comp_level); EMIT;

    (void)sprintf(out, "   Last start:  %s",
	deb_ascii_time_local(timelocalBuf,&file_p->file_ckpt_last_start)); EMIT;
    (void)sprintf(out, "   Last finish: %s",
	deb_ascii_time_local(timelocalBuf,&file_p->file_ckpt_last_finish)); EMIT;
    if (((file_p->file_ckpt_dbytes_out + file_p->file_ckpt_fbytes_out) > 0) &&
        ((file_p->file_ckpt_dbytes_in + file_p->file_ckpt_fbytes_in) > 0)) {
        (void)sprintf(out, "   Compression        data      free     total    buckets");
        EMIT;
        (void)sprintf(out, "   Compression In:  %8d  %8d  %8d  %7d   (%d nodes)",
            file_p->file_ckpt_dbytes_in, file_p->file_ckpt_fbytes_in,
            (file_p->file_ckpt_dbytes_in + file_p->file_ckpt_fbytes_in),
            file_p->file_ckpt_buckets_in, file_p->file_ckpt_node_count); EMIT;
        (void)sprintf(out, "   Compression Out: %8d  %8d  %8d  %7d   %f : 1",
            file_p->file_ckpt_dbytes_out, file_p->file_ckpt_fbytes_out,
            (file_p->file_ckpt_dbytes_out + file_p->file_ckpt_fbytes_out),
            file_p->file_ckpt_buckets_out,
            (float)(1.00 / ((float)(file_p->file_ckpt_dbytes_out + 
                                    file_p->file_ckpt_fbytes_out) /
                            (float)(file_p->file_ckpt_dbytes_in + 
                                    file_p->file_ckpt_fbytes_in))));
        EMIT;
    } else {
        (void)sprintf(out, "   No checkpoint compression statistics available"); EMIT;
    }

    (void)sprintf(out, "Transaction Log"); EMIT;

    switch(file_p->file_tlog_state) {
    case TLOG_FILE_OPEN:   (void)sprintf(out, "   State: Open"); break;
    case TLOG_FILE_CLOSED: (void)sprintf(out, "   State: Closed"); break;
    default: (void)sprintf(out, "   State: *UNKNOWN* (%d)", file_p->file_tlog_state);
    }
    EMIT;

    (void)sprintf(out, "   Mutex: 0x%lx    Buffer: 0x%lx  (%d bytes)    Next block: %d",
            file_p->file_tlog_mutex, file_p->file_tlog_buf_p,
            file_p->file_tlog_buf_len, file_p->file_tlog_next_block); EMIT;


    if (detail_level == 1) {
        char              work[50];
        int               idx;
        dir_hash_entry_t *hash_p;

        (void)sprintf(out, "UID Hash Table"); EMIT;
        (void)strcpy(out, "   ");
        for (idx = 0; idx < UID_HASH_LEN; idx++) {
            if (file_p->file_uid_table[idx]) {
                (void)sprintf(work, "0x%08x [%3d]  ",
                        file_p->file_uid_table[idx], idx);
                (void)strcat(out, work);
                if (strlen(out) > 64) {
                    EMIT;
                    (void)strcpy(out, "   ");
                }
                hash_p = file_p->file_uid_table[idx];
                while (TRUE) {
		    hash_p = hash_p->dir_uid_collision_p;
		    if (!hash_p) break;
                    (void)sprintf(work, "0x%08x [col]  ", hash_p);
                    (void)strcat(out, work);
                    if (strlen(out) > 64) {
                        EMIT;
                        (void)strcpy(out, "   ");
                    }
                }
            }
        }
        EMIT;

        (void)sprintf(out, "Name Hash Table"); EMIT;
        (void)strcpy(out, "   ");
        for (idx = 0; idx < NAME_HASH_LEN; idx++) {
            if (file_p->file_name_table[idx]) {
                (void)sprintf(work, "0x%08x [%3d]  ",
                        file_p->file_name_table[idx], idx);
                (void)strcat(out, work);
                if (strlen(out) > 64) {
                    EMIT;
                    (void)strcpy(out, "   ");
                }
                hash_p = file_p->file_name_table[idx];
                while (TRUE) {
		    hash_p = hash_p->dir_name_collision_p;
		    if (!hash_p) break;
                    (void)sprintf(work, "0x%08x [col]  ", hash_p);
                    (void)strcat(out, work);
                    if (strlen(out) > 64) {
                        EMIT;
                        (void)strcpy(out, "   ");
                    }
                }
            }
        }
        EMIT;
    }

    return (1);
}

/* -------------------------------------------------------------------------- */


int 
db_diag_dump_file_info_list (int                 detail_level,
                             int (*emit_routine  )(char *, void *),
                             void *            emit_arg)
{

    char         out[DIAG_OUTPUT_BUFFER_SIZE];
    file_info_t *file_p;
    int          status = 1;
    int          lock_state;
    int thstatus;


    lock_state = dthread_trylock(&dns_server_mutex);
    if (lock_state == 1) 
    {
	for (file_p = (file_info_t *)db_head.next_p;
	     file_p != (file_info_t *)&db_head;
	     file_p = (file_info_t *)file_p->file_queue.next_p) {
            if (detail_level == 0) {
                (void)sprintf(out, "  0x%lx  (%s)", file_p,
                        file_p->file_ch_p->ch_filename);
                BREAK_EMIT;
            } else {
                if (!(status = db_diag_dump_file_info(0, file_p,
                                           emit_routine, emit_arg))) {
                    break;
                }
            }
	}

	if ((thstatus = dthread_unlock(&(dns_server_mutex))) < 0)
	{
	  dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
	}
    } 
    else 
    {
        (void)sprintf(out, "  dns_server_mutex locked!  Try later."); EMIT;
        (void)sprintf(out, "  List head is 0x%lx", db_head.next_p); EMIT;
    }

    return(status);
}


/* -------------------------------------------------------------------------- */

int 
db_diag_dump_dir_hash_entry (int                 detail_level,
                             dir_hash_entry_t    *hash_p,
                             int (*emit_routine  )(char *, void *),
                             void *            emit_arg)
{
    LOG_ASCII_BUF_FullName(fullnameBuf)
    LOG_ASCII_BUF_ObjUID(objuidBuf)
    LOG_ASCII_BUF_Timestamp(timestampBuf)
    char out[DIAG_OUTPUT_BUFFER_SIZE];
    char RT_string[20];
    char RS_string[20];
    char ET_string[30];


    (void)sprintf(out,
	"___ Dir Hash Entry: 0x%lx ___________________________________",
            hash_p); EMIT;

    if (hash_p == NULL) {
        (void)sprintf(out, "** Invalid directory hash entry referenced"); EMIT;
        return (-1);
    }

    (void)sprintf(out, "Name: %s (ID: %s)",
            deb_ascii_FullName(fullnameBuf, hash_p->dir_name),
            deb_ascii_ObjUID(objuidBuf, (ObjUID_t *)hash_p->dir_uid)); EMIT;

    (void)sprintf(out, "Cell-relative name: %s ",
            deb_ascii_FullName(fullnameBuf, hash_p->dir_relative_name)); EMIT;

    (void)sprintf(out, "Replica state:   %-15s   Replica type: %s",
            replica_state_to_string(hash_p->dir_replica_state, RS_string),
            replica_type_to_string(hash_p->dir_replica_type, RT_string)); EMIT;

    switch (hash_p->dir_root) {
    case 1:
	(void)sprintf(out, "Root directory");
	break;
    case 0:
	(void)sprintf(out, "Not root directory");
	break;
    default:
	(void)sprintf(out, "Strange dir_root: %d", hash_p->dir_root);
	break;
    }; EMIT;

    (void)sprintf(out, "FastPath pointers:"); EMIT;
    (void)sprintf(out, "   AllUpTo:     0x%lx", hash_p->dir_allupto_p); EMIT;
    (void)sprintf(out, "   CellAliases: 0x%lx", hash_p->dir_cellalias_set_p); EMIT;

    (void)sprintf(out, "Last update:     %s",
            deb_ascii_Timestamp(timestampBuf, hash_p->dir_lastupdate)); EMIT;

    (void)sprintf(out, "Collisions:      UID: 0x%lx    Name: 0x%lx",
            hash_p->dir_uid_collision_p, hash_p->dir_name_collision_p); EMIT;

    (void)sprintf(out, "Hash indices:    UID: %d      Name: %d",
            hash_p->dir_uid_index, hash_p->dir_name_index); EMIT;

    (void)sprintf(out, "File info block: 0x%lx", hash_p->dir_file_info_p); EMIT;

    (void)sprintf(out, "Mutex:           0x%lx      State changed cv: 0x%lx",
            hash_p->dir_mutex, hash_p->dir_state_changed); EMIT;

    switch (hash_p->dir_state) {
    case dir_init:    (void)sprintf(out, "State: Initialized"); break;
    case dir_on:      (void)sprintf(out, "State: On");          break;
    case dir_rundown: (void)sprintf(out, "State: Rundown");     break;
    default: (void)sprintf(out, "State: *UNKNOWN* (%d)", hash_p->dir_state);
    }
    EMIT;

    (void)sprintf(out, "Users:           %d", hash_p->dir_users); EMIT;

    (void)sprintf(out, "Dir. bucket:     0x%lx     Type: %s",
            hash_p->dir_p, 
            entry_type_to_string(hash_p->dir_type, ET_string)); EMIT;

    (void)sprintf(out, "Upd. access. cv: 0x%lx     Access owner stream: 0x%lx",
            hash_p->dir_update_accessable, hash_p->dir_update_access_owner);
    EMIT;

    (void)sprintf(out, "Root bucket:     0x%lx", hash_p->dir_entry_p); EMIT;

    (void)sprintf(out,
	"Entry list lock: 0x%lx     List: (Next: 0x%lx  Prev: 0x%lx)",
            hash_p->dir_entry_locks_lock,
            hash_p->dir_entry_locks.next_p, hash_p->dir_entry_locks.prev_p);
    EMIT;

    (void)sprintf(out, "Skulk lock:      0x%lx     Skulk in progress: %d",
            hash_p->dir_skulk_lock, hash_p->dir_skulk_in_progress); EMIT;

    (void)sprintf(out, "Skulk done cv:   0x%lx     Skulk last status: 0x%lx(%d)",
            hash_p->dir_skulk_completed,
            hash_p->dir_skulk_last_status, hash_p->dir_skulk_last_status); EMIT;

    (void)sprintf(out, "Propagation sts: 0x%lx(%d)   Last propagation: %s",
            hash_p->dir_prop_status, hash_p->dir_prop_status,
            deb_ascii_Timestamp(timestampBuf, hash_p->dir_prop_time)); EMIT;
               
    return(1);
}


/* -------------------------------------------------------------------------- */

int 
db_diag_dump_dir_hash_table (int                 detail_level,
                             file_info_t         *file_p,
                             int (*emit_routine  )(char *, void *),
                             void *            emit_arg)
{
    LOG_ASCII_BUF_ObjUID(objuidBuf)
    LOG_ASCII_BUF_FullName(fullnameBuf)
    char              out[DIAG_OUTPUT_BUFFER_SIZE];
    char              RT_string[20];
    char              RS_string[20];
    int               idx;
    dir_hash_entry_t *hash_p;
    int               open_slots          = 0;
    int               max_collision_depth = 0;
    int               sum_collisions      = 0;
    int               collision_depth;


    (void)sprintf(out,
	"___ File Info Block: 0x%lx __________________________________",
            file_p); EMIT;

    if (file_p == NULL) {
        (void)sprintf(out, "** Invalid file information block referenced"); EMIT;
        return (-1);
    }

    (void)sprintf(out, "File:  %s", file_p->file_ch_p->ch_filename); EMIT;

    (void)sprintf(out, "CHID: %s", deb_ascii_ObjUID(objuidBuf,
            &file_p->file_ch_p->ch_uid)); EMIT;

    (void)sprintf(out, "UID Hash Table"); EMIT;

    for (idx = 0; idx < UID_HASH_LEN; idx++) {
        hash_p = file_p->file_uid_table[idx];
        if (hash_p) {
            if (detail_level == 0) {
                (void)sprintf(out, "  0x%08x [%3d] %s",
                        hash_p, idx,
                        deb_ascii_FullName(fullnameBuf, hash_p->dir_name));
                if (hash_p->dir_skulk_in_progress) {
                    (void)strcat(out, " (skulking)");
                }
                EMIT;
                (void)sprintf(out, "     %-50s [%s/%s]",
                     deb_ascii_ObjUID(objuidBuf, (ObjUID_t *)hash_p->dir_uid),
                     replica_type_to_string(hash_p->dir_replica_type, RT_string),
                     replica_state_to_string(hash_p->dir_replica_state, RS_string));
                EMIT;
            } else if (detail_level == 1) {
                (void)db_diag_dump_dir_hash_entry(0, hash_p, emit_routine, emit_arg);
            }

            collision_depth = 0;

            while (TRUE) {
		hash_p = hash_p->dir_uid_collision_p;
		if (!hash_p) break;
                collision_depth++;
                if (detail_level == 0) {
                    (void)sprintf(out, "  0x%08x [col] %s",
                            hash_p,
                            deb_ascii_FullName(fullnameBuf, hash_p->dir_name));
                    if (hash_p->dir_skulk_in_progress) {
                        (void)strcat(out, " (skulking)");
                    }
                    EMIT;
                    (void)sprintf(out, "     %-50s [%s/%s]",
                            deb_ascii_ObjUID(objuidBuf,
					     (ObjUID_t *)hash_p->dir_uid),
                            replica_type_to_string(hash_p->dir_replica_type,
                                                   RT_string),
                            replica_state_to_string(hash_p->dir_replica_state,
                                                    RS_string));
                EMIT;
                } else if (detail_level == 1) {
                    (void)db_diag_dump_dir_hash_entry(0, hash_p, emit_routine, emit_arg);
                }
            }

            if (collision_depth > max_collision_depth) {
                max_collision_depth = collision_depth;
            }
            sum_collisions += collision_depth;

        } else {
            open_slots++;
        }
    }

    (void)sprintf(out,
	"  Slots: %d   Open: %d   Collisions: %d   Max. Depth: %d",
            UID_HASH_LEN, open_slots, sum_collisions, max_collision_depth);
    EMIT;

    open_slots = 0;
    sum_collisions = 0;
    max_collision_depth = 0;

    (void)sprintf(out, "Name Hash Table"); EMIT;
    for (idx = 0; idx < NAME_HASH_LEN; idx++) {
        hash_p = file_p->file_uid_table[idx];
        if (hash_p) {
            collision_depth = 0;

            while (TRUE) {
		hash_p = hash_p->dir_uid_collision_p;
		if (!hash_p) break;
                collision_depth++;
            }

            if (collision_depth > max_collision_depth) {
                max_collision_depth = collision_depth;
            }

            sum_collisions += collision_depth;

        } else {
            open_slots++;
        }
    }

    (void)sprintf(out,
	"  Slots: %d   Open: %d   Collisions: %d   Max. Depth: %d",
            NAME_HASH_LEN, open_slots, sum_collisions, max_collision_depth);
    EMIT;

    return(1);
}


/* -------------------------------------------------------------------------- */

int 
db_diag_dump_nsgbl_ns (int                 detail_level,
                       nsgbl_ns_t          *ns_p,
                       int (*emit_routine  )(char *, void *),
                       void *            emit_arg)
{
    LOG_ASCII_BUF_FullName(fullnameBuf)
    LOG_ASCII_BUF_time_quad(timequadBuf)
    char out[DIAG_OUTPUT_BUFFER_SIZE];
    int thstatus;
    char timebuf[100];

    (void)sprintf(out,
	"___ Nameserver Entry: 0x%lx _________________________________",
            ns_p); EMIT;

    (void)sprintf(out, "Mutex: 0x%lx     State changed cv: 0x%lx",
            ns_p->ns_mutex, ns_p->ns_state_changed); EMIT;

    switch (ns_p->ns_state) {
    case dns_Off:     (void)sprintf(out, "State: Off");            break;
    case dns_Initial: (void)sprintf(out, "State: Initializing");   break;
    case dns_On:      (void)sprintf(out, "State: On");             break;
    case dns_Shut:    (void)sprintf(out, "State: Shutting down");  break;
    case dns_Broken:  (void)sprintf(out, "State: Broken");         break;
    default: (void)sprintf(out,"State:  *UNKNOWN* (%d)", ns_p->ns_state);
    }
    EMIT;

    utc_ascreltime(timebuf, sizeof(timebuf), &ns_p->ns_create_time);
    (void)sprintf(out, "Create time: %s", timebuf); EMIT;

    (void)sprintf(out, "Protocol:    min: %d.%d   max: %d.%d",
            EXT8(ns_p->ns_min_protocol.cv_major),
            EXT8(ns_p->ns_min_protocol.cv_minor),
            EXT8(ns_p->ns_max_protocol.cv_major),
            EXT8(ns_p->ns_max_protocol.cv_minor)); EMIT;

    (void)sprintf(out, "Reassembly:  %s",
	deb_ascii_time_quad(timequadBuf,&ns_p->ns_ReassemblyTimer)); EMIT;

    (void)sprintf(out, "Future Skew: %s",
	deb_ascii_time_quad(timequadBuf,&ns_p->ns_future_skew)); EMIT;

    (void)sprintf(out, "Skulk time:  %s",
	deb_ascii_time_quad(timequadBuf,&ns_p->ns_skulk_time)); EMIT;

    if (detail_level == 1) {
        (void)sprintf(out, "Mgmt. directory:  %s", ns_p->ns_mgmt_directory); EMIT;
        (void)sprintf(out, "Diag. mailbox:    %s", ns_p->ns_diag_name_p);    EMIT;
        (void)sprintf(out, "TA receiver id:   0x%lx", ns_p->ns_ta_id);        EMIT;
        (void)sprintf(out, "Advertiser block: 0x%lx", ns_p->ns_advertiser);   EMIT;
        (void)sprintf(out, "Loop first:       0x%lx", ns_p->ns_loop_first);   EMIT;
        (void)sprintf(out, "Loop free:        0x%lx", ns_p->ns_loop_free);    EMIT;
    }

    (void)sprintf(out, "CH count:    %d     List: ( Next: 0x%lx    Prev: 0x%lx )",
            ns_p->ns_clearinghouse_count,
            ns_p->ns_clearinghouses.next_p, ns_p->ns_clearinghouses.prev_p);
    EMIT;

    if (detail_level == 1) {
        int nslocked;

        (void)sprintf(out, "Clearinghouses"); EMIT;

        nslocked = dthread_trylock(&(ns_p->ns_mutex));
        if (nslocked == 1) {                    /* We have acquired the lock */
	    nsgbl_ch_t *ch_p = nsgbl_ch_next(ns_p, NULL);

            while (ch_p) {
                (void)sprintf(out, "   %s  ( chb 0x%lx )",
                       deb_ascii_FullName(fullnameBuf, &ch_p->ch_name),
                       ch_p); BREAK_EMIT;
	        ch_p = nsgbl_ch_next(ns_p, ch_p);
            }
	    
	    if ((thstatus = dthread_unlock(&(ns_p->ns_mutex))) < 0)
	    {
	      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_A_MSG, (long)thstatus);
            }
        } 
	else 
	{
            /*
             * The structure is locked by a thread.  The safe thing to do
             * is not display the list right now.
             */
            (void)sprintf(out, "    Lock active!  List will not be accessed.");
            EMIT;
        }
    }

    return(1);
}


/* -------------------------------------------------------------------------- */


int 
db_diag_dump_nsgbl_ch (int                 detail_level,
                       nsgbl_ch_t          *ch_p,
                       int (*emit_routine  )(char *, void *),
                       void *            emit_arg)
{
    LOG_ASCII_BUF_Timestamp(timestampBuf)
    LOG_ASCII_BUF_ObjUID(objuidBuf)
    LOG_ASCII_BUF_time_quad(utcBuf)
    LOG_ASCII_BUF_FullName(fullnameBuf)
    LOG_ASCII_BUF_SimpleName(simplenameBuf)
    char out[DIAG_OUTPUT_BUFFER_SIZE];


    (void)sprintf(out,
	"___ Clearinghouse Entry: 0x%lx ______________________________",
            ch_p); EMIT;

    if (ch_p == NULL) {
        (void)sprintf(out, "** Invalid clearinghouse mgmt. block referenced"); EMIT;
        return (-1);
    }

    (void)sprintf(out, "List:   (Next: 0x%lx  Prev: 0x%lx)",
            ch_p->ch_clearinghouses.next_p, ch_p->ch_clearinghouses.prev_p);
    EMIT;

    (void)sprintf(out, "Name:    %s", deb_ascii_FullName(fullnameBuf, &ch_p->ch_name));
    EMIT;

    (void)sprintf(out, "nsb:     0x%lx", ch_p->ch_nameserver); EMIT;

    (void)sprintf(out, "Mutex:   0x%lx        State changed cv: 0x%lx",
            ch_p->ch_mutex, ch_p->ch_state_changed); EMIT;

    switch (ch_p->ch_state) {
    case dns_Off:     (void)sprintf(out, "State:   Off");            break;
    case dns_Initial: (void)sprintf(out, "State:   Initializing");   break;
    case dns_On:      (void)sprintf(out, "State:   On");             break;
    case dns_Shut:    (void)sprintf(out, "State:   Shutting down");  break;
    case dns_Broken:  (void)sprintf(out, "State:   Broken");         break;
    default: (void)sprintf(out, "State:   *UNKNOWN* (%d)", ch_p->ch_state);
    }
    EMIT;

    (void)sprintf(out, "CHCTS:   %s",
            deb_ascii_Timestamp(timestampBuf, &ch_p->ch_cts)); EMIT;

    (void)sprintf(out, "CHUID:   %s",
            deb_ascii_ObjUID(objuidBuf, &ch_p->ch_uid)); EMIT;

    (void)sprintf(out, "AltList: (Next: 0x%lx  Prev: 0x%lx)",
            ch_p->ch_alt_chcts.next_p, ch_p->ch_alt_chcts.prev_p); EMIT;

    if (detail_level > 0) {
            alt_chcts_list_t    *alt_chcts_p;

            alt_chcts_p = (alt_chcts_list_t *)LIST_FIRST(&ch_p->ch_alt_chcts);
            while (alt_chcts_p)
                {
                (void)sprintf(out, "         AltCHCTS: %s",
                    deb_ascii_Timestamp(timestampBuf, &alt_chcts_p->alt_chcts)); EMIT;
                alt_chcts_p = (alt_chcts_list_t *)LIST_NEXT(&ch_p->ch_alt_chcts, alt_chcts_p);
                }

    }
    (void)sprintf(out, "NSUID:   %s  (%s)",
            deb_ascii_ObjUID(objuidBuf, &ch_p->ch_nsuid),
            deb_ascii_SimpleName(simplenameBuf, &ch_p->ch_nsnickname)); EMIT;

    (void)sprintf(out, "File:    %s", ch_p->ch_filename); EMIT;

    return(1);
}



/* -------------------------------------------------------------------------- */


int 
db_diag_dump_stream (int                 detail_level,
                     db_stream_t         *stream_p,
                     int (*emit_routine  )(char *, void *),
                     void                 *emit_arg)
{
    LOG_ASCII_BUF_FullName(fullnameBuf)
    char out[DIAG_OUTPUT_BUFFER_SIZE];


    (void)sprintf(out,
	    "___ Database Stream: 0x%lx ______________________________",
            stream_p); EMIT;

    if (stream_p == NULL) {
        (void)sprintf(out, "** Invalid stream referenced"); EMIT;
        return (-1);
    }

    (void)sprintf(out, "Free list:      0x%lx", stream_p->db_next_p);  EMIT;
    (void)sprintf(out, "Function:       %d", stream_p->db_function); EMIT;
    (void)sprintf(out, "Directory Data: 0x%lx", &stream_p->db_dir); EMIT;
    (void)sprintf(out, "Entry Data:     0x%lx", &stream_p->db_dir); EMIT;
    (void)sprintf(out, "Dir Hash Entry: 0x%lx", stream_p->db_dir_hash_p); EMIT;
    (void)sprintf(out, "Key: %s",
			deb_ascii_FullName(fullnameBuf, stream_p->db_key));
    EMIT;

    return (1);
}


/* -------------------------------------------------------------------------- */

int 
db_diag_set_force_back (file_info_t  *file_p,
                        int          which_way)
{
    file_p->file_back_force = which_way;
 
    return (1);
}


/* -------------------------------------------------------------------------- */

int 
db_diag_set_force_ckpt (file_info_t  *file_p,
                        int          which_way)
{
    file_p->file_ckpt_force = which_way;
 
    return (1);
}


/* -------------------------------------------------------------------------- */

int 
db_diag_set_inhibit_back (file_info_t  *file_p,
                          int          which_way)
{
    dns_status_t thstatus;

    file_p->file_back_inhibit = which_way;
    if ((thstatus = dthread_broadcast(&(file_p->file_state_changed))) < 0) {
	dce_svc_printf(CDS_S_PTHREADCONDBROADCAST_A_MSG, (long)thstatus);
    }

    return (1);
}

/* -------------------------------------------------------------------------- */

int 
db_diag_set_ckpt_comp_level (file_info_t  *file_p,
                             int          level)
{
    int previous_level = file_p->file_ckpt_comp_level;

    file_p->file_ckpt_comp_level = level;
 
    return (previous_level);
}
