/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: rs_prop_tasks.c,v $
 * Revision 1.1.18.2  1996/10/03  15:10:04  arvind
 * 	Add code to manage prop queue cleanup.
 * 	[1996/01/02  16:43 UTC  jrr  /main/HPDCE02/jrr_CHFts16735/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:39  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/11/07  20:05 UTC  psn  /main/HPDCE02/jrr_1.2_mothra/1]
 *
 * 	Fix parameter type for call to rs_task_join().
 * 	HP 	[1995/05/15  21:08 UTC  greg  /main/HPDCE02/2]
 *
 * 	merge in some replication fixes
 * 	HP 	[1995/01/30  22:20 UTC  emartin  /main/emartin_mothra2/4]
 *
 * 	prop_get_rep_binding - don't call rs_rep_bind_auth_to_slave
 * 	HP 	[1995/01/27  22:34 UTC  emartin  /main/emartin_mothra2/3]
 *
 * 	prop_updates_to_new_master - don't keep global write lock while sending all updates
 * 	HP 	[1995/01/26  21:31 UTC  emartin  /main/emartin_mothra2/2]
 *
 * 	do rs_prop_tasks_slave_alive
 * 	rs_prop_tasks_refrsh_slv_creds => refresh_slave_creds
 * 	HP 	[1995/01/25  21:40 UTC  emartin  /main/emartin_mothra2/1]
 *
 * 	prop_analyse_status - wrong_boot_time, etc error - call prop_maybe_bad_status
 * 	[1995/12/08  17:21:13  root]
 *
 * Revision 1.1.18.1  1996/05/10  13:22:42  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/12  21:39 UTC  pdd
 * 	clean up returned non-value for prop_driver()
 * 	[1996/02/29  22:02 UTC  pdd  /main/HPDCE02/pdd_branch_CHFts17478/1]
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:17 UTC  psn
 * 	Fix parameter type for call to rs_task_join().
 * 	[1995/11/07  20:05 UTC  psn  /main/HPDCE02/jrr_1.2_mothra/1]
 * 
 * 	Fix merge botches
 * 	[1995/04/24  17:36 UTC  greg  /main/HPDCE02/SEC_migrate_merge/2]
 * 
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:59 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	merge in some replication fixes
 * 	[1995/02/03  15:51 UTC  emartin  /main/HPDCE02/1]
 * 
 * 	prop_get_rep_binding - don't call rs_rep_bind_auth_to_slave
 * 	[1995/01/30  22:20 UTC  emartin  /main/emartin_mothra2/4]
 * 
 * 	prop_updates_to_new_master - don't keep global write lock while sending all updates
 * 	[1995/01/27  22:34 UTC  emartin  /main/emartin_mothra2/3]
 * 
 * 	do rs_prop_tasks_slave_alive
 * 	rs_prop_tasks_refrsh_slv_creds => refresh_slave_creds
 * 	[1995/01/26  21:31 UTC  emartin  /main/emartin_mothra2/2]
 * 
 * 	prop_analyse_status - wrong_boot_time, etc error - call prop_maybe_bad_status
 * 	[1995/01/25  21:40 UTC  emartin  /main/emartin_mothra2/1]
 * 
 * Revision 1.1.10.5  1994/08/25  20:47:38  sekhar
 * 	Put back SVC messages for fatal_st() and error_st() calls.
 * 	[1994/08/25  20:27:21  sekhar]
 * 
 * Revision 1.1.10.4  1994/08/19  17:59:28  mdf
 * 	1994/08/18 sekhar
 * 	Converted messages logged when rs_verbose set to SVC calls.
 * 	( mostly to NOTICE_VERBOSE calls ; a few to DCE_SVC_DEBUG calls ).
 * 	[1994/08/19  13:11:00  mdf]
 * 
 * Revision 1.1.10.3  1994/08/04  16:13:49  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:11:58  mdf]
 * 
 * Revision 1.1.10.1  1994/06/02  21:22:59  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:19:26  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/20  10:42:36  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, removed __STDC__ stuff.
 * 	[1994/05/19  19:59:53  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:56:32  mdf]
 * 
 * 	HP revision /main/GAMERA/KKOBS/KK/ODESSA_2/1  1994/05/03  18:54 UTC  hanfei
 * 	 merge
 * 
 * 	HP revision /main/GAMERA/KKOBS/KK/hanfei_ch_master/1  1994/04/20  15:16 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/HPDCE01/emartin_ch_master/1  1994/04/12  14:40 UTC  emartin
 * 	fix comments
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/26  20:03  mdf
 * 	merge
 * 
 * Revision 1.1.8.6  1993/12/20  22:31:23  hanfei
 * 	rs_prop_driver_sleep - fix missing long_incommunicado case
 * 	[1993/12/20  18:04:37  hanfei]
 * 
 * Revision 1.1.8.5  1993/12/17  22:23:55  hanfei
 * 	prop_driver_sleep - set sleep_cv first.
 * 	prop_tasks stop - destroy prop_tasks_lock mutex.
 * 	rs_prop_get_slave_seqno - chk whom slave thinks is
 * 	master.
 * 	do prop_mark_slv_for_init
 * 	[1993/12/17  22:02:51  hanfei]
 * 
 * Revision 1.1.8.4  1993/12/16  19:11:17  hanfei
 * 	pthread_testcancel is state_exiting and prop_updates
 * 	only use printf when in verbose mode.
 * 	[1993/12/15  22:14:25  hanfei]
 * 
 * 	do rs_prop_copy_propq
 * 	delete rs_prop_clear_replistq
 * 	rs_prop_tasks_stop checks whether prop_tasks_inited.
 * 	[1993/12/11  18:48:27  hanfei]
 * 
 * 	*** empty log message ***
 * 	[1993/12/08  04:37:16  hanfei]
 * 
 * Revision 1.1.8.3  1993/11/04  22:07:49  hanfei
 * 	clean out comments.
 * 	[1993/11/04  21:51:55  hanfei]
 * 
 * Revision 1.1.8.2  1993/10/29  17:03:35  hanfei
 * 	name correction for rs_prop_clear_replistq.
 * 	[1993/10/29  17:02:38  hanfei]
 * 
 * 	work for change master : add prop_this_update_to_new_master()
 * 	rs_prop_update_to_new_master() and rs_prop_clear_replistq().
 * 	[1993/10/29  16:57:42  hanfei]
 * 
 * Revision 1.1.5.2  1993/07/08  12:50:45  root
 * 	Initial King Kong branch
 * 	[1993/07/08  12:50:12  root]
 * 
 * 	Add printfs
 * 	[1992/12/16  19:30:35  emartin]
 * 
 * 	bl6c: do routines in this module
 * 	[1992/12/11  21:51:33  emartin]
 * 
 * 	bl6c: do this module
 * 	[1992/12/10  19:15:30  emartin]
 * 
 * Revision 1.1.3.2  1993/05/17  21:27:51  emartin
 * 	CR 7957.  rs_prop_tasks_wakeup - if !rs_prop_tasks_inited
 * 	just return.
 * 	[1993/05/14  18:06:41  emartin]
 * 
 * Revision 1.1.1.6  1993/05/04  20:39:24  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.3.11  1993/04/08  21:48:42  emartin
 * 	CR 7642. prop_driver sleep before checking whether
 * 	prop_tasks_inited
 * 	[1993/04/08  21:46:48  emartin]
 * 
 * Revision 1.1.3.10  1993/04/02  22:31:27  emartin
 * 	CR 7642. oops fix a comment
 * 	[1993/04/02  22:10:19  emartin]
 * 
 * 	CR 7642.  Possible fix (at least get more info).
 * 	prop_driver_sleep - take prop_tasks_lock, compute
 * 	currtime+sleep_secs, do timed_wait, release prop_tasks_lock.
 * 	[1993/04/02  22:04:05  emartin]
 * 
 * Revision 1.1.3.9  1993/03/25  16:18:34  emartin
 * 	CR 7521. prop_analyse_status - call rs_prop_tasks_refrsh_slv_creds
 * 	for most errors (so that auth handle will be refreshed).
 * 	[1993/03/24  20:39:55  emartin]
 * 
 * 	CR 7521.  get_rep_binding - when auth_handle != NULL
 * 	destroy the auth handle and re-create it.
 * 	rs_prop_tasks_refrsh_slv_creds now public
 * 	[1993/03/23  23:07:16  emartin]
 * 
 * Revision 1.1.3.8  1993/02/26  23:53:25  emartin
 * 	em_bl6f: remove some prints
 * 	[1993/02/22  23:01:27  emartin]
 * 
 * 	em_bl6f: prop_analyse_status - when get
 * 	sec_rgy_rep_master_bad_sw_vers status call rs_rep_mgmt_stop_til_sw_ok_task
 * 	[1993/02/19  15:52:14  emartin]
 * 
 * 	em_bl6f: rs_c_state_compat_software_version =>
 * 	rs_c_state_compat_software_vers
 * 	put prop_sleep times into vars
 * 	[1993/02/11  20:17:35  emartin]
 * 
 * Revision 1.1.3.7  1993/02/03  22:08:29  ahop
 * 	bl6e: prop_delete - don't free master_replist ptr (prp),
 * 	      just mark the replica as purged.
 * 	[1993/01/29  02:47:31  ahop]
 * 
 * Revision 1.1.3.6  1993/01/26  23:09:58  emartin
 * 	rs_bypass_auth
 * 	[1993/01/13  16:30:09  emartin]
 * 
 * Revision 1.1.3.5  1992/12/29  16:36:30  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:25:35  zeliff]
 * 
 * Revision 1.1.3.4  1992/12/18  22:12:34  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:53:08  sekhar]
 * 
 * Revision 1.1.3.3  1992/11/04  19:38:03  ahop
 * 	Third replication drop: bl6a
 * 	STUB for rs_prop_tasks_start
 * 	[1992/11/03  18:42:36  ahop]
 * 
 * Revision 1.1.3.2  1992/10/07  20:36:01  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:18:31  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  17:23:52  burati
 * 	Second replication drop: bl5
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*    
 *      Registry Server - Propagation task management
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_prop_tasks.c,v 1.1.18.2 1996/10/03 15:10:04 arvind Exp $";
#endif


#include <un_time.h>

