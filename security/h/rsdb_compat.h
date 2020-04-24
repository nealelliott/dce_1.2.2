/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_compat.h,v $
 * Revision 1.1.10.1  1996/05/10  13:16:53  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:45 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/03/15  17:01 UTC  hanfei
 * 	add rsdb_compat_dce122_foreign_person_create(),
 * 	    rsdb_compat_dce122_foreign_person_init()
 * 	and rsdb_compat_reformat_database_for_dce122().
 *
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:11 UTC  psn
 * 	Create SEC_migrate_merge mainline from SEC_migrate and HPDCE02.
 * 	[1995/04/22  13:35 UTC  greg  /main/SEC_migrate_merge/1]
 *
 * 	merge migration codes
 * 	[1995/02/07  14:53 UTC  hanfei  /main/SEC_migrate/1]
 *
 * 	work for rsdb replica compatibility to dce1.1
 * 	[1995/02/06  06:35 UTC  hanfei  /main/hanfei_comp_bl2/2]
 *
 * 	compatibility work for master_info.
 * 	[1995/02/03  04:23 UTC  hanfei  /main/hanfei_comp_bl2/1]
 *
 * Revision 1.1.4.2  1994/08/25  16:30:07  hanfei
 * 	dce1.1 rgy database backward compatibility work
 * 	[1994/08/25  14:52:11  hanfei]
 * 
 * Revision 1.1.4.1  1994/06/17  18:42:06  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:43  devsrc]
 * 
 * Revision 1.1.2.3  1993/02/19  17:11:58  ahop
 * 	dbcp: add rep_twrs to reformat_database arg list
 * 	[1993/02/19  08:40:22  ahop]
 * 
 * Revision 1.1.2.2  1993/02/10  17:19:19  ahop
 * 	Initial revision
 * 	[1993/02/03  21:32:43  ahop]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 * Registry Database Backwards Compatibility - private types
 */

#ifndef rsdb_compat_h__included
#define rsdb_compat_h__included

#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <rsdb_pvt.h>
#include <rsdb.h>


void rsdb_compat_reformat_database (
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st_p
);

void rsdb_compat_policy_replist_acls (
    sec_id_t                *local_cell,
    rsdb_pvt_id_t           policy_acl_id,
    rsdb_pvt_id_t           *replist_acl_id,
    error_status_t          *st_p
);

void rsdb_compat_reformat_database_for_dce11 (
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st_p
);

void rsdb_compat_convert_master_info_for_dce11 (
    rs_master_info_t  	*master_info_p,
    error_status_t	*st_p
);

void rsdb_compat_dce122_foreign_person_create (
    error_status_t          *st_p
);

void rsdb_compat_dce122_foreign_person_init (
);

void rsdb_compat_reformat_database_for_dce122 (
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st_p
);

#endif /* rsdb_compat_h__included */
