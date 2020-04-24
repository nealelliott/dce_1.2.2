/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log.c,v $
 * Revision 1.1.16.1  1996/10/03  15:09:08  arvind
 * 	Create rs_log_propq_delete to remove entries from the prop queue.
 * 	[1996/01/02  16:43 UTC  jrr  /main/HPDCE02/jrr_CHFts16735/1]
 *
 * 	Bump log version and create rs_log_pgo_global_member_t to handle global
 * 	group log entries.
 * 	[1996/08/08  14:09 UTC  jrr  /main/jrr_122_3/1]
 *
 * Revision 1.1.13.3  1996/02/18  00:17:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:44  marty]
 * 
 * Revision 1.1.13.2  1995/12/08  17:51:24  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/06/09  10:47 UTC  sekhar
 * 	Merge fixes to mothra.
 * 
 * 	HP revision /main/HPDCE02/sekhar_mothra_fix/1  1995/06/01  12:33 UTC  sekhar
 * 	[ CHFts13106 ] Replaced SVC code sec_sys_errno_text with sec_sys_errno_file_text.
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:06 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:57 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	changes for logging journal activity records
 * 	[1995/04/03  22:17 UTC  hondo  /main/SEC_migrate/2]
 * 
 * 	journal log initialization
 * 	[1995/03/28  19:26 UTC  hondo  /main/hondo_mig1/1]
 * 
 * 	Add software version log module  processing.
 * 	[1995/04/03  18:40 UTC  greg  /main/SEC_migrate/1]
 * 
 * Revision 1.1.9.1  1994/10/26  20:00:56  hondo
 * 	changes to support propagation of login activity.
 * 	[1994/10/25  15:00:05  hondo]
 * 
 * 	changes to support propagation of login activity.
 * 
 * 	changes for replication of login activity.
 * 
 * Revision 1.1.7.7  1994/09/22  20:08:15  cuti
 * 	memset entryp in rs_log_replay() after malloced.
 * 	[1994/09/22  02:57:01  cuti]
 * 
 * Revision 1.1.7.6  1994/08/25  19:08:58  sekhar
 * 	1994/08/23  sekhar
 * 	Put back messages for error_st() and fatal_st() calls.
 * 	[1994/08/25  19:08:43  sekhar]
 * 
 * Revision 1.1.7.5  1994/08/19  17:59:25  mdf
 * 	[ OT 11758 ] Missing parameter to dce_svc_printf()
 * 	             converted error_errno() calls to SVC calls.
 * 	[1994/08/19  13:10:56  mdf]
 * 
 * Revision 1.1.7.4  1994/08/04  16:13:29  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:11:19  mdf]
 * 
 * Revision 1.1.7.2  1994/06/02  21:25:06  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:18:23  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:55:43  mdf]
 * 
 * Revision 1.1.4.4  1993/12/17  22:23:51  hanfei
 * 	obsolete master exits itself.
 * 	[1993/12/17  22:02:34  hanfei]
 * 
 * Revision 1.1.4.3  1993/12/16  19:10:58  hanfei
 * 	rework rs_log_update_base_seqno (now
 * 	 rs_log_write_base_prop_seqno)
 * 	[1993/12/11  18:46:21  hanfei]
 * 
 * Revision 1.1.4.2  1993/10/29  16:38:06  hanfei
 * 	add rs_log_update_base_seqno() for change master work.
 * 	[1993/10/29  16:37:02  hanfei]
 * 
 * 	Second replication drop: bl5
 * 	[1992/10/06  16:10:09  burati]
 * 
 * Revision 1.1.2.3  1993/09/15  15:50:04  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.12  1993/04/08  21:48:32  emartin
 * 	CR 7543.  rs_log_propq_add, rs_log_propq_get_next swap
 * 	    upd_seqno bytes
 * 	[1993/04/08  21:45:00  emartin]
 * 
 * Revision 1.1.2.11  1993/02/19  17:12:50  ahop
 * 	dbcp: add rs_log_replay_entry
 * 	[1993/02/19  08:42:01  ahop]
 * 
 * Revision 1.1.2.10  1993/02/03  21:39:02  ahop
 * 	bl6e: prevent free of null pointer in propq_get_next
 * 	[1993/01/29  02:34:35  ahop]
 * 
 * Revision 1.1.2.9  1993/01/07  19:54:01  rps
 * 	[OT 6240] Truncate log file before replay
 * 	[1993/01/07  19:51:57  rps]
 * 
 * Revision 1.1.2.8  1992/12/29  16:35:05  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:23:41  zeliff]
 * 
 * Revision 1.1.2.7  1992/12/18  22:11:54  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:49:12  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  22:34:01  emartin
 * 	bl6c: rs_log_set_update_info, propq_mgmt routines
 * 
 * Revision 1.1.2.6  1992/12/01  18:28:55  sommerfeld
 * 	[OT6147] Fix hole at log file creation time.. fsync log on every successful
 * 	path through rs_log_truncate.
 * 	[1992/11/23  20:06:24  sommerfeld]
 * 
 * Revision 1.1.2.5  1992/11/04  19:35:56  ahop
 * 	Third replication drop: bl6a
 * 	rs_log_truncate - invoke rs_m_replist_get_new_base_prop_seqno
 * 	[1992/11/03  18:31:29  ahop]
 * 
 * Revision 1.1.2.4  1992/10/07  20:32:53  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:16:31  sekhar]
 * 
 * Revision 1.1.2.3  1992/09/29  20:12:46  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:36:58  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/31  18:24:29  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:11:35  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:43:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *      Registry Server - Log file management
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
    "$Id: rs_log.c,v 1.1.16.1 1996/10/03 15:09:08 arvind Exp $";
