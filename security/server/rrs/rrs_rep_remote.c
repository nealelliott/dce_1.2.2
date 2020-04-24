/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_rep_remote.c,v $
 * Revision 1.1.11.3  1996/02/18  00:17:03  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:02  marty]
 *
 * Revision 1.1.11.2  1995/12/08  17:48:48  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/15  21:04 UTC  greg
 * 	Merge client compatibility fixes from greg_sec_migrate2.
 * 	[1995/05/03  23:51 UTC  greg  /main/SEC_migrate_merge/2]
 * 
 * 	resolve binding handles before use.
 * 	[1995/05/02  20:30 UTC  greg  /main/SEC_migrate_merge/greg_sec_migrate2/1]
 * 
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/21  19:53 UTC  greg  /main/SEC_migrate_merge/1]
 * 
 * 	Merge .../greg_sec_migrate/LATEST --> .../SEC_migrate/LATEST.
 * 	[1995/02/11  22:14 UTC  greg  /main/SEC_migrate/2]
 * 
 * 	account for new version (1.2) of repadm interface,
 * 
 * 	[1995/02/07  14:20 UTC  greg  /main/SEC_migrate/greg_sec_migrate/2]
 * 
 * 	Use new 2.0 replist interface in all calls, even though only the
 * 	sematics of read_replist_full()  have changed.   This will allow us
 * 	to drop the old interface  registration as soon as all replicas are
 * 	up to 1.1.
 * 	[1995/02/02  18:33 UTC  greg  /main/SEC_migrate/greg_sec_migrate/1]
 * 
 * 	Add compatibility code for rrs_replist_read_full().
 * 
 * 	[1995/01/30  04:45 UTC  greg  /main/SEC_migrate/1]
 * 
 * Revision 1.1.7.1  1994/06/02  21:26:57  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:17:18  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  11:46:48  mdf]
 * 
 * Revision 1.1.4.2  1993/10/29  15:57:20  hanfei
 * 	work for change master : add entry rrs_replist_read_full.
 * 	[1993/10/29  15:56:14  hanfei]
 * 
 * 	 Second replication code drop: bl5
 * 	[1992/10/06  15:56:18  burati]
 * 
 * Revision 1.1.2.3  1993/09/15  15:49:27  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.8  1993/02/01  18:14:54  burati
 * 	CR7078 bsubmit/merge put dup copies of fcn in w/out notifying me...
 * 	[1993/02/01  16:23:06  burati]
 * 
 * Revision 1.1.2.7  1993/01/29  18:53:52  burati
 * 	Merge in Liza's bl6d changes so my copy will build in my sbox
 * 	[1993/01/29  16:35:30  burati]
 * 
 * 	Change st to status, to match ACF file
 * 	[1993/01/18  17:15:30  burati]
 * 
 * Revision 1.1.2.6  1993/01/26  23:03:01  emartin
 * 	bl6d: add rrs_rep_properties_get_info
 * 	[1993/01/08  16:12:32  emartin]
 * 
 * Revision 1.1.2.5  1992/12/29  16:34:00  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:22:07  zeliff]
 * 
 * Revision 1.1.2.4  1992/12/18  22:11:30  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:48:33  sekhar]
 * 
 * Revision 1.1.5.2  1992/12/11  21:39:25  emartin
 * 	bl6c: rs_repadm_reset->destroy
 * 
 * Revision 1.1.2.3  1992/10/07  20:31:01  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:14:50  sekhar]
 * 
 * Revision 1.1.2.2  1992/08/31  18:22:46  sekhar
 * 	First replication code drop.
 * 	[1992/08/31  17:51:07  sekhar]
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1992, 1993, 1995
 * Unpublished work. All Rights Reserved.
 */
/*    
 *      Registry server-to-server client calls 
 */
    
#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rrs_rep_remote.c,v 1.1.11.3 1996/02/18 00:17:03 marty Exp $";
#endif
    
