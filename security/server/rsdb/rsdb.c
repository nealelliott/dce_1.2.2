/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY 
 * $Log: rsdb.c,v $
 * Revision 1.1.17.2  1996/10/03  15:12:08  arvind
 * 	CHFts19896: public key version support
 * 	[1996/09/13  23:43 UTC  aha  /main/DCE_1.2.2/aha_pk9_3/1]
 *
 * 	Include rsdb_acct.h
 * 	[1996/09/06  18:19 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/2]
 *
 * 	Merge up from aha_pk8_1
 * 	[1996/08/29  15:41 UTC  aha  /main/DCE_1.2.2/aha_pk9_2/1]
 *
 * 	Merge up from aha_pk8
 * 	[1996/08/27  22:25 UTC  aha  /main/DCE_1.2.2/aha_pk8_1/1]
 *
 * 	Register pvtkey interfaces with sec_psm
 * 	[1996/08/02  13:02 UTC  aha  /main/DCE_1.2.2/aha_pk8/1]
 *
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:47 UTC  hanfei  /main/DCE_1.2/2]
 *
 * 	add rsdb_foreign_person_create_init() for database conversion.
 * 	[1996/03/20  15:59 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/3]
 *
 * 	cleanup
 * 	[1996/02/20  16:43 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/2]
 *
 * 	multi-cell group work: add database file foregin_principal and domain
 * 	foregin_person.
 * 	[1996/02/01  19:55 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:40  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/11/07  20:59 UTC  dat  /main/HPDCE02/jrr_1.2_mothra/1]
 *
 * 	Add missing "}" from not DCE_SEC_POSIX_F_LOCK condition in rsdb_state_close
 * 	HP 	[1995/06/09  10:42 UTC  sekhar  /main/HPDCE02/3]
 *
 * 	Merge fixes to mothra.
 * 	HP 	[1995/06/01  12:31 UTC  sekhar  /main/HPDCE02/sekhar_mothra_fix/1]
 *
 * 	[ CHFts12106 ] Replaced SVC code sec_sys_errno_text with sec_sys_errno_file_text.
 * 	HP 	[1995/05/30  15:39 UTC  hanfei  /main/HPDCE02/2]
 *
 * 	[OT 12872] merge fix.
 * 	HP 	[1995/05/25  15:40 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/1]
 *
 * 	[OT 12872] add file unlock in rsdb_state_close().
 * 	HP 	[1995/05/15  21:10 UTC  greg  /main/HPDCE02/1]
 * 	[1996/10/03  13:54:36  arvind]
 *
 * Revision 1.1.17.1  1996/05/10  13:24:05  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:47 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/3  1996/03/20  15:59 UTC  hanfei
 * 	add rsdb_foreign_person_create_init() for database conversion.
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/02/20  16:43 UTC  hanfei
 * 	cleanup
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/01  19:55 UTC  hanfei
 * 	multi-cell group work: add database file foregin_principal and domain
 * 	foregin_person.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:18 UTC  psn
 * 	Add missing "}" from not DCE_SEC_POSIX_F_LOCK condition in rsdb_state_close
 * 	[1995/11/07  20:59 UTC  dat  /main/HPDCE02/jrr_1.2_mothra/1]
 * 
 * 	Merge fixes to mothra.
 * 	[1995/06/09  10:42 UTC  sekhar  /main/HPDCE02/3]
 * 
 * 	[ CHFts12106 ] Replaced SVC code sec_sys_errno_text with sec_sys_errno_file_text.
 * 	[1995/06/01  12:31 UTC  sekhar  /main/HPDCE02/sekhar_mothra_fix/1]
 * 
 * 	[OT 12872] merge fix.
 * 	[1995/05/30  15:39 UTC  hanfei  /main/HPDCE02/2]
 * 
 * 	[OT 12872] add file unlock in rsdb_state_close().
 * 	[1995/05/25  15:40 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/1]
 * 
 * 	Ceate SEC_migrate_merge mainline from SEC_migrate and HPDCE02
 * 	[1995/04/21  12:57 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	merge
 * 	[1995/02/21  20:19 UTC  hanfei  /main/SEC_migrate/2]
 * 
 * 	invoke master_info compatibility check in rsdb_master_info_open().
 * 	[1995/02/21  20:05 UTC  hanfei  /main/SEC_migrate/hanfei_migrate_bl1/1]
 * 
 * 	merge migration codes
 * 	[1995/02/07  14:53 UTC  hanfei  /main/SEC_migrate/1]
 * 
 * 	work for rsdb replica compatibility to dce1.1
 * 	[1995/02/06  06:33 UTC  hanfei  /main/hanfei_comp_bl2/2]
 * 
 * 	compatiblity work for master_info.
 * 	[1995/02/03  04:24 UTC  hanfei  /main/hanfei_comp_bl2/1]
 * 
 * Revision 1.1.9.5  1994/08/25  16:30:33  hanfei
 * 	dce1.1 rgy database backward compatibility work
 * 	[1994/08/25  14:40:29  hanfei]
 * 
 * Revision 1.1.9.4  1994/08/19  17:59:35  mdf
 * 	1994/08/18  sekhar
 * 	converted rs_verbose calls to SVC calls.
 * 	[1994/08/19  13:11:15  mdf]
 * 
 * Revision 1.1.9.3  1994/08/04  16:14:12  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:12:50  mdf]
 * 
 * Revision 1.1.9.1  1994/03/02  17:55:56  hanfei
 * 	Initial security drop
 * 	[1994/02/18  20:52:19  hanfei]
 * 
 * 	HP revision /main/ODESSA_ERA2/1  1994/01/28  20:10  mdf
 * 	Merge forward.
 * 
 * 	HP revision /main/ODESSA_ERA1/4  1994/01/26  23:31  ahop
 * 	 merge forward
 * 
 * 	HP revision /main/ODESSA_ERA1/ahop_odss_era_bl3/1  1994/01/26  23:29  ahop
 * 	remove dup call to rsdb_attr_sch_init
 * 
 * 	HP revision /main/ODESSA_ERA1/3  1994/01/22  02:21  mob
 * 	 Merge forward.
 * 
 * 	HP revision /main/ODESSA_ERA1/mob_era_bl7/2  1994/01/22  02:19  mob
 * 	Fixed build errors
 * 
 * 	HP revision /main/ODESSA_ERA1/mob_era_bl7/1  1994/01/22  00:06  mob
 * 	change rsdb_attr_schema_init to rsdb_attr_sch_init
 * 
 * Revision 1.1.3.8  1993/02/26  23:55:44  emartin
 * 	em_bl6f: rsdb_clear_mem - call rsdb_pgo_clear
 * 	[1993/02/22  22:54:46  emartin]
 * 
 * Revision 1.1.3.7  1993/02/19  16:26:46  ahop
 * 	dbcp: store version in master_key file; close state file on create/open error.
 * 	Don't use RGY_FILE_DIRECTORY in rsdb_delete_rgy_files.  Remove unix dir creation.
 * 	Add rsdb_load_relations, rsdb_clear_database.
 * 	[1993/02/19  09:12:31  ahop]
 * 
 * Revision 1.1.3.6  1992/12/29  16:38:11  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:27:45  zeliff]
 * 
 * Revision 1.1.3.5  1992/11/20  15:53:51  ahop
 * 	Move init_database here from rsdb_create.c.  Uncomment clear_mem.
 * 	Add rsdb_mkey file i/o operations.
 * 	[1992/11/10  23:12:42  ahop]
 * 
 * Revision 1.1.3.4  1992/10/07  20:37:49  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:20:03  sekhar]
 * 
 * Revision 1.1.4.2  1992/10/06  16:02:09  burati
 * 	Second replication drop: bl5
 * 
 * Revision 1.1.3.3  1992/09/29  20:13:34  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:39:13  sekhar]
 * 
 * Revision 1.1.3.2  1992/08/31  18:27:51  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:12:20  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:44  devrcs
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
 *      Registry Server - database interlude
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
	"$Id: rsdb.c,v 1.1.17.2 1996/10/03 15:12:08 arvind Exp $";