#endif


#define RS_LOG_ALLOCATE_GLOBALS

#include <sys/types.h>
#include <dce/idlbase.h>
#include <sys/file.h>

#include <netinet/in.h>

#include <stdio.h>

#include <un_io.h>
#include <un_strng.h>
#include <bstring.h>
#include <un_maloc.h>
#include <un_time.h>

#include <dce/dce.h>
#include <sec_svc.h>

#include <rs_base.h>
#include <rs_pvt.h>
#include <rs_log.h> 
#include <rs_lock.h>
#include <rs_util.h>
#include <rs_state.h>
#include <rs_master.h>
#include <rs_master_replist.h>
#include <rs_prop_tasks.h>

#include <rs_log_replist.h>
#include <rs_log_pgo.h>
#include <rs_log_acct.h>
#include <rs_log_acl.h>
#include <rs_log_policy.h>

#include <rsdb_pvt.h>
#include <rsdb_log.h>
#include <rsdb_file.h>
#include <rsdb_sysdep.h>

/* sigh, need to share the log fd w. rsdb_log 
 */
extern int     rs_log_fd; 

/* 
 * Local constants and types
 */

#define rs_c_log_file_version           6    /* Current version */
#define rs_c_log_file_old_supp_version  5    /* Oldest file version we
                                              * support with the current code. */
#define rs_c_log_base_rec_padding_size  200

typedef struct  rs_log_base_rec_t {
    unsigned32          log_len;        /* Length of committed part of the log */
    unsigned32          version;

    /* 
     * important sequence numbers that are saved 
     * when a checkpt is done and which guide a log replay.
     */
    rs_update_seqno_t   update_seqno;   /* seqno of last GLOBAL update. 
                                         * replay updates after this seqno 
                                         */
    sec_timeval_t       update_ts;      /* ts of last GLOBAL update */
    rs_update_seqno_t   base_prop_seqno;/* seqno of last update that was propagated 
                                         * to all replicas 
                                         */
    /* room for future expansion 
     */
    char                padding[rs_c_log_base_rec_padding_size];
}   rs_log_base_rec_t;

/* 
 * Size of buffer to read log entries into.
 * It must be at least as big as rs_log_hdr_t
 */
#define rs_c_log_read_buffsize  1024

/*
 * Local variables
 */

PRIVATE unsigned32          rs_log_len;             /* current length of log file */
PRIVATE rs_update_seqno_t   rs_log_update_seqno;    /* seqno of last GLOBAL update */
PRIVATE sec_timeval_t       rs_log_update_ts;       /* ts of last GLOBAL update */
PRIVATE rs_update_seqno_t   rs_log_base_prop_seqno; /* seqno of last update that 
                                                     * was propagated to all replicas.
                                                     */

PRIVATE struct rs_log_info {
    unsigned32         num_ops;
    rs_log_ops_t       *ops;
} log_info[RS_LOG_MODULE_LAST];


/*
 * Local Routines
 */
PRIVATE void log_set_len (
    unsigned32      log_len,
    int             fd
);

PRIVATE void rs_log_propq_add (
    rs_log_hdr_t    *lep,
    error_status_t  *st
);

PRIVATE void rs_log_propq_delete (
    rs_update_seqno_t   *lep,
    error_status_t      *st
);


 /*
  * r s _ l o g _ i n i t i a l i z e
  *
  * Initialize each logging module. Each module that uses the log file manager
  * should insert its initialization call here.  These initialization functions
  * should call the rs_log_init_module function below to register their
  * module's EPV.
  */
PUBLIC void rs_log_initialize
  (
    error_status_t   *status
  )
{

    rs_log_replist_init(status);
    if (BAD_STATUS(status))
        return;

    rs_log_pgo_init(status);
    if (BAD_STATUS(status))
        return;

    rs_log_acct_init(status);
    if (BAD_STATUS(status))
        return;

    rs_log_policy_init(status);
    if (BAD_STATUS(status))
        return;

    rs_log_acl_init(status);
    if (BAD_STATUS(status))
        return;

    rs_log_attr_sch_init(status);
    if (BAD_STATUS(status))
        return;

    rs_log_attr_init(status);
    if (BAD_STATUS(status))
        return;

    rs_log_login_act_init(status);
    if (BAD_STATUS(status))
        return;

    rs_log_login_reset_init(status);
    if (BAD_STATUS(status))
        return;

    rs_log_jrnl_act_init(status);
    if (BAD_STATUS(status))
        return;

    rs_log_sw_version_init(status);
    if (BAD_STATUS(status))
        return;
    /*
    * Add additional initialization calls here.
    */
}

 /*
  * r s _ l o g _ i n i t _ m o d u l e
  *
  * Initialize the EPV for a logging module.  Each module will supply
  * its module code, the number of operations it supports and the epv
  * for each of those operations.
  */
PUBLIC void rs_log_init_module
  (
    unsigned32      module_code,
    unsigned32      num_ops,
    rs_log_ops_t    *ops,
    error_status_t  *st
  )
{
    if (module_code >= RS_LOG_MODULE_LAST) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_log,
                       svc_c_sev_error,
                       sec_s_pgmerr);
        SET_STATUS(st, sec_s_pgmerr);
    } else {
        log_info[module_code].num_ops   = num_ops;
        log_info[module_code].ops       = ops;
        CLEAR_STATUS(st);
    }
}
  
