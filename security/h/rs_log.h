/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log.h,v $
 * Revision 1.1.17.1  1996/10/03  14:50:47  arvind
 * 	Add rs_log_propq_clean().
 * 	[1996/01/03  16:45 UTC  jrr  /main/HPDCE02/jrr_CHFts16735/1]
 *
 * Revision 1.1.14.3  1996/02/18  22:58:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:31  marty]
 * 
 * Revision 1.1.14.2  1995/12/13  16:25:11  root
 * 	Submit
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:00 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:33 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Restore sw_version logging changes lost in login activity merge.
 * 
 * 	[1995/04/04  17:18 UTC  greg  /main/SEC_migrate/3]
 * 
 * 	additions for logging of journal activity records
 * 	[1995/04/03  22:11 UTC  hondo  /main/SEC_migrate/2]
 * 
 * 	add RS_LOG_MODULE_JOURNAL
 * 	[1995/03/28  19:31 UTC  hondo  /main/hondo_mig1/1]
 * 
 * 	Add software version log module  processing.
 * 	[1995/04/03  18:39 UTC  greg  /main/SEC_migrate/1]
 * 
 * Revision 1.1.10.1  1994/10/26  20:00:39  hondo
 * 	changes for propagating login activity.
 * 	[1994/10/24  20:27:11  hondo]
 * 
 * Revision 1.1.8.3  1994/08/08  14:23:29  mdf
 * 	Put back 64 bit porting changes
 * 	[1994/08/08  12:49:48  mdf]
 * 
 * Revision 1.1.8.2  1994/07/15  14:59:17  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:16:28  mdf]
 * 
 * Revision 1.1.8.1  1994/05/11  19:05:38  ahop
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  20:48:45  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 
 * 	hp_sec_to_osf_2 drop
 * 
 * Revision 1.1.4.3  1993/12/16  18:44:31  hanfei
 * 	rs_log_update_base_seqno => rs_log_write_base_prop_seqno
 * 	[1993/12/11  18:51:53  hanfei]
 * 
 * Revision 1.1.4.2  1993/10/29  17:39:01  hanfei
 * 	add rs_log_update_base_seqno() header
 * 	[1993/10/29  17:38:25  hanfei]
 * 
 * 	 Second replication code drop: bl5
 * 	[1992/10/06  15:34:59  burati]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:18  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.6  1993/02/19  17:11:33  ahop
 * 	dbcp: add rs_log_replay_entry
 * 	[1993/02/19  08:39:29  ahop]
 * 
 * Revision 1.1.2.5  1992/12/29  13:07:56  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:16  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/18  22:10:15  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:45:47  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:06:40  emartin
 * 	bl6c: set_update_info, get_propq_info, propq_get_next,
 * 	propq_check_update_seqno
 * 
 * Revision 1.1.2.3  1992/10/07  20:25:47  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:11:20  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/31  18:17:47  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  14:57:01  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:42:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* 
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - Log file management
 */

#ifndef rs_log_h__included
#define rs_log_h__included


#include <dce/lbase.h>
#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>

#include <rs_base.h>

/*
 * Known Modules: add new modules here
 */

#define RS_LOG_MODULE_NONE      (unsigned char) 0xff
#define RS_LOG_MODULE_ACCT      0
#define RS_LOG_MODULE_PGO       1
#define RS_LOG_MODULE_PROPS     2
#define RS_LOG_MODULE_ACL       3
#define RS_LOG_MODULE_REPLIST   4
#define RS_LOG_MODULE_ATTR_SCH  5
#define RS_LOG_MODULE_ATTR      6
#define RS_LOG_MODULE_LOGIN     7   
#define RS_LOG_MODULE_LOGINR    8
#define RS_LOG_MODULE_SW_VERS   9
#define RS_LOG_MODULE_JOURNAL   10
#define RS_LOG_MODULE_LAST      11   /* This is the last known module */


#ifdef RS_LOG_ALLOCATE_GLOBALS
static const char * rs_log_module_names[RS_LOG_MODULE_LAST] =
    { "Account", "PGO", "Policy", "ACL", "Replist",
      "Attribute Schema", "Attribute", "Login Activity", "Login Reset",
      "Software Version", "Journal"};
#endif

/*
 * Data types and constants
 */

/* In 1.0.1 this structure was a mulitple of both 4 and 8 bytes.  In
 * 1.0.2 this is only a multiple of 4 bytes, so 64 bit machines
 * have alignment problems during conversions of 1.0.1 databases to 1.0.2
 * Since the definitions of all log structures are spread throughout the
 * code, fixing the rsdb_compat code gets messy so the easiest fix is to 
 * add the padding to the header.
 */
typedef struct  rs_log_hdr_t {
    unsigned32          upd_reclen;
    unsigned16          upd_module;
    unsigned16          upd_op;
    rs_update_seqno_t   upd_seqno;
    sec_timeval_t       upd_ts;
#ifdef Bitsize64
    unsigned32          dummy;
#endif
}   rs_log_hdr_t;

