/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_rpladmn_util.c,v $
 * Revision 1.1.5.2  1996/02/18  00:20:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:13  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:56:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:21:37  root]
 * 
 * Revision 1.1.2.4  1992/12/29  16:37:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:26:46  zeliff]
 * 
 * Revision 1.1.2.3  1992/09/29  20:13:01  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:37:50  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/31  18:27:13  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  15:11:57  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:44:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_rpladmn_util.c V=4 11/20/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *     Registry Server - Replica administration utilities
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rs_rpladmn_util.c	4 - 11/20/91";
#endif

#include <sys/types.h>

#ifdef SNI_SVR4_POSIX
#include <unistd.h>
#endif /* SNI_SVR4_POSIX */

#include <sys/file.h>

#include <dce/nbase.h>
#include <dce/rplbase.h>

#include <rs_base.h>
#include <rs_pvt.h>
#include <rs_util.h>
#include <rs_lock.h>
#include <rs_rsdb.h>
#include <rs_audit_trail.h>
#include <rs_rpladmn_util.h>

#include <rsdb.h>
#include <rsdb_pvt.h>
#include <rsdb_log.h>
#include <rsdb_auth.h>
#include <rsdb_policy.h>


PUBLIC void rs_into_maintenance
#ifndef __STDC__
    ( handle, st )
    handle_t            handle;
    error_status_t      *(st);
#else
    (
    handle_t            handle,
    error_status_t      *(st)
    )
#endif
{

    lock_global(lock_mode_riw);

    if (!rs_state.master)
        SET_STATUS(st, sec_rgy_rep_not_master);
    else
    if (rs_state.rep_state != rs_c_state_in_service)
        SET_STATUS(st, sec_rgy_rep_bad_state);
    else
        CLEAR_STATUS(st);

    if (BAD_STATUS(st)) {
        unlock_global(lock_mode_riw);
        return;
    }

    rs_audit_trail_log(handle, "Into Maintenance state", (char *) NULL);

/* checkpt database, close files except state file, and promote RIW lock  */
    rsdb_shutdown();

    rs_state_save_state(rs_c_state_in_maintenance, rsdb_st_consistent, st);

    rs_state_close();

    unlock_global(lock_mode_riw);

/*REPLICA */
#ifdef NOTDEF
    /* Stop the prop tasks.  Replica list locks must not be held when this call is made. */
    rep_stop_prop_tasks();
#endif

    CLEAR_STATUS(st);
}

PUBLIC void rs_out_of_maintenance
#ifndef __STDC__
    ( handle, st )
    handle_t            handle;
    error_status_t      *(st);
#else
    (
    handle_t            handle,
    error_status_t      *(st)
    )
#endif
{
    lock_global(lock_mode_riw);

    if (!rs_state.master)
        SET_STATUS(st, sec_rgy_rep_not_master);
    else
    if (rs_state.rep_state != rs_c_state_in_maintenance)
        SET_STATUS(st, sec_rgy_rep_bad_state);
    else
        CLEAR_STATUS(st);

    if (BAD_STATUS(st)) {
        unlock_global(lock_mode_riw);
        return;
    }

    rs_audit_trail_log(handle, "Out of Maintenance state", (char *) NULL);

    /*
    * Make sure database area is properly protected
    */
    rsdb_tighten_directory_protection();
    rsdb_protect_files(true);

    lock_promote_global();

    /* rsdb_load will clear memory first */
    rsdb_initialize(st);
    if (BAD_STATUS(st))
        fatal_st(*st, "Cannot reload the database");

    if (rs_state.rep_state == rs_c_state_in_maintenance)
        rs_state_save_state(rs_c_state_in_service, rsdb_st_consistent, st);
    if (BAD_STATUS(st))
        fatal_st(*st, "Cannot save rgy_state file");

    /* Old rpcd registrations get bonked under rep_initialize. */
/*REPLICA */
#ifdef NOTDEF
    rep_initialize(st);
    if (BAD_STATUS(st))
        fatal_st(*st, "Cannot initialize master replica");
#endif

    unlock_global(lock_mode_riw);

    rsdb_start_checkpt_task(st);
    if (BAD_STATUS(st))
        fatal_st(*st, "Cannot start checkpoint task");

    /* Registration of query, update rs_rpladmn and related local_only interfaces */
/*     register_server(st);
    if (BAD_STATUS(st))
        fatal_st(*st, "Cannot register server");
*/

/*REPLICA */
#ifdef NOTDEF
    rep_start_task(st);
    if (BAD_STATUS(st))
        fatal_st(*st, "Cannot start propagation task");

    register_replist(st);
    if (BAD_STATUS(st))
        error_st(*st, "Cannot register replica list ");
#endif


    CLEAR_STATUS(st);
}