#include <dce/nbase.h>
#include <dce/rplbase.h>

#include <rs_repadm.h>
#include <rs_repadm_1_1.h>
#include <rs_repmgr.h>
#include <rs_replist.h>
#include <rs_replist_1_0.h>
#include <rs_plcy.h>

#include <rs_base.h>
#include <rs_pvt.h>
#include <rs_rep_pvt.h>

#include <rrs_rep_remote.h>
#include <sec_bind_util.h>

PUBLIC void rrs_rep_adm_info 
  (
    rpc_binding_handle_t    h,
    rs_replica_info_t       *rep_info,
    error_status_t          *status
  )
{
    (*rs_repadm_v1_1_c_epv.rs_rep_admin_info)
        (h, rep_info, status);
}

PUBLIC void rrs_rep_adm_destroy
  (
    rpc_binding_handle_t    h,
    error_status_t          *status
  )
{
    (*rs_repadm_v1_1_c_epv.rs_rep_admin_destroy)
        (h, status);
}

PUBLIC void rrs_rep_replist_add_replica
  (
    rpc_binding_handle_t    h,
    uuid_p_t                rep_id,
    unsigned_char_p_t       rep_name,
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *status
  )
{
    if (sec_bind_util_resolve_binding(h, rs_replist_v2_0_c_ifspec, NULL, status)) 
        (*rs_replist_v2_0_c_epv.rs_replist_add_replica)
            (h, rep_id, rep_name, rep_twrs, status);
    if (STATUS_EQUAL(status, rpc_s_unknown_if)
    || STATUS_EQUAL(status, ept_s_not_registered)) {
	(*rs_replist_v1_0_c_epv.rs_replist_add_replica)
	    (h, rep_id, rep_name, rep_twrs, status);
    }
}

PUBLIC void rrs_rep_replist_read
  (
    rpc_binding_handle_t    h,
    uuid_t                  *marker,
    unsigned32              max_ents,
    unsigned32              *n_ents,
    rs_replica_item_t       replist[],
    error_status_t          *status
  )
{
    if (sec_bind_util_resolve_binding(h, rs_replist_v2_0_c_ifspec, NULL, status)) 
	(*rs_replist_v2_0_c_epv.rs_replist_read)
	    (h, marker, max_ents, n_ents, replist, status);
    if (STATUS_EQUAL(status, rpc_s_unknown_if)
    || STATUS_EQUAL(status, ept_s_not_registered)) {
	(*rs_replist_v1_0_c_epv.rs_replist_read)
	    (h, marker, max_ents, n_ents, replist, status);
    }
}

PUBLIC  void  rrs_replist_read_full
  (
    handle_t                h,          /* [in] */
    uuid_t                  *marker,    /* [in, out] */
    unsigned32              max_ents,   /* [in] */
    unsigned32              *n_ents,    /* [out] */
    rs_replica_item_full_t  replist[],  /* [out, length_is(*n_ents),
 size_is(max_ents)] */
    error_status_t          *status
  )
{
    if (sec_bind_util_resolve_binding(h, rs_replist_v2_0_c_ifspec, NULL, status)) 
	(*rs_replist_v2_0_c_epv.rs_replist_read_full)
	    (h, marker, max_ents, n_ents, replist, status);
    if (STATUS_EQUAL(status, rpc_s_unknown_if)
    || STATUS_EQUAL(status, ept_s_not_registered)) {
	(*rs_replist_v1_0_c_epv.rs_replist_read_full)
	    (h, marker, max_ents, n_ents, replist, status);
    }
}

PUBLIC void rrs_rep_properties_get_info
  (
    rpc_binding_handle_t    h,
    sec_rgy_properties_t    *properties,
    rs_cache_data_t         *cache_info,
    error_status_t          *st
  )
{
    (*rs_policy_v1_0_c_epv.rs_properties_get_info)
	(h, properties, cache_info, st);
}