#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>

#include <rs_base.h>
#include <rs_ver.h>
#include <rs_pvt.h>
#include <rs_state.h>
#include <rs_master.h>
#include <rs_rep_pvt.h>
#include <rs_replist2.h>
#include <rs_master_replist_pvt.h>
#include <rs_master_replist.h>
#include <rs_util.h>
#include <rs_rep_util.h>
#include <rs_prop_tasks.h>
#include <rs_task.h>
#include <rs_lock.h>
#include <rs_log.h>
#include <rs_log_replist.h>
#include <rs_rep_bind.h>
#include <rs_rep_auth.h>

#include <rrs_rep_remote.h>
#include <rrs_repmgr.h>
#include <rrs_prop_replist.h>

#include <rsdb_replica.h>

#include <krb5/krb5.h>

#include <sec_svc.h>

/*
 * Local macros
 */
#define UNLOCK_FOR_PROP(m_prp, m_lock_taken)        \
    {                                               \
        m_prp->prop_in_progress = true;             \
        unlock(lock_replist, lock_mode_write);      \
        *m_lock_taken = false;                      \
    }

#define LOCK_FOR_PROP(m_prp, m_lock_taken, m_prop_type, m_st) \
    {                                                       \
        rs_replica_prop_t       m_tmp_prop_type;            \
                                                            \
        m_tmp_prop_type = m_prop_type;                      \
        lock(lock_replist, lock_mode_write);                \
        *m_lock_taken = true;                               \
        m_prp->prop_in_progress = false;                    \
        prop_chk_state_and_prop_type(m_prp, *(m_lock_taken),\
            &m_tmp_prop_type, m_st);                        \
    }

#define LOCK_FOR_ANY_PROP(m_prp, m_lock_taken, m_st)        \
    {                                                       \
        lock(lock_replist, lock_mode_write);                \
        *m_lock_taken = true;                               \
        m_prp->prop_in_progress = false;                    \
        prop_chk_state_and_prop_type(m_prp, *(m_lock_taken),\
            NULL, m_st);                                    \
    }


/* 
 * Local constants, typedefs, and variables
 */
PRIVATE unsigned32 rs_c_prop_pause_secs =       10;
PRIVATE unsigned32 rs_c_prop_short_sleep_secs = (3*60);     /* 3 minutes */
PRIVATE unsigned32 rs_c_prop_long_sleep_secs =  (15*60);    /* 15 minutes */

/* 
 * number of consecutive prop failures
 * before move a replica from comm_state
 * short_failure to long_failure
 */
PRIVATE unsigned32 rs_c_prop_many_failures =    4;

typedef enum {
    rs_e_prop_communicado,
    rs_e_prop_short_incommunicado,
    rs_e_prop_long_incommunicado
} rs_prop_job_type_t;

PRIVATE	rs_prop_job_type_t  prop_communicado_type = rs_e_prop_communicado;
PRIVATE rs_prop_job_type_t  prop_short_incommunicado_type = 
                                rs_e_prop_short_incommunicado;
PRIVATE rs_prop_job_type_t  prop_long_incommunicado_type = 
                                rs_e_prop_long_incommunicado;

PRIVATE boolean32   rs_prop_tasks_inited = false;

/*
 * The registry server locks indirectly
 * use pthread_mutex locks.  When the
 * tasks do pthread_cond_timedwait, they must
 * be holding a mutex lock; this lock is released 
 * on entering pthread_cond_timedwait and reaquired 
 * on coming out of the timedwait.  Ergo, we
 * have this bogus rs_prop_tasks_lock which is
 * taken just before the timedwait and released
 * on wakeup.
 */
PRIVATE pthread_mutex_t rs_prop_tasks_lock;

PRIVATE pthread_cond_t  prop_communicado_cv;
PRIVATE pthread_cond_t  prop_short_incommunicado_cv;
PRIVATE pthread_cond_t  prop_long_incommunicado_cv;

PRIVATE boolean32       prop_communicado_task2_created = false;

PRIVATE rs_task_handle_t    prop_communicado_task1_h;
PRIVATE rs_task_handle_t    prop_communicado_task2_h;
PRIVATE rs_task_handle_t    prop_short_incommunicado_task_h;
PRIVATE rs_task_handle_t    prop_long_incommunicado_task_h;

/* 
 * Local routines
 */

PRIVATE void prop_task_wakeup_short_fail (
    void
);

PRIVATE void *prop_driver (
    void    *prop_job_type
);

PRIVATE void prop_driver_sleep (
    rs_prop_job_type_t      my_prop_job_type,
    boolean32               my_prop_job_found,
    unsigned32              begin_sw_rev,
    rs_update_seqno_t       *begin_update_seqno,
    unsigned32              *n_bad_prop_state,
    boolean32               *replist_lock_taken,
    boolean32               *prop_tasks_lock_taken
);

PRIVATE boolean32 prop_state_exiting (
    void
);

PRIVATE boolean32 my_prop_job (
    rs_prop_job_type_t          my_prop_job_type,
    rs_rep_prop_mgt_item_p_t    prp
);

PRIVATE void prop_get_rep_binding (
    rs_rep_prop_mgt_item_p_t    prp,
    boolean32                   *replist_lock_taken,
    error_status_t              *st
);

PRIVATE void prop_init (
    rs_rep_prop_mgt_item_p_t    prp,
    rs_replica_master_info_t    *master_info,
    boolean32                   *replist_lock_taken,
    error_status_t              *st
);

PRIVATE void chk_prop_init (
    rs_rep_prop_mgt_item_p_t    prp,
    boolean32                   *replist_lock_taken,
    error_status_t              *st
);

PRIVATE void init_done (
    uuid_p_t            rep_id,
    uuid_p_t            init_id,
    uuid_p_t            init_from_rep_id,
    rs_update_seqno_t   *last_upd_seqno,
    sec_timeval_t       *last_upd_ts,
    error_status_t      *st
);

PRIVATE void prop_updates (
    rs_rep_prop_mgt_item_p_t    prp,
    rs_replica_master_info_t    *master_info,
    boolean32                   *replist_lock_taken,
    error_status_t              *st
);

PRIVATE void prop_this_update (
    rs_rep_prop_mgt_item_p_t    prp,
    unsigned_char_p_t           bp,
    rs_replica_master_info_t    *master_info,
    boolean32                   *replist_lock_taken,
    error_status_t              *st
);

PRIVATE void prop_delete (
    rs_rep_prop_mgt_item_p_t    prp,
    rs_replica_master_info_t    *master_info,
    boolean32                   *replist_lock_taken,
    error_status_t              *st
);

PRIVATE void prop_chk_state_and_prop_type (
    rs_rep_prop_mgt_item_p_t    prp,
    boolean32                   lock_taken,
    rs_replica_prop_t           *prop_type,
    error_status_t              *st
);

PRIVATE void prop_analyse_status (
    rs_rep_prop_mgt_item_p_t    prp,
    rs_replica_master_info_t    *master_info,
    error_status_t              *st
);

PRIVATE void refresh_slave_creds (
    rs_rep_prop_mgt_item_p_t    prp
);

PRIVATE void prop_analyze_slave_seqno (
    rs_rep_prop_mgt_item_p_t    prp,
    rs_replica_info_t           *rep_info,
    error_status_t              *st
);

PRIVATE void refresh_slave_seqno (
    rs_rep_prop_mgt_item_p_t    prp
);

PRIVATE void prop_not_master (
    rs_replica_master_info_t    *master_info
);

PRIVATE void prop_mark_slv_for_init (
    rs_rep_prop_mgt_item_p_t    prp
);

PRIVATE void prop_status_good (
    rs_rep_prop_mgt_item_p_t    prp,
    error_status_t              *st
);

PRIVATE void prop_status_maybe_bad (
    rs_rep_prop_mgt_item_p_t    prp,
    error_status_t              *st
);

PRIVATE void prop_status_bad (
    rs_rep_prop_mgt_item_p_t    prp,
    error_status_t              *st
);

PRIVATE void prop_status_very_bad (
    rs_rep_prop_mgt_item_p_t    prp,
    error_status_t              *st
);

PRIVATE void clear_props_in_progress (
    void
);

PRIVATE void prop_slave_bad_sw_rev (
    rs_rep_prop_mgt_item_p_t    prp
);

PRIVATE void prop_slave_sw_rev_ok (
    rs_rep_prop_mgt_item_p_t    prp,             /* [in] */
    unsigned32                  compat_sw_rev,   /* [in] */
    lock_mode_t                 lock_mode,       /* [in] */
    error_status_t              *st              /* [out] */
);

PRIVATE boolean32  need_rep_adm_info (
    rs_rep_prop_mgt_item_p_t    prp
);

PRIVATE void prepare_for_prop (
    rs_rep_prop_mgt_item_p_t    prp,
    boolean32                   *replist_lock_taken,
    rs_replica_master_info_t    *master_info,   
    error_status_t              *st
);

PRIVATE void prop_stop_til_sw_compat (
    rs_rep_prop_mgt_item_p_t    prp,
    boolean32                   *replist_lock_taken,
    rs_replica_master_info_t    *master_info,   
    error_status_t              *st
);

/*
*/
PUBLIC void rs_prop_tasks_start
  (
    error_status_t          *st
  )
{
    unsigned32          nprop_reps;

    CLEAR_STATUS(st);

    if (rs_prop_tasks_inited) return; 

    SET_STATUS(st, sec_s_pgmerr);

    if (pthread_mutex_init(&rs_prop_tasks_lock,
            pthread_mutexattr_default) == -1) {
        return;
    }
                                
    if (pthread_cond_init(&prop_communicado_cv, 
            pthread_condattr_default) == -1) {
        return;
    }

    if (pthread_cond_init(&prop_short_incommunicado_cv, 
            pthread_condattr_default) == -1) {
        return;
    }

    if (pthread_cond_init(&prop_long_incommunicado_cv, 
            pthread_condattr_default) == -1) {
        return;
    } 
     
    if ( ! rs_task_create(&prop_communicado_task1_h,
                          prop_driver,
                          RS_COMMUNICADO_THR,
                          true, 
                          (void *) &prop_communicado_type) ) {
        return;
    }

    rs_m_replist_get_nprop_to_reps(&nprop_reps);
    if (nprop_reps > 2) {
        if ( ! rs_task_create(&prop_communicado_task2_h,
                              prop_driver,
                              RS_COMMUNICADO_THR,
                              true, 
                              (void *) &prop_communicado_type) ) {
            return;
        }
        prop_communicado_task2_created = true;
    }

    if ( ! rs_task_create(&prop_short_incommunicado_task_h,
                          prop_driver,
                          RS_SHORT_INCOMMUNICADO_THR,
                          true, 
                          (void *) &prop_short_incommunicado_type) ) {
        return;
    }

    if ( ! rs_task_create(&prop_long_incommunicado_task_h,
                          prop_driver,
                          RS_LONG_INCOMMUNICADO_THR,
                          true, 
                          (void *) &prop_long_incommunicado_type) ) {
        return;
    }

    rs_prop_tasks_inited = true;

    CLEAR_STATUS(st);
}

