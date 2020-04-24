/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_master.c,v $
 * Revision 1.1.10.3  1996/02/18  00:18:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:07:09  marty]
 *
 * Revision 1.1.10.2  1995/12/08  17:52:49  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:07 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:58 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	include rs_ver.h.
 * 	[1995/02/11  22:14 UTC  greg  /main/SEC_migrate/3]
 * 
 * 	merge
 * 	[1995/02/10  19:40 UTC  hanfei  /main/SEC_migrate/2]
 * 
 * 	during initialization, check for software version for compatibility.
 * 	[1995/02/10  19:22 UTC  hanfei  /main/SEC_migrate/hanfei_comp_bl2/1]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:49 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	add rs_master_info_get/set_sw_rev().
 * 	add sw_rev to rs_master_info_create().
 * 	[1995/01/21  20:41 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.6.4  1994/08/25  16:30:15  hanfei
 * 	dce1.1 rgy database backward compatibility work
 * 	[1994/08/25  14:43:40  hanfei]
 * 
 * Revision 1.1.6.3  1994/08/04  16:13:35  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:11:34  mdf]
 * 
 * Revision 1.1.6.1  1994/06/02  21:25:26  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:18:47  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:56:01  mdf]
 * 
 * 	HP revision /main/KK/ODESSA_2/1  1994/05/03  18:52 UTC  hanfei
 * 	 merge
 * 
 * 	HP revision /main/KK/hanfei_ch_master/1  1994/04/20  15:08 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/HPDCE01/emartin_ch_master/2  1994/04/14  18:16 UTC  emartin
 * 	when clear master_info, null out master_id and mastert_seqno_seqno
 * 	check master_known more consistently.
 * 	master_info_set - chk whether current master_seqno >= new_master_seqno.
 * 
 * 	HP revision /main/HPDCE01/emartin_ch_master/1  1994/04/12  14:38 UTC  emartin
 * 	move master change propq seqno checking to rs_rep_mgmt.c
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/26  20:03  mdf
 * 	merge
 * 
 * Revision 1.1.4.4  1993/12/17  22:24:01  hanfei
 * 	do rs_master_info_check_master_seqno
 * 	fill in rs_master_strt_obs_master_tsk
 * 	[1993/12/17  22:02:42  hanfei]
 * 
 * Revision 1.1.4.3  1993/12/16  19:11:10  hanfei
 * 	do declare rs_master_change_copy_propq_done
 * 	[1993/12/11  18:46:29  hanfei]
 * 
 * 	do rs_master_info_become_master
 * 	[1993/12/08  04:43:48  hanfei]
 * 
 * Revision 1.1.4.2  1993/10/29  16:48:21  hanfei
 * 	work for change master : change error checking in master_info_set.
 * 	[1993/10/29  16:47:39  hanfei]
 * 
 * 	CR 7382 fix
 * 	rs_master_info_crnt_srgt_master - don't return inside lock
 * 	macros.  Add read locks to routines that were missing them.
 * 	[1993/03/05  20:21:43  emartin]
 * 
 * 	em_bl6f: no change
 * 	[1993/02/24  22:28:54  emartin]
 * 
 * 	em_bl6f
 * 	[1993/02/11  20:16:45  emartin]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  21:24:25  zeliff]
 * 
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:49:54  sekhar]
 * 
 * 	Add rs_master_info_get_file_version.
 * 	[1992/11/16  16:38:41  ahop]
 * 
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:17:22  sekhar]
 * 
 * Revision 1.1.2.3  1993/09/15  15:50:28  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.4.2  1992/12/11  21:42:46  emartin
 * 	bl6c: support surrogate master
 * 
 * Revision 1.1.1.2  1992/10/06  17:02:01  burati
 * 	Second replication drop: bl5
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - master info mgmt and master change 
 *      operations
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_master.c,v 1.1.10.3 1996/02/18 00:18:27 marty Exp $";
#endif


#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>

#include <macros.h>
#include <rs_pvt.h>
#include <rs_ver.h>
#include <rs_ver_pvt.h>
#include <rs_rep_pvt.h>
#include <rs_master.h>
#include <rs_state.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_util.h>
#include <rs_rep_mgmt.h>
#include <rs_task.h>
#include <rsdb.h>

#include <sec_svc.h>
/*
 * Local variables
 */
PRIVATE rs_master_info_t    master_info;
PRIVATE rs_master_info_t    surrogate_master_info;

/*
 * Local routine prototypes
 */

PRIVATE void master_info_set (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno,
    error_status_t      *st
);

