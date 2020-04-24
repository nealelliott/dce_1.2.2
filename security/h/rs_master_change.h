/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_master_change.h,v $
 * Revision 1.1.4.2  1996/02/18  22:59:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:44  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:27:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:27  root]
 * 
 * Revision 1.1.2.3  1994/06/02  21:03:57  mdf
 * 	Merged with changes from 1.1.2.2
 * 	[1994/06/02  21:03:46  mdf]
 * 
 * Revision 1.1.2.2  1994/06/02  20:21:45  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:14:57  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - master change declarations.
 */
#ifndef rs_master_change_h__included
#define rs_master_change_h__included


#include <dce/nbase.h>
#include <dce/rgynbase.h>

#include <dce/rplbase.h>

#include <macros.h>


/*
 * support for rs_rep_admin_change_master.
 *
 * old master side of change_master
 */
PUBLIC  void  rs_mc_change_master (
#ifdef __STDC__
    handle_t            h,
    uuid_p_t            new_master_id,  /* [in] */
    error_status_t      *st
#endif
);

/*
 * support for rs_rep_mgr_become_master.
 *
 * new master side of change master
 *
 * The master tells a slave to become the new master.
 * The base_prop_seqno is the sequence number of
 * the earliest update currently on the prop queue.
 */
PUBLIC void rs_mc_become_master (
#ifdef __STDC__
    rs_update_seqno_t           base_prop_seqno,   /* [in] */
    rs_replica_master_info_p_t  old_master_info,   /* [in] */
    rs_replica_master_info_t    *new_master_info,  /* [out] */
    error_status_t              *st
#endif
);

/*
 * support for rs_rep_admin_become_master
 *
 * do the work to force become a master.
 */
PUBLIC  void  rs_mc_force_become_master (
#ifdef __STDC__
    handle_t            h,
    error_status_t      *st
#endif
);

/*
 * support for rs_rep_admin_become_slave
 *
 * do the work to become a slave
 */
PUBLIC  void  rs_mc_become_slave (
#ifdef __STDC__
    handle_t            h,
    error_status_t      *st
#endif
);


#endif