#endif

#define RSDB_MAIN

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <errno.h>
#include <un_stat.h>
#include <un_strng.h>
#include <un_io.h>
#include <un_time.h>

#include <dce/dce.h>
#include <sec_svc.h>

#include <balanced_trees.h>

#include <dce/rpc.h>

#include <rsdb_pvt.h>
#include <rsdb.h>
#include <rsdb_create.h>
#include <rsdb_file.h>
#include <rsdb_replica.h>
#include <rsdb_sysdep.h>
#include <rsdb_log.h>
#include <rsdb_compat.h>
#include <rsdb_attr_schema.h>
#include <rs_pvt.h>
#include <rs_base.h>
#include <rs_util.h>
#include <rs_ver.h> 
#include <rs_reserved.h>
#include <rs_rep_util.h>
#include <rsdb_acct.h>
#include <dce/idlbase.h>

typedef struct rsdb_db {
    tree_handle tree;
    boolean32   dirty;
} rsdb_db;

/*
 * Private variables
 */

PRIVATE rsdb_db account_data;
PRIVATE rsdb_db person_data;
PRIVATE rsdb_db group_data;
PRIVATE rsdb_db org_data;
PRIVATE rsdb_db replica_data;
PRIVATE rsdb_db log_data;
PRIVATE rsdb_db acl_data;
PRIVATE rsdb_db attr_data;
PRIVATE rsdb_db attr_schema_data;
PRIVATE rsdb_db login_data;
PRIVATE rsdb_db journal;
PRIVATE rsdb_db foreign_person_data;

/* 
 * Files which are written when modified and
 * are not included in the "checkpoint"
 * protocol
 */
PRIVATE int        rsdb_rgy_state_fd = -1;
PRIVATE int        rsdb_master_info_fd = -1;
PRIVATE int        rsdb_mkey_fd = -1;

/* 
 * Files which are periodically written to
 * disk during a checkpoint.  Updates to
 * these files are saved in the log.  The
 * log is replayed at startup time and
 * truncated after a checkpoint (when
 * the current state of these files is
 * saved to disk).
 */
PUBLIC int         rsdb_rgy_fd = -1;
PUBLIC db_handle   rsdb_acct_db;
PUBLIC db_handle   rsdb_person_db;
PUBLIC db_handle   rsdb_group_db;
PUBLIC db_handle   rsdb_org_db;
PUBLIC db_handle   rsdb_replica_db;
PUBLIC db_handle   rsdb_acl_db;
PUBLIC db_handle   rsdb_attr_db;
PUBLIC db_handle   rsdb_attr_schema_db;
PUBLIC db_handle   rsdb_login_db;
PUBLIC db_handle   rsdb_journal_db;
PUBLIC db_handle   rsdb_foreign_person_db;

/*
 * The log file.  Updates are saved to this file
 * and the log is truncated after "checkpoint"
 * when the current state of the other files is
 * written to disk.
 * The log is also used to save the master's
 * propagation queue.  When the master's log is
 * truncated after a checkpoint, it is only
 * truncated back to the last update that was
 * delivered to all replicas.
 */
PUBLIC db_handle   rsdb_log_db;

PUBLIC domain_info_t domain_info[sec_rgy_domain_invalid];

/*
 * File creation protection bits
 */
#define UNIX_FILE_PROTECTION 0644
#define FILE_CREATION_PROTECTION 0600
#define DIR_CREATION_PROTECTION 0755

PRIVATE struct relation_info {
    rsdb_db     *db;
    char        *fname;
    long        protection;
    boolean32   needs_to_be_saved;
    boolean32   set_protection;
    boolean32   do_copy;
    boolean32   padding;
} relation_info [] = {
    { NULL,             RGY_FILE,           FILE_CREATION_PROTECTION,   false,  true,   true,   false, },
    { &account_data,    ACCT_FILE,          FILE_CREATION_PROTECTION,   true,   true,   true,   false, },
    { &person_data,     PERSON_FILE,        FILE_CREATION_PROTECTION,   true,   true,   true,   false, },
    { &group_data,      GROUP_FILE,         FILE_CREATION_PROTECTION,   true,   true,   true,   false, },
    { &org_data,        ORG_FILE,           FILE_CREATION_PROTECTION,   true,   true,   true,   false, },
    { &replica_data,    REPLICA_FILE,       FILE_CREATION_PROTECTION,   true,   true,   true,   false, },
    { &log_data,        NULL,               FILE_CREATION_PROTECTION,   false,  false,  false,  false, },

/*
    { NULL,             UNIX_PASSWD_FILE,   UNIX_FILE_PROTECTION,       true,   true,   false,  false, },
    { NULL,             UNIX_GROUP_FILE,    UNIX_FILE_PROTECTION,       true,   true,   false,  false, },
    { NULL,             UNIX_ORG_FILE,      UNIX_FILE_PROTECTION,       true,   true,   false,  false, },
*/

    { &acl_data,        ACL_FILE,           FILE_CREATION_PROTECTION,   true,   true,   true,   false, },
    { &attr_data,	ATTR_FILE,	    FILE_CREATION_PROTECTION,   true,   true,   true,   false, },
    { &attr_schema_data,	SCHEMA_FILE,	    FILE_CREATION_PROTECTION,   true,   true,   true,   false, },
    { &login_data,      LOGIN_FILE,         FILE_CREATION_PROTECTION,   true,   true,   true,   false, },
    { &journal,         JOURNAL_FILE,       FILE_CREATION_PROTECTION,   true,   true,   true,   false, },
    { &foreign_person_data, FOREIGN_PERSON_FILE, FILE_CREATION_PROTECTION,   true,   true,   true,   false, },

};

