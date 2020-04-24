/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: udt_svc.c,v $
 * Revision 1.1.7.1  1996/10/02  21:05:54  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:50:03  damon]
 *
 * Revision 1.1.2.1  1994/08/01  18:02:20  sasala
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/01  15:41:20  sasala]
 * 
 * $EndLog$
 */

/*
 * This module declares the DFS update (udt) handle and
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

#include <dfsudtmac.h>
#include <dfsudtsvc.h>
#include <dfsudtmsg.h>

DCE_SVC_DEFINE_HANDLE(udt_svc_handle,  /* The update handle (to the udt table) */
                      udt_svc_table,   /* The udt table itself */
                      "udt")

void dfs_define_udt_message_tables()
        {
          error_status_t st ;

          /* register udt messages       */
          dce_msg_define_msg_table(udt__table,  /* The update udt  msg table)         */
                                   sizeof udt__table / sizeof udt__table[0], /* number */
                                   &st);
          if (st != svc_s_ok)
             fprintf(stderr, "Cannot register udt incore message table 0x%lx\n", st);

        }