/*
 * r s _ l o g _ s e t _ u p d a t e _ i n f o
 *
 * Set last update seqno and timestamp.
 * Also, set the base_propq_seqno to the last
 * update seqno.
 *
 * Called by a slave that has just been
 * (re-)initialized.  
 *
 * Caller should have the global write lock
 */
PUBLIC void rs_log_set_update_info
  (
    rs_update_seqno_t   *last_update_seqno,
    sec_timeval_t       *last_update_ts,
    error_status_t      *st
  )
{ 
    WRITE_LOCK(lock_log) {
                          
        if (!rs_state_master()) {
            rs_log_update_seqno = *last_update_seqno;
            rs_log_update_ts = *last_update_ts;
            rs_log_base_prop_seqno = *last_update_seqno; 
            CLEAR_STATUS(st);
        }
        else {
            SET_STATUS(st, sec_rgy_rep_master);
        }

    } END_WRITE_LOCK;
}

PUBLIC void rs_log_get_update_info
  (
    rs_update_seqno_t   *last_update_seqno,
    sec_timeval_t       *last_update_ts,
    error_status_t      *st
  )
{
    READ_LOCK(lock_log) {

        *last_update_seqno = rs_log_update_seqno;
        *last_update_ts = rs_log_update_ts;

    } END_READ_LOCK;

    CLEAR_STATUS(st);
}

PUBLIC void rs_log_create
  (
    rs_update_seqno_t   *update_seqno,
    error_status_t      *st
  )
{
    rs_log_base_rec_t   log_base_info,
                        *lbp;

    lbp = &log_base_info;

    /*
     * Guarantee empty base record
     */
    bzero((Pointer) lbp, sizeof(*lbp));

    lbp->log_len         = sizeof(*lbp);
    lbp->version         = rs_c_log_file_version;
    lbp->update_seqno    = *update_seqno;
    rs_util_gettime(&lbp->update_ts);
    lbp->base_prop_seqno = *update_seqno;

    rsdb_log_create((Pointer) lbp, lbp->log_len, st);
    if (BAD_STATUS(st)) {
        SET_STATUS(st, sec_rgy_rep_cannot_create_db);
        return;
    }

    rs_log_len          = lbp->log_len;
    rs_log_update_seqno = lbp->update_seqno;
    rs_log_update_ts    = lbp->update_ts;
    rs_log_base_prop_seqno = lbp->base_prop_seqno;

    CLEAR_STATUS(st);
}

 /*
  * r s _ l o g _ s e t u p _ l o g _ h e a d e r
  *
  * Utility function used by each logging operation.  This function will
  * create a log header with the appropriate update timestamp information.
  * The caller provides the module code and operation number.
  */
PUBLIC void rs_log_setup_log_header
  (
    rs_log_hdr_t               *hdr,
    unsigned32                 reclen,
    unsigned32                 module,
    unsigned32                 op
  )
{
    hdr->upd_reclen = reclen;
    hdr->upd_module = module;
    hdr->upd_op     = op;
}

 /*
  * r s _ l o g _ w r i t e
  *
  * Write a log record to the log.
  *
  * If no update_seqno is passed in and this is
  * a GLOBAL update, get next update seqno and ts.
  * 
  */
PUBLIC void rs_log_write
  (
    rs_log_hdr_t        *bp,
    rs_update_seqno_t   *update_seqno,
    sec_timeval_t       *update_ts
  )
{
    error_status_t      status;

    /*
     * Don't bother to append to the log if we are in the middle of
     * initialization.
     */
    if (rs_state.rep_state == rs_c_state_initializing) {
        return;
    }

    WRITE_LOCK(lock_log) {

        if (rs_log_fd < 0) {
            dce_svc_printf(SEC_RS_LOG_FILE_CLOSED_MSG);
        }
    
        if (update_seqno != NULL) {
            bp->upd_seqno = *update_seqno;
            bp->upd_ts = *update_ts;

            /* if copying propq don't change update_seqno
             */
            if (!rs_state_current_state(rs_c_state_becoming_master)) {
                rs_log_update_seqno = *update_seqno;
                rs_log_update_ts = *update_ts;
            }
        } 
        else {
            if (log_info[bp->upd_module].ops[bp->upd_op].update_type
                    == GLOBAL_UPDATE) {
                rs_util_seqno_next(&rs_log_update_seqno);
                rs_util_gettime(&rs_log_update_ts);
            }
            bp->upd_seqno = rs_log_update_seqno;
            bp->upd_ts = rs_log_update_ts;
        }

        /*
         * Append entry to log file
         */
        if (!rsdb_file_position(rs_log_fd, rsdb_file_position_end)) {
            dce_svc_printf(DCE_SVC(sec_svc_handle,"%s%s"),
                           sec_s_rs_log,
                           svc_c_sev_fatal | svc_c_action_abort,
                           sec_sys_errno_text,
                           "lseek",
                           strerror(errno));
            /* lseek since that is what rsdb_file_position uses */
        }
        if (write(rs_log_fd, (char *) bp, bp->upd_reclen) != bp->upd_reclen) {
            dce_svc_printf(DCE_SVC(sec_svc_handle,"%d"),
                           sec_s_rs_log,
                           svc_c_sev_fatal | svc_c_action_abort,
                           sec_sys_file_write_error,
                           rs_log_fd);
        }
    
        /*
         * Update log header with new length.
         */
        rs_log_len += bp->upd_reclen;
    
        /*
         * Commit the new log record by writing the new length to the beginning of
         * the log and sync'ing the file.
         */
        log_set_len(rs_log_len, rs_log_fd);
    
        /*
         * If we are a master site, add this entry to the in memory prop queue
         * and wake up the propagation tasks.
         */
        if ((rs_state_master() || 
             rs_state_current_state(rs_c_state_becoming_master) ) &&
            (log_info[bp->upd_module].ops[bp->upd_op].update_type
                     == GLOBAL_UPDATE) ) {
            rs_log_propq_add(bp, &status);
            if (BAD_STATUS(&status)) {
                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                               sec_s_rs_log,
                               svc_c_sev_error,
                               status);
                dce_svc_printf(DCE_SVC(sec_svc_handle,""),
                               sec_s_rs_log,
                               svc_c_sev_fatal | svc_c_action_abort,
                               sec_rs_log_propq_add_fail);
            }

            rs_prop_tasks_wakeup();
        } 

    } END_WRITE_LOCK;
}

 /*
  * l o g _ s e t _ l e n
  *
  * Set the log length.  Will update the log file length and sync the file.
  */