#define NUM_RELATIONS (sizeof(relation_info) / sizeof(relation_info[0]))

/* Private routines
 */

PUBLIC void rsdb_create
#ifndef __STDC__
    ( rep_name, rgy_creator, rgy_local_cell, default_properties, st)
    unsigned_char_p_t       rep_name;
    sec_id_t                *rgy_creator;
    sec_id_t                *rgy_local_cell;
    sec_rgy_properties_t    *default_properties;
    error_status_t          *st;
#else
  (
    unsigned_char_p_t       rep_name,
    sec_id_t                *rgy_creator,
    sec_id_t                *rgy_local_cell,
    sec_rgy_properties_t    *default_properties,
    error_status_t          *st
  )
#endif
{
    int fd;
    int i;

    rsdb_clean_dir(st);
    rsdb_create_dirs(st);

    fd = rsdb_file_open(RGY_FILE, O_RDWR|O_CREAT, FILE_CREATION_PROTECTION);
    if (fd == -1) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%s"),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_sys_errno_file_text,
                       RGY_FILE,
                       strerror(errno));
        SET_STATUS(st, sec_rgy_rep_cannot_create_db);
        return;
    }
    rsdb_file_close(fd);

    rsdb_acct_db       = &account_data;
    rsdb_person_db     = &person_data;
    rsdb_group_db      = &group_data;
    rsdb_org_db        = &org_data;
    rsdb_replica_db    = &replica_data;
    rsdb_log_db        = &log_data;
    rsdb_acl_db        = &acl_data;
    rsdb_attr_db 	= &attr_data;
    rsdb_attr_schema_db	= &attr_schema_data;
    rsdb_login_db       = &login_data;
    rsdb_journal_db     = &journal;
    rsdb_foreign_person_db = &foreign_person_data;


    for (i = 0; i < NUM_RELATIONS; i++) {
        if (relation_info[i].db == NULL)
            continue;

        relation_info[i].db->tree   = NULL;
        relation_info[i].db->dirty  = true;
    }

    /* make seq list begin/end anchors for pgo, acct, acl files */
    rsdb_create_database(rgy_creator, rgy_local_cell, 
                         rep_name, default_properties);

    CLEAR_STATUS(st);
}

PUBLIC void rsdb_create_shutdown (
#ifdef __STDC__
    void
#endif
)
{
    rsdb_close_files();
    rsdb_state_close();
}

PUBLIC void rsdb_init_database (
#ifdef __STDC__
    void
#endif
)
{
    rsdb_acl_init();
    rsdb_pgo_init();
    rsdb_acct_init();
    rsdb_attr_sch_init();
    rsdb_login_init();
    rsdb_journal_init();
}


PUBLIC void rsdb_load_relations
#ifndef __STDC__
    ( st )
    error_status_t   *st;
#else
  (
    error_status_t   *st
  )
#endif
{
    int i;

    CLEAR_STATUS(st);

    rsdb_acct_db       = &account_data;
    rsdb_person_db     = &person_data;
    rsdb_group_db      = &group_data;
    rsdb_org_db        = &org_data;
    rsdb_replica_db    = &replica_data;
    rsdb_log_db        = &log_data;
    rsdb_acl_db        = &acl_data;
    rsdb_attr_db       = &attr_data;
    rsdb_attr_schema_db     = &attr_schema_data;
    rsdb_login_db       = &login_data;
    rsdb_journal_db     = &journal;
    rsdb_foreign_person_db = &foreign_person_data;


    for (i = 0; i < NUM_RELATIONS; i++) {
        if (relation_info[i].db == NULL )
            continue;

        relation_info[i].db->dirty  = false;

        if (!relation_info[i].needs_to_be_saved) {
            clear_database(&(relation_info[i].db->tree));
            continue;
        }

        input_database(relation_info[i].fname, &(relation_info[i].db->tree));
    }

    /* get seq list begin/end ptrs for pgo, acct, acl files */
    rsdb_init_database();

}


PUBLIC void rsdb_load
#ifndef __STDC__
    ( st )
    error_status_t   *st;
#else
  (
    error_status_t   *st
  )
#endif
{
    int fd;

    CLEAR_STATUS(st);

    fd = rsdb_file_open(RGY_FILE, O_RDWR, 0);
    if (fd == -1) {
        SET_STATUS(st, sec_rgy_rep_cannot_open_db);
        return;
    }
    rsdb_load_rgy_info(fd);
    rsdb_file_close(fd);

    rsdb_load_relations(st);

    rsdb_acct_register_pvtkey_mechanism();
}

/*
 * Have global write lock if need it (eg. multi-threaded, rpc_listening)
 */

PUBLIC void rsdb_save
#ifndef __STDC__
    ( postfix, st )
    char            *postfix;
    error_status_t  *st;
#else
  (
    char            *postfix,
    error_status_t  *st
  )
#endif
{
    int             fd;
    int             i;
    sec_rgy_pname_t fname,
                    passwd,
                    group,
                    org;
    struct  timeval tv;
    struct  timezone tz;

    switch(rs_state.rep_state) {
        case rs_c_state_unknown_to_master:
            break;
        case rs_c_state_uninitialized:
            break;
        case rs_c_state_initializing:
            break;
        case rs_c_state_in_service:
            break;
        case rs_c_state_mkey_changing:
            break;

        default:
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_rsdb,
                           svc_c_sev_error,
                           sec_rgy_rep_bad_state);
            SET_STATUS(st, sec_rgy_rep_bad_state);
            return;
    }

    dce_svc_printf(SEC_RGY_CHECKPOINT_MSG);
    strcpy( (char *) fname, RGY_FILE);
    
    dce_svc_printf(SEC_RGY_CHKPT_SAVE_FILE_MSG, fname);

    strcat( (char *) fname, postfix);

    fd = rsdb_file_open( (char *)fname, O_RDWR|O_CREAT, FILE_CREATION_PROTECTION);
    if (fd == -1) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%s"),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_sys_errno_file_text,
		       fname,
                       strerror(errno));
    }
    rsdb_store_rgy_info(fd);
    rsdb_file_close(fd);

    for (i = 0; i < NUM_RELATIONS; i++) {
        if (relation_info[i].db == NULL || !relation_info[i].needs_to_be_saved)
            continue;

        if (!relation_info[i].db->dirty)
            continue;

        dce_svc_printf(SEC_RGY_CHKPT_SAVE_RELATION_MSG, 
                       relation_info[i].fname);

        strcpy( (char *) fname, relation_info[i].fname);
        strcat( (char *) fname, postfix);
        save_database( (char *) fname, relation_info[i].db->tree);
        relation_info[i].db->dirty = false;
    }

    dce_svc_printf(SEC_RGY_CHECKPOINT_SUCC_MSG);

    CLEAR_STATUS(st);
}

