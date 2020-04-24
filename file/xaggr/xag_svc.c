/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xag_svc.c,v $
 * Revision 1.1.7.1  1996/10/02  21:11:31  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:31  damon]
 *
 * Revision 1.1.2.2  1994/08/30  15:56:19  sasala
 * 	DFS S12Y general cleanup - rename ftutil and tools cmd catalogs, add ftutil msgs to xaggr dfsexport, and episode salvage and vnops subcomponents
 * 	[1994/08/30  00:20:00  sasala]
 * 
 * Revision 1.1.2.1  1994/08/01  17:36:17  sasala
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  15:41:21  sasala]
 * 
 * $EndLog$
 */

/*
 * This module declares the DFS xaggr (xag) handle and
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
 * This module also registers the tables for the components used in
 * its invocation.
 */

#include <stdio.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>

#include <dfsxagmac.h>
#include <dfsxagsvc.h>
#include <dfsxagmsg.h>

#include <dcedfs/dfsftlmac.h>
#include <dcedfs/dfsftlmsg.h>
#include <dcedfs/dfsftlsvc.h>

DCE_SVC_DEFINE_HANDLE(xag_svc_handle,  /* The xaggr handle (to the xag table) */
                      xag_svc_table,   /* The xag table itself */
                      "xag")                                  

/* DCE_SVC_DEFINE_HANDLE(ftl_svc_handle,  * The handle (to the ftl table)       */
/*                      ftl_svc_table,   * and ftl table itself                 */             
/*                      "ftl")           * are defined in ftutil.               */



void dfs_define_xag_message_tables()
        {
          error_status_t st ;

          /* register xag messages       */
          dce_msg_define_msg_table(xag__table,  /* The xaggr xag  msg table)         */
                                   sizeof xag__table / sizeof xag__table[0], /* number */
                                   &st);
          if (st != svc_s_ok)
             fprintf(stderr, "Cannot register xag incore message table 0x%lx\n", st);

/* register ftl messages       */
          dce_msg_define_msg_table(ftl__table,  /* The ftutil ftl msg table)           */
                                   sizeof ftl__table / sizeof ftl__table[0], /* number */
                                   &st);
          if (st != svc_s_ok)
             fprintf(stderr, "Cannot register ftl incore message table 0x%lx\n", st);


        }
