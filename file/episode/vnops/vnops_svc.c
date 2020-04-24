/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: vnops_svc.c,v $
 * Revision 1.1.7.1  1996/10/02  17:45:22  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:36:30  damon]
 *
 * Revision 1.1.2.1  1994/08/30  15:55:55  sasala
 * 	DFS S12Y general cleanup - rename ftutil and tools cmd catalogs, add ftutil msgs to xaggr dfsexport, and episode salvage and vnops subcomponents
 * 	[1994/08/30  00:19:36  sasala]
 * 
 * $EndLog$
 */

/*
 * The DFS ftutil (ftl) handle is defined in ftutil which 
 * registers the table with a call to the OSF supplied macro
 * DCE_SVC_DEFINE_HANDLE.
 *
 * DFS s12y is done in a manner to minimize the amount of messages
 * that need be present for any given component. As such, each
 * directory where s12y is done contains a sams file for that code.
 *
 * In order to register the messages needed by a given function,
 * each of the separate message tables used needs to be registered.
 *
 * This module registers the tables for the components used in
 * its invocation.
 */

#include <stdio.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>

#include <dcedfs/dfsftlmac.h>
#include <dcedfs/dfsftlmsg.h>
#include <dcedfs/dfsftlsvc.h>


/*DCE_SVC_DEFINE_HANDLE(ftl_svc_handle,  * The handle (to the ftl table)     **/
/*                    ftl_svc_table,   * and the ftl table itself *           */
/*                    "ftl")           * are defined in ftutil.               */


void dfs_define_vop_message_tables()
        {
          error_status_t st ;

/* register ftl messages       */
          dce_msg_define_msg_table(ftl__table,  /* The ftutil ftl msg table)           */
                                   sizeof ftl__table / sizeof ftl__table[0], /* number */
                                   &st);
          if (st != svc_s_ok)
             fprintf(stderr, "Cannot register ftl incore message table 0x%lx\n", st);


        }