PRIVATE void log_set_len
  (
    unsigned32      log_len,
    int             fd
  )
{
    /*
     * Force new log record to disk...
     */
    rsdb_file_fsync(fd);

    /*
     * Commit the new log record by writing the new length to the beginning of
     * the log and sync'ing the file.
     */

    if (!rsdb_file_position(fd, rsdb_file_position_beginning)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle,"%s%s"),
                       sec_s_rs_log,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_sys_errno_text,
                       "lseek",
                       strerror(errno));
        /* lseek since that is what rsdb_file_position uses */
    }

    if (write(fd, (char *) &log_len, sizeof(log_len)) !=
                    sizeof(log_len)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle,"%d"),
                       sec_s_rs_log,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_sys_file_write_error,
                       fd);
    }

    rsdb_file_fsync(fd);
}

/*
 * write base_prop_seqno to the log file hdr.
 * set in memory rs_log_base_prop_seqno.
 */
PUBLIC void rs_log_write_base_prop_seqno 
  (
    rs_update_seqno_t   base_prop_seqno,
    error_status_t      *st
  )
{
    rs_log_base_rec_t   log_base_info,
                        *lbp;

    lbp = &log_base_info;

    rs_log_base_prop_seqno = base_prop_seqno;

    if (rs_log_fd < 0) {
        dce_svc_printf(SEC_RS_LOG_FILE_CLOSED_MSG);
    }

    /*
     * Force new log to disk...
     */
    rsdb_file_fsync(rs_log_fd);

    if (!rsdb_file_position(rs_log_fd, rsdb_file_position_beginning)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle,"%s%s"),
                       sec_s_rs_log,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_sys_errno_text,
                       "lseek",
                       strerror(errno));
        /* lseek since that is what rsdb_file_position uses */
    }

    if (read(rs_log_fd, (char *) lbp, sizeof(*lbp)) != sizeof(*lbp)) {
        dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG, "read", strerror(errno)) ;
        dce_svc_printf(DCE_SVC(sec_svc_handle,"%d"),
                       sec_s_rs_log,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_sys_file_read_error,
                       rs_log_fd);
    }

    lbp->base_prop_seqno = base_prop_seqno;

    if (!rsdb_file_position(rs_log_fd, rsdb_file_position_beginning)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle,"%s%s"),
                       sec_s_rs_log,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_sys_errno_text,
                       "lseek",
                       strerror(errno));
        /* lseek since that is what rsdb_file_position uses */
    }

    if (write(rs_log_fd, (char *) lbp, sizeof(*lbp)) != sizeof(*lbp)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle,"%d"),
                       sec_s_rs_log,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_sys_file_write_error,
                       rs_log_fd);
    }

    rsdb_file_fsync(rs_log_fd);
}

/*
 * r s _ l o g _ r e a d _ e n t r y
 *
 * Read an entry from the log.  
 *
 * entrypp points to a buffer which is at least as large as a log
 * header; sizep points to the buffer's length.  If the buffer cannot 
 * hold the entire entry which is being read, a new buffer is allocated 
 * and the old one is freed, and sizep is set to the new buffer's length.
 */
PRIVATE void rs_log_read_entry
  (
    int             fd,
    rs_log_hdr_t    **entrypp,
    unsigned32      *sizep,
    error_status_t  *st
  )
{
    rs_log_hdr_t    *entryp;
    unsigned32      buff_len;
    unsigned32      nb;
    char            *buff;

    entryp      = *entrypp;
    buff_len    = *sizep;

    /*
     * Read the log entry header to determine entry's size
     */
    if (read(fd, (char *) entryp, sizeof(*entryp)) != sizeof(*entryp)) {
        dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG, "read" , strerror(errno));
        dce_svc_printf(DCE_SVC(sec_svc_handle,"%d"),
                       sec_s_rs_log,
                       svc_c_sev_error,
                       sec_sys_file_read_error,
                       fd);
        SET_STATUS(st, sec_rgy_rep_cannot_read_db);
        return;
    }

    /*
     * Allocate a new buffer if the next item is larger than the
     * currently allocated buffer.
     */
    if (entryp->upd_reclen > buff_len) {
        char    *temp_p;

        temp_p = malloc(entryp->upd_reclen);
        if (temp_p == NULL) {
            dce_svc_printf(DCE_SVC(sec_svc_handle,""),
                           sec_s_rs_log,
                           svc_c_sev_error,
                           sec_s_no_memory);
            SET_STATUS(st, sec_s_no_memory);
            return;
        }

        /*
         * Copy the header info to the new buffer
         */
        bcopy((char *) entryp, temp_p, sizeof(*entryp));

        /*
         * Free the old buffer
         */
        free((char *) entryp);

        entryp      = (rs_log_hdr_t *) temp_p;
        buff_len    = entryp->upd_reclen;
        *entrypp    = entryp;
        *sizep      = buff_len;
    }

    nb = entryp->upd_reclen - sizeof(*entryp);
    buff = (char *) entryp;
    buff += sizeof(*entryp);
    if (read(fd, buff, nb) != nb) {
        dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG, "read" , strerror(errno));
        dce_svc_printf(DCE_SVC(sec_svc_handle,"%d"),
                       sec_s_rs_log,
                       svc_c_sev_error,
                       sec_sys_file_read_error,
                       fd);
        SET_STATUS(st, sec_rgy_rep_cannot_read_db);
        return;
    }

    CLEAR_STATUS(st);
}
/*
 * r s _ l o g _ r e p l a y _ e n t r y
 *
 * Invoke the replay operation on the input log entry.
 */
