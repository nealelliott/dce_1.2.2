/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_bind.c,v $
 * Revision 1.1.7.3  1996/02/18  00:17:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:06:38  marty]
 *
 * Revision 1.1.7.2  1995/12/08  17:51:06  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/15  03:28 UTC  rps
 * 
 * 	HP revision /main/rps_mothra/1  1994/12/15  00:16 UTC  rps
 * 	[OT12669] cell renaming
 * 	[1995/12/08  17:20:21  root]
 * 
 * Revision 1.1.2.5  1993/02/26  23:50:27  emartin
 * 	em_bl6f: cellname and update_site_name args
 * 	[1993/02/24  22:30:22  emartin]
 * 
 * Revision 1.1.2.4  1992/12/29  16:34:53  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:23:23  zeliff]
 * 
 * Revision 1.1.2.3  1992/12/18  22:11:47  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:49:01  sekhar]
 * 
 * Revision 1.1.3.2  1992/12/11  21:41:27  emartin
 * 	bl6c: use BEGIN_READ_EVENT
 * 
 * Revision 1.1.2.2  1992/11/04  19:35:22  ahop
 * 	Third replication drop: bl6a
 * 	[1992/11/03  18:29:03  ahop]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*    
 *      Registry server - get reference to update site
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = 
    "$Id: rs_bind.c,v 1.1.7.3 1996/02/18 00:17:42 marty Exp $";
#endif
    
#include <dce/rgynbase.h>

#include <dce/rplbase.h>
#include <rs_bind.h>

#include <rs_base.h>
#include <rs_pvt.h>
#include <rs_state.h>
#include <rs_master.h>
#include <rs_lock.h>
#include <rs_ch_lock.h>
#include <rs_util.h>
#include <rs_replist2.h>
#include <rsdb_policy.h>

PRIVATE void get_update_site
#ifndef __STDC__
    (cellname, update_site, 
     update_site_name, update_site_id, update_site_twrs, st)
    sec_rgy_name_t          cellname;
    boolean32               *update_site;       /* is this site an update site? */
    rs_replica_name_p_t     *update_site_name;  /* cell-relative name of update site */
    uuid_t                  *update_site_id;    /* uuid of an update site */
    rs_replica_twr_vec_p_t  *update_site_twrs;  /* tower set of update site */
    error_status_t          *st;
#else
  (
    sec_rgy_name_t          cellname,
    boolean32               *update_site,       /* is this site an update site? */
    rs_replica_name_p_t     *update_site_name,  /* cell-relative name of update site */
    uuid_t                  *update_site_id,    /* uuid of an update site */
    rs_replica_twr_vec_p_t  *update_site_twrs,  /* tower set of update site */
    error_status_t          *st
  )
#endif
{
    boolean32               master_known;
    uuid_t                  master_id;
    rs_update_seqno_t       master_seqno;
    char		    *cell_name;
    rs_replica_item_t       rep_item;
    unsigned32              nreps;

    *update_site = rs_state_master();

    rs_master_info_get(&master_known, &master_id,
        &master_seqno, st);
    if (BAD_STATUS(st)) return;

    if (!master_known) {
        SET_STATUS(st, sec_rgy_rep_master_not_found);
        return;
    }

    dce_cf_get_cell_name(&cell_name, st);
    if (BAD_STATUS(st)) return;

    strncpy((char *)cellname, cell_name,
        sizeof(sec_rgy_name_t));
    free(cell_name);

    /* pickup the master_id before calling
     * rs_replist2_read which will bump
     * the contents of master_id to the
     * next replica's uuid.
     */
    *update_site_id = master_id;

    /* get master's name and towers from replist.
     * rs_replist2_read uses rpc_ss_allocate
     * to get memory for towers and name; this 
     * memory will be freed by stub
     */
    rs_replist2_read(&master_id,
        1, &nreps, &rep_item, st);
    if (BAD_STATUS(st)) return;

    *update_site_name = rep_item.rep_name;
    *update_site_twrs = rep_item.rep_twrs;
}

PUBLIC void rs_bind_get_update_site
#ifndef __STDC__
    (h, cellname, update_site, 
     update_site_name, update_site_id, update_site_twrs, st)
    handle_t                h;
    sec_rgy_name_t          cellname;
    boolean32               *update_site;       /* is this site an update site? */
    rs_replica_name_p_t     *update_site_name;  /* cell-relative name of update site */
    uuid_t                  *update_site_id;    /* uuid of an update site */
    rs_replica_twr_vec_p_t  *update_site_twrs;  /* tower set of update site */
    error_status_t          *st;
#else
  (
    handle_t                h,
    sec_rgy_name_t          cellname,
    boolean32               *update_site,       /* is this site an update site? */
    rs_replica_name_p_t     *update_site_name,  /* cell-relative name of update site */
    uuid_t                  *update_site_id,    /* uuid of an update site */
    rs_replica_twr_vec_p_t  *update_site_twrs,  /* tower set of update site */
    error_status_t          *st
  )
#endif
{
    *update_site_name = NULL;
    *update_site_twrs = NULL;

    BEGIN_READ_EVENT(rs_c_event_primitive_read, lock_replist, st) {

        get_update_site(cellname, update_site, update_site_name,
            update_site_id, update_site_twrs, st);

    } END_READ_EVENT;
}


