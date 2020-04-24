/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_master.h,v $
 * Revision 1.1.10.3  1996/02/18  22:59:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:43  marty]
 *
 * Revision 1.1.10.2  1995/12/13  16:25:24  root
 * 	Submit
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:00 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:34 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:44 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	intermediate checkin just for safety
 * 	[1995/01/21  23:54 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.6.3  1994/06/02  21:01:20  mdf
 * 	Merged with changes from 1.1.6.2
 * 	[1994/06/02  21:00:50  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:14:54  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.4.3  1993/12/17  22:28:24  hanfei
 * 	declare rs_master_info_check_master_seqno
 * 	[1993/12/17  22:06:33  hanfei]
 * 
 * Revision 1.1.4.2  1993/12/16  18:44:37  hanfei
 * 	declare rs_master_change_copy_propq_done
 * 	declare rs_master_info_become_master
 * 	[1993/12/11  18:52:04  hanfei]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:37  zeliff]
 * 
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:00  sekhar]
 * 
 * 	Add rs_master_info_get_file_version
 * 	[1992/11/16  16:29:40  ahop]
 * 
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:11:40  sekhar]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:33  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.4.2  1992/12/11  21:08:50  emartin
 * 	bl6c: rs_master_info_current_master, rs_master_info_later_master,
 * 	rs_master_change_chk_update_seqno, rs_master_become_slave_task
 * 
 * Revision 1.1.1.2  1992/10/06  15:38:20  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.6.2  1994/06/02  20:21:39  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:14:54  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.4.3  1993/12/17  22:28:24  hanfei
 * 	declare rs_master_info_check_master_seqno
 * 	[1993/12/17  22:06:33  hanfei]
 * 
 * Revision 1.1.4.2  1993/12/16  18:44:37  hanfei
 * 	declare rs_master_change_copy_propq_done
 * 	declare rs_master_info_become_master
 * 	[1993/12/11  18:52:04  hanfei]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:37  zeliff]
 * 
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:00  sekhar]
 * 
 * 	Add rs_master_info_get_file_version
 * 	[1992/11/16  16:29:40  ahop]
 * 
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:11:40  sekhar]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:33  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.4.2  1992/12/11  21:08:50  emartin
 * 	bl6c: rs_master_info_current_master, rs_master_info_later_master,
 * 	rs_master_change_chk_update_seqno, rs_master_become_slave_task
 * 
 * Revision 1.1.1.2  1992/10/06  15:38:20  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.3.5  1992/12/29  13:08:21  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:37  zeliff]
 * 
 * Revision 1.1.3.4  1992/12/18  22:10:20  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:00  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:08:50  emartin
 * 	bl6c: rs_master_info_current_master, rs_master_info_later_master,
 * 	rs_master_change_chk_update_seqno, rs_master_become_slave_task
 * 
 * Revision 1.1.3.3  1992/11/20  15:50:25  ahop
 * 	Add rs_master_info_get_file_version
 * 	[1992/11/16  16:29:40  ahop]
 * 
 * Revision 1.1.3.2  1992/10/07  20:26:17  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:11:40  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:38:20  burati
 * 	 Second replication code drop: bl5
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


#ifndef rs_master_h__included
#define rs_master_h__included


#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>

#include <macros.h>
#include <rs_pvt.h>

/*
 *  Create the master_info disk file
 *  and save this master_info in it.
 */
void rs_master_info_create (
    uuid_p_t            master_id,
    rs_update_seqno_t  *master_seqno,
    unsigned32          global_sw_rev,
    error_status_t      *st
);

/*
 *  Open master_info file and
 *  read current master_info into memory.
 *  Set rs_state.master flag to
 *  true if this replica is the master
 *  or to false if this isn't the master.
 */
void rs_master_info_init (
    error_status_t      *st
);

/*
 *  Save new master_info.
 *  Set rs_state.master flag to
 *  true if this replica is the master
 *  or to false if this isn't the master.
 */
void rs_master_info_set (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno,
    error_status_t      *st
);

/*
 *  This replica is becoming the master.
 *  Gen up a new master_seqno and save
 *  this replica's id as the new master id 
 *  and save the new master_seqno in master_info.
 *
 *  Also, set the rs_state.master_flag to true.
 */
void rs_master_info_become_master (
    error_status_t      *st
);

/*
 * Master's identity is not currently known
 */
void rs_master_info_clear (
    error_status_t      *st
);

/* 
 *  Get information about the current master 
 *  replica
 */
void rs_master_info_get (
    boolean32           *master_known,
    uuid_t              *master_id,
    rs_update_seqno_t   *master_seqno,
    error_status_t      *st
);

void rs_master_info_get_file_version (
    unsigned32          *file_version,
    error_status_t      *st
);

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
void rs_master_info_check_master (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno,
    error_status_t      *st
);

/* 
 *  Return true if the master_id and master_seqno 
 *  represent the current master. Return false 
 *  otherwise.  
 *  If master_seqno is NULL, it is not included
 *  in the test to see if master_id is the current
 *  master.
 */
boolean32 rs_master_info_current_master (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno
);

/*
 * Return true if master_id and master_seqno
 * represent a different and more recent master
 * than the current master.  otherwise return 
 * false.
 */
boolean32 rs_master_info_later_master (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno
);

/*
 *  Make "master_id" the surrogate master.
 */
void rs_master_info_set_srgt_master (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno,
    error_status_t      *st
);

/*
 *  Get information about the current surrogate master 
 *  replica
 */
void rs_master_info_get_srgt_master (
    boolean32           *master_known,
    uuid_t              *master_id,
    rs_update_seqno_t   *master_seqno,
    error_status_t      *st
);

/*
 * Clear the surrogate master_info.  There is
 * no surrogate master now
 */
void rs_master_info_clr_srgt_master (
    error_status_t      *st
);

/* 
 *  Return true if the master_info represents
 *  the current surrogate master. Return false 
 *  otherwise.
 */
boolean32 rs_master_info_crnt_srgt_master (
    uuid_p_t            master_id,
    rs_update_seqno_t   *master_seqno,
    error_status_t      *st
);

/*
 * Slave will adopt a new master.
 * If its last_update_seqno is >= the
 * new master's seqno (ie. the seqno when
 * this master became master), then the
 * slave has received updates from another
 * master and it should be synced up with the
 * new master by re-init
 */
void rs_master_info_check_master_seqno (
    rs_update_seqno_t   *master_seqno,
    error_status_t      *st
);

/*
 * Start a task to make the die because its
 * found a dupe master
 */
void rs_master_strt_obs_master_tsk (
    void
);

void rs_master_info_set_sw_rev (
    unsigned32      global_sw_rev,   /* [in] */
    error_status_t  *st_p            /* [out] */
);

void rs_master_info_get_sw_rev (
    unsigned32      *global_sw_rev,  /* [out] */
    error_status_t  *st_p            /* [out] */
);

#endif
