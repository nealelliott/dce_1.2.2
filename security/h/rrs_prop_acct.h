/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_acct.h,v $
 * Revision 1.1.8.2  1996/02/18  22:58:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:09  marty]
 *
 * Revision 1.1.8.1  1995/12/13  16:24:45  root
 * 	Submit
 * 	[1995/12/11  15:14:27  root]
 * 
 * Revision 1.1.6.1  1994/07/15  14:59:03  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:15:47  mdf]
 * 
 * Revision 1.1.3.5  1993/03/19  22:19:38  emartin
 * 	CR 7472 rrs_prop_acct_replace takes unix_passwd arg
 * 	[1993/03/19  18:52:29  emartin]
 * 
 * Revision 1.1.3.4  1992/12/29  13:07:12  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:43:32  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/18  22:09:54  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:45:02  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:03:35  emartin
 * 	bl6c: replication initialization updates
 * 
 * Revision 1.1.3.2  1992/10/07  20:24:32  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:10:33  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:29:36  burati
 * 	 Second replication code drop: bl5
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server Propagation Client - Accounts
 */

#ifndef __rrs_prop_acct_h__included__
#define __rrs_prop_acct_h__included__

#include <dce/passwd.h>
#include <dce/rplbase.h>
#include <dce/rgynbase.h>
#include <rs_prop_acct.h>
#include <dce/rsbase.h>
#include <rs_rep_auth.h>
#include <rs_rep_pvt.h>

/*  rrs_prop_acct_add_all
 *
 * Propagate all existing accounts to an initializing slave.  
 * "encryption_key" will be used to encrypt secret keys 
 * on the wire.
 */
void  rrs_prop_acct_add_all (
#ifdef __STDC__
    rs_rep_binding_t           *rep_binding,     /* [in] */
    sec_passwd_rec_t           *encryption_key,  /* [in] */
    rs_replica_master_info_t   *master_info,     /* [in] */  
    error_status_t             *st  
#endif
);   

/* rrs_prop_acct_add
 * 
 * Propagate account adds to a slave.
 */
void  rrs_prop_acct_add (
#ifdef __STDC__
    handle_t                  h,              /* [in] */
    unsigned32                num_accts,      /* [in] */
    rs_prop_acct_add_data_t   accts[],        /* [in] */
    rs_replica_master_info_t  *master_info,   /* [in] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            *st  
#endif
);
 
/* rrs_prop_acct_delete
 */
void  rrs_prop_acct_delete (
#ifdef __STDC__ 
    handle_t                  h,              /* [in] */
    sec_rgy_login_name_t      *login_name,    /* [in] */
    rs_replica_master_info_t  *master_info,   /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            *st
#endif
);    

/*
 * rrs_prop_acct_rename
 */
void  rrs_prop_acct_rename ( 
#ifdef __STDC__
    handle_t                  h,                 /* [in] */
    sec_rgy_login_name_t      *old_login_name,   /* [in] */
    sec_rgy_login_name_t      *new_login_name,   /* [in] */
    rs_replica_master_info_t  *master_info,      /* [in, ref] */
    boolean32                 propq_only,        /* [in] */
    error_status_t            *st
#endif
);

/*
 * rrs_prop_acct_replace 
 * 
 * "client_ids" identifies (by uuid) the principal under whose key the
 * updated "key" and "unix_passwd" are encrypted.  If "client_ids" 
 * contains NIL uuids, "key" and "unix_passwd" are encrypted under 
 * a session key.
 */
void  rrs_prop_acct_replace (
#ifdef __STDC__
    handle_t                  h,              /* [in] */
    sec_rgy_login_name_t      *login_name,    /* [in] */
    rs_acct_parts_t           modify_parts,   /* [in] */
    sec_rgy_acct_user_t       *user_part,     /* [in] */
    sec_rgy_acct_admin_t      *admin_part,    /* [in] */
    rs_acct_key_transmit_t    *key,           /* [in]  may be NULL */
    sec_rgy_foreign_id_t      *client_ids,    /* [in, ref] */
    sec_passwd_type_t         new_keytype,    /* [in] */
    rs_acct_key_transmit_t    *unix_passwd,   /* [in]  may be NULL */
    sec_timeval_sec_t         *master_time,   /* [in, ref] */
    rs_replica_master_info_t  *master_info,   /* [in, ref] */
    boolean32                 propq_only,     /* [in] */
    error_status_t            *st
#endif
);  

#endif /*  __rrs_prop_acct_h__included__ */