PRIVATE boolean32 current_master (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno
);

PRIVATE boolean32 later_master (
#ifdef __STDC__
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno
#endif
);

/*
 *  Create the master_info disk file
 *  and save this master_info in it.
 *
 *  Assume caller has a global write lock
 *  on database or is single-threaded.
 */
PUBLIC void rs_master_info_create
  (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno,
    unsigned32          global_sw_rev,
    error_status_t      *st
  )
{
    boolean32       eq;
    error_status_t  lst;

    master_info.version = rs_c_master_info_file_version;
    master_info.master_known = true;
    master_info.master_id = *master_id;
    master_info.master_seqno = *master_seqno;
    master_info.global_sw_rev = global_sw_rev;
   
    rsdb_master_info_create(&master_info, st);
    if (BAD_STATUS(st)) return;

    eq = uuid_equal(master_id, &rs_state.rep_id, &lst);
    rs_state_set_master_flag(eq, st);
    if (BAD_STATUS(st)) return;
}

/*
 *  Open master_info file and
 *  read current master_info into memory.
 *  Set rs_state.master flag to
 *  true if this replica is the master
 *  or to false if this isn't the master.
 *
 *  Assume caller has a global write lock
 *  on database or is single-threaded.
 */
PUBLIC void rs_master_info_init
  (
    error_status_t      *st
  )
{
    boolean32       eq;

    rsdb_master_info_open(&master_info, st);
    if (BAD_STATUS(st)) return;

    if (master_info.master_known) {
        eq = uuid_equal(&master_info.master_id, &rs_state.rep_id, st);
        if (BAD_STATUS(st)) return;
    }
    else {
        /* assume this is a slave if the master
         * isn't known
         */
        eq = false;
    }

    if ( master_info.global_sw_rev < rs_c_default_min_sw_vers ||
         master_info.global_sw_rev > rs_c_default_max_sw_vers ) {
        SET_STATUS(st, sec_rgy_rep_bad_sw_vers);
        return;
    }

    rs_state_set_master_flag(eq, st);
    if (BAD_STATUS(st)) return;
}

/*
 *  Save new master_info about the current
 *  master replica.
 *  Set rs_state.master flag to
 *  true if this replica is the master
 *  or to false if this isn't the master.
 */
PUBLIC void rs_master_info_set
    (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno,
    error_status_t      *st
    )
{
    CLEAR_STATUS(st);

    WRITE_LOCK(lock_master_info) {

        master_info_set(master_id, master_seqno, st);

    } END_WRITE_LOCK;
}

PUBLIC void rs_master_info_set_sw_rev (
    unsigned32      global_sw_rev,   /* [in] */
    error_status_t  *st_p            /* [out] */
) 
{
    WRITE_LOCK(lock_master_info) {
	master_info.global_sw_rev = global_sw_rev;
	rsdb_master_info_write(&master_info, st_p);
    } END_WRITE_LOCK;
}


/*
 *  This replica is becoming the master.
 *  Gen up a new master_seqno and save
 *  this replica's id as the new master id 
 *  and save the new master_seqno in master_info.
 *
 *  Also, set the rs_state.master_flag to true.
 */
PUBLIC void rs_master_info_become_master
    (
    error_status_t      *st
    )
{
    rs_update_seqno_t   master_seqno;
    sec_timeval_t       upd_ts;

    CLEAR_STATUS(st);

    rs_log_get_update_info(&master_seqno, &upd_ts, st);
    rs_util_seqno_next(&master_seqno);

    if (master_info.master_known && 
        (rs_util_seqno_cmp(
            &master_info.master_seqno, &master_seqno) >= 0)) {
        master_seqno = master_info.master_seqno;
        rs_util_seqno_next(&master_seqno);
    }

    WRITE_LOCK(lock_master_info) {

        master_info_set(&rs_state.rep_id, &master_seqno, st);

    } END_WRITE_LOCK;
}

/*
 *  Save new master_info.  Must
 *  be invoked with lock_master_info write lock
 */
PRIVATE void master_info_set
    (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno,
    error_status_t      *st
    )
{
    boolean32       eq;
    error_status_t  lst;

    CLEAR_STATUS(st);

    if ((master_info.master_known) &&
        (! (later_master(master_id, master_seqno) ||
            current_master(master_id, master_seqno)) ) ) {
        SET_STATUS(st, sec_rgy_rep_bad_master_seqno);
        return;
    }

    master_info.master_known = true;
    master_info.master_id = *master_id;
    master_info.master_seqno = *master_seqno;

    rsdb_master_info_write(&master_info, st);
    if (BAD_STATUS(st)) return;

    eq = uuid_equal(master_id, &rs_state.rep_id, &lst);
    rs_state_set_master_flag(eq, st);
}


