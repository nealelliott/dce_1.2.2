/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: eif_acct.h,v $
 * Revision 1.1.6.2  1996/03/11  13:26:36  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:36  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:58:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:32  root]
 * 
 * Revision 1.1.4.1  1994/07/15  14:58:15  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:13:27  mdf]
 * 
 * Revision 1.1.2.2  1992/12/29  12:38:50  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:11:53  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:45:55  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  eif_acct.h V=10 11/06/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit generic interface account management
** 
*/

#ifndef eif_acct_h__included 
#define eif_acct_h__included 
  
#include <dce/nbase.h>

#include <ek_db.h>


boolean32  eif_gen_acct_passwd(
    ek_op_handle_t      acct_handle,
    sec_passwd_type_t   *passwd_type,
    sec_passwd_rec_t    *passwd,
    error_status_t      *stp
);

boolean32  eif_get_pwd_mgmt_info (
    ek_op_handle_t      acct_handle,
    ek_pwd_val_type_t   *pwd_val_type,
    error_status_t      *stp
);

boolean32  eif_write_acct_field (
#ifdef __STDC__
    ek_op_handle_t  acct_handle,
    ek_field_id_t   field,
    char            *data,
    boolean32       check_default,
    error_status_t  *stp
#endif
);

error_status_t eif_view_acct ( 
#ifdef __STDC__
    char      *pkey,
    char      *gkey,
    char      *okey,
    boolean32 show_full
#endif
);

error_status_t eif_add_acct (
#ifdef __STDC__
    ek_op_handle_t  add_handle
#endif
);

error_status_t eif_change_one_acct (
#ifdef __STDC__
    ek_op_handle_t  chg_handle
#endif
);

error_status_t eif_change_wild_acct (
#ifdef __STDC__
    char            *pkey, 
    char            *gkey, 
    char            *okey, 
    ek_op_handle_t  chg_handle,
    boolean32       verify
#endif
);

error_status_t eif_delete_acct (
#ifdef __STDC__
    char            *pkey,
    char            *gkey,
    char            *okey,
    ek_op_handle_t  del_handle,
    boolean32       verify
#endif
);


#endif
