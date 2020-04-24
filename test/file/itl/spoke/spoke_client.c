/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: spoke_client.c,v $
 * Revision 1.1.319.1  1996/10/17  18:28:42  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:54:25  damon]
 *
 * Revision 1.1.314.1  1994/02/04  20:46:23  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:25:00  devsrc]
 * 
 * Revision 1.1.312.1  1993/12/07  17:45:26  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  19:24:04  jaffe]
 * 
 * $EndLog$
 */
/*
 * ID: $Id: spoke_client.c,v 1.1.319.1 1996/10/17 18:28:42 damon Exp $
 *
 * COMPONENT_NAME: Distributed ITL Development Infrastructure
 *
 * ORIGINS: Transarc Corp.
 *
 * (C) COPYRIGHT Transarc Corp. 1992
 * All Rights Reserved
 * Licensed Materials - Property of Transarc
 *
 * US Government Users Restricted Rights - Use, duplication or
 * disclosure restricted by GSA ADP Schedule Contract with Transarc Corp
 */
#include <dce/rpc.h>
#include <utils/itl/itl.h>
#include <utils/itl/itl_stds.h>
#include <utils/itl/defs.h>

unsigned32 
spoke_setup( 
	    handle_t handle,
	    rpc_if_handle_t ifspec,
	    rpc_mgr_epv_t epv
	    )
{
    rpc_binding_vector_t * bvP = NULL;
    unsigned32		st;
    unsigned char 	* string_binding;
    
    /*
     * do the initial connection setup.
     */
	
    rpc_server_inq_bindings(&bvP, &st);
    if (st == rpc_s_no_bindings) {
	ns_Listen(NULL, ifspec, epv); /* create a listener for the spoke if */
	rpc_server_inq_bindings(&bvP, &st);
	if (st != rpc_s_ok) {
	    DCE_ERROR(st, "rpc_server_inq_bindings FAILED");
	    return;
	}
    }

    /* ok, we have a binding, send it to the server  */

    rpc_binding_to_string_binding(bvP->binding_h[0], &string_binding, &st);
    if (st != rpc_s_ok) {
	DCE_ERROR(st, "rpc_binding_to_string_binding FAILED");
	return;
    }
	    
    st = spoke_remote_setup(handle, string_binding);
    if (st)
	DCE_ERROR(st, "spoke_setup FAILED");

    return st;
}

unsigned32 
spoke_join_syncpoint(
		     handle_t h,
		     char * n,
		     unsigned32 t,
		     int * timed_out
		     )
{
    return spoke_remote_syncpoint(h, (idl_char *) n, t, (unsigned32 *)timed_out);
}