/*
*/
PUBLIC void rs_prop_tasks_stop
  (
    error_status_t          *st
  )
{
    pthread_addr_t join_st;

    CLEAR_STATUS(st);

    if (!rs_prop_tasks_inited) return;

    if ((pthread_cancel(prop_communicado_task1_h) == 0) &&
        rs_task_join(prop_communicado_task1_h, &join_st)) {
        rs_task_release(&prop_communicado_task1_h);
    }
    else {
        SET_STATUS(st, sec_s_pgmerr);
    }

    if (prop_communicado_task2_created) {
        if ((pthread_cancel(prop_communicado_task2_h) == 0) &&
            rs_task_join(prop_communicado_task2_h, &join_st)) {
            rs_task_release(&prop_communicado_task2_h);
            prop_communicado_task2_created = false;
        }
        else {
            SET_STATUS(st, sec_s_pgmerr);
        }
    }

    if ((pthread_cancel(prop_short_incommunicado_task_h) == 0) &&
        rs_task_join(prop_short_incommunicado_task_h, &join_st)) {
        rs_task_release(&prop_short_incommunicado_task_h);
    }
    else {
        SET_STATUS(st, sec_s_pgmerr);
    }

    if ((pthread_cancel(prop_long_incommunicado_task_h) == 0) &&
        rs_task_join(prop_long_incommunicado_task_h, &join_st)) {
        rs_task_release(&prop_long_incommunicado_task_h);
    }
    else {
        SET_STATUS(st, sec_s_pgmerr);
    }

    if (pthread_mutex_destroy(&rs_prop_tasks_lock) == -1) { 
        SET_STATUS(st, sec_s_pgmerr);
    }

    if (pthread_cond_destroy(&prop_communicado_cv) == -1) {
        SET_STATUS(st, sec_s_pgmerr);
    }

    if (pthread_cond_destroy(&prop_short_incommunicado_cv) == -1) {
        SET_STATUS(st, sec_s_pgmerr);
    }

    if (pthread_cond_destroy(&prop_long_incommunicado_cv) == -1) {
        SET_STATUS(st, sec_s_pgmerr);
    }

    /*
     * prop tasks may have been aborted when they
     * had outstanding propagations
     */
    clear_props_in_progress();

    rs_prop_tasks_inited = false;
}

/*
*/
PUBLIC void rs_prop_tasks_wakeup (
    void
)
{
    if (!rs_prop_tasks_inited) return;

    pthread_cond_broadcast(&prop_communicado_cv);
}

PRIVATE void prop_task_wakeup_short_fail (
    void
)
{
    pthread_cond_broadcast(&prop_short_incommunicado_cv);
}

PRIVATE void *prop_driver 
  (
    void    *prop_job_type
  )
{
    rs_prop_job_type_t          my_prop_job_type;
    boolean32                   replist_lock_taken;
    boolean32                   rs_prop_tasks_lock_taken;

    rs_replica_master_info_t    master_info;

    unsigned32                  n_bad_prop_state;
    rs_update_seqno_t           begin_update_seqno;
    sec_timeval_t               update_ts;
    boolean32                   my_prop_job_found;
    unsigned32                  master_compat_sw_rev;
                                
    rs_rep_prop_mgt_item_p_t    prp,
                                next_prp;
    error_status_t              lst;

    char                        *thr_name ;

    my_prop_job_type = *((rs_prop_job_type_t *) prop_job_type);

    switch (my_prop_job_type) {
        case rs_e_prop_communicado:
            dce_svc_printf(SEC_RS_THR_STARTED_MSG, RS_COMMUNICADO_THR);
            thr_name = RS_COMMUNICADO_THR ;
            break;

        case rs_e_prop_short_incommunicado:
            dce_svc_printf(SEC_RS_THR_STARTED_MSG, RS_SHORT_INCOMMUNICADO_THR);
            thr_name = RS_SHORT_INCOMMUNICADO_THR ;
            break;

        case rs_e_prop_long_incommunicado: 
            dce_svc_printf(SEC_RS_THR_STARTED_MSG, RS_LONG_INCOMMUNICADO_THR);
            thr_name = RS_LONG_INCOMMUNICADO_THR ;
            break;

        default:
            /*   N.B. Prior to SVC changes, a bad type would not
             *        be caught until much later or maybe after
             *        other errors. I believe it is safe to return
             *        here.
             */
            dce_svc_printf(SEC_PROP_BAD_TYPE_MSG);
            return 0; 
    }

    replist_lock_taken = false; 
    rs_prop_tasks_lock_taken = false;

    /*
     * Let things settle down for a minute
     */
    rs_task_sleep(60);

    if (!rs_prop_tasks_inited) {
        dce_svc_printf(SEC_PROP_NO_PROP_THRS_MSG);
        return 0;
    }

    if (!rs_state_master()) {
        dce_svc_printf(SEC_PROP_NOT_MASTER_MSG);
    }

    rs_prop_tasks_get_master_info(&master_info, &lst);
    if (BAD_STATUS(&lst)) {
        dce_svc_printf(SEC_PROP_NO_MASTER_INFO_MSG);
        return 0;
    }

    master_compat_sw_rev = master_info.master_compat_sw_rev;

    TRY {

        lock(lock_replist, lock_mode_write);
        replist_lock_taken = true;

        /* short or no sleep for first time around
         */
        rs_util_seqno_create(&begin_update_seqno);
        my_prop_job_found = true;   
        n_bad_prop_state = 0;

        while (!prop_state_exiting()) { 

            prop_driver_sleep(my_prop_job_type, my_prop_job_found, 
                master_compat_sw_rev, &begin_update_seqno, 
	        &n_bad_prop_state, &replist_lock_taken, &rs_prop_tasks_lock_taken);

            /* get seqno of last update on propq now
             * so will know whether more updates are
             * added to the propq during this loop
             * through the replist.
             */
            rs_log_get_update_info(&begin_update_seqno, &update_ts, &lst);
          
	    /*
	     * Get the current master_compat_sw_rev
	     * so we'll know if it changes during this loop
	     */
	    rs_master_info_get_sw_rev(&master_compat_sw_rev, &lst);

            my_prop_job_found = false;

            for (prp = (rs_rep_prop_mgt_item_p_t) rs_master_replistq.fwd;
                 prp != NULL;
                 prp = next_prp) {

                /* ok state for propagating?
                 */
                rs_state_check(rs_c_event_propagate, 
                        replist_lock_taken, &lst);
                if (BAD_STATUS(&lst)) {
                    n_bad_prop_state++;
                    break;
                }
                else {
                    n_bad_prop_state = 0;
                }
    
                /*
                 * might remove this entry from the list and free its
                 * storage, so get a ptr to the next entry on the list
                 * now
                 */
                next_prp = (rs_rep_prop_mgt_item_p_t) prp->list.fwd;
        
                if (prp->master) continue;

                /*
                 * ignore replicas being sent an update
                 * by another task
                 */
                if (prp->prop_in_progress) continue;
        
                /*
                 * if this replica is marked for purging,
                 * remove it from rs_master_replistq and
                 * free all its memory
                 */
                if (prp->purged) {
                    rs_m_replist_purge_replica((rs_rep_prop_mgt_handle_t) prp,
                        &prp->rep_id, &lst);
                    continue;
                }

                if (! my_prop_job(my_prop_job_type, prp)) continue;

                my_prop_job_found = true;

		prepare_for_prop(prp, &replist_lock_taken, 
				 &master_info, &lst);
                 
                if (GOOD_STATUS(&lst)) {
                    /*
                     * "worker" routines will release replist
                     * lock while propagation is outstanding
                     * but are required to return holding the lock
                     */
                    switch(prp->prop_info.prop_type) {
                        case rs_c_replica_prop_init:
                            prop_init(prp, &master_info, 
                                &replist_lock_taken, &lst);
                            break;
        
                        case rs_c_replica_prop_initing:
                            chk_prop_init(prp, 
                                &replist_lock_taken, &lst);
                            break;
        
                        case rs_c_replica_prop_update:
			    prop_updates(prp, &master_info, 
					 &replist_lock_taken, &lst);
			    break;
        
                        case rs_c_replica_prop_delete:
                            /* if propagates the delete replica
                             * update successfully, will delete
                             * the replica from stable storage and
                             * mark it for purge on the master
                             * replist.
                             */
                            prop_delete(prp, &master_info, 
                                &replist_lock_taken, &lst);
                            break;

                        case rs_c_replica_prop_bad_sw_rev:
			    /* 
			     * A replica in this state will get
			     * pinged as part of the prepare_for_prop()
			     * logic invoked above.  Since the same logic
			     * puts the replica in this state and 
			     * sets the comm state to "long_failure"
			     * this pinging occurs only in the
			     * long failure thread.
			     */
			    break;

                        default:
                            dce_svc_printf(SEC_PROP_BAD_TYPE_MSG);
                            break;
                    }
                }

                /*
                 * Save latest status.  If there was an error,
                 * try to recover from it. Assign replica to
                 * appropriate prop task.
                 */
                prop_analyse_status(prp, &master_info, &lst);

            } /* loop thru rs_master_replistq */

            /*
             * Have the long_incommunicado thread attempt to clean up
             * completed updates in the prop queue.
             */
            if (my_prop_job_type == rs_e_prop_long_incommunicado) {
                rs_log_propq_clean(&lst);
                if (BAD_STATUS(&lst)) {
                    dce_svc_printf(SEC_PROP_PROPQ_CLEAN_FAIL_MSG, lst);
                }
            }

        }   /* while true */

        if (replist_lock_taken) {
            unlock(lock_replist, lock_mode_write);
            replist_lock_taken = false;
        }

    } CATCH (pthread_cancel_e) {
        if (replist_lock_taken)
            unlock(lock_replist, lock_mode_write);
        if (rs_prop_tasks_lock_taken)
            pthread_mutex_unlock(&rs_prop_tasks_lock);
    
    } CATCH_ALL {
        if (replist_lock_taken)
            unlock(lock_replist, lock_mode_write);
        if (rs_prop_tasks_lock_taken)
            pthread_mutex_unlock(&rs_prop_tasks_lock);
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_prop,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_thr_exit_exc);
    }

    ENDTRY;
    dce_svc_printf(SEC_RS_THR_EXITING_MSG, thr_name);
    return 0;
}

