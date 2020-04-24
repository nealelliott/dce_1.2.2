/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_compat.c,v $
 * Revision 1.1.78.2  1996/08/09  12:11:28  arvind
 * 	Merge Registry migration functionality: database reformat and enable sec_passwd_genprivkey
 * 	[1996/07/18  18:45 UTC  aha  /main/DCE_1.2.2/aha_pk7/1]
 *
 * 	Merge registry migration work from aha_pk6
 * 	[1996/07/18  15:36 UTC  aha  /main/DCE_1.2.2/aha_pk6/1]
 *
 * 	Changes for Public Key Login; registry migration: set privkey_ok flag in
 * 	local krbtgt account flags
 * 	[1996/05/24  23:17 UTC  sommerfeld  /main/DCE_1.2.2/1]
 *
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:47 UTC  hanfei  /main/DCE_1.2/2]
 *
 * 	add rsdb_compat_dce122_foreign_person_create() for database conversion
 * 	to DCE 1.2.2.
 * 	[1996/03/20  16:00 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/3]
 *
 * 	multi-cell group work: add handle for database conversion to DCE 1.2.2.
 * 	[1996/03/15  15:00 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/2]
 *
 * 	multi-cell group work: initialize foreign member[ship] fields from using reserved fields.
 * 	[1996/02/01  19:20 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 *
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:06  marty]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/05/30  15:43 UTC  hanfei  /main/HPDCE02/3]
 *
 * 	merge in fix
 * 	HP 	[1995/05/25  15:43 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/1]
 *
 * 	[OT 12872] close state file properly.
 * 	HP 	[1995/05/15  21:11 UTC  greg  /main/HPDCE02/2]
 *
 * 	merge
 * 	HP 	[1995/02/09  15:21 UTC  hanfei  /main/hanfei_mothra_bugfix/3]
 *
 * 	bug fix : don't do rs_log_replay and rs_log_truncate in the db conversion.
 * 	[1995/12/08  17:22:10  root]
 *
 * Revision 1.1.78.1  1996/05/10  13:24:20  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:47 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/3  1996/03/20  16:00 UTC  hanfei
 * 	add rsdb_compat_dce122_foreign_person_create() for database conversion
 * 	to DCE 1.2.2.
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/03/15  15:00 UTC  hanfei
 * 	multi-cell group work: add handle for database conversion to DCE 1.2.2.
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/01  19:20 UTC  hanfei
 * 	multi-cell group work: initialize foreign member[ship] fields from using reserved fields.
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:19 UTC  psn
 * 	merge in fix
 * 	[1995/05/30  15:43 UTC  hanfei  /main/HPDCE02/3]
 * 
 * 	[OT 12872] close state file properly.
 * 	[1995/05/25  15:43 UTC  hanfei  /main/HPDCE02/hanfei_mothra_bl4/1]
 * 
 * 	Fixed merge inconsistency
 * 	[1995/04/24  15:01 UTC  hanfei  /main/HPDCE02/SEC_migrate_merge/3]
 * 
 * 	Fix botched merge.
 * 	[1995/04/22  20:05 UTC  greg  /main/HPDCE02/SEC_migrate_merge/2]
 * 
 * 	Ceate SEC_migrate_merge mainline from SEC_migrate and HPDCE02
 * 	[1995/04/21  12:57 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	bug fix for dce103 to dce11 database conversion.
 * 	[1995/02/03  04:25 UTC  hanfei  /main/hanfei_mothra_bugfix/1]
 * 
 * 	merge
 * 	[1995/02/17  16:20 UTC  hanfei  /main/HPDCE02/1]
 * 
 * 	bug fix : don't do rs_log_replay and rs_log_truncate in the db conversion.
 * 	[1995/02/09  15:21 UTC  hanfei  /main/hanfei_mothra_bugfix/3]
 * 
 * Revision 1.1.71.4  1994/08/25  21:36:13  sekhar
 * 	Put back SVC messages for fatal_st() and error_st() calls.
 * 	[1994/08/25  21:02:36  sekhar]
 * 
 * Revision 1.1.71.3  1994/08/25  16:30:39  hanfei
 * 	dce1.1 rgy database backward compatibility work
 * 	[1994/08/25  14:42:17  hanfei]
 * 
 * Revision 1.1.71.2  1994/08/19  17:59:37  mdf
 * 	1994/08/18  sekhar
 * 	Log messages using SVC calls.
 * 	[1994/08/19  13:11:19  mdf]
 * 
 * Revision 1.1.71.1  1994/08/04  16:14:33  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:13:31  mdf]
 * 
 * Revision 1.1.5.2  1993/06/24  20:20:32  hinman
 * 	[hinman] - Make include of sys/types.h available to all
 * 	[1993/06/22  11:45:19  hinman]
 * 
 * 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
 * 	[1993/06/17  14:38:58  hinman]
 * 
 * Revision 1.1.3.5  1993/03/26  14:41:10  rps
 * 	CR 7472 - rsdb_compat_translate_log_entry - generate
 * 	rs_log_passwd_item_dce102_t for 1.0.1 null unix passwords
 * 	[1993/03/24  21:46:06  rps]
 * 
 * Revision 1.1.3.4  1993/03/01  17:50:11  ahop
 * 	compat: support changes in state file
 * 	[1993/02/25  21:50:46  ahop]
 * 
 * Revision 1.1.3.3  1993/02/19  17:15:14  ahop
 * 	dbcp: make robust
 * 	[1993/02/19  08:45:42  ahop]
 * 
 * Revision 1.1.3.2  1993/02/10  17:13:31  ahop
 * 	Initial revision
 * 	[1993/02/10  17:11:04  ahop]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992, 1993
 * Unpublished work. All Rights Reserved.
 */
/*
 * Registry Database Backwards Compatibility Routines
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
	"$Id: rsdb_compat.c,v 1.1.78.2 1996/08/09 12:11:28 arvind Exp $";
#endif

# include <sys/types.h>	/*	Need for ushort definition in sys/file.h */
#include <sys/file.h>
#include <un_strng.h>
#include <un_io.h>
#include <errno.h>

#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/aclbase.h>
#include <dce/uuid.h>
#include <dce/passwd.h>
#include <balanced_trees.h>
#include <rsdb.h>
#include <rsdb_pvt.h>
#include <rsdb_acl.h>
#include <rsdb_file.h>
#include <rsdb_policy.h>
#include <rsdb_replica.h>
#include <rsdb_jrnl.h>
#include <rsdb_login.h>
#include <rsdb_compat.h>
#include <rsdb_compat_pvt.h>
#include <rs_ver.h>
#include <rs_pvt.h>
#include <rs_log.h>
#include <rs_lock.h>
#include <rs_state.h>
#include <rs_master.h>
#include <rs_util.h>
#include <rs_mkey.h>
#include <rs_rep_util.h>
#include <sec_svc.h>

/* declared in rsdb_log.c */
extern int rs_log_fd;


/* Local definitions
 */
#define LOG_HDR_FIXED_SZ_DIFF          ( sizeof(rs_log_hdr_t) - \
    sizeof(rs_log_hdr_dce101_t) )

#define LOG_ACCT_ADD_FIXED_SZ_DIFF     ( (sizeof(rs_log_acct_add_dce102_t) - \
    sizeof(rs_log_acct_add_dce101_t)) + sizeof(rs_log_passwd_item_dce102_t) )

#define LOG_ACCT_DELETE_FIXED_SZ_DIFF  LOG_HDR_FIXED_SZ_DIFF

#define LOG_ACCT_REPLACE_FIXED_SZ_DIFF ( LOG_ACCT_ADD_FIXED_SZ_DIFF + \
    sizeof(sec_timeval_sec_t) + sizeof(rs_log_passwd_item_dce102_t) )

#define LOG_ACCT_RENAME_FIXED_SZ_DIFF  ( sizeof(rs_log_acct_rename_dce102_t) - \
    sizeof(rs_log_acct_rename_dce101_t) )

#define LOG_ACL_REPLACE_FIXED_SZ_DIFF  ( sizeof(rs_log_acl_replace_dce102_t) - \
    sizeof(rs_log_acl_replace_dce101_t) )

/* Private typedefs and structures copied here from rs_log_*.c */
#define rs_log_acl_replace_op   0

#define rs_log_acct_add_op         0
#define rs_log_acct_delete_op      1
#define rs_log_acct_replace_op     2
#define rs_log_acct_rename_op      3



/*
 *  Private Routines
 */

/*
 *  r s d b _ c o m p a t _ s t a t e _ i n i t
 *
 *  Open the state file and set an advisory lock.
 *  Read in the dce101-format state file and use
 *  it to initialize the current rs_state structure.
 */
PRIVATE void rsdb_compat_state_init
#ifndef __STDC__
    ( db_version, state_fd, st_p )
    rsdb_compat_db_version_t    db_version;      /* [in] */
    int                         *state_fd;       /* [out] */
    error_status_t              *st_p;
#else
    (
    rsdb_compat_db_version_t    db_version,
    int                         *state_fd,
    error_status_t              *st_p
    )
#endif
{
    unsigned32              nb;
    rgy_state_file_dce101_t old_state;
    error_status_t           status;
#ifdef DCE_SEC_POSIX_F_LOCK
    struct flock            lock_arg;
#endif

    CLEAR_STATUS(st_p);

    if(db_version != rsdb_c_db_version_dce101) {
        SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
        return;
    }

    *state_fd = rsdb_file_open(RGY_STATE_FILE, O_RDWR, 0);
    if (*state_fd == -1) {
        if ((errno == ENOENT) || (errno == ENOTDIR))
            SET_STATUS(st_p, sec_rgy_object_not_found);  
        else
            SET_STATUS(st_p, sec_rgy_rep_cannot_open_db);
        return;
    }

#ifdef DCE_SEC_POSIX_F_LOCK
    lock_arg.l_type = F_WRLCK;
    lock_arg.l_whence = 0;
    lock_arg.l_start = 0;
    lock_arg.l_len = 0;

    if (fcntl(*state_fd, F_SETLK, &lock_arg) == -1)  {
        if (errno  == EACCES)
            SET_STATUS(st_p, sec_rgy_rep_db_locked);
        else
            SET_STATUS(st_p, sec_rgy_rep_cannot_open_db);
    }
#else
    if (flock(*state_fd, (LOCK_EX|LOCK_NB)) == -1) {
        if (errno == EWOULDBLOCK)
            SET_STATUS(st_p, sec_rgy_rep_db_locked);
        else
            SET_STATUS(st_p, sec_rgy_rep_cannot_open_db);
    }
#endif /* DCE_SEC_POSIX_F_LOCK */

    if(GOOD_STATUS(st_p)) {
        nb = read(*state_fd, (char *) &old_state, sizeof(old_state));

        if (old_state.version != rs_c_state_file_version_dce101) {
            SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
        } else if (nb < sizeof(old_state)) {
            SET_STATUS(st_p, sec_rgy_rep_cannot_read_db);
        }
    }
    if(BAD_STATUS(st_p)) {
        rsdb_file_close(*state_fd);
        return;
    }

    /* Initialize the current rs_state */
    rs_state.version = rs_c_state_file_version;

    switch(old_state.db_state) {
    case rsdb_c_st_dce101_consistent:
        switch(old_state.rep_state) {
        case rs_c_st_dce101_in_service:
            rs_state.rep_state = rs_c_state_in_service;
            break;
        case rs_c_st_dce101_in_maintenance:
            rs_state.rep_state = rs_c_state_in_maintenance;
            break;
        default:
            rs_state.rep_state = rs_c_state_uninitialized;
            break;
        }
        break;
    case rsdb_c_st_dce101_renaming:
        rs_state.rep_state = rs_c_state_renaming;
        break;
    case rsdb_c_st_dce101_mkey_changing:
        rs_state.rep_state = rs_c_state_mkey_changing;
        break;
    default:
        rs_state.rep_state = rs_c_state_uninitialized;
        break;
    }
    /* If the database is in an unacceptable state, return with error */
    if(rs_state.rep_state == rs_c_state_uninitialized) {
	dce_svc_printf(SEC_RSDB_DB_UNRECOG_STATE_MSG);
        return;
    }

    rs_state.next_rep_state = rs_c_state_in_service;
    rs_state.cell_sec_id = old_state.replist_id;
    rs_state.rep_id = old_state.rs_id;
    uuid_create_nil(&rs_state.init_id, &status);
    rs_state.unused = 0;  /* formerly global compat sw rev */
    rs_state.master = true;
}


