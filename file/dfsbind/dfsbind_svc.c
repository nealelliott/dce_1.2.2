/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsbind_svc.c,v $
 * Revision 1.1.8.1  1996/10/02  17:14:56  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:06:39  damon]
 *
 * Revision 1.1.2.1  1994/08/01  17:56:21  sasala
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  15:41:18  sasala]
 * 
 * $EndLog$
 */

/*
 * This module declares the DFS dfsbind (dsb) handle and
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

#include <dfsdsbmac.h>
#include <dfsdsbsvc.h>
#include <dfsdsbmsg.h>

DCE_SVC_DEFINE_HANDLE(dsb_svc_handle,  /* The dfsbind handle (to the dsb table) */
                      dsb_svc_table,   /* The dsb table itself */
                      "dsb")

void dfs_define_dsb_message_tables()
        {
          error_status_t st ;

          /* register dsb messages       */
          dce_msg_define_msg_table(dsb__table,  /* The dfsbind dsd  msg table)         */
                                   sizeof dsb__table / sizeof dsb__table[0], /* number */
                                   &st);
          if (st != svc_s_ok)
             fprintf(stderr, "Cannot register dsb incore message table 0x%lx\n", st);

        }