PRIVATE void prop_driver_sleep
  (
    rs_prop_job_type_t      my_prop_job_type,
    boolean32               my_prop_job_found,
    unsigned32              begin_sw_rev,
    rs_update_seqno_t       *begin_update_seqno,
    unsigned32              *n_bad_prop_state,
    boolean32               *replist_lock_taken,
    boolean32               *prop_tasks_lock_taken
  )
{
    unsigned32          sleep_secs;
    rs_update_seqno_t   update_seqno;
    sec_timeval_t       update_ts;
    struct timeval      time_tv;
    struct timespec     time_ts;
    pthread_cond_t      *sleep_cv;
    char                message[128];
    unsigned32          compat_sw_rev;
    error_status_t      lst;

    /* Fill sleep_cv
     */
    switch (my_prop_job_type) {
        case rs_e_prop_communicado:
            sleep_cv = &prop_communicado_cv;
            break;

        case rs_e_prop_short_incommunicado:
            sleep_cv = &prop_short_incommunicado_cv;
            break;

        case rs_e_prop_long_incommunicado:
            sleep_cv = &prop_long_incommunicado_cv;
            break;

        default:
            sleep_cv = &prop_long_incommunicado_cv;
            break;
    }

    /* Fill sleep_secs
     */
    if (*n_bad_prop_state > 0) {
        if (*n_bad_prop_state < 4) {
            sleep_secs = (*n_bad_prop_state) * 60 * 2;
        }
        else {
            sleep_secs = rs_c_prop_long_sleep_secs;
            *n_bad_prop_state = 10;
        }
    }

    else {

        /* default is long sleep
         */
        sleep_secs = rs_c_prop_long_sleep_secs;

        switch (my_prop_job_type) {

            /* If any work was done, only pause
             * for a few seconds because changes 
             * could have happened while the replist 
             * lock was released (eg. replicas could 
             * have been moved to my job_type).
             * If there are new updates on the propq
             * since the beginning of this loop thru
             * the replist, just pause.
             * Otherwise, long sleep waiting for a 
             * broadcast when a new update goes onto 
             * the propq or when a replica becomes 
             * communicado.
             */
            case rs_e_prop_communicado:
                if (my_prop_job_found) {
                    sleep_secs = rs_c_prop_pause_secs;
                }
                else {
		    /* If the master compat software rev
		     * has changed during the last iteration
		     * through the replist, pause briefly
		     * and go through again.  Otherwise, some
		     * replicas might not hear about the new
		     * compat_sw_rev until another
		     * update arrives.
		     */
		    rs_master_info_get_sw_rev(&compat_sw_rev, &lst);
		    if (compat_sw_rev > begin_sw_rev) {
			sleep_secs = rs_c_prop_pause_secs;
		    }
		    else {
			rs_log_get_update_info(
		            &update_seqno, &update_ts, &lst);
			if (rs_util_seqno_cmp(begin_update_seqno, &update_seqno) != 0) {
			    sleep_secs = rs_c_prop_pause_secs;
			}
		    }
		}

                break;
    
            /*
             * If there are jobs for this prop_task,
             * sleep for a short time
             */
            case rs_e_prop_short_incommunicado:
                if (my_prop_job_found) {
                    sleep_secs = rs_c_prop_short_sleep_secs;
                }

                break;
    
            /*
             * long sleep - set above
             */
            case rs_e_prop_long_incommunicado:
                break;

            default:
                dce_svc_printf(SEC_PROP_BAD_TYPE_MSG);
                break;
        }
    }

    if (sleep_secs > 0) {

        /* release replist lock,
         * take bogus lock for pthread_cond_timedwait,
         * wait for prop event or timeout 
         */
        unlock(lock_replist, lock_mode_write);
        *replist_lock_taken = false;
        pthread_mutex_lock(&rs_prop_tasks_lock);
        *prop_tasks_lock_taken = true;
    
        rs_util_gettimeofday(&time_tv);
        time_ts.tv_sec = time_tv.tv_sec + sleep_secs;
        time_ts.tv_nsec = 0;

        if (pthread_cond_timedwait(sleep_cv, 
                &rs_prop_tasks_lock, &time_ts) == -1) {
            if (errno != EAGAIN) {
                dce_svc_printf(SEC_SYS_ERRNO_TEXT_MSG,
                               "pthread_cond_timedwait",
                               strerror(errno));
            }
        }

        /* now release prop tasks lock and
         * take back replist lock
         */
        pthread_mutex_unlock(&rs_prop_tasks_lock);
        *prop_tasks_lock_taken = false;
        lock(lock_replist, lock_mode_write);
        *replist_lock_taken = true;
    }
}

PRIVATE boolean32 prop_state_exiting (
    void
)
{
    pthread_testcancel();

    if (!rs_state_master() ||
        rs_state_exiting())
        return true;
    else
        return false;
}

/*
 * Return true if this is a job for my
 * type of prop task.  Otherwise return
 * false.
 */
PRIVATE boolean32 my_prop_job
  (
    rs_prop_job_type_t          my_prop_job_type,
    rs_rep_prop_mgt_item_p_t    prp
  )
{
    boolean32       retval;
    unsigned32      compat_sw_rev;
    error_status_t  lst;

    switch (my_prop_job_type) {

        case rs_e_prop_communicado:
            if (prp->comm_info.comm_state == rs_c_replica_comm_ok) {
                switch (prp->prop_info.prop_type) {

		case rs_c_replica_prop_bad_sw_rev:
                    /*FALLTHROUGH*/
		case rs_c_replica_prop_init:
		    /*FALLTHROUGH*/
		case rs_c_replica_prop_delete:
		    retval = true;
		    break;

                    /*
                     * if nothing to propagate this isn't my job
                     */
		case rs_c_replica_prop_update:
		    /* 
		     * If we haven't sorted out the compat_sw_rev
		     * situation, then we need to do so, even if
		     * there are no outstanding GLOBAL updates
		     * to propagate
		     */
		    if (!PROP_MGT_SW_REV_INITED(prp)) {
			retval = true;
		    } else {
			rs_master_info_get_sw_rev(&compat_sw_rev, &lst);
		        if (STATUS_OK(&lst)
		        && compat_sw_rev > prp->compat_sw_rev) {
			    retval = true;
			} else {
			    retval = !(rs_log_nothing_to_propagate(&prp->prop_info));
			}
		    }
		    break;
		    
		default:
		    retval = false;
		    break;
		}
            }
            else {
                retval = false;
            }
            break;

        case rs_e_prop_short_incommunicado:
            if (prp->prop_info.prop_type == rs_c_replica_prop_initing) {
                retval = false;
            }
            else 
            if (prp->comm_info.comm_state == rs_c_replica_comm_short_failure) {
                retval = true;
            }
            else {
                retval = false;
            }
            break;

        case rs_e_prop_long_incommunicado:
            if (prp->prop_info.prop_type == rs_c_replica_prop_initing) {
                retval = true;
            } 
            else 
            if (prp->comm_info.comm_state == rs_c_replica_comm_long_failure) {
                retval = true;
            }
            else {
                retval = false;
            }
            break;

        default:
            dce_svc_printf(SEC_PROP_BAD_TYPE_MSG);
            retval = false;
            break;
    }

    return retval;
}

PRIVATE void prop_get_rep_binding
  (
    rs_rep_prop_mgt_item_p_t    prp,
    boolean32                   *replist_lock_taken,
    error_status_t              *st
  )
{
    rs_replica_info_t     rep_info;
    rs_replica_auth_p_t   rep_auth_info;

    CLEAR_STATUS(st);

    DCE_SVC_DEBUG((sec_svc_handle, sec_s_rs_prop, svc_c_debug1,
                   "prop_driver: attempt get rep binding\n"));

    /*
     * get new rpc binding handle
     */
    rs_rep_bind(&prp->rep_binding, st);
    if (BAD_STATUS(st)) return;

#ifdef RS_REP_NAUTH
    if (rs_bypass_auth) return;
#endif

    /*
     * mark handle not valid until get 
     * authenticated handle
     */
    prp->rep_binding.valid_handle = false;

    if (!prp->rep_binding.valid_auth_handle) {

        DCE_SVC_DEBUG((sec_svc_handle, sec_s_rs_prop, svc_c_debug1,
                       "prop_driver: attempt get rep binding auth info\n"));

        UNLOCK_FOR_PROP(prp, replist_lock_taken)
            rrs_rep_mgr_get_info_and_creds(prp->rep_binding.rep_handle, 
                &rep_info, &rep_auth_info, st);
        LOCK_FOR_ANY_PROP(prp, replist_lock_taken, st)

        if (BAD_STATUS(st)) return;

        READ_LOCK(lock_db) {

            if (prp->rep_binding.auth_handle != NULL) {
                rs_rep_auth_dispose_handle(&prp->rep_binding.auth_handle);
            }

            rs_rep_auth_get_handle(rep_auth_info, 
                &prp->rep_binding.auth_handle, st);

        } END_READ_LOCK;

        rpc_ss_client_free(rep_auth_info);

        if (BAD_STATUS(st)) return;

        prp->rep_binding.valid_auth_handle = true;

	DCE_SVC_DEBUG((sec_svc_handle, sec_s_rs_prop, svc_c_debug1,
                   "prop_driver: rep binding, get auth handle succeeded\n"));
    }

    /*
     * have valid auth handle.  now
     * associate auth info with rpc 
     * handle.
     */
    rs_rep_auth_binding_set_auth(
         prp->rep_binding.auth_handle,
         prp->rep_binding.rep_handle, st);
        
    prp->rep_binding.valid_handle = GOOD_STATUS(st);

    if (BAD_STATUS(st)) return;

    DCE_SVC_DEBUG((sec_svc_handle, sec_s_rs_prop, svc_c_debug1,
                       "prop_driver: rep binding succeeded\n"));
}