/*
 *  r s d b _ c o m p a t _ s t a t e _ w r i t e 
 *
 *  Save rs_state in the file represented
 *  by the input file descriptor.
 */
PRIVATE void rsdb_compat_state_write
#ifndef __STDC__
    ( rs_state_p, state_fd, st_p )
    rs_state_t                  *rs_state_p;     /* [in] */
    int                         state_fd;        /* [in] */
    error_status_t              *st_p;
#else
    (
    rs_state_t                  *rs_state_p,
    int                         state_fd,
    error_status_t              *st_p
    )
#endif
{
    int                         nb;

    lseek(state_fd, 0, L_SET);

    nb = write(state_fd, (char *)rs_state_p, sizeof(rs_state_t));
    if (nb < (signed32)sizeof(rs_state_t)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_sys_file_write_error,
                       state_fd);
    }
    rsdb_file_fsync(state_fd);
    CLEAR_STATUS(st_p);
}


/*
 *  r s d b _ c o m p a t _ s e t _ s t a t e
 *
 *  Save the replica state (along with the rest of
 *  the rs_state record) in the file represented
 *  by the input file descriptor.
 */
PRIVATE void rsdb_compat_set_state
#ifndef __STDC__
    ( rep_state, state_fd, st_p )
    unsigned32                  rep_state;       /* [in] */
    int                         state_fd;        /* [in] */
    error_status_t              *st_p;
#else
    (
    unsigned32                  rep_state,
    int                         state_fd,
    error_status_t              *st_p
    )
#endif
{
    rs_state.rep_state = rep_state; 

    rsdb_compat_state_write(&rs_state, state_fd, st_p);
}


/*
 *  r s d b _ c o m p a t _ l o a d _ r g y _ i n f o 
 *
 *  This operation (comparable to rsdb_load_rgy_info in 
 *  rsdb_policy.c) allows the caller to specify a software 
 *  version so that a rgy_info file of the specified version
 *  may be read and converted to the current version.
 */
PRIVATE void rsdb_compat_load_rgy_info
#ifndef __STDC__
    ( db_version, fd, st_p )
    rsdb_compat_db_version_t    db_version;      /* [in] */
    int                         fd;              /* [in] */
    error_status_t              *st_p;
#else
    (
    rsdb_compat_db_version_t    db_version,
    int                         fd,
    error_status_t              *st_p
    )
#endif
{
    unsigned32                      len;
    rsdb_rgy_file_info_dce101_t     old_rgy_info;

    CLEAR_STATUS(st_p);

    if(db_version != rsdb_c_db_version_dce101) {
        SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
        return;
    }

    len = read(fd, (char *)&old_rgy_info, sizeof(old_rgy_info));
    if(len != sizeof(old_rgy_info)) {
        SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
        return;
    }

    rsdb_init_rgy_info_dce102(&old_rgy_info.properties,
        &old_rgy_info.policy, &old_rgy_info.auth_policy,
        old_rgy_info.sequential_key, 
        &old_rgy_info.domain_cache_state[sec_rgy_domain_person],
        &old_rgy_info.domain_cache_state[sec_rgy_domain_group],
        &old_rgy_info.domain_cache_state[sec_rgy_domain_org],
        old_rgy_info.acl, old_rgy_info.myname, 
        &old_rgy_info.enc_master_key, st_p);
}


/*
 *  r s d b _ c o m p a t _ r e p l i c a s _ i n i t
 *
 *  Clear the old contents of the replicas database.
 *  Create and store a replica entry for this replica.
 */
PRIVATE void rsdb_compat_replicas_init
#ifndef __STDC__
    ( db_version, rep_twrs, st_p )
    rsdb_compat_db_version_t    db_version;      /* [in] */
    rs_replica_twr_vec_p_t      rep_twrs;        /* [in] */
    error_status_t              *st_p;
#else
    (
    rsdb_compat_db_version_t    db_version,
    rs_replica_twr_vec_p_t      rep_twrs,
    error_status_t              *st_p
    )
#endif
{
    sec_rgy_name_t              rep_name;
    unsigned_char_p_t           rep_name_p;
    rs_replica_mgt_item_t       rep_mgt_item;

    CLEAR_STATUS(st_p);

    if(db_version != rsdb_c_db_version_dce101) {
        SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
        return;
    }
    rsdb_clear_database(rsdb_replica_db);

    /* Get this replica's name from rgy_info */
    rsdb_get_my_server_name(rep_name);
    rep_name_p = (unsigned_char_p_t)rep_name;

    rs_rep_util_rep_item_pack(&rs_state.rep_id, rep_name_p,
                              rep_twrs, rs_c_replica_prop_init,
			       rs_c_replica_prop_unknown,
                              NULL, NULL, &rep_mgt_item);
    rsdb_replica_add(&rep_mgt_item, st_p);
}


/*
 *  r s d b _ c o m p a t _ l o a d 
 *
 *  This operation (comparable to rsdb_load in rsdb.c)
 *  allows the caller to specify a software version so that
 *  a Registry database of the specified version may be read
 *  and converted to the current version.
 */
PRIVATE void rsdb_compat_load
#ifndef __STDC__
    ( db_version, rep_twrs, st_p )
    rsdb_compat_db_version_t    db_version;      /* [in] */
    rs_replica_twr_vec_p_t      rep_twrs;        /* [in] */
    error_status_t              *st_p;
#else
    (
    rsdb_compat_db_version_t    db_version,
    rs_replica_twr_vec_p_t      rep_twrs,
    error_status_t              *st_p
    )
#endif
{
    int fd;

    CLEAR_STATUS(st_p);

    if(db_version != rsdb_c_db_version_dce101) {
        SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
        return;
    }

    rsdb_load_relations(st_p);
    if(BAD_STATUS(st_p)) {
        return;
    }

    /* acl database must be initialized before 
     * calling rsdb_compat_load_rgy_info which creates new acls
     */
    fd = rsdb_file_open(RGY_FILE, O_RDWR, 0);
    if (fd == -1) {
        SET_STATUS(st_p, sec_rgy_rep_cannot_open_db);
        return;
    }
    rsdb_compat_load_rgy_info(db_version, fd, st_p);
    rsdb_file_close(fd);
    if(BAD_STATUS(st_p)) {
        return;
    }

    /* Scrap the old contents of the replicas database and 
     * reinitialize it with an entry for this master replica.
     */
    rsdb_compat_replicas_init(db_version, rep_twrs, st_p);
}


/*
 *  r s d b _ c o m p a t _ m a s t e r _ i n f o _ i n i t 
 *
 *  Create the master_info file with initial values
 *  determined by rs_state and seqno.
 */
PRIVATE void rsdb_compat_master_info_init
#ifndef __STDC__
    ( db_version, seqno, st_p )
    rsdb_compat_db_version_t    db_version;      /* [in] */
    rs_update_seqno_t           *seqno;          /* [in] */
    error_status_t              *st_p;
#else
    (
    rsdb_compat_db_version_t    db_version,
    rs_update_seqno_t           *seqno,
    error_status_t              *st_p
    )
#endif
{
    CLEAR_STATUS(st_p);

    if(db_version != rsdb_c_db_version_dce101) {
        SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
        return;
    }

    /* Since the master_info file doesn't exist in the 1.0.1 database,
     * create it now.
     *
     *XXX MIGRATION - need to convert 1.0.2 database master info
     * file using global compat sw rev from the state info.  If it
     * is o, then we  assume it is rs_c_software_vers_1_0, otherwise,
     * we just use whatever we find. 
     */
    rs_master_info_create(&rs_state.rep_id, seqno, 
			  rs_c_software_vers_unknown, st_p);
}


/*
 *  r s d b _ c o m p a t _ m k e y _ i n i t
 *
 *  Open and read the dce101-formatted mkey file.  Write the
 *  file back to disk in the dce102 format.  Call rs_mkey_init
 *  to initialize the mkey variables.  Use the rep_state and
 *  the existence of the mkey and old_mkey files to determine
 *  which file to read (in case a crash interrupted an mkey_change
 *  operation).
 *
 *  Since the mkey file is not included in the set of database
 *  files that gets "atomically" saved with the rsdb_checkpt
 *  facility, there is some risk that the dce102 version of the
 *  mkey file will be saved to disk while a crash prevents the
 *  dce102 version of the rest of the database from being saved.
 *  To handle this case (reading dce101-formatted database with
 *  a dce102-formatted mkey file), the rsdb_compat_mkey_init
 *  operation must recognize and handle both the old and the 
 *  current formats.
 */
PRIVATE void rsdb_compat_mkey_init
#ifndef __STDC__
    ( db_version, rep_state, st_p )
    rsdb_compat_db_version_t    db_version;      /* [in] */
    unsigned32                  rep_state;       /* [in] */
    error_status_t              *st_p;
#else
    (
    rsdb_compat_db_version_t    db_version,
    unsigned32                  rep_state,
    error_status_t              *st_p
    )
