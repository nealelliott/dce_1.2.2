/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_acct.h,v $
 * Revision 1.1.7.2  1996/02/18  22:58:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:32  marty]
 *
 * Revision 1.1.7.1  1995/12/13  16:25:12  root
 * 	Submit
 * 	[1995/12/11  15:14:43  root]
 * 
 * Revision 1.1.5.1  1994/07/15  14:59:18  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:16:31  mdf]
 * 
 * Revision 1.1.3.4  1993/03/19  22:19:45  emartin
 * 	CR 7472 rs_log_acct_add/replace take unix_passwd arg
 * 	[1993/03/19  18:52:04  emartin]
 * 
 * Revision 1.1.3.3  1992/12/29  13:07:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:20  zeliff]
 * 
 * Revision 1.1.3.2  1992/10/07  20:25:58  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:11:26  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:35:24  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1  1992/01/19  14:42:44  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/* 
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */

#ifndef rs_log_acct__included
#define rs_log_acct__included

#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>
#include <dce/rsbase.h>


void rs_log_acct_init (
#ifdef __STDC__
    error_status_t   *st
#endif
);

void rs_log_acct_add (
#ifdef __STDC__
    sec_rgy_login_name_t       *login_name,
    sec_rgy_acct_user_t        *user_part,
    sec_rgy_acct_admin_t       *admin_part,
    rs_acct_key_transmit_t     *passwd,
    sec_rgy_foreign_id_t       *client_ids,
    sec_passwd_type_t          keytype,     /* used if passwd is transmitted
                                             * in plaintext to determine which
                                             * which type of ciphertext key
                                             * to generate 
                                             */
    rs_acct_key_transmit_t     *unix_passwd,
    rs_replica_master_info_t   *master_info
#endif
);

void rs_log_acct_delete (
#ifdef __STDC__
    sec_rgy_login_name_t       *name,
    rs_replica_master_info_t   *master_info
#endif
);

void rs_log_acct_replace (
#ifdef __STDC__
    sec_rgy_login_name_t       *login_name,
    rs_acct_parts_t            data_parts,
    sec_rgy_acct_user_t        *user_part,
    sec_rgy_acct_admin_t       *admin_part,
    rs_acct_key_transmit_t     *passwd,
    sec_rgy_foreign_id_t       *client_ids,
    sec_passwd_type_t          keytype,
    rs_acct_key_transmit_t     *unix_passwd,
    sec_timeval_sec_t          *time_now,  /* for key garbage collection */
    rs_replica_master_info_t   *master_info
#endif
);

void rs_log_acct_rename (
#ifdef __STDC__
    sec_rgy_login_name_t       *new_login_name,
    sec_rgy_login_name_t       *old_login_name,
    rs_replica_master_info_t   *master_info
#endif
);

#endif

