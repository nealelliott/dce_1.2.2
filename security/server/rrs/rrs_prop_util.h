/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_util.h,v $
 * Revision 1.1.6.2  1996/02/18  23:03:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:17  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:48:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:19:41  root]
 * 
 * Revision 1.1.3.4  1992/12/29  16:33:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:22:04  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/18  22:11:27  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:48:22  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:38:55  emartin
 * 	bl6c: replication updates
 * 
 * Revision 1.1.3.2  1992/10/07  20:30:57  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:14:46  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:55:45  burati
 * 	 Second replication code drop: bl5
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server Propagation Client - Utilities
 */

#ifndef __rrs_prop_util_h_included__
#define __rrs_prop_util_h_included__

#include <dce/rpcbase.h>
#include <rs_rep_auth.h>

/* number of times to retry a propagation call before giving up */
#define rrs_c_prop_max_tries    3
#define rrs_c_init_sleep_secs   45

#define PROP_SETUP_RETRY \
{ \
    unsigned32 num__tries = 0;   \
    boolean32  prop__done = false;    \
    while (!prop__done) 

#define PROP_CHECK_RETRY(handler, bindings, stp)        \
        prop__done = (GOOD_STATUS((stp)) ||                   \
               (++num__tries > rrs_c_prop_max_tries) ||   \
               handler((bindings), (stp)) );              

#define PROP_END_RETRY \
}


/* rrs_prop_util_retry_handler
 *
 * Try to recover from communication and authentication failures:
 * the former by rebinding to the server and the latter by 
 * "pinging"  the replica for its replica auth info and refreshing 
 * the virtual memory credential cache for that replica.
 *
 * Return false if the operation is not done (ie. the calling
 * routine should retry the operation).
 * Return true if the operation is done (ie. the operation 
 * succeeded or encountered a "fatal" error).
 */
PUBLIC boolean32  rrs_prop_util_retry_handler ( 
#ifdef __STDC__
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
#endif
);

#endif