PUBLIC void rs_log_replay_entry
  (
    rs_log_hdr_t     *entryp,
    error_status_t   *st
  )
{
    rs_util_seqno_str_t seqno_str;

    CLEAR_STATUS(st);
    /*
     * Make sure this entry is valid - make sure the
     * module and op code are known.  We will report a
     * warning if the operation is out of range - but
     * will continue processing the log.
     */
    if (entryp->upd_module >= RS_LOG_MODULE_LAST ||
        entryp->upd_op > log_info[entryp->upd_module].num_ops) {
        SET_STATUS(st, sec_rgy_log_entry_out_of_range);
        return;
    }

    rs_util_seqno_sprintf(&entryp->upd_seqno, seqno_str);
    dce_svc_printf(SEC_RS_LOG_REPLAY_ENTRY_MSG,
                   rs_log_module_names[entryp->upd_module],
                   log_info[entryp->upd_module].ops[entryp->upd_op].printstring,
                   seqno_str);

    (log_info[entryp->upd_module].ops[entryp->upd_op].replay)
                             ((rs_log_rec_t *)entryp, st);
}


 /*
  * r s _ l o g _ r e p l a y
  *
  * Read the log file and replay the events stored in the file.  At a master
  * site only replay the events that are LOCAL updates or are later than the 
  * update_seqno which was saved with the last checkpoint. (At slave sites 
  * the log only contains updates which should be replayed, but at the master 
  * the log may contain propq entries that have already been saved to the 
  * database during a checkpoint but that must still exist in the log for 
  * propagation.)
  *
  * Should have locked at a higher level or should be in a state when no other 
  * tasks are running (ie. startup).
  */

PUBLIC void rs_log_replay
  (
    error_status_t   *st
  )
{
    rs_log_base_rec_t   log_base_info,
                        *lbp;
    rs_util_seqno_str_t seqno_str;
    rs_log_hdr_t        *entryp;
    unsigned32          buff_len;
    boolean32           master;
    rs_update_seqno_t   checkpt_seqno;

    lbp = &log_base_info;

    dce_svc_printf(SEC_RS_LOG_REPLAY_MSG);

    rsdb_log_close();
    rsdb_log_open();
    if (rs_log_fd < 0) {

        /**** SVC_OPEN :
         *    This path cannot be reached since if rs_log_fd < 0,
         *    rsdb_log_open() will abort ( used to call fatal_errno ; 
         *    now calls SVC routines ). 
         *
         */

        error("Unable to open log file");
        SET_STATUS(st, sec_rgy_rep_cannot_open_db);
        return;
    }

    if (read(rs_log_fd, (char *) lbp, sizeof(*lbp)) != sizeof(*lbp)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle,"%d"),
                       sec_s_rs_log,
                       svc_c_sev_error,
                       sec_sys_file_read_error,
                       rs_log_fd);
        SET_STATUS(st, sec_rgy_rep_cannot_read_db);
        return;
    }

    
    /* 
     * If log file version > current or log file version older than
     * the oldest version we support, fail.
     */
    if ((lbp->version > rs_c_log_file_version) ||
        (lbp->version < rs_c_log_file_old_supp_version)) {
        dce_svc_printf(SEC_RS_LOG_BAD_VERSION_MSG);
        SET_STATUS(st, sec_rgy_rep_bad_db_version);
        return;
    }

    rs_log_len          = lbp->log_len;
    rs_log_update_seqno = lbp->update_seqno;
    rs_log_update_ts    = lbp->update_ts;
    rs_log_base_prop_seqno = lbp->base_prop_seqno;

    /*
     * Flush any incomplete transaction
     */
    if (ftruncate(rs_log_fd, rs_log_len) < 0) {
        dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG, "ftruncate", strerror(errno));
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
                       sec_s_rs_log,
                       svc_c_sev_error,
                       sec_sys_file_ftruncate_fail,
                       rs_log_fd);
	SET_STATUS(st, sec_rgy_rep_cannot_read_db);
	return;
    }

    rs_util_seqno_sprintf(&lbp->base_prop_seqno, seqno_str);
    dce_svc_printf(SEC_RS_LOG_BASE_PROP_SEQ_MSG, seqno_str);

    master = rs_state_master();
    checkpt_seqno = rs_log_update_seqno;

    /*
     * Allocate a buffer to read the entries into
     */
    entryp = (rs_log_hdr_t *) malloc(rs_c_log_read_buffsize);
    memset(entryp, 0, sizeof(rs_log_hdr_t));
    buff_len = rs_c_log_read_buffsize;

    /*
     * Enable use of rpc_ss_allocate for decoding pickles.
     */
    rpc_ss_enable_allocate();

    for (rs_log_len = sizeof(*lbp); 
             rs_log_len < lbp->log_len;
             rs_log_len += entryp->upd_reclen) {

        rs_log_read_entry(rs_log_fd, &entryp, &buff_len, st);
        if (BAD_STATUS(st)) {                        
            goto err_out;
        }

        if (entryp->upd_module == RS_LOG_MODULE_NONE) continue;

        /*
         * If the update is a LOCAL update, apply it to the 
         * database.
         * If the update is a GLOBAL update and is later than the 
         * last checkpoint, then apply it to the database.
         */
        if ((log_info[entryp->upd_module].ops[entryp->upd_op].update_type
                     == LOCAL_UPDATE) ||
            (rs_util_seqno_cmp(&entryp->upd_seqno, &checkpt_seqno) > 0)) {

            rs_log_replay_entry(entryp, st);
            if(STATUS_EQUAL(st, sec_rgy_log_entry_out_of_range)) {
                dce_svc_printf(SEC_LOGENT_OUT_OF_BOUNDS_MSG);
                CLEAR_STATUS(st);
                continue;
            }
            if (BAD_STATUS(st)) {
                SEC_SVC_PRINT_MSG_ST(SEC_RS_LOG_REPLAY_ERR_MSG, *st);
                goto err_out;
            }
            if (log_info[entryp->upd_module].ops[entryp->upd_op].update_type
                        == GLOBAL_UPDATE) {
                rs_log_update_seqno = entryp->upd_seqno;
                rs_log_update_ts = entryp->upd_ts;
            }
        }

        /* 
         * If this is the master and the update is a GLOBAL update,
         * save the update on the propq
         */
        if (master &&
            (log_info[entryp->upd_module].ops[entryp->upd_op].update_type
                     == GLOBAL_UPDATE) &&
            (rs_util_seqno_cmp(&entryp->upd_seqno, 
                               &rs_log_base_prop_seqno) > 0)) {
            rs_log_propq_add(entryp, st);
            if (BAD_STATUS(st)) {
                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                               sec_s_rs_log,
                               svc_c_sev_error,
                               *st);
                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                               sec_s_rs_log,
                               svc_c_sev_error,
                               sec_rs_log_propq_add_fail);
                goto err_out;
            }
        }

    } /* for rs_log_len ... */

    CLEAR_STATUS(st);
    dce_svc_printf(SEC_RS_LOG_REPLAY_SUCC_MSG);