#endif
{
    int                 fd;
    int                 nb = 0;
    rsdb_auth_key_t     mkey; 
    rsdb_mkey_file_t    dce102_mkey;
    sec_rgy_name_t      old_master_key_filename;
    boolean32           old_mkey_file_exists = false;
    error_status_t      tmp_status;

    CLEAR_STATUS(st_p);

    if(db_version != rsdb_c_db_version_dce101) {
        SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
        return;
    }

    u_strcpy( (char *)old_master_key_filename, MASTER_KEY_FILE );
    u_strcat( (char *)old_master_key_filename, ".old" );

    if(rsdb_mkey_exists((char *)old_master_key_filename))
        old_mkey_file_exists = true;

    if(old_mkey_file_exists) {
        if (rep_state == rs_c_state_mkey_changing) {
            /* Rename <mkey>.old to <mkey>, overwriting
             * <mkey> if it already exists.
             */
            rsdb_mkey_rename((char *)old_master_key_filename,
                             MASTER_KEY_FILE, st_p);
            if(BAD_STATUS(st_p)) {
                return;
            }
        } else {
            /* delete <mkey>.old  */
            rsdb_mkey_delete((char *)old_master_key_filename, &tmp_status);
        }
    }

    /* Try reading it in dce102 format (using rsdb_mkey_read).
     * If that fails, read in the dce101 format
     * and write the dce102 format.
     */
    rsdb_mkey_read(MASTER_KEY_FILE, &mkey, st_p);
    if(BAD_STATUS(st_p)) {
        CLEAR_STATUS(st_p);
        fd = rsdb_file_mkey_open(MASTER_KEY_FILE, O_RDWR, 0600);
        if(fd == -1) {
            SET_STATUS(st_p, sec_rgy_mkey_file_io_failed);
            return;
        }
        nb = read(fd, (char *)&mkey, sizeof(mkey));
        if(nb < sizeof(mkey)) {
            SET_STATUS(st_p, sec_rgy_mkey_bad_stored);
        }
        if(GOOD_STATUS(st_p)) {
            dce102_mkey.file_version = rsdb_c_mkey_file_version;
            dce102_mkey.mkey = mkey;
            lseek(fd, 0, L_SET);
            write(fd, (char *)&dce102_mkey, sizeof(dce102_mkey));
        }
        rsdb_file_close(fd);
    }
    if(BAD_STATUS(st_p)) {
        return;
    }
    rs_mkey_init(rep_state, st_p);
}


/*
 *  r s d b _ c o m p a t _ l o g _ r e a d _ e n t r y
 *
 *  This operation (comparable to rs_log_read_entry in rs_log.c)
 *  allows the caller to specify a software version so that
 *  a Registry database of the specified version may be read
 *  and converted to the current version.
 */
PRIVATE void rsdb_compat_log_read_entry
#ifndef __STDC__
    ( db_version, fd, entrypp, sizep, st_p )
    rsdb_compat_db_version_t    db_version;      /* [in] */
    int                         fd;
    rs_log_hdr_dce101_t         **entrypp;
    unsigned32                  *sizep;
    error_status_t              *st_p;
#else
    (
    rsdb_compat_db_version_t    db_version,
    int                         fd,
    rs_log_hdr_dce101_t         **entrypp,
    unsigned32                  *sizep,
    error_status_t              *st_p
    )
#endif
{
    rs_log_hdr_dce101_t         *entryp;
    unsigned32                  buff_len;
    unsigned32                  nb;
    char                        *buff;
    char                        *temp_p;

    entryp      = *entrypp;
    buff_len    = *sizep;

    CLEAR_STATUS(st_p);

    if(db_version != rsdb_c_db_version_dce101) {
        SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
        return;
    }
    /*
     * Read the log entry header to determine entry's size
     */
    if (read(fd, (char *) entryp, sizeof(*entryp)) != sizeof(*entryp)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
                       sec_s_rs_rsdb,
                       svc_c_sev_error,
                       sec_sys_file_read_error,
                       fd);
        SET_STATUS(st_p, sec_rgy_rep_cannot_read_db);
        return;
    }

    /*
     * Allocate a new buffer if the next item is larger than the
     * currently allocated buffer.
     */
    if (entryp->upd_reclen > buff_len) {
        temp_p = (char *)malloc(entryp->upd_reclen);
        if (temp_p == NULL) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_rsdb,
                           svc_c_sev_error,
                           sec_rgy_cant_allocate_memory);
            SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
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

        entryp      = (rs_log_hdr_dce101_t *) temp_p;
        buff_len    = entryp->upd_reclen;
        *entrypp    = entryp;
        *sizep      = buff_len;
    }

    nb = entryp->upd_reclen - sizeof(*entryp);
    buff = (char *) entryp;
    buff += sizeof(*entryp);
    if (read(fd, buff, nb) != nb) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_sys_file_write_error,
                       fd);
        SET_STATUS(st_p, sec_rgy_rep_cannot_read_db);
        return;
    }
}


/*
 * r s d b _ c o m p a t _ c m p _ c l o c k
 *
 * Compare two clock values:
 *  if clock1 > clock2 return 1
 *  if clock1 = clock2 return 0
 *  if clock1 < clock2 return -1
 */
PRIVATE short rsdb_compat_cmp_clock
#ifndef __STDC__
    (clock1, clock2)
    sec_timeval_t   clock1;
    sec_timeval_t   clock2;
#else
  (
    sec_timeval_t   clock1,
    sec_timeval_t   clock2
  )
#endif
{
    short cmp_clock;

    if (timercmp((struct timeval *) &clock1, 
                 (struct timeval *) &clock2, <)) {
         cmp_clock = -1;
    } else if (timercmp((struct timeval *) &clock1,
                        (struct timeval *) &clock2, ==)) {
         cmp_clock = 0;
    } else if (timercmp((struct timeval *) &clock1,
                        (struct timeval *) &clock2, >)) {
         cmp_clock = 1;
    }

    return(cmp_clock);
}


/*
 *  r s d b _ c o m p a t _ t r a n s l a t e _ l o g _ e n t r y
 *
 *  Copy dce101-format log entry information from entryp
 *  into the dce102-format new_entryp, translating where 
 *  necessary.
 *  Increase the buffer size of new_entryp if needed.
 */
PRIVATE void rsdb_compat_translate_log_entry
#ifndef __STDC__
    ( db_version, entryp, new_entryp, new_buff_len, st_p )
    rsdb_compat_db_version_t    db_version;      /* [in] */
    rs_log_hdr_dce101_t         *entryp;         /* [in] */
    rs_log_hdr_t                **new_entryp;    /* [in,out] */
    unsigned32                  *new_buff_len;   /* [in,out] */
    error_status_t              *st_p;
#else
    (
    rsdb_compat_db_version_t    db_version,
    rs_log_hdr_dce101_t         *entryp,
    rs_log_hdr_t                **new_entryp,
    unsigned32                  *new_buff_len,
    error_status_t              *st_p
    )