PUBLIC void rsdb_close_files (
#ifdef __STDC__
    void
#endif
)
{
    rsdb_master_info_close();
    rsdb_log_close();
}

PUBLIC void rsdb_open_files (
#ifdef __STDC__
    void
#endif
)
{
    rs_master_info_t    master_info;
    error_status_t      status;

    rsdb_master_info_open(&master_info, &status);

    rsdb_log_open();
}

PUBLIC void rsdb_clear_database
#ifndef __STDC__
    ( db )
    db_handle       db;
#else
  (
    db_handle       db
  )
#endif
{
    clear_database(&(db->tree));
}

              
PUBLIC void rsdb_clear_mem (
#ifdef __STDC__
    void
#endif
)
{
    int i;

    for (i = 0; i < NUM_RELATIONS; i++) {
        if (relation_info[i].db == NULL )
            continue;

        relation_info[i].db->dirty  = false;

        clear_database(&(relation_info[i].db->tree));
    }

    /* clear domain_info
     */
    rsdb_pgo_clear();
} 

 /*
  * This routine clears out the in-memory log.  This operation needs to be
  * done when a master stops being a master (due to a change_master or
  * become_slave ) operation.   If it is not done, then a fatal error
  * might occur during a subsequent change_master operation: When the
  * server (about to become master again) tries to bring the log entries
  * the current master sent into its memory it can fail.  It can fail
  * because the rs_log_store operation demands that entries made in the
  * in-memory database be unique.    An attempt to 'replace' an existing
  * log entry causes the server to kill itself.   Rather than adjust the
  * rs_log_store code, we simply make sure that no entries exist in the
  * in-memory log once the 'master' is no longer the master..
  *
  */
PUBLIC void rsdb_clear_mem_log(
#ifdef __STDC__
    void
#endif
)
{

    if ( log_data.tree != NULL )  {
        clear_database(&log_data.tree);
        /* XXX What about the 'dirty' flag (log_data.dirty) in the rsdb_db struct ???*/
    }

}


PUBLIC void rsdb_create_dirs
#ifndef __STDC__
    ( st )
    error_status_t  *st;
#else
  (
    error_status_t  *st
  )
#endif
{
    if (rsdb_file_mkdir("", DIR_CREATION_PROTECTION)) {
        CLEAR_STATUS(st);
    } else {
        SET_STATUS(st, sec_rgy_rep_cannot_create_db);
    }
}

PUBLIC void rsdb_loosen_directory_protection (
#ifdef __STDC__
    void
#endif
)
{
    rsdb_file_mkdir("", 0775);
}

PUBLIC void rsdb_tighten_directory_protection (
#ifdef __STDC__
    void
#endif
)
{
    rsdb_file_mkdir("", DIR_CREATION_PROTECTION);
}

 /*
  * rsdb_protect_files set protection bits for files that are copied across
  * nodes.
  */
PUBLIC void rsdb_protect_files
#ifndef __STDC__
    ( secure )
    boolean32  secure;
#else
  (
    boolean32  secure
  )
#endif
{
    int     i;
    long    protection;
    long    mask;

    if (!secure) {
        mask = 0040;        /* Group read access */
    } else {
        mask = 0;
    }

    for (i = 0; i < NUM_RELATIONS; i++) {
        if (relation_info[i].set_protection) {
            protection = relation_info[i].protection | mask;
            rsdb_file_chmod(relation_info[i].fname, protection);
        }
    }
}

PUBLIC void rsdb_clean_dir
#ifndef __STDC__
    ( st )
    error_status_t  *st;
#else
  (
    error_status_t  *st
  )
#endif
{
    rsdb_delete_files("", st);
    rsdb_delete_files(".bak", st);
    rsdb_delete_files(".new", st);
}

/* 
 * Delete files listed in relation_info.
 * Note that rgy_state is not included in relation_info
 */
PUBLIC void rsdb_delete_files
#ifndef __STDC__
    ( postfix, st )
    char            *postfix;
    error_status_t  *st;
#else
  (
    char            *postfix,
    error_status_t  *st
  )
#endif
{
    sec_rgy_pname_t     fname;
    int     i;

    if (strlen(postfix) == 0) {
        for (i = 0; i < NUM_RELATIONS; i++) {
            if (relation_info[i].fname == NULL)
                continue;

            rsdb_file_unlink(relation_info[i].fname);
        }
        rsdb_file_unlink(MASTER_INFO_FILE);
        rsdb_file_unlink(LOG_FILE);
    }

    else {
        for (i = 0; i < NUM_RELATIONS; i++) {
            if (relation_info[i].fname == NULL)
                continue;

            strcpy( (char *) fname, relation_info[i].fname);
            strcat( (char *) fname, postfix);
            rsdb_file_unlink((char *) fname);
        }

        strcpy( (char *) fname, MASTER_INFO_FILE);
        strcat( (char *) fname, postfix);
        rsdb_file_unlink( (char *) fname);

        strcpy( (char *) fname, LOG_FILE);
        strcat( (char *) fname, postfix);
        rsdb_file_unlink( (char *) fname);
    }

    CLEAR_STATUS(st);
}


PUBLIC void rsdb_rename_files
#ifndef __STDC__
    ( fr_postfix, to_postfix, st )
    char            *fr_postfix;
    char            *to_postfix;
    error_status_t  *st;
#else
  (
    char            *fr_postfix,
    char            *to_postfix,
    error_status_t  *st
  )
