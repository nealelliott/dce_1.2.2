/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bss_svc.c,v $
 * Revision 1.1.10.1  1996/10/02  17:05:18  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:13  damon]
 *
 * Revision 1.1.2.1  1994/10/20  19:29:57  sasala
 * 	Implement S12Y in bosserver subcomponent -UYHP: 12618
 * 	[1994/10/20  18:15:36  sasala]
 * 
 * $EndLog$
 */

/*
 * This module declares the DFS bosserver (bss) handle and
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

#include <dfsbssmac.h>
#include <dfsbsssvc.h>
#include <dfsbssmsg.h>

DCE_SVC_DEFINE_HANDLE(bss_svc_handle,  /* The bosserver handle (to the bss table) */
                      bss_svc_table,   /* The bss table itself */
                      "bss")

void dfs_define_bss_message_tables()
        {
          error_status_t st ;

          /* register bos messages       */
          dce_msg_define_msg_table(bss__table,  /* The bosserver bss  msg table)         */
                                   sizeof bss__table / sizeof bss__table[0], /* number */
                                   &st);
          if (st != svc_s_ok)
             fprintf(stderr, "Cannot register bss incore message table 0x%lx\n", st);

        }