#endif
{
    rs_log_acct_add_dce101_t        *old_acct_add;
    rs_log_acct_add_dce101_t        *old_acct_replace;
    rs_log_acct_rename_dce101_t     *old_acct_rename;
    rs_log_acl_replace_dce101_t     *old_acl_replace;
    rs_log_acct_add_dce102_t        *new_acct_add;
    rs_log_acct_add_dce102_t        *new_acct_replace;
    rs_log_acct_rename_dce102_t     *new_acct_rename;
    rs_log_acl_replace_dce102_t     *new_acl_replace;
    char                            *old_buf, *new_buf;
    unsigned32                      size_required;
    unsigned32                      nb;
    rs_log_passwd_item_dce102_t     pw;
    sec_timeval_sec_t               time_now;
    sec_rgy_name_t                  acl_component_name;

    CLEAR_STATUS(st_p);

    if(db_version != rsdb_c_db_version_dce101) {
        SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
        return;
    }

    /* Determine the buffer size required for the 
     * new format for the log entry in entryp
     */
    size_required = entryp->upd_reclen;

    switch(entryp->upd_module) {
    case RS_LOG_MODULE_ACCT:
        switch(entryp->upd_op) {
        case rs_log_acct_add_op:
            size_required += LOG_ACCT_ADD_FIXED_SZ_DIFF;
            break;
        case rs_log_acct_delete_op:
            size_required += LOG_ACCT_DELETE_FIXED_SZ_DIFF;
            break;
        case rs_log_acct_replace_op:
            size_required += LOG_ACCT_REPLACE_FIXED_SZ_DIFF;
            break;
        case rs_log_acct_rename_op:
            size_required += LOG_ACCT_RENAME_FIXED_SZ_DIFF;
            break;
        default:
            size_required += LOG_HDR_FIXED_SZ_DIFF;
            break;
        }
        break;
    case RS_LOG_MODULE_ACL:
        switch(entryp->upd_op) {
        case rs_log_acl_replace_op:
            size_required += LOG_ACL_REPLACE_FIXED_SZ_DIFF;
            break;
        default:
            size_required += LOG_HDR_FIXED_SZ_DIFF;
            break;
        }
        break;
    default:
        size_required += LOG_HDR_FIXED_SZ_DIFF;
        break;
    }


    /* Increase buffer size of new_entryp if necessary.
     */
    if(size_required > *new_buff_len) {
        if(!(new_buf = (char *)malloc(size_required))) {
            SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
            return;
        }
        free((char *)(*new_entryp));
        *new_entryp = (rs_log_hdr_t *)new_buf;
        *new_buff_len = size_required;
    }

    /* Copy info from entryp header into new_entryp header */
    (*new_entryp)->upd_reclen     = size_required;
    (*new_entryp)->upd_module     = (unsigned16)entryp->upd_module;
    (*new_entryp)->upd_op         = (unsigned16)entryp->upd_op;
    (*new_entryp)->upd_ts         = entryp->upd_ts;
    (*new_entryp)->upd_seqno.high = 0;
    (*new_entryp)->upd_seqno.low  = 0;

    switch(entryp->upd_module) {
    case RS_LOG_MODULE_ACCT:
        switch(entryp->upd_op) {
        case rs_log_acct_add_op:
            old_acct_add = (rs_log_acct_add_dce101_t *)entryp;
            new_acct_add = (rs_log_acct_add_dce102_t *)(*new_entryp);
            new_acct_add->client_ids = old_acct_add->client_ids;
            /* sec_passwd_des was the only legal keytype in dce101 */
            new_acct_add->key_type = sec_passwd_des;
            new_acct_add->item.data_parts = 
                old_acct_add->item.data_parts;
            new_acct_add->item.person_name_len = 
                old_acct_add->item.person_name_len;
            new_acct_add->item.group_name_len = 
                old_acct_add->item.group_name_len;
            new_acct_add->item.org_name_len = 
                old_acct_add->item.org_name_len;
            /* Copy in the variable length portion */
            nb = entryp->upd_reclen - sizeof(*old_acct_add);
            old_buf = (char *)entryp;
            old_buf += sizeof(*old_acct_add);
            new_buf = (char *)(*new_entryp);
            new_buf += sizeof(*new_acct_add);
            bcopy(old_buf, new_buf, nb);
            /* dce102 acct_add expects a unix passwd, put a 
             * NULL one into the log
             */
            new_buf += nb;
            pw.key_pickle_len = 0;
            pw.key_len = 0;
            pw.checksum_len = 0;
            bcopy( (Pointer) &pw, new_buf, sizeof(rs_log_passwd_item_dce102_t));
            break;
        case rs_log_acct_replace_op:
            old_acct_replace = (rs_log_acct_add_dce101_t *)entryp;
            new_acct_replace = (rs_log_acct_add_dce102_t *)(*new_entryp);
            rs_util_apply_timestamp(&time_now);
            new_acct_replace->client_ids = old_acct_replace->client_ids;
            /* sec_passwd_des was the only legal keytype in dce101 */
            new_acct_replace->key_type = sec_passwd_des;
            new_acct_replace->item.data_parts = 
                old_acct_replace->item.data_parts;
            new_acct_replace->item.person_name_len = 
                old_acct_replace->item.person_name_len;
            new_acct_replace->item.group_name_len = 
                old_acct_replace->item.group_name_len;
            new_acct_replace->item.org_name_len = 
                old_acct_replace->item.org_name_len;
            /* Copy in the variable length portion */
            nb = entryp->upd_reclen - sizeof(*old_acct_replace);
            old_buf = (char *)entryp;
            old_buf += sizeof(*old_acct_replace);
            new_buf = (char *)(*new_entryp);
            new_buf += sizeof(*new_acct_replace);
            bcopy(old_buf, new_buf, nb); 
            /* If the auth key/password was changed, dce102 expects
             * a UNIX passwd.  Put a NULL UNIX passwd into the log
             */
            if (new_acct_replace->item.data_parts & rs_acct_part_passwd) {
                new_buf += nb;
                pw.key_pickle_len = 0;
                pw.key_len = 0;
                pw.checksum_len = 0;
                bcopy( (Pointer) &pw, new_buf, sizeof(rs_log_passwd_item_dce102_t));
            }
            /* dce102 acct_replaces expect a sec_timeval_sec_t */
            new_buf += sizeof(rs_log_passwd_item_dce102_t);
            bcopy((char *)(&time_now), new_buf, sizeof(time_now));
            break;
        case rs_log_acct_rename_op:
            old_acct_rename = (rs_log_acct_rename_dce101_t *)entryp;
            new_acct_rename = (rs_log_acct_rename_dce102_t *)(*new_entryp);
            /* Copy in everything after old_acct_rename->new_key_parts */
            nb = entryp->upd_reclen - sizeof(old_acct_rename->upd_hdr) -
                 sizeof(old_acct_rename->new_key_parts);
            old_buf = (char *)entryp;
            old_buf += sizeof(old_acct_rename->upd_hdr) +
                       sizeof(old_acct_rename->new_key_parts);
            new_buf = (char *)(*new_entryp);
            new_buf += sizeof(new_acct_rename->upd_hdr);
            bcopy(old_buf, new_buf, nb);
            break;
        default:
            /* Copy in everything after the log_hdr */
            nb = entryp->upd_reclen - sizeof(*entryp);
            old_buf = (char *)entryp;
            old_buf += sizeof(*entryp);
            new_buf = (char *)(*new_entryp);
            new_buf += sizeof(*(*new_entryp));
            bcopy(old_buf, new_buf, nb);
            break;
        }
        break;
    case RS_LOG_MODULE_ACL:
        switch(entryp->upd_op) {
        case rs_log_acl_replace_op:
            old_acl_replace = (rs_log_acl_replace_dce101_t *)entryp;
            new_acl_replace = (rs_log_acl_replace_dce102_t *)(*new_entryp);
            rsdb_acl_get_name_from_ids(&(old_acl_replace->sec_acl_key), 
                                       acl_component_name, st_p);
            if(BAD_STATUS(st_p)) {
                return;
            }
            new_acl_replace->sec_acl_mgr = old_acl_replace->sec_acl_mgr;
            new_acl_replace->component_name_len = 
                u_strlen(acl_component_name);
            new_acl_replace->manager_type = old_acl_replace->manager_type;
            new_acl_replace->sec_acl_type = old_acl_replace->sec_acl_type;
            /* Copy in acl item */
            old_buf = (char *)entryp;
            old_buf += (sizeof(*old_acl_replace) - 
                        sizeof(old_acl_replace->item));
            new_buf = (char *)(*new_entryp);
            new_buf += (sizeof(*new_acl_replace) - 
                        sizeof(new_acl_replace->item));
            bcopy(old_buf, new_buf, old_acl_replace->item.acl_len);
            new_buf += old_acl_replace->item.acl_len;
            /* Copy in the component name */
            u_strncpy(new_buf, acl_component_name, 
                      new_acl_replace->component_name_len);
            break;
        default:
            /* Copy in everything after the log_hdr */
            nb = entryp->upd_reclen - sizeof(*entryp);
            old_buf = (char *)entryp;
            old_buf += sizeof(*entryp);
            new_buf = (char *)(*new_entryp);
            new_buf += sizeof(*(*new_entryp));
            bcopy(old_buf, new_buf, nb);
            break;
        }
        break;
    default:
        /* Copy in everything after the log_hdr */
        nb = entryp->upd_reclen - sizeof(*entryp);
        old_buf = (char *)entryp;
        old_buf += sizeof(*entryp);
        new_buf = (char *)(*new_entryp);
        new_buf += sizeof(*(*new_entryp));
        bcopy(old_buf, new_buf, nb);
        break;
    }
}


/*
 *  r s d b _ c o m p a t _ l o g _ r e p l a y 
 *
 *  This operation (comparable to rs_log_replay in rs_log.c)
 *  allows the caller to specify a software version so that
 *  a Registry database of the specified version may be read
 *  and converted to the current version.
 */
PRIVATE void rsdb_compat_log_replay
#ifndef __STDC__
    ( db_version, st_p )
    rsdb_compat_db_version_t    db_version;      /* [in] */
    error_status_t              *st_p;
#else
    (
    rsdb_compat_db_version_t    db_version,
    error_status_t              *st_p
    )
#endif
{
    rs_log_base_rec_dce101_t    log_base_info, *lbp;
    rs_log_hdr_dce101_t         *entryp;
    rs_log_hdr_t                *new_entryp;
    unsigned32                  buff_len, new_buff_len;
    unsigned long               rs_log_len;
    unsigned32                  nb;

    CLEAR_STATUS(st_p);
    lbp = &log_base_info;

    dce_svc_printf(SEC_RGY_COMPAT_LOG_REPLAY_MSG);

    if(db_version != rsdb_c_db_version_dce101) {
        SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
        return;
    }

    rsdb_log_close();
    rsdb_log_open();
    if(rs_log_fd < 0) {
	/*** SVC_OPEN : Can never be reached since  rsdb_log_open()
	 *              aborts on error 
	 */
        error("Unable to open log file");
        SET_STATUS(st_p, sec_rgy_rep_cannot_open_db);
        return;
    }

    if(read(rs_log_fd, (char *)lbp, sizeof(*lbp)) != sizeof(*lbp)) {
	dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
		       sec_s_rs_rsdb,
		       svc_c_sev_error,
		       sec_sys_file_read_error,
		       rs_log_fd);
        SET_STATUS(st_p, sec_rgy_rep_cannot_read_db);
        rsdb_log_close();
        return;
    }
    
    if(lbp->version != rs_c_log_file_version_dce101) {
	dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
		       sec_s_rs_rsdb,
		       svc_c_sev_error, 
		       sec_rgy_rep_bad_db_version);
        SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
        rsdb_log_close();
        return;
    }

    /* Allocate a buffer to read the entries into
     */
    if(!(entryp = 
         (rs_log_hdr_dce101_t *)malloc(rs_c_log_read_buffsize_dce102))) {
        SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        return;
    }
    buff_len = rs_c_log_read_buffsize_dce102;

    /* Allocate a buffer into which old log entries,
     * translated to the current format, are copied.
     */
    if(!(new_entryp = 
         (rs_log_hdr_t *)malloc(rs_c_log_read_buffsize_dce102))) {
        SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        return;
    }
    new_buff_len = rs_c_log_read_buffsize_dce102;

    /* Enable use of rpc_ss_allocate for decoding pickles
     */
    rpc_ss_enable_allocate();

    for( rs_log_len = sizeof(*lbp); rs_log_len < lbp->log_len;
         rs_log_len += entryp->upd_reclen ) {
        rsdb_compat_log_read_entry(db_version, rs_log_fd, &entryp, 
                                   &buff_len, st_p);
        if(BAD_STATUS(st_p)) {
            goto err_out;
        }

        if (entryp->upd_module == RS_LOG_MODULE_NONE) continue;

        /*
         * If the update is a GLOBAL update and is later than the
         * last checkpoint, then apply it to the database.
         */
        if (rsdb_compat_cmp_clock(entryp->upd_ts, lbp->checkpt_ts) > 0) {
            rsdb_compat_translate_log_entry(db_version, entryp, 
                                &new_entryp, &new_buff_len, st_p);
            if (BAD_STATUS(st_p)) {
                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                               sec_s_rs_rsdb,
                               svc_c_sev_warning,
                               *st_p);
                dce_svc_printf(SEC_RSDB_ENT_NOT_XLATED_MSG) ;
                CLEAR_STATUS(st_p);
                continue;
            }

            /* Call the dce102 version log replay operation */
            rs_log_replay_entry(new_entryp, st_p);

            /* Report a warning if operation is out of range,
             * then continue.
             */ 
            if(STATUS_EQUAL(st_p, sec_rgy_log_entry_out_of_range)) {
                dce_svc_printf(SEC_LOGENT_OUT_OF_BOUNDS_MSG);
                CLEAR_STATUS(st_p);
                continue;
            }

            if (BAD_STATUS(st_p)) {
                dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                               sec_s_rs_rsdb,
                               svc_c_sev_warning,
                               *st_p);
                dce_svc_printf(SEC_RSDB_LOGENT_REPLAY_ERR_MSG);
                CLEAR_STATUS(st_p);
                continue;
            }
        }
    } /* for rs_log_len ... */

err_out:

    free(entryp);
    free(new_entryp);
    /*
     * Release any storage allocated by unpickling routines.
     */
    rpc_ss_disable_allocate();
}


/*
 *  r s d b _ c o m p a t _ l o a d _ i n i t 
 *
 *  This operation (comparable to rsdb_load_init in rs_rsdb.c)
 *  allows the caller to specify a software version so that
 *  a Registry database of the specified version may be read
 *  and converted to the current version.
 */