/*
 * Master's identity is not currently known
 * so set master_info.master_known to false
 * and clear master_id and master_seqno.
 */
PUBLIC void rs_master_info_clear
  (
    error_status_t      *st
  )
{
    error_status_t    lst;

    WRITE_LOCK(lock_master_info) {

        master_info.master_known = false;
        uuid_create_nil(&master_info.master_id, &lst);
        rs_util_seqno_create(&master_info.master_seqno);
    
        rsdb_master_info_write(&master_info, st);

        if (GOOD_STATUS(st)) {
            rs_state_set_master_flag(false, st);
        }

    } END_WRITE_LOCK;
}

/*
 *  Get information about the current master 
 *  replica
 */
PUBLIC void rs_master_info_get
    (
    boolean32           *master_known,
    uuid_t              *master_id,
    rs_update_seqno_t   *master_seqno,
    error_status_t      *st
    )
{
    READ_LOCK(lock_master_info) {

        *master_known = master_info.master_known;
        *master_id = master_info.master_id;
        *master_seqno = master_info.master_seqno;

    } END_READ_LOCK;

    CLEAR_STATUS(st);
}

PUBLIC void rs_master_info_get_file_version
    (
    unsigned32          *file_version,
    error_status_t      *st
    )
{
    READ_LOCK(lock_master_info) {

        *file_version = master_info.version;

    } END_READ_LOCK;

    CLEAR_STATUS(st);
}

PUBLIC void rs_master_info_get_sw_rev (
    unsigned32      *global_sw_rev,  /* [out] */
    error_status_t  *st_p            /* [out] */
) 
{
    READ_LOCK(lock_master_info) {

        *global_sw_rev = master_info.global_sw_rev;

    } END_READ_LOCK;

    CLEAR_STATUS(st_p);
}

/*  
 *  Slave check that an update is from master.
 *
 *  Checks that "master_id" and "master_seqno" represent 
 *  the current master.  If not, the routine makes 
 *  this new master the master, if its "master_seqno"
 *  is > than the current master's master_seqno.
 *  Info about the new master is saved to disk and
 *  in memory.
 *
 *  Should be invoked from inside the replist or 
 *  dbase write lock which prevents the rep_state
 *  from changing.
 */
PUBLIC void rs_master_info_check_master 
    (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno,
    error_status_t      *st
    )
{
    error_status_t      lst;

    CLEAR_STATUS(st);

    READ_INTEND_WRITE_LOCK(lock_master_info) {

        if ((!master_info.master_known) ||
	    (later_master(master_id, master_seqno)) ) {
            /* master not know or this is a later 
             * master, so adopt it.
             */
            READ_INTEND_WRITE_PROMOTE(lock_master_info);
            master_info_set(master_id, master_seqno, st);
        }

        else
        if (!current_master(master_id, NULL)) { 
            /* Not current master and
             * Not later master, so its update is rejected.
             */
            SET_STATUS(st, sec_rgy_rep_not_from_master);
        }

    } END_READ_INTEND_WRITE_LOCK;
}

PUBLIC boolean32 rs_master_info_current_master
    (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno
    )
{ 
    boolean32   retval;

    READ_LOCK(lock_master_info) {

        retval = current_master(master_id, master_seqno);

    } END_READ_LOCK;

    return retval;
}

/* 
 *  Return true if the master_id and master_seqno 
 *  represent the current master. Return false 
 *  otherwise.  
 *  If master_seqno is NULL, it is not included
 *  in the test to see if master_id is the current
 *  master.
 *
 *  Assume the caller has the read lock.
 */
PRIVATE boolean32 current_master
    (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno
    )
{
    error_status_t  lst;

    if (master_info.master_known &&
        uuid_equal(&master_info.master_id, master_id, &lst) && 
        ((master_seqno == NULL) ||
         (rs_util_seqno_cmp(
            &master_info.master_seqno, master_seqno) == 0)) )
        return true;
    else
        return false;
}

/*
 * Return true if master_id and master_seqno
 * represent a different and more recent master
 * than master_info.  otherwise return false.
 */
