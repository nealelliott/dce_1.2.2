/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_unix.h,v $
 * Revision 1.1.7.2  1996/02/18  23:00:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:04  marty]
 *
 * Revision 1.1.7.1  1995/12/08  17:28:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:00  root]
 * 
 * Revision 1.1.5.2  1992/12/29  13:26:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:46:58  zeliff]
 * 
 * Revision 1.1.3.2  1992/06/02  20:57:03  sekhar
 * 	Drop Unix interface changes made by frisco
 * 	[1992/06/01  21:06:14  sekhar]
 * 
 * Revision 1.1.1.2  1992/05/22  18:39:14  frisco
 * 	Initial revision - Unix interface support
 * 
 * $EndLog$
 */

/*  rsdb_unix.h V=1 05/21/92 //littl/prgy/src/server/rsdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - database interlude - Unix interfaces
 */

#ifndef rsdb_unix__included
#define rsdb_unix__included

#include <dce/rgynbase.h>

void rsdb_unix_gen_pwent ( 
#ifdef __STDC__
    sec_rgy_name_t         login_name,
    sec_rgy_unix_sid_t     *unix_sid,
    sec_rgy_sid_t          *sid,
    sec_rgy_acct_key_t     key_parts,
    sec_rgy_acct_user_t    *user_part,
    sec_rgy_acct_admin_t   *admin_part,
    sec_rgy_unix_passwd_t  *passwd_rec,
    error_status_t         *status
#endif
);

void rsdb_unix_gen_dummy_pwent ( 
#ifdef __STDC__
    sec_rgy_name_t         name,
    sec_rgy_pgo_item_t     *pgo,
    sec_rgy_unix_passwd_t  *passwd_rec
#endif
);

void rsdb_unix_gen_grent ( 
#ifdef __STDC__
    sec_rgy_name_t         name,
    sec_rgy_pgo_item_t     *grp_pgo,
    sec_rgy_member_t       members[],
    long                   num_members,
    sec_rgy_unix_group_t   *result,
    error_status_t         *status
#endif
);

#endif
