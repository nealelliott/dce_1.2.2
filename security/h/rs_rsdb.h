/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_rsdb.h,v $
 * Revision 1.1.5.2  1996/02/18  22:59:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:13  marty]
 *
 * Revision 1.1.5.1  1995/12/08  17:27:38  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/03/27  19:32 UTC  cuti
 * 	CHFts14111 bug fix
 * 
 * 	HP revision /main/HPDCE02/cuti_mothra_bug/1  1995/03/23  18:49 UTC  cuti
 * 	Add signature to rsdb_shutdown
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/08  17:29 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_cpi_bl2/1  1994/12/07  19:31 UTC  hanfei
 * 	merge
 * 
 * 	HP revision /main/hanfei_cpi_bl1/1  1994/12/05  14:48 UTC  hanfei
 * 	work for rgy checkpoint to be configurable
 * 	[1995/12/08  16:53:33  root]
 * 
 * Revision 1.1.2.6  1993/02/26  23:48:11  emartin
 * 	em_bl6f: rsdb_checkpt takes next_rep_state arg.
 * 	[1993/02/22  22:52:50  emartin]
 * 
 * Revision 1.1.2.5  1993/02/19  17:11:44  ahop
 * 	dbcp: add rep_twrs to rsdb_initialize argument list
 * 	[1993/02/19  08:39:47  ahop]
 * 
 * Revision 1.1.2.4  1992/12/29  13:25:23  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:43  zeliff]
 * 
 * Revision 1.1.2.3  1992/10/07  20:27:46  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:34  sekhar]
 * 
 * Revision 1.1.3.2  1992/10/06  15:42:13  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1.2.2  1992/08/31  18:19:29  sekhar
 * 	First security replication code drop
 * 	[1992/08/31  14:57:24  sekhar]
 * 
 * Revision 1.1  1992/01/19  14:42:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/* 
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Server - database interlude - function definitions
 *
 */

#ifndef rs_rsdb_h__included
#define rs_rsdb_h__included

#include <dce/rgynbase.h>
#include <rs_lock.h>

void rsdb_initialize (
    rs_replica_twr_vec_p_t  rep_twrs,
    error_status_t          *st
);

void rsdb_load_init (
    unsigned32      rep_state,
    error_status_t  *st
);

void rsdb_shutdown (
    boolean32       checkpoint,
    lock_mode_t     lock_mode
);

void rsdb_set_checkpt_interval (
    signed32        interval
);

void  rsdb_checkpt_reset_interval (
    boolean32   do_checkpt_now,
    signed32	new_checkpt_interval,
    char	*at_time_str,
    error_status_t    *status
);

void rsdb_start_checkpt_task (
    error_status_t  *st
);

void rsdb_checkpt (
    unsigned32      next_rep_state,
    lock_mode_t     lock_mode,
    error_status_t  *st
);


#endif /* rs_rsdb_h__included */

