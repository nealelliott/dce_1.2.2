/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_rep_auth.h,v $
 * Revision 1.1.5.2  1996/02/18  22:59:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:58  marty]
 *
 * Revision 1.1.5.1  1995/12/13  16:25:39  root
 * 	Submit
 * 	[1995/12/11  15:15:00  root]
 * 
 * Revision 1.1.2.6  1993/04/21  20:58:19  sommerfeld
 * 	[7700] Add prototype for rs_rep_auth_init().
 * 	[1993/04/21  20:44:00  sommerfeld]
 * 
 * Revision 1.1.2.5  1992/12/29  13:24:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:08  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/18  22:10:40  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:34  sekhar]
 * 
 * Revision 1.1.3.2  1992/12/11  21:11:14  emartin
 * 	bl6c: move rs_rep_auth_handle_t to rs_rep_pvt.h
 * 
 * Revision 1.1.2.3  1992/11/04  19:32:20  ahop
 * 	Third replication drop: bl6a
 * 	[1992/11/03  18:12:14  ahop]
 * 
 * Revision 1.1.2.2  1992/08/31  18:18:46  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  16:46:19  sekhar]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - Propagation authentication utilities
 */

#ifndef rs_rep_auth_h__included
#define rs_rep_auth_h__included

#include <dce/id_base.h>
#include <dce/rpcbase.h>
#include <dce/sec_login.h>
#include <dce/rplbase.h>

#include <rs_rep_pvt.h>

/* 
 * Master-only routines
 */

/* rs_rep_auth_get_handle
 *
 * Used by the (possibly surrogate) master, to decode and decrypt a slave's
 * auth info obtained via rs_replica_mgr_get_info_and_creds.  If the decoding/
 * decrypting is successful, return an opaque handle on the decoded info. 
 * The replica session key and a login context suitable for authenticating
 * to the slave may extracted from the auth handle using routines defined 
 * below.
 *
 * NOTE:  We assume that the database is locked with a least a read lock
 *        because we need to look up the security server's key in the 
 *        key database.
 */                            
void  rs_rep_auth_get_handle ( 
#ifdef __STDC__
    rs_replica_auth_t     *auth_info,  /* [in] */
    rs_rep_auth_handle_t  *handle,     /* [out] */
    error_status_t        *status
#endif
);        

/* rs_rep_auth_refresh_handle
 *
 * Refresh an existing replica auth handle.  Assume the caller has the
 * database locked.
 */
void  rs_rep_auth_refresh_handle ( 
#ifdef __STDC__
    rs_replica_auth_t     *auth_info,  /* [in] */
    rs_rep_auth_handle_t  handle,      /* [in] */
    error_status_t        *status      /* [out] */
#endif
);


/* rs_rep_auth_get_rep_skey
 *
 * return the session key from a replica's encoded PTGT.  Assume the
 * caller has the database locked.
 *
 * The session key should be freed by calling sec_crypt_free_key
 */
void  rs_rep_auth_get_rep_skey (
#ifdef __STDC__
    rs_rep_auth_handle_t  handle,    /* [in] */
    sec_passwd_rec_t      **skey,    /* [out] */
    error_status_t        *status    /* [out] */
#endif
);
 
/* rs_rep_auth_binding_set_auth
 *
 * authenticate a binding handle using the credentials referenced
 * by auth_h, and the architecturally-defined registry service principal.
 */
void  rs_rep_auth_binding_set_auth ( 
#ifdef __STDC__
    rs_rep_auth_handle_t  auth_h,
    rpc_binding_handle_t  rep_handle,
    error_status_t        *status
#endif
);

/* rs_rep_auth_get_login_context
 * 
 * extract a sec_login_handle_t for use in authenticating
 * bindings to replica.     
 *
 * The login context returned by this routine should *NOT* be used 
 * as a parameter to any public DCE API routine except for 
 * rpc_binding_set_auth_info, and then only with authz = authz_name
 * (NOT authz_dce).  
 */
void  rs_rep_auth_get_login_context ( 
#ifdef __STDC__
    rs_rep_auth_handle_t  handle,
    sec_login_handle_t    *login_handle_p,
    error_status_t        *status
#endif
);