/*
 *  r s _ r e e n c r y p t _ d b _ k e y s
 *
 *  For each acct record in the database, call
 *  rsdb_auth_reencrypt_acct_key with the input
 *  new and old master_key info.
 *  Lock_mode should be lock_mode_write or lock_mode_riw.
 *  If this operation is called with lock_mode_riw,
 *  demote and promote lock every 150 accts so as
 *  to allow database queries in during large
 *  database reencryption.
 *  If lock_mode == lock_mode_write, don't promote or demote.
 *  Return to caller in the same lock_mode as at the start.
 */
PUBLIC void rs_reencrypt_db_keys
#ifndef __STDC__
    ( old_eblock, old_version, new_eblock, new_version, lock_mode, st )
    krb5_encrypt_block          *old_eblock;    /* [in] */
    krb5_kvno                   old_version;    /* [in] */
    krb5_encrypt_block          *new_eblock;    /* [in] */
    krb5_kvno                   new_version;    /* [in] */
    lock_mode_t                 lock_mode;      /* [in] */
    error_status_t              *(st);
#else
    (
    krb5_encrypt_block          *old_eblock,
    krb5_kvno                   old_version,
    krb5_encrypt_block          *new_eblock,
    krb5_kvno                   new_version,
    lock_mode_t                 lock_mode,
    error_status_t              *(st)
    )
#endif
{
    unsigned32              i;
    sec_rgy_cursor_t        cursor;

    CLEAR_STATUS(st);

    if (lock_mode == lock_mode_riw)
        READ_INTEND_WRITE_PROMOTE(lock_db);

    /* For each account in the database, decrypt its key
     * with the old master key and re-encrypt with the
     * new master key.  Demote the write lock every 150
     * accounts so as not to hog the database.
     */
    i = 0;
    cursor.valid = false;
    while( GOOD_STATUS(st) ) {
        i++;
        rsdb_auth_reencrypt_acct_key(&cursor, old_eblock,
            old_version, new_eblock, new_version, st);
        if( lock_mode == lock_mode_riw && i >= 150 ) {
            READ_INTEND_WRITE_DEMOTE(lock_db);
            READ_INTEND_WRITE_PROMOTE(lock_db);
            i = 0;
        }
    }
    if (lock_mode == lock_mode_riw)
        READ_INTEND_WRITE_DEMOTE(lock_db);
}