err_out:

    free(entryp);

    /*
     * Release any storage allocated by unpickling routines.
     */
    rpc_ss_disable_allocate();
}

 /*
  * r s _ l o g _ t r u n c a t e
  *
  * Truncate the log
  */
PUBLIC void rs_log_truncate
  (
    char             *postfix,
    error_status_t   *st
  )
{
    int                 new_fd, old_fd;
    sec_rgy_pname_t     new_log_fname;

    rs_log_base_rec_t   log_base_info,
                        *lbp;
    rs_log_hdr_t        *entryp;
    unsigned32          buff_len;
    unsigned32          new_log_len, old_log_len;
    unsigned32          log_len_read;

    rs_update_seqno_t   new_base_prop_seqno;

    lbp = &log_base_info;

    READ_LOCK(lock_log) {

        old_fd = rsdb_log_open_logfile(LOG_FILE, O_RDONLY);
        if (old_fd < 0) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, "%s"),
                           sec_s_rs_log,
                           svc_c_sev_fatal | svc_c_action_abort,
                           sec_sys_errno_file_text,
                           LOG_FILE,
                           strerror(errno));

            dce_svc_printf(SEC_RS_LOG_OPEN_FAIL_MSG);
        }
    
        if (read(old_fd, (char *) lbp, sizeof(*lbp)) != sizeof(*lbp)) {
            dce_svc_printf(DCE_SVC(sec_svc_handle,"%d"),
                           sec_s_rs_log,
                           svc_c_sev_fatal | svc_c_action_abort,
                           sec_sys_file_read_error, old_fd);
        }
    
        strcpy( (char *) new_log_fname, LOG_FILE);
        strcat( (char *) new_log_fname, postfix);
        new_fd = rsdb_log_open_logfile( (char *) new_log_fname,
                                        O_RDWR | O_CREAT | O_TRUNC);
        if (new_fd < 0) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, "%s"),
                           sec_s_rs_log,
                           svc_c_sev_fatal | svc_c_action_abort,
                           sec_sys_errno_file_text, 
                           new_log_fname,
                           strerror(errno));
        }

        old_log_len = lbp->log_len;
    
        lbp->log_len      = sizeof(*lbp);
        lbp->version      = rs_c_log_file_version;
        lbp->update_seqno = rs_log_update_seqno;
        lbp->update_ts    = rs_log_update_ts;
    
        if (rs_state_master()) {
            /*
             * rs_log_base_prop_seqno is not changed if any replica's
             * prop_seqno is unknown
             */
            rs_m_replist_get_base_prop_seqn(
                &rs_log_base_prop_seqno, &rs_log_update_seqno, 
                &new_base_prop_seqno);
            rs_log_base_prop_seqno = new_base_prop_seqno;
        } else {
            rs_log_base_prop_seqno = lbp->update_seqno;
        }
    
        lbp->base_prop_seqno = rs_log_base_prop_seqno;
    
        if (write(new_fd, (char *) lbp, sizeof(*lbp)) != sizeof(*lbp)) {
            dce_svc_printf(DCE_SVC(sec_svc_handle,"%d"),
                           sec_s_rs_log,
                           svc_c_sev_fatal | svc_c_action_abort,
                           sec_sys_file_write_error,
                           new_fd);
        }
    
        new_log_len = sizeof(*lbp);
        
        /*
         * If this is the master, save any outstanding updates on the
         * propq in the new log.  More specifically, if the base propagation 
         * seqno is not the same as the seqno of the last update to the log, 
         * copy the updates that are later than the base propagation seqno 
         * from the old log to the new log.   
         * (note: we could probably just read the in memory propq and
         * write it to the log - but this needs further study).
         */
        if (rs_util_seqno_cmp(&lbp->update_seqno, &lbp->base_prop_seqno) != 0) {
    
            /*
             * Allocate a buffer to read the entries into.
             * It must be at least as big as rs_log_hdr_t.
             */
            entryp = (rs_log_hdr_t *) malloc(rs_c_log_read_buffsize);
            buff_len = rs_c_log_read_buffsize;

            for (log_len_read = sizeof(*lbp); 
                    log_len_read < old_log_len;
                    log_len_read += entryp->upd_reclen) {

                rs_log_read_entry(old_fd, &entryp, &buff_len, st);
                if (BAD_STATUS(st)) {
                    dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                                   sec_s_rs_log,
                                   svc_c_sev_fatal | svc_c_action_abort,
                                   *st);
                }
    
                if (entryp->upd_module == RS_LOG_MODULE_NONE) continue;

                /* Local updates don't go on propq
                 */
                if (log_info[entryp->upd_module].ops[entryp->upd_op].update_type
                        == LOCAL_UPDATE) continue;
    
                if (rs_util_seqno_cmp(&entryp->upd_seqno, &lbp->base_prop_seqno) > 0) {
                    if (write(new_fd, (char *) entryp, entryp->upd_reclen)
                                != entryp->upd_reclen) {
                        dce_svc_printf(DCE_SVC(sec_svc_handle,"%d"),
                                       sec_s_rs_log,
                                       svc_c_sev_fatal | svc_c_action_abort,
                                       sec_sys_file_write_error,
                                       new_fd);
                    }

                    new_log_len += entryp->upd_reclen;
                }
            }

            free(entryp);
        }

        log_set_len(new_log_len, new_fd);

        rs_log_len = new_log_len;

        rsdb_file_close(old_fd);
        rsdb_file_close(new_fd);

    } END_READ_LOCK;

    CLEAR_STATUS(st);
}


