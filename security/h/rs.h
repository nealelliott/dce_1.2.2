/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs.h,v $
 * Revision 1.1.8.3  1996/02/18  22:58:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:18  marty]
 *
 * Revision 1.1.8.2  1995/12/13  16:24:55  root
 * 	Submit
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:00 UTC  greg
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:33 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Create initial SEC_migrate mainline.
 * 	Compiled but not tested.
 * 	[1995/01/27  21:44 UTC  greg  /main/SEC_migrate/1]
 * 
 * 	Add prototype for rs_server_register_if().
 * 
 * 	[1995/01/23  01:38 UTC  greg  /main/greg_migration2/1]
 * 
 * Revision 1.1.2.5  1992/12/29  13:07:28  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:43:55  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/18  22:10:10  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:45:37  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:06:05  emartin
 * 	bl6c: decl rs_main_startup_after_init
 * 
 * Revision 1.1.2.3  1992/10/07  20:25:22  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:11:07  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:33:56  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.2.2  1992/08/31  18:17:09  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  14:56:46  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:43:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1991
 * Unpublished work. All Rights Reserved.
*/
/*
 *      Registry Server - mainline
 */

#ifndef rs_h__included
#define rs_h__included

#include <dce/nbase.h>
#include <dce/rgynbase.h>


/*
 * Initialize the server's bindings to which
 * RPCs may be made.
 * Derive the server's protocol towers from
 * these bindings
 */
void rs_server_bindings_init (
    rpc_binding_vector_p_t  *svr_bindings,
    rs_replica_twr_vec_p_t  *rep_twrs,
    error_status_t          *st
);

/*
 * Get a pointer to the server's
 * binding vector
 */
void rs_server_bindings_inquire (
    rpc_binding_vector_p_t  *svr_bindings,
    error_status_t          *st
);

/*
 * Return the number of network addresses in 
 * "bindings" which match the server's addresses
 */
void rs_server_bindings_naddr_equal (
    rpc_binding_vector_p_t  bindings,
    unsigned32              *nequal,
    error_status_t          *st
);

/*  
 *  Return true if the handle h identifies
 *  contains a local network address.
 *  Return false otherwise.
 */
boolean32 rs_server_bindings_local_client (
    handle_t                h,
    error_status_t          *st
);

void rs_server_register (
    error_status_t  *status
);

void rs_server_register_if (
    unsigned32      compat_sw_rev,
    error_status_t  *status
);

void rs_server_unregister (
    void
);

void rs_server_register_prop_if (
    error_status_t  *status
);

void rs_server_unregister_prop_if (
    void
);

void rs_start_kerberos (
    void
);

void rs_setup_dce_rgy_identity (
    error_status_t      *status
);

#endif  /* rs_h__included */