/*
 *  r s _ r e e n c r y p t _ d a t a b a s e
 *
 *  This operation is called by both rs_rep_admin_mkey in
 *  rs_rpladmn.c for the change_master_key function, and
 *  by rsdb_initialize to recover from a possible crash during
 *  the change master key function.
 *  Invoking routine must lock_global_riw or lock_global_write if it needs a
 *  lock (ie. is multi-threaded or rpc_listening).
 *  If no lock is required, call with lock_mode == lock_mode_write.
 *
 *  Crash scenarios:
 *  (1) Crash in rs_reencrypt_database before "mkey" renamed to "mkey.old"
 *      OR normal call by rs_rep_admin_mkey().
 *  (2) Crash after (1) and before new master key stored in "mkey."
 *  (3) Crash after (2) and before db_state gets changed to renaming
 *      inside rsdb_checkpt.
 *  (4) Crash after (3) and before "mkey.old" deleted.
 *  (5) Crash after (4)
 *      OR normal call by rsdb_initialize (no crash at all).
 *
 *  Recovery Code Algorithm:
 *  If db_state == rsdb_st_mkey_changing {
 *      If mkey file exists {
 *          If mkey.old doesn't exist {
 *  (1)         The mkey file contains the old mkey; no reencryption took place
 *              Create new mkey, reencrypt, etc.
 *          }
 *          If mkey.old exists {
 *  (3)         Save new master_key from mkey in rgy_info, reencrypt, etc.
 *          }
 *      }
 *      If mkey file doesn't exist {
 *          If mkey.old exists {
 *  (2)         create new master_key and store in mkey, reencrypt, etc.
 *          }
 *          If mkey.old doesn't exist {
 *              Major problem: no master key information exists
 *              Fatal.
 *          }
 *      }
 *  }
 *  If db_state == rsdb_st_consistent {
 *      If mkey file exists {
 *          If mkey.old doesn't exist {
 *  (5)         Normal case, nothing to do.
 *          }
 *          If mkey.old exists {
 *  (4)         Reencryption took place and was checkpointed.
 *              Simply delete "mkey.old."
 *          }
 *      }
 *  }
 *
 *
 *  Note: this operation assumes that rs_init_master_key has
 *  run so that master_key globals (like master_keyblock_p)
 *  have been initialized appropriately for this routine.
 *
 */
PUBLIC void rs_reencrypt_database
#ifndef __STDC__
    ( db_state, lock_mode, st )
    short                       db_state;       /* [in] */
    lock_mode_t                 lock_mode;      /* [in] */
    error_status_t              *(st);
#else
    (
    short                       db_state,
    lock_mode_t                 lock_mode,
    error_status_t              *(st)
    )