PRIVATE void rsdb_compat_load_init
#ifndef __STDC__
    ( db_version, seqno, rep_twrs, state_fd, st_p )
    rsdb_compat_db_version_t    db_version;     /* [in] */
    rs_update_seqno_t           *seqno;         /* [in] */
    rs_replica_twr_vec_p_t      rep_twrs;       /* [in] */
    int                         *state_fd;      /* [out] */
    error_status_t              *st_p;
#else
    (
    rsdb_compat_db_version_t    db_version,
    rs_update_seqno_t           *seqno,
    rs_replica_twr_vec_p_t      rep_twrs,
    int                         *state_fd,
    error_status_t              *st_p
    )
#endif
{

    CLEAR_STATUS(st_p);

    if(db_version != rsdb_c_db_version_dce101) {
        SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
        return;
    }

    rsdb_compat_state_init(db_version, state_fd, st_p);
    if(BAD_STATUS(st_p)) return;

    rsdb_compat_load(db_version, rep_twrs, st_p);
    if(BAD_STATUS(st_p)) return;

    rsdb_compat_master_info_init(db_version, seqno, st_p);
    if(BAD_STATUS(st_p)) return;

    rsdb_compat_mkey_init(db_version, rs_state.rep_state, st_p);
    if(BAD_STATUS(st_p)) return;

    rsdb_compat_log_replay(db_version, st_p);
    if(BAD_STATUS(st_p)) return;
}


/* r s d b _ c o m p a t _ l o g _ c r e a t e
 *
 *  This operation (comparable to rs_log_create in rs_log.c)
 *  allows the caller to specify a software version so that
 *  a Registry database of the specified version may be read
 *  and converted to the current version.
 *
 *  A new log file (with no entries) is created with postfix ".new."
 */
PRIVATE void rsdb_compat_log_create
#ifndef __STDC__
    ( old_db_version, update_seqno, st_p )
    rsdb_compat_db_version_t    old_db_version;      /* [in] */
    rs_update_seqno_t           *update_seqno;       /* [in] */
    error_status_t              *st_p;
#else
  (
    rsdb_compat_db_version_t    old_db_version,
    rs_update_seqno_t           *update_seqno,
    error_status_t              *st_p
  )
#endif
{
    rs_log_base_rec_dce102_t    log_base_info,
                                *lbp;
    int                         new_fd;
    sec_rgy_name_t              new_log_fname;

    CLEAR_STATUS(st_p);

    if(old_db_version != rsdb_c_db_version_dce101) {
        SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
        return;
    }
    lbp = &log_base_info;

    /*
     * Guarantee empty base record
     */
    bzero((Pointer) lbp, sizeof(*lbp));
    
    lbp->log_len         = sizeof(*lbp);
    lbp->version         = rs_c_log_file_version_dce102;
    lbp->update_seqno    = *update_seqno;
    rs_util_gettime(&lbp->update_ts);
    lbp->base_prop_seqno = *update_seqno;

    strcpy( (char *) new_log_fname, LOG_FILE);
    strcat( (char *) new_log_fname, ".new");
    new_fd = rsdb_log_open_logfile((char *) new_log_fname,
                                   O_RDWR | O_CREAT | O_TRUNC);
    if (new_fd < 0) {
	/* SVC message already logged - no logging required */
        SET_STATUS(st_p, sec_rgy_rep_cannot_create_db);
        return;
    }
    if(write(new_fd, (char *)lbp, sizeof(*lbp)) != sizeof(*lbp)) {
	dce_svc_printf(DCE_SVC(sec_svc_handle, "%d"),
		       sec_s_rs_rsdb,
		       svc_c_sev_error,
		       sec_sys_file_write_error,
		       new_fd);
        SET_STATUS(st_p, sec_rgy_rep_cannot_create_db);
        return;
    }
    rsdb_file_fsync(new_fd);
    rsdb_file_close(new_fd);
}


/* r s d b _ c o m p a t _ c h e c k p t
 *
 *  This operation (comparable to rsdb_checkpt in rs_rsdb.c)
 *  allows the caller to specify a software version so that
 *  a Registry database of the specified version may be read
 *  and converted to the current version.
 *
 *  The current version is saved to the ".new" files.
 *  A new log file (with no entries) is created with ".new."
 *  The normal procedure is used to rename the files.
 */
PRIVATE void rsdb_compat_checkpt
#ifndef __STDC__
    ( old_db_version, lock_mode, seqno, state_fd, st_p )
    rsdb_compat_db_version_t    old_db_version;      /* [in] */
    lock_mode_t                 lock_mode;           /* [in] */
    rs_update_seqno_t           *seqno;              /* [in] */
    int                         state_fd;            /* [in] */
    error_status_t              *st_p;
#else
  (
    rsdb_compat_db_version_t    old_db_version,
    lock_mode_t                 lock_mode,
    rs_update_seqno_t           *seqno,
    int                         state_fd,
    error_status_t              *st_p
  )
#endif
{
    int new_log_len;

    CLEAR_STATUS(st_p);

    if(old_db_version != rsdb_c_db_version_dce101) {
        SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
        return;
    }

    rsdb_delete_files(".new", st_p);
    if (BAD_STATUS(st_p)) return;

    rsdb_save(".new", st_p);
    if (BAD_STATUS(st_p)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rsdb,
                       svc_c_sev_error,
                       *st_p);
        dce_svc_printf(SEC_RSDB_DB_CHKPT_ERR_MSG);
        return;
    }

    rsdb_compat_log_create(old_db_version, seqno, st_p);
    if (BAD_STATUS(st_p)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rsdb,
                       svc_c_sev_error,
                       *st_p);
        dce_svc_printf(SEC_RSDB_LOG_CHKPT_ERR_MSG);
        return;
    }

    if (lock_mode == lock_mode_riw)
        lock_promote_global();

    rsdb_compat_set_state(rs_c_state_renaming, state_fd, st_p);
    if (BAD_STATUS(st_p)) return;

    rsdb_close_files();

    rsdb_rename_files(".new", "", st_p);
    if (BAD_STATUS(st_p)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rsdb,
                       svc_c_sev_error,
                       *st_p);
        dce_svc_printf(SEC_RSDB_FILE_RENAME_ERR_MSG);
        return;
    }

    rsdb_open_files();

    rsdb_compat_set_state(rs_c_state_in_service, state_fd, st_p);
    if (BAD_STATUS(st_p)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rsdb,
                       svc_c_sev_error,
                       *st_p);
        dce_svc_printf(SEC_RSDB_REP_STATE_NOT_SAVED_MSG);
        return;
    }
}


/*
 *  r s d b _ c o m p a t _ r e f o r m a t _ d a t a b a s e 
 *
 *  Load the dce101-formatted database, converting it to dce102 
 *  format in memory.  Checkpoint the database to save the
 *  dce102 format to disk.  Close all database files so that
 *  normal database initialization may take place.
 * 
 */
PUBLIC void rsdb_compat_reformat_database
#ifndef __STDC__
    ( rep_twrs, st_p )
    rs_replica_twr_vec_p_t  rep_twrs;
    error_status_t          *st_p;
#else
    (
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st_p
    )
#endif
{
    unsigned32              save_rep_state;
    rs_update_seqno_t       seqno;
    int                     state_fd;     /* tmp file descriptor of state file */

    CLEAR_STATUS(st_p);

    /* Create the initial seqno since the 
     * 1.0.1 database doesn't have one 
     */
    rs_util_seqno_create(&seqno);

    /* Read dce101-formatted database */
    rsdb_compat_load_init(rsdb_c_db_version_dce101, &seqno, rep_twrs,
                          &state_fd, st_p);
    if(BAD_STATUS(st_p)) {
        return;
    }
    /* Save the rep_state.  In case of mkey_changing,
     * we want to restore saved state so that rs_mkey_change
     * may be retried.
     */
    save_rep_state = rs_state.rep_state;

    /* Save the dce102-formatted database to disk. */
    rsdb_compat_checkpt(rsdb_c_db_version_dce101,
                        lock_mode_write, &seqno, state_fd, st_p);
    if(BAD_STATUS(st_p)) {
        return;
    }
    rsdb_close_files();

    if(save_rep_state == rs_c_state_mkey_changing) {
        rsdb_compat_set_state(rs_c_state_mkey_changing, state_fd, st_p);
    }
    rsdb_file_fsync(state_fd);
    rsdb_file_close(state_fd);
}


/*
 *  r s d b _ c o m p a t _ p o l i c y _ r e p l i s t _ a c l s
 *
 *  DCE 1.0.2 requires a new acl for the replist.
 *  The replist acl must contain the Admin ('A') 
 *  permission bit that was previously (dce 1.0.1)
 *  stored on the policy acl.  
 *  Look up the policy acl.  For each of its entries
 *  that contains the 'A' bit, create an entry for the
 *  replist acl with the same key and the 'A' bit.
 *  Remove the 'A' bit from the policy acl entries.
 *  Create the new replist acl.  Use as the "creator"
 *  the key of the first policy acl entry (entry type user)
 *  to contain the control ('c') bit.
 */
PUBLIC void rsdb_compat_policy_replist_acls
#ifndef __STDC__
    ( local_cell, policy_acl_id, replist_acl_id, st_p )
    sec_id_t                *local_cell;        /* [in] */
    rsdb_pvt_id_t           policy_acl_id;      /* [in] */
    rsdb_pvt_id_t           *replist_acl_id;    /* [out] */
    error_status_t          *st_p;
#else
    (
    sec_id_t                *local_cell,
    rsdb_pvt_id_t           policy_acl_id,
    rsdb_pvt_id_t           *replist_acl_id,
    error_status_t          *st_p
    )
