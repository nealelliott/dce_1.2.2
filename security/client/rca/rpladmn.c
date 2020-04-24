/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpladmn.c,v $
 * Revision 1.1.6.2  1996/03/11  13:28:13  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:59  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:07:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:32:37  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:03:40  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:13:45  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:47:24  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rpladmn.c V=3 10/11/91 //littl/prgy/src/client/rca
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 *	Registry replication - Client agent 
 */
  
#if !defined(LINT) && !defined(apollo)
static char *VersionID = "@(#)rpladmn.c	3 - 10/11/91";
#endif

#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>
#include <signal.h>

#include "rca_pvt.h"

#include <rpladmn.h>
#include <rs_rpladmn.h>


#ifdef NOTDEF
 
#define Crs_rep_admin_info(CH, b, c)\
        (*rs_rpladmn_v1_0_c_epv.rs_rep_admin_info)(HANDLE(CH), b, c)

#endif /* NOTDEF */

#define Crs_rep_admin_stop(CH, b)\
        (*rs_rpladmn_v1_0_c_epv.rs_rep_admin_stop)(HANDLE(CH), b)

#define Crs_rep_admin_maint(CH, b, c)\
        (*rs_rpladmn_v1_0_c_epv.rs_rep_admin_maint)(HANDLE(CH), b, c)

#define Crs_rep_admin_mkey(CH, b)\
        (*rs_rpladmn_v1_0_c_epv.rs_rep_admin_mkey)(HANDLE(CH), b)


PUBLIC void sec_rgy_rep_admin_stop 
#ifndef __STDC__
    ( context, st )
    sec_rgy_handle_t    context;
    error_status_t      *st;
#else
    (
    sec_rgy_handle_t    context,
    error_status_t      *st
    )
#endif
{

    SETUP_RETRY(rca_op_read, st) {
        NORMAL_CASE {
	    Crs_rep_admin_stop(context, st);  
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_rep_admin_maint 
#ifndef __STDC
    ( context, in_maintenance, st )
    sec_rgy_handle_t    context;
    boolean32           in_maintenance;
    error_status_t      *st;
#else
    (
    sec_rgy_handle_t    context,
    boolean32           in_maintenance,
    error_status_t      *st
    )
#endif
{
    SETUP_RETRY(rca_op_read, st) {
        NORMAL_CASE {
	    Crs_rep_admin_maint(context, in_maintenance, st);  
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_rep_admin_mkey
#ifndef __STDC__
    ( context, st )
    sec_rgy_handle_t    context;
    error_status_t      *st;
#else
    (
    sec_rgy_handle_t    context,
    error_status_t      *st
    )
#endif
{

    SETUP_RETRY(rca_op_read, st) {
        NORMAL_CASE {
	    Crs_rep_admin_mkey(context, st);  
        }
        RETRY_CASE;
    }
    RETRY_END;
}


#ifdef NOTDEF
PUBLIC void sec_rgy_rep_admin_info
#ifndef __STDC__
    ( context, infop, st ) 
    sec_rgy_handle_t    context;
    rs_replica_info_t   *infop;
    error_status_t      *st;
#else
  (
    sec_rgy_handle_t    context,
    rs_replica_info_t   *infop,
    error_status_t      *st
  )
#endif
{
    SETUP_RETRY(rca_op_read, st) {
        NORMAL_CASE {
	    Crs_rep_admin_info(context, infop, st);  
        }
        RETRY_CASE;
    }
    RETRY_END;
}

#endif

