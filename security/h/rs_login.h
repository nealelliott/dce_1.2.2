/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_login.h,v $
 * Revision 1.1.5.2  1996/02/18  22:59:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:42  marty]
 *
 * Revision 1.1.5.1  1995/12/13  16:25:23  root
 * 	Submit
 * 	[1995/12/11  15:14:49  root]
 * 
 * Revision 1.1.2.5  1993/03/08  14:37:40  ahop
 * 	7340: add rs_login_setup_host_identity
 * 	[1993/03/04  21:30:16  ahop]
 * 
 * Revision 1.1.2.4  1992/12/29  13:08:19  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:32  zeliff]
 * 
 * Revision 1.1.2.3  1992/10/07  20:26:07  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:11:35  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:36:11  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.2.2  1992/08/31  18:18:05  sekhar
 * 	First replication code drop
 * 	[1992/08/31  16:42:51  sekhar]
 * 
 * $EndLog$
 */
/* 
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *     Registry login context management
 *
 *     A slave runs under the host machine identity for the purposes
 *     of communicating with the master.  All replicas run as the
 *     security service principal for the purposes of registering
 *     towers in the namespace.
 *
 *     It is crucial that a slave inheriting the machine identity
 *     do so before logging in as the security service principal.
 *     Therefore, we kill the server immediately when we detect
 *     a bad ordering of calls to the login module.
 */

#ifndef rs_login_h__included
#define rs_login_h__included

#include <dce/sec_login.h>

/* rs_login_inherit_host_identity
 *
 * Called by an uninitialized slave to inherit the host machine identity.
 * Obtain and release the current login context first so we're sure we're
 * not inheriting a logged-in administrator's identity.
 *
 * This routine *MUST* be called (if it is called at all) *before* 
 * rs_login_become_rgy.  To do otherwise is a fatal error.
 */
void  rs_login_inherit_host_identity ( 
#ifdef __STDC__
    error_status_t  *st_p
#endif
);

/* rs_login_setup_host_identity
 *
 * Called by an initialized slave to setup and maintain the host machine identity
 * which it can do because its database contains the host principal's key.
 * By doing this, the slave is not dependent on sec_clientd for this task.
 */
void  rs_login_setup_host_identity (
#ifdef __STDC__
    error_status_t  *st_p
#endif
);



/* rs_login_host_set_auth_info
 *
 * Called by a slave to attach the host machine auth info to its 
 * binding handle to the named server
 *
 */
void rs_login_host_set_auth_info (
#ifdef __STDC__
    rpc_binding_handle_t    rpc_handle,
    unsigned_char_t         *server_name,
    error_status_t          *st_p
#endif
);


/* rs_login_get_host_login_context
 *
 * Obtain the host machine login context inherited by a previous call to
 * rs_login_inherit_host_identity. 
 */
void  rs_login_get_host_login_context (
#ifdef __STDC__
    sec_login_handle_t  *host_login_context,
    error_status_t      *st_p
#endif
);
                   
  
/* rs_login_become_rgy
 *
 * set the default process identity to the architecturally-defined DCE registry 
 * service principal.  If succesful, start a background thread to refresh 
 * the process identity as appropriate. 
 *
 * This routine performs a database lookup to obtain the registry service
 * principal's secret key.  We rely on the caller to provide the appropriate 
 * concurrency control.
 */
void  rs_login_become_rgy ( 
#ifdef __STDC__
    error_status_t  *st_p
#endif
);
    

/* rs_login_shutdown
 *
 * cleans up any persistent data associated with the identities managed by
 * the rs_login_ module.  Should be called by server shutdown routines.
 */
void  rs_login_shutdown  (
#ifdef __STDC__
    void
#endif
);

#endif  /* #ifndef rs_login_h__included */
