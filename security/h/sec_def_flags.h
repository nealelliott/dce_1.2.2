/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_def_flags.h,v $
 * Revision 1.1.4.2  1996/02/18  23:00:55  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:20  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:29:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:14  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:27:22  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:15:58  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:43:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  sec_def_flags.h V=3 11/05/91 //littl/prgy/src/h
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Client And Server - Default Flag Settings
 * 
 */

#ifndef sec_def_flags__included
#define sec_def_flags__included

#include <dce/rgynbase.h>

/* ACCT_AUTH_FLAGS */
#define SEC_DEF_FLGS_ACCT_AUTH          sec_rgy_acct_auth_forwardable | \
                                        sec_rgy_acct_auth_tgt         | \
                                        sec_rgy_acct_auth_renewable

#define SEC_DEF_FLGS_ACCT_AUTH_CELL     sec_rgy_acct_auth_post_dated  | \
                                        sec_rgy_acct_auth_forwardable | \
                                        sec_rgy_acct_auth_tgt         | \
                                        sec_rgy_acct_auth_renewable   | \
                                        sec_rgy_acct_auth_proxiable   | \
                                        sec_rgy_acct_auth_dup_skey

/* ACCT_ADMIN_FLAGS */
#define SEC_DEF_FLGS_ACCT_ADMIN         sec_rgy_acct_admin_client   | \
                                        sec_rgy_acct_admin_server

#define SEC_DEF_FLGS_ACCT_ADMIN_LOGIN   sec_rgy_acct_admin_valid    | \
                                        sec_rgy_acct_admin_client   | \
                                        sec_rgy_acct_admin_server

#define SEC_DEF_FLGS_ACCT_ADMIN_CELL    sec_rgy_acct_admin_server

/* ACCT_USER_FLAGS */
#define SEC_DEF_FLGS_ACCT_USER          sec_rgy_acct_user_flags_none

#define SEC_DEF_FLGS_ACCT_USER_CELL     sec_rgy_acct_user_passwd_valid


                                                          
#endif