#endif
{
    rsdb_acl_t              *policy_acl;
    rsdb_acl_t              *replist_acl;
    unsigned32              i;
    rsdb_acl_entry_t        *acl_entry;
    boolean32               creator_found = false;
    unsigned32              num_rep_entries = 0;
    signed32                c_entry_index = -1;
    unsigned32              acl_len;
    sec_acl_permset_t       Admin_perm;

    CLEAR_STATUS(st_p);

    if(!rsdb_acl_get_by_id(policy_acl_id, &policy_acl, st_p)) {
        return;
    }

    for(i = 0; i < policy_acl->num_entries; i++) {
        acl_entry = &(policy_acl->entries[i]);
        /* The creator (max-privileges) entry must be
         * selected from among the policy acl entries
         * carrying the 'c' (owner) bit.  Select with
         * preference for entry type "user."
         */
        if(!creator_found && 
           IS_SET(acl_entry->perms, sec_acl_perm_owner)) {
            if(acl_entry->entry_type == sec_acl_e_type_user) {
                creator_found = true;
                c_entry_index = i;
                num_rep_entries++;
                continue;
            }
            c_entry_index = i;
        }
        if(IS_SET(acl_entry->perms, sec_acl_perm_Admin)) {
            num_rep_entries++;
        }
    }
    if(c_entry_index < 0) {
        /* policy acl has bad syntax */
        SET_STATUS(st_p, sec_acl_no_owner);
        return;
    }
    /* Calculate the number of entries required for the 
     * replist acl for the creator-not-found case.
     */
    if(!creator_found) {
        acl_entry = &(policy_acl->entries[c_entry_index]);
        /* if the c_entry_index entry has the Admin bit set,
         * it was already counted.  Else, increment num_rep_entries.
         */
        if(!IS_SET(acl_entry->perms, sec_acl_perm_Admin)) {
            num_rep_entries++;
        }
    }

    acl_len = sizeof(rsdb_acl_fixed_t) + 
              ((num_rep_entries - 1) * sizeof(rsdb_acl_entry_t));
    if(!(replist_acl = (rsdb_acl_t *)malloc(acl_len))) {
        SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        return;
    }
    replist_acl->acl_len = acl_len;

    replist_acl->default_realm.uuid = local_cell->uuid;
    replist_acl->default_realm.name_len = 0;
    replist_acl->sec_acl_manager_type = 
        rsdb_acl_mgr_uuids[rsdb_acl_mgr_type_replist];
    replist_acl->num_entries = num_rep_entries;

    /* Warning: the following acl_entry assignment code
     * assumes no name strings are stored in the acl_entries.
     */
    /* Create the max-privileges entry. */
    acl_entry = &(policy_acl->entries[c_entry_index]);
    replist_acl->entries[0] =  *acl_entry;
    replist_acl->entries[0].perms = sec_acl_perm_owner     |
                                    sec_acl_perm_insert    |
                                    sec_acl_perm_delete    |
                                    sec_acl_perm_mgmt_info |
                                    sec_acl_perm_Admin;
    /* num_rep_entries now used as index into replist_acl->entries */
    num_rep_entries = 1;
    /* Step through policy acl entries again to:
     * (1) copy entries with the 'A' bit to the replist acl
     * (2) remove the 'A' bit from policy acl entries
     */
    Admin_perm = sec_acl_perm_Admin;
    for(i = 0; i < policy_acl->num_entries; i++) {
        acl_entry = &(policy_acl->entries[i]);
        /* c_entry_index already included in replist_acl */
        if(i != c_entry_index &&
           (IS_SET(acl_entry->perms, sec_acl_perm_Admin))) {
            replist_acl->entries[num_rep_entries] = *acl_entry;
            replist_acl->entries[num_rep_entries].perms = 
                                            sec_acl_perm_Admin;
            num_rep_entries++;
        }
        acl_entry->perms &= ~Admin_perm;
    }

    rsdb_acl_store_new_acl(replist_acl, replist_acl_id);
    rsdb_acl_store_by_id(policy_acl_id, policy_acl);
    free(policy_acl);
    free(replist_acl);
}



/*
 * all rsdb_compat_dce11_*() are for dce102 to dce11
 * database conversion.
 */



/*
 *  r s d b _ c o m p a t _ d c e 1 1 _ s a m e _ r g y ...
 *
 *  Check if the rgy file size is the same as dce102 rgy file size;
 *  if so, returns true.  Since database conversion depends on
 *  rs_c_state_file_version get updated.  For DCE 1.1, the version
 *  was udpated after the database had been changed.  To save
 *  developers from re-configuring their cell, this check ensures
 *  database conversion will happen if rgy file size is different.
 *
 */
PRIVATE boolean32 rsdb_compat_dce11_same_rgy_size
   (
    error_status_t              *st_p
   )
{
    struct stat                     rgy_fstat;
    int                             fd;

    CLEAR_STATUS(st_p);

    if ( rsdb_file_stat(RGY_FILE, &rgy_fstat) != 0 ) {
        SET_STATUS(st_p, sec_rgy_rep_cannot_open_db);
        return false;
    }

    if(rgy_fstat.st_size == sizeof(rsdb_rgy_file_info_dce102_t)) {
	return true;
    }
    else {
 	return false;
    }
}



/*
 *  r s d b _ c o m p a t _ d c e 1 1 _ l o a d _ r g y _ i n f o 
 *
 *  Convert rgy_info from DCE 1.0.2 to DCE 1.1 data structure.
 */
PRIVATE void rsdb_compat_dce11_load_rgy_info
    (
    error_status_t              *st_p
    )
{
    unsigned32                      len;
    int                             fd;
    rsdb_rgy_file_info_dce102_t     old_rgy_info;

    CLEAR_STATUS(st_p);

    fd = rsdb_file_open(RGY_FILE, O_RDWR, 0);
    if (fd == -1) {
        SET_STATUS(st_p, sec_rgy_rep_cannot_open_db);
        return;
    }

    len = read(fd, (char *)&old_rgy_info, sizeof(old_rgy_info));
    if(len != sizeof(old_rgy_info)) {
        SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
        return;
    }

    rsdb_init_rgy_info_dce11(old_rgy_info, st_p);
    if(BAD_STATUS(st_p)) {
        return;
    }

    rsdb_store_rgy_info(fd);
    rsdb_file_close(fd);

}



/*
 *  r s d b _ c o m p a t _ d c e 1 1 _ c o n v e r t _ p g o d i r
 *
 *  Convert pgo from DCE 1.0.2 to DCE 1.1 data structure.
 */
PRIVATE void rsdb_compat_dce11_convert_pgodir_item
    (
    rsdb_named_item_dce102_t   	old_data,
    rsdb_named_item_dce11_t     *new_data_p,
    error_status_t		*st_p
    )
{

    CLEAR_STATUS(st_p);
    if ( old_data.type == rsdb_pgo_dce102_type ) {
	new_data_p->type = rsdb_pgo_type_dce11;
	new_data_p->body.pgo.threads = old_data.body.pgo.threads;
    	new_data_p->body.pgo.membership = old_data.body.pgo.membership;
    	new_data_p->body.pgo.num_members = old_data.body.pgo.num_members;
    	new_data_p->body.pgo.info = old_data.body.pgo.info;
    	new_data_p->body.pgo.unix_id = old_data.body.pgo.unix_id;
    	new_data_p->body.pgo.num_foreign_members = 0;
    	new_data_p->body.pgo.foreign_membership = END_OF_LIST;
    	new_data_p->body.pgo.fullname_len = old_data.body.pgo.fullname_len;
    	new_data_p->body.pgo.flags = old_data.body.pgo.flags;
    	new_data_p->body.pgo.quota = old_data.body.pgo.quota;
    	new_data_p->body.pgo.num_attrs = 0;
    	new_data_p->body.pgo.attr_list_id = END_OF_LIST;
    	strncpy((char *)new_data_p->body.pgo.fullname, (char *)old_data.body.pgo.fullname,
        	                old_data.body.pgo.fullname_len);
    }
    else if ( old_data.type == rsdb_dir_dce102_type ) {
	new_data_p->type = rsdb_dir_type_dce11;
	new_data_p->body.dir.threads = old_data.body.dir.threads;
	new_data_p->body.dir.uid = old_data.body.dir.uid;
	new_data_p->body.dir.acl = old_data.body.dir.acl;
	new_data_p->body.dir.def_dir_acl = old_data.body.dir.def_dir_acl;
	new_data_p->body.dir.def_obj_acl = old_data.body.dir.def_obj_acl;
	new_data_p->body.dir.num_attrs = 0;
	new_data_p->body.dir.attr_list_id = END_OF_LIST;
	new_data_p->body.dir.reserved = old_data.body.dir.reserved;
    }
    else {
	SET_STATUS(st_p, sec_rgy_bad_data);
	return;
    }

}



/*
 *  r s d b _ c o m p a t _ d c e 1 1 _ c o n v e r t _ b y _ i d
 *
 *  Convert a pgo record, identified by id,
 *  from DCE 1.0.2 to DCE 1.1 data structure.
 */
PRIVATE void rsdb_compat_dce11_convert_by_id
    (
    rsdb_pvt_id_t		id,
    db_handle			domain_db,
    error_status_t              *st_p
    )
{
    rsdb_name_key_dce11_t 	name_key;
    int             	name_key_len;
    int             	data_len;
    rsdb_named_item_dce102_t   old_data;
    rsdb_named_item_dce11_t    new_data;
    int                 new_data_len;
    char                *data;
    rsdb_sequential_key_dce11_t  sequential_key;

    bzero( (Pointer) &sequential_key, sizeof(sequential_key));
    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = id;

    bzero((Pointer) &name_key, sizeof(name_key));
    rsdb_fetch(domain_db, (Pointer) &sequential_key,
               sizeof(sequential_key), &data, &name_key_len, st_p);
    if(BAD_STATUS(st_p)) {
        return;
    }
    if (name_key_len > sizeof(rsdb_named_item_dce102_t)) {
        dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                    sec_s_rs_rsdb,
                    svc_c_sev_fatal | svc_c_action_abort,
                    sec_buf_too_small);
    } else {
        bcopy(data, (Pointer) &name_key, name_key_len);
        rsdb_fetch(domain_db, (Pointer) &name_key, name_key_len,
		   &data, &data_len, st_p);
        if(BAD_STATUS(st_p)) {
            return;
        }
        bcopy(data, (Pointer) &old_data, data_len);
    }

    rsdb_compat_dce11_convert_pgodir_item(old_data, &new_data, st_p);
    if(BAD_STATUS(st_p)) {
        return;
    }
    new_data_len = NAMED_ITEM_LEN(&new_data); 
    rsdb_replace(domain_db, (Pointer) &name_key, name_key_len,
		 (Pointer) &new_data, new_data_len, st_p);
    if (BAD_STATUS(st_p)) {
        return;
    }
}



/*
 *  r s d b _ c o m p a t _ d c e 1 1 _ c o n v e r t _ r e p _ i t e m
 *  
 */
PRIVATE void rsdb_compat_dce11_convert_rep_item
    (
    rsdb_replica_item_dce102_t  *old_rep_data,
    rsdb_replica_item_dce11_t   **new_rep_data,
    error_status_t              *st_p
    )
{
    rsdb_replica_item_dce11_t	*new_rp;

    new_rp = *new_rep_data;
    memcpy(&new_rp->replica.rep_id, &old_rep_data->replica.rep_id, 
		sizeof(uuid_t));
    /* 
     * assume rep_name and rep_twrs are kept in rep_buffers
     * when stored in disk.  Those information are reconstructed
     * when reading into memory and rs_rep_util_rep_buff_unpack
     * is called.
     */ 
    new_rp->replica.rep_name = (rs_replica_name_p_t) 0;
    new_rp->replica.rep_twrs = (rs_replica_twr_vec_p_t) 0;
    new_rp->replica.prop_type = old_rep_data->replica.prop_type;
    new_rp->replica.prev_prop_type = rs_c_replica_prop_unknown;
    memcpy(&new_rp->replica.init_id, &old_rep_data->replica.init_id,
		sizeof(uuid_t));
    new_rp->replica.alloc_type = old_rep_data->replica.alloc_type;
    new_rp->replica.prop_mgt = old_rep_data->replica.prop_mgt; 
    new_rp->rep_buffers_len = old_rep_data->rep_buffers_len;
    memcpy(&new_rp->rep_buffers, &old_rep_data->rep_buffers,
		old_rep_data->rep_buffers_len);
}



