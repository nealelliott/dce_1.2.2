/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_state.h,v $
 * Revision 1.1.9.3  1996/02/18  22:59:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:16  marty]
 *
 * Revision 1.1.9.2  1995/12/08  17:27:43  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:02 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:34 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Merge .../greg_sec_migrate/LATEST --> .../SEC_migrate/LATEST.
 * 	[1995/02/11  22:13 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	rs_state_ set_global_sw_vers() is obsolete.
 * 
 * 	[1995/02/06  08:23 UTC  greg  /main/greg_sec_migrate/1]
 * 
 * Revision 1.1.5.6  1994/08/25  16:30:04  hanfei
 * 	dce1.1 rgy database backward compatibility work
 * 	[1994/08/25  14:46:28  hanfei]
 * 
 * Revision 1.1.5.5  1994/06/02  21:40:42  mdf
 * 	Merged with changes from 1.1.5.4
 * 	[1994/06/02  21:40:24  mdf]
 * 
 * 	Merged with changes from 1.1.5.3
 * 	[1994/06/02  21:35:29  mdf]
 * 
 * 	Merged with changes from 1.1.5.2
 * 	[1994/06/02  21:14:39  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:19  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	decl rs_state_current_state
 * 	[1993/12/10  14:03:31  hanfei]
 * 
 * 	em_bl6f: decl rs_state_set_state_renaming
 * 	[1993/02/22  22:50:46  emartin]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:51  zeliff]
 * 
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:39  sekhar]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:56  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.1.2  1992/10/06  15:42:29  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.5.4  1994/06/02  21:35:53  mdf
 * 	Merged with changes from 1.1.5.3
 * 	[1994/06/02  21:35:29  mdf]
 * 
 * 	Merged with changes from 1.1.5.2
 * 	[1994/06/02  21:14:39  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:19  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	decl rs_state_current_state
 * 	[1993/12/10  14:03:31  hanfei]
 * 
 * 	em_bl6f: decl rs_state_set_state_renaming
 * 	[1993/02/22  22:50:46  emartin]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:51  zeliff]
 * 
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:39  sekhar]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:56  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.1.2  1992/10/06  15:42:29  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.5.3  1994/06/02  21:14:53  mdf
 * 	Merged with changes from 1.1.5.2
 * 	[1994/06/02  21:14:39  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:19  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	decl rs_state_current_state
 * 	[1993/12/10  14:03:31  hanfei]
 * 
 * 	em_bl6f: decl rs_state_set_state_renaming
 * 	[1993/02/22  22:50:46  emartin]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:51  zeliff]
 * 
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:39  sekhar]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:56  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.1.2  1992/10/06  15:42:29  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.5.2  1994/06/02  20:22:04  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:19  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	decl rs_state_current_state
 * 	[1993/12/10  14:03:31  hanfei]
 * 
 * 	em_bl6f: decl rs_state_set_state_renaming
 * 	[1993/02/22  22:50:46  emartin]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:51  zeliff]
 * 
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:39  sekhar]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:56  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.1.2  1992/10/06  15:42:29  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.3.4  1993/02/26  23:48:20  emartin
 * 	em_bl6f: decl rs_state_set_state_renaming
 * 	[1993/02/22  22:50:46  emartin]
 * 
 * Revision 1.1.3.3  1992/12/29  13:25:33  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:51  zeliff]
 * 
 * Revision 1.1.3.2  1992/10/07  20:27:57  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:39  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:42:29  burati
 * 	 Second replication code drop: bl5
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


#ifndef rs_state_h__included
#define rs_state_h__included


#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>

#include <macros.h>
#include <rs_pvt.h>

/*  Want to create the rgy_state file.
 *  return an error if it already exists.
 */
void rs_state_create_chk (
    error_status_t      *st
);

/*
 *  Creating rgy_state and rgy database.
 *  Fill rs_state in memory but don't save it 
 *  to disk
 */
void rs_state_create_volatile (
    unsigned32          rep_state,
    uuid_p_t            cell_sec_id,
    uuid_p_t            rep_id,
    boolean32           masterf
);

/*  Create rgy_state file and save the current
 *  rs_state info in it.
 */
void rs_state_create (
    error_status_t      *st
);

/*  
 *  Open and lock the rgy_state file.
 *  Read its info into rs_state.
 */
void rs_state_init (
    error_status_t      *st
);

/*
 *  Save replica state
 */
void rs_state_set_state (
    unsigned32          rep_state,
    error_status_t      *st
);

/*
 *  Save replica state as renaming
 *  and save the next_rep_state which
 *  is to be assumed after renaming is 
 *  done.
 */
void rs_state_set_state_renaming (
    unsigned32          next_rep_state,
    error_status_t      *st
);

/*
 *  Set replica state in memory only
 *  For initializing, copying_dbase, becoming_master, 
 *  closed
 */
void rs_state_set_state_volatile (
    unsigned32          rep_state,
    error_status_t      *st
);

/*
 *  Save replica's initialization id
 */
void rs_state_set_init_id (
    uuid_p_t            init_id,
    error_status_t      *st
);

/*
 *  Get replica's initialization id
 */
void rs_state_get_init_id (
    uuid_t              *init_id,
    error_status_t      *st
);

/*
 * Save the flag which indicates whether or not
 * this replica is the master
 */
void rs_state_set_master_flag (
    boolean32           masterf,
    error_status_t      *st
);

/*
 *  Return true if this replica is the master
 *  replica
 */
boolean32 rs_state_master (
    void
);

unsigned32 rs_state_get_version (
    void
);

/*
 *  Return true if this server is exiting
 */
boolean32 rs_state_exiting (
    void
);

/*
 *  Return true if the replica's state is "state"
 */
boolean32 rs_state_current_state (
    unsigned32          state
);

/*
 *  Check whether the replica is in a state
 *  which is ok for this event.  Return
 *  bad status if it isn't.
 */
void rs_state_check (
    rs_state_event_t    event,
    boolean32           lock_taken,
    error_status_t      *st
);

#endif

