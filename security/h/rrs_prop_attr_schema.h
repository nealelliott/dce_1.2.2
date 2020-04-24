/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rrs_prop_attr_schema.h,v $
 * Revision 1.1.8.3  1996/02/18  22:58:20  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:12  marty]
 *
 * Revision 1.1.8.2  1995/12/08  17:27:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:24  root]
 * 
 * Revision 1.1.4.1  1994/10/25  21:13:55  burati
 * 	CR12713 Attribute schema updates not logged or propagated
 * 	[1994/10/25  19:55:15  burati]
 * 
 * Revision 1.1.2.4  1994/06/10  17:13:14  annie
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:09:31  annie]
 * 
 * Revision 1.1.2.3  1994/06/02  20:47:36  mdf
 * 	Merged with changes from 1.1.2.2
 * 	[1994/06/02  20:47:26  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 
 * 	hp_sec_to_osf_3 drop
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
 *      Registry Server Propagation Client - Attribute Schema
 */
#ifndef __rrs_prop_attr_sch_h__included__
#define __rrs_prop_attr_sch_h__included__

#include <dce/rgynbase.h>
#include <dce/rplbase.h>
#include <rs_prop_attr_schema.h>
#include <rs_rep_auth.h>
#include <rs_rep_pvt.h>

void rrs_prop_attr_schema_create
(
    handle_t                            h,
    unsigned32                          number,
    rs_prop_attr_sch_create_data_t      entries[],
    rs_replica_master_info_t            *master_info,
    boolean32                           propq_only,
    error_status_t                      *st_p
);

void rrs_prop_attr_schema_create_all
(
    rs_rep_binding_t          *rep_binding,
    rs_replica_master_info_t  *master_info,
    error_status_t            *st_p
);

void rrs_prop_attr_schema_delete
(
    handle_t                    h,
    sec_attr_component_name_t   schema_name,
    uuid_t                      *attr_id,
    rs_replica_master_info_t    *master_info,
    boolean32                   propq_only,
    error_status_t              *st_p
);

void rrs_prop_attr_schema_update
(
    handle_t                        h,
    rs_prop_attr_sch_update_data_t  *entry_p,
    rs_replica_master_info_t        *master_info,
    boolean32                       propq_only,
    error_status_t                  *st_p
);


#endif  /* __rrs_prop_attr_sch_h__included__ */