/* rs_rep_auth_dispose_handle
 *
 * free memory associated with  an auth info handle
 */
void  rs_rep_auth_dispose_handle ( 
#ifdef __STDC__
    rs_rep_auth_handle_t  *handle
#endif
);


/*
 * Slave-only routines
 */

/* rs_rep_auth_get_auth_info
 * 
 * Called by a slave to acquire its rs_replica_auth_t info, which it then
 * passes to the (possibly surrogate) master so the master may authenticate
 * to the slave using the the session key sealed in the auth info.  The
 * returned auth_info data should be freed by calling rs_rep_auth_free_auth_info.
 * This routine queries the database to obtain the replica's cell name.  The
 * caller must provide the appropriate concurreny control.
 */
void  rs_rep_auth_get_auth_info ( 
#ifdef __STDC__
    rs_replica_auth_t  **auth_info,  /* [out] */
    error_status_t     *status
#endif
);

void  rs_rep_auth_free_auth_info ( 
#ifdef __STDC__
    rs_replica_auth_t  **auth_info,  /* [out] */
    error_status_t     *status
#endif
);

/* rs_rep_auth_check
 *
 * Verify that the caller is another registry server in our cell
 * (accept only tickets in the name of the architecturally-defined 
 * registry server principal).
 */
void  rs_rep_auth_check (
#ifdef __STDC__
    handle_t        handle,  /* [in] */
    error_status_t  *status  /* [out] */
#endif
);                                       

/* rs_rep_auth_get_init_skey
 *
 * Look up the slave's PTGT credential and return the session key.  This
 * routine could be sped up with some caching, but that would expose the 
 * session key in memory for long periods of time, which may not be such a
 *  great idea (but probably doesn't really matter at the moment as the 
 * session key is stored raw on the disk in the credential cache file).
 *
 * This routine queries the database to obtain the replica's cell name.  The
 * caller must provide the appropriate concurreny control.
 */
void  rs_rep_auth_get_init_skey ( 
#ifdef __STDC__
    sec_passwd_rec_t  **skey,    /* [out] */
    error_status_t    *status
#endif
);


/* rs_rep_auth_gen_init_ekey
 *
 *  Use the master_key to randomly generate a DES key
 *  to be used to encrypt/decrypt account auth_keys
 *  during initialization account propagation.
 *  (The master key must have been initialized already;
 *   this occurs in rsdb_initialize.)
 *  This key is stored in "init_encrypt_key," private to
 *  this file, so that it may be accessed by other 
 *  modules using the get_init_ekey call.
 *  The init_encrypt_key should be destroyed, after it
 *  has fulfilled its purpose, by calling destroy_ekey.
 *
 *  This operation is called by an initializing slave
 *  to obtain the encryption_key to pass to a (surrogate) master
 *  in rs_rep_mgr_copy_all.  
 *  This operation returns a pointer to the static init_encrypt_key.
 *  The pointer should be reset to NULL after the caller is 
 *  finished with it.
 */
void  rs_rep_auth_gen_init_ekey (
#ifdef __STDC__
    sec_passwd_rec_t  **ekey,
    error_status_t    *status
#endif
);


/* rs_rep_auth_get_init_ekey
 *
 *  Return a pointer to the static init_encrypt_key.
 *  This operation is called by an initializing slave
 *  in rs_prop_acct_add to obtain the encryption_key in
 *  order to decrypt account auth_keys.
 *  The pointer should be reset to NULL after the caller is 
 *  finished with it.
 */
void  rs_rep_auth_get_init_ekey (
#ifdef __STDC__
    sec_passwd_rec_t  **ekey,
    error_status_t    *status
#endif
);


/* rs_rep_auth_destroy_init_ekey
 *
 *  Clear the memory associated with the init_encrypt_key.
 *  This operation is called by an initializing slave
 *  after completion (successful or not) of rs_rep_mgr_copy_all.  
 */
void  rs_rep_auth_destroy_init_ekey (
#ifdef __STDC__
    error_status_t    *status
#endif
);

/*
 * rs_rep_auth_init
 * 
 * Initialize static data used by this module.
 */

void  rs_rep_auth_init (
#ifdef __STDC__
    void
#endif
);


#endif  /* #ifndef rs_rep_auth_h__included */
