/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_rep_bind.h,v $
 * Revision 1.1.7.2  1996/02/18  22:59:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:59  marty]
 *
 * Revision 1.1.7.1  1995/12/13  16:25:41  root
 * 	Submit
 * 
 * 	HP revision /main/HPDCE02/1  1995/02/03  15:54 UTC  emartin
 * 	merge in some replication fixes
 * 
 * 	HP revision /main/emartin_mothra2/1  1995/01/30  22:21 UTC  emartin
 * 	nuke rs_rep_bind_auth_to_slave
 * 	[1995/12/11  15:15:01  root]
 * 
 * Revision 1.1.5.3  1994/06/02  21:08:07  mdf
 * 	Merged with changes from 1.1.5.2
 * 	[1994/06/02  21:07:56  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:08  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	add decl for rs_rep_bind_init_auth_to_master
 * 	[1993/12/16  17:16:12  hanfei]
 * 
 * 	bl6c: many new routine decls
 * 	[1992/12/11  21:31:51  emartin]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:45  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.5.2  1994/06/02  20:21:52  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:15:08  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * 	add decl for rs_rep_bind_init_auth_to_master
 * 	[1993/12/16  17:16:12  hanfei]
 * 
 * 	bl6c: many new routine decls
 * 	[1992/12/11  21:31:51  emartin]
 * 
 * Revision 1.1.2.3  1993/09/15  15:39:45  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.6  1993/02/10  20:34:01  emartin
 * 	bl6e: nuke rs_rep_bind_to_master
 * 	[1993/02/04  23:26:32  emartin]
 * 
 * Revision 1.1.2.5  1993/01/26  23:00:34  emartin
 * 	decl rs_rep_bind_init_from_handle_na
 * 	[1993/01/13  16:25:50  emartin]
 * 
 * Revision 1.1.2.4  1992/12/29  13:25:00  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:12  zeliff]
 * 
 * Revision 1.1.2.3  1992/12/18  22:10:42  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:39  sekhar]
 * 
 * Revision 1.1.3.2  1992/12/11  21:31:51  emartin
 * 	bl6c: many new routine decls
 * 
 * Revision 1.1.2.2  1992/08/31  19:23:24  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  16:47:02  sekhar]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*    
 *  Registry server routines to bind to another replica
 */
    
#ifndef rs_rep_bind_h__included
#define rs_rep_bind_h__included

#include <dce/nbase.h>
#include <dce/rgynbase.h>

#include <dce/rplbase.h>

PUBLIC void rs_rep_bind_from_twrs (
#ifdef __STDC__
    rs_replica_twr_vec_p_t  twrs,
    rpc_binding_handle_t    *ohandle,
    error_status_t          *st
#endif
);

/*
 * Initialize a rep_binding.  Copy
 * towers with compatible protocols
 * into a tower vector for this 
 * rep_binding.
 * NB: the rep_handle is not filled by this
 * call.  It is filled by later calls to
 * the various flavors of rs_rep_bind...
 */
PUBLIC void rs_rep_bind_init_binding (
#ifdef __STDC__
    rs_replica_twr_vec_p_t  rep_twrs,
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
#endif
);

PUBLIC void rs_rep_bind_init_from_handle (
#ifdef __STDC__
    rpc_binding_handle_t    rpc_handle,
    rs_replica_auth_p_t     auth_info,
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
#endif
);

#ifdef RS_REP_NAUTH
PUBLIC void rs_rep_bind_init_from_handle_na (
#ifdef __STDC__
    rpc_binding_handle_t    rpc_handle,
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
#endif
);                 
#endif

/*
 * Get an rpc binding handle from the "next" good tower
 */
PUBLIC void rs_rep_bind (
#ifdef __STDC__
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
#endif
);

/*
 * Free storage associated with rep_binding
 */
PUBLIC void rs_rep_bind_free_binding (
#ifdef __STDC__
    rs_rep_binding_t        *rep_binding
#endif
);

PUBLIC void rs_rep_bind_auth_to_slave_rfrsh (
#ifdef __STDC__
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
#endif
);

PUBLIC void rs_rep_bind_init_auth_to_slave (
#ifdef __STDC__
    rs_replica_twr_vec_p_t  rep_twrs,
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
#endif
);

/* 
 * Get an authenticated rpc handle for slave to master
 * operations.  
 *
 * The slave uses its host machine identity when 
 * doing operations with the (surrogate) master;
 * the host login context should have already been
 * setup.
 */
PUBLIC void rs_rep_bind_auth_to_master (
#ifdef __STDC__
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
#endif
);

/*
 * Init a binding to the master as remembered
 * in rs_master_info
 *
 * Must be invoked with replist read lock.
 */
PUBLIC void  rs_rep_bind_init_to_master (
#ifdef __STDC__
    rs_rep_binding_t        *rep_binding,
    error_status_t          *st
#endif
);

/*
 *  Bind new master with authentication to old master.
 */
PUBLIC  void rs_rep_bind_init_auth_to_master (
#ifdef __STDC__
    rs_rep_binding_t    *rep_binding,   /* [out] */
    error_status_t      *st             /* [out] */
#endif
);

#endif