#endif
{
    int                     fd;
    sec_rgy_name_t          old_master_key_file;
    boolean32               mkey_file_exists = false;
    boolean32               old_mkey_file_exists = false;
    krb5_encrypted_keyblock ekey;
    rsdb_encrypt_key_t      encrypted_mkey;
    krb5_kvno               tmp_mkey_version;
    error_status_t          tmp_status;
    krb5_principal          dummy_princ;

#define cleanup { \
    if(ekey.contents) { \
        memset((char *)(ekey.contents), 0, ekey.length); \
        free(ekey.contents); \
        ekey.contents = 0; \
    } \
    if(encrypted_mkey.length > 0) { \
        memset((char *)(encrypted_mkey.contents), 0, encrypted_mkey.length); \
        encrypted_mkey.length = 0; \
    } \
}
#define err_cleanup { \
    if(new_master_keyblock_p) { \
        if(new_master_keyblock_p->contents) \
            krb5_free_keyblock(new_master_keyblock_p); \
            new_master_keyblock_p = 0; \
    } \
    if(new_master_encblock.priv) { \
        krb5_finish_key(&new_master_encblock); \
        new_master_encblock.priv = 0; \
    } \
    if(new_rblock.rseed) { \
        krb5_finish_random_key(&new_master_encblock, &new_rblock.rseed); \
        new_rblock.rseed = 0; \
    } \
    cleanup; \
}
    CLEAR_STATUS(st);

    /* Do some initialization and setup */
    new_master_keyblock_p    = 0;
    ekey.contents            = 0;
    encrypted_mkey.length    = 0;
    new_master_encblock.priv = 0;
    new_rblock.rseed         = 0;
    krb5_use_cstype(&new_master_encblock, RSDB_AUTH_DEFAULT_ENCTYPE);
    ustrcpy( (char *)old_master_key_file, MASTER_KEY_FILE );
    ustrcat( (char *)old_master_key_file, ".old" );

    if(db_state != rsdb_st_mkey_changing) {
        /* (4) and (5) */
        fd = access((char *) old_master_key_file, F_OK);
        if (fd == 0)
            /* (4) old mkey file exists, delete it */
            unlink((char *)old_master_key_file);
        return;
    }
    /*  We are here if db_state == mkey_changing.
     *  Check for the existence of the <mkey> and <mkey>.old
     *  files.  If the contents of a file are bad,
     *  delete the file and continue with
     *  the file-doesn't-exist case.
     *
     *  Because rs_init_master_key is called before
     *  rs_reencrypt_database, we have some information:
     *      If ( <mkey>.old exists ||
     *           (<mkey> exists && <mkey>.old doesn't exist) ) {
     *          We already know the contents of the existing file are good
     *          because the contents of the existing file were put in the
     *          global master_key variables by rs_init_master_key.
     *          The existing file won't be accessed again in this routine.
     *      }
     *
     *  If ( <mkey> exists && <mkey>.old exists ) {
     *      Fetch contents of <mkey>.  If contents
     *      are bad, delete the file.
     *  }
     */
    fd = access(MASTER_KEY_FILE, F_OK);
    if (fd == 0)
        mkey_file_exists = true;
    fd = access((char *) old_master_key_file, F_OK);
    if (fd == 0)
        old_mkey_file_exists = true;

    if(mkey_file_exists && old_mkey_file_exists) {
        /* (3)
         * Both old and new master key files exist.
         * Verify contents of mkey_file by fetching
         * new_master_key information.
         */
        if (!(new_master_keyblock_p =
                  (krb5_keyblock *)malloc(sizeof(*new_master_keyblock_p)))) {
            err_cleanup;
            SET_STATUS(st, sec_rgy_cant_allocate_memory);
            return;
        }
        new_master_keyblock_p->keytype = RSDB_AUTH_DEFAULT_KEY_TYPE;
        new_master_keyblock_p->contents = 0;

        rsdb_auth_fetch_mkey(MASTER_KEY_FILE, new_master_keyblock_p,
                             &new_master_key_version, st);
        if(BAD_STATUS(st)) {
            /* delete <mkey> file */
            krb5_free_keyblock(new_master_keyblock_p);
            new_master_keyblock_p = 0;
            unlink(MASTER_KEY_FILE);
            mkey_file_exists = false;
        } else {
            if (*st = krb5_process_key(&new_master_encblock, new_master_keyblock_p)) {
                err_cleanup;
                return;
            }
            new_rblock.eblock = &new_master_encblock;
            if (*st = krb5_init_random_key(&new_master_encblock, new_master_keyblock_p,
                                           &new_rblock.rseed)) {
                err_cleanup;
                return;
            }
            rsdb_get_master_key_version(&tmp_mkey_version, st);
            if(BAD_STATUS(st)) {
                err_cleanup;
                return;
            }
            if(tmp_mkey_version != master_key_version) {
                /* current master_key version in database doesn't match
                 * old master key
                 */
                fatal("Master key version in database doesn't match file-stored");
            }
        }
    }

    if( (mkey_file_exists && !old_mkey_file_exists) ||
        (!mkey_file_exists && old_mkey_file_exists) ) {
        /* (1) and (2) */
        /* Use the old master key to generate a new random master key. */
        if (*st = krb5_random_key(rblock.eblock, rblock.rseed, &new_master_keyblock_p)) {
            err_cleanup;
            return;
        }
        if (*st = krb5_process_key(&new_master_encblock, new_master_keyblock_p)) {
            err_cleanup;
            return;
        }
        new_rblock.eblock = &new_master_encblock;
        if (*st = krb5_init_random_key(&new_master_encblock, new_master_keyblock_p,
                                       &new_rblock.rseed)) {
            err_cleanup;
            return;
        }
        if (mkey_file_exists) {
            /* (1) Rename the current master_key file to <file>.old */
            rename(MASTER_KEY_FILE, (char *)old_master_key_file );
        }
        /* Verify that master_key_version matches version stored in database */
        rsdb_get_master_key_version(&tmp_mkey_version, st);
        if(BAD_STATUS(st)) {
            err_cleanup;
            return;
        }
        if(tmp_mkey_version != master_key_version) {
            /* current master_key version in database doesn't match
             * either master key that we know about -- big problem
             */
            fatal("Master key version in database doesn't match file-stored");
        }
       /* Store the new master_key in the master_key
         * file read&write-able only by root
         */
        rsdb_get_new_master_key_version(&new_master_key_version, st);
        rsdb_auth_store_mkey(MASTER_KEY_FILE, new_master_keyblock_p,
                             new_master_key_version, st );
        if(BAD_STATUS(st)) {
            err_cleanup;
            return;
        }
    }

    /* Encrypt new master_key for storage in rgy_info
     */
    rsdb_auth_encrypt_key(&new_master_encblock, new_master_keyblock_p,
                          &ekey, st);
    if(BAD_STATUS(st)) {
        err_cleanup;
        return;
    }
    if(ekey.length > rsdb_auth_encrypt_key_max_len) {
        err_cleanup;
        return;
    }
    encrypted_mkey.mkey_version = new_master_key_version;
    encrypted_mkey.keytype = ekey.keytype;
    encrypted_mkey.length = ekey.length;
    bcopy(ekey.contents, encrypted_mkey.contents, ekey.length);

    if(lock_mode == lock_mode_riw) {
        READ_INTEND_WRITE_PROMOTE(lock_db);
    }
    rsdb_store_new_enc_master_key(&encrypted_mkey, st);
    if(lock_mode == lock_mode_riw) {
        READ_INTEND_WRITE_DEMOTE(lock_db);
    }
    if(BAD_STATUS(st)) {
        err_cleanup;
        return;
    }
    /* For each account in the database, decrypt its key
     * with the old master key and re-encrypt with the
     * new master key.
     */
    rs_reencrypt_db_keys(&master_encblock, master_key_version,
                         &new_master_encblock, new_master_key_version,
                         lock_mode, st);

    /* Don't return bad status if status is "no_more_entries"  */
    if(BAD_STATUS(st)) {
        if(STATUS_EQUAL(st, sec_rgy_no_more_entries)) {
            CLEAR_STATUS(st);
        } else {
            err_cleanup;
            return;
        }
    }
    if(lock_mode == lock_mode_riw) {
        /* rsdb_checkpt calls lock_promote_global if lock_mode == riw */
        lock(lock_replist, lock_mode_riw);
    }
    /* rsdb_checkpt sets db_state to rsdb_st_renaming, then to rsdb_st_consistent */
    rsdb_checkpt(lock_mode, st);
    if(lock_mode == lock_mode_riw) {
        unlock(lock_replist, lock_mode_riw);
    }
    if(BAD_STATUS(st)) {
        err_cleanup;
        return;
    }

    /* delete old mkey file */
    unlink((char *)old_master_key_file);

    /* Free old master key info and
     * copy new master key into global master key variables.
     * Clear new_master_key globals to indicate they're invalid.
     */
    krb5_free_keyblock(master_keyblock_p);
    krb5_finish_key(&master_encblock);
    krb5_finish_random_key(&master_encblock, &rblock.rseed);
    master_keyblock_p = new_master_keyblock_p;
    new_master_keyblock_p = 0;
    master_encblock = new_master_encblock;
    new_master_encblock.priv = 0;
    rblock.rseed = new_rblock.rseed;
    new_rblock.rseed = 0;
    master_key_version = new_master_key_version;
    new_master_key_version = 0;

    if(lock_mode == lock_mode_riw) {
        READ_INTEND_WRITE_DEMOTE(lock_db);
    }
    cleanup;
}




