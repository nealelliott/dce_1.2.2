/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_state.c,v $
 * Revision 1.1.10.3  1996/02/18  00:20:14  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:19  marty]
 *
 * Revision 1.1.10.2  1995/12/08  17:56:50  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:10 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  18:59 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	merge
 * 	[1995/04/12  18:13 UTC  hanfei  /main/SEC_migrate/5]
 * 
 * 	rs_state_check() event stop_til_compat_sw should not check for master
 * 	[1995/04/12  18:09 UTC  hanfei  /main/SEC_migrate/hanfei_migrate_bl2/1]
 * 
 * 	Merge .../greg_sec_migrate/LATEST --> .../SEC_migrate/LATEST.
 * 	[1995/02/11  22:15 UTC  greg  /main/SEC_migrate/4]
 * 
 * 	global_compat_sw_rev field of state info is obsolete.
 * 	rs_state_set_global_sw_vers() is obsolete.
 * 	[1995/02/06  08:18 UTC  greg  /main/SEC_migrate/greg_sec_migrate/1]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:48 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	add rs_c_event_set_sw_rev to state checks.
 * 	remove boot-time check for valid software
 * 	revision form rs_state_init--the software
 * 	version has been moved to the master info.
 * 	[1995/01/21  23:06 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.6.3  1994/08/25  16:30:26  hanfei
 * 	dce1.1 rgy database backward compatibility work
 * 	[1994/08/25  14:44:17  hanfei]
 * 
 * Revision 1.1.6.2  1994/08/04  16:14:07  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:12:35  mdf]
 * 
 * Revision 1.1.6.1  1994/06/02  21:20:40  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:20:16  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:57:14  mdf]
 * 
 * 	HP revision /main/KK/ODESSA_2/1  1994/05/03  21:16 UTC  hanfei
 * 	 merge
 * 
 * 	HP revision /main/KK/hanfei_ch_master/1  1994/05/03  20:42 UTC  hanfei
 * 	label change master work
 * 
 * Revision 1.1.4.4  1993/12/17  22:24:14  hanfei
 * 	add event for rs_c_event_no_longer_master
 * 	[1993/12/17  22:06:01  hanfei]
 * 
 * Revision 1.1.4.3  1993/12/16  19:12:10  hanfei
 * 	add action for event : rs_c_event_copy_propq
 * 	[1993/12/11  18:51:15  hanfei]
 * 
 * 	rs_state_check - deal w. rs_c_event_i_am_master
 * 	do rs_state_current_state
 * 	[1993/12/08  04:42:24  hanfei]
 * 
 * Revision 1.1.4.2  1993/10/29  17:23:28  hanfei
 * 	work for change master : add rs_c_state_becoming_master in
 * 	for force become master.
 * 	[1993/10/29  17:22:52  hanfei]
 * 
 * 	em_bl6f: do rs_state_set_state_renaming,
 * 	 rs_state_set_global_sw_vers
 * 	 rs_state_check - chk rs_c_event_stop_til_compat_sw
 * 	[1993/02/11  20:19:26  emartin]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  21:26:59  zeliff]
 * 
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:54:00  sekhar]
 * 
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:19:46  sekhar]
 * 
 * Revision 1.1.2.3  1993/09/15  15:51:12  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.4.2  1992/12/11  21:54:40  emartin
 * 	bl6c: more events in rs_state_chk
 * 
 * Revision 1.1.1.2  1992/10/06  17:29:08  burati
 * 	Second replication drop: bl5
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - replica state management
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_state.c,v 1.1.10.3 1996/02/18 00:20:14 marty Exp $";
#endif


#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>

#include <rs_pvt.h>
#include <rs_state.h>
#include <rsdb.h>
#include <rs_util.h>
#include <sec_svc.h>

/*
 *  Public data
 */
PUBLIC rs_state_t       rs_state;


/*  Want to create the rgy_state file.
 *  return an error if it already exists.
 */
PUBLIC void rs_state_create_chk
    (
    error_status_t          *st
    )
{
    /*
     * Check whether the database already exists
     * by trying to read the state file.
     * State file read should fail.
     */
    rsdb_state_open(&rs_state, st);
    if (STATUS_OK(st)) {
        switch (rs_state.rep_state) {
            /* create failed, just wipeout database
             */
            case rs_c_state_unknown_to_master:
                rsdb_state_delete();
                break;

            /* just wipeout database
             */
            case rs_c_state_deleted:
                rsdb_state_delete();
                break;

            default:
                SET_STATUS(st, sec_rgy_object_exists);
                rsdb_state_close();
                break;
        }
    }
    else
    if (STATUS_EQUAL(st, sec_rgy_object_not_found))
        CLEAR_STATUS(st);
}


