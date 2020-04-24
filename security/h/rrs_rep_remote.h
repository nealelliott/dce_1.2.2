/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * HISTORY
 * $Log: rrs_rep_remote.h,v $
 * Revision 1.1.8.2  1996/02/18  22:58:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:16  marty]
 *
 * Revision 1.1.8.1  1995/12/13  16:24:52  root
 * 	Submit
 * 	[1995/12/11  15:14:31  root]
 * 
 * Revision 1.1.6.1  1994/07/15  14:59:05  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:15:51  mdf]
 * 
 * Revision 1.1.2.6  1993/01/27  16:40:38  emartin
 * 	bl6d: add rrs_rep_properties_get_info
 * 	[1993/01/18  23:33:59  emartin]
 * 
 * Revision 1.1.2.5  1992/12/29  13:07:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:43:48  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/18  22:10:06  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:45:27  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:05:31  emartin
 * 	bl6c: reset => destroy
 * 
 * Revision 1.1.2.3  1992/10/07  20:25:08  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:10:58  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:33:16  burati
 * 	 Second replication code drop: bl5
 * 	 Decl rrs_rep_adm_reset
 * 
 * Revision 1.1.2.2  1992/08/31  18:04:59  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  16:32:16  sekhar]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*    
 *      Registry server-to-server client calls 
 */
    
    
#ifndef rrs_rep_remote_h__included
#define rrs_rep_remote_h__included

#include <dce/nbase.h>
#include <dce/rgynbase.h>

#include <dce/rplbase.h>

PUBLIC void rrs_rep_adm_info (
#ifdef __STDC__
    rpc_binding_handle_t    h,
    rs_replica_info_t       *rep_info,
    error_status_t          *st
#endif
);

PUBLIC void rrs_rep_adm_destroy (
#ifdef __STDC__
    rpc_binding_handle_t    h,
    error_status_t          *st
#endif
);

PUBLIC void rrs_rep_replist_add_replica (
#ifdef __STDC__
    rpc_binding_handle_t    h,
    uuid_p_t                rep_id,
    unsigned_char_p_t       rep_name,
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st
#endif
);

PUBLIC void rrs_rep_replist_read (
#ifdef __STDC__
    rpc_binding_handle_t    h,
    uuid_t                  *marker,
    unsigned32              max_ents,
    unsigned32              *n_ents,
    rs_replica_item_t       replist[],
    error_status_t          *st
#endif
);

PUBLIC void rrs_rep_properties_get_info (
#ifdef __STDC__
    rpc_binding_handle_t    h,
    sec_rgy_properties_t    *properties,
    rs_cache_data_t         *cache_info,
    error_status_t          *st
#endif
);

#endif
