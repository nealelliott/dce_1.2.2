/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_log_pgo.h,v $
 * Revision 1.1.8.1  1996/05/10  13:15:35  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 *
 * 	HP revision /main/DCE_1.2/1  1996/03/27  17:45 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 *
 * 	HP revision /main/hanfei_dce1.2_b1/1  1996/02/29  16:20 UTC  hanfei
 * 	add uuids as arguments to rs_log_pgo_add_member().
 * 	[1996/05/09  20:49:28  arvind]
 *
 * Revision 1.1.4.1  1994/07/15  14:59:21  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:16:36  mdf]
 * 
 * Revision 1.1.2.2  1992/12/29  13:08:05  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:14:50  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rs_log_pgo.h V=8 08/22/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - PGO info Log operations
 * 
 */

#ifndef rs_log_pgo_h__included
#define rs_log_pgo_h__included

#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/rplbase.h>


void rs_log_pgo_init ( 
#ifdef __STDC__
    error_status_t   *st
#endif
);

void rs_log_pgo_add ( 
#ifdef __STDC__
    sec_rgy_domain_t           domain,
    sec_rgy_name_t             pgo_name,
    sec_rgy_pgo_item_t         *pgo_item,
    sec_rgy_foreign_id_t       *client_ids,
    rs_replica_master_info_t   *master_info
#endif
);

void rs_log_pgo_delete ( 
#ifdef __STDC__
    sec_rgy_domain_t           domain,
    sec_rgy_name_t             name,
    rs_replica_master_info_t   *master_info
#endif
);

void rs_log_pgo_replace ( 
#ifdef __STDC__
    sec_rgy_domain_t           domain,
    sec_rgy_name_t             name,
    sec_rgy_pgo_item_t         *item,
    rs_replica_master_info_t   *master_info
#endif
);

void rs_log_pgo_rename ( 
#ifdef __STDC__
    sec_rgy_domain_t           domain,
    sec_rgy_name_t             old_name,
    sec_rgy_name_t             new_name,
    rs_replica_master_info_t   *master_info
#endif
);

void rs_log_pgo_add_member ( 
#ifdef __STDC__
    sec_rgy_domain_t           domain,
    sec_rgy_name_t             go_name,
    sec_rgy_name_t             person_name,
    uuid_p_t		       cell_uuid_p,
    uuid_p_t		       princ_uuid_p,
    rs_replica_master_info_t   *master_info
#endif
);

void rs_log_pgo_delete_member ( 
#ifdef __STDC__
    sec_rgy_domain_t           domain,
    sec_rgy_name_t             go_name,
    sec_rgy_name_t             person_name,
    rs_replica_master_info_t   *master_info
#endif
);

#endif /* rs_log_pgo_h__included */