PUBLIC void rs_log_get_propq_info
  (
    rs_update_seqno_t   *base_prop_seqno,
    rs_update_seqno_t   *last_update_seqno,
    error_status_t      *st
  )
{
    READ_LOCK(lock_log) {

        *base_prop_seqno = rs_log_base_prop_seqno;
        *last_update_seqno = rs_log_update_seqno;

    } END_READ_LOCK;

    CLEAR_STATUS(st);
}

 /*
  * r s _ l o g _ p r o p q _ a d d
  *
  * Write a log record to the in memory propagation queue.
  */
PRIVATE void rs_log_propq_add
  (
    rs_log_hdr_t    *lep,
    error_status_t  *st
  )
{
    rs_update_seqno_t   upd_seqno,
                        swab_upd_seqno;

    upd_seqno = lep->upd_seqno;
    swab_upd_seqno.high = htonl(upd_seqno.high);
    swab_upd_seqno.low = htonl(upd_seqno.low);

    rsdb_store(rsdb_log_db, (Pointer) &swab_upd_seqno, sizeof(upd_seqno), 
                    (Pointer) lep, lep->upd_reclen, st);
}

 /*
  * r s _ l o g _ p r o p q _ d e l e t e
  *
  * Remove the specified record from the in-memory propagation queue.
  */
PRIVATE void rs_log_propq_delete
  (
    rs_update_seqno_t   *upd_seqno,
    error_status_t      *st
  )
{
    rs_update_seqno_t   swab_upd_seqno;

    swab_upd_seqno.high = htonl(upd_seqno->high);
    swab_upd_seqno.low = htonl(upd_seqno->low);

    rsdb_delete(rsdb_log_db, (Pointer )&swab_upd_seqno, 
                sizeof(swab_upd_seqno), st);
}

 /*
  * r s _ l o g _ p r o p q _ g e t _ n e x t
  *
  * Get the next propagation item from the propagation queue.
  * blen is length of buffer pointed to by bp.
  *
  */
PUBLIC void rs_log_propq_get_next
  (
    rs_update_seqno_t   *upd_seqno,
    unsigned_char_p_t   *bp,
    unsigned32          *blen,
    error_status_t      *st
  )
{
    rs_update_seqno_t   tmp_upd_seqno,
                        swab_upd_seqno;
    unsigned32          tmp_blen;
    unsigned_char_p_t   tmp_bp,
                        tmp_bp2;
    char                *next_kp;
    int                 next_klen;
    char                *dp;
    int                 dlen;

    tmp_upd_seqno = *upd_seqno;
    swab_upd_seqno.high = htonl(tmp_upd_seqno.high);
    swab_upd_seqno.low = htonl(tmp_upd_seqno.low);

    tmp_blen = *blen;
    tmp_bp = *bp;

    READ_LOCK(lock_log) {

        rsdb_fetch_next(rsdb_log_db, (Pointer) &swab_upd_seqno, sizeof(rs_update_seqno_t),
            (Pointer *) &next_kp, &next_klen, (Pointer *) &dp, &dlen, st);
        if (GOOD_STATUS(st)) {

            if (dlen > tmp_blen) {
                tmp_bp2 = malloc(dlen);
                if (tmp_bp2 == NULL) {
                    SET_STATUS(st, sec_s_no_memory);
                }
                else {
                    if(tmp_bp)
                        free(tmp_bp);
                    tmp_bp = tmp_bp2;
                    tmp_blen = dlen;
                }
            }
            if (GOOD_STATUS(st)) {
                memcpy(tmp_bp, dp, dlen);
            }
        }

    } END_READ_LOCK;

    *blen = tmp_blen;
    *bp = tmp_bp;
}

 /*
  * r s _ l o g _ p r o p q _ c h e c k _ u p d a t e _ s e q n o
  *
  * Verify that the next update which a slave expects to
  * receive from the master is correct (the slave's next 
  * update must be one of the updates on the master's 
  * propq)
  */