PRIVATE void prop_init
  (
    rs_rep_prop_mgt_item_p_t    prp,
    rs_replica_master_info_t    *master_info,
    boolean32                   *replist_lock_taken,
    error_status_t              *st
  )
{
    unsigned32              nreps;
    rs_replica_mgt_item_p_t *init_from_reps;
    uuid_p_t                *init_from_rep_ids;
    rs_replica_twr_vec_p_t  *init_from_rep_twrs;
    uuid_t                  init_id;
    uuid_t                  init_from_rep_id;
    rs_update_seqno_t       last_upd_seqno;
    sec_timeval_t           last_upd_ts;

    dce_svc_printf(SEC_PROP_SEND_INIT_SLAVE_MSG);

    init_id = prp->init_id;

    rs_m_replist_get_init_frm_reps(&prp->init_from_rep_id, &nreps,
        &init_from_reps, &init_from_rep_ids, &init_from_rep_twrs, st);
    if (BAD_STATUS(st)) return;

    UNLOCK_FOR_PROP(prp, replist_lock_taken)

        rrs_rep_mgr_init(prp->rep_binding.rep_handle, &init_id,
            nreps, init_from_rep_ids, init_from_rep_twrs, master_info, 
            &init_from_rep_id, &last_upd_seqno, &last_upd_ts, st);
    
    LOCK_FOR_PROP(prp, replist_lock_taken, rs_c_replica_prop_init, st)

    rs_m_replist_free_init_frm_reps(nreps, &init_from_reps,
        &init_from_rep_ids, &init_from_rep_twrs);

    if (STATUS_EQUAL(st, sec_rgy_rep_already_inited)) {
        init_done(&prp->rep_id, 
            &init_id, NULL,
            &last_upd_seqno, &last_upd_ts, st);
    }
    else
    if (GOOD_STATUS(st)) {
        prp->prop_info.prop_type = rs_c_replica_prop_initing;
        prp->prop_info.last_upd_seqno = last_upd_seqno;
        prp->prop_info.last_upd_ts = last_upd_ts;

        prp->init_from_rep_id = init_from_rep_id;
    } 

    /* Not required. Status logged by analyse_prop_status().
     * if (rs_verbose)
     *     printf("%s prop_driver: init slave status %lx\n", 
     *            rs_prod_name, *st);
     */
}

/*
 * check how an initialization is going
 */
PRIVATE void chk_prop_init
  (
    rs_rep_prop_mgt_item_p_t    prp,
    boolean32                   *replist_lock_taken,
    error_status_t              *st
  )
{
    rs_replica_info_t   rep_info;
    error_status_t      lst;

    dce_svc_printf(SEC_PROP_CHK_PROP_SLAVE_INIT_MSG);
                   
    UNLOCK_FOR_PROP(prp, replist_lock_taken)

        rrs_rep_adm_info(prp->rep_binding.rep_handle, &rep_info, st);

    LOCK_FOR_PROP(prp, replist_lock_taken, rs_c_replica_prop_initing, st)

    if (BAD_STATUS(st)) return;

    switch(rep_info.rep_state) {
        case rs_c_state_uninitialized:
            /* init failed or slave died during init
             */
            prp->prop_info.prop_type = rs_c_replica_prop_init;
            rs_prop_tasks_wakeup();
            break;

        case rs_c_state_initializing:
            break;

        case rs_c_state_in_service:
            if (uuid_equal(&rep_info.init_id, &prp->init_id, &lst)) {
                /* guess we didn't receive the init_done
                 */
                init_done(&prp->rep_id, 
                    &prp->init_id, &prp->init_from_rep_id,
                    &rep_info.last_upd_seqno, &rep_info.last_upd_ts, st);
            }
            else {
                /* weirdo - maybe slave restored from backup
                 */
                prp->prop_info.prop_type = rs_c_replica_prop_init;
                rs_prop_tasks_wakeup();
            }
            break;

        default:
            break;
    }
}                            

PRIVATE void init_done
  (
    uuid_p_t            rep_id,
    uuid_p_t            init_id,
    uuid_p_t            init_from_rep_id,
    rs_update_seqno_t   *last_upd_seqno,
    sec_timeval_t       *last_upd_ts,
    error_status_t      *st
  )
{
    error_status_t      status_ok;

    dce_svc_printf(SEC_PROP_SLAVE_INIT_DONE_MSG);

    CLEAR_STATUS(&status_ok);
    rs_replist2_master_init_rep_don(rep_id, 
        init_id, init_from_rep_id,
        last_upd_seqno, last_upd_ts, &status_ok, st);
    if (GOOD_STATUS(st)) {
        rs_log_replist_init_rep_done(
            rep_id, init_id);
    }
}
                            
PRIVATE void prop_analyze_slave_seqno
  (
    rs_rep_prop_mgt_item_p_t    prp,
    rs_replica_info_t           *rep_info,
    error_status_t              *st
  )
{
    boolean32                   init_rep;
    uuid_t                      init_id;

    /*
     * Check that slave's seqno is within 
     * master's propq.
     */

    init_rep = false;

    rs_log_propq_check_update_seqno(rep_info, st);
    if (STATUS_EQUAL(st, sec_rgy_rep_update_seqno_high)) {
        /* slave's latest update seqno is less than
         * any updates on propq, so mark the slave for
         * initialization in the stable database and the
         * master_replist.
         */
        init_rep = true;
    }
    else
    if (STATUS_EQUAL(st, sec_rgy_rep_update_seqno_low)) {
        /*
         * slave has updates with seqnos that are higher 
         * than this master's.
         * If slave will adopt this master, a failure
         * during change master probably occurred so 
         * mark slave for re-init.
         * Otherwise assume that this master has an obsolete
         * database and crash.
         */
        if (!rs_master_info_current_master(
            &rep_info->master_id, NULL)) {
            init_rep = true;
        } 
        else
            SEC_SVC_PRINT_MSG_ST(SEC_RGY_REP_MASTER_OBSOLETE_MSG, *st);
    }
    else
    if (GOOD_STATUS(st)) {
        prp->prop_info.last_upd_inited = true;
        prp->prop_info.last_upd_seqno = rep_info->last_upd_seqno;
        prp->prop_info.last_upd_ts = rep_info->last_upd_ts;
    }

    if (init_rep) {
        uuid_create(&init_id, st);
        if (GOOD_STATUS(st)) {
            rs_replist2_master_init_rep(
                &prp->rep_id, &init_id, st);
        }
        if (GOOD_STATUS(st)) {
            rs_log_replist_init_replica(&prp->rep_id, &init_id);
        }
    }
}

PRIVATE void prop_updates
  (
    rs_rep_prop_mgt_item_p_t    prp,
    rs_replica_master_info_t    *master_info,
    boolean32                   *replist_lock_taken,
    error_status_t              *st
  )
{
    unsigned_char_p_t   bp;
    unsigned32          blen;

    CLEAR_STATUS(st);

    if (rs_log_nothing_to_propagate(&prp->prop_info)) 
        return;

    dce_svc_printf(SEC_PROP_UPDATES_TO_SLAVES_MSG);

    bp = NULL;
    blen = 0;

    while (true) {

        pthread_testcancel();

        rs_log_propq_get_next(&prp->prop_info.last_upd_seqno, &bp, &blen, st);
        if (BAD_STATUS(st)) {
            if (STATUS_EQUAL(st, sec_rgy_object_not_found)) {
                CLEAR_STATUS(st);
            }
            break;
        }

        prop_this_update(prp, bp, master_info, replist_lock_taken, st);
        if (BAD_STATUS(st)) break;
    } 

    if (bp != NULL)
        free(bp);

    /* Not required. Status logged by analyse_prop_status().
     *
     *  if (rs_verbose)
     *      printf("%s prop_driver: status of propagate update(s) to slave %lx\n", 
     *             rs_prod_name, *st);
     */
}

/*
 * Propagate this update to the replica managed in prp.
 * If the update is successfully sent, bump the replica's
 * prop_info last update seqno and timestamp.
 */
PRIVATE void prop_this_update
  (
    rs_rep_prop_mgt_item_p_t    prp,
    unsigned_char_p_t           bp,
    rs_replica_master_info_t    *master_info,
    boolean32                   *replist_lock_taken,
    error_status_t              *st
  )
{
    rs_log_hdr_t    *lep;

    master_info->previous_update_seqno = prp->prop_info.last_upd_seqno;
    rs_master_info_get_sw_rev(&master_info->master_compat_sw_rev, st);
    if (BAD_STATUS(st)) 
	return;

    UNLOCK_FOR_PROP(prp, replist_lock_taken)

        rs_log_propagate(prp->rep_binding.rep_handle, bp, master_info,
            false, st);

    LOCK_FOR_PROP(prp, replist_lock_taken, rs_c_replica_prop_update, st)

    if (GOOD_STATUS(st)) {
        lep = (rs_log_hdr_t *) bp;
        prp->prop_info.last_upd_seqno = lep->upd_seqno;
        prp->prop_info.last_upd_ts = lep->upd_ts;
    }
}

/*
 * Propagate this update to new master
 * If the update is successfully sent, bump the replica's
 * prop_info last update seqno and timestamp.
 */
PRIVATE void prop_this_update_to_new_master
  (
    rs_rep_binding_t            *rep_binding,
    rs_replica_prop_info_t      *prop_info,
    unsigned_char_p_t           bp,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st
  )
{
    rs_log_hdr_t    *lep;

    master_info->previous_update_seqno = prop_info->last_upd_seqno;
    rs_log_propagate(rep_binding->rep_handle, bp, master_info,
            propq_only, st);

    if (GOOD_STATUS(st)) {
        lep = (rs_log_hdr_t *) bp;
        prop_info->last_upd_seqno = lep->upd_seqno;
        prop_info->last_upd_ts = lep->upd_ts;
    }
}