/*
 *  Creating rgy_state and rgy database.
 *  Fill rs_state in memory but don't save it 
 *  to disk yet
 */
PUBLIC void rs_state_create_volatile 
  (
    unsigned32          rep_state,
    uuid_p_t            cell_sec_id,
    uuid_p_t            rep_id,
    boolean32           masterf
  )
{
    error_status_t      lst;

    rs_state.version               = rs_c_state_file_version;
    rs_state.rep_state             = rep_state;
    rs_state.cell_sec_id           = *cell_sec_id;
    rs_state.rep_id                = *rep_id;

    uuid_create_nil(&rs_state.init_id, &lst);

    rs_state.unused = 0;  /* formerly global_compat_sw_rev */

    rs_state.master = masterf;
}

/*  Create rgy_state file and save the current
 *  rs_state info in it.
 */
PUBLIC void rs_state_create
    (
    error_status_t          *st
    )
{
    rsdb_state_create(&rs_state, st);
}

/*  
 *  Open and lock the rgy_state file.
 *  Read its info into rs_state.
 *  Check that this server's software
 *    is at a compatible rev.
 *  Note: Even though rs_state_init failed because 
 *    the sec_rgy_rep_bad_db_version error, 
 *    we expect the rs_state.version to have been
 *    initialized
 */
PUBLIC void rs_state_init
    (
    error_status_t          *st
    )
{
    boolean32   eq;

    rsdb_state_open(&rs_state, st);
    if (BAD_STATUS(st)) return;

}

/*
 *  Save replica state
 */
PUBLIC void rs_state_set_state 
    (
    unsigned32          rep_state,
    error_status_t      *st
    )
{
    rs_state.rep_state = rep_state;

    rsdb_state_write(&rs_state, st);
    if (BAD_STATUS(st)) return;
}

/*
 *  Save replica state as renaming
 *  and save the next_rep_state which
 *  is to be assumed after renaming is 
 *  done.
 */
PUBLIC void rs_state_set_state_renaming 
  (
    unsigned32          next_rep_state,
    error_status_t      *st
  )
{
    rs_state.rep_state = rs_c_state_renaming;
    rs_state.next_rep_state = next_rep_state;

    rsdb_state_write(&rs_state, st);
    if (BAD_STATUS(st)) return;
}

/*
 *  Set replica state in memory only
 *  For initializing, copying_dbase, becoming_master, 
 *  closed
 */
PUBLIC void rs_state_set_state_volatile 
    (
    unsigned32          rep_state,
    error_status_t      *st
    )
{
    rs_state.rep_state = rep_state;

    CLEAR_STATUS(st);
}

/*
 *  Save replica's initialization id
 */
PUBLIC void rs_state_set_init_id
    (
    uuid_p_t            init_id,
    error_status_t      *st
    )
{
    rs_state.init_id = *init_id;

    rsdb_state_write(&rs_state, st);
    if (BAD_STATUS(st)) return;
}

/*
 *  Get replica's initialization id
 */
PUBLIC void rs_state_get_init_id 
    (
    uuid_t              *init_id,
    error_status_t      *st
    )
{
    *init_id = rs_state.init_id;

    CLEAR_STATUS(st);
}

/*
 * Set the flag which indicates whether or not
 * this replica is the master
 */
PUBLIC void rs_state_set_master_flag 
  (
    boolean32           masterf,
    error_status_t      *st
  )
{
    rs_state.master = masterf;

    CLEAR_STATUS(st);
}

/*
 *  Return true if this replica is the master
 *  replica
 */
PUBLIC boolean32 rs_state_master (
    void
)
{
    return (rs_state.master);
}

/*
 *  Return state version
 */
PUBLIC unsigned32 rs_state_get_version 
  (
  )
{
    return(rs_state.version);
}

/*
 *  Return true if this server is exiting
 */
PUBLIC boolean32 rs_state_exiting (
    void
)
{
    boolean32   retval;

    switch (rs_state.rep_state) {
        case rs_c_state_closed:
            retval = true;
            break;

        case rs_c_state_deleted:
            retval = true;
            break;

        default:
            retval = false;
            break;
    }

    return (retval);
}

/*
 * If replica's current state is "state"
 * return true, otherwise return false
 */