PUBLIC void rs_log_propq_check_update_seqno
  (
    rs_replica_info_t   *info,
    error_status_t      *st
  )
{
    error_status_t     lst;
    boolean32          eq;

    CLEAR_STATUS(st);

    if (rs_util_seqno_cmp(&info->last_upd_seqno, &rs_log_update_seqno) > 0) {
        /* 
         * Slave's latest update seqno is greater than this 
         * master's latest update seqno.
         */
        SET_STATUS(st, sec_rgy_rep_update_seqno_low);
    }
    else
    if (rs_util_seqno_cmp(
        &info->last_upd_seqno, &rs_log_base_prop_seqno) < 0) {
        /*
         * Slave's latest update before master's earliest update in propq. 
         * propq doesn't have all the updates this slave needs,
         * will need to re-init the slave
         */
        SET_STATUS(st, sec_rgy_rep_update_seqno_high);
    }
}

/*
 * r s _ l o g _ p r o p q _ c l e a n
 *
 * Clean up entries on the prop queue that have been 
 * propagated to all replicas. Modify the replay log
 * to reflect the new base sequence number.
 * 
 * Must be called with a lock on the replist.
 * A lock on the log will be taken if changes are 
 * required.
 */
PUBLIC void rs_log_propq_clean (
    error_status_t              *st_p
)
{
    rs_update_seqno_t           base_prop_seqno;
    rs_update_seqno_t           new_base_prop_seqno;
    rs_update_seqno_t           tmp_base_prop_seqno;
    
    CLEAR_STATUS(st_p);

    base_prop_seqno = rs_log_base_prop_seqno;

    rs_m_replist_get_base_prop_seqn(&rs_log_base_prop_seqno, 
                                    &rs_log_update_seqno, 
                                    &new_base_prop_seqno);

    /*
     * If the new base is the same as the old base, there's no
     * work to do.
     */
    if (rs_util_seqno_cmp(&rs_log_base_prop_seqno, 
                          &new_base_prop_seqno) == 0) {
        return;
    }

    WRITE_LOCK(lock_log) {

        /* Update base propq seqno in log header */
        rs_log_write_base_prop_seqno(new_base_prop_seqno, st_p);

        if (GOOD_STATUS(st_p)) {
            rs_log_base_prop_seqno = new_base_prop_seqno;

            while(true) {
                /*
                 * Stop when we reach the first entry that
                 * has not been completely propagated.
                 */
                if (rs_util_seqno_cmp(&base_prop_seqno, 
                                      &rs_log_base_prop_seqno) > 0) {
                    break;
                }
                
                tmp_base_prop_seqno = base_prop_seqno;
                rs_util_seqno_next(&base_prop_seqno);
                
                /* Remove the enetry from the prop queue */
                rs_log_propq_delete(&tmp_base_prop_seqno, st_p);
                if (BAD_STATUS(st_p)) {
                    if (STATUS_EQUAL(st_p, sec_rgy_object_not_found)) {
                        CLEAR_STATUS(st_p);
                    }
                    else {
                        break;
                    }
                }
            }
        }

    } END_WRITE_LOCK;
}

 /*
  * r s _ l o g _ n o t h i n g _ t o _ p r o p a g a t e
  *
  * Predicate to determine if there is anything that should be 
  * propagated to this replica.
  */
PUBLIC boolean32 rs_log_nothing_to_propagate
  (
    rs_replica_prop_info_p_t    prop_info
  )
{
    if ((prop_info->prop_type == rs_c_replica_prop_update) &&
        (prop_info->last_upd_inited) &&
        (rs_util_seqno_cmp(
             &rs_log_update_seqno, &prop_info->last_upd_seqno) == 0) )
        return(true);
    else
        return(false);
}

 /*
  * r s _ l o g _ p r o p a g a t e
  *
  * Propagate an update from the master to a slave.
  */
PUBLIC void rs_log_propagate
  (
    handle_t                   handle,
    unsigned_char_p_t          bp,
    rs_replica_master_info_t   *master_info,
    boolean32                  propq_only,
    error_status_t             *st
  )
{  
    rs_log_hdr_t               *lep;

    lep = (rs_log_hdr_t *) bp;

    master_info->update_seqno = lep->upd_seqno;
    master_info->update_ts = lep->upd_ts;

    (log_info[lep->upd_module].ops[lep->upd_op].propagate)
        (handle, (rs_log_rec_t *) bp, master_info, propq_only, st);
    if (BAD_STATUS(st)) return;
}

 /*
  * r s _ l o g _ p r o p _ n o p
  *
  * Generic nop function.
  */
PUBLIC void rs_log_prop_nop
  (
    handle_t                   handle,
    rs_log_rec_t               *pep,
    rs_replica_master_info_t   *master_info,
    boolean32                  propq_only,
    error_status_t             *st
  )
{
    CLEAR_STATUS(st);
}