/* 
 * Propagate outstanding updates to the new master.
 * If the update is successfully sent, update new master's
 * prop_info last update seqno and timestamp.
 * Invoked with GLOBAL_RIW_LOCK .
 */
PUBLIC void rs_prop_updates_to_new_master
  (
    uuid_p_t                    new_master_id,  /* [in] */
    rs_rep_binding_t            *rep_binding,
    error_status_t              *st
  )
{
    rs_log_hdr_t                *lep;
    rs_rep_prop_mgt_item_p_t    prp,
                                next_prp;
    rs_replica_prop_info_t      prop_info;
    rs_replica_master_info_t    master_info;
    unsigned_char_p_t   	bp=NULL;
    unsigned32          	blen=0;
    rs_replica_info_t           rep_info;

    CLEAR_STATUS(st);

    /* 
     * get new master's entry on current master's volatile replist
     */
    for (prp = (rs_rep_prop_mgt_item_p_t) rs_master_replistq.fwd;
	 prp != NULL;
	 prp = next_prp) {
        next_prp = (rs_rep_prop_mgt_item_p_t) prp->list.fwd;
	if ( uuid_equal(&prp->rep_id, new_master_id, st) )
	    break;
    }

    if ( prp == NULL ) {
        /* 
         * entry for new master not found on volatile replist
         */
        SET_STATUS(st, sec_rgy_rep_invalid_entry);
	return;
    }

    if (prp->prop_info.prop_type != rs_c_replica_prop_update) {
        SET_STATUS(st, sec_rgy_rep_bad_prop_type);
        return;
    }

    /*
     * keep prop_info on local stack so don't need
     * to keep taking a write lock
     */

    prop_info = prp->prop_info;

    if (!prp->prop_info.last_upd_inited) {
        /*
         * get new master's latest update seqno
         */
        rrs_rep_adm_info(rep_binding->rep_handle, &rep_info, st);
        if (BAD_STATUS(st)) return;

        rs_log_propq_check_update_seqno(&rep_info, st);
        if (BAD_STATUS(st)) return;

        prop_info.last_upd_inited = true;
        prop_info.last_upd_seqno = rep_info.last_upd_seqno;
        prop_info.last_upd_ts = rep_info.last_upd_ts;
    }

    if (rs_log_nothing_to_propagate(&prop_info)) 
       return;

    rs_prop_tasks_get_master_info(&master_info, st);
    if (BAD_STATUS(st)) {
        dce_svc_printf(SEC_PROP_NO_MASTER_INFO_MSG);
        return;
    }

    /*
     * won't update this info as propagate updates.
     */
    GLOBAL_RIW_PROMOTE;
    prp->prop_info.last_upd_inited = false;
    GLOBAL_RIW_DEMOTE;

    while (true) {
         rs_log_propq_get_next(&prop_info.last_upd_seqno, 
             &bp, &blen, st);
         if (BAD_STATUS(st)) {
             if (STATUS_EQUAL(st, sec_rgy_object_not_found)) {
                 CLEAR_STATUS(st);
             }
             break;
         }

         prop_this_update_to_new_master(rep_binding, &prop_info, bp, 
					&master_info, false, st);
         if (BAD_STATUS(st)) break;
    } 

    if (bp != NULL)
        free(bp);
}

/*
 * Invoked with replist read lock
 */
PUBLIC void rs_prop_copy_propq
  (
    handle_t                    h,
    error_status_t              *st
  )
{
    rpc_binding_handle_t    rpc_handle;
    rs_rep_binding_t        rep_binding;
    rs_replica_master_info_t 
                            master_info;
    rs_update_seqno_t       last_update_seqno;
    unsigned_char_p_t       bp = NULL;
    unsigned32              blen = 0;
    rs_log_hdr_t            *lep;
    error_status_t          lst;
 
    rpc_binding_server_from_client(h, &rpc_handle, st);
    if (BAD_STATUS(st)) return;

    rs_rep_bind_init_from_handle_na(
        rpc_handle, &rep_binding, st);
    if (BAD_STATUS(st)) {
        rpc_binding_free(&rpc_handle, &lst);
        return;
    }

    READ_LOCK(lock_db) {

        rs_rep_bind_auth_to_slave_rfrsh(&rep_binding, st);

    } END_READ_LOCK

    if (BAD_STATUS(st)) {
        rpc_binding_free(&rpc_handle, &lst);
        return;
    }

    rs_prop_tasks_get_master_info(&master_info, st);

    if (GOOD_STATUS(st)) {
        /* put base propq seqno in master_info
         * prev seqno so will get first update
         * on propq
         */
        rs_log_get_propq_info(&master_info.previous_update_seqno, 
            &last_update_seqno, st);
    }

    if (GOOD_STATUS(st)) {

        while(true) {

            rs_log_propq_get_next(&master_info.previous_update_seqno,
                &bp, &blen, st);

            if (BAD_STATUS(st)) {
                if (STATUS_EQUAL(st, sec_rgy_object_not_found)) {
                    CLEAR_STATUS(st);
                }
                break;
            }

            /* send update for propq only
             */
            rs_log_propagate(rep_binding.rep_handle, bp, &master_info,
                true, st);
            if (BAD_STATUS(st)) break;

            lep = (rs_log_hdr_t *) bp;
            master_info.previous_update_seqno = lep->upd_seqno;
        }
    }

    rs_rep_bind_free_binding(&rep_binding);

    if (bp != NULL)
        free(bp);
}

/*
 * Propagate a delete replica command to the
 * replica that is being deleted.  If the 
 * propagation is successful, remove the
 * replica from the stable and volatile
 * replists and log the update for replay.
 */
PRIVATE void prop_delete
  (
    rs_rep_prop_mgt_item_p_t    prp,
    rs_replica_master_info_t    *master_info,
    boolean32                   *replist_lock_taken,
    error_status_t              *st
  )
{
    uuid_t                    rep_id;
    unsigned_char_p_t         rep_str;
    error_status_t            lst;
    rs_replica_master_info_t  del_master_info;

    rep_id = prp->rep_id;

    rep_str = NULL;
    uuid_to_string(&rep_id, &rep_str, &lst);
    dce_svc_printf(SEC_PROP_SEND_DELETE_REP_MSG,rep_str);
    if (rep_str != NULL)
        rpc_string_free(&rep_str, &lst);

    master_info->previous_update_seqno = prp->prop_info.last_upd_seqno;

    /*
     * We send the delete replica propagation using the
     * current software version of the slave, which may not be the 
     * the same as the master if an admin-initiated set-version
     * operation is underway.  We don't want to overwrite the
     * global master info in use by the prop driver, so we copy it
     * and set the compat_sw_rev from the volatile state of
     * the replica.
     */
    del_master_info = *master_info;
    del_master_info.master_compat_sw_rev = prp->compat_sw_rev;

    UNLOCK_FOR_PROP(prp, replist_lock_taken)

        rrs_prop_replist_del_replica(prp->rep_binding.rep_handle,
            &rep_id, &del_master_info, false, st);

    LOCK_FOR_PROP(prp, replist_lock_taken, rs_c_replica_prop_delete, st)

    if (GOOD_STATUS(st)) {

        rsdb_replica_delete(&rep_id, st);
        if (BAD_STATUS(st)) return;
    
        rs_m_replist_mark_rep_purged((rs_rep_prop_mgt_handle_t) prp,
            &rep_id, &lst);
    
        rs_log_replist_delete_rep_done(&rep_id);

    } else if (STATUS_EQUAL(st, sec_rgy_rep_master_bad_sw_vers)) {
	/*
	 * If our master info identified us as an earlier
	 * compat rev than we actually are, it would be a
	 * big mistake to act on this error message, which
	 * will cause us to shut down.  Since the slave 
	 * thinks it is newer than we are, just set things 
	 * up for a renegotiation of the slave sw rev on the
	 * next iteration through the replist.  If the slave
	 * really is newer, we'll take the appropriate steps
	 * then.
	 */
	if ((del_master_info.master_compat_sw_rev < master_info->master_compat_sw_rev)
        && (prp->compat_sw_rev < master_info->master_compat_sw_rev)) {
	    SET_STATUS(st, sec_rgy_rep_slave_bad_sw_vers);
	}
    }
}

PRIVATE void prop_chk_state_and_prop_type
  (
    rs_rep_prop_mgt_item_p_t    prp,
    boolean32                   lock_taken,
    rs_replica_prop_t           *prop_type,
    error_status_t              *st
  )
{
    if (BAD_STATUS(st)) return;

    rs_state_check(rs_c_event_propagate, lock_taken, st);
    if (BAD_STATUS(st)) return;

    if ((prop_type != NULL) &&
        (*prop_type != prp->prop_info.prop_type)) {
        SET_STATUS(st, sec_rgy_rep_bad_prop_type);
    }
}

/*
 */