/*
 *  r s d b _ c o m p a t _ d c e 1 1 _ c o n v e r t _ p g o
 *  
 */
PRIVATE void rsdb_compat_dce11_convert_pgo
    (
    error_status_t              *st_p
    )
{
    rsdb_name_key_dce11_t 	name_key;
    rsdb_name_key_dce11_t 	next_key;
    int             	name_key_len;
    int             	next_key_len;
    rsdb_named_item_dce102_t   old_data;
    rsdb_named_item_dce11_t    new_data;
    int                 new_data_len;
    int             	old_data_len;
    sec_rgy_domain_t    domain;
    char		*nkey;
    Pointer             data;
    rsdb_sequential_key_dce11_t  sequential_key;

    CLEAR_STATUS(st_p);

    /* convert pgodir */
    for (domain = 0; domain < sec_rgy_domain_last; domain++) {

	/* for each domain, convert its head and tail */

	rsdb_compat_dce11_convert_by_id(START_OF_LIST, domain_info[domain].db,
		st_p);
    	if(BAD_STATUS(st_p)) {
            return;
	}

	rsdb_compat_dce11_convert_by_id(END_OF_LIST, domain_info[domain].db,
		st_p);
    	if(BAD_STATUS(st_p)) {
            return;
	}

        /* convert real data */

        bzero((Pointer) &name_key, sizeof(name_key));
        name_key.record_type = NAME_TYPE;
        name_key.parent_id = START_OF_LIST;
        name_key.name_len = 0;
        name_key_len = NAME_LEN(&name_key);

	while ( true ) {
    	    rsdb_fetch_next(domain_info[domain].db, (Pointer) & name_key, 
		    name_key_len, &nkey, &next_key_len,
                    &data, &old_data_len, st_p);

            if (BAD_STATUS(st_p)) {
            	return;
    	    } 
	    else {
                if (old_data_len > sizeof(rsdb_named_item_dce102_t)) {
                    dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_rsdb,
                           svc_c_sev_fatal | svc_c_action_abort,
                           sec_buf_too_small);
                } else {
	            bcopy(nkey, (Pointer) &next_key, next_key_len);
	            if (next_key.record_type != NAME_TYPE) {
                        break;
            	    }	
            	    bcopy(data, (Pointer) &old_data, old_data_len);
		}
            }
	    rsdb_compat_dce11_convert_pgodir_item(old_data, &new_data, st_p);
    	    if(BAD_STATUS(st_p)) {
        	return;
	    }

	    new_data_len = NAMED_ITEM_LEN(&new_data);
	    rsdb_replace(domain_info[domain].db, (Pointer) &next_key, 
		 next_key_len, (Pointer) &new_data, new_data_len, st_p);
    	    if(BAD_STATUS(st_p)) {
        	return;
	    }
        
	    name_key.parent_id = next_key.parent_id;
            name_key.name_len = next_key.name_len;
    	    strncpy((char *) name_key.name, (char *) next_key.name, 
				next_key.name_len);
            name_key_len = NAME_LEN(&next_key);
	
    	}
    }

}



/*
 *  r s d b _ c o m p a t _ d c e 1 1 _ c o n v e r t _ a c c t
 *
 */
PRIVATE void rsdb_compat_dce11_convert_acct
    (
    error_status_t              *st_p
    )
{
    rsdb_sequential_key_dce11_t  sequential_key;
    char                   *data;
    int                    data_len;
    rsdb_acct_item_dce11_t   	   new_account;
    rsdb_acct_login_activity_t     login_activity;
    rsdb_acct_journal_t            journal;

    bzero((Pointer) &sequential_key, sizeof(sequential_key));
    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = START_OF_LIST;

    while ( true ) {
    	rsdb_fetch(rsdb_acct_db, (Pointer) &sequential_key, 
		sizeof(sequential_key), &data, &data_len, st_p);
    	if (BAD_STATUS(st_p)) {
	    break;
	}
    	if (data_len <= sizeof(new_account)) {
            bcopy(data, (Pointer) &new_account, data_len);
	} else {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                       sec_s_rs_rsdb,
                       svc_c_sev_fatal | svc_c_action_abort,
                       sec_buf_too_small);
            }

    	new_account.login_activity_id = rsdb_new_login_id();

    	rsdb_replace(rsdb_acct_db, (Pointer) &sequential_key,
                 sizeof(sequential_key), (Pointer) &new_account, 
		 data_len, st_p);
    	if (BAD_STATUS(st_p)) {
	    break;
	}

        if (new_account.threads.id == END_OF_LIST) {
            break;
        } else {
    	    bzero((Pointer) &sequential_key, sizeof(sequential_key));
    	    sequential_key.record_type = SEQUENTIAL_TYPE;
            sequential_key.id = new_account.threads.next;
        }
    }

}



/*
 *  r s d b _ c o m p a t _ d c e 1 1 _ c o n v e r t _ r e p l i c a
 *
 */
PRIVATE void rsdb_compat_dce11_convert_replica
    (
    error_status_t              *st_p
    )
{
    uuid_t                  cursor;
    rsdb_replica_item_dce102_t   *rp;
    rsdb_replica_item_dce11_t    *new_rep_item_p;
    char         		 *data;
    int                     data_len;
    uuid_p_t                next_id;
    int                     next_id_len;
    error_status_t          lst;

    uuid_create_nil(&cursor, &lst);
    do {
        rsdb_fetch_next(rsdb_replica_db, (Pointer) &cursor, sizeof(cursor),            
            (Pointer *) &next_id, &next_id_len,
            (Pointer *) &data, &data_len, st_p);
	if ( STATUS_EQUAL(st_p, sec_rgy_object_not_found) ) {
	    CLEAR_STATUS(st_p);
	    return;
	}

    	rp = (rsdb_replica_item_dce102_t *) data;
	data_len += sizeof(rs_replica_prop_t);
	new_rep_item_p = (rsdb_replica_item_dce11_t *) 
		malloc(data_len);
	rsdb_compat_dce11_convert_rep_item(rp, &new_rep_item_p, st_p);
        if ( BAD_STATUS(st_p) ) {
	    return;
	}

    	rsdb_replace(rsdb_replica_db, (Pointer) &new_rep_item_p->replica.rep_id, 
        	sizeof(new_rep_item_p->replica.rep_id),
		(Pointer) new_rep_item_p, data_len, st_p);
        if ( BAD_STATUS(st_p) ) {
	    return;
	}

	free(new_rep_item_p);
        memcpy((void *) &cursor, (void *) next_id, sizeof(cursor));

    } while ( ! uuid_is_nil(&cursor, &lst) );
}



/*
 *  r s d b _ c o m p a t _ d c e 1 1 _ f i l e _ c o n v e r s i o n
 *
 */
PRIVATE void rsdb_compat_dce11_file_conversion
    (
    error_status_t              *st_p
    )
{

    CLEAR_STATUS(st_p);

    rsdb_compat_dce11_convert_pgo(st_p);
    if(BAD_STATUS(st_p)) {
        return;
    }

    rsdb_compat_dce11_convert_acct(st_p);
    if(BAD_STATUS(st_p)) {
        return;
    }

    rsdb_compat_dce11_convert_replica(st_p);
    if(BAD_STATUS(st_p)) {
        return;
    }
   
}



/* r s d b _ c o m p a t _ d c e 1 1 _ c h e c k p t
 *
 * Save the in-memory copy of the database to disk.
 * Set state to next_rep_state after database has
 * been saved.
 *
 * Invoking routine must lock_global_riw or lock_global_write 
 * if it needs a lock (ie. is multi-threaded or rpc_listening)
 *
 * Compare with rsdb_checkpt ( rs_rsdb.c ), this routine
 * does not issue rs_log_truncate.
 */
PRIVATE void rsdb_compat_dce11_checkpt
  (
    unsigned32      next_rep_state,
    lock_mode_t     lock_mode,
    error_status_t  *st
  )
{

    CLEAR_STATUS(st);

    switch(rs_state.rep_state) {
        case rs_c_state_initializing: 
            break;
        case rs_c_state_uninitialized: 
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

    rsdb_delete_files(".new", st);
    if (BAD_STATUS(st)) return;

    rsdb_save(".new", st);
    if (BAD_STATUS(st))
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_RSDB_CHECKPT_MSG, *st);

    if (lock_mode == lock_mode_riw)
        lock_promote_global();

    rs_state_set_state_renaming(next_rep_state, st);
    if (BAD_STATUS(st)) return;

    rsdb_close_files();

    rsdb_rename_files(".new", "", st);
    if (BAD_STATUS(st))
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_CHECKPT_RENAME_FILES_MSG, *st);

    rsdb_open_files();

    rs_state_set_state(next_rep_state, st);
    if (BAD_STATUS(st))
        SEC_SVC_PRINT_MSG_ST(SEC_RGY_CHECKPT_SAVE_REP_STATE_MSG, *st);
}

/*
 *  r s d b _ c o m p a t _ r e f o r m a t _ d a t a b a s e ... 
 *
 *  Load the dce 102-formatted database, converting it to dce11
 *  format in memory.  Checkpoint the database to save the
 *  dce11 format to disk.  Close all database files so that
 *  normal database initialization may take place.
 * 
 */