#endif
{
    int i;

    for (i = 0; i < NUM_RELATIONS; i++) {
        if (relation_info[i].fname == NULL)
            continue;

        rsdb_rename_file(relation_info[i].fname, fr_postfix, to_postfix, st);
        if (BAD_STATUS(st)) return;
    }

    rsdb_rename_file(LOG_FILE, fr_postfix, to_postfix, st);
    if (BAD_STATUS(st)) return;
}

PUBLIC void rsdb_rename_file
#ifndef __STDC__
    ( fname, fr_postfix, to_postfix, st )
    char            *fname;
    char            *fr_postfix;
    char            *to_postfix;
    error_status_t  *st;
#else
  (
    char            *fname,
    char            *fr_postfix,
    char            *to_postfix,
    error_status_t  *st
  )
#endif
{
    sec_rgy_pname_t     fr_fname,
                    to_fname;
    struct stat     f_stat;

    strcpy( (char *) fr_fname, fname);
    strcat( (char *) fr_fname, fr_postfix);
    if (rsdb_file_stat( (char *) fr_fname, &f_stat) == -1) {
        if (errno == ENOENT)
            CLEAR_STATUS(st);
        else
            SET_STATUS(st, sec_rgy_rep_cannot_rename_db);
        return;
    }

    strcpy( (char *) to_fname,  fname);
    strcat( (char *) to_fname, to_postfix);
    rsdb_file_unlink( (char *) to_fname);

    if (rsdb_file_rename( (char *) fr_fname,  (char *) to_fname) == -1) {
        SET_STATUS(st, sec_rgy_rep_cannot_rename_db);
        return;
    }

    CLEAR_STATUS(st);
}

/*  r s d b _ d e l e t e _ r g y _ f i l e s
 *
 *  Delete the rgy_data directory with its subfiles.
 */
PUBLIC void rsdb_delete_rgy_files (
#ifdef __STDC__
    void
#endif
)
{
    error_status_t  status;
    sec_rgy_pname_t fname;

    rsdb_delete_files( "", &status);
    /* rsdb_delete_files doesn't delete state file */
    strcpy((char *)fname, RGY_STATE_FILE);
    rsdb_file_unlink((char *)fname);
}


/*
 * Create rgy_state file and
 * save current rs_state info
 * in the file.
 */
PUBLIC void rsdb_state_create
#ifndef __STDC__
    ( rs_state_p, st )
    rs_state_t      *rs_state_p;
    error_status_t  *st;
#else
  (
    rs_state_t      *rs_state_p,
    error_status_t  *st
  )
#endif
{
#ifdef DCE_SEC_POSIX_F_LOCK
    struct flock    lock_arg;
#endif

    CLEAR_STATUS(st);

    rsdb_rgy_state_fd = rsdb_file_open(RGY_STATE_FILE, O_RDWR|O_CREAT,
                                            FILE_CREATION_PROTECTION);
    if (rsdb_rgy_state_fd == -1) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%s"),
                       sec_s_rs_rsdb,
                       svc_c_sev_error,
                       sec_sys_errno_file_text,
		       RGY_STATE_FILE,
                       strerror(errno));
        SET_STATUS(st, sec_rgy_rep_cannot_create_db);
        return;
    }

#ifdef DCE_SEC_POSIX_F_LOCK
    lock_arg.l_type = F_WRLCK;
    lock_arg.l_whence = 0;
    lock_arg.l_start = 0;
    lock_arg.l_len = 0;

    if (fcntl(rsdb_rgy_state_fd, F_SETLK, &lock_arg) == -1)  {
        if (errno  == EACCES)
            SET_STATUS(st, sec_rgy_rep_db_locked);
        else
            SET_STATUS(st, sec_rgy_rep_cannot_create_db);
    }
#else
    if (flock(rsdb_rgy_state_fd, (LOCK_EX|LOCK_NB)) == -1) {
        if (errno == EWOULDBLOCK)
            SET_STATUS(st, sec_rgy_rep_db_locked);
        else
            SET_STATUS(st, sec_rgy_rep_cannot_create_db);
    }
#endif /* DCE_SEC_POSIX_F_LOCK */

    if(BAD_STATUS(st)) {
        rsdb_file_close(rsdb_rgy_state_fd);
        rsdb_rgy_state_fd = -1;
        return;
    }

    rsdb_state_write(rs_state_p, st);
}

PUBLIC void rsdb_state_delete (
#ifdef __STDC__
    void
#endif
)
{
    if (rsdb_rgy_state_fd >= 0)
        rsdb_file_close(rsdb_rgy_state_fd);
    rsdb_rgy_state_fd = -1;

    rsdb_file_unlink(RGY_STATE_FILE);
}

PUBLIC void rsdb_state_open
#ifndef __STDC__
    ( rs_state_p, st )
    rs_state_t      *rs_state_p;
    error_status_t  *st;
#else
  (
    rs_state_t      *rs_state_p,
    error_status_t  *st
  )
#endif
{
    int             nb;
    boolean32       eq;
#ifdef DCE_SEC_POSIX_F_LOCK
    struct flock    lock_arg;
#endif

    CLEAR_STATUS(st);

    rsdb_rgy_state_fd = rsdb_file_open(RGY_STATE_FILE, O_RDWR, 0);
    if (rsdb_rgy_state_fd == -1) {
        if ((errno == ENOENT) || (errno == ENOTDIR))
            SET_STATUS(st, sec_rgy_object_not_found);        /* important error code */
        else
            SET_STATUS(st, sec_rgy_rep_cannot_open_db);
        return;
    }

#ifdef DCE_SEC_POSIX_F_LOCK
    lock_arg.l_type = F_WRLCK;
    lock_arg.l_whence = 0;
    lock_arg.l_start = 0;
    lock_arg.l_len = 0;

    if (fcntl(rsdb_rgy_state_fd, F_SETLK, &lock_arg) == -1)  {
        if (errno  == EACCES)
            SET_STATUS(st, sec_rgy_rep_db_locked);
        else
            SET_STATUS(st, sec_rgy_rep_cannot_open_db);
    }
#else
    if (flock(rsdb_rgy_state_fd, (LOCK_EX|LOCK_NB)) == -1) {
        if (errno == EWOULDBLOCK)
            SET_STATUS(st, sec_rgy_rep_db_locked);
        else
            SET_STATUS(st, sec_rgy_rep_cannot_open_db);
    }
#endif /* DCE_SEC_POSIX_F_LOCK */

    if(GOOD_STATUS(st)) {
        nb = read(rsdb_rgy_state_fd, (char *) rs_state_p,
                  sizeof(rs_state_t));

        if (rs_state_p->version != rs_c_state_file_version) {
            SET_STATUS(st, sec_rgy_rep_bad_db_version);
        } else if (nb < (signed32)sizeof(rs_state_t)) {
            SET_STATUS(st, sec_rgy_rep_cannot_read_db);
        }
    }

    if(BAD_STATUS(st)) {
	rsdb_state_close();
    }
    return;
}