PRIVATE void prop_analyse_status
  (
    rs_rep_prop_mgt_item_p_t    prp,
    rs_replica_master_info_t    *master_info,
    error_status_t              *st
  )
{
    error_status_t              lst;
    unsigned char               *msgp = NULL;

    if (GOOD_STATUS(st)) {
        dce_svc_printf(SEC_PROP_SUCC_MSG);
	/*
	 * Perhaps paradoxically, a successful propagation
	 * of the stop_until_compat_sw operation results
	 * in an incommunicado replica in with a prop
	 * type of bad_sw_rev.  In that case a 
	 * good status actaully means "very_bad."
	 */
	if (prp->prop_info.prop_type == rs_c_replica_prop_bad_sw_rev) {
	    prop_status_very_bad(prp, st);
	} else {
	    prop_status_good(prp, st);
	}
        return;
    }

    dce_svc_printf(SEC_PROP_FAIL_MSG,
                   msgp = dce_sprintf(*st));
    if (msgp) 
        free(msgp);

    switch(*st) {

        case rpc_s_wrong_boot_time:
        case rpc_s_auth_bad_integrity:
        case rpc_s_auth_tkt_expired:
        case KRB5_NO_TKT_IN_RLM:
            refresh_slave_creds(prp);
            prop_status_maybe_bad(prp, st);
            break;

        case sec_rgy_rep_not_from_master:
            /* Decide if there's enough evidence to 
             * stop being master.  If so, spawn a task
             * to make this master become a slave.  
             * If not, retry update.
             */
            prop_not_master(master_info);
            prop_status_bad(prp, st);
            break;

        case sec_rgy_rep_update_seqno_low:
        case sec_rgy_rep_update_seqno_high:
            /* force retrieval of slave's update
             * seqno so that can resync its last 
             * update seqno with the master's
             */
            refresh_slave_seqno(prp);
            prop_status_good(prp, st);
            break;

        case sec_rgy_rep_slave_bad_sw_vers:
	    /*
	     * Set slave up for a remote set_compat_sw_rev() 
	     * on the next pass through.
	     */
            prop_slave_bad_sw_rev(prp);
	    prop_status_good(prp, st);
	    break;

        case sec_rgy_rep_master_bad_sw_vers:
            /* slave has later sw version that
             * isn't compatible with the master's 
             * sw version.  Don't know what correct
             * sw version is so just save our current
             * one + 1.  
             */
            rs_rep_mgmt_stop_til_sw_ok_task();
            prop_status_very_bad(prp, st);
            break;

        case sec_rgy_rep_must_init_slave:
            prop_mark_slv_for_init(prp);
            prop_status_good(prp, st);
            break;

        default:
            /*
             * It may be overkill to refresh the auth info
             * on all errors but comm failures were sometimes
             * reported when there were auth problems, etc.
             */
            refresh_slave_creds(prp);
            prop_status_bad(prp, st);
            break;
    }
} 

/*
 * Just received a message from a slave that may have
 * been down.  refresh its creds and mark it as
 * communicado
 */
PUBLIC void rs_prop_tasks_slave_alive
  (
     rs_rep_prop_mgt_handle_t  prop_mgt_h
  )
{
    rs_rep_prop_mgt_item_p_t    prp;
    error_status_t              lst;

    prp = (rs_rep_prop_mgt_item_p_t) prop_mgt_h;

    refresh_slave_creds(prp);

    if (prp->prop_info.prop_type == rs_c_replica_prop_initing) {
        prp->prop_info.prop_type = rs_c_replica_prop_init;
        rs_prop_tasks_wakeup();
    }

    rs_prop_tasks_status_comm_good(prop_mgt_h, &lst);
}

/*
 * Refresh a slave's credentials by marking
 * its binding info as not valid. The prop_driver
 * will invoke prop_get_rep_binding to refresh
 * the slave's credentials.
 */
PRIVATE void refresh_slave_creds
  (
    rs_rep_prop_mgt_item_p_t    prp
  )
{
    prp->rep_binding.valid_handle = false;
    prp->rep_binding.valid_auth_handle = false;
}

PRIVATE void refresh_slave_seqno
  (
    rs_rep_prop_mgt_item_p_t    prp
  )
{
    prp->prop_info.last_upd_inited = false;
}

PUBLIC void rs_prop_tasks_get_master_info
  (
    rs_replica_master_info_t    *master_info,
    error_status_t              *st
  )
{
    boolean32   master_known;

    rs_master_info_get(&master_known, 
        &master_info->master_id, &master_info->master_seqno, st);
    if ((!master_known) || BAD_STATUS(st)) {
        return;
    }
    rs_master_info_get_sw_rev(&master_info->master_compat_sw_rev, st);
}

/*
 * A slave reported that this isn't the master.
 * Do some checks to see whether this task just
 * doesn't have up to date master_info and, if
 * so, get the latest master_info.
 * If this task's master_info is correct, trust
 * the slave and spawn a task to make this replica
 * become a slave.  Need to spawn a task so that
 * this task can exit during the master shutdown
 * mgmt.
 */
PRIVATE void prop_not_master
  (
    rs_replica_master_info_t    *master_info
  )
{
    error_status_t      lst;

    if (!rs_state_master()) {
        /* Not master anymore. This task should
         * exit soon.
         */
        return;
    }

    if (rs_master_info_current_master(
            &master_info->master_id, 
            &master_info->master_seqno)) {
        /* our info about master is current but slave says
         * there's another master so stop being master
         */
        rs_master_strt_obs_master_tsk();
    }
    else {
        /* get more recent master_info
         */
        rs_prop_tasks_get_master_info(master_info, &lst);
    }
}

PRIVATE void prop_mark_slv_for_init
  (
    rs_rep_prop_mgt_item_p_t    prp
  )
{
    uuid_t          init_id;
    error_status_t  lst;

    uuid_create(&init_id, &lst);
    if (BAD_STATUS(&lst)) return;

    rs_m_replist_init_replica((rs_rep_prop_mgt_handle_t) prp, 
			      &prp->rep_id, &init_id, &lst);
    if (BAD_STATUS(&lst)) return;

    rsdb_replica_set_prop_info(&prp->rep_id, rs_c_replica_prop_init,
        rs_c_replica_prop_unknown, &init_id, &lst);
    if (BAD_STATUS(&lst)) return;

    rs_log_replist_init_replica(&prp->rep_id, &init_id);
}

/*
 * Got a good status returned from a remote prop call.
 * Mark binding handle as valid.
 * If in a bad communication state, transition to
 * the good comm state.
 * Save away this most recent prop status.
 */
PRIVATE void prop_status_good
  (
    rs_rep_prop_mgt_item_p_t    prp,
    error_status_t              *st
  )
{
    prp->rep_binding.valid_handle = true;

    rs_prop_tasks_status_comm_good(
        (rs_rep_prop_mgt_handle_t) prp, st);
}

/* Mark this replica as being in a good
 * comm state
 */
PUBLIC void rs_prop_tasks_status_comm_good
  (
    rs_rep_prop_mgt_handle_t    prop_mgt_h,
    error_status_t              *st
  )
{
    rs_rep_prop_mgt_item_p_t    prp;

    prp = (rs_rep_prop_mgt_item_p_t) prop_mgt_h;

    /* If were not in good comm state, set comm
     * state to ok, prop retry counter to 0,
     * and wakeup the tasks which handle
     * the communicado replicas
     */
    if (prp->comm_info.comm_state != rs_c_replica_comm_ok) {
        prp->comm_info.comm_state = rs_c_replica_comm_ok;
        prp->comm_info.nprop_tries = 0;
        rs_prop_tasks_wakeup();
    }
    prp->comm_info.last_status = *st;
}

/*
 * Invoked when status may mean that should
 * refresh handle and retry immediately
 * (eg. wrong boot time).
 * If this status is the same as the previous
 * comm status, treat it as a bad status.  Would
 * get into loop if retry soon.
 * If this status is the first occurrence of the
 * status, treat it as a good status (retry soon).
 */
PRIVATE void prop_status_maybe_bad
  (
    rs_rep_prop_mgt_item_p_t    prp,
    error_status_t              *st
  )
{
    if (prp->comm_info.last_status == *st) {
        prop_status_bad(prp, st);
    }
    else {
        /* doesn't change rep_binding.valid_handle flag
         */
        rs_prop_tasks_status_comm_good((rs_rep_prop_mgt_handle_t) prp, st);
    }
}

PRIVATE void prop_status_bad
  (
    rs_rep_prop_mgt_item_p_t    prp,
    error_status_t              *st
  )
{
    prp->rep_binding.valid_handle = false;

    if (prp->comm_info.nprop_tries >= rs_c_prop_many_failures) {
        prp->comm_info.comm_state = rs_c_replica_comm_long_failure;
    }
    else {
        if (prp->comm_info.comm_state != rs_c_replica_comm_short_failure) {
            prop_task_wakeup_short_fail ();
        }
        prp->comm_info.comm_state = rs_c_replica_comm_short_failure;
        prp->comm_info.nprop_tries++;
    }
    prp->comm_info.last_status = *st;
}

/*
 * Make this replica's comm state long_failure
 */
PRIVATE void prop_status_very_bad
  (
    rs_rep_prop_mgt_item_p_t    prp,
    error_status_t              *st
  )
{
    prp->comm_info.nprop_tries = rs_c_prop_many_failures;
    prop_status_bad(prp, st);
}

PRIVATE void clear_props_in_progress (
    void
)
{
    rs_rep_prop_mgt_item_p_t    prp;

    for (prp = (rs_rep_prop_mgt_item_p_t) rs_master_replistq.fwd;
         prp != NULL;
         prp = (rs_rep_prop_mgt_item_p_t) prp->list.fwd) {
        prp->prop_in_progress = false;
    }
}

/*
 * Set the slave up for a re-negotiation of the 
 * software rev on the next pass through. 
 */
PRIVATE void prop_slave_bad_sw_rev
  (
    rs_rep_prop_mgt_item_p_t    prp
  )
{
    prp->compat_sw_rev = PROP_MGT_SW_REV_UNKNOWN;
}


PRIVATE boolean32  need_rep_adm_info
  (
    rs_rep_prop_mgt_item_p_t    prp
  )
{
    boolean32 retval = false;


    switch (prp->prop_info.prop_type) {

    case rs_c_replica_prop_initing:
	/*FALLTHROUGH*/
    default:
	/* 
	 * we've got everything we need
	 */
	break;
	
    case rs_c_replica_prop_update:
	/*
         * If in the update state we may need to
	 * obtain the slave's update sequence number
	 */
	if (!prp->prop_info.last_upd_inited) {
	    retval = true;
	}
	/*FALLTHROUGH*/
    case rs_c_replica_prop_init:
	/*FALLTHROUGH*/
    case rs_c_replica_prop_delete:
	/*FALLTHROUGH*/
    case rs_c_replica_prop_bad_sw_rev:
	if (!PROP_MGT_SW_REV_INITED(prp)) {
	    /* 
	     * can't do much of anything until we know the
	     * compat slave's software revision
	     */
	    retval = true;
	}
	break;
    }
    
    return retval;
}