PUBLIC void rsdb_compat_reformat_database_for_dce11
    (
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st_p
    )
{
    unsigned32              save_rep_state;
    rs_update_seqno_t       seqno;
    unsigned32		    compat_sw_rev;

    compat_sw_rev = rs_state_get_version();

    CLEAR_STATUS(st_p);

    if ( ( ! rsdb_compat_dce11_same_rgy_size(st_p) ) ||
	 ( compat_sw_rev == rsdb_c_db_version_pre_migrate_dce11 ) ) {
    /* 
     * If rgy file size is not dce 102 size, it implies database
     * could have been converted or database is created for dce11.
     * But replica rsdb is not converted yet, otherwise the version
     * number should equal to current setting - rsdb_c_db_verions_dce11.
     */
	CLEAR_STATUS(st_p);

    	rsdb_load(st_p);
    	if(BAD_STATUS(st_p)) {
            return;
    	}

	rs_state.version = rsdb_c_db_version_dce11;
    	rsdb_compat_state_open_set_version(&rs_state, st_p);
    	if(BAD_STATUS(st_p)) {
            return;
    	}

	rsdb_compat_dce11_convert_replica(st_p);
    	if(BAD_STATUS(st_p)) {
            return;
    	}
    }
    else { /* a conversion from dce 1.0.2 to 1.1 */

    	/* create database files that are new for DCE 1.1 */
    	rsdb_compat_dce11_create();

    	rsdb_load_relations(st_p);
    	if(BAD_STATUS(st_p)) {
            return;
    	}

    	/* convert rgy_info file */
    	rsdb_compat_dce11_load_rgy_info(st_p);
    	if(BAD_STATUS(st_p)) {
            return;
    	}
    
	rsdb_state_open(&rs_state, st_p);
    	if( ! STATUS_EQUAL(st_p, sec_rgy_rep_bad_db_version)) {
            return;
    	}
	
    	rsdb_state_close();

    	rsdb_compat_state_open_set_version(&rs_state, st_p);
    	if(BAD_STATUS(st_p)) {
            return;
    	}

    	/* Read and convert dce102-formatted database */
    	rsdb_compat_dce11_file_conversion(st_p);
    	if(BAD_STATUS(st_p)) {
            return;
    	}

    	/* add canned attribute entries */
    	rs_resv_add_attr_sch_entries(st_p);
    	if(BAD_STATUS(st_p)) {
            return;
    	}
    }

    /* Save the rep_state.  In case of mkey_changing,
     * we want to restore saved state so that rs_mkey_change
     * may be retried.
     */
    save_rep_state = rs_state.rep_state;
    rsdb_compat_dce11_checkpt(rs_state.rep_state, lock_mode_write, st_p);

    rsdb_close_files();

    if(save_rep_state == rs_c_state_mkey_changing) {
        rs_state_set_state(rs_c_state_mkey_changing, st_p);
    }

    rsdb_state_close();

}

PUBLIC void rsdb_compat_convert_master_info_for_dce11
    (
    rs_master_info_t	*master_info_p,
    error_status_t	*st_p
    )
{
    if ( master_info_p->version == rs_c_master_info_file_version_dce103  &&
         rs_c_master_info_file_version == rs_c_master_info_file_version_dce11 ) {
	CLEAR_STATUS(st_p);
        WRITE_LOCK(lock_master_info) {
	    master_info_p->global_sw_rev = rs_c_software_vers_1_0_2;
	    master_info_p->version = rs_c_master_info_file_version_dce11;
            rsdb_master_info_write(master_info_p, st_p);
        } END_WRITE_LOCK;
    }
    else {
        SET_STATUS(st_p, sec_rgy_rep_bad_db_version);
    }
}


PUBLIC void rsdb_compat_dce122_foreign_person_create
    (
    error_status_t          *st_p
    )
{
    rsdb_sequential_key_t  sequential_key;
    rsdb_name_key_t        name_key;
    int                    name_key_len;
    int                    item_data_len;
    sec_rgy_domain_t	   domain;

    domain = sec_rgy_domain_foreign_person;

    /* create database dummy head and tail record */
    bzero((Pointer) &sequential_key, sizeof(sequential_key));
    bzero((Pointer) &name_key, sizeof(name_key));

    name_key.record_type  = DUMMY_TYPE;
    name_key.parent_id    = END_OF_LIST;

    sequential_key.record_type = SEQUENTIAL_TYPE;
    sequential_key.id = START_OF_LIST;
    bzero((Pointer) &domain_info[domain].first, 
		sizeof(domain_info[domain].first));
    domain_info[domain].first.type = rsdb_dir_type;
    item_data_len = NAMED_ITEM_LEN(&domain_info[domain].first);
    domain_info[domain].first.body.threads.id = START_OF_LIST;
    domain_info[domain].first.body.threads.next = END_OF_LIST;
    domain_info[domain].first.body.threads.prev = END_OF_LIST;

    RSDB_DIR(&domain_info[domain].first).num_attrs = 0;
    RSDB_DIR(&domain_info[domain].first).attr_list_id = END_OF_LIST;
    RSDB_DIR(&domain_info[domain].first).acl = 0;
    RSDB_DIR(&domain_info[domain].first).def_obj_acl = 0;
    RSDB_DIR(&domain_info[domain].first).def_dir_acl = 0;
    strcpy((char *) name_key.name, SEC_FOREIGN_PRINCIPAL_OBJ_NAME);
    name_key.name_len = strlen(SEC_FOREIGN_PRINCIPAL_OBJ_NAME);
    name_key_len = NAME_LEN(&name_key);

    rsdb_store(domain_info[domain].db, (Pointer) &name_key, name_key_len,
                    (Pointer) &domain_info[domain].first, item_data_len, 
		    st_p);
    rsdb_store(domain_info[domain].db, (Pointer) &sequential_key,
                        sizeof(sequential_key), (Pointer) &name_key,
                        name_key_len, st_p);

    name_key.name_len     = 4;
    strcpy((char *) name_key.name, "last");
    name_key_len = NAME_LEN(&name_key);

    sequential_key.id = END_OF_LIST;
    bzero((Pointer) &domain_info[domain].last, sizeof(domain_info[domain].last));
    domain_info[domain].last.type = rsdb_pgo_type;
    RSDB_PGO(&domain_info[domain].last).unix_id = -1;
    item_data_len = NAMED_ITEM_LEN(&domain_info[domain].last);
    domain_info[domain].last.body.threads.id = END_OF_LIST;
    domain_info[domain].last.body.threads.next = START_OF_LIST;
    domain_info[domain].last.body.threads.prev = START_OF_LIST;

    rsdb_store(domain_info[domain].db, (Pointer) &name_key, name_key_len,
                        (Pointer) &domain_info[domain].last, item_data_len, 
			st_p);
    rsdb_store(domain_info[domain].db, (Pointer) &sequential_key,
                        sizeof(sequential_key), (Pointer) &name_key,
                        name_key_len, st_p);

}


PUBLIC void rsdb_compat_dce122_foreign_person_init
    (
    )
{

    rsdb_util_get_item_by_id(sec_rgy_domain_foreign_person, START_OF_LIST, 
		&domain_info[sec_rgy_domain_foreign_person].first);
    rsdb_util_get_item_by_id(sec_rgy_domain_foreign_person, END_OF_LIST, 
		&domain_info[sec_rgy_domain_foreign_person].last);
}    


/*
 *  r s d b _ c o m p a t _ d c e 1 2 _ i n i t _ p g o _ f g
 *  
 */
PRIVATE void rsdb_compat_dce122_init_pgo_fgroup
    (
    error_status_t              *st_p
    )
{
    rsdb_name_key_dce11_t 	name_key;
    rsdb_name_key_dce11_t 	next_key;
    int             	name_key_len;
    int             	next_key_len;
    rsdb_named_item_dce11_t    new_data;
    int                 new_data_len;
    sec_rgy_domain_t    domain;
    char		*nkey;
    Pointer             data;
    rsdb_sequential_key_dce11_t  sequential_key;

    CLEAR_STATUS(st_p);

    /* 
     * retrieve all data in pgo domain, then init its pgo data
     * num_foreign_members and foreign_membership.  Those two
     * fields are reused from reserved1 and reserved2.
     */
    for (domain = 0; domain < sec_rgy_domain_last; domain++) {

        /* convert real data */

        bzero((Pointer) &name_key, sizeof(name_key));
        name_key.record_type = NAME_TYPE;
        name_key.parent_id = START_OF_LIST;
        name_key.name_len = 0;
        name_key_len = NAME_LEN(&name_key);

	while ( true ) {
    	    rsdb_fetch_next(domain_info[domain].db, (Pointer) & name_key, 
		    name_key_len, &nkey, &next_key_len,
                    &data, &new_data_len, st_p);

            if (BAD_STATUS(st_p)) {
            	return;
    	    } 
	    else {
                if (new_data_len > sizeof(rsdb_named_item_dce102_t)) {
                    dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                           sec_s_rs_rsdb,
                           svc_c_sev_fatal | svc_c_action_abort,
                           sec_buf_too_small);
                } else {
	            bcopy(nkey, (Pointer) &next_key, next_key_len);
	            if (next_key.record_type != NAME_TYPE) {
                        break;
            	    }	
            	    bcopy(data, (Pointer) &new_data, new_data_len);
		}
            }

    	    RSDB_PGO(&new_data).num_foreign_members = 0;
    	    RSDB_PGO(&new_data).foreign_membership = END_OF_LIST;

	    rsdb_replace(domain_info[domain].db, (Pointer) &next_key, 
		 next_key_len, (Pointer) &new_data, new_data_len, st_p);
    	    if(BAD_STATUS(st_p)) {
        	return;
	    }
        
	    name_key.parent_id = next_key.parent_id;
            name_key.name_len = next_key.name_len;
    	    strncpy((char *) name_key.name, (char *) next_key.name, 
				next_key.name_len);
            name_key_len = NAME_LEN(&next_key);
	
    	}
    }

}



/*
 *  r s d b _ c o m p a t _ r e f o r m a t _ d a t a b a s e ... 
 *
 *  Load the dce 1.1-formatted database, converting it to dce1.2.2
 *  format in memory.  Checkpoint the database to save the
 *  dce11 format to disk.  Close all database files so that
 *  normal database initialization may take place.
 * 
 */
PUBLIC void rsdb_compat_reformat_database_for_dce122
    (
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st_p
    )
{
    unsigned32              save_rep_state;

    CLEAR_STATUS(st_p);

    /* create and initialize foreign_person database */
    rsdb_foreign_person_create_init(st_p);
    if(BAD_STATUS(st_p)) {
        return;
    }

    rsdb_load(st_p);
    if(BAD_STATUS(st_p)) {
        return;
    }

    /* initialize foreign_group and num_foreign_gorup */
    rsdb_compat_dce122_init_pgo_fgroup(st_p);
    if (BAD_STATUS(st_p)) {
        return;
    } 

    /* Set rsdb_acct_item_privkey_ok flag on accounts where allowed */
    rsdb_auth_init_privkey_allowed(st_p);
    if (BAD_STATUS(st_p)) {
        return;
    } 

    /* save the new db version in rs_state file */
    rs_state.version = rsdb_c_db_version_dce122;
    rsdb_compat_state_open_set_version(&rs_state, st_p);
    if(BAD_STATUS(st_p)) {
        return;
    }

    /* do checkpoint */
    save_rep_state = rs_state.rep_state;
    rsdb_compat_dce11_checkpt(rs_state.rep_state, lock_mode_write, st_p);

    rsdb_close_files();

    if(save_rep_state == rs_c_state_mkey_changing) {
        rs_state_set_state(rs_c_state_mkey_changing, st_p);
    }

    rsdb_state_close();

}