PUBLIC boolean32 rs_state_current_state 
    (
    unsigned32          state
    )
{
    if (rs_state.rep_state == state)
        return(true);
    else
        return(false);
}



/*
 *  Check whether the replica is in a state
 *  which is ok for this event.  Return
 *  bad status if it isn't.
 *
 *  A replica's state is usually checked before
 *  an attempt to acquire a lock for the operation 
 *  is done and after the lock is acquired.  In some
 *  cases a state is ok before the lock is taken
 *  but not after it has been taken.  For example,
 *  when a server is in rs_c_state_renaming it
 *  takes the global write lock to prevent all
 *  other operations from taking place.  But this
 *  state is short-lived and some operations can
 *  wait on the lock until the renaming is finished
 *  and the server has reverted to its more permanent 
 *  state.
 */
PUBLIC void rs_state_check
  (
    rs_state_event_t    event,
    boolean32           lock_taken,
    error_status_t      *st
  )
{
    CLEAR_STATUS(st);

    switch (event) {
        case rs_c_event_read:
            switch(rs_state.rep_state) {
                case rs_c_state_unknown_to_master:
                case rs_c_state_uninitialized:
                case rs_c_state_initializing:
                case rs_c_state_closed:
                case rs_c_state_deleted:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;

                case rs_c_state_in_service:
                case rs_c_state_copying_dbase:
                case rs_c_state_in_maintenance:
                case rs_c_state_mkey_changing:
                case rs_c_state_becoming_master:
                case rs_c_state_becoming_slave:
                    break;

                case rs_c_state_renaming:
                    if (lock_taken) {
                        SET_STATUS(st, sec_s_pgmerr);
                    }
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }

            break;

        /*
         * read basic data that even an uninitialized slave
         * maintains
         */
        case rs_c_event_primitive_read:
            switch(rs_state.rep_state) {
                case rs_c_state_unknown_to_master:
                case rs_c_state_closed:
                case rs_c_state_deleted:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;

                case rs_c_state_renaming:
                    if (lock_taken) {
                        SET_STATUS(st, sec_s_pgmerr);
                    }
                    break;

                default:
                    break;
            }

            break;

        case rs_c_event_master_write:
            if (!rs_state_master()) {
                SET_STATUS(st, sec_rgy_rep_not_master);
                break;
            }

            switch(rs_state.rep_state) {
                case rs_c_state_in_service:
                    break;

                /* 
                 * When a server is in rs_c_state_renaming 
                 * it holds the global write lock
                 * but it's a quick operation, so let a
                 * master write wait on the lock for the
                 * renaming operation to finish.
                 */
                case rs_c_state_renaming:
                    if (lock_taken) {
                        SET_STATUS(st, sec_s_pgmerr);
                    }
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

        case rs_c_event_slave_write:
            if (rs_state_master()) {
                SET_STATUS(st, sec_rgy_rep_master);
                break;
            }

            switch(rs_state.rep_state) {
                case rs_c_state_initializing:
                case rs_c_state_in_service:
                    break;

                /* 
                 * When a server is in rs_c_state_renaming 
                 * it holds the global write lock
                 * but it's a quick operation,  so let a
                 * slave write wait on the lock for the
                 * renaming operation to finish
                 */
                case rs_c_state_renaming:
                    if (lock_taken) {
                        SET_STATUS(st, sec_s_pgmerr);
                    }
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

        case rs_c_event_slave_initialize:
            if (rs_state_master()) {
                SET_STATUS(st, sec_rgy_rep_master);
                break;
            }

            switch(rs_state.rep_state) {
                case rs_c_state_uninitialized:
                case rs_c_state_initializing:
                case rs_c_state_in_service:
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

        case rs_c_event_copy_propq:
            if (!rs_state_master()) {
                SET_STATUS(st, sec_rgy_rep_not_master);
                break;
            }

            switch(rs_state.rep_state) {
                case rs_c_state_becoming_slave:
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

        case rs_c_event_propq_write:
            if (rs_state_master()) {
                SET_STATUS(st, sec_rgy_rep_master);
                break;
            }

            switch(rs_state.rep_state) {
                case rs_c_state_becoming_master:
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

        case rs_c_event_checkpt_dbase:
            switch(rs_state.rep_state) {
                case rs_c_state_uninitialized:
                case rs_c_state_initializing:
                case rs_c_state_in_service:
                case rs_c_state_mkey_changing:
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

        case rs_c_event_save_dbase_to_disk:
            switch(rs_state.rep_state) {
                /* used by rgy_create */
                case rs_c_state_unknown_to_master:
                    break;

                case rs_c_state_uninitialized:
                case rs_c_state_initializing:
                case rs_c_state_in_service:
                case rs_c_state_mkey_changing:
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

        case rs_c_event_propagate:
            if (!rs_state_master()) {
                SET_STATUS(st, sec_rgy_rep_not_master);
                break;
            }

            switch(rs_state.rep_state) {
                case rs_c_state_in_service:
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;



        case rs_c_event_copy_dbase_from_me:
            switch(rs_state.rep_state) {
                case rs_c_state_in_service:
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

        case rs_c_event_copy_dbase_to_me:
            switch(rs_state.rep_state) {
                case rs_c_state_initializing:
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

        case rs_c_event_mkey_change:
            switch(rs_state.rep_state) {
                case rs_c_state_in_service:
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

        case rs_c_event_into_maintenance:
            if (!rs_state_master()) {
                SET_STATUS(st, sec_rgy_rep_not_master);
                break;
            }

            switch(rs_state.rep_state) {
                case rs_c_state_in_service:
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

        case rs_c_event_out_of_maintenance:
            if (!rs_state_master()) {
                SET_STATUS(st, sec_rgy_rep_not_master);
                break;
            }

            switch(rs_state.rep_state) {
                case rs_c_state_in_maintenance:
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

        case rs_c_event_become_master:
            if (rs_state_master()) {
                SET_STATUS(st, sec_rgy_rep_master);
                break;
            }

            switch(rs_state.rep_state) {
                case rs_c_state_in_service:
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

        case rs_c_event_set_sw_rev:
        case rs_c_event_become_slave:
        case rs_c_event_change_master:
            if (!rs_state_master()) {
                SET_STATUS(st, sec_rgy_rep_not_master);
                break;
            }

            switch(rs_state.rep_state) {
                case rs_c_state_in_service:
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

        case rs_c_event_delete:
            if (rs_state_master()) {
                SET_STATUS(st, sec_rgy_rep_master);
                break;
            }

            switch(rs_state.rep_state) {
                case rs_c_state_unknown_to_master:
                case rs_c_state_uninitialized:
                case rs_c_state_initializing:
                case rs_c_state_in_service:
                case rs_c_state_in_maintenance:
                case rs_c_state_deleted:
                    break;

                /* 
                 * When a server is in rs_c_state_renaming 
                 * it holds the global write lock
                 * but it's a quick operation, so let a
                 * master write wait on the lock for the
                 * renaming operation to finish.
                 */
                case rs_c_state_renaming:
                    if (lock_taken) {
                        SET_STATUS(st, sec_s_pgmerr);
                    }
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

        case rs_c_event_stop_til_compat_sw:
            switch(rs_state.rep_state) {
                case rs_c_state_uninitialized:
                case rs_c_state_initializing:
                case rs_c_state_in_service:
                case rs_c_state_copying_dbase:
                    break;

                /* 
                 * When a server is in rs_c_state_renaming 
                 * it holds the global write lock
                 * but it's a quick operation, so let
                 * a stop wait on the lock for the
                 * renaming operation to finish.
                 */
                case rs_c_state_renaming:
                    if (lock_taken) {
                        SET_STATUS(st, sec_s_pgmerr);
                    }
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }

            break;

	  case rs_c_event_i_am_master:
            if (rs_state_master()) {
                SET_STATUS(st, sec_rgy_rep_master);
                break;
            }

            switch(rs_state.rep_state) {
                case rs_c_state_uninitialized:
                case rs_c_state_initializing:
                case rs_c_state_in_service:
                    break;

                /* 
                 * When a server is in rs_c_state_renaming 
                 * it holds the global write lock
                 * but it's a quick operation,  so let a
                 * slave write wait on the lock for the
                 * renaming operation to finish
                 */
                case rs_c_state_renaming:
                    if (lock_taken) {
                        SET_STATUS(st, sec_s_pgmerr);
                    }
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

	  case rs_c_event_no_longer_master :
            if (!rs_state_master()) {
                SET_STATUS(st, sec_rgy_rep_not_master);
                break;
            }

            switch(rs_state.rep_state) {
                case rs_c_state_in_service:
                case rs_c_state_copying_dbase:
                case rs_c_state_in_maintenance:
                    break;

                default:
                    SET_STATUS(st, sec_rgy_rep_bad_state);
                    break;
            }
            break;

        default:
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs,
                           svc_c_sev_error,
                           sec_s_pgmerr);
            SET_STATUS(st, sec_s_pgmerr);
            break;
    }
}