PUBLIC void rsdb_state_close (
#ifdef __STDC__
    void
#endif
)
{
#ifdef DCE_SEC_POSIX_F_LOCK
    struct flock    lock_arg;
#endif
    if (rsdb_rgy_state_fd == -1) return;

#ifdef DCE_SEC_POSIX_F_LOCK
    lock_arg.l_type = F_UNLCK;
    lock_arg.l_whence = 0;
    lock_arg.l_start = 0;
    lock_arg.l_len = 0;

    fcntl(rsdb_rgy_state_fd, F_SETLK, &lock_arg);
#else
    if (flock(rsdb_rgy_state_fd, LOCK_UN) == -1) {
#endif /* DCE_SEC_POSIX_F_LOCK */

    rsdb_file_fsync(rsdb_rgy_state_fd);
    rsdb_file_close(rsdb_rgy_state_fd);
    rsdb_rgy_state_fd = -1;
#ifndef DCE_SEC_POSIX_F_LOCK
    }
#endif
}

PUBLIC void rsdb_state_write
#ifndef __STDC__
    ( rs_state_p, st )
    rs_state_t      *rs_state_p;
    error_status_t  *st;
#else
  (
    rs_state_t      *rs_state_p,
    error_status_t  *st
  )
#endif
{
    int nb;

    lseek(rsdb_rgy_state_fd, 0, L_SET);

    nb = write(rsdb_rgy_state_fd, (char *) rs_state_p,
                    sizeof(rs_state_t));
    if (nb < (signed32)sizeof(rs_state_t)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_sys_file_write_error,
                       rsdb_rgy_state_fd);
    }

    rsdb_file_fsync(rsdb_rgy_state_fd);

    CLEAR_STATUS(st);
}

/*
 * Create master_info file and
 * save current master_info
 * in the file.
 */
PUBLIC void rsdb_master_info_create
#ifndef __STDC__
    ( master_info, st )
    rs_master_info_t    *master_info;
    error_status_t      *st;
#else
  (
    rs_master_info_t    *master_info,
    error_status_t      *st
  )
#endif
{
    rsdb_master_info_fd = rsdb_file_open(MASTER_INFO_FILE, O_RDWR|O_CREAT,
                                            FILE_CREATION_PROTECTION);
    if (rsdb_master_info_fd == -1) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%s"),
                       sec_s_rs_rsdb,
                       svc_c_sev_error,
                       sec_sys_errno_file_text,
		       MASTER_INFO_FILE,
                       strerror(errno));
        SET_STATUS(st, sec_rgy_rep_cannot_create_db);
        return;
    }

    rsdb_master_info_write(master_info, st);
}

PUBLIC void rsdb_master_info_delete (
#ifdef __STDC__
    void
#endif
)
{
    if (rsdb_master_info_fd >= 0)
        rsdb_file_close(rsdb_master_info_fd);
    rsdb_master_info_fd = -1;

    rsdb_file_unlink(MASTER_INFO_FILE);
}

PUBLIC void rsdb_master_info_open
#ifndef __STDC__
    ( master_info, st )
    rs_master_info_t    *master_info;
    error_status_t      *st;
#else
  (
    rs_master_info_t    *master_info,
    error_status_t      *st
  )
#endif
{
    int             nb;
    boolean32       eq;

    CLEAR_STATUS(st);

    rsdb_master_info_fd = rsdb_file_open(MASTER_INFO_FILE, O_RDWR, 0);
    if (rsdb_master_info_fd == -1) {
        SET_STATUS(st, sec_rgy_rep_cannot_open_db);
        return;
    }

    nb = read(rsdb_master_info_fd, (char *) master_info,
	      sizeof(rs_master_info_t));

    if (master_info->version != rs_c_master_info_file_version) {
	rsdb_compat_convert_master_info_for_dce11(master_info,
		st);
	return;
    }

    if (nb < (signed32)sizeof(rs_master_info_t)) {
        SET_STATUS(st, sec_rgy_rep_cannot_read_db);
        return;
    }
}

PUBLIC void rsdb_master_info_close (
#ifdef __STDC__
    void
#endif
)
{
    if (rsdb_master_info_fd == -1) return;

    rsdb_file_fsync(rsdb_master_info_fd);
    rsdb_file_close(rsdb_master_info_fd);
    rsdb_master_info_fd = -1;
}

PUBLIC void rsdb_master_info_write
#ifndef __STDC__
    ( master_info, st )
    rs_master_info_t    *master_info;
    error_status_t      *st;
#else
  (
    rs_master_info_t    *master_info,
    error_status_t      *st
  )
#endif
{
    int nb;

    lseek(rsdb_master_info_fd, 0, L_SET);

    nb = write(rsdb_master_info_fd, (char *) master_info,
                    sizeof(rs_master_info_t));
    if (nb < (signed32)sizeof(rs_master_info_t)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_sys_file_write_error,
                       rsdb_master_info_fd);
    }

    rsdb_file_fsync(rsdb_master_info_fd);

    CLEAR_STATUS(st);
}

/*
 * Create the master_key file.
 * Open the file, write rsdb_key into the file and close it.
 */
PUBLIC void rsdb_mkey_create
#ifndef __STDC__
    ( fname, mkey, st )
    char            *fname;
    rsdb_auth_key_t *mkey;
    error_status_t  *st;
#else
    (
    char            *fname,
    rsdb_auth_key_t *mkey,
    error_status_t  *st
    )
#endif
{
    rsdb_mkey_file_t    mkey_file;

    CLEAR_STATUS(st);

    rsdb_file_mkey_create(fname, st);
    if(GOOD_STATUS(st)) {
        mkey_file.file_version = rsdb_c_mkey_file_version;
        mkey_file.mkey = *mkey;
        rsdb_mkey_fd = rsdb_file_mkey_open(fname, O_RDWR, 0600);
        if(rsdb_mkey_fd > 0) {
            write(rsdb_mkey_fd, (char *)(&mkey_file), sizeof(mkey_file));
            rsdb_file_close(rsdb_mkey_fd);
            rsdb_mkey_fd = -1;
            return;
        }
    }
    SET_STATUS(st, sec_rgy_mkey_file_io_failed);
    return;
}