PRIVATE void prepare_for_prop
  (
    rs_rep_prop_mgt_item_p_t    prp,
    boolean32                   *replist_lock_taken,
    rs_replica_master_info_t    *master_info,   
    error_status_t              *st
  )
{
    rs_replica_info_t     rep_info;
    boolean32             got_rep_info = false;

    CLEAR_STATUS(st);
    
    if (!prp->rep_binding.valid_handle) {
	prop_get_rep_binding(prp, replist_lock_taken, st);
    }
    
    if (STATUS_OK(st) && need_rep_adm_info(prp)) {
	UNLOCK_FOR_PROP(prp, replist_lock_taken)
	    
	    rrs_rep_adm_info(prp->rep_binding.rep_handle, &rep_info, st);
	    got_rep_info = true;
	
	LOCK_FOR_ANY_PROP(prp, replist_lock_taken, st)
	    
    } 
    
    if (STATUS_OK(st) && got_rep_info) {
	/*
	 * chk whether slave thinks there's a
	 * more recent master
	 */
	if (rep_info.master_known &&
	    rs_master_info_later_master(&rep_info.master_id, 
					&rep_info.master_seqno)) {
	    /* we're not the master any more
	     */
	    rs_master_strt_obs_master_tsk();
	    SET_STATUS(st, sec_rgy_rep_not_master);
	}
	
	if (STATUS_OK(st)) {
	    /*
	     * Will crash if slave's compat software version is
	     * later than the master's
	     */
	    rs_rep_mgmt_master_ck_sw_compat(rep_info.compat_sw_rev, st);
	    if (STATUS_EQUAL(st, sec_rgy_rep_slave_bad_sw_vers)) {
		CLEAR_STATUS(st);  /* we'll handle this below. */
	    }
	}
	
	if (STATUS_OK(st)) {
	    if (rep_info.compat_sw_rev == master_info->master_compat_sw_rev) {
		/* 
                 * This can cause a prop_type change from
		 * rs_c_replica_prop_bad_sw_rev to whatever
		 * prev_prop_type was in effect for the replica,
		 * so we do it before we check update sequence
		 * numbers immediately below.  It will cause
		 * the slave's compat_sw_rev to be set to 
		 * master_info->master_compat_sw_rev.
		 */
		prop_slave_sw_rev_ok(prp, master_info->master_compat_sw_rev,
				     lock_mode_write, st);
	    } else {
		prp->compat_sw_rev = rep_info.compat_sw_rev;
	    }
	}
    }
    
    if (!STATUS_OK(st)) return;
    
    /*
     * If our software rev is up-to-date and we're in any state other than
     * rs_c_replica_bad_sw_rev, then we're done, ohwerwise, we go ahead and
     * tell the slave to adopt the current software rev.  
     */
    if ((prp->compat_sw_rev < master_info->master_compat_sw_rev)
    || (prp->prop_info.prop_type == rs_c_replica_prop_bad_sw_rev)) {
	
	switch (prp->prop_info.prop_type) {
	    
	case rs_c_replica_prop_initing:
	    /*FALLTHROUGH
	     * Leave an initing rep alone until it is finished.
	     */
	case rs_c_replica_prop_delete:
	    /*FALLTHROUGH
	     * Don't bother bumping the version on replica 
	     * scheduled for delete--we'll just send it the delete
	     * using whatever it claims is its current
	     * software rev.
	     */
	default:
	    /* Do nothing */
	    break;
	    
	case rs_c_replica_prop_bad_sw_rev:
	    /*FALLTHROUGH*/
	case rs_c_replica_prop_init:
	    /*FALLTHROUGH*/
	case rs_c_replica_prop_update:
	    /*
             * Send the slave the stop_til_compat_sw message.
	     * The slave will either adopt the new rev and respond with
	     * a status code of sec_rgy_rep_sw_rev_adopted, or shut
	     * down and respond with error_status_ok.
	     */
	    prop_stop_til_sw_compat(prp, replist_lock_taken, 
				    master_info, st);
	    if (STATUS_OK(st)) {
		/*
		 * Slave shut itself down, so we we assign it
                 * "rs_c_replica_prop_bad_sw_rev prop type
		 * The prop_analyse_status() routine will 
		 * treat this replica as "very_bad" causing
		 * the long incommunicado task to periodically
		 * ping it by sending another stop_til_sw_compat_ok
                 * message.
		 */

		/* The following call sets the replica's prev_prop_type */
		rs_m_replist_bad_sw_rev((rs_rep_prop_mgt_handle_t) prp,
					&prp->rep_id, st);
		if (STATUS_OK(st)) {
		    rsdb_replica_set_prop_info(&prp->rep_id, 
					       rs_c_replica_prop_bad_sw_rev,
					       prp->prev_prop_type,
					       NULL, st);
		    if (STATUS_OK(st)) {
			rs_log_replist_bad_sw_rev(&prp->rep_id, 
						  &prp->prev_prop_type);
		    }
		}
		
	    }  else if (STATUS_EQUAL(st, sec_rgy_rep_sw_rev_adopted)) {
		prop_slave_sw_rev_ok(prp, master_info->master_compat_sw_rev,
				     lock_mode_write, st);
	    }
	} /* end switch */
    }

    /* 
     * If we've successfully pinged the replica and the replica's
     * prop_type is update, then sanity-check the replica's update
     * sequence numbers.  We save this for last because the
     * sw_rev negotiation above may cause a prop_type change
     * from bad_sw_rev to update.
     */
    if (STATUS_OK(st)
    && got_rep_info
    && (prp->prop_info.prop_type == rs_c_replica_prop_update)) {
	prop_analyze_slave_seqno(prp, &rep_info, st);
    }
}
    

    
/*
 * Tell the slave to either adopt the correct rev, or
 * shut down.
 */
PRIVATE void prop_stop_til_sw_compat
  (
    rs_rep_prop_mgt_item_p_t    prp,
    boolean32                   *replist_lock_taken,
    rs_replica_master_info_t    *master_info,   
    error_status_t              *st
  )
{
    rs_replica_master_info_t   my_master_info;

    /* 
     * we need to propagate the  stop_til_compat_sw() operation 
     * using whatever the the replica thinks is the current sw compat
     * version.  Do this in such a way that we don't step on the
     * the master info in use by the prop driver.
     */       

    master_info->previous_update_seqno = prp->prop_info.last_upd_seqno;

    my_master_info = *master_info;
    my_master_info.master_compat_sw_rev = prp->compat_sw_rev;

    UNLOCK_FOR_PROP(prp, replist_lock_taken)

       rrs_rep_mgr_stop_til_compat_sw(prp->rep_binding.rep_handle,
            master_info->master_compat_sw_rev, &my_master_info, st);

    LOCK_FOR_ANY_PROP(prp, replist_lock_taken, st)

    /*
     * Need to do some special error handling in the 
     * stop_til_compat_sw() case.
     */
    if (STATUS_EQUAL(st, sec_rgy_rep_master_bad_sw_vers)) {
	/*
	 * If our master info identified us as an earlier
	 * compat rev than we actually are, it would be a
	 * big mistake to act on this error message, which
	 * will cause us to shut down.  Since the slave 
	 * thinks it is newer than we are, just set things 
	 * up for a renegotiation of the slave sw rev on the
	 * next iteration through the replist.  If the slave
	 * really is newer, we'll take the appropriate steps
	 * then.
	 */
	if (my_master_info.master_compat_sw_rev != master_info->master_compat_sw_rev
        && prp->compat_sw_rev < master_info->master_compat_sw_rev) {
	    SET_STATUS(st, sec_rgy_rep_slave_bad_sw_vers);
	}
    }
}

/* 
 * Must be invoked with at least a read-intend-write lock
 * on the replica list.  If necessary, the lock will be
 * promoted to (and sunsequently demoted from) a write lock.
 *
 * Will always have the effect of setting the slave's
 * software compat revision to the value specified
 * in the "global_sw_rev" parameter, so be
 * certain that you really want that to happen.
 * If the replica currently has a prop_type of
 * rs_c_replica_prop_bad_sw_rev, will also restore
 * the previous prop type that was saved when the replica
 * was assigned the rs_c_replica_prop_bad_sw_rev
 * prop type.
 */
PRIVATE void prop_slave_sw_rev_ok 
  (
    rs_rep_prop_mgt_item_p_t    prp,             /* [in] */
    unsigned32                  global_sw_rev,   /* [in] */
    lock_mode_t                 lock_mode,       /* [in] */
    error_status_t              *st              /* [out] */
  )
{
    rs_replica_prop_t         saved_prop_type;

    saved_prop_type = prp->prop_info.prop_type;   
    
    if (lock_mode == lock_mode_riw)
        READ_INTEND_WRITE_PROMOTE(lock_replist);

    rs_m_replist_sw_rev_ok(prp, &prp->rep_id, 
			   global_sw_rev, st);
    /*
     * The above call may have changed our prop_type.  If so, we need to 
     * update the in-memory replist database and log the update to 
     * stable storage.
     */
    if (STATUS_OK(st) && (saved_prop_type != prp->prop_info.prop_type)) {
	rsdb_replica_set_prop_info(&prp->rep_id, 
				   prp->prop_info.prop_type,
				   rs_c_replica_prop_unknown /* prev prop type */,
				   NULL, /* no change to init_id */
				   st);
	if (STATUS_OK(st)) {
	    rs_log_replist_sw_rev_ok(&prp->rep_id, &prp->prop_info.prop_type);
	    
	}
    }

    if (lock_mode == lock_mode_riw)
        READ_INTEND_WRITE_DEMOTE(lock_replist);
}

/*
 * Called by the master when it receives an i_am_slave
 * from a replica.  
 *
 * Must be called with at least a replist RIW lock.
 * Lock will bw promoted if a replist update is
 * required.
 */
PUBLIC void rs_prop_tasks_handle_i_am_slave (
    rs_rep_prop_mgt_handle_t  prop_mgt_h,
    unsigned32                slave_compat_sw_rev,
    lock_mode_t               lock_mode,
    error_status_t            *st_p
)
{
    unsigned32  global_sw_rev;
    rs_rep_prop_mgt_item_p_t  prp = (rs_rep_prop_mgt_item_p_t) prop_mgt_h;

    rs_master_info_get_sw_rev(&global_sw_rev, st_p);
    if (STATUS_OK(st_p)
    && (slave_compat_sw_rev == global_sw_rev)) {
	prop_slave_sw_rev_ok(prp, slave_compat_sw_rev, lock_mode, st_p);
    }
    
}
