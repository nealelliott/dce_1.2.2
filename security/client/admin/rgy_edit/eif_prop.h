/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: eif_prop.h,v $
 * Revision 1.1.6.2  1996/03/11  13:26:42  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:41  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:58:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:49  root]
 * 
 * Revision 1.1.4.2  1992/12/29  12:39:16  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:35:19  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/03  19:40:02  burati
 * 	CR2419 Add support for local registry.
 * 	[1992/03/31  20:39:53  burati]
 * 
 * Revision 1.1  1992/01/19  14:45:38  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  eif_prop.h V=9 03/02/92 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit generic interface properties/policies management
**
*/

#ifndef eif_prop_h__included
#define eif_prop_h__included

#include <dce/nbase.h>

#include <ek_db.h>


boolean32 eif_write_policy_field (
#ifdef __STDC__
    ek_op_handle_t  op_handle,
    ek_field_id_t   field,
    char            *data,
    boolean32       check_default,
    error_status_t  *stp
#endif
);

boolean32 eif_write_auth_policy_field (
#ifdef __STDC__
    ek_op_handle_t  op_handle,
    ek_field_id_t   field,
    char            *data,
    boolean32       check_default,
    error_status_t  *stp
#endif
);

boolean32 eif_write_prop_field (
#ifdef __STDC__
    ek_op_handle_t  op_handle,
    ek_field_id_t   field,
    char            *data,
    boolean32       check_default,
    error_status_t  *stp
#endif
);

boolean32 eif_write_lrgy_prop_field (
#ifdef __STDC__
    ek_op_handle_t  op_handle,
    ek_field_id_t   field,
    char            *data,
    boolean32       check_default,
    error_status_t  *stp
#endif
);

boolean32 eif_write_override_field (
#ifdef __STDC__
    ek_op_handle_t  op_handle,
    ek_field_id_t   field,
    char            *data,
    boolean32       check_default,
    error_status_t  *stp
#endif
);

error_status_t eif_change_policy (
#ifdef __STDC__
    ek_op_handle_t  op_handle
#endif
);

error_status_t eif_change_auth_policy (
#ifdef __STDC__
    ek_op_handle_t  op_handle
#endif
);


error_status_t eif_change_prop (
#ifdef __STDC__
    ek_op_handle_t  op_handle
#endif
);

error_status_t eif_view_override (
#ifdef __STDC__
    char  *key
#endif
);

error_status_t eif_change_override (
#ifdef __STDC__
    ek_op_handle_t  chg_handle
#endif
);

#endif