PUBLIC boolean32 rs_master_info_later_master
    (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno
    )
{
    boolean32       retval;
    error_status_t  lst; 

    READ_LOCK(lock_master_info) {

        if (master_info.master_known &&
            (later_master(master_id, master_seqno)) ) 
            retval = true;
        else
            retval = false;
    
    } END_READ_LOCK;

    return retval;
}

/*
 * If master_seqno > current master_seqno (in master_info),
 * return true.
 * If seqnos are equal and master_id uuid is > current
 * master_id uuid, return true.
 */
PRIVATE boolean32 later_master
#ifndef __STDC__
      (master_id, master_seqno)
    uuid_p_t            master_id;
    rs_update_seqno_t   *master_seqno;
#else
    (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno
    )
#endif
{
    signed32        cmp, cmp2;
    boolean32       retval;
    error_status_t  lst;

    if (!master_info.master_known) 
        return false;

    cmp = rs_util_seqno_cmp(master_seqno, &master_info.master_seqno);
    if (cmp > 0)
        retval = true;
    else
    if (cmp < 0)
        retval = false;
    else {
        /* seqnos are equal
         */

         /* if master ids are equal, master_id isn't later
          * master
          */
        if (uuid_equal(master_id, &master_info.master_id, &lst)) {
            retval = false;
        }         
        else {
            /* if master_id uuid is > current master_id,
             * master_id is later master.
             */
            cmp2 = uuid_compare(master_id, &master_info.master_id, &lst);
            retval = (cmp2 > 0) ? true : false;
        }
    }

    return retval;
}

/*
 *  Make "master_id" the surrogate master.
 */
PUBLIC void rs_master_info_set_srgt_master
    (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno,
    error_status_t      *st
    )
{
    WRITE_LOCK(lock_master_info) {

        surrogate_master_info.master_known = true;
        surrogate_master_info.master_id = *master_id;
        surrogate_master_info.master_seqno = *master_seqno;

    } END_WRITE_LOCK;
}

/*
 *  Get information about the current surrogate master 
 *  replica
 */
PUBLIC void rs_master_info_get_srgt_master
    (
    boolean32           *master_known,
    uuid_t              *master_id,
    rs_update_seqno_t   *master_seqno,
    error_status_t      *st
    )
{
    READ_LOCK(lock_master_info) {

        *master_known = surrogate_master_info.master_known;
    
        if (surrogate_master_info.master_known) {
            *master_id = surrogate_master_info.master_id;
            *master_seqno = surrogate_master_info.master_seqno;
        }

    } END_READ_LOCK;

    CLEAR_STATUS(st);
}


/*
 * Clear the surrogate master_info.  There is
 * no surrogate master now
 */
PUBLIC void rs_master_info_clr_srgt_master
  (
    error_status_t      *st
  )
{
    WRITE_LOCK(lock_master_info) {

        surrogate_master_info.master_known = false;

    } END_WRITE_LOCK;
}

/* 
 *  Return true if the master_info represents
 *  the current surrogate master. Return false 
 *  otherwise.
 */
PUBLIC boolean32 rs_master_info_crnt_srgt_master
    (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno,
    error_status_t      *st
    )
{
    boolean32       retval;
    error_status_t  lst;

    READ_LOCK(lock_master_info) {

        if (surrogate_master_info.master_known &&
            uuid_equal(&surrogate_master_info.master_id, master_id, &lst) &&
            (rs_util_seqno_cmp(
                &surrogate_master_info.master_seqno, master_seqno) == 0))
            retval = true;
        else
            retval = false;

    } END_READ_LOCK;

    return retval;
}

PRIVATE void obsolete_master_task (
    void
)
{
    error_status_t    lst;

    dce_svc_printf(SEC_RS_THR_STARTED_MSG , RS_OBSOLETE_MASTER_THR);

    BEGIN_GLOBAL_RIW_EVENT(rs_c_event_no_longer_master, &lst) {
   
        rs_state_set_state(rs_c_state_dup_master, &lst);
        
    } END_GLOBAL_RIW_EVENT;

    if (GOOD_STATUS(&lst))
        dce_svc_printf(SEC_RS_REP_NOT_MASTER_MSG);

    dce_svc_printf(SEC_RS_THR_EXITING_MSG , RS_OBSOLETE_MASTER_THR);
}

/*
 * Start a task to make the master become a slave.
 */
PUBLIC void rs_master_strt_obs_master_tsk (
    void
)
{
    rs_task_handle_t        task_h;

    rs_task_create(&task_h,
                   (rs_task_func_ptr_t) obsolete_master_task,
                   RS_OBSOLETE_MASTER_THR,
                   false, NULL);
}