/*
 * Open the master_key file, read it, close it.
 */
PUBLIC void rsdb_mkey_read
#ifndef __STDC__
    ( fname, mkey, st )
    char                *fname;     /* [in] */
    rsdb_auth_key_t     *mkey;      /* [out] */
    error_status_t      *st;        
#else
    (
    char                *fname,
    rsdb_auth_key_t     *mkey,    
    error_status_t      *st
    )
#endif
{
    rsdb_mkey_file_t    mkey_file;
    int                 nb = 0;

    CLEAR_STATUS(st);

    rsdb_mkey_fd = rsdb_file_mkey_open(fname, O_RDWR, 0600);
    if(rsdb_mkey_fd > 0) {
        nb = read(rsdb_mkey_fd, (char *)(&mkey_file), sizeof(mkey_file));
        rsdb_file_close(rsdb_mkey_fd);
        rsdb_mkey_fd = -1;
        if(nb < (signed32)sizeof(mkey_file)) {
            SET_STATUS(st, sec_rgy_mkey_bad_stored);
            return;
        }
        if (mkey_file.file_version != rsdb_c_mkey_file_version) {
            SET_STATUS(st, sec_rgy_rep_bad_db_version);
            return;
        }
        *mkey = mkey_file.mkey;
        return;
    }
    SET_STATUS(st, sec_rgy_mkey_file_io_failed);
    return;
}


PUBLIC void rsdb_mkey_delete
#ifndef __STDC__
    ( fname, st)
    char                *fname;     /* [in] */
    error_status_t      *st;
#else
    (
    char                *fname,
    error_status_t      *st
    )
#endif
{
    CLEAR_STATUS(st);
    
    if(rsdb_file_mkey_unlink(fname) != 0)
        SET_STATUS(st, sec_rgy_mkey_file_io_failed);
        
    return;
}

PUBLIC void rsdb_mkey_rename
#ifndef __STDC__
    ( src_name, tgt_name, st)
    char                *src_name;     /* [in] */
    char                *tgt_name;     /* [in] */
    error_status_t      *st;
#else
    (
    char                *src_name,
    char                *tgt_name,
    error_status_t      *st
    )
#endif
{
    CLEAR_STATUS(st);

    if(rsdb_file_mkey_rename(src_name, tgt_name) != 0)
        SET_STATUS(st, sec_rgy_mkey_file_io_failed);
        
    return;
}

PUBLIC boolean32 rsdb_mkey_exists
#ifndef __STDC__
    ( fname )
    char                *fname;        /* [in] */
#else
    (
    char                *fname
    )
#endif
{
    struct stat         buf;
    
    if(rsdb_file_mkey_stat(fname, &buf) == 0)
        return true;
    else
        return false;
}

PUBLIC void rsdb_fetch
#ifndef __STDC__
    ( db, key, key_len, data, data_len, status )
    db_handle       db;
    Pointer         key;
    int             key_len;
    Pointer         *data;
    int             *data_len;
    error_status_t  *status;
#else
  (
    db_handle       db,
    Pointer         key,
    int             key_len,
    Pointer         *data,
    int             *data_len,
    error_status_t  *status
  )
#endif
{
    b_datum search_key;
    b_datum *ret_data;

    search_key.dptr = key;
    search_key.dsize = key_len;
    ret_data = fetch_data(db->tree, &search_key);
    if (ret_data == NULL) {
        *data       = NULL;
        *data_len   = 0;
        SET_STATUS(status, sec_rgy_object_not_found);
    } else {
        *data       = ret_data->dptr;
        *data_len   = ret_data->dsize;
        CLEAR_STATUS(status);
    }
}

PUBLIC void rsdb_fetch_next
#ifndef __STDC__
    ( db, key, key_len, next_key, next_key_len, data, data_len, status )
    db_handle       db;
    Pointer         key;
    int             key_len;
    Pointer         *next_key;
    int             *next_key_len;
    Pointer         *data;
    int             *data_len;
    error_status_t  *status;
#else
  (
    db_handle       db,
    Pointer         key,
    int             key_len,
    Pointer         *next_key,
    int             *next_key_len,
    Pointer         *data,
    int             *data_len,
    error_status_t  *status
  )
#endif
{
    b_datum search_key;
    b_datum *ret_data;
    b_datum *new_key;

    search_key.dptr = key;
    search_key.dsize = key_len;
    ret_data = fetch_next(db->tree, &search_key, &new_key);
    if (ret_data == NULL) {
        *next_key       = NULL;
        *next_key_len   = 0;
        *data           = NULL;
        *data_len       = 0;
        SET_STATUS(status, sec_rgy_object_not_found);
    } else {
        *next_key       = new_key->dptr;
        *next_key_len   = new_key->dsize;
        *data           = ret_data->dptr;
        *data_len       = ret_data->dsize;
        CLEAR_STATUS(status);
    }
}

PUBLIC void rsdb_store
#ifndef __STDC__
    ( db, key, key_len, data, data_len, status )
    db_handle       db;
    Pointer         key;
    int             key_len;
    Pointer         data;
    int             data_len;
    error_status_t  *status;
#else
  (
    db_handle       db,
    Pointer         key,
    int             key_len,
    Pointer         data,
    int             data_len,
    error_status_t  *status
  )
#endif
{
    b_datum     new_data;
    b_datum     new_key;

    CLEAR_STATUS(status);
    new_data.dptr   = data;
    new_data.dsize  = data_len;
    new_key.dptr    = key;
    new_key.dsize   = key_len;
    if (!insert_data(false, &(db->tree), &new_key, &new_data)) {
        dce_svc_printf(SEC_RSDB_DBSTORE_FAIL_MSG);
    }
    db->dirty = true;
}

PUBLIC void rsdb_replace
#ifndef __STDC__
    ( db, key, key_len, data, data_len, status )
    db_handle       db;
    Pointer         key;
    int             key_len;
    Pointer         data;
    int             data_len;
    error_status_t  *status;
#else
  (
    db_handle       db,
    Pointer         key,
    int             key_len,
    Pointer         data,
    int             data_len,
    error_status_t  *status
  )
#endif
{
    b_datum     new_key;
    b_datum     new_data;

    CLEAR_STATUS(status);
    new_data.dptr   = data;
    new_data.dsize  = data_len;
    new_key.dptr    = key;
    new_key.dsize   = key_len;
    if (!insert_data(true, &(db->tree), &new_key, &new_data)) {
        dce_svc_printf(SEC_RSDB_DBSTORE_FAIL_MSG);
    }
    db->dirty = true;
}

