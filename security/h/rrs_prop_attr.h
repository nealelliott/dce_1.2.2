/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_attr.h,v $
 * Revision 1.1.4.2  1996/02/18  22:58:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:11  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:27:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:23  root]
 * 
 * Revision 1.1.2.5  1994/09/02  17:47:25  mdf
 * 	CR 11868  Initial checkin of missing functionality that propagates removal of attributes
 * 	[1994/08/30  13:36:09  mdf]
 * 
 * Revision 1.1.2.4  1994/06/10  17:13:13  annie
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:09:30  annie]
 * 
 * Revision 1.1.2.3  1994/06/02  20:45:25  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 
 * 	 /main/ODESSA_2/1  1994/04/26  19:53 UTC  mdf
 * 	Support for ERA propagation.
 * 
 * 	 /main/mdf_odessa_era_bl33/1  1994/04/26  19:47 UTC  mdf
 * 	Support for ERA propagation.
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server Propagation Client - Attributes
 */
#ifndef __rrs_prop_attr_h__included__
#define __rrs_prop_attr_h__included__

#include <dce/rgynbase.h>
#include <dce/rplbase.h>
#include <rs_prop_attr.h>
#include <rs_rep_auth.h>
#include <rs_rep_pvt.h>

void rrs_prop_attr_update
(
    handle_t                            h,
    unsigned32                          number,
    rs_prop_attr_data_t		        entries[],
    rs_replica_master_info_t            *master_info,
    boolean32                           propq_only,
    error_status_t                      *st_p
);

void rrs_prop_attr_update_all
(
    rs_rep_binding_t          *rep_binding,
    rs_replica_master_info_t  *master_info,
    error_status_t            *st_p
);

void rrs_prop_attr_delete
(
    handle_t                    h,
    unsigned32                  number,
    rs_prop_attr_data_t		entries[],
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st_p
) ;

#endif  /* __rrs_prop_attr_h__included__ */

