/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: eif_pgo.h,v $
 * Revision 1.1.6.2  1996/03/11  13:26:40  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:39  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:58:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:43  root]
 * 
 * Revision 1.1.4.2  1994/09/09  19:11:52  mob
 * 	Removing the temporary, incomplete ERA support from rgy_edit (OT 11652,11262)
 * 	[1994/09/09  00:10:50  mob]
 * 
 * Revision 1.1.4.1  1994/03/02  17:54:13  hanfei
 * 	Initial ERA drop
 * 	[1994/02/24  21:07:57  hanfei]
 * 
 * Revision 1.1.2.3  1992/12/29  12:39:05  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:35:09  zeliff]
 * 
 * Revision 1.1.2.2  1992/10/25  20:14:19  burati
 * 	CR5732 Extra argument to eif_view_pgo for viewing by unix number
 * 	[1992/10/23  19:49:35  burati]
 * 
 * Revision 1.1  1992/01/19  14:46:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  eif_pgo.h V=7 11/22/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit generic interface pgo management
*/

#ifndef eif_pgo_h__included 
#define eif_pgo_h__included 
  
#include <dce/nbase.h>

#include <ek_db.h>
#include <eif.h>


boolean32 eif_write_pgo_field (
#ifdef __STDC__
    ek_op_handle_t  pgo_handle,
    ek_field_id_t   field,
    char             *data,
    eif_domain_t    domain,
    boolean32        check_default,
    error_status_t        *stp
#endif
);

boolean32 eif_write_member_field ( 
#ifdef __STDC__
    ek_op_handle_t  op_handle,
    ek_field_id_t   field,
    char             *data,
    error_status_t        *stp
#endif
);


/* FUNCTION eif_view_pgo
 *
 * Takes a handle and some booleans indicating the amount of info to be 
 * displayed.  The operation id in the handle indicates the kind of pgo query.
 * If it's a name query and the ekn_pgo_key field of the handle contains a
 * non-zero length name, then just do one lookup. If it's a name query on a zero
 * length name or a unix number query then invoke the operation repeatedly until
 * a non-zero status is returned.  An ek_misc_no_more_entries is normal -
 * anything  else gets reported..
 */
error_status_t eif_view_pgo ( 
#ifdef __STDC__
    eif_domain_t   domain,
    char           *key,
    boolean32      show_full,
    boolean32      show_members,
    boolean32      show_policy,
    boolean32      list_by_num
#endif
);

   
error_status_t eif_add_pgo (
#ifdef __STDC__
    ek_op_handle_t  add_handle,
    eif_domain_t    domain
#endif
);
 
error_status_t eif_change_pgo (
#ifdef __STDC__
    ek_op_handle_t  chg_handle,
    eif_domain_t    domain
#endif
);

error_status_t eif_adopt_pgo (
#ifdef __STDC__
    ek_op_handle_t  op_handle,
    eif_domain_t    domain
#endif
);

error_status_t eif_delete_pgo (
#ifdef __STDC__
    ek_op_handle_t  del_handle,
    eif_domain_t    domain,
    boolean32       verify
#endif
);

error_status_t eif_do_member_op ( 
#ifdef __STDC__
    ek_op_handle_t  op_handle,
    boolean32       verify
#endif
);


error_status_t eif_member_update ( 
#ifdef __STDC__
    ek_op_handle_t  op_handle,
    char            *go_name,
    char            *member_list,
    boolean32       verify
#endif
);

#endif
