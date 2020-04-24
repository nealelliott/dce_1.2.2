/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsd_svc.c,v $
 * Revision 1.1.8.1  1996/10/02  16:56:58  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:24  damon]
 *
 * Revision 1.1.2.1  1994/08/01  17:46:18  sasala
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  15:41:17  sasala]
 * 
 * $EndLog$
 */

/*
 * This module declares the DFS dfsd (dsd) handle and
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

#include <dfsdsdmac.h>
#include <dfsdsdsvc.h>
#include <dfsdsdmsg.h>

DCE_SVC_DEFINE_HANDLE(dsd_svc_handle,  /* The dfsd handle (to the dsd table)     */
                      dsd_svc_table,   /* The dsd table itself */
                      "dsd")

void dfs_define_dsd_message_tables()
        {
          error_status_t st ;

          /* register dsd messages       */
          dce_msg_define_msg_table(dsd__table,  /* The dfsbind dsd  msg table)         */
                                   sizeof dsd__table / sizeof dsd__table[0], /* number */
                                   &st);
          if (st != svc_s_ok)
             fprintf(stderr, "Cannot register dsd incore message table 0x%lx\n", st);

        }