PUBLIC void rsdb_delete
#ifndef __STDC__
    ( db, key, key_len, status )
    db_handle       db;
    Pointer         key;
    int             key_len;
    error_status_t  *status;
#else
  (
    db_handle       db,
    Pointer         key,
    int             key_len,
    error_status_t  *status
  )
#endif
{
    b_datum     old_key;

    old_key.dptr    = key;
    old_key.dsize   = key_len;
    if (!delete_data(db->tree, &old_key)) {
        SET_STATUS(status, sec_rgy_object_not_found);
    } else {
        db->dirty = true;
        CLEAR_STATUS(status);
    }
}



/*
 *  r s d b _ c o m p a t _ s t a t e _ o p e n _ s e t ...
 *
 */
PUBLIC void rsdb_compat_state_open_set_version (
    rs_state_t      *rs_state_p,
    error_status_t  *st
    )
{
    int             nb;
    boolean32       eq;
#ifdef DCE_SEC_POSIX_F_LOCK
    struct flock    lock_arg;
#endif

    CLEAR_STATUS(st);

    rsdb_rgy_state_fd = rsdb_file_open(RGY_STATE_FILE, O_RDWR, 0);
    if (rsdb_rgy_state_fd == -1) {
        if ((errno == ENOENT) || (errno == ENOTDIR))
            SET_STATUS(st, sec_rgy_object_not_found);        /* important error code */
        else
            SET_STATUS(st, sec_rgy_rep_cannot_open_db);
        return;
    }

#ifdef DCE_SEC_POSIX_F_LOCK
    lock_arg.l_type = F_WRLCK;
    lock_arg.l_whence = 0;
    lock_arg.l_start = 0;
    lock_arg.l_len = 0;

    if (fcntl(rsdb_rgy_state_fd, F_SETLK, &lock_arg) == -1)  {
        if (errno  == EACCES)
            SET_STATUS(st, sec_rgy_rep_db_locked);
        else
            SET_STATUS(st, sec_rgy_rep_cannot_open_db);
    }
#else
    if (flock(rsdb_rgy_state_fd, (LOCK_EX|LOCK_NB)) == -1) {
        if (errno == EWOULDBLOCK)
            SET_STATUS(st, sec_rgy_rep_db_locked);
        else
            SET_STATUS(st, sec_rgy_rep_cannot_open_db);
    }
#endif /* DCE_SEC_POSIX_F_LOCK */

    if(GOOD_STATUS(st)) {
        nb = read(rsdb_rgy_state_fd, (char *) rs_state_p,
                  sizeof(rs_state_t));

        if (rs_state_p->version != rs_c_state_file_version) {
	    rs_state_p->version = rs_c_state_file_version;
        } else if (nb < (signed32)sizeof(rs_state_t)) {
            SET_STATUS(st, sec_rgy_rep_cannot_read_db);
        }
    }

    if(BAD_STATUS(st)) {
        rsdb_state_close();
    }
    return;
}


#define NUM_RELATIONS_LAST_DCE102 8
/*
 *  r s d b _ c o m p a t _ d c e 1 1 _ c r e a t e
 *
 *  create databases that is newly introduced in DCE 1.1
 *  before database conversion starts. 
 */
PUBLIC void rsdb_compat_dce11_create (
    )
{
    int             i;
    sec_rgy_pname_t fname;

    rsdb_attr_db        = &attr_data;
    rsdb_attr_schema_db = &attr_schema_data;
    rsdb_login_db       = &login_data;
    rsdb_journal_db     = &journal;


    for (i = NUM_RELATIONS_LAST_DCE102; i < NUM_RELATIONS; i++) {
        if (relation_info[i].db == NULL)
            continue;

        relation_info[i].db->tree   = NULL;
        relation_info[i].db->dirty  = true;
    }

    rsdb_attr_sch_create();
    rsdb_login_create();
    rsdb_journal_create();

    rsdb_attr_sch_init();
    rsdb_login_init();
    rsdb_journal_init();

    for (i = NUM_RELATIONS_LAST_DCE102; i < NUM_RELATIONS; i++) {
        if (relation_info[i].db == NULL || !relation_info[i].needs_to_be_saved)
            continue;

        if (!relation_info[i].db->dirty)
            continue;

        dce_svc_printf(SEC_RGY_CHKPT_SAVE_RELATION_MSG, 
                       relation_info[i].fname);

        strcpy( (char *) fname, relation_info[i].fname);
        save_database( (char *) fname, relation_info[i].db->tree);
        relation_info[i].db->dirty = false;
    }

}


#define FOREIGN_PRINCIPAL 12
/*
 *  r s d b _ f o r e i g n _ p e r s o n _ c r e a t e _ i n i t
 *
 *  create foreign principal databases that is newly introduced in DCE 1.2.2
 */
PUBLIC void rsdb_foreign_person_create_init (
    error_status_t          *st_p
    )
{
    sec_rgy_pname_t 	   fname;
    sec_rgy_domain_t	   domain;

    domain = sec_rgy_domain_foreign_person;
    domain_info[domain].db = rsdb_foreign_person_db
			   = &foreign_person_data;


    relation_info[FOREIGN_PRINCIPAL].db->tree   = NULL;
    relation_info[FOREIGN_PRINCIPAL].db->dirty  = true;

    /* create foreign person dummy database */
    rsdb_compat_dce122_foreign_person_create(st_p);
    if(BAD_STATUS(st_p)) {
        return;
    }

    /* initialize in-memory foreign person domain dummy head and tail */
    rsdb_compat_dce122_foreign_person_init();

    /* servicibility message */
    dce_svc_printf(SEC_RGY_CHKPT_SAVE_RELATION_MSG, 
                       relation_info[FOREIGN_PRINCIPAL].fname);

    /* save foreign domain database to disk */
    strcpy( (char *) fname, relation_info[FOREIGN_PRINCIPAL].fname);
    save_database( (char *) fname, relation_info[FOREIGN_PRINCIPAL].db->tree);
    relation_info[FOREIGN_PRINCIPAL].db->dirty = false;
    
}


#ifdef DB_DEBUGGING

PUBLIC void rsdb_print_key (
#ifdef __STDC__
    char    *key,
    char    *trailer
#endif
);

PUBLIC void rsdb_dump_database
#ifndef __STDC__
    ( db )
    db_handle   db;
#else
  (
    db_handle   db
  )
#endif
{
    dump_database_root(db->tree, rsdb_print_key);
}

#endif /* DB_DEBUGGING */

#undef RSDB_MAIN