/*
 *  r s _ i n i t _ m a s t e r _ k e y
 *
 *  Lookup the master key.  Process the key
 *  in preparation for its use in encrypting database keys
 *  and initializing the random key generator.
 *  The rs_state.db_state and the presence or one or
 *  both of the <mkey> and <mkey>.old files determines
 *  which version of the stored master_key to use to
 *  initialize the globals.
 *  If db_state is mkey_changing and <mkey>.old exists,
 *  then rs_reencrypt_database will require the master
 *  key globals to reflect the old master key.
 *  If the state is anything else, database reencryption
 *  is not necessary and the current master key information
 *  is in <mkey>.
 *
 *  The following numbered cases correspond to the
 *  crash scenario cases in rs_reencrypt_database:
 *
 *      <mkey>  <mkey>.old                  contents of this file
 *      exists  exists      db_state        copied into mkey globals
 *      ------  ----------  --------------  ------------------------
 *  (1) yes     no          mkey_changing        <mkey>
 *  (2) no      yes         mkey_changing        <mkey>.old
 *  (3) yes     yes         mkey_changing        <mkey>.old
 *  (4) yes     yes         !mkey_changing       <mkey>
 *  (5) yes     no          !mkey_changing       <mkey>
 *
 */
PUBLIC void rs_init_master_key
#ifndef __STDC__
    ( st_p )
    error_status_t      *st_p;
