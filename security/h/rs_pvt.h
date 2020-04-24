/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_pvt.h,v $
 * Revision 1.1.14.1  1996/05/10  13:15:46  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:45 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/03/14  18:56 UTC  hanfei
 * 	increment rs_c_state_file_version for DCE 1.2.2 .
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:10 UTC  psn
 * 	Add  still more master_info_file_version constants.
 * 	[1995/04/22  21:13 UTC  greg  /main/HPDCE02/SEC_migrate_merge/3]
 *
 * 	Add master_info_file_version constants.
 * 	[1995/04/22  20:05 UTC  greg  /main/HPDCE02/SEC_migrate_merge/2]
 *
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:34 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 *
 * 	CHFts14111 bug fix
 * 	[1995/03/27  19:31 UTC  cuti  /main/HPDCE02/1]
 *
 * 	Add RS_SIGCATCH_THR
 * 	[1995/03/23  18:45 UTC  cuti  /main/cuti_mothra_bug/1]
 *
 * Revision 1.1.8.5  1994/08/25  16:29:53  hanfei
 * 	dce1.1 rgy database backward compatibility work
 * 	[1994/08/25  14:46:04  hanfei]
 * 
 * Revision 1.1.8.4  1994/08/04  16:12:46  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:09:29  mdf]
 * 
 * Revision 1.1.8.3  1994/06/02  21:06:46  mdf
 * 	Merged with changes from 1.1.8.2
 * 	[1994/06/02  21:06:30  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:05  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 
 * Revision 1.1.8.2  1994/06/02  20:21:50  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:05  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 
 * Revision 1.1.6.1  1993/10/13  20:37:40  mccann
 * 	CR8651 64bit porting
 * 	[1993/10/13  20:37:11  mccann]
 * 
 * Revision 1.1.2.7  1993/02/26  23:47:47  emartin
 * 	em_bl6f: new fields in rs_state_t - next_rep_state,
 * 	 global_compat_software_vers
 * 	[1993/02/22  22:52:17  emartin]
 * 
 * Revision 1.1.2.6  1993/01/26  23:00:22  emartin
 * 	extern boolean32 rs_bypass_auth
 * 	[1993/01/13  16:25:26  emartin]
 * 
 * Revision 1.1.2.5  1992/12/29  13:24:54  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:00  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/18  22:10:37  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:29  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:10:49  emartin
 * 	bl6c: rs_c_event_primitive_read,
 * 	rs_c_event_copy_dbase_to/from_me,
 * 	rs_c_state_compat_software_version
 * 
 * Revision 1.1.2.3  1992/10/07  20:26:47  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:01  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:39:29  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.2.2  1992/08/31  18:18:34  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  14:57:05  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:42:07  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Internal RS state and master information
 *
 */

#ifndef rs_pvt_h__included
#define rs_pvt_h__included

#include <dce/uuid.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>

/*
* Macros
*/

#ifndef NULL
#   define NULL 0L
#endif

/*
* rs_state info (saved in rgy_state file)
*/

#define rs_c_state_file_version         9 

#define rs_c_master_info_file_version_dce103	1
#define rs_c_master_info_file_version_dce11	2

/*
 * The "plain" master_info_file_version constant
 * should equal to the latest version.  The earlier
 * defined versions are needed so the database init code
 * can sense an earlier version of the master info  file
 * and convert it.
 */
#define rs_c_master_info_file_version  rs_c_master_info_file_version_dce11
 
#define rs_c_align_size         sizeof(long)

/*
 * rs_state_t information is managed by the rsdb/rsdb.c rs_state
 * operations. They assume that a disk write of the rs_state_t
 * structure is atomic; therefore the rs_state_t struct MUST
 * fit into one disk block.
 */
typedef struct rs_state_t {
    unsigned32      version;
    unsigned32      rep_state;          /* Replica state (in rplbase.idl) */
    unsigned32      next_rep_state;     /* Replica state after rename */
    uuid_t          cell_sec_id;        /* Identity of the cell security service */
    uuid_t          rep_id;             /* Identity of this server */
    uuid_t          init_id;            /* initialization event id */
    unsigned32      unused;             /* formerly global_compat_sw_vers */

    /* 
     * The following fields are re-created at startup time.
     */
    boolean32       master;             /* true if this is the master */
}   rs_state_t;


typedef struct rs_master_info_t {
    unsigned32      version;
    boolean32       master_known;       /* true if identity of master is known */
    uuid_t          master_id;          /* Identity of master replica */
    rs_update_seqno_t   
                    master_seqno;       /* Seqno when this master became master */
    unsigned32      global_sw_rev;      /* cell-wide software rev (added at 1.1) */
} rs_master_info_t;


typedef enum {
    rs_c_event_read, 
    rs_c_event_primitive_read, 
    rs_c_event_master_write, 
    rs_c_event_slave_write,
    rs_c_event_slave_initialize, 
    rs_c_event_propq_write, 
    rs_c_event_checkpt_dbase, 
    rs_c_event_save_dbase_to_disk, 
    rs_c_event_propagate,
    rs_c_event_copy_dbase_from_me, 
    rs_c_event_copy_dbase_to_me, 
    rs_c_event_mkey_change, 
    rs_c_event_into_maintenance,
    rs_c_event_out_of_maintenance,
    rs_c_event_become_master, 
    rs_c_event_become_slave, 
    rs_c_event_delete,
    rs_c_event_stop_til_compat_sw,
    rs_c_event_i_am_master,
    rs_c_event_change_master,
    rs_c_event_copy_propq,
    rs_c_event_no_longer_master,
    rs_c_event_set_sw_rev
} rs_state_event_t;

/*
* Global Variables
*/

extern rs_state_t           rs_state;   /* Defined in rs_state.c */

extern boolean32            rs_verbose;     /* Defined and init'd in rs_main.c */

#ifdef RS_REP_NAUTH
extern boolean32            rs_bypass_auth; /* Defined and init'd in rs_main.c */
#endif

/*  Define names for all threads started in security server.
 *  These are used for logging SVC messages.
 */

#define RS_NSI_REGISTER_THR          "NSI register"
#define RS_KDC_THR                   "KDC "
#define RS_LOGIN_THR                 "context refresh"
#define RS_LISTENER_THR              "listner" 
#define RS_OBSOLETE_MASTER_THR       "obsolete master"
#define RS_COMMUNICADO_THR           "prop_communicado"
#define RS_SHORT_INCOMMUNICADO_THR   "prop_short_incommunicado"
#define RS_LONG_INCOMMUNICADO_THR    "prop_long_incommunicado"
#define RS_INIT_SLAVE_THR            "init slave"
#define RS_CHECKPT_THR               "checkpoint"
#define RS_SIGCATCH_THR              "signal catch"
#endif