typedef struct rs_log_rec_t {
    rs_log_hdr_t        upd_hdr;
    unsigned_char_t     upd_args[VARYING];
}   rs_log_rec_t;

typedef void (*rs_log_prop_function_t) (
#ifdef __STDC__
    handle_t                   handle,
    rs_log_rec_t               *rec,
    rs_replica_master_info_t   *master_info,
    boolean32                  propq_only,
    error_status_t             *st
#endif
);

typedef void (*rs_log_replay_function_t) (
#ifdef __STDC__
    rs_log_rec_t    *rec,
    error_status_t  *st
#endif
);

#ifdef __STDC__

#   define PROP_FUNCTION(name) void name (\
    handle_t                   handle, \
    rs_log_rec_t               *rec, \
    rs_replica_master_info_t   *master_info, \
    boolean32                  propq_only, \
    error_status_t             *st )

#   define REPLAY_FUNCTION(name) void name (\
    rs_log_rec_t   *rec, \
    error_status_t *st )

#else

#   define PROP_FUNCTION(name) void name ()
#   define REPLAY_FUNCTION(name) void name ()

#endif

/* Update Types */
#define GLOBAL_UPDATE   1
#define LOCAL_UPDATE    2

typedef struct rs_log_ops {
    unsigned_char_t             update_type;
    rs_log_prop_function_t      propagate;
    rs_log_replay_function_t    replay;
    char                        *printstring;
} rs_log_ops_t;

/*
* Public entry points
*/

PUBLIC void rs_log_initialize (
#ifdef __STDC__
    error_status_t   *status
#endif
);

PUBLIC void rs_log_init_module (
#ifdef __STDC__
    unsigned32      module_code,
    unsigned32      num_ops,
    rs_log_ops_t    *ops,
    error_status_t  *st
#endif
);

/*
 * Set last update seqno and timestamp.
 * Called by a replica that has just been
 * (re-)initialized.
 *
 * Caller should have the global write lock
 */
PUBLIC void rs_log_set_update_info (
#ifdef __STDC__
    rs_update_seqno_t   *last_update_seqno,
    sec_timeval_t       *last_update_ts,
    error_status_t      *st
#endif
);

PUBLIC void rs_log_get_update_info (
#ifdef __STDC__
    rs_update_seqno_t   *last_update_seqno,
    sec_timeval_t       *last_update_ts,
    error_status_t      *st
#endif
);

PUBLIC void rs_log_write_base_prop_seqno (
#ifdef __STDC__
    rs_update_seqno_t   base_prop_seqno,
    error_status_t      *st
#endif
);

PUBLIC void rs_log_create (
#ifdef __STDC__
    rs_update_seqno_t   *update_seqno,
    error_status_t      *st
#endif
);

PUBLIC void rs_log_setup_log_header (
#ifdef __STDC__
    rs_log_hdr_t               *hdr,
    unsigned32                 reclen,
    unsigned32                 module,
    unsigned32                 op
#endif
);

PUBLIC void rs_log_write (
#ifdef __STDC__
    rs_log_hdr_t        *bp,
    rs_update_seqno_t   *update_seqno,
    sec_timeval_t       *update_ts
#endif
);

PUBLIC void rs_log_replay_entry (
#ifdef __STDC__
    rs_log_hdr_t     *entryp,
    error_status_t   *st
#endif
);

PUBLIC void rs_log_replay (
#ifdef __STDC__
    error_status_t   *st
#endif
);

PUBLIC void rs_log_truncate (
#ifdef __STDC__
    char            *postfix,
    error_status_t  *st
#endif
);

PUBLIC void rs_log_get_propq_info (
#ifdef __STDC__ 
    rs_update_seqno_t   *base_prop_seqno,
    rs_update_seqno_t   *last_update_seqno,
    error_status_t      *st
#endif 
);

PUBLIC void rs_log_propq_get_next (
#ifdef __STDC__
    rs_update_seqno_t   *upd_seqno,
    unsigned_char_p_t   *bp,
    unsigned32          *blen,
    error_status_t      *st
#endif
);

PUBLIC void rs_log_propq_check_update_seqno (
#ifdef __STDC__
    rs_replica_info_t   *info,
    error_status_t      *st
#endif
);

PUBLIC void rs_log_propq_clean (
#ifdef __STDC__ 
    error_status_t              *st_p
#endif
);

PUBLIC boolean32 rs_log_nothing_to_propagate (
#ifdef __STDC__
    rs_replica_prop_info_p_t    prop_info
#endif
);

PUBLIC void rs_log_propagate (
#ifdef __STDC__
    handle_t                   handle,
    unsigned_char_p_t          bp,
    rs_replica_master_info_t   *master_info,
    boolean32                  propq_only,
    error_status_t             *st
#endif
);

PUBLIC PROP_FUNCTION(rs_log_prop_nop);

#endif /* rs_log_h__included */