#else
  (
    error_status_t      *st_p
  )
#endif
{
    int                     fd;
    sec_rgy_name_t          old_master_key_file;
    boolean32               old_mkey_file_exists = false;

    ustrcpy( (char *)old_master_key_file, MASTER_KEY_FILE );
    ustrcat( (char *)old_master_key_file, ".old" );

    fd = access((char *) old_master_key_file, F_OK);
    if (fd == 0)
        old_mkey_file_exists = true;

    krb5_use_cstype(&master_encblock, RSDB_AUTH_DEFAULT_ENCTYPE);
    if (!(master_keyblock_p =
              (krb5_keyblock *)malloc(sizeof(*master_keyblock_p)))) {
        SET_STATUS(st_p, sec_rgy_cant_allocate_memory);
        return;
    }
    master_keyblock_p->keytype = RSDB_AUTH_DEFAULT_KEY_TYPE;

    if (rs_state.db_state == rsdb_st_mkey_changing && old_mkey_file_exists) {
        rsdb_auth_fetch_mkey((char *)old_master_key_file, master_keyblock_p,
                             &master_key_version, st_p);
    } else {
        rsdb_auth_fetch_mkey(MASTER_KEY_FILE, master_keyblock_p,
                             &master_key_version, st_p);
    }
    if (BAD_STATUS(st_p)) {
        return;
    }

    if (*st_p = krb5_process_key(&master_encblock, master_keyblock_p)) {
        krb5_free_keyblock(master_keyblock_p);
        return;
    }
    rblock.eblock = &master_encblock;
    if (*st_p = krb5_init_random_key(&master_encblock, master_keyblock_p,
				      &rblock.rseed)) {
        krb5_free_keyblock(master_keyblock_p);
	krb5_finish_key(&master_encblock);
        return;
    }
}



/*
 *  r s _ v e r i f y _ m a s t e r _ k e y
 *
 *  Verify that the master key in the global master_keyblock_p
 *  matches the master_key stored in the database.
 *
 */
PUBLIC void rs_verify_master_key
#ifndef __STDC__
    ( st_p )
    error_status_t      *st_p;
#else
  (
    error_status_t      *st_p
  )
#endif
{
    rsdb_auth_verify_mkey(master_keyblock_p, master_key_version,
                          &master_encblock, st_p);
    if(BAD_STATUS(st_p)) {
        krb5_free_keyblock(master_keyblock_p);
	(void) krb5_finish_key(&master_encblock);
        krb5_finish_random_key(&master_encblock, &rblock.rseed);
        return;
    }
}









