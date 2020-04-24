/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_ns.h,v $
 * Revision 1.1.5.2  1996/02/18  22:59:13  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:50  marty]
 *
 * Revision 1.1.5.1  1995/12/13  16:25:30  root
 * 	Submit
 * 	[1995/12/11  15:14:55  root]
 * 
 * Revision 1.1.2.5  1993/02/10  20:33:54  emartin
 * 	bl6e: nuke rs_ns_group_lookup_ifspec,
 * 	 rs_ns_names_init, rs_ns_server_add_name
 * 	add rs_ns_entry_add
 * 	[1993/02/04  23:25:23  emartin]
 * 
 * Revision 1.1.2.4  1992/12/29  13:08:30  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:51  zeliff]
 * 
 * Revision 1.1.2.3  1992/12/18  22:10:32  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:20  sekhar]
 * 
 * Revision 1.1.3.2  1992/12/11  21:10:02  emartin
 * 	bl6c: decl rs_ns_server_register_wakeup
 * 
 * Revision 1.1.2.2  1992/08/31  18:18:26  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  16:46:13  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:43:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_ns.h V=12 12/16/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - mainline
 */

#ifndef rs_ns_h__included
#define rs_ns_h__included

#include <dce/nbase.h>
#include <dce/rgynbase.h>


PUBLIC void rs_ns_bootstrap_enable (
#ifdef __STDC__
    void
#endif
);

PUBLIC void rs_ns_server_register_start_tsk (
#ifdef __STDC__
    error_status_t  *st
#endif
);

PUBLIC void rs_ns_server_register_wakeup (
#ifdef __STDC__
    void
#endif
);

/*
 * Delete the server's server entry and
 * group entry from the nameservice.
 *
 * Stop the rs_ns_server_register task
 * before deleting the names from the
 * nameservice (it will be trying to add
 * the names).
 */
PUBLIC void rs_ns_server_delete (
#ifdef __STDC__
    unsigned_char_p_t   name,
    error_status_t      *st
#endif
);

/*
 * Change the server's name in the name service.
 *
 * Invoked at startup time before the rs_ns_server_register
 * task has started.
 */
PUBLIC void rs_ns_server_change_name (
#ifdef __STDC__
    unsigned_char_p_t       old_name,
    unsigned_char_p_t       new_name,
    uuid_p_t                cell_sec_id,
    uuid_p_t                rep_id,
    rpc_binding_vector_p_t  svr_bindings,
    error_status_t          *st
#endif
);

/*
 * Given a cell name and a cell relative name (w/out any
 * prefix) compose a full name.
 */
PUBLIC void rs_ns_name_compose (
#ifdef __STDC__
    unsigned_char_p_t   cell_name,
    unsigned_char_p_t   name,
    unsigned_char_p_t   *full_name,
    error_status_t      *st
#endif
);

/* 
 * Validate that it's ok for this server to use "name"
 * as its nameservice entry
 */
PUBLIC void rs_ns_entry_validate (
#ifdef __STDC__
    unsigned_char_p_t       name,
    uuid_p_t                cell_sec_id,
    uuid_p_t                rep_id,
    rpc_binding_vector_p_t  svr_bindings,
    rpc_if_handle_t         ifspec,
    error_status_t          *st
#endif
);

/*
 * Add a server entry to the nameservice.
 */
PUBLIC void rs_ns_entry_add (
#ifdef __STDC__
    unsigned_char_p_t       name,
    uuid_p_t                cell_sec_id,
    uuid_p_t                rep_id,
    rpc_if_handle_t         ifspec,
    rpc_binding_vector_p_t  bindings,
    error_status_t          *st
#endif
);

/*
 * Remove a server entry from the nameservice
 */
PUBLIC void rs_ns_entry_delete (
#ifdef __STDC__
    unsigned_char_p_t       name,
    error_status_t          *st
#endif
);

#endif 
